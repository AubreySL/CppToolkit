#include "snipastapp.h"
#include <QApplication>
#include <QScreen>
#include <QCursor>
#include <QEvent>
#include "keycapture.h"
#include "keymonitor.h"

SnipastApp::SnipastApp(QObject *parent)
    : QObject{parent},
    sysMenu(new QSystemTrayIcon(this)),
    mainView(new ScreenWidget)
{
    this->sysMenu->setIcon(QIcon("://logo"));

    initSysMenu();
    this->sysMenu->show();
    connect(this,&SnipastApp::full_screen, this->mainView, &ScreenWidget::receivePix);
    connect(this,&SnipastApp::partial_screen, this->mainView, &ScreenWidget::showFullScreen);
    connect(this,&SnipastApp::hide_screen, this->mainView, &ScreenWidget::hideScreen);
    //键盘监听
    connect(KeyCapture::instance(), &KeyCapture::getKey, [=](int key){
        // qDebug()<<QString::number(key);
        if(key == keycode::F1){
            graspScreen();
        }
        if(key == keycode::F2){
            graspFullScreen();
        }
        if(key == keycode::ESC){
            hideScreen();
        }
    });
    startHook();
}

void SnipastApp::graspScreen()
{
    emit partial_screen();
}

void SnipastApp::graspFullScreen()
{
    QList<QScreen*> screens = qApp->screens();
    if(screens.isEmpty())
    {
        qWarning()<< "没有找到显示器";
        return;
    }
    QPoint cursorPos = QCursor::pos();
    QPixmap pix;
    for(auto screen:screens)
    {
        QRect rect = screen->geometry();
        if(rect.contains(cursorPos))
        {
            pix = screen->grabWindow();
        }
    }

    if(pix.isNull())
    {
        qWarning()<< "截图像素为空";
        return;
    }

    emit full_screen(pix);
}

void SnipastApp::hideScreen()
{
    emit hide_screen();
}

void SnipastApp::initSysMenu()
{
    QMenu* menu = new QMenu;
    menu->addAction("截图 F1", this, SLOT(graspScreen()));
    menu->addAction("截全屏 F2", this, SLOT(graspFullScreen()));
    menu->addAction("退出", qApp, SLOT(quit()));

    this->sysMenu->setContextMenu(menu);
}

