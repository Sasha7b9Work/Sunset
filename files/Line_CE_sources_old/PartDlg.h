#if !defined(AFX_PARTDLG_H__7CD27891_13A0_47CB_8F07_19304553F40B__INCLUDED_)
#define AFX_PARTDLG_H__7CD27891_13A0_47CB_8F07_19304553F40B__INCLUDED_
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PartDlg.h : header file
//
#include "GradBtn.h"
#include "Klava.h"

/////////////////////////////////////////////////////////////////////////////
// CPartDlg dialog

class CPartDlg : public CDialog
{
// Construction
public:
	CPartDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPartDlg)
	enum { IDD = IDD_PART_DLG };
	CGradBtn	m_btn_ok;
	CGradBtn	m_btn_cancel;
	CString	m_sPart;
	CString	m_sPlast;
	CString	m_sPoint;
	CString	m_sProduct;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKlava *m_pKlava;

	// Generated message map functions
	//{{AFX_MSG(CPartDlg)
	virtual void OnOK();
	afx_msg void OnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTDLG_H__7CD27891_13A0_47CB_8F07_19304553F40B__INCLUDED_)
