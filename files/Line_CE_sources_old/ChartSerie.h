#pragma once

#include <vector>
#include <string>

#include "ChartObject.h"
#include "ChartAxis.h"


class CChartSerie: public CChartObject
{
	friend CChartCtrl;
public:
	CChartSerie(CChartCtrl* pParent, int Type);
	~CChartSerie(void);

	enum 
	{
		stPoints = 0,
		stLine = 1,
		stMixed = 2
	};

protected:
	struct SChartPoint
	{
		double X;
		double Y;
	};

	std::vector<SChartPoint> m_vPoints;
	std::string m_strSerieName;			//Name displayed in legend

	int m_iSerieType;					//Type of the serie (line, point, mixed)
	COLORREF m_Color;
	CChartAxis* m_pVerticalAxis;
	CChartAxis* m_pHorizontalAxis;
	int m_iPenWidth;
	int m_iPenStyle;
	void SortPoints(void);
	bool m_isSortPoints;
	bool m_isXYInverted;
	int m_iFamilyPointsCount;

public:
	void AddPoint(double X, double Y);
	void ClearSerie(void);
	void SetPoints(double* X, double* Y, int Count);
	double GetXPointValue(int PointIndex);
	double GetYPointValue(int PointIndex);
	void SetXPointValue(int PointIndex, double NewValue);
	void SetYPointValue(int PointIndex, double NewValue);
	void ValueToScreen(double XValue, double YValue, CPoint& ScreenPoint);
	virtual void Draw(CDC* pDC) =0;
	virtual void DrawAll(CDC *pDC) =0;
	bool GetXMinMax(double& Min, double& Max);
	bool GetYMinMax(double& Min, double& Max);
	std::string GetName(void);
	void SetName(std::string Name);
	void SetColor(COLORREF Color);
	int GetPenWidth();
	int GetPenStyle();
	void SetPenWidth(int iPenWidth);
	void SetPenStyle(int iPenStyle);
	COLORREF GetColor(void);

	void DumpPointsToTrace();
	void ScreenToValue(double& XValue, double& YValue, CPoint ScreenPoint);
	
	virtual CPoint GetNearestPoint(CPoint ScreenPoint, double& XValue, double& YValue) = 0;
	virtual CPoint GetNearestLeftPoint(double& XValue, double& YValue) = 0;
	virtual CPoint GetNearestRightPoint(double& XValue, double& YValue) = 0;
	
	void GetMidPoint(double& XValue, double& YValue);
	
	void SetSortPoints(bool isSortPoints);
	void SetXYInverted(bool isInverted);

	double CChartSerie::GetYByX(double X);
	double CChartSerie::GetXByY(double Y);

};

