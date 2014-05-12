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
    double array[20];
    void getTemp(void);
    int gpuType;
};

#endif // GPUTEMP_H
