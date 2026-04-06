#pragma once
#include "ChartObject.h"
#include <string>
#include <list>

class CChartGrid;
class CChartAxisLabel;
class CChartSerie;


class CChartAxis :
	public CChartObject
{
public:
	CChartAxis(CChartCtrl* pParent,bool bHorizontal);
	~CChartAxis(void);


private:

	double m_MaxValue;		// Maximum value on the axis
	double m_MinValue;		
	double m_UnzoomMin;		// Min and max values of the axis before it has been zoomed
	double m_UnzoomMax;		// (used when we unzoom the chart -> go back to previous state)
	
	int m_StartPos;			    // Start position of the axis 
	int m_EndPos;
	COLORREF m_Color;
	COLORREF m_TextColor;

	int  m_nFontSize;			
	bool m_isDrawMaxTick;
	int m_iTickCount;			//number of tick between Min & Max if m_isDrawMaxTick then MaxValue tick is not included
	double m_nSmallTickIncrement;
	int m_iSmallTickCenterNumber;
	int m_iTickLengthBig;
	int m_iTickLengthSmall;
	int m_iTickLengthMedium;
	int m_iSpaceTickText;
	int m_iAxisSize;
	bool m_isAutomatic;
	bool m_isBoxed;
	bool m_isLogarithmic;
	CRect m_MarginRect;
	CSize m_szTextSize;

	std::string m_strFontName;
	CChartGrid* m_pGrid;
	CChartAxisLabel* m_pAxisLabel;
	

public:
	long ValueToScreen(double Value);
	double ScreenToValue(long ScreenValue);
	bool m_isHorizontal;
	bool m_isInverted;
	int GetAxisLength(void);
	unsigned int m_DecCount;
	unsigned int m_InitialDecCount;
	double m_nBigTickIncrement;
	int m_iSmallTickCount;

	bool SetAxisSize(CRect ClientRect, CRect MarginRect);
	void ClipMargin(CRect& MarginRect, CDC* pDC);
	void Draw(CDC* pDC);
	void SetFont(int iFontSize, std::string strFontName)
	{
		m_nFontSize = iFontSize;
		m_strFontName = strFontName;
	}
	
	void CalculateTickValues(void);
	void CountTicks(void);
	long GetTickPosition(int TickIndex);
	void RegisterSeries(CChartSerie* pSerie);
	void UnregisterSeries(CChartSerie* pSerie);

	typedef std::list<CChartSerie*> SeriesList;
	SeriesList m_pRelatedSeries;		// List containing pointers to series related to this axis	

	void CalculateAutoAxis(void);
	void RefreshAutoAxis(void);
	void SetInverted(bool isInverted);
	void SetBoxed();
	void SetCrossed();
	void DrawCrossed(CDC* pDC);

	void SetAutomatic(bool isAutomatic);
	int GetZeroTickPosition(void);

	
	CSize GetTextSize(CDC* pDC);
	void SetMinMax(double Min, double Max);
	void SetZoomMinMax(double Min, double Max);
	void SetLogarithmic(bool isLogarithmic);
	void GetMinMaxTickIncrement(double& Min, double& Max, double& TickIncrement);
	void SetBigTickIncrement(double Increment);
	void SetLabelText(CString Text);
	void SetLabelFont(int iPointSize, CString strFaceName);
	void GetMinMax(double& Min, double& Max);
	void SetGridColor(COLORREF Color);
	void SetTextColor(COLORREF Color);
	void SetLabelTextColor(COLORREF Color);

	double RoundDouble(double doValue, int nPrecision);
	void SetSmallTickCount(int TickCount);
};
