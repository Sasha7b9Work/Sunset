#include "StdAfx.h"
#include ".\chartmarker.h"
#include "ChartCtrl.h"

CChartMarker::CChartMarker(CChartCtrl* pParent) : CChartObject(pParent)
{
	m_MarkerHeight = 10;
	m_MarkerHalfBorder = 0.6 * m_MarkerHeight;

	m_iPenSize = 0;
	m_iPenStyle = PS_SOLID;
	m_Color = RGB(0,0,0);
	m_FillColor = RGB(255,255,255);

	m_iRelatedSerieIndex = 0;
}

CChartMarker::~CChartMarker(void)
{
}

void CChartMarker::SetPosition(double XValue, double YValue)
{
	X = XValue;
	Y = YValue;
	/*
	CRect NewRect;
	NewRect.bottom = iPosY;
	NewRect.top = iPosY - m_MarkerHeight;
	NewRect.left = iPosX - m_MarkerHalfBorder;
	NewRect.right = iPosX + m_MarkerHalfBorder;

	CChartObject::SetRect(NewRect);
	*/
}

void CChartMarker::Draw(CDC* pDC)
{
	if(! pDC->GetSafeHdc())
		return;
	if(!IsVisible())
		return;

	CPen pen(m_iPenStyle,m_iPenSize,m_Color);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CBrush brush(m_FillColor);
	CBrush* pOldBrush;
	pOldBrush = pDC->SelectObject(&brush);

	pDC->IntersectClipRect(m_WorkingRect);
	CPoint ScreenPoint;
	m_pParent->GetSerie(m_iRelatedSerieIndex)->ValueToScreen(X,Y,ScreenPoint);

	CPoint Points[3];
	/*
	Points[0].x = m_ObjectRect.left;
	Points[0].y = m_ObjectRect.top;
	Points[1].x = m_ObjectRect.right;
	Points[1].y = m_ObjectRect.top;
	Points[2].x = m_ObjectRect.left + m_MarkerHalfBorder;
	Points[2].y = m_ObjectRect.bottom;
*/	

	Points[0].x = ScreenPoint.x - m_MarkerHalfBorder;
	Points[0].y = ScreenPoint.y - m_MarkerHeight;
	Points[1].x = ScreenPoint.x + m_MarkerHalfBorder;
	Points[1].y = ScreenPoint.y - m_MarkerHeight;
	Points[2].x = ScreenPoint.x;
	Points[2].y = ScreenPoint.y;

	pDC->Polygon(Points,3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SelectClipRgn(NULL);
}


void CChartMarker::SetRelatedSerie(int SerieIndex)
{
	m_iRelatedSerieIndex = SerieIndex;
}

int CChartMarker::GetRelatedSerieIndex(void)
{
	return m_iRelatedSerieIndex;
}

void CChartMarker::GetValues(double& XValue, double& YValue)
{
	XValue = X;
	YValue = Y;
}

void CChartMarker::SetColor(COLORREF Color)
{
	m_Color = Color;
}
void CChartMarker::SetFillColor(COLORREF Color)
{
	m_FillColor = Color;
}
