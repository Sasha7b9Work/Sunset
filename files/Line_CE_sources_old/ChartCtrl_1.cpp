// ChartCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"
#include "ChartPointSerie.h"
#include "ChartMixedSerie.h"
#include ".\chartctrl.h"
#include "ChartLegend.h"


#define WM_REFRESHMARKER WM_USER+1
// CChartCtrl

IMPLEMENT_DYNAMIC(CChartCtrl, CWnd)
CChartCtrl::CChartCtrl()
{
	RegisterWindowClass();

	EdgeType = 0;//EDGE_RAISED;
	m_BackColor = GetSysColor(COLOR_BTNFACE);
	
	m_bMemDCCreated = false;
	
	m_PlottingRect.top = 0;
	m_PlottingRect.left = 0;
	m_PlottingRect.right = 0;
	m_PlottingRect.bottom = 0;
	m_iSeriesCount = 0;
	m_isZoomEnabled = true;
	m_ZoomRect.SetRect(0,0,0,0);

	m_pAxisBottom = new CChartAxis(this,true);
	m_pAxisLeft = new CChartAxis(this,false);
	m_pChartLegend = new CChartLegend(this);
	m_pSecant = new CChartSecant(this);

	m_isMoveMarker = false;
	m_isDrawSecant = false;
	m_iActiveMarkerIndex = 0;
	m_isRefreshMarkerOnMouseMove = true;
	m_isRefreshDisabled = false;
}

CChartCtrl::~CChartCtrl()
{
	for(int i=0; i<m_pSeriesList.size();i++)
	{
		delete m_pSeriesList[i];
	}
	delete m_pAxisBottom;
	delete m_pAxisLeft;
	delete m_pChartLegend;
	for(i=0; i<m_pMarkersList.size();i++)
		delete m_pMarkersList[i];
	delete m_pSecant;
}


BEGIN_MESSAGE_MAP(CChartCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CChartCtrl message handlers


bool CChartCtrl::RegisterWindowClass()
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CHARTCTRL_CLASSNAME, &wndcls)))
    {
		memset(&wndcls, 0, sizeof(WNDCLASS));   

		wndcls.hInstance		= hInst;
		wndcls.lpfnWndProc		= ::DefWindowProc;
	//	wndcls.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wndcls.hIcon			= 0;
		wndcls.lpszMenuName		= NULL;
		wndcls.hbrBackground	= (HBRUSH) ::GetStockObject(WHITE_BRUSH);
		wndcls.style			= CS_GLOBALCLASS; // To be modified
		wndcls.cbClsExtra		= 0;
		wndcls.cbWndExtra		= 0;
		wndcls.lpszClassName    = CHARTCTRL_CLASSNAME;

        if (!RegisterClass(&wndcls))
        {
          //  AfxThrowResourceException();
            return false;
        }
    }

    return true;

}
int CChartCtrl::Create(CWnd *pParentWnd, const RECT &rect, UINT nID, DWORD dwStyle)
{
	int Result = CWnd::Create(CHARTCTRL_CLASSNAME, L"", dwStyle, rect, pParentWnd, nID);
	
	this->SetParent(pParentWnd);

	if (Result)
		RefreshCtrl();
	
	return Result;
}

