#include "wsaPacketSender.h"

namespace roco {
auto wsaDetourSend(
    SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) -> int {

    return NO_ERROR;
}
}

wsaPacketSender::wsaPacketSender(QObject *parent)
    : QObject{parent}
{}
