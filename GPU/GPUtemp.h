#ifndef GPUTEMP_H
#define GPUTEMP_H

class GPUtemp
{
public:
    GPUtemp();
    int gputemps[20];
    int numgpu;
    int HgpuTemp;
    int LgpuTemp;
    void getTemp(void);
    int gpuType;
};

#endif // GPUTEMP_H
