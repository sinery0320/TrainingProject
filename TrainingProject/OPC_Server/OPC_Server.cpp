// OPC_Server.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "OPC_Server_i.h"


using namespace ATL;


class COPC_ServerModule : public ATL::CAtlExeModuleT< COPC_ServerModule >
{
public :
	DECLARE_LIBID(LIBID_OPC_ServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OPC_SERVER, "{205FABDE-81CC-422A-A3BC-B0A50E178CDC}")
	};

COPC_ServerModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

