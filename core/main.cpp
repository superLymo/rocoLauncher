#include <winsock2.h>
#include <Windows.h>

#include <QApplication>
#include <QThread>

#include "MinHook.h"
#include "roco_window/rocoWindow.h"
#include "packet/sendProxy.h"
#include "detour_function/detourFuncs.h"


int main(int argc, char *argv[])
{
    WSADATA wsadt;

    if (WSAStartup(MAKEWORD(2, 2), &wsadt) != NO_ERROR) {
        qDebug() << "wsa start up failed!\n";

        return EXIT_FAILURE;
    }

    if (MH_Initialize() != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return EXIT_FAILURE;
    }

    QApplication qtApp(argc, argv);

    auto ws2Handle {GetModuleHandleA("ws2_32")};

    if (!ws2Handle) {
        qDebug() << "wsw_32 module cannot be loaded!\n";

        return EXIT_FAILURE;
    }

    auto pTargetFunc {reinterpret_cast<decltype(&send)>(GetProcAddress(ws2Handle, "send"))};

    if (!pTargetFunc) {
        qDebug() << "send func cannot be found!\n";

        return EXIT_FAILURE;
    }

    decltype(pTargetFunc) pOriginalFunc {};

    if (MH_CreateHook(reinterpret_cast<LPVOID>(pTargetFunc),
                      reinterpret_cast<LPVOID>(&roco::detourSend),
                      reinterpret_cast<LPVOID*>(&pOriginalFunc)) != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return EXIT_FAILURE;
    }

    roco::sendProxy::ref().setSendFunc(pOriginalFunc);

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return EXIT_FAILURE;
    }

    rocoWindow rocoApp;
    rocoApp.show();

    auto qtAppRet {qtApp.exec()};

    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return EXIT_FAILURE;
    }

    if (MH_Uninitialize() != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return EXIT_FAILURE;
    }

    closesocket(roco::sendProxy::ref().getSendSocket());

    WSACleanup();

    return qtAppRet;
}
