#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QList>

class MessageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MessageRoles {
        ContentRole = Qt::UserRole + 1,
        IsSentRole,
        TimeRole,
        TypeRole,
        FileNameRole,
        FileSizeRole,
        ProgressRole
    };

    enum MessageType {
        Text = 0,
        File = 1,
        Image = 2
    };
    Q_ENUM(MessageType)

    explicit MessageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void addMessage(const QString &content, bool isSent, int type = Text);
    Q_INVOKABLE void addFileMessage(const QString &fileName, qint64 fileSize, bool isSent, const QString &filePath = QString());
    Q_INVOKABLE void updateProgress(int index, qreal progress);
    Q_INVOKABLE void clear();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    struct MessageData {
        QString content;
        QString fileName;
        QString filePath;
        qint64 fileSize = 0;
        bool isSent = false;
        MessageType type = Text;
        QDateTime time;
        qreal progress = 0.0;
    };
    QList<MessageData> m_messages;
};
