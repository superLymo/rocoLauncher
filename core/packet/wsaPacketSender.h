#ifndef WSAPACKETSENDER_H
#define WSAPACKETSENDER_H

#include <winsock2.h>
#include <Windows.h>

#include <QObject>

namespace roco {
auto wsaDetourSend(
    SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) -> int;
}

class wsaPacketSender : public QObject
{
    Q_OBJECT
public:
    explicit wsaPacketSender(QObject *parent = nullptr);

signals:
};

#endif // WSAPACKETSENDER_H
