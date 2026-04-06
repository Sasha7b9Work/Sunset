// InfoCopyFileDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "InfoCopyFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoCopyFileDlg dialog


CInfoCopyFileDlg::CInfoCopyFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoCopyFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoCopyFileDlg)
	m_sTo = _T("");
	m_sFrom = _T("");
	//}}AFX_DATA_INIT
	m_iWhatDo = FILE_COPY;
}


void CInfoCopyFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoCopyFileDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Text(pDX, IDC_EDIT_TO, m_sTo);
	DDX_Text(pDX, IDC_EDIT_FROM, m_sFrom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoCopyFileDlg, CDialog)
	//{{AFX_MSG_MAP(CInfoCopyFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoCopyFileDlg message handlers

BOOL CInfoCopyFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;

	switch(m_iWhatDo)
	{ case FILE_COPY: str = _T("Копировать файл: ");
					  GetDlgItem(IDOK)->SetWindowText(_T("Копировать"));
						break;
	  case FILE_MOVE: str = _T("Перенести файл: ");
					  GetDlgItem(IDOK)->SetWindowText(_T("Перенести"));
						break;
	}
	GetDlgItem(IDC_STATIC_FROM)->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
