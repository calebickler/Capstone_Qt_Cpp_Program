#ifndef OHM_H
#define OHM_H

class OHM
{
public:
    OHM();
    int CPUspeed;
    int CPUtemp[20];
    void update();
};
#endif // OHM_H
