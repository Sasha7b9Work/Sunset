#include "StdAfx.h"
#include ".\chartserie.h"
#include "ChartCtrl.h"
#include "math.h"

CChartSerie::CChartSerie(CChartCtrl* pParent, int Type) : CChartObject(pParent)

{
	m_pParent = pParent;
	m_iSerieType = Type;
	m_strSerieName = "";
	m_pVerticalAxis = m_pParent->GetLeftAxis();
	m_pHorizontalAxis = m_pParent->GetBottomAxis();
	m_pVerticalAxis->RegisterSeries(this);
	m_pHorizontalAxis->RegisterSeries(this);
	m_Color = RGB(0,0,0);
	m_iPenWidth = 0;
	m_iPenStyle = PS_SOLID;
	m_isSortPoints = true;
	m_isXYInverted = false;
	m_iFamilyPointsCount = 0;
}

CChartSerie::~CChartSerie(void)
{
	m_pVerticalAxis->UnregisterSeries(this);
	m_pHorizontalAxis->UnregisterSeries(this);
}


void CChartSerie::AddPoint(double X, double Y)
{
	SChartPoint NewPoint;
	NewPoint.X = X;
	NewPoint.Y = Y;
	m_vPoints.push_back(NewPoint);
	
	if(m_isSortPoints)
		SortPoints();

	m_pParent->RefreshCtrl();


}

void CChartSerie::ClearSerie(void)
{
	m_vPoints.clear();
	m_pParent->RefreshCtrl();
}

void CChartSerie::SetPoints(double* X, double* Y, int Count)
{
	m_vPoints.clear();
	for(int i=0;i<Count;i++)
	{
		SChartPoint NewPoint;
		NewPoint.X = X[i];
		NewPoint.Y = Y[i];
		m_vPoints.push_back(NewPoint);
	}

	//Do not refresh parent so if more series are added, we refresh just once and gain some
	// speed
//	m_pParent->RefreshCtrl();

	if(m_isSortPoints)
		SortPoints();
}

double CChartSerie::GetXPointValue(int PointIndex)
{
	if( (PointIndex >= (int)m_vPoints.size()) || PointIndex < 0)
		return 0;
	return m_vPoints[PointIndex].X;
}

double CChartSerie::GetYPointValue(int PointIndex)
{
	if( (PointIndex >= (int)m_vPoints.size()) || PointIndex < 0)
		return 0;
	return m_vPoints[PointIndex].Y;
}



void CChartSerie::SetXPointValue(int PointIndex, double NewValue)
{
	if( (PointIndex >= (int)m_vPoints.size()) || PointIndex < 0)
		return;

	m_vPoints[PointIndex].X = NewValue;

	m_pParent->RefreshCtrl();
}

void CChartSerie::SetYPointValue(int PointIndex, double NewValue)
{
	if( (PointIndex >= (int)m_vPoints.size()) || PointIndex < 0)
		return;

	m_vPoints[PointIndex].Y = NewValue;

	m_pParent->RefreshCtrl();
}

void CChartSerie::ValueToScreen(double XValue, double YValue, CPoint& ScreenPoint)
{
	ScreenPoint.x = m_pHorizontalAxis->ValueToScreen(XValue);
	ScreenPoint.y = m_pVerticalAxis->ValueToScreen(YValue);
}
void CChartSerie::ScreenToValue(double& XValue, double& YValue, CPoint ScreenPoint)
{
	XValue = m_pHorizontalAxis->ScreenToValue(ScreenPoint.x);
	YValue = m_pVerticalAxis->ScreenToValue(ScreenPoint.y);
}

bool CChartSerie::GetXMinMax(double& Min, double& Max)
{
	if(m_vPoints.size() == 0)
		return false;
	Min = m_vPoints[0].X;
	Max = m_vPoints[0].X;
	for(int i=0;i<m_vPoints.size();i++)
	{
		if(m_vPoints[i].X > Max) Max = m_vPoints[i].X;
		if(m_vPoints[i].X < Min) Min = m_vPoints[i].X;
	}

	return true;
}

