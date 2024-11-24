#include <windows.h>

#include <QAxObject>
#include <QProcess>

#include <QWKWidgets/widgetwindowagent.h>
#include "../widget_frame/windowbar.h"
#include "../widget_frame/windowbutton.h"
#include "../game_manager/gameManager.h"

#include "rocoWindow.h"
#include "ui_rocoWindow.h"

namespace roco {
rocoWindow::rocoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::rocoWindow), game {new roco::gameManager(this)}
{
    this->setAttribute(Qt::WA_DontCreateNativeAncestors);

    installWindowAgent();

    ui->setupUi(this);

    this->setWindowTitle(QStringLiteral("洛克王国社区版"));

    this->setFixedWidth(960);
    this->setFixedHeight(597);
    //this->windowAgent->centralize();

    ui->axWidget->dynamicCall("SetSilent(bool)", true);
    ui->axWidget->dynamicCall("Navigate(QString)", "https://17roco.qq.com/qzone.html");
}

rocoWindow::~rocoWindow()
{
    delete ui;
}

auto rocoWindow::installWindowAgent() -> void {
    this->windowAgent = new QWK::WidgetWindowAgent(this);
    this->windowAgent->setup(this);

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

    auto menuBar {this->createMenuBar()};

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

auto rocoWindow::createMenuBar() -> QMenuBar * {
    auto menuBar {new QMenuBar(this)};

    // game menu
    auto gameMenu {new QMenu(QStringLiteral("游戏"), menuBar)};

    auto refreshAction {new QAction(QStringLiteral("重新登录"), menuBar)};

    connect(refreshAction, &QAction::triggered, this, [this]{
        this->game->refresh();

        this->ui->axWidget->dynamicCall("Refresh()");
    });

    auto clearTracksAction {new QAction(QStringLiteral("清理缓存"), menuBar)};

    connect(clearTracksAction, &QAction::triggered, this, [this]{
        QProcess{}.startDetached(
            QStringLiteral("RunDll32.exe"),
            {QStringLiteral("InetCpl.cpl,ClearMyTracksByProcess"), QStringLiteral("255")});
    });

    auto silentAction {new QAction(QStringLiteral("游戏静音"), menuBar)};
    silentAction->setCheckable(true);

    connect(silentAction, &QAction::triggered, this, [this](bool checked){
        waveOutSetVolume(nullptr, checked ? 0x0000 : 0xFFFF);
    });

    silentAction->setChecked(false);

    auto timeFreezeAction {new QAction(QStringLiteral("战斗免时"), menuBar)};
    timeFreezeAction->setCheckable(true);

    connect(timeFreezeAction, &QAction::triggered, this, [this](bool checked){

    });

    timeFreezeAction->setChecked(false);

    gameMenu->addAction(refreshAction);
    gameMenu->addAction(clearTracksAction);
    gameMenu->addSeparator();
    gameMenu->addAction(silentAction);
    gameMenu->addAction(timeFreezeAction);

    // operation menu
    auto operationMenu {new QMenu(QStringLiteral("操作"), menuBar)};

    auto healAllAction {new QAction(QStringLiteral("全宠物恢复"), menuBar)};

    connect(healAllAction, &QAction::triggered, this, [this]{
        this->game->healPetsInBag();
    });

    // auto ngplPP0Action {new QAction(QStringLiteral("马桶防控压0"), menuBar)};
    // auto ngplPP1Action {new QAction(QStringLiteral("马桶防控压1"), menuBar)};

    operationMenu->addAction(healAllAction);
    // operationMenu->addSeparator();
    // operationMenu->addAction(ngplPP0Action);
    // operationMenu->addAction(ngplPP1Action);

    // create menus over

    menuBar->addMenu(gameMenu);
    menuBar->addMenu(operationMenu);

    menuBar->setObjectName(QStringLiteral("roco-menu-bar"));

    return menuBar;
}
} // namespace roco
