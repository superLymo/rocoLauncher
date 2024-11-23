#ifndef PACKETSUBMITTER_H
#define PACKETSUBMITTER_H

namespace roco {

class packetSubmitter : public QObject
{
    Q_OBJECT
public:
    explicit packetSubmitter(QObject *parent = nullptr);

signals:
};

} // namespace roco

#endif // PACKETSUBMITTER_H
