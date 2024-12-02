#ifndef SENDMODIFIER_H
#define SENDMODIFIER_H

#include <QObject>

namespace roco {

class sendModifier : public QObject
{
    Q_OBJECT
public:
    explicit sendModifier(QObject *parent = nullptr);

signals:
};

} // namespace roco

#endif // SENDMODIFIER_H
