#pragma once



#include "ChartSerie.h"
#include "ChartAxis.h"
#include "ChartGrid.h"
#include "ChartLegend.h"
#include "ChartMarker.h"
#include "ChartSecant.h"
#include "ChartBMPSaver.h"
#include "ChartSerieFamily.h"

#include <vector>

#define CHARTCTRL_CLASSNAME    L"ChartCtrl"  // Window class name

class CChartCtrl : public CWnd
{
	DECLARE_DYNAMIC(CChartCtrl)

public:
	CChartCtrl();
	virtual ~CChartCtrl();
	int Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle=WS_VISIBLE);
	void RefreshCtrl();
	void RefreshPlottingRect();

protected:
	//{{AFX_MSG(CChartCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool RegisterWindowClass();
	CDC m_BackgroundDC;
	CDC m_DrawedDCWNMarkers;
	bool m_bMemDCCreated;
	
	CRect m_PlottingRect;
	CRect m_ZoomRect;
	bool m_isZoomEnabled;
	bool m_isMoveMarker;
	bool m_isDrawSecant;
	CWnd* m_pParent;
	UINT EdgeType;
	COLORREF m_BackColor;
	bool m_isRefreshMarkerOnMouseMove;
	bool m_isRefreshDisabled;

	std::vector<CChartSerie*> m_pSeriesList;		// Table containing all the series (dynamic)
	std::vector<CChartSerieFamily*> m_pFamilyList;

public:
	std::vector<CChartMarker*> m_pMarkersList;

	
public:

	void DrawMarkers();
	CChartSerie* AddSerie(int Type);
	void DeleteSerie(int Index);
	size_t m_iSeriesCount;
	int m_iActiveMarkerIndex;
	CChartSecant* m_pSecant;
	CChartAxis* m_pAxisBottom;
	CChartAxis* m_pAxisLeft;
	CChartLegend* m_pChartLegend;
	CChartAxis* GetBottomAxis(void);
	CChartAxis* GetLeftAxis(void);
	CRect GetPlottingRect(void);
	void RefreshAutoAxis(void);
	int GetSeriesCount(void);
	int GetFamiliesCount(void);
	CChartSerie* GetSerie(int Index);
	void SetBackColor(COLORREF Color);
	void SetCrossedAxis(void);
	void SetBoxedAxis(void);
	CPoint GetNearestSeriePoint(CPoint point, double& XValue, double& YValue);
	CPoint GetNearestSeriePoint(int SerieIndex, CPoint point, double& XValue, double& YValue);
	CChartMarker* CreateMarker(int RelatedSerieIndex = 0);
	void SetActiveMarker(int ActiveMarkerIndex);
	void DrawSecant(bool isDrawSecant);
	void SetRefreshMarkerOnMouseMove(bool isRefresh);
	void DisableRefresh(bool isDisabled);
	void SaveToBMP(CString FileName);
	void ShowLegend(bool isShow);
	
	void DeleteMarker(int MarkerIndex);
	void MarkerMoveLeft(void);
	void MarkerMoveRight(void);
	void RefreshMarkersPos(CChartSerie* pSerie);

	CChartSerieFamily* GetFamily(int Index);
	CChartSerieFamily* AddFamily();
	void DeleteFamily(int Index);
	COLORREF m_FrameColor;
	void SetFrameColor(COLORREF Color);
};	


