// DiffDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "DiffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiffDlg dialog


CDiffDlg::CDiffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiffDlg)
	m_iDifference = 0;
	//}}AFX_DATA_INIT
}


void CDiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiffDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_iDifference);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiffDlg, CDialog)
	//{{AFX_MSG_MAP(CDiffDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiffDlg message handlers

HBRUSH CDiffDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	(pWnd -> GetDlgCtrlID() == IDC_STATIC_TITLE)
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
