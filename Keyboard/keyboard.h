#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QGraphicsScene>
#include <windows.h>
#define MAX 100 //arbitrary max number of keys
class Keyboard
{
public:
    Keyboard();
    int i;
    void getKeys();
    int keys[MAX];
};
#endif // KEYBOARD_H
