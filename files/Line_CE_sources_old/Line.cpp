// Line.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Line.h"
#include "LineDlg.h"
#include "conio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma intrinsic(_outp)

//HANDLE hCom1=0;
HANDLE hCom2=0;
//HANDLE hEpp=0;

OVERLAPPED o;
DWORD dwEvtMask;

COMMTIMEOUTS oldTimeouts1;
COMMTIMEOUTS oldTimeouts2;

CWinThread* pThreadComIn=0;
CWinThread* pThreadComOut=0;
CWinThread* pThreadEppIn=0;

HANDLE hEventComOut=0;		//событие "Послать команды теста в Pic"
HANDLE hEventEppIn=0;		//событие "Принять массив"
HANDLE hEventStop=0;		//событие "Нажали кнопку "Стоп" "	

HANDLE hMutexO=0;			//для работы с bufOut и NumO
//HANDLE hMutexI_1=0;
HANDLE hMutexI=0;			//для работы c bufIn2
HANDLE hMutexRes=0;			//для работы c bufRes
HANDLE hMutexEpp=0;			//для работы c bufIn_Epp
HANDLE hMutexStop=0;		//для работы с bStop и bPusk;
HANDLE hMutexRukoyatka=0;	//для работы с m_iPosTek (состояние рукоятки);
HANDLE hMutexDCres_gotov=0; //для работы с res_gotov
 
/////////////////////////////////////////////////////////////////////////////
// CLineApp

BEGIN_MESSAGE_MAP(CLineApp, CWinApp)
	//{{AFX_MSG_MAP(CLineApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineApp construction

CLineApp::CLineApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLineApp object

CLineApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLineApp initialization

