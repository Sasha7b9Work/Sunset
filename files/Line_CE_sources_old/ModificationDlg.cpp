// ModificationDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ModificationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModificationDlg dialog


CModificationDlg::CModificationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModificationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModificationDlg)
	m_inIPPP = 0;
	m_sSerNum = _T("000");
	//}}AFX_DATA_INIT
}


void CModificationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModificationDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_inIPPP);
	DDX_Text(pDX, IDC_EDIT1, m_sSerNum);
	DDV_MaxChars(pDX, m_sSerNum, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModificationDlg, CDialog)
	//{{AFX_MSG_MAP(CModificationDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModificationDlg message handlers
