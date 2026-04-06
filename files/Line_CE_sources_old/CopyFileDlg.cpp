// CopyFileDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "CopyFileDlg.h"
#include "InfoCopyFileDlg.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyFileDlg dialog
static UINT mas_ICON_FILE[] = {IDI_ICON_EXE,
							IDI_ICON_MOD,
							IDI_LEVEL2,
							IDI_LEVEL2,
							IDI_ICON_REF,
							IDI_ICON_XLS,
							IDI_ICON_TXT};
const LPCTSTR mas_Catalog[] = {_T("Psw\\"),
							 _T("Mod\\"),
							 _T("Tst_standart\\"),
							 _T("Tst_user\\"),
							 _T("Ref\\"),
							 _T("Xls\\"),
							 _T("Txt\\")};
const LPCTSTR mas_Ext[]    = {_T("*.psw"),
							 _T("*.mod"),
							 _T("*.tst"),
							 _T("*.tst"),
							 _T("*.ref"),
							 _T("*.xls"),
							 _T("*.txt")};


CCopyFileDlg::CCopyFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyFileDlg)
	m_itip = 1;
	m_sName = _T("");
	m_sDiskCatalog = _T("");
	m_sUsbCatalog = _T("");
	//}}AFX_DATA_INIT
	m_sWhereFrom = _T("");
	m_sWhereTo = _T("");
	m_sDisk = _T("\\Hard Disk\\");
	m_sUsb = _T("\\USBDisk\\");
	m_sServer = _T("\\Hard Disk\\FTPRoot\\");
//	m_sServer = _T("\\Hard Disk\\Cosmos\\");
	m_sCatalog = _T("");
	m_sExt = _T("");
	m_iFromTo = 0;		//0 - Disk=>Usb  1 - Usb=>Disk
	m_iswitch = 1;		//переключатель удаленного носителя: 0-USB, 1 - FTPRoot
}


void CCopyFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyFileDlg)
	DDX_Control(pDX, IDC_LIST_USB, m_listUsb);
	DDX_Control(pDX, IDC_LIST_DISK, m_listDisk);
	DDX_Control(pDX, IDC_BUTTON_MOVE, m_btnMove);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_CBIndex(pDX, IDC_COMBO_FILTER, m_itip);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_STATIC_TEXT_DISK, m_sDiskCatalog);
	DDX_Text(pDX, IDC_STATIC_TEXT_USB, m_sUsbCatalog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCopyFileDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyFileDlg)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER, OnSelchangeComboFilter)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DISK, OnDblclkListDisk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USB, OnDblclkListUsb)
	ON_BN_CLICKED(IDOK, OnCopyFile)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, OnButtonMove)
	ON_BN_CLICKED(IDC_STATIC_HELP, OnStaticHelp)
	ON_BN_CLICKED(IDC_STATIC_ICON_SWITCH, OnStaticIconSwitch)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DISK, OnClickListDisk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USB, OnClickListUsb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyFileDlg message handlers

