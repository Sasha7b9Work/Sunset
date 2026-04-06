// ScalDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ScalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScalDlg dialog


CScalDlg::CScalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScalDlg)
	m_bInversion = FALSE;
	//}}AFX_DATA_INIT
	m_iXY = 0;
}


void CScalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScalDlg)
	DDX_Check(pDX, IDC_CHECK_INVERSION, m_bInversion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScalDlg, CDialog)
	//{{AFX_MSG_MAP(CScalDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScalDlg message handlers

BOOL CScalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_iXY)
	{ case 0: this->SetWindowText(_T("Ўкала по оси X"));
			  break;
	  case 1: this->SetWindowText(_T("Ўкала по оси Y"));
		      break;
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
