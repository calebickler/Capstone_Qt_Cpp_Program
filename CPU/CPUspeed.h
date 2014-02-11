#ifndef CPUSPEED_H
#define CPUSPEED_H
#include <stdlib.h>
class CPUspeed
{
public:
    CPUspeed();
    void getSpeed(void);
    double cpuSpeed;
private:
    double start;
    double duration;
};

#endif // CPUSPEED_H
