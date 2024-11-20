#include <QAxObject>

#include <QWKWidgets/widgetwindowagent.h>
#include "../widget_frame/windowbar.h"
#include "../widget_frame/windowbutton.h"

#include "rocoWindow.h"
#include "ui_rocoWindow.h"

namespace roco {
rocoWindow::rocoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rocoWindow)
{
    this->setAttribute(Qt::WA_DontCreateNativeAncestors);

    installWindowAgent();

    ui->setupUi(this);

    this->setWindowTitle(QStringLiteral("洛克王国社区版"));

    ui->axWidget->dynamicCall("Navigate(https://17roco.qq.com/qzone.html)");
}

rocoWindow::~rocoWindow()
{
    delete ui;
}

auto rocoWindow::installWindowAgent() -> void {
    this->windowAgent = new QWK::WidgetWindowAgent(this);
    this->windowAgent->setup(this);

    auto menuBar {new QMenuBar(this)};

    // game menu
    auto gameMenu {new QMenu(QStringLiteral("游戏"), menuBar)};

    auto refreshAction {new QAction(QStringLiteral("刷新"), menuBar)};
    auto silentAction {new QAction(QStringLiteral("静音"), menuBar)};
    silentAction->setCheckable(true);
    auto timeFreezeAction {new QAction(QStringLiteral("战斗免时"), menuBar)};
    timeFreezeAction->setCheckable(true);

    gameMenu->addAction(refreshAction);
    gameMenu->addSeparator();
    gameMenu->addAction(silentAction);
    gameMenu->addAction(timeFreezeAction);

    // operation menu
    auto operationMenu {new QMenu(QStringLiteral("行为"), menuBar)};

    auto ngplPP0Action {new QAction(QStringLiteral("马桶防控压0"), menuBar)};
    auto ngplPP1Action {new QAction(QStringLiteral("马桶防控压1"), menuBar)};

    operationMenu->addAction(ngplPP0Action);
    operationMenu->addAction(ngplPP1Action);

    menuBar->addMenu(gameMenu);
    menuBar->addMenu(operationMenu);

    menuBar->setObjectName(QStringLiteral("roco-menu-bar"));

    auto titleLabel {new QLabel};
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName(QStringLiteral("roco-title-label"));

    auto iconButton {new QWK::WindowButton};
    iconButton->setObjectName(QStringLiteral("roco-icon-button"));
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto minButton {new QWK::WindowButton};
    minButton->setObjectName(QStringLiteral("roco-min-button"));
    minButton->setProperty("system-button", true);
    minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto closeButton {new QWK::WindowButton};
    closeButton->setObjectName(QStringLiteral("roco-close-button"));
    closeButton->setProperty("system-button", true);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto windowBar {new QWK::WindowBar};

    windowBar->setIconButton(iconButton);
    windowBar->setMinButton(minButton);
    windowBar->setCloseButton(closeButton);

    windowBar->setMenuBar(menuBar);
    windowBar->setTitleLabel(titleLabel);
    windowBar->setHostWidget(this);

    this->windowAgent->setTitleBar(windowBar);

    this->windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    this->windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
    this->windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);

    this->windowAgent->setHitTestVisible(menuBar, true);

    this->setMenuWidget(windowBar);

    connect(windowBar, &QWK::WindowBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(windowBar, &QWK::WindowBar::closeRequested, this, &QWidget::close);
}
} // namespace roco
