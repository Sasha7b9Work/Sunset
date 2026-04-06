#include "StdAfx.h"
#include ".\chartlineserie.h"
#include "math.h"

CChartLineSerie::CChartLineSerie(CChartCtrl* pParent) : CChartSerie(pParent,stLine)
{
}

CChartLineSerie::~CChartLineSerie(void)
{
}

void CChartLineSerie::DrawAll(CDC* pDC)
{
	if(!IsVisible())
		return;

	CPen pen(m_iPenStyle,m_iPenWidth,m_Color);
	CPen* pOldPen;

	
	
	if(pDC->GetSafeHdc())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->IntersectClipRect(m_ObjectRect);
		pOldPen = pDC->SelectObject(&pen);

		for(int i=0; i<(int)m_vPoints.size()-1; i++)
		{
			//We don't draw a line between the origin and the first point -> we must have
			// a least 2 points before begining drawing
			CPoint ScreenPoint;
			ValueToScreen(m_vPoints[i].X,m_vPoints[i].Y,ScreenPoint);
			pDC->MoveTo(ScreenPoint.x,ScreenPoint.y);
			ValueToScreen(m_vPoints[i+1].X,m_vPoints[i+1].Y,ScreenPoint);
			pDC->LineTo(ScreenPoint.x,ScreenPoint.y);
			}
	
				


		pDC->SelectClipRgn(NULL);
		pDC->SelectObject(pOldPen);
	}

}

void CChartLineSerie::Draw(CDC* pDC)
{

	

}



CPoint CChartLineSerie::GetNearestPoint(CPoint ScreenPoint, double& XValue, double& YValue)
{

	double XVal;
	double YVal;
	double X1p;
	double X2p;
	double Y1p;
	double Y2p;
	double X1, X2;
	double Y1, Y2;
	double nIncrement = 0;
	int iRes = 100;
	CPoint NearestPoint;
	bool isFound = false;

	ScreenToValue(XVal,YVal,ScreenPoint);
	
	size_t size = m_vPoints.size();
	int iIndex = 0;

	if(!m_isXYInverted)
	{
		for(int i=0; i<size;i++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
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
		for(int i=0; i<size;i++)
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



	/*
	double XVal;
	double YVal;
	double X1p;
	double X2p;
	double Y1p;
	double Y2p;
	double X1, X2;
	double nIncrement = 0;
	int iRes = 100;
	CPoint NearestPoint;
	bool isFound = false;

	ScreenToValue(XVal,YVal,ScreenPoint);
	size_t size = m_vPoints.size();
	int iIndex = 0;
	for(int i=0; i<size;i++)
	{
		if(m_vPoints[iIndex].X > XVal) break;
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
	X1 = X1p;
	X2 = X2p;

	nIncrement = fabs(X2-X1)/iRes;

	for(int i=1;i<3;i++)
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
	ValueToScreen(XVal,YVal,NearestPoint);

	return NearestPoint;
	*/
}

CPoint CChartLineSerie::GetNearestLeftPoint(double& XValue, double& YValue)
{
	double XVal;
	double YVal;
	double X1p;
	double X2p;
	double Y1p;
	double Y2p;
	double X1, X2;
	double Y1, Y2;
	double nIncrement = 0;
	int iRes = 100;
	CPoint NearestPoint;
	bool isFound = false;

	XVal = XValue;
	YVal = YValue;
	size_t size = m_vPoints.size();
	int iIndex = 0;

	if(!m_isXYInverted)
	{
		for(int i=0; i<size;i++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
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
		for(int i=0; i<size;i++)
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

CPoint CChartLineSerie::GetNearestRightPoint(double& XValue, double& YValue)
{
	double XVal;
	double YVal;
	double X1p;
	double X2p;
	double Y1p;
	double Y2p;
	double X1, X2;
	double Y1, Y2;
	double nIncrement = 0;
	int iRes = 100;
	CPoint NearestPoint;
	bool isFound = false;

	XVal = XValue;
	YVal = YValue;
	size_t size = m_vPoints.size();
	int iIndex = 0;

	if(!m_isXYInverted)
	{
		for(int i=0; i<size;i++)
		{
			if(m_vPoints[iIndex].X > XVal) break;
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
		for(int i=0; i<size;i++)
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
		//YVal += nIncrement;
		XVal = (YVal-Y1p)*(X2p-X1p)/(Y2p-Y1p) + X1p;
		XValue = XVal;
		YValue = YVal;
	}
	

	ValueToScreen(XVal,YVal,NearestPoint);

	return NearestPoint;
}