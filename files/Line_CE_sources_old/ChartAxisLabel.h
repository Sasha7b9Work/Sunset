#pragma once
#include "chartobject.h"
#include <string>

class CChartAxisLabel :
	public CChartObject
{
public:
	CChartAxisLabel(CChartCtrl* pParent, bool isHorizontal);
	~CChartAxisLabel(void);
	bool m_bIsHorizontal;
	void SetPosition(int Left, int Top, CDC* pDC);
	void SetFont(int iPointSize, CString strFaceName)
	{
		m_strFontName = strFaceName;
		m_iFontSize = iPointSize;
	}
	void SetText(CString Text){ m_strLabelText = Text;}
	void Draw(CDC* pDC);
	CString GetText() {return m_strLabelText;}
	CSize GetSize(CDC *pDC);
	void SetColor(COLORREF Color);

protected:
	COLORREF m_TextColor;
	int m_iFontSize;
	int m_iLeftMargin;
	int m_iRightMargin;
	int m_iTopMargin;
	int m_iBottomMargin;
	CString m_strFontName;
	CString m_strLabelText;
};
	