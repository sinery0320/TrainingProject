// dllmain.cpp: DllMain ��ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "SignalGenerator_i.h"
#include "dllmain.h"
#include "compreg.h"

CSignalGeneratorModule _AtlModule;

class CSignalGeneratorApp : public CWinApp
{
public:

// ��д
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CSignalGeneratorApp, CWinApp)
END_MESSAGE_MAP()

CSignalGeneratorApp theApp;

BOOL CSignalGeneratorApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CSignalGeneratorApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
