#include "CPUspeed.h"
#include <QDebug.h>
#include <windows.h>
#define MHZ 1000
#define GHZ 1000
#define TIMESLICE MHZ*GHZ

CPUspeed::CPUspeed()
{
        cpuSpeed = 0;
}

void CPUspeed::getSpeed(void)
{
    int i = 0;
    start = clock();//start clock
    while(i<TIMESLICE){i++;}//1Ghz worth of calculations
    duration = clock() - start;//number of clocks it took to do calculations
    cpuSpeed = duration/CLOCKS_PER_SEC;//clocks/second

}
