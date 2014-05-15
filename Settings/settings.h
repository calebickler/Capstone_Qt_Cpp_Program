#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    Settings();
    bool cpuUse, cpuTemp, cpuSpeed, gpuTemp, memUse, cpuCoreTemp, collecting, HLcpuUse,
        HLmemUsage, HLcpuSpeed, HLcpuTemp, HLgpuTemp, Keyboard, memGraph, CPUUseGraph, CPUSpeedGraph, CPUTempGraph, GPUTempGraph, updated, macro;
    long refresh;
};

#endif // SETTINGS_H
