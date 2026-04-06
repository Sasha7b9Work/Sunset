// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__465C06A3_71D8_4163_B3C8_F7FFA642E802__INCLUDED_)
#define AFX_STDAFX_H__465C06A3_71D8_4163_B3C8_F7FFA642E802__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#if defined(_WIN32_WCE) && (_WIN32_WCE >= 211) && (_AFXDLL)
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>

#include "afxmt.h"
#include <afxsock.h>		// MFC socket extensions

#include    <iphlpapi.h>

#define  ID_ENDLABELEDIT	WM_USER+30
#define SIZEOF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

#define	 PORT_EPP	0x378 + 4
#define  NE			50              // Макс. число тестируемых элементов в файле модели измерения
#define  NT			20              // Макс. число тестов для элемента
#define  NTEST		100             // Макс. число тестов в памяти
#define  NK			1000 			// Максим.число команд в массиве команд
#define  NBYTE      6			    // Максим.число байт в команде
#define  NREF		200				// Максим.число файлов эталонных графиков
#define  NFILE		200				// Максим.число файлов разных типов

#define  COLLECTOR	0
#define	 BAZA		1
#define  DOP		2
#define	 EMITTER	3
#define  GATE		4

#define  DIOD		0
#define	 RES		1
#define  NPN		2
#define	 PNP		3
#define  NMOS		4
#define  PMOS		5
#define  NPN4		6
#define	 PNP4		7
#define  NMOS4		8
#define  PMOS4		9
#define	 TS			10

/*
#define  IMP_POS	0				//вид развёртки: 0 - +IMP
#define	 DC_POS		1				//				 1 - +DC
#define  AC			2				//				 2 - ±AC
#define	 DC_NEG		3				//				 3 - -DC
#define  IMP_NEG	4				//				 4 - -IMP
*/
#define  TIP_SYN	0				//тип импульса: 0 - выпрямленный синус
#define	 TIP_IMP	1				//				1 - импульсы возрастающей амплитуды


#define  IMP_POS	0				//вид развёртки: 0 - +IMP
#define	 DC_POS		1				//				 1 - +DC
#define  SYN_POS	2				//вид развёртки: 2 - +SYN
#define  AC			3				//				 3 - ±AC
#define  SYN_NEG	4				//				 4 - -SYN
#define	 DC_NEG		5				//				 5 - -DC
#define  IMP_NEG	6				//				 6 - -IMP

#define  COUNT		4

#define  NSTEPQ1	200				// Число точек, снимаемых с коллектора 
									//(если аргумент - напряжение на коллекторе=>это есть число точек 1-ой очереди,
									// в противном => число точек 2-ой очереди)
extern   int NPOINT;

#define  NPOINTS 	200				// Число точек, снимаемых с ACP
#define  NSTEP		10				// MAX число шагов (ступенек)
#define  NREZ   	1000 //2000			// МАХ число результатов (NPOINT * NSTEP)
#define  NPOINT100 	100				// NPOINT / 2
#define  NMAS   	100 //50			    // МАХ число массивов результатов (для импульсного режима)
#define  POINTDC 	149				// № точки в DC

#define  AMPLITUDA 	357				// амплитуда ступеньки
#define  MAXRANGEIKOL 34			// число диапазонов по току в канале С (от 0 до 33)

#define  MEAS		0				// Измеренная кривая 
#define  REF		1				// Эталонная кривая (из памяти)
#define  CALC		2				// Расчетный график
#define  DIFF		3				// Расчетный график:разность ВАХ

#define  MEAS_SINGLE		0		// Измерение "однократное" 
#define  MEAS_MULTIPLE		1		// Измерение "многократное"

#define  HAND		0				// Способ измерения "ручной" (выбран один тест)
#define  AUTOMAT	1				// Способ измерения "автоматический" (выбран элемент (выполняются все тесты для этого элемента))

#define  LEFT		0				//Положение координаты (0,0)
#define	 CENTER		1
#define  RIGHT		2
 
#define  MARKER0	0				//№ активного маркера
#define	 MARKER1	1
#define  MARKER2	2

#define  STYLE_LINE			0		//Стиль кривой
#define	 STYLE_POINT		1
#define  STYLE_LINEPOINT	2

#define	 stateMin	0				//0-окно отчёта минимизировано, 1-развёрнуто;
#define  stateMax   1

#define	 TST_STANDART   0
#define  TST_USER       1

#define	 FILE_EXE		1			//тип файла
#define	 FILE_MOD		1
#define  FILE_TST		2
#define  FILE_TST_US	3
#define  FILE_REF		4
#define  FILE_XLS		5
#define  FILE_TXT		6	
#define  FILE_PSW		7