HBRUSH CCopyFileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	((pWnd -> GetDlgCtrlID() == IDC_STATIC_TITLE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_TEXT_NET))
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	if	(pWnd -> GetDlgCtrlID() == IDC_STATIC_TEXT_DISK)
	{  hbr = m_brushBlack;
	   pDC->SetBkColor(RGB(0,0,0));
	   pDC->SetTextColor(RGB(0,255,0));
	}
	if	(pWnd -> GetDlgCtrlID() == IDC_STATIC_TEXT_USB)
	{  hbr = m_brushBlack;
	   pDC->SetBkColor(RGB(0,0,0));
		  pDC->SetTextColor(RGB(0,255,0));
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CCopyFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brushBlack.CreateSolidBrush(RGB(0,0,0));
	m_imyImageList.Create(16,16,0,0,NFILE);
//	m_imyImageList.SetImageCount(NFILE);
    m_imyImageListUsb.Create(16,16,0,0,NFILE);
//	m_imyImageListUsb.SetImageCount(NFILE);

	CComboBox* 	pmyComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);
    pmyComboBox->ResetContent();
	pmyComboBox->AddString(_T("Files(\\Hard Disk\\Psw\\*.psw)"));
	pmyComboBox->AddString(_T("Files(\\Hard Disk\\Mod\\*.mod)"));
	pmyComboBox->AddString(_T("Files(\\Hard Disk\\Tst_standart\\*.tst)"));
	pmyComboBox->AddString(_T("Files(\\Hard Disk\\Tst_user\\*.tst)"));
	pmyComboBox->AddString(_T("Files(\\Hard Disk\\Ref\\*.ref)"));
	pmyComboBox->AddString(_T("Files(\\Hard Disk\\Xls\\*.xls)"));
	pmyComboBox->AddString(_T("Files(\\Hard Disk\\Txt\\*.txt)"));
	pmyComboBox->SetCurSel(m_itip);
	m_itip = pmyComboBox->GetCurSel();
	m_sName = mas_Ext[m_itip];
	m_sCatalog = mas_Catalog[m_itip];
	m_sExt = mas_Ext[m_itip];
	m_sDiskCatalog = m_sDisk + m_sCatalog + m_sExt;
	switch(m_iswitch)
	{ case 0:	//USB
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_USB)->ShowWindow(SW_SHOW);
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_NET)->ShowWindow(SW_HIDE);
				((CStatic*)GetDlgItem(IDC_STATIC_TEXT_NET))->SetWindowText(_T("USB-flash накопитель"));
				m_sUsbCatalog = m_sUsb + m_sExt;
				break;
	  case 1:	//FTPRoot
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_USB)->ShowWindow(SW_HIDE);
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_NET)->ShowWindow(SW_SHOW);
				((CStatic*)GetDlgItem(IDC_STATIC_TEXT_NET))->SetWindowText(_T("FTP-сервер"));
				m_sUsbCatalog = m_sServer + m_sCatalog  + m_sExt;
				break;
	}
	BuildListFileDisk();
	BuildListFileUsb();
	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCopyFileDlg::OnSelchangeComboFilter() 
{
	// TODO: Add your control notification handler code here
	CComboBox* 	pmyComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);
	m_itip = pmyComboBox->GetCurSel();
	m_sName = mas_Ext[m_itip];
	m_sCatalog = mas_Catalog[m_itip];
	m_sExt = mas_Ext[m_itip];
	m_sDiskCatalog = m_sDisk + m_sCatalog + m_sExt;
	switch(m_iswitch)
	{ case 0:	//USB
				m_sUsbCatalog = m_sUsb + m_sExt;
				break;
	  case 1:	//FTPRoot
				m_sUsbCatalog = m_sServer + m_sCatalog + m_sExt;
				break;
	}
	BuildListFileDisk();
	BuildListFileUsb();
	UpdateData(false);
	
}

void CCopyFileDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_brushBlack.DeleteObject();
	CDialog::PostNcDestroy();
}

void CCopyFileDlg::BuildListFileDisk() 
{
	HANDLE hFile;
	WIN32_FIND_DATA fd;
	CString str;
	int i = 0;
    HICON hIcon[NFILE];

	CListCtrl* pmyListCtrl= (CListCtrl*) GetDlgItem(IDC_LIST_DISK);

	// Delete all of the items from the list view control.
	pmyListCtrl->DeleteAllItems();
	ASSERT(pmyListCtrl->GetItemCount() == 0);

	m_imyImageList.DeleteImageList();

	m_imyImageList.Create(16,16,0,0,8);
	
	pmyListCtrl->SetImageList(&m_imyImageList, LVSIL_SMALL);
	  
    CString strCatalog = m_sDiskCatalog;//m_sDisk+m_sCatalog+m_sExt;

	hFile=::FindFirstFile(strCatalog,&fd);	//

	if (hFile != INVALID_HANDLE_VALUE)
	{ BOOL b = true;
	  while (b)
	  { //список имён файлов
		hIcon[i]=AfxGetApp()->LoadIcon(mas_ICON_FILE[m_itip]);//IDI_ICON_FILE);//IDI_LEVEL2);
		m_imyImageList.Add(hIcon[i]);
	  	str.Format(L"%s",fd.cFileName);
		pmyListCtrl->InsertItem(i,str,i);
		i++;
	    //поиск всех файлов
		b=FindNextFile(hFile,&fd);
	  }	//while (b)
	}

}

