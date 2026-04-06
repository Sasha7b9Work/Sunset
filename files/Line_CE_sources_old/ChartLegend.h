#pragma once
#include "chartobject.h"
#include <string>

class CChartLegend : public CChartObject
{
public:
	CChartLegend(CChartCtrl* pParent);
	~CChartLegend(void);

	void Draw(CDC* pDC);
	CSize GetSize(CDC* pDC);

private:
	std::string m_strFontName;
	int m_iFontSize;
	COLORREF m_Color;
	int m_iTopMargin;
	int m_iBottomMargin;
	int m_iLeftMargin;
	int m_iRightMargin;
	int m_iVerticalSpace;
	int m_iHorizontalSpace;
	COLORREF m_BackColor;
	int m_iFamiliesCount;
	int m_iImageWidth;
public:
	void SetPosition(int LeftBorder, int TopBorder, CDC* pDC);
	void SetFont(int iPointSize, std::string strFaceName)
	{
		m_strFontName = strFaceName;
		m_iFontSize = iPointSize;
	}
	void SetColor(COLORREF Color)
	{
		m_BackColor = Color;
	}
};
