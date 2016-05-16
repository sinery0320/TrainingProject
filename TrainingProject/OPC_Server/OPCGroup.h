// OPCGroup.h : Declaration of the COPCGroup

#pragma once
#include "resource.h"       // main symbols


#include "OPC_Server_i.h"
#include "IOPCDataCallback_CP.H"
extern IMalloc * pIMalloc;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace std;
//COPCItem
class COPCItem
{
public:
    friend class COPCGroup;
    COPCItem()
    {
        //m_bInUse = false;
        m_bActive = false;
        m_hClientItem = 0;
        m_hServerItem = 0;
        m_szItemName = NULL;
        m_szAccessPath = NULL;

        m_varData.vt = VT_R8;
        m_varData.dblVal = 0.0;
        //m_ftTimeStamp.dwHighDateTime = 0;
        //m_ftTimeStamp.dwLowDateTime = 0;
        m_pParentGroup = NULL;
        m_AsyncMask = 0;
    }
    //~COPCItem()
    //{
    //    if (m_szItemName != NULL)
    //    {
    //        delete [] m_szItemName;
    //        m_szItemName = NULL;
    //    }
    //    m_pParentGroup = NULL;
    //}
    //bool m_bInUse;
    bool m_bActive;
    OPCHANDLE m_hClientItem;
    OPCHANDLE m_hServerItem;// Sequence of item in group
    VARTYPE m_vtCanonicalDataType;
    VARTYPE m_vtRequestedDataType;
    WCHAR * m_szItemName;
    WCHAR * m_szAccessPath;

    VARIANT m_varData;
    //FILETIME m_ftTimeStamp;
    WORD m_AsyncMask;
    COPCGroup * m_pParentGroup;


    HRESULT InitItem(OPCHANDLE hServerItem, OPCITEMDEF * pOPCItemDef, OPCITEMRESULT * pOPCItemResult);
};

// COPCGroup

class ATL_NO_VTABLE COPCGroup :
    public CComObjectRootEx<CComSingleThreadModel>,
    //public CComCoClass<COPCGroup, &CLSID_OPCGroup>,
    public IDispatchImpl<IOPCItemMgt, &__uuidof(IOPCItemMgt), &LIBID_OPC_ServerLib, /* wMajor = */ 3, /* wMinor = */ 00>,
    public IConnectionPointContainerImpl<COPCGroup>,
    public CProxyIOPCDataCallback<COPCGroup>
    //public IDispatchImpl<IOPCGroup, &IID_IOPCGroup, &LIBID_OPC_ServerLib, /*wMajor =*/ 3, /*wMinor =*/ 00>,

{
public:
    COPCGroup()
    {
        m_nSequenceIndex = 0;
        //m_bItemInUse = false;
        m_pParent = NULL;
        m_wcSzName = 0;
        m_bActive = false;
        m_lTimeBias = 0L;
        m_percentDeadband = 0.0;
        m_dwLCID = 0;;
        m_dwUpdateRate = 0;
        m_hServerGroup = 0;
        m_hClientGroup = 0;
        
        for (size_t i = 0; i < ITEM_NUMBER; i++)
        {
            m_bItemInUse[i] = false;
            //m_cItem[i] = NULL;
        }
    }

    //~COPCGroup()
    //{
    //    if (m_wcSzName != NULL)
    //    {
    //        delete [] m_wcSzName;
    //        m_wcSzName = NULL;
    //    }
    //    for (size_t i = 0; i < ITEM_NUMBER; i++)
    //    {
    //        if (m_cItem[i] != NULL)
    //        {
    //            delete m_cItem[i];
    //            m_cItem[i] = NULL;
    //        }
    //    }
    //}
    //DECLARE_REGISTRY_RESOURCEID(IDR_OPCGROUP)


    BEGIN_COM_MAP(COPCGroup)
        COM_INTERFACE_ENTRY(IOPCItemMgt)
        COM_INTERFACE_ENTRY(IConnectionPointContainer)
    END_COM_MAP()



    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        //for (size_t i = 0; i < ITEM_NUMBER; i++)
        //{
        //    m_cItem[i] = new COPCItem;
        //    m_cItem[i]->m_pParentGroup = this;
        //    if (m_cItem[i] == NULL)
        //    {
        //        ATLTRACE(L"COPCGroup::FinalConstruct - New operation of m_cItem failed, returning E_OUTOFMEMORY");
        //        return E_OUTOFMEMORY;
        //    }
        //}
        return S_OK;
    }

    void FinalRelease()
    {
        //for (size_t i = 0; i < ITEM_NUMBER; i++)
        //{
        //    if (m_cItem[i] != NULL)
        //    {
        //        delete m_cItem[i];
        //        m_cItem[i] = NULL;
        //    }
        //}
    }

public:
    friend class COPCServer;
    int m_nSequenceIndex;
    COPCServer * m_pParent;
    LPWSTR m_wcSzName;
    bool m_bActive;
    LONG m_lTimeBias;
    FLOAT m_percentDeadband;
    DWORD m_dwLCID;
    DWORD m_dwUpdateRate;
    OPCHANDLE m_hServerGroup;
    OPCHANDLE m_hClientGroup;

    VARIANT * m_pvValue;
    WORD * m_pwQualities;

    bool m_bItemInUse[ITEM_NUMBER];
    map<int, COPCItem> m_mapIndextoItem;
    //COPCItem * m_cItem[ITEM_NUMBER];


    // IOPCItemMgt Methods
public:
    STDMETHOD(AddItems)(DWORD dwCount, OPCITEMDEF * pItemArray, OPCITEMRESULT ** ppAddResults, HRESULT ** ppErrors);
    STDMETHOD(ValidateItems)(DWORD dwCount, OPCITEMDEF * pItemArray, BOOL bBlobUpdate, OPCITEMRESULT ** ppValidationResults, HRESULT ** ppErrors);
    STDMETHOD(RemoveItems)(DWORD dwCount, OPCHANDLE * phServer, HRESULT ** ppErrors);
    STDMETHOD(SetActiveState)(DWORD dwCount, OPCHANDLE * phServer, BOOL bActive, HRESULT ** ppErrors);
    STDMETHOD(SetClientHandles)(DWORD dwCount, OPCHANDLE * phServer, OPCHANDLE * phClient, HRESULT ** ppErrors);
    STDMETHOD(SetDatatypes)(DWORD dwCount, OPCHANDLE * phServer, VARTYPE * pRequestedDatatypes, HRESULT ** ppErrors);
    STDMETHOD(CreateEnumerator)(REFIID riid, LPUNKNOWN * ppUnk);

    //IOPCDataCallback
    STDMETHOD(OnDataChange)(
        DWORD dwTransid,
        OPCHANDLE hGroup,
        HRESULT hrMasterquality,
        HRESULT hrMastererror,
        DWORD dwCount,
        OPCHANDLE * phClientItems,
        VARIANT * pvValues,
        WORD * pwQualities,
        FILETIME * pftTimeStamps,
        HRESULT * pErrors
        );


private:
public:
    BEGIN_CONNECTION_POINT_MAP(COPCGroup)
        CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
    END_CONNECTION_POINT_MAP()
};

//OBJECT_ENTRY_AUTO(__uuidof(OPCGroup), COPCGroup)
