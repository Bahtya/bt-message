#pragma once

#include <QObject>
#include <QBluetoothSocket>
#include <QBluetoothDeviceInfo>

class BtClient : public QObject
{
    Q_OBJECT

public:
    explicit BtClient(QObject *parent = nullptr);
    ~BtClient();

    bool isConnected() const;

public slots:
    void connectToDevice(const QBluetoothDeviceInfo &device, const QString &serviceUuid);
    void disconnect();
    void sendData(const QByteArray &data);

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray &data);
    void error(const QString &errorMessage);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QBluetoothSocket::SocketError error);

private:
    QBluetoothSocket *m_socket = nullptr;
};
