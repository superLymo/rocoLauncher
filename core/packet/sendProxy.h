#ifndef SENDPROXY_H
#define SENDPROXY_H

#include "../concurrentqueue/blockingconcurrentqueue.h"

#include <winsock2.h>

#include <QObject>
#include <QByteArray>

namespace roco {
class sendProxy : public QObject
{
    Q_OBJECT

public:
    using elementType = std::pair<QByteArray, int>;
    using packetQueue = moodycamel::BlockingConcurrentQueue<elementType>;
private:
    packetQueue pkts {1024};

    std::atomic<decltype(&send)> sendFunc {};
    std::atomic<SOCKET> sendSock {};

public:
    static auto ref() -> sendProxy &;

    auto submit(char const * buf, int len, int flags) -> bool;

    auto setSendFunc(decltype(&send) originalSend) -> void;
    auto getSendFunc() const -> decltype(&send);

    auto setSendSocket(SOCKET sock) -> void;
    auto getSendSocket() const -> SOCKET;
private:
    explicit sendProxy(QObject *parent = nullptr);

    sendProxy(sendProxy const &) = delete;
    auto operator=(sendProxy const &) -> sendProxy & = delete;
signals:
};
} //roco

#endif // SENDPROXY_H
