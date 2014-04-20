// WMIserver.cpp : main project file.

#include "stdafx.h"

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib") 

using namespace System;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Text;
using namespace System::Threading;


HRESULT hRes;
IWbemLocator* pLocator;
IWbemServices* pService;

void init();
int getSpeed();
void getTemp();
void listenPort(int port);

int temps[10];
boolean tempIntel;
boolean speedIntel;

public ref class Threaded
{
public:
	int port;
	Threaded(int portIn) {
		port = portIn;
	}
	void listenPort() {
		try
		{
			IPAddress^ localAddr = IPAddress::Parse("127.0.0.1");

			TcpListener^ server = gcnew TcpListener(localAddr, port);

			server->Start();

			array<Byte>^bytes = gcnew array<Byte>(256);
			String^ data = nullptr;

			while (true)
			{

				TcpClient^ client = server->AcceptTcpClient();

				data = nullptr;


				NetworkStream^ stream = client->GetStream();
				Int32 i;


				while (i = stream->Read(bytes, 0, bytes->Length))
				{


					data = Text::Encoding::ASCII->GetString(bytes, 0, i);


					getTemp();

					array<Byte>^msg;

					msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(getSpeed()) + "    ");
					stream->Write(msg, 0, 4);


					msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(" " + temps[0]) + "    ");
					stream->Write(msg, 0, 4);

					for (int i = 1; i < 10; i++) {
						if (temps[i] > 0) {
							msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(" " + temps[i]) + "    ");
							stream->Write(msg, 0, 4);
						}
					}
				}
				client->Close();
			}
		}
		catch (Exception^ e)
		{
			exit(1);
		}
	}
};

int main(array<System::String ^> ^args)
{
	FreeConsole();
	init();

	Threaded^ threadWork = gcnew Threaded(10540);
	Thread^ oThread = gcnew Thread(gcnew ThreadStart(threadWork, &Threaded::listenPort));
	oThread->Start();

	Threaded^ threadWork2 = gcnew Threaded(10541);
	Thread^ oThread2 = gcnew Thread(gcnew ThreadStart(threadWork2, &Threaded::listenPort));
	oThread2->Start();

	Threaded^ threadWork3 = gcnew Threaded(10542);
	Thread^ oThread3 = gcnew Thread(gcnew ThreadStart(threadWork3, &Threaded::listenPort));
	oThread3->Start();

	Threaded^ threadWork4 = gcnew Threaded(10543);
	Thread^ oThread4 = gcnew Thread(gcnew ThreadStart(threadWork4, &Threaded::listenPort));
	oThread4->Start();

	Threaded^ threadWork5 = gcnew Threaded(10544);
	Thread^ oThread5 = gcnew Thread(gcnew ThreadStart(threadWork5, &Threaded::listenPort));
	oThread5->Start();


	return 0;
}

void init() {
	tempIntel = true;
	speedIntel = true;
	for (int i = 0; i < 10; i++) {
		temps[i] = 0;
	}

	using std::cout;
	using std::cin;
	using std::endl;

	hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hRes))
	{
		cout << "Unable to launch COM: 0x" << std::hex << hRes << endl;
	}

	if ((FAILED(hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_CONNECT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0))))
	{
		cout << "Unable to initialize security: 0x" << std::hex << hRes << endl;
	}

	pLocator = NULL;
	if (FAILED(hRes = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pLocator))))
	{
		cout << "Unable to create a WbemLocator: " << std::hex << hRes << endl;
	}

	pService = NULL;
	if (FAILED(hRes = pLocator->ConnectServer(L"root\\OpenHardwareMonitor", NULL, NULL, NULL, WBEM_FLAG_CONNECT_USE_MAX_WAIT, NULL, NULL, &pService)))
	{
		pLocator->Release();
		cout << "Unable to connect to \"CIMV2\": " << std::hex << hRes << endl;
	}
}

void getTemp() {
	IEnumWbemClassObject* pEnumeratorTemp = NULL;
	if (tempIntel) {
		if (FAILED(hRes = pService->ExecQuery(L"WQL", L"select * from Sensor where Identifier like '/intelcpu/0/temperature/_'", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumeratorTemp)))
		{
			pLocator->Release();
			pService->Release();
			cout << "Unable to retrive " << std::hex << hRes << endl;
		}
		int count = 0;
		IWbemClassObject* clsObj = NULL;
		int numElems;
		while ((hRes = pEnumeratorTemp->Next(WBEM_INFINITE, 1, &clsObj, (ULONG*)&numElems)) != WBEM_S_FALSE)
		{
			if (FAILED(hRes)){
				break;
			}

			VARIANT vRet;
			VariantInit(&vRet);
			if (SUCCEEDED(clsObj->Get(L"Value", 0, &vRet, NULL, NULL)))
			{
				if (count < 10) {
					temps[count] = vRet.fltVal;
				}
				count++;
			}
			clsObj->Release();
		}
	}
	if (temps[0] == 0) {
		tempIntel = false;

		if (FAILED(hRes = pService->ExecQuery(L"WQL", L"select * from Sensor where Identifier like '/amdcpu/0/temperature/_'", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumeratorTemp)))
		{
			pLocator->Release();
			pService->Release();
			cout << "Unable to retrive " << std::hex << hRes << endl;
		}
		int count = 0;
		IWbemClassObject* clsObj = NULL;
		int numElems;
		while ((hRes = pEnumeratorTemp->Next(WBEM_INFINITE, 1, &clsObj, (ULONG*)&numElems)) != WBEM_S_FALSE)
		{
			if (FAILED(hRes)){
				break;
			}

			VARIANT vRet;
			VariantInit(&vRet);
			if (SUCCEEDED(clsObj->Get(L"Value", 0, &vRet, NULL, NULL)))
			{
				if (count < 10) {
					temps[count] = vRet.fltVal;
				}
				count++;
			}
			clsObj->Release();
		}

		if (temps[0] == 0) {
			tempIntel = true;
		}
	}
	
}

int getSpeed() {
	int cpuSpeed = 0;
	IEnumWbemClassObject* pEnumeratorSpeed = NULL;

	if (speedIntel) {
		if (FAILED(hRes = pService->ExecQuery(L"WQL", L"select * from Sensor where Identifier like '/intelcpu/0/clock/1'", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumeratorSpeed)))
		{
			pLocator->Release();
			pService->Release();
			cout << "Unable to retrive " << std::hex << hRes << endl;
		}

		IWbemClassObject* clsObj = NULL;
		int numElems;
		while ((hRes = pEnumeratorSpeed->Next(WBEM_INFINITE, 1, &clsObj, (ULONG*)&numElems)) != WBEM_S_FALSE)
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
	}
	if (cpuSpeed == 0) {
		speedIntel = false;

		if (FAILED(hRes = pService->ExecQuery(L"WQL", L"select * from Sensor where Identifier like '/amdcpu/0/clock/1'", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumeratorSpeed)))
		{
			pLocator->Release();
			pService->Release();
			cout << "Unable to retrive " << std::hex << hRes << endl;
		}
		int cpuSpeed = 0;

		IWbemClassObject* clsObj = NULL;
		int numElems;
		while ((hRes = pEnumeratorSpeed->Next(WBEM_INFINITE, 1, &clsObj, (ULONG*)&numElems)) != WBEM_S_FALSE)
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

		if (cpuSpeed == 0) {
			speedIntel = true;
		}
	}
	return cpuSpeed;
}
