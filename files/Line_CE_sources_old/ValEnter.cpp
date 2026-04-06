// ValEnter.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ValEnter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CValEnter dialog


CValEnter::CValEnter(CWnd* pParent /*=NULL*/)
	: CDialog(CValEnter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValEnter)
	m_sValEnter = _T("");
	m_sRazm = _T("");
	//}}AFX_DATA_INIT
	m_pKlava= NULL;
	m_bWhat = false;
}


void CValEnter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValEnter)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Text(pDX, IDC_EDIT_VALENTER, m_sValEnter);
	DDX_Text(pDX, IDC_STATIC_RAZM, m_sRazm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValEnter, CDialog)
	//{{AFX_MSG_MAP(CValEnter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CValEnter message handlers

BOOL CValEnter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	akt_button = akt_button +20;
	if (m_pKlava == NULL)
	{ m_pKlava=new CKlava(this);
	  m_pKlava ->n123=1;	//
	  m_pKlava ->n_move_klava_x=320;
	  m_pKlava ->n_move_klava_y=530;
	  m_pKlava -> Create();
	}
	 CWnd* pwnd=GetDlgItem(IDC_STATIC_WHAT);
	if (m_bWhat)	  pwnd->SetWindowText(_T("Введите номинал резистора"));
	else  pwnd->SetWindowText(_T("Введите показания внешнего прибора"));
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CValEnter::OnOK() 
{
	// TODO: Add extra validation here
	if (m_pKlava!= NULL) {m_pKlava->DestroyWindow(); m_pKlava=NULL;}
	
	CDialog::OnOK();
}

CString CValEnter::GetStringFromEdit() 
{
	UpdateData();
	CString strEdit;
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_VALENTER: 
			strEdit=m_sValEnter;
			strEdit.Replace(',','.');
		   break;
	}
    UpdateData(false);
	return strEdit;
}

int CValEnter::PushKlava(CString strSymbol) 
{
	UpdateData();
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_wnd=pWnd->GetDlgCtrlID();
	switch (id_wnd)
	{ case IDC_EDIT_VALENTER: 
		   if (strSymbol.GetLength() > 20) strSymbol=strSymbol.Left(20);
		   m_sValEnter = strSymbol;
		   m_sValEnter.Replace(',','.');
		   break;
	}
    UpdateData(false);
	return strSymbol.GetLength();
}


