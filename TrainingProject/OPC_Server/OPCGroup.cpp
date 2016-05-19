// OPCGroup.cpp : Implementation of COPCGroup

#include "stdafx.h"
#include "OPCGroup.h"


// COPCGroup::AddItems - Add items to group.
STDMETHODIMP COPCGroup::AddItems(DWORD dwCount, OPCITEMDEF * pItemArray, OPCITEMRESULT ** ppAddResults, HRESULT ** ppErrors)
{
    // Add your function implementation here.
    OPCHANDLE hOPC;
    OPCITEMRESULT * pItemResult = NULL;
    HRESULT * pHr = NULL;
    bool bSuccess = true;
    COPCItem * pNewItem = NULL;
    //*ppAddResults = pItemResult;
    //*ppErrors = pHr;
    //pItemResult = new OPCITEMRESULT[dwCount];
    pItemResult = (OPCITEMRESULT*)CoTaskMemAlloc(dwCount * sizeof(OPCITEMRESULT));
    if (pItemResult == NULL)
    {
        ATLTRACE(L"COPCGroup::AddItems - New operation of pItemResult failed, returning E_OUTOFMEMORY");
        return E_OUTOFMEMORY;
    }

    //pHr = new HRESULT[dwCount];
    pHr = (HRESULT*)CoTaskMemAlloc(dwCount * sizeof(HRESULT));
    if (pHr == NULL)
    {
        ATLTRACE(L"COPCGroup::AddItems - New operation of pHr failed, returning E_OUTOFMEMORY");
        return E_OUTOFMEMORY;
    }

    //*ppAddResults = pItemResult;
    //*ppErrors = pHr;

    // For every item needs to be added
    for (size_t i = 0; i < dwCount; i++)
    {
        size_t stCurrentPosition;
        // Check available resource
        pHr[i] = S_OK;
        for (stCurrentPosition = 0; stCurrentPosition < ITEM_NUMBER; stCurrentPosition++)
        {
            if (m_bItemInUse[stCurrentPosition] == false)
            {
                break;
            }
        }
        // No resources
        if (stCurrentPosition >= ITEM_NUMBER)
        {
            ATLTRACE(L"COPCGroup::AddItems - No avaliable item resource, pHr[%d] = E_OUTOFMEMORY", i);
            pHr[i] = E_OUTOFMEMORY;
            bSuccess = false;
            continue;
        }
        // Set server handle
        hOPC = stCurrentPosition;
        //pNewItem = new COPCItem;
        pNewItem = (COPCItem*)CoTaskMemAlloc(sizeof(COPCItem));
        if (pNewItem == NULL)
        {
            ATLTRACE(L"COPCGroup::AddItems - New operation of pNewItem failed, pHr[%d] = E_OUTOFMEMORY", i);
            pHr[i] = E_OUTOFMEMORY;
            bSuccess = false;
            continue;
        }
        // Initialize item
        pHr[i] = pNewItem->InitItem(hOPC, &pItemArray[i], &pItemResult[i]);
        if (FAILED(pHr[i]))
        {
            delete pNewItem;
            bSuccess = false;
            continue;
        }
        // Set in use flag
        m_bItemInUse[stCurrentPosition] = true;
        if (m_mapIndextoItem.find(stCurrentPosition) != m_mapIndextoItem.end())
        {
            ATLTRACE(L"COPCGroup::AddItems - Already has item with same server handle, pHr[%d] = ", i);
            pHr[i] = E_HANDLE;
            bSuccess = false;
            continue;
        }
        // Set parent group
        pNewItem->m_pParentGroup = this;
        // Add new item to map
        m_mapIndextoItem[stCurrentPosition] = *pNewItem;
    }
    // Set results and errors
    *ppAddResults = pItemResult;
    *ppErrors = pHr;
    if (bSuccess == true)
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
    //return E_NOTIMPL;
}
STDMETHODIMP COPCGroup::ValidateItems(DWORD dwCount, OPCITEMDEF * pItemArray, BOOL bBlobUpdate, OPCITEMRESULT ** ppValidationResults, HRESULT ** ppErrors)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCGroup::RemoveItems(DWORD dwCount, OPCHANDLE * phServer, HRESULT ** ppErrors)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCGroup::SetActiveState(DWORD dwCount, OPCHANDLE * phServer, BOOL bActive, HRESULT ** ppErrors)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCGroup::SetClientHandles(DWORD dwCount, OPCHANDLE * phServer, OPCHANDLE * phClient, HRESULT ** ppErrors)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCGroup::SetDatatypes(DWORD dwCount, OPCHANDLE * phServer, VARTYPE * pRequestedDatatypes, HRESULT ** ppErrors)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCGroup::CreateEnumerator(REFIID riid, LPUNKNOWN * ppUnk)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}

