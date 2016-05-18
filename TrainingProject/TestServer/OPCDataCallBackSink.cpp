#include "stdafx.h"
#include "OPCDataCallBackSink.h"

using namespace ATL;
using namespace std;


OPCDataCallBackSink::OPCDataCallBackSink()
{
    m_nRef = 0;
}


OPCDataCallBackSink::~OPCDataCallBackSink()
{
}

STDMETHODIMP OPCDataCallBackSink::QueryInterface(_In_ REFIID iid, _Outptr_ void** pp)
{
    if (iid == IID_IUnknown)
    {
        *pp = static_cast<IUnknown*>(this);
        AddRef();
    }

    else if (iid == IID_IOPCDataCallback)
    {
        *pp = static_cast<IOPCDataCallback*>(this);
        AddRef();
    }
    else
    {
        *pp = NULL;
        return E_NOINTERFACE;
    }
    return S_OK;
}
ULONG __stdcall OPCDataCallBackSink::AddRef(void)
{
    return ++m_nRef;
}
ULONG __stdcall OPCDataCallBackSink::Release(void)
{
    --m_nRef;
    if (m_nRef == 0)
    {
        delete this;
        return 0;
    }
    return m_nRef;
}
STDMETHODIMP OPCDataCallBackSink::OnDataChange(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE *phClientItems,
    /* [size_is][in] */ VARIANT *pvValues,
    /* [size_is][in] */ WORD *pwQualities,
    /* [size_is][in] */ FILETIME *pftTimeStamps,
    /* [size_is][in] */ HRESULT *pErrors)
{
    //SYSTEMTIME sysTime;
    cout << "Calling OnDataChange" << endl;
    for (size_t i = 0; i<dwCount; i++)
    {
        cout << pvValues[i].dblVal << endl;
        //WCHAR * wcQualityText = new WCHAR[10];
        //GetQualityText(pwQualities[i], wcQualityText);
        //cout << pwQualities[i] << endl;
        //FileTimeToSystemTime(&pftTimeStamps[i], &sysTime);
        //wprintf(L"UTC System Time2 (%u-%u-%u %u:%u:%u)\n", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    }
    return S_OK;
    //return E_NOTIMPL;
}

STDMETHODIMP OPCDataCallBackSink::OnReadComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE *phClientItems,
    /* [size_is][in] */ VARIANT *pvValues,
    /* [size_is][in] */ WORD *pwQualities,
    /* [size_is][in] */ FILETIME *pftTimeStamps,
    /* [size_is][in] */ HRESULT *pErrors)
{
    return E_NOTIMPL;
}

STDMETHODIMP OPCDataCallBackSink::OnWriteComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMastererr,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE *pClienthandles,
    /* [size_is][in] */ HRESULT *pErrors)
{
    return E_NOTIMPL;
}

STDMETHODIMP OPCDataCallBackSink::OnCancelComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup)
{
    return E_NOTIMPL;
}

STDMETHODIMP OPCDataCallBackSink::GetQualityText(UINT qnr, WCHAR* wcQualityText)
{
    switch (qnr)

    {
    case OPC_QUALITY_BAD:           wcQualityText = L"BAD";
        break;
    case OPC_QUALITY_UNCERTAIN:     wcQualityText = L"UNCERTAIN";
        break;
    case OPC_QUALITY_GOOD:          wcQualityText = L"GOOD";
        break;
    case OPC_QUALITY_NOT_CONNECTED: wcQualityText = L"NOT_CONNECTED";
        break;
    case OPC_QUALITY_DEVICE_FAILURE:wcQualityText = L"DEVICE_FAILURE";
        break;
    case OPC_QUALITY_SENSOR_FAILURE:wcQualityText = L"SENSOR_FAILURE";
        break;
    case OPC_QUALITY_LAST_KNOWN:    wcQualityText = L"LAST_KNOWN";
        break;
    case OPC_QUALITY_COMM_FAILURE:  wcQualityText = L"COMM_FAILURE";
        break;
    case OPC_QUALITY_OUT_OF_SERVICE:wcQualityText = L"OUT_OF_SERVICE";
        break;
    case OPC_QUALITY_LAST_USABLE:   wcQualityText = L"LAST_USABLE";
        break;
    case OPC_QUALITY_SENSOR_CAL:    wcQualityText = L"SENSOR_CAL";
        break;
    case OPC_QUALITY_EGU_EXCEEDED:  wcQualityText = L"EGU_EXCEEDED";
        break;
    case OPC_QUALITY_SUB_NORMAL:    wcQualityText = L"SUB_NORMAL";
        break;
    case OPC_QUALITY_LOCAL_OVERRIDE:wcQualityText = L"LOCAL_OVERRIDE";
        break;
    default:                        wcQualityText = L"UNKNOWN ERROR";
    }
    return S_OK;
}