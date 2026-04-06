// TreeAddDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "TreeAddDlg.h"
#include "LineDlg.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeAddDlg dialog


CTreeAddDlg::CTreeAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreeAddDlg)
	m_EL = _T("");
	m_MOD = _T("");
	m_nelement = -1;
	//}}AFX_DATA_INIT
    index=-1;
	m_pDlg = (CLineDlg*)pParent;
}


void CTreeAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeAddDlg)
	DDX_Control(pDX, IDC_BUTADD, m_btn_add);
	DDX_CBString(pDX, IDC_COMBOEL, m_EL);
	DDX_CBString(pDX, IDC_COMBOMOD, m_MOD);
	DDX_CBIndex(pDX, IDC_COMBOCATEGORIA, m_nelement);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeAddDlg, CDialog)
	//{{AFX_MSG_MAP(CTreeAddDlg)
	ON_BN_CLICKED(IDC_BUTADD, OnButadd)
	ON_CBN_SELCHANGE(IDC_COMBOCATEGORIA, OnSelchangeCombocategoria)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeAddDlg message handlers

void CTreeAddDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}


void CTreeAddDlg::OnButadd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
 if (m_EL.IsEmpty() || m_MOD.IsEmpty())
     MessageBox(_T("Введите наименования элемента и теста!"),NULL,MB_OK|MB_ICONERROR);
 else
 {//0
	nameTst.Empty();
	if (m_pDlg->FindTest(m_EL,m_MOD))
	{ MessageBox(_T("В списке уже существует тест ")+m_MOD+_T(" для элемента ")+m_EL,NULL,MB_OK|MB_ICONEXCLAMATION);
	  UpdateData(FALSE);
	}
	else
	{//1
 	  CComboBox *pcb = (CComboBox*) GetDlgItem(IDC_COMBOMOD);
	  index=pcb->FindStringExact(0,m_MOD);
/*	  if (index==-1)	//не надо!!!!
	  { nameTst.Empty();
 	    m_pDlg->Add();
	  }
*/	  if (index>0)
	  { //выбран стандартный тест
		//добавить тест со стандартными данными из
		//каталога Tst_standart
		CString rStrTest;
		pcb->GetLBText(index,rStrTest);
		nameTst=_T("\\Hard Disk\\Tst_standart\\")+rStrTest+_T(".tst");//_T("\\Line\\Tst\\Tst_standart\\")+
 	    m_pDlg->Add();
	  }
	  if (index==0)
	  { //выбран пользовательский тест
		//добавить тест с пользовательскими данными из
		//каталога Tst_user
		nameTst=_T("\\Hard Disk\\Tst_user\\");//_T("\\Line\\Tst\\");

		CFileDlg dlg;
		dlg.m_sTitle = _T("Укажите файл для экстракции параметров");
		dlg.m_iWhatDo = FILE_LOAD;
		dlg.m_sFilter =_T("Files (\\Hard Disk\\Tst_user\\*.tst)");
		dlg.m_sCatalog =_T("\\Hard Disk\\Tst_user\\*.tst");
		dlg.m_sPath = _T("\\Hard Disk\\Tst_user\\");
		dlg.m_sName = _T("*.tst");
		dlg.m_itip = FILE_TST_US;
		dlg.m_sFullNameFile = _T("");
		UpdateData(false);
		if (dlg.DoModal()==IDOK)
		{ UpdateData();
		  nameTst = dlg.m_sFullNameFile;
/*
		 CFileDialog dl(TRUE);
	   CString strd(_T("\\Disk\\Tst_user\\ (*.tst)")); strd+=(TCHAR)NULL;
	   strd+=_T("*.tst"); strd+=(TCHAR)NULL;
	   dl.m_ofn.lpstrFilter=strd;
	   dl.m_ofn.nFilterIndex=1;
	   TCHAR strName[_MAX_PATH];
	   strName[0]='*';
	   strName[1]='.';
	   strName[2]='t';
	   strName[3]='s';
	   strName[4]='t';
	   strName[5]=(TCHAR)NULL;
	   dl.m_ofn.lpstrFile=strName;
	   dl.m_ofn.lpstrTitle=_T("Укажите файл для экстракции параметров");

	   if (dl.DoModal()==IDOK)
	   { nameTst = dl.GetPathName();
*/		 //получение имени польз.теста из полного пути
	     CString str;
//		 str=dl.GetFileTitle();
		 str = dlg.m_sName;
		 int l=str.GetLength();
		 str=str.Mid(0,l-4);
		 if (m_pDlg->FindTest(m_EL,str))
		 {	MessageBox(_T("В файле модели уже существует тест ")+str+_T(" для элемента ")+m_EL,NULL,MB_OK|MB_ICONEXCLAMATION);
	        UpdateData(FALSE);
		 }
		 else
		 { m_MOD=str;
	       UpdateData(FALSE);
 	       m_pDlg->Add();
		 }
	   }//end of IDOK
	  }//end of index==0

	}//end of 1
 }//end of 0)
}

