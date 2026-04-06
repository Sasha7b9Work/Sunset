// ParolDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ParolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParolDlg dialog


CParolDlg::CParolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParolDlg)
	m_sparol = _T("");
	//}}AFX_DATA_INIT
	what_save = FILE_MOD;
}


void CParolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParolDlg)
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Text(pDX, IDC_EDPAROL, m_sparol);
	DDV_MaxChars(pDX, m_sparol, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParolDlg, CDialog)
	//{{AFX_MSG_MAP(CParolDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParolDlg message handlers

BOOL CParolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(what_save)
	{ case FILE_DELETE:  this->SetWindowText(L"Право удаления моделей измерения защищёно паролем");
						 break;
	  case FILE_MOD:     this->SetWindowText(L"  Файл модели измерения защищён паролем");
						 break;
	  case FILE_TST:	 this->SetWindowText(L"  Стандартный тест защищён паролем");
						 break;
	  case FILE_PSW:	 this->SetWindowText(L"  Паспортный файл защищён паролем");
						 break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
