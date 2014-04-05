#include "CPUspeed.h"
#include <QProcess>

CPUspeed::CPUspeed()
{
    cpuSpeed = 0;
    HcpuSpeed = 0;
    LcpuSpeed = 1000000;
}

void CPUspeed::getSpeed(void)
{
    QProcess *process = new QProcess();
    process->start("cpuSpeed.exe");
    process->waitForFinished();
    cpuSpeed = (double) process->exitCode();
    cpuSpeed = (cpuSpeed / 1000);

    if (cpuSpeed > HcpuSpeed) {
        HcpuSpeed = cpuSpeed;
    }
    if (cpuSpeed < LcpuSpeed && cpuSpeed > 0) {
        LcpuSpeed = cpuSpeed;
    }

}
