#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class MessageStore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString storagePath READ storagePath WRITE setStoragePath NOTIFY storagePathChanged)

public:
    explicit MessageStore(QObject *parent = nullptr);

    QString storagePath() const;
    void setStoragePath(const QString &path);

public slots:
    void saveMessage(const QString &deviceAddress, const QString &content, bool isSent, int type);
    void saveFileMessage(const QString &deviceAddress, const QString &fileName, qint64 fileSize, bool isSent);
    QJsonArray loadMessages(const QString &deviceAddress);
    void clearMessages(const QString &deviceAddress);
    void clearAllMessages();

signals:
    void storagePathChanged();
    void messageSaved(const QString &deviceAddress);
    void error(const QString &errorMessage);

private:
    QString m_storagePath;
    QString getFilePath(const QString &deviceAddress) const;
    QJsonObject createMessageObject(const QString &content, bool isSent, int type) const;
};
