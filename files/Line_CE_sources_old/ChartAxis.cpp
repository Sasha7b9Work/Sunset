#include "StdAfx.h"
#include "ChartAxis.h"
#include "ChartAxisLabel.h"
#include "ChartGrid.h"
#include "ChartCtrl.h"

#include "math.h"
#include ".\chartaxis.h"



CChartAxis::CChartAxis(CChartCtrl* pParent, bool bHorizontal) : CChartObject(pParent)
{

	m_MinValue = m_UnzoomMin = 0.1;
	m_MaxValue = m_UnzoomMax = 10;
	m_StartPos = m_EndPos = 0;

	m_InitialDecCount = 1;
	m_DecCount = m_InitialDecCount;

	m_nBigTickIncrement = 1;

	m_iTickLengthBig = 8;
	m_iTickLengthSmall = 4;
	m_iTickLengthMedium = 6;
    m_iSpaceTickText = 2;
	m_iSmallTickCount = 4;//3;

	m_isAutomatic = false;
	m_isInverted = false;
	m_isLogarithmic = false;
	
	m_isHorizontal = bHorizontal;
	m_Color = RGB(0,0,0);
	m_TextColor = RGB(0,0,0);

	m_nFontSize = 100;//
	m_strFontName = "Microsoft Sans Serif";
	m_isDrawMaxTick = false;

	m_iAxisSize = 0;
	m_szTextSize.cx = 0;
	m_szTextSize.cy = 0;
	m_isBoxed = true;

	CountTicks();
	
	
	m_pGrid = new CChartGrid(m_pParent,this,m_isHorizontal);
	m_pAxisLabel = new CChartAxisLabel(m_pParent, m_isHorizontal);
	m_pAxisLabel->SetText("Label");

	
}

CChartAxis::~CChartAxis(void)
{
	delete m_pGrid;
	delete m_pAxisLabel;
}



long CChartAxis::ValueToScreen(double Value)
{
	long Offset = 0;
	
	if (m_isLogarithmic)
        Offset = (int)floor((log10(Value)-log10(m_MinValue)) * GetAxisLength()/(log10(m_MaxValue)-log10(m_MinValue)) );
	else
		Offset = (int)floor( (Value - m_MinValue)*GetAxisLength()/(m_MaxValue - m_MinValue) );

	if(m_isHorizontal)
	{
		if(!m_isInverted)
			return m_StartPos + Offset;
		else
			return m_EndPos - Offset;
	}
	else
	{
		if(!m_isInverted)
			return m_StartPos - Offset;
		else
			return m_EndPos + Offset;
	}

	
}
double CChartAxis::ScreenToValue(long ScreenValue)
{
	long Offset = 0;
	if(m_isHorizontal)
	{
		if(!m_isInverted)
			Offset = ScreenValue - m_StartPos;
		else
			Offset = m_EndPos - ScreenValue;

		if(m_isLogarithmic)
			return 	pow(10.0,(Offset *1.0 / GetAxisLength()*(log10(m_MaxValue)-log10(m_MinValue)) ) + log10(m_MinValue)) ;
		else
			return (Offset*(m_MaxValue - m_MinValue)/GetAxisLength() + m_MinValue);
	}
	else
	{
		if(!m_isInverted)
			Offset = m_StartPos - ScreenValue;
		else
			Offset = ScreenValue - m_EndPos;
		return (Offset*(m_MaxValue - m_MinValue)/GetAxisLength() + m_MinValue);
	}

}

int CChartAxis::GetAxisLength(void)
{
	int Length = (int)fabs( (m_EndPos-m_StartPos) * 1.0);
    return Length;
}


bool CChartAxis::SetAxisSize(CRect ClientRect, CRect MarginRect)
{
	if(m_isBoxed)
	{
		if(m_isHorizontal)
		{
			m_StartPos = MarginRect.left;
			m_EndPos = MarginRect.right;
			CRect AxisRect = MarginRect;
			AxisRect.top = MarginRect.bottom;
			AxisRect.bottom = ClientRect.bottom;
			SetRect(AxisRect);
		}
		else
		{
			m_StartPos = MarginRect.bottom;
			m_EndPos = MarginRect.top;
			CRect AxisRect = MarginRect;
			AxisRect.left = ClientRect.left;
			AxisRect.right = MarginRect.left;
			SetRect(AxisRect);
		}
	}
	else
	{
		if(m_isHorizontal)
		{
			m_StartPos = MarginRect.left;
			m_EndPos = MarginRect.right;
			CRect AxisRect = MarginRect;
			int iYZero = m_pParent->m_pAxisLeft->GetZeroTickPosition();
			if( (iYZero + m_iAxisSize) > MarginRect.bottom)
			{
				m_isBoxed = true;
				return false;
			}
			AxisRect.top = iYZero ;//- m_iAxisSize;
			AxisRect.bottom = iYZero + m_iAxisSize;
			AxisRect.left -= m_szTextSize.cx/2;
			AxisRect.right += m_szTextSize.cx/2;

//			AxisRect.top = (MarginRect.bottom - MarginRect.top)/2 - m_iAxisSize;
//			AxisRect.bottom = (MarginRect.bottom - MarginRect.top)/2 + m_iAxisSize;

			SetRect(AxisRect);
		}
		else
		{
			m_StartPos = MarginRect.bottom;
			m_EndPos = MarginRect.top;
			CRect AxisRect = MarginRect;
			
			int iXZero = m_pParent->m_pAxisBottom->GetZeroTickPosition();
			if( (iXZero - m_iAxisSize) < MarginRect.left)
			{
				m_isBoxed = true;
				return false;
			}
			AxisRect.right = iXZero;
			AxisRect.left = iXZero - m_iAxisSize;
			AxisRect.bottom += m_szTextSize.cy/2;
			AxisRect.top -= m_szTextSize.cy/2;
			
			//AxisRect.left = (MarginRect.right - MarginRect.left)/2 - m_iAxisSize;
			//AxisRect.right = (MarginRect.right - MarginRect.left)/2 + m_iAxisSize;

			SetRect(AxisRect);
		}

		m_MarginRect = MarginRect;
	}
	return true;
}

