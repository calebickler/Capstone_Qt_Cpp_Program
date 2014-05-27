#include "CPUtemp.h"
#include <QProcess>


CPUtemp::CPUtemp(){
    cpuHighTemp = 0;
    cpu0Temp = 0;
    cpu1Temp = 0;
    cpu2Temp = 0;
    cpu3Temp = 0;
    cpu4Temp = 0;
    cpu5Temp = 0;
    cpu6Temp = 0;
    cpu7Temp = 0;
    cpu8Temp = 0;
    cpu9Temp = 0;
}

void CPUtemp::getTemp(int temps[10]) {
    cpu0Temp = temps[0];
    cpu1Temp = temps[1];
    cpu2Temp = temps[2];
    cpu3Temp = temps[3];
    cpu4Temp = temps[4];
    cpu5Temp = temps[5];
    cpu6Temp = temps[6];
    cpu7Temp = temps[7];
    cpu8Temp = temps[8];
    cpu9Temp = temps[9];

    if (cpu2Temp == 0) {
        cpu1Temp = 0;
    }
    if (cpu3Temp == 0) {
        cpu2Temp = 0;
    }
    if (cpu4Temp == 0) {
        cpu3Temp = 0;
    }
    if (cpu5Temp == 0) {
        cpu4Temp = 0;
    }
    if (cpu6Temp == 0) {
        cpu5Temp = 0;
    }
    if (cpu7Temp == 0) {
        cpu6Temp = 0;
    }
    if (cpu8Temp == 0) {
        cpu7Temp = 0;
    }
    if (cpu9Temp == 0) {
        cpu8Temp = 0;
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
    if (cpuHighTemp < cpu6Temp) {
        cpuHighTemp = cpu6Temp;
    }
    if (cpuHighTemp < cpu7Temp) {
        cpuHighTemp = cpu7Temp;
    }
    if (cpuHighTemp < cpu8Temp) {
        cpuHighTemp = cpu8Temp;
    }
    if (cpuHighTemp < cpu9Temp) {
        cpuHighTemp = cpu9Temp;
    }

    //set highlow of high
    if (cpuHighTemp > HcpuTemp) {
        HcpuTemp = cpuHighTemp;
    }
    if (cpuHighTemp < LcpuTemp || LcpuTemp == 0) {
        LcpuTemp = cpuHighTemp;
    }
    array[ct] = cpuHighTemp;
    if(ct == 9)
    {
        ct = 0;
    }
    else
    {
        ct++;
    }
}
