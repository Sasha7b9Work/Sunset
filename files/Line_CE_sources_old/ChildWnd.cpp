// ChildWnd.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "ChildWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD adwStyles[]=
{  WS_CAPTION|WS_SYSMENU|WS_BORDER|WS_THICKFRAME|FWS_ADDTOTITLE,   // Дерево   
   WS_CAPTION|WS_SYSMENU|WS_BORDER|WS_THICKFRAME|FWS_ADDTOTITLE	// Отчёт   
};
/////////////////////////////////////////////////////////////////////////////
// CChildWnd

IMPLEMENT_DYNCREATE(CChildWnd, CFrameWnd)

CChildWnd::CChildWnd()
{
}

CChildWnd::~CChildWnd()
{
}


BEGIN_MESSAGE_MAP(CChildWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CChildWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildWnd message handlers

int CChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	switch(++nCountChildWnd)
	{ case 1:	//ClientToScreen(&RectTuning);//Окно "Модель измерения" (Дерево)
				MoveWindow(&RectTuning);	 
//	            //ScreenToClient(&RectTuning);
	            break;
	  case 2:	
		        MoveWindow(&RectOtcht);     //Окно "Отчёт"
		        break;   
	}

	//Окно "Модель измерения"
	//-----------------------
	if (nCountChildWnd==1)	
	{ 
      if (!m_TreeCtrl.Create(WS_CHILD | WS_BORDER | TVS_HASLINES |	//WS_VISIBLE | 
						TVS_HASBUTTONS | TVS_LINESATROOT | TVS_EDITLABELS | TVS_SHOWSELALWAYS |
						TVS_TRACKSELECT,							//TVS_INFOTIP |
	 					CRect(0,0,0,0), this,IDC_TREE1))//124))
				return -1;
	  m_TreeCtrl.ModifyStyleEx(m_TreeCtrl.GetStyle(), WS_EX_CLIENTEDGE);

	  HICON hIcon[4];
	  int n;
	  m_imageList.Create(16,16,0,0,3);
	  hIcon[0]=AfxGetApp()->LoadIcon(IDI_SELECT);
	  hIcon[1]=AfxGetApp()->LoadIcon(IDI_LEVEL1);
	  hIcon[2]=AfxGetApp()->LoadIcon(IDI_LEVEL2);
	  for (n=0;n<3;n++)
		m_imageList.Add(hIcon[n]);
	  m_TreeCtrl.DeleteAllItems();
	  m_TreeCtrl.SetImageList(&m_imageList,TVSIL_NORMAL);
/*	  
	  if (!m_wndToolBarTree.CreateEx(this,TBSTYLE_FLAT,
		    WS_CHILD |  CBRS_BORDER_BOTTOM | CBRS_BORDER_3D |CBRS_ALIGN_TOP //WS_VISIBLE | CBRS_TOP |
		    | CBRS_TOOLTIPS | CBRS_FLYBY|CBRS_SIZE_DYNAMIC| CBRS_GRIPPER, CRect(0, 0, 0, 0)) ||	//				 
		  !m_wndToolBarTree.LoadToolBar(IDR_TOOLBARTREE))
		{
		  TRACE0("Failed to create toolbartree\n");
		  return -1;      // fail to create
		}

	  m_wndToolBarTree.CalcFixedLayout(FALSE,TRUE);		//панель плавающая, горизонтальная
	  m_wndToolBarTree.EnableDocking(CBRS_ALIGN_TOP);
	  this->EnableDocking(CBRS_ALIGN_TOP);				//приклеенная к верхней границе окна
*/	}

	//Окно "Отчёт"
	//------------
	if (nCountChildWnd==2)	
	{ 
	  if (!m_editOtcht.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|
	 					     ES_MULTILINE|ES_AUTOVSCROLL,	//|ES_READONLY|ES_WANTRETURN
	 						 CRect(0,0,0,0), this,124))		//IDC_EDITINFO))
		        return -1;
	  //m_editOtcht.ModifyStyleEx(0,WS_EX_LEFTSCROLLBAR);
	  if (!m_fontOtcht.CreateStockObject(SYSTEM_FONT))
		if (!m_fontOtcht.CreatePointFont(8, _T("Arial")))
			return -1;
	  m_editOtcht.SetFont(&m_fontOtcht);
/*
	  if (!m_wndToolBarOtcht.CreateEx(this,TBSTYLE_FLAT,
		    WS_CHILD |  CBRS_BORDER_BOTTOM | CBRS_BORDER_3D |CBRS_ALIGN_LEFT //WS_VISIBLE | CBRS_TOP |
		    | CBRS_TOOLTIPS | CBRS_FLYBY|CBRS_SIZE_DYNAMIC| CBRS_GRIPPER, CRect(0, 0, 0, 0)) ||	//				 
		  !m_wndToolBarOtcht.LoadToolBar(IDR_TOOLBAROTCHT))
		{
		  TRACE0("Failed to create toolbarotcht\n");
		  return -1;      // fail to create
		}
	  m_wndToolBarOtcht.CalcFixedLayout(FALSE,FALSE);	//панель плавающая, вертикальная
	  m_wndToolBarOtcht.EnableDocking(CBRS_ALIGN_LEFT);
	  this->EnableDocking(CBRS_ALIGN_LEFT);			    //приклеенная к левой границе окна
*/	}

	
	return 0;
}

