#include "packetSender.h"

namespace roco {
packetSender::packetSender(QObject *parent)
    : QObject{parent}
{}

auto packetSender::ref() -> packetSender & {
    static packetSender sender {};

    return sender;
}

} //roco
