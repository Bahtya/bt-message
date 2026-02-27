#include "messagestore.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QDateTime>
#include <QStandardPaths>

MessageStore::MessageStore(QObject *parent)
    : QObject(parent)
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    m_storagePath = dataPath + "/messages";
    QDir messageDir(m_storagePath);
    if (!messageDir.exists()) {
        messageDir.mkpath(".");
    }
}

QString MessageStore::storagePath() const
{
    return m_storagePath;
}

void MessageStore::setStoragePath(const QString &path)
{
    if (m_storagePath != path) {
        m_storagePath = path;
        emit storagePathChanged();
    }
}

void MessageStore::saveMessage(const QString &deviceAddress, const QString &content, bool isSent, int type)
{
    QJsonObject msgObj = createMessageObject(content, isSent, type);

    QString filePath = getFilePath(deviceAddress);
    QFile file(filePath);

    QJsonArray messages;
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if (doc.isArray()) {
                messages = doc.array();
            }
            file.close();
        }
    }

    messages.append(msgObj);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(messages);
        file.write(doc.toJson(QJsonDocument::Compact));
        file.close();
        emit messageSaved(deviceAddress);
    } else {
        emit error(tr("Failed to save message: ") + file.errorString());
    }
}

void MessageStore::saveFileMessage(const QString &deviceAddress, const QString &fileName, qint64 fileSize, bool isSent)
{
    QJsonObject msgObj;
    msgObj["content"] = fileName;
    msgObj["fileName"] = fileName;
    msgObj["fileSize"] = fileSize;
    msgObj["isSent"] = isSent;
    msgObj["type"] = 1;
    msgObj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QString filePath = getFilePath(deviceAddress);
    QFile file(filePath);

    QJsonArray messages;
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if (doc.isArray()) {
                messages = doc.array();
            }
            file.close();
        }
    }

    messages.append(msgObj);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(messages);
        file.write(doc.toJson(QJsonDocument::Compact));
        file.close();
        emit messageSaved(deviceAddress);
    } else {
        emit error(tr("Failed to save file message: ") + file.errorString());
    }
}

QJsonArray MessageStore::loadMessages(const QString &deviceAddress)
{
    QString filePath = getFilePath(deviceAddress);
    QFile file(filePath);

    if (!file.exists()) {
        return QJsonArray();
    }

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        if (doc.isArray()) {
            return doc.array();
        }
    }

    return QJsonArray();
}

void MessageStore::clearMessages(const QString &deviceAddress)
{
    QString filePath = getFilePath(deviceAddress);
    QFile file(filePath);
    if (file.exists()) {
        file.remove();
    }
}

void MessageStore::clearAllMessages()
{
    QDir dir(m_storagePath);
    QStringList files = dir.entryList(QDir::Files);
    for (const QString &file : files) {
        dir.remove(file);
    }
}

QString MessageStore::getFilePath(const QString &deviceAddress) const
{
    QString safeAddress = deviceAddress;
    safeAddress.replace(":", "_");
    return m_storagePath + "/" + safeAddress + ".json";
}

QJsonObject MessageStore::createMessageObject(const QString &content, bool isSent, int type) const
{
    QJsonObject obj;
    obj["content"] = content;
    obj["isSent"] = isSent;
    obj["type"] = type;
    obj["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return obj;
}
