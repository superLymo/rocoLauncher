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
class rocoWindow : public QMainWindow
{
    Q_OBJECT

public:
    rocoWindow(QWidget *parent = nullptr);
    ~rocoWindow();

private:
    auto installWindowAgent() -> void;

private:
    Ui::rocoWindow * ui {};
    QWK::WidgetWindowAgent * windowAgent {};
};
} // namespace roco
#endif // ROCOWINDOW_H
