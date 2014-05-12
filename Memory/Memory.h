#ifndef MEMORY_H
#define MEMORY_H

#include <windows.h>
#include <winperf.h>
#include <stdlib.h>


class Memory{
public:
    Memory();
    long memoryUsage;
    int HmemUsage;
    int LmemUsage;
    double array[20];
    void getUsage(void);
};
#endif // MEMORY_H