HRESULT COPCItem::InitItem(OPCHANDLE hServerItem, OPCITEMDEF * pOPCItemDef, OPCITEMRESULT * pOPCItemResult)
{
    HRESULT hr;
    SYSTEMTIME systemTime;

    m_vtCanonicalDataType = VT_R8;
    m_vtRequestedDataType = m_vtCanonicalDataType;
    m_szItemName = (WCHAR*)CoTaskMemAlloc(wcslen(pOPCItemDef->szItemID) + 1);
    wcscpy_s(m_szItemName, wcslen(pOPCItemDef->szItemID) + 1, pOPCItemDef->szItemID);
    if (pOPCItemDef->szAccessPath == NULL)
    {
        m_szAccessPath = NULL;
    }
    else
    {
        m_szAccessPath = (WCHAR*)CoTaskMemAlloc(wcslen(pOPCItemDef->szAccessPath) + 1);
        wcscpy_s(m_szAccessPath, wcslen(pOPCItemDef->szItemID) + 1, pOPCItemDef->szAccessPath);
    }
    m_hServerItem = hServerItem;
    m_hClientItem = pOPCItemDef->hClient;
    m_bActive = pOPCItemDef->bActive;
    m_varData.vt = m_vtRequestedDataType;
    m_varData.dblVal = 0.0;

    pOPCItemResult->hServer = m_hServerItem;
    pOPCItemResult->vtCanonicalDataType = m_vtCanonicalDataType;
    pOPCItemResult->dwAccessRights = OPC_READABLE | OPC_WRITEABLE;
    pOPCItemResult->pBlob = NULL;
    pOPCItemResult->dwBlobSize = 0;

    return S_OK;
}

STDMETHODIMP COPCGroup::Advise(
    /* [in] */ __RPC__in_opt IUnknown *pUnkSink,
    /* [out] */ __RPC__out DWORD *pdwCookie)
{
    if (pUnkSink == NULL || pdwCookie == NULL)
    {
        ATLTRACE(L"COPCGroup::Advise - pUnknownSink or pdwCookie is NULL, returning E_INVALIDARG");
        return E_INVALIDARG;
    }
    IUnknown * pSink = NULL;
    *pdwCookie = 0;
    HRESULT hr = S_OK;
    if (m_nConnectionNumber == MAX_CONNECTION_NUMBER)
    {
        ATLTRACE(L"COPCGroup::Advise - Max connections, returning CONNECT_E_ADVISELIMIT");
        return CONNECT_E_ADVISELIMIT;
    }
    IID tempIID;
    GetConnectionInterface(&tempIID);
    hr = pUnkSink->QueryInterface(tempIID, (void **)&pSink);
    if (FAILED(hr))
    {
        ATLTRACE(L"COPCGroup::Advise - Can't get IUnknown, returning CONNECT_E_CANNOTCONNECT");
        return CONNECT_E_CANNOTCONNECT;
    }
    int currentPosition = 0;
    for (; currentPosition < MAX_CONNECTION_NUMBER; currentPosition++)
    {
        if (NULL == m_pConnectionIUnknown[currentPosition])
        {
            break;
        }
    }
    Lock();
    m_pConnectionIUnknown[currentPosition] = pSink;
    *pdwCookie = m_dwConnectionCookies[currentPosition];
    ++m_nConnectionNumber;
    Unlock();
    return S_OK;
    //return E_NOTIMPL;
}

