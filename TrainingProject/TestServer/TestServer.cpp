// TestServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\OPC_Server\OPC_Server_i.h"
#include "..\OPC_Server\OPC_Server_i.c"

using namespace std;


int main()
{
    CLSID clsid;
    HRESULT hr;
    //hr = CoInitialize(NULL);
    //hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    //CComPtr<IOPCServer> pIOPCServer;
    IUnknown *pUnk = NULL;
    IOPCServer *pIOPCServer = NULL;
    if (SUCCEEDED(hr))
    {
        cout << "COM Initialized succeessfully" << endl;
    }
    else
    {
        cout << "COM Initialized failed" << endl;
        return -1;
    }
    hr = CLSIDFromProgID(L"TrainingProject.OPC_Server.OPCServer.1", &clsid);
    if (SUCCEEDED(hr))
    {
        cout << "Get CLSID succeessfully" << endl;
    }
    else
    {
        cout << "Get CLSID failed" << endl;
        return -1;
    }
    //IID_PPV_ARGS(&pIOPCServer);
    hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IUnknown, (LPVOID*)&pUnk);
    //pIOPCServer.CoCreateInstance(CLSID_OPCServer);
    if (SUCCEEDED(hr))
    {
        cout << "OPCServer Initialization successfully!" << endl;
    }
    else
    {
        cout << "OPCServer Initialization failed!" << endl;
    }

    hr = pUnk->QueryInterface(IID_IOPCServer, (LPVOID*)&pIOPCServer);
    if (SUCCEEDED(hr))
    {
        cout << "Get OPCServer interface successfully!" << endl;
    }
    else
    {
        cout << "Get OPCServer interface failed!" << endl;
        pUnk->Release();
    }
    
    LONG lTimeBias = 0;
    FLOAT dDeadBand = 0.0;
    OPCHANDLE phServerGroup;
    DWORD dwUpdateRate = 0;
    IOPCItemMgt * pIOPCItemMgt = NULL;
    DWORD dwLCID = 2052;
    hr = pIOPCServer->AddGroup(L"Group1", true, 200, 1, &lTimeBias, &dDeadBand, dwLCID, &phServerGroup, &dwUpdateRate, IID_IOPCItemMgt, (LPUNKNOWN*)&pIOPCItemMgt);
    if (SUCCEEDED(hr))
    {
        cout << "Call AddGroup function successfully!" << endl;
    }
    else
    {
        cout << "Call AddGroup function failed!" << endl;
        pIOPCServer->Release();
        pUnk->Release();
    }

    OPCITEMDEF opcItem = {
        NULL,
        L"Item1",
        TRUE,
        0,
        0,
        NULL,
        VT_R8,
        0
    };
    OPCITEMRESULT * opcItemResult = NULL;
    HRESULT * errorResult = NULL;
    hr = pIOPCItemMgt->AddItems(1, &opcItem, &opcItemResult, &errorResult);
    if (SUCCEEDED(hr))
    {
        cout << "AddItems successfully!" << endl;
    }
    else
    {
        cout << "AddItems failed!" << endl;
        pIOPCItemMgt->Release();
        pIOPCServer->Release();
        pUnk->Release();
    }
    CoUninitialize();
    return 0;
}

