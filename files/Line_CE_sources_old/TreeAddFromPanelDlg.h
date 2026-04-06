#if !defined(AFX_TREEADDFROMPANELDLG_H__C487C1D2_2509_40ED_8ACA_52F8DB8FDE66__INCLUDED_)
#define AFX_TREEADDFROMPANELDLG_H__C487C1D2_2509_40ED_8ACA_52F8DB8FDE66__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TreeAddFromPanelDlg.h : header file
//
#include "GradBtn.h"
#include "Klava.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeAddFromPanelDlg dialog

class CTreeAddFromPanelDlg : public CDialog
{
// Construction
public:
	CTreeAddFromPanelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTreeAddFromPanelDlg)
	enum { IDD = IDD_ADD_TREEFROMPANEL };
	CGradBtn	m_btn_ok;
	CString	m_MOD;
	CString	m_PRIM;
	CString	m_EL;
	//}}AFX_DATA

	int		PushKlava(CString strSymbol);
	CString GetStringFromEdit(); 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeAddFromPanelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKlava *m_pKlava;

	// Generated message map functions
	//{{AFX_MSG(CTreeAddFromPanelDlg)
	virtual void OnOK();
	afx_msg void OnClickedOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEADDFROMPANELDLG_H__C487C1D2_2509_40ED_8ACA_52F8DB8FDE66__INCLUDED_)
