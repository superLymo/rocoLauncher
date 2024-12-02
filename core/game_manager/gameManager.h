#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <atomic>

#include <QByteArray>
#include <QObject>

#include "../readerwriterqueue/readerwriterqueue.h"

namespace roco {

class gameManager : public QObject
{
    Q_OBJECT

private:
    enum class operation {
        nothing_to_do,
        head_pets_in_bag,
        time_freeze,
    };

    std::atomic<bool> working {true};

    std::atomic<quint32> qqNumber {};
    std::atomic<bool> vip {};
    std::atomic<quint16> currentMap {};

    moodycamel::BlockingReaderWriterQueue<operation> opQue {256};

public:
    auto nothingToDo() -> void;
    auto healPetsInBag() -> void;

    auto timeFreeze() -> void;

    auto refresh() -> void;
    auto gameOver() -> void;

public:
    explicit gameManager(QObject *parent = nullptr);

signals:
};

} // namespace roco

#endif // GAMEMANAGER_H
