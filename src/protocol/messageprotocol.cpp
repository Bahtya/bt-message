#include "messageprotocol.h"
#include <QDataStream>
#include <QIODevice>

MessageProtocol::MessageProtocol(QObject *parent)
    : QObject(parent)
{
}

QByteArray MessageProtocol::createPacket(MessageType type, const QByteArray &payload)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << quint16(0xAA55);
    stream << quint8(type);
    stream << quint32(payload.size());
    stream.writeRawData(payload.constData(), payload.size());

    quint16 checksum = calculateChecksum(payload);
    stream << checksum;

    return packet;
}

quint16 MessageProtocol::calculateChecksum(const QByteArray &data)
{
    quint16 sum = 0;
    for (char byte : data) {
        sum += static_cast<quint8>(byte);
    }
    return sum;
}

QByteArray MessageProtocol::encodeText(const QString &text)
{
    return createPacket(Text, text.toUtf8());
}

QByteArray MessageProtocol::encodeFileHeader(const QString &fileName, qint64 fileSize)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream << fileName;
    stream << fileSize;
    return createPacket(FileHeader, payload);
}

QByteArray MessageProtocol::encodeFileChunk(const QByteArray &chunk, int chunkIndex)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream << quint32(chunkIndex);
    stream.writeRawData(chunk.constData(), chunk.size());
    return createPacket(FileChunk, payload);
}

QByteArray MessageProtocol::encodeFileEnd()
{
    return createPacket(FileEnd, QByteArray());
}

QByteArray MessageProtocol::encodeHeartbeat()
{
    return createPacket(Heartbeat, QByteArray());
}

MessageProtocol::Packet MessageProtocol::decode(const QByteArray &data)
{
    Packet packet;
    packet.type = Text;
    packet.data = QByteArray();

    if (data.size() < HEADER_SIZE + 2) {
        return packet;
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);

    quint16 header;
    quint8 type;
    quint32 payloadSize;

    stream >> header;
    if (header != 0xAA55) {
        return packet;
    }

    stream >> type;
    stream >> payloadSize;

    if (data.size() < static_cast<int>(HEADER_SIZE + payloadSize + 2)) {
        return packet;
    }

    QByteArray payload(payloadSize, Qt::Uninitialized);
    stream.readRawData(payload.data(), payloadSize);

    quint16 receivedChecksum;
    stream >> receivedChecksum;

    quint16 calculatedChecksum = calculateChecksum(payload);
    if (receivedChecksum != calculatedChecksum) {
        return packet;
    }

    packet.type = static_cast<MessageType>(type);
    packet.data = payload;

    return packet;
}
