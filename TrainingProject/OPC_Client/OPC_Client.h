
// OPC_Client.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COPC_ClientApp:
// See OPC_Client.cpp for the implementation of this class
//

class COPC_ClientApp : public CWinApp
{
public:
	COPC_ClientApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern COPC_ClientApp theApp;