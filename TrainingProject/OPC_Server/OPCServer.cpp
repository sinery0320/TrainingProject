// OPCServer.cpp : Implementation of COPCServer

#include "stdafx.h"
#include "OPCServer.h"


// COPCServer
STDMETHODIMP COPCServer::AddGroup(LPCWSTR szName, BOOL bActive, DWORD dwRequestedUpdateRate, OPCHANDLE hClientGroup, LONG * pTimeBias, FLOAT * pPercentDeadband, DWORD dwLCID, OPCHANDLE * phServerGroup, DWORD * pRevisedUpdateRate, REFIID riid, LPUNKNOWN * ppUnk)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCServer::GetErrorString(HRESULT dwError, LCID dwLocale, LPWSTR * ppString)
{
    // Add your function implementation here.
    return E_NOTIMPL;
}
STDMETHODIMP COPCServer::GetGroupByName(LPCWSTR szName, REFIID riid, LPUNKNOWN * ppUnk)
{
    // Add your function implementation here.
    return E_NOTIMPL;
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

