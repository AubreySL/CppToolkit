#ifndef SNIPASTAPP_H
#define SNIPASTAPP_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include "screenwidget.h"
class SnipastApp : public QObject
{
    Q_OBJECT
public:
    explicit SnipastApp(QObject *parent = nullptr);

public slots:
    void graspScreen();
    void graspFullScreen();
    void hideScreen();

signals:
    void partial_screen();
    void full_screen(QPixmap pix);
    void hide_screen();

private:
    void initSysMenu();

private:
    QSystemTrayIcon* sysMenu;//菜单
    ScreenWidget* mainView;//显示截图窗口
};

#endif // SNIPASTAPP_H
