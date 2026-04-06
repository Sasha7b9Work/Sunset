#if !defined(AFX_PAROLDLG_H__D72AFD17_4866_46F4_89E7_577D03C72E7A__INCLUDED_)
#define AFX_PAROLDLG_H__D72AFD17_4866_46F4_89E7_577D03C72E7A__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ParolDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParolDlg dialog

class CParolDlg : public CDialog
{
// Construction
public:
	CParolDlg(CWnd* pParent = NULL);   // standard constructor
	int what_save;

// Dialog Data
	//{{AFX_DATA(CParolDlg)
	enum { IDD = IDD_PAROL_DLG };
	CGradBtn	m_btn_ok;
	CString	m_sparol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParolDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAROLDLG_H__D72AFD17_4866_46F4_89E7_577D03C72E7A__INCLUDED_)
