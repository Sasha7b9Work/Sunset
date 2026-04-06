// KalibrDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "KalibrDlg.h"
#include "ParolDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKalibrDlg dialog


CKalibrDlg::CKalibrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKalibrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKalibrDlg)
	m_sserial = _T("");
	m_sdatek = _T("");
	m_iNsetK = 0;
	m_iRange = 0;
	m_sMes = _T("");
	//}}AFX_DATA_INIT
}


void CKalibrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKalibrDlg)
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDCANCEL, m_bCancel);
	DDX_Control(pDX, IDOK, m_bOk);
	DDX_Text(pDX, IDC_EDIT_SERIAL, m_sserial);
	DDX_Text(pDX, IDC_EDIT_DATEKALIBR, m_sdatek);
	DDX_Radio(pDX, IDC_RADIO_ICM, m_iNsetK);
	DDX_CBIndex(pDX, IDC_COMBO_RANGE, m_iRange);
	DDX_Text(pDX, IDC_STATIC_MES, m_sMes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKalibrDlg, CDialog)
	//{{AFX_MSG_MAP(CKalibrDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_RANGE, OnSelchangeComboRange)
	ON_BN_CLICKED(IDC_RADIO_ICM, OnRadioIcm)
	ON_BN_CLICKED(IDC_RADIO_IB, OnRadioIb)
	ON_BN_CLICKED(IDC_RADIO_IS, OnRadioIs)
	ON_BN_CLICKED(IDC_RADIO_UB, OnRadioUb)
	ON_BN_CLICKED(IDC_RADIO_UCM, OnRadioUcm)
	ON_BN_CLICKED(IDC_RADIO_US, OnRadioUs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKalibrDlg message handlers

HBRUSH CKalibrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	((pWnd -> GetDlgCtrlID() == IDC_STATIC_TITLE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_CHANEL_C)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_CHANEL_B)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_CHANEL_S)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_INFO)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_RANGE))
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CKalibrDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
if (MessageBox(_T("Вы уверены, что хотите сохранить таблицу коэффициентов?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
{
	CParolDlg dlgp;
	dlgp.what_save = FILE_PSW;
	UpdateData(false);
	if (dlgp.DoModal()==IDOK)
	{   UpdateData();
		if (m_sparol == dlgp.m_sparol) 
		{ //пароль верный
			BOOL b = WriteKoefToPsw();
			if (b) MessageBox(_T("Сохранение завершено успешно!"),NULL,MB_OK|MB_ICONINFORMATION);
			else   MessageBox(_T("Недостаточно места на диске! Таблица коэффициентов не сохранена!"),NULL,MB_OK|MB_ICONERROR);
		}
		else MessageBox(_T("Неверный пароль! Таблица коэффициентов не сохранена!"),NULL,MB_OK|MB_ICONERROR);
	}
	else MessageBox(_T("Пароль не введен! Таблица коэффициентов не сохранена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
}	
}

BOOL CKalibrDlg::WriteKoefToPsw()
{ BOOL b;
  CFile file;
  CString str  = _T("");
  DWORD l;
  int i;

   b = file.Open((LPCTSTR)fileKoefPsw,CFile::modeCreate|CFile::modeWrite);
	if (b)
	{//if b
	 file.SeekToBegin();	
	   //заводской номер прибора
       str.Format(_T("strpsw=%s;\r\n"),strpsw);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //модификация ИППП:
	   str.Format(_T("n_IPPP=%d;\r\n"),n_IPPP);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //дата последней калибровки
	   m_sdatek = strDateCurrent;
       str.Format(_T("strdatekalibr=%s;\r\n"),m_sdatek);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //коллектор (источник)
	   //номинальное значение кода сети (1 байт) - общее для всех диапазонов
	   str.Format(_T("nominal=%d;\r\n"),nominal);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //максимальный код ЦАПа Da для каждого диапазона (5 диапазонов) 
/*	   for (i=0;i<5;i++)
	   { str.Format(_T("V_[%d]=%d;\r\n"),i,V_[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
*/	  //максимальный код ЦАПа Dz (1 байт) 
/*	   str.Format(_T("Nz_max=%d;\r\n"),Nz_max);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	  //минимальный код ЦАПа Dz (1 байт) 
	   str.Format(_T("Nz_min=%d;\r\n"),Nz_min);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
*/	 //калибровочные коэффициенты
	   //КОЛЛЕКТОР
	   str=_T("[COLLECTOR]\r\n[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<13;i++)
	   { str.Format(_T("BMCU[%d]=%.5e;\r\n"),i,BMCU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<13;i++)
	   { str.Format(_T("KMCU[%d]=%.5e;\r\n"),i,KMCU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("BMCI[%d]=%.5e;\r\n"),i,BMCI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("BMCI_DUBL[%d]=%.5e;\r\n"),i,BMCI_DUBL[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("KMCI[%d]=%.5e;\r\n"),i,KMCI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("KMCI_DUBL[%d]=%.5e;\r\n"),i,KMCI_DUBL[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   //БАЗА
	   str=_T("[BASE]\r\n[SOURCE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BSBU[%d]=%.5e;\r\n"),i,BSBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KSBU[%d]=%.5e;\r\n"),i,KSBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[SOURCE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BSBI[%d]=%.5e;\r\n"),i,BSBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KSBI[%d]=%.5e;\r\n"),i,KSBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION U]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BLBU[%d]=%.5e;\r\n"),i,BLBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KLBU[%d]=%.5e;\r\n"),i,KLBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION I]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BLBI[%d]=%.5e;\r\n"),i,BLBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KLBI[%d]=%.5e;\r\n"),i,KLBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BMBU[%d]=%.5e;\r\n"),i,BMBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KMBU[%d]=%.5e;\r\n"),i,KMBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BMBI[%d]=%.5e;\r\n"),i,BMBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KMBI[%d]=%.5e;\r\n"),i,KMBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   //ПОДЛОЖКА
	   str=_T("[SUBSTRATE]\r\n[SOURCE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BSDU[%d]=%.5e;\r\n"),i,BSDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KSDU[%d]=%.5e;\r\n"),i,KSDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[SOURCE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BSDI[%d]=%.5e;\r\n"),i,BSDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KSDI[%d]=%.5e;\r\n"),i,KSDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION U]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BLDU[%d]=%.5e;\r\n"),i,BLDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KLDU[%d]=%.5e;\r\n"),i,KLDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION I]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BLDI[%d]=%.5e;\r\n"),i,BLDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KLDI[%d]=%.5e;\r\n"),i,KLDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BMDU[%d]=%.5e;\r\n"),i,BMDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KMDU[%d]=%.5e;\r\n"),i,KMDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BMDI[%d]=%.5e;\r\n"),i,BMDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KMDI[%d]=%.5e;\r\n"),i,KMDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	 file.Close();
	}
	return b;
}


