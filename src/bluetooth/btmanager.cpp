#include "btmanager.h"

static const QString SERVICE_UUID = "e8e10f95-1a70-4b27-9ccf-02010264e9c8";

BtManager::BtManager(QObject *parent)
    : QObject(parent)
    , m_localDevice(new QBluetoothLocalDevice(this))
    , m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this))
    , m_server(new BtServer(this))
    , m_client(new BtClient(this))
    , m_reconnectTimer(new QTimer(this))
{
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BtManager::onDeviceDiscovered);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BtManager::onDiscoveryFinished);
    
    connect(m_server, &BtServer::clientConnected,
            this, &BtManager::onClientConnected);
    connect(m_server, &BtServer::dataReceived,
            this, &BtManager::onDataReceived);
    
    connect(m_client, &BtClient::connected,
            this, [this]() {
                m_reconnectAttempts = 0;
                m_connectionState = Connected;
                emit connectionStateChanged();
            });
    connect(m_client, &BtClient::disconnected,
            this, &BtManager::onClientDisconnected);
    connect(m_client, &BtClient::dataReceived,
            this, &BtManager::onDataReceived);
    
    connect(m_reconnectTimer, &QTimer::timeout,
            this, &BtManager::onReconnectTimer);
    m_reconnectTimer->setSingleShot(true);
}

BtManager::~BtManager()
{
    m_autoReconnect = false;
    stopServer();
    disconnect();
}

QString BtManager::localDeviceName() const
{
    return m_localDevice ? m_localDevice->name() : QString();
}

QString BtManager::localDeviceAddress() const
{
    return m_localDevice ? m_localDevice->address().toString() : QString();
}

bool BtManager::isScanning() const
{
    return m_isScanning;
}

bool BtManager::isServerRunning() const
{
    return m_server ? m_server->isRunning() : false;
}

BtManager::ConnectionState BtManager::connectionState() const
{
    return m_connectionState;
}

bool BtManager::autoReconnect() const
{
    return m_autoReconnect;
}

void BtManager::setAutoReconnect(bool enabled)
{
    if (m_autoReconnect != enabled) {
        m_autoReconnect = enabled;
        emit autoReconnectChanged();
    }
}

void BtManager::startDiscovery()
{
    if (m_isScanning) return;
    
    m_isScanning = true;
    emit isScanningChanged();
    m_discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
}

void BtManager::stopDiscovery()
{
    if (!m_isScanning) return;
    
    m_discoveryAgent->stop();
    m_isScanning = false;
    emit isScanningChanged();
}

void BtManager::startServer()
{
    m_server->startServer(SERVICE_UUID);
    emit isServerRunningChanged();
}

void BtManager::stopServer()
{
    m_server->stopServer();
    emit isServerRunningChanged();
}

void BtManager::connectToDevice(const QString &address)
{
    m_lastConnectedAddress = address;
    m_reconnectAttempts = 0;
    m_connectionState = Connecting;
    emit connectionStateChanged();
    
    QBluetoothDeviceInfo device;
    const QList<QBluetoothDeviceInfo> devices = m_discoveryAgent->discoveredDevices();
    for (const auto &d : devices) {
        if (d.address().toString() == address) {
            device = d;
            break;
        }
    }
    
    if (device.isValid()) {
        m_client->connectToDevice(device, SERVICE_UUID);
    } else {
        m_connectionState = Disconnected;
        emit connectionStateChanged();
        emit error(tr("Device not found"));
    }
}

void BtManager::disconnect()
{
    m_autoReconnect = false;
    m_reconnectTimer->stop();
    m_connectionState = Disconnecting;
    emit connectionStateChanged();
    m_client->disconnect();
}

void BtManager::sendMessage(const QString &message)
{
    QByteArray data = message.toUtf8();
    if (m_client->isConnected()) {
        m_client->sendData(data);
    } else if (m_server->hasConnectedClients()) {
        m_server->sendData(data);
    }
}

void BtManager::sendFile(const QString &filePath)
{
    Q_UNUSED(filePath)
}

void BtManager::onDeviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if (device.isValid()) {
        emit deviceDiscovered(device.name(), device.address().toString());
    }
}

void BtManager::onDiscoveryFinished()
{
    m_isScanning = false;
    emit isScanningChanged();
    emit discoveryFinished();
}

void BtManager::onClientConnected(const QString &address)
{
    Q_UNUSED(address)
    m_connectionState = Connected;
    emit connectionStateChanged();
}

void BtManager::onClientDisconnected()
{
    if (m_autoReconnect && !m_lastConnectedAddress.isEmpty() && m_reconnectAttempts < m_maxReconnectAttempts) {
        m_connectionState = Reconnecting;
        emit connectionStateChanged();
        attemptReconnect();
    } else {
        m_connectionState = Disconnected;
        emit connectionStateChanged();
        if (m_reconnectAttempts >= m_maxReconnectAttempts) {
            emit error(tr("Reconnect failed after %1 attempts").arg(m_maxReconnectAttempts));
        }
    }
}

void BtManager::onDataReceived(const QByteArray &data)
{
    QString message = QString::fromUtf8(data);
    emit messageReceived(message, QString());
}

void BtManager::onReconnectTimer()
{
    if (m_connectionState != Reconnecting) return;
    
    QBluetoothDeviceInfo device;
    const QList<QBluetoothDeviceInfo> devices = m_discoveryAgent->discoveredDevices();
    for (const auto &d : devices) {
        if (d.address().toString() == m_lastConnectedAddress) {
            device = d;
            break;
        }
    }
    
    if (device.isValid()) {
        m_client->connectToDevice(device, SERVICE_UUID);
    } else {
        startDiscovery();
    }
}

void BtManager::attemptReconnect()
{
    m_reconnectAttempts++;
    int delay = qMin(1000 * m_reconnectAttempts, 5000);
    m_reconnectTimer->start(delay);
}
