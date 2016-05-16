// OPCServer.h : Declaration of the COPCServer

#pragma once
#include "resource.h"       // main symbols
#include "opcerror.h"
#include "OPC_Server_i.h"
#include "OPCGroup.h"
#include "IOPCDataCallback_CP.H"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace std;

// COPCServer

class ATL_NO_VTABLE COPCServer :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<COPCServer, &CLSID_OPCServer>,
    //public IDispatchImpl<IOPCServer_temp, &IID_IOPCServer_temp, &LIBID_OPC_ServerLib, /*wMajor =*/ 3, /*wMinor =*/ 00>,
    public IDispatchImpl<IOPCServer, &IID_IOPCServer, &LIBID_OPC_ServerLib, /*wMajor =*/ 3, /*wMinor =*/ 00>,
    public IConnectionPointContainerImpl<COPCServer>,
    public CProxyIOPCDataCallback<COPCServer>
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
        //COM_INTERFACE_ENTRY_AGGREGATE(IID_IOPCItemMgt, m_mapNametoGroup)
        COM_INTERFACE_ENTRY(IConnectionPointContainer)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        for (size_t i = 0; i < GROUP_NUMBER; i++)
        {
            m_bGroupInUse[i] = false;
        }
        // Useless code
        //HRESULT hr;
        //for (size_t i = 0; i < GROUP_NUMBER; i++)
        //{
        //    m_bGroupInUse[i] = false;
        //    hr = CComObject<COPCGroup>::CreateInstance(&m_pGroupObject[i]);
        //    if (SUCCEEDED(hr))
        //    {
        //        m_pGroupObject[i]->m_nSequenceIndex = i;
        //        m_pGroupObject[i]->m_pParent = this;
        //    }
        //    else
        //    {
        //        ATLTRACE(L"COPCServer::FinalConstruct - Failed to creat COPCGroup instance, returning E_HANDLE");
        //        return E_HANDLE;
        //    }
        //}
        return S_OK;
    }

    void FinalRelease()
    {
        // Useless code
        //for (size_t i = 0; i < GROUP_NUMBER; i++)
        //{
        //    m_mapNametoGroup[i]->Release();
        //    return;
        //}

        // Manually release may cause heap corruption
        //if (!m_mapNametoGroup.empty())
        //{
        //    map<wstring, CComObject<COPCGroup>>::iterator iter = m_mapNametoGroup.begin();
        //    while (iter != m_mapNametoGroup.end())
        //    {
        //        iter->second.Release();
        //        iter = m_mapNametoGroup.erase(iter);
        //    }
        //}
    }

public:
    friend class COPCGroup;
    map<wstring, CComObject<COPCGroup>> m_mapNametoGroup;
    //CComObject<COPCGroup> * m_pGroupObject[GROUP_NUMBER];
    bool m_bGroupInUse[GROUP_NUMBER];

    // IOPCServer Methods
public:
    STDMETHOD(AddGroup)(LPCWSTR szName, BOOL bActive, DWORD dwRequestedUpdateRate, OPCHANDLE hClientGroup, LONG * pTimeBias, FLOAT * pPercentDeadband, DWORD dwLCID, OPCHANDLE * phServerGroup, DWORD * pRevisedUpdateRate, REFIID riid, LPUNKNOWN * ppUnk);
    STDMETHOD(GetErrorString)(HRESULT dwError, LCID dwLocale, LPWSTR * ppString);
    STDMETHOD(GetGroupByName)(LPCWSTR szName, REFIID riid, LPUNKNOWN * ppUnk);
    STDMETHOD(GetStatus)(OPCSERVERSTATUS ** ppServerStatus);
    STDMETHOD(RemoveGroup)(OPCHANDLE hServerGroup, BOOL bForce);
    STDMETHOD(CreateGroupEnumerator)(OPCENUMSCOPE dwScope, REFIID riid, LPUNKNOWN * ppUnk);
    BEGIN_CONNECTION_POINT_MAP(COPCServer)
        CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
    END_CONNECTION_POINT_MAP()
};

OBJECT_ENTRY_AUTO(__uuidof(OPCServer), COPCServer)
