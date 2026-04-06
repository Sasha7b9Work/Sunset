#if !defined(AFX_SPECIALFILEDIALOG_H__7CCAAB93_B937_434D_80EE_6FD3F61664A8__INCLUDED_)
#define AFX_SPECIALFILEDIALOG_H__7CCAAB93_B937_434D_80EE_6FD3F61664A8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SpecialFileDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpecialFileDialog dialog

class CSpecialFileDialog : public CFileDialog
{
// Construction
public:
	CSpecialFileDialog (BOOL bOpenFileDialog,
		   LPCTSTR lpszDefExt = NULL,
		   LPCTSTR lpszFileName = NULL,
		   DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		   LPCTSTR lpszFilter = NULL,
		   CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpecialFileDialog)
//	enum { IDD = IDD_FILESPECIAL };
	BOOL	m_bZaschitaParol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecialFileDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpecialFileDialog)
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECIALFILEDIALOG_H__7CCAAB93_B937_434D_80EE_6FD3F61664A8__INCLUDED_)