#define	 FILE_DELETE   0			//вид действия 
#define	 FILE_LOAD	   1
#define	 FILE_SAVE_AS  2
#define	 FILE_SAVE	   3
#define	 FILE_COPY	   5
#define	 FILE_MOVE	   6

#define  NBUF		512             // размер bufOut (для перeдачи команд в Pic)
#define  NBUFI		2               // размер bufComIn и bufComIn2
									// 2 - число байт в принимаемом коде физ.кнопки или ручки
									// (<код кнопки>+<данное>) 
#define  NBUFE		1600            // число байт результата (для приёма массива)(200 точек по 8 байт)
#define  NBUFE_AC	800             // число байт результата(для приёма массива АС)(100 точек по 8 байт)
#define  NBYTEBUT   1				// число байт, указыв. новое положение рукоятки (число импульсов от 
									// предыдущего положения рукоятки)               

#define  EPSILON    0.0000000000000001	// чтобы избежать деления на 0

#define  WM_BUTTON		WM_USER+5
#define  WM_ERROR		WM_USER+10
#define  WM_RESPONSE	WM_USER+15
//#define  WM_ERROR_EPP	WM_USER+20
#define  WM_MASSIV		WM_USER+25

//extern   HANDLE hCom1;
extern   HANDLE hCom2;
//extern   HANDLE hEpp;
extern   OVERLAPPED o;
extern   DWORD dwEvtMask;

extern   CWinThread* pThreadComIn;
extern   CWinThread* pThreadComOut;
extern   CWinThread* pThreadEppIn;

extern   HANDLE hEventComOut;	
extern   HANDLE hEventEppIn;
extern	 HANDLE hEventStop;	

extern   HANDLE hMutexO;
//extern   HANDLE hMutexI_1;
extern   HANDLE hMutexI;
extern   HANDLE hMutexRes;
extern   HANDLE hMutexEpp;
extern   HANDLE hMutexStop;
extern   HANDLE hMutexRukoyatka;
extern   HANDLE hMutexDCres_gotov;

extern   UINT   ThreadComIn(LPVOID pParam);
extern   UINT   ThreadComOut(LPVOID pParam);
extern   UINT   ThreadEppIn(LPVOID pParam);
extern	 UINT	nCountChildWnd;
extern	 BOOL	nCheckTree;
extern	 BOOL	nCheckOtcht;

//extern	 CString	 strpsw;
extern   CString     mas_rangeKolMeasI[MAXRANGEIKOL];
extern   CString     mas_rangeKolMeasU[13];
extern   CString	 mas_range_meas_bazadop_I[27];
extern   CString	 mas_range_meas_bazadop_U[6];
extern   CString	 mas_range_bias_bazadop_I[27];
extern   CString	 mas_range_bias_bazadop_U[6];
extern	 double		 BMCI[MAXRANGEIKOL];
extern	 double		 KMCI[MAXRANGEIKOL];
extern	 double      BMCI_DUBL[MAXRANGEIKOL];
extern	 double      KMCI_DUBL[MAXRANGEIKOL];
extern	 double		 BMCU[13];
extern	 double		 KMCU[13];
extern	 double		 BMBI[27];
extern	 double		 KMBI[27];
extern	 double		 BMBU[6];
extern	 double		 KMBU[6];
extern	 double		 BSBI[27];
extern	 double		 KSBI[27];
extern	 double		 BSBU[6];
extern	 double		 KSBU[6];
extern	 double		 BLBI[27];
extern	 double		 KLBI[27];
extern	 double		 BLBU[6];
extern	 double		 KLBU[6];

extern	 double		 BMDI[17];
extern	 double		 KMDI[17];
extern	 double		 BMDU[6];
extern	 double		 KMDU[6];
extern	 double		 BSDI[17];
extern	 double		 KSDI[17];
extern	 double		 BSDU[6];
extern	 double		 KSDU[6];
extern	 double		 BLDI[17];
extern	 double		 KLDI[17];
extern	 double		 BLDU[6];
extern	 double		 KLDU[6];

extern   BYTE	     nominal;				//= 184;номинальное значение кода сети (1 байт) 
											//общее для всех диапазонов
//extern	 int	     V_[5];					//={2220,3660,3660,3660,2920};//максимальный код ЦАПа Da 
											//для каждого диапазона (5 диапазонов)
extern	 BYTE		 Nz_max;				//= 145;//максимальный код ЦАПа Dz (1 байт)
extern	 BYTE		 Nz_min;				//= 4;	//минимальный код ЦАПа Dz (1 байт)

