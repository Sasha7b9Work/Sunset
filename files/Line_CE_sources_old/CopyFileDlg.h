#if !defined(AFX_COPYFILEDLG_H__7315EF86_544E_4791_B06A_49C4F3FC4616__INCLUDED_)
#define AFX_COPYFILEDLG_H__7315EF86_544E_4791_B06A_49C4F3FC4616__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CopyFileDlg.h : header file
//
#include "GradBtn.h"
/////////////////////////////////////////////////////////////////////////////
// CCopyFileDlg dialog

class CCopyFileDlg : public CDialog
{
// Construction
public:
	CCopyFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyFileDlg)
	enum { IDD = IDD_COPYFILE_DLG };
	CListCtrl	m_listUsb;
	CListCtrl	m_listDisk;
	CGradBtn	m_btnMove;
	CGradBtn	m_btnOK;
	CGradBtn	m_btnCancel;
	int		m_itip;
	CString	m_sName;
	CString	m_sDiskCatalog;
	CString	m_sUsbCatalog;
	//}}AFX_DATA
	CBrush      m_brushBlack;
	CString		m_sWhereFrom;
	CString		m_sWhereTo;
	CString		m_sDisk;
	CString		m_sServer;
	CString		m_sUsb;
	CString		m_sCatalog;
	CString		m_sExt;
	int			m_iFromTo;		//направление
	int			m_iswitch;		//переключатель удаленного носителя
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_imyImageList;
	CImageList	m_imyImageListUsb;
	void	BuildListFileDisk(); 
	void	BuildListFileUsb(); 
	BOOL	CopyFile(int what_do);
	void	BuildPathDiskToUsb(int n);
	void	BuildPathUsbToDisk(int n);
	// Generated message map functions
	//{{AFX_MSG(CCopyFileDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFilter();
	afx_msg void OnDblclkListDisk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListUsb(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopyFile();
	afx_msg void OnButtonMove();
	afx_msg void OnStaticHelp();
	afx_msg void OnStaticIconSwitch();
	afx_msg void OnClickListDisk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListUsb(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYFILEDLG_H__7315EF86_544E_4791_B06A_49C4F3FC4616__INCLUDED_)
