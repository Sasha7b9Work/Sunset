#if !defined(AFX_KALIBRDLG_H__CCB6C30A_1DD3_4364_B113_EDC2B87EEAC6__INCLUDED_)
#define AFX_KALIBRDLG_H__CCB6C30A_1DD3_4364_B113_EDC2B87EEAC6__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KalibrDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKalibrDlg dialog

class CKalibrDlg : public CDialog
{
// Construction
public:
	CKalibrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKalibrDlg)
	enum { IDD = IDD_KALIBR_DLG };
	CGradBtn	m_buttonSave;
	CGradBtn	m_bCancel;
	CGradBtn	m_bOk;
	CString	m_sserial;
	CString	m_sdatek;
	int		m_iNsetK;
	int		m_iRange;
	CString	m_sMes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKalibrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKalibrDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboRange();
	afx_msg void OnRadioIcm();
	afx_msg void OnRadioIb();
	afx_msg void OnRadioIs();
	afx_msg void OnRadioUb();
	afx_msg void OnRadioUcm();
	afx_msg void OnRadioUs();
	//}}AFX_MSG
	BOOL WriteKoefToPsw();
	void InitRange(); 

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KALIBRDLG_H__CCB6C30A_1DD3_4364_B113_EDC2B87EEAC6__INCLUDED_)
