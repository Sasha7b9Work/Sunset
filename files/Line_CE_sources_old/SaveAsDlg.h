#if !defined(AFX_SAVEASDLG_H__7EE3A947_CB0C_4B46_A7A2_B27A892EC780__INCLUDED_)
#define AFX_SAVEASDLG_H__7EE3A947_CB0C_4B46_A7A2_B27A892EC780__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SaveAsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveAsDlg dialog

class CSaveAsDlg : public CDialog
{
// Construction
public:
	CSaveAsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveAsDlg)
	enum { IDD = IDD_SAVEAS_DLG };
	CGradBtn	m_btn_cancel;
	CGradBtn	m_btn_ok;
	CGradBtn	m_btn_path;
	CString	m_sFileNameSave;
	BOOL	m_bZaschitaParol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveAsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveAsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButpath();
	afx_msg void OnCheck1();
	afx_msg void OnKillfocusEdfilename();
	virtual void OnOK();
	afx_msg void OnClickedOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEASDLG_H__7EE3A947_CB0C_4B46_A7A2_B27A892EC780__INCLUDED_)
