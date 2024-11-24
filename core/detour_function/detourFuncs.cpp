#include <QDebug>
#include <QtEndian>

#include "../packet/sendProxy.h"

#include "detourFuncs.h"

namespace roco {
auto detourSend(SOCKET s, char const * buf, int len, int flags) -> int {
    if (len >= 26 &&
        *reinterpret_cast<quint32 const *>(buf) == static_cast<quint32>(qToBigEndian(0x7467775F)) &&
        *reinterpret_cast<quint32 const *>(buf + 22) == static_cast<quint32>(qToBigEndian(0x7A6F6E65))
        ) {
        roco::sendProxy::ref().setSendSocket(s);
    }

    if (roco::sendProxy::ref().getSendSocket() == s) {
        sendProxy::ref().submit(buf, len, flags);

        return len;
    }

    return sendProxy::ref().getSendFunc()(s, buf, len, flags);
}
} // namespace roco
