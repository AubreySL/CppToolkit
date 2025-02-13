#ifndef KEYCAPTURE_H
#define KEYCAPTURE_H

#include <QObject>
enum keycode {
    ESC=27, F1=112, F2, F3, F4, F5,F6,F7,F8
};

class KeyCapture : public QObject
{
    Q_OBJECT
public:
    virtual ~KeyCapture();
    static KeyCapture*& instance()
    {
        static KeyCapture *s = nullptr;
        if(s == nullptr)
        {
            s = new KeyCapture();
        }
        return s;
    }
public:
    void setkeyValue(int key);

protected:
    KeyCapture();

signals:
    void getKey(int);

private:

};

#endif // KEYCAPTURE_H
