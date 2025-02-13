#include "screenwidget.h"
#include <QMenu>
#include <QMutex>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QFileDialog>
#include <QMouseEvent>
#include <QGuiApplication>
#include <QKeyEvent>
#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

QScopedPointer<ScreenWidget> ScreenWidget::self;
ScreenWidget *ScreenWidget::instance()
{
    if(self.isNull())
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(self.isNull()){
            self.reset(new ScreenWidget);
        }
    }
    return self.data();
}

ScreenWidget::ScreenWidget(QWidget *parent)
    : QWidget(parent)
{
    menu = new QMenu(this);
    menu->addAction("保存当前截图 Enter", this, SLOT(saveScreen()));
    menu->addAction("保存全屏截图", this, SLOT(saveFullScreen()));
    menu->addAction("截图另存为",this, SLOT(saveScreenOther()));
    menu->addAction("全屏另存为", this, SLOT(saveFullOther()));
    menu->addAction("取消截图 ESC", this, SLOT(hide()));

    // qDebug()<<qApp->devicePixelRatio();
    // qDebug()<<QGuiApplication::primaryScreen()->size();
    // qDebug()<<qApp->primaryScreen()->size();
    // qDebug()<<qApp->primaryScreen()->geometry();
    // qDebug()<<QGuiApplication::primaryScreen()->availableGeometry();
    // qDebug()<<QGuiApplication::primaryScreen()->availableSize();
    // qDebug()<<QGuiApplication::primaryScreen()->geometry();
    //取得屏幕大小
    screen = new Screen(QGuiApplication::primaryScreen()->size(), qApp->devicePixelRatio());
    //全屏
    fullScreen = new QPixmap();

}


void ScreenWidget::receivePix(QPixmap pix)
{
    this->windowPix = pix;
    this->show();
}

void ScreenWidget::saveScreen()
{
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;

    // 计算实际截取的物理尺寸
    const int pixelDensity = screen->getDPI();
    int actualWidth = w * pixelDensity;
    int actualHeight = h * pixelDensity;
    int realX = x * pixelDensity;
    int realY = y * pixelDensity;

    QString filename = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
    fullScreen->copy(realX,realY,actualWidth,actualHeight).save(filename, "png");
    hide();
}

void ScreenWidget::saveFullScreen()
{
    QString filename = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
    fullScreen->save(filename, "png");
    hide();
}

void ScreenWidget::saveScreenOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
    if(!fileName.endsWith(".png"))
    {
        fileName +=".png";
    }
    if(fileName.length() > 0)
    {
        int x = screen->getLeftUp().x();
        int y = screen->getLeftUp().y();
        int w = screen->getRightDown().x() - x;
        int h = screen->getRightDown().y() - y;

        // 计算实际截取的物理尺寸
        const int pixelDensity = screen->getDPI();
        int actualWidth = w * pixelDensity;
        int actualHeight = h * pixelDensity;
        int realX = x * pixelDensity;
        int realY = y * pixelDensity;

        fullScreen->copy(realX,realY,actualWidth,actualHeight).save(fileName, "png");
        hide();
    }

}

void ScreenWidget::saveFullOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
    if(!fileName.endsWith(".png"))
    {
        fileName +=".png";
    }
    if(fileName.length() > 0)
    {

        fullScreen->save(fileName, "png");
        hide();
    }
}

void ScreenWidget::hideScreen()
{
    hide();
}


void ScreenWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;

    QPen pen;
    pen.setColor(QColor(201,22,35));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *bgScreen);

    if(w!=0 && h!=0)
    {
        // 计算实际截取的物理尺寸
        const int pixelDensity = screen->getDPI();
        int actualWidth = w * pixelDensity;
        int actualHeight = h * pixelDensity;
        int realX = x * pixelDensity;
        int realY = y * pixelDensity;
        painter.drawPixmap(x, y , fullScreen->copy(realX, realY, actualWidth , actualHeight));
    }

    painter.drawRect(x, y , w, h);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                                       .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}


void ScreenWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event->pos();
    int status = screen->getStatus();

    if(status == Screen::SELECT)
    {
        screen->setStart(p);
    }
    else if(status == Screen::MOV)
    {
        if(screen->isInArea(p) == false)
        {
            screen->setStart(p);
            screen->setStatus(Screen::SELECT);
        } else {
            movPos = p;
            this->setCursor(Qt::SizeAllCursor);
        }
    }
    this->update();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(screen->getStatus() == Screen::SELECT)
    {
        screen->setStatus(Screen::MOV);
    } else if(screen->getStatus() == Screen::MOV){
        this->setCursor(Qt::ArrowCursor);
    }
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(screen->getStatus() == Screen::SELECT)
    {
        screen->setEnd(event->pos());
    }
    else if(screen->getStatus() == Screen::MOV)
    {
        QPoint p(event->pos().x() - movPos.x(), event->pos().y() - movPos.y());
        screen->move(p);
        movPos = event->pos();
    }
    this->update();
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    menu->exec(cursor().pos());
}

void ScreenWidget::showEvent(QShowEvent *event)
{
    qDebug()<<"showEvent";
    QPoint point(-1, -1);
    screen->setStart(point);
    screen->setEnd(point);

    QScreen* primaryScreen = qApp->primaryScreen();
    //QWidget::winId()
    *fullScreen = primaryScreen->grabWindow(0);

    //模糊背景
    QPixmap pix(screen->getMaxWidth(), screen->getMaxHeight());
    pix.fill(QColor(160,160,160,200));
    bgScreen = new QPixmap(*fullScreen);
    QPainter p(bgScreen);
    p.drawPixmap(0,0,pix);

}


void ScreenWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        this->saveScreen();
    }
}
