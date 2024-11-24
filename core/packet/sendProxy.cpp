#include "sendProxy.h"

#include <thread>

#include <QDebug>

namespace roco {
auto sendProxy::gameOver() -> void {
    this->working = false;

    this->submit(QByteArray{});
}

auto sendProxy::ref() -> sendProxy & {
    static sendProxy sender {};

    return sender;
}

auto sendProxy::submit(char const * buf, int len, int flags) -> bool {
    return pkts.try_enqueue(std::make_pair(QByteArray(buf, len), flags));
}

auto sendProxy::submit(QByteArray && packetData, int flags) -> bool {
    return pkts.try_enqueue(std::make_pair(std::move(packetData), flags));
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
        elementType pkt {};

        while (this->working) {
            pkts.wait_dequeue(pkt);

            if (this->sendSock == 0 || this->sendFunc == nullptr) {
                continue;
            }

            auto ret {this->sendFunc.load()(
                this->sendSock, pkt.first.data(), pkt.first.size(), pkt.second)};

            if (ret == SOCKET_ERROR) {
                qDebug() << "send error!!!\n";

                qDebug() << WSAGetLastError() << '\n';
            }
        }
    }).detach();
}

} // namespace roco
