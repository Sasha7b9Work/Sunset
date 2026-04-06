// TreeViewDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "TreeViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeViewDlg dialog


CTreeViewDlg::CTreeViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreeViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent=pParent;
	m_nID=CTreeViewDlg::IDD;
}


void CTreeViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeViewDlg)
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeViewDlg, CDialog)
	//{{AFX_MSG_MAP(CTreeViewDlg)
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeViewDlg message handlers

BOOL CTreeViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->MoveWindow(&RectTree);
	this->ShowWindow(SW_SHOW);
	this->SetWindowText(strFileName);
	CRect Rect;
	GetClientRect(&Rect);
	m_TreeCtrl.MoveWindow(&Rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreeViewDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	this->MoveWindow(0,0,0,0);
	this->ShowWindow(SW_HIDE);
//	CDialog::OnClose();//не надо!!!
}

void CTreeViewDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
//	CDialog::PostNcDestroy();//не надо!!!
}



BOOL CTreeViewDlg::Create() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::Create(m_nID, m_pParent);
}

/*
void CTreeViewDlg::OnSetdispinfoTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CTreeViewDlg::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}


void CTreeViewDlg::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint	point;
	UINT uFlags;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hSelected = m_TreeCtrl.HitTest(point, &uFlags);
//	if (hSelected != NULL)
	
//	HTREEITEM hItem = m_TreeCtrl.HitTest(mPoint, &uFlags);

//	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
//	{
//		m_TreeCtrl.Select(hItem, TVGN_CARET);
//	}
//
	  CMenu menu;
	  menu.LoadMenu(IDR_MENUFLOATINGTEST);
	  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	
	*pResult = 0;
}
*/

void CTreeViewDlg::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UINT i=0;
	UINT k=0;

	HTREEITEM hSelected = pNMTreeView -> itemNew.hItem;
	if (hSelected != NULL)
	{  char text[15];
	   TV_ITEM item;
	   item.mask = TVIF_HANDLE |TVIF_TEXT|TVIF_PARAM;	
	   item.hItem = hSelected;
	   item.pszText = (unsigned short*)text;
	   item.cchTextMax = 14;
	   VERIFY(m_TreeCtrl.GetItem(&item));
       DWORD dw=(DWORD)(m_TreeCtrl.GetItemData(hSelected));
	   if (dw!=0xffff)	     //выбрали уровень теста => разрешить измерение
	   { 
	     i = dw/1000;		 //i - элемент
	     k = dw%1000;		 //k - тест
		 if (k < NT)
		 { i_SelUrov = 2;    //выделенный уровень:2-тест
		 }
	     else 
		 { i_SelUrov = 1;    //выделенный уровень:1-элемент
		   k = 0;
		 }
         i_EL       = i;     //текущий номер элемента
         i_MOD      = k;     //текущий номер теста

	     AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_TREE1,0);
//		 if (i_SelUrov) { m_bEditCut = TRUE; }
	   }
	}	
	*pResult = 0;
}






