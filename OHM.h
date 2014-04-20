#ifndef OHM_H
#define OHM_H

class OHM
{
public:
    OHM();
    bool loaded;
    int CPUspeed;
    int CPUtemp[10];
    void update();
};
#endif // OHM_H
