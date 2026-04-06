// ChartSerieFamily.h: interface for the CChartSerieFamily class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTSERIEFAMILY_H__9CC63FDF_C9EE_484C_B173_EAA94E08F95A__INCLUDED_)
#define AFX_CHARTSERIEFAMILY_H__9CC63FDF_C9EE_484C_B173_EAA94E08F95A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CChartSerieFamily  
{
	COLORREF m_Color;
	int m_iPenWidth;
	int m_iPenStyle;
	std::string sName;

public:
	CChartSerieFamily();
	virtual ~CChartSerieFamily();

	
	std::string GetName(){return sName;}
	void SetName(std::string Name){sName = Name;}
	COLORREF GetColor(void);
	int GetPenWidth();
	int GetPenStyle();
	void SetColor(COLORREF Color);
	void SetPenWidth(int iPenWidth);
	void SetPenStyle(int iPenStyle);

};

#endif // !defined(AFX_CHARTSERIEFAMILY_H__9CC63FDF_C9EE_484C_B173_EAA94E08F95A__INCLUDED_)



