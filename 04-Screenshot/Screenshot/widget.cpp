#include "widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{}

Widget::~Widget() {}

void Widget::receivePix(QPixmap pix)
{
    this->windowPix = pix;
    this->show();
}


void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(!this->windowPix.isNull())
    {
        painter.drawPixmap(this->rect(),this->windowPix);
    }
}
