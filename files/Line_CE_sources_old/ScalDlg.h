#if !defined(AFX_SCALDLG_H__6EFE3B1D_922D_424F_8215_9D9E85535590__INCLUDED_)
#define AFX_SCALDLG_H__6EFE3B1D_922D_424F_8215_9D9E85535590__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ScalDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScalDlg dialog

class CScalDlg : public CDialog
{
// Construction
public:
	CScalDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScalDlg)
	enum { IDD = IDD_SCALE_DLG };
	BOOL	m_bInversion;
	//}}AFX_DATA

	int m_iXY;	//0-ось Х, 1 - ось Y
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScalDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALDLG_H__6EFE3B1D_922D_424F_8215_9D9E85535590__INCLUDED_)
