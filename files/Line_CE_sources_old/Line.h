// Line.h : main header file for the LINE application
//

#if !defined(AFX_LINE_H__41E1E134_7051_44E4_A851_EB98A70E3169__INCLUDED_)
#define AFX_LINE_H__41E1E134_7051_44E4_A851_EB98A70E3169__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLineApp:
// See Line.cpp for the implementation of this class
//

class CLineApp : public CWinApp
{
public:
	CLineApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
//	void ComInit1();
	void ComInit2();
	void EppInit();

// Implementation

	//{{AFX_MSG(CLineApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINE_H__41E1E134_7051_44E4_A851_EB98A70E3169__INCLUDED_)
