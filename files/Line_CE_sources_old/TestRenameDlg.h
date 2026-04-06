#if !defined(AFX_TESTRENAMEDLG_H__87C1CF8E_4C4D_482F_8BDA_68E9C9CAEBF7__INCLUDED_)
#define AFX_TESTRENAMEDLG_H__87C1CF8E_4C4D_482F_8BDA_68E9C9CAEBF7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TestRenameDlg.h : header file
//
#include "GradBtn.h"
#include "Klava.h"

/////////////////////////////////////////////////////////////////////////////
// CTestRenameDlg dialog

class CTestRenameDlg : public CDialog
{
// Construction
public:
	CTestRenameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestRenameDlg)
	enum { IDD = IDD_TESTRENAME_DLG };
	CGradBtn	m_btn_ok;
	CString	m_EL;
	CString	m_MOD;
	CString	m_PRIM;
	//}}AFX_DATA
	int		PushKlava(CString strSymbol);
	CString GetStringFromEdit(); 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestRenameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKlava *m_pKlava;

	// Generated message map functions
	//{{AFX_MSG(CTestRenameDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClickedOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTRENAMEDLG_H__87C1CF8E_4C4D_482F_8BDA_68E9C9CAEBF7__INCLUDED_)
