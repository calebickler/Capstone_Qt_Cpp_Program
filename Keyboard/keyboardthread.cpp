#include "keyboardthread.h"
#include "keyboard.h"
#include "time.h"
#include <QDebug>
#include <windows.h>

int refresh;//refresh interval set by user
boolean timerK;
Keyboard k;
int j;

KeyboardThread::KeyboardThread()
{
    refresh = 10000;//arbitrary 10 seconds for testing
}

void KeyboardThread::run()
{
    clock_t start;
    double duration = 0;
    while(true)
    {
        qDebug() << keys;
        keys = "";
        j = 0;
        k.i = 0;
        std::fill_n(k.keys, MAX, 0);
        start = clock();
        timerK = true;
        while(timerK)
        {
           duration = (clock() - start);
           k.getKeys();
           switchString(k.keys[j]);
           if(duration > refresh)
           {
                timerK = false;
           }
        }
    }

}

void KeyboardThread::switchString(int num)
{
    if(num != 0)
    {
           keys += QString::number(num) += QString::fromStdString(", "); //writes all hex(converted to decimal) to a QString to be used by driver
           j++; //increment string number count
    }
}
