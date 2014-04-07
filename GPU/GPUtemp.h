#ifndef GPUTEMP_H
#define GPUTEMP_H

class GPUtemp
{
public:
    GPUtemp();
    int gputemp1;
    int gputemp2;
    int gputemp3;
    int numgpu;
    int HgpuTemp;
    int LgpuTemp;
    void getTemp(void);
};

#endif // GPUTEMP_H
