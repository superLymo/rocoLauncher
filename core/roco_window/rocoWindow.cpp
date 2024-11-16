#include <QAxObject>

#include "rocoWindow.h"
#include "ui_rocoWindow.h"

rocoWindow::rocoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rocoWindow)
{
    ui->setupUi(this);

    ui->axWidget->dynamicCall("Navigate(QString const &)","https://17roco.qq.com/qzone.html");
}

rocoWindow::~rocoWindow()
{
    delete ui;
}
