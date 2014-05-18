#ifndef MACRO_H
#define MACRO_H

#include <QGraphicsScene>

class Macro
{
public:
    Macro();
    void draw(QGraphicsScene* scene, QColor background, QColor font);
    void recHit();
    void onHit();
    void offHit();
};

#endif // MACRO_H
