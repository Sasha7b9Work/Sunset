#if !defined(AFX_DIFFDLG_H__EDCA43C5_3C70_4894_8D79_74194600386F__INCLUDED_)
#define AFX_DIFFDLG_H__EDCA43C5_3C70_4894_8D79_74194600386F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DiffDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiffDlg dialog

class CDiffDlg : public CDialog
{
// Construction
public:
	CDiffDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDiffDlg)
	enum { IDD = IDD_DIFF_DLG };
	int		m_iDifference;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiffDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIFFDLG_H__EDCA43C5_3C70_4894_8D79_74194600386F__INCLUDED_)
