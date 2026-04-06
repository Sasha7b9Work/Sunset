#if !defined(AFX_GRADBTN_H__5D96A39A_C8AE_4706_A23F_13FF6F258785__INCLUDED_)
#define AFX_GRADBTN_H__5D96A39A_C8AE_4706_A23F_13FF6F258785__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GradBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGradBtn window

class CGradBtn : public CButton
{
// Construction
public:
	CGradBtn();

// Attributes
public:
	bool	m_bActive;
	bool	m_bClick;
	int		m_ntimer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGradBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGradBtn)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	void GradientRectDL(HDC pDC, CRect ARect);
	void GradientRectLD(HDC pDC, CRect ARect);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADBTN_H__5D96A39A_C8AE_4706_A23F_13FF6F258785__INCLUDED_)
