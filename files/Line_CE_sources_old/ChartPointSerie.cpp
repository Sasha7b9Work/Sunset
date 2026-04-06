#include "StdAfx.h"
#include ".\chartpointserie.h"
#include "ChartSerie.h"
#include "math.h"

CChartPointSerie::CChartPointSerie(CChartCtrl* pParent):CChartSerie(pParent, CChartSerie::stPoints)
{
	m_iPointSize = 3;
	m_iPointType = ptRectangle;//ptEllipse;
	
}

CChartPointSerie::~CChartPointSerie(void)
{
}

void CChartPointSerie::DrawAll(CDC* pDC)
{
	if(!IsVisible())
		return;

	if(!pDC->GetSafeHdc())
		return;

	CBrush brush(m_Color);
	CBrush* pOldBrush;

	pOldBrush = pDC->SelectObject(&brush);

	CPen pen(m_iPenStyle,m_iPenWidth,m_Color);
	CPen *pOldPen;
	pOldPen = pDC->SelectObject(&pen);

	pDC->IntersectClipRect(m_ObjectRect);
	pDC->SetBkMode(TRANSPARENT);

	size_t iPointsCount = m_vPoints.size();
	CPoint ScreenPoint(0,0);
	if (size_point==1)
	SetPointSize(4);
	else
	SetPointSize(3);
	int iHalfLength = 0;
	for(int i=0;i<iPointsCount;i++)
	{
		ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,ScreenPoint);
		iHalfLength = m_iPointSize/2;
		if(m_iPointType == ptEllipse)
			pDC->Ellipse(ScreenPoint.x - iHalfLength, ScreenPoint.y - iHalfLength,
						 ScreenPoint.x + iHalfLength, ScreenPoint.y + iHalfLength);
		else if(m_iPointType == ptRectangle)
		{
			CRect rect(CPoint(ScreenPoint.x - iHalfLength, ScreenPoint.y - iHalfLength),
				CPoint(ScreenPoint.x + iHalfLength, ScreenPoint.y + iHalfLength));
			pDC->Rectangle(&rect);
		}
        else if(m_iPointType == ptTriangle)
		{
			CPoint TrPoints[3];
			TrPoints[0].x = ScreenPoint.x - iHalfLength;
			TrPoints[0].y = ScreenPoint.y + iHalfLength;
			TrPoints[1].x = ScreenPoint.x + iHalfLength;
			TrPoints[1].y = ScreenPoint.y + iHalfLength;
			TrPoints[2].x = ScreenPoint.x;
			TrPoints[2].y = ScreenPoint.y - iHalfLength;
			pDC->Polygon(TrPoints,3);
		}

		
	}

	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CChartPointSerie::Draw(CDC* pDC)
{
}

int CChartPointSerie::GetPointSize()
{
	return m_iPointSize;
}

void CChartPointSerie::SetPointSize(int iPointSize)
{
	m_iPointSize = iPointSize;
}
void CChartPointSerie::SetPointType(PointType Type)
{
	m_iPointType = Type;
}

/*
CPoint CChartPointSerie::GetNearestPoint(CPoint ScreenPoint)
{
	double XValue;
	double YValue;
	CPoint NearestPoint;

	ScreenToValue(XValue,YValue,ScreenPoint);
	int size = m_vPoints.size();
	int iIndex = 0;
	for(iIndex=0; iIndex<size;iIndex++)
	{
		if(m_vPoints[iIndex].X > XValue) break;
	}

	if(iIndex > 0)
	{
		if( abs(m_vPoints[iIndex-1].X - XValue)<abs(m_vPoints[iIndex].X - XValue))
			ValueToScreen(m_vPoints[iIndex-1].X,m_vPoints[iIndex-1].Y,NearestPoint);
		else
			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
	}
	else
	{
		ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
	}

	return NearestPoint;
}

*/