BOOL CKalibrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitRange();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKalibrDlg::InitRange() 
{
  int i=0;
  int n_r=0;
  int n_k=0;
  if (n_IPPP==0 || (n_IPPP==1 && n_Switch==0)) {n_r=5; n_k=MAXRANGEIKOL;}//4;
  if (n_IPPP==1 && n_Switch!=0) {n_r=0; n_k=4;}
    CComboBox *ptr = (CComboBox*) GetDlgItem(IDC_COMBO_RANGE);
    ptr->ResetContent();
	switch(m_iNsetK)
	{ 
	  case 0:   //Канал С Измеритель I
//			    for(i=n_r;i<MAXRANGEIKOL;i=i+1)			
			    for(i=n_r;i<n_k;i=i+1)			
				ptr->AddString(mas_rangeKolMeasI[i]);
				if ((m_iRange+n_r)>=21)
				m_sMes = _T("  Соберите схему для определения основной погрешности измерения тока по каналу С на диапазонах от 10 нА до 100 мкА (рис В.6 РЭ) и установите резистор Rн");
				if ((m_iRange+n_r)>=8 && (m_iRange+n_r)<21)
				m_sMes = _T("  Соберите схему для определения основной погрешности измерения тока по каналу С на диапазонах от 200 мкА до 2А (рис В.7 РЭ)");//(рис 5.5 ТУ)
//				if ((m_iRange+n_r)==6)
//				m_sMes = _T("  Соберите схему для определения основной погрешности измерения тока по каналу С на диапазонах от 10 А до 50 А (рис 5.6 ТУ) и установите резистор номиналом 1 Ом");
				if ((m_iRange+n_r)<8)
				m_sMes = _T("  Соберите схему для определения основной погрешности измерения тока по каналу С на диапазонах от 5 А до 50 А (рис В.8 РЭ) и установите резистор Rн");
				break;
	  case 1:   //Канал С Измеритель U
				for(i=0;i<=12;i=i+1)			
				ptr->AddString(mas_rangeKolMeasU[i]);
				m_sMes = _T("  Соберите схему для определения основной погрешности измерения напряжения по каналу С (рис. В.5 РЭ)");
				break;
	  case 2:   //Канал B Измеритель I
				for (i=0;i<27;i++)			// от 20 A до 40 nA
				ptr->AddString(mas_range_meas_bazadop_I[i]);
				m_sMes = _T("  Соберите схему для определения основной погрешности формирования и измерения тока по каналу B (рис В.3 РЭ)");
				break;
	  case 3:   //Канал B Измеритель U
				for (i=0;i<6;i++)			//  от 1V до 40 V
				ptr->AddString(mas_range_meas_bazadop_U[i]);
				m_sMes = _T("  Соберите схему для определения основной погрешности формирования и измерения напряжения по каналу B (рис. В.4 РЭ)");
				break;
	  case 4:   //Канал S Измеритель I
				for (i=10;i<27;i++)			// от 10 mA до 40 nA
				ptr->AddString(mas_range_meas_bazadop_I[i]);
				m_sMes = _T("  Соберите схему для определения основной погрешности формирования и измерения тока по каналу S (рис В.1 РЭ)");
				break;
	  case 5:   //Канал S Измеритель U
				for (i=0;i<6;i++)			// от 1 V до 40 V
				ptr->AddString(mas_range_meas_bazadop_U[i]);
				m_sMes = _T("  Соберите схему для определения основной погрешности формирования и измерения напряжения по каналу S (рис В.2 РЭ)");
				break;
	}
	UpdateData(false);
}

void CKalibrDlg::OnSelchangeComboRange() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	InitRange();
	UpdateData(false);
}

void CKalibrDlg::OnRadioIcm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iRange = 0;
	InitRange();
	UpdateData(false);
	
}

void CKalibrDlg::OnRadioIb() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iRange = 0;
	InitRange();
	UpdateData(false);
	
}

void CKalibrDlg::OnRadioIs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iRange = 0;
	InitRange();
	UpdateData(false);
	
}

void CKalibrDlg::OnRadioUb() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iRange = 0;
	InitRange();
	UpdateData(false);
	
}

void CKalibrDlg::OnRadioUcm() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iRange = 0;
	InitRange();
	UpdateData(false);
	
}

void CKalibrDlg::OnRadioUs() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iRange = 0;
	InitRange();
	UpdateData(false);
	
}
