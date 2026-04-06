// FileDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "FileDlg.h"
#include "ParolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog
static UINT mas_ICON[] = {IDI_ICON_EXE,IDI_ICON_MOD,IDI_LEVEL2,IDI_LEVEL2,IDI_ICON_REF,IDI_ICON_XLS,IDI_ICON_TXT};
/*CArrayString array_Filter = {_T("Files(\\Disk\\FTPRoot\\*.exe)"),
							 _T("Files(\\Disk\\Mod\\*.mod)"),
							 _T("Files(\\Disk\\Tst_standart\\*.tst)"),
							 _T("Files(\\Disk\\Tst_user\\*.tst)"),
							 _T("Files(\\Disk\\Ref\\*.ref)"),
							 _T("Files(\\Disk\\FTPRoot\\*.xls)"),
							 _T("Files(\\Disk\\FTPRoot\\*.txt)")};
*/
CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDlg)
	m_sName = _T("");
	m_bZaschParol = FALSE;
	m_sTitle = _T("");
	m_sFilter = _T("");
	m_isposob = 0;
	//}}AFX_DATA_INIT
	m_iWhatDo = FILE_LOAD;
	m_sPath = _T("");
	m_sCatalog = _T("");
	m_sFullNameFile =  _T("");
	m_itip = 0;
}


void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDlg)
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT1, m_sName);
	DDX_Check(pDX, IDC_CHECK_ZASCH_PAROL, m_bZaschParol);
	DDX_Text(pDX, IDC_STATIC_TITLE, m_sTitle);
	DDV_MaxChars(pDX, m_sTitle, 50);
	DDX_CBString(pDX, IDC_COMBO1, m_sFilter);
	DDV_MaxChars(pDX, m_sFilter, 40);
	DDX_Radio(pDX, IDC_RADIO1, m_isposob);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	//{{AFX_MSG_MAP(CFileDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE, OnDblclkListFile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers

BOOL CFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	UpdateData();
	
	switch(m_iWhatDo)
	{ case FILE_DELETE: //удалить
//				GetDlgItem(IDOK)->SetWindowText(_T("Удалить"));
				GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_SHOW);
				GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHECK_ZASCH_PAROL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
				break;
	  case FILE_LOAD: //открыть
				GetDlgItem(IDOK)->SetWindowText(_T("Открыть"));
				GetDlgItem(IDC_CHECK_ZASCH_PAROL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
				break;
	  case FILE_SAVE_AS: //сохранить как (с запросом о защите пароля)
				GetDlgItem(IDOK)->SetWindowText(_T("Сохранить"));
				if ((m_itip==FILE_REF)||(m_itip==FILE_TST)||(m_itip==FILE_TST_US))
					 GetDlgItem(IDC_CHECK_ZASCH_PAROL)->ShowWindow(SW_HIDE);
				else GetDlgItem(IDC_CHECK_ZASCH_PAROL)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
				break;
	  case FILE_SAVE: //сохранить (с запросом добавить или обновить)
				GetDlgItem(IDOK)->SetWindowText(_T("Сохранить"));
				GetDlgItem(IDC_CHECK_ZASCH_PAROL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO1)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
				break;
	  case FILE_COPY: //копировать
				break;
	}
	CComboBox* 	pmyComboBox = (CComboBox*)GetDlgItem(IDC_COMBO1);
    pmyComboBox->ResetContent();
	pmyComboBox->AddString(m_sFilter);
	pmyComboBox->SetCurSel(0);
	BuildListFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileDlg::BuildListFile() 
{
	HANDLE hFile;
	WIN32_FIND_DATA fd;
	CString str;
	int i = 0;
    HICON hIcon[NFILE];

	CListCtrl* pmyListCtrl= (CListCtrl*) GetDlgItem(IDC_LIST_FILE);

	// Delete all of the items from the list view control.
	pmyListCtrl->DeleteAllItems();
	ASSERT(pmyListCtrl->GetItemCount() == 0);

	  m_imyImageList.Create(16,16,0,0,8);

	  pmyListCtrl->SetImageList(&m_imyImageList, LVSIL_SMALL);

	hFile=::FindFirstFile(m_sCatalog,&fd);	//
	
	if (hFile != INVALID_HANDLE_VALUE)
	{ BOOL b = true;
	  while (b)
	  { //список имён файлов
		hIcon[i]=AfxGetApp()->LoadIcon(mas_ICON[m_itip]);//IDI_ICON_FILE);//IDI_LEVEL2);
		m_imyImageList.Add(hIcon[i]);
	  	str.Format(L"%s",fd.cFileName);
		pmyListCtrl->InsertItem(i,str,i);
		i++;
	    //поиск всех файлов
		b=FindNextFile(hFile,&fd);
	  }	//while (b)
	}

}

