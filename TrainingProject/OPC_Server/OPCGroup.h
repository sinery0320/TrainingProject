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
class COPCGroup;
class CWinHidden :
    public CWindowImpl<CWinHidden, CWindow, CNullTraits>
{
    BEGIN_MSG_MAP(CWinHidden)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
    END_MSG_MAP()
public:
    CWinHidden() :m_nTimer(0), m_pFullCtrl(NULL){};
    ~CWinHidden()
    {
        if (m_nTimer != 0)
        {
            KillTimer(m_nTimer);
            m_nTimer = 0;
        }
        if (m_hWnd != NULL)
        {
            ::DestroyWindow(m_hWnd);
            m_hWnd = NULL;
        }
    }
    void AttachCtl(COPCGroup* pFullCtrl)
    {
        m_pFullCtrl = pFullCtrl;
    }
    BOOL SetThisTimer(UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
    {
        if (m_hWnd == NULL)
        {
            RECT rt = { 0, 0, 0, 0 };
            Create(NULL, rt);
        }
        m_nTimer = ::SetTimer(m_hWnd, nIDEvent, uElapse, lpTimerFunc);
        return m_nTimer == 0;
    }
    void KillThisTimer()
    {
        if (m_nTimer != 0)
        {
            KillTimer(m_nTimer);
            m_nTimer = 0;
        }
    }
public:
    LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);
private:
    COPCGroup*   m_pFullCtrl;
    UINT        m_nTimer;
};
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
    //public IConnectionPointImpl<COPCGroup, &IID_IOPCDataCallback, CComDynamicUnkArray>
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
        //m_hThread = 0;
        //m_nElapseTime = MIN_UPDATE_RATE;

        for (size_t i = 0; i < ITEM_NUMBER; i++)
        {
            m_bItemInUse[i] = false;
            //m_cItem[i] = NULL;
        }

        for (size_t i = 0; i < MAX_CONNECTION_NUMBER; i++)
        {
            m_dwConnectionCookies[i] = i + 1;
            m_pConnectionIUnknown[i] = NULL;

        }

        m_nConnectionNumber = 0;

        //m_hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)(&m_nElapseTime), 0, NULL);
        //SetTimer(NULL, 1, MIN_UPDATE_RATE, TimerProc);

        m_cWinHidden.AttachCtl(this);
        m_cWinHidden.SetThisTimer(1, MIN_UPDATE_RATE, NULL);
    }

    ~COPCGroup()
    {
        //if (m_wcSzName != NULL)
        //{
        //    delete [] m_wcSzName;
        //    m_wcSzName = NULL;
        //}
        //for (size_t i = 0; i < ITEM_NUMBER; i++)
        //{
        //    if (m_cItem[i] != NULL)
        //    {
        //        delete m_cItem[i];
        //        m_cItem[i] = NULL;
        //    }
        //}

        //CloseHandle(m_hThread);
    }
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

    VARIANT m_vValue[MAX_CONNECTION_NUMBER];
    WORD m_wQualities[MAX_CONNECTION_NUMBER];
    FILETIME m_ftTimeStamps[MAX_CONNECTION_NUMBER];
    HRESULT m_hrErrors[MAX_CONNECTION_NUMBER];
    OPCHANDLE m_hClientItem[MAX_CONNECTION_NUMBER];

    bool m_bItemInUse[ITEM_NUMBER];
    map<int, COPCItem> m_mapIndextoItem;
    //COPCItem * m_cItem[ITEM_NUMBER];

    int m_nConnectionNumber;
    DWORD m_dwConnectionCookies[MAX_CONNECTION_NUMBER];
    IUnknown * m_pConnectionIUnknown[MAX_CONNECTION_NUMBER];

    //HANDLE m_hThread;
    //int m_nElapseTime;

    CWinHidden m_cWinHidden;
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
    //STDMETHOD(OnDataChange)(
    //    DWORD dwTransid,
    //    OPCHANDLE hGroup,
    //    HRESULT hrMasterquality,
    //    HRESULT hrMastererror,
    //    DWORD dwCount,
    //    OPCHANDLE * phClientItems,
    //    VARIANT * pvValues,
    //    WORD * pwQualities,
    //    FILETIME * pftTimeStamps,
    //    HRESULT * pErrors
    //    );

    STDMETHOD(Advise)(
        /* [in] */ __RPC__in_opt IUnknown *pUnkSink,
        /* [out] */ __RPC__out DWORD *pdwCookie);
    STDMETHOD(Unadvise)(
        /* [in] */ DWORD dwCookie);

    //static DWORD WINAPI ThreadFunc(LPVOID lpParam);

    //static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
    //{
    //    if (idEvent == 1)
    //    {
    //        KillTimer(NULL, idEvent);
    //        ATLTRACE(L"Timer");
    //        //Fire_OnDataChange(0, m_hClientGroup, S_OK, S_OK, m_mapIndextoItem.size(), hClientItems, vValues, wQualities, ftTimeStamps, Errors);
    //        SetTimer(NULL, 1, MIN_UPDATE_RATE, TimerProc);
    //    }
    //}
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        //Fire_OnDataChange(DWORD dwTransid, OPCHANDLE hGroup, HRESULT hrMasterquality, HRESULT hrMastererror, DWORD dwCount, OPCHANDLE * phClientItems, VARIANT * pvValues, WORD * pwQualities, FILETIME * pftTimeStamps, HRESULT * pErrors)
        return CProxyIOPCDataCallback::Fire_OnDataChange(0, m_hClientGroup, S_OK, S_OK, m_mapIndextoItem.size(), m_hClientItem, m_vValue, m_wQualities, m_ftTimeStamps, m_hrErrors);
    }


private:
public:
    BEGIN_CONNECTION_POINT_MAP(COPCGroup)
        CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
    END_CONNECTION_POINT_MAP()
};

//OBJECT_ENTRY_AUTO(__uuidof(OPCGroup), COPCGroup)

