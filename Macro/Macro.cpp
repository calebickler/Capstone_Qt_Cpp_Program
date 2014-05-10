
#include "Macro.h"
#include <QGraphicsTextItem>
#include <QDebug>

Macro::Macro() {

}

void Macro::draw(QGraphicsScene* scene, QColor background, QColor font) {
    //size 241, 281
    QFont titleFont("Times", 10, QFont::Bold);
    QGraphicsTextItem * titleItem = new QGraphicsTextItem;
    titleItem->setPos(70,10);
    titleItem->setDefaultTextColor(font);
    titleItem->setFont(titleFont);
    titleItem->setPlainText("Macro Recorder");
    scene->addItem(titleItem);

    QFont recFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * recItem = new QGraphicsTextItem;
    recItem->setPos(30,40);
    recItem->setDefaultTextColor(font);
    recItem->setFont(recFont);
    recItem->setPlainText("Record Key");
    scene->addItem(recItem);

    QFont actFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * actItem = new QGraphicsTextItem;
    actItem->setPos(30,60);
    actItem->setDefaultTextColor(font);
    actItem->setFont(actFont);
    actItem->setPlainText("Activation Key");
    scene->addItem(actItem);

}