void CChartCtrl::RefreshCtrl()
{
//	TRACE(L"RefreshCtrl\n");

	if(m_isRefreshDisabled) 
		return; 

	CClientDC dc(this);
	CRect ClientRect;
	GetClientRect(ClientRect);


	CBrush m_BrushBack;
	m_BrushBack.CreateSolidBrush(m_BackColor);

	if(!m_BackgroundDC.GetSafeHdc())
	{
		CBitmap memBitmap;
		m_BackgroundDC.CreateCompatibleDC(&dc);
		memBitmap.CreateCompatibleBitmap(&dc, ClientRect.Width(),ClientRect.Height());
		m_BackgroundDC.SelectObject(&memBitmap);

		CBitmap memBitmap1;
		m_DrawedDCWNMarkers.CreateCompatibleDC(&dc);
		memBitmap1.CreateCompatibleBitmap(&dc, ClientRect.Width(),ClientRect.Height());
		m_DrawedDCWNMarkers.SelectObject(&memBitmap1);
	}
	
	m_BackgroundDC.SetBkColor(m_BackColor);
	m_BackgroundDC.FillRect(ClientRect,&m_BrushBack);
	m_BackgroundDC.DrawEdge(ClientRect,EdgeType,BF_RECT);


	
	/*
	m_DrawedDCWNMarkers.SetBkColor(m_BackColor);
	m_DrawedDCWNMarkers.FillRect(ClientRect,&m_BrushBack);
	m_DrawedDCWNMarkers.DrawEdge(ClientRect,EdgeType,BF_RECT);
	*/

	
	ClientRect.DeflateRect(3,3);
	m_PlottingRect = ClientRect;
	
	
	//TRACE("befor axis m_PlottingRect: top=%d, left=%d, bottom=%d, right=%d\n",
	//		m_PlottingRect.top,m_PlottingRect.left,m_PlottingRect.bottom,m_PlottingRect.right);

	
	//DrawLegend
	CSize szLegend = m_pChartLegend->GetSize(&m_BackgroundDC);
	m_PlottingRect.right -= szLegend.cx;
	m_pChartLegend->SetPosition(m_PlottingRect.right,m_PlottingRect.Height()/2-szLegend.cy,&m_BackgroundDC);
	m_pChartLegend->Draw(&m_BackgroundDC);

	m_PlottingRect.right -= 5;

	//DrawAxis
	m_pAxisBottom->ClipMargin(m_PlottingRect,&m_BackgroundDC);
	m_pAxisLeft->ClipMargin(m_PlottingRect,&m_BackgroundDC);
	if(!m_pAxisBottom->SetAxisSize(ClientRect,m_PlottingRect))
		m_pAxisBottom->SetAxisSize(ClientRect,m_PlottingRect);
	if(!m_pAxisLeft->SetAxisSize(ClientRect,m_PlottingRect))
		m_pAxisLeft->SetAxisSize(ClientRect,m_PlottingRect);
	m_pAxisBottom->Draw(&m_BackgroundDC);
	m_pAxisLeft->Draw(&m_BackgroundDC);

	CPen pen;
	pen.CreatePen(PS_SOLID,0,m_FrameColor);
	CPen *oldPen = m_BackgroundDC.SelectObject(&pen);

	m_BackgroundDC.MoveTo(m_PlottingRect.left,m_PlottingRect.top);
	m_BackgroundDC.LineTo(m_PlottingRect.right,m_PlottingRect.top);
	m_BackgroundDC.LineTo(m_PlottingRect.right,m_PlottingRect.bottom);
	m_BackgroundDC.LineTo(m_PlottingRect.left,m_PlottingRect.bottom);
	m_BackgroundDC.LineTo(m_PlottingRect.left,m_PlottingRect.top);
	
	m_BackgroundDC.SelectObject(oldPen);
	

	//Draw Markers
	//DrawMarkers();
//	for(int i=0;i<m_pMarkersList.size();i++)
//		m_pMarkersList[i]->Draw(&m_BackgroundDC);
	

	//Draw Graphs
	for(int i=0;i<m_iSeriesCount;i++)
	{
		m_pSeriesList[i]->SetRect(m_PlottingRect);
		m_pSeriesList[i]->DrawAll(&m_BackgroundDC);
	}

	for(i=0;i<m_pMarkersList.size();i++)
	{
		m_pMarkersList[i]->SetWorkingRect(m_PlottingRect);
	}


	TRACE(L"W: %d, H: %d\n",ClientRect.Width(),ClientRect.Height());
	m_DrawedDCWNMarkers.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&m_BackgroundDC, 3, 3,SRCCOPY);
	DrawMarkers();

	
	Invalidate();
	
}
void CChartCtrl::DrawMarkers()
{

	TRACE(L"DrawMarker_1\n");

	CClientDC dc(this);
	CRect ClientRect;
	GetClientRect(ClientRect);

	TRACE(L"W: %d, H: %d\n",ClientRect.Width(),ClientRect.Height());
	m_BackgroundDC.BitBlt(3, 3, ClientRect.Width() - 6, ClientRect.Height() - 6 ,&m_DrawedDCWNMarkers, 0, 0, SRCCOPY);

	for(int i=0;i<m_pMarkersList.size();i++)
		m_pMarkersList[i]->Draw(&m_BackgroundDC);

	if(m_isDrawSecant)
	{
		m_pSecant->SetRect(m_PlottingRect);
		m_pSecant->Draw(&m_BackgroundDC);
	}


	Invalidate();
}

