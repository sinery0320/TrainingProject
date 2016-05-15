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

STDMETHODIMP COPCGroup::OnDataChange(
    DWORD dwTransid,
    OPCHANDLE hGroup,
    HRESULT hrMasterquality,
    HRESULT hrMastererror,
    DWORD dwCount,
    OPCHANDLE * phClientItems,
    VARIANT * pvValues,
    WORD * pwQualities,
    FILETIME * pftTimeStamps,
    HRESULT * pErrors)
{

    Lock();
    return E_NOTIMPL;
}
