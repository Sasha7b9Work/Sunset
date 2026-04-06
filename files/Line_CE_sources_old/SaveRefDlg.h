#if !defined(AFX_SAVEREFDLG_H__45DBFCFF_18C5_4B60_81EC_DD27990F9B02__INCLUDED_)
#define AFX_SAVEREFDLG_H__45DBFCFF_18C5_4B60_81EC_DD27990F9B02__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SaveRefDlg.h : header file
//
#include "GradBtn.h"
#include "Klava.h"

/////////////////////////////////////////////////////////////////////////////
// CSaveRefDlg dialog

class CSaveRefDlg : public CDialog
{
// Construction
public:
	CSaveRefDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveRefDlg)
	enum { IDD = IDD_SAVEREF_DLG };
	CGradBtn	m_btnPath;
	CGradBtn	m_btn_cancel;
	CGradBtn	m_btn_ok;
	CString	m_sNameFileRef;
	CString	m_sPrimRef;
	CString	m_sNameRef;
	//}}AFX_DATA
	CString	m_sTimeRef;

	int		PushKlava(CString strSymbol);
	CString GetStringFromEdit(); 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveRefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKlava *m_pKlava;

	// Generated message map functions
	//{{AFX_MSG(CSaveRefDlg)
	afx_msg void OnUpdateEditNameRef();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonPath();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEREFDLG_H__45DBFCFF_18C5_4B60_81EC_DD27990F9B02__INCLUDED_)
