#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <windows.h>
#define MAX 100 //arbitrary max number of keys
class Keyboard
{
public:
    struct keyData {
        int id;
        int x;
        int y;
        int width;
        int height;
    };
    Keyboard();
    int i;
    void getKeys();
    int idle;
    bool full;
    keyData keys[MAX];
};
#endif // KEYBOARD_H
