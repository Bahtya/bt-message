#include "filetransfermanager.h"
#include "messageprotocol.h"
#include <QDir>
#include <QStandardPaths>
#include <QDataStream>

FileTransferManager::FileTransferManager(QObject *parent)
    : QObject(parent)
    , m_sendTimer(new QTimer(this))
{
    m_saveDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    connect(m_sendTimer, &QTimer::timeout, this, &FileTransferManager::sendNextChunk);
}

bool FileTransferManager::isTransferring() const
{
    return m_isTransferring;
}

qreal FileTransferManager::progress() const
{
    return m_progress;
}

QString FileTransferManager::currentFileName() const
{
    return m_currentTransfer.fileName;
}

void FileTransferManager::sendFile(const QString &filePath)
{
    QFile *file = new QFile(filePath);
    if (!file->exists()) {
        emit transferError(tr("File not found: ") + filePath);
        file->deleteLater();
        return;
    }

    QFileInfo fileInfo(filePath);
    TransferInfo info;
    info.fileName = fileInfo.fileName();
    info.filePath = filePath;
    info.fileSize = fileInfo.size();
    info.transferred = 0;
    info.chunkIndex = 0;
    info.isSending = true;

    m_transferQueue.enqueue(info);
    
    if (!m_isTransferring) {
        m_currentTransfer = m_transferQueue.dequeue();
        m_currentFile = file;
        
        if (m_currentFile->open(QIODevice::ReadOnly)) {
            m_isTransferring = true;
            emit isTransferringChanged();
            emit currentFileNameChanged();
            
            QByteArray header = MessageProtocol::encodeFileHeader(
                m_currentTransfer.fileName, 
                m_currentTransfer.fileSize
            );
            emit sendData(header);
            
            m_sendTimer->start(10);
        } else {
            emit transferError(tr("Cannot open file: ") + file->errorString());
            file->deleteLater();
        }
    }
}

void FileTransferManager::cancelTransfer()
{
    m_sendTimer->stop();
    
    if (m_currentFile) {
        m_currentFile->close();
        m_currentFile->deleteLater();
        m_currentFile = nullptr;
    }
    
    m_isTransferring = false;
    m_progress = 0.0;
    m_transferQueue.clear();
    
    emit isTransferringChanged();
    emit progressChanged();
}

void FileTransferManager::handleReceivedData(const QByteArray &data)
{
    MessageProtocol::Packet packet = MessageProtocol::decode(data);
    
    switch (packet.type) {
    case MessageProtocol::FileHeader: {
        QDataStream stream(packet.data);
        QString fileName;
        qint64 fileSize;
        stream >> fileName >> fileSize;
        startReceiving(fileName, fileSize);
        break;
    }
    case MessageProtocol::FileChunk: {
        if (m_currentFile && m_currentFile->isOpen()) {
            QDataStream stream(packet.data);
            quint32 chunkIndex;
            stream >> chunkIndex;
            
            QByteArray chunk = packet.data.mid(4);
            receiveChunk(chunk, chunkIndex);
        }
        break;
    }
    case MessageProtocol::FileEnd:
        completeReceiving();
        break;
    default:
        break;
    }
}

void FileTransferManager::setSaveDirectory(const QString &directory)
{
    m_saveDirectory = directory;
}

void FileTransferManager::sendNextChunk()
{
    if (!m_currentFile || !m_currentFile->isOpen()) {
        m_sendTimer->stop();
        return;
    }

    QByteArray chunk = m_currentFile->read(CHUNK_SIZE);
    
    if (chunk.isEmpty()) {
        m_sendTimer->stop();
        m_currentFile->close();
        m_currentFile->deleteLater();
        m_currentFile = nullptr;
        
        QByteArray endPacket = MessageProtocol::encodeFileEnd();
        emit sendData(endPacket);
        
        emit fileSendComplete(m_currentTransfer.fileName);
        
        if (!m_transferQueue.isEmpty()) {
            m_currentTransfer = m_transferQueue.dequeue();
            sendFile(m_currentTransfer.filePath);
        } else {
            m_isTransferring = false;
            m_progress = 0.0;
            emit isTransferringChanged();
            emit progressChanged();
        }
        return;
    }

    QByteArray chunkPacket = MessageProtocol::encodeFileChunk(chunk, m_currentTransfer.chunkIndex);
    emit sendData(chunkPacket);
    
    m_currentTransfer.transferred += chunk.size();
    m_currentTransfer.chunkIndex++;
    
    m_progress = qreal(m_currentTransfer.transferred) / m_currentTransfer.fileSize;
    emit progressChanged();
    emit fileSendProgress(m_currentTransfer.fileName, m_progress);
}

void FileTransferManager::startReceiving(const QString &fileName, qint64 fileSize)
{
    QString savePath = m_saveDirectory + "/" + fileName;
    
    int counter = 1;
    QString baseName = fileName;
    QString extension;
    int dotPos = fileName.lastIndexOf('.');
    if (dotPos > 0) {
        baseName = fileName.left(dotPos);
        extension = fileName.mid(dotPos);
    }
    
    while (QFile::exists(savePath)) {
        savePath = m_saveDirectory + "/" + baseName + QString(" (%1)").arg(counter) + extension;
        counter++;
    }
    
    m_currentFile = new QFile(savePath);
    if (m_currentFile->open(QIODevice::WriteOnly)) {
        m_currentTransfer.fileName = fileName;
        m_currentTransfer.filePath = savePath;
        m_currentTransfer.fileSize = fileSize;
        m_currentTransfer.transferred = 0;
        m_currentTransfer.isSending = false;
        
        m_isTransferring = true;
        m_progress = 0.0;
        
        emit isTransferringChanged();
        emit progressChanged();
        emit currentFileNameChanged();
    } else {
        emit transferError(tr("Cannot create file: ") + m_currentFile->errorString());
        m_currentFile->deleteLater();
        m_currentFile = nullptr;
    }
}

void FileTransferManager::receiveChunk(const QByteArray &chunk, int chunkIndex)
{
    Q_UNUSED(chunkIndex)
    
    if (m_currentFile) {
        m_currentFile->write(chunk);
        m_currentTransfer.transferred += chunk.size();
        
        m_progress = qreal(m_currentTransfer.transferred) / m_currentTransfer.fileSize;
        emit progressChanged();
        emit fileReceiveProgress(m_currentTransfer.fileName, m_progress);
    }
}

void FileTransferManager::completeReceiving()
{
    if (m_currentFile) {
        m_currentFile->close();
        m_currentFile->deleteLater();
        m_currentFile = nullptr;
        
        emit fileReceiveComplete(m_currentTransfer.fileName, m_currentTransfer.filePath);
    }
    
    m_isTransferring = false;
    m_progress = 0.0;
    
    emit isTransferringChanged();
    emit progressChanged();
}
