#ifndef CPUUSAGETHREAD_H
#define CPUUSAGETHREAD_H
#include <QtCore>

class CPUusagethread : public QThread
{
public:
    CPUusagethread();
    long cpuUsage;
    void run();
};

#endif // MYTHREAD_H
