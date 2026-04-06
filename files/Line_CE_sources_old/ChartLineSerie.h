#pragma once
#include "chartserie.h"

class CChartLineSerie :
	public CChartSerie
{
public:
	CChartLineSerie(CChartCtrl* pParent);
	~CChartLineSerie(void);
	void DrawAll(CDC* pDC);
	void Draw(CDC* pDC);
	CPoint GetNearestPoint(CPoint ScreenPoint, double& XValue, double& YValue);
	CPoint GetNearestLeftPoint(double& XValue, double& YValue);
	CPoint GetNearestRightPoint(double& XValue, double& YValue);
};
