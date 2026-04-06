// Klava.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "Klava.h"
#include "LineDlg.h"
#include "SaveOtchDlg.h"
#include "SaveRefDlg.h"
#include "SaveTstDlg.h"
#include "TreeAddFromPanelDlg.h"
#include "TestRenameDlg.h"
#include "ValEnter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
	const LPCTSTR alphabetEng1_m=_T("qwertyuiop[]");
	const LPCTSTR alphabetEng2_m=_T("asdfghjkl:;\"");
	const LPCTSTR alphabetEng3_m=_T("zxcvbnm,.-+");
	const LPCTSTR alphabetEng1=_T("QWERTYUIOP[]");
	const LPCTSTR alphabetEng2=_T("ASDFGHJKL:;\"");
	const LPCTSTR alphabetEng3=_T("ZXCVBNM,.-+");
	const LPCTSTR alphabetRus1_m=_T("éöóêåíãøùçõú");
	const LPCTSTR alphabetRus2_m=_T("ôûâàïðîëäæý-");
	const LPCTSTR alphabetRus3_m=_T("ÿ÷ñìèòüáþ¸+");
	const LPCTSTR alphabetRus1=_T("ÉÖÓÊÅÍÃØÙÇÕÚ");
	const LPCTSTR alphabetRus2=_T("ÔÛÂÀÏÐÎËÄÆÝ-");
	const LPCTSTR alphabetRus3=_T("ß×ÑÌÈÒÜÁÞ¨=");	
	const LPCTSTR alphabetNumber1=_T("1234567890_?");
	const LPCTSTR alphabetNumber2=_T("~!@#$%:.*()'");
	const LPCTSTR alphabetNumber3=_T("¹<>{}/|\\°«»");

/////////////////////////////////////////////////////////////////////////////
// CKlava dialog


CKlava::CKlava(CWnd* pParent /*=NULL*/)
	: CDialog(CKlava::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKlava)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CKlava::IDD;
}


