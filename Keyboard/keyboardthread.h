#ifndef KEYBOARDTHREAD_H
#define KEYBOARDTHREAD_H
#include <QtCore>
#include <qstring.h>

class KeyboardThread : public QThread
{
public:
    KeyboardThread();
    void switchString(int);
    void run();
    QString keys;
};

#endif // KEYBOARDTHREAD_H
