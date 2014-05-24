
#include "Graph.h"
#include <QGraphicsTextItem>
#include <QDebug>

Graph::Graph() {
}

void Graph::draw(QGraphicsScene* scene, double* dataArray, int numDisplayed, int index, QColor grid, QColor line, QString title, int min, int max) {
    //QGraphicsScene* scene = new QGraphicsScene(0, 0, 371, 271);
    //labels
    scene->setBackgroundBrush(QBrush(Qt::transparent));

    QFont titleFont("Times", 10, QFont::Bold);
    QGraphicsTextItem * titleItem = new QGraphicsTextItem;
    titleItem->setPos(150,10);
    titleItem->setDefaultTextColor(grid);
    titleItem->setFont(titleFont);
    titleItem->setPlainText(title);
    scene->addItem(titleItem);

    QFont maxFont("Times", 7, QFont::Bold);
    QGraphicsTextItem * maxItem = new QGraphicsTextItem;
    maxItem->setPos(3,35);
    maxItem->setDefaultTextColor(grid);
    maxItem->setFont(maxFont);
    maxItem->setPlainText(QString::number(max));
    scene->addItem(maxItem);

    QFont midFont("Times", 7, QFont::Bold);
    QGraphicsTextItem * midItem = new QGraphicsTextItem;
    midItem->setPos(3,140);
    midItem->setDefaultTextColor(grid);
    midItem->setFont(midFont);
    midItem->setPlainText(QString::number((max / 2)));
    scene->addItem(midItem);

    QFont minFont("Times", 7, QFont::Bold);
    QGraphicsTextItem * minItem = new QGraphicsTextItem;
    minItem->setPos(3,247);
    minItem->setDefaultTextColor(grid);
    minItem->setFont(minFont);
    minItem->setPlainText(QString::number(min));
    scene->addItem(minItem);

    //grid
    QPen linepen(grid);
    linepen.setWidth(2);
    scene->addLine(30,40,30,260, linepen);
    scene->addLine(65,40,65,260, linepen);
    scene->addLine(100,40,100,260, linepen);
    scene->addLine(135,40,135,260, linepen);
    scene->addLine(170,40,170,260, linepen);
    scene->addLine(205,40,205,260, linepen);
    scene->addLine(240,40,240,260, linepen);
    scene->addLine(275,40,275,260, linepen);
    scene->addLine(310,40,310,260, linepen);
    scene->addLine(345,40,345,260, linepen);

    scene->addLine(30,40,345,40, linepen);
    scene->addLine(30,84,345,84, linepen);
    scene->addLine(30,128,345,128, linepen);
    scene->addLine(30,172,345,172, linepen);
    scene->addLine(30,216,345,216, linepen);
    scene->addLine(30,260,345,260, linepen);

    //draw graph lines

    QPen graphPen(line);
    graphPen.setWidth(2);
    double data[numDisplayed];

    if (index == 0) {
        index = (numDisplayed - 1);
    }
    else {
        index --;
    }


    for (int i = 0; i < numDisplayed; i++) {
        data[i] = (((1 - (dataArray[index] / max))* (260-40)) + 40);
        if (data[i] > 260) {
            data[i] = 260;
        }
        if (data[i] < 40) {
            data[i] = 40;
        }
        if ((index) == 0) {
            index = (numDisplayed - 1);
        }
        else {
            index--;
        }
    }

    int xAdd = (345 - 30) / (numDisplayed - 1);
    int xDraw = 30;

    for (int i = 0; i < (numDisplayed - 1); i++) {
        if (data[i+1] != -7777) {
            scene->addLine(xDraw, data[i], (xDraw + xAdd), data[i+1], graphPen);
        }
        xDraw = (xDraw + xAdd);
    }
}
