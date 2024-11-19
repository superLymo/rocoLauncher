#include "wsaPacketSender.h"

namespace roco {
auto wsaDetourSend(
    SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) -> int {

    return NO_ERROR;
}
}

wsaSendProxy::wsaSendProxy(QObject *parent)
    : QObject{parent}
{}
