#if !defined(AFX_SAVETABLDLG_H__B1AA929F_1F34_4CA1_9781_D5F3B370751E__INCLUDED_)
#define AFX_SAVETABLDLG_H__B1AA929F_1F34_4CA1_9781_D5F3B370751E__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SaveTablDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveTablDlg dialog

class CSaveTablDlg : public CDialog
{
// Construction
public:
	CSaveTablDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveTablDlg)
	enum { IDD = IDD_SAVETAB_DLG };
	CGradBtn	m_btn_ok;
	CGradBtn	m_btn_cancel;
	int		m_isposob;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveTablDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveTablDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVETABLDLG_H__B1AA929F_1F34_4CA1_9781_D5F3B370751E__INCLUDED_)
