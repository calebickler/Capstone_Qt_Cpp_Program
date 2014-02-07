#include "Memory.h"

Memory::Memory(){
    memoryUsage = 0;
}

void Memory::getUsage(void)
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    memoryUsage = status.dwMemoryLoad;
}