void CChartAxis::ClipMargin(CRect& MarginRect, CDC* pDC)
{
	if(!IsVisible())
		return;

	m_szTextSize = GetTextSize(pDC);
	int iAxisSize = 0;

	CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);

	if(m_isBoxed)
	{
		if(m_isHorizontal)
		{
			iAxisSize += m_iTickLengthBig;
			iAxisSize += m_szTextSize.cy;
			iAxisSize += szAxisLabel.cy;
			iAxisSize += m_iSpaceTickText;

			MarginRect.bottom -= iAxisSize;
			MarginRect.right -= m_szTextSize.cx/2;
		}
		else
		{
			iAxisSize += szAxisLabel.cx;
			iAxisSize += m_szTextSize.cx;
			iAxisSize += m_iTickLengthBig;
			iAxisSize += m_iSpaceTickText; //Text Offset
			MarginRect.left += iAxisSize;
			MarginRect.top += m_szTextSize.cy;
		}
	}
	else
	{
		if(m_isHorizontal)
		{
			CSize TextSize = GetTextSize(pDC);

			iAxisSize += TextSize.cy;
			iAxisSize += m_iSpaceTickText;
			MarginRect.left += TextSize.cx/2;
			MarginRect.right -= TextSize.cx/2;
			m_iAxisSize = iAxisSize;
			MarginRect.bottom -= szAxisLabel.cy;
		}
		else
		{

			iAxisSize += szAxisLabel.cx;
			iAxisSize += m_szTextSize.cx/2;
			iAxisSize += m_iSpaceTickText;
			MarginRect.left += iAxisSize;
			MarginRect.top += m_szTextSize.cy/2;
			MarginRect.bottom -= m_szTextSize.cy/2;

			m_iAxisSize = iAxisSize;
		}
	}
}




