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
    SYSTEMTIME * sysTime = NULL;
    for (size_t i = 0; i<dwCount; i++)
    {
        cout << pvValues[i].dblVal << endl;
        cout << GetQualityText(pwQualities[i]) << endl;
        FileTimeToSystemTime(&pftTimeStamps[i], sysTime);
        cout << "UTC System Time2 (YYYY-MM-DD HH:MM:SS):" + sysTime->wYear + sysTime->wMonth + sysTime->wDay + sysTime->wHour + sysTime->wMinute + sysTime->wSecond << endl;
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

CString OPCDataCallBackSink::GetQualityText(UINT qnr)
{
    CString qstr;

    switch (qnr)

    {
    case OPC_QUALITY_BAD:           qstr = "BAD";
        break;
    case OPC_QUALITY_UNCERTAIN:     qstr = "UNCERTAIN";
        break;
    case OPC_QUALITY_GOOD:          qstr = "GOOD";
        break;
    case OPC_QUALITY_NOT_CONNECTED: qstr = "NOT_CONNECTED";
        break;
    case OPC_QUALITY_DEVICE_FAILURE:qstr = "DEVICE_FAILURE";
        break;
    case OPC_QUALITY_SENSOR_FAILURE:qstr = "SENSOR_FAILURE";
        break;
    case OPC_QUALITY_LAST_KNOWN:    qstr = "LAST_KNOWN";
        break;
    case OPC_QUALITY_COMM_FAILURE:  qstr = "COMM_FAILURE";
        break;
    case OPC_QUALITY_OUT_OF_SERVICE:qstr = "OUT_OF_SERVICE";
        break;
    case OPC_QUALITY_LAST_USABLE:   qstr = "LAST_USABLE";
        break;
    case OPC_QUALITY_SENSOR_CAL:    qstr = "SENSOR_CAL";
        break;
    case OPC_QUALITY_EGU_EXCEEDED:  qstr = "EGU_EXCEEDED";
        break;
    case OPC_QUALITY_SUB_NORMAL:    qstr = "SUB_NORMAL";
        break;
    case OPC_QUALITY_LOCAL_OVERRIDE:qstr = "LOCAL_OVERRIDE";
        break;
    default:                        qstr = "UNKNOWN ERROR";
    }
    return qstr;
}