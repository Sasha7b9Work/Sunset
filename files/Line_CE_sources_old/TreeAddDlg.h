#if !defined(AFX_TREEADDDLG_H__34D4AD22_AD6C_453F_9661_FA3FE7B1AC0F__INCLUDED_)
#define AFX_TREEADDDLG_H__34D4AD22_AD6C_453F_9661_FA3FE7B1AC0F__INCLUDED_
#include "GradBtn.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TreeAddDlg.h : header file
//
class CLineDlg;
/////////////////////////////////////////////////////////////////////////////
// CTreeAddDlg dialog

class CTreeAddDlg : public CDialog
{
// Construction
public:
	CTreeAddDlg(CWnd* pParent = NULL);   // standard constructor
	int index;				//индекс: -1 - имя теста не найдено в списке тестов,
							//       >=0 - индекс теста в списке

// Dialog Data
	//{{AFX_DATA(CTreeAddDlg)
	enum { IDD = IDD_ADD_TREE_DLG };
	CGradBtn	m_btn_add;
	CString	m_EL;
	CString	m_MOD;
	int		m_nelement;
	//}}AFX_DATA

	void		BuildArrayTstStandart();
	void		SetListTestTst();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLineDlg* m_pDlg;

	// Generated message map functions
	//{{AFX_MSG(CTreeAddDlg)
	virtual void OnOK();
	afx_msg void OnButadd();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombocategoria();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEADDDLG_H__34D4AD22_AD6C_453F_9661_FA3FE7B1AC0F__INCLUDED_)
