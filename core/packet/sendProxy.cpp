#include "sendProxy.h"

#include <thread>

#include <QDebug>
#include <QtEndian>

#include "../utils/bytes.h"

namespace roco {
auto sendProxy::gameOver() -> void {
    this->working = false;

    this->submit(QByteArray{});
}

auto sendProxy::ref() -> sendProxy & {
    static sendProxy sender {};

    return sender;
}

auto sendProxy::submit(SOCKET sourceSock, char const * buf, int len, int flags) -> bool {
    return pkts.try_enqueue(std::make_pair(QByteArray(buf, len), packetMeta{sourceSock, flags}));
}

auto sendProxy::submit(QByteArray && packetData) -> bool {
    return pkts.try_enqueue(std::make_pair(std::move(packetData), packetMeta{this->getSendSocket(), 0}));
}

auto sendProxy::setSendFunc(decltype(&send) originalSend) -> void {
    this->sendFunc = originalSend;
}

auto sendProxy::getSendFunc() const -> decltype(&send) {
    return this->sendFunc;
}

auto sendProxy::setSendSocket(SOCKET sock) -> void {
    this->sendSock = sock;
}

auto sendProxy::getSendSocket() const -> SOCKET {
    return this->sendSock;
}

auto sendProxy::setFreeTimeTheater(bool value) -> void {
    this->freeTimeTheater = value;
}

auto sendProxy::applyFreeTimeTheater(QByteArray & sourcePacket) -> void {
    if (sourcePacket.size() < 22) {
        return;
    }

    if (*reinterpret_cast<quint32 const *>(sourcePacket.constData() + 4) != qToBigEndian<quint32>(0x00030003)) {
        return;
    }

    if (*reinterpret_cast<quint16 const *>(sourcePacket.constData() + 20) != qToBigEndian<quint16>(0x0090)) {
        return;
    }

    sourcePacket[20] = qToBigEndian<char>(0x01);
    sourcePacket[21] = qToBigEndian<char>(0xE7);

    this->freeTimeTheater = false;
}

auto sendProxy::applyModifier(QByteArray & sourcePacket) -> void {
    if (sourcePacket.size() < 4) {
        return;
    }

    if (*reinterpret_cast<quint32 const *>(sourcePacket.constData()) != qToBigEndian<quint32>(0x95270000)) {
        return;
    }

    if (this->freeTimeTheater) {
        this->applyFreeTimeTheater(sourcePacket);
    }
}

sendProxy::sendProxy(QObject *parent)
    : QObject{parent}
{
    std::thread([this]{
        elementType pkt {};

        while (this->working) {
            pkts.wait_dequeue(pkt);

            if (pkt.second.sourceSock == 0 || this->sendFunc == nullptr) {
                continue;
            }

            this->applyModifier(pkt.first);

            auto ret {this->sendFunc.load()(
                pkt.second.sourceSock, pkt.first.data(), pkt.first.size(), pkt.second.flags)};

            // qDebug() << "what sent is : " << byteArrToStr16(pkt.first) << '\n';

            if (ret == SOCKET_ERROR) {
                qDebug() << "send error!!!\n";

                qDebug() << WSAGetLastError() << '\n';
            }
        }
    }).detach();
}

} // namespace roco
