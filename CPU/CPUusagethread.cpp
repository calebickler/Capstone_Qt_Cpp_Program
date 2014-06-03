#include "CPUusage.h"
#include "CPUusagethread.h"

CPUusagethread::CPUusagethread()
{
    cpuUsage = 0;
    HcpuUse = 0;
    LcpuUse = 1000;
}

void CPUusagethread::run()
{
    CPUusage cpu;
    while(true)
    {
        cpu.getUsage();
        cpuUsage = (int)cpu.cpuUsage + .5;
        if (cpuUsage > HcpuUse) {
            HcpuUse = cpuUsage;
        }
        if (cpuUsage < LcpuUse) {
            LcpuUse = cpuUsage;
        }
        array[cu] = cpuUsage;
        if(cu == 9)
        {
            cu = 0;
        }
        else
        {
            cu++;
        }
        Sleep(500);
    }
}
