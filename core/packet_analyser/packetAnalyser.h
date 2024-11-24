#ifndef PACKETANALYSER_H
#define PACKETANALYSER_H

#include <QObject>

namespace roco {

class packetAnalyser : public QObject
{
    Q_OBJECT
public:
    explicit packetAnalyser(QObject *parent = nullptr);

signals:
};

} // namespace roco

#endif // PACKETANALYSER_H
