// ReparolDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ReparolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReparolDlg dialog


CReparolDlg::CReparolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReparolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReparolDlg)
	m_sparolold = _T("");
	m_sparolnew = _T("");
	m_sparolnew1 = _T("");
	//}}AFX_DATA_INIT
}


void CReparolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReparolDlg)
	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_OK, m_btn_ok);
	DDX_Text(pDX, IDC_EDPAROLOLD, m_sparolold);
	DDV_MaxChars(pDX, m_sparolold, 10);
	DDX_Text(pDX, IDC_EDPAROLNEW, m_sparolnew);
	DDV_MaxChars(pDX, m_sparolnew, 10);
	DDX_Text(pDX, IDC_EDPAROLNEW1, m_sparolnew1);
	DDV_MaxChars(pDX, m_sparolnew1, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReparolDlg, CDialog)
	//{{AFX_MSG_MAP(CReparolDlg)
	ON_BN_CLICKED(IDC_OK, OnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_STATIC_KEY, OnStaticKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReparolDlg message handlers

void CReparolDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CReparolDlg::OnClickedOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	
}

void CReparolDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
	
}

HBRUSH CReparolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CReparolDlg::OnStaticKey() 
{
	// TODO: Add your control notification handler code here
	if (MessageBox(_T("¬ы уверены, что хотите установить пароль по умолчанию?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
	{ m_sparol = _T("admin");//_T("LINE");
	  m_sparolold = _T("admin");//_T("LINE"); 
	  m_sparolnew = _T("admin");//_T("LINE"); 
	  m_sparolnew1 = _T("admin");//_T("LINE"); 
	  MessageBox(_T("ѕароль установлен в состо€ние по умолчанию!"),NULL,MB_OK|MB_ICONINFORMATION);	
	}
}