BOOL CLineApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	SetRegistryKey(_T("MNIPI"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	int j = 0;

    // Инициализируем СОМ2-порт
	ComInit2();
	if (!hCom2)
	{ 
	  j = AfxGetApp()->m_pMainWnd->MessageBox(_T("Ошибка инициализации порта COM1!\n\nЗавершить работу приложения?"),_T("Внимание!"),MB_YESNO|MB_ICONERROR);
	  if (j==IDYES) 
	  { if(hCom2)
		{ ::SetCommTimeouts(hCom2,&oldTimeouts2);
	      ::CloseHandle(hCom2);
		}
		return FALSE;
	  }
	}
	
    // Инициализируем порт
	EppInit();

	CLineDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK

	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
		int t=0;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CLineApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(hEventComOut) ::CloseHandle(hEventComOut);
	if(hEventEppIn) ::CloseHandle(hEventEppIn);
	if(hEventStop) ::CloseHandle(hEventStop);
	
	if(o.hEvent) ::CloseHandle(o.hEvent);//==================

	if(hMutexO) ::CloseHandle(hMutexO);
//	if(hMutexI_1) ::CloseHandle(hMutexI_1);
	if(hMutexI) ::CloseHandle(hMutexI);
	if(hMutexRes) ::CloseHandle(hMutexRes);
	if(hMutexEpp) ::CloseHandle(hMutexEpp);
	if(hMutexStop) ::CloseHandle(hMutexStop);
	if(hMutexRukoyatka) ::CloseHandle(hMutexRukoyatka);
	if(hMutexDCres_gotov) ::CloseHandle(hMutexDCres_gotov);

	if(hCom2)
	{ ::SetCommTimeouts(hCom2,&oldTimeouts2);
	  ::CloseHandle(hCom2);
	}
//	if (hEpp)
//	{ ::CloseHandle(hEpp);
//	}
	
	return CWinApp::ExitInstance();
}
/*
void CLineApp::ComInit1()
{	DCB dcb;
    FillMemory(&dcb,sizeof(dcb),0);
    COMMTIMEOUTS timeouts;

	DWORD dwErr;
	BOOL fS;

//SECURITY_ATTRIBUTES sec={sizeof (SECURITY_ATTRIBUTES),NULL,TRUE};

	hCom1= ::CreateFile(_T("COM1:"),GENERIC_READ|GENERIC_WRITE,0,NULL,
				       OPEN_EXISTING,0,NULL);

    if(hCom1==INVALID_HANDLE_VALUE)
	{
	  dwErr=::GetLastError();
	  TRACE(_T("Ошибка - %d при открытии COM1\n"),dwErr);
      hCom1=0;
	}
    else
	{
	  fS=::GetCommState(hCom1,&dcb);
      dcb.BaudRate=CBR_115200;
	  dcb.StopBits=ONESTOPBIT;
	  dcb.ByteSize=8;
	  dcb.fParity=TRUE;
      dcb.Parity=EVENPARITY;//NOPARITY;
	  dcb.fDtrControl=DTR_CONTROL_DISABLE;
	  dcb.fRtsControl=RTS_CONTROL_DISABLE;
	  fS=::SetCommState(hCom1,&dcb);

	  fS=::GetCommTimeouts(hCom1,&timeouts);
      oldTimeouts1=timeouts;
      timeouts.ReadIntervalTimeout=0;//100;
      timeouts.ReadTotalTimeoutMultiplier=200;//101;//1;
      timeouts.ReadTotalTimeoutConstant=2000;//1000;
      timeouts.WriteTotalTimeoutMultiplier=100;//1;//100;
      timeouts.WriteTotalTimeoutConstant=1000;//100;
      fS=::SetCommTimeouts(hCom1,&timeouts);

	  PurgeComm(hCom1,PURGE_TXCLEAR|PURGE_RXCLEAR);
	}
}
*/
void CLineApp::ComInit2()
{	DCB dcb;
    FillMemory(&dcb,sizeof(dcb),0);
    COMMTIMEOUTS timeouts;

	DWORD dwErr;
	BOOL fS;

//SECURITY_ATTRIBUTES sec={sizeof (SECURITY_ATTRIBUTES),NULL,TRUE};

	hCom2= ::CreateFile(_T("COM2:"),	//COM1-3343 (старыйобраз),//COM2-3343 (новый образ) COM2-3375
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,	//
				        CREATE_NEW,//OPEN_EXISTING,
						FILE_FLAG_OVERLAPPED,	//==========0,
						NULL);

    if(hCom2==INVALID_HANDLE_VALUE)
	{
	  dwErr=::GetLastError();
	  TRACE(_T("Ошибка - %d при открытии COM1\n"),dwErr);
      hCom2=0;
	}
    else
	{
	  fS=::GetCommState(hCom2,&dcb);
      dcb.BaudRate = CBR_38400;
	  dcb.StopBits = ONESTOPBIT;
	  dcb.ByteSize = 8;
	  dcb.fParity  = FALSE;//TRUE;
      dcb.Parity   = NOPARITY;//EVENPARITY;
	  dcb.fDtrControl=DTR_CONTROL_DISABLE;
	  dcb.fRtsControl=RTS_CONTROL_DISABLE;
	  fS=::SetCommState(hCom2,&dcb);

	  fS=::GetCommTimeouts(hCom2,&timeouts);
      oldTimeouts2=timeouts;
      timeouts.ReadIntervalTimeout=0;
      timeouts.ReadTotalTimeoutMultiplier=200;//101;//1;
      timeouts.ReadTotalTimeoutConstant=2000;//1000;
      timeouts.WriteTotalTimeoutMultiplier=100;//1;//100;
      timeouts.WriteTotalTimeoutConstant=1000;//100;
      fS=::SetCommTimeouts(hCom2,&timeouts);
//==========
	  SetCommMask(hCom2,EV_RXCHAR);//|EV_RLSD|EV_RING|EV_DSR|EV_CTS
	  o.hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	  ASSERT(o.hEvent);
//=========
	  PurgeComm(hCom2,PURGE_RXCLEAR|PURGE_TXCLEAR);
	}
}

/*
void CLineApp::EppInit()
{	
	DWORD dwErr;

	hEpp= ::CreateFile(_T("LPT1:"),GENERIC_READ,0,NULL,
				       OPEN_EXISTING,
					   0,			//или FILE_FLAG_OVERLAPPED ???
					   NULL);

    if(hEpp==INVALID_HANDLE_VALUE)
	{
	  dwErr=::GetLastError();
	  TRACE(_T("Ошибка - %d при открытии LPT1\n"),dwErr);
      hEpp=0;
	}
    else
	{ 
	  TRACE(_T("EppInit\n"));
//	  int port = _inp(0x378+2);
//	  port = port|0x29;				//D5<-1 , D0<-1, D3<-1
	  _outp(0x378+2,0x29);	 
	  PurgeComm(hEpp,PURGE_RXCLEAR);
	}
}
*/

void CLineApp::EppInit()
{	

	  TRACE(_T("EppInit\n"));
	  _outp(0x98,0x00);	 
	  _outp(0x99,0x03);	 
int	ens = _inp(0x79);		//запретить подачу сигнала на объект исследования
		ens = ens & 0xfd;
 		_outp(0x79,ens);	         //D1<-0 (ENB_PCM <- 0)
}