void CChartCtrl::OnPaint()
{
	CPaintDC dc(this);
	if (!m_bMemDCCreated)
	{
		RefreshCtrl();
		m_bMemDCCreated = true;
	}

	CRect rect;
	GetClientRect(&rect);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_BackgroundDC,0,0,SRCCOPY);
}

BOOL CChartCtrl::OnEraseBkgnd(CDC* pDC)
{
	//return CWnd::OnEraseBkgnd(pDC);
	return false;
}

CChartSerie* CChartCtrl::AddSerie(int Type)
{
	CChartSerie* pNewSerie;
	switch(Type)
	{
	case CChartSerie::stLine:
		pNewSerie = new CChartLineSerie(this);
		break;
	case CChartSerie::stPoints:
		pNewSerie = new CChartPointSerie(this);
		break;
	case CChartSerie::stMixed:
		pNewSerie = new CChartMixedSerie(this);
		break;
	default:
		pNewSerie = NULL;
		break;
	}
	
	if(pNewSerie)
	{
		m_pSeriesList.push_back(pNewSerie);
		m_iSeriesCount = m_pSeriesList.size();
	}
	
	return pNewSerie;
}

void CChartCtrl::DeleteSerie(int Index)
{
	
	if(m_pMarkersList.size() > 0)
	{
		int iMarkerIndex = 0;
		for(int i=0;i<m_pMarkersList.size();i++)
		{
			if(m_pMarkersList[iMarkerIndex]->GetRelatedSerieIndex() == Index)
		{
			CChartMarker* pMarker = m_pMarkersList[i];
			std::vector<CChartMarker*>::iterator iter = m_pMarkersList.begin() + iMarkerIndex;
			m_pMarkersList.erase(iter);
			if(pMarker) delete pMarker;
			
		}
			else iMarkerIndex++;
		}
	}

	m_pAxisBottom->UnregisterSeries(m_pSeriesList[Index]);
	m_pAxisLeft->UnregisterSeries(m_pSeriesList[Index]);

	size_t Count = m_pSeriesList.size();
	if (Index>=Count)
		return;

	CChartSerie* pToDelete = m_pSeriesList[Index];

    std::vector<CChartSerie*>::iterator it = m_pSeriesList.begin() + Index;
	m_pSeriesList.erase(it);
	if (pToDelete)
	{
		delete pToDelete;
		pToDelete = NULL;
	}

	m_iSeriesCount = GetSeriesCount();
	RefreshCtrl();

}

CChartAxis* CChartCtrl::GetBottomAxis(void)
{
	return m_pAxisBottom;
}

CChartAxis* CChartCtrl::GetLeftAxis(void)
{
	return m_pAxisLeft;
}

CRect CChartCtrl::GetPlottingRect(void)
{
	return m_PlottingRect;
}

int CChartCtrl::GetSeriesCount(void)
{
	return (int)m_pSeriesList.size();
}

CChartSerie* CChartCtrl::GetSerie(int Index)
{
	return m_pSeriesList[Index];
}
CChartSerieFamily* CChartCtrl::GetFamily(int Index)
{
	return m_pFamilyList[Index];
}


void CChartCtrl::SetBackColor(COLORREF Color)
{
	m_BackColor = Color;
	RefreshCtrl();
}


void CChartCtrl::SetBoxedAxis(void)
{
	m_pAxisBottom->SetBoxed();
	m_pAxisLeft->SetBoxed();
	RefreshCtrl();
}

