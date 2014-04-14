#include "GPUtemp.h"
#include "stdlib.h"
#include "stdio.h"
#include "Libraries/Nvidia/nvapi.h"
#include "Libraries/AMD/include/adl_defines.h"
#include "Libraries/AMD/include/adl_sdk.h"
#include "Libraries/AMD/include/adl_structures.h"
#include <QDebug>

int gputemps;
int numgpu;
int gpuType;
ADLTemperature adlt;

GPUtemp::GPUtemp()
{
    gputemps[0] = 0;
    numgpu = 0;
    HgpuTemp = 0;
    LgpuTemp = 1000;
}

void GPUtemp::getTemp(void)
{

       NvAPI_Status ret = NVAPI_OK;

       NvDisplayHandle hDisplay_a[NVAPI_MAX_PHYSICAL_GPUS*2] = {0};

       ret = NvAPI_Initialize();

       NvU32 cnt;

       NvPhysicalGpuHandle phys;

       ret = NvAPI_EnumPhysicalGPUs(&phys, &cnt);

       NvAPI_ShortString name;

       NV_GPU_THERMAL_SETTINGS thermal;

       ret = NvAPI_GPU_GetFullName(phys, name);

       thermal.version =NV_GPU_THERMAL_SETTINGS_VER;
       ret = NvAPI_GPU_GetThermalSettings(phys,0, &thermal);
       numgpu = cnt;
       if (cnt > 0)
           gpuType = 0;
       else if(adlt.iTemperature < 999 && adlt.iTemperature > 0)
           gpuType = 1;
       else
           gpuType = 2;
   if(gpuType == 0)
   {
       for(int i = 0; i < cnt ;i++)
       {
           gputemps[i] = thermal.sensor[i].currentTemp;
       }
    }
    else if(gpuType == 1)
    {
        gputemps[0] = adlt.iTemperature;
    }
    else
    {
        gputemps[0] = 9999;
    }

   if (gputemps[0] > HgpuTemp) {
       HgpuTemp = gputemps[0];
   }
   if (gputemps[0] < LgpuTemp) {
       LgpuTemp = gputemps[0];
   }
}