BOOL CTreeAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//список наименований элементов
     CComboBox *pcb = (CComboBox*) GetDlgItem(IDC_COMBOEL);
     m_pDlg->AddMasEl();
     for (int i=0;i<NE;i++)
	 { if (m_masStrEl[i]==_T("")) break;
       pcb->AddString((LPCTSTR)m_masStrEl[i]);
	 }

	//список наименований тестов в зависимости от вида элемента (m_nelement)
    CComboBox *pct = (CComboBox*) GetDlgItem(IDC_COMBOMOD);
	pct->ResetContent();
	int ind = -1;
	m_nelement = m_pDlg->m_nelement;
	SetListTestTst();
/*
  switch(m_pDlg->m_nelement)
  { case -1: 
	         pct -> SetCurSel(ind);
			 break;
	case 0:	 //DIOD
	case 1:	 //RES		//двухполюсники
			 pct->AddString(_T("_USER"));
	         pct->AddString(_T("Cap"));
	         pct->AddString(_T("Forvard"));
	         pct->AddString(_T("Res"));
	         pct->AddString(_T("Reverse"));
             ind = pct->FindStringExact(0,m_MOD);
	         pct -> SetCurSel(ind);
			 break;
	case 2:	 //NPN
	case 3:	 //PNP		//трёхполюсники
	         pct->AddString(_T("_USER"));
	         pct->AddString(_T("IcVc"));
//	         pct->AddString(_T("Beta"));		//NPN (PNP) (трёхполюсный)
//	         pct->AddString(_T("Betai"));
	         pct->AddString(_T("Vce_for"));
	         pct->AddString(_T("Vce_rev"));
//	         pct->AddString(_T("Ibe"));
	         pct->AddString(_T("Vcb_for"));
	         pct->AddString(_T("Vcb_rev"));
	         pct->AddString(_T("Vbe_for"));
	         pct->AddString(_T("Vbe_rev"));
             ind = pct->FindStringExact(0,m_MOD);
	         pct -> SetCurSel(ind);
			 break;
	case 4:	 //NMOS
	case 5:	 //PMOS		//трёхполюсники
			 pct->AddString(_T("_USER"));
//	         pct->AddString(_T("IdVg"));		//NMOS (PMOS) (трёхполюсный)
	         pct->AddString(_T("IdVd"));
	         pct->AddString(_T("Vd_break"));
	         pct->AddString(_T("Vg_break"));
             ind = pct->FindStringExact(0,m_MOD);
	         pct -> SetCurSel(ind);
			 break;
	case 6:	 //NPN4
	case 7:	 //PNP4		//четырёхполюсники
			 pct->AddString(_T("_USER"));
//			 pct->AddString(_T("Beta_s"));		//NPN (PNP) (четырёхполюсный)
//	         pct->AddString(_T("Betai_s"));
//	         pct->AddString(_T("Vce_s_for"));
//	         pct->AddString(_T("Vce_s_rev"));
//	         pct->AddString(_T("IcVc_s"));
//	         pct->AddString(_T("Ibe_s"));
//	         pct->AddString(_T("Vcb_s_for"));
//	         pct->AddString(_T("Vcb_s_rev"));
//	         pct->AddString(_T("Vbe_s_for"));
//	         pct->AddString(_T("Vbe_s_rev"));
	         pct->AddString(_T("Vcs_for"));
	         pct->AddString(_T("Vcs_rev"));
	         pct->AddString(_T("Vbs_for"));
	         pct->AddString(_T("Vbs_rev"));
   	         pct->AddString(_T("IbVg_b"));
			 ind = pct->FindStringExact(0,m_MOD);
	         pct -> SetCurSel(ind);
			 break;
	case 8:	 //NMOS4
	case 9:	 //PMOS4	//четырёхполюсники
			 pct->AddString(_T("_USER"));
	         pct->AddString(_T("IdVg_b"));		//NMOS (PMOS) (четырёхполюсный)
	         pct->AddString(_T("IbVg_b"));
             ind = pct->FindStringExact(0,m_MOD);
	         pct -> SetCurSel(ind);
			 break;
  }
 */
	UpdateData(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreeAddDlg::OnSelchangeCombocategoria() 
{
	// TODO: Add your control notification handler code here
	SetListTestTst();
}

void CTreeAddDlg::SetListTestTst() 
{//построение списка стандартных тестов
 //************************************
	UpdateData();
	
	if (m_nelement==RES) { m_nelement = DIOD; UpdateData(false);}
	
	BuildArrayTstStandart();
    CComboBox *pct = (CComboBox*) GetDlgItem(IDC_COMBOMOD);
	pct->ResetContent();
	int ind = -1;
    int n_tst = array_tst_standart.GetSize();
  if(m_nelement==-1) pct -> SetCurSel(ind);
  else
  {
	pct->AddString(_T("_USER"));
	for(int i=0;i<n_tst;i++)
	{ CString str = array_tst_standart.GetAt(i);		  
	  pct->AddString(str);
	}
    ind = pct->FindStringExact(0,m_MOD);
	if (ind>=0) pct -> SetCurSel(ind);
  }
  UpdateData(FALSE);
}

void CTreeAddDlg::BuildArrayTstStandart() 
{
	HANDLE hFileTst;
	WIN32_FIND_DATA fd;
	CString str_pass_tst,str_name_tst,str,str_prim;
	CFile file;
	int iStrLen;
	CFileException er;
	int p1,p2;
    char* cc;
	int nelement,razv,dlitimp,iKolSourceU;
	
	array_tst_standart.RemoveAll();
    int n_tst = array_tst_standart.GetSize();
	
	hFileTst=::FindFirstFile(strFileTstStandart,&fd);	//
	
	if (hFileTst != INVALID_HANDLE_VALUE)
	{ BOOL g = true;
	  while (g)
	  { //массив имён файлов ссылок
		str_name_tst.Format(L"%s",fd.cFileName);
	    str_pass_tst = strPassTstStandart+str_name_tst;
     	BOOL b = file.Open((LPCTSTR)str_pass_tst,CFile::modeRead);
		  if (!b)
		  { TCHAR szError[1024];
			er.GetErrorMessage(szError, 1024);
			str.Format(_T("ERROR FILE TST %s"),szError);
			  TRACE(str);
		  }
          else
		  {//1 
		   //чтение файла
	       file.SeekToBegin();	
	       DWORD  l = file.GetLength();
	       DWORD* lp = new DWORD[l];
	       file.Read(lp,l);
		   file.Close();
		   CString strFile((LPCTSTR)&lp[0],l);
	       delete[] lp;
		   if (!strFile.IsEmpty())
		   {//2
			//категория
		    p1=strFile.Find(L'=',0);
		    p2=strFile.Find(L'\n',p1);
		    str=strFile.Mid(p1+1,p2-p1-2);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    nelement = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			//--------------------------------------------------------------------------------
			  //комментарии
		      p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L'\n',p1);
		      str_prim=strFile.Mid(p1+1,p2-p1-2);
		      iStrLen = str.GetLength();
			  str.ReleaseBuffer();
			//пропускаем 7 параметров
			for(int i=0;i<7;i++)	
			{ p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L'\n',p1);
		      str=strFile.Mid(p1+1,p2-p1-2);
		      iStrLen = str.GetLength();
		      cc = new char[iStrLen];
		      WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		      delete[] cc;
		      str.ReleaseBuffer();
			}
			//развертка
		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L'\n',p1);
		    str=strFile.Mid(p1+1,p2-p1-2);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    razv = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			//длительность импульса
		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L'\n',p1);
		    str=strFile.Mid(p1+1,p2-p1-2);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    dlitimp = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			//------------------------------------------------------------------------------
			//пропускаем 2 параметрa
			for( i=0;i<2;i++)	
			{ p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L'\n',p1);
		      str=strFile.Mid(p1+1,p2-p1-2);
		      iStrLen = str.GetLength();
		      cc = new char[iStrLen];
		      WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		      delete[] cc;
		      str.ReleaseBuffer();
			}
			//источник Uc
			p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   iKolSourceU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
			//------------------------------------------------------------------------------
//			UpdateData();
			if (nelement == m_nelement)
			{  //проверка на соответствие категории и положению переключателя 3000V/50V
			  if ((n_Switch==0 && dlitimp<=4)||							//+-3000V
				  (n_IPPP==1 && n_Switch==1 && razv==IMP_POS && dlitimp > 4) ||
//				  (n_IPPP==1 && n_Switch==1 && razv==SYN_POS && dlitimp ==0 && (iKolSourceU+bias_KolSourceU)==0) ||
//				  (n_IPPP==1 && n_Switch==2 && razv==SYN_NEG && dlitimp ==0 && (iKolSourceU+bias_KolSourceU)==0)||	//+50V
				  (n_IPPP==1 && n_Switch==2 && razv==IMP_NEG && dlitimp > 4))	//-50V
			  { //тест
			//файл
			  l = str_name_tst.GetLength();
			  str = str_name_tst.Mid(0,l-4);
			  array_tst_standart.SetAtGrow(n_tst,str);		  
			  //комментарии
/*		      p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L'\n',p1);
		      str=strFile.Mid(p1+1,p2-p1-2);
		      iStrLen = str.GetLength();
			  str.ReleaseBuffer();
*/			  array_tst_prim.SetAtGrow(n_tst,str_prim);		  
			  n_tst++;
			  }
			}
		   }//2
		  }//1
		  //поиск всех файлов *.tst
	      g=FindNextFile(hFileTst,&fd);
	  }	//while (g)
	}

}
