#ifndef CPUSPEED_H
#define CPUSPEED_H

class CPUspeed
{
public:
    CPUspeed();
    double cpuSpeed;
    double array[20];
    double HcpuSpeed;
    double LcpuSpeed;
    void getSpeed(int speed);
};

#endif // CPUSPEED_H
