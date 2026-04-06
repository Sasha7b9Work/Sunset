// TreeAddFromPanelDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "TreeAddFromPanelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeAddFromPanelDlg dialog


CTreeAddFromPanelDlg::CTreeAddFromPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeAddFromPanelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreeAddFromPanelDlg)
	m_MOD = _T("");
	m_PRIM = _T("");
	m_EL = _T("");
	//}}AFX_DATA_INIT
	m_pKlava= NULL;
}


void CTreeAddFromPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeAddFromPanelDlg)
	DDX_Control(pDX, IDC_OK, m_btn_ok);
	DDX_Text(pDX, IDC_EDITNAMETEST, m_MOD);
	DDX_Text(pDX, IDC_EDITTEXTPRIM, m_PRIM);
	DDV_MaxChars(pDX, m_PRIM, 60);
	DDX_Text(pDX, IDC_EDITNAMEEL, m_EL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeAddFromPanelDlg, CDialog)
	//{{AFX_MSG_MAP(CTreeAddFromPanelDlg)
	ON_BN_CLICKED(IDC_OK, OnClickedOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeAddFromPanelDlg message handlers

void CTreeAddFromPanelDlg::OnOK() 
{
	// TODO: Add extra validation here
//заглушка	
//	CDialog::OnOK();
}

void CTreeAddFromPanelDlg::OnClickedOk() 
{
	// TODO: Add your control notification handler code here
			if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	CDialog::OnOK();
	
}

BOOL CTreeAddFromPanelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//список наименований элементов
/*    CComboBox *pcb = (CComboBox*) GetDlgItem(IDC_COMBONAMEEL);
     for (int i=0;i<NE;i++)
	 { if (m_masStrEl[i]==_T("")) break;
       pcb->AddString((LPCTSTR)m_masStrEl[i]);
	 }
*/	akt_button = akt_button+10; //(111)
	 if (m_pKlava == NULL)
	{ m_pKlava=new CKlava(this);
	  m_pKlava ->n123=0;	//
	  m_pKlava ->n_move_klava_x=330;
	  m_pKlava ->n_move_klava_y=520;
	  m_pKlava -> Create();
	}


  UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
CString CTreeAddFromPanelDlg::GetStringFromEdit() 
{
	UpdateData();
	CString strEdit;
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDITNAMEEL:
			strEdit = m_EL;
		   break;
	  case IDC_EDITNAMETEST: 
			strEdit = m_MOD;
		   break;
	  case IDC_EDITTEXTPRIM: 
			strEdit = m_PRIM;
		   break;
	}
    UpdateData(false);
	return strEdit;
}

int CTreeAddFromPanelDlg::PushKlava(CString strSymbol) 
{
	UpdateData();
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDITNAMEEL:
		   if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
		   m_EL = strSymbol;
		   break;
	  case IDC_EDITNAMETEST: 
		   if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
		   m_MOD = strSymbol;
		   break;
	  case IDC_EDITTEXTPRIM: 
		   if (strSymbol.GetLength() > 40) strSymbol=strSymbol.Left(40);
		   m_PRIM = strSymbol;
		   break;
	}
    UpdateData(false);
	return strSymbol.GetLength();
}

