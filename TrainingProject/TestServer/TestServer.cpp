// TestServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\OPC_Server\OPC_Server_i.h"
#include "..\OPC_Server\OPC_Server_i.c"

using namespace std;


int main()
{
    HRESULT hr;
    hr = CoInitialize(NULL);
    CComPtr<IOPCServer> pIOPCServer;
    //IOPCServer *pIOPCServer = NULL;
    if (SUCCEEDED(hr))
    {
        cout << "COM Initialized succeessfully" << endl;
    }
    else
    {
        cout << "COM Initialized failed" << endl;
        return -1;
    }
    IID_PPV_ARGS(&pIOPCServer);
    hr = CoCreateInstance(CLSID_OPCServer, NULL, CLSCTX_LOCAL_SERVER, IID_IOPCServer, (LPVOID*)pIOPCServer);
    if (SUCCEEDED(hr))
    {
        cout << "OPCServer Initialization successfully!" << endl;
    }
    else
    {
        cout << "OPCServer Initialization failed!" << endl;
    }
    return 0;
}

