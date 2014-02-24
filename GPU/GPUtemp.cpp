#include "GPUtemp.h"
#include "stdlib.h"
#include "stdio.h"
#include "Libraries/Nvidia/nvapi.h"

int gputemp;

GPUtemp::GPUtemp()
{
    gputemp = 0;
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

       gputemp = thermal.sensor[0].currentTemp;

}
