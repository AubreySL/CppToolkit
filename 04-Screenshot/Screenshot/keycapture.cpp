#include "keycapture.h"

KeyCapture::~KeyCapture()
{

}

void KeyCapture::setkeyValue(int key)
{
    emit getKey(key);
}

KeyCapture::KeyCapture()
{

}