/*
void CChartAxis::DrawCrossed(CDC* pDC)
{
	CPen newPen(PS_SOLID,0,m_Color);
	CPen *pOldPen = pDC->SelectObject(&newPen);
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,m_strFontName.c_str(),pDC);
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&NewFont);
	CSize szFontSize;
	char szBuffer[255];
	int iHalfPosition = fabs((m_StartPos + m_EndPos)/2.0);


	if(m_isHorizontal)
	{
		//Draw Axis Line
		pDC->MoveTo(m_ObjectRect.left + m_szTextSize.cx/2,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right - m_szTextSize.cx/2,m_ObjectRect.top);
		
		//Draw Axis Label
		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(iHalfPosition - szAxisLabel.cx/2,m_pParent->m_pAxisLeft->GetRect().bottom ,pDC);
		m_pAxisLabel->Draw(pDC);


		double nNumber = m_MinValue;
		double nSmallNumber;
		int HorizPosition = 0;
		//Min_Value Tick
		//Draw Small Ticks
		nSmallNumber = nNumber;
		for(int i=0; i<m_iSmallTickCount;i++)
		{
			nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
			HorizPosition = ValueToScreen(nSmallNumber);	
			//pDC->MoveTo(HorizPosition,m_ObjectRect.top);
			if(i != m_iSmallTickCenterNumber)
			{
				pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthSmall);
				pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthSmall);
			}
			else
			{
				pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthMedium);
				pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthMedium);
			}
		}

		//Draw Min_Value Text
		HorizPosition = ValueToScreen(nNumber);	
		sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
		szFontSize = pDC->GetTextExtent(szBuffer);
		pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
		nNumber += m_nBigTickIncrement;
		//Draw Tick
		pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthBig);
		pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);

	
		//Draw Ticks & Ticks Text
		for(int i=0; i<m_iTickCount; i++)
		{
			//Draw Tick Text
			HorizPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
			if(nNumber == 0)
				pDC->ExtTextOut(HorizPosition + szFontSize.cx + 2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			else
				pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);

			//Draw Big Tic
			pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthBig);
			pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);

			//Draw Small Ticks
			nSmallNumber = nNumber;
			for(int i=0; i<m_iSmallTickCount;i++)
			{
				nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
				HorizPosition = ValueToScreen(nSmallNumber);	
				
				if(HorizPosition > m_ObjectRect.right) break;

				pDC->MoveTo(HorizPosition,m_ObjectRect.top);
				if(i != m_iSmallTickCenterNumber)
				{
					pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthSmall);
					pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthSmall);
				}
				else
				{
					pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthMedium);
					pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthMedium);
				}
			}
			nNumber += m_nBigTickIncrement;
		}

		//Max Tick
	//	if(m_isDrawMaxTick)
	//	{
			//Draw Max_Value Text
			HorizPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			//DrawTick
			pDC->MoveTo(HorizPosition,m_ObjectRect.top);
			pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);
			
	//	}

		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);
	}
	else
	{
		pDC->MoveTo(m_ObjectRect.right,m_ObjectRect.top + m_szTextSize.cy/2);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.bottom - m_szTextSize.cy/2);

		//Draw Axis Label
		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(m_MarginRect.left - m_iAxisSize, iHalfPosition + szAxisLabel.cy/2,pDC);
		m_pAxisLabel->Draw(pDC);

		
		double nNumber = m_MinValue;
		int VertPosition = 0;
		double nSmallNumber;
		long lTextOffset = 2 + m_iTickLengthBig;

		//Draw Min_Value Tick Text
		VertPosition = ValueToScreen(nNumber);	
		sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
		szFontSize = pDC->GetTextExtent(szBuffer);
		pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx,VertPosition - szFontSize.cy/2 - m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
		//Draw Tick
		pDC->MoveTo(m_ObjectRect.right - m_iTickLengthBig, VertPosition);
		pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig, VertPosition);
		//Draw Ticks
		nSmallNumber = nNumber;
		for(int i=0; i<m_iSmallTickCount;i++)
		{
			nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
			VertPosition = ValueToScreen(nSmallNumber);	
			//pDC->MoveTo(m_ObjectRect.right, VertPosition);
			if(i != m_iSmallTickCenterNumber)
			{
				pDC->MoveTo(m_ObjectRect.right - m_iTickLengthSmall,VertPosition);
				pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall,VertPosition);
			}
			else
			{
				pDC->MoveTo(m_ObjectRect.right - m_iTickLengthMedium,VertPosition);
				pDC->LineTo(m_ObjectRect.right + m_iTickLengthMedium,VertPosition);
			}
		}
		nNumber += m_nBigTickIncrement;

		//Draw Ticks & Ticks Text
		for(int i=0; i<m_iTickCount; i++)
		{
			//Draw Tick Text

			if(nNumber == 0) 
			{
				nSmallNumber = nNumber;
				for(int i=0; i<m_iSmallTickCount;i++)
				{
					nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
					VertPosition = ValueToScreen(nSmallNumber);	
					if(VertPosition < m_ObjectRect.top) break;
					pDC->MoveTo(m_ObjectRect.right, VertPosition);
					if(i != m_iSmallTickCenterNumber)
					{
						pDC->MoveTo(m_ObjectRect.right - m_iTickLengthSmall, VertPosition);
						pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall, VertPosition);
					}
					else
					{
						pDC->MoveTo(m_ObjectRect.right - m_iTickLengthMedium, VertPosition);
						pDC->LineTo(m_ObjectRect.right + m_iTickLengthMedium, VertPosition);
					}
				}

				nNumber += m_nBigTickIncrement;
				continue;
			}

			VertPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);

			//Draw Big Tick
			pDC->MoveTo(m_ObjectRect.right - m_iTickLengthBig,VertPosition);
			pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig,VertPosition);

			//Draw Small Ticks
			nSmallNumber = nNumber;
			for(int i=0; i<m_iSmallTickCount;i++)
			{
				nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
				VertPosition = ValueToScreen(nSmallNumber);	
				if(VertPosition < m_ObjectRect.top) break;
				pDC->MoveTo(m_ObjectRect.right, VertPosition);
				if(i != m_iSmallTickCenterNumber)
				{
					pDC->MoveTo(m_ObjectRect.right - m_iTickLengthSmall, VertPosition);
					pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall, VertPosition);
				}
				else
				{
					pDC->MoveTo(m_ObjectRect.right - m_iTickLengthMedium, VertPosition);
					pDC->LineTo(m_ObjectRect.right + m_iTickLengthMedium, VertPosition);
				}
			}

			nNumber += m_nBigTickIncrement;
		}

		//Draw Max Tick Text
	//	if(m_isDrawMaxTick)
	//	{
			VertPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			//Draw Tick
			pDC->MoveTo(m_ObjectRect.right - m_iTickLengthBig, VertPosition);
			pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig, VertPosition);	
	//	}

		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

}

*/

