#if !defined(AFX_FREEMEMORYDLG_H__124C0441_61EB_47E1_8444_5FEDB3B9E7E9__INCLUDED_)
#define AFX_FREEMEMORYDLG_H__124C0441_61EB_47E1_8444_5FEDB3B9E7E9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FreeMemoryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFreeMemoryDlg dialog

class CFreeMemoryDlg : public CDialog
{
// Construction
public:
	CFreeMemoryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFreeMemoryDlg)
	enum { IDD = IDD_FREEMEMORY_DLG };
	CStatic	m_st_flash;
	CProgressCtrl	m_progress_flash;
	//}}AFX_DATA
	int free_space;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreeMemoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFreeMemoryDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREEMEMORYDLG_H__124C0441_61EB_47E1_8444_5FEDB3B9E7E9__INCLUDED_)