STDMETHODIMP COPCGroup::Unadvise(DWORD dwCookie)
{
    if (dwCookie == 0)
    {
        return E_INVALIDARG;
    }
    for (size_t i = 0; i < MAX_CONNECTION_NUMBER; i++)
    {
        if (dwCookie == m_dwConnectionCookies[i])
        {
            if (m_pConnectionIUnknown[i] != NULL)
            {
                m_pConnectionIUnknown[i]->Release();
                m_pConnectionIUnknown[i] = NULL;
            }
            m_nConnectionNumber--;
            return S_OK;
        }
    }
    return CONNECT_E_NOCONNECTION;
    //return E_NOTIMPL;
}
LRESULT COPCGroup::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (m_mapIndextoItem.size() == 0)
    {
        return 0;
    }
    DOUBLE dSignal[3] = { 0.0 };
    int nSignal = 3;
    LRESULT lr = 0;
    m_pSignal->GenerateSignal(dSignal, nSignal);
    map<int, COPCItem>::iterator iter = m_mapIndextoItem.begin();
    for (; iter != m_mapIndextoItem.end(); iter++)
    {
        if (wcscmp(iter->second.m_szItemName, L"ItemY1") == 0)
        {
            iter->second.m_varData.dblVal = dSignal[0];
            m_vValue[0] = iter->second.m_varData;
        }
        else if (wcscmp(iter->second.m_szItemName, L"ItemY2") == 0)
        {
            iter->second.m_varData.dblVal = dSignal[1];
            m_vValue[1] = iter->second.m_varData;
        }
        else if (wcscmp(iter->second.m_szItemName, L"ItemY3") == 0)
        {
            iter->second.m_varData.dblVal = dSignal[2];
            m_vValue[2] = iter->second.m_varData;
        }
    }
    for (size_t i = 0; i < m_mapIndextoItem.size(); i++)
    {
        //m_vValue[i] = m_mapIndextoItem.find(i)->second.m_varData;
        SYSTEMTIME st;
        GetSystemTime(&st);
        SystemTimeToFileTime(&st, &m_ftTimeStamps[i]);
    }
    lr = CProxyIOPCDataCallback::Fire_OnDataChange(0, m_hClientGroup, S_OK, S_OK, m_mapIndextoItem.size(), m_hClientItem, m_vValue, m_wQualities, m_ftTimeStamps, m_hrErrors);
    Unlock();
    return lr;

}

//CWinHidden
void CWinHidden::AttachCtl(COPCGroup* pFullCtrl)
{
    m_pFullCtrl = pFullCtrl;
}
BOOL CWinHidden::SetThisTimer(UINT nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc)
{
    if (m_hWnd == NULL)
    {
        RECT rt = { 0, 0, 0, 0 };
        Create(NULL, rt);
    }
    m_nTimer = ::SetTimer(m_hWnd, nIDEvent, uElapse, lpTimerFunc);
    return m_nTimer == 0;
}
void CWinHidden::KillThisTimer()
{
    if (m_nTimer != 0)
    {
        KillTimer(m_nTimer);
        m_nTimer = 0;
    }
}
LRESULT CWinHidden::OnTimer(UINT uMsg, WPARAM wParam,
    LPARAM lParam, BOOL& bHandled)
{
    if ((UINT)wParam != m_nTimer)
        return -1;
    KillTimer(m_nTimer);
    if (m_pFullCtrl != NULL)
    {
        m_pFullCtrl->OnTimer(uMsg, wParam, lParam, bHandled);
    }
    m_nTimer = ::SetTimer(m_hWnd, 1, MIN_UPDATE_RATE, NULL);
    return 0;
}
