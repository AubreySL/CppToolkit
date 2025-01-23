#ifndef SNIPASTAPP_H
#define SNIPASTAPP_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include "widget.h"
class SnipastApp : public QObject
{
    Q_OBJECT
public:
    explicit SnipastApp(QObject *parent = nullptr);

public slots:
    void graspScreen();

signals:
    void shotFinished(QPixmap pix);

private:
    void initSysMenu();

private:
    QSystemTrayIcon* sysMenu;
    Widget* mainView;
};

#endif // SNIPASTAPP_H
