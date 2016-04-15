// Signal.h : CSignal ������

#pragma once
#include "resource.h"       // ������



#include "SignalGenerator_i.h"
#include "_ISignalEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CSignal

class ATL_NO_VTABLE CSignal :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSignal, &CLSID_Signal>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CSignal>,
	public CProxy_ISignalEvents<CSignal>,
	public IDispatchImpl<ISignal, &IID_ISignal, &LIBID_SignalGeneratorLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSignal()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SIGNAL)


BEGIN_COM_MAP(CSignal)
	COM_INTERFACE_ENTRY(ISignal)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CSignal)
	CONNECTION_POINT_ENTRY(__uuidof(_ISignalEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:


    //Signal Generate Interface Function.
    STDMETHOD(SigGen)(LONG valNum, DOUBLE* funCurValue);

private:
    //Signal Generate Function.
    double signalY1(clock_t currentRunTime);
    double signalY2(clock_t currentRunTime);
    double signalY3(clock_t currentRunTime);
};

OBJECT_ENTRY_AUTO(__uuidof(Signal), CSignal)