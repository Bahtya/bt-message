#include "filetransfermodel.h"

FileTransferModel::FileTransferModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int FileTransferModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_transfers.count();
}

QVariant FileTransferModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_transfers.count())
        return QVariant();

    const TransferData &transfer = m_transfers[index.row()];

    switch (role) {
    case FileNameRole:
        return transfer.fileName;
    case FilePathRole:
        return transfer.filePath;
    case FileSizeRole:
        return formatFileSize(transfer.fileSize);
    case TransferredRole:
        return transfer.transferred;
    case ProgressRole:
        return transfer.fileSize > 0 ? qreal(transfer.transferred) / transfer.fileSize : 0.0;
    case IsSendingRole:
        return transfer.isSending;
    case StatusRole:
        return transfer.status;
    default:
        return QVariant();
    }
}

int FileTransferModel::addTransfer(const QString &fileName, const QString &filePath, qint64 fileSize, bool isSending)
{
    beginInsertRows(QModelIndex(), m_transfers.count(), m_transfers.count());
    TransferData transfer;
    transfer.fileName = fileName;
    transfer.filePath = filePath;
    transfer.fileSize = fileSize;
    transfer.isSending = isSending;
    transfer.status = Pending;
    m_transfers.append(transfer);
    endInsertRows();
    return m_transfers.count() - 1;
}

void FileTransferModel::updateProgress(int index, qint64 transferred)
{
    if (index < 0 || index >= m_transfers.count())
        return;

    m_transfers[index].transferred = transferred;
    m_transfers[index].status = InProgress;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {TransferredRole, ProgressRole, StatusRole});
}

void FileTransferModel::setCompleted(int index)
{
    if (index < 0 || index >= m_transfers.count())
        return;

    m_transfers[index].status = Completed;
    m_transfers[index].transferred = m_transfers[index].fileSize;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {StatusRole, TransferredRole, ProgressRole});
    emit transferCompleted(index, m_transfers[index].filePath);
}

void FileTransferModel::setFailed(int index, const QString &error)
{
    if (index < 0 || index >= m_transfers.count())
        return;

    m_transfers[index].status = Failed;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {StatusRole});
    emit transferFailed(index, error);
}

void FileTransferModel::cancelTransfer(int index)
{
    if (index < 0 || index >= m_transfers.count())
        return;

    m_transfers[index].status = Cancelled;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {StatusRole});
}

void FileTransferModel::clearCompleted()
{
    for (int i = m_transfers.count() - 1; i >= 0; --i) {
        if (m_transfers[i].status == Completed || m_transfers[i].status == Failed || m_transfers[i].status == Cancelled) {
            beginRemoveRows(QModelIndex(), i, i);
            m_transfers.removeAt(i);
            endRemoveRows();
        }
    }
}

QString FileTransferModel::formatFileSize(qint64 bytes) const
{
    if (bytes < 1024)
        return QString("%1 B").arg(bytes);
    else if (bytes < 1024 * 1024)
        return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    else if (bytes < 1024 * 1024 * 1024)
        return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    else
        return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
}

QHash<int, QByteArray> FileTransferModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";
    roles[FileSizeRole] = "fileSizeText";
    roles[TransferredRole] = "transferred";
    roles[ProgressRole] = "progress";
    roles[IsSendingRole] = "isSending";
    roles[StatusRole] = "status";
    return roles;
}
