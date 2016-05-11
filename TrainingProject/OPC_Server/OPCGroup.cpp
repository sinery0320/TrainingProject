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
    *ppAddResults = pItemResult;
    *ppErrors = pHr;

    //return E_NOTIMPL;
}

