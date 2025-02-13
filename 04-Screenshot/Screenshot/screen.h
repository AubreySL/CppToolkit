#ifndef SCREEN_H
#define SCREEN_H

#include <QPoint>
#include <QSize>

/**
 * @brief The Screen class
 * 屏幕信息对象
 */

class Screen
{
public:
    enum STATUS{SELECT, MOV, SET_W_H};
    Screen();
    Screen(const QSize& size, const int dpi);

    void setStart(const QPoint& pos);
    void setEnd(const QPoint& pos);
    QPoint getStart();
    QPoint getEnd();

    QPoint getLeftUp();
    QPoint getRightDown();

    STATUS getStatus();
    void setStatus(const STATUS& status);

    int getMaxWidth();
    int getMaxHeight();
    bool isInArea(const QPoint& pos);//是否在截图区域内
    void move(const QPoint& p);//移动截图区域
    int getDPI();


private:
    QPoint leftUpPos, rightDownPos;//左上角、右下角坐标
    QPoint startPos, endPos;//鼠标开始结束位置
    int maxWidth, maxHeight;//屏幕大小
    STATUS status;
    qreal devicePixRatio;// 屏幕缩放比例

    void cmpPoint(QPoint& leftTop, QPoint& rightDown);//比较两个位置坐标

};

#endif // SCREEN_H
