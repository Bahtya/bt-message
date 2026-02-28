#include "androidpermissions.h"

#ifdef Q_OS_ANDROID
#include <QJniObject>
#include <QCoreApplication>
#include <QNativeInterface>
#endif

AndroidPermissions::AndroidPermissions(QObject *parent)
    : QObject(parent)
{
    checkPermissions();
}

bool AndroidPermissions::bluetoothGranted() const
{
    return m_bluetoothGranted;
}

bool AndroidPermissions::locationGranted() const
{
    return m_locationGranted;
}

bool AndroidPermissions::storageGranted() const
{
    return m_storageGranted;
}

void AndroidPermissions::requestBluetoothPermissions()
{
#ifdef Q_OS_ANDROID
    auto activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    if (activity.isValid()) {
        activity.callMethod<void>("requestBluetoothPermissions");
    }
#else
    m_bluetoothGranted = true;
    emit bluetoothGrantedChanged();
#endif
}

void AndroidPermissions::requestLocationPermission()
{
#ifdef Q_OS_ANDROID
    auto activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    if (activity.isValid()) {
        activity.callMethod<void>("requestLocationPermission");
    }
#else
    m_locationGranted = true;
    emit locationGrantedChanged();
#endif
}

void AndroidPermissions::requestStoragePermissions()
{
#ifdef Q_OS_ANDROID
    auto activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    if (activity.isValid()) {
        activity.callMethod<void>("requestStoragePermissions");
    }
#else
    m_storageGranted = true;
    emit storageGrantedChanged();
#endif
}

void AndroidPermissions::requestAllPermissions()
{
#ifdef Q_OS_ANDROID
    requestBluetoothPermissions();
    requestLocationPermission();
    requestStoragePermissions();
#else
    m_bluetoothGranted = true;
    m_locationGranted = true;
    m_storageGranted = true;
    emit bluetoothGrantedChanged();
    emit locationGrantedChanged();
    emit storageGrantedChanged();
    emit allPermissionsGranted();
#endif
}

void AndroidPermissions::checkPermissions()
{
#ifdef Q_OS_ANDROID
    auto activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    if (activity.isValid()) {
        m_bluetoothGranted = activity.callMethod<jboolean>("hasBluetoothPermissions");
        m_locationGranted = activity.callMethod<jboolean>("hasLocationPermission");
        m_storageGranted = activity.callMethod<jboolean>("hasStoragePermissions");
    }
#else
    m_bluetoothGranted = true;
    m_locationGranted = true;
    m_storageGranted = true;
#endif
}
