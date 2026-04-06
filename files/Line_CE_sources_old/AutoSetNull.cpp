// AutoSetNull.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "AutoSetNull.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoSetNull dialog
CRect RectDlg;
CRect RectCxema;

static UINT bmpCxema[]={IDC_STATIC_CXEMA1,
						IDC_STATIC_CXEMA2,
						IDC_STATIC_CXEMA1,
						IDC_STATIC_CXEMA3,
						IDC_STATIC_CXEMA3,
						IDC_STATIC_CXEMA1,
						IDC_STATIC_CXEMA1,
						IDC_STATIC_CXEMA4,
						IDC_STATIC_CXEMA4,
						IDC_STATIC_CXEMA1,
						IDC_STATIC_CXEMA5};

CAutoSetNull::CAutoSetNull(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoSetNull::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoSetNull)
	m_iNset = 0;
	m_sUkaz = _T("");
	m_iRange = 0;
	//}}AFX_DATA_INIT
}


void CAutoSetNull::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoSetNull)
	DDX_Control(pDX, IDCANCEL, m_bvyxod);
	DDX_Control(pDX, IDOK, m_bpusk);
	DDX_Radio(pDX, IDC_RADIO_ICM, m_iNset);
	DDX_Text(pDX, IDC_STATIC_MES, m_sUkaz);
	DDX_CBIndex(pDX, IDC_COMBO_RANGE, m_iRange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoSetNull, CDialog)
	//{{AFX_MSG_MAP(CAutoSetNull)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_ICM, OnRadioIcm)
	ON_BN_CLICKED(IDC_RADIO_IBS, OnRadioIbs)
	ON_BN_CLICKED(IDC_RADIO_IBM, OnRadioIbm)
	ON_BN_CLICKED(IDC_RADIO_ISM, OnRadioIsm)
	ON_BN_CLICKED(IDC_RADIO_ISS, OnRadioIss)
	ON_BN_CLICKED(IDC_RADIO_UBM, OnRadioUbm)
	ON_BN_CLICKED(IDC_RADIO_UBS, OnRadioUbs)
	ON_BN_CLICKED(IDC_RADIO_UCM, OnRadioUcm)
	ON_BN_CLICKED(IDC_RADIO_USM, OnRadioUsm)
	ON_BN_CLICKED(IDC_RADIO_USS, OnRadioUss)
	ON_BN_CLICKED(IDC_RADIO_ICM_10N, OnRadioIcm10n)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoSetNull message handlers

HBRUSH CAutoSetNull::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	((pWnd -> GetDlgCtrlID() == IDC_STATIC_TITLE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_INFO))
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	if	(
//		(pWnd -> GetDlgCtrlID() == IDC_STATIC)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_ICM)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_IBM)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_UBM)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_ISM)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_USM)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_UBS)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_USS)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_UCM)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_IBS)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_ISS))
	{	hbr = hbr;
	 //	hbr = m_brushGrayLight;//Black;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//134,134,134));//0,0,0));
		pDC->SetTextColor(RGB(0,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CAutoSetNull::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CClientDC dc(this);	
	GetClientRect(RectDlg);
	RectCxema.left   = RectDlg.left+(RectDlg.right-RectDlg.left)/2+10;//20;
	RectCxema.right  = RectCxema.left+139;
	RectCxema.top    = RectDlg.top+(RectDlg.bottom-RectDlg.top)/4+20;//6;
	RectCxema.bottom = RectCxema.top+101;
	UpdateData();
	ShowCxema();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void	CAutoSetNull::ShowCxema()
{	CWnd* pwnd;

	//всё стереть
    ShowControls(bmpCxema,SIZEOF_ARRAY(bmpCxema),FALSE);

	pwnd=GetDlgItem(bmpCxema[m_iNset]);
	if (pwnd)	{ pwnd->MoveWindow(&RectCxema);
				  pwnd->ShowWindow(SW_SHOW);
				}
	UpdateWindow();
		pwnd=GetDlgItem(IDC_COMBO_RANGE);
	if (m_iNset==10)
	{
		pwnd->ShowWindow(SW_SHOW);
		m_sUkaz = _T("Подключите к УК все элементы для подключения объекта тестирования. Объект не устанавливать.\nОбеспечьте соединение попарно гнезд С и С', E и E', а также гнезд Uc и E' перемычкой \".027\". \n Установите экран.");
	}
	else
	{
		pwnd->ShowWindow(SW_HIDE);
	m_sUkaz = _T("Подключите устройство контактирующее и соедините гнезда с помощью перемычек согласно схеме.");
	}
	UpdateData(false);

}

void CAutoSetNull::ShowControls(UINT* pControls,UINT cControls,BOOL fVisible)
{ for (UINT uIndex=0;uIndex<cControls;uIndex++)
	{ CWnd* pwnd=GetDlgItem(pControls[uIndex]);
	  if (pwnd)
	  { pwnd->ShowWindow(fVisible? SW_SHOW:SW_HIDE);
	    pwnd->EnableWindow(fVisible);
	  }
	}
}

void CAutoSetNull::OnRadioIcm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioIbs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioIbm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioIsm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioIss() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioUbm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioUbs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioUcm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioUsm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioUss() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}

void CAutoSetNull::OnRadioIcm10n() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ShowCxema();
	UpdateData(false);
	
}
