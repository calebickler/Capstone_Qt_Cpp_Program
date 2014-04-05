#include "CPUtemp.h"
#include <QProcess>

int numcores;

CPUtemp::CPUtemp(){
    cpuHighTemp = 0;
    cpu0Temp = 0;
    cpu1Temp = 0;
    cpu2Temp = 0;
    cpu3Temp = 0;
    cpu4Temp = 0;
    cpu5Temp = 0;
    HcpuTemp = 0;
    LcpuTemp = 0;
    numcores = 8;
}

void CPUtemp::getTemp(void) {
    QProcess *process = new QProcess();
    process->start("cpuTemp.exe");
    process->waitForFinished();
    int cpuTemp = process->exitCode();

    //get bits into correct variables
    cpu0Temp = cpuTemp >> 24;
    cpuTemp = cpuTemp & 16777215;
    cpu1Temp = cpuTemp >> 16;
    cpuTemp = cpuTemp & 65535;
    cpu2Temp = cpuTemp >> 8;
    cpuTemp = cpuTemp & 255;
    cpu3Temp = cpuTemp;

    //remove package reading (last reading)
    if (cpu2Temp == 0) {
        cpu1Temp = 0;
        numcores = 1;
    }
    if (cpu3Temp == 0) {
        cpu2Temp = 0;
        numcores = 2;
    }

    if (numcores < 4) {
        QProcess *process2 = new QProcess();
        process2->start("cpuTemp2.exe");
        process2->waitForFinished();
        int cpuTemp2 = process2->exitCode();

        int read7;
        cpu4Temp = cpuTemp2 >> 24;
        cpuTemp2 = cpuTemp2 & 16777215;
        cpu5Temp = cpuTemp2 >> 16;
        cpuTemp2 = cpuTemp2 & 65535;
        read7 = cpuTemp2 >> 8;

        if (cpu4Temp == 0) {
            cpu3Temp = 0;
            numcores = 3;
        }
        if (cpu5Temp == 0) {
            cpu4Temp = 0;
            numcores = 4;
        }
        if (read7 == 0) {
            cpu5Temp = 0;
            numcores = 5;
        }
    }

    //get highest reading

    cpuHighTemp = cpu0Temp;
    if (cpuHighTemp < cpu1Temp) {
        cpuHighTemp = cpu1Temp;
    }

    if (cpuHighTemp < cpu2Temp) {
        cpuHighTemp = cpu2Temp;
    }

    if (cpuHighTemp < cpu3Temp) {
        cpuHighTemp = cpu3Temp;
    }

    if (cpuHighTemp < cpu4Temp) {
        cpuHighTemp = cpu4Temp;
    }

    if (cpuHighTemp < cpu5Temp) {
        cpuHighTemp = cpu5Temp;
    }

    //set highlow of high
    if (cpuHighTemp > HcpuTemp) {
        HcpuTemp = cpuHighTemp;
    }
    if (cpuHighTemp < LcpuTemp || LcpuTemp == 0) {
        LcpuTemp = cpuHighTemp;
    }
}
