#include "btclient.h"

BtClient::BtClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this))
{
    connect(m_socket, &QBluetoothSocket::connected,
            this, &BtClient::onConnected);
    connect(m_socket, &QBluetoothSocket::disconnected,
            this, &BtClient::onDisconnected);
    connect(m_socket, &QBluetoothSocket::readyRead,
            this, &BtClient::onReadyRead);
    connect(m_socket, &QBluetoothSocket::errorOccurred,
            this, &BtClient::onError);
}

BtClient::~BtClient()
{
    disconnect();
}

bool BtClient::isConnected() const
{
    return m_socket->state() == QBluetoothSocket::ConnectedState;
}

void BtClient::connectToDevice(const QBluetoothDeviceInfo &device, const QString &serviceUuid)
{
    if (isConnected())
        disconnect();

    m_socket->connectToService(device, QBluetoothUuid(serviceUuid));
}

void BtClient::disconnect()
{
    if (m_socket)
        m_socket->disconnectFromService();
}

void BtClient::sendData(const QByteArray &data)
{
    if (isConnected())
        m_socket->write(data);
}

void BtClient::onConnected()
{
    emit connected();
}

void BtClient::onDisconnected()
{
    emit disconnected();
}

void BtClient::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    emit dataReceived(data);
}

void BtClient::onError(QBluetoothSocket::SocketError error)
{
    Q_UNUSED(error)
    emit this->error(m_socket->errorString());
}
