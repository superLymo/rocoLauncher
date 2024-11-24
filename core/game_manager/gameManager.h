#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>

namespace roco {

class gameManager : public QObject
{
    Q_OBJECT
public:
    explicit gameManager(QObject *parent = nullptr);

signals:
};

} // namespace roco

#endif // GAMEMANAGER_H
