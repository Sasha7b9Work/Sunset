#pragma once
#include "chartobject.h"

class CChartMarker : public CChartObject
{
private:
	int m_MarkerHeight;
	int m_MarkerHalfBorder;
	int m_iPenSize;
	int m_iPenStyle;
	COLORREF m_Color;
	COLORREF m_FillColor;
	double X;
	double Y;
	CRect m_WorkingRect;

public:
	CChartMarker(CChartCtrl* pParent);
	~CChartMarker(void);
	void SetPosition(double XValue, double YValue);
	void Draw(CDC* pDC);
	int m_iRelatedSerieIndex;
	void SetRelatedSerie(int SerieIndex);
	int GetRelatedSerieIndex(void);
	void GetValues(double& XValue, double& YValue);
	void SetMarkerHeight(int Height)
	{
		m_MarkerHeight = Height;
		m_MarkerHalfBorder = 0.6 * m_MarkerHeight;
	}
	void SetColor(COLORREF Color);
	void SetFillColor(COLORREF Color);
	void SetWorkingRect(CRect rect)
	{
		m_WorkingRect = rect;
	}
};
