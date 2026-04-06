// PartDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "PartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartDlg dialog


CPartDlg::CPartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPartDlg)
	m_sPart = _T("");
	m_sPlast = _T("");
	m_sPoint = _T("");
	m_sProduct = _T("");
	//}}AFX_DATA_INIT
}


void CPartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPartDlg)
	DDX_Control(pDX, IDC_OK, m_btn_ok);
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_Text(pDX, IDC_EDITPART, m_sPart);
	DDV_MaxChars(pDX, m_sPart, 10);
	DDX_Text(pDX, IDC_EDITPLAST, m_sPlast);
	DDV_MaxChars(pDX, m_sPlast, 3);
	DDX_Text(pDX, IDC_EDITPOINT, m_sPoint);
	DDV_MaxChars(pDX, m_sPoint, 3);
	DDX_Text(pDX, IDC_EDIT_PRODUCT, m_sProduct);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPartDlg, CDialog)
	//{{AFX_MSG_MAP(CPartDlg)
	ON_BN_CLICKED(IDC_OK, OnClickedOk)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartDlg message handlers



void CPartDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CPartDlg::OnClickedOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	
}

HBRUSH CPartDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	(pWnd -> GetDlgCtrlID() == IDC_STATIC_TEXT)
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
