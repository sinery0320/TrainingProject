// OPCGroup.cpp : Implementation of COPCGroup

#include "stdafx.h"
#include "OPCGroup.h"


// COPCGroup
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
    // for every item needs to be added
    for (size_t i = 0; i < dwCount; i++)
    {
        size_t stCurrentPosition;
        // check available resource
        pHr[i] = S_OK;
        for (stCurrentPosition = 0; stCurrentPosition < ITEM_NUMBER; stCurrentPosition++)
        {
            //if (m_cItem[stCurrentPosition]->m_bInUse == false)
            if (m_bItemInUse[stCurrentPosition] == false)
            {
                break;
            }
        }
        if (stCurrentPosition >= ITEM_NUMBER)
        {
            ATLTRACE(L"COPCGroup::AddItems - No avaliable item resource, pHr[%d] = E_OUTOFMEMORY", i);
            pHr[i] = E_OUTOFMEMORY;
            bSuccess = false;
            continue;
        }
        hOPC = stCurrentPosition;
        pNewItem = new COPCItem;
        if (pNewItem == NULL)
        {
            ATLTRACE(L"COPCGroup::AddItems - New operation of pNewItem failed, pHr[%d] = E_OUTOFMEMORY", i);
            pHr[i] = E_OUTOFMEMORY;
            bSuccess = false;
            continue;
        }
        pHr[i] = pNewItem->InitItem(hOPC, &pItemArray[i], &pItemResult[i]);
        if (FAILED(pHr[i]))
        {
            delete pNewItem;
            bSuccess = false;
            continue;
        }
        m_bItemInUse[stCurrentPosition] = true;
        if (m_mapIndextoItem.find(stCurrentPosition) != m_mapIndextoItem.end())
        {
            ATLTRACE(L"COPCGroup::AddItems - Already has item with same server handle, pHr[%d] = ", i);
            pHr[i] = E_HANDLE;
            bSuccess = false;
            continue;
        }
        pNewItem->m_pParentGroup = this;
        m_mapIndextoItem[stCurrentPosition] = *pNewItem;
    }
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
    m_szItemName = pOPCItemDef->szItemID;
    m_szAccessPath = pOPCItemDef->szAccessPath;
    m_hServerItem = hServerItem;
    m_hClientItem = pOPCItemDef->hClient;
    m_bActive = pOPCItemDef->bActive;
    m_AsyncMask |= OPC_ODC_ANY;

    pOPCItemResult->hServer = m_hServerItem;
    pOPCItemResult->vtCanonicalDataType = m_vtCanonicalDataType;
    pOPCItemResult->dwAccessRights = OPC_READABLE | OPC_WRITEABLE;
    pOPCItemResult->pBlob = NULL;
    pOPCItemResult->dwBlobSize = 0;

    return S_OK;
}

//STDMETHODIMP COPCGroup::OnDataChange(
//    DWORD dwTransid,
//    OPCHANDLE hGroup,
//    HRESULT hrMasterquality,
//    HRESULT hrMastererror,
//    DWORD dwCount,
//    OPCHANDLE * phClientItems,
//    VARIANT * pvValues,
//    WORD * pwQualities,
//    FILETIME * pftTimeStamps,
//    HRESULT * pErrors)
//{
//    return E_NOTIMPL;
//}

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

LRESULT CWinHidden::OnTimer(UINT uMsg, WPARAM wParam,
    LPARAM lParam, BOOL& bHandled)
{
    if ((UINT)wParam != m_nTimer)
        return -1;

    if (m_pFullCtrl != NULL)
        m_pFullCtrl->OnTimer(uMsg, wParam, lParam, bHandled);
    return 0;
}
