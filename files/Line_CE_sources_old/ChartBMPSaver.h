#pragma once

class CChartBMPSaver
{
public:
	CChartBMPSaver(void);
	~CChartBMPSaver(void);
	void SaveBMP(HBITMAP hBMP, CString FileName, HDC hDC);

private:
	PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBitmap);
	void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
	int m_MAXWRITE;

};
