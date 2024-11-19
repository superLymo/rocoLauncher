#ifndef DETOURFUNCS_H
#define DETOURFUNCS_H

#include "../packet/sendProxy.h"

namespace roco {
auto detourSend(SOCKET s, char const * buf, int len, int flags) -> int;
} // namespace roco

#endif // DETOURFUNCS_H
