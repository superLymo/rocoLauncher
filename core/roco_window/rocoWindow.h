#ifndef ROCOWINDOW_H
#define ROCOWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class rocoWindow;
}
QT_END_NAMESPACE

class rocoWindow : public QMainWindow
{
    Q_OBJECT

public:
    rocoWindow(QWidget *parent = nullptr);
    ~rocoWindow();

private:
    Ui::rocoWindow *ui;
};
#endif // ROCOWINDOW_H
