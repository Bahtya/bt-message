#pragma once

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QQueue>

class FileTransferManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isTransferring READ isTransferring NOTIFY isTransferringChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString currentFileName READ currentFileName NOTIFY currentFileNameChanged)

public:
    explicit FileTransferManager(QObject *parent = nullptr);

    bool isTransferring() const;
    qreal progress() const;
    QString currentFileName() const;

public slots:
    void sendFile(const QString &filePath);
    void cancelTransfer();
    void handleReceivedData(const QByteArray &data);
    void setSaveDirectory(const QString &directory);

signals:
    void isTransferringChanged();
    void progressChanged();
    void currentFileNameChanged();
    void fileSendProgress(const QString &fileName, qreal progress);
    void fileSendComplete(const QString &fileName);
    void fileReceiveProgress(const QString &fileName, qreal progress);
    void fileReceiveComplete(const QString &fileName, const QString &savePath);
    void transferError(const QString &error);
    void sendData(const QByteArray &data);

private slots:
    void sendNextChunk();

private:
    struct TransferInfo {
        QString fileName;
        QString filePath;
        qint64 fileSize = 0;
        qint64 transferred = 0;
        int chunkIndex = 0;
        bool isSending = false;
    };

    void startReceiving(const QString &fileName, qint64 fileSize);
    void receiveChunk(const QByteArray &chunk, int chunkIndex);
    void completeReceiving();

    QFile *m_currentFile = nullptr;
    QTimer *m_sendTimer = nullptr;
    TransferInfo m_currentTransfer;
    QQueue<TransferInfo> m_transferQueue;
    QString m_saveDirectory;
    bool m_isTransferring = false;
    qreal m_progress = 0.0;

    static constexpr int CHUNK_SIZE = 4096;
};
