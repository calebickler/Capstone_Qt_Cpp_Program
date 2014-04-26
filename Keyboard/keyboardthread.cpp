#include "keyboardthread.h"
#include "keyboard.h"
#include "time.h"
#include <QDebug>
#include <windows.h>
#include <QGraphicsPixmapItem>

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
        keys = "";
        j = 0;
        k.i = 0;
        for (int i = 0; i < 100; i++) {
            k.keys[i].id = 0;
            k.keys[i].x = 0;
            k.keys[i].y = 0;
            k.keys[i].width = 0;
            k.keys[i].height = 0;
        }

        start = clock();
        timerK = true;
        while(timerK)
        {
           duration = (clock() - start);
           k.getKeys();
           switchString(k.keys[j].id);
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

void KeyboardThread::draw() {


    QPixmap pixmapObject("debug/keyboard.png");
    QGraphicsPixmapItem * keypic = new QGraphicsPixmapItem();
    keypic->setPixmap(pixmapObject.scaledToWidth(486,Qt::SmoothTransformation));

    QBrush brush(Qt::gray);
    QPen pen(Qt::gray);
    scene = new QGraphicsScene(0, 0, 486, 146);
    scene->addRect(0,0,486,146, pen, brush);
    QBrush Ybrush(Qt::yellow);
    QPen Ypen(Qt::yellow);
    for (int c = 0; c < k.i; c++) {
        scene->addRect(k.keys[c].x,k.keys[c].y,(k.keys[c].width - k.keys[c].x),(k.keys[c].height - k.keys[c].y), Ypen, Ybrush);
    }



    scene->addItem(keypic);
}

void KeyboardThread::setTime(int time) {
    refresh = time;
}
