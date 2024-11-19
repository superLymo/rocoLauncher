#include "sendProxy.h"

#include <thread>

#include <QDebug>

namespace roco {
auto detourSend(SOCKET s, char const * buf, int len, int flags) -> int {
    static WSAPROTOCOL_INFOW gameSockInfo {};

    if (roco::sendProxy::ref().getSendSocket() == 0 && len == 49) {
        if (WSADuplicateSocket(s, GetCurrentProcessId(), &gameSockInfo) == NO_ERROR) {
            SOCKET copiedSock {WSASocket(
                FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, &gameSockInfo, 0, 0)};

            if (copiedSock != INVALID_SOCKET) {
                qDebug() << "i got copiedSock : " << copiedSock << '\n';

                roco::sendProxy::ref().setSendSocket(copiedSock);
            } else {
                qDebug() << "copied socket created failed!\n";
                qDebug() << WSAGetLastError() << '\n';
            }
        } else {
            qDebug() << "socket duplicated failed!\n";
            qDebug() << WSAGetLastError() << '\n';
        }
    }

    if (roco::sendProxy::ref().getSendSocket() == s) {
        sendProxy::ref().submit(s, buf, len, flags);

        return len;
    }

    return sendProxy::ref().getSendFunc()(s, buf, len, flags);
}

auto sendProxy::ref() -> sendProxy & {
    static sendProxy sender {};

    return sender;
}

auto sendProxy::submit(SOCKET s, char const * buf, int len, int flags) -> bool {
    // if (u_long blockMode {1}; ioctlsocket(s, FIONBIO, &blockMode) != NO_ERROR) {
    //     qDebug() << "set sock non blocking mode is damn shit!\n";
    // }

    return pkts.try_push(std::make_pair(packetMeta{s, flags}, QByteArray(buf, len)));
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

sendProxy::sendProxy(QObject *parent)
    : QObject{parent}
{
    std::thread([this]{
        while (true) {
            auto pkt {this->pkts.pop()};

            if (this->sendSock == 0 || this->sendFunc == nullptr) {
                continue;
            }

            auto ret {this->sendFunc.load()(
                this->sendSock, pkt.second.data(), pkt.second.size(), pkt.first.sendFlags)};

            if (ret == SOCKET_ERROR) {
                qDebug() << "send error!!!\n";
            }
        }
    }).detach();
}

} // namespace roco
