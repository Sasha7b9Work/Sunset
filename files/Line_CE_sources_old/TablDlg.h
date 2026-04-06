#if !defined(AFX_TABLDLG_H__C487744D_0AB8_43A6_BB50_96420F1CE258__INCLUDED_)
#define AFX_TABLDLG_H__C487744D_0AB8_43A6_BB50_96420F1CE258__INCLUDED_
#include "GradBtn.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TablDlg.h : header file
//
class CLineDlg;

/////////////////////////////////////////////////////////////////////////////
// CTablDlg dialog

class CTablDlg : public CDialog
{
// Construction
public:
	CTablDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTablDlg)
	enum { IDD = IDD_TABLDLG };
	CGradBtn	m_btn_savetabres;
	CListCtrl	m_listRes;
	//}}AFX_DATA
	CString	strInfoTest;

	CString strDateMeas;
	CString strTimeMeas;
	CString strElement;
	CString strPrim;
	CString strOtcht;		//строка расчетных значений из отсчета
	CString strNameColumn;	//строка наименований колонок измеренных данных
	CString	m_sKolMeasU;
	CString	m_sKolMeasI;
	CString	m_sBazaSource;
	CString	m_sBazaMeas;
	CString	m_sDopSource;
	CString	m_sDopMeas;
	int		m_iCircuitBaza;
	int		m_iCircuitDop;
	int		m_isposob;
	int		m_iBazaMeas;
	int		m_iDopMeas;
	BOOL	m_bBaza;
	BOOL	m_bDop;
	BOOL	m_bR;
	BOOL	m_bS;
	BOOL	m_bBeta;
	BOOL	m_bBetai;
	int		rej_Y;
	int		m_itipimp;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTablDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CLineDlg* m_pdlg;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTablDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButsavetabres();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLDLG_H__C487744D_0AB8_43A6_BB50_96420F1CE258__INCLUDED_)
