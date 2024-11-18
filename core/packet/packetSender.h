#ifndef PACKETSENDER_H
#define PACKETSENDER_H

#include <QObject>

namespace roco {
class packetSender : public QObject
{
    Q_OBJECT
public:
    static auto ref() -> packetSender &;

private:
    explicit packetSender(QObject *parent = nullptr);

signals:
};
} //roco

#endif // PACKETSENDER_H
