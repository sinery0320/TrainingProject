// OPCGroup.h : Declaration of the COPCGroup

#pragma once
#include "resource.h"       // main symbols


#include "OPC_Server_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
//COPCItem
class COPCItem
{
public:
    friend class COPCGroup;
    //COPCItem();
    //~COPCItem();
    bool m_bInUse;
    bool m_bActive;
    OPCHANDLE m_hClient;
    OPCHANDLE m_hServer;
    WCHAR * m_szItemName;

    VARIANT m_varData;
    FILETIME m_ftTimeStamp;
    COPCGroup * m_pParentGroup;
};
//COPCItem::COPCItem()
//{
//}
//
//COPCItem::~COPCItem()
//{
//}


// COPCGroup

class ATL_NO_VTABLE COPCGroup :
    public CComObjectRootEx<CComSingleThreadModel>,
    //public CComCoClass<COPCGroup, &CLSID_OPCGroup>,
    public IDispatchImpl<IOPCItemMgt, &__uuidof(IOPCItemMgt), &LIBID_OPC_ServerLib, /* wMajor = */ 3, /* wMinor = */ 00>
    //public IDispatchImpl<IOPCGroup, &IID_IOPCGroup, &LIBID_OPC_ServerLib, /*wMajor =*/ 3, /*wMinor =*/ 00>,

{
public:
    COPCGroup()
    {
        m_nSequenceIndex = 0;
        m_bInUse = false;
        m_pParent = NULL;
        m_wcSzName = 0;
        m_bActive = false;
        m_lTimeBias = 0L;
        m_percentDeadband = 0.0;
        m_dwLCID = 0;;
        m_dwUpdateRate = 0;
        m_hServerGroup = 0;
        m_hClientGroup = 0;
    }

    ~COPCGroup()
    {
        if (m_wcSzName)
        {
            delete m_wcSzName;
            m_wcSzName = NULL;
        }
    }
    //DECLARE_REGISTRY_RESOURCEID(IDR_OPCGROUP)


    BEGIN_COM_MAP(COPCGroup)
        COM_INTERFACE_ENTRY(IOPCItemMgt)
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
    friend class COPCServer;
    int m_nSequenceIndex;
    bool m_bInUse;
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
    COPCItem * m_cItem[ITEM_NUMBER];


    // IOPCItemMgt Methods
public:
    STDMETHOD(AddItems)(DWORD dwCount, OPCITEMDEF * pItemArray, OPCITEMRESULT ** ppAddResults, HRESULT ** ppErrors);
    STDMETHOD(ValidateItems)(DWORD dwCount, OPCITEMDEF * pItemArray, BOOL bBlobUpdate, OPCITEMRESULT ** ppValidationResults, HRESULT ** ppErrors);
    STDMETHOD(RemoveItems)(DWORD dwCount, OPCHANDLE * phServer, HRESULT ** ppErrors);
    STDMETHOD(SetActiveState)(DWORD dwCount, OPCHANDLE * phServer, BOOL bActive, HRESULT ** ppErrors);
    STDMETHOD(SetClientHandles)(DWORD dwCount, OPCHANDLE * phServer, OPCHANDLE * phClient, HRESULT ** ppErrors);
    STDMETHOD(SetDatatypes)(DWORD dwCount, OPCHANDLE * phServer, VARTYPE * pRequestedDatatypes, HRESULT ** ppErrors);
    STDMETHOD(CreateEnumerator)(REFIID riid, LPUNKNOWN * ppUnk);
private:
};

//OBJECT_ENTRY_AUTO(__uuidof(OPCGroup), COPCGroup)
