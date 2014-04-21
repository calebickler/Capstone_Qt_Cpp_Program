#include "keyboardg.h"

keyboardg::keyboardg() {

}

void keyboardg::update() {
    QBrush redBrush(Qt::gray);
    QPen blackPen(Qt::gray);
    scene = new QGraphicsScene(0, 0, 365, 175);
    scene->addRect(10,10,345,155, blackPen, redBrush);
}
