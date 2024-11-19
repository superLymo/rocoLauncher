#include "detourFuncs.h"

#include <QDebug>

namespace roco {
auto detourSend(SOCKET s, char const * buf, int len, int flags) -> int {
    if (roco::sendProxy::ref().getSendSocket() == 0 && len == 49) {
        WSAPROTOCOL_INFOW gameSockInfo {};

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
} // namespace roco
