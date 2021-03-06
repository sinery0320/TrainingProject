
// OPC_ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPC_Client.h"
#include "OPC_ClientDlg.h"
#include "afxdialogex.h"
#include "OPC_Server_i.c"
//#include "ClientDataCallbackSink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COPC_ClientDlg dialog



COPC_ClientDlg::COPC_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COPC_ClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pIOPCServer = NULL;
    m_pIUnk = NULL;
    m_pIOPCItemMgt = NULL;
    m_pConnectionPointContainer = NULL;
    m_pConnectionPoint = NULL;
    m_pSink = NULL;
    m_dwCookie = 0;
}
COPC_ClientDlg::~COPC_ClientDlg()
{
    for (size_t i = 0; i < ITEM_NUMBER; i++)
    {
        if (!m_listY[i].empty())
        {
            m_listY[i].clear();
        }
    }
    if (m_pConnectionPoint != NULL)
    {
        m_pConnectionPoint->Unadvise(m_dwCookie);
        if (m_pSink != NULL)
        {
            delete m_pSink;
            m_pSink = NULL;
        }
        delete m_pConnectionPoint;
        m_pConnectionPoint = NULL;
    }
    m_pConnectionPointContainer = NULL;
    m_pIOPCItemMgt = NULL;
    m_pIOPCServer = NULL;
    m_pIUnk = NULL;
}

void COPC_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COPC_ClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &COPC_ClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COPC_ClientDlg message handlers

