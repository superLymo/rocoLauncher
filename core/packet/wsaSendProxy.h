#ifndef WSASENDPROXY_H
#define WSASENDPROXY_H

#include <winsock2.h>

#include <QObject>

namespace roco {
auto wsaDetourSend(
    SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) -> int;
}

class wsaSendProxy : public QObject
{
    Q_OBJECT
public:
    explicit wsaSendProxy(QObject *parent = nullptr);

signals:
};

#endif // WSASENDPROXY_H
