#ifndef CPUUSAGE_H
#define CPUUSAGE_H

#include <windows.h>
#include <winperf.h>
#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <sigar.h>
extern "C"{
#include <sigar_format.h>
}

class CPUusage{

public:
    CPUusage();
    double cpuUsage;
    void getUsage(void);
};

#endif // CPUUSAGE_H
