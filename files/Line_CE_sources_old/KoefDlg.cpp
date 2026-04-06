// KoefDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "KoefDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKoefDlg dialog
static UINT mas_Range[]={ IDC_STATIC1, IDC_STATIC2, IDC_STATIC3, IDC_STATIC4, IDC_STATIC5, IDC_STATIC6,
						  IDC_STATIC7, IDC_STATIC8, IDC_STATIC9,IDC_STATIC10,IDC_STATIC11,IDC_STATIC12,
						 IDC_STATIC13,IDC_STATIC14,IDC_STATIC15,IDC_STATIC16,IDC_STATIC17,IDC_STATIC18,
						 IDC_STATIC19,IDC_STATIC20,IDC_STATIC21,IDC_STATIC22,IDC_STATIC23,IDC_STATIC24,
						 IDC_STATIC25,IDC_STATIC26,IDC_STATIC27,IDC_STATIC28,IDC_STATIC29,IDC_STATIC30,
						 IDC_STATIC31,IDC_STATIC32,IDC_STATIC33,IDC_STATIC34};
static UINT mas_Value[]={ IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6,
						  IDC_EDIT7, IDC_EDIT8, IDC_EDIT9,IDC_EDIT10,IDC_EDIT11,IDC_EDIT12,
						 IDC_EDIT13,IDC_EDIT14,IDC_EDIT15,IDC_EDIT16,IDC_EDIT17,IDC_EDIT18,
						 IDC_EDIT19,IDC_EDIT20,IDC_EDIT21,IDC_EDIT22,IDC_EDIT23,IDC_EDIT24,
						 IDC_EDIT25,IDC_EDIT26,IDC_EDIT27,IDC_EDIT28,IDC_EDIT29,IDC_EDIT30,
						 IDC_EDIT31,IDC_EDIT32,IDC_EDIT33,IDC_EDIT34};

CKoefDlg::CKoefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKoefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKoefDlg)
	m_iBias = 0;
	m_iNset = 0;
	//}}AFX_DATA_INIT
	
}


void CKoefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKoefDlg)
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_buttonApply);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDOK, m_buttonOk);
	DDX_Radio(pDX, IDC_RADIO_BIAS, m_iBias);
	DDX_Radio(pDX, IDC_RADIO_ICM, m_iNset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKoefDlg, CDialog)
	//{{AFX_MSG_MAP(CKoefDlg)
	ON_BN_CLICKED(IDC_RADIO_BIAS, OnRadioBias)
	ON_BN_CLICKED(IDC_RADIO_COEF, OnRadioCoef)
	ON_BN_CLICKED(IDC_RADIO_UCM, OnRadioUcm)
	ON_BN_CLICKED(IDC_RADIO_IBM, OnRadioIbm)
	ON_BN_CLICKED(IDC_RADIO_IBS, OnRadioIbs)
	ON_BN_CLICKED(IDC_RADIO_ICM, OnRadioIcm)
	ON_BN_CLICKED(IDC_RADIO_ISM, OnRadioIsm)
	ON_BN_CLICKED(IDC_RADIO_ISS, OnRadioIss)
	ON_BN_CLICKED(IDC_RADIO_UBM, OnRadioUbm)
	ON_BN_CLICKED(IDC_RADIO_UBS, OnRadioUbs)
	ON_BN_CLICKED(IDC_RADIO_USM, OnRadioUsm)
	ON_BN_CLICKED(IDC_RADIO_USS, OnRadioUss)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_RADIO_LBU, OnRadioLbu)
	ON_BN_CLICKED(IDC_RADIO_LBI, OnRadioLbi)
	ON_BN_CLICKED(IDC_RADIO_LDI, OnRadioLdi)
	ON_BN_CLICKED(IDC_RADIO_LDU, OnRadioLdu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKoefDlg message handlers

BOOL CKoefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTablCoef();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKoefDlg::InitTablCoef()
{ 
  CString str=_T("");
  int i=0;
  int n_r=0;
  if (n_IPPP==0) n_r=5;//4;
	//Ó˜ËÒÚÍ‡
	for (i=0;i<MAXRANGEIKOL;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(str);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iBias==0)	
	{ if (m_iNset<10)
		((CStatic*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(_T("“¿¡À»÷¿ —Ã≈Ÿ≈Õ»… Õ”Àﬂ"));
	  else
		((CStatic*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(_T("“¿¡À»÷¿ —Ã≈Ÿ≈Õ»… Œ√–¿Õ»◊≈Õ»ﬂ"));
	}
	else  
	{ if (m_iNset<10)
		((CStatic*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(_T("“¿¡À»÷¿  ¿À»¡–Œ¬Œ◊Õ€’  Œ›‘‘»÷»≈Õ“Œ¬"));		
	  else
		((CStatic*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(_T("“¿¡À»÷¿  Œ›‘‘»÷»≈Õ“Œ¬ Œ√–¿Õ»◊≈Õ»ﬂ"));			
	}

	if (m_iNset==0)//Icm
	for (i=0;i<MAXRANGEIKOL-n_r;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_rangeKolMeasI[i+n_r]);
		if (m_iBias==0) str.Format(_T("%.12f"),bmci[i+n_r]);
		          else  str.Format(_T("%.5f"),kmci[i+n_r]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==1)//Ucm
	for (i=0;i<13;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_rangeKolMeasU[i]);
		if (m_iBias==0) str.Format(_T("%.5f"),bmcu[i]);
		          else  str.Format(_T("%.5f"),kmcu[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==2)//Ibm
	for (i=0;i<27;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_I[i]);
		if (m_iBias==0) str.Format(_T("%.12f"),bmbi[i]);
		          else  str.Format(_T("%.6f"),kmbi[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==3)//Ubm
	for (i=0;i<6;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_U[i]);
		if (m_iBias==0) str.Format(_T("%.5f"),bmbu[i]);
		          else  str.Format(_T("%.5f"),kmbu[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==4)//Ibs
	for (i=0;i<27;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_I[i]);//(mas_range_bias_bazadop_I[i]);
		if (m_iBias==0) str.Format(_T("%.12f"),bsbi[i]);
		          else  str.Format(_T("%.5f"),ksbi[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==5)//Ubs
	for (i=0;i<6;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_U[i]);//(mas_range_bias_bazadop_U[i]);
		if (m_iBias==0) str.Format(_T("%.5f"),bsbu[i]);
		          else  str.Format(_T("%.5f"),ksbu[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}

	if (m_iNset==6)//Ism
	for (i=0;i<17;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_I[i+10]);
		if (m_iBias==0) str.Format(_T("%.12f"),bmdi[i]);
		          else  str.Format(_T("%.6f"),kmdi[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==7)//Usm
	for (i=0;i<6;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_U[i]);
		if (m_iBias==0) str.Format(_T("%.5f"),bmdu[i]);
		          else  str.Format(_T("%.5f"),kmdu[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==8)//Iss
	for (i=0;i<17;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_I[i+10]);//(mas_range_bias_bazadop_I[i+10]);
		if (m_iBias==0) str.Format(_T("%.12f"),bsdi[i]);
		          else  str.Format(_T("%.5f"),ksdi[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==9)//Uss
	for (i=0;i<6;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_U[i]);//(mas_range_bias_bazadop_U[i]);
		if (m_iBias==0) str.Format(_T("%.5f"),bsdu[i]);
		          else  str.Format(_T("%.5f"),ksdu[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==10)//LimitBazaU
	for (i=0;i<6;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_U[i]);//(mas_range_bias_bazadop_U[i]);
		if (m_iBias==0) str.Format(_T("%.5f"),blbu[i]);
		          else  str.Format(_T("%.5f"),klbu[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==11)//LimitBazaI
	for (i=0;i<27;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_I[i]);//(mas_range_bias_bazadop_I[i]);
		if (m_iBias==0) str.Format(_T("%.12f"),blbi[i]);
		          else  str.Format(_T("%.5f"),klbi[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==12)//LimitDopU
	for (i=0;i<6;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_U[i]);
		if (m_iBias==0) str.Format(_T("%.5f"),bldu[i]);
		          else  str.Format(_T("%.5f"),kldu[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}
	if (m_iNset==13)//LimitDopI
	for (i=0;i<17;i++)
	{
		((CStatic*)GetDlgItem(mas_Range[i]))->SetWindowText(mas_range_meas_bazadop_I[i+10]);//(mas_range_bias_bazadop_I[i+10]);
		if (m_iBias==0) str.Format(_T("%.12f"),bldi[i]);
		          else  str.Format(_T("%.5f"),kldi[i]);
		((CEdit*)GetDlgItem(mas_Value[i]))->SetWindowText(str);
	}

}

void CKoefDlg::OnRadioBias() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
}

void CKoefDlg::OnRadioCoef() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioUcm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioIbm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioIbs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioIcm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioIsm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioIss() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioUbm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioUbs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioUsm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioUss() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

HBRUSH CKoefDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	((pWnd -> GetDlgCtrlID() == IDC_STATIC_TITLE)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_BIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_COEF)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_LBU)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_LBI)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_LDU)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_LDI)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_KANALC)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_KANALB)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_KANALS))
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CKoefDlg::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	BOOL b = Apply();
	if (!b) MessageBox(_T("ÕÂ‚ÂÌÓ Á‡‰‡ÌÓ ÁÌ‡˜ÂÌËÂ!"),NULL,MB_OK|MB_ICONERROR);
}

BOOL CKoefDlg::GetDoubleFromString(CString str,double& receive)
{
	double num;
	TCHAR tch;
	str.Replace(',', '.');
	char* lpsz=new char[str.GetLength()+1];
	for (int i=0;i<str.GetLength();i++)
	{
	tch=str.GetAt(i);
	if ((tch<L'0')||(tch>L'9')) 
		{if ((tch!=L'.')&&(tch!=L'-')) return FALSE;}
	lpsz[i]=(char)tch;
	}
	lpsz[i]=0;
	num=atof(lpsz);
	delete []lpsz;
	receive=num;
	//return num;
	return TRUE;
}

BOOL CKoefDlg::Apply()
{
	CString str;
	double value;
	int i;

  int n_r=0;
  if (n_IPPP==0) n_r=5;//4;

	if (m_iNset==0)//Icm
	for (i=0;i<MAXRANGEIKOL-n_r;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bmci[i+n_r] = value;
		          else  kmci[i+n_r] = value;
	}
	if (m_iNset==1)//Ucm
	for (i=0;i<13;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bmcu[i] = value;
		          else  kmcu[i] = value;
	}
	if (m_iNset==2)//Ibm
	for (i=0;i<27;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bmbi[i] = value;
		          else  kmbi[i] = value;
	}
	if (m_iNset==3)//Ubm
	for (i=0;i<6;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bmbu[i] = value;
		          else  kmbu[i] = value;
	}
	if (m_iNset==4)//Ibs
	for (i=0;i<27;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bsbi[i] = value;
		          else  ksbi[i] = value;
	}
	if (m_iNset==5)//Ubs
	for (i=0;i<6;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bsbu[i] = value;
		          else  ksbu[i] = value;
	}

	if (m_iNset==6)//Ism
	for (i=0;i<17;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bmdi[i] = value;
		          else  kmdi[i] = value;
	}
	if (m_iNset==7)//Usm
	for (i=0;i<6;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bmdu[i] = value;
		          else  kmdu[i] = value;
	}
	if (m_iNset==8)//Iss
	for (i=0;i<17;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bsdi[i] = value;
		          else  ksdi[i] = value;
	}
	if (m_iNset==9)//Uss
	for (i=0;i<6;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bsdu[i] = value;
		          else  ksdu[i] = value;
	}
	if (m_iNset==10)//BLBU
	for (i=0;i<6;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) blbu[i] = value;
		          else  klbu[i] = value;
	}
	if (m_iNset==11)//BLBI
	for (i=0;i<27;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) blbi[i] = value;
		          else  klbi[i] = value;
	}
	if (m_iNset==12)//BLDU
	for (i=0;i<6;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bldu[i] = value;
		          else  kldu[i] = value;
	}
	if (m_iNset==13)//BLDI
	for (i=0;i<17;i++)
	{
		((CEdit*)GetDlgItem(mas_Value[i]))->GetWindowText(str);
		if (!GetDoubleFromString(str,value)) return FALSE;
		if (m_iBias==0) bldi[i] = value;
		          else  kldi[i] = value;
	}

return true;	
}

void CKoefDlg::OnRadioLbu() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioLbi() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioLdi() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}

void CKoefDlg::OnRadioLdu() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitTablCoef();
    UpdateData(false);
	
}