void CFileDlg::OnDblclkListFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int n = m_listFile.GetSelectionMark();
	m_sName = m_listFile.GetItemText(n,0);
	m_sFullNameFile = m_sPath + m_sName;
	UpdateData(false);
	*pResult = 0;
}

void CFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	BOOL fl=false;
	CString str;

	UpdateData();

  if ((m_iWhatDo==FILE_LOAD)||
	  (m_iWhatDo==FILE_DELETE))
  { //файл открыть 
	m_sFullNameFile = m_sPath + m_sName;
    CDialog::OnOK();
  }

  if ((m_iWhatDo==FILE_SAVE_AS) || 
	  (m_iWhatDo==FILE_SAVE))
  { //файл сохранить
	//проверка на совпадение
	int n = m_listFile.GetItemCount();
	for (int i=0;i<n;i++)
	{
	  str = m_listFile.GetItemText(i,0);
	  if (m_sName==str)  { fl=true; break;}
	}
	if (fl)
	{ str.Format(_T("Файл с именем %s уже существует. Перезаписать?"),m_sName);
	  if (MessageBox(str,NULL,MB_YESNO|MB_ICONQUESTION)==IDYES) 
	  {	m_sFullNameFile = m_sPath + m_sName;
	    CDialog::OnOK();
	  }
	  else CDialog::OnCancel();
	}
	else
	{ m_sFullNameFile = m_sPath + m_sName;
	  CDialog::OnOK();
	}
  }

}

void CFileDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CFileDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	CString str;
	LVFINDINFO info;
	int nIndex;
	BOOL fl = false;

	UpdateData();

	CListCtrl* pmyListCtrl= (CListCtrl*) GetDlgItem(IDC_LIST_FILE);

	str.Format(_T("Вы уверены, что хотите удалить файл %s?"),m_sName);
	if (MessageBox(str,NULL,MB_YESNO|MB_ICONQUESTION)==IDYES) 
	{//уверены, что хотим удалить
	  info.flags = LVFI_PARTIAL|LVFI_STRING;
	  info.psz = m_sName;
	  m_sFullNameFile = m_sPath + m_sName;
     // Delete all of the items that begin with the string lpszmyString.
/*	  while ((nIndex=pmyListCtrl->FindItem(&info)) != -1)
	  {	 pmyListCtrl->DeleteItem(nIndex);
		 CFile::Remove(m_sFullNameFile);
		 fl = true;
	  }
*/
	  nIndex=pmyListCtrl->FindItem(&info);
	  if (nIndex!=-1)
	  {  pmyListCtrl->DeleteItem(nIndex);
		 CFile::Remove(m_sFullNameFile);
		 fl = true;
	  }
	

  if (!fl)
	  {  str.Format(_T("Указанный файл с именем %s не найден"),m_sName);
	     MessageBox(str,NULL,MB_OK|MB_ICONEXCLAMATION);
	  }
	  else
	  { if(m_sCatalog == strFileNoName)	m_sName = _T("*.mod");
								else 	m_sName = _T("*.txt");
		UpdateData(false);
	  }
	}
}

HBRUSH CFileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if	(pWnd -> GetDlgCtrlID() == IDC_STATIC_TITLE)
	{	hbr = hbr;
		pDC->SetTextColor(RGB(119,0,0));
	}
	
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