void CKlava::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKlava)
	DDX_Control(pDX, IDC_BTN_LANGUAGE, m_language);
	DDX_Control(pDX, IDC_BTN_A1, m_btnA);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKlava, CDialog)
	//{{AFX_MSG_MAP(CKlava)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_ESC, OnBtnEsc)
	ON_BN_CLICKED(IDC_BTN_SHIFT, OnBtnShift)
	ON_BN_CLICKED(IDC_BTN_123, OnBtn123)
	ON_BN_CLICKED(IDC_BTN_A1, OnBtnA1)
	ON_BN_CLICKED(IDC_BTN_B1, OnBtnB1)
	ON_BN_CLICKED(IDC_BTN_BACKSPACE, OnBtnBackspace)
	ON_BN_CLICKED(IDC_BTN_C1, OnBtnC1)
	ON_BN_CLICKED(IDC_BTN_CAPSLOCK, OnBtnCapslock)
	ON_BN_CLICKED(IDC_BTN_D1, OnBtnD1)
	ON_BN_CLICKED(IDC_BTN_DVT, OnBtnDvt)
	ON_BN_CLICKED(IDC_BTN_E1, OnBtnE1)
	ON_BN_CLICKED(IDC_BTN_F, OnBtnF)
	ON_BN_CLICKED(IDC_BTN_FSK1, OnBtnFsk1)
	ON_BN_CLICKED(IDC_BTN_FSK2, OnBtnFsk2)
	ON_BN_CLICKED(IDC_BTN_G1, OnBtnG1)
	ON_BN_CLICKED(IDC_BTN_H1, OnBtnH1)
	ON_BN_CLICKED(IDC_BTN_I1, OnBtnI1)
	ON_BN_CLICKED(IDC_BTN_J1, OnBtnJ1)
	ON_BN_CLICKED(IDC_BTN_K1, OnBtnK1)
	ON_BN_CLICKED(IDC_BTN_L1, OnBtnL1)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE, OnBtnLanguage)
	ON_BN_CLICKED(IDC_BTN_M1, OnBtnM1)
	ON_BN_CLICKED(IDC_BTN_MINYS, OnBtnMinys)
	ON_BN_CLICKED(IDC_BTN_N1, OnBtnN1)
	ON_BN_CLICKED(IDC_BTN_O1, OnBtnO1)
	ON_BN_CLICKED(IDC_BTN_P1, OnBtnP1)
	ON_BN_CLICKED(IDC_BTN_PLUS1, OnBtnPlus1)
	ON_BN_CLICKED(IDC_BTN_PROBEL, OnBtnProbel)
	ON_BN_CLICKED(IDC_BTN_Q1, OnBtnQ1)
	ON_BN_CLICKED(IDC_BTN_R1, OnBtnR1)
	ON_BN_CLICKED(IDC_BTN_S1, OnBtnS1)
	ON_BN_CLICKED(IDC_BTN_T1, OnBtnT1)
	ON_BN_CLICKED(IDC_BTN_THK, OnBtnThk)
	ON_BN_CLICKED(IDC_BTN_TZ, OnBtnTz)
	ON_BN_CLICKED(IDC_BTN_U1, OnBtnU1)
	ON_BN_CLICKED(IDC_BTN_V1, OnBtnV1)
	ON_BN_CLICKED(IDC_BTN_W1, OnBtnW1)
	ON_BN_CLICKED(IDC_BTN_X1, OnBtnX1)
	ON_BN_CLICKED(IDC_BTN_Y1, OnBtnY1)
	ON_BN_CLICKED(IDC_BTN_Z1, OnBtnZ1)
	ON_BN_CLICKED(IDC_BTN_ZPT, OnBtnZpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKlava message handlers

HBRUSH CKlava::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
		if (nCtlColor == CTLCOLOR_BTN)
	{
		CWnd* pWnd1=GetFocus();
		if (pWnd1->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
        hbr = br_button;
		}
  
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
BOOL CKlava::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

BOOL CKlava::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	nCapsLock=0;	//0-ìàëåíüêèå áóêâû
	//n123=0;		//1-öèôðû, 0-áóêâû
	//nSymbols=0;
	nShift=0;			//
	nLanguage=1;	//0-àíãë., 1-ðóññêèé
	br_button.CreateSolidBrush (RGB(165,165,165));
	GetWindowRect(rect);
	rect.left=rect.left+n_move_klava_x;
	rect.right=rect.right+n_move_klava_x;
	rect.top=rect.top+n_move_klava_y;
	rect.bottom=rect.bottom+n_move_klava_y;
	MoveWindow(rect,TRUE);
	m_btnA.SetFocus();
	CButton* p123=(CButton*)GetDlgItem(IDC_BTN_123);
	if (n123==0)
	{ ChangeLanguage(alphabetRus1_m,alphabetRus2_m,alphabetRus3_m);
	  p123->SetWindowText(L"123");}
	else
	{ ChangeLanguage(alphabetNumber1,alphabetNumber2,alphabetNumber3); 
	  p123->SetWindowText(L"abc");}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKlava::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	br_button.DeleteObject();
	delete this;
	
	//CDialog::PostNcDestroy();
}
void CKlava::ChangeLanguage(CString alphabet1,CString alphabet2,CString alphabet3) 
{
	CWnd* pWndNext1;
	CString strCharButton;
 	pWndNext1=GetDlgItem(IDC_BTN_Q1);
	for (int i=0;i<12;i++)
		{
		 strCharButton=alphabet1.GetAt(i);
		 pWndNext1->SetWindowText(strCharButton);
		 pWndNext1=GetNextDlgTabItem(pWndNext1,false);
		 }
	pWndNext1=GetDlgItem(IDC_BTN_A1);
	for (i=0;i<12;i++)
		 {
		  strCharButton=alphabet2.GetAt(i);
	 	  pWndNext1->SetWindowText(strCharButton);
		  pWndNext1=GetNextDlgTabItem(pWndNext1,false);
		 }
	pWndNext1=GetDlgItem(IDC_BTN_C1);
	for (i=0;i<11;i++)
		 {
		  strCharButton=alphabet3.GetAt(i);
		  pWndNext1->SetWindowText(strCharButton);
		  pWndNext1=GetNextDlgTabItem(pWndNext1,false);
		 }
}
void CKlava::KlavaDown()
{
	SetActiveWindow();
	CWnd* pWnd=GetFocus();
	CButton* pTmp=(CButton*)GetDlgItem(IDC_BTN_SHIFT);
	int id_button=pWnd->GetDlgCtrlID();
	switch (id_button)
	{
		case IDC_BTN_ESC:
		case IDC_BTN_Q1:
		case IDC_BTN_W1:
		case IDC_BTN_E1:
		case IDC_BTN_R1:
		case IDC_BTN_T1:
		case IDC_BTN_Y1:
		case IDC_BTN_U1:
		case IDC_BTN_I1:
		case IDC_BTN_O1:
		case IDC_BTN_P1:
		case IDC_BTN_FSK1:
		case IDC_BTN_FSK2:
			pTmp->SetFocus();
			break;
		case IDC_BTN_SHIFT:
		case IDC_BTN_A1:
		case IDC_BTN_S1:
		case IDC_BTN_D1:
		case IDC_BTN_F:
		case IDC_BTN_G1:
		case IDC_BTN_H1:
		case IDC_BTN_J1:
		case IDC_BTN_K1:
		case IDC_BTN_L1:
		case IDC_BTN_Z1:
		case IDC_BTN_X1:
		case IDC_BTN_INCLINE:
			pTmp=(CButton*)GetDlgItem(IDC_BTN_CAPSLOCK);
			pTmp->SetFocus();
			break;
		case IDC_BTN_CAPSLOCK:
		case IDC_BTN_C1:
		case IDC_BTN_V1:
		case IDC_BTN_B1:
		case IDC_BTN_N1:
		case IDC_BTN_M1:
		case IDC_BTN_TZ:
		case IDC_BTN_DVT:
		case IDC_BTN_ZPT:
		case IDC_BTN_THK:
		case IDC_BTN_MINYS:
		case IDC_BTN_PLUS1:
			pTmp=(CButton*)GetDlgItem(IDC_BTN_LANGUAGE);
			pTmp->SetFocus();
			break;
		case IDC_BTN_LANGUAGE:
		case IDC_BTN_PROBEL:
		case IDC_BTN_BACKSPACE:
		case IDC_BTN_123:
			pTmp=(CButton*)GetDlgItem(IDC_BTN_ESC);
			pTmp->SetFocus();
			break;
	}
	pTmp->Invalidate();
}
void CKlava::KlavaRight()
{
	NextDlgCtrl( );	
}
void CKlava::KlavaLeft()
{
	PrevDlgCtrl( );	
}

BOOL CKlava::KlavaMove(int offer)
{
	CRect rect;
	this->GetWindowRect(rect);
	rect.top=rect.top+offer;
	rect.bottom=rect.bottom+offer;
	this->MoveWindow(rect,TRUE);
	return TRUE;
}
void CKlava::KlavaPress()
{
	int count_symbol;
	CButton* p123=(CButton*)GetDlgItem(IDC_BTN_123);
	this->SetActiveWindow();
	CWnd* pWnd=GetFocus();
	int id_button=pWnd->GetDlgCtrlID();
	switch(akt_button)
	{ 
		case 101: 	strChar1=((CSaveTstDlg*)m_pParent)->GetStringFromEdit();break;
		case 104: 	strChar1=((CSaveRefDlg*)m_pParent)->GetStringFromEdit();break;
		case 106: 	strChar1=((CSaveOtchDlg*)m_pParent)->GetStringFromEdit();break;
		case 111: 	strChar1=((CTreeAddFromPanelDlg*)m_pParent)->GetStringFromEdit();break;
		case 121: 	strChar1=((CTestRenameDlg*)m_pParent)->GetStringFromEdit();break;
		case 122: case 142:	strChar1=((CValEnter*)m_pParent)->GetStringFromEdit();break;
	}
	switch (id_button)
	{
	case IDC_BTN_ESC:
		strChar1=_T("");
		switch(akt_button)
		{ 
		  case 101:	((CSaveTstDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 104:	((CSaveRefDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 106:	((CSaveOtchDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 111:	((CTreeAddFromPanelDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 121:	((CTestRenameDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 122:	 case 142:	((CValEnter*)m_pParent)->PushKlava(strChar1);break;
		}
		break;
	case IDC_BTN_SHIFT:
		n123=0;
		if (!nShift)
		{
		if (!nLanguage) ChangeLanguage(alphabetEng1,alphabetEng2,alphabetEng3);
		else  ChangeLanguage(alphabetRus1,alphabetRus2,alphabetRus3);
		nShift=1;
		nCapsLock=1;
		}
		break;
	case IDC_BTN_CAPSLOCK:	
		n123=0;
		if ((nCapsLock==1)&&(nLanguage==0)) ChangeLanguage(alphabetEng1_m,alphabetEng2_m,alphabetEng3_m);
		if ((nCapsLock==0)&&(nLanguage==0)) ChangeLanguage(alphabetEng1,alphabetEng2,alphabetEng3);
		if ((nCapsLock==1)&&(nLanguage==1)) ChangeLanguage(alphabetRus1_m,alphabetRus2_m,alphabetRus3_m);
		if ((nCapsLock==0)&&(nLanguage==1)) ChangeLanguage(alphabetRus1,alphabetRus2,alphabetRus3);
		if (nCapsLock) nCapsLock=0;
		else nCapsLock=1;
		break;
	case IDC_BTN_LANGUAGE:
		n123=0;
		if ((nCapsLock==0)&&(nLanguage==1)) ChangeLanguage(alphabetEng1_m,alphabetEng2_m,alphabetEng3_m);
		if ((nCapsLock==1)&&(nLanguage==1)) ChangeLanguage(alphabetEng1,alphabetEng2,alphabetEng3);
		if ((nCapsLock==0)&&(nLanguage==0)) ChangeLanguage(alphabetRus1_m,alphabetRus2_m,alphabetRus3_m);
		if ((nCapsLock==1)&&(nLanguage==0)) ChangeLanguage(alphabetRus1,alphabetRus2,alphabetRus3);
		if (nLanguage) nLanguage=0;
		else nLanguage=1;
		break;
	case IDC_BTN_PROBEL:
		strChar1+=_T(" ");
		switch(akt_button)
		{ 
		  case 101:	count_symbol=((CSaveTstDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 104:	count_symbol=((CSaveRefDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 106:	count_symbol=((CSaveOtchDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 111:	count_symbol=((CTreeAddFromPanelDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 121:	count_symbol=((CTestRenameDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 122: case 142:		count_symbol=((CValEnter*)m_pParent)->PushKlava(strChar1);break;
		}
		strChar1=strChar1.Left(count_symbol);
		break;
	case IDC_BTN_BACKSPACE:
		strChar1.Delete(strChar1.GetLength()-1,1);
		switch(akt_button)
		{ 
		  case 101:	((CSaveTstDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 104:	((CSaveRefDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 106:	((CSaveOtchDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 111:	((CTreeAddFromPanelDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 121:	((CTestRenameDlg*)m_pParent)->PushKlava(strChar1);break;
		  case 122:	 case 142:	((CValEnter*)m_pParent)->PushKlava(strChar1);break;
		}
		break;
	case IDC_BTN_123:
		if (!n123)
		{
		 n123=1;
		 nLanguage=0;
		 nCapsLock=0;
		 ChangeLanguage(alphabetNumber1,alphabetNumber2,alphabetNumber3); 
		 p123->SetWindowText(L"abc");
		}
		else
		{
		n123=0;
		ChangeLanguage(alphabetEng1_m,alphabetEng2_m,alphabetEng3_m);
		p123->SetWindowText(L"123");
		}
			break;
	default:
		pWnd->GetWindowText(strBtn);
		strChar1+=strBtn;
		switch(akt_button)
		{ 
		    case 101: count_symbol=((CSaveTstDlg*)m_pParent)->PushKlava(strChar1);break;
			case 104: count_symbol=((CSaveRefDlg*)m_pParent)->PushKlava(strChar1);break;
			case 106: count_symbol=((CSaveOtchDlg*)m_pParent)->PushKlava(strChar1);break;
		    case 111: count_symbol=((CTreeAddFromPanelDlg*)m_pParent)->PushKlava(strChar1);break;
		    case 121: count_symbol=((CTestRenameDlg*)m_pParent)->PushKlava(strChar1);break;
		    case 122: case 142:	 count_symbol=((CValEnter*)m_pParent)->PushKlava(strChar1);break;
		}
		strChar1=strChar1.Left(count_symbol);
		if (nShift)
		{
			nShift=0;
			nCapsLock=0;
			if (!nLanguage) ChangeLanguage(alphabetEng1_m,alphabetEng2_m,alphabetEng3_m);
			else ChangeLanguage(alphabetRus1_m,alphabetRus2_m,alphabetRus3_m);
		}

	}
}

void CKlava::OnBtnEsc() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnShift() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtn123() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnA1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnB1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnBackspace() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnC1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnCapslock() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnD1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnDvt() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnE1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnF() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnFsk1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnFsk2() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnG1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnH1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnI1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnJ1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnK1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnL1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnLanguage() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnM1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnMinys() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnN1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnO1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnP1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnPlus1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnProbel() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnQ1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnR1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnS1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnT1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnThk() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnTz() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnU1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnV1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnW1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnX1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnY1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnZ1() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}

void CKlava::OnBtnZpt() 
{
	// TODO: Add your control notification handler code here
	KlavaPress();
	
}
