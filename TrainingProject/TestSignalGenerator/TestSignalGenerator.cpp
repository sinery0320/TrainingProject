// TestSignalGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define SIGNAL_NUMBER 3
using namespace std;

int main(int argc, _TCHAR* argv[])
{
    argc;
    argv;
    HRESULT hr;
    ISignal *pSignal = NULL;// Get ISignal pointer
    ofstream ofSignalGenerateLog("SignalGenerate.log");// Output log
    if (!ofSignalGenerateLog)
    {
        cout << "Open Log file Failure!" << endl;
    }
    // Initialize COM
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        cout << "COM Initialization Failed" << endl;
        ofSignalGenerateLog << "COM Initialization Failed" << endl;
    }
    // Create COM object
    hr = CoCreateInstance(CLSID_Signal, NULL, CLSCTX_INPROC_SERVER, IID_ISignal, (void**)&pSignal);
    if (FAILED(hr))
    {
        cout << "Failed to Create COM Instance" << endl;
        ofSignalGenerateLog << "Failed to Create COM Instance" << endl;
    }
    //Using dValue to get the return value of COM signal function
    double dValue[SIGNAL_NUMBER] = {0.0};
    //5Hz sample time.
    clock_t lSampleTarget = 0;
    if (SUCCEEDED(hr)) {
        while (true) {
            //Get tick time.
            lSampleTarget = clock();
            //Read value per 200ms.
            if (lSampleTarget % 200 == 0)
            {
                cout << lSampleTarget << ' ';
                ofSignalGenerateLog << lSampleTarget << ' ';
                //Get value from COM ISignal via Generate Signal function.
                pSignal->GenerateSignal(dValue, SIGNAL_NUMBER);
                for (int i = 0; i < SIGNAL_NUMBER; ++i)
                {
                    cout << dValue[i] << ' ';
                    ofSignalGenerateLog << dValue[i] << ' ';
                }
                cout << endl;
                ofSignalGenerateLog << endl;
            }
            //else
            //{
            //    Sleep(1);
            //}
            //Total run time.
            if (lSampleTarget >= 24000)
            {
                break;
            }
        }
    }
    //Release the pointer.
    ofSignalGenerateLog.close();
	return 0;
}

