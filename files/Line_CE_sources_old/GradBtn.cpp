// GradBtn.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "GradBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradBtn

CGradBtn::CGradBtn()
{
	m_ntimer=0;
	m_bActive = false;
	m_bClick=FALSE;
}

CGradBtn::~CGradBtn()
{
}


BEGIN_MESSAGE_MAP(CGradBtn, CButton)
	//{{AFX_MSG_MAP(CGradBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradBtn message handlers

void CGradBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	UINT uStyle = DFCS_BUTTONPUSH;
	 // This code only works with buttons.
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

  // If drawing selected, add the pushed style to DrawFrameControl.
  if (lpDrawItemStruct->itemState & ODS_SELECTED)
    uStyle |= DFCS_PUSHED;

  // Draw the button frame.
  ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
    DFC_BUTTON, uStyle);

  // Get the button's text.
  CString strText;
  GetWindowText(strText);
  CRect rect = lpDrawItemStruct->rcItem;
  rect.DeflateRect(2,2,2,2);
  if (m_bClick && m_bActive)
	{
	GradientRectDL(lpDrawItemStruct->hDC, rect);
	}
  else 
	{
	 GradientRectLD(lpDrawItemStruct->hDC, rect);
	}
	if (m_bActive) m_ntimer=SetTimer(1, 100, NULL);
  // Draw the button text using the text color red.
  rect.top=rect.top;//+5;
  ::SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
  if (strText.Find(L'\n')==-1) rect.top=rect.top+6;
  ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), rect,DT_CENTER);
	
	
}
void CGradBtn::GradientRectDL(HDC pDC, CRect ARect)
{
	CRect ColorBand;
	int I;
	double Delta;
	HBRUSH Brush;
	ColorBand = ARect;

	const double color1_light=233.0;//253.0;234;//209;//
	const double color2_light=233.0;//253.0;234;//209;//
	const double color3_light=233.0;//253.0;234;//209;//

	const double color1_dark=173.0;//77;//178.0;//198.0;153;//119;//
	const double color2_dark=173.0;//77;//180.0;//197.0;153;//119;//
	const double color3_dark=173.0;//77;//191.0;//215.0;153;//119;//

	double n_height1=ColorBand.Height()/3;
	double n_height2=ColorBand.Height()-n_height1;

	double delta1=(color1_light-color1_dark)/n_height2;
	double delta2=(color2_light-color2_dark)/n_height2;
	double delta3=(color3_light-color3_dark)/n_height2;

	Delta = (ARect.Height()-n_height1) / n_height2;
	double current_color1=color1_dark;
	double current_color2=color2_dark;
	double current_color3=color3_dark;

	for (I=0;I<n_height2;I++)
		{
			ColorBand.top = (long)((double)ARect.top + I * Delta);
			ColorBand.bottom = (long)((double)ColorBand.top + Delta);
			Brush = CreateSolidBrush(RGB((BYTE)current_color1,(BYTE)current_color2,(BYTE)current_color3));
			FillRect(pDC, ColorBand, Brush);
			current_color1+=delta1;
			current_color2+=delta2;
			current_color3+=delta3;
			DeleteObject(Brush);
		}
	ColorBand.top = ColorBand.bottom;
	ColorBand.bottom = ARect.bottom;
	Brush = CreateSolidBrush(RGB((BYTE)color1_light,(BYTE)color2_light,(BYTE)color3_light));
	FillRect(pDC, ColorBand, Brush);
	DeleteObject(Brush);
	
}
void CGradBtn::GradientRectLD(HDC pDC, CRect ARect)
{
	CRect ColorBand,ARect1;
	int I;
	double Delta;
	HBRUSH Brush;
	ColorBand = ARect;
	
	if (m_bActive | m_bClick)
	{
/*		HPEN  hpenYellow;//,hpenOld;
		hpenYellow = CreatePen (PS_SOLID, 2, RGB(252,210,121));
		SelectObject(pDC,hpenYellow);
		//hpenOld = SelectObject(pDC,hpenYellow);

		Rectangle(pDC,ColorBand.left,ColorBand.top,ColorBand.right,ColorBand.bottom);//,ColorBand.Width(), ColorBand.Height());
		//SelectObject(pDC,hpenOld);
		DeleteObject(hpenYellow);
*/		ColorBand.DeflateRect(2,2);
		ARect1=ColorBand;
	}
	else
		ARect1=ARect;

	const double color1_light=233.0;//253.0;234;//209;//
	const double color2_light=233.0;//253.0;234;//209;//
	const double color3_light=233.0;//253.0;234;//209;//

	const double color1_dark=173.0;//198.0;153;//119;//
	const double color2_dark=173.0;//197.0;153;//119;//
	const double color3_dark=173.0;//215.0;153;//119;//

	double n_height1=ColorBand.Height()/3;
	double n_height2=ColorBand.Height()-n_height1;

	double delta1=(color1_light-color1_dark)/n_height2;
	double delta2=(color2_light-color2_dark)/n_height2;
	double delta3=(color3_light-color3_dark)/n_height2;

	ColorBand.bottom = ARect1.top+(long)n_height1;
	Brush = CreateSolidBrush(RGB((BYTE)color1_light,(BYTE)color2_light,(BYTE)color3_light));
	FillRect(pDC, ColorBand, Brush);
	DeleteObject(Brush);

	Delta = (ARect1.Height()-n_height1) / n_height2;
	double current_color1=color1_light;
	double current_color2=color2_light;
	double current_color3=color3_light;
	for (I=0;I<n_height2;I++)
		{
			ColorBand.top = (long)((double)(ARect1.top+n_height1) + I * Delta);
			ColorBand.bottom = (long)((double)ColorBand.top + Delta);
			Brush = CreateSolidBrush(RGB((BYTE)current_color1,(BYTE)current_color2,(BYTE)current_color3));
			FillRect(pDC, ColorBand, Brush);
			current_color1-=delta1;
			current_color2-=delta2;
			current_color3-=delta3;
			DeleteObject(Brush);
		}
}


void CGradBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bClick=TRUE;
	
	CButton::OnLButtonDown(nFlags, point);
}

void CGradBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bClick=FALSE;
	RedrawWindow();
	
	CButton::OnLButtonUp(nFlags, point);
}

void CGradBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bActive)
	{
	  CPoint pt; 
	  CRect rect;
	  GetCursorPos(&pt);
	  GetWindowRect(&rect);
	  if(rect.PtInRect(pt))
		{	 
		m_bActive = true;
		RedrawWindow();
		}
	}
	
	
	CButton::OnMouseMove(nFlags, point);
}

void CGradBtn::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
    {
	  CPoint pt; 
	  CRect rect;
	  GetCursorPos(&pt);
	  GetWindowRect(&rect);
	  if(!rect.PtInRect(pt))
		{	 
		m_bActive = false;
		KillTimer(m_ntimer);
		RedrawWindow();
		}
    }
	
	CButton::OnTimer(nIDEvent);
}
