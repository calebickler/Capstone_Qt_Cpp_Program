#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsScene>

class Graph
{
public:
    Graph();
    void draw(QGraphicsScene* scene, double* dataArray, int numDisplayed, int index, QColor grid, QColor line, QString title, int min, int max);
};

#endif // GRAPH_H
