#pragma once
#include "ChartObject.h"
#include "afxwin.h"

class CChartAxis;

class CChartGrid :
	public CChartObject
{
public:
	CChartGrid(CChartCtrl* pParent, CChartAxis* pAxis, bool isHorizontal);
	CChartGrid();
	~CChartGrid(void);
	void Draw(CDC* pDC);

private:
	bool m_isHorizontal;
	CChartAxis* m_pParentAxis;
	
	COLORREF m_Color;

public:
	int m_iTicksCount;
	void SetTicksCount(int iTicksCount);
	void SetColor(COLORREF Color);
};
