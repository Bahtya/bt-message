#pragma once

#include <QObject>
#include <QBluetoothAddress>

class BtDevice
{
    Q_GADGET
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString address READ address CONSTANT)

public:
    BtDevice() = default;
    BtDevice(const QString &name, const QString &address);

    QString name() const { return m_name; }
    QString address() const { return m_address; }

    bool isValid() const { return !m_address.isEmpty(); }

private:
    QString m_name;
    QString m_address;
};
