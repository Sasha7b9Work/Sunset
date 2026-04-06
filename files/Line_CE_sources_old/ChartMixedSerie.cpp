#include "StdAfx.h"
#include ".\chartmixedserie.h"
#include "ChartCtrl.h"
#include "math.h"

CChartMixedSerie::CChartMixedSerie(CChartCtrl* pParent): CChartSerie(pParent, CChartSerie::stMixed)
{
	m_iPointType = ptRectangle;//ptEllipse;
	m_iPointSize = 3;
}

CChartMixedSerie::~CChartMixedSerie(void)
{
}

void CChartMixedSerie::DrawAll(CDC* pDC)
{
	if(!IsVisible())
		return;

	if(!pDC->GetSafeHdc())
		return;

	CBrush brush(m_Color);
	CBrush* pOldBrush;
	CPen pen(m_iPenStyle,m_iPenWidth,m_Color);
	CPen *pOldPen;
	pOldPen = pDC->SelectObject(&pen);

	pDC->IntersectClipRect(m_ObjectRect);
	pDC->SetBkMode(TRANSPARENT);

	pOldBrush = pDC->SelectObject(&brush);


	size_t iPointsCount = m_vPoints.size();
	CPoint ScreenPoint(0,0);
	int iHalfLength = 0;
	bool bFirstPoint = true;

	if (size_point==1)
	SetPointSize(4);
	else
	SetPointSize(3);

	for(int i=0;i<iPointsCount;i++)
	{
		ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,ScreenPoint);
		iHalfLength = m_iPointSize/2;
		
		if(m_iPointType == ptEllipse)
		{
			pDC->Ellipse(ScreenPoint.x - iHalfLength, ScreenPoint.y - iHalfLength,
						 ScreenPoint.x + iHalfLength, ScreenPoint.y + iHalfLength);
			if(bFirstPoint)
				pDC->MoveTo(ScreenPoint);
			else
				pDC->LineTo(ScreenPoint);

			bFirstPoint = false;
		}
		else if(m_iPointType == ptRectangle)
		{
			CRect rect(CPoint(ScreenPoint.x - iHalfLength, ScreenPoint.y - iHalfLength),
				CPoint(ScreenPoint.x + iHalfLength, ScreenPoint.y + iHalfLength));
			pDC->Rectangle(&rect);
			if(bFirstPoint)
				pDC->MoveTo(ScreenPoint);
			else
				pDC->LineTo(ScreenPoint);

			bFirstPoint = false;
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
			
			if(bFirstPoint)
				pDC->MoveTo(ScreenPoint);
			else
				pDC->LineTo(ScreenPoint);

			bFirstPoint = false;
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectClipRgn(NULL);
}

void CChartMixedSerie::Draw(CDC* pDC)
{
}

int CChartMixedSerie::GetPointSize()
{
	return m_iPointSize;
}

void CChartMixedSerie::SetPointSize(int iPointSize)
{
	m_iPointSize = iPointSize;
}
void CChartMixedSerie::SetPointType(PointType Type)
{
	m_iPointType = Type;
}

CPoint CChartMixedSerie::GetNearestPoint(CPoint ScreenPoint, double& XValue, double& YValue)
{
	double XVal;
	double YVal;
	double X1p;
	double X2p;
	double Y1p;
	double Y2p;
	double X1, X2, Y1, Y2;
	double nIncrement = 0;
	int iRes = 100;
	CPoint NearestPoint;
	bool isFound = false;

	ScreenToValue(XVal,YVal,ScreenPoint);
	
	size_t size = m_vPoints.size();
	int iIndex = 0;
/*
	for(int i=0; i<size;i++)
	{
		if(m_vPoints[iIndex].X > XVal) break;
		iIndex++;
	}
*/
	int Min, Max;
	Min = 0;
	Max = size - 1;
	iIndex = size/2;
	int count = 0;

	if(!m_isXYInverted)
	{

		while(count<5)
		{
			if( m_vPoints[iIndex].X > XVal )
				Max = iIndex;
			else if(m_vPoints[iIndex].X < XVal)
				Min = iIndex;
			else if(m_vPoints[iIndex].X == XVal)
				break;
			count++;
		}
		iIndex = Min;
		count = Max - Min +1;

		for(int i=0;i<count;i++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
			iIndex++;
		}

		if(iIndex == 0)
		{
			XValue = m_vPoints[iIndex].X;
			YValue = m_vPoints[iIndex].Y;

			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			return NearestPoint;
		}
		if(iIndex == size)
		{
			XValue = m_vPoints[iIndex - 1].X;
			YValue = m_vPoints[iIndex - 1].Y;
			ValueToScreen(m_vPoints[iIndex - 1].X,m_vPoints[iIndex - 1].Y,NearestPoint);
			return NearestPoint;
		}

		X1p = m_vPoints[iIndex - 1].X;
		Y1p = m_vPoints[iIndex - 1].Y;
		X2p = m_vPoints[iIndex].X;
		Y2p = m_vPoints[iIndex].Y;
		X1 = X1p;
		X2 = X2p;

		nIncrement = fabs(X2-X1)/iRes;

		for(i=1;i<3;i++)
		{
			if((X1 + nIncrement*iRes/(2*i)) < XVal) 
				X1 = X1 + nIncrement*iRes/(2*i);
			else if ( (X1 + nIncrement*iRes/(2*i)) == XVal)
			{
				XVal = X1 + nIncrement*iRes/(2*i);
				isFound = true;
				break;
			}
			else
				X2 = X1 + nIncrement*iRes/(2*i);
		}

		if(!isFound)
		{
			int iIndex = 0;
			for(iIndex=0;iIndex<iRes/4;iIndex++)
			{
				if( (X1+nIncrement*iIndex) > XVal) 
				{	XVal = X1 + nIncrement*iIndex;
					break;			
				}
			}

		}
		
		YVal = Y1p + (XVal - X1p)*(Y2p-Y1p)/(X2p-X1p);
		XValue = XVal;
		YValue = YVal;
	}
	else//isXYInverted
	{
		while(count<5)
		{
			if( m_vPoints[iIndex].Y > YVal )
				Max = iIndex;
			else if(m_vPoints[iIndex].Y < YVal)
				Min = iIndex;
			else if(m_vPoints[iIndex].Y == YVal)
				break;
			count++;
		}
		iIndex = Min;
		count = Max - Min +1;

		for(int i=0;i<count;i++)
		{
			if(m_vPoints[iIndex].Y > YVal) break;
			iIndex++;
		}

		if(iIndex == 0)
		{
			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			return NearestPoint;
		}
		if(iIndex == size)
		{
			ValueToScreen(m_vPoints[iIndex - 1].X,m_vPoints[iIndex - 1].Y,NearestPoint);
			return NearestPoint;
		}

		X1p = m_vPoints[iIndex - 1].X;
		Y1p = m_vPoints[iIndex - 1].Y;
		X2p = m_vPoints[iIndex].X;
		Y2p = m_vPoints[iIndex].Y;
		Y1 = Y1p;
		Y2 = Y2p;

		nIncrement = fabs(Y2-Y1)/iRes;

		for(i=1;i<3;i++)
		{
			if((Y1 + nIncrement*iRes/(2*i)) < YVal) 
				Y1 = Y1 + nIncrement*iRes/(2*i);
			else if ( (Y1 + nIncrement*iRes/(2*i)) == YVal)
			{
				YVal = Y1 + nIncrement*iRes/(2*i);
				isFound = true;
				break;
			}
			else
				Y2 = Y1 + nIncrement*iRes/(2*i);
		}

		if(!isFound)
		{
			int iIndex = 0;
			for(iIndex=0;iIndex<iRes/4;iIndex++)
			{
				if( (Y1+nIncrement*iIndex) > YVal) 
				{	YVal = Y1 + nIncrement*iIndex;
					break;			
				}
			}

		}
		XVal = (YVal-Y1p)*(X2p-X1p)/(Y2p-Y1p) + X1p;
		XValue = XVal;
		YValue = YVal;
	}



	ValueToScreen(XVal,YVal,NearestPoint);
	return NearestPoint;
}

CPoint CChartMixedSerie::GetNearestLeftPoint(double& XValue, double& YValue)
{
	double XVal;
	double YVal;
	double X1p;
	double X2p;
	double Y1p;
	double Y2p;
	double X1, X2, Y1, Y2;
	double nIncrement = 0;
	int iRes = 100;
	CPoint NearestPoint;
	bool isFound = false;

	XVal = XValue;
	YVal = YValue;
	
	size_t size = m_vPoints.size();
	int iIndex = 0;
/*
	for(int i=0; i<size;i++)
	{
		if(m_vPoints[iIndex].X > XVal) break;
		iIndex++;
	}
*/
	int Min, Max;
	Min = 0;
	Max = size - 1;
	iIndex = size/2;
	int count = 0;

	if(!m_isXYInverted)
	{

		while(count<5)
		{
			if( m_vPoints[iIndex].X > XVal )
				Max = iIndex;
			else if(m_vPoints[iIndex].X < XVal)
				Min = iIndex;
			else if(m_vPoints[iIndex].X == XVal)
				break;
			count++;
		}
		iIndex = Min;
		count = Max - Min +1;

		for(int i=0;i<count;i++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
			iIndex++;
		}

		if(iIndex == 0)
		{
			XValue = m_vPoints[iIndex].X;
			YValue = m_vPoints[iIndex].Y;

			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			return NearestPoint;
		}
		if(iIndex == size)
		{
			XValue = m_vPoints[iIndex - 1].X;
			YValue = m_vPoints[iIndex - 1].Y;
			ValueToScreen(m_vPoints[iIndex - 1].X,m_vPoints[iIndex - 1].Y,NearestPoint);
			return NearestPoint;
		}

		X1p = m_vPoints[iIndex - 1].X;
		Y1p = m_vPoints[iIndex - 1].Y;
		X2p = m_vPoints[iIndex].X;
		Y2p = m_vPoints[iIndex].Y;
		X1 = X1p;
		X2 = X2p;

		nIncrement = fabs(X2-X1)/iRes;

		for(i=1;i<3;i++)
		{
			if((X1 + nIncrement*iRes/(2*i)) < XVal) 
				X1 = X1 + nIncrement*iRes/(2*i);
			else if ( (X1 + nIncrement*iRes/(2*i)) == XVal)
			{
				XVal = X1 + nIncrement*iRes/(2*i);
				isFound = true;
				break;
			}
			else
				X2 = X1 + nIncrement*iRes/(2*i);
		}

		if(!isFound)
		{
			int iIndex = 0;
			for(iIndex=0;iIndex<iRes/4;iIndex++)
			{
				if( (X1+nIncrement*iIndex) > XVal) 
				{	XVal = X1 + nIncrement*iIndex;
					break;			
				}
			}

		}
		XVal -= nIncrement*2;
		YVal = Y1p + (XVal - X1p)*(Y2p-Y1p)/(X2p-X1p);
		XValue = XVal;
		YValue = YVal;
	}
	else//isXYInverted
	{
		while(count<5)
		{
			if( m_vPoints[iIndex].Y > YVal )
				Max = iIndex;
			else if(m_vPoints[iIndex].Y < YVal)
				Min = iIndex;
			else if(m_vPoints[iIndex].Y == YVal)
				break;
			count++;
		}
		iIndex = Min;
		count = Max - Min +1;

		for(int i=0;i<count;i++)
		{
			if(m_vPoints[iIndex].Y > YVal) break;
			iIndex++;
		}

		if(iIndex == 0)
		{
			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			return NearestPoint;
		}
		if(iIndex == size)
		{
			ValueToScreen(m_vPoints[iIndex - 1].X,m_vPoints[iIndex - 1].Y,NearestPoint);
			return NearestPoint;
		}

		X1p = m_vPoints[iIndex - 1].X;
		Y1p = m_vPoints[iIndex - 1].Y;
		X2p = m_vPoints[iIndex].X;
		Y2p = m_vPoints[iIndex].Y;
		Y1 = Y1p;
		Y2 = Y2p;

		nIncrement = fabs(Y2-Y1)/iRes;

		for(i=1;i<3;i++)
		{
			if((Y1 + nIncrement*iRes/(2*i)) < YVal) 
				Y1 = Y1 + nIncrement*iRes/(2*i);
			else if ( (Y1 + nIncrement*iRes/(2*i)) == YVal)
			{
				YVal = Y1 + nIncrement*iRes/(2*i);
				isFound = true;
				break;
			}
			else
				Y2 = Y1 + nIncrement*iRes/(2*i);
		}

		if(!isFound)
		{
			int iIndex = 0;
			for(iIndex=0;iIndex<iRes/4;iIndex++)
			{
				if( (Y1+nIncrement*iIndex) > YVal) 
				{	YVal = Y1 + nIncrement*iIndex;
					break;			
				}
			}

		}
		YVal -= nIncrement*2;
		XVal = (YVal-Y1p)*(X2p-X1p)/(Y2p-Y1p) + X1p;
		XValue = XVal;
		YValue = YVal;
	}



	ValueToScreen(XVal,YVal,NearestPoint);
	return NearestPoint;
}

CPoint CChartMixedSerie::GetNearestRightPoint(double& XValue, double& YValue)
{
	double XVal;
	double YVal;
	double X1p;
	double X2p;
	double Y1p;
	double Y2p;
	double X1, X2, Y1, Y2;
	double nIncrement = 0;
	int iRes = 100;
	CPoint NearestPoint;
	bool isFound = false;

	XVal = XValue;
	YVal = YValue;
	
	size_t size = m_vPoints.size();
	int iIndex = 0;
/*
	for(int i=0; i<size;i++)
	{
		if(m_vPoints[iIndex].X > XVal) break;
		iIndex++;
	}
*/
	int Min, Max;
	Min = 0;
	Max = size - 1;
	iIndex = size/2;
	int count = 0;

	if(!m_isXYInverted)
	{

		while(count<5)
		{
			if( m_vPoints[iIndex].X > XVal )
				Max = iIndex;
			else if(m_vPoints[iIndex].X < XVal)
				Min = iIndex;
			else if(m_vPoints[iIndex].X == XVal)
				break;
			count++;
		}
		iIndex = Min;
		count = Max - Min +1;

		for(int i=0;i<count;i++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
			iIndex++;
		}

		if(iIndex == 0)
		{
			XValue = m_vPoints[iIndex].X;
			YValue = m_vPoints[iIndex].Y;

			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			return NearestPoint;
		}
		if(iIndex == size)
		{
			XValue = m_vPoints[iIndex - 1].X;
			YValue = m_vPoints[iIndex - 1].Y;
			ValueToScreen(m_vPoints[iIndex - 1].X,m_vPoints[iIndex - 1].Y,NearestPoint);
			return NearestPoint;
		}

		X1p = m_vPoints[iIndex - 1].X;
		Y1p = m_vPoints[iIndex - 1].Y;
		X2p = m_vPoints[iIndex].X;
		Y2p = m_vPoints[iIndex].Y;
		X1 = X1p;
		X2 = X2p;

		nIncrement = fabs(X2-X1)/iRes;

		for(i=1;i<3;i++)
		{
			if((X1 + nIncrement*iRes/(2*i)) < XVal) 
				X1 = X1 + nIncrement*iRes/(2*i);
			else if ( (X1 + nIncrement*iRes/(2*i)) == XVal)
			{
				XVal = X1 + nIncrement*iRes/(2*i);
				isFound = true;
				break;
			}
			else
				X2 = X1 + nIncrement*iRes/(2*i);
		}

		if(!isFound)
		{
			int iIndex = 0;
			for(iIndex=0;iIndex<iRes/4;iIndex++)
			{
				if( (X1+nIncrement*iIndex) > XVal) 
				{	XVal = X1 + nIncrement*iIndex;
					break;			
				}
			}

		}
		
		XVal += nIncrement;
		YVal = Y1p + (XVal - X1p)*(Y2p-Y1p)/(X2p-X1p);
		XValue = XVal;
		YValue = YVal;
	}
	else//isXYInverted
	{
		while(count<5)
		{
			if( m_vPoints[iIndex].Y > YVal )
				Max = iIndex;
			else if(m_vPoints[iIndex].Y < YVal)
				Min = iIndex;
			else if(m_vPoints[iIndex].Y == YVal)
				break;
			count++;
		}
		iIndex = Min;
		count = Max - Min +1;

		for(int i=0;i<count;i++)
		{
			if(m_vPoints[iIndex].Y > YVal) break;
			iIndex++;
		}

		if(iIndex == 0)
		{
			ValueToScreen(m_vPoints[iIndex].X,m_vPoints[iIndex].Y,NearestPoint);
			return NearestPoint;
		}
		if(iIndex == size)
		{
			ValueToScreen(m_vPoints[iIndex - 1].X,m_vPoints[iIndex - 1].Y,NearestPoint);
			return NearestPoint;
		}

		X1p = m_vPoints[iIndex - 1].X;
		Y1p = m_vPoints[iIndex - 1].Y;
		X2p = m_vPoints[iIndex].X;
		Y2p = m_vPoints[iIndex].Y;
		Y1 = Y1p;
		Y2 = Y2p;

		nIncrement = fabs(Y2-Y1)/iRes;

		for(i=1;i<3;i++)
		{
			if((Y1 + nIncrement*iRes/(2*i)) < YVal) 
				Y1 = Y1 + nIncrement*iRes/(2*i);
			else if ( (Y1 + nIncrement*iRes/(2*i)) == YVal)
			{
				YVal = Y1 + nIncrement*iRes/(2*i);
				isFound = true;
				break;
			}
			else
				Y2 = Y1 + nIncrement*iRes/(2*i);
		}

		if(!isFound)
		{
			int iIndex = 0;
			for(iIndex=0;iIndex<iRes/4;iIndex++)
			{
				if( (Y1+nIncrement*iIndex) > YVal) 
				{	YVal = Y1 + nIncrement*iIndex;
					break;			
				}
			}

		}

		YVal += nIncrement;
		XVal = (YVal-Y1p)*(X2p-X1p)/(Y2p-Y1p) + X1p;
		XValue = XVal;
		YValue = YVal;
	}



	ValueToScreen(XVal,YVal,NearestPoint);
	return NearestPoint;
}
