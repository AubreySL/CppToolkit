#include "snipastapp.h"
#include <QApplication>
#include <QScreen>
#include <QCursor>
SnipastApp::SnipastApp(QObject *parent)
    : QObject{parent},
    sysMenu(new QSystemTrayIcon(this)),
    mainView(new Widget)
{
    this->sysMenu->setIcon(QIcon("://logo"));

    initSysMenu();
    this->sysMenu->show();
    connect(this,&SnipastApp::shotFinished, this->mainView, &Widget::receivePix);
}

void SnipastApp::graspScreen()
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

    emit shotFinished(pix);

}

void SnipastApp::initSysMenu()
{
    QMenu* menu = new QMenu;
    menu->addAction("截图", this, SLOT(graspScreen()));
    menu->addAction("退出", qApp, SLOT(quit()));

    this->sysMenu->setContextMenu(menu);
}
