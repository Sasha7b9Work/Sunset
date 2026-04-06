#include "StdAfx.h"
#include ".\chartbmpsaver.h"
#include "STScreenBuffer.h"

CChartBMPSaver::CChartBMPSaver(void)
{
	m_MAXWRITE = 2048;
}

CChartBMPSaver::~CChartBMPSaver(void)
{
	
}

void CChartBMPSaver::SaveBMP(HBITMAP hBitmap, CString FileName, HDC hDC)
{
	PBITMAPINFO pbi =  CreateBitmapInfoStruct(hBitmap);
	CreateBMPFile(FileName.GetBuffer(), pbi,hBitmap, hDC);
	LocalFree(pbi);
}

PBITMAPINFO CChartBMPSaver::CreateBitmapInfoStruct(HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) MessageBox(NULL,"","",NULL);

    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else
        cClrBits = 32;


    if (cClrBits != 24)
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                    sizeof(BITMAPINFOHEADER) +
                    sizeof(RGBQUAD) * (2^cClrBits));
    else
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                    sizeof(BITMAPINFOHEADER));

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;

    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)

        pbmi->bmiHeader.biClrUsed = 2^cClrBits;

    pbmi->bmiHeader.biCompression = BI_RGB;
    pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8
                                  * pbmi->bmiHeader.biHeight
                                  * cClrBits;

    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi;

}

void CChartBMPSaver::CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi,HBITMAP hBMP, HDC hDC)
 {

    HANDLE hf;                  /* file handle */
    BITMAPFILEHEADER hdr;       /* bitmap file-header */
    PBITMAPINFOHEADER pbih;     /* bitmap info-header */
  //  LPBYTE lpBits;              /* memory pointer */
	void* lpBits;
	
//    DWORD dwTotal;              /* total count of bytes */
    DWORD cb;                   /* incremental count of bytes */
    BYTE *hp;                   /* byte pointer */

    DWORD dwTmp;

    hf = CreateFile(pszFile,
                   GENERIC_READ | GENERIC_WRITE,
                   (DWORD) 0,
                   (LPSECURITY_ATTRIBUTES) NULL,
                   CREATE_ALWAYS,
                   FILE_ATTRIBUTE_NORMAL,
                   (HANDLE) NULL);

    if (hf == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL,pszFile,"CreateFile Failed.",NULL);
        return;
    }

    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
    if (!lpBits)  MessageBox(NULL,"Global alloc","",NULL);

	

    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight,
                   lpBits, pbi, DIB_RGB_COLORS))
        MessageBox(NULL,"GetDIBits","",NULL);



    hdr.bfType = 0x4d42;        /* 0x42 = "B" 0x4d = "M" */


    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +
                 pbih->biSize + pbih->biClrUsed
                 * sizeof(RGBQUAD) + pbih->biSizeImage);

    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;


    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
                    pbih->biSize + pbih->biClrUsed
                    * sizeof (RGBQUAD);


    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),
       (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL))
        MessageBox(NULL,"","WriteFile",NULL);

    /* Copy the BITMAPINFOHEADER and RGBQUAD array into the file. */

    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
                  + pbih->biClrUsed * sizeof (RGBQUAD),
                  (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL))
        MessageBox(NULL,"","WriteFile",NULL);

    cb = pbih->biSizeImage;
    hp = (BYTE*)lpBits;
    while (cb > m_MAXWRITE)  {
            if (!WriteFile(hf, (LPSTR) hp, (int) m_MAXWRITE,
                          (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL))
			{
				//MessageBox(NULL,"","WriteFile",NULL);
				TRACE("WriteFile error: %d\n", GetLastError());

			}
            cb-= m_MAXWRITE;
            hp += m_MAXWRITE;
    }
    if (!WriteFile(hf, (LPSTR) hp, (int) cb,
         (LPDWORD) &dwTmp, (LPOVERLAPPED) NULL))
        MessageBox(NULL,"","WriteFile",NULL);

    if (!CloseHandle(hf))   MessageBox(NULL,"","CloseHandle",NULL);

    GlobalFree((HGLOBAL)lpBits);
}