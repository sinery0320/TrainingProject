// OPCGroup.h : Declaration of the COPCGroup

#pragma once
#include "resource.h"       // main symbols

#include "OPC_Server_i.h"
#include "IOPCDataCallback_CP.H"
#include "SignalGenerator_i.c"
#include "SignalGenerator_i.h"

#define ITEM_NUMBER                     50 // 2016.6.20 Increase max number
#define MIN_UPDATE_RATE                 200
#define MAX_CONNECTION_NUMBER           50 // 2016.6.20 Increase max number

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
using namespace std;

class COPCGroup;

// Class of Hidden timer window
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
    void AttachCtl(COPCGroup* pFullCtrl);
    BOOL SetThisTimer(UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
    void KillThisTimer();
    LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);
private:
    COPCGroup * m_pFullCtrl;    // Timer binding control
    UINT m_nTimer;              // Timer ID
};

// Class of COPCItem
class COPCItem
{
public:
    friend class COPCGroup;
    COPCItem()
    {
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
    }
    ~COPCItem()
    {
        if (m_szItemName != NULL)
        {
            CoTaskMemFree(m_szItemName);
            m_szItemName = NULL;
        }
        if (m_szAccessPath != NULL)
        {
            CoTaskMemFree(m_szAccessPath);
            m_szAccessPath = NULL;
        }
        m_pParentGroup = NULL;
    }
    bool m_bActive;                 // Active flag
    OPCHANDLE m_hClientItem;        // Handle of Client
    OPCHANDLE m_hServerItem;        // Handle of Server
    VARTYPE m_vtCanonicalDataType;  // Canonical data type
    VARTYPE m_vtRequestedDataType;  // Requested data type
    WCHAR * m_szItemName;           // Item name
    WCHAR * m_szAccessPath;         // Access path

    VARIANT m_varData;              // Data of item
    //FILETIME m_ftTimeStamp;
    COPCGroup * m_pParentGroup;     // Pointer of parent group

public:
    // InitItem - Initialize item.
    HRESULT InitItem(OPCHANDLE hServerItem, OPCITEMDEF * pOPCItemDef, OPCITEMRESULT * pOPCItemResult);
};

// Class of COPCGroup
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
        }

        for (size_t i = 0; i < MAX_CONNECTION_NUMBER; i++)
        {
            m_vValue[i].vt = VT_R8;
            m_vValue[i].dblVal = 0.0;
            m_wQualities[i] = 0;
            m_ftTimeStamps[i].dwHighDateTime = 0;
            m_ftTimeStamps[i].dwLowDateTime = 0;
            m_hrErrors[i] = 0;
            m_hClientItem[i] = 0;
            m_dwConnectionCookies[i] = i + 1;
            m_pConnectionIUnknown[i] = NULL;
        }
        m_nConnectionNumber = 0;

        // Other tries of timer
        //m_hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)(&m_nElapseTime), 0, NULL);
        //SetTimer(NULL, 1, MIN_UPDATE_RATE, TimerProc);

        m_cWinHidden = new CWinHidden();
        m_cWinHidden->AttachCtl(this);
        // 2016/6/21 Not set timer on initialization
        //m_cWinHidden->SetThisTimer(1, MIN_UPDATE_RATE, NULL);
        // 2016/6/21 Move from FinalConstruct function to here
        m_pSignal.CoCreateInstance(CLSID_Signal);
    }

    ~COPCGroup()
    {
        // 2016/6/21 Move from FinalRelease function to here and assign NULL
        m_pSignal = NULL;
        //if (m_wcSzName != NULL)
        //{
        //    delete m_wcSzName;
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
        //if (m_cWinHidden != NULL)
        //{
        //    m_cWinHidden->KillThisTimer();
        //    delete m_cWinHidden;
        //    m_cWinHidden = NULL;
        //}
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
        //m_pSignal.CoCreateInstance(CLSID_Signal);
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
        //if (m_cWinHidden != NULL)
        //{
        //    m_cWinHidden->KillThisTimer();
        //    delete m_cWinHidden;
        //    m_cWinHidden = NULL;
        //}
        //m_pSignal.Release();
    }

public:
    friend class COPCServer;
    int m_nSequenceIndex;                                   // Index of group in server
    COPCServer * m_pParent;                                 // Pointer to parent server
    LPWSTR m_wcSzName;                                      // Group name
    bool m_bActive;                                         // Active flag
    LONG m_lTimeBias;                                       // Time Bias
    FLOAT m_percentDeadband;                                // Deadband
    DWORD m_dwLCID;                                         // Language ID
    DWORD m_dwUpdateRate;                                   // Update rate
    OPCHANDLE m_hServerGroup;                               // Handle of server
    OPCHANDLE m_hClientGroup;                               // Handle of group

    VARIANT m_vValue[MAX_CONNECTION_NUMBER];                // Values
    WORD m_wQualities[MAX_CONNECTION_NUMBER];               // Qualities
    FILETIME m_ftTimeStamps[MAX_CONNECTION_NUMBER];         // TimeStamps
    HRESULT m_hrErrors[MAX_CONNECTION_NUMBER];              // Errors
    OPCHANDLE m_hClientItem[MAX_CONNECTION_NUMBER];         // Handle of item client

    bool m_bItemInUse[ITEM_NUMBER];                         // In use flag
    map<int, COPCItem> m_mapIndextoItem;                    // int to item map(name can be the same, but index can't)

    int m_nConnectionNumber;                                // Active connection number
    DWORD m_dwConnectionCookies[MAX_CONNECTION_NUMBER];     // Connections cookies
    IUnknown * m_pConnectionIUnknown[MAX_CONNECTION_NUMBER];// Connections IUnknown interface

    CWinHidden * m_cWinHidden;                              // Pointer of Hidden timer window
    CComPtr<ISignal> m_pSignal;                             // Smart pointer of ISignal
    // IOPCItemMgt Methods
public:
    STDMETHOD(AddItems)(DWORD dwCount, OPCITEMDEF * pItemArray, OPCITEMRESULT ** ppAddResults, HRESULT ** ppErrors);
    STDMETHOD(ValidateItems)(DWORD dwCount, OPCITEMDEF * pItemArray, BOOL bBlobUpdate, OPCITEMRESULT ** ppValidationResults, HRESULT ** ppErrors);
    STDMETHOD(RemoveItems)(DWORD dwCount, OPCHANDLE * phServer, HRESULT ** ppErrors);
    STDMETHOD(SetActiveState)(DWORD dwCount, OPCHANDLE * phServer, BOOL bActive, HRESULT ** ppErrors);
    STDMETHOD(SetClientHandles)(DWORD dwCount, OPCHANDLE * phServer, OPCHANDLE * phClient, HRESULT ** ppErrors);
    STDMETHOD(SetDatatypes)(DWORD dwCount, OPCHANDLE * phServer, VARTYPE * pRequestedDatatypes, HRESULT ** ppErrors);
    STDMETHOD(CreateEnumerator)(REFIID riid, LPUNKNOWN * ppUnk);
    STDMETHOD(Advise)(
        /* [in] */ __RPC__in_opt IUnknown *pUnkSink,
        /* [out] */ __RPC__out DWORD *pdwCookie);
    STDMETHOD(Unadvise)(
        /* [in] */ DWORD dwCookie);

    // Other tries of timer
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

    // OnTimer - Timer response function.
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    BEGIN_CONNECTION_POINT_MAP(COPCGroup)
        CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
    END_CONNECTION_POINT_MAP()
};

//OBJECT_ENTRY_AUTO(__uuidof(OPCGroup), COPCGroup)