BOOL CChildWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	cs.style |= adwStyles[nCountChildWnd];
	return CFrameWnd::PreCreateWindow(cs);
}

void CChildWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	WORD nWnd = (WORD)::GetWindowLong(this->GetSafeHwnd(),GWL_USERDATA);
	switch(nWnd)
	{ case 0: AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_VIEW_TREE,0);
			  break;
	  case 1: AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_VIEW_OTCHT,0);
			  break;
	}
	
	//CFrameWnd::OnClose();// - не надо!!! - просто сворачиваем в точку
}

/*
void CChildWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	WORD nWnd = (WORD)::GetWindowLong(this->GetSafeHwnd(),GWL_USERDATA);
//	WINDOWPLACEMENT wp;
//	apChildWnd[nWnd]->GetWindowPlacement(&wp);
	CRect Rect;
	apChildWnd[nWnd]->GetClientRect(&Rect);
	switch(nWnd)
	{ case 0:	if (nCheckTree)								
				{ 
//		          RectTuning=wp.rcNormalPosition;
				  apChildWnd[nWnd]->MoveWindow(&RectTuning);	
	              apChildWnd[nWnd]->ShowWindow(SW_SHOW);
				  Rect.top=Rect.top+24;
		          //apChildWnd[nWnd]->m_TreeCtrl.MoveWindow(&Rect);	
 			      //apChildWnd[nWnd]->m_TreeCtrl.ShowWindow(SW_SHOW);
				  //apChildWnd[nWnd]->ViewToolBarTree();
				  ScreenToClient(&RectTuning);
				} 
				break;
	  case 1:   if (nCheckOtcht)
				{ 
//				  RectOtcht=wp.rcNormalPosition;
				  apChildWnd[nWnd]->MoveWindow(&RectOtcht);	
	              apChildWnd[nWnd]->ShowWindow(SW_SHOW);
				  Rect.left=Rect.left+24;
				  apChildWnd[nWnd]->m_editOtcht.MoveWindow(&Rect);	
				  apChildWnd[nWnd]->m_editOtcht.ShowWindow(SW_SHOW);
				  //apChildWnd[nWnd]->ViewToolBarOtcht();
				  ScreenToClient(&RectOtcht);
				}
				break;
	}
	apChildWnd[nWnd]->UpdateWindow();
	Invalidate();	

}
*/


void CChildWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	WORD nWnd = (WORD)::GetWindowLong(this->GetSafeHwnd(),GWL_USERDATA);
	CRect rect(0,0,0,0);
	switch(nWnd)
	{ case 0: if (nCheckTree) rect = RectTuning;
		      break;
	  case 1: if (nCheckOtcht) rect = RectOtcht;
		      break;
	}
	int cx=rect.right-rect.left;
	int cy=rect.bottom-rect.top;
	if (nState==WA_INACTIVE)
	{ //EnableWindow(FALSE); 
	  SetWindowPos(&CWnd::wndNoTopMost,
				  rect.left,rect.top,cx,cy,SWP_NOZORDER);
	}
	else
	{ //EnableWindow(TRUE);
	  SetWindowPos(&CWnd::wndTop,
				  rect.left,rect.top,cx,cy,SWP_NOZORDER);
	}
	
}