BOOL COPC_ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    //HRESULT hr = S_OK;
    //IUnknown * pIUnk = NULL;
    //CLSID clsid;
    //// COM initialize
    //hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"COM Initialized succeessfully\n");
    //}
    //else
    //{
    //    TRACE(L"COM Initialized failed\n");
    //    return FALSE;
    //}
    //// Get CLSID
    //hr = CLSIDFromProgID(L"TrainingProject.OPC_Server.OPCServer.1", &clsid);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"Get CLSID succeessfully\n");
    //}
    //else
    //{
    //    TRACE(L"Get CLSID failed\n");
    //    return FALSE;
    //}
    //// Get OPC server's IUnknown interface
    //hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IUnknown, (LPVOID*)&pIUnk);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"OPCServer Initialization successfully!\n");
    //}
    //else
    //{
    //    TRACE(L"OPCServer Initialization failed!\n");
    //}
    //// Query IOPCServer interface
    //hr = pIUnk->QueryInterface(IID_IOPCServer, (LPVOID*)&m_pIOPCServer);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"Get OPCServer interface successfully!\n");
    //}
    //else
    //{
    //    TRACE(L"Get OPCServer interface failed!\n");
    //    if (pIUnk != NULL)
    //    {
    //        pIUnk->Release();
    //    }
    //    return FALSE;
    //}
    //LONG lTimeBias = 0;
    //FLOAT dDeadBand = 0.0;
    //OPCHANDLE phServerGroup;
    //DWORD dwUpdateRate = 0;
    //IOPCItemMgt * pIOPCItemMgt = NULL;
    //DWORD dwLCID = 2052;
    //// Add Group
    //hr = m_pIOPCServer->AddGroup(L"Group1", true, 200, 1, &lTimeBias, &dDeadBand, dwLCID, &phServerGroup, &dwUpdateRate, IID_IOPCItemMgt, (LPUNKNOWN*)&pIOPCItemMgt);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"Call AddGroup function successfully!\n");
    //}
    //else
    //{
    //    TRACE(L"Call AddGroup function failed!\n");
    //    IOPCServer * tempIOPCServer = m_pIOPCServer;
    //    if (tempIOPCServer != NULL)
    //    {
    //        m_pIOPCServer = NULL;
    //        tempIOPCServer->Release();
    //        tempIOPCServer = NULL;
    //    }
    //    if (pIUnk != NULL)
    //    {
    //        pIUnk->Release();
    //    }
    //    return FALSE;
    //}
    //// Add connection point container
    //IConnectionPointContainer * pConnectionPointContainer = NULL;
    //hr = pIOPCItemMgt->QueryInterface(IID_IConnectionPointContainer, (void **)&pConnectionPointContainer);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"Get IConnectionPointContainer interface successfully.\n");
    //}
    //else
    //{
    //    TRACE(L"Get IConnectionPointContainer interface failed.\n");
    //    if (pIOPCItemMgt != NULL)
    //    {
    //        pIOPCItemMgt->Release();
    //    }
    //    IOPCServer * tempIOPCServer = m_pIOPCServer;
    //    if (tempIOPCServer != NULL)
    //    {
    //        m_pIOPCServer = NULL;
    //        tempIOPCServer->Release();
    //        tempIOPCServer = NULL;
    //    }
    //    if (pIUnk != NULL)
    //    {
    //        pIUnk->Release();
    //    }
    //    return FALSE;
    //}
    //// Add connection point
    //IConnectionPoint * pConnectionPoint = NULL;
    //hr = pConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &pConnectionPoint);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"Get ConnettionPoint successfully.\n");
    //}
    //else
    //{
    //    TRACE(L"Get ConnettionPoint failed.\n");
    //    if (pConnectionPointContainer != NULL)
    //    {
    //        pConnectionPointContainer->Release();
    //    }
    //    if (pIOPCItemMgt != NULL)
    //    {
    //        pIOPCItemMgt->Release();
    //    }
    //    IOPCServer * tempIOPCServer = m_pIOPCServer;
    //    if (tempIOPCServer != NULL)
    //    {
    //        m_pIOPCServer = NULL;
    //        tempIOPCServer->Release();
    //        tempIOPCServer = NULL;
    //    }
    //    if (pIUnk != NULL)
    //    {
    //        pIUnk->Release();
    //    }
    //    return FALSE;
    //}
    //// Advise
    //ClientDataCallbackSink * pSink = NULL;
    //pSink = new ClientDataCallbackSink(m_listY, this);
    //DWORD dwCookie = 0;
    //if (pSink != NULL)
    //{
    //    hr = pConnectionPoint->Advise(pSink, &dwCookie);
    //    //cout << hr << endl;
    //    if (SUCCEEDED(hr))
    //    {
    //        TRACE(L"Advise successfully\n");
    //    }
    //    else
    //    {
    //        TRACE(L"Advise failed\n");
    //        if (pConnectionPoint != NULL)
    //        {
    //            pConnectionPoint->Release();
    //        }
    //        if (pConnectionPointContainer != NULL)
    //        {
    //            pConnectionPointContainer->Release();
    //        }
    //        if (pIOPCItemMgt != NULL)
    //        {
    //            pIOPCItemMgt->Release();
    //        }
    //        IOPCServer * tempIOPCServer = m_pIOPCServer;
    //        if (tempIOPCServer != NULL)
    //        {
    //            m_pIOPCServer = NULL;
    //            tempIOPCServer->Release();
    //            tempIOPCServer = NULL;
    //        }
    //        if (pIUnk != NULL)
    //        {
    //            pIUnk->Release();
    //        }
    //        return FALSE;
    //    }
    //}
    //// Add item
    //OPCITEMDEF * opcItem = (OPCITEMDEF*)CoTaskMemAlloc(3 * sizeof(OPCITEMDEF));
    //opcItem[0] = { NULL, L"ItemY1", TRUE, 0, 0, NULL, VT_R8, 0 };
    //opcItem[1] = { NULL, L"ItemY2", TRUE, 0, 0, NULL, VT_R8, 0 };
    //opcItem[2] = { NULL, L"ItemY3", TRUE, 0, 0, NULL, VT_R8, 0 };
    //OPCITEMRESULT * opcItemResult = NULL;
    //HRESULT * errorResult = NULL;
    //hr = pIOPCItemMgt->AddItems(3, opcItem, &opcItemResult, &errorResult);
    //if (SUCCEEDED(hr))
    //{
    //    TRACE(L"AddItems successfully!\n");
    //}
    //else
    //{
    //    TRACE(L"AddItems failed!\n");
    //    if (pConnectionPoint != NULL)
    //    {
    //        pConnectionPoint->Release();
    //    }
    //    if (pConnectionPointContainer != NULL)
    //    {
    //        pConnectionPointContainer->Release();
    //    }
    //    if (pIOPCItemMgt != NULL)
    //    {
    //        pIOPCItemMgt->Release();
    //    }
    //    IOPCServer * tempIOPCServer = m_pIOPCServer;
    //    if (tempIOPCServer != NULL)
    //    {
    //        m_pIOPCServer = NULL;
    //        tempIOPCServer->Release();
    //        tempIOPCServer = NULL;
    //    }
    //    if (pIUnk != NULL)
    //    {
    //        pIUnk->Release();
    //    }
    //    return FALSE;
    //}
    // Draw initialize
    m_pCWnd[ITEMY1] = GetDlgItem(IDC_STATIC4);
    m_pCWnd[ITEMY2] = GetDlgItem(IDC_STATIC5);
    m_pCWnd[ITEMY3] = GetDlgItem(IDC_STATIC6);

    m_pCWnd[ITEMY1]->GetWindowRect(m_CRect[ITEMY1]);
    m_pCWnd[ITEMY2]->GetWindowRect(m_CRect[ITEMY2]);
    m_pCWnd[ITEMY3]->GetWindowRect(m_CRect[ITEMY3]);

    m_pCDC[ITEMY1] = m_pCWnd[ITEMY1]->GetDC();
    m_pCDC[ITEMY2] = m_pCWnd[ITEMY2]->GetDC();
    m_pCDC[ITEMY3] = m_pCWnd[ITEMY3]->GetDC();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COPC_ClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
        // Draw three plots
        DrawPlot(1);
        DrawPlot(2);
        DrawPlot(3);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COPC_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// COPC_ClientDlg::DrawPlot - Draw plot.
