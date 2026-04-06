#pragma once

class CChartCtrl;

class CChartObject
{
	friend CChartCtrl;
public:
	CChartObject(CChartCtrl *pParent)
	{
		m_pParent = pParent;
		m_ObjectRect.left = m_ObjectRect.right = 0;
		m_ObjectRect.bottom = m_ObjectRect.top = 0;
		m_isVisible = true;
	}

	~CChartObject(void){};
	void SetRect(CRect NewRect) {m_ObjectRect = NewRect;}
	CRect GetRect() {return m_ObjectRect;}
	void DrawRect(CDC* pDC)
	{
		pDC->MoveTo(m_ObjectRect.left,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.bottom);
		pDC->LineTo(m_ObjectRect.left,m_ObjectRect.bottom);
		pDC->LineTo(m_ObjectRect.left,m_ObjectRect.top);
	}
	bool IsVisible(){return m_isVisible;}

	void TracePenColor(CString text, CPen* pen)
	{
		LOGPEN LogPen;
		pen->GetLogPen(&LogPen);
		TRACE(text + " %d\n",LogPen.lopnColor);
	}
	
protected:
	CChartCtrl* m_pParent;
	CRect m_ObjectRect;
	bool m_isVisible;
};
