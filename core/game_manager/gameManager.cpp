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
                sendProxy::ref().submit(QByteArray::fromHex(QStringLiteral("95270000000B00180B97BD56000000000000000400000000").toLocal8Bit()));

                break;
            }
            default:
                break;
            }
        }
    }).detach();
}

} // namespace roco