bool CChartSerie::GetYMinMax(double& Min, double& Max)
{
	if(m_vPoints.size() == 0)
		return false;
	Min = m_vPoints[0].Y;
	Max = m_vPoints[0].Y;
	for(int i=0;i<m_vPoints.size();i++)
	{
		if(m_vPoints[i].Y > Max) Max = m_vPoints[i].Y;
		if(m_vPoints[i].Y < Min) Min = m_vPoints[i].Y;
	}

	return true;
}


std::string CChartSerie::GetName(void)
{
	return m_strSerieName;
}

void CChartSerie::SetName(std::string Name)
{
	m_strSerieName = Name;
}

void CChartSerie::SetColor(COLORREF Color)
{
	m_Color = Color;
}

COLORREF CChartSerie::GetColor(void)
{
	return m_Color;
}

int CChartSerie::GetPenWidth()
{
	return m_iPenWidth;
}
int CChartSerie::GetPenStyle()
{
	return m_iPenStyle;
}
void CChartSerie::SetPenWidth(int iPenWidth)
{
	m_iPenWidth = iPenWidth;
}
void CChartSerie::SetPenStyle(int iPenStyle)
{
	m_iPenStyle = iPenStyle;
}
void CChartSerie::DumpPointsToTrace()
{
	for(int i=0; i<m_vPoints.size();i++)
	{
		TRACE(L"%f, %f\n",m_vPoints[i].X,m_vPoints[i].Y);
	}
}




void CChartSerie::GetMidPoint(double& XValue, double& YValue)
{
	if(m_vPoints.size() == 0)
	{
		XValue = 0;
		YValue = 0;
		return;
	}

	XValue = m_vPoints[m_vPoints.size()/2].X;
	YValue = m_vPoints[m_vPoints.size()/2].Y;
}

void CChartSerie::SortPoints(void)
{
	if(!m_isXYInverted)
	{
		SChartPoint pTemp;
		bool isSorted = false;
		while(!isSorted)
		{
			isSorted = true;
			for(int i = 1;i<m_vPoints.size();i++)
			{
				if(m_vPoints[i-1].X > m_vPoints[i].X)
				{
					isSorted = false;
					pTemp  = m_vPoints[i];
					m_vPoints[i] = m_vPoints[i-1];
					m_vPoints[i-1] = pTemp;
				}
			}
		}
	}
	else
	{
		SChartPoint pTemp;
		bool isSorted = false;
		while(!isSorted)
		{
			isSorted = true;
			for(int i = 1;i<m_vPoints.size();i++)
			{
				if(m_vPoints[i-1].Y > m_vPoints[i].Y)
				{
					isSorted = false;
					pTemp  = m_vPoints[i];
					m_vPoints[i] = m_vPoints[i-1];
					m_vPoints[i-1] = pTemp;
				}
			}
		}
	}


}


void CChartSerie::SetSortPoints(bool isSortPoints)
{
	m_isSortPoints = isSortPoints;
}

void CChartSerie::SetXYInverted(bool isInverted)
{
	m_isXYInverted = isInverted;

	std::vector<SChartPoint> vInverted;
	int size = m_vPoints.size();
	for(int i = 0; i < size; i++)
	{
		SChartPoint point;
		point.Y = m_vPoints[i].X;
		point.X = m_vPoints[i].Y;

		vInverted.push_back(point);
	}
	m_vPoints.swap(vInverted);
	SortPoints();

}

double CChartSerie::GetYByX(double X)
{
	double dPrecision = 0.0001;
	int size = m_vPoints.size();
	
	for(int i = 0; i < size; i++)
	{
		if( fabs(m_vPoints[i].X - X) < dPrecision)
		{
			return m_vPoints[i].Y;
		}
	}
	
	return NULL;
}

double CChartSerie::GetXByY(double Y)
{
	double dPrecision = 0.0001;
	int size = m_vPoints.size();
	
	for(int i = 0; i < size; i++)
	{
		if( fabs(m_vPoints[i].Y - Y) < dPrecision)
		{
			return m_vPoints[i].X;
		}
	}
	
	return NULL;
}
