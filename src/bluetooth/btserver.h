#pragma once

#include <QObject>
#include <QBluetoothServer>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>

class BtServer : public QObject
{
    Q_OBJECT

public:
    explicit BtServer(QObject *parent = nullptr);
    ~BtServer();

    bool isRunning() const;
    bool hasConnectedClients() const;

public slots:
    void startServer(const QString &serviceUuid);
    void stopServer();
    void sendData(const QByteArray &data);

signals:
    void clientConnected(const QString &address);
    void clientDisconnected(const QString &address);
    void dataReceived(const QByteArray &data);
    void error(const QString &errorMessage);

private slots:
    void onNewConnection();
    void onClientDisconnected();
    void onReadyRead();

private:
    QBluetoothServer *m_server = nullptr;
    QBluetoothServiceInfo m_serviceInfo;
    QList<QBluetoothSocket *> m_clients;
    QBluetoothSocket *m_currentSocket = nullptr;
};
