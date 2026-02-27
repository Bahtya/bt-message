#pragma once

#include <QObject>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothServer>
#include <QList>

#include "btdevice.h"
#include "btserver.h"
#include "btclient.h"

class BtManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localDeviceName READ localDeviceName NOTIFY localDeviceNameChanged)
    Q_PROPERTY(QString localDeviceAddress READ localDeviceAddress NOTIFY localDeviceAddressChanged)
    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)
    Q_PROPERTY(bool isServerRunning READ isServerRunning NOTIFY isServerRunningChanged)
    Q_PROPERTY(ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)

public:
    enum ConnectionState {
        Disconnected = 0,
        Connecting = 1,
        Connected = 2,
        Disconnecting = 3
    };
    Q_ENUM(ConnectionState)

    explicit BtManager(QObject *parent = nullptr);
    ~BtManager();

    QString localDeviceName() const;
    QString localDeviceAddress() const;
    bool isScanning() const;
    bool isServerRunning() const;
    ConnectionState connectionState() const;

public slots:
    void startDiscovery();
    void stopDiscovery();
    void startServer();
    void stopServer();
    void connectToDevice(const QString &address);
    void disconnect();
    void sendMessage(const QString &message);
    void sendFile(const QString &filePath);

signals:
    void localDeviceNameChanged();
    void localDeviceAddressChanged();
    void isScanningChanged();
    void isServerRunningChanged();
    void connectionStateChanged();
    void deviceDiscovered(const QString &name, const QString &address);
    void discoveryFinished();
    void messageReceived(const QString &message, const QString &fromAddress);
    void fileTransferProgress(const QString &fileName, qreal progress);
    void fileTransferComplete(const QString &fileName);
    void error(const QString &errorMessage);

private slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onDiscoveryFinished();
    void onClientConnected(const QString &address);
    void onClientDisconnected();
    void onDataReceived(const QByteArray &data);

private:
    QBluetoothLocalDevice *m_localDevice = nullptr;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent = nullptr;
    BtServer *m_server = nullptr;
    BtClient *m_client = nullptr;
    bool m_isScanning = false;
    ConnectionState m_connectionState = Disconnected;
};
