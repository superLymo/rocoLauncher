#include "packetSender.h"

#include <thread>

#include <QDebug>

#include "../utils/bytes.h"

namespace roco {
auto detourSend(SOCKET s, char const * buf, int len, int flags) -> int {
    qDebug() << "ready to summit packet: ";
    qDebug() << byteArrToStr16(QByteArray(buf, len)) << '\n';

    packetSender::ref().submit(s, buf, len, flags);

    return NO_ERROR;
}

packetSender::packetSender(QObject *parent)
    : QObject{parent}
{}

auto packetSender::ref() -> packetSender & {
    static packetSender sender {};

    return sender;
}

auto packetSender::run(std::function<decltype(send)> const & originalSend) -> void {
    if (this->working) {
        qDebug() << "you can run twice!\n";

        return;
    }

    this->originalSend = originalSend;
    this->working = true;

    std::thread([this]{
        while (this->working) {
            auto pkt {this->pkts.pop()};

            auto ret {this->originalSend(
                pkt.first.sock, pkt.second.data(), pkt.first.bufLen, pkt.first.sendFlags)};

            if (ret == SOCKET_ERROR) {
                qDebug() << "send error!!!\n";
            } else {
                qDebug() << pkt.first.sock << "   send successful for num: " << pkt.first.bufLen << " : ";
                qDebug() << byteArrToStr16(pkt.second) << '\n';
            }
        }
    }).detach();
}

auto packetSender::stop() -> void {
    this->working = false;
}

auto packetSender::running() const -> bool {
    return this->working;
}

auto packetSender::submit(SOCKET s, char const * buf, int len, int flags) -> bool {
    // if (u_long blockMode {1}; ioctlsocket(s, FIONBIO, &blockMode) != NO_ERROR) {
    //     qDebug() << "set sock non blocking mode is damn shit!\n";
    // }

    qDebug() << "submitted!\n";

    return pkts.try_push(std::make_pair(packetMeta{s, len, flags}, QByteArray(buf, len)));
}

auto packetSender::isGameSocket(SOCKET sock) const -> bool {
    return this->gameSock == sock;
}

} //roco
