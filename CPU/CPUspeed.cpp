#include "CPUspeed.h"
#include <QProcess>
#include <QTcpSocket>

CPUspeed::CPUspeed()
{
    cpuSpeed = 0;
    HcpuSpeed = 0;
    LcpuSpeed = 1000000;
}

void CPUspeed::getSpeed(int speed)
{
    double dSpeed = speed;
    cpuSpeed = (dSpeed / 1000);

    if (cpuSpeed > HcpuSpeed) {
        HcpuSpeed = cpuSpeed;
    }
    if (cpuSpeed < LcpuSpeed && cpuSpeed > 0) {
        LcpuSpeed = cpuSpeed;
    }
}
