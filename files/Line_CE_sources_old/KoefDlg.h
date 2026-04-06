#if !defined(AFX_KOEFDLG_H__21E6BD77_2526_446D_B935_1AAB0F36B92A__INCLUDED_)
#define AFX_KOEFDLG_H__21E6BD77_2526_446D_B935_1AAB0F36B92A__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KoefDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKoefDlg dialog

class CKoefDlg : public CDialog
{
// Construction
public:
	CKoefDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKoefDlg)
	enum { IDD = IDD_TABL_KOEF_DLG };
	CGradBtn	m_buttonApply;
	CGradBtn	m_buttonCancel;
	CGradBtn	m_buttonOk;
	int		m_iBias;
	int		m_iNset;
	//}}AFX_DATA
	 double		 bmci[MAXRANGEIKOL];
	 double		 kmci[MAXRANGEIKOL];
	 double		 bmcu[13];
	 double		 kmcu[13];
	 double		 bmbi[27];
	 double		 kmbi[27];
	 double		 blbi[27];
	 double		 klbi[27];
	 double		 bmbu[6];
	 double		 kmbu[6];
	 double		 blbu[6];
	 double		 klbu[6];
	 double		 bsbi[27];
	 double		 ksbi[27];
	 double		 bsbu[6];
	 double		 ksbu[6];
	 double		 bmdi[17];
	 double		 kmdi[17];
	 double		 bmdu[6];
	 double		 kmdu[6];
	 double		 bsdi[17];
	 double		 ksdi[17];
	 double		 bsdu[6];
	 double		 ksdu[6];
	 double		 bldi[17];
	 double		 kldi[17];
	 double		 bldu[6];
	 double		 kldu[6];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKoefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKoefDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioBias();
	afx_msg void OnRadioCoef();
	afx_msg void OnRadioUcm();
	afx_msg void OnRadioIbm();
	afx_msg void OnRadioIbs();
	afx_msg void OnRadioIcm();
	afx_msg void OnRadioIsm();
	afx_msg void OnRadioIss();
	afx_msg void OnRadioUbm();
	afx_msg void OnRadioUbs();
	afx_msg void OnRadioUsm();
	afx_msg void OnRadioUss();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonApply();
	afx_msg void OnRadioLbu();
	afx_msg void OnRadioLbi();
	afx_msg void OnRadioLdi();
	afx_msg void OnRadioLdu();
	//}}AFX_MSG
	void InitTablCoef();
	BOOL GetDoubleFromString(CString str,double& receive);
	BOOL Apply();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KOEFDLG_H__21E6BD77_2526_446D_B935_1AAB0F36B92A__INCLUDED_)