void CChartAxis::DrawCrossed(CDC *pDC)
{

	CPen newPen(PS_SOLID,0,m_Color);
	CPen *pOldPen = pDC->SelectObject(&newPen);
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,(LPCTSTR)m_strFontName.c_str(),pDC);
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&NewFont);
	CSize szFontSize;
	char szBuffer[255];
	int iHalfPosition = (int)fabs((m_StartPos + m_EndPos)/2.0);

	COLORREF oldTextColor = pDC->SetTextColor(m_TextColor);

	if(m_isHorizontal)
	{
		//Draw Axis Line
		pDC->MoveTo(m_ObjectRect.left + m_szTextSize.cx/2,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right - m_szTextSize.cx/2,m_ObjectRect.top);
		//Draw Axis Label
		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(iHalfPosition - szAxisLabel.cx/2,m_pParent->m_pAxisLeft->GetRect().bottom ,pDC);
		m_pAxisLabel->Draw(pDC);
		//Draw Grid
		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);
		
		double nNumber = m_MinValue;
		int HorizPosition = 0;
		bool isFirst = true;
		double nSmallTickNumber; 

		while(nNumber <= m_MaxValue)
		{
			if(!isFirst)
			{
				nSmallTickNumber = nNumber;
				for(int i=0; i<m_iSmallTickCount;i++)
				{
					nSmallTickNumber = nSmallTickNumber - m_nSmallTickIncrement;
					HorizPosition = ValueToScreen(nSmallTickNumber);
					pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthSmall);
					pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthSmall);
				}
			}
			if(nNumber == 0)
			{
				HorizPosition = ValueToScreen(nNumber);	
				sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
				szFontSize = pDC->GetTextExtent(szBuffer);
//				pDC->ExtTextOut(10 + HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
				pDC->ExtTextOut(10 + HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_OPAQUE,NULL,szBuffer,NULL);
				nNumber += m_nBigTickIncrement;
				isFirst = false;
				continue;
			}

			HorizPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->MoveTo(HorizPosition,m_ObjectRect.top + m_iTickLengthBig);
			pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);
			nNumber += m_nBigTickIncrement;
			isFirst = false;
		}
		if(nNumber - m_nBigTickIncrement != m_MaxValue)
		{
			HorizPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig + m_iSpaceTickText,ETO_OPAQUE,NULL,szBuffer,NULL);
		}
			
		


	}
	//Draw vertical
	else
	{
		//Draw Axis Line
		pDC->MoveTo(m_ObjectRect.right,m_ObjectRect.top + m_szTextSize.cy/2);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.bottom - m_szTextSize.cy/2);

		//Draw Axis Label
		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(m_MarginRect.left - m_iAxisSize, iHalfPosition + szAxisLabel.cy/2,pDC);
		m_pAxisLabel->Draw(pDC);

		//Draw Grid
		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);
		

		double nNumber = m_MinValue;
		int VertPosition = 0;
		double nSmallTickNumber;
		long lTextOffset = 2 + m_iTickLengthBig;
		bool isFirst = true;

		while(nNumber <= m_MaxValue)
		{
			if(!isFirst)
			{
				nSmallTickNumber = nNumber;
				for(int i=0; i<m_iSmallTickCount;i++)
				{
					nSmallTickNumber = nSmallTickNumber - m_nSmallTickIncrement;
					VertPosition = ValueToScreen(nSmallTickNumber);
					pDC->MoveTo(m_ObjectRect.right - m_iTickLengthSmall, VertPosition);
					pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall, VertPosition);
				}
			}
			if(nNumber == 0)
			{
				nNumber += m_nBigTickIncrement;
				continue;
			}

			VertPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - m_iSpaceTickText,ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->MoveTo(m_ObjectRect.right - m_iTickLengthBig, VertPosition);
			pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig, VertPosition);
			nNumber += m_nBigTickIncrement;
			isFirst = false;
		}

		if(nNumber - m_nBigTickIncrement != m_MaxValue)
		{
			VertPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - m_iSpaceTickText,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - m_iSpaceTickText,ETO_OPAQUE,NULL,szBuffer,NULL);
			
		}
	}


	pDC->SetTextColor(oldTextColor);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

}

