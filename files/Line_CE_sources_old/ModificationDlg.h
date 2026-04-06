#if !defined(AFX_MODIFICATIONDLG_H__DDAE695D_59BB_4161_A14B_9E64A6E53920__INCLUDED_)
#define AFX_MODIFICATIONDLG_H__DDAE695D_59BB_4161_A14B_9E64A6E53920__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ModificationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModificationDlg dialog

class CModificationDlg : public CDialog
{
// Construction
public:
	CModificationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModificationDlg)
	enum { IDD = IDD_MODIFICATION_DLG };
	int		m_inIPPP;
	CString	m_sSerNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModificationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModificationDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFICATIONDLG_H__DDAE695D_59BB_4161_A14B_9E64A6E53920__INCLUDED_)
