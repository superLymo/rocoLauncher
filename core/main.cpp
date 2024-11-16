#include <Windows.h>
#include <QApplication>

#include "MinHook.h"
#include "roco_window/rocoWindow.h"


using pWs2SendFuncType = int(*)(SOCKET s, char const * buf, int len, int flags);

auto rocoHookSend(SOCKET s, char const * buf, int len, int flags) -> int {


    return 0;
}


int main(int argc, char *argv[])
{
    MH_Initialize();

    QApplication a(argc, argv);

    HMODULE ws2Handle = GetModuleHandleA("ws2_32");

    if (!ws2Handle) {
        qDebug() << "wsw_32 module cannot be loaded!\n";

        return 1;
    }

    auto pTargetFunc = reinterpret_cast<pWs2SendFuncType>(GetProcAddress(ws2Handle, "send"));

    if (!pTargetFunc) {
        qDebug() << "send func cannot be found!\n";

        return 1;
    }

    qDebug() << reinterpret_cast<uint64_t>(pTargetFunc) << '\n';

    rocoWindow w;
    w.show();

    auto qtAppRet {a.exec()};

    MH_Uninitialize();

    return qtAppRet;
}
