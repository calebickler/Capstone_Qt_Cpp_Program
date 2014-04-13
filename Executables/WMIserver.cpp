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

int temps[10];

int main(array<System::String ^> ^args)
{
	init();
	//ShowWindow(hWnd, 0);
	try
	{

		IPAddress^ localAddr = IPAddress::Parse("127.0.0.1");

		// TcpListener* server = new TcpListener(port);
		TcpListener^ server = gcnew TcpListener(localAddr, 10543);

		// Start listening for client requests.
		server->Start();

		// Buffer for reading data 
		array<Byte>^bytes = gcnew array<Byte>(256);
		String^ data = nullptr;

		// Enter the listening loop. 
		while (true)
		{
			Console::Write("Waiting for a connection... ");

			// Perform a blocking call to accept requests. 
			// You could also user server.AcceptSocket() here.
			TcpClient^ client = server->AcceptTcpClient();
			Console::WriteLine("Connected!");
			data = nullptr;

			// Get a stream Object* for reading and writing
			NetworkStream^ stream = client->GetStream();
			Int32 i;

			// Loop to receive all the data sent by the client. 
			while (i = stream->Read(bytes, 0, bytes->Length))
			{

				// Translate data bytes to a ASCII String*.
				data = Text::Encoding::ASCII->GetString(bytes, 0, i);
				Console::WriteLine("Received: {0}", data);

				// Process the data sent by the client.
				getTemp();

				array<Byte>^msg;

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(getSpeed()) + "    ");
				stream->Write(msg, 0, 4);
				Console::WriteLine(msg->Length);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[0])+ "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[1]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[2]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[3]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[4]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[5]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[6]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[7]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[8]) + "    ");
				stream->Write(msg, 0, 3);

				msg = Text::Encoding::ASCII->GetBytes(System::Convert::ToString(temps[9]) + "    ");
				stream->Write(msg, 0, 3);
			}

			// Shutdown and end connection
			client->Close();
		}
	}
	catch (SocketException^ e)
	{
		Console::WriteLine("SocketException: {0}", e);
	}

    return 0;
}

void init() {
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

int getSpeed() {
	IEnumWbemClassObject* pEnumeratorSpeed = NULL;
	if (FAILED(hRes = pService->ExecQuery(L"WQL", L"select * from Sensor where Identifier like '/intelcpu/0/clock/1'", WBEM_FLAG_FORWARD_ONLY, NULL, &pEnumeratorSpeed)))
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
	return cpuSpeed;
}
