// ScaleDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog


CScaleDlg::CScaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScaleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScaleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScaleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScaleDlg, CDialog)
	//{{AFX_MSG_MAP(CScaleDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg message handlers
