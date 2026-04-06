#include "StdAfx.h"
#include "ChartGrid.h"
#include "ChartAxis.h"
#include ".\chartgrid.h"

CChartGrid::CChartGrid(CChartCtrl* pParent, CChartAxis* pAxis, bool isHorizontal) : CChartObject(pParent)
, m_iTicksCount(0)
{
	
	m_isHorizontal = isHorizontal;
	m_pParentAxis = pAxis;
	m_pParent = pParent;

	m_Color = RGB(211,0,0);
	
}

CChartGrid::~CChartGrid(void)
{
}

void CChartGrid::Draw(CDC* pDC)
{
	if(!IsVisible())
		return;

	if(!pDC->GetSafeHdc())
		return;


	
	

	CPen pen;
	CPen penSolid;
	pen.CreatePen(PS_DASH,0,m_Color);
	penSolid.CreatePen(PS_SOLID,1,m_Color);
	

	CPen *pOldPen;

	pOldPen = pDC->SelectObject(&pen);

	long lPos = 0;

		
	if(m_isHorizontal)
	{
		for(int i=1;i<=m_iTicksCount;i++)
		{
			lPos = m_pParentAxis->GetTickPosition(i);
			pDC->MoveTo(lPos,m_ObjectRect.top);
			pDC->LineTo(lPos,m_ObjectRect.bottom);
		}
	}
	else
	{
		for(int i=1;i<=m_iTicksCount;i++)
		{
			lPos = m_pParentAxis->GetTickPosition(i);
			pDC->MoveTo(m_ObjectRect.left, lPos);
			pDC->LineTo(m_ObjectRect.right, lPos);
		}
	}

//	pDC->SelectObject(pOldPen);

	

 
	//Draw solid line on point (0,0)
	int ZeroTickPosition = m_pParentAxis->GetZeroTickPosition();

	if(m_isHorizontal)
	{
		if(ZeroTickPosition > m_ObjectRect.left && ZeroTickPosition <m_ObjectRect.right)
		{
			pOldPen = pDC->SelectObject(&penSolid);
			pDC->MoveTo(ZeroTickPosition,m_ObjectRect.top);
			pDC->LineTo(ZeroTickPosition,m_ObjectRect.bottom);
			pDC->SelectObject(pOldPen);
		}
	}
	else
	{
		if(ZeroTickPosition > m_ObjectRect.top && ZeroTickPosition <m_ObjectRect.bottom)
		{
			pOldPen = pDC->SelectObject(&penSolid);
			pDC->MoveTo(m_ObjectRect.left, ZeroTickPosition);
			pDC->LineTo(m_ObjectRect.right, ZeroTickPosition);
			pDC->SelectObject(pOldPen);
		}
	}

	pDC->SelectObject(pOldPen);
	
}



void CChartGrid::SetTicksCount(int iTicksCount)
{
	m_iTicksCount = iTicksCount;
}

void CChartGrid::SetColor(COLORREF Color)
{
	m_Color = Color;
}
