#include <stdafx.h>
#include <Windows.h>  
#include <iostream>  
#include <WbemCli.h>  

#pragma comment(lib, "wbemuuid.lib")  

int main()
{
	int cpuSpeed = 0;
	using std::cout;
	using std::cin;
	using std::endl;

	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hRes))
	{
		cout << "Unable to launch COM: 0x" << std::hex << hRes << endl;
		return 1;
	}

	if ((FAILED(hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_CONNECT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0))))
	{
		cout << "Unable to initialize security: 0x" << std::hex << hRes << endl;
		return 1;
	}

	IWbemLocator* pLocator = NULL;
	if (FAILED(hRes = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pLocator))))
	{
		cout << "Unable to create a WbemLocator: " << std::hex << hRes << endl;
		return 1;
	}

	IWbemServices* pService = NULL;
	if (FAILED(hRes = pLocator->ConnectServer(L"root\\OpenHardwareMonitor", NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, NULL, NULL, &pService)))
	{
		pLocator->Release();
		cout << "Unable to connect to \"CIMV2\": " << std::hex << hRes << endl;
		return 1;
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	if (FAILED(hRes = pService->ExecQuery(L"WQL", L"select * from Sensor where Identifier like '/intelcpu/0/clock/1'", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumerator)))
	{
		pLocator->Release();
		pService->Release();
		cout << "Unable to retrive desktop monitors: " << std::hex << hRes << endl;
		return 1;
	}

	IWbemClassObject* clsObj = NULL;
	int numElems;
	while ((hRes = pEnumerator->Next(WBEM_INFINITE, 1, &clsObj, (ULONG*)&numElems)) != WBEM_S_FALSE)
	{
		if (FAILED(hRes)){
			break;
		}

		VARIANT vRet;
		VariantInit(&vRet);
		if (SUCCEEDED(clsObj->Get(L"Value", 0, &vRet, NULL, NULL)))
		{
			cpuSpeed = vRet.fltVal;
			VariantClear(&vRet);
		}
		clsObj->Release();
	}

	pEnumerator->Release();
	pService->Release();
	pLocator->Release();

	return cpuSpeed;   // Program successfully completed.
}