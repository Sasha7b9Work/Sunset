#if !defined(AFX_KLAVA_H__E5C3179B_5068_4932_8E59_42B910DED41A__INCLUDED_)
#define AFX_KLAVA_H__E5C3179B_5068_4932_8E59_42B910DED41A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Klava.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKlava dialog

class CKlava : public CDialog
{
// Construction
public:
	CKlava(CWnd* pParent = NULL);   // standard constructor
	BOOL Create();
	void ChangeLanguage(CString alphabet1,CString alphabet2,CString alphabet3);
	void KlavaDown();
	void KlavaRight();
	void KlavaLeft();
	BOOL KlavaMove(int offer);
	void KlavaPress();

// Dialog Data
	//{{AFX_DATA(CKlava)
	enum { IDD = IDD_KLAVA_DIALOG };
	CButton	m_language;
	CButton	m_btnA;
	//}}AFX_DATA
	CString strBtn;
	CBrush br_button;
	CString strChar1;
	CString strTempChar1;
	int n_move_klava_x;//коорд. 'x' верхнего левого угла клавы
	int n_move_klava_y;//коорд. 'y' верхнего левого угла клавы
	int n123;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKlava)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;
	int nLanguage;
	int nLanguage1;
	int	nCapsLock;
	int	nShift;

	// Generated message map functions
	//{{AFX_MSG(CKlava)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnEsc();
	afx_msg void OnBtnShift();
	afx_msg void OnBtn123();
	afx_msg void OnBtnA1();
	afx_msg void OnBtnB1();
	afx_msg void OnBtnBackspace();
	afx_msg void OnBtnC1();
	afx_msg void OnBtnCapslock();
	afx_msg void OnBtnD1();
	afx_msg void OnBtnDvt();
	afx_msg void OnBtnE1();
	afx_msg void OnBtnF();
	afx_msg void OnBtnFsk1();
	afx_msg void OnBtnFsk2();
	afx_msg void OnBtnG1();
	afx_msg void OnBtnH1();
	afx_msg void OnBtnI1();
	afx_msg void OnBtnJ1();
	afx_msg void OnBtnK1();
	afx_msg void OnBtnL1();
	afx_msg void OnBtnLanguage();
	afx_msg void OnBtnM1();
	afx_msg void OnBtnMinys();
	afx_msg void OnBtnN1();
	afx_msg void OnBtnO1();
	afx_msg void OnBtnP1();
	afx_msg void OnBtnPlus1();
	afx_msg void OnBtnProbel();
	afx_msg void OnBtnQ1();
	afx_msg void OnBtnR1();
	afx_msg void OnBtnS1();
	afx_msg void OnBtnT1();
	afx_msg void OnBtnThk();
	afx_msg void OnBtnTz();
	afx_msg void OnBtnU1();
	afx_msg void OnBtnV1();
	afx_msg void OnBtnW1();
	afx_msg void OnBtnX1();
	afx_msg void OnBtnY1();
	afx_msg void OnBtnZ1();
	afx_msg void OnBtnZpt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KLAVA_H__E5C3179B_5068_4932_8E59_42B910DED41A__INCLUDED_)
