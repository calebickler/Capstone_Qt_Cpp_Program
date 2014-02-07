#ifndef CPUSPEED_H
#define CPUSPEED_H
#include <stdlib.h>
#include <time.h>
class CPUspeed
{
public:
    CPUspeed();
    void getSpeed(void);
    double cpuSpeed;
private:
    clock_t start;
    double duration;
};

#endif // CPUSPEED_H
