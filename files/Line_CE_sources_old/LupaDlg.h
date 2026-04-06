#if !defined(AFX_LUPADLG_H__51318B79_BDF2_42FC_8049_2DB071CA7146__INCLUDED_)
#define AFX_LUPADLG_H__51318B79_BDF2_42FC_8049_2DB071CA7146__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LupaDlg.h : header file
//
#include "GradBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CLupaDlg dialog

class CLupaDlg : public CDialog
{
// Construction
public:
	CLupaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLupaDlg)
	enum { IDD = IDD_LUPA_DLG };
	CGradBtn	m_butCancel;
	CGradBtn	m_butOk;
	CString	m_sScaleX;
	CString	m_sScaleY;
	UINT	m_uPrecision;
	CString	m_sXmin;
	CString	m_sXmax;
	CString	m_sYmax;
	CString	m_sYmin;
	CString	m_sStepX;
	CString	m_sStepY;
	CString	m_sNpoints;
	//}}AFX_DATA
	double  X_min;
	double  X_max;
	double  Y_min;
	double  Y_max;
	double	StepX;
	double	StepY;
	BOOL	m_bCheckKolRejHigh;
	BOOL	m_bCheckLooping;
	UINT	m_uNpoints;
	int		m_irazv;
	int		m_iQue1;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLupaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLupaDlg)
	virtual void OnOK();
	afx_msg void OnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinPrecision(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUPADLG_H__51318B79_BDF2_42FC_8049_2DB071CA7146__INCLUDED_)
