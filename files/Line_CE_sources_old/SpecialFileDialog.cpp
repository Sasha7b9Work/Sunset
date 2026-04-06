// SpecialFileDialog.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "SpecialFileDialog.h"
#include "Winuser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecialFileDialog dialog


//CSpecialFileDialog::CSpecialFileDialog(CWnd* pParent /*=NULL*/)
//	: CDialog(CSpecialFileDialog::IDD, pParent)
CSpecialFileDialog::CSpecialFileDialog(BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
	LPCTSTR lpszFilter, CWnd* pParent) 	: CFileDialog(
	bOpenFileDialog,lpszDefExt,lpszFileName,dwFlags,
	lpszFilter,pParent)
{
	//{{AFX_DATA_INIT(CSpecialFileDialog)
//	m_bZaschitaParol = FALSE;
	//}}AFX_DATA_INIT
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
//	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_FILESPECIAL);
	m_ofn.lpstrTitle=_T("Сохранить как");
	m_ofn.lpstrFilter=_T("Line Files (*.mod)");
}


void CSpecialFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecialFileDialog)
	DDX_Check(pDX, IDC_CHECK1, m_bZaschitaParol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpecialFileDialog, CDialog)
	//{{AFX_MSG_MAP(CSpecialFileDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecialFileDialog message handlers

void CSpecialFileDialog::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	m_bZaschitaParol=!m_bZaschitaParol;
	
}