void CChartCtrl::SetCrossedAxis(void)
{
	m_pAxisBottom->SetCrossed();
	m_pAxisLeft->SetCrossed();
	RefreshCtrl();
}




void CChartCtrl::OnLButtonDown(UINT nFlags,CPoint point)
{
	/*
	if(m_isZoomEnabled)
	{
		m_ZoomRect.TopLeft() = point;
	}
	CWnd::OnLButtonDown(nFlags,point);
	*/
	if(m_pMarkersList.size() != 0)
		m_isMoveMarker = true;

}
/*
void CChartCtrl::OnLButtonUp(UINT nFlags,CPoint point)
{
		DWORD tick = ::GetTickCount();

	if(m_isMoveMarker)
	{
		m_isMoveMarker = false;
		double X;
		double Y;
////Sergey*********************************		
		CPoint p = GetNearestSeriePoint(point,X,Y);
		
		//m_pSeriesList[m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex()]->ScreenToValue(X,Y,p);
		m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(X,Y);
		m_pSecant->SetPoints(X,Y);
	//	RefreshCtrl();
		DrawMarkers();
//************************************
//My->*********************************
		CPoint p = GetNearestSeriePoint(point,X,Y);
		
		m_pSeriesList[m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex()]->ScreenToValue(X,Y,p);
//		m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(X,Y);
//		m_pSecant->SetPoints(X,Y);
	//	RefreshCtrl();

		m_pMarkersList[m_iActiveMarkerIndex]->SetRelatedSerie(2);
		
		DrawMarkers();

		if(m_isRefreshMarkerOnMouseMove)
		{								
//			m_pSecant->SetPoints(X,Y);
		}
//<-***********************************
		this->GetParent()->PostMessage(WM_REFRESHMARKER,NULL,NULL);
	}

		tick = ::GetTickCount() - tick;
		TRACE(L"OnLButtonUp: %d ms\n",tick);

		///*************************
		if(m_isZoomEnabled)
		{
			if(m_ZoomRect.right < m_ZoomRect.left)
			{
				//unzoom
			}
			else
			{
				double Min = m_pAxisBottom->ScreenToValue(m_ZoomRect.left);
				double Max = m_pAxisBottom->ScreenToValue(m_ZoomRect.right);
				if(Min<Max)
					m_pAxisBottom->SetZoomMinMax(Min,Max);
				else m_pAxisBottom->SetZoomMinMax(Max,Min);

				Min = m_pAxisLeft->ScreenToValue(m_ZoomRect.top);
				Max = m_pAxisLeft->ScreenToValue(m_ZoomRect.bottom);
				if(Min<Max)
					m_pAxisLeft->SetZoomMinMax(Min,Max);
				else m_pAxisLeft->SetZoomMinMax(Max,Min);

				RefreshCtrl();
			}
		}
		//************************
}
*/

