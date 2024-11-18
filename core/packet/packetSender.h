#ifndef PACKETSENDER_H
#define PACKETSENDER_H

#include <winsock2.h>
#include <Windows.h>

#include <QObject>
#include <QByteArray>

#include "../atomic_queue/atomic_queue.h"

namespace roco {

auto detourSend(SOCKET s, char const * buf, int len, int flags) -> int;

class packetSender : public QObject
{
    Q_OBJECT

public:
    struct packetMeta {
        SOCKET sock {};
        int bufLen {};
        int sendFlags {};
    };

    using elementType = std::pair<packetMeta, QByteArray>;
    using packetQueue = atomic_queue::AtomicQueueB2<elementType, std::allocator<elementType>>;
private:
    packetQueue pkts {65536};

    std::function<decltype(send)> originalSend;

    std::atomic<bool> working {};

    std::atomic<SOCKET> gameSock {};
public:
    static auto ref() -> packetSender &;

    auto run(std::function<decltype(send)> const & originalSend) -> void;
    auto stop() -> void;
    auto running() const -> bool;

    auto submit(SOCKET s, char const * buf, int len, int flags) -> bool;

    auto isGameSocket(SOCKET sock) const -> bool;
private:
    explicit packetSender(QObject *parent = nullptr);

    packetSender(packetSender const &) = delete;
    auto operator=(packetSender const &) -> packetSender & = delete;
signals:
};
} //roco

#endif // PACKETSENDER_H
