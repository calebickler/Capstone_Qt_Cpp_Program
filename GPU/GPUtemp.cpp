#include "GPUtemp.h"
#include "stdlib.h"
#include "stdio.h"
#include "Libraries/Nvidia/nvapi.h"
#include <QDebug>

int gputemp1;
int gputemp2;
int gputemp3;
int numgpu;

GPUtemp::GPUtemp()
{
    gputemp1 = 0;
    gputemp2 = 0;
    gputemp3 = 0;
    numgpu;
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
   if(cnt > 0)
   {
       gputemp1 = thermal.sensor[0].currentTemp;
       if(cnt > 1)
       {
           gputemp2 = thermal.sensor[1].currentTemp;
           if(cnt > 2)
                gputemp3 = thermal.sensor[2].currentTemp;
       }
   }

   if (gputemp1 > HgpuTemp) {
       HgpuTemp = gputemp1;
   }
   if (gputemp1 < LgpuTemp) {
       LgpuTemp = gputemp1;
   }
}
