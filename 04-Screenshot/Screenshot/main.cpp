#include "screenwidget.h"
#include "snipastapp.h"

#include <QApplication>
#include <QIcon>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));

    SnipastApp sapp;
    //ScreenWidget::instance()->showFullScreen();   //直接调用实例
    return a.exec();
}
