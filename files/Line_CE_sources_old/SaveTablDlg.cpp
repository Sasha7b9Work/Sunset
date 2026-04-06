// SaveTablDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "SaveTablDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveTablDlg dialog


CSaveTablDlg::CSaveTablDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveTablDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveTablDlg)
	m_isposob = 0;
	//}}AFX_DATA_INIT
}


void CSaveTablDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveTablDlg)
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_Radio(pDX, IDC_RADIOADDDAT, m_isposob);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveTablDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveTablDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveTablDlg message handlers
