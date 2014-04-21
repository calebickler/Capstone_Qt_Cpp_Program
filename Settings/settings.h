#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    Settings();
    bool cpuUse, cpuTemp, cpuSpeed, gpuTemp, memUse, cpuCoreTemp, collecting, HLcpuUse, HLmemUsage, HLcpuSpeed, HLcpuTemp, HLgpuTemp, Keyboard, updated;
    long refresh;
};

#endif // SETTINGS_H
