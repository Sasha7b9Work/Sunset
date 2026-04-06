// SaveRefDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "SaveRefDlg.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveRefDlg dialog


CSaveRefDlg::CSaveRefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveRefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveRefDlg)
	m_sNameFileRef = _T("");
	m_sPrimRef = _T("");
	m_sNameRef = _T("");
	//}}AFX_DATA_INIT
	m_sTimeRef = _T("");
	m_pKlava= NULL;

}


void CSaveRefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveRefDlg)
	DDX_Control(pDX, IDC_BUTTON_PATH, m_btnPath);
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Text(pDX, IDC_EDITNAMEFILEREF, m_sNameFileRef);
	DDV_MaxChars(pDX, m_sNameFileRef, 50);
	DDX_Text(pDX, IDC_EDITPRIMREF, m_sPrimRef);
	DDV_MaxChars(pDX, m_sPrimRef, 40);
	DDX_Text(pDX, IDC_EDIT_NAME_REF, m_sNameRef);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveRefDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveRefDlg)
	ON_EN_UPDATE(IDC_EDIT_NAME_REF, OnUpdateEditNameRef)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnButtonPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveRefDlg message handlers

void CSaveRefDlg::OnUpdateEditNameRef() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();
	m_sNameFileRef = m_sNameRef+m_sTimeRef;
	UpdateData(false);
	
}

HBRUSH CSaveRefDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CSaveRefDlg::OnButtonPath() 
{
	// TODO: Add your control notification handler code here
	CFileDlg dlg;
	dlg.m_sTitle = _T("Сохранить график в архиве");
	dlg.m_iWhatDo = FILE_SAVE_AS;
	dlg.m_sFilter =_T("Files (\\Hard Disk\\Ref\\*.ref)");
	dlg.m_sCatalog =_T("\\Hard Disk\\Ref\\*.ref");
	dlg.m_sPath = _T("\\Hard Disk\\Ref\\");
	dlg.m_sName = _T("*.ref");
	dlg.m_itip = FILE_REF;
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
	  m_sNameFileRef = dlg.m_sName;
	  UpdateData(false);
	}
	
}

BOOL CSaveRefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
				if (m_pKlava == NULL)
			{m_pKlava=new CKlava(this);
			m_pKlava ->n123=0;	//
			m_pKlava ->n_move_klava_x=630;
			m_pKlava ->n_move_klava_y=480;
			m_pKlava -> Create();}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveRefDlg::OnOK() 
{
	// TODO: Add extra validation here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	
	CDialog::OnOK();
}

void CSaveRefDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	
	CDialog::OnCancel();
}
CString CSaveRefDlg::GetStringFromEdit() 
{
	UpdateData();
	CString strEdit;
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAME_REF: 
			strEdit=m_sNameRef;
		   break;
	  case IDC_EDITPRIMREF: 
			strEdit=m_sPrimRef;
		   break;
	}
    UpdateData(false);
	return strEdit;
}

int CSaveRefDlg::PushKlava(CString strSymbol) 
{
	UpdateData();
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_NAME_REF: 
		   if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
		   m_sNameRef = strSymbol;
		   m_sNameFileRef = m_sNameRef +m_sTimeRef;
		   break;
	  case IDC_EDITPRIMREF: 
		   if (strSymbol.GetLength() > 40) strSymbol=strSymbol.Left(40);
		   m_sPrimRef = strSymbol;
		   break;
	}
    UpdateData(false);
	return strSymbol.GetLength();
}
