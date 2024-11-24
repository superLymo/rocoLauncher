#ifndef ROCOWINDOW_H
#define ROCOWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class rocoWindow;
}
QT_END_NAMESPACE

namespace QWK {
class WidgetWindowAgent;
}

namespace roco {
class gameManager;
}

namespace roco {
class rocoWindow : public QMainWindow
{
    Q_OBJECT

public:
    rocoWindow(QWidget *parent = nullptr);
    ~rocoWindow();

public:
    auto gameOver() -> void;

private:
    auto installWindowAgent() -> void;
    auto createMenuBar() -> QMenuBar *;

private:
    Ui::rocoWindow * ui {};
    QWK::WidgetWindowAgent * windowAgent {};

    roco::gameManager * game {};
};
} // namespace roco
#endif // ROCOWINDOW_H
