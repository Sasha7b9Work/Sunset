#pragma once
#include "chartobject.h"
#include "ChartAxis.h"

class CChartSecant : public CChartObject
{
private:
	double X1,Y1;
	double X2,Y2;
	CChartAxis* m_pHorizontalAxis;
	CChartAxis* m_pVerticalAxis;
	
	COLORREF m_Color;
	int m_iPenWidth;
	int m_iPenStyle;

	void ValueToScreen(double XValue, double YValue, CPoint& ScreenPoint);
	void ScreenToValue(double& XValue, double& YValue, CPoint ScreenPoint);

public:
	CChartSecant(CChartCtrl* pParent);
	~CChartSecant(void);
	void SetPoints(double X, double Y, int Index);
	void SetPoints(double X, double Y);
	void Draw(CDC* pDC);
	void SetColor(COLORREF Color)
	{
		m_Color = Color;
	}
	void SetPenWidth(int Width)
	{
		m_iPenWidth = Width;
	}
	void SetPenStyle(int PenStyle)
	{
		m_iPenStyle = PenStyle;
	}
	void GetIntersection(double& X1, double& Y1, double& X2, double& Y2);
};
