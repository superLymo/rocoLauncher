#include "sendProxy.h"

#include <thread>

#include <QDebug>

namespace roco {
auto sendProxy::ref() -> sendProxy & {
    static sendProxy sender {};

    return sender;
}

auto sendProxy::submit(SOCKET s, char const * buf, int len, int flags) -> bool {
    auto ret {pkts.try_push(std::make_pair(packetMeta{s, flags}, QByteArray(buf, len)))};

    cv.notify_one();

    return ret;
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
        std::unique_lock<std::mutex> lck(mtx);

        while (true) {
            cv.wait(lck, [this, &pkt]() -> bool {
                return this->pkts.try_pop(pkt);
            });

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
