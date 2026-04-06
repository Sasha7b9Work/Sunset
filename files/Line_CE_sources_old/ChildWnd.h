#if !defined(AFX_CHILDWND_H__E486F488_C400_42DA_A574_B6D2919588D7__INCLUDED_)
#define AFX_CHILDWND_H__E486F488_C400_42DA_A574_B6D2919588D7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ChildWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildWnd frame

class CChildWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CChildWnd)
protected:

// Attributes
public:
	CEdit	   m_editOtcht;		//поле для отображения отчёта
	CFont	   m_fontOtcht;
	CTreeCtrl  m_TreeCtrl;

// Operations
public:
	CChildWnd();           // protected constructor used by dynamic creation

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildWnd();

	// Generated message map functions
	//{{AFX_MSG(CChildWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDWND_H__E486F488_C400_42DA_A574_B6D2919588D7__INCLUDED_)
