#if !defined(AFX_DELTSTDLG_H__555FF293_6956_42B6_B513_D799A56F14B0__INCLUDED_)
#define AFX_DELTSTDLG_H__555FF293_6956_42B6_B513_D799A56F14B0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DelTstDlg.h : header file
//
#include "GradBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CDelTstDlg dialog

class CDelTstDlg : public CDialog
{
// Construction
public:
	CDelTstDlg(CWnd* pParent = NULL);   // standard constructor

	UINT what_directory;
	BOOL bFileOpen;
// Dialog Data
	//{{AFX_DATA(CDelTstDlg)
	enum { IDD = IDD_DEL_TST_DLG };
	CGradBtn	m_butDelAll;
	CGradBtn	m_butDel;
	CGradBtn	m_butOK;
	CListCtrl	m_listTst;
	int		m_nelement;
	//}}AFX_DATA
	int			n_tst;
	CString		m_sname_tst;
	CBrush      m_brushGray;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelTstDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitTablTst();
	void BuildArrayTstStandart();
	void BuildArrayTstUser();

	// Generated message map functions
	//{{AFX_MSG(CDelTstDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboEl();
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListFileTst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDel();
	afx_msg void OnButtonDelAll();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELTSTDLG_H__555FF293_6956_42B6_B513_D799A56F14B0__INCLUDED_)
