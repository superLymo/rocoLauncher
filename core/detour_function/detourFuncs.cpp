#include "detourFuncs.h"

#include <QDebug>

namespace roco {
auto detourSend(SOCKET s, char const * buf, int len, int flags) -> int {
    if (roco::sendProxy::ref().getSendSocket() == 0 && len == 49) {
        roco::sendProxy::ref().setSendSocket(s);
    }

    if (roco::sendProxy::ref().getSendSocket() == s) {
        sendProxy::ref().submit(s, buf, len, flags);

        return len;
    }

    return sendProxy::ref().getSendFunc()(s, buf, len, flags);
}
} // namespace roco
