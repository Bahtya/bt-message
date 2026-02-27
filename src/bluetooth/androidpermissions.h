#pragma once

#include <QObject>
#include <QString>

class AndroidPermissions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool bluetoothGranted READ bluetoothGranted NOTIFY bluetoothGrantedChanged)
    Q_PROPERTY(bool locationGranted READ locationGranted NOTIFY locationGrantedChanged)
    Q_PROPERTY(bool storageGranted READ storageGranted NOTIFY storageGrantedChanged)

public:
    explicit AndroidPermissions(QObject *parent = nullptr);

    bool bluetoothGranted() const;
    bool locationGranted() const;
    bool storageGranted() const;

public slots:
    void requestBluetoothPermissions();
    void requestLocationPermission();
    void requestStoragePermissions();
    void requestAllPermissions();

signals:
    void bluetoothGrantedChanged();
    void locationGrantedChanged();
    void storageGrantedChanged();
    void allPermissionsGranted();

private:
    bool m_bluetoothGranted = false;
    bool m_locationGranted = false;
    bool m_storageGranted = false;

    void checkPermissions();
};
