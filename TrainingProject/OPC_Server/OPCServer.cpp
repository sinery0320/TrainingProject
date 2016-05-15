// OPCServer.cpp : Implementation of COPCServer

#include "stdafx.h"
#include "OPCServer.h"
using namespace std;

// COPCServer
STDMETHODIMP COPCServer::AddGroup(LPCWSTR szName, BOOL bActive, DWORD dwRequestedUpdateRate, OPCHANDLE hClientGroup, LONG * pTimeBias, FLOAT * pPercentDeadband, DWORD dwLCID, OPCHANDLE * phServerGroup, DWORD * pRevisedUpdateRate, REFIID riid, LPUNKNOWN * ppUnk)
{
    // Add your function implementation here.
    if (phServerGroup == NULL || ppUnk == NULL)
    {
        ATLTRACE(L"IOPCServer::AddGroup() - phServerGroup or ppUnk is NULL, returning E_INVALIDARG");
        return E_INVALIDARG;
    }
    if (pPercentDeadband)
    {
        if (*pPercentDeadband < 0.0 || *pPercentDeadband > 100.0)
        {
            ATLTRACE(L"IOPCServer::AddGroup() - pPercentDeadband is out of range, returning E_INVALIDARG");
            return E_INVALIDARG;
        }
    }

    // default return value
    *phServerGroup = NULL;
    *ppUnk = NULL;

    HRESULT hr = S_OK;
    CComObject<COPCGroup> * pGroup = NULL;
    LPWSTR pGroupName = NULL;
    int currentPosition = 0;
    for (currentPosition = 0; currentPosition < GROUP_NUMBER; currentPosition++)
    {
        //if (m_pGroupObject[currentPosition]->m_bInUse == false)
        if (m_bGroupInUse[currentPosition] == false)
        {
            break;
        }
        else
        {
            continue;
        }
    }
    if (currentPosition >= GROUP_NUMBER)
    {
        ATLTRACE(L"IOPCServer::AddGroup() - No available resource of group, returning E_OUTOFMEMORY");
        return E_OUTOFMEMORY;
    }

    if (szName == NULL || *szName == (WCHAR)NULL)
    {
        // don't have group name;
        pGroupName = new WCHAR[32];
        if (pGroupName == NULL)
        {
            ATLTRACE(L"IOPCServer::AddGroup() - Failed to allocate memory of pGroupName, returning E_OUTOFMEMORY");  
            return E_OUTOFMEMORY;
        }
        else
        {
            // default group name.
            wsprintf(pGroupName, L"Group%d", currentPosition);
        }
    }
    else
    {
        //ATLTRACE(L"%d\n", wcslen(szName) + 1);
        pGroupName = new WCHAR[wcslen(szName) + 1];
        if (pGroupName == NULL)
        {
            ATLTRACE(L"IOPCServer::AddGroup() - Failed to allocate memory of pGroupName, returning E_OUTOFMEMORY");
            return E_OUTOFMEMORY;
        }
        else
        {
            wcscpy_s(pGroupName, wcslen(szName) + 1, szName);
        }
    }

    LPUNKNOWN test;
    hr = GetGroupByName(pGroupName, IID_IUnknown, &test);
    if (hr == S_OK)
    {
        test->Release();
        ATLTRACE(L"IOPCServer::AddGroup - Duplicate Group Name, returning OPC_E_DUPLICATENAME");
        return OPC_E_DUPLICATENAME;
    }

    // Assign value of new Group
    //m_pGroupObject[currentPosition]->m_bInUse = true;
    m_bGroupInUse[currentPosition] = true;
    // pGroupName is created by new, the release of m_wcSzName is in the destructor function
    hr = CComObject<COPCGroup>::CreateInstance(&pGroup);
    if (FAILED(hr))
    {
        ATLTRACE(L"IOPCServer::AddGroup - Failed to CreateInstance of pGroup, returning hr");
        return hr;
    }
    pGroup->m_wcSzName = pGroupName;
    pGroup->m_bActive = bActive;
    //m_pGroupObject[currentPosition]->m_wcSzName = pGroupName;
    //m_pGroupObject[currentPosition]->m_bActive = bActive;

    if (dwRequestedUpdateRate > MIN_UPDATE_RATE)
    {
        pGroup->m_dwUpdateRate = MIN_UPDATE_RATE;
    }
    else if (dwRequestedUpdateRate >= 0 && dwRequestedUpdateRate <= MIN_UPDATE_RATE)
    {
        pGroup->m_dwUpdateRate = dwRequestedUpdateRate;
    }
    if (pRevisedUpdateRate)
    {
        *pRevisedUpdateRate = pGroup->m_dwUpdateRate;
    }
    pGroup->m_hServerGroup = currentPosition;
    pGroup->m_hClientGroup = hClientGroup;
    *phServerGroup = (OPCHANDLE)pGroup;
    if (pTimeBias)
    {
        pGroup->m_lTimeBias = *pTimeBias;
    }
    else
    {
        TIME_ZONE_INFORMATION timeZoneInfo;
        if (GetTimeZoneInformation(&timeZoneInfo) != TIME_ZONE_ID_INVALID)
        {
            pGroup->m_lTimeBias = timeZoneInfo.Bias;
        }
        else
        {
            pGroup->m_lTimeBias = 0L;
        }
    }
    if (pPercentDeadband)
    {
        pGroup->m_percentDeadband = *pPercentDeadband;
    }
    else
    {
        pGroup->m_percentDeadband = 0.0;
    }
    pGroup->m_dwLCID = dwLCID;
    pGroup->m_pParent = this;
    if (pRevisedUpdateRate)
    {
        *pRevisedUpdateRate = pGroup->m_dwUpdateRate;
    }
    hr = pGroup->QueryInterface(riid, (LPVOID*)ppUnk);
    if (FAILED(hr))
    {
        ATLTRACE(L"IOPCServer::AddGroup - Failed to query interface, returning hr");
        return hr;
    }
    m_mapNametoGroup[szName] = *pGroup;
    return S_OK;
    //return E_NOTIMPL;

}
STDMETHODIMP COPCServer::GetErrorString(HRESULT dwError, LCID dwLocale, LPWSTR * ppString)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCServer::GetGroupByName(LPCWSTR szName, REFIID riid, LPUNKNOWN * ppUnk)
{
    // Add your function implementation here.
    HRESULT hrFindResult;
    *ppUnk = 0;
    if (szName == NULL || *szName == (WCHAR)NULL)
    {
        ATLTRACE(L"IOPCServer::GetGroupByName - szName is empty, returning E_INVALIDARG");
        return E_INVALIDARG;
    }
    if (riid == IID_NULL)
    {
        ATLTRACE(L"IOPCServer::GetGroupByName - riid is IID_NULL, returning E_INVALIDARG");
        return E_INVALIDARG;
    }
    if (m_mapNametoGroup.find(szName) != m_mapNametoGroup.end())
    {
        hrFindResult = m_mapNametoGroup[szName].QueryInterface(riid, (LPVOID*)ppUnk);
        if (FAILED(hrFindResult))
        {
            return hrFindResult;
        }
        else
        {
            return S_OK;
        }
    }
    else
    {
        return E_FAIL;
    }
    // Useless code
    //for (size_t i = 0; i <szName GROUP_NUMBER; i++)
    //{
    //    if (m[i]->m_bInUse == false)
    //    {
    //        continue;
    //    }
    //    if (wcscmp(m_pGroupObject[i]->m_wcSzName, szName) == 0)
    //    {
    //        hrFindResult = m_pGroupObject[i]->QueryInterface(riid, (LPVOID*)ppUnk);
    //        if (FAILED(hrFindResult))
    //        {
    //            return hrFindResult;
    //        }
    //        else
    //        {
    //            return S_OK;
    //        }
    //    }
    //}
    //return E_FAIL;
    //return E_NOTIMPL;
}
STDMETHODIMP COPCServer::GetStatus(OPCSERVERSTATUS ** ppServerStatus)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCServer::RemoveGroup(OPCHANDLE hServerGroup, BOOL bForce)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCServer::CreateGroupEnumerator(OPCENUMSCOPE dwScope, REFIID riid, LPUNKNOWN * ppUnk)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}