void CChartAxis::Draw(CDC *pDC)
{
	if(!IsVisible())
		return;
	if(pDC->GetSafeHdc() == NULL)
		return;
	
	if(m_isAutomatic)
		CalculateAutoAxis();

	if(!m_isBoxed)
	{
		DrawCrossed(pDC);
		return;
	}	


	CPen newPen(PS_SOLID,0,m_Color);
	CPen *pOldPen = pDC->SelectObject(&newPen);
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,(LPCTSTR)m_strFontName.c_str(),pDC);
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&NewFont);
	CSize szFontSize;
	char szBuffer[255];
	int iHalfPosition = (int)fabs((m_StartPos + m_EndPos)/2.0);

	COLORREF oldTextColor = pDC->SetTextColor(m_TextColor);

	if(m_isHorizontal)
	{
		//Draw Axis Label
		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(iHalfPosition - szAxisLabel.cx/2,m_ObjectRect.bottom - szAxisLabel.cy,pDC);
		m_pAxisLabel->Draw(pDC);

		//Draw Axis Line
		pDC->MoveTo(m_ObjectRect.left - 5,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.top);



		double nNumber = m_MinValue;
		int HorizPosition = 0;
		bool isFirst = true;
		double nSmallTickNumber; 
		int iTicksCount = -1;

		while(nNumber <= m_MaxValue)
		{
			if(!isFirst)
			{
				if(!m_isLogarithmic)
				{
					nSmallTickNumber = nNumber;
					for(int i=0; i<m_iSmallTickCount;i++)
					{
						nSmallTickNumber = nSmallTickNumber - m_nSmallTickIncrement;
						HorizPosition = ValueToScreen(nSmallTickNumber);
						pDC->MoveTo(HorizPosition,m_ObjectRect.top);
						pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthSmall);
					}
				}
				else
				{
					nSmallTickNumber = nNumber / m_nBigTickIncrement;
					for(int i=1; i<=m_iSmallTickCount;i++)
					{
						HorizPosition = ValueToScreen(nSmallTickNumber + i * m_nSmallTickIncrement * nSmallTickNumber);
						pDC->MoveTo(HorizPosition,m_ObjectRect.top);
						pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthSmall);
					}
				}


			}
			HorizPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,
				m_ObjectRect.top + m_iTickLengthBig,ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->MoveTo(HorizPosition,m_ObjectRect.top);
			pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);
			if(!m_isLogarithmic)
				nNumber = RoundDouble(nNumber + m_nBigTickIncrement,7);
			else
				nNumber *= m_nBigTickIncrement;
			isFirst = false;
			iTicksCount ++;
		}

		if( ( RoundDouble(nNumber - m_nBigTickIncrement,7) != m_MaxValue) && !m_isLogarithmic) 
		{
			HorizPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig,ETO_OPAQUE,NULL,szBuffer,NULL);
		}
		
		//Draw Grid
		m_pGrid->SetTicksCount(iTicksCount);
		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);

		
	}
	//Draw Vertical
	
	else
	{
		//Draw Axis Label
		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(m_ObjectRect.left, iHalfPosition + szAxisLabel.cy/2,pDC);
		m_pAxisLabel->Draw(pDC);

		//Draw Axis Line
		pDC->MoveTo(m_ObjectRect.right,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.bottom + 5);

		
		double nNumber = m_MinValue;
		int VertPosition = 0;
		long lTextOffset = 2 + m_iTickLengthBig;
		bool isFirst = true;
		double nSmallTickNumber; 
		int iTicksCount = -1;

		while(nNumber <= m_MaxValue)
		{
			if(!isFirst)
			{
				if(!m_isLogarithmic)
				{
					nSmallTickNumber = nNumber;
					for(int i=0; i<m_iSmallTickCount;i++)
					{
						nSmallTickNumber = nSmallTickNumber - m_nSmallTickIncrement;
						VertPosition = ValueToScreen(nSmallTickNumber);
						pDC->MoveTo(m_ObjectRect.right, VertPosition);
						pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall, VertPosition);
					}
				}
				else
				{
					nSmallTickNumber = nNumber / m_nBigTickIncrement;
					for(int i=1; i<=m_iSmallTickCount;i++)
					{
						VertPosition = ValueToScreen(nSmallTickNumber + i*m_nSmallTickIncrement * nSmallTickNumber);
						pDC->MoveTo(m_ObjectRect.right, VertPosition);
						pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall, VertPosition);
					}
				}
			}
			VertPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx,VertPosition - szFontSize.cy/2 - 1,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx,VertPosition - szFontSize.cy/2 - 1,ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->MoveTo(m_ObjectRect.right,VertPosition);
			pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig,VertPosition);
			if(!m_isLogarithmic)
				nNumber = RoundDouble(nNumber + m_nBigTickIncrement,7);
			else
				nNumber *= m_nBigTickIncrement;
			isFirst = false;
			iTicksCount ++;
		}
		if( RoundDouble(nNumber - m_nBigTickIncrement,7) != m_MaxValue)
		{
			VertPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
//			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx,VertPosition - szFontSize.cy/2 - 1,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx,VertPosition - szFontSize.cy/2 - 1,ETO_OPAQUE,NULL,szBuffer,NULL);
		}
	
		//DrawGrid
		m_pGrid->SetTicksCount(iTicksCount);
		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);
	}

	
	pDC->SetTextColor(oldTextColor);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
}


