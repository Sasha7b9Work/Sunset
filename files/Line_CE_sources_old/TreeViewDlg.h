#if !defined(AFX_TREEVIEWDLG_H__70187CB2_929F_45F4_A88E_AFEB0AD87925__INCLUDED_)
#define AFX_TREEVIEWDLG_H__70187CB2_929F_45F4_A88E_AFEB0AD87925__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TreeViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeViewDlg dialog

class CTreeViewDlg : public CDialog
{
// Construction
public:
	CTreeViewDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL Create(); 

// Dialog Data
	//{{AFX_DATA(CTreeViewDlg)
	enum { IDD = IDD_TREE_DLG };
	CTreeCtrl	m_TreeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nID;
	CWnd* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CTreeViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEVIEWDLG_H__70187CB2_929F_45F4_A88E_AFEB0AD87925__INCLUDED_)
