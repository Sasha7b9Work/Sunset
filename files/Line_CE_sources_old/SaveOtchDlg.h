#if !defined(AFX_SAVEOTCHDLG_H__C67535F1_50D6_4E37_BE7E_30F639471622__INCLUDED_)
#define AFX_SAVEOTCHDLG_H__C67535F1_50D6_4E37_BE7E_30F639471622__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SaveOtchDlg.h : header file
//
#include "GradBtn.h"
#include "Klava.h"

/////////////////////////////////////////////////////////////////////////////
// CSaveOtchDlg dialog

class CSaveOtchDlg : public CDialog
{
// Construction
public:
	CSaveOtchDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveOtchDlg)
	enum { IDD = IDD_SAVEOTCH_DLG };
	CGradBtn	m_btnPath;
	CGradBtn	m_btnOk;
	CGradBtn	m_btnCancel;
	CString	m_sName;
	CString	m_sFile;
	CString	m_sFIO;
	CString	m_sPost;
	CString	m_sPrim;
	//}}AFX_DATA
	int		m_isposob;
	CString m_sData;
// Overrides
	int		PushKlava(CString strSymbol);
	CString GetStringFromEdit(); 
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveOtchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKlava *m_pKlava;
	// Generated message map functions
	//{{AFX_MSG(CSaveOtchDlg)
	afx_msg void OnButtonpath();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEOTCHDLG_H__C67535F1_50D6_4E37_BE7E_30F639471622__INCLUDED_)
