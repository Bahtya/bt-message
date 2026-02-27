#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_messages.count();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_messages.count())
        return QVariant();

    const MessageData &msg = m_messages[index.row()];

    switch (role) {
    case ContentRole:
        return msg.content;
    case IsSentRole:
        return msg.isSent;
    case TimeRole:
        return msg.time.toString("hh:mm");
    case TypeRole:
        return msg.type;
    case FileNameRole:
        return msg.fileName;
    case FileSizeRole:
        return formatFileSize(msg.fileSize);
    case ProgressRole:
        return msg.progress;
    default:
        return QVariant();
    }
}

void MessageModel::addMessage(const QString &content, bool isSent, int type)
{
    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    MessageData msg;
    msg.content = content;
    msg.isSent = isSent;
    msg.type = static_cast<MessageType>(type);
    msg.time = QDateTime::currentDateTime();
    m_messages.append(msg);
    endInsertRows();
}

void MessageModel::addFileMessage(const QString &fileName, qint64 fileSize, bool isSent, const QString &filePath)
{
    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    MessageData msg;
    msg.fileName = fileName;
    msg.fileSize = fileSize;
    msg.filePath = filePath;
    msg.isSent = isSent;
    msg.type = File;
    msg.time = QDateTime::currentDateTime();
    m_messages.append(msg);
    endInsertRows();
}

void MessageModel::updateProgress(int index, qreal progress)
{
    if (index < 0 || index >= m_messages.count())
        return;

    m_messages[index].progress = progress;
    emit dataChanged(createIndex(index, 0), createIndex(index, 0), {ProgressRole});
}

void MessageModel::clear()
{
    if (m_messages.isEmpty())
        return;

    beginResetModel();
    m_messages.clear();
    endResetModel();
}

QString MessageModel::formatFileSize(qint64 bytes) const
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

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ContentRole] = "content";
    roles[IsSentRole] = "isSent";
    roles[TimeRole] = "timeText";
    roles[TypeRole] = "messageType";
    roles[FileNameRole] = "fileName";
    roles[FileSizeRole] = "fileSizeText";
    roles[ProgressRole] = "progress";
    return roles;
}
