// Signal.cpp : Implementation of CSignal

#include "stdafx.h"
#include "Signal.h"


// CSignal

// CSignal::GenerateSignal - Generate signals.
// DOUBLE* pdCurrentSignalValue: Array to store signals
// LONG nSignal: Number of signals
STDMETHODIMP CSignal::GenerateSignal(/* [out] */DOUBLE* pdCurrentSignalValue, /* [in] */LONG nSignal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // TODO: Add your implementation code here
    // Check Validation of arguments
    if (pdCurrentSignalValue == NULL || nSignal == 0)
    {
        return E_INVALIDARG;
    }

    // Local variables
    clock_t lCurrentRunTime = 0;
    int nSampleRate = 200;
    long lSampledTime = 0;

    lCurrentRunTime = clock();// Get current run time of program

    lSampledTime = lCurrentRunTime / nSampleRate * nSampleRate;// Get sample time with 200ms unit

    if (nSignal == 3)// Get signals
    {
        pdCurrentSignalValue[0] = GenerateSignalY1(lSampledTime);
        pdCurrentSignalValue[1] = GenerateSignalY2(lSampledTime);
        pdCurrentSignalValue[2] = GenerateSignalY3(lSampledTime);
    }
    
    return S_OK;
}

double CSignal::GenerateSignalY1(clock_t lCurrentRunTime)
{
    int K1 = 50;
    int T1 = 2000;
    return K1 * sin(1.0 * lCurrentRunTime / T1);
}

double CSignal::GenerateSignalY2(clock_t lCurrentRunTime)
{
    int K2 = 50;
    int T2 = 12000;
    double flag = 1.0 * (lCurrentRunTime % T2) / T2;
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

double CSignal::GenerateSignalY3(clock_t lCurrentRunTime)
{
    int K3 = 50;
    int T3 = 12000;
    double flag = 1.0 * (lCurrentRunTime % T3) / T3;
    // Calculate cycle
    int N = (lCurrentRunTime + T3 / 2) / T3;
    if (flag >= 0 && flag < 0.5)
    {
        return K3 - (2.0 * K3 / T3 * lCurrentRunTime - 2.0 * N * K3);
    }
    else if (flag >= 0.5 && flag < 1)
    {
        return K3 + (2.0 * K3 / T3 * lCurrentRunTime - 2.0 * N * K3);
    }
    else
    {
        return 0.0;
    }
}
