// HelpDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog


CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpDlg)
	m_sHelp = _T("");
	//}}AFX_DATA_INIT
}


void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDlg)
	DDX_Text(pDX, IDC_EDIT_HELP, m_sHelp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_STATIC_ICON_EXIT, OnStaticIconExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg message handlers

BOOL CHelpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brushYellow.CreateSolidBrush(RGB(255,255,81));
    CRect rect,RectHelp;
	GetClientRect(&rect);
	GetDlgItem(IDC_EDIT_HELP)->GetWindowRect(&RectHelp);
	RectHelp.left=rect.left+2;
	RectHelp.top=rect.top+2;
	RectHelp.right=rect.right-2;
	RectHelp.bottom=rect.bottom-2;
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_EDIT_HELP);
	pSt->MoveWindow(&RectHelp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CHelpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	(pWnd -> GetDlgCtrlID() == IDC_EDIT_HELP)
	{	hbr = m_brushYellow;//hbr;
		pDC->SetBkColor(RGB(255,255,81));
		pDC->SetTextColor(RGB(0,0,0));
	}
	if	(pWnd -> GetDlgCtrlID() == IDC_STATIC_ICON_EXIT)
	{	hbr = m_brushYellow;//hbr;
		pDC->SetBkColor(RGB(255,255,81));
		pDC->SetTextColor(RGB(0,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CHelpDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_brushYellow.DeleteObject();			
			
	CDialog::PostNcDestroy();
}

void CHelpDlg::OnStaticIconExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