CPoint CChartPointSerie::GetNearestPoint(CPoint ScreenPoint, double& XValue, double &YValue)
{
	double XVal;
	double YVal;
	CPoint NearestPoint;

	if(!m_isXYInverted)
	{
		ScreenToValue(XVal,YVal,ScreenPoint);
		size_t size = m_vPoints.size();
		int iIndex = 0;
		for(iIndex=0; iIndex<size;iIndex++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
		}

		if(iIndex > 0)
		{

			if( fabs(m_vPoints[iIndex-1].X - XVal)< fabs(m_vPoints[iIndex].X - XVal))
			{
				XValue = m_vPoints[iIndex-1].X;
				YValue = m_vPoints[iIndex-1].Y;
				ValueToScreen(m_vPoints[iIndex-1].X,m_vPoints[iIndex-1].Y,NearestPoint);
			}
			else
			{
				XValue = m_vPoints[iIndex].X;
				YValue = m_vPoints[iIndex].Y;
				ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			}
		}
		else
		{
			XValue = m_vPoints[iIndex].X;
			YValue = m_vPoints[iIndex].Y;
			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
		}
	}
	else //isXYInverted
	{
		ScreenToValue(XVal,YVal,ScreenPoint);
		size_t size = m_vPoints.size();
		int iIndex = 0;
		for(iIndex=0; iIndex<size;iIndex++)
		{
			if(m_vPoints[iIndex].Y > YVal) break;
		}

		if(iIndex > 0)
		{

			if( fabs(m_vPoints[iIndex-1].Y - YVal)< fabs(m_vPoints[iIndex].Y - YVal))
			{
				XValue = m_vPoints[iIndex-1].X;
				YValue = m_vPoints[iIndex-1].Y;
				ValueToScreen(m_vPoints[iIndex-1].X,m_vPoints[iIndex-1].Y,NearestPoint);
			}
			else
			{
				XValue = m_vPoints[iIndex].X;
				YValue = m_vPoints[iIndex].Y;
				ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			}
		}
		else
		{
			XValue = m_vPoints[iIndex].X;
			YValue = m_vPoints[iIndex].Y;
			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
		}
	}


	return NearestPoint;
}

CPoint CChartPointSerie::GetNearestLeftPoint(double& XValue, double& YValue)
{
	double XVal;
	double YVal;
	CPoint NearestPoint;

	XVal = XValue;
	YVal = YValue;

	size_t size = m_vPoints.size();
	int iIndex = 0;

	if(!m_isXYInverted)
	{
		
		for(iIndex=0; iIndex<size;iIndex++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
		}
	
		if( fabs(m_vPoints[iIndex-1].X - XVal)< fabs(m_vPoints[iIndex].X - XVal))
		{
			iIndex--;
		}
	}
	else
	{
		for(iIndex=0; iIndex<size;iIndex++)
		{
			if(m_vPoints[iIndex].Y > YVal) break;
		}
	
		if( fabs(m_vPoints[iIndex-1].Y - YVal)< fabs(m_vPoints[iIndex].Y - YVal))
		{
			iIndex--;
		}
	}


    iIndex--;
	if(iIndex < 0) iIndex = 0;


	XValue = m_vPoints[iIndex].X;
	YValue = m_vPoints[iIndex].Y;
	ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);

	return NearestPoint;	
}

CPoint CChartPointSerie::GetNearestRightPoint(double& XValue, double& YValue)
{
	double XVal;
	double YVal;
	CPoint NearestPoint;

	XVal = XValue;
	YVal = YValue;

	size_t size = m_vPoints.size();
	int iIndex = 0;
	

	if(!m_isXYInverted)
	{
		for(iIndex=0; iIndex<size;iIndex++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
		}

		if( fabs(m_vPoints[iIndex-1].X - XVal)< fabs(m_vPoints[iIndex].X - XVal))
		{
			iIndex--;
		}
	}
	else
	{
		for(iIndex=0; iIndex<size;iIndex++)
		{
			if(m_vPoints[iIndex].Y > YVal) break;
		}

		if( fabs(m_vPoints[iIndex-1].Y - YVal)< fabs(m_vPoints[iIndex].Y - YVal))
		{
			iIndex--;
		}
	}


	iIndex++;
	if(iIndex > size - 1) iIndex = size - 1;
	

	XValue = m_vPoints[iIndex].X;
	YValue = m_vPoints[iIndex].Y;
	ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);

	return NearestPoint;	
}