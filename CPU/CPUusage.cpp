#include "CPUusage.h"

CPUusage::CPUusage(){
    cpuUsage = 0;
}

void CPUusage::getUsage(void)
{
    cpuLast = cpuUsage;
    double x = 0;
    double high = 0;
    sigar_t *sigar_cpu;
    sigar_cpu_t old;
    sigar_cpu_t current;

    sigar_open(&sigar_cpu);
    sigar_cpu_get(sigar_cpu, &old);

    sigar_cpu_perc_t perc;
    int i = 0;
    while(high == 0)
    {
        sigar_cpu_get(sigar_cpu, &current);
        sigar_cpu_perc_calculate(&old, &current, &perc);

        x = perc.combined * 100;
        old = current;
        Sleep(500);
        if(high < x)
            high = x;
        i++;
    }
    cpuUsage = high;
    sigar_close(sigar_cpu);
}
