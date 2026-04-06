#if !defined(AFX_VALENTER_H__FFC6AC62_2C7A_4DEC_9003_1DA343798121__INCLUDED_)
#define AFX_VALENTER_H__FFC6AC62_2C7A_4DEC_9003_1DA343798121__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ValEnter.h : header file
//
#include "GradBtn.h"
#include "Klava.h"

/////////////////////////////////////////////////////////////////////////////
// CValEnter dialog

class CValEnter : public CDialog
{
// Construction
public:
	CValEnter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CValEnter)
	enum { IDD = IDD_VALENTER_DLG };
	CGradBtn	m_btnCancel;
	CGradBtn	m_btnOk;
	CString	m_sValEnter;
	CString	m_sRazm;
	//}}AFX_DATA
	BOOL    m_bWhat;	//false - показания внешнего прибора
						//true - номинал резистора
	int		PushKlava(CString strSymbol);
	CString GetStringFromEdit(); 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CValEnter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CKlava *m_pKlava;

	// Generated message map functions
	//{{AFX_MSG(CValEnter)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALENTER_H__FFC6AC62_2C7A_4DEC_9003_1DA343798121__INCLUDED_)
