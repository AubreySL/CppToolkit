#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    void receivePix(QPixmap pix);

private:
    QPixmap windowPix;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif // WIDGET_H
