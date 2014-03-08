#ifndef GPUTEMP_H
#define GPUTEMP_H

class GPUtemp
{
public:
    GPUtemp();
    int gputemp;
    int HgpuTemp;
    int LgpuTemp;
    void getTemp(void);
};

#endif // GPUTEMP_H
