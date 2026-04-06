// TablDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "TablDlg.h"
#include "SaveTablDlg.h"
#include "LineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTablDlg dialog
//static CString strData[NREZ][7];
static CString strData[NMAS][NREZ][7];
typedef struct {
	LPTSTR psz1;
	LPTSTR psz2;
	LPTSTR psz3;
	LPTSTR psz4;
	LPTSTR psz5;
	LPTSTR psz6;
	LPTSTR psz7;
} ITEMDATA, *PITEMDATA; 

//ITEMDATA* m_pData[NREZ];//[10]; 
ITEMDATA* m_pData[NREZ*NMAS];//[10]; 


CTablDlg::CTablDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTablDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTablDlg)
	//}}AFX_DATA_INIT
	strInfoTest = _T("");
	strElement = _T("");
	strPrim = _T("");
	strDateMeas = _T("");
	strTimeMeas = _T("");
	strOtcht      = _T("");
	strNameColumn = _T("");
	m_sKolMeasU=_T("");
	m_sKolMeasI=_T("");
	m_sBazaSource=_T("");
	m_sBazaMeas=_T("");
	m_sDopSource=_T("");
	m_sDopMeas=_T("");
	m_iCircuitBaza=-1;
	m_iCircuitDop=-1;
	m_isposob = 0;
	m_iBazaMeas = 1;
	m_iDopMeas = 1;
	m_bBaza = false;
	m_bDop  = false;
	m_bR    = false;
	m_bS	= false;
	m_bBeta = false;
	m_bBetai= false;
	rej_Y	= -1;
	m_pdlg = (CLineDlg*)pParent;
	m_itipimp=0;
}


void CTablDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTablDlg)
	DDX_Control(pDX, IDC_BUTSAVETABRES, m_btn_savetabres);
	DDX_Control(pDX, IDC_LISTRES, m_listRes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTablDlg, CDialog)
	//{{AFX_MSG_MAP(CTablDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTSAVETABRES, OnButsavetabres)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTablDlg message handlers

BOOL CTablDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UINT i,k;
	CString str;
    CRect rect,RectListRes;
	UINT j=0,s=0;

	int n_point = NPOINT;
	if (m_itipimp==TIP_IMP) n_point = 1;
	

    str.Format(_T("%s %s  ‘айл: %s  Ёлемент: %s  “ест: %s  %s"),
			strDateMeas,strTimeMeas,strFileName,strElement,strTestName,strPrim);
	int n = array_otcht.GetSize();
	if (n) strOtcht = array_otcht.GetAt(n-1);	//строка из отчЄта
	strInfoTest=str+strOtcht;//m_sInfoTest;//strTestName;
	this->SetWindowText(_T("“аблица результатов измерени€ ") + str);

	for (s=0;s<n_Mas;s++)
	for (i=0;i<NREZ;i++)
		for (k=0;k<7;k++)	//7 - максимальное число колонок
			strData[s][i][k]=_T("");

	pM[0]=&pM1[0][0];
	pM[1]=&pM2[0][0];
	pM[2]=&pM3[0][0];
	pM[3]=&pM4[0][0];
	pM[4]=&pM5[0][0];

	GetClientRect(&rect);
	
	GetDlgItem(IDC_LISTRES)->GetWindowRect(&RectListRes);
	RectListRes.left=rect.left;//+1;
	RectListRes.top=rect.top;//+72;
	RectListRes.right=rect.right;//-1;
	RectListRes.bottom=rect.bottom-72;//-1;
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_LISTRES);
	pSt->MoveWindow(&RectListRes);

	m_listRes.SetExtendedStyle(m_listRes.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	//заголовки колонок
	k= 0;
	str =_T("“очка");
	m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/6);
	strNameColumn=str+_T("\t");
	k++;
	str =m_sKolMeasU+_T(", V");
	m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	strNameColumn=strNameColumn+str+_T("\t");
	k++;
	str = m_sKolMeasI+_T(", A");
	m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	strNameColumn=strNameColumn+str+_T("\t");
	k++;
