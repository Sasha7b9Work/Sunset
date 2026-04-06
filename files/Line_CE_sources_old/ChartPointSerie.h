#pragma once
#include "chartobject.h"
#include "ChartSerie.h"

class CChartPointSerie : public CChartSerie
{
public:
	enum PointType
	{
		ptEllipse=0,
		ptRectangle=1,
		ptTriangle=2
	};

	CChartPointSerie(CChartCtrl* pParent);
	~CChartPointSerie(void);
	void DrawAll(CDC* pDC);
	void Draw(CDC* pDC);
	int GetPointSize();
	void SetPointSize(int iPointSize);
	void SetPointType(PointType Type);
	int m_iPointSize;
	PointType m_iPointType;
	//CPoint GetNearestPoint(CPoint ScreenPoint);
	CPoint GetNearestPoint(CPoint ScreenPoint, double& XValue, double &YValue);
	CPoint GetNearestLeftPoint(double& XValue, double& YValue);
	CPoint GetNearestRightPoint(double& XValue, double& YValue);
};

