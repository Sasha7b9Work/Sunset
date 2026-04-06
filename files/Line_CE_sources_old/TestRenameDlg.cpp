// TestRenameDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "TestRenameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestRenameDlg dialog


CTestRenameDlg::CTestRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestRenameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestRenameDlg)
	m_EL = _T("");
	m_MOD = _T("");
	m_PRIM = _T("");
	//}}AFX_DATA_INIT
	m_pKlava= NULL;
}


void CTestRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestRenameDlg)
	DDX_Control(pDX, IDC_OK, m_btn_ok);
	DDX_Text(pDX, IDC_EDIT_NAMEEL, m_EL);
	DDX_Text(pDX, IDC_EDIT_NAMETEST, m_MOD);
	DDX_Text(pDX, IDC_EDIT_NEWPRIM, m_PRIM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestRenameDlg, CDialog)
	//{{AFX_MSG_MAP(CTestRenameDlg)
	ON_BN_CLICKED(IDC_OK, OnClickedOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestRenameDlg message handlers

BOOL CTestRenameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	akt_button = akt_button +20;
	if (m_pKlava == NULL)
	{ m_pKlava=new CKlava(this);
	  m_pKlava ->n123=0;	//
	  m_pKlava ->n_move_klava_x=320;
	  m_pKlava ->n_move_klava_y=530;
	  m_pKlava -> Create();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestRenameDlg::OnOK() 
{
	// TODO: Add extra validation here
	//заглушка
	//CDialog::OnOK();
}

void CTestRenameDlg::OnClickedOk() 
{
	// TODO: Add your control notification handler code here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	CDialog::OnOK();
	
}
CString CTestRenameDlg::GetStringFromEdit() 
{
	UpdateData();
	CString strEdit;
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAMEEL: 
			strEdit=m_EL;
		   break;
	  case IDC_EDIT_NAMETEST: 
			strEdit=m_MOD;
		   break;
	  case IDC_EDIT_NEWPRIM: 
			strEdit=m_PRIM;
		   break;
	}
    UpdateData(false);
	return strEdit;
}

int CTestRenameDlg::PushKlava(CString strSymbol) 
{
	UpdateData();
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAMEEL: 
		   if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
		   m_EL = strSymbol;
		   break;
	  case IDC_EDIT_NAMETEST: 
		   if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
		   m_MOD = strSymbol;
		   break;
	  case IDC_EDIT_NEWPRIM: 
		   if (strSymbol.GetLength() > 40) strSymbol=strSymbol.Left(40);
		   m_PRIM = strSymbol;
		   break;
	}
    UpdateData(false);
	return strSymbol.GetLength();
}

