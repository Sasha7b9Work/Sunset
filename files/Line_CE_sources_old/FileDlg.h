#if !defined(AFX_FILEDLG_H__50C743E9_F589_4799_929A_F9C20B9EF0E8__INCLUDED_)
#define AFX_FILEDLG_H__50C743E9_F589_4799_929A_F9C20B9EF0E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FileDlg.h : header file
//
#include "GradBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

class CFileDlg : public CDialog
{
// Construction
public:
	CFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileDlg)
	enum { IDD = IDD_FILE_DIALOG };
	CGradBtn	m_btnDelete;
	CListCtrl	m_listFile;
	CGradBtn	m_btnCancel;
	CGradBtn	m_btnOK;
	CString	m_sName;
	BOOL	m_bZaschParol;
	CString	m_sTitle;
	CString	m_sFilter;
	int		m_isposob;
	//}}AFX_DATA

	int	    m_iWhatDo;			//что делать: 0 - открыть, 1 - сохранить
	CString	m_sPath;
	CString	m_sCatalog;
	CString	m_sFullNameFile;
	void	BuildListFile();
	int     m_itip;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_imyImageList;

	// Generated message map functions
	//{{AFX_MSG(CFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListFile(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonDelete();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDLG_H__50C743E9_F589_4799_929A_F9C20B9EF0E8__INCLUDED_)
