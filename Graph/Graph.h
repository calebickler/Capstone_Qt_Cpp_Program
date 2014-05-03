#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsScene>

class Graph
{
public:
    Graph(QString title, int minIn, int maxIn);
    void draw(double* dataArray, int numDisplayed, int index, Qt::GlobalColor grid, Qt::GlobalColor line);
    QGraphicsScene *scene;
};

#endif // GRAPH_H