/*
void CChartAxis::Draw(CDC* pDC)
{
	if(!IsVisible())
		return;

	if(pDC->GetSafeHdc() == NULL)
		return;

	if(m_isAutomatic)
		CalculateAutoAxis();
	
	if(!m_isBoxed)
	{
		DrawCrossed(pDC);
		return;
	}

	CPen newPen(PS_SOLID,0,m_Color);
	CPen *pOldPen = pDC->SelectObject(&newPen);
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,m_strFontName.c_str(),pDC);
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&NewFont);
	CSize szFontSize;
	char szBuffer[255];
	int iHalfPosition = fabs((m_StartPos + m_EndPos)/2.0);

	if(!m_isLogarithmic)
	{
		if(m_isHorizontal)
	{

		//Draw Axis Label
		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(iHalfPosition - szAxisLabel.cx/2,m_ObjectRect.bottom - szAxisLabel.cy,pDC);
		m_pAxisLabel->Draw(pDC);

		//Draw Axis Line
		pDC->MoveTo(m_ObjectRect.left - 5,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.top);
		

		double nNumber = m_MinValue;
		double nSmallNumber;
		int HorizPosition = 0;
		//Min_Value Tick
		//Draw Small Ticks
		nSmallNumber = nNumber;
		for(int i=0; i<m_iSmallTickCount;i++)
		{
			nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
			HorizPosition = ValueToScreen(nSmallNumber);	
			pDC->MoveTo(HorizPosition,m_ObjectRect.top);
			if(i != m_iSmallTickCenterNumber)
			{pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthSmall);}
			else
			{pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthMedium);}
		}

		//Draw Min_Value Text
		HorizPosition = ValueToScreen(nNumber);	
		sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
		szFontSize = pDC->GetTextExtent(szBuffer);
		pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
		nNumber += m_nBigTickIncrement;
		//Draw Tick
		pDC->MoveTo(HorizPosition,m_ObjectRect.top);
		pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);

	
		//Draw Ticks & Ticks Text
		for(int i=0; i<m_iTickCount; i++)
		{
			//Draw Tick Text
			HorizPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);

			//Draw Big Tic
			pDC->MoveTo(HorizPosition,m_ObjectRect.top);
			pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);

			//Draw Small Ticks
			nSmallNumber = nNumber;
			for(int i=0; i<m_iSmallTickCount;i++)
			{
				nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
				HorizPosition = ValueToScreen(nSmallNumber);	
				
				if(HorizPosition > m_ObjectRect.right) break;

				pDC->MoveTo(HorizPosition,m_ObjectRect.top);
				if(i != m_iSmallTickCenterNumber)
				{
					pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthSmall);
				}
				else
				{
					pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthMedium);
				}
			}
			nNumber += m_nBigTickIncrement;
		}

		//Max Tick
	//	if(m_isDrawMaxTick)
	//	{
			//Draw Max_Value Text
			HorizPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
			pDC->ExtTextOut(HorizPosition - szFontSize.cx/2,m_ObjectRect.top + m_iTickLengthBig,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			//DrawTick
			pDC->MoveTo(HorizPosition,m_ObjectRect.top);
			pDC->LineTo(HorizPosition,m_ObjectRect.top - m_iTickLengthBig);
			
	//	}

		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);

	}
		//Draw Vertical Axis
		else 
	{

		pDC->MoveTo(m_ObjectRect.right,m_ObjectRect.top);
		pDC->LineTo(m_ObjectRect.right,m_ObjectRect.bottom + 5);

		//Draw Axis Label

		CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
		m_pAxisLabel->SetPosition(m_ObjectRect.left, iHalfPosition + szAxisLabel.cy/2,pDC);
		m_pAxisLabel->Draw(pDC);

		
		double nNumber = m_MinValue;
		int VertPosition = 0;
		double nSmallNumber;
		long lTextOffset = 2 + m_iTickLengthBig;

		//Draw Min_Value Tick Text
		VertPosition = ValueToScreen(nNumber);	
		sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
		szFontSize = pDC->GetTextExtent(szBuffer);
		pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx,VertPosition - szFontSize.cy/2 - 1,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
		//Draw Tick
		pDC->MoveTo(m_ObjectRect.right, VertPosition);
		pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig, VertPosition);
		//Draw Ticks
		nSmallNumber = nNumber;
		for(int i=0; i<m_iSmallTickCount;i++)
		{
			nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
			VertPosition = ValueToScreen(nSmallNumber);	
			pDC->MoveTo(m_ObjectRect.right, VertPosition);
			if(i != m_iSmallTickCenterNumber)
			{pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall,VertPosition);}
			else{pDC->LineTo(m_ObjectRect.right + m_iTickLengthMedium,VertPosition);}
		}
		nNumber += m_nBigTickIncrement;

		//Draw Ticks & Ticks Text
		for(int i=0; i<m_iTickCount; i++)
		{
			//Draw Tick Text
			VertPosition = ValueToScreen(nNumber);	
			sprintf(szBuffer,"%.*f",m_DecCount,nNumber);
			szFontSize = pDC->GetTextExtent(szBuffer);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - 1,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);

			//Draw Big Tick
			pDC->MoveTo(m_ObjectRect.right,VertPosition);
			pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig,VertPosition);

			//Draw Small Ticks
			nSmallNumber = nNumber;
			for(int i=0; i<m_iSmallTickCount;i++)
			{
				nSmallNumber = nSmallNumber + m_nSmallTickIncrement;
				VertPosition = ValueToScreen(nSmallNumber);	
				if(VertPosition < m_ObjectRect.top) break;
				pDC->MoveTo(m_ObjectRect.right, VertPosition);
				if(i != m_iSmallTickCenterNumber)
				{pDC->LineTo(m_ObjectRect.right + m_iTickLengthSmall, VertPosition);}
				else{pDC->LineTo(m_ObjectRect.right + m_iTickLengthMedium, VertPosition);}
			}

			nNumber += m_nBigTickIncrement;
		}

		//Draw Max Tick Text
	//	if(m_isDrawMaxTick)
	//	{
			VertPosition = ValueToScreen(m_MaxValue);	
			sprintf(szBuffer,"%.*f",m_DecCount,m_MaxValue);
			szFontSize = pDC->GetTextExtent(szBuffer);
			pDC->ExtTextOut(m_ObjectRect.right - lTextOffset - szFontSize.cx, VertPosition - szFontSize.cy/2 - 1,ETO_CLIPPED|ETO_OPAQUE,NULL,szBuffer,NULL);
			//Draw Tick
			pDC->MoveTo(m_ObjectRect.right, VertPosition);
			pDC->LineTo(m_ObjectRect.right + m_iTickLengthBig, VertPosition);	
	//	}

		m_pGrid->SetRect(m_pParent->GetPlottingRect());
		m_pGrid->Draw(pDC);


	
		
	}
	}
	else
	{
		if(m_isHorizontal)
		{
			CSize szAxisLabel = m_pAxisLabel->GetSize(pDC);
			m_pAxisLabel->SetPosition(iHalfPosition - szAxisLabel.cx/2,m_ObjectRect.bottom - szAxisLabel.cy,pDC);
			m_pAxisLabel->Draw(pDC);

			//Draw Axis Line
			pDC->MoveTo(m_ObjectRect.left - 5,m_ObjectRect.top);
			pDC->LineTo(m_ObjectRect.right,m_ObjectRect.top);
		
		}
		//Draw Vertical
		else
		{}
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);


}





*/





