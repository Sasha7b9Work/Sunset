// SaveOtchDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "SaveOtchDlg.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveOtchDlg dialog


CSaveOtchDlg::CSaveOtchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveOtchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveOtchDlg)
	m_sName = _T("");
	m_sFile = _T("");
	m_sFIO = _T("");
	m_sPost = _T("");
	m_sPrim = _T("");
	//}}AFX_DATA_INIT
	m_isposob = 1;
	m_sData =_T("");
	m_pKlava= NULL;
}


void CSaveOtchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveOtchDlg)
	DDX_Control(pDX, IDC_BUTTONPATH, m_btnPath);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_FILE, m_sFile);
	DDX_Text(pDX, IDC_EDIT_FIO, m_sFIO);
	DDX_Text(pDX, IDC_EDIT_POST, m_sPost);
	DDX_Text(pDX, IDC_EDIT_PRIM, m_sPrim);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveOtchDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveOtchDlg)
	ON_BN_CLICKED(IDC_BUTTONPATH, OnButtonpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveOtchDlg message handlers

void CSaveOtchDlg::OnButtonpath() 
{
	// TODO: Add your control notification handler code here
	CFileDlg dlg;
	dlg.m_sTitle = _T("Сохранить отчет");
	dlg.m_iWhatDo = FILE_SAVE;
	dlg.m_sFilter =_T("Files (\\Hard Disk\\Txt\\*.txt)");
	dlg.m_sCatalog =_T("\\Hard Disk\\Txt\\*.txt");
	dlg.m_sPath = _T("\\Hard Disk\\Txt\\");
	dlg.m_sName = m_sFile;//_T("*.txt");
	dlg.m_itip = FILE_TXT;
	dlg.m_sFullNameFile = _T("");
	dlg.m_isposob = m_isposob;
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
      m_isposob = dlg.m_isposob;
	  m_sFile = dlg.m_sName;
	  UpdateData(false);
	}
	  
}
BOOL CSaveOtchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
				if (m_pKlava == NULL)
			{m_pKlava=new CKlava(this);
			m_pKlava ->n123=1;	//цифры
			m_pKlava ->n_move_klava_x=620;
			m_pKlava ->n_move_klava_y=380;
			m_pKlava -> Create();}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSaveOtchDlg::OnOK() 
{
	// TODO: Add extra validation here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	
	CDialog::OnOK();
}

void CSaveOtchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	
	CDialog::OnCancel();
}

CString CSaveOtchDlg::GetStringFromEdit() 
{
	UpdateData();
	CString strEdit;
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAME: 
			strEdit=m_sName;
		   break;
	  case IDC_EDIT_PRIM: 
			strEdit=m_sPrim;
		   break;
	  case IDC_EDIT_FIO: 
			strEdit=m_sFIO;
		   break;
	  case IDC_EDIT_POST: 
			strEdit=m_sPost;
		   break;

	}
    UpdateData(false);
	return strEdit;
}
int CSaveOtchDlg::PushKlava(CString strSymbol) 
{
	UpdateData();
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAME: 
		   if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
		   m_sName = strSymbol;
		   m_sFile = m_sName +m_sData;
		   break;
	  case IDC_EDIT_PRIM: 
		   if (strSymbol.GetLength() > 50) strSymbol=strSymbol.Left(50);
		   m_sPrim = strSymbol;
		   break;
	  case IDC_EDIT_FIO: 
		   if (strSymbol.GetLength() > 30) strSymbol=strSymbol.Left(30);
		   m_sFIO = strSymbol;
		   break;
	  case IDC_EDIT_POST: 
		   if (strSymbol.GetLength() > 30) strSymbol=strSymbol.Left(30);
		   m_sPost = strSymbol;
		   break;

	}
    UpdateData(false);
	return strSymbol.GetLength();
}
