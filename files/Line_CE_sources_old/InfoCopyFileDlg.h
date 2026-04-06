#if !defined(AFX_INFOCOPYFILEDLG_H__136EB7C4_45ED_49A5_86D2_C1D77D3EA989__INCLUDED_)
#define AFX_INFOCOPYFILEDLG_H__136EB7C4_45ED_49A5_86D2_C1D77D3EA989__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InfoCopyFileDlg.h : header file
//
#include "GradBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoCopyFileDlg dialog

class CInfoCopyFileDlg : public CDialog
{
// Construction
public:
	CInfoCopyFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInfoCopyFileDlg)
	enum { IDD = IDD_INFOCOPYFILE_DLG };
	CGradBtn	m_btnCancel;
	CGradBtn	m_btnOk;
	CString	m_sTo;
	CString	m_sFrom;
	//}}AFX_DATA
	int		m_iWhatDo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoCopyFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoCopyFileDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOCOPYFILEDLG_H__136EB7C4_45ED_49A5_86D2_C1D77D3EA989__INCLUDED_)
