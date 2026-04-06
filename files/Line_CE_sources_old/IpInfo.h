#if !defined(AFX_IPINFO_H__E0610DDD_1DAA_455E_826A_FFCEE5F0655C__INCLUDED_)
#define AFX_IPINFO_H__E0610DDD_1DAA_455E_826A_FFCEE5F0655C__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// IpInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIpInfo dialog

class CIpInfo : public CDialog
{
// Construction
public:
	CIpInfo(CWnd* pParent = NULL);   // standard constructor
	BOOL GetIPAddressPCM(CString &strIP); 
	BOOL CheckIP();
	BOOL SetIPAddressPCM(CString strIP);

// Dialog Data
	//{{AFX_DATA(CIpInfo)
	enum { IDD = IDD_IP_DLG };
	CGradBtn	m_btnOK;
	CGradBtn	m_btnCancel;
	CEdit	m_edt_ip;
	CString	m_edt_tmp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIpInfo)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeIP();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPINFO_H__E0610DDD_1DAA_455E_826A_FFCEE5F0655C__INCLUDED_)
