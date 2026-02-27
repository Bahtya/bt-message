#pragma once

#include <QAbstractListModel>
#include <QList>
#include "bluetooth/btdevice.h"

class DeviceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DeviceRoles {
        NameRole = Qt::UserRole + 1,
        AddressRole,
        ConnectedRole
    };

    explicit DeviceModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void addDevice(const QString &name, const QString &address);
    Q_INVOKABLE void removeDevice(int index);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void setConnected(int index, bool connected);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    struct DeviceData {
        QString name;
        QString address;
        bool connected = false;
    };
    QList<DeviceData> m_devices;
};
