#include "screen.h"
#include <QDebug>
Screen::Screen(const QSize& size, const int dpi = 1)
{
    maxWidth = size.width();
    maxHeight = size.height();

    startPos = QPoint(-1, -1);
    endPos = startPos;
    leftUpPos = startPos;
    rightDownPos = startPos;
    status=SELECT;
    devicePixRatio = dpi;
}

void Screen::setStart(const QPoint& pos)
{
    startPos = pos;
}

void Screen::setEnd(const QPoint& pos)
{
    endPos = pos;
    leftUpPos = startPos;
    rightDownPos = endPos;
    cmpPoint(leftUpPos, rightDownPos);
}

QPoint Screen::getStart()
{
    return startPos;
}

QPoint Screen::getEnd()
{
    return endPos;
}

QPoint Screen::getLeftUp()
{
    return leftUpPos;
}

QPoint Screen::getRightDown()
{
    return rightDownPos;
}

void Screen::setStatus(const STATUS& status)
{
    this->status = status;
}


Screen::STATUS Screen::getStatus()
{
    return status;
}

int Screen::getMaxWidth()
{
    return maxWidth;
}

int Screen::getMaxHeight()
{
    return maxHeight;
}

bool Screen::isInArea(const QPoint& pos)
{
    int x = pos.x();
    int y = pos.y();
    if(x > leftUpPos.x() && x<rightDownPos.x()
        && y > leftUpPos.y() && y<rightDownPos.y())
    {
        return true;
    }
    return false;
}

void Screen::move(const QPoint& p)
{
    int x = p.x();
    int y = p.y();
    int lx = leftUpPos.x() + x;
    int ly = leftUpPos.y() + y;
    int rx = rightDownPos.x() + x;
    int ry = rightDownPos.y() + y;

    //检查是否在区域内
    if(lx < 0)
    {
        lx = 0;
        rx -=x;
    }
    if(ly < 0)
    {
        ly = 0;
        ry -=y;
    }

    if(rx > maxWidth)
    {
        rx = maxWidth;
        lx -=x;
    }
    if(ry > maxHeight)
    {
        ry = maxHeight;
        ly -=y;
    }

    leftUpPos = QPoint(lx, ly);
    rightDownPos = QPoint(rx, ry);
    startPos = leftUpPos;
    endPos = rightDownPos;
}

int Screen::getDPI()
{
    return devicePixRatio;
}

void Screen::cmpPoint(QPoint& leftTop, QPoint& rightDown)
{
    QPoint l = leftTop;
    QPoint r = rightDown;

    if(l.x() <= r.x())
    {
        if(l.y() <= r.y())
        {

        }else{
            leftTop.setY(r.y());
            rightDown.setY(l.y());
        }
    }
    else
    {
        if(l.y()<r.y())
        {
            leftTop.setX(r.x());
            rightDown.setX(l.x());
        } else {
            // std::swap(leftTop, rightDown);
            QPoint tmp;
            tmp = leftTop;
            leftTop = rightDown;
            rightDown = tmp;
        }
    }
}

