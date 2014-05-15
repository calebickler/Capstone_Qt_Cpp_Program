#include "settings.h"

Settings::Settings()
{
    cpuSpeed = 1;
    cpuUse = 1;
    cpuTemp = 1;
    cpuCoreTemp = 1;
    gpuTemp = 1;
    memUse = 1;
    collecting = 1;
    HLcpuUse = 1;
    HLmemUsage = 1;
    HLcpuSpeed = 1;
    HLcpuTemp = 1;
    HLgpuTemp = 1;
    Keyboard = 1;
    memGraph = 1;
    CPUUseGraph = 1;
    CPUSpeedGraph = 1;
    CPUTempGraph = 1;
    GPUTempGraph = 1;
    updated = 0;
    refresh = 500;
    macro = 1;
}
