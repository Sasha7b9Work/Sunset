// DelTstDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "DelTstDlg.h"
#include "LineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelTstDlg dialog
static CString strDataTst[NTEST][3];
typedef struct {
	LPTSTR psz1;
	LPTSTR psz2;
	LPTSTR psz3;
} ITEMDATATST, *PITEMDATATST; 

ITEMDATATST* m_pDataTst[NTEST];//[100]; 


CDelTstDlg::CDelTstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDelTstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDelTstDlg)
	m_nelement = -1;
	//}}AFX_DATA_INIT
	what_directory = TST_USER;
	m_sname_tst = _T("");
	bFileOpen = false;
}


void CDelTstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelTstDlg)
	DDX_Control(pDX, IDC_BUTTON_DEL_ALL, m_butDelAll);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_butDel);
	DDX_Control(pDX, IDOK, m_butOK);
	DDX_Control(pDX, IDC_LIST_FILE_TST, m_listTst);
	DDX_CBIndex(pDX, IDC_COMBO_EL, m_nelement);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDelTstDlg, CDialog)
	//{{AFX_MSG_MAP(CDelTstDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_EL, OnSelchangeComboEl)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILE_TST, OnItemchangedListFileTst)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL, OnButtonDelAll)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelTstDlg message handlers

BOOL CDelTstDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(what_directory)
	{ case TST_STANDART: this->SetWindowText(L"  Библиотека стандартных тестов");
						 break;
	  case TST_USER:	 this->SetWindowText(L"  Библиотека пользовательских тестов");
						 break;
	}
	m_brushGray.CreateSolidBrush(RGB(228,228,228));//111,111,111));//134,134,134));//128,128,128));
	CRect rect;
	GetClientRect(&rect);
	CRect RectListTst;
	GetDlgItem(IDC_LIST_FILE_TST)->GetWindowRect(&RectListTst);
	RectListTst.left=rect.left+1;
	RectListTst.top=rect.top;
	RectListTst.right=rect.right-155;
	RectListTst.bottom=rect.bottom;
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_LIST_FILE_TST);
	pSt->MoveWindow(&RectListTst);
	CRect RectEl;
	GetDlgItem(IDC_STATIC_EL)->GetWindowRect(&RectEl);
	RectEl.left=RectListTst.right+2;
	RectEl.top=rect.top+2;
	RectEl.right=rect.right-2;
	RectEl.bottom=RectEl.top+70;
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_EL);
	pSt->MoveWindow(&RectEl);
	CRect RectTest;
	GetDlgItem(IDC_STATIC_TEST)->GetWindowRect(&RectTest);
	RectTest.left=RectEl.left;
	RectTest.top=RectEl.bottom+2;
	RectTest.right=RectEl.right;
	RectTest.bottom=rect.bottom-2;
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_TEST);
	pSt->MoveWindow(&RectTest);
	m_listTst.SetExtendedStyle(m_listTst.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listTst.InsertColumn(0,_T("№"),LVCFMT_LEFT,(RectListTst.Width()-2)/10);
	m_listTst.InsertColumn(1,_T("Наименование файла"),LVCFMT_LEFT,(RectListTst.Width()-2)/3);
	m_listTst.InsertColumn(2,_T("Комментарий"),LVCFMT_LEFT,(RectListTst.Width()+70)/2);

	InitTablTst();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDelTstDlg::InitTablTst() 
{
	int n = 0;
	int i = 0;
	CString str;

    CListCtrl *plb = (CListCtrl*) GetDlgItem(IDC_LIST_FILE_TST);
	plb->DeleteAllItems();// Delete all of the items from the list view control.
	
	switch(what_directory)
	{ case TST_STANDART: BuildArrayTstStandart();
						 n = array_tst_standart.GetSize();
						 for (i=0;i<n;i++)
						 { str.Format(_T("%d"),i+1);
						   strDataTst[i][0] = str;
						   str = array_tst_standart.GetAt(i);
						   strDataTst[i][1] = str;
   						   str = array_tst_prim.GetAt(i);
						   strDataTst[i][2] = str;
						 }
						 if (n)
						 { m_listTst.SetSelectionMark(0);
	                       m_sname_tst = array_tst_standart.GetAt(0);	//имя пользовательского файла
						 }
						 else m_sname_tst=_T("");
						 break;
	  case TST_USER:	 BuildArrayTstUser();
						 n = array_tst_user.GetSize();
						 for (i=0;i<n;i++)
						 { str.Format(_T("%d"),i+1);
						   strDataTst[i][0] = str;
						   str = array_tst_user.GetAt(i);
						   strDataTst[i][1] = str;
   						   str = array_tst_prim.GetAt(i);
						   strDataTst[i][2] = str;
						 }
					     
						 if (n)
						 { m_listTst.SetSelectionMark(0);
	                       m_sname_tst = array_tst_user.GetAt(0);	//имя пользовательского файла
						 }
						 else m_sname_tst=_T("");
						 break;
	}
	for(i=0;i<n;i++)
	{	plb->InsertItem(i, strDataTst[i][0]);
		plb->SetItemText(i, 1, strDataTst[i][1]);
		plb->SetItemText(i, 2, strDataTst[i][2]);
		plb->SetItemData(i, (LPARAM)m_pDataTst[i]);
	}
//	CWnd* pwnd;
	if (n==0) 
	{   GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DEL_ALL)->ShowWindow(SW_HIDE);
	}
	else
	{   GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_DEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_DEL_ALL)->ShowWindow(what_directory? SW_SHOW:SW_HIDE);
	}
	if (bFileOpen)
	{ GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	}
}

