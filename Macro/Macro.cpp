
#include "Macro.h"
#include <QGraphicsTextItem>
#include <QDebug>

Macro::Macro() {

}

void Macro::draw(QGraphicsScene* scene, QColor background, QColor font) {
    //size 241, 281
    //QBrush Ybrush(Qt::blue);
    //scene->setBackgroundBrush(Ybrush);

    QFont titleFont("Times", 10, QFont::Bold);
    QGraphicsTextItem * titleItem = new QGraphicsTextItem;
    titleItem->setPos(70,10);
    titleItem->setDefaultTextColor(font);
    titleItem->setFont(titleFont);
    titleItem->setPlainText("Macro Recorder");
    scene->addItem(titleItem);

    QFont reckFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * reckItem = new QGraphicsTextItem;
    reckItem->setPos(30,40);
    reckItem->setDefaultTextColor(font);
    reckItem->setFont(reckFont);
    reckItem->setPlainText("Record Key");
    scene->addItem(reckItem);

    QFont actFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * actItem = new QGraphicsTextItem;
    actItem->setPos(30,60);
    actItem->setDefaultTextColor(font);
    actItem->setFont(actFont);
    actItem->setPlainText("Activation Key");
    scene->addItem(actItem);

    QBrush onB(font);
    QPen onP(font);
    scene->addRect(160,150,40,40, onP, onB);
    scene->addRect(160,200,40,40, onP, onB);

    QFont onFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * onItem = new QGraphicsTextItem;
    onItem->setPos(168,160);
    onItem->setDefaultTextColor(Qt::black);
    onItem->setFont(onFont);
    onItem->setPlainText("On");
    scene->addItem(onItem);

    QFont offFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * offItem = new QGraphicsTextItem;
    offItem->setPos(168,210);
    offItem->setDefaultTextColor(Qt::black);
    offItem->setFont(offFont);
    offItem->setPlainText("Off");
    scene->addItem(offItem);


    QBrush recB(Qt::red);
    QPen recP(Qt::red);
    scene->addRect(100,100,20,20, recP, recB);

    QFont recFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * recItem = new QGraphicsTextItem;
    recItem->setPos(96,100);
    recItem->setDefaultTextColor(Qt::black);
    recItem->setFont(recFont);
    recItem->setPlainText("Rec");
    scene->addItem(recItem);

}

void Macro::recHit() {
    qDebug() << "Record";
}

void Macro::onHit() {
    qDebug() << "On";
}

void Macro::offHit() {
    qDebug() << "Off";
}