void COPC_ClientDlg::DrawPlot(int index)
{
    // Refresh value text
    WCHAR * wcText = new WCHAR[20];
    if (m_listY[index - 1].size() > 0)
    {
        swprintf(wcText, 20, L"%f", *(m_listY[index - 1].rbegin()));
        if (index == 1)
        {
            SetDlgItemTextW(IDC_STATIC1, wcText);
        }
        else if (index == 2)
        {
            SetDlgItemTextW(IDC_STATIC2, wcText);
        }
        else if (index == 3)
        {
            SetDlgItemTextW(IDC_STATIC3, wcText);
        }
    }
    // Prepare for drawing
    int width = m_CRect[index - 1].Width();
    int height = m_CRect[index - 1].Height();
    CDC MemDC;
    CBitmap MemBitmap;
    CPen newPen;
    CPen * pOldPen;
    CBrush newBrush;
    CBrush * pOldBrush;
    MemDC.CreateCompatibleDC(m_pCDC[index - 1]);
    MemBitmap.CreateCompatibleBitmap(m_pCDC[index - 1], width, height);
    CBitmap * pOldBit = MemDC.SelectObject(&MemBitmap);

    // Clear background
    MemDC.FillSolidRect(0, 0, width, height, RGB(0, 0, 0));
    //MemDC.FillSolidRect(0, 0, width, height, RGB((index == 1) * 255, (index == 2) * 255, (index == 3) * 255));
    
    // Draw standard line
    newBrush.CreateSolidBrush(RGB(0, 0, 0));
    pOldBrush = MemDC.SelectObject(&newBrush);
    MemDC.Rectangle(m_CRect);
    MemDC.SelectObject(pOldBrush);
    newBrush.DeleteObject();
    newPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    pOldPen = MemDC.SelectObject(&newPen);
    MemDC.MoveTo(0, height / 2);
    MemDC.LineTo(width, height / 2);
    MemDC.MoveTo(0, 0.1 * height);
    MemDC.LineTo(width, 0.1 * height);
    MemDC.MoveTo(0, 0.9 * height);
    MemDC.LineTo(width, 0.9 * height);
    MemDC.SelectObject(pOldPen);
    newPen.DeleteObject();
    newPen.CreatePen(PS_SOLID, 2, RGB((index == 1 || index ==2) * 255, (index == 2 || index == 3) * 255, (index == 1 || index == 3) * 255));
    pOldPen = MemDC.SelectObject(&newPen);

    // Draw plot with 240 points
    int nScale = 100;
    int nPointCount = 240;
    if (m_listY[index - 1].size() > 0)
    {
        if (m_listY[index - 1].size() > nPointCount)
        {
            for (size_t i = 0; i < m_listY[index - 1].size() - nPointCount; i++)
            {
                m_listY[index - 1].erase(m_listY[index - 1].begin());
            }
        }
        int nXCount = 0;
        list<double>::iterator iterEnd = m_listY[index - 1].end();
        iterEnd--;
        for (list<double>::iterator iter = m_listY[index - 1].begin(); iter != iterEnd;)
        {
            int nXSource = nXCount * width / nPointCount;
            int nYSource = (*iter * 0.8 + nScale / 2) * height / nScale;
            iter++;
            nXCount++;
            int nXDest = nXCount * width / nPointCount;
            int nYDest = (*iter * 0.8 + nScale / 2) * height / nScale;
            MemDC.MoveTo(nXSource, height - nYSource);
            MemDC.LineTo(nXDest, height - nYDest);
        }
        //int ny = (*(m_listY[index - 1].rbegin()) + nScale / 2) * height / nScale;
        //MemDC.MoveTo(0, height - ny);
        //MemDC.LineTo(width, height - ny);
    }
    MemDC.SelectObject(pOldPen);
    newPen.DeleteObject();
    // Copy new bitmap
    m_pCDC[index - 1]->BitBlt(0, 0, width, height, &MemDC, 0, 0, SRCCOPY);
    MemBitmap.DeleteObject();
    MemDC.DeleteDC();
    return;
}