void CChartCtrl::OnLButtonUp(UINT nFlags,CPoint point)
{
		DWORD tick = ::GetTickCount();

	if(m_isMoveMarker)
	{
		m_isMoveMarker = false;
		double X;
		double Y;
/*//Sergey		
		CPoint p = GetNearestSeriePoint(point,X,Y);
		
		//m_pSeriesList[m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex()]->ScreenToValue(X,Y,p);
		m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(X,Y);
		m_pSecant->SetPoints(X,Y);
	//	RefreshCtrl();
		DrawMarkers();
*/
//My->*********************************
		int NearestPointIndex;
		CPoint p;
		CPoint NearestPoint = GetNearestSeriePoint(point,X,Y); //Для начала берём за 0 за ближайший график
		int NearestSerieIndex = m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex();
		
		int seriesCount = m_pSeriesList.size();
		
		for(int i = 1; i < seriesCount; i++) //Оределяем график, ближайший к текущему положению маркера
		{
			p = GetNearestSeriePoint(point,X,Y);	
			if (abs (point.y - NearestPoint.y) > abs(p.y - point.y) )
			{
				NearestPoint = p;
				NearestPointIndex = i; 
			}
		}
		
		//переустанавливаем маркер на найденную ближайшую кривую
		m_pMarkersList[m_iActiveMarkerIndex]->SetRelatedSerie(NearestPointIndex);		
		//устанавливаем позицию маркера
		m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(X,Y);
		
		
//		m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(X,Y);
//		m_pSecant->SetPoints(X,Y);
	//	RefreshCtrl();


		
		DrawMarkers();

		if(m_isRefreshMarkerOnMouseMove)
		{								
//			m_pSecant->SetPoints(X,Y);
		}
//<-***********************************
		this->GetParent()->PostMessage(WM_REFRESHMARKER,NULL,NULL);
	}

		tick = ::GetTickCount() - tick;
		TRACE(L"OnLButtonUp: %d ms\n",tick);

		/*
		if(m_isZoomEnabled)
		{
			if(m_ZoomRect.right < m_ZoomRect.left)
			{
				//unzoom
			}
			else
			{
				double Min = m_pAxisBottom->ScreenToValue(m_ZoomRect.left);
				double Max = m_pAxisBottom->ScreenToValue(m_ZoomRect.right);
				if(Min<Max)
					m_pAxisBottom->SetZoomMinMax(Min,Max);
				else m_pAxisBottom->SetZoomMinMax(Max,Min);

				Min = m_pAxisLeft->ScreenToValue(m_ZoomRect.top);
				Max = m_pAxisLeft->ScreenToValue(m_ZoomRect.bottom);
				if(Min<Max)
					m_pAxisLeft->SetZoomMinMax(Min,Max);
				else m_pAxisLeft->SetZoomMinMax(Max,Min);

				RefreshCtrl();
			}
		}
		*/
}


void CChartCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{

	
	double X;
	double Y;	
	
	//DWORD tick = ::GetTickCount();
	
	if(m_isMoveMarker)
	{
		m_pSeriesList[m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex()]->ScreenToValue(X,Y,point);
		m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(X,Y);
	//	RefreshCtrl();
		DrawMarkers();

		if(m_isRefreshMarkerOnMouseMove)
		{
			m_pSecant->SetPoints(X,Y);
			this->GetParent()->PostMessage(WM_REFRESHMARKER,NULL,NULL);
		}
	}
	//tick = ::GetTickCount() - tick;
	//TRACE("OnMouseMove: %d ms\n", tick);


	/*
	if(m_isZoomEnabled)
	{
		m_ZoomRect.BottomRight() = point;
		Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
	*/
}

CPoint CChartCtrl::GetNearestSeriePoint(CPoint point, double& XValue, double& YValue)
{
	return m_pSeriesList[m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex()]->GetNearestPoint(point, XValue, YValue);	
}

CChartMarker* CChartCtrl::CreateMarker(int RelatedSerieIndex)
{
	if(m_pMarkersList.size() == 2)
	{
		AfxMessageBox(L"Only 2 markers allowed");
		return NULL;
	}

	if(m_pSeriesList.size() == 0)
	{
		AfxMessageBox(L"At least one serie should be added");
		return NULL;
	}
	if(m_pSeriesList.size() <= RelatedSerieIndex)
	{
		AfxMessageBox(L"Related serie index is not correct");
		return NULL;
	}

	CChartMarker* pMarker = new CChartMarker(this);
	pMarker->SetRelatedSerie(RelatedSerieIndex);
	
	double X;
	double Y;
	CPoint ScreenPoint;
	GetSerie(RelatedSerieIndex)->GetMidPoint(X,Y);
//	GetSerie(RelatedSerieIndex)->ValueToScreen(X,Y,ScreenPoint);
    pMarker->SetPosition(X,Y);
	int iActiveMarker = m_iActiveMarkerIndex;
	m_iActiveMarkerIndex = (int)m_pMarkersList.size();
	m_pSecant->SetPoints(X,Y);
	m_iActiveMarkerIndex = iActiveMarker;
	m_pMarkersList.push_back(pMarker);

	//RefreshCtrl();
	DrawMarkers();

	return pMarker;
}

