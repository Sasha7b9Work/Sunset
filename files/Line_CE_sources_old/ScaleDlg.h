#if !defined(AFX_SCALEDLG_H__EC1C8D62_C697_498A_968C_85E53B9AF5D3__INCLUDED_)
#define AFX_SCALEDLG_H__EC1C8D62_C697_498A_968C_85E53B9AF5D3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScaleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog

class CScaleDlg : public CDialog
{
// Construction
public:
	CScaleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScaleDlg)
	enum { IDD = IDD_SCALE_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScaleDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALEDLG_H__EC1C8D62_C697_498A_968C_85E53B9AF5D3__INCLUDED_)
