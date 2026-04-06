// SaveTstDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "SaveTstDlg.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveTstDlg dialog


CSaveTstDlg::CSaveTstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveTstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveTstDlg)
	m_snameTst = _T("");
	m_sPrim = _T("");
	m_sPath = _T("");
	//}}AFX_DATA_INIT
    m_uWhere = TST_USER;
	m_pKlava= NULL;
}


void CSaveTstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveTstDlg)
	DDX_Control(pDX, IDC_BUTTON_PATH, m_btnPath);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_but_OK);
	DDX_Text(pDX, IDC_EDIT_NAME, m_snameTst);
	DDX_Text(pDX, IDC_EDIT_PRIM, m_sPrim);
	DDV_MaxChars(pDX, m_sPrim, 50);
	DDX_Text(pDX, IDC_EDIT_FILE, m_sPath);
	DDV_MaxChars(pDX, m_sPath, 40);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveTstDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveTstDlg)
	ON_EN_UPDATE(IDC_EDIT_NAME, OnUpdateEditName)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnButtonPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveTstDlg message handlers

BOOL CSaveTstDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_uWhere)
	{ case TST_STANDART: this->SetWindowText(L"—охранить в библиотеке стандартных тестов");
						 break;
	  case TST_USER:	 this->SetWindowText(L"—охранить в библиотеке пользовательских тестов");
						 break;
	}
	if (m_pKlava == NULL)
	{ m_pKlava=new CKlava(this);
	  m_pKlava ->n123=0;	//
	  m_pKlava ->n_move_klava_x=620;
	  m_pKlava ->n_move_klava_y=460;
	  m_pKlava -> Create();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveTstDlg::OnUpdateEditName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	switch(m_uWhere)
	{ case TST_STANDART: m_sPath=_T("\\Hard Disk\\Tst_standart\\")+m_snameTst+_T(".tst");
						 break;
	  case TST_USER:	 m_sPath=_T("\\Hard Disk\\Tst_user\\")+m_snameTst+_T(".tst");
						 break;
	}
	UpdateData(false);
}

void CSaveTstDlg::OnButtonPath() 
{
	// TODO: Add your control notification handler code here
	CFileDlg dlg;
	switch(m_uWhere)
	{ case TST_STANDART:dlg.m_sTitle = _T("—охранить в библиотеке стандартных тестов");
						dlg.m_iWhatDo = FILE_SAVE_AS;
						dlg.m_sFilter =_T("Files (\\Hard Disk\\Tst_standart\\*.tst)");
						dlg.m_sCatalog =_T("\\Hard Disk\\Tst_standart\\*.tst");
						dlg.m_sPath = _T("\\Hard Disk\\Tst_standart\\");
						dlg.m_sName = _T("*.tst");
						dlg.m_itip = FILE_TST;
						dlg.m_sFullNameFile = _T("");

						 break;
	  case TST_USER:	dlg.m_sTitle = _T("—охранить в библиотеке пользовательских тестов");
						dlg.m_iWhatDo = FILE_SAVE_AS;
						dlg.m_sFilter =_T("Files (\\Hard Disk\\Tst_user\\*.tst)");
						dlg.m_sCatalog =_T("\\Hard Disk\\Tst_user\\*.tst");
						dlg.m_sPath = _T("\\Hard Disk\\Tst_user\\");
						dlg.m_sName = _T("*.tst");
						dlg.m_itip = FILE_TST_US;
						dlg.m_sFullNameFile = _T("");
						 break;
	}
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
	  m_sPath = dlg.m_sPath+dlg.m_sName;
	  CString str = dlg.m_sName;
	  int l= str.GetLength();
	  m_snameTst = str.Mid(0,l-4);
	  UpdateData(false);
	}
	
}
CString CSaveTstDlg::GetStringFromEdit() 
{
	UpdateData();
	CString strEdit;
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAME: 
			strEdit=m_snameTst;
		   break;
	  case IDC_EDIT_PRIM: 
			strEdit=m_sPrim;
		   break;
	}
    UpdateData(false);
	return strEdit;
}

int CSaveTstDlg::PushKlava(CString strSymbol) 
{
	UpdateData();
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAME: 
		   if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
		   m_snameTst = strSymbol;
	       switch(m_uWhere)
		   { case TST_STANDART: m_sPath=_T("\\Hard Disk\\Tst_standart\\")+m_snameTst+_T(".tst");
						 break;
	         case TST_USER:	 m_sPath=_T("\\Hard Disk\\Tst_user\\")+m_snameTst+_T(".tst");
						 break;
		   }
		   break;
	  case IDC_EDIT_PRIM: 
		   if (strSymbol.GetLength() > 40) strSymbol=strSymbol.Left(40);
		   m_sPrim = strSymbol;
		   break;
	}
    UpdateData(false);
	return strSymbol.GetLength();
}

void CSaveTstDlg::OnOK() 
{
	// TODO: Add extra validation here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	
	CDialog::OnOK();
}

void CSaveTstDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	
	CDialog::OnCancel();
}