void CCopyFileDlg::BuildListFileUsb() 
{
	HANDLE hFile;
	WIN32_FIND_DATA fd;
	CString str;
	int i = 0;
	HICON hIcon[NFILE];

	CListCtrl* pmyListCtrl= (CListCtrl*) GetDlgItem(IDC_LIST_USB);

	// Delete all of the items from the list view control.
	pmyListCtrl->DeleteAllItems();
	ASSERT(pmyListCtrl->GetItemCount() == 0);

	m_imyImageListUsb.DeleteImageList();

	m_imyImageListUsb.Create(16,16,0,0,8);

	  pmyListCtrl->SetImageList(&m_imyImageListUsb, LVSIL_SMALL);

	  CString strCatalog = m_sUsbCatalog;// m_sUsb+m_sExt;

	hFile=::FindFirstFile(strCatalog,&fd);	//

	if (hFile != INVALID_HANDLE_VALUE)
	{ BOOL b = true;
	  while (b)
	  { //список имён файлов
		hIcon[i]=AfxGetApp()->LoadIcon(mas_ICON_FILE[m_itip]);//IDI_ICON_FILE);//IDI_LEVEL2);
		m_imyImageListUsb.Add(hIcon[i]);
	  	str.Format(L"%s",fd.cFileName);
		pmyListCtrl->InsertItem(i,str,i);
		i++;
	    //поиск всех файлов
		b=FindNextFile(hFile,&fd);
	  }	//while (b)
	}

}

void CCopyFileDlg::OnDblclkListDisk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	m_iFromTo = 0;
	int n = m_listDisk.GetSelectionMark();
	BuildPathDiskToUsb(n);
	*pResult = 0;
}

void CCopyFileDlg::BuildPathDiskToUsb(int n)
{
	m_sName = m_listDisk.GetItemText(n,0);
	m_sWhereFrom =  m_sDisk + m_sCatalog + m_sName;
	switch(m_iswitch)
	{ case 0:	//USB
				m_sWhereTo = m_sUsb + m_sName;
				break;
	  case 1:	//FTPRoot
				m_sWhereTo = m_sServer + m_sCatalog  + m_sName;
				break;
	}
	UpdateData(false);
}

void CCopyFileDlg::OnDblclkListUsb(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_iFromTo = 1;
	int n = m_listUsb.GetSelectionMark();
    BuildPathUsbToDisk(n);
	*pResult = 0;
}

void CCopyFileDlg::BuildPathUsbToDisk(int n)
{
	m_sName = m_listUsb.GetItemText(n,0);
	switch(m_iswitch)
	{ case 0:	//USB
				m_sWhereFrom =  m_sUsb + m_sName;
				break;
	  case 1:	//FTPRoot
				m_sWhereFrom = m_sServer + m_sCatalog  + m_sName;
				break;
	}
	m_sWhereTo = m_sDisk + m_sCatalog + m_sName;
	UpdateData(false);
}

void CCopyFileDlg::OnCopyFile() 
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	BOOL b;
	int nItem;
