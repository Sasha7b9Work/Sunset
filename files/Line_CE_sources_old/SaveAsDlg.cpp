// SaveAsDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "SaveAsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg dialog


CSaveAsDlg::CSaveAsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveAsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveAsDlg)
	m_sFileNameSave = _T("\\Hard Disk\\Mod\\*.mod");	//Hard Disk - WinCE_4.0, DiskC - WinCE_3.0
//	m_bZaschitaParol = FALSE;
	//}}AFX_DATA_INIT
}


void CSaveAsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveAsDlg)
	DDX_Control(pDX, IDCANCEL, m_btn_cancel);
	DDX_Control(pDX, IDC_OK, m_btn_ok);
	DDX_Control(pDX, IDC_BUTPATH, m_btn_path);
	DDX_Text(pDX, IDC_EDFILENAME, m_sFileNameSave);
	DDV_MaxChars(pDX, m_sFileNameSave, 40);
	DDX_Check(pDX, IDC_CHECK1, m_bZaschitaParol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveAsDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveAsDlg)
	ON_BN_CLICKED(IDC_BUTPATH, OnButpath)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_EN_KILLFOCUS(IDC_EDFILENAME, OnKillfocusEdfilename)
	ON_BN_CLICKED(IDC_OK, OnClickedOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg message handlers

BOOL CSaveAsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveAsDlg::OnButpath() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgs(FALSE,_T("mod"),_T("\\Hard Disk\\Mod\\*.mod"));
	dlgs.m_ofn.lpstrTitle=_T("Сохранить как");
	dlgs.m_ofn.lpstrFilter=_T("Line Files (\\Hard Disk\\Mod\\*.mod)");
	UpdateData(FALSE);
	if (dlgs.DoModal()==IDOK)
	{	UpdateData();
		m_sFileNameSave = dlgs.GetPathName();
		UpdateData(FALSE);
	}

}

void CSaveAsDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	m_bZaschitaParol=!m_bZaschitaParol;
	UpdateData(FALSE);
}

void CSaveAsDlg::OnKillfocusEdfilename() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CSaveAsDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CSaveAsDlg::OnClickedOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	
}
