// FreeMemoryDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "FreeMemoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreeMemoryDlg dialog


CFreeMemoryDlg::CFreeMemoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFreeMemoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFreeMemoryDlg)
	//}}AFX_DATA_INIT
	free_space = 0;
}


void CFreeMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFreeMemoryDlg)
	DDX_Control(pDX, IDC_ST_FREEMEMORY, m_st_flash);
	DDX_Control(pDX, IDC_PROGRESS, m_progress_flash);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFreeMemoryDlg, CDialog)
	//{{AFX_MSG_MAP(CFreeMemoryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreeMemoryDlg message handlers

BOOL CFreeMemoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strFreeSpace;
			if (free_space==0) strFreeSpace.Format(_T("0%% свободно"));//100%% "));
			else strFreeSpace.Format(_T("%u%% свободно"),free_space);//100-free_space);
			m_progress_flash.SetPos(free_space);//100-free_space);
			m_st_flash.SetWindowText(strFreeSpace);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
