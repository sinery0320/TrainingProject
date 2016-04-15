// Signal.cpp : CSignal 的实现

#include "stdafx.h"
#include "Signal.h"


// CSignal

STDMETHODIMP CSignal::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ISignal
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CSignal::SigGen(LONG valNum, DOUBLE* funCurValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: 在此添加实现代码
    clock_t currentRunTime;
    int sampleRate = 200;
    long sampledTime = 0;

    currentRunTime = clock();
    //Get sample time with 200ms unit.
    sampledTime = currentRunTime / sampleRate * sampleRate;

    //for (int i = 0; i < valNum; i++)
    //{
    //    funCurValue[i] = 0.1 * (i + 1) * sampledTime;
    //}
    //Call signal generating functions.
    funCurValue[0] = signalY1(sampledTime);
    funCurValue[1] = signalY2(sampledTime);
    funCurValue[2] = signalY3(sampledTime);

    return S_OK;
}

double CSignal::signalY1(clock_t currentRunTime)
{
    int K1 = 50;
    int T1 = 2000;
    return K1 * sin(1.0 * currentRunTime / T1);
}

double CSignal::signalY2(clock_t currentRunTime)
{
    int K2 = 50;
    int T2 = 12000;
    double flag = 1.0 * (currentRunTime % T2) / T2;
    if (flag >= 0 && flag < 0.5)
    {
        return K2;
    }
    else if (flag >= 0.5 && flag < 1)
    {
        return -K2;
    }
    else
    {
        return 0.0;
    }
}

double CSignal::signalY3(clock_t currentRunTime)
{
    int K3 = 50;
    int T3 = 12000;
    double flag = 1.0 * (currentRunTime % T3) / T3;
    //Calculate cycle.
    int N = (currentRunTime + T3 / 2) / T3;
    if (flag >= 0 && flag < 0.5)
    {
        return K3 - (2.0 * K3 / T3 * currentRunTime - 2.0 * N * K3);
    }
    else if (flag >= 0.5 && flag < 1)
    {
        return K3 + (2.0 * K3 / T3 * currentRunTime - 2.0 * N * K3);
    }
    else
    {
        return 0.0;
    }
}
