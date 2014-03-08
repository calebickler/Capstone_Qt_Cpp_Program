#ifndef CPUTEMP_H
#define CPUTEMP_H

class CPUtemp
{
public:
    CPUtemp();
    int cpuHighTemp;
    int cpu0Temp;
    int cpu1Temp;
    int cpu2Temp;
    int cpu3Temp;
    int cpu4Temp;
    int cpu5Temp;
    int HcpuTemp;
    int LcpuTemp;
    void getTemp(void);
};
#endif // CPUTEMP_H
