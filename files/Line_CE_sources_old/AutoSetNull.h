#if !defined(AFX_AUTOSETNULL_H__B76D4945_EC56_499F_9B4A_5AD12F479B21__INCLUDED_)
#define AFX_AUTOSETNULL_H__B76D4945_EC56_499F_9B4A_5AD12F479B21__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AutoSetNull.h : header file
//
#include "GradBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoSetNull dialog

class CAutoSetNull : public CDialog
{
// Construction
public:
	CAutoSetNull(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoSetNull)
	enum { IDD = IDD_AUTOSETNULL_DLG };
	CGradBtn	m_bvyxod;
	CGradBtn	m_bpusk;
	int		m_iNset;
	CString	m_sUkaz;
	int		m_iRange;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoSetNull)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	ShowCxema();
	void	ShowControls(UINT* pControls,UINT cControls,BOOL fVisible);

	// Generated message map functions
	//{{AFX_MSG(CAutoSetNull)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioIcm();
	afx_msg void OnRadioIbs();
	afx_msg void OnRadioIbm();
	afx_msg void OnRadioIsm();
	afx_msg void OnRadioIss();
	afx_msg void OnRadioUbm();
	afx_msg void OnRadioUbs();
	afx_msg void OnRadioUcm();
	afx_msg void OnRadioUsm();
	afx_msg void OnRadioUss();
	afx_msg void OnRadioIcm10n();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSETNULL_H__B76D4945_EC56_499F_9B4A_5AD12F479B21__INCLUDED_)
