#include "StdAfx.h"
#include ".\chartaxislabel.h"

CChartAxisLabel::CChartAxisLabel(CChartCtrl* pParent, bool isHorizontal):CChartObject(pParent)
{
	m_bIsHorizontal = isHorizontal;
	
	m_iFontSize = 100;
	m_strFontName = "Microsoft Sans Serif";
	m_strLabelText = "";

	m_iLeftMargin = 2;
	m_iRightMargin = 2;
	m_iTopMargin = 2;
	m_iBottomMargin = 2;
	m_TextColor = RGB(0,0,0);
}

CChartAxisLabel::~CChartAxisLabel(void)
{
}

void CChartAxisLabel::Draw(CDC* pDC)
{

	if(m_strLabelText == "")
		return;
	if(!pDC->GetSafeHdc())
		return;
	if(!IsVisible())
		return;

	CFont NewFont;
	CFont *pOldFont;


//VERIFY(	NewFont.CreatePointFont(m_iFontSize,(LPCTSTR)m_strFontName,pDC));
	NewFont.CreatePointFont(m_iFontSize,(LPCTSTR)m_strFontName,pDC);
	

	if(m_bIsHorizontal)
	{
		pOldFont = pDC->SelectObject(&NewFont);
		
		COLORREF oldColor = pDC->SetTextColor(m_TextColor);
		
		pDC->ExtTextOut(m_ObjectRect.left + m_iLeftMargin,m_ObjectRect.top + m_iTopMargin,
//					ETO_CLIPPED,NULL,m_strLabelText,NULL);
					ETO_OPAQUE,NULL,m_strLabelText,NULL);
		pDC->SetTextColor(oldColor);

	}
	else
	{
		LOGFONT* pLogFont = new LOGFONT;
		NewFont.GetLogFont(pLogFont);
		pLogFont->lfOrientation = 900;
		pLogFont->lfEscapement = 900;

		CFont VertFont;
		VertFont.CreateFontIndirect(pLogFont);
		pOldFont = pDC->SelectObject(&VertFont);
		delete pLogFont;

		COLORREF oldColor = pDC->SetTextColor(m_TextColor);

		pDC->ExtTextOut(m_ObjectRect.left + m_iLeftMargin ,m_ObjectRect.top + m_iTopMargin, 
//					ETO_CLIPPED,NULL,m_strLabelText,NULL);
					ETO_OPAQUE,NULL,m_strLabelText,NULL);

		pDC->SetTextColor(oldColor);

	}

	pDC->SelectObject(pOldFont);

}

void CChartAxisLabel::SetPosition(int Left, int Top, CDC* pDC)
{
	CSize size = GetSize(pDC);
	CRect rc;
	rc.top = Top;
	rc.left = Left;
	rc.bottom = Top + size.cy;
	rc.right = Left + size.cx;

	CChartObject::SetRect(rc);
}


CSize CChartAxisLabel::GetSize(CDC* pDC)
{
	CSize LabelSize;
	LabelSize.cx = LabelSize.cy = 0;

	if(!IsVisible())
		return LabelSize;

	CFont NewFont;
	CFont *pOldFont;

	NewFont.CreatePointFont(m_iFontSize,(LPCTSTR)m_strFontName,pDC);

	pOldFont = pDC->SelectObject(&NewFont);

	LabelSize = pDC->GetTextExtent(m_strLabelText);
	
	LabelSize.cx += m_iLeftMargin;
	LabelSize.cx += m_iRightMargin;
	LabelSize.cy += m_iTopMargin;
	LabelSize.cy += m_iBottomMargin;


	if (!m_bIsHorizontal)
	{
		int Width = LabelSize.cy;
		int Height = LabelSize.cx;
		LabelSize.cx = Width;
		LabelSize.cy = Height;
	}
	pDC->SelectObject(pOldFont);
	

	return LabelSize;
}

void CChartAxisLabel::SetColor(COLORREF Color)
{
	m_TextColor = Color;
}