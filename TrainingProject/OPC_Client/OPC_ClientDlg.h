
// OPC_ClientDlg.h : header file
//

#pragma once
#define ITEM_NUMBER                     3
#define UPDATE_RATE                     100
#include "OPC_Server_i.h"

using namespace std;
// COPC_ClientDlg dialog
class COPC_ClientDlg : public CDialogEx
{
// Construction
public:
	COPC_ClientDlg(CWnd* pParent = NULL);	// standard constructor
    void DrawPlot(int index);

// Dialog Data
	enum { IDD = IDD_OPC_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    enum{ITEMY1 = 0, ITEMY2 = 1, ITEMY3 = 2};
    CComPtr<IOPCServer> m_pIOPCServer;
    CWnd * m_pCWnd[ITEM_NUMBER];        // Item draw window
    CRect m_CRect[ITEM_NUMBER];         // Item draw rectangle
    CDC * m_pCDC[ITEM_NUMBER];          // Item draw DC

    list<double> m_listY[ITEM_NUMBER];  // list for item value
    
};
