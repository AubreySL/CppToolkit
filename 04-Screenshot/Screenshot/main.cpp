#include "snipastapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SnipastApp sapp;
    return a.exec();
}
