// TestSignal.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define SIG_NUM 3
using namespace std;


int main()
{
    HRESULT hr;
    //Get ISignal pointer.
    ISignal *pSignal = NULL;
    //Initialize COM.
    hr = CoInitialize(NULL);
    //Create COM object.
    hr = CoCreateInstance(CLSID_Signal, NULL, 1, IID_ISignal, (void**) &pSignal);
    //Using value to get the return value of COM function.
    double *value = new double[3];
    //5Hz sample time.
    clock_t sampleTarget = 0;
    //Output log.
    ofstream valLog("Value.log");
    if (!valLog)
    {
        cout << "Open Log file Failure!" << endl;
    }
    if (SUCCEEDED(hr)) {
        while (true) {
            //Get tick time.
            sampleTarget = clock();
            //Read value per 200ms.
            if (sampleTarget % 200 == 0) {
                cout << sampleTarget << ' ';
                valLog << sampleTarget << ' ';
                //Get value from COM ISignal via SigGen function.
                pSignal->SigGen(SIG_NUM, value);
                for (int i = 0; i < SIG_NUM; ++i)
                {
                    cout << value[i] << ' ';
                    valLog << value[i] << ' ';
                }
                cout << endl;
                valLog << endl;
            }
            //Total run time.
            if (sampleTarget >= 24000)
            {
                break;
            }
        }
    }
    //Release the pointer.
    value = NULL;
    valLog.close();
    return 0;
}

