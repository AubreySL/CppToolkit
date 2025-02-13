#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include "screen.h"

#include <QWidget>
#include <QPixmap>
class ScreenWidget : public QWidget
{
    Q_OBJECT

public:
    static ScreenWidget* instance();
    explicit ScreenWidget(QWidget *parent = nullptr);

public slots:
    void receivePix(QPixmap pix);
    void saveScreen();
    void saveFullScreen();
    void saveScreenOther();
    void saveFullOther();
    void hideScreen();


private:
    static QScopedPointer<ScreenWidget> self;

    QPixmap windowPix;//全屏
    QMenu* menu;//右键菜单对象
    Screen* screen;//截屏对象
    QPixmap* fullScreen;
    QPixmap* bgScreen;
    QPoint movPos;


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void showEvent(QShowEvent *event) override;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // SCREENWIDGET_H
