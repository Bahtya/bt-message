#include "devicemodel.h"

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_devices.count();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_devices.count())
        return QVariant();

    const DeviceData &device = m_devices[index.row()];

    switch (role) {
    case NameRole:
        return device.name;
    case AddressRole:
        return device.address;
    case ConnectedRole:
        return device.connected;
    default:
        return QVariant();
    }
}

void DeviceModel::addDevice(const QString &name, const QString &address)
{
    for (const auto &device : m_devices) {
        if (device.address == address)
            return;
    }

    beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
    DeviceData device;
    device.name = name.isEmpty() ? tr("Unknown Device") : name;
    device.address = address;
    m_devices.append(device);
    endInsertRows();
}

void DeviceModel::removeDevice(int index)
{
    if (index < 0 || index >= m_devices.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_devices.removeAt(index);
    endRemoveRows();
}

void DeviceModel::clear()
{
    if (m_devices.isEmpty())
        return;

    beginResetModel();
    m_devices.clear();
    endResetModel();
}

void DeviceModel::setConnected(int index, bool connected)
{
    if (index < 0 || index >= m_devices.count())
        return;

    m_devices[index].connected = connected;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {ConnectedRole});
}

QHash<int, QByteArray> DeviceModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "deviceName";
    roles[AddressRole] = "deviceAddress";
    roles[ConnectedRole] = "isConnected";
    return roles;
}
