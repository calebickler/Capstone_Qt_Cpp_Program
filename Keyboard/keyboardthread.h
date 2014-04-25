#ifndef KEYBOARDTHREAD_H
#define KEYBOARDTHREAD_H
#include <QtCore>
#include <qstring.h>
#include <QGraphicsScene>

class KeyboardThread : public QThread
{
public:
    KeyboardThread();
    void switchString(int);
    void run();
    QString keys;
    void draw();
    QGraphicsScene *scene;
};

#endif // KEYBOARDTHREAD_H
