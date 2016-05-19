#pragma once
#include "OPC_Server_i.h"
//#include <atlbase.h>
//#include <atlcom.h>
using namespace std;
class ClientDataCallbackSink:
    public IOPCDataCallback
{
private:
    ULONG m_nRef;
    list<double> * m_listY;
    CWnd * m_pWnd;
public:
    ClientDataCallbackSink(list<double> * value, CWnd * pWnd);
    ~ClientDataCallbackSink();
    STDMETHOD(QueryInterface)(_In_ REFIID iid, _Outptr_ void** pp);
    ULONG __stdcall AddRef(void);
    ULONG __stdcall Release(void);

    STDMETHOD(OnDataChange)(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors);

    STDMETHOD(OnReadComplete)(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMasterquality,
        /* [in] */ HRESULT hrMastererror,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *phClientItems,
        /* [size_is][in] */ VARIANT *pvValues,
        /* [size_is][in] */ WORD *pwQualities,
        /* [size_is][in] */ FILETIME *pftTimeStamps,
        /* [size_is][in] */ HRESULT *pErrors);

    STDMETHOD(OnWriteComplete)(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup,
        /* [in] */ HRESULT hrMastererr,
        /* [in] */ DWORD dwCount,
        /* [size_is][in] */ OPCHANDLE *pClienthandles,
        /* [size_is][in] */ HRESULT *pErrors);

    STDMETHOD(OnCancelComplete)(
        /* [in] */ DWORD dwTransid,
        /* [in] */ OPCHANDLE hGroup);

    STDMETHOD(GetQualityText)(UINT qnr, WCHAR* wcQualityText);
};