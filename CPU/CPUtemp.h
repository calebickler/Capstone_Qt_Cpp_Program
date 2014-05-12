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
    int cpu6Temp;
    int cpu7Temp;
    int cpu8Temp;
    int cpu9Temp;
    int HcpuTemp;
    int LcpuTemp;
    double array[20];
    void getTemp(int temps[10]);
};
#endif // CPUTEMP_H
