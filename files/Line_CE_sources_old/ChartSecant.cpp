#include "StdAfx.h"
#include ".\chartsecant.h"
#include "ChartCtrl.h"

CChartSecant::CChartSecant(CChartCtrl* pParent) : CChartObject(pParent)
{
	m_pHorizontalAxis = m_pParent->m_pAxisBottom;
	m_pVerticalAxis = m_pParent->m_pAxisLeft;
	m_Color = RGB(0,0,0); 
	m_iPenWidth = 0;
	m_iPenStyle = PS_SOLID;//PS_DASH;
}

CChartSecant::~CChartSecant(void)
{
}

void CChartSecant::SetPoints(double X, double Y, int Index)
{
	if(Index == 0)
	{
		X1 = X;
		Y1 = Y;
	}
	else
	{
		X2 = X;
		Y2 = Y;
	}
}

void CChartSecant::SetPoints(double X, double Y)
{
	if(m_pParent->m_iActiveMarkerIndex == 0)
	{
		X1 = X;
		Y1 = Y;
	}
	else
	{
		X2 = X;
		Y2 = Y;
	}
}

void CChartSecant::Draw(CDC* pDC)
{
	if(!IsVisible())
		return;

	pDC->IntersectClipRect(m_ObjectRect);

	CPen pen(m_iPenStyle,m_iPenWidth,m_Color);
	CPen* pOldPen;

	pOldPen = pDC->SelectObject(&pen);
//	YVal = Y1 + (XVal - X1)*(Y2-Y1)/(X2-X1);

	CPoint ScreenPoint1;
	CPoint ScreenPoint2;

	ValueToScreen(X1,Y1,ScreenPoint1);
	
	//pDC->MoveTo(ScreenPoint1.x,ScreenPoint1.y);
	ValueToScreen(X2,Y2,ScreenPoint2);
	//pDC->LineTo(ScreenPoint2.x,ScreenPoint2.y);

	if(ScreenPoint2.x != ScreenPoint1.x)
	{

		pDC->MoveTo(m_ObjectRect.left, (ScreenPoint1.y + (m_ObjectRect.left - ScreenPoint1.x)*(ScreenPoint2.y - ScreenPoint1.y)/(ScreenPoint2.x - ScreenPoint1.x) ));
		//pDC->LineTo(ScreenPoint1.x,ScreenPoint1.y);
		pDC->LineTo(m_ObjectRect.right, (ScreenPoint1.y + (m_ObjectRect.right - ScreenPoint1.x)*(ScreenPoint2.y - ScreenPoint1.y)/(ScreenPoint2.x - ScreenPoint1.x) ));
		//pDC->LineTo(ScreenPoint2.x,ScreenPoint2.y);
	}
	else 
	{	
		pDC->MoveTo(ScreenPoint1.x, m_ObjectRect.top);
		pDC->LineTo(ScreenPoint1.x,m_ObjectRect.bottom);
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectClipRgn(NULL);

}

void CChartSecant::ValueToScreen(double XValue, double YValue, CPoint& ScreenPoint)
{
	ScreenPoint.x = m_pHorizontalAxis->ValueToScreen(XValue);
	ScreenPoint.y = m_pVerticalAxis->ValueToScreen(YValue);
}

void CChartSecant::ScreenToValue(double& XValue, double& YValue, CPoint ScreenPoint)
{
	XValue = m_pHorizontalAxis->ScreenToValue(ScreenPoint.x);
	YValue = m_pVerticalAxis->ScreenToValue(ScreenPoint.y);
}


void CChartSecant::GetIntersection(double& X1, double& Y1, double& X2, double& Y2)
{
	CPoint ScreenPoint1;
	CPoint ScreenPoint2;

	ValueToScreen(X1,Y1,ScreenPoint1);
	ValueToScreen(X2,Y2,ScreenPoint2);

	double k = (Y2-Y1)/(X2-X1);
	double b = Y1 - X1*(Y2-Y1)/(X2-X1);

	double Min, Max;

	m_pVerticalAxis->GetMinMax(Min,Max);
	
	X1 = (Min - b)/k;
	X2 = (Max - b)/k;

	m_pHorizontalAxis->GetMinMax(Min,Max);

	Y1 = k*Min + b;
	Y2 = k*Max + b;

}
