#pragma once

#include <QAbstractListModel>
#include <QList>

class FileTransferModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum TransferRoles {
        FileNameRole = Qt::UserRole + 1,
        FilePathRole,
        FileSizeRole,
        TransferredRole,
        ProgressRole,
        IsSendingRole,
        StatusRole
    };

    enum TransferStatus {
        Pending = 0,
        InProgress = 1,
        Completed = 2,
        Failed = 3,
        Cancelled = 4
    };
    Q_ENUM(TransferStatus)

    explicit FileTransferModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE int addTransfer(const QString &fileName, const QString &filePath, qint64 fileSize, bool isSending);
    Q_INVOKABLE void updateProgress(int index, qint64 transferred);
    Q_INVOKABLE void setCompleted(int index);
    Q_INVOKABLE void setFailed(int index, const QString &error);
    Q_INVOKABLE void cancelTransfer(int index);
    Q_INVOKABLE void clearCompleted();

signals:
    void transferCompleted(int index, const QString &filePath);
    void transferFailed(int index, const QString &error);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QString formatFileSize(qint64 bytes) const;

    struct TransferData {
        QString fileName;
        QString filePath;
        qint64 fileSize = 0;
        qint64 transferred = 0;
        bool isSending = false;
        TransferStatus status = Pending;
    };
    QList<TransferData> m_transfers;
};
