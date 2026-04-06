#if !defined(AFX_SAVETSTDLG_H__69962ABC_14F9_4CA9_9D9F_34164E5EFE1D__INCLUDED_)
#define AFX_SAVETSTDLG_H__69962ABC_14F9_4CA9_9D9F_34164E5EFE1D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SaveTstDlg.h : header file
//
#include "GradBtn.h"
#include "Klava.h"

/////////////////////////////////////////////////////////////////////////////
// CSaveTstDlg dialog

class CSaveTstDlg : public CDialog
{
// Construction
public:
	CSaveTstDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveTstDlg)
	enum { IDD = IDD_SAVETST_DLG };
	CGradBtn	m_btnPath;
	CGradBtn	m_btnCancel;
	CGradBtn	m_but_OK;
	CString	m_snameTst;
	CString	m_sPrim;
	CString	m_sPath;
	//}}AFX_DATA

	UINT	m_uWhere;
	int		PushKlava(CString strSymbol);
	CString GetStringFromEdit(); 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveTstDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKlava *m_pKlava;

	// Generated message map functions
	//{{AFX_MSG(CSaveTstDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditName();
	afx_msg void OnButtonPath();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVETSTDLG_H__69962ABC_14F9_4CA9_9D9F_34164E5EFE1D__INCLUDED_)
