#include <thread>

#include "gameManager.h"

#include "../packet/sendProxy.h"

namespace roco {

auto gameManager::nothingToDo() -> void {
    this->opQue.try_enqueue(operation::nothing_to_do);
}

auto gameManager::healPetsInBag() -> void {
    this->opQue.try_enqueue(operation::head_pets_in_bag);
}

auto gameManager::timeFreeze() -> void {
    this->opQue.try_enqueue(operation::time_freeze);
}

auto gameManager::refresh() -> void {
    sendProxy::ref().setSendSocket(0);

    this->qqNumber = {};
    this->vip = {};
    this->currentMap = {};
}

auto gameManager::gameOver() -> void {
    this->working = false;

    this->nothingToDo();
}

gameManager::gameManager(QObject *parent)
    : QObject{parent}
{
    std::thread([this]{
        while (this->working) {
            operation opValue {};

            this->opQue.wait_dequeue(opValue);

            switch (opValue) {
            case operation::head_pets_in_bag: {
                sendProxy::ref().submit(QByteArray::fromHex(QStringLiteral("95270000000B001809FF8FEE000000000000000400000000").toLocal8Bit()));

                break;
            }
            case operation::time_freeze: {
                sendProxy::ref().setFreeTimeTheater(true);

                sendProxy::ref().submit(QByteArray::fromHex(QStringLiteral("952700000003000409FF8FEE000000000000000A000D0090000000000001").toLocal8Bit()));

                break;
            }
            default:
                break;
            }
        }
    }).detach();
}

} // namespace roco
