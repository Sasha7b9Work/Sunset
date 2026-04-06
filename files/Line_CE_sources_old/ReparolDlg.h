#if !defined(AFX_REPAROLDLG_H__BA5165BC_5E7F_4FEC_B4A4_E0AC61D2D720__INCLUDED_)
#define AFX_REPAROLDLG_H__BA5165BC_5E7F_4FEC_B4A4_E0AC61D2D720__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ReparolDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReparolDlg dialog

class CReparolDlg : public CDialog
{
// Construction
public:
	CReparolDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReparolDlg)
	enum { IDD = IDD_REPAROL_DLG };
	CGradBtn	m_btnCancel;
	CGradBtn	m_btn_ok;
	CString	m_sparolold;
	CString	m_sparolnew;
	CString	m_sparolnew1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReparolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReparolDlg)
	virtual void OnOK();
	afx_msg void OnClickedOk();
	afx_msg void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStaticKey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPAROLDLG_H__BA5165BC_5E7F_4FEC_B4A4_E0AC61D2D720__INCLUDED_)
