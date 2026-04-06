// LupaDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "LupaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLupaDlg dialog


CLupaDlg::CLupaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLupaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLupaDlg)
	m_sScaleX = _T("");
	m_sScaleY = _T("");
	m_uPrecision = 1;
	m_sXmin = _T("");
	m_sXmax = _T("");
	m_sYmax = _T("");
	m_sYmin = _T("");
	m_sStepX = _T("");
	m_sStepY = _T("");
	m_sNpoints = _T("");
	//}}AFX_DATA_INIT
	X_min = 0.0;
	X_max = 0.0;
	Y_min = 0.0;
	Y_max = 0.0;
	StepX = 0.0;
	StepY = 0.0;
	m_bCheckKolRejHigh = false;
	m_bCheckLooping = false;
	m_uNpoints = 20;
	m_irazv = SYN_POS;
	m_iQue1 = COLLECTOR;
}


void CLupaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLupaDlg)
	DDX_Control(pDX, IDCANCEL, m_butCancel);
	DDX_Control(pDX, IDC_OK, m_butOk);
	DDX_Text(pDX, IDC_TEXT_SCALEX, m_sScaleX);
	DDX_Text(pDX, IDC_TEXT_SCALEY, m_sScaleY);
	DDX_Text(pDX, IDC_EDIT_PRECISION, m_uPrecision);
	DDV_MinMaxUInt(pDX, m_uPrecision, 1, 4);
	DDX_Text(pDX, IDC_EDIT_XMIN, m_sXmin);
	DDX_Text(pDX, IDC_EDIT_XMAX, m_sXmax);
	DDX_Text(pDX, IDC_EDIT_YMAX, m_sYmax);
	DDX_Text(pDX, IDC_EDIT_YMIN, m_sYmin);
	DDX_Text(pDX, IDC_EDIT_STEPX, m_sStepX);
	DDX_Text(pDX, IDC_EDIT_STEPY, m_sStepY);
	DDX_CBString(pDX, IDC_COMBO_POINTS, m_sNpoints);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLupaDlg, CDialog)
	//{{AFX_MSG_MAP(CLupaDlg)
	ON_BN_CLICKED(IDC_OK, OnClickedOk)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRECISION, OnDeltaposSpinPrecision)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLupaDlg message handlers

void CLupaDlg::OnOK() 
{
	// TODO: Add extra validation here
	//заглушка
//	CDialog::OnOK();
}

void CLupaDlg::OnClickedOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	
}

HBRUSH CLupaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	((pWnd -> GetDlgCtrlID() == IDC_STATIC_SCALEX)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_SCALEY)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_POINTS)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_PRECISION)||
		(pWnd -> GetDlgCtrlID() == IDC_EDIT_STEPX) ||
		(pWnd -> GetDlgCtrlID() == IDC_EDIT_STEPY))
	{	hbr = hbr;
		pDC->SetTextColor(RGB(0,0,0));//255,255));
	}
	if	((pWnd -> GetDlgCtrlID() == IDC_TEXT_SCALEX)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXT_SCALEY))
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CLupaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//программирование наборного счётчика (точность)
	//**********************************************
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PRECISION);
	pSpin -> SetRange(1,4);
	pSpin -> SetPos(m_uPrecision);
	m_sXmin.Format(_T("%.*f"),m_uPrecision,X_min);
	m_sXmax.Format(_T("%.*f"),m_uPrecision,X_max);
	m_sYmin.Format(_T("%.*f"),m_uPrecision,Y_min);
	m_sYmax.Format(_T("%.*f"),m_uPrecision,Y_max);
	m_sStepX.Format(_T("%.*f"),m_uPrecision,StepX);
	m_sStepY.Format(_T("%.*f"),m_uPrecision,StepY);

    CComboBox *pct = (CComboBox*) GetDlgItem(IDC_COMBO_POINTS);
	pct->ResetContent();
/*	switch(i_tipimp)
	{case -1: 
	 case TIP_SYN:	//режим экранной лупы выключен
			  if (m_irazv==DC_POS || m_irazv==DC_NEG)
			  {
			    pct->AddString(_T("20"));
			    pct->AddString(_T("50"));
			    if (m_uNpoints==20) pct -> SetCurSel(0);
							  else  pct -> SetCurSel(1);
			  }
			  else
			  {
			    if (m_bCheckLooping) pct->AddString(_T("100"));
							else   pct->AddString(_T("200"));
		        pct -> SetCurSel(0);
			  }
			  break;
	  case TIP_IMP:	//режим экранной лупы включен
			  pct->AddString(_T("20"));
			  pct->AddString(_T("50"));
			  if (m_uNpoints==20) pct -> SetCurSel(0);
							else  pct -> SetCurSel(1);
		      break;
	}
*/
			  if (m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG)
			  {
			    if (m_iQue1==COLLECTOR)
				{ pct->AddString(_T("2"));
				  pct->AddString(_T("5"));
				  pct->AddString(_T("10"));
				  pct->AddString(_T("20"));
			      pct->AddString(_T("50"));
//			      if (m_uNpoints==20) pct -> SetCurSel(0);
//							    else  pct -> SetCurSel(1);
				  switch(m_uNpoints)
				  { case 2: pct -> SetCurSel(0); break;
				    case 5: pct -> SetCurSel(1); break;
					case 10: pct -> SetCurSel(2); break;
					case 20: pct -> SetCurSel(3); break;
					case 50: pct -> SetCurSel(4); break;
				  }
				}
				else
				{ m_sNpoints.Format(_T("%d"),m_uNpoints);
				  pct->AddString(m_sNpoints);
		          pct -> SetCurSel(0);
				}
			  }
			  else
			  {
			    if (m_bCheckLooping) pct->AddString(_T("100"));
							else     pct->AddString(_T("200"));
		        pct -> SetCurSel(0);
			  }
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLupaDlg::OnDeltaposSpinPrecision(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData();
	if (pNMUpDown->iDelta>0 && pNMUpDown->iPos<4)
	{
	m_sXmin.Format(_T("%.*f"),m_uPrecision+1,X_min);
	m_sXmax.Format(_T("%.*f"),m_uPrecision+1,X_max);
	m_sYmin.Format(_T("%.*f"),m_uPrecision+1,Y_min);
	m_sYmax.Format(_T("%.*f"),m_uPrecision+1,Y_max);
	m_sStepX.Format(_T("%.*f"),m_uPrecision+1,StepX);
	m_sStepY.Format(_T("%.*f"),m_uPrecision+1,StepY);
	}
	if (pNMUpDown->iDelta<0 && pNMUpDown->iPos>1)
	{
	m_sXmin.Format(_T("%.*f"),m_uPrecision-1,X_min);
	m_sXmax.Format(_T("%.*f"),m_uPrecision-1,X_max);
	m_sYmin.Format(_T("%.*f"),m_uPrecision-1,Y_min);
	m_sYmax.Format(_T("%.*f"),m_uPrecision-1,Y_max);
	m_sStepX.Format(_T("%.*f"),m_uPrecision-1,StepX);
	m_sStepY.Format(_T("%.*f"),m_uPrecision-1,StepY);
	}

	UpdateData(false);
	*pResult = 0;
}


