#include "keyboardthread.h"
#include "keyboard.h"
#include "time.h"
#include <QDebug>
#include <windows.h>
#include <QGraphicsPixmapItem>
#include <QBitmap>
#include <QPainter>

int refresh;//refresh interval set by user
boolean timerK;
Keyboard k;
int j;
QColor highlight, lines;
int hfromset;

KeyboardThread::KeyboardThread()
{
    refresh = 10000;//arbitrary 10 seconds for testing
    hfromset = 0;
    lines.setRgb(70,255,69);

}

void KeyboardThread::run()
{
    //clock_t start;
    //double duration = 0;
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

        //start = clock();
        timerK = true;
        Sleep(1000);
        while(timerK)
        {
           //duration = (clock() - start);
           k.getKeys();
           //switchString(k.keys[j].id);
           if(k.idle > 5000)
           {
                timerK = false;
                k.idle = 0;
                k.full = false;
           }
           //qDebug() << keys;
        }
    }
}

bool KeyboardThread::checkKeys(int key)
{
    int i;
    for(i = 0; i < MAX; i++)
    {
        if(k.keys[i].id == key)
        {
            return 1;
        }
    }
    return 0;
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
    QBitmap mask = pixmapObject.createMaskFromColor(Qt::black,Qt::MaskOutColor);
    QPainter p(&pixmapObject);
    p.setBackgroundMode(Qt::TransparentMode);
    p.setPen(lines);
    p.drawPixmap(pixmapObject.rect(),mask,pixmapObject.rect());
    p.end();
    keypic->setPixmap(pixmapObject.scaledToWidth(486,Qt::SmoothTransformation));
    scene = new QGraphicsScene(0, 0, 486, 146);
    //scene->addRect(0,0,486,146, pen, brush);
    QBrush Ybrush(Qt::yellow);
    QPen Ypen(Qt::yellow);
    if(hfromset)
    {
        Ybrush.setColor(highlight);
        Ypen.setColor(highlight);
    }
    if(k.full)
        for (int c = 0; c < 21; c++) {
            scene->addRect(k.keys[c].x,k.keys[c].y,(k.keys[c].width - k.keys[c].x),(k.keys[c].height - k.keys[c].y), Ypen, Ybrush);
        }
    else
        for (int c = 0; c < k.i; c++) {
            scene->addRect(k.keys[c].x,k.keys[c].y,(k.keys[c].width - k.keys[c].x),(k.keys[c].height - k.keys[c].y), Ypen, Ybrush);
        }



    scene->addItem(keypic);
}

void KeyboardThread::setHighlight(QColor color)
{
    highlight = color;
}
void KeyboardThread::setLineColor(QColor color)
{
    lines = color;
}


void KeyboardThread::setTime(int time) {
    refresh = time;
}
