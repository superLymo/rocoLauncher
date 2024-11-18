#ifndef HOOKCONFIGURE_H
#define HOOKCONFIGURE_H

namespace roco {
template<typename pFuncType_>
struct hookConfigure {
    using pFuncType = pFuncType_;

    pFuncType pTargetFunc {};
    pFuncType pDetourFunc {};
    pFuncType pOriginalFunc {};
};
} //roco

#endif // HOOKCONFIGURE_H
