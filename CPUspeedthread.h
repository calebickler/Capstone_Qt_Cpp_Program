#ifndef CPUSPEEDTHREAD_H
#define CPUSPEEDTHREAD_H
#include <QtCore>
#include <windows.h>

class CPUspeedthread : public QThread
{
public:
    CPUspeedthread();
    long cpuSpeed;
    void run();
};

#endif // CPUSPEEDTHREAD_H