/*
	b = CopyFile(FILE_COPY);
	if (b)
	{
	  BuildListFileDisk();
	  BuildListFileUsb();
	}
*/
	switch(m_iFromTo)
	{ case 0: pos = m_listDisk.GetFirstSelectedItemPosition();
			  if (pos == NULL)
			  TRACE0("No items were selected!\n");
			  else
			  {
				while (pos)
				{
					nItem = m_listDisk.GetNextSelectedItem(pos);
				     TRACE1("Item %d was selected!\n", nItem);
				     // you could do your own processing on nItem here
					BuildPathDiskToUsb(nItem);
					b = CopyFile(FILE_COPY);
				}
			  }				
			  break;
	  case 1: pos = m_listUsb.GetFirstSelectedItemPosition();
			  if (pos == NULL)
			  TRACE0("No items were selected!\n");
			  else
			  {
				while (pos)
				{
					 nItem = m_listUsb.GetNextSelectedItem(pos);
				     TRACE1("Item %d was selected!\n", nItem);
				     // you could do your own processing on nItem here
					BuildPathUsbToDisk(nItem);
					b = CopyFile(FILE_COPY);
				}
			  }
				break;
	}
    BuildListFileDisk();
    BuildListFileUsb();
}

BOOL CCopyFileDlg::CopyFile(int what_do)
{
    CString	str;
	CFile file;
	CFileException er;
	int n = 0;
	if(m_iFromTo) n = m_listDisk.GetItemCount();
	        else  n = m_listUsb.GetItemCount();
	//проверка на совпадение
	BOOL fl = false;
	for (int i=0;i<n;i++)
	{
	  if (m_iFromTo) str = m_listDisk.GetItemText(i,0);
				else str = m_listUsb.GetItemText(i,0);
	  if (m_sName==str)  { fl=true; break;}
	}
	if (fl)
	{ str.Format(_T("Файл с именем %s уже существует. Перезаписать?"),m_sName);
	  if (MessageBox(str,NULL,MB_YESNO|MB_ICONQUESTION)==IDNO) return false;
	}  
	  //копирование
	  CInfoCopyFileDlg dlg;
	  dlg.m_sFrom = m_sWhereFrom;
	  dlg.m_sTo = m_sWhereTo;
	  dlg.m_iWhatDo = what_do;//FILE_COPY;
	  UpdateData(false);
	  if (dlg.DoModal()==IDOK)
	  {//0
		UpdateData();
	    m_sWhereTo = dlg.m_sTo;
    	  BOOL b = file.Open((LPCTSTR)m_sWhereFrom,CFile::modeRead);
		  if (!b)
		  { TCHAR szError[1024];
			er.GetErrorMessage(szError, 1024);
			str.Format(_T("ERROR FILE %s"),szError);
			  TRACE(str);
			  return false;
		  }
          else
		  {//1 
		   //чтение файла
	       file.SeekToBegin();	
	       DWORD  l = file.GetLength();
	       DWORD* lp = new DWORD[l];
	       file.Read(lp,l);
		   file.Close();
		   b = file.Open((LPCTSTR)m_sWhereTo,CFile::modeCreate|CFile::modeWrite);
			if (!b)
			{ TCHAR szError[1024];
			  er.GetErrorMessage(szError, 1024);
			 str.Format(_T("Ошибка %s"),szError);
			   TRACE(str);
			   delete[] lp;
			   return false;
			}
	        else
			{//2 
		 	 //сохранение данных
		      CCmdTarget::BeginWaitCursor();		
			  file.Write(lp,l);
			  file.Close();
			  delete[] lp;
//			  BuildListFileDisk();
//			  BuildListFileUsb();
			  UpdateData(false);
			  EndWaitCursor();
			  return true;
			}//2
		  }//1
	  }//0
 return false;
}	