void CDelTstDlg::BuildArrayTstUser() 
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

	array_tst_user.RemoveAll();
    int n_tst = array_tst_user.GetSize();
	
	hFileTst=::FindFirstFile(strFileTstUser,&fd);	//
	
	if (hFileTst != INVALID_HANDLE_VALUE)
	{ BOOL g = true;
	  while (g)
	  { //массив имён файлов ссылок
		str_name_tst.Format(L"%s",fd.cFileName);
	    str_pass_tst = strPassTstUser+str_name_tst;
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
		    int nelement = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			  //комментарии
		      p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L'\n',p1);
		      str_prim=strFile.Mid(p1+1,p2-p1-2);
		      iStrLen = str.GetLength();
			  str.ReleaseBuffer();
			//--------------------------------------------------------------------------------
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
			for(i=0;i<2;i++)	
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
//			UpdateData();
			if (nelement == m_nelement)
			{ //проверка на соответствие категории и положению переключателя 3000V/50V
			  if ((n_Switch==0 && dlitimp<=4)||							//+-3000V
				  (n_IPPP==1 && n_Switch==1 && razv==IMP_POS && dlitimp > 4) ||
				  (n_IPPP==1 && n_Switch==1 && razv==SYN_POS && dlitimp == 6) ||
				  (n_IPPP==1 && n_Switch==2 && razv==SYN_NEG && dlitimp == 6)||	//-5V
				  (n_IPPP==1 && n_Switch==2 && razv==IMP_NEG && dlitimp > 4))	//+5V
			  {//файл
			  array_tst_user.SetAtGrow(n_tst,str_name_tst);		  
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

void CDelTstDlg::BuildArrayTstStandart() 
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
		    int nelement = atoi(cc);
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
			for(i=0;i<2;i++)	
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
//			UpdateData();
			if (nelement == m_nelement)
			{  //проверка на соответствие категории и положению переключателя 3000V/50V
			  if ((n_Switch==0 && dlitimp<=4)||							//+-3000V
				  (n_IPPP==1 && n_Switch==1 && razv==IMP_POS && dlitimp > 4) ||
				  (n_IPPP==1 && n_Switch==1 && razv==SYN_POS && dlitimp == 6) ||
				  (n_IPPP==1 && n_Switch==2 && razv==SYN_NEG && dlitimp == 6)||	//-5V
				  (n_IPPP==1 && n_Switch==2 && razv==IMP_NEG && dlitimp > 4))	//+5V
			  {//файл
			  array_tst_standart.SetAtGrow(n_tst,str_name_tst);		  
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


void CDelTstDlg::OnSelchangeComboEl() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_nelement==RES) { m_nelement = DIOD; UpdateData(false);}

	InitTablTst();
}

void CDelTstDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_brushGray.DeleteObject();
	
}

void CDelTstDlg::OnItemchangedListFileTst(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int i_tst = m_listTst.GetItemCount();
	if(i_tst)
	{
		int m_tst = m_listTst.GetSelectionMark();
		if (m_tst==-1) MessageBox(_T("Выберите наименование теста!"),NULL,MB_OK|MB_ICONERROR);
		else 
		switch(what_directory)
		{ case TST_STANDART: m_sname_tst = array_tst_standart.GetAt(m_tst);	//имя стандартного файла
							 break;
		  case TST_USER:     m_sname_tst = array_tst_user.GetAt(m_tst);	//имя пользовательского файла
							 break;
		}
	}
	*pResult = 0;
}


void CDelTstDlg::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CString str,strn,strFullPass;
	CFile file;
	int i_tst = m_listTst.GetItemCount();
	if(i_tst)
	{
		int m_tst = m_listTst.GetSelectionMark();
		if (m_tst==-1) MessageBox(_T("Выберите наименование теста!"),NULL,MB_OK|MB_ICONERROR);
		else
		{  switch(what_directory)
			{ case TST_STANDART: m_sname_tst = array_tst_standart.GetAt(m_tst);	//имя пользовательского файла
								 strFullPass = strPassTstStandart + m_sname_tst;
								 break;
			  case TST_USER:     m_sname_tst = array_tst_user.GetAt(m_tst);	//имя пользовательского файла
								 strFullPass = strPassTstUser + m_sname_tst;
								 break;
			}
		    strn = m_sname_tst;
			int l = strn.GetLength();
		    strn = strn.Mid(0,l-4);
		    str.Format(_T("Вы уверены, что хотите удалить тест %s из библиотеки?"),strn);
		    if (MessageBox(str,NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
			{//удалить файл
		      file.Remove((LPCTSTR)strFullPass);
	          InitTablTst();
			}

		}
	}
}

void CDelTstDlg::OnButtonDelAll() 
{
	// TODO: Add your control notification handler code here
	CString str_Name_Tst,strFullPass;
	CFile file;

	int i_tst = m_listTst.GetItemCount();
	if(i_tst)
	{ if (MessageBox(_T("Вы уверены, что хотите удалить все файлы пользовательских тестов?"),NULL,MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		{//удалить все файлы *.ref
          int n_tst = array_tst_user.GetSize();
		  if (n_tst)
		  { for (int i=0;i<n_tst;i++)
			{
			  str_Name_Tst = array_tst_user.GetAt(i);	//имя файла
			  if (str_Name_Tst!=_T(""))
			  { strFullPass = strPassTstUser + str_Name_Tst;
		        file.Remove((LPCTSTR)strFullPass);//удалить файл
			  }
			}//end for
	        InitTablTst();
		  }//end if (n_ref)
		}//end if IDYES
	}// end if (i_ref)
	
}

HBRUSH CDelTstDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ((pWnd -> GetDlgCtrlID() == IDC_STATIC_EL)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_TEST))
	{	hbr = m_brushGray;//Black;
		pDC->SetBkColor(RGB(111,111,111));//0,0,0));//128,128,128));//
		pDC->SetTextColor(RGB(0,0,0));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
