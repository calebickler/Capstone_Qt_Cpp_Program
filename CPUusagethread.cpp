#include "CPUusage.h"
#include "CPUusagethread.h"

CPUusagethread::CPUusagethread()
{
    cpuUsage = 0;
}

void CPUusagethread::run()
{
    CPUusage cpu;
    while(true)
    {
        cpu.getUsage();
        cpuUsage = cpu.cpuUsage;
        Sleep(500);
    }
}
