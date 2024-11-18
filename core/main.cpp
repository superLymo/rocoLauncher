#include <winsock2.h>
#include <Windows.h>
#include <vector>

#include <QApplication>
#include <QThread>

#include "MinHook.h"
#include "hook_configure/hookConfigure.h"
#include "roco_window/rocoWindow.h"


roco::hookConfigure<decltype(&send)> sendHookConf;
roco::hookConfigure<decltype(&recv)> recvHookConf;
roco::hookConfigure<decltype(&WSASend)> wsaSendHookConf;
roco::hookConfigure<decltype(&WSARecv)> wsaRecvHookConf;

auto rocoDetourSend(SOCKET s, char const * buf, int len, int flags) -> int {

    return sendHookConf.pOriginalFunc(s, buf, len, flags);
}

auto rocoDetourWsaSend(
    SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) -> int {

    return wsaSendHookConf.pOriginalFunc(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
}


int main(int argc, char *argv[])
{
    if (MH_Initialize() != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return 1;
    }

    QApplication qtApp(argc, argv);

    auto ws2Handle {GetModuleHandleA("ws2_32")};

    if (!ws2Handle) {
        qDebug() << "wsw_32 module cannot be loaded!\n";

        return 1;
    }

    sendHookConf.pTargetFunc = reinterpret_cast<decltype(&send)>(GetProcAddress(ws2Handle, "send"));
    sendHookConf.pDetourFunc = &rocoDetourSend;

    wsaSendHookConf.pTargetFunc =
        reinterpret_cast<decltype(&WSASend)>(GetProcAddress(ws2Handle, "WSASend"));
    wsaSendHookConf.pDetourFunc = &rocoDetourWsaSend;

    if (!sendHookConf.pTargetFunc) {
        qDebug() << "send func cannot be found!\n";

        return 1;
    }

    if (!wsaSendHookConf.pTargetFunc) {
        qDebug() << "wsaSend func cannot be found!\n";

        return 1;
    }

    if (MH_CreateHook(reinterpret_cast<LPVOID>(sendHookConf.pTargetFunc),
                      reinterpret_cast<LPVOID>(sendHookConf.pDetourFunc),
                      reinterpret_cast<LPVOID*>(&sendHookConf.pOriginalFunc)) != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return 1;
    }

    if (MH_CreateHook(reinterpret_cast<LPVOID>(wsaSendHookConf.pTargetFunc),
                      reinterpret_cast<LPVOID>(wsaSendHookConf.pDetourFunc),
                      reinterpret_cast<LPVOID*>(&wsaSendHookConf.pOriginalFunc)) != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return 1;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return 1;
    }

    rocoWindow rocoApp;
    rocoApp.show();

    auto qtAppRet {qtApp.exec()};

    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return 1;
    }

    if (MH_Uninitialize() != MH_OK) {
        qDebug() << __LINE__ << '\n';

        return 1;
    }

    return qtAppRet;
}
