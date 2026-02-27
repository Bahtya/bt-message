#pragma once

#include <QObject>
#include <QByteArray>
#include <QString>

class MessageProtocol : public QObject
{
    Q_OBJECT

public:
    enum MessageType {
        Text = 0x01,
        FileHeader = 0x02,
        FileChunk = 0x03,
        FileEnd = 0x04,
        Heartbeat = 0x05
    };
    Q_ENUM(MessageType)

    struct Packet {
        MessageType type;
        QByteArray data;
    };

    explicit MessageProtocol(QObject *parent = nullptr);

    static QByteArray encodeText(const QString &text);
    static QByteArray encodeFileHeader(const QString &fileName, qint64 fileSize);
    static QByteArray encodeFileChunk(const QByteArray &chunk, int chunkIndex);
    static QByteArray encodeFileEnd();
    static QByteArray encodeHeartbeat();

    static Packet decode(const QByteArray &data);

    static constexpr int CHUNK_SIZE = 4096;
    static constexpr int HEADER_SIZE = 7;
    static constexpr int MAX_PACKET_SIZE = 4103;

private:
    static QByteArray createPacket(MessageType type, const QByteArray &payload);
    static quint16 calculateChecksum(const QByteArray &data);
};