void CChartCtrl::SetActiveMarker(int ActiveMarkerIndex)
{
	m_iActiveMarkerIndex = ActiveMarkerIndex;
}

void CChartCtrl::DrawSecant(bool isDrawSecant)
{
	if(m_pMarkersList.size()<2)
	{
		AfxMessageBox(L"Two markers needed to draw");
		return;
	}
	m_isDrawSecant = isDrawSecant;
	RefreshCtrl();

	//m_pMarkersList[0]->GetValues

}
void CChartCtrl::SetRefreshMarkerOnMouseMove(bool isRefresh)
{
	m_isRefreshMarkerOnMouseMove = isRefresh;
}

void CChartCtrl::DisableRefresh(bool isDisabled)
{
	m_isRefreshDisabled = isDisabled;
	if(!m_isRefreshDisabled)
	{
		RefreshCtrl();
		DrawMarkers();
	}
}



void CChartCtrl::SaveToBMP(CString FileName)
{
	
	CRect rect;
	GetClientRect(&rect);


	//HDC hdcScreen = ::GetWindowDC(::GetDesktopWindow());
	HDC hdcScreen = (this->GetWindowDC())->GetSafeHdc();
	//int cxScreen = GetDeviceCaps(hdcScreen, HORZRES);
	//int cyScreen = GetDeviceCaps(hdcScreen, VERTRES);

	CRect ClientRect;
	GetClientRect(ClientRect);
	int cxScreen = ClientRect.Width();
	int cyScreen = ClientRect.Height();

	HDC	hdcCompatible = CreateCompatibleDC(hdcScreen);
	HBITMAP bmpScreen = CreateCompatibleBitmap(hdcScreen, cxScreen, cyScreen);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcCompatible, bmpScreen);
	BitBlt(hdcCompatible, 0, 0, cxScreen, cyScreen, hdcScreen, 0, 0, SRCCOPY);
	

	//Способ №3 
	//работает в CE


	if (NULL != bmpScreen)
    {
        //Here stores the bitmap to the file
        // ...

        //Gets bitmap size
        BITMAP    csBitmapSize;
        // Get bitmap size
        int nRetValue = ::GetObject(bmpScreen, sizeof(csBitmapSize), &csBitmapSize);
        if (nRetValue)
        {
            DWORD dwWidth = (DWORD)csBitmapSize.bmWidth;
            DWORD dwHeight = (DWORD)csBitmapSize.bmHeight;

            HDC hDC = ::GetDC(NULL);
            HDC hSrcDC = ::CreateCompatibleDC(hDC);
            HDC hDestDC = ::CreateCompatibleDC(hDC);

            HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hSrcDC, bmpScreen);
            
            LPBITMAPINFO lpbiSrc;
            // Fill in the BITMAPINFOHEADER
            lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
            lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            lpbiSrc->bmiHeader.biWidth = dwWidth;
            lpbiSrc->bmiHeader.biHeight = dwHeight;
            lpbiSrc->bmiHeader.biPlanes = 1;
            lpbiSrc->bmiHeader.biBitCount = 32;
            lpbiSrc->bmiHeader.biCompression = BI_RGB;
            lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
            lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
            lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
            lpbiSrc->bmiHeader.biClrUsed = 0;
            lpbiSrc->bmiHeader.biClrImportant = 0;
            
            COLORREF* pSrcBits = NULL;
            HBITMAP hSrcDib = CreateDIBSection (
                hSrcDC, lpbiSrc, DIB_RGB_COLORS, (void **)&pSrcBits,
                NULL, NULL);

            HBITMAP hOldDestBmp = (HBITMAP)::SelectObject(hDestDC, hSrcDib);
            ::BitBlt(hDestDC, 0, 0, dwWidth, dwHeight, hSrcDC, 0, 0, SRCCOPY);
            ::SelectObject(hDestDC, hOldDestBmp);
            ::DeleteObject(hOldDestBmp);
            ::DeleteDC(hDestDC);
            
            ::SelectObject(hSrcDC, hOldBitmap);
            ::DeleteObject(hOldBitmap);
            ::DeleteDC(hSrcDC);
            ::ReleaseDC(NULL, hDC);

            CFile file;
			if (file.Open(L"image.bmp", CFile::modeCreate | CFile::modeWrite))
            {
                //Creates DIB from the BITMAP
                BITMAPFILEHEADER bfh;
                memset(&bfh, 0, sizeof (BITMAPFILEHEADER));
                bfh.bfType = 'MB';
                bfh.bfSize = sizeof(bfh) + dwWidth * dwHeight * 4 + sizeof( BITMAPINFOHEADER );
                bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

                file.Write(&bfh, sizeof(bfh));
                file.Write(lpbiSrc, sizeof(*lpbiSrc));
                file.Write((BYTE*)pSrcBits, dwWidth * dwHeight * 4);
                file.Close();
            } //if

            ::DeleteObject(hSrcDib);
            delete [] lpbiSrc;
        } //if
    } //if


}


