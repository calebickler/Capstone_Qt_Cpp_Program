#ifndef CPUUSAGETHREAD_H
#define CPUUSAGETHREAD_H
#include <QtCore>

class CPUusagethread : public QThread
{
public:
    CPUusagethread();
    int cpuUsage;
    int HcpuUse;
    int LcpuUse;
    void run();
};

#endif // MYTHREAD_H