long CChartAxis::GetTickPosition(int TickIndex)
{
	double dTickVal = 0;

	if(!m_isLogarithmic)
		dTickVal = m_MinValue + TickIndex * m_nBigTickIncrement;
	else
	{
		dTickVal = m_MinValue * pow(m_nBigTickIncrement, (double)TickIndex);
	}

	return ValueToScreen(dTickVal);

}

void CChartAxis::RegisterSeries(CChartSerie* pSerie)
{
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for(iter; iter != m_pRelatedSeries.end(); iter++)
	{
		if( (*iter) == pSerie)
			return;
	}
	m_pRelatedSeries.push_back(pSerie);
}
void CChartAxis::UnregisterSeries(CChartSerie* pSerie)
{
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	for(iter; iter != m_pRelatedSeries.end(); iter++)
	{
		if( (*iter) == pSerie)
		{
			m_pRelatedSeries.erase(iter);
			return;
		}
	}
}

void CChartAxis::CalculateAutoAxis(void)
{
	if(m_isLogarithmic)
	{
		m_nBigTickIncrement = 10;
		CountTicks();
		return;
	}

	double Min = 0;
	double Max = 0;
	double TempMin = 0;
	double TempMax = 0;
/*
	SeriesList::iterator iter = m_pRelatedSeries.begin();
	if(iter != m_pRelatedSeries.end())
	{
		if(m_isHorizontal)
			(*iter)->GetXMinMax(Min,Max);
		else
			(*iter)->GetYMinMax(Min,Max);
	}

	for(iter; iter != m_pRelatedSeries.end(); iter++)
	{
		if(m_isHorizontal)
			(*iter)->GetXMinMax(TempMin,TempMax);
		else
			(*iter)->GetYMinMax(TempMin,TempMax);

		if (TempMin < Min)
			Min = TempMin;
		if (TempMax > Max)
			Max = TempMax;
	}
*/
	Max = m_MaxValue;
	Min = m_MinValue;
	
	int PixelSpace = 30;

	double MaxTicksCount = fabs(1.0*(m_EndPos - m_StartPos)/PixelSpace);
	double TempTickIncrement = (Max - Min)/MaxTicksCount;
	double Step = (int)floor(log10(TempTickIncrement));
	double MinTickIncrement = pow(10.0,Step);

	if(MinTickIncrement>TempTickIncrement)
		m_nBigTickIncrement = MinTickIncrement;

	else if(MinTickIncrement*5>TempTickIncrement)
		m_nBigTickIncrement = MinTickIncrement*5;
	
	else if(MinTickIncrement*10>TempTickIncrement)
		m_nBigTickIncrement = MinTickIncrement*10;


	CountTicks();
}
void CChartAxis::CountTicks(void)
{
	//count ticks
	m_nSmallTickIncrement = m_nBigTickIncrement/(m_iSmallTickCount + 1);
    m_iSmallTickCenterNumber = (int)floor((float)m_iSmallTickCount/2);
	if(m_isLogarithmic)
	{
		m_nSmallTickIncrement = 9.0 /(m_iSmallTickCount +1);
	}

/*
	double nNumber = m_MinValue;
	m_iTickCount = 0;
		if(!m_isLogarithmic)
		{
			while(1)
			{
				nNumber += m_nBigTickIncrement;
				if(nNumber >= m_MaxValue) 
				{
					break;
				}
				m_iTickCount++;
			}
			if((nNumber - m_nBigTickIncrement) == m_MaxValue)
			{
				m_iTickCount--;
			}
		
		
			m_nSmallTickIncrement = m_nBigTickIncrement/(m_iSmallTickCount + 1);
			m_iSmallTickCenterNumber = (int)floor((float)m_iSmallTickCount/2);

		}
		//if logarithmic
		else
		{

		}
*/
/*
#ifdef _DEBUG
	TRACE("Draw Max Tick: %d\n", m_isDrawMaxTick);
	TRACE("Ticks count: %d\n", m_iTickCount);
	TRACE("Small ticks increment: %f\n", m_nSmallTickIncrement);
#endif
 */
}


void CChartAxis::RefreshAutoAxis()
{
	double Min = 0;
	double Max = 0;
	double TempMin = 0;
	double TempMax = 0;

	SeriesList::iterator iter = m_pRelatedSeries.begin();
	if(iter != m_pRelatedSeries.end())
	{
		if(m_isHorizontal)
			(*iter)->GetXMinMax(Min,Max);
		else
			(*iter)->GetYMinMax(Min,Max);
	}

	for(iter; iter != m_pRelatedSeries.end(); iter++)
	{
		if(m_isHorizontal)
			(*iter)->GetXMinMax(TempMin,TempMax);
		else
			(*iter)->GetYMinMax(TempMin,TempMax);

		if (TempMin < Min)
			Min = TempMin;
		if (TempMax > Max)
			Max = TempMax;
	}

	SetMinMax(Min,Max);
	
}