void CCopyFileDlg::OnButtonMove() 
{
	// TODO: Add your control notification handler code here
/*	BOOL b = CopyFile(FILE_MOVE);
	if (b) 
	{ CFile::Remove(m_sWhereFrom);
	  BuildListFileDisk();
	  BuildListFileUsb();
	}
*/
	POSITION pos;
	BOOL b;
	int nItem;
	switch(m_iFromTo)
	{ case 0: pos = m_listDisk.GetFirstSelectedItemPosition();
			  if (pos == NULL)
			  TRACE0("No items were selected!\n");
			  else
			  {
				while (pos)
				{
					nItem = m_listDisk.GetNextSelectedItem(pos);
				     TRACE1("Item %d was selected!\n", nItem);
				     // you could do your own processing on nItem here
					BuildPathDiskToUsb(nItem);
					b = CopyFile(FILE_MOVE);
					if (b) CFile::Remove(m_sWhereFrom);
				}
			  }				
			  break;
	  case 1: pos = m_listUsb.GetFirstSelectedItemPosition();
			  if (pos == NULL)
			  TRACE0("No items were selected!\n");
			  else
			  {
				while (pos)
				{
					 nItem = m_listUsb.GetNextSelectedItem(pos);
				     TRACE1("Item %d was selected!\n", nItem);
				     // you could do your own processing on nItem here
					BuildPathUsbToDisk(nItem);
					b = CopyFile(FILE_MOVE);
					if (b) CFile::Remove(m_sWhereFrom);
				}
			  }
				break;
	}
    BuildListFileDisk();
    BuildListFileUsb();
	
}

void CCopyFileDlg::OnStaticHelp() 
{
	// TODO: Add your control notification handler code here
	CHelpDlg dlg;
	switch(m_itip)
	{ case 0: dlg.m_sHelp = _T("Файлы настроек");break;
	  case 1: dlg.m_sHelp = _T("Файлы \\Hard Disk\\Mod\\*.mod - файлы моделей измерения");break;
	  case 2: dlg.m_sHelp = _T("Файлы \\Hard Disk\\Tst_standart\\*.tst - файлы стандартных тестов");break;
	  case 3: dlg.m_sHelp = _T("Файлы \\Hard Disk\\Tst_user\\*.tst - файлы пользовательских тестов");break;
	  case 4: dlg.m_sHelp = _T("Файлы \\Hard Disk\\Ref\\*.ref - файлы эталонных графиков");break;
	  case 5: dlg.m_sHelp = _T("Файлы \\Hard Disk\\Xls\\*.xls - файлы таблиц результатов измерения");break;
	  case 6: dlg.m_sHelp = _T("Файлы \\Hard Disk\\Txt\\*.txt - файлы отчетов о проведенных измерениях");break;
	}
//	dlg.m_itip = m_itip;
	UpdateData(false);
	dlg.DoModal();
}

void CCopyFileDlg::OnStaticIconSwitch() 
{
	// TODO: Add your control notification handler code here
	m_iswitch =! m_iswitch;
	switch(m_iswitch)
	{ case 0:	//USB
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_USB)->ShowWindow(SW_SHOW);
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_NET)->ShowWindow(SW_HIDE);
				((CStatic*)GetDlgItem(IDC_STATIC_TEXT_NET))->SetWindowText(_T("USB-flash накопитель"));
				m_sUsbCatalog = m_sUsb + m_sExt;
				break;
	  case 1:	//FTPRoot
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_USB)->ShowWindow(SW_HIDE);
				(CWnd*)GetDlgItem(IDC_STATIC_ICON_NET)->ShowWindow(SW_SHOW);
				((CStatic*)GetDlgItem(IDC_STATIC_TEXT_NET))->SetWindowText(_T("FTP-сервер"));
				m_sUsbCatalog = m_sServer + m_sCatalog  + m_sExt;
				break;
	}
	BuildListFileUsb();
	UpdateData(false);
}

void CCopyFileDlg::OnClickListDisk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_iFromTo = 0;
	
	*pResult = 0;
}

void CCopyFileDlg::OnClickListUsb(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_iFromTo = 1;
	
	*pResult = 0;
}
