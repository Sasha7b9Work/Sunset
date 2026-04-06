#include "StdAfx.h"
#include ".\chartlegend.h"
#include "ChartCtrl.h"


CChartLegend::CChartLegend(CChartCtrl* pParent) : CChartObject(pParent)
{
	m_Color = RGB(255,255,255);
	m_iFontSize = 100;
	m_strFontName = "Microsoft Sans Serif";
	m_iTopMargin = 2;
	m_iBottomMargin = 2;
	m_iLeftMargin = 4;
	m_iRightMargin = 4;
	m_iVerticalSpace = 2;
	m_iHorizontalSpace = 4;
	m_BackColor = RGB(240,240,240);
	m_iFamiliesCount = 0;
	m_iImageWidth = 12;
}

CChartLegend::~CChartLegend(void)
{
}

void CChartLegend::Draw(CDC* pDC)
{
	if(!IsVisible())
		return;

	CBrush BrushBack;
	BrushBack.CreateSolidBrush(m_BackColor);
	pDC->FillRect(m_ObjectRect,&BrushBack);
	
	CFont* pOldFont;
	CFont font;
	font.CreatePointFont(m_iFontSize,(LPCTSTR)m_strFontName.c_str(),pDC);
	CSize szText;
	pOldFont = pDC->SelectObject(&font);
	std::string sText;

	int iYText = m_ObjectRect.top + m_iTopMargin;
	int iXText = m_ObjectRect.left + m_iLeftMargin + m_iImageWidth + m_iHorizontalSpace;

	CPen *pen;
	CPen *pOldPen;
	

	for(int i=0;i<m_iFamiliesCount;i++)
	{
		CChartSerieFamily* pFamily = m_pParent->GetFamily(i);
		
		sText = pFamily->GetName();
//		pDC->ExtTextOut(iXText,iYText,ETO_CLIPPED,NULL,sText.c_str(),NULL);
		pDC->ExtTextOut(iXText,iYText,ETO_OPAQUE,NULL,sText.c_str(),NULL);
		szText = pDC->GetTextExtent(sText.c_str());

		pen = new CPen(pFamily->GetPenStyle(),pFamily->GetPenWidth(),pFamily->GetColor());
		pOldPen = pDC->SelectObject(pen);

		pDC->MoveTo(m_ObjectRect.left + m_iLeftMargin, iYText + szText.cy/2);
		pDC->LineTo(m_ObjectRect.left + m_iLeftMargin + m_iImageWidth, iYText + szText.cy/2);
		pDC->SelectObject(pOldPen);
		
		delete pen;
		
		iYText += m_iVerticalSpace;
		iYText += szText.cy;
	}

	DrawRect(pDC);

	pDC->SelectObject(pOldFont);

}

CSize CChartLegend::GetSize(CDC* pDC)
{
	CSize size(0,0);

	if(!IsVisible())
		return size;

	CFont* pOldFont;
	CFont font;
	font.CreatePointFont(m_iFontSize,(LPCTSTR)m_strFontName.c_str(),pDC);
	pOldFont = pDC->SelectObject(&font);
	
	
	CSize szTemp;
	std::string sName;

	m_iFamiliesCount = m_pParent->GetFamiliesCount();

	for(int i=0;i<m_iFamiliesCount;i++)
	{
		CChartSerieFamily* pFamily = m_pParent->GetFamily(i);
		sName = pFamily->GetName();
		if(sName.size() == 0)
			continue;
		szTemp = pDC->GetTextExtent(sName.c_str());

		if(szTemp.cx>size.cx)
			size.cx = szTemp.cx;

		size.cy += szTemp.cy;
		size.cy += m_iVerticalSpace;
	}

	size.cy -= m_iVerticalSpace;

	size.cy += m_iTopMargin;
	size.cy += m_iBottomMargin;
	size.cx += m_iLeftMargin;
	size.cx += m_iRightMargin;
	size.cx += m_iImageWidth;
	size.cx += m_iHorizontalSpace;

	pDC->SelectObject(pOldFont);

	return size;
}

void CChartLegend::SetPosition(int LeftBorder, int TopBorder, CDC* pDC)
{
	CSize szLegend = GetSize(pDC);
	CRect NewRect;
	NewRect.top = TopBorder;
	NewRect.left = LeftBorder;
	NewRect.bottom = TopBorder + szLegend.cy;
	NewRect.right = LeftBorder + szLegend.cx;

	CChartObject::SetRect(NewRect);
}