void CChartCtrl::ShowLegend(bool isShow)
{
	if(!isShow)
		m_pChartLegend->m_isVisible = false;
	else
		m_pChartLegend->m_isVisible = true;

	RefreshCtrl();
}

void CChartCtrl::DeleteMarker(int MarkerIndex)
{
	CChartMarker* pMarker = m_pMarkersList[MarkerIndex];
	std::vector<CChartMarker*>::iterator iter = m_pMarkersList.begin() + MarkerIndex;
	m_pMarkersList.erase(iter);
	if(pMarker) delete pMarker;
}


void CChartCtrl::MarkerMoveLeft()
{
	double XValue,YValue;
	m_pMarkersList[m_iActiveMarkerIndex]->GetValues(XValue,YValue);
	m_pSeriesList[m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex()]->GetNearestLeftPoint(XValue, YValue);
	m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(XValue,YValue);
	m_pSecant->SetPoints(XValue,YValue,m_iActiveMarkerIndex);
	RefreshCtrl();
	
}

void CChartCtrl::MarkerMoveRight()
{
	double XValue,YValue;
	m_pMarkersList[m_iActiveMarkerIndex]->GetValues(XValue,YValue);
	m_pSeriesList[m_pMarkersList[m_iActiveMarkerIndex]->GetRelatedSerieIndex()]->GetNearestRightPoint(XValue, YValue);
	m_pMarkersList[m_iActiveMarkerIndex]->SetPosition(XValue,YValue);
	m_pSecant->SetPoints(XValue,YValue,m_iActiveMarkerIndex);
	RefreshCtrl();
	
}

void CChartCtrl::RefreshMarkersPos(CChartSerie* pSerie)
{
	double X,Y;
	for(int i=0; i<m_pMarkersList.size();i++)
	{
		if( m_pSeriesList[(m_pMarkersList[i]->GetRelatedSerieIndex())] == pSerie)
		{
			CPoint ScreenPoint;
			m_pMarkersList[i]->GetValues(X,Y);
			m_pMarkersList[i]->SetPosition(Y,X);
			m_pSecant->SetPoints(Y,X,i);
		}

	}
}

int CChartCtrl::GetFamiliesCount()
{
	return m_pFamilyList.size();
}

CChartSerieFamily* CChartCtrl::AddFamily()
{

	CChartSerieFamily* pNewFamily = new CChartSerieFamily;
	if(pNewFamily)
	{
		m_pFamilyList.push_back(pNewFamily);
	}
	
	return pNewFamily;
}

void CChartCtrl::DeleteFamily(int Index)
{
	
	size_t Count = m_pFamilyList.size();
	if (Index>=Count)
		return;

	CChartSerieFamily* pToDelete = m_pFamilyList[Index];

    std::vector<CChartSerieFamily*>::iterator it = m_pFamilyList.begin() + Index;
	m_pFamilyList.erase(it);
	if (pToDelete)
	{
		delete pToDelete;
		pToDelete = NULL;
	}

	
	RefreshCtrl();
}

void CChartCtrl::SetFrameColor(COLORREF Color)
{
	m_FrameColor = Color;
}