// OPCServer.h : Declaration of the COPCServer

#pragma once
#include "resource.h"       // main symbols



#include "OPC_Server_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// COPCServer

class ATL_NO_VTABLE COPCServer :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<COPCServer, &CLSID_OPCServer>,
    //public IDispatchImpl<IOPCServer_temp, &IID_IOPCServer_temp, &LIBID_OPC_ServerLib, /*wMajor =*/ 3, /*wMinor =*/ 00>,
    public IDispatchImpl<IOPCServer, &IID_IOPCServer, &LIBID_OPC_ServerLib, /*wMajor =*/ 3, /*wMinor =*/ 00>
    //public IDispatchImpl<IOPCServer, &__uuidof(IOPCServer), &LIBID_OPC_ServerLib, /* wMajor = */ 3, /* wMinor = */ 00>
{
public:
    COPCServer()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_OPCSERVER)


    BEGIN_COM_MAP(COPCServer)
        //COM_INTERFACE_ENTRY(IOPCServer_temp)
        //COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IOPCServer)
    END_COM_MAP()



    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:




    // IOPCServer Methods
public:
    STDMETHOD(AddGroup)(LPCWSTR szName, BOOL bActive, DWORD dwRequestedUpdateRate, OPCHANDLE hClientGroup, LONG * pTimeBias, FLOAT * pPercentDeadband, DWORD dwLCID, OPCHANDLE * phServerGroup, DWORD * pRevisedUpdateRate, REFIID riid, LPUNKNOWN * ppUnk);
    //{
    //    // Add your function implementation here.
    //    return E_NOTIMPL;
    //}
    STDMETHOD(GetErrorString)(HRESULT dwError, LCID dwLocale, LPWSTR * ppString);
    //{
    //    // Add your function implementation here.
    //    return E_NOTIMPL;
    //}
    STDMETHOD(GetGroupByName)(LPCWSTR szName, REFIID riid, LPUNKNOWN * ppUnk);
    //{
    //    // Add your function implementation here.
    //    return E_NOTIMPL;
    //}
    STDMETHOD(GetStatus)(OPCSERVERSTATUS ** ppServerStatus);
    //{
    //    // Add your function implementation here.
    //    return E_NOTIMPL;
    //}
    STDMETHOD(RemoveGroup)(OPCHANDLE hServerGroup, BOOL bForce);
    //{
    //    // Add your function implementation here.
    //    return E_NOTIMPL;
    //}
    STDMETHOD(CreateGroupEnumerator)(OPCENUMSCOPE dwScope, REFIID riid, LPUNKNOWN * ppUnk);
    //{
    //    // Add your function implementation here.
    //    return E_NOTIMPL;
    //}
};

OBJECT_ENTRY_AUTO(__uuidof(OPCServer), COPCServer)
