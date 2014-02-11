#include "CPUspeed.h"
#include "CPUspeedthread.h"

CPUspeedthread::CPUspeedthread()
{
    cpuSpeed = 0;
}

void CPUspeedthread::run()
{
    CPUspeed cpu;
    while(true)
    {
        cpuSpeed = 0;
        cpu.getSpeed();
        cpuSpeed = cpu.cpuSpeed;
        Sleep(60000);//once a minute
    }
}