//	if (m_iCircuitBaza>0)
//	if (m_iCircuitBaza==1)
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	{ 
/*		if (m_bBaza)
		{ if (rej_Y) str = m_sBazaSource +_T(", A");
				else str = m_sBazaSource+_T(", V");
		}
		else
		{ if (rej_Y) str = m_sBazaMeas+_T(", A");
				else str = m_sBazaMeas+_T(", V");
		}
*/
		if (!m_bBaza)
		{ if (rej_Y) str = m_sBazaSource +_T(", A");
				else str = m_sBazaSource+_T(", V");
		}
		else
		{ if (rej_Y) str = m_sBazaMeas+_T(", A");
				else str = m_sBazaMeas+_T(", V");
		}
	  m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
/*	else
	{ //Ѕаза в обрыве
	  str=_T("Ѕаза в обрыве");
	  m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	}
*/	
//	if (m_iCircuitDop>0)
	if (m_iCircuitDop==1 && m_iKnotDop==1)
//	if (m_iCircuitDop==1)
	{ 
/*		if (m_bDop)
		{ if (rej_Y)  str = m_sDopSource+_T(", A");
				 else str = m_sDopSource+_T(", V");
		}
		else
		{ if (rej_Y)  str = m_sDopMeas+_T(", A");
				else  str = m_sDopMeas+_T(", V");
		}
*/		if (!m_bDop)
		{ if (rej_Y)  str = m_sDopSource+_T(", A");
				 else str = m_sDopSource+_T(", V");
		}
		else
		{ if (rej_Y)  str = m_sDopMeas+_T(", A");
				else  str = m_sDopMeas+_T(", V");
		}
	  m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
	if (m_bR)
	{ str = _T("R, Om");
	  m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
	if (m_bS)
	{ str = _T("S");
	  m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
	if (m_bBeta)
	{ str = _T("Beta");
	  m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
	if (m_bBetai)
	{ str = _T("Betai");
	  m_listRes.InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
	//заполнение данными строк таблицы
	int m=0;
	for (j=0;j<n_Step;j++)
	for (s=0;s<n_Mas;s++)
	{
	for (i=0;i<n_point;i++)
	{ 
	  //m-номер строки в таблице,j-номер очереди,i-номер точки в очереди,k-номер колонки
	  str.Format(_T("%d \\ %d \\ %d"),m+1,j+1,i+1);
	  strData[s][i+NPOINT*j][0]=str;
	  //данные Uc
	  str.Format(_T("%7.4e"),pM1[s][i+NPOINT*j]);
//	  m_pdlg->ValueToString(0,pM1[i+NPOINT*j],str);
	  strData[s][i+NPOINT*j][1]=str;
	  //данные Ic
	  str.Format(_T("%7.4e"),pM2[s][i+NPOINT*j]);
//	  m_pdlg->ValueToString(1,pM2[i+NPOINT*j],str);
	  strData[s][i+NPOINT*j][2]=str;
	  
//	  if (m_iCircuitBaza>0)
//	  if (m_iCircuitBaza==1)
	if ( m_iCircuitBaza==1 && m_iKnotBaza==1)
	  {//данные по базе
	     str.Format(_T("%7.4e"),pM3[s][i+NPOINT*j]);
//		 m_pdlg->ValueToString(m_iBazaMeas,pM3[i+NPOINT*j],str);
	     strData[s][i+NPOINT*j][3]=str;
	  }
	  
//	  if (m_iCircuitDop>0)
	if (m_iCircuitDop==1 && m_iKnotDop==1)
//	  if (m_iCircuitDop==1)
	  {//данные по доп.каналу
	     str.Format(_T("%7.4e"),pM4[s][i+NPOINT*j]);
//		 m_pdlg->ValueToString(m_iDopMeas,pM4[i+NPOINT*j],str);
	     strData[s][i+NPOINT*j][4]=str;
	  }

	  if (m_bR || m_bBeta || m_bBetai || m_bS)
	  {//рассчитанный массив
	     str.Format(_T("%7.4e"),pM5[s][i+NPOINT*j]);
//		 m_pdlg->ValueToString(rej_Y,pM5[i+NPOINT*j],str);
	     strData[s][i+NPOINT*j][5]=str;
	  }
	  m++;
	}
	}
/*	for (i=0; i<n_Step*NPOINT; i++)
	{
		m_pData[i] = new ITEMDATA;
		m_pData[i]->psz1 = (LPTSTR)(LPCTSTR)strData[i][0];
		m_pData[i]->psz2 = (LPTSTR)(LPCTSTR)strData[i][1];
		m_pData[i]->psz3 = (LPTSTR)(LPCTSTR)strData[i][2];
		m_pData[i]->psz4 = (LPTSTR)(LPCTSTR)strData[i][3];
		m_pData[i]->psz5 = (LPTSTR)(LPCTSTR)strData[i][4];
		m_pData[i]->psz6 = (LPTSTR)(LPCTSTR)strData[i][5];
		m_pData[i]->psz7 = (LPTSTR)(LPCTSTR)strData[i][6];

		m_listRes.InsertItem(i, strData[i][0]);
		m_listRes.SetItemText(i, 1, strData[i][1]);
		m_listRes.SetItemText(i, 2, strData[i][2]);
		m_listRes.SetItemText(i, 3, strData[i][3]);
		m_listRes.SetItemText(i, 4, strData[i][4]);
		m_listRes.SetItemText(i, 5, strData[i][5]);
		m_listRes.SetItemText(i, 6, strData[i][6]);
		m_listRes.SetItemText(i, 7, strData[i][7]);
		m_listRes.SetItemData(i, (LPARAM)m_pData[i]);
	} 
*/	
//	for (s=0; s<n_Mas;s++)
//	for (i=0; i<n_Step*n_point; i++)
	for (j=0; j<n_Step;j++)
	for (s=0; s<n_Mas;s++)
	for (i=0; i<n_point; i++)
	{
		m_pData[i+n_Step*n_point*s] = new ITEMDATA;
		m_pData[i+n_Step*n_point*s]->psz1 = (LPTSTR)(LPCTSTR)strData[s][i][0];
		m_pData[i+n_Step*n_point*s]->psz2 = (LPTSTR)(LPCTSTR)strData[s][i][1];
		m_pData[i+n_Step*n_point*s]->psz3 = (LPTSTR)(LPCTSTR)strData[s][i][2];
		m_pData[i+n_Step*n_point*s]->psz4 = (LPTSTR)(LPCTSTR)strData[s][i][3];
		m_pData[i+n_Step*n_point*s]->psz5 = (LPTSTR)(LPCTSTR)strData[s][i][4];
		m_pData[i+n_Step*n_point*s]->psz6 = (LPTSTR)(LPCTSTR)strData[s][i][5];
		m_pData[i+n_Step*n_point*s]->psz7 = (LPTSTR)(LPCTSTR)strData[s][i][6];

		m_listRes.InsertItem(i+n_Step*n_point*s, strData[s][i][0]);
		m_listRes.SetItemText(i+n_Step*n_point*s, 1, strData[s][i][1]);
		m_listRes.SetItemText(i+n_Step*n_point*s, 2, strData[s][i][2]);
		m_listRes.SetItemText(i+n_Step*n_point*s, 3, strData[s][i][3]);
		m_listRes.SetItemText(i+n_Step*n_point*s, 4, strData[s][i][4]);
		m_listRes.SetItemText(i+n_Step*n_point*s, 5, strData[s][i][5]);
		m_listRes.SetItemText(i+n_Step*n_point*s, 6, strData[s][i][6]);
		m_listRes.SetItemText(i+n_Step*n_point*s, 7, strData[s][i][7]);
		m_listRes.SetItemData(i+n_Step*n_point*s, (LPARAM)m_pData[i+n_Step*n_point*s]);
	} 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTablDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	for (UINT i=0; i<n_Step*NPOINT; i++)
	delete m_pData[i]; 
	
}

void CTablDlg::OnButsavetabres() 
{
	// TODO: Add your control notification handler code here

	UINT s,i;
	int j;
	int n_point = NPOINT;
	if (m_itipimp==TIP_IMP) n_point = 1;

 if (MessageBox(_T("—охранение займЄт некоторое врем€!\n¬ы уверены, что хотите сохранить таблицу результатов измерени€?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
 {//0)
	CSaveTablDlg stbl;
	stbl.m_isposob = m_isposob;
	UpdateData(FALSE);
	if (stbl.DoModal()==IDOK)
	{	UpdateData();
		m_isposob = stbl.m_isposob;
		UpdateData(FALSE);

		CFileDialog dlgs(FALSE,_T("tab"),_T("\\Hard Disk\\Tab\\*.tab"));
		dlgs.m_ofn.lpstrTitle=_T("—охранить как");
		dlgs.m_ofn.lpstrFilter=_T("Line Files (\\Hard Disk\\Tab\\*.tab)");
		UpdateData(FALSE);
		if (dlgs.DoModal()==IDOK)
		{	UpdateData();
			CString name = dlgs.GetPathName();
			CFile file;
			CString str;
			int l;
			if (m_isposob)
			{ //обновить данные
			  file.Open((LPCTSTR)name,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
			  file.SeekToBegin();	
			}
		    else
			{ //добавить данные
			  file.Open((LPCTSTR)name,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeBinary);
			  file.SeekToEnd();
			}
			//формирование данных
		    //1) дата проведени€ измерени€ + заголовок таблицы
			str=strInfoTest+_T("\r\n");
			l=str.GetLength();
	 		file.Write((LPCTSTR)str,l*2);
			//2) рассчетные данные из отчета
			if (!strOtcht.IsEmpty())
			{ int p1=strOtcht.Find(_T(":"),9);	//9 - пропустили врем€ с ":"
			  int p2=strOtcht.Find(_T("\n"),p1);
			  str = strOtcht.Mid(p1+2,p2-p1-1);	//рассчетные данные
			  str = str+_T("\r\n");
		      l=str.GetLength();
	 	      if (l) file.Write((LPCTSTR)str,l*2);
			}
			//3) наименование колонок + измеренные данные
			str=strNameColumn+_T("\r\n");		//строка с наименовани€ми колонок
			l=str.GetLength();
	 		file.Write((LPCTSTR)str,l*2);
			for (s=0;s<n_Mas;s++)
			{
			for (i=0;i<n_Step*n_point;i++)
			{ for (j=0;j<7;j++)
				{ str=strData[s][i][j]+_T("\t");	//данные
				  l=str.GetLength();
	 			  file.Write((LPCTSTR)str,l*2);
				}
			  str=_T("\r\n");					//переход к следующей строке
			  l=str.GetLength();
	 		  file.Write((LPCTSTR)str,l*2);
			}
			}
			str=_T("\n");
			l=str.GetLength();
	 		file.Write((LPCTSTR)str,l*2);
			file.Close();
			MessageBox(_T("—охранение завершено успешно!"),NULL,MB_OK|MB_ICONINFORMATION);	
		}
	}
 }//0)

}


