#if !defined(AFX_HELPDLG_H__F030A3E8_65F7_48E9_BD2E_E6DCB5C7CED9__INCLUDED_)
#define AFX_HELPDLG_H__F030A3E8_65F7_48E9_BD2E_E6DCB5C7CED9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HelpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog

class CHelpDlg : public CDialog
{
// Construction
public:
	CHelpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHelpDlg)
	enum { IDD = IDD_HELP_DLG };
	CString	m_sHelp;
	//}}AFX_DATA
	CBrush	m_brushYellow;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelpDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStaticIconExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPDLG_H__F030A3E8_65F7_48E9_BD2E_E6DCB5C7CED9__INCLUDED_)
