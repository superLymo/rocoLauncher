#ifndef PACKETMODIFIER_H
#define PACKETMODIFIER_H

#include <QObject>

namespace roco {

class packetModifier : public QObject
{
    Q_OBJECT
public:
    explicit packetModifier(QObject *parent = nullptr);

signals:
};

} // namespace roco

#endif // PACKETMODIFIER_H
