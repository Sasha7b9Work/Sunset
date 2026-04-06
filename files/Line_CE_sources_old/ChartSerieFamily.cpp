// ChartSerieFamily.cpp: implementation of the CChartSerieFamily class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "chart.h"
#include "ChartSerieFamily.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartSerieFamily::CChartSerieFamily()
{

	m_Color = RGB(0,0,0);
	m_iPenWidth = 0;
	m_iPenStyle = PS_SOLID;
}

CChartSerieFamily::~CChartSerieFamily()
{

}
COLORREF CChartSerieFamily::GetColor(void)
{
	return m_Color;
}

int CChartSerieFamily::GetPenWidth()
{
	return m_iPenWidth;
}
int CChartSerieFamily::GetPenStyle()
{
	return m_iPenStyle;
}

void CChartSerieFamily::SetColor(COLORREF Color)
{
	m_Color = Color;
}

void CChartSerieFamily::SetPenWidth(int iPenWidth)
{
	m_iPenWidth = iPenWidth;
}
void CChartSerieFamily::SetPenStyle(int iPenStyle)
{
	m_iPenStyle = iPenStyle;
}