extern	 CString     fileKoefPsw;
extern   CString     strpsw;		//= _T("SMCE00001");	//заводской номер
extern	 CString	 m_sparol;		//пароль
extern   CString	 strFileName;	//имя текущего файла модели измерения
extern   CString	 strFileNoName;	//фильтр файла модели измерения
extern	 CString	 strFileUntitled;//== _T("\\DiskC\\Untitled.mod");
extern	 CString     nameTst;       //полный путь к файлу параметров
extern	 CString	strFileTstUser;
extern	 CString	strFileTstStandart;
extern	 CString	strPassTstUser;			//файлы *.tst пользовательских тестов
extern	 CString	strPassTstStandart;	//файлы *.tst стандартных тестов
extern	 CString		  strDateCurrent;

extern   CStringArray array_otcht;	// отчёт
extern	 CStringArray array_tst_standart;		// список стандартных тестов
extern	 CStringArray array_tst_user;			// список пользовательских тесто
extern	 CStringArray array_tst_prim;			// список комментарий тестов библиотеки

extern    CRect	     RectTree;		//зона управления выбором теста (дерево)
extern    CRect	     RectTuning;
extern    CRect	     RectOtcht;
extern    CImageList m_imageList;   //Список имеджей для дерева модели измерения

extern    BOOL	     bZapusk;		//вид запуска: 0 - "внутренний" (по кнопке "Пуск"или порукоядке),
									//             1 - "внешний" (по внешнему сигналу)
extern	  BOOL		 bAutomat;		//режим измерения: 0 - "ручной", 1 - "автоматический"

extern    CString    m_masStrEl[NE];		  //массив наименований элементов загруженной модели измерения
extern    CString    m_masStrMod[NE][NT];
extern    CString    str_EL,str_MOD,str_PRIM; //наименование текущего теста
extern	  CString	 strTestName;
extern    CString    str_NewEL,str_NewMOD;	  //новое наименование теста
extern    int		 i_EL;					  //текущий номер элемента
extern    int		 i_MOD;					  //текущий номер теста
extern    int		 i_SelUrov;				  //выделенный уровень в дереве модели измерения:0-нет,1-элемент,2-тест
extern	  int		 akt_button;	
extern    UINT	     n_Test;		//число тестов для выбранного элемента в автоматическом режиме,
									//в "ручном" n_Test = 1
extern	  int		 n_IPPP;		//модификация ИППП:
									//0 - ИППП-3(без переключателя (макс.ток 20А))
									//1 - ИППП-3/1(с переключателем (макс.ток 100А))
extern    int        m_iKnotBaza;	//0 - узел элемента не подключен (внешняя коммутация)
									//1 - подключен
extern	  int        m_iKnotKol;	//0 - узел элемента не подключен (внешняя коммутация)
extern	  int        m_iKnotDop;				//0 - узел элемента не подключен (внешняя коммутация)
extern    int        m_iKnopEm;				//0 - узел элемента не подключен (внешняя коммутация)
//extern UINT		m_udlitpauza;
extern UINT   n_Step;
extern UINT   n_Mas;
extern UINT   uPrecigion;
//extern int    nK41;
//extern int	  i_tipimp;
extern int	  size_point;

extern double*  pM[5];		//массив указателей на массивы результатов (pM1,pM2,pM3,pM4,pM5);
extern double pM1[NMAS][NREZ];	//массивы результатов (ACP1)
extern double pM2[NMAS][NREZ];	//(ACP2)
extern double pM3[NMAS][NREZ];	//(ACP3)
extern double pM4[NMAS][NREZ];	//(ACP4)
extern double pM5[NMAS][NREZ];	//расчетный массив

extern double bM1[NPOINT100];//массивы результатов для АС(ACP1) - Uc
extern double bM2[NPOINT100];//(ACP2)					 - Ic
extern double bM3[NPOINT100];//(ACP3)					 - база
extern double bM4[NPOINT100];//(ACP4)					 - доп.канал

extern double cM1[NPOINT100];//массивы результатов для АС(ACP1) - Uc
extern double cM2[NPOINT100];//(ACP2)					 - Ic
extern double cM3[NPOINT100];//(ACP3)					 - база
extern double cM4[NPOINT100];//(ACP4)					 - доп.канал

//extern int		Switch_temp;
//extern int		Switch_old;
extern int		n_Switch;				//0 - переключатель в положении 2000V
											//1 - переключатель в положении +5V
											//2 - переключатель в положении -5V

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__465C06A3_71D8_4163_B3C8_F7FFA642E802__INCLUDED_)
