#include "btserver.h"

BtServer::BtServer(QObject *parent)
    : QObject(parent)
    , m_server(new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this))
{
    connect(m_server, &QBluetoothServer::newConnection,
            this, &BtServer::onNewConnection);
}

BtServer::~BtServer()
{
    stopServer();
}

bool BtServer::isRunning() const
{
    return m_server->isListening();
}

bool BtServer::hasConnectedClients() const
{
    return !m_clients.isEmpty();
}

void BtServer::startServer(const QString &serviceUuid)
{
    if (m_server->isListening())
        return;

    QBluetoothAddress localAddress;
    QList<QBluetoothHostInfo> localAdapters = QBluetoothLocalDevice::allDevices();
    if (!localAdapters.isEmpty())
        localAddress = localAdapters.first().address();
    else
        return;

    if (!m_server->listen(localAddress)) {
        emit error(tr("Cannot bind server"));
        return;
    }

    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothClassIds, classId);

    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("BT Message"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                               tr("Bluetooth chat and file transfer"));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("Bahtya"));

    m_serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence record;
    record << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
           << QVariant::fromValue(quint8(m_server->serverPort()));
    profileSequence.append(QVariant::fromValue(record));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                               profileSequence);

    QBluetoothServiceInfo::Sequence protocol;
    QBluetoothServiceInfo::Sequence protocolList;
    QBluetoothServiceInfo::Sequence rfcomm;
    rfcomm << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
           << QVariant::fromValue(quint8(m_server->serverPort()));
    protocolList.append(QVariant::fromValue(rfcomm));
    protocol.append(QVariant::fromValue(protocolList));
    m_serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocol);

    m_serviceInfo.registerService(localAddress);
}

void BtServer::stopServer()
{
    for (QBluetoothSocket *socket : m_clients) {
        socket->disconnectFromService();
        socket->deleteLater();
    }
    m_clients.clear();

    m_serviceInfo.unregisterService();
    m_server->close();
}

void BtServer::sendData(const QByteArray &data)
{
    for (QBluetoothSocket *socket : m_clients) {
        socket->write(data);
    }
}

void BtServer::onNewConnection()
{
    QBluetoothSocket *socket = m_server->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, &QBluetoothSocket::disconnected,
            this, &BtServer::onClientDisconnected);
    connect(socket, &QBluetoothSocket::readyRead,
            this, &BtServer::onReadyRead);

    m_clients.append(socket);
    m_currentSocket = socket;
    emit clientConnected(socket->peerAddress().toString());
}

void BtServer::onClientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerAddress().toString());
    m_clients.removeOne(socket);
    socket->deleteLater();

    if (m_currentSocket == socket)
        m_currentSocket = m_clients.isEmpty() ? nullptr : m_clients.first();
}

void BtServer::onReadyRead()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    QByteArray data = socket->readAll();
    emit dataReceived(data);
}
