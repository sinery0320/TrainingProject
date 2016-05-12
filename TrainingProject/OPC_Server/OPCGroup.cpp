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
    bool bSuccess = false;
    COPCItem * pNewItem = NULL;
    //*ppAddResults = pItemResult;
    //*ppErrors = pHr;
    pItemResult = new OPCITEMRESULT[dwCount];
    if (!pItemResult)
    {
        ATLTRACE(L"COPCGroup::AddItems - New operation of pItemResult failed, returning E_OUTOFMEMORY");
        return E_OUTOFMEMORY;
    }

    pHr = new HRESULT[dwCount];
    if (!pHr)
    {
        ATLTRACE(L"COPCGroup::AddItems - New operation of pHr failed, returning E_OUTOFMEMORY");
        return E_OUTOFMEMORY;
    }

    *ppAddResults = pItemResult;
    *ppErrors = pHr;
    for (size_t i = 0; i < dwCount; i++)
    {
        
    }
    return E_NOTIMPL;
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