//void COPC_ClientDlg::OnTimer(UINT nIDEvent)
//{
//    
//
//
//    CRect I_rect;
//    GetClientRect(&I_rect);
//    m_pCDC = this->GetDC();
//
//    CBitmap newBitmap;
//    CBitmap * pOldBitmap = NULL;
//    
//    m_CDC.CreateCompatibleDC(m_pCDC);
//    newBitmap.CreateCompatibleBitmap(m_pCDC, I_rect.right - I_rect.left, I_rect.bottom - I_rect.top);
//    pOldBitmap = m_CDC.SelectObject(&newBitmap);
//
//    m_CDC.FillSolidRect(I_rect, RGB(255, 255, 255));
//    CPen RedPen, GreenPen, BluePen;
//    RedPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
//    GreenPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
//    BluePen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
//
//
//
//
//}


void COPC_ClientDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    //CDialogEx::OnOK();
    HRESULT hr = S_OK;
    // 2016/6/21 Change to CComPtr
    // 2016/6/22 Change to member variable
    //CComPtr<IUnknown> pIUnk = NULL;
    CLSID clsid;
    // COM initialize
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (SUCCEEDED(hr))
    {
        TRACE(L"COM Initialized succeessfully\n");
    }
    else
    {
        TRACE(L"COM Initialized failed\n");
        MessageBox(L"COM Initialized failed\n");
        return;
    }
    // Get CLSID
    hr = CLSIDFromProgID(L"TrainingProject.OPC_Server.OPCServer.1", &clsid);
    if (SUCCEEDED(hr))
    {
        TRACE(L"Get CLSID succeessfully\n");
    }
    else
    {
        TRACE(L"Get CLSID failed\n");
        MessageBox(L"Get CLSID failed\n");
        return;
    }
    // Get OPC server's IUnknown interface
    //hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IUnknown, (LPVOID*)&pIUnk);
    hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IUnknown, (LPVOID*)&m_pIUnk);
    if (SUCCEEDED(hr))
    {
        TRACE(L"OPCServer Initialization successfully!\n");
    }
    else
    {
        TRACE(L"OPCServer Initialization failed!\n");
        MessageBox(L"OPCServer Initialization failed!\n");
        return;
    }
    // Query IOPCServer interface
    //hr = pIUnk->QueryInterface(IID_IOPCServer, (LPVOID*)&m_pIOPCServer);
    hr = m_pIUnk->QueryInterface(IID_IOPCServer, (LPVOID*)&m_pIOPCServer);
    if (SUCCEEDED(hr))
    {
        TRACE(L"Get OPCServer interface successfully!\n");
    }
    else
    {
        TRACE(L"Get OPCServer interface failed!\n");
        MessageBox(L"Get OPCServer interface failed!\n");
        //if (pIUnk != NULL)
        if (m_pIUnk != NULL)
        {
            //pIUnk = NULL;
            m_pIUnk = NULL;
        }
        return;
    }
    LONG lTimeBias = 0;
    FLOAT dDeadBand = 0.0;
    OPCHANDLE phServerGroup;
    DWORD dwUpdateRate = 0;
    //IOPCItemMgt * pIOPCItemMgt = NULL;
    // 2016/6/23 Change to member variable
    //CComPtr<IOPCItemMgt> pIOPCItemMgt = NULL;
    DWORD dwLCID = 2052;
    // Add Group
    //hr = m_pIOPCServer->AddGroup(L"Group1", true, 200, 1, &lTimeBias, &dDeadBand, dwLCID, &phServerGroup, &dwUpdateRate, IID_IOPCItemMgt, (LPUNKNOWN*)&pIOPCItemMgt);
    hr = m_pIOPCServer->AddGroup(L"Group1", true, 200, 1, &lTimeBias, &dDeadBand, dwLCID, &phServerGroup, &dwUpdateRate, IID_IOPCItemMgt, (LPUNKNOWN*)&m_pIOPCItemMgt);
    if (SUCCEEDED(hr))
    {
        TRACE(L"Call AddGroup function successfully!\n");
    }
    else
    {
        TRACE(L"Call AddGroup function failed!\n");
        MessageBox(L"Call AddGroup function failed!\n");
        IOPCServer * tempIOPCServer = m_pIOPCServer;
        if (tempIOPCServer != NULL)
        {
            m_pIOPCServer = NULL;
            tempIOPCServer->Release();
            tempIOPCServer = NULL;
        }
        //if (pIUnk != NULL)
        if (m_pIUnk != NULL)
        {
            //pIUnk = NULL;
            m_pIUnk = NULL;
        }
        return;
    }
    // Add connection point container
    //IConnectionPointContainer * pConnectionPointContainer = NULL;
    // 2016/6/23 Change to member function
    //CComPtr<IConnectionPointContainer> pConnectionPointContainer = NULL;
    //hr = pIOPCItemMgt->QueryInterface(IID_IConnectionPointContainer, (void **)&pConnectionPointContainer);
    hr = m_pIOPCItemMgt->QueryInterface(IID_IConnectionPointContainer, (void **)&m_pConnectionPointContainer);
    if (SUCCEEDED(hr))
    {
        TRACE(L"Get IConnectionPointContainer interface successfully.\n");
    }
    else
    {
        TRACE(L"Get IConnectionPointContainer interface failed.\n");
        MessageBox(L"Get IConnectionPointContainer interface failed.\n");
        //if (pIOPCItemMgt != NULL)
        if (m_pIOPCItemMgt != NULL)
        {
            //pIOPCItemMgt->Release();
            //pIOPCItemMgt = NULL;
            m_pIOPCItemMgt = NULL;
        }
        IOPCServer * tempIOPCServer = m_pIOPCServer;
        if (tempIOPCServer != NULL)
        {
            m_pIOPCServer = NULL;
            tempIOPCServer->Release();
            tempIOPCServer = NULL;
        }
        //if (pIUnk != NULL)
        if (m_pIUnk != NULL)
        {
            //pIUnk = NULL;
            m_pIUnk = NULL;
        }
        return;
    }
    // Add connection point
    // 2016/6/21 pConnectionPoint can't be CComPtr
    // 2016/6/23 Change to member variable
    //IConnectionPoint * pConnectionPoint = NULL;
    //hr = pConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &pConnectionPoint);
    hr = m_pConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &m_pConnectionPoint);
    if (SUCCEEDED(hr))
    {
        TRACE(L"Get ConnettionPoint successfully.\n");
    }
    else
    {
        TRACE(L"Get ConnettionPoint failed.\n");
        MessageBox(L"Get ConnettionPoint failed.\n");
        //if (pConnectionPointContainer != NULL)
        if (m_pConnectionPointContainer != NULL)
        {
            //pConnectionPointContainer->Release();
            //pConnectionPointContainer = NULL;
            m_pConnectionPointContainer = NULL;
        }
        //if (pIOPCItemMgt != NULL)
        if (m_pIOPCItemMgt != NULL)
        {
            //pIOPCItemMgt->Release();
            //pIOPCItemMgt = NULL;
            m_pIOPCItemMgt = NULL;
        }
        IOPCServer * tempIOPCServer = m_pIOPCServer;
        if (tempIOPCServer != NULL)
        {
            m_pIOPCServer = NULL;
            tempIOPCServer->Release();
            tempIOPCServer = NULL;
        }
        //if (pIUnk != NULL)
        if (m_pIUnk != NULL)
        {
            //pIUnk = NULL;
            m_pIUnk = NULL;
        }
        return;
    }
    // Advise
    // 2016/6/23 change to member variable
    //ClientDataCallbackSink * pSink = NULL;
    //pSink = new ClientDataCallbackSink(m_listY, this);
    m_pSink = new ClientDataCallbackSink(m_listY, this);
    //DWORD dwCookie = 0;
    //if (pSink != NULL)
    if (m_pSink != NULL)
    {
        //hr = pConnectionPoint->Advise(pSink, &dwCookie);
        hr = m_pConnectionPoint->Advise(m_pSink, &m_dwCookie);
        //cout << hr << endl;
        if (SUCCEEDED(hr))
        {
            TRACE(L"Advise successfully\n");
        }
        else
        {
            TRACE(L"Advise failed\n");
            MessageBox(L"Advise failed\n");
            //if (pConnectionPoint != NULL)
            if (m_pConnectionPoint != NULL)
            {
                //pConnectionPoint->Release();
                m_pConnectionPoint->Release();
            }
            //if (pConnectionPointContainer != NULL)
            if (m_pConnectionPointContainer != NULL)
            {
                //pConnectionPointContainer->Release();
                //pConnectionPointContainer = NULL;
                m_pConnectionPointContainer = NULL;
            }
            //if (pIOPCItemMgt != NULL)
            if (m_pIOPCItemMgt != NULL)
            {
                //pIOPCItemMgt->Release();
                //pIOPCItemMgt = NULL;
                m_pIOPCItemMgt = NULL;
            }
            IOPCServer * tempIOPCServer = m_pIOPCServer;
            if (tempIOPCServer != NULL)
            {
                m_pIOPCServer = NULL;
                tempIOPCServer->Release();
                tempIOPCServer = NULL;
            }
            //if (pIUnk != NULL)
            if(m_pIUnk != NULL)
            {
                //pIUnk = NULL;
                m_pIUnk = NULL;
            }
            return;
        }
    }
    // Add item
    OPCITEMDEF * opcItem = (OPCITEMDEF*)CoTaskMemAlloc(3 * sizeof(OPCITEMDEF));
    opcItem[0] = { NULL, L"ItemY1", TRUE, 0, 0, NULL, VT_R8, 0 };
    opcItem[1] = { NULL, L"ItemY2", TRUE, 0, 0, NULL, VT_R8, 0 };
    opcItem[2] = { NULL, L"ItemY3", TRUE, 0, 0, NULL, VT_R8, 0 };
    OPCITEMRESULT * opcItemResult = NULL;
    HRESULT * errorResult = NULL;
    //hr = pIOPCItemMgt->AddItems(3, opcItem, &opcItemResult, &errorResult);
    hr = m_pIOPCItemMgt->AddItems(3, opcItem, &opcItemResult, &errorResult);
    if (SUCCEEDED(hr))
    {
        TRACE(L"AddItems successfully!\n");
        GetDlgItem(IDOK)->EnableWindow(FALSE);
    }
    else
    {
        TRACE(L"AddItems failed!\n");
        MessageBox(L"AddItems failed!\n");
        //if (pConnectionPoint != NULL)
        if (m_pConnectionPoint != NULL)
        {
            //pConnectionPoint->Release();
            m_pConnectionPoint->Release();
        }
        //if (pConnectionPointContainer != NULL)
        if (m_pConnectionPointContainer != NULL)
        {
            //pConnectionPointContainer->Release();
            //pConnectionPointContainer = NULL;
            m_pConnectionPointContainer = NULL;
        }
        //if (pIOPCItemMgt != NULL)
        if (m_pIOPCItemMgt != NULL)
        {
            //pIOPCItemMgt->Release();
            //pIOPCItemMgt = NULL;
            m_pIOPCItemMgt = NULL;
        }
        IOPCServer * tempIOPCServer = m_pIOPCServer;
        if (tempIOPCServer != NULL)
        {
            m_pIOPCServer = NULL;
            tempIOPCServer->Release();
            tempIOPCServer = NULL;
        }
        //if (pIUnk != NULL)
        if (m_pIUnk != NULL)
        {
            //pIUnk = NULL;
            m_pIUnk = NULL;
        }
        return;
    }
}