void CChartAxis::SetInverted(bool isInverted)
{
	m_isInverted = isInverted;
	m_pParent->RefreshCtrl();
}


void CChartAxis::SetAutomatic(bool isAutomatic)
{
	m_isAutomatic = isAutomatic;
	RefreshAutoAxis();
	m_pParent->RefreshCtrl();
}

int CChartAxis::GetZeroTickPosition(void)
{
	return ValueToScreen(0);
}
void CChartAxis::SetBoxed()
{
	m_isBoxed = true;
}
void CChartAxis::SetCrossed()
{
	m_isBoxed = false;
}

CSize CChartAxis::GetTextSize(CDC* pDC)
{
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize,(LPCTSTR)m_strFontName.c_str(),pDC);
	CFont* pOldFont;
	pOldFont = pDC->SelectObject(&NewFont);

	CString strBuffer;

	m_DecCount = uPrecigion;
	strBuffer.Format(_T("%.*f"),m_DecCount,m_MaxValue);
	CSize maxSize = pDC->GetTextExtent(strBuffer);

	strBuffer.Format(_T("%.*f"),m_DecCount,m_MinValue);
	CSize minSize = pDC->GetTextExtent(strBuffer);
	int maxLen = maxSize.cx;
	if(maxLen<minSize.cx) maxLen = minSize.cx;

	CSize TextSize = maxSize;
	TextSize.cx = maxLen;
	

	pDC->SelectObject(pOldFont);
	
	return TextSize;
}

void CChartAxis::SetMinMax(double Min, double Max)
{
	if(Min > Max) 
	{
		TRACE(L"!!! Max should be greater then Min\n");
		return;
	}
	if(m_isLogarithmic && Min<=0)
	{
		TRACE(L"!!! Values must be greater then 0 in logarithmic scale\n");
		return;
	}
	m_MinValue = m_UnzoomMin = Min;
	m_MaxValue = m_UnzoomMax = Max;
	m_pParent->RefreshCtrl();
}



void CChartAxis::SetZoomMinMax(double Min, double Max)
{
	if(Max<Min) return;

	m_MinValue = m_UnzoomMin = Min;
	m_MaxValue = m_UnzoomMax = Max;
}

void CChartAxis::SetLogarithmic(bool isLogarithmic)
{
	m_isLogarithmic = isLogarithmic;
	m_nBigTickIncrement = 10;
	m_iSmallTickCount = 9;
	
	CountTicks();

	if(m_isLogarithmic)
	{
		double Min, Max;
		GetMinMax(Min,Max);
		m_DecCount = abs(log10(Min));
		if(abs(log10(Max)) > m_DecCount)
			m_DecCount = abs(log10(Max));
	}
	else 
	{
//		m_DecCount = m_InitialDecCount;
		m_DecCount = uPrecigion;//2;//1;
	}

	m_pParent->RefreshCtrl();
}

void CChartAxis::GetMinMaxTickIncrement(double& Min, double& Max, double& TickIncrement)
{
	Min = m_MinValue;
	Max = m_MaxValue;
	TickIncrement = m_nBigTickIncrement;
}


/*
void CChartAxis::FormatString(char* szBuffer, double nNumber)
{

}
*/
void CChartAxis::SetBigTickIncrement(double Increment)
{
	if(m_isLogarithmic)
	{	
		AfxMessageBox(L"Increment is fixed for logarithmic scale.\n Please do not call SetBigTickIncrement");
		return;
	}

	m_nBigTickIncrement = Increment;
	CountTicks();
}

void CChartAxis::SetSmallTickCount(int TickCount)
{
	m_iSmallTickCount = TickCount;
	CountTicks();
	m_pParent->RefreshCtrl();
}

void CChartAxis::SetLabelText(CString Text)
{
	m_pAxisLabel->SetText(Text);
}
void CChartAxis::SetLabelFont(int iPointSize, CString strFaceName)
{
	m_pAxisLabel->SetFont(iPointSize, strFaceName);
}

void CChartAxis::GetMinMax(double& Min, double& Max)
{
	Min = m_MinValue;
	Max = m_MaxValue;
}

void CChartAxis::SetGridColor(COLORREF Color)
{	m_Color = Color;

	m_pGrid->SetColor(Color);
}

void CChartAxis::SetTextColor(COLORREF Color)
{
	m_TextColor = Color;
}

void CChartAxis::SetLabelTextColor(COLORREF Color)
{
	m_pAxisLabel->SetColor(Color);
}


double CChartAxis::RoundDouble(double doValue, int nPrecision)
{
	static const double doBase = 10.0;
	double doComplete5, doComplete5i;
	
	doComplete5 = doValue * pow(doBase, (double) (nPrecision + 1));
	
	if(doValue < 0.0)
		doComplete5 -= 5.0;
	else
		doComplete5 += 5.0;
	
	doComplete5 /= doBase;
	modf(doComplete5, &doComplete5i);
	
	return doComplete5i / pow(doBase, (double) nPrecision);
}
