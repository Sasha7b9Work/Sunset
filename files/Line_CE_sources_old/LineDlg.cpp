// LineDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "conio.h"
#include "afxmt.h"
#include "Winuser.h"

#include "Line.h"
#include "LineDlg.h"
#include "ParolDlg.h"
#include "ReparolDlg.h"
#include "SaveAsDlg.h"
#include "TreeViewDlg.h"
#include "TreeAddDlg.h"
#include "TreeAddFromPanelDlg.h"
#include "PartDlg.h"
#include "TestRenameDlg.h"
#include "TablDlg.h"
#include "SaveRefDlg.h"
#include "FreeMemoryDlg.h"
#include "SaveTablDlg.h"
#include "DelTstDlg.h"
#include "SaveTstDlg.h"
#include "ScalDlg.h"
#include "FileDlg.h"
#include "SaveOtchDlg.h"
#include "CopyFileDlg.h"
#include "LupaDlg.h"
#include "KalibrDlg.h"
#include "AutoSetNull.h"
#include "KoefDlg.h"
#include "ValEnter.h"
#include "IpInfo.h"
#include "ModificationDlg.h"
#include "DiffDlg.h"

#pragma intrinsic(_inp)
#pragma intrinsic(_outp)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_RERRESH WM_USER+1

//#define SIZEOF_ARRAY(a) (sizeof(a)/sizeof(a[0]))

UINT ThreadComIn(LPVOID pParam);
UINT ThreadComOut(LPVOID pParam);
UINT ThreadEppIn(LPVOID pParam);

//CCriticalSection criticalSection;
int nK41=0;
int n_obryv=0;							//счетчик числа обрывов при внешней коммутации

int nCountPointDC=0;
int n_pointDC = 1;
int ready = 0;
//int cikl = 0;
//int	  i_tipimp = 0;
int	    size_point = 0;					//размер точки
int		n_propusk=0;					//число пропускаемых измерений в реж. DC 
//BOOL	res_propusk=true;
BOOL	b_InitAxis=true;				//true - перерисовывать оси графика  при контроле параметров
BOOL	b_flagCheckParam = false;		//true - восстановить после измерения работу с маркерами
BOOL	b_flagStatusRef = false;		//true - восстановить после измерения эталонный график

BYTE		My_buf_But[NBUFI+1];			// -"-
//BYTE		My_buf_Err[NBUFI+1];			// -"-

BYTE		bufOut[NBUF];				// для передачи команды в СОМ-порт
BYTE		My_buf_Out[NBUF];			// -"-

BYTE		bufRes[NBUFI+1];			// -"-
BYTE		My_buf_Res[NBUFI+1];		// -"-

BYTE		bufComIn2[100];				// для приёма кода физической кнопки из СОМ-порта
BYTE		bufIn2[100];				// -"-

BYTE		bufEppIn[NBUFE];			// для приёма "готовности" и массива через EPP-порт
BYTE		bufIn_Epp[NBUFE];			
BYTE		My_buf_Mas[NBUFE];
//BYTE		My_buf_Err_Epp[NBUFI+1];			

DWORD		dwWrite=0;
DWORD		dwRead2=0;
DWORD		dwReadEpp=0;

int			NPOINT = 200;
int			NumO = 0;					//кол-во байт для передачи
int			NumI = 0;					//кол-во байт для приёма массива
BOOL	    bPusk		= FALSE;
BOOL		bStop		= TRUE;
BOOL	    bAutoSetNull= FALSE;
BOOL	    bAutoSetNullIc_10nA= FALSE;
BOOL		bKalibr		= false;
BOOL	    bStatusDiff = false;		//0 - вычитание ВАХ-ов не производить
										//1 - вычитание ВАХ производим по способу m_iDifference
BOOL		bMeasDiff	= false;		//0 - измерение ВАХ доп. эл-та выключено
										//1 - измерение ВАХ доп. эл-та включено

int		    MAS_RESULT[4][NPOINTS];		//массив результатов АЦП (4 АЦП по 200 точек)
int			stup  = 0;					//текущее значение №-ра ступеньки
//UINT        i_Error = 0;				//счетчик ошибок при измерении

BOOL		FlagOL[4];					//флаги переполнения АЦП
BOOL		Flag_sboy=false;
BOOL	    flag_G = 0;
static CString strDataFileRef[NREF][5];
typedef struct {
	LPTSTR psz1;
	LPTSTR psz2;
	LPTSTR psz3;
	LPTSTR psz4;
	LPTSTR psz5;
} ITEMDATA, *PITEMDATA; 

ITEMDATA* m_pDataFileRef[NREF];
static CString strData[NMAS][NREZ][7];
typedef struct {
	LPTSTR psz1;
	LPTSTR psz2;
	LPTSTR psz3;
	LPTSTR psz4;
	LPTSTR psz5;
	LPTSTR psz6;
	LPTSTR psz7;
} ITEMDATARES, *PITEMDATARES; 

//ITEMDATA* m_pData[NREZ];//[10]; 
ITEMDATARES* m_pData[NREZ*NMAS];//[10]; 

static UINT aSignal[]={IDC_STATICMIGSIGNAL};

static UINT aNoSignal[]={IDC_STATICNOMIGSIGNAL};

static UINT aSignalMemoryFull[]={IDC_STATIC_MEMORY_FULL};

static UINT aNoSignalMemoryFull[]={IDC_STATIC_MEMORY};

//Вид экрана
//==========
static UINT aVid1[]={IDC_LIST_RESULT};//IDC_STATICEDINFO};//,IDC_TABCTRLPARAM};

//отoбражение параметров теста
static UINT aVid1_OFF[]={IDC_STATICNOMIGSIGNAL,IDC_STATICMIGSIGNAL,IDC_STATICZONATEXT};
static UINT aVid2[]={IDC_MARKER1,IDC_MARKER2,IDC_TANGENT,//IDC_EDITDISPLAYMEAS,//IDC_EDITDISPLAYREF,
					 IDC_STATIC_MARKER1,IDC_STATIC_NAME_MARKER1,//	
					  IDC_STATIC_MARKER2,IDC_STATIC_NAME_MARKER2,//IDC_EDNSTEP,
					  IDC_BUTTON_MARKER_RESET,//IDC_BUTTON_LEFT,IDC_BUTTON_RIGHT,
					  IDC_STATIC_BUTTON_TANGENT,IDC_STATIC_BUTTON_TANGENT_ON,
						IDC_STATIC_MARKER1_ON,IDC_STATIC_MARKER2_ON};

static UINT aVid5[]={IDC_EDITOTCHT};
static UINT aVid6[]={IDC_LISTREF};

//Закладка "Схема включения":
//===========================
static UINT aPage0[]={IDC_STATIC_ELEMENT,IDC_COMBO_KOMMUTATOR,IDC_STATIC_VKL,//IDC_STATICTEXTPAGE0,IDC_EDKOLSOURCEU,
					  IDC_STATICBITMAP_DIOD,IDC_STATICBITMAP_RES,//IDC_BUTTON_RES,IDC_BUTTON_DIOD,
					  IDC_STATICBITMAP_NPN,IDC_STATICBITMAP_PNP,//IDC_BUTTON_NPN,IDC_BUTTON_PNP,
					  IDC_STATICBITMAP_NMOS,IDC_STATICBITMAP_PMOS,//IDC_BUTTON_NMOS,IDC_BUTTON_PMOS,							
					  IDC_STATICBITMAP_NPN4,IDC_STATICBITMAP_PNP4,//IDC_BUTTON_NPN4,IDC_BUTTON_PNP4,
					  IDC_STATICBITMAP_NMOS4,IDC_STATICBITMAP_PMOS4,//IDC_BUTTON_NMOS4,IDC_BUTTON_PMOS4,
					  IDC_STATICGNEZDO_C,IDC_STATICGNEZDO_B,IDC_STATICGNEZDO_S,IDC_STATICGNEZDO_E,	
					  IDC_COMBO_GNEZDO_C,IDC_COMBO_GNEZDO_B,IDC_STATIC_GNEZDO_S,IDC_COMBO_GNEZDO_E,	
					  IDC_STATICKNOT,
					  IDC_COMBOCHECKKOL,IDC_COMBOCHECKBAZA,IDC_COMBOCHECKDOP,//IDC_TEXTCOMBOCHECKEM,
					  IDC_STATICMOD,IDC_COMBOMOD//,IDC_BUTTON_LOADTST
					};
	
static UINT aElement_OFF[]={IDC_STATICBITMAP_DIOD,IDC_STATICBITMAP_RES,
							IDC_STATICBITMAP_NPN,IDC_STATICBITMAP_PNP,
							IDC_STATICBITMAP_NMOS,IDC_STATICBITMAP_PMOS,							
							IDC_STATICBITMAP_NPN4,IDC_STATICBITMAP_PNP4,
							IDC_STATICBITMAP_NMOS4,IDC_STATICBITMAP_PMOS4};

//недоступны для двухполюсников:
static UINT aPage0_bipolar2[]={IDC_STATICGNEZDO_B,IDC_STATICGNEZDO_S,IDC_COMBOCHECKBAZA,IDC_COMBOCHECKDOP,
								IDC_COMBO_GNEZDO_B,IDC_STATIC_GNEZDO_S};
//недоступны для трёхполюсников:
static UINT aPage0_bipolar3[]={IDC_STATICGNEZDO_S,IDC_COMBOCHECKDOP,IDC_STATIC_GNEZDO_S};
//недоступны если загрузка теста из дерева :
static UINT aPage0_loadtst[]={IDC_BUTTON_LOADTST};//,IDC_STATICMOD,IDC_COMBOMOD

//Закладка "Коллектор":
//===========================
static UINT aPage1_break[]={IDC_COMBOKOLSOURU};
			  

static UINT aPage1[]={IDC_STATICRAZV,IDC_STATICIMPPOSITIVE,IDC_STATICAC,IDC_STATICIMPNEGATIVE,
					  IDC_RADIO_RAZVIMPPOSITIVE,IDC_RADIO_RAZVIMPNEGATIVE,
					  IDC_RADIO_RAZVDCPOSITIVE,IDC_RADIO_RAZVDCNEGATIVE,IDC_RADIO_RAZVAC,IDC_STATICIMP,
					  IDC_EDITDLITIMP,//IDC_STATICDLITPAUZA,IDC_STATICTIPIMP,IDC_STATICTIPIMP1,
					  IDC_STATICIMP1POS,IDC_STATICIMP1NEG,//IDC_STATICTIPIMP1NEG,IDC_STATICTIPIMPNEG,
					  IDC_RADIO_SYNPOSITIVE,IDC_RADIO_SYNNEGATIVE,IDC_CHECKPAUZA,//IDC_COMBO_PERIOD,IDC_EDITDLITPAUZA,
					  IDC_EDKOLMEASU,IDC_EDKOLMEASI,IDC_COMBOKOLU,IDC_COMBOKOLI,//IDC_STATICKOLMEAS,
					  IDC_STATICKOLLECTORSOURCE,IDC_COMBOKOLSOURU,//IDC_EDKOLSOURCEU,
					  IDC_STATICLIMIT,IDC_SLIDERKOLI,//,IDC_EDSLIDERKOLI
					  IDC_TEXTRANGMINKOLI,IDC_EDTEXTRANGMAXKOLI,IDC_EDITLIMITKOLI,
					  IDC_EDSLIDERKOLU,IDC_SLIDERKOLU,IDC_STATICICONDCPOS,IDC_STATICICONDCNEGATIVE,
					  IDC_TEXTRANGMINKOLU,IDC_EDITLIMITKOLU,//,IDC_CHECKKOLREJHIGH,IDC_TEXTRANGMAXKOLU
					  IDC_STATICSTOP,IDC_STATICSTART};//IDC_COMBONAKOPLENIE,

//закладка "База"
//===========================
static UINT aPage2[]={IDC_STATICBAZA,IDC_EDBAZASOURCEUI,IDC_RADIOBAZASOURCEU,IDC_RADIOBAZASOURCEI,IDC_CHECK_BAZA_VKL,
					  IDC_TEXTAMPLITUDEBAZA,IDC_COMBOAMPLITUDEBAZA,IDC_TEXTSTEPBAZA,IDC_EDITSTEPBAZA,IDC_SPINSTEPBAZA,
					  IDC_TEXTPOLARITYSTEP,IDC_COMBOPOLARITYSTEP,
					  IDC_STATICBAZABIAS,IDC_EDITRANGBAZABIAS,//IDC_TEXTRANGBAZABIAS,
					  IDC_SLIDERBAZABIAS,IDC_TEXTMINBAZABIAS,IDC_EDITBAZABIAS,//IDC_TEXTMAXBAZABIAS,
					  IDC_TEXTPOLARITYBIAS,IDC_COMBOPOLARITYBAZABIAS,
					  IDC_CHECKBAZAREJHIGH,IDC_EDMEASBAZA,//IDC_RADIOMEASBAZAU,IDC_RADIOMEASBAZAI,
					  IDC_STATICLIMITBAZA,IDC_TEXTRANGLIMITBAZA,IDC_COMBOBAZALIMITMAXUI,
					  IDC_SLIDERLIMITBAZA,IDC_EDITLIMITBAZA,
					  IDC_TEXTMINLIMITBAZA};//,IDC_TEXTMAXLIMITBAZA

//закладка "Дополнительный канал"
//===============================
static UINT aPage3[]={IDC_STATICDOP,IDC_EDDOPSOURCEUI,IDC_RADIODOPU,IDC_RADIODOPI,
					  IDC_TEXTAMPLITUDEDOP,IDC_COMBOAMPLITUDEDOP,IDC_TEXTSTEPDOP,IDC_EDITSTEPDOP,IDC_SPINSTEPDOP,
					  IDC_TEXTPOLARITYSTEPDOP,IDC_COMBOPOLARITYSTEPDOP,
					  IDC_STATICDOPBIAS,IDC_EDITRANGDOPBIAS,//IDC_TEXTRANGDOPBIAS,
					  IDC_SLIDERDOPBIAS,IDC_TEXTMINDOPBIAS,IDC_EDITDOPBIAS,//IDC_TEXTMAXDOPBIAS,
					  IDC_TEXTPOLARITYBIASDOP,IDC_COMBOPOLARITYDOPBIAS,
					  IDC_EDMEASDOP,//IDC_RADIOMEASDOPU,IDC_RADIOMEASDOPI,
					  IDC_STATICLIMITDOP,IDC_TEXTRANGLIMITDOP,IDC_COMBODOPLIMITMAXUI,
					  IDC_SLIDERLIMITDOP,IDC_EDITLIMITDOP,
					  IDC_TEXTMINLIMITDOP};//IDC_EDITNAMEMEASDOP,,IDC_TEXTMAXLIMITDOP

//закладка "Расчёт"
//===========================
static UINT aPage4[]={IDC_ZONACHARACTERISTIC,IDC_AXISY,IDC_FORMULA,IDC_AXISX,IDC_ENDFORMULA,
//					  IDC_ZONAREJAXES,IDC_STATICREJX,IDC_COMBOREJX,IDC_STATICREJY,IDC_COMBOREJY,
					  IDC_ZONAPARAM,IDC_TEXTFIND,IDC_EDITNAMEPAR,IDC_TEXTFOR,IDC_EDITNAMEARG,
					  IDC_TEXT,IDC_TEXTUR1,IDC_EDITUR1,IDC_TEXTUR2,IDC_EDITUR2,
					  IDC_EDITPOINT,//IDC_TEXTPOINT,
					  IDC_ZONAFORMULA,IDC_RADIO1,IDC_ICONRADIO1,IDC_RADIO2,IDC_ICONRADIO2,
					  IDC_STATICPOROG,IDC_TEXTMIN,IDC_EDITMIN,IDC_TEXTMAX,IDC_EDITMAX};
//недоступны, если не заданы оба значения 1 и 2 для аргумента:
static UINT aFormula_OFF[]={IDC_RADIO1,IDC_ICONRADIO1,IDC_RADIO2,IDC_ICONRADIO2};//IDC_ZONAFORMULA,

//недоступны, если № ступеньки в базе и в доп.канале = 0:
static UINT aPoint_OFF[]={IDC_EDITPOINT};//IDC_TEXTPOINT,

//недоступны, если не задан параметр, аргумент и хотя бы одно значение аргумента:
static UINT aPorog_OFF[]={IDC_TEXTMIN,IDC_EDITMIN,IDC_TEXTMAX,IDC_EDITMAX};//IDC_STATICPOROG,

//Экран
//=====
//закладка "Шкала"//"Дисплей"
static UINT aTuningPage0[]  ={IDC_STATIC_NULL_LEFT,IDC_RADIO_LEFT,//IDC_STATIC_BUT_OFF_LEFT,IDC_STATIC_NULL_LEFT,
							  IDC_STATIC_NULL_CENTER,IDC_RADIO_CENTER,//IDC_STATIC_BUT_OFF_CENTER,IDC_STATIC_NULL_CENTER,
							  IDC_STATIC_NULL_RIGHT,IDC_RADIO_RIGHT,//IDC_STATIC_BUT_OFF_RIGHT,IDC_STATIC_NULL_RIGHT,
							  IDC_STATIC_ICON_INVERS,IDC_CHECK_INVERS_X,//IDC_STATIC_BUT_INVERS_OFF,IDC_STATIC_INVERSY_OFF,
							  IDC_CHECK_INVERS_Y,IDC_STATIC_TEXT_INVERSY,IDC_STATIC_FON,
							  IDC_CHECK_LOOPING,IDC_STATIC_ICON_LOOPING,
							  IDC_CHECK_FAMILY,IDC_STATIC_ICON_FAMILY,
							  IDC_CHECKKOLREJHIGH,IDC_STATIC_ICON_LUPA,
								IDC_BUTTON_MARKER_RESET};//,

static UINT aMarker_OFF[]   ={IDC_STATIC,IDC_STATIC_MARKER1,IDC_STATIC_MARKER2};//
static UINT aMarker_ON[]    ={IDC_STATIC,IDC_STATIC_MARKER1_ON,IDC_STATIC_MARKER2_ON};//

static UINT aTangent_OFF[]={IDC_STATIC_BUTTON_TANGENT};
static UINT aTangent_ON[] ={IDC_STATIC_BUTTON_TANGENT_ON};

//закладка "Маркеры"
static UINT aTuningPage1[]={IDC_STATIC,
							  IDC_STATIC_FON//IDC_STATIC_TEXT_TANGENT,/
							};

//закладка "Ошибки"
static UINT aTuningPage2[]={IDC_EDITERRORS};

//Массив наименований узлов (strKnot[элемент][канал])
//----------------------------------------------------
CString strKnot[11][4]={_T("a"),_T(" "),_T(" "),_T("k"),	//диод (двухполюсник): а-анод, k-катод
						_T("a"),_T(" "),_T(" "),_T("b"),	//резистор (двухполюсник): а- измеряемый электрод, b-опорный электрод
					    _T("c"),_T("b"),_T(" "),_T("e"),	//биполярный NPN-транзистор (трёхполюсный): с-коллектор, b-база, е-эмиттер
					    _T("c"),_T("b"),_T(" "),_T("e"),	//биполярный PNP-транзистор (трёхполюсный): с-коллектор, b-база, е-эмиттер
					    _T("d"),_T("g"),_T(" "),_T("s"),	//полевой или МОП транзистор NMOS(трёхполюсный): d-сток, g-затвор, s-исток
					    _T("d"),_T("g"),_T(" "),_T("s"),	//полевой или МОП транзистор PMOS(трёхполюсный): d-сток, g-затвор, s-исток
					    _T("c"),_T("b"),_T("s"),_T("e"),	//биполярный NPN-транзистор (четырёхполюсный): с-коллектор, b-база, s-подложка, е-эмиттер
					    _T("c"),_T("b"),_T("s"),_T("e"),	//биполярный PNP-транзистор (четырёхполюсный): с-коллектор, b-база, s-подложка, е-эмиттер
					    _T("d"),_T("g"),_T("b"),_T("s"),	//полевой или МОП транзистор NMOS(четырёхполюсный): d-сток, g-затвор, b-карман, s-исток,
					    _T("d"),_T("g"),_T("b"),_T("s"),	//полевой или МОП транзистор PMOS(четырёхполюсный): d-сток, g-затвор, b-карман, s-исток,
						_T("a"),_T("g"),_T("s"),_T("k")};	//тиристор TS

//Битмапы элементов (bmpEl[элемент])
//----------------------------------
static UINT bmpEl[11]={IDC_STATICBITELEMENT_DIOD, IDC_STATICBITELEMENT_RES,
					   IDC_STATICBITELEMENT_NPN,  IDC_STATICBITELEMENT_PNP,
					   IDC_STATICBITELEMENT_NMOS, IDC_STATICBITELEMENT_PMOS,
					   IDC_STATICBITELEMENT_NPN4, IDC_STATICBITELEMENT_PNP4,
					   IDC_STATICBITELEMENT_NMOS4,IDC_STATICBITELEMENT_PMOS4,
					   IDC_STATICBITELEMENT_RES
						};
//Битмапы режимов узлов(bmpRejKnot[узел][режим])
//----------------------------------------------
static UINT bmpRejKnot[5][7]={IDC_STATICBIT_OFF, IDC_STATUS_KOL_OFF, IDC_STATUS_KOL_ON,  IDC_STATUS_KOL_BAZA, IDC_STATUS_KOL_OFF_BAZA,IDC_STATICBIT_OFF,	IDC_STATICBIT_OFF,
							  IDC_STATICBIT_OFF, IDC_STATUS_BAZA_OFF,IDC_STATUS_BAZA_ON, IDC_STATUS_BAZA_KOL, IDC_STATUS_BAZA_EM,     IDC_STATUS_BAZA_EM,	IDC_STATICBIT_OFF,
							  IDC_STATICBIT_OFF, IDC_STATUS_DOP_OFF, IDC_STATUS_DOP_ON,  IDC_STATUS_DOP_EM,   IDC_STATICBIT_OFF,      IDC_STATICBIT_OFF,	IDC_STATICBIT_OFF,
							  IDC_STATICBITKNOT_E, IDC_STATUS_EM_ON,   IDC_STATUS_EM_BAZA, IDC_STATUS_EM_DOP,   IDC_STATUS_EM_BAZA_DOP, IDC_STATUS_EM1KOM_BAZA, IDC_STATUS_BAZA_EM1KOM_DOP,
							  IDC_STATICBIT_OFF, IDC_STATUS_GATE_OFF, IDC_STATUS_GATE_ON, IDC_STATUS_GATE_KOL, IDC_STATUS_GATE_EM,     IDC_STATUS_GATE_EM,	IDC_STATICBIT_OFF};

static UINT bmpRejKnotExtern[]={IDC_STATUS_KOL_OFF, IDC_STATUS_KOL_ON,
							   IDC_STATUS_BAZA_OFF,IDC_STATUS_BAZA_ON,//IDC_STATUS_DOP_OFF, IDC_STATUS_DOP_ON,
							   IDC_STATUS_EM_ON,   IDC_STATUS_EM_OFF,
							   IDC_STATUS_GATE_OFF, IDC_STATUS_GATE_ON};

//Иконки для отображения режима включения видов развертки в канале С
//-----------------------
static UINT aIconRejimChanelC[]={
								  IDC_STATICTIPIMP1,IDC_STATICTIPDCPOS,IDC_STATICTIPIMP,
								  IDC_STATICTIPAC,
								  IDC_STATICTIPIMPNEG,IDC_STATICTIPDCNEG,IDC_STATICTIPIMP1NEG,
								  IDC_STATICTIPCONSTPOS,IDC_STATICTIPCONSTNEG,
								  IDC_STATICTIPIMP2,IDC_STATICTIPIMP2NEG
								};
//Битмапы схемы включения
//-----------------------
static UINT aBmpCircuit_Off[]={
					   IDC_STATICBIT_OFF,IDC_STATICBIT_OFF,
					   IDC_STATICBITELEMENT_DIOD, IDC_STATICBITELEMENT_RES,
					   IDC_STATICBITELEMENT_NPN,  IDC_STATICBITELEMENT_PNP,
					   IDC_STATICBITELEMENT_NMOS, IDC_STATICBITELEMENT_PMOS,
					   IDC_STATICBITELEMENT_NPN4, IDC_STATICBITELEMENT_PNP4,
					   IDC_STATICBITELEMENT_NMOS4,IDC_STATICBITELEMENT_PMOS4,
					   IDC_STATICBITKNOT_E,//IDC_STATICBITEM_COMMON,
					   IDC_STATUS_KOL_OFF,IDC_STATUS_KOL_ON,IDC_STATUS_KOL_BAZA,IDC_STATUS_KOL_OFF_BAZA,
					   IDC_STATUS_BAZA_OFF,IDC_STATUS_BAZA_ON,IDC_STATUS_BAZA_KOL,IDC_STATUS_BAZA_EM,
					   IDC_STATUS_DOP_OFF,IDC_STATUS_DOP_ON,IDC_STATUS_DOP_EM,IDC_STATUS_EM_OFF, 
					   IDC_STATUS_EM_ON,IDC_STATUS_EM_BAZA,IDC_STATUS_EM_DOP,IDC_STATUS_EM_BAZA_DOP,IDC_STATUS_EM1KOM_BAZA,IDC_STATUS_BAZA_EM1KOM_DOP,
					   IDC_STATUS_GATE_EM,IDC_STATUS_GATE_OFF,IDC_STATUS_GATE_ON, IDC_STATUS_GATE_KOL};//,
//Массив значений диапазонов измерителя U (коллектор)
//----------------------------------------------------
CString mas_rangeKolMeasU[]={_T("5.0kV"),_T("2.0kV"),_T("1.0kV"),
							 _T("500V"), _T("200V"), _T("100V"), 
							 _T("50V"),  _T("20V"),  _T("10V"), 
							 _T("5V"),	 _T("2V"),   _T("1V"),   _T("500mV")};

double mas_d_rangeKolMeasU[]={5000.0,2000.0,1000.0,
							   500.0, 200.0, 100.0,
							    50.0,  20.0,  10.0,
							     5.0,   2.0,   1.0,  0.5};
//Массив значений диапазонов измерителя I (коллектор)
//----------------------------------------------------

CString mas_rangeKolMeasI[]={_T("50A"),  _T("20A"),   _T("10A"),  _T("5A"),    
							 _T("50A"),  _T("20A"),   _T("10A"),  _T("5A"),    _T("2A"),   _T("1A"),   _T("500mA"),
							 _T("200mA"),_T("100mA"), _T("50mA"), _T("20mA"),  _T("10mA"), _T("5mA"),
							 _T("2mA"),  _T("1mA"),   _T("500uA"),_T("200uA"), _T("100uA"),_T("50uA"),
							 _T("20uA"), _T("10uA"),  _T("5uA"),  _T("2uA"),   _T("1uA"),  _T("500nA"),
							 _T("200nA"),_T("100nA"), _T("50nA"), _T("20nA"),  _T("10nA")};//, _T("5nA"),
							 //_T("2nA"),  _T("1nA")};

double mas_d_rangeKolMeasI[]={ 50.000000000, 20.000000000,10.000000000, 5.000000000,
							   50.000000000, 20.000000000,10.000000000, 5.000000000, 2.000000000, 1.000000000, 0.500000000,
							    0.200000000,  0.100000000, 0.050000000, 0.020000000, 0.010000000, 0.005000000,
							    0.002000000,  0.001000000, 0.000500000, 0.000200000, 0.000100000, 0.000050000,
							    0.000020000,  0.000010000, 0.000005000, 0.000002000, 0.000001000, 0.000000500,
							    0.000000200,  0.000000100, 0.000000050, 0.000000020, 0.000000010, 0.000000005,
							    0.000000002,  0.000000001};

//Массив значений диапазонов источника U (коллектор)
//----------------------------------------------------
CString mas_rangeKolSourceU[]={_T("5V"),_T("20V"),_T("100V"),_T("500V"),_T("2000V")};//_T("3000V")};

double mas_d_rangeKolSourceU[]={5.0,20.0,100.0,500.0,2000.0};//3000.0};

//Массив значений амплитуды ступеньки источника U (база, доп. канал)
//---------------------------------------------------------
CString  mas_range_ampl_bazadop_U[]={_T("  2 V"),_T("  1 V "),_T("500 mV"),//_T("0.5 V"),
									 _T("200 mV"),_T("100 mV"), _T("50 mV")};//_T("0.2 V"),_T("0.1 V"),
double mas_d_range_ampl_bazadop_U[]={2.000000000,1.000000000,0.500000000,
									 0.200000000,0.100000000,0.050000000};//,
//Массив значений смещения ступеньки источника U (база, доп. канал)
//---------------------------------------------------------
CString  mas_range_bias_bazadop_U[]={_T("  20 V"),_T("  10 V"),_T("   5 V"),
									 _T("   2 V"),_T("   1 V"),_T(" 0.5 V")};//,
double mas_d_range_bias_bazadop_U[]={20.00000000,10.00000000,5.000000000,
									 2.000000000,1.000000000,0.500000000};//,

//Массив значений диапазонов измерителя U (база, доп. канал)
//---------------------------------------------------------
CString  mas_range_meas_bazadop_U[]  ={_T(" 40 V"),_T(" 20 V"),_T(" 10 V"),
									   _T("  4 V"),_T("  2 V"),_T("  1 V")};//,

double mas_d_range_meas_bazadop_U[]  ={40.00000000,20.00000000,10.00000000,
										4.000000000,2.000000000,1.000000000};//,

//Массив значений диапазонов источника/измерителя I (база, доп. канал)
//---------------------------------------------------------------------
CString  mas_range_ampl_bazadop_I[]=
							{   _T("  1 A "),_T("500 mA"),_T("200 mA"),
								_T("100 mA"),_T(" 50 mA"),_T(" 20 mA"),
								_T(" 10 mA"),_T("  5 mA"),_T("  2 mA"),
								_T("  1 mA"),_T("500 uA"),_T("200 uA"),
								_T("100 uA"),_T(" 50 uA"),_T(" 20 uA"),
								_T(" 10 uA"),_T("  5 uA"),_T("  2 uA"),
								_T("  1 uA"),_T("500 nA"),_T("200 nA"),
								_T("100 nA"),_T(" 50 nA"),_T(" 20 nA"),
								_T(" 10 nA"),_T("  5 nA"),_T("  2 nA")};//,_T("  1 nA"),_T("0.5 nA"),_T("0.2 nA")};

double mas_d_range_ampl_bazadop_I[]=
							{	1.000000000,0.500000000,0.200000000,
								0.100000000,0.050000000,0.020000000,
								0.010000000,0.005000000,0.002000000,
								0.001000000,0.000500000,0.000200000,
								0.000100000,0.000050000,0.000020000,
								0.000010000,0.000005000,0.000002000,
								0.000001000,0.000000500,0.000000200,
								0.000000100,0.000000050,0.000000020,
								0.000000010,0.000000005,0.000000002};//,0.000000001,0.0000000005,0.0000000002};

CString  mas_range_bias_bazadop_I[]= 
							{   _T(" 10 A "),_T("  5 A "),_T("  2 A "),
								_T("  1 A "),_T("500 mA"),_T("200 mA"),
								_T("100 mA"),_T(" 50 mA"),_T(" 20 mA"),
								_T(" 10 mA"),_T("  5 mA"),_T("  2 mA"),
								_T("  1 mA"),_T("500 uA"),_T("200 uA"),
								_T("100 uA"),_T(" 50 uA"),_T(" 20 uA"),
								_T(" 10 uA"),_T("  5 uA"),_T("  2 uA"),
								_T("  1 uA"),_T("500 nA"),_T("200 nA"),
								_T("100 nA"),_T(" 50 nA"),_T(" 20 nA")};//,_T(" 10 nA"),_T("  5 nA"),_T("  2 nA")};

double mas_d_range_bias_bazadop_I[]= 
							{   10.00000000,5.000000000,2.000000000,
								1.000000000,0.500000000,0.200000000,
								0.100000000,0.050000000,0.020000000,
								0.010000000,0.005000000,0.002000000,
								0.001000000,0.000500000,0.000200000,
								0.000100000,0.000050000,0.000020000,
								0.000010000,0.000005000,0.000002000,
								0.000001000,0.000000500,0.000000200,
								0.000000100,0.000000050,0.000000020};//,0.000000010,0.000000005,0.000000002};
CString  mas_range_meas_bazadop_I[]={_T("20 A"),
								_T(" 10 A "),_T("  4 A "),_T("  2 A "),
								_T("  1 A "),_T("400 mA"),_T("200 mA"),
								_T("100 mA"),_T(" 40 mA"),_T(" 20 mA"),
								_T(" 10 mA"),_T("  4 mA"),_T("  2 mA"),
								_T("  1 mA"),_T("400 uA"),_T("200 uA"),
								_T("100 uA"),_T(" 40 uA"),_T(" 20 uA"),
								_T(" 10 uA"),_T("  4 uA"),_T("  2 uA"),
								_T("  1 uA"),_T("400 nA"),_T("200 nA"),
								_T("100 nA"),_T(" 40 nA")};//,_T(" 20 nA"),_T(" 10 nA"),_T("  4 nA")};
double mas_d_range_meas_bazadop_I[]= { 20.00000000,
							    10.00000000,4.000000000,2.000000000,
								1.000000000,0.400000000,0.200000000,
								0.100000000,0.040000000,0.020000000,
								0.010000000,0.004000000,0.002000000,
								0.001000000,0.000400000,0.000200000,
								0.000100000,0.000040000,0.000020000,
								0.000010000,0.000004000,0.000002000,
								0.000001000,0.000000400,0.000000200,
								0.000000100,0.000000040};//,0.000000020,0.000000010,0.000000004};
//Дискреты Коллектор (U - 12 диапазонов, I - 36 диапазонов)
//=========================================================
double koef_kol_U[]  = {0.223,
						0.0892,0.0446,0.0223,
						0.00892,0.00446,0.00223,
						0.000892,0.000446,0.000223,
						0.0000892,0.0000446,0.0000223};

double koef_kol_I[]  = {0.00220,0.00088,0.00044,0.00022,
						0.00220,0.00088,0.00044,0.00022,
						0.000088,0.000044,0.000022,
						0.0000088,0.0000044,0.0000022,
						0.00000088,0.00000044,0.00000022,
						0.000000088,0.000000044,0.000000022,
						0.0000000088,0.0000000044,0.0000000022,
						0.00000000088,0.00000000044,0.00000000022,
						0.000000000088,0.000000000044,0.000000000022,
						0.0000000000088,0.0000000000044,0.0000000000022,
						0.00000000000088,0.00000000000044,
						0.00000000000018,1.0,1.0};

//Дискреты База-подложка (U - 6(9) диапазонов, I - 27(30) диапазонов)
//============================================================
//Амплитуда/cмещение
double koeff_bazadop_U[]   =   {0.0056,0.0028,0.0014,
							    0.00056,0.00028,0.00014};
double koeff_bazadop_I[]   =   {0.0028,0.0014,0.00056,
								0.00028,0.00014,0.000056,
								0.000028,0.000014,0.0000056,
								0.0000028,0.0000014,0.00000056,
								0.00000028,0.00000014,0.000000056,
								0.000000028,0.000000014,0.0000000056,
								0.0000000028,0.0000000014,0.00000000056,
								0.00000000028,0.00000000014,0.000000000056,
								0.000000000028,0.000000000014,0.0000000000056};
//Ограничение
double koeff_limit_bazadop_U[] = {  0.2,0.1,0.05,
								    0.02,0.01,0.005,
								    0.005,0.005,0.005};
double koeff_limit_bazadop_I[] = {  0.1,0.05,0.02,
									0.01,0.005,0.002,								
									0.001,0.0005,0.0002,								
									0.0001,0.00005,0.00002,								
									0.00001,0.000005,0.000002,								
									0.000001,0.0000005,0.0000002,						
									0.0000001,0.00000005,0.00000002,
									0.00000001,0.000000005,0.000000002,
									0.000000001,0.0000000005,0.0000000002};
  //Измеритель
double koeff_meas_bazadop_U[] =   { 0.00178,0.000892,0.000446,
								    0.000178,0.0000892,0.0000446};

double koeff_meas_bazadop_I[] =   { 0.000892,0.000446,0.000178,
                                    0.0000892,0.0000446,0.0000178,
									0.00000892,0.00000446,0.00000178,
									0.000000892,0.000000446,0.000000178,
									0.0000000892,0.0000000446,0.0000000178,
									0.00000000892,0.00000000446,0.00000000178,
									0.000000000892,0.000000000446,0.000000000178,
									0.0000000000892,0.0000000000446,0.0000000000178,
									0.00000000000892,0.00000000000446,0.00000000000178};

int		n_Switch		= 0;				//0 - переключатель в положении 2000V
											//1 - переключатель в положении +5V
											//2 - переключатель в положении -5V
											//3 - переключатель в нейтральном положении
											//4 - переключатель неисправлен

UINT	m_udlitpauza;				//0 - скважность х 2 выкл, 1 - вкл.
int     m_iKnotBaza=1;				//0 - узел элемента не подключен (внешняя коммутация)
int     m_iKnotKol=1;				//0 - узел элемента не подключен (внешняя коммутация)
int     m_iKnotDop=1;				//0 - узел элемента не подключен (внешняя коммутация)
int     m_iKnotEm=1;				//0 - узел элемента не подключен (внешняя коммутация)

//=================КАЛИБРОВКА==============
//=========================================
//ИППП-3
//=======
CString fileKoefPsw		= _T("\\Hard Disk\\Psw\\SMCE00000.psw");	//SMCE00006.psw");	//паспортный файл прибора (XACE00001.psw-характериогаф, SMCE0000n - ИППП-3)
CString strpsw			= _T("000");	//_T("SMCE00006");	//заводской номер 
CString	strdatekalibr	= _T("20.10.14");
int		n_IPPP			= 1;    			//модификация ИППП:
											//0 - без переключателя (макс.ток 50А)(ИППП-3)
											//1 - с переключателем (макс.ток 200А) (Характериограф и ИППП-3/1)
BYTE	nominal			= 184;				//номинальное значение кода сети (1 байт) 
											//общее для всех диапазонов
														  //в зависимости от длит.импульса 
BYTE	Nz_max			= 180;//130;//145;	//максимальный код ЦАПа Dz (1 байт)
BYTE	Nz_min			= 4;				//минимальный код ЦАПа Dz (1 байт)
BYTE	mas_Nz_max[]    = {180,90,90,90,90,90,180};//массив максимальных кодов ЦАПа Dz
											       //в зависимости от длит.импульса 

CString	mas_dlit_imp[]  = {_T("10 ms"),_T("2 ms"),_T("1 ms"),_T("0.5 ms"),_T("0.2 ms"),_T("0.2 ms"),_T("10 ms")};//ИППП-3

//число точек результата (в зависимости от длительности импульса)
int		mas_n_points[]  = {200,28,14,8,5,5,200}; //ИППП-3										

UINT	n_Mas = 1;					//число массивов (импульсов) на одном цикле измерения
int     n_t = 1;					//№ точки в конце импульса, которую берём в расчёт (1=>последнюю)

//Массив длительностей скважности (mas_dlit_pauza[индекс длительности импульса])
//------------------------------------------------------------------------------

//Калибровочные коэффициенты Коллектор (U - 13 диапазонов, I - 34 диапазонa)
//===========================================================================

double BMCU[] =  {0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0};
double BMCI[] =  {0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double BMCI_DUBL[] =  {0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0,0.0,
				  0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double KMCU[] =  {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0};
double KMCI[] =  {1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};
double KMCI_DUBL[] =  {1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0,1.0,
				  1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};

//Калибровочные коэффициенты База (источник)
//==============================================

//Источник(U - 6 диапазонов, I - 27 диапазонов)
//=============================================
//Амплитуда + Смещение
double BSBU[]=  {0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double BSBI[]=  {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				 0.0,0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double KSBU[]=  {1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};			
double KSBI[]=  {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
			     1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
			     1.0,1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};		
//Ограничение 
double BLBU[]=  {0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double BLBI[]=  {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				 0.0,0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double KLBU[]=  {1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};			
double KLBI[]=  {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
			     1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
			     1.0,1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};		
//Измеритель(U - 6 диапазонов, I - 27 диапазонов)
//=============================================
double BMBU[]=  {0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double BMBI[]=  {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
				 0.0,0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double KMBU[]=  {1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};			
double KMBI[]=  {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
			     1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
			     1.0,1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};
				 
//Калибровочные коэффициенты Подложка (источник)
//================================================

//Подложка Источник(U - 6 диапазонов, I - 17 диапазонов)
//======================================================
//Амплитуда + Смещение
double BSDU[]=  {0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double BSDI[]=  {0.0,0.0,
                 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0};//,0.0,0.0,0.0};
double KSDU[]=  {1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};			
double KSDI[]=  {1.0,1.0,
                 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0};//,1.0,1.0,1.0};	
//Ограничение 
double BLDU[]=  {0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double BLDI[]=  {0.0,0.0,0.0,
                 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0};//,0.0,0.0,0.0};
double KLDU[]=  {1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};			
double KLDI[]=  {1.0,1.0,1.0,
                 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0};//,1.0,1.0,1.0};  
//Подложка Измеритель(U - 6 диапазонов, I - 17 диапазонов)
//========================================================
double BMDU[]=  {0.0,0.0,0.0,0.0,0.0,0.0};//,0.0,0.0,0.0};
double BMDI[]=  {0.0,0.0,
                 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0,
				 0.0,0.0,0.0};//,0.0,0.0,0.0};
double KMDU[]=  {1.0,1.0,1.0,1.0,1.0,1.0};//,1.0,1.0,1.0};			
double KMDI[]=  {1.0,1.0,
                 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0,
				 1.0,1.0,1.0};//,1.0,1.0,1.0};  

CString mas_cifra_axes_X[3][3]={_T(" 0.0       0.1       0.2       0.3       0.4     0.5      0.6       0.7       0.8       0.9       1.0  "),
								_T(" 0.0       0.25     0.5      0.75     1.0      1.25     1.5      1.75      2.0       2.25      2.5    "),
								_T(" 0.0       0.5      1.0      1.5       2.0      2.5      3.0       3.5       4.0       4.5       5.0   "),
								_T("-1.0      -0.8     -0.6     -0.4      -0.2      0.0      0.2       0.4       0.6       0.8       1.0   "),
								_T("-2.5      -2.0     -1.5     -1.0      -0.5      0.0      0.5       1.0    	1.5       2.0       2.5   "),
								_T("-5.0      -4.0     -3.0     -2.0      -1.0      0.0      1.0       2.0       3.0       4.0       5.0   "),
								_T("-1.0      -0.9     -0.8     -0.7      -0.6     -0.5     -0.4      -0.3      -0.2      -0.1       0.0   "),
								_T("-2.5      -2.25    -2.0     -1.75    -1.5    -1.25    -1.0     -0.75    -0.5     -0.25     0.0         "),
								_T("-5.0      -4.5     -4.0     -3.5     -3.0      -2.5     -2.0      -1.5      -1.0      -0.5       0.0   ")};

CString mas_cifra_axes_Y[3][3] = {_T("0.0      0.1      0.2       0.3      0.4       0.5      0.6       0.7       0.8      0.9      1.0  "),
								  _T("0.0     0.25     0.5      0.75      1.0      1.25      1.5      1.75     2.0     2.25     2.5      "),
								  _T("0.0     0.5       1.0       1.5       2.0       2.5       3.0       3.5       4.0      4.5      5.0   "),
								  _T("-1.0    -0.8    -0.6      -0.4     -0.2       0.0      0.2       0.4       0.6      0.8      1.0   "),
								  _T("-2.5    -2.0    -1.5     -1.0      -0.5       0.0      0.5       1.0       1.5      2.0      2.5   "),
								  _T("-5.0    -4.0    -3.0      -2.0     -1.0       0.0      1.0       2.0       3.0      4.0      5.0   "),
								  _T("-1.0    -0.9    -0.8     -0.7      -0.6     -0.5      -0.4     -0.3     -0.2      -0.1      0.0       "),
								  _T("-2.5   -2.25    -2.0    -1.75    -1.5    -1.25     -1.0     -0.75    -0.5    -0.25     0.0        "),
								  _T("-5.0    -4.5    -4.0     -3.5      -3.0      -2.5      -2.0     -1.5     -1.0      -0.5      0.0   ")};

double  cifra_axes[3][3][2]={0.0,1.0,
							 0.0,2.0,
							 0.0,5.0,
							-1.0,1.0,
							-2.0,2.0,
							-5.0,5.0,
							-1.0,0.0,
							-2.0,0.0,
							-5.0,0.0};

  double  big_tick[3][3] = {0.1,0.2,0.5,0.2,0.4,1.0,0.1,0.2,0.5};

double  X_axis[NREZ];			//массив координат по Х (if по X - не Uc)
double  Y_axis[NREZ];			//массив координат по Y (if по Y - не Ic и не Uc)
double  x_axis[NPOINTS];
double  y_axis[NPOINTS];

double  refX[NMAS][NREZ];				//массив координат по Х (ссылка)
double  refY[NMAS][NREZ];				//массив координат по Y (ссылка)
double  diffX[NMAS][NREZ];				//массив координат по Х (для разности ВАХ)
double  diffY[NMAS][NREZ];				//массив координат по Y (для разности ВАХ)

CRect	RectWnd;
CRect	RectTablRes;
CRect	RectButtons;		//зона виртуальных кнопок управления
CRect	RectButtonFile;
CRect	RectButtonTest;
CRect	RectButtonTuning;
CRect	RectButtonMemory;
CRect	RectButtonGraf;
CRect	RectButtonOtcht;
CRect	RectButtonReset;
CRect   RectButtonMeas;

CRect	RectPanel;			//зона управления		
CRect	RectInfoTest;		//зона-info
CRect	RectEdDate;
CRect   RectEdTime;	
CRect	RectEdFile;
CRect	RectEdInfo;
CRect	RectMigSignal;
CRect	RectEdInfoMeas;

CRect	RectPicFreeMemory;
CRect	RectProgressFreeMemory;
CRect	RectTextFreeMemory;

CRect	RectParam;			//зона задания параметров теста
CRect   RectPage12;			//параметры
CRect   RectPage34;
CRect	RectDisplayMeas;	//зона отображения параметров теста

CRect	RectRejim;			//отображение Режима включения видов сигналов
CRect   RectChanelC;
CRect   RectChanelB;
CRect   RectRejimChanelC;
CRect   RectRejimChanelB;
CRect   RectRejimChanelCB;
CRect	RectQue1;
CRect	RectTextQue1;

							//закладка "Схема включения"
CRect	RectTextZagolovok;
CRect	RectCategoria;		//Элемент

CRect	RectRadioDiod;
CRect	RectRadioRes;
CRect	RectRadioNpn;
CRect	RectRadioPnp;
CRect	RectRadioNmos;
CRect	RectRadioPmos;
CRect	RectRadioNpn4;
CRect	RectRadioPnp4;
CRect	RectRadioNmos4;
CRect	RectRadioPmos4;

CRect	RectBitmapDiod;
CRect	RectBitmapRes;
CRect	RectBitmapNpn;
CRect	RectBitmapPnp;
CRect	RectBitmapNmos;
CRect	RectBitmapPmos;
CRect	RectBitmapNpn4;
CRect	RectBitmapPnp4;
CRect	RectBitmapNmos4;
CRect	RectBitmapPmos4;
CRgn	RgnDiod;
CRgn	RgnRes;
CRgn	RgnNpn;
CRgn	RgnPnp;
CRgn	RgnNmos;
CRgn	RgnPmos;
CRgn	RgnNpn4;
CRgn	RgnPnp4;
CRgn	RgnNmos4;
CRgn	RgnPmos4;

CRect	RectKnot;			//Коммутатор
CRect	RectKommutator;
CRect	RectKnotKol;
CRect	RectKnotBaza;
CRect	RectKnotDop;
CRect	RectKnotEm;
CRect	RectStatusKol;
CRect	RectStatusBaza;
CRect	RectStatusDop;
CRect	RectStatusEm;
CRect	RectTest;			//Тест
CRect	RectComboTest;
CRect	RectButtonLoadTst;
CRect	RectBitmapCircuit;	//отображение схемы включения
CRect	RectBitElement;
CRect   RectBitKnotKol;
CRect   RectBitKnotBaza;
CRect   RectBitKnotDop;
CRect   RectBitKnotEm;
CRect   RectBitKol;
CRect   RectBitBaza;
CRect   RectBitDop;
CRect   RectBitEm;
							//закладка "Коллектор"
CRect	RectRazv;			//Развёртка
CRect	RectRazvRadImpPos;
CRect	RectRazvRadDcPos;
CRect	RectRazvRadSynPos;
CRect	RectRazvRadAc;
CRect	RectRazvRadSynNeg;
CRect	RectRazvRadDcNeg;
CRect	RectRazvRadImpNeg;
CRect	RectIconImpPos;
CRect	RectIconSynPos;
CRect	RectIconDcPos;
CRect	RectIconAc;
CRect	RectIconSynNeg;
CRect	RectIconDcNeg;
CRect	RectIconImpNeg;
CRect   RectStatImp;		//Импульс
CRect   RectStatDlitPauza;
CRect	RectDlitImp;
CRect	RectPeriod;
CRect	RectNakoplenie;
CRect   RectKolMeas;		//Измеритель
CRect   RectU;
CRect   RectI;
CRect   RectKU;
CRect   RectKI;
CRect   RectKolSource;		//Источник
CRect	RectTextSourceKolU;
CRect	RectKolSourU;
CRect	RectLimit;			//Ограничение
CRect   RectTextSliderKolU;
CRect	RectSliderKolU;
CRect   RectTextStop;
CRect   RectTextStart;
CRect	RectTextRangMaxKolU;
CRect   RectEditSliderKolU;
CRect   RectTextSliderKolI;
CRect	RectSliderKolI;;
CRect	RectTextRangMinKolI;
CRect	RectTextRangMaxKolI;
CRect   RectEditSliderKolI;
CRect	RectKolRejHigh;		//Высокое разрешение по коллектору
CRect   RectIconLupa;
CRect	RectIconTipImp;
CRect	RectIconTipImp1;
							//закладка "База"
CRect	RectBaza;			//генератор ступенек	
CRect	RectBazaU;
CRect	RectBazaI;
CRect	RectBazaVkl;
CRect	RectEditBazaSourceUI;
CRect	RectBitmapBazaSourceUI;
CRect	RectTextAmplitude;	//амплитуда
CRect	RectComboAmplitude;
CRect	RectTextStep;
CRect	RectEditStepBaza;
CRect	RectSpinStepBaza;
CRect	RectBazaRejHigh;
CRect	RectTextPolarityStep;
CRect	RectComboPolarityStep;
CRect	RectTextBazaBias;
CRect	RectRangBazaBias;
CRect	RectSliderBazaBias;
CRect	RectMinBazaBias;
CRect	RectMaxBazaBias;
CRect	RectEditBazaBias;
CRect	RectTextPolarityBias;
CRect	RectComboPolarityBazaBias;
CRect	RectEditMeasBaza;
CRect	RectTextLimitBaza;
CRect	RectTextRangLimitBaza;
CRect	RectComboBazaSourceUI;
CRect	RectSliderLimitBaza;
CRect	RectEditLimitBaza;
CRect	RectTextMinLimitBaza;
CRect	RectTextMaxLimitBaza;

CRect	RectDop;				//доп. канал
CRect	RectDopU;
CRect	RectDopI;
CRect	RectEditDopSourceUI;
CRect	RectTextAmplitudeDop;
CRect	RectComboAmplitudeDop;
CRect	RectTextStepDop;
CRect	RectEditStepDop;
CRect	RectSpinStepDop;
CRect	RectTextPolarityStepDop;
CRect	RectComboPolarityStepDop;
CRect	RectTextDopBias;
CRect	RectRangDopBias;
CRect	RectSliderDopBias;
CRect	RectMinDopBias;
CRect	RectMaxDopBias;
CRect	RectEditDopBias;
CRect	RectTextPolarityBiasDop;
CRect	RectComboPolarityDopBias;
CRect	RectEditMeasDop;
CRect	RectTextLimitDop;
CRect	RectTextRangLimitDop;
CRect	RectComboDopSourceUI;
CRect	RectSliderLimitDop;
CRect	RectEditLimitDop;
CRect	RectTextMinLimitDop;
CRect	RectTextMaxLimitDop;
								 //расчёт
CRect	RectZonaCharacteristic;  //измеряемая характеристика
CRect	RectAxisY;
CRect	RectAxisX;
CRect	RectFormula;
CRect	RectEndFormula;
CRect	RectZonaRejAxes;
CRect	RectStRejX;
CRect	RectStRejY;
CRect	RectRejX;
CRect	RectRejY;
CRect	RectZonaPar;
CRect	RectTextFind;
CRect	RectEditPar;
CRect	RectTextFor;
CRect	RectEditArg;
CRect	RectText;
CRect	RectTextUr1;
CRect	RectEditUr1;
CRect	RectTextUr2;
CRect	RectEditUr2;
CRect	RectEditPoint;
CRect	RectZonaFormula;
CRect	RectRadio1;
CRect	RectIconRadio1;
CRect	RectRadio2;
CRect	RectIconRadio2;
CRect	RectStaticPorog;
CRect	RectTextMin;
CRect	RectEditMin;
CRect	RectTextMax;
CRect	RectEditMax;

CRect   RectZona;			//зона отображения состояния физической рукоятки
CRect	RectZonaText;
CRect	RectEditRukoyatka;

CRect	RectTabTuning;		//зона управления дисплеем и памятью
							//закладка "Дисплей"
CRect	RectButtonLeft;		//координата (0,0)
CRect	RectButtonCenter;
CRect	RectButtonRight;
CRect	RectIconLeft;
CRect	RectIconCenter;
CRect	RectIconRight;
CRect	RectButtonInversion;
CRect	RectIconInversion;
CRect	RectButtonInversionY;
CRect	RectIconInversionY;
CRect	RectDisplayRef;
CRect	RectFon;
CRect	RectLooping;
CRect	RectTextLooping;
CRect	RectFamilyVkl;
CRect	RectIconFamily;

CRect	RectButtonMarkerReset;	//маркер
CRect	RectButtonMarkerLeft;
CRect	RectButtonMarkerRight;
CRect	RectButtonMarker1;
CRect	RectButtonMarker2;
CRect	RectTextMarker1;
CRect	RectTextMarker2;
CRect	RectButtonTangent;
CRgn	RgnButtonMarker1;
CRgn	RgnButtonMarker2;
CRgn	RgnButtonTangent;
CRect	RectMarker1;
CRect	RectMarker2;
CRect	RectTangent;

CRgn	RgnExit;
CRect	RectExit;

CRect	RectListRef;		//Архив		
CRect	RectProgressProcess;	
CRect	RectEditErrors;		//закладка "Ошибки"

CRect	RectTree;			//зона управления выбором теста (дерево)
CRect	RectNameFileMod;
CRect	RectOtcht;			//зона отчёта

UINT	nCountChildWnd = 0;
BOOL	nCheckTree     = 0;
BOOL	nCheckInfo     = 0;
BOOL	nCheckOtcht    = 0;			//0-окно отчёта отсутствует на экране, 1-отобразить окно отчёта на экране
BOOL    bStateOtcht    = stateMin;	//0-окно отчёта минимизировано, 1-развёрнут;
BOOL	bStateListRef  = false;		//0-окно архива эталонных файлов графиков отсутствует на экране
BOOL	nCheckParam	   = 0;			//0-маркеры не активны, 1-маркеры активны

BOOL	res_gotov	   = false;		//1-в режиме DC многократный произошла смена ступеньки в первой очереди 
									//(пришла команда  5 от Pic-a)

CString	m_sparol        = _T("");						    //пароль
CString	strFileName     = _T("");						    //имя текущего файла документа
CString strFileUntitled = _T("\\Hard Disk\\Mod\\Untitled.mod");	//Hard Disk - WinCE_4.0, DiskC - WinCE_3.0
CString strTestName     = _T("IcVc");					    //наименование текущего теста
CString strFileNoName   = _T("\\Hard Disk\\Mod\\*.mod");			//Hard  Disk - WinCE_4.0, DiskC - WinCE_3.0
CString nameTst  		= _T("\\Hard Disk\\");				    //полный путь к файлу параметров
CString filePassWord	= _T("\\Hard Disk\\Psw\\LinePsw.psw");	//файл настройки (пароль, вид запуска, цвет экрана)
CString	strPassRef		= _T("\\Hard Disk\\Ref\\");				//Line\\Ref\\каталог файлов *.ref (эталонных графиков)
CString	strFileRef		= _T("\\Hard Disk\\Ref\\*.ref");			//файлы *.ref (эталонные графики)
CString strPrimRef		= _T("");
CString strNameRef      = _T("");					    	//имя текущего файла Ref
CString	strFileTstUser	= _T("\\Hard Disk\\Tst_user\\*.tst");	//файлы *.tst пользовательских тестов
CString	strFileTstStandart = _T("\\Hard Disk\\Tst_standart\\*.tst");	//файлы *.tst стандартных тестов
CString	strPassTstUser	= _T("\\Hard Disk\\Tst_user\\");			//файлы *.tst пользовательских тестов
CString	strPassTstStandart = _T("\\Hard Disk\\Tst_standart\\");	//файлы *.tst стандартных тестов
CString	strPassOtch		= _T("\\Hard Disk\\Txt\\");			//Файлы отчетов
CString nameXLS         = _T("\\Hard Disk\\FTPRoot\\Xls\\Untitled.xls");	//Файл таблиц результатов при автосохранении
CString nameOtch        = _T("\\Hard Disk\\FTPRoot\\Txt\\Untitled.txt");	//Файл отчета при автосохранении

CString		  strDateCurrent;

CString     m_masStrEl[NE];
HTREEITEM   m_masHEl[NE];

CString     m_masStrMod[NE][NT];

CString     str_EL,str_MOD,str_PRIM;	//наименование текущего теста
CString     str_NewEL,str_NewMOD;	    //новое наименование теста (при копировании)
int			i_EL      = 0;				//текущий номер элемента
int			i_MOD     = 0;				//текущий номер теста
int			i_SelUrov = 0;				//выделенный уровень:0-нет,1-элемент,2-тест
int			akt_button;	

UINT	    i_TIP=0;					//тип проводимости (канала) (D или R_C, NPN или PNP, NMOS или PMOS)
UINT		i_VID=0;					//вид элемента (2-хполюсные: D или R, 3-х,4-xполюсные: биполярный или полевой (МОП и т.п.)
UINT    	uPrecigion = 1;				//разрешение
UINT		uNpointsLupa = 20;//50;//			//число точек на графике в режиме IMP и DC (м. б. 20 или 50)
UINT		uNpointsLupaDC = 20;//50;//20;		//число точек на графике в режиме IMP и DC (м. б. 20 или 50)

UINT		uFileNumber = 0;			//номер файла для автоматического сохранения таблицы результатов (*_n.xls)

CImageList  m_imageList;

CStringArray array_error;				// ошибки
CStringArray array_otcht;				// отчёт
CStringArray array_ref;					// ссылки
CStringArray array_tst_standart;		// список стандартных тестов
CStringArray array_tst_user;			// список пользовательских тестов
CStringArray array_tst_prim;			// список комментарий тестов библиотеки


CString str_razmer_BiasBaza  =_T("");
CString str_razmer_LimitBaza =_T("");
CString str_razmer_BiasDop   =_T("");
CString str_razmer_LimitDop  =_T("");

CString ScaleX =_T("");
CString ScaleY =_T("");

double MinX = 0.0;
double MaxX = 0.0;
double MinY = 0.0;
double MaxY = 0.0;
double StepX = 0.0;
double StepY = 0.0;

//Управляющие команды
//*******************
BYTE K1[4] = {1,1,0,0};				    // Команда "Схема включения" (коллектор вкл.,база и доп.канал в обрыве)
BYTE K2[3] = {2,2,0};					// Команда "Вид развёртки": SYN_POS, 1-ая очередь - канал С
BYTE K3[3] = {3,0,0};					// Команда "Импульс" (длит. 10 ms, режим 1)

BYTE K4[2] = {4,0};						// Команда "Диапазон источника U коллектора"
BYTE K5[2] = {5,7};						// Команда "Диапазон измерителя U коллектора"
BYTE K6[2] = {6,17};					// Команда "Диапазон измерителя I коллектора"
BYTE K7[2] = {7,100};					// Команда "Ограничение напряжения коллектора" (в %)
BYTE K8[2] = {8,0x00};					// Команда "Порог ограничения тока коллектора" (1 байт из расчёта: 100% - код 220)

BYTE K9[2] = {9,1};						// Команда "Режим источника базы"
BYTE K10[2] = {10,0};					// Команда "Диапазон амплитуды ступени базы"
BYTE K11[2] = {11,0};					// Команда "Число ступеней базы"
BYTE K12[3] = {12,0x00,0x00};			// Команда "Значение амплитуды ступени базы"
BYTE K13[1] = {13};						// Команда "Останов автоустановки и калибровки" 
BYTE K14[3] = {14,0x00,0x00};			// Команда "Смещение базы"
BYTE K15[2] = {15,0};					// Команда "Режим измерителя базы"
BYTE K16[2] = {16,0};					// Команда "Диапазон измерителя  базы"
BYTE K17[2] = {17,6};					// Команда "Диапазон ограничения базы"
BYTE K18[3] = {18,0x00,0x00};			// Команда "Порог ограничения измерителя базы"

BYTE K19[2] = {19,0};					// Команда "Режим источника в доп.канале"
BYTE K20[2] = {20,4};					// Команда "Диапазон амплитуды ступени в доп.канале"
BYTE K21[2] = {21,0};					// Команда "Число ступеней в доп.канале"
BYTE K22[3] = {22,0x00,0x00};			// Команда "Значение амплитуды ступени в доп.канале"
BYTE K24[3] = {24,0x00,0x00};			// Команда "Смещение в доп.канале"
BYTE K25[2] = {25,1};					// Команда "Режим измерителя в доп.канале"
BYTE K26[2] = {26,0};					// Команда "Диапазон измерителя доп.канала"
BYTE K27[2] = {27,22};					// Команда "Диапазон ограничения в доп.канале"
BYTE K28[3] = {28,0x00,0x00};			// Команда "Порог ограничения измерителя в доп.канале"
BYTE K29[13] = {29,0x00,0x00};			// Команда "Данные настройки"

BYTE K30[2] = {30,50};					// Команда "Число снимаемых точек (массивов рез.-тов в DC и IMP)"
BYTE K31[1] = {31};//,0};				// Команда "Вкл. режим автоустановки нуля" или "режим Калибровки"
BYTE K32[2] = {32,0};					// Команда "Высокое разрешение по базе"
BYTE K33[2] = {33,0};					// Команда "Выдать сигнал годен/не годен" (0 - годен, 1 - не годен)
BYTE K34[3] = {34,0,0};					// Команда "Вид запуска" (1-е данное: 0 - "внутренний", 1 - "внешний",
										//						 (2-е данное: 0 - "однократный", 1 - "многократный")
BYTE K35[2] = {35,1};					// Команда "Режим измерения" (1 - "ручной", >1 - "автоматический")
BYTE K36[2] = {36,130};					// Команда "Максимальный код ЦАПа Dz"
BYTE K37[2] = {37,0};					// Команда "Состояние бита Q18" (результат проверки на "крест")
BYTE K38[1] = {38};//,0}; 				// Команда "Экстренный останов" (Прервать измерение)
										// 1 - Прервать измерение немедленно
										// 2 - дойти до конца цикла и прервать измерение
BYTE K39[2] = {39,0};					// резерв (пока)
BYTE K40[2] = {40,0};					// Биполярный тр-р + импульс (генератор ступенек - I) (0 - нет, 1-да)
BYTE K41[1] = {41};						// Команда "Измерить"

int nK[41] =   {4,3,3,2,2,2,2,2,2,2,2,3,1,3,2,2,2,3,2,2,2,3,2,3,2,2,2,3,3,2,1,2,2,3,2,2,2,1,2,2,1};

//команды для калибровки
//-------------------------------
//измеритель Ic
//массив диапазонов амплитуды тока и напряжения базы, соотв. диапазону Ic 
int mas_Ab_Ic[] =  {0,0,0,0,			//Табл. 5.2 ТУ
					0,0,0,0,
					2,3,4,				//диапаз. ист. Ib (Ic от 2 А - 200 uA)(условие 2)
					5,6,7,8,9,10,
					11,12,13,14,
					1,2,				//диапаз. ист. Ub (Ic от 100 uA - 10 nA)(условие 1)
					3,4,5,0,1,2,
					3,4,5,0,1};	//,0,0,0};
int mas_bias_Ub_for_KalibrIc[]= {0,0,0,0,
								 0,0,0,0,0,0,0,
								 0,0,0,0,0,0,
								 0,0,0,0,0,0,
								 675,760,660,0,0,0,
								 675,760,660,0,0};

BYTE mas_KalibrIc_100uA[] = {31,35,1,34,0,0,1,0,1,0,2,1,0,3,0,0,	//условие 1
								  30,20,32,0,37,0,36,180};
BYTE mas_KalibrIc_5A[]    = {31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//условие 2
								  30,20,32,0,37,0,36,180};
BYTE mas_KalibrIc_50A[]   = {//31,35,1,34,0,0,1,1,0,0,2,0,1,3,4,0,	//условие 3
							   31,35,1,34,0,0,1,1,0,0,2,0,0,3,4,0,	//условие 3
							   30,20,37,0,40,0,36,180};

//массив значений диапазонов Uc (по табл. 5.3 ТУ) в зависимости от калибруемого диапазона Ic 
int  mas_range_meas_Uc[]  = {9,10,11,9,	//50A, 20A, 10A, 5A на 5V
							 12,10,11,9};	//50A, 20A, 10A, 5A на 2000V 

//массив значений ограничение Uc max (Стоп в %) в зависимости от калибруемого диапазона Ic 
int  mas_Lstop_Uc_forKalibrIc[] = {95,55,33,90, //50A, 20A, 10A, 5A на 5V
								   60,53,30,35};//50A, 20A, 10A, 5A на 2000V 

//измеритель Uc
BYTE mas_KalibrUc[]       = {31,35,1,34,0,0,1,0,0,0,2,1,0,3,0,0,	//C,B,S - обрыв, DC
								  30,20,32,0,37,0,36,180};
//канал B
BYTE mas_Kalibr_Baza[]    = {31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//DC
								  30,20,32,0,37,0,36,180};
//канал S
BYTE mas_Kalibr_Dop[]     = {31,35,1,34,0,0,1,1,0,1,2,1,0,3,0,0,	//DC
								  30,20,32,0,37,0,36,180};

//команды для автоустановки нуля
//-------------------------------
//измеритель Ic
BYTE mas_ControlAutoSetupIcm[] ={//31,35,1,34,0,0,1,1,0,0,2,0,1,3,4,0,	//IMP
								   31,35,1,34,0,0,1,1,0,0,2,0,0,3,4,0,	//IMP
								   30,20,37,0,36,90,40,0};
BYTE mas_ControlAutoSetupIcm_[] ={//31,35,1,34,0,0,1,1,0,0,2,4,1,3,4,0,	//-IMP
								    31,35,1,34,0,0,1,1,0,0,2,6,0,3,4,0,	//-IMP
									30,20,37,0,36,90,40,0};
BYTE mas_AutoSetupIcm_200A[]   ={//31,35,1,34,0,0,1,1,0,0,2,0,1,3,4,0,	//IMP,  
								   31,35,1,34,0,0,1,1,0,0,2,0,0,3,4,0,	//IMP,  
								   30,20,37,0,36,180};
BYTE mas_AutoSetupIcm_50A[]    ={//31,35,1,34,0,0,1,1,0,0,2,0,1,3,4,0,	//IMP, 
								   31,35,1,34,0,0,1,1,0,0,2,0,0,3,4,0,	//IMP, 
								   30,20,37,0,36,180};
BYTE mas_AutoSetupIcm_200mA[]  ={31,35,1,34,0,0,1,1,0,0,2,1,0,3,0,0,	//DC, 
								  30,20,37,0,36,180};
BYTE mas_AutoSetupIcm_10nA[]   ={31,35,1,34,0,0,1,0,0,0,2,1,0,3,0,0,	//DC, 
								  30,20,37,0,36,180};
//измеритель Uc
BYTE mas_ControlAutoSetupUcm[] ={//31,35,1,34,0,0,1,1,0,0,2,0,1,3,4,0,	//IMP, 
								   31,35,1,34,0,0,1,1,0,0,2,0,0,3,4,0,	//IMP, 
								   30,20,37,1,36,180};
BYTE mas_ControlAutoSetupUcm_[]={//31,35,1,34,0,0,1,1,0,0,2,4,1,3,4,0,	//-IMP, 
								   31,35,1,34,0,0,1,1,0,0,2,6,0,3,4,0,	//-IMP, 
								   30,20,37,1,36,180};
BYTE mas_AutoSetupUcm[]		   ={31,35,1,34,0,0,1,1,0,0,2,1,0,3,0,0,	//DC,
								  30,20,37,0,36,180};
/*
BYTE mas_AutoSetupUcm_5V[]     ={//31,35,1,34,0,0,1,1,0,0,2,0,1,3,4,0,	//IMP, 
								   31,35,1,34,0,0,1,1,0,0,2,0,0,3,4,0,	//IMP, 
								   30,20,37,1,36,180};
*/
//измеритель Ib
BYTE mas_ControlAutoSetupIbm[]={31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//DC, 
								30,20,32,0,37,0,36,180};				  
//измеритель Ub
BYTE mas_ControlAutoSetupUbm[]={31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				
//источник Ib
BYTE mas_ControlAutoSetupIbs[]={31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				
BYTE mas_AutoSetupIbs[]       ={31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				
//источник Ub
BYTE mas_ControlAutoSetupUbs[]={31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				
BYTE mas_AutoSetupUbs[]       ={31,35,1,34,0,0,1,1,1,0,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				

//измеритель Is
BYTE mas_ControlAutoSetupIsm[]={31,35,1,34,0,0,1,1,1,1,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};					
//измеритель Us
BYTE mas_ControlAutoSetupUsm[]={31,35,1,34,0,0,1,1,1,1,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};					
//источник Is
BYTE mas_ControlAutoSetupIss[]={31,35,1,34,0,0,1,1,1,1,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};					
BYTE mas_AutoSetupIss[]		  ={31,35,1,34,0,0,1,1,1,1,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				
//источник Us
BYTE mas_ControlAutoSetupUss[]={31,35,1,34,0,0,1,1,1,1,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				
BYTE mas_AutoSetupUss[]       ={31,35,1,34,0,0,1,1,1,1,2,1,0,3,0,0,	//DC
								30,20,32,0,37,0,36,180};				

BYTE KK4[2] = {4,0};					// Команда "Диапазон источника U коллектора"
BYTE KK5[2] = {5,7};					// Команда "Диапазон измерителя U коллектора"
BYTE KK6[2] = {6,0};					// Команда "Диапазон измерителя I коллектора"
BYTE KK7[2] = {7,0};					// Команда "Ограничение напряжения коллектора max" (стоп)
BYTE KK8[2] = {8,0};					// Команда "Ограничение напряжения коллектора min" (старт)
BYTE KK9[2] = {9,0};					// Команда "Режим источника базы"
BYTE KK10[2] = {10,0};					// Команда "Диапазон амплитуды ступени базы"
BYTE KK11[2] = {11,0};					// Команда "Число ступеней базы"
BYTE KK12[3] = {12,0x00,0x00};			// Команда "Значение амплитуды ступени базы"
BYTE KK14[3] = {14,0x00,0x00};			// Команда "Смещение базы"
BYTE KK15[2] = {15,0};					// Команда "Режим измерителя базы"
BYTE KK16[2] = {16,0};					// Команда "Диапазон измерителя  базы"
BYTE KK17[2] = {17,6};					// Команда "Диапазон ограничения базы"
BYTE KK18[3] = {18,0x00,0x00};			// Команда "Порог ограничения измерителя базы"
BYTE KK19[2] = {19,0};					// Команда "Режим источника в доп.канале"
BYTE KK20[2] = {20,0};					// Команда "Диапазон амплитуды ступени в доп.канале"
BYTE KK21[2] = {21,0};					// Команда "Число ступеней в доп.канале"
BYTE KK22[3] = {22,0x00,0x00};			// Команда "Значение амплитуды ступени в доп.канале"
BYTE KK24[3] = {24,0x00,0x00};			// Команда "Смещение в доп.канале"
BYTE KK25[2] = {25,0};					// Команда "Режим измерителя доп. кан."
BYTE KK26[2] = {26,0};					// Команда "Диапазон измерителя доп.канала"
BYTE KK27[2] = {27,22};					// Команда "Диапазон ограничения в доп.канале"
BYTE KK28[3] = {28,0x00,0x00};			// Команда "Порог ограничения измерителя в доп.канале"

BOOL bMeas    = MEAS_SINGLE;	// Режим измерения "однократный"

BOOL bZapusk  = 0;				// Вид запуска: 0 - "внутренний" (по кнопке "Пуск"или по рукоятке),
								//              1 - "внешний" (по внешнему сигналу)
BOOL bAutomat = HAND;			// Способ проведения измерений: 0 - "ручной" (выбран один тест),
								// 1 - "автоматический" (выбран элемент (проходят все тесты для этого элемента))
BOOL bSignal  = false;			// Признак: 1 - индицировать сигнал "Высокое напряжение", 0 - нет
BOOL bStatusTablRes = stateMin;	// Признак: 1 - таблица результатов развёрнута, 0 - нет

BOOL bStatusRef = false;		// Признак: 1 - эталонный график на экране, 0 - нет
BOOL bStatusMeas = true;		// Признак: 1 - измеренный график на экране, 0 - нет
BOOL bStatusCalc = false;		// Признак: 1 - расчетный график на экране, 0 - нет
BOOL bSound	= false;			//1-автоматическое сохранениие результатов включен0, 0 - выключено

UINT     n_Result = 200;		//число измеренных точек
UINT 	 n_Step = 1;			//число шагов (ступенек)
BOOL	 bGenerationStup = false;	//признак: 1 - генерировать ступеньки, 0 - нет
UINT	 n_Test = 1;			//число тестов для выбранного элемента в автоматическом режиме,
								//в "ручном" n_Test = 1

double*  pM[5];			//массив указателей на массивы результатов (pM1,pM2,pM3,pM4,pM5);
double	 pM1[NMAS][NREZ];		//массивы результатов (ACP1) - Uc
double   pM2[NMAS][NREZ];		//(ACP2)					 - Ic
double   pM3[NMAS][NREZ];		//(ACP3)					 - база
double   pM4[NMAS][NREZ];		//(ACP4)					 - доп.канал
double   pM5[NMAS][NREZ];		//расчетный массив

double	 bM1[NPOINT100];//массивы результатов для АС(ACP1) - Uc
double   bM2[NPOINT100];//(ACP2)					 - Ic
double   bM3[NPOINT100];//(ACP3)					 - база
double   bM4[NPOINT100];//(ACP4)					 - доп.канал

double	 cM1[NPOINT100];//массивы результатов для АС(ACP1) - Uc
double   cM2[NPOINT100];//(ACP2)					 - Ic
double   cM3[NPOINT100];//(ACP3)					 - база
double   cM4[NPOINT100];//(ACP4)					 - доп.канал

double	 mM1[NMAS][NREZ];//массивы результатов для IMP (ACP1) - Uc
double   mM2[NMAS][NREZ];//(ACP2)					 - Ic
double   mM3[NMAS][NREZ];//(ACP3)					 - база
double   mM4[NMAS][NREZ];//(ACP4)					 - доп.канал

double	 aM1[NREZ];//массивы результатов для DC (ACP1) - Uc
double   aM2[NREZ];//(ACP2)					 - Ic
double   aM3[NREZ];//(ACP3)					 - база
double   aM4[NREZ];//(ACP4)					 - доп.канал

double pX[2000];
double pY[2000];

/////////////////////////////////////////////////////////////////////////////
// CLineDlg dialog

CLineDlg::CLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineDlg::IDD, pParent)
{
	m_sInfoMeas = _T("");
	m_sInfoSignal = _T("");
	m_sKnotKol = _T("");
	m_sKnotBaza = _T("");
	m_sKnotDop = _T("");
	m_sKnotEm = _T("");
	m_iCircuitKol = 1;		//вкл.
	m_iCircuitBaza = 1;		//база	//обрыв
	m_iCircuitDop  = 0;		//выкл.
	m_MOD = _T("IcVc");
	m_sKolMeasU = _T("Uc");
	m_sKolMeasI = _T("Ic");
	m_irazv = SYN_POS;//DC_POS;//			//IMP_POS
	m_sSliderKolU = _T("");
	m_iKolU = 8;//7;			//10V
	m_iKolI = 13;//14;			//1mA
	m_iKolSourceU = 0;		//20V
	m_bCheckKolRejHigh = FALSE;
	m_sLimitKolI = _T("  0");
	m_sLimitKolU = _T(" 50");
	m_sRangMaxKolI = _T("");
	m_sBazaSource = _T("Ib");
	m_iBazaSource = 1;
	m_iAmplitudeStepBaza = 12;// 50 uA	18;//1uA
	m_uStepBaza = 0;//10???
	m_iPolarityStepBaza = 0;
	m_iPolarityBiasBaza = 0;
	m_iRangLimitBaza = 5;	//1 V
	m_sBiasBaza = _T("");
	m_sLimitBaza = _T("");
	m_bCheckBazaRejHigh = FALSE;
	m_sDopSource = _T("");
	m_iAmplitudeStepDop = -1;
	m_uStepDop = 0;
	m_iPolarityStepDop = 0;
	m_sBiasDop = _T("");
	m_iPolarityBiasDop = 0;
	m_iRangLimitDop = -1;
	m_sLimitDop = _T("");
	m_sAxisY = _T("Ic");
	m_sAxisX = _T("Uc");
	m_sPar = _T("");//Ic
	m_sArg = _T("");//Uc
	m_sUr1 = _T("");//5
	m_sUr2 = _T("");
	m_iForm = -1;
	m_sMin = _T("");
	m_sMax = _T("");
	m_iButNull = 1;
	m_iDopSource = 0;
	m_sDlitImp = _T("");
	m_bCheckPauza = FALSE;
	m_sNameFileMod = _T("Файл модели");
	m_st_flash = _T("");
	m_sRangBiasBaza = _T("");
	m_sRangBiasDop = _T("");
	m_bLooping = FALSE;
	m_iGnezdoC = 0;
	m_iGnezdoB = 1;
	m_iGnezdoE = 2;
	m_iKommutator = 0;
	m_sBazaMeasLimit = _T("Ub");
	m_sDopMeasLimit = _T("");
	m_bBazaVkl = FALSE;
	m_bFamilyVkl = FALSE;
	m_sStupN = _T("на ступеньке № 0");
	m_nelement = 2;//-1;
	m_iQue1 = COLLECTOR;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_pSaveOtchDlg = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bCapt				= FALSE;
	m_bModifiedFlag		= FALSE;
	m_bZaschitaParol	= FALSE;	//защита паролем файла *.mod: 0-нет, 1-да
	m_bFileOpen			= FALSE;	
    m_bCopy			    = FALSE;
	m_bModifiedMemory	= FALSE;
	m_bLoopingOld		= m_bLooping;
	m_bFamilyVklOld		= m_bFamilyVkl;
    m_bKrishka			= false;	//false - крышка закрыта //true - крышка открыта
	free_space = 0;

	Height = 0;		//высота окна графика
	Width = 0;		//ширина окна графика
	m_uN = 0;
	m_uTp = 0;		//поиск параметра на ступеньке c № = m_uTp
					//if (bGenerationStup) ступеньки генерируются и m_uTp = № последней ступеньки
					//if (!bGenerationStup) ступеньки не генерируются, отрабатывается уровень последней 
					//ступеньки и m_uTp = 1 (одна ветка, соответствующая уровню последней ступеньки)
	
		
	m_colorFon		  = RGB(240,240,240);
	m_colorBMP  	  = RGB(255,255,0);
	m_colorSetka      = RGB(0,0,0);//150,150,150);
	m_colorTangent    = RGB(0,0,255);
	m_colorText       = RGB(0,0,0);//250,250,250);
	m_colorCurve	  = RGB(0,0,255);
	m_colorRef   	  = RGB(255,0,0);

	array_error.RemoveAll();
	array_otcht.RemoveAll();
	array_ref.RemoveAll();
	array_tst_standart.RemoveAll();
	array_tst_user.RemoveAll();

	m_fontMin.CreateFont(14,0,0,0,FW_NORMAL,
		                 0,0,0,RUSSIAN_CHARSET,
						 OUT_DEFAULT_PRECIS,
						 CLIP_DEFAULT_PRECIS,
						 DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
						 _T("Arial"));
	m_fontMax.CreateFont(24,0,0,0,FW_NORMAL,
		                 0,0,0,RUSSIAN_CHARSET,
						 OUT_DEFAULT_PRECIS,
						 CLIP_DEFAULT_PRECIS,
						 DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
						 _T("Arial"));
	m_bMigSignal = FALSE;
 
	pModSelected = NULL;
    pModBuf      = new CMod;//NULL;

	HICON hIcon[3];
	int n;
	m_imageList.Create(16,16,0,0,3);
	hIcon[0]=AfxGetApp()->LoadIcon(IDI_SELECT);
	hIcon[1]=AfxGetApp()->LoadIcon(IDI_LEVEL1);
	hIcon[2]=AfxGetApp()->LoadIcon(IDI_LEVEL2);
	for (n=0;n<3;n++)
		m_imageList.Add(hIcon[n]);

	strValueRukoyatka = _T("100 % ");

    m_sEL_old     = _T("");	//элемент
    m_sPart_old   = _T("");	//№ партии
    m_sPlast_old  = _T("");	//№ пластины
	m_sPoint_old  = _T("");	//№ точки
    m_sPart       = _T("");	//№ партии
    m_sPlast      = _T("");	//№ пластины
	m_sPoint      = _T("");	//№ точки
	m_sProduct	  = _T(""); //Изделие
	m_sNameOtch	  = _T("Protocol");
	m_sFIO		  = _T(""); //ФИО
	m_sPost		  = _T(""); //должность
	m_uCountTest  = 0;
	m_nCountMas   = 0;      // счётчик числа принятых массивов результатов
	m_nMas		  = 2;      // число массивов, которое необходимо принять (зависит от вида развёртки)
							// =2 - для AC, =1 - для остальных
	index		  = 1;		// индекс выбранного теста в предлагаемом списке тестов
	i_SelUrov     = 0;		// выделенный уровень:0-нет,1-элемент,2-тест
    i_EL	      = 0;		// текущий номер тестового элемента
    i_MOD		  = 0;		// текущий номер теста
	m_idlitimp    = 0;
	m_udlitpauza = 0;

	m_EL		  = _T("NPN");
	m_PRIM		  = _T("Выходная характеристика транзистора");
	str_PRIM = m_PRIM;
	m_nelement    = NPN;	// NPN
	m_iCircuitEm  = 0;		// эмиттер - всегда общий
	//коллектор
    m_iLimitKolU  = 50;	    // текущее значение слайдера ограничения U коллектора (в %)
	m_iLimitKolI  = 0;	// текущее значение слайдера ограничения I коллектора
	valueLimitKolU= 0.0;	// ограничение источника коллектора (U)
    m_dLimitKolU  = 0.0;	// максимальное значение диапазона изменения ограничения по U
	valueLimitKolI= 0.0;	// ограничение измерителя коллектора (I)
    m_dLimitKolI  = 0.0;	// максимальное значение диапазона изменения ограничения по I
	bias_KolMeasI = 0;		// смещение индекса в массиве диапазонов измерителя I
	bias_KolSourceU = 0;		// смещение индекса в массиве диапазонов измерителя U
	m_bSignal	  = FALSE;	// признак: 1 - поджечь сигнал "Высокое напряжение", 0 - погасить
	m_bSignalMemoryFull	  = FALSE;	// признак: 1 - поджечь сигнал "Память переполнена", 0 - погасить
	//база
	valueAmplBaza = 0.0;	// амплитуда ступени базы
	m_iBiasBaza	  = 0; 		// текущее значение слайдера смещения базы
	valueBiasBaza = 0.0;	// смещение генератора ступенек базы
    m_dBiasBaza   = 0.0;	// максимальное значение диапазона изменения смещения
	m_iLimitBaza  = 100;	// текущее значение слайдера ограничения базы
	valueLimitBaza= 0.0;	// ограничение измерителя базы
    m_dLimitBaza  = 0.0;	// максимальное значение диапазона изменения ограничения 
	m_iBazaMeas   = 0;		// 0 - измеритель U, 1 - измеритель I в базе
	m_iRangMeasBaza = -1;	// индекс диапазона измерителя в базе
	sign_ampl_baza = 0;		// знак амплитуды ступеньки в базе (0 - "+", 1 - "-")
	sign_bias_baza = 0;		// знак смещения в базе (0 - "+", 1 - "-")
	m_sBazaMeas = _T("");
	//доп.канал
	valueAmplDop  = 0.0;	// амплитуда ступени доп.канала
	m_iBiasDop	  = 0; 		// текущее значение слайдера смещения доп.канала
	valueBiasDop  = 0.0; 	// смещение генератора ступенек доп.канала 
    m_dBiasDop	  = 0.0;	// максимальное значение диапазона изменения смещения
	m_iLimitDop   = 100;	// текущее значение слайдера ограничения доп.канала
	valueLimitDop = 0.0;	// ограничение измерителя доп.канала 
    m_dLimitDop   = 0.0;	// максимальное значение диапазона изменения ограничения   
	m_iDopMeas    = 1;		// 0 - измеритель U, 1 - измеритель I в доп.канале
	m_iRangMeasDop = -1;	// индекс диапазона измерителя в доп.канале
	bias_Dop_I	  = 10;//9;		// смещение индекса в массиве диапазонов измерителя I(доп.канал)
	sign_ampl_dop = 0;		// знак амплитуды ступеньки в доп.канале (0 - "+", 1 - "-")
	sign_bias_dop = 0;		// знак смещения в доп.канале (0 - "+", 1 - "-")
	m_sDopMeas = _T("");

	m_iButNull	  = CENTER;	// 0 - LEFT, 1 - CENTER, 2 - RIGHT
	m_bButInversion = FALSE;// 0 - выкл., 1 - вкл.
	m_bButInversionY= FALSE;// 0 - выкл., 1 - вкл.
	m_iMarker	  = MARKER0;//номер текущего маркера: 1 - маркер1, 2 - маркер2, 0 - недоступен
	m_bTangent	  = FALSE;

	m_iX		  = 0;		//развёртка по оси X : 0-"1" ед.,1-"2" ед.,2-"5" ед.
	m_iY		  = 2;		//развёртка по оси Y : 0-"1" ед.,1-"2" ед.,2-"5" ед.
	m_bminusX = FALSE;
	m_bminusY = FALSE;
	nStepQ1		  = 0;		//число шагов 1-ой очереди
	nStepQ2		  = 0;		//число шагов 2-ой очереди
	iX			  = 0;		//канал - аргумент-источник 1-ой очереди: 0-коллектор, 1-база, 2-доп.канал
	iY			  = 0;		//канал - функция: 0-коллектор, 1-база, 2-доп.канал 
	rej_X		  = 0;		//характеристика, отображаемая по оси X: 0 - U, 1 - I
	rej_Y		  = 1;		//характеристика, отображаемая по оси Y: 0 - U, 1 - I
	rej_Y_Meas	  = 1;		//характеристика, отображаемая по оси Y: 0 - U, 1 - I
	expo_X		  = 1;
	expo_Y		  = -5;
	po_X		  = 0;
	po_Y		  = 0;
	X_min		  = -10.0;	//-10 V
	X_max		  = 10.0;	//+10 V
	Y_min		  = -0.001;//-1 mA            -0.000050000;//-50 uA
	Y_max		  = 0.001;//1 mA               0.000050000; //+50 uA
	mashtab_X     = 0.0;
	mashtab_Y     = 0.0;
	Ur1			  = 0.0;
	Ur2			  = 0.0;
	Porog_min	  = 0.0;	//пороги
	Porog_max	  = 0.0;
	m_bBeta = FALSE;
	m_bBetai= FALSE;
	m_bS = FALSE;
    m_bErli = FALSE; 
	m_bR = FALSE;	
	m_bKolX	= false;		//признак:	0 - по оси Х - Uc, 1 - Ic
	m_bKolY	= false;		//признак:	0 - по оси Y - Ic, 1 - Uc

	m_bBaza = false;		//признак: = 0 - измеряем то, что генерируем; 
							//		   = 1 - измеряем то, что в ограничении
	m_bDop  = false;		//признак: = 0 - измеряем то, что генерируем; 
							//		   = 1 - измеряем то, что в ограничении
	m_uFunc = 0;			//признак: 0 - найти значение функции по значению аргумента
							//		   1 - найти значение аргумента по значению функции
							//		   2 - рассчитать рассчётный массив (pM5[]) и 
							//			   найти параметр по значению аргумента
							//		   3 - рассчитать рассчётный массив (pM5[]) и
							//             найти параметр по значению функции
	
	m_iStyleCurve = STYLE_LINEPOINT;//STYLE_POINT;	//0 - линия, 1 - точки, 2 - линия с точками
	m_iNset = 0;			//№ программы автоустановки нуля
	m_iNproxod = 0;			//№ прохода автостановки нуля
	m_iNsetK = 0;			//№ программы калибровки
	m_iRange = 0;			//№ калибруемого диапазона
    m_iDifference = 0;		//режим измерения разности ВАХ: 0-выключить, 1-последовательное вкючение, 2- параллельное включение

	//автоустановка нуля и калибровка
	//-------------------------------
	m_ir = 0;
	m_iUc = 0;
	m_iIc = 0;
	m_iUcs= 0;		//диапазон источника Uc
	m_iLstart= 0;	//ограничение Uc min
	m_iLstop= 0;	//ограничение Uc max
	m_iBs = 0;
	m_iBm = 0;
	m_iAb = 0;
	m_uSb = 0;
	m_iRb = 0;
	m_iBb = 0;	//смещение 
	m_iLb = 0;	//ограничение 
	m_iDs = 0;
	m_iDm = 0;
	m_iAd = 0;
	m_uSd = 0;
	m_iRd = 0;
	bias_Ucs = 0;
	bias_Uc = 0;
	bias_Ic = 0;
	bias_Id = 0;
	m_iBd = 0;	//смещение 
	m_iLd = 0;	//ограничение 

	//расчет
	Beta_max	  = 0.0;
	Betai_max	  = 0.0;
	Ib_Beta_max	  = 0.0;
	Ib_Betai_max  = 0.0;
	r1			  = 0.0;
	r2			  = 0.0;
	Par			  = 0.0;
    Flag          = FALSE;
    Flag_All      = FALSE;

	int i=0;
	for(i=0;i<NPOINT;i++) 
	{ X_axis[i]=0.0;
	  Y_axis[i]=0.0;
	}
	for (UINT m=0;m<NMAS;m++)
	for(i=0;i<NREZ;i++)
	{ refX[m][i]=0.0;
	  refY[m][i]=0.0;
	  diffX[m][i]=0.0;
	  diffY[m][i]=0.0;
	}
	for(i=0;i<NBUF;i++)
	My_buf_Out[i]=0;

	ClearResults();
	ResetResults();
	FlagOLClear();
}

void CLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineDlg)
	DDX_Control(pDX, IDC_BUTTON_MEAS, m_btn_meas);
	DDX_Control(pDX, IDC_PROGRESS_PROCESS, m_Progress);
	DDX_Control(pDX, IDC_PROGRESS_FREE_MEMORY, m_progress_flash);
	DDX_Control(pDX, IDC_TREE_TEST, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listRes);
	DDX_Control(pDX, IDC_BUTTON_LOADTST, m_bst_loadtst);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btn_reset);
	DDX_Control(pDX, IDC_BUTTON_OTCHT, m_btn_otcht);
	DDX_Control(pDX, IDC_BUTTON_GRAF, m_btn_graf);
	DDX_Control(pDX, IDC_BUTTON_MEMORY, m_btn_memory);
	DDX_Control(pDX, IDC_BUTTON_TUNING, m_btn_tuning);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_btn_test);
	DDX_Control(pDX, IDC_BUTTON_FILE, m_btn_file);
	DDX_Control(pDX, IDC_LISTREF, m_listRef);
	DDX_Control(pDX, IDC_TABCTRLPARAM, m_TabCtrlParam);
	DDX_Control(pDX, IDC_TABCTRLTUNING, m_TabCtrlTuning);
	DDX_CBIndex(pDX, IDC_COMBOCHECKKOL, m_iCircuitKol);
	DDX_CBIndex(pDX, IDC_COMBOCHECKBAZA, m_iCircuitBaza);
	DDX_CBIndex(pDX, IDC_COMBOCHECKDOP, m_iCircuitDop);
	DDX_CBString(pDX, IDC_COMBOMOD, m_MOD);
	DDX_Text(pDX, IDC_EDKOLMEASU, m_sKolMeasU);
	DDV_MaxChars(pDX, m_sKolMeasU, 2);
	DDX_Text(pDX, IDC_EDKOLMEASI, m_sKolMeasI);
	DDV_MaxChars(pDX, m_sKolMeasI, 2);
	DDX_Radio(pDX, IDC_RADIO_RAZVIMPPOSITIVE, m_irazv);
	DDX_Text(pDX, IDC_EDSLIDERKOLU, m_sSliderKolU);
	DDV_MaxChars(pDX, m_sSliderKolU, 2);
	DDX_CBIndex(pDX, IDC_COMBOKOLU, m_iKolU);
	DDX_CBIndex(pDX, IDC_COMBOKOLI, m_iKolI);
	DDX_CBIndex(pDX, IDC_COMBOKOLSOURU, m_iKolSourceU);
	DDX_Check(pDX, IDC_CHECKKOLREJHIGH, m_bCheckKolRejHigh);
	DDX_Text(pDX, IDC_EDITLIMITKOLI, m_sLimitKolI);
	DDV_MaxChars(pDX, m_sLimitKolI, 13);
	DDX_Text(pDX, IDC_EDITLIMITKOLU, m_sLimitKolU);
	DDV_MaxChars(pDX, m_sLimitKolU, 13);
	DDX_Text(pDX, IDC_EDTEXTRANGMAXKOLI, m_sRangMaxKolI);
	DDX_Text(pDX, IDC_EDBAZASOURCEUI, m_sBazaSource);
	DDV_MaxChars(pDX, m_sBazaSource, 2);
	DDX_Radio(pDX, IDC_RADIOBAZASOURCEU, m_iBazaSource);
	DDX_CBIndex(pDX, IDC_COMBOAMPLITUDEBAZA, m_iAmplitudeStepBaza);
	DDX_Text(pDX, IDC_EDITSTEPBAZA, m_uStepBaza);
	DDV_MinMaxUInt(pDX, m_uStepBaza, 0, 10);
	DDX_CBIndex(pDX, IDC_COMBOPOLARITYSTEP, m_iPolarityStepBaza);
	DDX_CBIndex(pDX, IDC_COMBOPOLARITYBAZABIAS, m_iPolarityBiasBaza);
	DDX_CBIndex(pDX, IDC_COMBOBAZALIMITMAXUI, m_iRangLimitBaza);
	DDX_Text(pDX, IDC_EDITBAZABIAS, m_sBiasBaza);
	DDX_Text(pDX, IDC_EDITLIMITBAZA, m_sLimitBaza);
	DDX_Check(pDX, IDC_CHECKBAZAREJHIGH, m_bCheckBazaRejHigh);
	DDX_Text(pDX, IDC_EDDOPSOURCEUI, m_sDopSource);
	DDV_MaxChars(pDX, m_sDopSource, 2);
	DDX_CBIndex(pDX, IDC_COMBOAMPLITUDEDOP, m_iAmplitudeStepDop);
	DDX_Text(pDX, IDC_EDITSTEPDOP, m_uStepDop);
	DDV_MinMaxUInt(pDX, m_uStepDop, 0, 10);
	DDX_CBIndex(pDX, IDC_COMBOPOLARITYSTEPDOP, m_iPolarityStepDop);
	DDX_Text(pDX, IDC_EDITDOPBIAS, m_sBiasDop);
	DDX_CBIndex(pDX, IDC_COMBOPOLARITYDOPBIAS, m_iPolarityBiasDop);
	DDX_CBIndex(pDX, IDC_COMBODOPLIMITMAXUI, m_iRangLimitDop);
	DDX_Text(pDX, IDC_EDITLIMITDOP, m_sLimitDop);
	DDX_Text(pDX, IDC_AXISY, m_sAxisY);
	DDV_MaxChars(pDX, m_sAxisY, 3);
	DDX_Text(pDX, IDC_AXISX, m_sAxisX);
	DDV_MaxChars(pDX, m_sAxisX, 20);
	DDX_Text(pDX, IDC_EDITNAMEPAR, m_sPar);
	DDV_MaxChars(pDX, m_sPar, 7);
	DDX_Text(pDX, IDC_EDITNAMEARG, m_sArg);
	DDX_Text(pDX, IDC_EDITUR1, m_sUr1);
	DDX_Text(pDX, IDC_EDITUR2, m_sUr2);
	DDX_Radio(pDX, IDC_RADIO1, m_iForm);
	DDX_Text(pDX, IDC_EDITMIN, m_sMin);
	DDX_Text(pDX, IDC_EDITMAX, m_sMax);
	DDX_Radio(pDX, IDC_RADIO_LEFT, m_iButNull);
	DDX_Radio(pDX, IDC_RADIODOPU, m_iDopSource);
	DDX_Text(pDX, IDC_EDITDLITIMP, m_sDlitImp);
	DDV_MaxChars(pDX, m_sDlitImp, 10);
	DDX_Check(pDX, IDC_CHECKPAUZA, m_bCheckPauza);
	DDX_Text(pDX, IDC_STATIC_NAMEFILE, m_sNameFileMod);
	DDX_Text(pDX, IDC_ST_FREEMEMORY, m_st_flash);
	DDV_MaxChars(pDX, m_st_flash, 10);
	DDX_Text(pDX, IDC_EDITRANGBAZABIAS, m_sRangBiasBaza);
	DDV_MaxChars(pDX, m_sRangBiasBaza, 10);
	DDX_Text(pDX, IDC_EDITRANGDOPBIAS, m_sRangBiasDop);
	DDV_MaxChars(pDX, m_sRangBiasDop, 10);
	DDX_Check(pDX, IDC_CHECK_LOOPING, m_bLooping);
	DDX_CBIndex(pDX, IDC_COMBO_GNEZDO_C, m_iGnezdoC);
	DDX_CBIndex(pDX, IDC_COMBO_GNEZDO_B, m_iGnezdoB);
	DDX_CBIndex(pDX, IDC_COMBO_GNEZDO_E, m_iGnezdoE);
	DDX_CBIndex(pDX, IDC_COMBO_KOMMUTATOR, m_iKommutator);
	DDX_Text(pDX, IDC_EDMEASBAZA, m_sBazaMeasLimit);
	DDV_MaxChars(pDX, m_sBazaMeasLimit, 2);
	DDX_Text(pDX, IDC_EDMEASDOP, m_sDopMeasLimit);
	DDV_MaxChars(pDX, m_sDopMeasLimit, 2);
	DDX_Check(pDX, IDC_CHECK_BAZA_VKL, m_bBazaVkl);
	DDX_Check(pDX, IDC_CHECK_FAMILY, m_bFamilyVkl);
	DDX_Text(pDX, IDC_EDITPOINT, m_sStupN);
	DDX_CBIndex(pDX, IDC_COMBO_QUE1, m_iQue1);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CLineDlg, CDialog)
	//{{AFX_MSG_MAP(CLineDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_COMMAND(IDR_FILE_NEW, OnFileNew)
	ON_COMMAND(IDR_FILE_OPEN, OnFileOpen)
	ON_COMMAND(IDR_FILE_SAVE, OnFileSave)
	ON_COMMAND(IDR_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(IDR_FILE_CLOSE, OnFileClose)
	ON_COMMAND(IDR_FILE_EXIT, OnFileExit)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_COMMAND(ID_DEL, OnDel)
	ON_COMMAND(ID_DEL_ALL, OnDelAll)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_COMMAND(ID_PASTE, OnPaste)
	ON_COMMAND(ID_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_BUTTON_LOADTST, OnButtonLoadtst)
	ON_BN_CLICKED(IDC_RADIO_RAZVIMPPOSITIVE, OnRadioRazvimppositive)
	ON_BN_CLICKED(IDC_RADIO_RAZVIMPNEGATIVE, OnRadioRazvimpnegative)
	ON_BN_CLICKED(IDC_RADIO_RAZVDCPOSITIVE, OnRadioRazvdcpositive)
	ON_BN_CLICKED(IDC_RADIO_RAZVDCNEGATIVE, OnRadioRazvdcnegative)
	ON_BN_CLICKED(IDC_RADIO_RAZVAC, OnRadioRazvac)
	ON_CBN_SELCHANGE(IDC_COMBOKOLI, OnSelchangeCombokoli)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBOKOLSOURU, OnSelchangeCombokolsouru)
	ON_BN_CLICKED(IDC_RADIOBAZASOURCEU, OnRadiobazasourceu)
	ON_BN_CLICKED(IDC_RADIOBAZASOURCEI, OnRadiobazasourcei)
	ON_EN_UPDATE(IDC_EDITUR1, OnUpdateEditur1)
	ON_EN_UPDATE(IDC_EDITUR2, OnUpdateEditur2)
	ON_EN_UPDATE(IDC_EDITNAMEPAR, OnUpdateEditnamepar)
	ON_EN_UPDATE(IDC_EDITNAMEARG, OnUpdateEditnamearg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRLTUNING, OnSelchangeTabctrltuning)
	ON_BN_CLICKED(IDC_BUTTON_OTCHT, OnButtonOtcht)
	ON_COMMAND(ID_OTCHT_NEW, OnOtchtNew)
	ON_COMMAND(ID_OTCHT_MAX, OnOtchtMax)
	ON_COMMAND(ID_OTCHT_MIN, OnOtchtMin)
	ON_COMMAND(ID_PART, OnPart)
	ON_COMMAND(IDR_FILE_REPAROL, OnFileReparol)
	ON_BN_CLICKED(IDC_BUTTON_TUNING, OnButtonTuning)
	ON_BN_CLICKED(IDC_BUTTON_GRAF, OnButtonGraf)
	ON_BN_CLICKED(IDC_BUTTON_MEMORY, OnButtonMemory)
	ON_COMMAND(ID_TST_USER, OnSaveTstUser)
	ON_COMMAND(ID_TST_STANDART, OnSaveTstStandart)
	ON_COMMAND(ID_ADD_TREEFROMPANEL, OnAddTreefrompanel)
	ON_COMMAND(ID_ADD_TREEFROMMEMORY, OnAddTreefrommemory)
	ON_CBN_SELCHANGE(IDC_COMBOCHECKKOL, OnSelchangeCombocheckkol)
	ON_CBN_SELCHANGE(IDC_COMBOCHECKBAZA, OnSelchangeCombocheckbaza)
	ON_CBN_SELCHANGE(IDC_COMBOCHECKDOP, OnSelchangeCombocheckdop)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRLPARAM, OnSelchangeTabctrlparam)
	ON_CBN_SELCHANGE(IDC_COMBOBAZALIMITMAXUI, OnSelchangeCombobazalimitmaxui)
	ON_CBN_SELCHANGE(IDC_COMBODOPLIMITMAXUI, OnSelchangeCombodoplimitmaxui)
	ON_CBN_SELCHANGE(IDC_COMBOAMPLITUDEBAZA, OnSelchangeComboamplitudebaza)
	ON_CBN_SELCHANGE(IDC_COMBOAMPLITUDEDOP, OnSelchangeComboamplitudedop)
	ON_EN_KILLFOCUS(IDC_AXISY, OnKillfocusAxisy)
	ON_EN_KILLFOCUS(IDC_AXISX, OnKillfocusAxisx)
	ON_CBN_KILLFOCUS(IDC_COMBOMOD, OnKillfocusCombomod)
	ON_CBN_SELENDOK(IDC_COMBOPOLARITYSTEP, OnSelendokCombopolaritystep)
	ON_CBN_SELENDOK(IDC_COMBOPOLARITYSTEPDOP, OnSelendokCombopolaritystepdop)
	ON_CBN_SELENDOK(IDC_COMBOPOLARITYBAZABIAS, OnSelendokCombopolaritybazabias)
	ON_CBN_SELENDOK(IDC_COMBOPOLARITYDOPBIAS, OnSelendokCombopolaritydopbias)
	ON_CBN_SELCHANGE(IDC_COMBOKOLU, OnSelchangeCombokolu)
	ON_EN_KILLFOCUS(IDC_EDITNAMEPAR, OnKillfocusEditnamepar)
	ON_EN_KILLFOCUS(IDC_EDITNAMEARG, OnKillfocusEditnamearg)
	ON_COMMAND(ID_TEST_RENAME, OnTestRename)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_COMMAND(ID_ZAPUSK_INTERNAL, OnZapuskInternal)
	ON_COMMAND(ID_ZAPUSK_EXTERNAL, OnZapuskExternal)
	ON_COMMAND(ID_CURVE_LINE, OnCurveLine)
	ON_COMMAND(ID_CURVE_LINEWITHPOINT, OnCurveLinewithpoint)
	ON_COMMAND(ID_CURVE_POINT, OnCurvePoint)
	ON_COMMAND(ID_COLOR_FON, OnColorFon)
	ON_COMMAND(ID_COLOR_CURVE, OnColorCurve)
	ON_COMMAND(ID_COLOR_FONT, OnColorFont)
	ON_COMMAND(ID_COLOR_REF, OnColorRef)
	ON_COMMAND(ID_COLOR_SETKA, OnColorSetka)
	ON_COMMAND(ID_COLOR_TANGENT, OnColorTangent)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_MARKER_RESET, OnButtonMarkerReset)
	ON_COMMAND(ID_SAVE_REF, OnSaveRef)
	ON_COMMAND(ID_EXTRACT_REF, OnExtractRef)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LISTREF, OnGetdispinfoListref)
	ON_COMMAND(ID_DEL_MEMORY, OnDelMemory)
	ON_COMMAND(ID_CLEAR_MEMORY, OnClearMemory)
	ON_COMMAND(ID_OTCHT_MAIL, OnOtchtMail)
	ON_COMMAND(ID_LISTREF_OPEN, OnListrefOpen)
	ON_COMMAND(ID_LISTREF_CLOSE, OnListrefClose)
	ON_COMMAND(ID_REF_RESET, OnRefReset)
	ON_COMMAND(ID_OPENTABL, OnOpentabl)
	ON_BN_CLICKED(IDC_RADIO_LEFT, OnRadioLeft)
	ON_BN_CLICKED(IDC_RADIO_CENTER, OnRadioCenter)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, OnRadioRight)
	ON_BN_CLICKED(IDC_CHECK_INVERS_X, OnCheckInversX)
	ON_BN_CLICKED(IDC_CHECK_INVERS_Y, OnCheckInversY)
	ON_BN_CLICKED(IDC_RADIODOPU, OnRadiodopu)
	ON_BN_CLICKED(IDC_RADIODOPI, OnRadiodopi)
	ON_BN_CLICKED(IDC_CHECKPAUZA, OnCheckpauza)
	ON_COMMAND(ID_SOUND_ON, OnSoundOn)
	ON_COMMAND(ID_SOUND_OFF, OnSoundOff)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TEST, OnSelchangedTreeTest)
	ON_COMMAND(ID_TABL_MIN, OnTablMin)
	ON_COMMAND(ID_TABLRES_SAVE, OnTablresSave)
	ON_COMMAND(ID_MARKER_AKT, OnMarkerAkt)
	ON_COMMAND(ID_MARKER_NOAKT, OnMarkerNoakt)
	ON_COMMAND(ID_DEL_TST_USER, OnDelTstUser)
	ON_COMMAND(ID_DEL_TST_STANDART, OnDelTstStandart)
	ON_COMMAND(IDC_FILE_DELETE, OnFileDelete)
	ON_COMMAND(IDR_TABL_DELETE, OnTablDelete)
	ON_COMMAND(ID_OTCHT_SAVE, OnOtchtSave)
	ON_COMMAND(IDC_DELETE_OTCH, OnDeleteOtch)
	ON_COMMAND(IDC_TUNING_COPYFILE, OnTuningCopyfile)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINSTEPBAZA, OnDeltaposSpinstepbaza)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINSTEPDOP, OnDeltaposSpinstepdop)
	ON_BN_CLICKED(IDC_CHECK_LOOPING, OnCheckLooping)
	ON_EN_KILLFOCUS(IDC_EDITUR1, OnKillfocusEditur1)
	ON_BN_CLICKED(IDC_CHECKKOLREJHIGH, OnCheckkolrejhigh)
	ON_COMMAND(ID_GRAF_LUPA, OnGrafLupa)
	ON_COMMAND(ID_KALIBR, OnKalibr)
	ON_UPDATE_COMMAND_UI(ID_KALIBR, OnUpdateKalibr)
	ON_COMMAND(ID_AUTO_SETUP_NULL, OnAutoSetupNull)
	ON_UPDATE_COMMAND_UI(ID_AUTO_SETUP_NULL, OnUpdateAutoSetupNull)
	ON_CBN_SELCHANGE(IDC_COMBO_KOMMUTATOR, OnSelchangeComboKommutator)
	ON_CBN_SELCHANGE(IDC_COMBO_GNEZDO_C, OnSelchangeComboGnezdoC)
	ON_CBN_SELCHANGE(IDC_COMBO_GNEZDO_B, OnSelchangeComboGnezdoB)
	ON_CBN_SELCHANGE(IDC_COMBO_GNEZDO_E, OnSelchangeComboGnezdoE)
	ON_COMMAND(ID_TABL_COEF, OnTablCoef)
	ON_BN_CLICKED(IDC_BUTTON_MEAS, OnButtonMeas)
	ON_COMMAND(IDC_MODIFY_IP_ADDRESS, OnModifyIpAddress)
	ON_COMMAND(ID_POINT_SIZE_SMALL, OnPointSizeSmall)
	ON_COMMAND(ID_POINT_SIZE_BIG, OnPointSizeBig)
	ON_WM_VSCROLL()
	ON_WM_HIBERNATE()
	ON_BN_CLICKED(IDC_CHECKBAZAREJHIGH, OnCheckbazarejhigh)
	ON_BN_CLICKED(IDC_RADIO_SYNPOSITIVE, OnRadioSynpositive)
	ON_BN_CLICKED(IDC_RADIO_SYNNEGATIVE, OnRadioSynnegative)
	ON_BN_CLICKED(IDC_CHECK_BAZA_VKL, OnCheckBazaVkl)
	ON_BN_CLICKED(IDC_CHECK_FAMILY, OnCheckFamily)
	ON_CBN_SELENDOK(IDC_COMBO_QUE1, OnSelendokComboQue1)
	ON_EN_KILLFOCUS(IDC_EDITMIN, OnKillfocusEditmin)
	ON_EN_KILLFOCUS(IDC_EDITMAX, OnKillfocusEditmax)
	ON_COMMAND(ID_DIFFERENCE_VAX, OnDifferenceVax)
	ON_EN_KILLFOCUS(IDC_EDITSTEPBAZA, OnKillfocusEditstepbaza)
	ON_COMMAND(ID_ENDLABELEDIT,OnEndLabelEdit)
	ON_MESSAGE(WM_BUTTON, OnButton)
	ON_MESSAGE(WM_RESPONSE, OnResponse)
	ON_MESSAGE(WM_MASSIV, OnMassiv)
	ON_MESSAGE(WM_RERRESH,OnRefresh)

	ON_BN_CLICKED(IDC_CANCEL, OnCancel)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineDlg message handlers

BOOL CLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

		// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	BOOL b = GetPassWordLine();
	b = GetKoefPsw();

	int cx= ::GetSystemMetrics(SM_CXSCREEN);
	int cy= ::GetSystemMetrics(SM_CYSCREEN);
	RectWnd.left=0;
	RectWnd.right=cx;
	RectWnd.top=0;
	RectWnd.bottom=cy;
	AfxGetApp()->m_pMainWnd->MoveWindow(&RectWnd);
	
	ShowTaskBar(false);

	int dh = RectWnd.bottom/20 +6;//3;
	int dw = RectWnd.right/4;


 	m_RectWndGraf.left   = RectWnd.left+135;
	m_RectWndGraf.top    = RectWnd.top+70;//51;//+(RectWnd.bottom/20)+8;
	m_RectWndGraf.right  = RectWnd.right - dw - 3;//13;
	m_RectWndGraf.bottom = RectWnd.bottom - dh -94;//100;


	Width  = m_RectWndGraf.right-m_RectWndGraf.left;
	Height = m_RectWndGraf.bottom-m_RectWndGraf.top;

 	RectExit.left   = RectWnd.left;
	RectExit.top    = RectWnd.top;
	RectExit.right  = RectWnd.left+20;
	RectExit.bottom = RectWnd.top+20;
	RgnExit.CreateRectRgnIndirect(&RectExit);

//Cоздание образа виртуального окна графика
//=========================================
	CClientDC dc(this);	

	brFon.CreateSolidBrush(m_colorFon);

	m_penSetka_DASH.CreatePen(PS_DASH,1,m_colorSetka);  
	m_penSetka_SOLID.CreatePen(PS_SOLID,1,m_colorSetka);   
	m_penTangent.CreatePen(PS_SOLID,1,m_colorTangent);
	m_penFon.CreatePen(PS_SOLID,1,m_colorFon);
	m_penCurve.CreatePen(PS_SOLID,1,m_colorCurve);
	m_penRef.CreatePen(PS_SOLID,1,m_colorRef);

	//Панель "Виртуальные кнопки"
    //============================
	RectButtons.left=RectWnd.left;
	RectButtons.top=RectWnd.bottom-dh;//+3;
	RectButtons.right=RectWnd.right-dw-3;//13;
	RectButtons.bottom=RectWnd.bottom-2;
	int m = (RectButtons.bottom-RectButtons.top)/8;
	int l = RectButtons.right/8;//7
	int v = 1;

    GetDlgItem(IDC_BUTTON_FILE)->GetWindowRect(&RectButtonFile);
	RectButtonFile.left=RectButtons.left+2;
	RectButtonFile.top=RectButtons.top;
	RectButtonFile.right=RectButtonFile.left+l-5;
	RectButtonFile.bottom=RectButtons.bottom;

    GetDlgItem(IDC_BUTTON_TEST)->GetWindowRect(&RectButtonTest);
	RectButtonTest.left=RectButtonFile.right+v;
	RectButtonTest.top=RectButtonFile.top;
	RectButtonTest.right=RectButtonTest.left+l-5;
	RectButtonTest.bottom=RectButtonFile.bottom;

    GetDlgItem(IDC_BUTTON_TUNING)->GetWindowRect(&RectButtonTuning);
	RectButtonTuning.left=RectButtonTest.right+v;
	RectButtonTuning.top=RectButtonFile.top;
	RectButtonTuning.right=RectButtonTuning.left+l+6+2+2;
	RectButtonTuning.bottom=RectButtonFile.bottom;

    GetDlgItem(IDC_BUTTON_MEMORY)->GetWindowRect(&RectButtonMemory);
	RectButtonMemory.left=RectButtonTuning.right+v;
	RectButtonMemory.top=RectButtonFile.top;
	RectButtonMemory.right=RectButtonMemory.left+l-2;
	RectButtonMemory.bottom=RectButtonFile.bottom;

    GetDlgItem(IDC_BUTTON_GRAF)->GetWindowRect(&RectButtonGraf);
	RectButtonGraf.left=RectButtonMemory.right+v;
	RectButtonGraf.top=RectButtonFile.top;
	RectButtonGraf.right=RectButtonGraf.left+l;
	RectButtonGraf.bottom=RectButtonFile.bottom;

    GetDlgItem(IDC_BUTTON_RESET)->GetWindowRect(&RectButtonReset);
	RectButtonReset.left=RectButtonGraf.right+v;
	RectButtonReset.top=RectButtonFile.top;
	RectButtonReset.right=RectButtonReset.left+l-3-1;
	RectButtonReset.bottom=RectButtonFile.bottom;

    GetDlgItem(IDC_BUTTON_OTCHT)->GetWindowRect(&RectButtonOtcht);
	RectButtonOtcht.left=RectButtonReset.right+v;
	RectButtonOtcht.top=RectButtonFile.top;
	RectButtonOtcht.right=RectButtonOtcht.left+l-7;
	RectButtonOtcht.bottom=RectButtonFile.bottom;

    GetDlgItem(IDC_BUTTON_MEAS)->GetWindowRect(&RectButtonMeas);
	RectButtonMeas.left=RectButtonOtcht.right+v;
	RectButtonMeas.top=RectButtonFile.top;
	RectButtonMeas.right=RectButtonMeas.left+l+7;
	RectButtonMeas.bottom=RectButtonFile.bottom;

	CStatic* pSt;
	ScreenToClient(&RectButtonFile);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_FILE);
	pSt->MoveWindow(&RectButtonFile);

	ScreenToClient(&RectButtonTest);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_TEST);
	pSt->MoveWindow(&RectButtonTest);

	ScreenToClient(&RectButtonTuning);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_TUNING);
	pSt->MoveWindow(&RectButtonTuning);

	ScreenToClient(&RectButtonMemory);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_MEMORY);
	pSt->MoveWindow(&RectButtonMemory);

	ScreenToClient(&RectButtonGraf);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_GRAF);
	pSt->MoveWindow(&RectButtonGraf);

	ScreenToClient(&RectButtonOtcht);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_OTCHT);
	pSt->MoveWindow(&RectButtonOtcht);

	ScreenToClient(&RectButtonReset);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_RESET);
	pSt->MoveWindow(&RectButtonReset);

	ScreenToClient(&RectButtonMeas);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_MEAS);
	pSt->MoveWindow(&RectButtonMeas);

    ScreenToClient(&RectButtons);



//Таблица результатов
//===================
	RectTablRes.left=RectWnd.left+135;//2;
	RectTablRes.top=m_RectWndGraf.bottom+3;//RectWnd.top+(RectWnd.bottom/20)+8;///8);
	RectTablRes.right=RectWnd.right-dw-3;//13;
	RectTablRes.bottom=RectButtons.top-3;
	ScreenToClient(&RectTablRes);
    pSt = (CStatic*)GetDlgItem(IDC_LIST_RESULT);
	pSt->MoveWindow(&RectTablRes);
//Панель "Отчёт"
//==============
	RectOtcht.left=RectTablRes.left;
	RectOtcht.top=RectTablRes.top;
	RectOtcht.right=RectTablRes.right;
	RectOtcht.bottom=RectButtons.top-2;

	ScreenToClient(&RectOtcht);
    pSt = (CStatic*)GetDlgItem(IDC_EDITOTCHT);
	pSt->MoveWindow(&RectOtcht);
//Панель "Модель измерения"
//=========================
	RectTree.left=RectWnd.left+2;
	RectTree.top=RectWnd.top+97;//72;//162;//142;//82;//97;
	RectTree.right=RectTree.left+133;
	RectTree.bottom=RectTablRes.top-104;//102;//132;//162;//62;//-32;

	ScreenToClient(&RectTree);
	m_TreeCtrl.MoveWindow(&RectTree);
	
	RectNameFileMod.left=RectWnd.left+2;//*(RectWnd.right/4);
	RectNameFileMod.top=RectTree.top-25;//RectWnd.top+45;//135;//115;//55;//70;//RectWnd.bottom/20+50;//1;
	RectNameFileMod.right=RectTree.left+133;//128;//+dh;
	RectNameFileMod.bottom=RectTree.top;//RectOtcht.top;//RectWnd.bottom-(RectWnd.bottom/20)-4;
	ScreenToClient(&RectNameFileMod);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_NAMEFILE);
	pSt->MoveWindow(&RectNameFileMod);


	//Панель "Режим"
	GetDlgItem(IDC_STATICREJIM)->GetWindowRect(&RectRejim);
	RectRejim.left=RectTree.left+2;
	RectRejim.top=RectTree.bottom+60;//46;//50;//60;//70;//2;
	RectRejim.right=RectTree.right-2;
	RectRejim.bottom=RectButtons.top-2;//RectTablRes.top-2;//

	GetDlgItem(IDC_STATICCHANELC)->GetWindowRect(&RectChanelC);
	RectChanelC.left=RectRejim.left+10;
	RectChanelC.top=RectRejim.top+24;//30;
	RectChanelC.right=RectChanelC.left+70;
	RectChanelC.bottom=RectChanelC.top+20;//

	GetDlgItem(IDC_STATICCHANELB)->GetWindowRect(&RectChanelB);
	RectChanelB.left=RectChanelC.left;
	RectChanelB.top=RectChanelC.bottom+10;//14;
	RectChanelB.right=RectChanelC.right;
	RectChanelB.bottom=RectChanelB.top+20;//

	GetDlgItem(IDC_STATICTIPIMP)->GetWindowRect(&RectRejimChanelC);
	RectRejimChanelC.left=RectChanelC.right+4;
	RectRejimChanelC.top=RectChanelC.top+4;
	RectRejimChanelC.right=RectRejimChanelC.left+32;
	RectRejimChanelC.bottom=RectRejimChanelC.top+16;//

	GetDlgItem(IDC_STATICTIPDCPOS)->GetWindowRect(&RectRejimChanelB);
	RectRejimChanelB.left=RectRejimChanelC.left;
	RectRejimChanelB.top=RectChanelB.top+4;
	RectRejimChanelB.right=RectRejimChanelC.right;
	RectRejimChanelB.bottom=RectRejimChanelB.top+16;//

	RectRejimChanelCB.left=RectRejimChanelC.left;
	RectRejimChanelCB.top=RectChanelB.top-10;
	RectRejimChanelCB.right=RectRejimChanelC.right;
	RectRejimChanelCB.bottom=RectRejimChanelCB.top+16;//

	GetDlgItem(IDC_STATIC_QUE1)->GetWindowRect(&RectTextQue1);
	RectTextQue1.left=RectTree.left+24;
	RectTextQue1.top=RectChanelB.bottom+6;//14;//
	RectTextQue1.right=RectTree.right-14;
	RectTextQue1.bottom=RectTextQue1.top+20;//

	GetDlgItem(IDC_COMBO_QUE1)->GetWindowRect(&RectQue1);
	RectQue1.left=RectTree.left+14;
	RectQue1.top=RectTextQue1.bottom+4;//RectRejim.bottom-40;//RectChanelB.bottom+20;//
	RectQue1.right=RectTree.right-14;
	RectQue1.bottom=RectQue1.top+10;//

	ScreenToClient(&RectQue1);
    pSt = (CStatic*)GetDlgItem(IDC_COMBO_QUE1);
	pSt->MoveWindow(&RectQue1);

	ScreenToClient(&RectTextQue1);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_QUE1);
	pSt->MoveWindow(&RectTextQue1);

	ScreenToClient(&RectRejim);
    pSt = (CStatic*)GetDlgItem(IDC_STATICREJIM);
	pSt->MoveWindow(&RectRejim);

	ScreenToClient(&RectChanelC);
    pSt = (CStatic*)GetDlgItem(IDC_STATICCHANELC);
	pSt->MoveWindow(&RectChanelC);
	
	ScreenToClient(&RectChanelB);
    pSt = (CStatic*)GetDlgItem(IDC_STATICCHANELB);
	pSt->MoveWindow(&RectChanelB);
	
	ScreenToClient(&RectRejimChanelC);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPIMP);
	pSt->MoveWindow(&RectRejimChanelC);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPIMP1);
	pSt->MoveWindow(&RectRejimChanelC);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPAC);
	pSt->MoveWindow(&RectRejimChanelC);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPIMPNEG);
	pSt->MoveWindow(&RectRejimChanelC);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPDCNEG);
	pSt->MoveWindow(&RectRejimChanelC);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPIMP1NEG);
	pSt->MoveWindow(&RectRejimChanelC);
	
	ScreenToClient(&RectRejimChanelB);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPDCPOS);
	pSt->MoveWindow(&RectRejimChanelB);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPCONSTPOS);
	pSt->MoveWindow(&RectRejimChanelB);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPCONSTNEG);
	pSt->MoveWindow(&RectRejimChanelB);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPIMP2NEG);
	pSt->MoveWindow(&RectRejimChanelB);
    pSt = (CStatic*)GetDlgItem(IDC_STATICTIPIMP2);
	pSt->MoveWindow(&RectRejimChanelB);


	//Панель "Индикатор памяти"

	RectPicFreeMemory.left=RectWnd.left+30;
	RectPicFreeMemory.top=RectTree.bottom+12;//10;//+8;
	RectPicFreeMemory.right=RectPicFreeMemory.left+22;
	RectPicFreeMemory.bottom=RectPicFreeMemory.top+22;
	ScreenToClient(&RectPicFreeMemory);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_MEMORY);
	pSt->MoveWindow(&RectPicFreeMemory);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_MEMORY_FULL);
	pSt->MoveWindow(&RectPicFreeMemory);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	RectProgressFreeMemory.left=RectWnd.left+4;//10;
	RectProgressFreeMemory.top=RectTree.bottom+44;//40;//+8;
	RectProgressFreeMemory.right=RectTree.right;//-45;
	RectProgressFreeMemory.bottom=RectProgressFreeMemory.top+12;//RectTablRes.top-2;//-4;
	ScreenToClient(&RectProgressFreeMemory);
    pSt = (CStatic*)GetDlgItem(IDC_PROGRESS_FREE_MEMORY);
	pSt->MoveWindow(&RectProgressFreeMemory);

	RectTextFreeMemory.left=RectWnd.left+58;//RectProgressFreeMemory.right+2;
	RectTextFreeMemory.top=RectTree.bottom+14;//RectProgressFreeMemory.top;//
	RectTextFreeMemory.right=RectTree.right;
	RectTextFreeMemory.bottom=RectTextFreeMemory.top+22;//RectProgressFreeMemory.top-6;//RectProgressFreeMemory.bottom;


	ScreenToClient(&RectTextFreeMemory);
    pSt = (CStatic*)GetDlgItem(IDC_ST_FREEMEMORY);
	pSt->MoveWindow(&RectTextFreeMemory);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
	m_progress_flash.SetRange(0,100);//100-free_space);
	m_progress_flash.SetPos(0);//100-free_space);

	WhatFreeMemory();


//Панель управления (включает: окно текущей информации, параметры, //кнопки "Отмена" и "Применить")
//=====================================================================================================
	RectPanel.left=m_RectWndGraf.right;//+2;
	RectPanel.top=RectWnd.top;//RectWnd.top+(RectWnd.bottom/20)+8;///8);
	RectPanel.right=RectWnd.right;
	RectPanel.bottom=RectPanel.top+3*(RectWnd.bottom/4);
	//"окнo текущей информации"
	//=========================
	RectInfoTest.left=RectWnd.left;//+2;
	RectInfoTest.top=RectWnd.top+1;//+2;
	RectInfoTest.right=RectPanel.left;//+1;//RectWnd.right-2;
	RectInfoTest.bottom=RectWnd.top+70;//51;//+(RectWnd.bottom/20)+10;//+13;

	//текущая дата и время
	GetDlgItem(IDC_STATICEDDATE)->GetWindowRect(&RectEdDate);
	RectEdDate.left=RectInfoTest.left+2;//5;//2;//4;
	RectEdDate.top=RectInfoTest.top+2;//5;//2;
	RectEdDate.right=RectEdDate.left+RectWnd.right/10+30;//25;//7;//-4;
	RectEdDate.bottom=RectInfoTest.bottom-2;//-5;//-23;//5//+15;//RectEdDate.top+RectInfoTest.bottom/2;//-1;//2;

	//текущее сообщение (информация) пользователю
	GetDlgItem(IDC_STATICEDINFO)->GetWindowRect(&RectEdInfo);
	RectEdInfo.left=RectEdDate.right+2;//5;//+1;//RectInfoTest.left+1;
	RectEdInfo.top=RectInfoTest.top+1;//2;
	RectEdInfo.right=RectPanel.left;//-1;
	RectEdInfo.bottom=RectInfoTest.bottom-2;//RectEdInfo.top+RectInfoTest.bottom/2-2;
	fontMessage.CreateFont((RectEdInfo.bottom-RectEdInfo.top)/2 - 4,
		                 0,
					     0,0,FW_NORMAL,
	                     0,0,0,RUSSIAN_CHARSET,
					     OUT_DEFAULT_PRECIS,
					     CLIP_DEFAULT_PRECIS,
					     DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
					     _T("Arial"));
	fontParam.CreateFont((RectEdInfo.bottom-RectEdInfo.top)/3-5,
		                 0,
					     0,0,FW_NORMAL,
	                     0,0,0,RUSSIAN_CHARSET,
					     OUT_DEFAULT_PRECIS,
					     CLIP_DEFAULT_PRECIS,
					     DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
					     _T("Arial"));

	fontStr.CreateFont((RectEdInfo.bottom-RectEdInfo.top)/3,//-1,
		                 0,
					     0,0,FW_NORMAL,
	                     0,0,0,RUSSIAN_CHARSET,
					     OUT_DEFAULT_PRECIS,
					     CLIP_DEFAULT_PRECIS,
					     DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
					     _T("Arial"));

	ScreenToClient(&RectInfoTest);
	
	ScreenToClient(&RectEdDate);
    CEdit* pEd = (CEdit*)GetDlgItem(IDC_STATICEDDATE);
	pEd->MoveWindow(&RectEdDate);

	ScreenToClient(&RectEdInfo);
    pEd = (CEdit*)GetDlgItem(IDC_STATICEDINFO);
	pEd->MoveWindow(&RectEdInfo);


    //Параметры
    //==========
	RectParam.left=m_RectWndGraf.right;//+1;
	RectParam.top=RectPanel.top;
	RectParam.right=RectPanel.right;//-2;
	RectParam.bottom=RectWnd.bottom-1*(RectWnd.bottom/4)-40;//-19-4;//-20+4-3;
	
	//Для закладок Page1, Page2, Page3 ("Схема включения", "Коллектор" и "База")
	//-------------------------------------------------
	RectPage12.left=RectParam.left;
	RectPage12.top=RectParam.top+(RectWnd.bottom/35)*2-RectWnd.bottom/60+10+17;//4;
	RectPage12.right=RectParam.right;
	RectPage12.bottom=RectParam.bottom;

	//Для закладок Page4, Page5 ("Дополнительный канал" и "Расчёт")
	//--------------------------------------------------------------
	RectPage34.left=RectParam.left;
	RectPage34.top=RectParam.top+(RectWnd.bottom/35);
	RectPage34.right=RectParam.right;
	RectPage34.bottom=RectParam.bottom-(RectWnd.bottom/35);

	UINT t = (RectPage12.right-RectPage12.left)/22;	// отступ
	UINT h = (RectWnd.bottom/12);					// высота
	UINT w = (RectPage12.right-RectPage12.left)/2;  // ширина



	//Закладка "Схема включения"
	//==========================
	GetDlgItem(IDC_STATIC_ELEMENT)->GetWindowRect(&RectCategoria);
	RectCategoria.left=RectPage12.left+t/2+1;//+2;
	RectCategoria.top=RectPage12.top;//+5;//-5;//+5;//+h/3-4;//h/2;//+8;//-14;//h/4+2;//+h/2;
	RectCategoria.right=RectPage12.left+w-t*2-t/2;//-4;
	RectCategoria.bottom=RectCategoria.top+h*5-8;//4+15;+25;//*5+13;//7+18;//23;//28;//6;//-6;//h/4;//RectPage12.bottom-h+2;

	GetDlgItem(IDC_STATICBITMAP_DIOD)->GetWindowRect(&RectBitmapDiod);
	RectBitmapDiod.left=RectCategoria.left+t-1;//-2;//+2;
	RectBitmapDiod.top=RectCategoria.top+h/2-6;//-9;//+4;//10;//8;
	RectBitmapDiod.right=RectBitmapDiod.left+32;//25;//17;
	RectBitmapDiod.bottom=RectBitmapDiod.top+41;//35;//25;
	RgnDiod.CreateRectRgnIndirect(&RectBitmapDiod);

	RectRadioDiod.left=RectBitmapDiod.left;//-2;//+6;//-t/2;//-2;//;
	RectRadioDiod.top=RectBitmapDiod.bottom+4;//5;//9;
	RectRadioDiod.right=RectRadioDiod.left+32;
	RectRadioDiod.bottom=RectRadioDiod.top+7;//-4;

	GetDlgItem(IDC_STATICBITMAP_RES)->GetWindowRect(&RectBitmapRes);
	RectBitmapRes.left=RectBitmapDiod.right+t*2-10;//6;//3;//+2;
	RectBitmapRes.top=RectBitmapDiod.top;
	RectBitmapRes.right=RectBitmapRes.left+32;//25;
	RectBitmapRes.bottom=RectBitmapRes.top+41;//35;
	RgnRes.CreateRectRgnIndirect(&RectBitmapRes);

	RectRadioRes.left=RectBitmapRes.left;//-2;//+6;//-t/2;//-2;//-3;
	RectRadioRes.top=RectBitmapRes.bottom+4;//5;//9;
	RectRadioRes.right=RectRadioRes.left+32;
	RectRadioRes.bottom=RectRadioRes.top+7;//-4;

	GetDlgItem(IDC_STATICBITMAP_NPN)->GetWindowRect(&RectBitmapNpn);
	RectBitmapNpn.left=RectBitmapDiod.left;
	RectBitmapNpn.top=RectBitmapDiod.bottom+h/6+6;//-2;//14;//6;//18;//20;//18;
	RectBitmapNpn.right=RectBitmapNpn.left+32;//25;
	RectBitmapNpn.bottom=RectBitmapNpn.top+41;//35;
	RgnNpn.CreateRectRgnIndirect(&RectBitmapNpn);

	RectRadioNpn.left=RectBitmapNpn.left;//-2;//+6;//-t/2;//-2;//-3;
	RectRadioNpn.top=RectBitmapNpn.bottom+4;//5;//9;
	RectRadioNpn.right=RectRadioNpn.left+32;//t*3;
	RectRadioNpn.bottom=RectRadioNpn.top+7;//h/3-10;//-4;

	GetDlgItem(IDC_STATICBITMAP_PNP)->GetWindowRect(&RectBitmapPnp);
	RectBitmapPnp.left=RectBitmapNpn.right+t*2-10;//3;//+2;
	RectBitmapPnp.top=RectBitmapNpn.top;
	RectBitmapPnp.right=RectBitmapPnp.left+32;//25;
	RectBitmapPnp.bottom=RectBitmapPnp.top+41;//35;
	RgnPnp.CreateRectRgnIndirect(&RectBitmapPnp);

	RectRadioPnp.left=RectBitmapPnp.left;//-2;//+6;//-t/2;//-1;//-3;
	RectRadioPnp.top=RectBitmapPnp.bottom+4;//5;//9;
	RectRadioPnp.right=RectRadioPnp.left+32;
	RectRadioPnp.bottom=RectRadioPnp.top+7;//-4;

	GetDlgItem(IDC_STATICBITMAP_NMOS)->GetWindowRect(&RectBitmapNmos);
	RectBitmapNmos.left=RectBitmapDiod.left;
	RectBitmapNmos.top=RectBitmapNpn.bottom+h/6+6;//-2;//+14;//6;//18;//20;//18;
	RectBitmapNmos.right=RectBitmapNmos.left+32;//25;
	RectBitmapNmos.bottom=RectBitmapNmos.top+41;//35;
	RgnNmos.CreateRectRgnIndirect(&RectBitmapNmos);

	RectRadioNmos.left=RectBitmapNmos.left;//-2;//+6;//-t/2;//-2;//-3;
	RectRadioNmos.top=RectBitmapNmos.bottom+4;//5;//9;
	RectRadioNmos.right=RectRadioNmos.left+32;
	RectRadioNmos.bottom=RectRadioNmos.top+7;//-4;

	GetDlgItem(IDC_STATICBITMAP_PMOS)->GetWindowRect(&RectBitmapPmos);
	RectBitmapPmos.left=RectBitmapNmos.right+t*2-10;//3;//+4;
	RectBitmapPmos.top=RectBitmapNmos.top;
	RectBitmapPmos.right=RectBitmapPmos.left+32;//25;
	RectBitmapPmos.bottom=RectBitmapPmos.top+41;//35;
	RgnPmos.CreateRectRgnIndirect(&RectBitmapPmos);

	RectRadioPmos.left=RectBitmapPmos.left;//-2;//+6;//-t/2;//-1;//-3;
	RectRadioPmos.top=RectBitmapPmos.bottom+4;//5;//9;
	RectRadioPmos.right=RectRadioPmos.left+32;
	RectRadioPmos.bottom=RectRadioPmos.top+7;//-4;

	GetDlgItem(IDC_STATICBITMAP_NPN4)->GetWindowRect(&RectBitmapNpn4);
	RectBitmapNpn4.left=RectBitmapDiod.left;
	RectBitmapNpn4.top=RectBitmapNmos.bottom+h/6+6;//-2;//+8;//+14;//6;//18;//20;//18;
	RectBitmapNpn4.right=RectBitmapNpn4.left+32;//26;
	RectBitmapNpn4.bottom=RectBitmapNpn4.top+41;//35;
	RgnNpn4.CreateRectRgnIndirect(&RectBitmapNpn4);

	RectRadioNpn4.left=RectBitmapNpn4.left;//-2;//+6;//-t/2;//-2;//-3;
	RectRadioNpn4.top=RectBitmapNpn4.bottom+4;//5;//9;
	RectRadioNpn4.right=RectRadioNpn4.left+32;
	RectRadioNpn4.bottom=RectRadioNpn4.top+7;//-4;

	GetDlgItem(IDC_STATICBITMAP_PNP4)->GetWindowRect(&RectBitmapPnp4);
	RectBitmapPnp4.left=RectBitmapNpn4.right+t*2-10;//3;//+4;
	RectBitmapPnp4.top=RectBitmapNpn4.top;
	RectBitmapPnp4.right=RectBitmapPnp4.left+32;//26;
	RectBitmapPnp4.bottom=RectBitmapPnp4.top+41;//35;
	RgnPnp4.CreateRectRgnIndirect(&RectBitmapPnp4);

	RectRadioPnp4.left=RectBitmapPnp4.left;//-2;//+6;//-t/2;//-1;//-3;
	RectRadioPnp4.top=RectBitmapPnp4.bottom+4;//5;//9;
	RectRadioPnp4.right=RectRadioPnp4.left+32;
	RectRadioPnp4.bottom=RectRadioPnp4.top+7;//-4;//;

	GetDlgItem(IDC_STATICBITMAP_NMOS4)->GetWindowRect(&RectBitmapNmos4);
	RectBitmapNmos4.left=RectBitmapDiod.left;
	RectBitmapNmos4.top=RectBitmapNpn4.bottom+h/6+6;//-2;//+14;//6;//18;//20;//18;
	RectBitmapNmos4.right=RectBitmapNmos4.left+32;//26;
	RectBitmapNmos4.bottom=RectBitmapNmos4.top+41;//35;
	RgnNmos4.CreateRectRgnIndirect(&RectBitmapNmos4);

	RectRadioNmos4.left=RectBitmapNmos4.left;//-2;//+6;//-t/2;//-2;//-3;
	RectRadioNmos4.top=RectBitmapNmos4.bottom+4;//5;//9;
	RectRadioNmos4.right=RectRadioNmos4.left+32;
	RectRadioNmos4.bottom=RectRadioNmos4.top+7;//-4;

	GetDlgItem(IDC_STATICBITMAP_PMOS4)->GetWindowRect(&RectBitmapPmos4);
	RectBitmapPmos4.left=RectBitmapNmos4.right+t*2-10;//3;//+4;
	RectBitmapPmos4.top=RectBitmapNmos4.top;
	RectBitmapPmos4.right=RectBitmapPmos4.left+32;//26;
	RectBitmapPmos4.bottom=RectBitmapPmos4.top+41;//35;
	RgnPmos4.CreateRectRgnIndirect(&RectBitmapPmos4);

	RectRadioPmos4.left=RectBitmapPmos4.left;//-2;//+6;//-t/2;//-1;//-3;
	RectRadioPmos4.top=RectBitmapPmos4.bottom+4;//5;//9;
	RectRadioPmos4.right=RectRadioPmos4.left+32;
	RectRadioPmos4.bottom=RectRadioPmos4.top+7;//-4;

	//Коммутатор
	GetDlgItem(IDC_STATICKNOT)->GetWindowRect(&RectKnot);
	RectKnot.left=RectCategoria.left;//right+t/2-2;
	RectKnot.top=RectCategoria.top+h*2-16;//10;
	RectKnot.right=RectPage12.right-t/2-2;
	RectKnot.bottom=RectPage12.bottom-8;//RectCategoria.bottom;//RectKnot.top+h*5+6;//3+2;//+4;//-3;//+2;

	GetDlgItem(IDC_COMBO_KOMMUTATOR)->GetWindowRect(&RectKommutator);
	RectKommutator.left=RectCategoria.right+t-2;
	RectKommutator.top=RectKnot.top+h/2-6;//6;
	RectKommutator.right=RectKnot.right-t/2;
	RectKommutator.bottom=RectKommutator.top+h/3;

	GetDlgItem(IDC_STATICGNEZDO_C)->GetWindowRect(&RectKnotKol);
	RectKnotKol.left=RectCategoria.right+t/2-8+t;//RectKnot.left+t;
	RectKnotKol.top=RectKnot.top+h;//+18;//4;
	RectKnotKol.right=RectKnotKol.left+35;//w/8;
	RectKnotKol.bottom=RectKnotKol.top+22;//+h/3;

	GetDlgItem(IDC_STATICGNEZDO_B)->GetWindowRect(&RectKnotBaza);
	RectKnotBaza.left=RectKnotKol.left;
	RectKnotBaza.top=RectKnotKol.bottom+h/3-11;
	RectKnotBaza.right=RectKnotKol.right;
	RectKnotBaza.bottom=RectKnotBaza.top+22;//+h/3;

	GetDlgItem(IDC_STATICGNEZDO_S)->GetWindowRect(&RectKnotDop);
	RectKnotDop.left=RectKnotKol.left;
	RectKnotDop.top=RectKnotBaza.bottom+h/3-11;
	RectKnotDop.right=RectKnotKol.right;
	RectKnotDop.bottom=RectKnotDop.top+22;//+h/3;

	GetDlgItem(IDC_STATICGNEZDO_E)->GetWindowRect(&RectKnotEm);
	RectKnotEm.left=RectKnotKol.left;
	RectKnotEm.top=RectKnotDop.bottom+h/3-11;//6;//2;
	RectKnotEm.right=RectKnotEm.left+56;//RectKnotKol.right;
	RectKnotEm.bottom=RectKnotEm.top+22;//+h/3;

	GetDlgItem(IDC_COMBOCHECKKOL)->GetWindowRect(&RectStatusKol);
	RectStatusKol.left=RectKnotKol.right+t/2;
	RectStatusKol.top=RectKnotKol.top;
	RectStatusKol.right=RectKnot.right-t/2;
	RectStatusKol.bottom=RectStatusKol.top+h/4;

	GetDlgItem(IDC_COMBOCHECKBAZA)->GetWindowRect(&RectStatusBaza);
	RectStatusBaza.left=RectStatusKol.left;
	RectStatusBaza.top=RectKnotBaza.top;
	RectStatusBaza.right=RectStatusKol.right;
	RectStatusBaza.bottom=RectStatusBaza.top+h/4;

	GetDlgItem(IDC_COMBOCHECKDOP)->GetWindowRect(&RectStatusDop);
	RectStatusDop.left=RectStatusKol.left;
	RectStatusDop.top=RectKnotDop.top;
	RectStatusDop.right=RectStatusKol.right;
	RectStatusDop.bottom=RectStatusDop.top+h/4;

	//Тест
	GetDlgItem(IDC_STATICMOD)->GetWindowRect(&RectTest);
	RectTest.left=RectCategoria.right+t/2-1;//RectKnot.left;
	RectTest.top=RectCategoria.top;//RectStatusEm.bottom+140;//131;//89;//RectKnot.bottom+88;//RectKnot.bottom+2;//RectBitmapCircuit.bottom+2;//RectKnot.bottom+2;//+2;//4;
	RectTest.right=RectKnot.right;
	RectTest.bottom=RectTest.top+h*2-23;//17;//15;//RectCategoria.bottom;//;

	GetDlgItem(IDC_COMBOMOD)->GetWindowRect(&RectComboTest);
	RectComboTest.left=RectTest.left+t-2;
	RectComboTest.top=RectTest.top+h/2-9;//+4;//-2-3;
	RectComboTest.right=RectTest.right-t/2-2;
	RectComboTest.bottom=RectComboTest.top+h/3;

	GetDlgItem(IDC_BUTTON_LOADTST)->GetWindowRect(&RectButtonLoadTst);
	RectButtonLoadTst.left=RectComboTest.left+t;
	RectButtonLoadTst.top=RectComboTest.bottom+10;//h/2-6;//-2;
	RectButtonLoadTst.right=RectComboTest.right-t;
	RectButtonLoadTst.bottom=RectButtonLoadTst.top+h/2+4;//RectTest.bottom-h/6-2;

	//отображение схемы включения ПП
	RectBitmapCircuit.left=RectCategoria.right+t/2-27;//RectKnot.left-25;//30;//10;//RectTest.left+10;//
	RectBitmapCircuit.top=RectKnot.bottom-125;//125;//+30;//30;//RectKnot.bottom+4;//RectTest.bottom+4;//
	RectBitmapCircuit.right=RectBitmapCircuit.left+89;//RectKnot.left+93;//RectBitmapCircuit.left+83;//79;
	RectBitmapCircuit.bottom=RectBitmapCircuit.top+89;//RectStatusEm.bottom+91;//RectKnot.bottom+87;//RectBitmapCircuit.top+83;//79;

	GetDlgItem(IDC_COMBO_GNEZDO_C)->GetWindowRect(&RectBitKnotKol);
	RectBitKnotKol.left=RectBitmapCircuit.left+30;//38;//36;//34;
	RectBitKnotKol.top=RectBitmapCircuit.top-26;//24
	RectBitKnotKol.right=RectBitKnotKol.left+54;//44;
	RectBitKnotKol.bottom=RectBitKnotKol.top+10;

	RectBitKol.left=RectBitmapCircuit.left+13;//RectBitBazaKol.right;
	RectBitKol.top=RectBitmapCircuit.top;//RectBitBazaKol.top;
	RectBitKol.right=RectBitKol.left+63;//21;//19;
	RectBitKol.bottom=RectBitKol.top+13;//21;//19;

	GetDlgItem(IDC_COMBO_GNEZDO_B)->GetWindowRect(&RectBitKnotBaza);
	RectBitKnotBaza.left=RectBitmapCircuit.left-53;//43;
	RectBitKnotBaza.top=RectBitmapCircuit.top+35;//RectBitBazaKol.bottom+5;//4;
	RectBitKnotBaza.right=RectBitKnotBaza.left+54;//44;
	RectBitKnotBaza.bottom=RectBitKnotBaza.top+10;

	RectBitBaza.left=RectBitmapCircuit.left;//RectBitKnotBaza.right;
	RectBitBaza.top=RectBitmapCircuit.top;//RectBitBazaKol.bottom;
	RectBitBaza.right=RectBitBaza.left+13;//21;//19;
	RectBitBaza.bottom=RectBitBaza.top+89;//21;//19;

	RectBitElement.left=RectBitmapCircuit.left+13;//RectBitBaza.right;
	RectBitElement.top=RectBitmapCircuit.top+13;//RectBitBaza.top;
	RectBitElement.right=RectBitElement.left+63;//21;//19;
	RectBitElement.bottom=RectBitElement.top+63;//21;//19;

	RectBitDop.left=RectBitElement.right;
	RectBitDop.top=RectBitmapCircuit.top;//RectBitElement.top;
	RectBitDop.right=RectBitDop.left+13;//21;//19;
	RectBitDop.bottom=RectBitDop.top+89;//21;//19;

	GetDlgItem(IDC_STATIC_GNEZDO_S)->GetWindowRect(&RectBitKnotDop);
	RectBitKnotDop.left=RectBitmapCircuit.right;//RectBitDop.right;
	RectBitKnotDop.top=RectBitKnotBaza.top;//RectBitDop.top+5;//4;
	RectBitKnotDop.right=RectBitKnotDop.left+54;//44;
	RectBitKnotDop.bottom=RectBitKnotDop.top+23;


	RectBitEm.left=RectBitElement.left;//RectBitBazaEm.right;
	RectBitEm.top=RectBitElement.bottom;//RectBitBazaEm.top;
	RectBitEm.right=RectBitEm.left+63;//21;//19;
	RectBitEm.bottom=RectBitEm.top+13;//21;//19;

	GetDlgItem(IDC_COMBO_GNEZDO_E)->GetWindowRect(&RectBitKnotEm);
	RectBitKnotEm.left=RectBitKnotKol.left;//RectBitEm.left+5;//4;
	RectBitKnotEm.top=RectBitmapCircuit.bottom;//RectBitEm.bottom;//RectBitmapCircuit.bottom-11;
	RectBitKnotEm.right=RectBitKnotEm.left+54;//44;
	RectBitKnotEm.bottom=RectBitKnotEm.top+10;//RectBitmapCircuit.bottom;

    ScreenToClient(&RectBitmapCircuit);
	
	ScreenToClient(&RectBitKol);
	ScreenToClient(&RectBitBaza);
	ScreenToClient(&RectBitElement);
	ScreenToClient(&RectBitDop);
	ScreenToClient(&RectBitEm);


	//Закладка Page1 ("Канал С")
	//============================
	//развёртка
	GetDlgItem(IDC_STATICRAZV)->GetWindowRect(&RectRazv);
	RectRazv.left=RectPage12.left+t/2;//+2;
	RectRazv.top=RectPage12.top-22;//+5;//+h/4;//+6;//;//;
	RectRazv.right=RectPage12.right-t+4;//RectPage12.left+w-t*2-t/2;//RectRazv.left+w-30;//-4;
	RectRazv.bottom=RectRazv.top+h*3-20;//3-14;//*4-2;//-4;//-h/4+7;//3;//4;//-2;

	GetDlgItem(IDC_RADIO_RAZVIMPPOSITIVE)->GetWindowRect(&RectRazvRadImpPos);
	RectRazvRadImpPos.left=RectRazv.left+t*2-6;
	RectRazvRadImpPos.top=RectRazv.top+h/2-6;//3;//-5;
	RectRazvRadImpPos.right=RectRazvRadImpPos.left+t*2;//w/2-t/2;
	RectRazvRadImpPos.bottom=RectRazvRadImpPos.top+(h*3)/7;//h/3-8;//

	GetDlgItem(IDC_STATICIMP1POS)->GetWindowRect(&RectIconImpPos);
	RectIconImpPos.left=RectRazvRadImpPos.right;//+4;//+t/2-4;
	RectIconImpPos.top=RectRazvRadImpPos.top+7;;////10;//5;
	RectIconImpPos.right=RectIconImpPos.left+32;//22;
	RectIconImpPos.bottom=RectIconImpPos.top+16;

	GetDlgItem(IDC_RADIO_RAZVDCPOSITIVE)->GetWindowRect(&RectRazvRadDcPos);
	RectRazvRadDcPos.left=RectRazvRadImpPos.left;
	RectRazvRadDcPos.top=RectRazvRadImpPos.top+h/2;
	RectRazvRadDcPos.right=RectRazvRadImpPos.right;
	RectRazvRadDcPos.bottom=RectRazvRadDcPos.top+(h*3)/7;//h/3-8;//+(h*3)/7;

	GetDlgItem(IDC_STATICICONDCPOS)->GetWindowRect(&RectIconDcPos);
	RectIconDcPos.left=RectRazvRadDcPos.right;//+4;//t/2-4;
	RectIconDcPos.top=RectRazvRadDcPos.top+7;//10;//5;
	RectIconDcPos.right=RectIconDcPos.left+32;//22;
	RectIconDcPos.bottom=RectIconDcPos.top+16;

	GetDlgItem(IDC_RADIO_SYNPOSITIVE)->GetWindowRect(&RectRazvRadSynPos);
	RectRazvRadSynPos.left=RectRazvRadImpPos.left;//RectStatImp.left+t*2;
	RectRazvRadSynPos.top=RectRazvRadDcPos.top+h/2;//-2;//RectRazvRadImpPos.top+2;//RectStatImp.top+h/2;//-2;
	RectRazvRadSynPos.right=RectRazvRadImpPos.right;//RectTipImp.left+t+t/2;
	RectRazvRadSynPos.bottom=RectRazvRadSynPos.top+(h*3)/7;//h/3-8;//+(h*3)/7;//RectTipImp.top+h/3;

	GetDlgItem(IDC_STATICIMPPOSITIVE)->GetWindowRect(&RectIconSynPos);
	RectIconSynPos.left=RectRazvRadSynPos.right;//t/2-4;
	RectIconSynPos.top=RectRazvRadSynPos.top+7;//10;//5;
	RectIconSynPos.right=RectIconSynPos.left+32;//22;
	RectIconSynPos.bottom=RectIconSynPos.top+16;


	GetDlgItem(IDC_RADIO_RAZVIMPNEGATIVE)->GetWindowRect(&RectRazvRadImpNeg);
	RectRazvRadImpNeg.left=RectIconImpPos.right+t+10;//;//RectRazvRadImpPos.left;
	RectRazvRadImpNeg.top=RectRazvRadImpPos.top;//RectRazvRadDcNeg.top+h/2;//-2;
	RectRazvRadImpNeg.right=RectRazvRadImpNeg.left+t*2;
	RectRazvRadImpNeg.bottom=RectRazvRadImpNeg.top+(h*3)/7;//h/3-8;//+(h*3)/7;

	GetDlgItem(IDC_STATICIMP1NEG)->GetWindowRect(&RectIconImpNeg);
	RectIconImpNeg.left=RectRazvRadImpNeg.right;//t/2-4;
	RectIconImpNeg.top=RectRazvRadImpNeg.top+7;//+10;//4;
	RectIconImpNeg.right=RectIconImpNeg.left+32;//22;
	RectIconImpNeg.bottom=RectIconImpNeg.top+16;

	GetDlgItem(IDC_RADIO_RAZVDCNEGATIVE)->GetWindowRect(&RectRazvRadDcNeg);
	RectRazvRadDcNeg.left=RectRazvRadImpNeg.left;
	RectRazvRadDcNeg.top=RectRazvRadDcPos.top;//RectRazvRadSynNeg.top+h/2;//-2;
	RectRazvRadDcNeg.right=RectRazvRadDcNeg.left+t*2;//RectRazvRadImpPos.right;
	RectRazvRadDcNeg.bottom=RectRazvRadDcNeg.top+(h*3)/7;//h/3-8;//+(h*3)/7;

	GetDlgItem(IDC_STATICICONDCNEGATIVE)->GetWindowRect(&RectIconDcNeg);
	RectIconDcNeg.left=RectRazvRadDcNeg.right;//t/2-4;
	RectIconDcNeg.top=RectRazvRadDcNeg.top+7;//+7;//10;//5;
	RectIconDcNeg.right=RectIconDcNeg.left+32;//22;
	RectIconDcNeg.bottom=RectIconDcNeg.top+16;

	GetDlgItem(IDC_RADIO_SYNNEGATIVE)->GetWindowRect(&RectRazvRadSynNeg);
	RectRazvRadSynNeg.left=RectRazvRadImpNeg.left;
	RectRazvRadSynNeg.top=RectRazvRadSynPos.top;//RectRazvRadAc.top+h/2;//-2;
	RectRazvRadSynNeg.right=RectRazvRadSynNeg.left+t*2;//RectRazvRadImpPos.right;
	RectRazvRadSynNeg.bottom=RectRazvRadSynNeg.top+(h*3)/7;//h/3-8;//+(h*3)/7;

	GetDlgItem(IDC_STATICIMPNEGATIVE)->GetWindowRect(&RectIconSynNeg);
	RectIconSynNeg.left=RectRazvRadSynNeg.right;//t/2-4;
	RectIconSynNeg.top=RectRazvRadSynNeg.top+7;//+10;//4;
	RectIconSynNeg.right=RectIconSynNeg.left+32;//22;
	RectIconSynNeg.bottom=RectIconSynNeg.top+16;

	GetDlgItem(IDC_RADIO_RAZVAC)->GetWindowRect(&RectRazvRadAc);
	RectRazvRadAc.left=RectIconSynNeg.right+t+10;//RectRazvRadImpPos.left;
	RectRazvRadAc.top=RectRazvRadImpPos.top;//RectRazvRadSynPos.top;//RectRazvRadSynPos.top+h/2;//-2;
	RectRazvRadAc.right=RectRazvRadAc.left+t*2;//RectRazvRadImpPos.right;
	RectRazvRadAc.bottom=RectRazvRadAc.top+(h*3)/7;//h/3-8;//+(h*3)/7;

	GetDlgItem(IDC_STATICAC)->GetWindowRect(&RectIconAc);
	RectIconAc.left=RectRazvRadAc.right;//t/2-4;//-2;
	RectIconAc.top=RectRazvRadAc.top+7;//10;//3;
	RectIconAc.right=RectIconAc.left+32;//22;
	RectIconAc.bottom=RectIconAc.top+16;

	//импульс
	GetDlgItem(IDC_STATICIMP)->GetWindowRect(&RectStatImp);
	RectStatImp.left=RectRazv.left+t;//RectRazv.right+t/2-2;
	RectStatImp.top=RectIconSynNeg.bottom+5;//10;//top;//-4;//RectPage12.top+h/4;
	RectStatImp.right=RectRazvRadSynNeg.left-2;
	RectStatImp.bottom=RectStatImp.top+h-15;//8;//+46;//RectRazv.bottom;//RectStatImp.top+h*2-2;//-h/4;//4-2;//

	//длительность импульса
	GetDlgItem(IDC_EDITDLITIMP)->GetWindowRect(&RectDlitImp);
	RectDlitImp.left=RectStatImp.left+t+4;//*4;
	RectDlitImp.top=RectStatImp.top+h/2-10;//8;//RectTipImp1.bottom+10;
	RectDlitImp.right=RectStatImp.right-t/2;//+2;//RectDlitImp.left+t*4;
	RectDlitImp.bottom=RectDlitImp.top+h/3;

	GetDlgItem(IDC_CHECKPAUZA)->GetWindowRect(&RectPeriod);
	RectPeriod.left=RectStatImp.right+t*2;//RectStatDlitPauza.left+5;//right-1;//RectStatImp.left+t*2;//3-4;//RectIconTipImp.left;//RectDlitImp.left;
	RectPeriod.top=RectIconSynNeg.bottom+18;//RectStatDlitPauza.top;//bottom;//-1;//RectDlitImp.bottom+8;////RectIconTipImp1.bottom+h;////RectDlitImp.bottom+8;
	RectPeriod.right=RectRazv.right-t*2;//RectStatImp.right-t/2;//*2-2;//t*4+24;
	RectPeriod.bottom=RectPeriod.top+h/3+2;//+8;

	//измеритель I (коллектор)
	GetDlgItem(IDC_STATICKOLLECTORSOURCE)->GetWindowRect(&RectKolSource);
	RectKolSource.left=RectRazv.left;//+t/2+2;
	RectKolSource.top=RectRazv.bottom+6;//RectStatImp.bottom+6;//RectKolMeas.top;
	RectKolSource.right=RectRazv.right;
	RectKolSource.bottom=RectKolSource.top+h+7;//RectKolMeas.bottom;

	GetDlgItem(IDC_EDKOLMEASI)->GetWindowRect(&RectI);
	RectI.left=RectIconSynNeg.left+t/2+4;//-4;///2;//RectKolSource.left+t;///2;//RectKU.right+t;//RectU.left;
	RectI.top=RectKolSource.top+h/2-2;//-3;//+6;//RectU.top;//RectU.bottom+h/4-4;
	RectI.right=RectI.left+t*3-2;//w/6;//+3;//5;//10;//RectU.right;
	RectI.bottom=RectI.top+h/3+6;//;RectU.bottom

	GetDlgItem(IDC_COMBOKOLI)->GetWindowRect(&RectKI);
	RectKI.left=RectI.right+4;//+6;//4;
	RectKI.top=RectI.top+1;//-2;
	RectKI.right=RectKolSource.right-t/2;//RectKU.right;
	RectKI.bottom=RectKI.top+h/3;

	//измеритель U(коллектор)
	GetDlgItem(IDC_EDKOLMEASU)->GetWindowRect(&RectU);
	RectU.left=RectKolSource.left+t/2;//+2;///2+4;//+t/2;//-2;//+2;//;
	RectU.top=RectI.top;//+h/2+14;//-2;
	RectU.right=RectU.left+t*3-2;//w/6+3;//10;//8;
	RectU.bottom=RectU.top+h/3+6;


	GetDlgItem(IDC_COMBOKOLU)->GetWindowRect(&RectKU);
	RectKU.left=RectU.right+4;//+6;//4;
	RectKU.top=RectU.top+1;//-2;
	RectKU.right=RectI.left-t+4;//-4;//RectKolSource.right-t/2;//RectRazv.right-t/2;//
	RectKU.bottom=RectKU.top+h/3;

	//ограничение(максимальная амплитуда (коллектор))
	GetDlgItem(IDC_STATICLIMIT)->GetWindowRect(&RectLimit);
	RectLimit.left=RectRazv.left;//RectKolMeas.left;
	RectLimit.top=RectKolSource.bottom+6;//RectRazv.bottom+6;//RectKolMeas.bottom+4;
	RectLimit.right=RectPage12.right-t+4;
	RectLimit.bottom=RectLimit.top+h*3+20;//12;//10;//8;//10;//4-44;//+h*3-h/2-16;//12;

	GetDlgItem(IDC_EDSLIDERKOLU)->GetWindowRect(&RectTextSliderKolU);
	RectTextSliderKolU.left=RectLimit.left+t;///2+4;//+7;
	RectTextSliderKolU.top=RectLimit.top+h/2-2;//4;
	RectTextSliderKolU.right=RectTextSliderKolU.left+w/6+10;
	RectTextSliderKolU.bottom=RectTextSliderKolU.top+h/3+6;
	
	GetDlgItem(IDC_COMBOKOLSOURU)->GetWindowRect(&RectKolSourU);
	RectKolSourU.left=RectKI.left;//-14;//+30;
	RectKolSourU.top=RectTextSliderKolU.top;//-2;
	RectKolSourU.right=RectKI.right;
	RectKolSourU.bottom=RectKolSourU.top+h/3;//RectTextSliderKolU.bottom;

	GetDlgItem(IDC_SLIDERKOLU)->GetWindowRect(&RectSliderKolU);
	RectSliderKolU.left=RectLimit.left+4;
	RectSliderKolU.top=RectLimit.top+h*2+39;//25;
	RectSliderKolU.right=RectSliderKolU.left+w+t*2+10;
	RectSliderKolU.bottom=RectSliderKolU.top+h/2+2;

	GetDlgItem(IDC_EDITLIMITKOLU)->GetWindowRect(&RectEditSliderKolU);
	RectEditSliderKolU.left=RectSliderKolU.right+12;
	RectEditSliderKolU.top=RectSliderKolU.top+6;//+2;
	RectEditSliderKolU.right=RectLimit.right-t/2-2;//RectPeriod.right;//RectDlitPauza.right;
	RectEditSliderKolU.bottom=RectEditSliderKolU.top+h/3+5;

 	GetDlgItem(IDC_SLIDERKOLI)->GetWindowRect(&RectSliderKolI);
	RectSliderKolI.left=RectSliderKolU.left;//RectLimit.left+w-4;
	RectSliderKolI.top=RectSliderKolU.top-h/2-22;//25;//20;//13;
	RectSliderKolI.right=RectSliderKolU.right;
	RectSliderKolI.bottom=RectSliderKolI.top+h/2;
		
	GetDlgItem(IDC_TEXTRANGMINKOLI)->GetWindowRect(&RectTextRangMinKolI);
	RectTextRangMinKolI.left=RectTextSliderKolU.right+15;
	RectTextRangMinKolI.top=RectTextSliderKolU.top+2;
	RectTextRangMinKolI.right=RectTextRangMinKolI.left+t*5+25;//+t*3;
	RectTextRangMinKolI.bottom=RectTextRangMinKolI.top+h/3+2;

	GetDlgItem(IDC_EDTEXTRANGMAXKOLI)->GetWindowRect(&RectTextRangMaxKolI);
	RectTextRangMaxKolI.left=RectLimit.left+t*2;//RectSliderKolI.left+50;//-t*2;//*3+2;
	RectTextRangMaxKolI.top=RectSliderKolI.top-h/2-15;//-6;//4;
	RectTextRangMaxKolI.right=RectLimit.right-t*2;//-2;//RectSliderKolI.right;
	RectTextRangMaxKolI.bottom=RectTextRangMaxKolI.top+h/3+2;//RectSliderKolI.top;

	GetDlgItem(IDC_STATICSTART)->GetWindowRect(&RectTextStart);
	RectTextStart.left=RectSliderKolI.left+10;//-t*2;//*3+2;
	RectTextStart.top=RectSliderKolI.top-h/3+4;//-6;//4;
	RectTextStart.right=RectLimit.right-t*2;//-2;//RectSliderKolI.right;
	RectTextStart.bottom=RectSliderKolI.top;
	
	GetDlgItem(IDC_STATICSTOP)->GetWindowRect(&RectTextStop);
	RectTextStop.left=RectTextStart.left;//RectTextRangMaxKolI.left;
	RectTextStop.top=RectSliderKolU.top-h/3+4;//-6;//4;
	RectTextStop.right=RectLimit.right-t*2;//-2;//RectSliderKolI.right;
	RectTextStop.bottom=RectSliderKolU.top;

	GetDlgItem(IDC_EDITLIMITKOLI)->GetWindowRect(&RectEditSliderKolI);
	RectEditSliderKolI.left=RectSliderKolI.right+12;
	RectEditSliderKolI.top=RectSliderKolI.top;//+2;
	RectEditSliderKolI.right=RectEditSliderKolU.right;
	RectEditSliderKolI.bottom=RectEditSliderKolI.top+h/3+5;

	ScreenToClient(&RectRazv);
    pSt = (CStatic*)GetDlgItem(IDC_STATICRAZV);
	pSt->MoveWindow(&RectRazv);

	ScreenToClient(&RectTextStop);
    pSt = (CStatic*)GetDlgItem(IDC_STATICSTOP);
	pSt->MoveWindow(&RectTextStop);
	ScreenToClient(&RectTextStart);
    pSt = (CStatic*)GetDlgItem(IDC_STATICSTART);
	pSt->MoveWindow(&RectTextStart);

	ScreenToClient(&RectRazvRadImpPos);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_RAZVIMPPOSITIVE);
	pSt->MoveWindow(&RectRazvRadImpPos);
	
	ScreenToClient(&RectIconImpPos);
    pSt = (CStatic*)GetDlgItem(IDC_STATICIMP1POS);
	pSt->MoveWindow(&RectIconImpPos);

	ScreenToClient(&RectRazvRadDcPos);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_RAZVDCPOSITIVE);
	pSt->MoveWindow(&RectRazvRadDcPos);

	ScreenToClient(&RectIconDcPos);
    pSt = (CStatic*)GetDlgItem(IDC_STATICICONDCPOS);
	pSt->MoveWindow(&RectIconDcPos);

	ScreenToClient(&RectIconSynPos);
    pSt = (CStatic*)GetDlgItem(IDC_STATICIMPPOSITIVE);
	pSt->MoveWindow(&RectIconSynPos);

	ScreenToClient(&RectRazvRadAc);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_RAZVAC);
	pSt->MoveWindow(&RectRazvRadAc);

	ScreenToClient(&RectIconAc);
    pSt = (CStatic*)GetDlgItem(IDC_STATICAC);
	pSt->MoveWindow(&RectIconAc);

	ScreenToClient(&RectRazvRadDcNeg);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_RAZVDCNEGATIVE);
	pSt->MoveWindow(&RectRazvRadDcNeg);

	ScreenToClient(&RectIconDcNeg);
    pSt = (CStatic*)GetDlgItem(IDC_STATICICONDCNEGATIVE);
	pSt->MoveWindow(&RectIconDcNeg);

	ScreenToClient(&RectRazvRadImpNeg);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_RAZVIMPNEGATIVE);
	pSt->MoveWindow(&RectRazvRadImpNeg);

 	ScreenToClient(&RectIconSynNeg);
    pSt = (CStatic*)GetDlgItem(IDC_STATICIMPNEGATIVE);
	pSt->MoveWindow(&RectIconSynNeg);

	ScreenToClient(&RectIconImpNeg);
    pSt = (CStatic*)GetDlgItem(IDC_STATICIMP1NEG);
	pSt->MoveWindow(&RectIconImpNeg);

	ScreenToClient(&RectStatImp);
    pSt = (CStatic*)GetDlgItem(IDC_STATICIMP);
	pSt->MoveWindow(&RectStatImp);

	ScreenToClient(&RectDlitImp);
    pSt = (CStatic*)GetDlgItem(IDC_EDITDLITIMP);
	pSt->MoveWindow(&RectDlitImp);

	ScreenToClient(&RectRazvRadSynPos);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_SYNPOSITIVE);
	pSt->MoveWindow(&RectRazvRadSynPos);

	ScreenToClient(&RectRazvRadSynNeg);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_SYNNEGATIVE);
	pSt->MoveWindow(&RectRazvRadSynNeg);

	ScreenToClient(&RectPeriod);
    pSt = (CStatic*)GetDlgItem(IDC_CHECKPAUZA);
	pSt->MoveWindow(&RectPeriod);

	ScreenToClient(&RectU);
    pSt = (CStatic*)GetDlgItem(IDC_EDKOLMEASU);
	pSt->MoveWindow(&RectU);
	
	ScreenToClient(&RectI);
    pSt = (CStatic*)GetDlgItem(IDC_EDKOLMEASI);
	pSt->MoveWindow(&RectI);

	ScreenToClient(&RectKU);
    pSt = (CStatic*)GetDlgItem(IDC_COMBOKOLU);
	pSt->MoveWindow(&RectKU);
	
	ScreenToClient(&RectKI);
    pSt = (CStatic*)GetDlgItem(IDC_COMBOKOLI);
	pSt->MoveWindow(&RectKI);

	ScreenToClient(&RectKolSource);
    pSt = (CStatic*)GetDlgItem(IDC_STATICKOLLECTORSOURCE);
	pSt->MoveWindow(&RectKolSource);

	ScreenToClient(&RectKolSourU);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOKOLSOURU);
	pSt->MoveWindow(&RectKolSourU);

	ScreenToClient(&RectLimit);
    pSt = (CStatic*)GetDlgItem(IDC_STATICLIMIT);
	pSt->MoveWindow(&RectLimit);

	ScreenToClient(&RectSliderKolI);
    pSt = (CStatic*)GetDlgItem(IDC_SLIDERKOLI);
	pSt->MoveWindow(&RectSliderKolI);

	ScreenToClient(&RectTextRangMinKolI);
    pSt = (CStatic*)GetDlgItem(IDC_TEXTRANGMINKOLI);
	pSt->MoveWindow(&RectTextRangMinKolI);

	ScreenToClient(&RectTextRangMaxKolI);
    pEd = (CEdit*)GetDlgItem(IDC_EDTEXTRANGMAXKOLI);
	pEd->MoveWindow(&RectTextRangMaxKolI);

	ScreenToClient(&RectEditSliderKolI);
    pSt = (CStatic*)GetDlgItem(IDC_EDITLIMITKOLI);
	pSt->MoveWindow(&RectEditSliderKolI);

	ScreenToClient(&RectTextSliderKolU);
    pSt = (CStatic*)GetDlgItem(IDC_EDSLIDERKOLU);
	pSt->MoveWindow(&RectTextSliderKolU);

	ScreenToClient(&RectSliderKolU);
    pSt = (CStatic*)GetDlgItem(IDC_SLIDERKOLU);
	pSt->MoveWindow(&RectSliderKolU);

	ScreenToClient(&RectEditSliderKolU);
    pSt = (CStatic*)GetDlgItem(IDC_EDITLIMITKOLU);
	pSt->MoveWindow(&RectEditSliderKolU);

    //Закладка Page2 ("База")
	//-----------------------
	//Генератор ступенек
	GetDlgItem(IDC_STATICBAZA)->GetWindowRect(&RectBaza);
	RectBaza.left=RectPage12.left+t/2;//+2;
	RectBaza.top=RectPage12.top-20;//23;//6;
	RectBaza.right=RectPage12.right-t+4;
	RectBaza.bottom=RectBaza.top+h*5+h/2-7;//12;

	GetDlgItem(IDC_EDBAZASOURCEUI)->GetWindowRect(&RectEditBazaSourceUI);
	RectEditBazaSourceUI.left=RectBaza.left+t+2;//-2;//+t/2;
	RectEditBazaSourceUI.top=RectBaza.top+h/2+4;//-6;
	RectEditBazaSourceUI.right=RectEditBazaSourceUI.left+t*3;//w/6;
	RectEditBazaSourceUI.bottom=RectEditBazaSourceUI.top+h/3+6;

	GetDlgItem(IDC_RADIOBAZASOURCEU)->GetWindowRect(&RectBazaU);
	RectBazaU.left=RectEditBazaSourceUI.right+t+2;//4;//w/2++t/2;//+w/4;
	RectBazaU.top=RectEditBazaSourceUI.top-10;
	RectBazaU.right=RectBazaU.left+w/3;
	RectBazaU.bottom=RectBazaU.top+h/3+3;//-8;///7;

	GetDlgItem(IDC_RADIOBAZASOURCEI)->GetWindowRect(&RectBazaI);
	RectBazaI.left=RectBazaU.left;//RectBazaU.right+t;//*2;
	RectBazaI.top=RectBazaU.bottom+4;//-4;//RectBazaU.top;
	RectBazaI.right=RectBazaI.left+w/3;
	RectBazaI.bottom=RectBazaI.top+h/3+3;//-8;///7;//RectBazaU.bottom;

	GetDlgItem(IDC_CHECK_BAZA_VKL)->GetWindowRect(&RectBazaVkl);
	RectBazaVkl.left=RectBazaI.right+30;//+w/2+18;//;
	RectBazaVkl.top=RectEditBazaSourceUI.top;//-8;//12;//RectBazaU.top;
	RectBazaVkl.right=RectBazaVkl.left+w-22;//+8;//+8;//-t/2;
	RectBazaVkl.bottom=RectEditBazaSourceUI.bottom-12;//RectBazaU.bottom;

	//Амплитуда
	GetDlgItem(IDC_TEXTAMPLITUDEBAZA)->GetWindowRect(&RectTextAmplitude);
	RectTextAmplitude.left=RectBaza.left+t/2;
	RectTextAmplitude.top=RectEditBazaSourceUI.bottom+20;//8;//4;
	RectTextAmplitude.right=RectTextAmplitude.left+w+t*3-7;
	RectTextAmplitude.bottom=RectTextAmplitude.top+h/3;

	GetDlgItem(IDC_COMBOAMPLITUDEBAZA)->GetWindowRect(&RectComboAmplitude);
	RectComboAmplitude.left=RectTextAmplitude.right;
	RectComboAmplitude.top=RectTextAmplitude.top-5;//RectBazaI.bottom+5;//1;//2;
	RectComboAmplitude.right=RectBaza.right-t/2-3;
	RectComboAmplitude.bottom=RectComboAmplitude.top+h/3;

	//Высокое разрешение
	GetDlgItem(IDC_CHECKBAZAREJHIGH)->GetWindowRect(&RectBazaRejHigh);
	RectBazaRejHigh.left=RectBaza.left+t*10;
	RectBazaRejHigh.top=RectTextAmplitude.bottom+h/3-16;//RectTextLimitBaza.bottom+3;
	RectBazaRejHigh.right=RectBaza.right-t/2-3;
	RectBazaRejHigh.bottom=RectBazaRejHigh.top+h/2+3;//3+2;

	GetDlgItem(IDC_TEXTSTEPBAZA)->GetWindowRect(&RectTextStep);
	RectTextStep.left=RectBaza.left+t/2;
	RectTextStep.top=RectTextAmplitude.bottom+h/3+h/2-9;//5;//1;
	RectTextStep.right=RectTextAmplitude.left+w;
	RectTextStep.bottom=RectTextStep.top+h/3;

	GetDlgItem(IDC_EDITSTEPBAZA)->GetWindowRect(&RectEditStepBaza);
	RectEditStepBaza.left=RectTextStep.right+t*3-8;
	RectEditStepBaza.top=RectTextStep.top-2;
	RectEditStepBaza.right=RectEditStepBaza.left+t*4+8;
	RectEditStepBaza.bottom=RectEditStepBaza.top+h/3+5;

	GetDlgItem(IDC_SPINSTEPBAZA)->GetWindowRect(&RectSpinStepBaza);
	RectSpinStepBaza.left=RectEditStepBaza.right;//+3;
	RectSpinStepBaza.top=RectEditStepBaza.top-1;
	RectSpinStepBaza.right=RectComboAmplitude.right;//RectSpinStepBaza.left+t*2;
	RectSpinStepBaza.bottom=RectEditStepBaza.bottom+2;

	GetDlgItem(IDC_TEXTPOLARITYSTEP)->GetWindowRect(&RectTextPolarityStep);
	RectTextPolarityStep.left=RectBaza.left+t/2+2;
	RectTextPolarityStep.top=RectSpinStepBaza.bottom+h/8+2;//-1;
	RectTextPolarityStep.right=RectTextPolarityStep.left+w-t;//RectTextAmplitude.right;
	RectTextPolarityStep.bottom=RectTextPolarityStep.top+h/3;

	GetDlgItem(IDC_COMBOPOLARITYSTEP)->GetWindowRect(&RectComboPolarityStep);
	RectComboPolarityStep.left=RectTextPolarityStep.right+10;
	RectComboPolarityStep.top=RectTextPolarityStep.top-2;
	RectComboPolarityStep.right=RectBaza.right-t/2-3;
	RectComboPolarityStep.bottom=RectComboPolarityStep.top+h/3;

	//Смещение
	GetDlgItem(IDC_STATICBAZABIAS)->GetWindowRect(&RectTextBazaBias);
	RectTextBazaBias.left=RectBaza.left+t/2;
	RectTextBazaBias.top=RectComboPolarityStep.bottom+15;//6;
	RectTextBazaBias.right=RectBaza.right-t/2;
	RectTextBazaBias.bottom=RectTextBazaBias.top+h*2-4;//*3-h/2-20;//12;

	GetDlgItem(IDC_SLIDERBAZABIAS)->GetWindowRect(&RectSliderBazaBias);
	RectSliderBazaBias.left=RectTextBazaBias.left+2;//+t/2;
	RectSliderBazaBias.top=RectTextBazaBias.top+h/2;//RectRangBazaBias.bottom+7;//10;//;
	RectSliderBazaBias.right=RectSliderBazaBias.left+w+15;//-t*2;
	RectSliderBazaBias.bottom=RectSliderBazaBias.top+h/2;

	GetDlgItem(IDC_TEXTMINBAZABIAS)->GetWindowRect(&RectMinBazaBias);
	RectMinBazaBias.left=RectSliderBazaBias.left+t;
	RectMinBazaBias.top=RectSliderBazaBias.bottom+1;
	RectMinBazaBias.right=RectMinBazaBias.left+t*3;//RectSliderBazaBias.right+t*3;//RectMinBazaBias.left+t*3;
	RectMinBazaBias.bottom=RectMinBazaBias.top+h/3+1;

	GetDlgItem(IDC_EDITRANGBAZABIAS)->GetWindowRect(&RectRangBazaBias);
	RectRangBazaBias.left=RectSliderBazaBias.right-30;//RectComboPolarityStep.left-20;
	RectRangBazaBias.top=RectMinBazaBias.top;//RectTextRangBazaBias.top;
	RectRangBazaBias.right=RectRangBazaBias.left+t*6;
	RectRangBazaBias.bottom=RectRangBazaBias.top+h/3;



	GetDlgItem(IDC_EDITBAZABIAS)->GetWindowRect(&RectEditBazaBias);
	RectEditBazaBias.left=RectSliderBazaBias.right+t;///2;//RectRangBazaBias.left;
	RectEditBazaBias.top=RectSliderBazaBias.top;//RectRangBazaBias.bottom+h/4-7;
	RectEditBazaBias.right=RectBaza.right-t/2-4;//RectRangBazaBias.right;
	RectEditBazaBias.bottom=RectEditBazaBias.top+h/3+5;

	GetDlgItem(IDC_TEXTPOLARITYBIAS)->GetWindowRect(&RectTextPolarityBias);
	RectTextPolarityBias.left=RectTextBazaBias.left+t;//RectTextRangBazaBias.left;
	RectTextPolarityBias.top=RectMinBazaBias.bottom+4;//-2;//+2;
	RectTextPolarityBias.right=RectTextPolarityBias.left+w-t;//RectTextAmplitude.right;
	RectTextPolarityBias.bottom=RectTextPolarityBias.top+h/3;

	GetDlgItem(IDC_COMBOPOLARITYBAZABIAS)->GetWindowRect(&RectComboPolarityBazaBias);
	RectComboPolarityBazaBias.left=RectComboPolarityStep.left;
	RectComboPolarityBazaBias.top=RectTextPolarityBias.top-3;
	RectComboPolarityBazaBias.right=RectEditBazaBias.right;
	RectComboPolarityBazaBias.bottom=RectComboPolarityBazaBias.top+h/3;

	//Ограничение
	GetDlgItem(IDC_STATICLIMITBAZA)->GetWindowRect(&RectTextLimitBaza);
	RectTextLimitBaza.left=RectBaza.left;
	RectTextLimitBaza.top=RectBaza.bottom+3;
	RectTextLimitBaza.right=RectBaza.right;
	RectTextLimitBaza.bottom=RectTextLimitBaza.top+h*2+h/3-30;//9;//7;

	GetDlgItem(IDC_EDMEASBAZA)->GetWindowRect(&RectEditMeasBaza);
	RectEditMeasBaza.left=RectTextLimitBaza.left+t+2;//+t/2;
	RectEditMeasBaza.top=RectTextLimitBaza.top+h/2-3;//5;
	RectEditMeasBaza.right=RectEditMeasBaza.left+t*3;//2;
	RectEditMeasBaza.bottom=RectEditMeasBaza.top+h/3+8;

	GetDlgItem(IDC_TEXTRANGLIMITBAZA)->GetWindowRect(&RectTextRangLimitBaza);
	RectTextRangLimitBaza.left=RectEditMeasBaza.right-t*3;//+t*3;//RectTextLimitBaza.left+t+3;
	RectTextRangLimitBaza.top=RectEditMeasBaza.top;//+3;//RectEditMeasBaza.bottom-5;//2;//+2;
	RectTextRangLimitBaza.right=RectTextRangLimitBaza.left+w;//RectTextAmplitude.right;
	RectTextRangLimitBaza.bottom=RectTextRangLimitBaza.top+h/3-2;

	GetDlgItem(IDC_COMBOBAZALIMITMAXUI)->GetWindowRect(&RectComboBazaSourceUI);
	RectComboBazaSourceUI.left=RectEditBazaBias.left;//RectComboPolarityBazaBias.left+30;//RectTextRangLimitBaza.right-7;
	RectComboBazaSourceUI.top=RectTextRangLimitBaza.top-4;//-2;
	RectComboBazaSourceUI.right=RectTextLimitBaza.right-t/2-3;
	RectComboBazaSourceUI.bottom=RectComboBazaSourceUI.top+h/3;

	GetDlgItem(IDC_SLIDERLIMITBAZA)->GetWindowRect(&RectSliderLimitBaza);
	RectSliderLimitBaza.left=RectBaza.left+2;//t/2;
	RectSliderLimitBaza.top=RectComboBazaSourceUI.bottom+14;//RectTextRangLimitBaza.bottom+5;//3;//RectComboBazaSourceUI.bottom+7;
	RectSliderLimitBaza.right=RectSliderBazaBias.right;//+8;//RectSliderLimitBaza.left+w-t*2;
	RectSliderLimitBaza.bottom=RectSliderLimitBaza.top+h/2;

	GetDlgItem(IDC_EDITLIMITBAZA)->GetWindowRect(&RectEditLimitBaza);
	RectEditLimitBaza.left=RectSliderLimitBaza.right+t;///2;//RectComboBazaSourceUI.left;
	RectEditLimitBaza.top=RectComboBazaSourceUI.bottom+h/4+1;
	RectEditLimitBaza.right=RectComboBazaSourceUI.right;
	RectEditLimitBaza.bottom=RectEditLimitBaza.top+h/3+5;

	GetDlgItem(IDC_TEXTMINLIMITBAZA)->GetWindowRect(&RectTextMinLimitBaza);
	RectTextMinLimitBaza.left=RectSliderLimitBaza.left+t/2;//-3;
	RectTextMinLimitBaza.top=RectSliderLimitBaza.bottom+1;
	RectTextMinLimitBaza.right=RectSliderLimitBaza.right+t*3;//RectTextMinLimitBaza.left+t*3;
	RectTextMinLimitBaza.bottom=RectTextMinLimitBaza.top+h/3+1;

	ScreenToClient(&RectBaza);
	pSt = (CStatic*)GetDlgItem(IDC_STATICBAZA);
	pSt->MoveWindow(&RectBaza);

	ScreenToClient(&RectBazaU);
	pSt = (CStatic*)GetDlgItem(IDC_RADIOBAZASOURCEU);
	pSt->MoveWindow(&RectBazaU);

	ScreenToClient(&RectBazaI);
	pSt = (CStatic*)GetDlgItem(IDC_RADIOBAZASOURCEI);
	pSt->MoveWindow(&RectBazaI);

	ScreenToClient(&RectBazaVkl);
	pSt = (CStatic*)GetDlgItem(IDC_CHECK_BAZA_VKL);
	pSt->MoveWindow(&RectBazaVkl);

	ScreenToClient(&RectEditBazaSourceUI);
	pSt = (CStatic*)GetDlgItem(IDC_EDBAZASOURCEUI);
	pSt->MoveWindow(&RectEditBazaSourceUI);

	ScreenToClient(&RectTextAmplitude);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTAMPLITUDEBAZA);
	pSt->MoveWindow(&RectTextAmplitude);

	ScreenToClient(&RectComboAmplitude);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOAMPLITUDEBAZA);
	pSt->MoveWindow(&RectComboAmplitude);

	ScreenToClient(&RectTextStep);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTSTEPBAZA);
	pSt->MoveWindow(&RectTextStep);

	ScreenToClient(&RectEditStepBaza);
	pSt = (CStatic*)GetDlgItem(IDC_EDITSTEPBAZA);
	pSt->MoveWindow(&RectEditStepBaza);

	ScreenToClient(&RectSpinStepBaza);
	pSt = (CStatic*)GetDlgItem(IDC_SPINSTEPBAZA);
	pSt->MoveWindow(&RectSpinStepBaza);

	ScreenToClient(&RectBazaRejHigh);
	pSt = (CStatic*)GetDlgItem(IDC_CHECKBAZAREJHIGH);
	pSt->MoveWindow(&RectBazaRejHigh);

	ScreenToClient(&RectTextPolarityStep);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTPOLARITYSTEP);
	pSt->MoveWindow(&RectTextPolarityStep);

	ScreenToClient(&RectComboPolarityStep);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOPOLARITYSTEP);
	pSt->MoveWindow(&RectComboPolarityStep);

	ScreenToClient(&RectTextBazaBias);
	pSt = (CStatic*)GetDlgItem(IDC_STATICBAZABIAS);
	pSt->MoveWindow(&RectTextBazaBias);

	ScreenToClient(&RectRangBazaBias);
	pSt = (CStatic*)GetDlgItem(IDC_EDITRANGBAZABIAS);
	pSt->MoveWindow(&RectRangBazaBias);

	ScreenToClient(&RectSliderBazaBias);
	pSt = (CStatic*)GetDlgItem(IDC_SLIDERBAZABIAS);
	pSt->MoveWindow(&RectSliderBazaBias);

	ScreenToClient(&RectMinBazaBias);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTMINBAZABIAS);
	pSt->MoveWindow(&RectMinBazaBias);

	ScreenToClient(&RectEditBazaBias);
	pSt = (CStatic*)GetDlgItem(IDC_EDITBAZABIAS);
	pSt->MoveWindow(&RectEditBazaBias);

	ScreenToClient(&RectTextPolarityBias);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTPOLARITYBIAS);
	pSt->MoveWindow(&RectTextPolarityBias);

	ScreenToClient(&RectComboPolarityBazaBias);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOPOLARITYBAZABIAS);
	pSt->MoveWindow(&RectComboPolarityBazaBias);

	ScreenToClient(&RectTextLimitBaza);
	pSt = (CStatic*)GetDlgItem(IDC_STATICLIMITBAZA);
	pSt->MoveWindow(&RectTextLimitBaza);

	ScreenToClient(&RectEditMeasBaza);
	pSt = (CStatic*)GetDlgItem(IDC_EDMEASBAZA);
	pSt->MoveWindow(&RectEditMeasBaza);

	ScreenToClient(&RectTextRangLimitBaza);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTRANGLIMITBAZA);
	pSt->MoveWindow(&RectTextRangLimitBaza);

	ScreenToClient(&RectComboBazaSourceUI);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOBAZALIMITMAXUI);
	pSt->MoveWindow(&RectComboBazaSourceUI);

	ScreenToClient(&RectSliderLimitBaza);
	pSt = (CStatic*)GetDlgItem(IDC_SLIDERLIMITBAZA);
	pSt->MoveWindow(&RectSliderLimitBaza);

	ScreenToClient(&RectEditLimitBaza);
	pSt = (CStatic*)GetDlgItem(IDC_EDITLIMITBAZA);
	pSt->MoveWindow(&RectEditLimitBaza);

	ScreenToClient(&RectTextMinLimitBaza);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTMINLIMITBAZA);
	pSt->MoveWindow(&RectTextMinLimitBaza);

   //Закладка Page3 ("Дополнительный канал")
	//---------------------------------------
	//Генератор ступенек
	GetDlgItem(IDC_STATICDOP)->GetWindowRect(&RectDop);
	RectDop.left=RectPage34.left+t/2;//+2;
	RectDop.top=RectPage12.top-20;//6;//RectPage34.top+h/4-4;
	RectDop.right=RectPage34.right-t+6-2;
	RectDop.bottom=RectDop.top+h*5+8;//10;//6;//9;//11;//6;

	GetDlgItem(IDC_EDDOPSOURCEUI)->GetWindowRect(&RectEditDopSourceUI);
	RectEditDopSourceUI.left=RectDop.left+t+2;//-2;//+t/2;
	RectEditDopSourceUI.top=RectDop.top+h/2+10;//-5;
	RectEditDopSourceUI.right=RectEditDopSourceUI.left+t*3;//w/6;
	RectEditDopSourceUI.bottom=RectEditDopSourceUI.top+h/3+6;

	GetDlgItem(IDC_RADIODOPU)->GetWindowRect(&RectDopU);
	RectDopU.left=RectEditDopSourceUI.right+t+4;//w/2+t+t/2;
	RectDopU.top=RectEditDopSourceUI.top-10;
	RectDopU.right=RectDopU.left+w/3;
	RectDopU.bottom=RectDopU.top+h/3+3;//(h*3)/7;

	GetDlgItem(IDC_RADIODOPI)->GetWindowRect(&RectDopI);
	RectDopI.left=RectDopU.left;//RectDopU.right+t;
	RectDopI.top=RectDopU.bottom+4;//RectDopU.top;
	RectDopI.right=RectDopI.left+w/3;
	RectDopI.bottom=RectDopI.top+h/3+3;//(h*3)/7;


	GetDlgItem(IDC_TEXTAMPLITUDEDOP)->GetWindowRect(&RectTextAmplitudeDop);
	RectTextAmplitudeDop.left=RectDop.left+t/2;
	RectTextAmplitudeDop.top=RectEditDopSourceUI.bottom+20;
	RectTextAmplitudeDop.right=RectTextAmplitudeDop.left+w+t*3-7;
	RectTextAmplitudeDop.bottom=RectTextAmplitudeDop.top+h/3;

	GetDlgItem(IDC_COMBOAMPLITUDEDOP)->GetWindowRect(&RectComboAmplitudeDop);
	RectComboAmplitudeDop.left=RectTextAmplitudeDop.right;
	RectComboAmplitudeDop.top=RectTextAmplitudeDop.top-5;
	RectComboAmplitudeDop.right=RectDop.right-t/2-3;
	RectComboAmplitudeDop.bottom=RectComboAmplitudeDop.top+h/3;

	GetDlgItem(IDC_TEXTSTEPDOP)->GetWindowRect(&RectTextStepDop);
	RectTextStepDop.left=RectDop.left+t/2;
	RectTextStepDop.top=RectTextAmplitudeDop.bottom+h/3-10;
	RectTextStepDop.right=RectTextAmplitudeDop.left+w;
	RectTextStepDop.bottom=RectTextStepDop.top+h/3;

	GetDlgItem(IDC_EDITSTEPDOP)->GetWindowRect(&RectEditStepDop);
	RectEditStepDop.left=RectTextStepDop.right+t*3-8;
	RectEditStepDop.top=RectTextStepDop.top-2;
	RectEditStepDop.right=RectEditStepDop.left+t*4+8;
	RectEditStepDop.bottom=RectEditStepDop.top+h/3+5;

	GetDlgItem(IDC_SPINSTEPDOP)->GetWindowRect(&RectSpinStepDop);
	RectSpinStepDop.left=RectEditStepDop.right;//+3;
	RectSpinStepDop.top=RectEditStepDop.top-1;
	RectSpinStepDop.right=RectComboAmplitudeDop.right;//RectSpinStepDop.left+t*2+3;
	RectSpinStepDop.bottom=RectEditStepDop.bottom+2;

	GetDlgItem(IDC_TEXTPOLARITYSTEPDOP)->GetWindowRect(&RectTextPolarityStepDop);
	RectTextPolarityStepDop.left=RectDop.left+t/2+2;
	RectTextPolarityStepDop.top=RectSpinStepDop.bottom+h/7+2;//8-1;
	RectTextPolarityStepDop.right=RectTextPolarityStepDop.left+w;//RectTextAmplitudeDop.right;
	RectTextPolarityStepDop.bottom=RectTextPolarityStepDop.top+h/3;

	GetDlgItem(IDC_COMBOPOLARITYSTEPDOP)->GetWindowRect(&RectComboPolarityStepDop);
	RectComboPolarityStepDop.left=RectTextPolarityStepDop.right;//RectComboAmplitudeDop.left;//RectTextPolarityStepDop.right;
	RectComboPolarityStepDop.top=RectTextPolarityStepDop.top-2;
	RectComboPolarityStepDop.right=RectDop.right-t/2-3;
	RectComboPolarityStepDop.bottom=RectComboPolarityStepDop.top+h/3;

	//Смещение
	GetDlgItem(IDC_STATICDOPBIAS)->GetWindowRect(&RectTextDopBias);
	RectTextDopBias.left=RectDop.left+t/2;
	RectTextDopBias.top=RectComboPolarityStepDop.bottom+16;
	RectTextDopBias.right=RectDop.right-t/2;
	RectTextDopBias.bottom=RectTextDopBias.top+h*2+2;//-h/2-12;//11;//7;

	GetDlgItem(IDC_SLIDERDOPBIAS)->GetWindowRect(&RectSliderDopBias);
	RectSliderDopBias.left=RectTextDopBias.left+2;//t/2;
	RectSliderDopBias.top=RectTextDopBias.top+h/2+2;
	RectSliderDopBias.right=RectSliderDopBias.left+w+15;//-t*2;
	RectSliderDopBias.bottom=RectSliderDopBias.top+h/2;

	GetDlgItem(IDC_TEXTMINDOPBIAS)->GetWindowRect(&RectMinDopBias);
	RectMinDopBias.left=RectSliderDopBias.left+t;
	RectMinDopBias.top=RectSliderDopBias.bottom+3;
	RectMinDopBias.right=RectMinDopBias.left+t*3;//RectSliderDopBias.right+t*3;//RectMinDopBias.left+t*3;
	RectMinDopBias.bottom=RectMinDopBias.top+h/3+2;

	GetDlgItem(IDC_EDITRANGDOPBIAS)->GetWindowRect(&RectRangDopBias);
	RectRangDopBias.left=RectSliderDopBias.right-30;//25;//RectComboPolarityStepDop.left-20;
	RectRangDopBias.top=RectMinDopBias.top;////RectTextRangDopBias.top;//-3;
	RectRangDopBias.right=RectRangDopBias.left+t*6;//RectTextDopBias.right-t/2-3;
	RectRangDopBias.bottom=RectRangDopBias.top+h/3;

	GetDlgItem(IDC_EDITDOPBIAS)->GetWindowRect(&RectEditDopBias);
	RectEditDopBias.left=RectSliderDopBias.right+t;//RectRangDopBias.left;
	RectEditDopBias.top=RectSliderDopBias.top;//RectRangDopBias.bottom+h/4-7;
	RectEditDopBias.right=RectDop.right-t/2-4;//RectRangDopBias.right;
	RectEditDopBias.bottom=RectEditDopBias.top+h/3+5;

	GetDlgItem(IDC_TEXTPOLARITYBIASDOP)->GetWindowRect(&RectTextPolarityBiasDop);
	RectTextPolarityBiasDop.left=RectTextDopBias.left+t;//RectTextRangDopBias.left;
	RectTextPolarityBiasDop.top=RectMinDopBias.bottom+4;//h/10;
	RectTextPolarityBiasDop.right=RectTextPolarityStepDop.right;//RectTextAmplitudeDop.right;
	RectTextPolarityBiasDop.bottom=RectTextPolarityBiasDop.top+h/3;

	GetDlgItem(IDC_COMBOPOLARITYDOPBIAS)->GetWindowRect(&RectComboPolarityDopBias);
	RectComboPolarityDopBias.left=RectComboPolarityStepDop.left;
	RectComboPolarityDopBias.top=RectTextPolarityBiasDop.top-3;
	RectComboPolarityDopBias.right=RectComboPolarityStepDop.right-2;//RectRangDopBias.right;
	RectComboPolarityDopBias.bottom=RectComboPolarityDopBias.top+h/3;
	
	//Ограничение
	GetDlgItem(IDC_STATICLIMITDOP)->GetWindowRect(&RectTextLimitDop);
	RectTextLimitDop.left=RectDop.left;
	RectTextLimitDop.top=RectDop.bottom+5;
	RectTextLimitDop.right=RectDop.right;
	RectTextLimitDop.bottom=RectTextLimitDop.top+h*2+7;//+h/3;//2-18;//2;//-3;

	GetDlgItem(IDC_EDMEASDOP)->GetWindowRect(&RectEditMeasDop);
	RectEditMeasDop.left=RectTextLimitDop.left+t+2;//-2;//+t/2;
	RectEditMeasDop.top=RectTextLimitDop.top+h/2-3;//-5;
	RectEditMeasDop.right=RectEditMeasDop.left+t*3;
	RectEditMeasDop.bottom=RectEditMeasDop.top+h/3+8;

	GetDlgItem(IDC_TEXTRANGLIMITDOP)->GetWindowRect(&RectTextRangLimitDop);
	RectTextRangLimitDop.left=RectEditMeasDop.right-t*3;//RectDop.left+t+t/2;
	RectTextRangLimitDop.top=RectEditMeasDop.top;//+3;//+2;
	RectTextRangLimitDop.right=RectTextRangLimitDop.left+w;//RectTextAmplitudeDop.right;
	RectTextRangLimitDop.bottom=RectTextRangLimitDop.top+h/3-2;

	GetDlgItem(IDC_COMBODOPLIMITMAXUI)->GetWindowRect(&RectComboDopSourceUI);
	RectComboDopSourceUI.left=RectTextRangLimitDop.right+8;
	RectComboDopSourceUI.top=RectTextRangLimitDop.top-4;//-1;
	RectComboDopSourceUI.right=RectTextLimitBaza.right-t/2-3;
	RectComboDopSourceUI.bottom=RectComboDopSourceUI.top+h/3;

	GetDlgItem(IDC_SLIDERLIMITDOP)->GetWindowRect(&RectSliderLimitDop);
	RectSliderLimitDop.left=RectTextLimitDop.left+2;//t/2;
	RectSliderLimitDop.top=RectTextRangLimitDop.bottom+20;//19;//23;//RectComboDopSourceUI.bottom+8;
	RectSliderLimitDop.right=RectSliderDopBias.right;//+6;
	RectSliderLimitDop.bottom=RectSliderLimitDop.top+h/2;

	GetDlgItem(IDC_EDITLIMITDOP)->GetWindowRect(&RectEditLimitDop);
	RectEditLimitDop.left=RectComboDopSourceUI.left;//RectSliderLimitDop.right+t;//RectEditDopBias.left+3;
	RectEditLimitDop.top=RectComboDopSourceUI.bottom+h/4+6;//+2;
	RectEditLimitDop.right=RectTextLimitBaza.right-t/2-3;
	RectEditLimitDop.bottom=RectEditLimitDop.top+h/3+5;

	GetDlgItem(IDC_TEXTMINLIMITDOP)->GetWindowRect(&RectTextMinLimitDop);
	RectTextMinLimitDop.left=RectSliderLimitDop.left+t/2-3;
	RectTextMinLimitDop.top=RectSliderLimitDop.bottom+3;
	RectTextMinLimitDop.right=RectSliderLimitDop.right+t*3;//RectTextMinLimitDop.left+t*3;
	RectTextMinLimitDop.bottom=RectTextMinLimitDop.top+h/3+2;

	ScreenToClient(&RectDop);
	pSt = (CStatic*)GetDlgItem(IDC_STATICDOP);
	pSt->MoveWindow(&RectDop);

	ScreenToClient(&RectDopU);
	pSt = (CStatic*)GetDlgItem(IDC_RADIODOPU);
	pSt->MoveWindow(&RectDopU);

	ScreenToClient(&RectDopI);
	pSt = (CStatic*)GetDlgItem(IDC_RADIODOPI);
	pSt->MoveWindow(&RectDopI);


	ScreenToClient(&RectEditDopSourceUI);
	pSt = (CStatic*)GetDlgItem(IDC_EDDOPSOURCEUI);
	pSt->MoveWindow(&RectEditDopSourceUI);

	ScreenToClient(&RectTextAmplitudeDop);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTAMPLITUDEDOP);
	pSt->MoveWindow(&RectTextAmplitudeDop);

	ScreenToClient(&RectComboAmplitudeDop);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOAMPLITUDEDOP);
	pSt->MoveWindow(&RectComboAmplitudeDop);

	ScreenToClient(&RectTextStepDop);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTSTEPDOP);
	pSt->MoveWindow(&RectTextStepDop);

	ScreenToClient(&RectEditStepDop);
	pSt = (CStatic*)GetDlgItem(IDC_EDITSTEPDOP);
	pSt->MoveWindow(&RectEditStepDop);

	ScreenToClient(&RectSpinStepDop);
	pSt = (CStatic*)GetDlgItem(IDC_SPINSTEPDOP);
	pSt->MoveWindow(&RectSpinStepDop);

	ScreenToClient(&RectTextPolarityStepDop);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTPOLARITYSTEPDOP);
	pSt->MoveWindow(&RectTextPolarityStepDop);

	ScreenToClient(&RectComboPolarityStepDop);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOPOLARITYSTEPDOP);
	pSt->MoveWindow(&RectComboPolarityStepDop);

	ScreenToClient(&RectTextDopBias);
	pSt = (CStatic*)GetDlgItem(IDC_STATICDOPBIAS);
	pSt->MoveWindow(&RectTextDopBias);
	ScreenToClient(&RectRangDopBias);
	pSt = (CStatic*)GetDlgItem(IDC_EDITRANGDOPBIAS);
	pSt->MoveWindow(&RectRangDopBias);

	ScreenToClient(&RectSliderDopBias);
	pSt = (CStatic*)GetDlgItem(IDC_SLIDERDOPBIAS);
	pSt->MoveWindow(&RectSliderDopBias);

	ScreenToClient(&RectMinDopBias);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTMINDOPBIAS);
	pSt->MoveWindow(&RectMinDopBias);

	ScreenToClient(&RectEditDopBias);
	pSt = (CStatic*)GetDlgItem(IDC_EDITDOPBIAS);
	pSt->MoveWindow(&RectEditDopBias);

	ScreenToClient(&RectTextPolarityBiasDop);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTPOLARITYBIASDOP);
	pSt->MoveWindow(&RectTextPolarityBiasDop);

	ScreenToClient(&RectComboPolarityDopBias);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOPOLARITYDOPBIAS);
	pSt->MoveWindow(&RectComboPolarityDopBias);

	ScreenToClient(&RectTextLimitDop);
	pSt = (CStatic*)GetDlgItem(IDC_STATICLIMITDOP);
	pSt->MoveWindow(&RectTextLimitDop);

	ScreenToClient(&RectEditMeasDop);
	pSt = (CStatic*)GetDlgItem(IDC_EDMEASDOP);
	pSt->MoveWindow(&RectEditMeasDop);

	ScreenToClient(&RectTextRangLimitDop);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTRANGLIMITDOP);
	pSt->MoveWindow(&RectTextRangLimitDop);

	ScreenToClient(&RectComboDopSourceUI);
	pSt = (CStatic*)GetDlgItem(IDC_COMBODOPLIMITMAXUI);
	pSt->MoveWindow(&RectComboDopSourceUI);

	ScreenToClient(&RectSliderLimitDop);
	pSt = (CStatic*)GetDlgItem(IDC_SLIDERLIMITDOP);
	pSt->MoveWindow(&RectSliderLimitDop);

	ScreenToClient(&RectEditLimitDop);
	pSt = (CStatic*)GetDlgItem(IDC_EDITLIMITDOP);
	pSt->MoveWindow(&RectEditLimitDop);

	ScreenToClient(&RectTextMinLimitDop);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTMINLIMITDOP);
	pSt->MoveWindow(&RectTextMinLimitDop);

    //Закладка Page4 ("Расчёт")
	//-----------------------
	//График
	GetDlgItem(IDC_ZONACHARACTERISTIC)->GetWindowRect(&RectZonaCharacteristic);
	RectZonaCharacteristic.left=RectPage34.left+t/2;//+2;
	RectZonaCharacteristic.top=RectPage12.top+5;//-20;//6;//RectPage34.top+h/4;//-3;
	RectZonaCharacteristic.right=RectPage34.right-t+2;
	RectZonaCharacteristic.bottom=RectZonaCharacteristic.top+h*2-24;//14;//+5;//-2;

	GetDlgItem(IDC_FORMULA)->GetWindowRect(&RectFormula);
	RectFormula.left=RectZonaCharacteristic.left+t;//*3;RectAxisY.right+t/2;
	RectFormula.top=RectZonaCharacteristic.top+h/2;//-4;//RectAxisY.top;
	RectFormula.right=RectFormula.left+t*6;
	RectFormula.bottom=RectFormula.top+h/3+5;//RectAxisY.bottom;

	GetDlgItem(IDC_AXISX)->GetWindowRect(&RectAxisX);
	RectAxisX.left=RectFormula.right+t/2;
	RectAxisX.top=RectFormula.top-3;//RectAxisY.top;
	RectAxisX.right=RectAxisX.left+t*8;
	RectAxisX.bottom=RectFormula.bottom;//RectAxisY.bottom;

	GetDlgItem(IDC_ENDFORMULA)->GetWindowRect(&RectEndFormula);
	RectEndFormula.left=RectFormula.left;//RectAxisX.right;//+t/2;
	RectEndFormula.top=RectFormula.bottom+8;//RectAxisY.top;
	RectEndFormula.right=RectFormula.right;//RectEndFormula.left+t;
	RectEndFormula.bottom=RectEndFormula.top+h/3+5;//RectAxisY.bottom;

	GetDlgItem(IDC_AXISY)->GetWindowRect(&RectAxisY);
	RectAxisY.left=RectAxisX.left;//RectZonaCharacteristic.left+t;//*3;
	RectAxisY.top=RectEndFormula.top-3;//RectZonaCharacteristic.top+h/2-4;
	RectAxisY.right=RectAxisX.right;//RectAxisY.left+t*5;
	RectAxisY.bottom=RectEndFormula.bottom;//RectAxisY.top+h/3+5;

	//Параметр
	GetDlgItem(IDC_ZONAPARAM)->GetWindowRect(&RectZonaPar);
	RectZonaPar.left=RectZonaCharacteristic.left;
	RectZonaPar.top=RectZonaCharacteristic.bottom+4;//RectZonaRejAxes.bottom;
	RectZonaPar.right=RectZonaCharacteristic.right;
	RectZonaPar.bottom=RectZonaPar.top+h*4;//4+1;//6-9;//+3;//RectRejY.top+h*6-h/4+1;

	GetDlgItem(IDC_TEXTFIND)->GetWindowRect(&RectTextFind);
	RectTextFind.left=RectZonaPar.left+t-2;
	RectTextFind.top=RectZonaPar.top+h/2;//-3;
	RectTextFind.right=RectTextFind.left+t*4+2;
	RectTextFind.bottom=RectTextFind.top+h/3;

	GetDlgItem(IDC_EDITNAMEPAR)->GetWindowRect(&RectEditPar);
	RectEditPar.left=RectTextFind.right+t/2;
	RectEditPar.top=RectTextFind.top-2;
	RectEditPar.right=RectEditPar.left+t*5;
	RectEditPar.bottom=RectEditPar.top+h/3+5;

	GetDlgItem(IDC_TEXTFOR)->GetWindowRect(&RectTextFor);
	RectTextFor.left=RectEditPar.right+t/2+8;
	RectTextFor.top=RectTextFind.top;
	RectTextFor.right=RectTextFor.left+t*3;
	RectTextFor.bottom=RectTextFind.bottom;

	GetDlgItem(IDC_EDITNAMEARG)->GetWindowRect(&RectEditArg);
	RectEditArg.left=RectTextFor.right;
	RectEditArg.top=RectEditPar.top;
	RectEditArg.right=RectEditArg.left+t*5;
	RectEditArg.bottom=RectEditPar.bottom;

	GetDlgItem(IDC_TEXT)->GetWindowRect(&RectText);
	RectText.left=RectTextFind.left;
	RectText.top=RectEditPar.bottom+5;
	RectText.right=RectText.left+t*6;
	RectText.bottom=RectText.top+h/3;

	GetDlgItem(IDC_TEXTUR1)->GetWindowRect(&RectTextUr1);
	RectTextUr1.left=RectTextFind.left;
	RectTextUr1.top=RectText.bottom+5;
	RectTextUr1.right=RectTextUr1.left+w-w/4;
	RectTextUr1.bottom=RectTextUr1.top+h/3;

	GetDlgItem(IDC_TEXTUR2)->GetWindowRect(&RectTextUr2);
	RectTextUr2.left=RectTextUr1.left;
	RectTextUr2.top=RectTextUr1.bottom+7;
	RectTextUr2.right=RectTextUr1.right;
	RectTextUr2.bottom=RectTextUr2.top+h/3;

	GetDlgItem(IDC_EDITUR1)->GetWindowRect(&RectEditUr1);
	RectEditUr1.left=RectTextUr1.right;
	RectEditUr1.top=RectTextUr1.top-4;
	RectEditUr1.right=RectZonaPar.right-t/2;
	RectEditUr1.bottom=RectTextUr1.bottom+3;

	GetDlgItem(IDC_EDITUR2)->GetWindowRect(&RectEditUr2);
	RectEditUr2.left=RectEditUr1.left;
	RectEditUr2.top=RectTextUr2.top-1;
	RectEditUr2.right=RectEditUr1.right;
	RectEditUr2.bottom=RectTextUr2.bottom+5;
	
	//№ ступеньки

	GetDlgItem(IDC_EDITPOINT)->GetWindowRect(&RectEditPoint);
	RectEditPoint.left=RectTextUr1.left;//RectEditUr1.right-t*4;
	RectEditPoint.top=RectTextUr2.bottom+12;//RectTextPoint.top-2;
	RectEditPoint.right=RectEditUr1.right;
	RectEditPoint.bottom=RectEditPoint.top+h/3+5;

	//Формула
	GetDlgItem(IDC_ZONAFORMULA)->GetWindowRect(&RectZonaFormula);
	RectZonaFormula.left=RectZonaPar.left+t/2;
	RectZonaFormula.top=RectEditPoint.bottom+5;//-3;
	RectZonaFormula.right=RectZonaPar.right-t/2;
	RectZonaFormula.bottom=RectZonaFormula.top+h+8;

	GetDlgItem(IDC_RADIO1)->GetWindowRect(&RectRadio1);
	RectRadio1.left=RectZonaFormula.left+t*3;
	RectRadio1.top=RectZonaFormula.top+h/2;//-2;
	RectRadio1.right=RectRadio1.left+t*2;
	RectRadio1.bottom=RectRadio1.top+(h*3)/7;

	GetDlgItem(IDC_ICONRADIO1)->GetWindowRect(&RectIconRadio1);
	RectIconRadio1.left=RectRadio1.right;
	RectIconRadio1.top=RectRadio1.top-7;
	RectIconRadio1.right=RectIconRadio1.left+32;
	RectIconRadio1.bottom=RectIconRadio1.top+32;

	GetDlgItem(IDC_RADIO2)->GetWindowRect(&RectRadio2);
	RectRadio2.left=RectIconRadio1.right+t*4;
	RectRadio2.top=RectRadio1.top;
	RectRadio2.right=RectRadio2.left+t*2;
	RectRadio2.bottom=RectRadio1.bottom;

	GetDlgItem(IDC_ICONRADIO2)->GetWindowRect(&RectIconRadio2);
	RectIconRadio2.left=RectRadio2.right;
	RectIconRadio2.top=RectIconRadio1.top;
	RectIconRadio2.right=RectIconRadio2.left+32;
	RectIconRadio2.bottom=RectIconRadio1.bottom;
	
	//Порог
	GetDlgItem(IDC_STATICPOROG)->GetWindowRect(&RectStaticPorog);
	RectStaticPorog.left=RectZonaCharacteristic.left;//RectZonaFormula.left;
	RectStaticPorog.top=RectZonaPar.bottom+3;//RectZonaFormula.bottom+16;
	RectStaticPorog.right=RectZonaCharacteristic.right;//RectZonaFormula.right;
	RectStaticPorog.bottom=RectStaticPorog.top+h+h/2+4;//-3;

	GetDlgItem(IDC_TEXTMIN)->GetWindowRect(&RectTextMin);
	RectTextMin.left=RectStaticPorog.left+t;
	RectTextMin.top=RectStaticPorog.top+h/2;//-2;
	RectTextMin.right=RectTextMin.left+t*5-8;
	RectTextMin.bottom=RectTextMin.top+h/3;

	GetDlgItem(IDC_EDITMIN)->GetWindowRect(&RectEditMin);
	RectEditMin.left=RectTextMin.right+8;
	RectEditMin.top=RectTextMin.top-2;
	RectEditMin.right=RectStaticPorog.right-t/2;
	RectEditMin.bottom=RectEditMin.top+h/3+5;

	GetDlgItem(IDC_TEXTMAX)->GetWindowRect(&RectTextMax);
	RectTextMax.left=RectTextMin.left;
	RectTextMax.top=RectEditMin.bottom+7;
	RectTextMax.right=RectTextMin.right;
	RectTextMax.bottom=RectTextMax.top+h/3;

	GetDlgItem(IDC_EDITMIN)->GetWindowRect(&RectEditMax);
	RectEditMax.left=RectEditMin.left;
	RectEditMax.top=RectTextMax.top-2;
	RectEditMax.right=RectEditMin.right;
	RectEditMax.bottom=RectEditMax.top+h/3+5;

	ScreenToClient(&RectZonaCharacteristic);
	pSt = (CStatic*)GetDlgItem(IDC_ZONACHARACTERISTIC);
	pSt->MoveWindow(&RectZonaCharacteristic);

	ScreenToClient(&RectAxisY);
	pSt = (CStatic*)GetDlgItem(IDC_AXISY);
	pSt->MoveWindow(&RectAxisY);

	ScreenToClient(&RectFormula);
	pSt = (CStatic*)GetDlgItem(IDC_FORMULA);
	pSt->MoveWindow(&RectFormula);

	ScreenToClient(&RectAxisX);
	pSt = (CStatic*)GetDlgItem(IDC_AXISX);
	pSt->MoveWindow(&RectAxisX);

	ScreenToClient(&RectEndFormula);
	pSt = (CStatic*)GetDlgItem(IDC_ENDFORMULA);
	pSt->MoveWindow(&RectEndFormula);
	ScreenToClient(&RectZonaPar);
	pSt = (CStatic*)GetDlgItem(IDC_ZONAPARAM);
	pSt->MoveWindow(&RectZonaPar);

	ScreenToClient(&RectTextFind);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTFIND);
	pSt->MoveWindow(&RectTextFind);

	ScreenToClient(&RectEditPar);
	pSt = (CStatic*)GetDlgItem(IDC_EDITNAMEPAR);
	pSt->MoveWindow(&RectEditPar);

	ScreenToClient(&RectTextFor);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTFOR);
	pSt->MoveWindow(&RectTextFor);

	ScreenToClient(&RectEditArg);
	pSt = (CStatic*)GetDlgItem(IDC_EDITNAMEARG);
	pSt->MoveWindow(&RectEditArg);

	ScreenToClient(&RectText);
	pSt = (CStatic*)GetDlgItem(IDC_TEXT);
	pSt->MoveWindow(&RectText);

	ScreenToClient(&RectTextUr1);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTUR1);
	pSt->MoveWindow(&RectTextUr1);

	ScreenToClient(&RectTextUr2);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTUR2);
	pSt->MoveWindow(&RectTextUr2);

	ScreenToClient(&RectEditUr1);
	pSt = (CStatic*)GetDlgItem(IDC_EDITUR1);
	pSt->MoveWindow(&RectEditUr1);

	ScreenToClient(&RectEditUr2);
	pSt = (CStatic*)GetDlgItem(IDC_EDITUR2);
	pSt->MoveWindow(&RectEditUr2);

	ScreenToClient(&RectEditPoint);
	pSt = (CStatic*)GetDlgItem(IDC_EDITPOINT);
	pSt->MoveWindow(&RectEditPoint);


	ScreenToClient(&RectZonaFormula);
	pSt = (CStatic*)GetDlgItem(IDC_ZONAFORMULA);
	pSt->MoveWindow(&RectZonaFormula);

	ScreenToClient(&RectRadio1);
	pSt = (CStatic*)GetDlgItem(IDC_RADIO1);
	pSt->MoveWindow(&RectRadio1);

	ScreenToClient(&RectIconRadio1);
	pSt = (CStatic*)GetDlgItem(IDC_ICONRADIO1);
	pSt->MoveWindow(&RectIconRadio1);

	ScreenToClient(&RectRadio2);
	pSt = (CStatic*)GetDlgItem(IDC_RADIO2);
	pSt->MoveWindow(&RectRadio2);

	ScreenToClient(&RectIconRadio2);
	pSt = (CStatic*)GetDlgItem(IDC_ICONRADIO2);
	pSt->MoveWindow(&RectIconRadio2);

	ScreenToClient(&RectStaticPorog);
	pSt = (CStatic*)GetDlgItem(IDC_STATICPOROG);
	pSt->MoveWindow(&RectStaticPorog);

	ScreenToClient(&RectTextMin);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTMIN);
	pSt->MoveWindow(&RectTextMin);

	ScreenToClient(&RectEditMin);
	pSt = (CStatic*)GetDlgItem(IDC_EDITMIN);
	pSt->MoveWindow(&RectEditMin);

	ScreenToClient(&RectTextMax);
	pSt = (CStatic*)GetDlgItem(IDC_TEXTMAX);
	pSt->MoveWindow(&RectTextMax);

	ScreenToClient(&RectEditMax);
	pSt = (CStatic*)GetDlgItem(IDC_EDITMAX);
	pSt->MoveWindow(&RectEditMax);

	//Зона отображения положения рукоятки
	//====================================
	RectZona.left=RectParam.left+2;//2;
	RectZona.top=RectParam.bottom+1;
	RectZona.right=RectParam.right-2;
	RectZona.bottom=RectParam.bottom+h-2;//-8;//-1;//
	RectEditRukoyatka.left=RectParam.left+w-t-2;//RectKolSourU.left;
	RectEditRukoyatka.top=RectZona.top;//+2;//2;//bottom;//RectParam.bottom+h/2-8;//RectStatImp.bottom;//RectKI.top;
	RectEditRukoyatka.right=RectParam.right-t*2;//RectKolSourU.right;
	RectEditRukoyatka.bottom=RectEditRukoyatka.top+h/2+5;//RectStatImp.bottom+22;//RectKI.bottom;

	//светодиод (сигнал о высоком напряжении)
	GetDlgItem(IDC_STATICMIGSIGNAL)->GetWindowRect(&RectMigSignal);
	RectMigSignal.left=RectZona.left-2;//+1;//12;//8;//10;
	RectMigSignal.top=RectZona.top;//RectZonaText.top;//+1;//8;//4;
	RectMigSignal.right=RectMigSignal.left+64;//39;//24;
	RectMigSignal.bottom=RectMigSignal.top+60;//26;//16;
	GetDlgItem(IDC_STATICZONATEXT)->GetWindowRect(&RectZonaText);
	RectZonaText.left=RectMigSignal.right+4;//RectParam.left+2;//10;//RectZona.left+1;//t*5-10;
	RectZonaText.top=RectZona.top+10;//h/8;
	RectZonaText.right=RectZona.right-1;//*3;
	RectZonaText.bottom=RectZona.bottom-1;

	GetDlgItem(IDC_PROGRESS_PROCESS)->GetWindowRect(&RectProgressProcess);
	RectProgressProcess.left=RectZonaText.left;//RectTabTuning.left+t*3;
	RectProgressProcess.top=RectZonaText.top+6;//RectTabTuning.top+h+h/3;
	RectProgressProcess.right=RectZonaText.right-2;//RectTabTuning.right-t*3;
	RectProgressProcess.bottom=RectProgressProcess.top+h/3+5;//RectTabTuning.bottom-5;//h/2-7;
	ScreenToClient(&RectProgressProcess);
    pSt = (CStatic*)GetDlgItem(IDC_PROGRESS_PROCESS);
	pSt->MoveWindow(&RectProgressProcess);
						
	ScreenToClient(&RectZona);
	ScreenToClient(&RectZonaText);
    pSt = (CStatic*)GetDlgItem(IDC_STATICZONATEXT);
	pSt->MoveWindow(&RectZonaText);

	ScreenToClient(&RectMigSignal);
    pSt = (CStatic*)GetDlgItem(IDC_STATICMIGSIGNAL);
	pSt->MoveWindow(&RectMigSignal);
    pSt = (CStatic*)GetDlgItem(IDC_STATICNOMIGSIGNAL);
	pSt->MoveWindow(&RectMigSignal);

	//начальная установка положения рукоятки
	//**************************************
	fontRukoyatka.CreateFont((RectZona.bottom-RectZona.top)-24,//(RectEditRukoyatka.bottom-RectEditRukoyatka.top),
		                 0,
					     0,0,FW_NORMAL,
	                     0,0,0,RUSSIAN_CHARSET,
					     OUT_DEFAULT_PRECIS,
					     CLIP_DEFAULT_PRECIS,
					     DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,
					     _T("Arial"));
   ::WaitForSingleObject(hMutexRukoyatka,INFINITE);
 	m_iPosTek = 0;
	m_iPosTekOld = m_iPosTek;
   ::ReleaseMutex(hMutexRukoyatka);
	
	ScreenToClient(&RectParam);
	m_TabCtrlParam.MoveWindow(&RectParam);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	m_TabCtrlParam.InsertItem(0,_T("Cхема включения"));		//Page0
	m_TabCtrlParam.InsertItem(1,_T("Расчёт"));				//Page1
	m_TabCtrlParam.InsertItem(2,_T("Канал  C"));			//Page2 - Коллектор
	m_TabCtrlParam.InsertItem(3,_T("Канал  B"));			//Page3 - База
	m_TabCtrlParam.InsertItem(4,_T("Канал  S"));			//Page4 - Подложка (Доп. канал)
	m_TabCtrlParam.SetCurSel(0);

	ScreenToClient(&RectPage12);
	ScreenToClient(&RectPage34);

	ScreenToClient(&RectBitmapDiod);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_DIOD);
	pSt->MoveWindow(&RectBitmapDiod);

	ScreenToClient(&RectBitmapRes);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_RES);
	pSt->MoveWindow(&RectBitmapRes);

	ScreenToClient(&RectBitmapNpn);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_NPN);
	pSt->MoveWindow(&RectBitmapNpn);

	ScreenToClient(&RectRadioNpn);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_VKL);
	pSt->MoveWindow(&RectRadioNpn);

	ScreenToClient(&RectBitmapPnp);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_PNP);
	pSt->MoveWindow(&RectBitmapPnp);

	ScreenToClient(&RectBitmapNmos);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_NMOS);
	pSt->MoveWindow(&RectBitmapNmos);

	ScreenToClient(&RectBitmapPmos);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_PMOS);
	pSt->MoveWindow(&RectBitmapPmos);

	ScreenToClient(&RectBitmapNpn4);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_NPN4);
	pSt->MoveWindow(&RectBitmapNpn4);

	ScreenToClient(&RectBitmapPnp4);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_PNP4);
	pSt->MoveWindow(&RectBitmapPnp4);

	ScreenToClient(&RectBitmapNmos4);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_NMOS4);
	pSt->MoveWindow(&RectBitmapNmos4);

	ScreenToClient(&RectBitmapPmos4);
    pSt = (CStatic*)GetDlgItem(IDC_STATICBITMAP_PMOS4);
	pSt->MoveWindow(&RectBitmapPmos4);

	ScreenToClient(&RectCategoria);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_ELEMENT);
	pSt->MoveWindow(&RectCategoria);

	ScreenToClient(&RectKnot);
    pSt = (CStatic*)GetDlgItem(IDC_STATICKNOT);
	pSt->MoveWindow(&RectKnot);

	ScreenToClient(&RectKommutator);
    pSt = (CStatic*)GetDlgItem(IDC_COMBO_KOMMUTATOR);
	pSt->MoveWindow(&RectKommutator);

	ScreenToClient(&RectKnotKol);
	pSt = (CStatic*)GetDlgItem(IDC_STATICGNEZDO_C);
	pSt->MoveWindow(&RectKnotKol);

	ScreenToClient(&RectKnotBaza);
	pSt = (CStatic*)GetDlgItem(IDC_STATICGNEZDO_B);
	pSt->MoveWindow(&RectKnotBaza);

	ScreenToClient(&RectKnotDop);
	pSt = (CStatic*)GetDlgItem(IDC_STATICGNEZDO_S);
	pSt->MoveWindow(&RectKnotDop);

	ScreenToClient(&RectKnotEm);
	pSt = (CStatic*)GetDlgItem(IDC_STATICGNEZDO_E);
	pSt->MoveWindow(&RectKnotEm);

	ScreenToClient(&RectBitKnotKol);
	pSt = (CStatic*)GetDlgItem(IDC_COMBO_GNEZDO_C);
	pSt->MoveWindow(&RectBitKnotKol);

	ScreenToClient(&RectBitKnotBaza);
	pSt = (CStatic*)GetDlgItem(IDC_COMBO_GNEZDO_B);
	pSt->MoveWindow(&RectBitKnotBaza);

	ScreenToClient(&RectBitKnotDop);
	pSt = (CStatic*)GetDlgItem(IDC_STATIC_GNEZDO_S);
	pSt->MoveWindow(&RectBitKnotDop);

	ScreenToClient(&RectBitKnotEm);
	pSt = (CStatic*)GetDlgItem(IDC_COMBO_GNEZDO_E);
	pSt->MoveWindow(&RectBitKnotEm);

	ScreenToClient(&RectStatusKol);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOCHECKKOL);
	pSt->MoveWindow(&RectStatusKol);

	ScreenToClient(&RectStatusBaza);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOCHECKBAZA);
	pSt->MoveWindow(&RectStatusBaza);

	ScreenToClient(&RectStatusDop);
	pSt = (CStatic*)GetDlgItem(IDC_COMBOCHECKDOP);
	pSt->MoveWindow(&RectStatusDop);

	ScreenToClient(&RectTest);
    pSt = (CStatic*)GetDlgItem(IDC_STATICMOD);
	pSt->MoveWindow(&RectTest);

	ScreenToClient(&RectComboTest);
    pSt = (CStatic*)GetDlgItem(IDC_COMBOMOD);
	pSt->MoveWindow(&RectComboTest);

	ScreenToClient(&RectButtonLoadTst);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_LOADTST);
	pSt->MoveWindow(&RectButtonLoadTst);

	//Панель управления дисплеем и памятью
	//=====================================
	VERIFY(m_LEFT.AutoLoad(IDC_BUTTON_LEFT,this));
	VERIFY(m_RIGHT.AutoLoad(IDC_BUTTON_RIGHT,this));
	VERIFY(m_RESET.AutoLoad(IDC_BUTTON_MARKER_RESET,this));

	RectTabTuning.left=RectParam.left+1;
	RectTabTuning.top=RectParam.bottom+h;//-6;//RectZona.bottom+5;//RectPanel.bottom+h;
	RectTabTuning.right=RectParam.right;
	RectTabTuning.bottom=RectWnd.bottom;//-2;

	GetDlgItem(IDC_STATIC_FON)->GetWindowRect(&RectFon);
	RectFon.left=RectTabTuning.left+6;
	RectFon.top=RectTabTuning.top+h/2-2;//+2;//-4;//-10;//h/2;//+2;//5;//8;//-4;
	RectFon.right=RectTabTuning.right-8;//32;//31;//25;
	RectFon.bottom=RectTabTuning.bottom-8;//+20;//14;
	ScreenToClient(&RectFon);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_FON);
	pSt->MoveWindow(&RectFon);

	GetDlgItem(IDC_RADIO_LEFT)->GetWindowRect(&RectButtonLeft);
	RectButtonLeft.left=RectTabTuning.left+t*2-4;//25;//18;//t*3-8;//RectRadioDiod.left;//RectTabTuning.left+t+t/2+2;
	RectButtonLeft.top=RectTabTuning.top+h-2;//+2;//h/2;//+2;//5;//8;//-4;
	RectButtonLeft.right=RectButtonLeft.left+t*3;//22;//32;//31;//25;
	RectButtonLeft.bottom=RectButtonLeft.top+h/3-8;//4;//21;//20;//14;

	GetDlgItem(IDC_RADIO_CENTER)->GetWindowRect(&RectButtonCenter);
	RectButtonCenter.left=RectButtonLeft.left;
	RectButtonCenter.top=RectButtonLeft.bottom+20;
	RectButtonCenter.right=RectButtonCenter.left+t*3;//22;//32;//31;//25;
	RectButtonCenter.bottom=RectButtonCenter.top+h/3-8;//4;//21;//20;//14;

	GetDlgItem(IDC_RADIO_RIGHT)->GetWindowRect(&RectButtonRight);
	RectButtonRight.left=RectButtonLeft.left;
	RectButtonRight.top=RectButtonCenter.bottom+20;
	RectButtonRight.right=RectButtonRight.left+t*3;//22;//32;//31;//25;
	RectButtonRight.bottom=RectButtonRight.top+h/3-8;//4;//21;//20;//14;

	GetDlgItem(IDC_STATIC_NULL_LEFT)->GetWindowRect(&RectIconLeft);
	RectIconLeft.left=RectButtonLeft.right+2;//-4;//+t/2;//+2;//8;//+t/2-3;
	RectIconLeft.top=RectTabTuning.top+h/2+27;//23;//+2;//8;//3-1;//+3;
	RectIconLeft.right=RectIconLeft.left+32;//23;
	RectIconLeft.bottom=RectIconLeft.top+32;//23;

	GetDlgItem(IDC_STATIC_NULL_CENTER)->GetWindowRect(&RectIconCenter);
	RectIconCenter.left=RectButtonCenter.right+t/2+4;//-4;//;//-4;
	RectIconCenter.top=RectIconLeft.bottom+2;//15;//8;
	RectIconCenter.right=RectIconCenter.left+32;//23;
	RectIconCenter.bottom=RectIconCenter.top+32;//23;

	GetDlgItem(IDC_STATIC_NULL_RIGHT)->GetWindowRect(&RectIconRight);
	RectIconRight.left=RectButtonRight.right+t/2+4;//-4;//;//-4;
	RectIconRight.top=RectIconCenter.bottom-8;//15;//8;
	RectIconRight.right=RectIconRight.left+32;//23;
	RectIconRight.bottom=RectIconRight.top+32;//23;
 
	GetDlgItem(IDC_CHECK_INVERS_X)->GetWindowRect(&RectButtonInversion);
	RectButtonInversion.left=RectIconLeft.right+t*2;//2-2;//3-4;//3-4;//RectButtonLeft.left;
	RectButtonInversion.top=RectButtonLeft.top-4;//bottom-10;//RectButtonCenter.top;//RectButtonLeft.top;//RectButtonRight.bottom+7;//19;
	RectButtonInversion.right=RectButtonInversion.left+t*2-4;//3;//20;//RectFon.right-t*3+10;//RectButtonInversion.left+32;//31;//25;
	RectButtonInversion.bottom=RectButtonInversion.top+h/3-3;//28;//14;

	GetDlgItem(IDC_STATIC_ICON_INVERS)->GetWindowRect(&RectIconInversion);
	RectIconInversion.left=RectButtonInversion.right+8;//+6;//7;//+t-2;//-5;
	RectIconInversion.top=RectButtonInversion.top;//+6;//RectIconRight.bottom+15;//25;
	RectIconInversion.right=RectIconInversion.left+22;//21;
	RectIconInversion.bottom=RectIconInversion.top+14;//8;

	GetDlgItem(IDC_CHECK_INVERS_Y)->GetWindowRect(&RectButtonInversionY);
	RectButtonInversionY.left=RectIconInversion.right+t*3;//2-2;//RectButtonInversion.left;//RectButtonLeft.left;//RectButtonMarker2.left;
	RectButtonInversionY.top=RectButtonInversion.top;//RectButtonInversion.bottom+8;//17;//RectButtonInversion.top;
	RectButtonInversionY.right=RectButtonInversionY.left+t*2-4;//3;//20;//RectFon.right-t*3+10;//RectButtonInversionY.left+32;//31;//25;
	RectButtonInversionY.bottom=RectButtonInversionY.top+h/3-3;//28;//14;

	GetDlgItem(IDC_STATIC_TEXT_INVERSY)->GetWindowRect(&RectIconInversionY);
	RectIconInversionY.left=RectButtonInversionY.right+6;//+6;//7;//-5;
	RectIconInversionY.top=RectButtonInversionY.top;//+6;//4;//RectIconInversion.top;
	RectIconInversionY.right=RectIconInversionY.left+24;//21;
	RectIconInversionY.bottom=RectIconInversionY.top+14;//8;

	GetDlgItem(IDC_CHECK_LOOPING)->GetWindowRect(&RectLooping);
	RectLooping.left=RectButtonInversion.left;
	RectLooping.top=RectButtonCenter.top;//RectIconInversion.bottom+10;
	RectLooping.right=RectLooping.left+t*2-4;//4;//3;//RectTabTuning.right-10;//RectLooping.left+40;
	RectLooping.bottom=RectLooping.top+h/3-3;//3;//RectTabTuning.bottom-10;//RectLooping.top+h/3;//+2;
	
	GetDlgItem(IDC_STATIC_ICON_LOOPING)->GetWindowRect(&RectTextLooping);
	RectTextLooping.left=RectLooping.right+t/2-4;//+7;//RectButtonInversion.left;
	RectTextLooping.top=RectIconCenter.top;//RectButtonCenter.top-11;//RectButtonCenter.top+10;//RectIconInversion.bottom+10;
	RectTextLooping.right=RectTextLooping.left+32;//RectTabTuning.right-17;//RectLooping.left+40;
	RectTextLooping.bottom=RectIconCenter.bottom;//RectTabTuning.bottom-10;//RectLooping.top+h/3;//+2;

	GetDlgItem(IDC_CHECK_FAMILY)->GetWindowRect(&RectFamilyVkl);
	RectFamilyVkl.left=RectButtonInversionY.left;
	RectFamilyVkl.top=RectLooping.top;//RectButtonCenter.top;
	RectFamilyVkl.right=RectButtonInversionY.right;
	RectFamilyVkl.bottom=RectLooping.bottom;//RectButtonCenter.bottom;

	GetDlgItem(IDC_STATIC_ICON_FAMILY)->GetWindowRect(&RectIconFamily);
	RectIconFamily.left=RectFamilyVkl.right+t/2-3;//RectIconInversionY.left;
	RectIconFamily.top=RectIconCenter.top;
	RectIconFamily.right=RectIconFamily.left+32;//RectIconInversionY.right;
	RectIconFamily.bottom=RectIconCenter.bottom;

	ScreenToClient(&RectFamilyVkl);
	pSt = (CStatic*)GetDlgItem(IDC_CHECK_FAMILY);
	pSt->MoveWindow(&RectFamilyVkl);
	ScreenToClient(&RectIconFamily);
	pSt = (CStatic*)GetDlgItem(IDC_STATIC_ICON_FAMILY);
	pSt->MoveWindow(&RectIconFamily);

	//высокое разрешение (коллектор) (режим "Экранная лупа" )
	GetDlgItem(IDC_CHECKKOLREJHIGH)->GetWindowRect(&RectKolRejHigh);
	RectKolRejHigh.left=RectLooping.left;//RectLimit.left;//+t;
	RectKolRejHigh.top=RectButtonRight.top;//-2;//+20;//RectLimit.bottom+3;//10;//h/10;
	RectKolRejHigh.right=RectKolRejHigh.left+t*2-4;//RectLimit.right;
	RectKolRejHigh.bottom=RectKolRejHigh.top+h/3-3;//RectKolRejHigh.top+h/2+3;//3+4;
	
	GetDlgItem(IDC_STATIC_ICON_LUPA)->GetWindowRect(&RectIconLupa);
	RectIconLupa.left=RectKolRejHigh.right+6;//RectIconInversion.left;
	RectIconLupa.top=RectKolRejHigh.top-9;
	RectIconLupa.right=RectIconLupa.left+32;
	RectIconLupa.bottom=RectIconLupa.top+32;//+2;

	ScreenToClient(&RectKolRejHigh);
	pSt=(CStatic*)GetDlgItem(IDC_CHECKKOLREJHIGH);
	pSt->MoveWindow(&RectKolRejHigh);
	ScreenToClient(&RectIconLupa);
	pSt=(CStatic*)GetDlgItem(IDC_STATIC_ICON_LUPA);
	pSt->MoveWindow(&RectIconLupa);

	RectButtonMarkerReset.left=RectIconLupa.right+t*2+4;//RectButtonMarkerLeft.right+6;//t/2;
	RectButtonMarkerReset.top=RectIconFamily.bottom+2;//RectButtonMarkerLeft.top;
	RectButtonMarkerReset.right=RectButtonMarkerReset.left+42;
	RectButtonMarkerReset.bottom=RectButtonMarkerReset.top+23;

	GetDlgItem(IDC_MARKER1)->GetWindowRect(&RectMarker1);
	RectMarker1.left=RectTablRes.left+3;//m_RectWndGraf.left+3;//+t;//*2;
	RectMarker1.top=RectTablRes.top+3;//RectWnd.top+3;
	RectMarker1.right=RectPanel.left-50;//3;//-t+5;
	RectMarker1.bottom=RectMarker1.top+h/3;//+2;

	GetDlgItem(IDC_MARKER2)->GetWindowRect(&RectMarker2);
	RectMarker2.left=RectMarker1.left;
	RectMarker2.top=RectMarker1.bottom+2;
	RectMarker2.right=RectMarker1.right;
	RectMarker2.bottom=RectMarker2.top+h/3;//+2;

	GetDlgItem(IDC_TANGENT)->GetWindowRect(&RectTangent);
	RectTangent.left=RectMarker1.left;
	RectTangent.top=RectMarker2.bottom+2;
	RectTangent.right=RectMarker1.right;
	RectTangent.bottom=RectTangent.top+h/3;//+2;

	GetDlgItem(IDC_STATIC_MARKER1)->GetWindowRect(&RectButtonMarker1);
	RectButtonMarker1.left=RectMarker1.right+4;//t;//RectTabTuning.left+t*2;
	RectButtonMarker1.top=RectMarker1.top+2;//RectTabTuning.top+h-5;
	RectButtonMarker1.right=RectButtonMarker1.left+18;//32;
	RectButtonMarker1.bottom=RectButtonMarker1.top+18;//20
	RgnButtonMarker1.CreateRectRgnIndirect(&RectButtonMarker1);

	GetDlgItem(IDC_STATIC_NAME_MARKER1)->GetWindowRect(&RectTextMarker1);
	RectTextMarker1.left=RectButtonMarker1.right+4;//-2;//+t/2+5;
	RectTextMarker1.top=RectButtonMarker1.top+4;//-1;
	RectTextMarker1.right=RectTextMarker1.left+25;//50;//RectTabTuning.right-t;
	RectTextMarker1.bottom=RectTextMarker1.top+h/3;

	GetDlgItem(IDC_STATIC_MARKER2)->GetWindowRect(&RectButtonMarker2);
	RectButtonMarker2.left=RectButtonMarker1.left;
	RectButtonMarker2.top=RectButtonMarker1.bottom+4;//12;//18;//;
	RectButtonMarker2.right=RectButtonMarker2.left+18;//32;//31;//25;
	RectButtonMarker2.bottom=RectButtonMarker2.top+18;//20;//14;
	RgnButtonMarker2.CreateRectRgnIndirect(&RectButtonMarker2);

	GetDlgItem(IDC_STATIC_NAME_MARKER2)->GetWindowRect(&RectTextMarker2);
	RectTextMarker2.left=RectTextMarker1.left;
	RectTextMarker2.top=RectButtonMarker2.top+4;//-1;
	RectTextMarker2.right=RectTextMarker1.right;
	RectTextMarker2.bottom=RectTextMarker2.top+h/3;


	GetDlgItem(IDC_STATIC_BUTTON_TANGENT)->GetWindowRect(&RectButtonTangent);
	RectButtonTangent.left=RectButtonMarker1.left;//RectTextMarker1.right+10;//RectIconLeft.right+t*4;//RectIconInversion.right+t;
	RectButtonTangent.top=RectButtonMarker2.bottom+4;//18;//RectButtonMarker2.top;//RectButtonInversion.top;
	RectButtonTangent.right=RectButtonTangent.left+18;//32;//31;//25;
	RectButtonTangent.bottom=RectButtonTangent.top+18;//20;//14;
	RgnButtonTangent.CreateRectRgnIndirect(&RectButtonTangent);

	//Закладка "Память"
	//=================
	//Архив
	//=====
	GetDlgItem(IDC_LISTREF)->GetWindowRect(&RectListRef);
	RectListRef.left=RectWnd.left+1;
	RectListRef.top=RectInfoTest.bottom;
	RectListRef.right=RectButtons.right;//RectWnd.right-(RectWnd.right/4);
	RectListRef.bottom=RectWnd.bottom-(RectWnd.bottom/20)-6;

	ScreenToClient(&RectListRef);
    pSt = (CStatic*)GetDlgItem(IDC_LISTREF);
	pSt->MoveWindow(&RectListRef);
	//...
	m_listRef.SetExtendedStyle(m_listRef.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//Закладка "Ошибки"
	//=================
	GetDlgItem(IDC_EDITERRORS)->GetWindowRect(&RectEditErrors);
	RectEditErrors.left=RectTabTuning.left+6;//t-4;///2;
	RectEditErrors.top=RectTabTuning.top+h/2-2;//+2;//3+h/10;
	RectEditErrors.right=RectTabTuning.right-t+4;///2;
	RectEditErrors.bottom=RectTabTuning.bottom-5;//h/2-7;

	ScreenToClient(&RectEditErrors);
    pSt = (CStatic*)GetDlgItem(IDC_EDITERRORS);
	pSt->MoveWindow(&RectEditErrors);


	ScreenToClient(&RectTabTuning);
	m_TabCtrlTuning.MoveWindow(&RectTabTuning);
	m_TabCtrlTuning.InsertItem(0,_T("  Шкала  "));//Дисплей  "));
	m_TabCtrlTuning.InsertItem(2,_T(" Ошибки "));

	ScreenToClient(&RectButtonLeft);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_LEFT);
	pSt->MoveWindow(&RectButtonLeft);
	
	ScreenToClient(&RectButtonCenter);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_CENTER);
	pSt->MoveWindow(&RectButtonCenter);

	ScreenToClient(&RectButtonRight);
    pSt = (CStatic*)GetDlgItem(IDC_RADIO_RIGHT);
	pSt->MoveWindow(&RectButtonRight);

	ScreenToClient(&RectIconLeft);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_NULL_LEFT);
	pSt->MoveWindow(&RectIconLeft);

	ScreenToClient(&RectIconCenter);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_NULL_CENTER);
	pSt->MoveWindow(&RectIconCenter);

	ScreenToClient(&RectIconRight);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_NULL_RIGHT);
	pSt->MoveWindow(&RectIconRight);

	ScreenToClient(&RectButtonInversion);
    pSt = (CStatic*)GetDlgItem(IDC_CHECK_INVERS_X);
	pSt->MoveWindow(&RectButtonInversion);

	ScreenToClient(&RectIconInversion);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_ICON_INVERS);
	pSt->MoveWindow(&RectIconInversion);

	ScreenToClient(&RectButtonInversionY);
    pSt = (CStatic*)GetDlgItem(IDC_CHECK_INVERS_Y);
	pSt->MoveWindow(&RectButtonInversionY);

	ScreenToClient(&RectIconInversionY);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_TEXT_INVERSY);
	pSt->MoveWindow(&RectIconInversionY);

	ScreenToClient(&RectLooping);
    pSt = (CStatic*)GetDlgItem(IDC_CHECK_LOOPING);
	pSt->MoveWindow(&RectLooping);
	ScreenToClient(&RectTextLooping);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_ICON_LOOPING);
	pSt->MoveWindow(&RectTextLooping);

	ScreenToClient(&RectButtonMarkerReset);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_MARKER_RESET);
	pSt->MoveWindow(&RectButtonMarkerReset);

	ScreenToClient(&RectButtonMarkerLeft);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_LEFT);
	pSt->MoveWindow(&RectButtonMarkerLeft);

	ScreenToClient(&RectButtonMarkerRight);
    pSt = (CStatic*)GetDlgItem(IDC_BUTTON_RIGHT);
	pSt->MoveWindow(&RectButtonMarkerRight);

	ScreenToClient(&RectButtonMarker1);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_MARKER1);
	pSt->MoveWindow(&RectButtonMarker1);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_MARKER1_ON);
	pSt->MoveWindow(&RectButtonMarker1);

	ScreenToClient(&RectButtonMarker2);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_MARKER2);
	pSt->MoveWindow(&RectButtonMarker2);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_MARKER2_ON);
	pSt->MoveWindow(&RectButtonMarker2);

	ScreenToClient(&RectTextMarker1);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_NAME_MARKER1);
	pSt->MoveWindow(&RectTextMarker1);

	ScreenToClient(&RectTextMarker2);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_NAME_MARKER2);
	pSt->MoveWindow(&RectTextMarker2);

	ScreenToClient(&RectButtonTangent);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT);
	pSt->MoveWindow(&RectButtonTangent);
    pSt = (CStatic*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT_ON);
	pSt->MoveWindow(&RectButtonTangent);

	ScreenToClient(&RectMarker1);
    pSt = (CStatic*)GetDlgItem(IDC_MARKER1);
	pSt->MoveWindow(&RectMarker1);

	ScreenToClient(&RectMarker2);
    pSt = (CStatic*)GetDlgItem(IDC_MARKER2);
	pSt->MoveWindow(&RectMarker2);

	ScreenToClient(&RectTangent);
    pSt = (CStatic*)GetDlgItem(IDC_TANGENT);
	pSt->MoveWindow(&RectTangent);

	//Начальная установка
	//===================
	//**************************
    SetTimer(6,1000,NULL);	//индикация текущего времени
    SetTimer(3,250,NULL);	//сигнал "Высокое напряжение"
	//программирование ползунка ограничения по U (коллектор)
	//******************************************************
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLU);
	pSl -> SetRange(0,100);
	pSl -> SetTicFreq(5);
	//программирование ползунка ограничения по I (коллектор)
	//******************************************************
    pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLI);
	pSl -> SetRange(0,100);
	pSl -> SetTicFreq(5);	
	//программирование наборного счётчика числа ступенек (база)
	//*********************************************************
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINSTEPBAZA);
	pSpin -> SetRange(0,10);
	pSpin -> SetPos(m_uStepBaza);
	//программирование ползунка смещения (база)
	//*****************************************
	pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERBAZABIAS);
	pSl -> SetRange(0,1000);
	pSl -> SetTicFreq(50);
	//программирование ползунка ограничения (база)
	//********************************************
	pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERLIMITBAZA);
	pSl -> SetRange(10,110);
	pSl -> SetTicFreq(5);

	//программирование наборного счётчика числа ступенек (доп.канал)
	//*********************************************************
	pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINSTEPDOP);
	pSpin -> SetRange(0,10);
	pSpin -> SetPos(m_uStepDop);
	//программирование ползунка смещения (доп.канал)
	//*****************************************
	pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERDOPBIAS);
	pSl -> SetRange(0,1000);
	pSl -> SetTicFreq(50);
	//программирование ползунка ограничения (доп.канал)
	//********************************************
	pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERLIMITDOP);
	pSl -> SetRange(10,110);
	pSl -> SetTicFreq(5);
	//индикатор процесса
	m_Progress.SetRange(0,100);
	m_Progress.SetStep(5);
	m_ProgPos=0;
 

	InitStatusKnot();	//инициализация состояния узлов (в зависимости от типа элемента)
	InitGnezdo();
	
	//наименования узлов, характеристик (коллектор, база, доп. источник, эмиттер)
	//***************************************************************************
	//начальная установка диапазонов измерителя и ограничения по U и I
	//****************************************************************
			InitIconImp();

	DefinitionDiapazonKol();
	DefinitionDiapazonBaza();
	DefinitionDiapazonDop();

	InitDiapazonKol();		//инициализация диапазонов коллектора (в зависимости от вида развёртки)
	InitDiapazonBaza();		//инициализация диапазонов базы
	InitDiapazonDop();		//инициализация диапазонов доп.канала
	//индикация начального состояния панели
	//*************************************
	int  nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
	m_TabCtrlTuning.SetCurSel(nTab);
	InitPageTuning(nTab);
 	  CComboBox *pcb = (CComboBox*) GetDlgItem(IDC_COMBOMOD);
	  index=pcb->FindStringExact(0,m_MOD);

	//начальная установка схемы включения
	//***********************************
	//начальная установка списка файлов эталонных графиков в архиве
	//*************************************************************
	m_listRef.InsertColumn(0,_T("№"),LVCFMT_LEFT,(RectListRef.Width()-2)/20);
	m_listRef.InsertColumn(1,_T("График"),LVCFMT_LEFT,(RectListRef.Width()-2)/4);
	m_listRef.InsertColumn(2,_T("Дата"),LVCFMT_LEFT,(RectListRef.Width()-2)/9);
	m_listRef.InsertColumn(3,_T("Время"),LVCFMT_LEFT,(RectListRef.Width()-2)/9);
	m_listRef.InsertColumn(4,_T("Комментарий"),LVCFMT_LEFT,(RectListRef.Width()-2)/2);

	DisplayListRef();

	m_brushBlack.CreateSolidBrush(RGB(0,0,0));
	m_brushGray.CreateSolidBrush(RGB(203,203,203));//111,111,111));//134,134,134));//128,128,128));
	m_brushGrayLight.CreateSolidBrush(RGB(241,241,241));//234,234,234));//213,213,213));//203,203,203));
	m_brushWhite.CreateSolidBrush(RGB(255,255,255));
	m_brushBlue.CreateSolidBrush(RGB(0,71,142));
	brFonCircuit.CreateSolidBrush(RGB(255,255,0));
	m_brushYellowLight.CreateSolidBrush(RGB(255,255,81));
	m_brushGreenLight.CreateSolidBrush(RGB(128,255,128));

	m_sInfoMeas=_T("");
	strFileName= _T("");

   hEventComOut=::CreateEvent(NULL,FALSE,FALSE,NULL);
    hEventEppIn=::CreateEvent(NULL,FALSE,FALSE,NULL);
    hEventStop=::CreateEvent(NULL,FALSE,FALSE,NULL);

	hMutexO =::CreateMutex(NULL,FALSE,NULL);
	hMutexI =::CreateMutex(NULL,FALSE,NULL);
	hMutexEpp =::CreateMutex(NULL,FALSE,NULL);
    hMutexStop=::CreateMutex(NULL,FALSE,NULL);
    hMutexRukoyatka=::CreateMutex(NULL,FALSE,NULL);
	hMutexDCres_gotov=::CreateMutex(NULL,FALSE,NULL);

	HWND hWnd = GetSafeHwnd();

    pThreadComIn = AfxBeginThread(ThreadComIn,hWnd,
					               THREAD_PRIORITY_NORMAL,
					               0,CREATE_SUSPENDED);
    pThreadComIn->ResumeThread();

    pThreadComOut = AfxBeginThread(ThreadComOut,hWnd,
					               THREAD_PRIORITY_NORMAL,
					               0,CREATE_SUSPENDED);
    pThreadComOut->ResumeThread();

	pThreadEppIn = AfxBeginThread(ThreadEppIn,hWnd,
					              THREAD_PRIORITY_NORMAL,//THREAD_PRIORITY_ABOVE_NORMAL- нельзя!!!
					              0,CREATE_SUSPENDED);
    pThreadEppIn->ResumeThread();


//	Передать данные настройки
//============================
	SetKoefPswToPic();
	
	TRACE(_T("=============OnInitDialog()================\n"));
//	График
//==========
	SetTextInfo(_T("TEST: NPN \\ IcVc"));
	UINT nID = 3223;
	int i=0;
	int j=0;
//пример упорядочивания массива по возрастанию
//=============================================
//===========================================================

	CRect rect(m_RectWndGraf.left,m_RectWndGraf.top,m_RectWndGraf.right,m_RectWndGraf.bottom);//(5,5,500,400);

	m_Chart.Create(this,rect,nID);

	m_Chart.DisableRefresh(true);


    SubGetAxes(X_min,X_max,&m_iX,&expo_X);
	SubGetAxes(Y_min,Y_max,&m_iY,&expo_Y);
	CChartSerie*  pGraph;


n_Step = 1;//!!!!!!!!!!!!!!!
n_Mas  =1;
  for (j=0;j<n_Step;j++)
  {//семейство кривых
	switch(m_iStyleCurve)
	{ case STYLE_LINE:	    pGraph = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraph = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraph = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}
	
	for (UINT m=0;m<n_Mas;m++)
	{
		for (i=0;i<NPOINT;i++)
		{
		switch(iX)
		{ case 0: pX[i] = pM1[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
				  break;
		  case 1: 
		  case 2: pX[i] = X_axis[i] / pow(10.0,expo_X-po_X); 
				  break;
		}
		switch(iY)
		{ case 0: pY[i] = pM2[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
				  break;
		  case 1: pY[i] = pM3[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
				  break;
		  case 2: pY[i] = pM4[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y);
				  break;
		}
		}
	}//end of m

	pGraph->SetColor(m_colorCurve);							//цвет кривой
	pGraph->SetPoints(pX,pY,NPOINT*n_Mas);
  }//end of семейство кривых
	CChartSerieFamily* pFamily = m_Chart.AddFamily();
	pFamily->SetColor(m_colorCurve);
	pFamily->SetName("Meas");

	  po_X = SubGetStrRazmer(rej_X,expo_X,m_sAxisX);
	  m_Chart.m_pAxisBottom->SetLabelText(strRazmer);
	  ScaleX = strRazmer;
	  m_Chart.m_pAxisBottom->SetMinMax(cifra_axes[m_iButNull][m_iX][0] * pow(10.0,po_X),
										cifra_axes[m_iButNull][m_iX][1] * pow(10.0,po_X));
	  MinX = cifra_axes[m_iButNull][m_iX][0] * pow(10.0,po_X);
	  MaxX = cifra_axes[m_iButNull][m_iX][1] * pow(10.0,po_X);
	  m_Chart.m_pAxisBottom->SetBigTickIncrement(big_tick[m_iButNull][m_iX] * pow(10.0,po_X));
	  StepX = big_tick[m_iButNull][m_iX] * pow(10.0,po_X);
	  po_Y = SubGetStrRazmer(rej_Y,expo_Y,m_sAxisY);
	  m_Chart.m_pAxisLeft->SetLabelText(strRazmer);
	  ScaleY = strRazmer;
	  m_Chart.m_pAxisLeft->SetMinMax(cifra_axes[m_iButNull][m_iY][0] * pow(10.0,po_Y),
									 cifra_axes[m_iButNull][m_iY][1] * pow(10.0,po_Y));
	  MinY = cifra_axes[m_iButNull][m_iY][0] * pow(10.0,po_Y);
	  MaxY = cifra_axes[m_iButNull][m_iY][1] * pow(10.0,po_Y);
	  m_Chart.m_pAxisLeft->SetBigTickIncrement(big_tick[m_iButNull][m_iY] * pow(10.0,po_Y));
	  StepY = big_tick[m_iButNull][m_iY] * pow(10.0,po_Y);
	
	m_Chart.SetBackColor(m_colorFon);						//цвет фона
	m_Chart.m_pAxisBottom->SetGridColor(m_colorSetka);	    //цвет сетки по оси Х
	m_Chart.m_pAxisLeft->SetGridColor(m_colorSetka);		//цвет сетки по оси Y
	m_Chart.m_pAxisBottom->SetLabelTextColor(m_colorText);  //цвет надписи по оси Х
	m_Chart.m_pAxisLeft->SetLabelTextColor(m_colorText);	//цвет надписи по оси Y
	m_Chart.m_pAxisBottom->SetTextColor(m_colorSetka);
	m_Chart.m_pAxisLeft->SetTextColor(m_colorSetka);
	m_Chart.SetFrameColor(m_colorSetka);
	m_Chart.m_pSecant->SetColor(m_colorTangent);			//цвет секущей
	m_Chart.ShowLegend(true);

	m_Chart.DisableRefresh(false);

	strDateMeas = strDateCurrent;
    strTimeMeas = strTimeCurrent;

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLineDlg::SetMassivNpoint(BOOL bInversion) 
{ 
  //iX - канал, с которого берётся результат по оси Х 
  //(он же - источник 1-ой очереди): 0-коллектор, 1-база, 2-доп.канал
  //if iX = 0 - массив результатов измерения напряжения на коллекторе,
  //if 1 или 2 - массив X_axis
  //iY - канал, с которого берётся результат измерения по оси Y (0-коллектор, 1-база, 2-доп.канал) 
  // pM1[t] - АЦП1 - Uc
  // pM2[t] - АЦП2 - Ic
  // pM3[t] - АЦП3 - Baza
  // pM4[t] - АЦП4 - Dop

//	DWORD tick = ::GetTickCount();

	int i=0;
	int j=0;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_iQue1==COLLECTOR)
	{
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	if (m_bBeta || m_bBetai || m_bS)  n_point =1;	//одно значение на каждой ступеньке при Uc = 5V
	}
	if (m_iQue1==BAZA)
	{
		if (m_irazv==IMP_POS || m_irazv==IMP_NEG) {n_point = 1; n_Mas=m_uStepBaza*10;}
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}

    m_Chart.DisableRefresh(true);
	CChartSerie*  pGraph;
	
 for (j=0;j<n_Step;j++)
 { 	
	pGraph = m_Chart.GetSerie(j);

 
	if (((m_irazv==SYN_POS || m_irazv==SYN_NEG) && !m_bLooping) ||
		(m_irazv==AC && !m_bLooping) ||
		(m_nelement==TS))	pGraph->SetSortPoints(false);
    else  pGraph->SetSortPoints(true);
	
  for (UINT m=0;m<n_Mas;m++)
  {	
		for(i=0;i<n_point;i++)
		{
			switch(iX)
			{ case 0: //pX[i+n_point*m] = pM1[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
					  if (m_bKolX==false) pX[i+n_point*m] = pM1[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
								     else pX[i+n_point*m] = pM2[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
					  break;
			  case 1: if(iX == iY)
						  pX[i+n_point*m] = X_axis[i+n_point*m] / pow(10.0,expo_X-po_X); 
					  else	
						  pX[i+n_point*m] = pM3[m][i+NPOINT*j] / pow(10.0,expo_X-po_X); 
			 		  break;
			  case 2: if(iX == iY)
						  pX[i+n_point*m] = X_axis[i+n_point*m] / pow(10.0,expo_X-po_X); 
					  else
						  pX[i+n_point*m] = pM4[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
					  break;
			}
			switch(iY)
			{ case 0: //pY[i+n_point*m] = pM2[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
					  if (m_bKolY==false) pY[i+n_point*m] = pM2[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
								     else pY[i+n_point*m] = pM1[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
                      break;
			  case 1: pY[i+n_point*m] = pM3[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
			 		  break;
			  case 2: pY[i+n_point*m] = pM4[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y);
					  break;
			}
		}

  }//end of m
	pGraph->SetPoints(pX,pY,n_point*n_Mas);
}//end of j

	bStatusMeas = true;
	m_Chart.DisableRefresh(false);
}

void CLineDlg::SetMassiv(BOOL bInversion) 
{ 
  //iX - канал, с которого берётся результат по оси Х 
  //(он же - источник 1-ой очереди): 0-коллектор, 1-база, 2-доп.канал
  //if iX = 0 - массив результатов измерения напряжения на коллекторе,
  //if 1 или 2 - массив X_axis
  //iY - канал, с которого берётся результат измерения по оси Y (0-коллектор, 1-база, 2-доп.канал) 
  // pM1[t] - АЦП1 - Uc
  // pM2[t] - АЦП2 - Ic
  // pM3[t] - АЦП3 - Baza
  // pM4[t] - АЦП4 - Dop

      r1=0.0;
	  r2=0.0;
	  double x  = 0.0;	//искомое значение аргумента при значении 1 или 2 параметра
	  double y  = 0.0;	//искомое значение функции при значении 1 или 2 параметра

	int i=0;
	int j=0;
	int n_step=n_Step;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_iQue1==COLLECTOR)
	{
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	if (m_bBeta || m_bBetai || m_bS)  n_point =1;	//одно значение на каждой ступеньке при Uc = 5V
	}
	if (m_iQue1==BAZA)
	{
		if (m_irazv==IMP_POS || m_irazv==IMP_NEG) {n_point = 1; n_Mas=m_uStepBaza*10;}
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}

	m_Chart.DisableRefresh(true);
	CChartSerie*  pGraph;

 for(j=0;j<n_step;j++)
 {  
	pGraph = m_Chart.GetSerie(j);

	if (((m_irazv==SYN_POS || m_irazv==SYN_NEG) && !m_bLooping) ||
		(m_irazv==AC && !m_bLooping) ||
		(m_nelement==TS))	pGraph->SetSortPoints(false);
    else  pGraph->SetSortPoints(true);


	for (UINT m=0;m<n_Mas;m++)
	{
		for(i=0;i<n_point;i++)
		{
		  switch(iX)
		  { case 0: if (m_bKolX==false) pX[i+n_point*m] = pM1[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
								   else pX[i+n_point*m] = pM2[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
					break;
			  case 1: if(iX == iY)
						  pX[i+n_point*m] = X_axis[i+n_point*m] / pow(10.0,expo_X-po_X); 
					  else
						  pX[i+n_point*m] = pM3[m][i+NPOINT*j] / pow(10.0,expo_X-po_X); 
			 		  break;
			  case 2: if(iX == iY)
						  pX[i+n_point*m] = X_axis[i+n_point*m] / pow(10.0,expo_X-po_X); 
					  else
						  pX[i+n_point*m] = pM4[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
					  break;
		  }
		  switch(iY)
		  { case 0: if (m_bKolY==false) pY[i+n_point*m] = pM2[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
								   else pY[i+n_point*m] = pM1[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
					break;
			case 1: pY[i+n_point*m] = pM3[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
					break;
		    case 2: pY[i+n_point*m] = pM4[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y);
				    break;
		  }		
		  if (!b_flagStatusRef)
		  {//копия для архива
		    refX[m][i+n_point*j] = pX[i+n_point*m];
		    refY[m][i+n_point*j] = pY[i+n_point*m];
		  }
		  if (bMeasDiff)	
		  {//копия для разности ВАХ
			diffX[m][i+n_point*j] = pX[i+n_point*m]*pow(10.0,expo_X-po_X);
		    diffY[m][i+n_point*j] = pY[i+n_point*m]*pow(10.0,expo_Y-po_Y);
		  }
		}

	}//end of m
	pGraph->SetColor(m_colorCurve);							//цвет кривой
	pGraph->SetPoints(pX,pY,n_point*n_Mas);
}//end of for j

	bStatusMeas = true;
	m_Chart.DisableRefresh(false);

	// поиск параметра на ступеньке c № m_uTp = j-ой кривой
	//--------------------------------------------------------
  if (!bMeasDiff && !bStatusDiff)	
  {
	if (!m_sPar.IsEmpty() && m_uFunc<2)
	{ 
		//расчет параметра 1
	  // из уравнения прямой по 2-м точкам (x1,y1) (x2,y2): 
	  // (y-y1)/(y2-y1)=(x-x1)/(x2-x1) получаем: 
	  // y = (y2-y1)*(x-x1)/(x2-x1)+y1;
	  // x = ((y-y1)*(x2-x1))/(y2-y1)+x1;
	  if (m_uFunc==0)	// найти значение функции по значению аргумента 
		{ if (!m_sUr1.IsEmpty())
			{ 
		      Ur1 = Ur1 / pow(10.0,expo_X-po_X);

		      y = GetYbyX(Ur1,m_uTp);
			  
			  Ur1 = Ur1 * pow(10.0,expo_X-po_X);
			  r1 = y * pow(10.0,expo_Y-po_Y);
			}//end of if (!m_sUr1.IsEmpty())

		 if (!m_sUr2.IsEmpty())
			{
			  Ur2 = Ur2 / pow(10.0,expo_X-po_X);
			  y  = GetYbyX(Ur2,m_uTp);

			  Ur2 = Ur2 * pow(10.0,expo_X-po_X);
			  r2 = y * pow(10.0,expo_Y-po_Y);
			}// end of if (!m_sUr2.IsEmpty())

		  if (!m_sUr1.IsEmpty() && !m_sUr2.IsEmpty()) 
		  { switch(m_iForm)
			{ case 0: if ((Ur2-Ur1)!= 0.0) Par = (r2-r1)/(Ur2-Ur1);
					  break;
			  case 1: if ((r2-r1)!=0.0) Par = (Ur2-Ur1)/(r2-r1);
					  break;
			}
		  }
	      else Par = r1;
		}//end of m_uFunc==0

	  if (m_uFunc==1)	//найти значение аргумента по значению функции
		{ if (!m_sUr1.IsEmpty()) 
			{
		      Ur1 = Ur1 / pow(10.0,expo_Y-po_Y);
		      x = GetXbyY(Ur1,m_uTp);

			  Ur1 = Ur1 * pow(10.0,expo_Y-po_Y);
			  r1 = x * pow(10.0,expo_X-po_X);
			}//end of if (!m_sUr1.IsEmpty())

		  if (!m_sUr2.IsEmpty())
			{
			  Ur2 = Ur2 / pow(10.0,expo_Y-po_Y);
			  x  = GetXbyY(Ur2,m_uTp);

			  Ur2 = Ur2 * pow(10.0,expo_Y-po_Y);
			  r2 = x * pow(10.0,expo_X-po_X);
			}
		  if (!m_sUr1.IsEmpty() && !m_sUr2.IsEmpty()) 
		  { switch(m_iForm)
			{ case 0: if ((r2-r1)!=0.0) Par = (Ur2-Ur1)/(r2-r1);
					  break;
			  case 1: if ((Ur2-Ur1)!=0.0) Par = (r2-r1)/(Ur2-Ur1);
					  break;
			}
		  }
	      else Par = r1;
		}//end of m_uFunc==1
	}//end of поиск параметра

  }//end of if (!bMeasDiff && !bStatusDiff)	

	 //-------------------------

	TRACE(L"SetMassiv()!!!\n");
}

double CLineDlg::GetXbyY(double Ur_y, int index_j)
{ //поиск аргумента по функции 
	if (index_j>0) index_j=index_j-1;

	int n_step=n_Step;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	double Ur_x=0.0;
	double delta_min = fabs(refY[0][0+n_point*index_j]- Ur_y);
	int    index_m,m;
	int    index,i;

 for (m=0;m<n_Mas;m++)
  for(i=0;i<n_point;i++)
	 if (fabs(refY[m][i+n_point*index_j]- Ur_y)<=delta_min) 
	 { delta_min = fabs(refY[m][i+n_point*index_j]-Ur_y);
	   index_m = m;
	   index = i;
	 }
	 else continue;
if (index<=n_point) Ur_x = refX[index_m][index+n_point*index_j];

return Ur_x;
}

double CLineDlg::GetYbyX(double Ur_x, int index_j)
{//поиск функции по аргументу
	if (index_j>0) index_j=index_j-1;
	int    index_m,m;
	int    index,i;

	int n_step=n_Step;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	double Ur_y=0.0;
	double delta_min = 0.0;

	delta_min = fabs(refX[0][0+n_point*index_j]- Ur_x);

 for (m=0;m<n_Mas;m++)
  for(i=0;i<n_point;i++)
	 if (fabs(refX[m][i+n_point*index_j]- Ur_x)<=delta_min) 
	 { delta_min = fabs(refX[m][i+n_point*index_j]-Ur_x);
	   index_m = m;
	   index = i;
	 }
	 else continue;
if (index<=n_point) Ur_y = refY[index_m][index+n_point*index_j];

return Ur_y;
}


void CLineDlg::InitAxis() 
{
	TRACE (L"InitAxis();\n");

	m_Chart.DisableRefresh(true);
	
	CChartSerieFamily* pFamily = m_Chart.GetFamily(0);
			pFamily->SetName("Meas");

  if(!m_bCheckKolRejHigh)
  {
	  m_Chart.m_pAxisBottom->SetAutomatic(false);
	  m_Chart.m_pAxisLeft->SetAutomatic(false);

	uPrecigion = 1;
	if ((m_iX==1 || m_iY==1) && (m_iButNull==LEFT || m_iButNull==RIGHT)) uPrecigion = 2;
	
	  po_X = SubGetStrRazmer(rej_X,expo_X,m_sAxisX);
	  m_Chart.m_pAxisBottom->SetLabelText(strRazmer);
	  ScaleX = strRazmer;
	  TRACE(L"%f, %f\n",cifra_axes[m_iButNull][m_iX][0],cifra_axes[m_iButNull][m_iX][1]);
      m_Chart.m_pAxisBottom->SetMinMax(cifra_axes[m_iButNull][m_iX][0] * pow(10.0,po_X),
		                               cifra_axes[m_iButNull][m_iX][1] * pow(10.0,po_X));
	  MinX = cifra_axes[m_iButNull][m_iX][0] * pow(10.0,po_X);
	  MaxX = cifra_axes[m_iButNull][m_iX][1] * pow(10.0,po_X);
      m_Chart.m_pAxisBottom->SetBigTickIncrement(big_tick[m_iButNull][m_iX] * pow(10.0,po_X));
	  StepX = big_tick[m_iButNull][m_iX] * pow(10.0,po_X);

	  po_Y = SubGetStrRazmer(rej_Y,expo_Y,m_sAxisY);
	  m_Chart.m_pAxisLeft->SetLabelText(strRazmer);
	  ScaleY = strRazmer;
	  m_Chart.m_pAxisLeft->SetMinMax(cifra_axes[m_iButNull][m_iY][0] * pow(10.0,po_Y),
		                             cifra_axes[m_iButNull][m_iY][1] * pow(10.0,po_Y));
	  MinY = cifra_axes[m_iButNull][m_iY][0] * pow(10.0,po_Y);
	  MaxY = cifra_axes[m_iButNull][m_iY][1] * pow(10.0,po_Y);
	  m_Chart.m_pAxisLeft->SetBigTickIncrement(big_tick[m_iButNull][m_iY] * pow(10.0,po_Y));
	  StepY = big_tick[m_iButNull][m_iY] * pow(10.0,po_Y);

  }
  if(m_bCheckKolRejHigh)
  {
      m_Chart.m_pAxisBottom->SetMinMax(MinX,MaxX);
      m_Chart.m_pAxisBottom->SetBigTickIncrement(StepX);
	  m_Chart.m_pAxisLeft->SetMinMax(MinY,MaxY);
	  m_Chart.m_pAxisLeft->SetBigTickIncrement(StepY);
	  m_Chart.m_pAxisBottom->SetAutomatic(true);
	  m_Chart.m_pAxisLeft->SetAutomatic(true);
  }
	m_Chart.DisableRefresh(false);
}


void CLineDlg::InitStatusKol()
{  
	UpdateData();
	switch(m_iCircuitKol)
	{ case -1: case 0:	//обрыв
			   if (n_IPPP==1 && (n_Switch==1 || n_Switch==2))
			   {//запретить обрыв на +- 5V
				 m_iCircuitKol=1;
			   }
			   if (n_Switch==0)	
			   {
			     m_irazv = DC_POS;//-1;//---!
			     if (m_nelement==PNP ||m_nelement==PNP4 ||
				   m_nelement==PMOS ||m_nelement==PMOS4) m_irazv = DC_NEG;
			     m_idlitimp = 0;
			   }
			   break;
	  case 1:  //вкл.
				if (m_iQue1==COLLECTOR)
				{
			   m_irazv = SYN_POS;//IMP_POS; //AC	//---!
			   if ((n_Switch==0) && 
				   (m_nelement==PNP ||m_nelement==PNP4 ||
				   m_nelement==PMOS ||m_nelement==PMOS4)) m_irazv = SYN_NEG;
				}	 
				if (m_iQue1==BAZA)
				{
			   m_irazv = DC_POS;//IMP_POS; //AC	//---!
			   if ((n_Switch==0) && 
				   (m_nelement==PNP ||m_nelement==PNP4 ||
				   m_nelement==PMOS ||m_nelement==PMOS4)) m_irazv = DC_NEG;
				}	 
			   m_idlitimp = 0;
			   if (n_Switch==1) { m_idlitimp=5;m_irazv=IMP_POS;}
			   if (n_Switch==2) { m_idlitimp=5;m_irazv=IMP_NEG;}
		       break;
	}
	UpdateData(FALSE);
}

void CLineDlg::InitStatusBaza()
{
	UpdateData();
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	{ //база
	  m_bBaza= false;
	  m_bBazaVkl = false;
	  m_uStepBaza = 0;
	  m_bFamilyVkl = false;
	  m_iFamilyVkl = 0;
	  m_bFamilyVklOld = m_bFamilyVkl;
	  n_Step = 1;
	  switch(m_nelement)
	  {	 
		case NPN:	 //2
		case PNP:	 //3		
		case NPN4:	 //6
		case PNP4:	 //7	//биполярные
				m_iBazaSource = 1;	//ист. I
				m_iBazaMeas = 1;	//изм. I	//0;	//изм. U
				break;
		case NMOS:	 //4
		case PMOS:	 //5	
		case NMOS4:	 //8
		case PMOS4:	 //9	//МОП
				m_iBazaSource = 0;	//ист. U
				m_iBazaMeas = 0;	//изм. U	//1;	//изм. I
				break;
		case TS:	 //10
				m_iBazaSource = 1;	//ист. I
				m_iBazaMeas = 1;	//изм. I	
				break;
	  }
	}
	else
	{ 
	  m_bBazaVkl = false;
	  m_bBaza= false;
	  m_uStepBaza = 0;
	  m_bFamilyVkl = false;
	  m_iFamilyVkl = 0;
	  m_bFamilyVklOld = m_bFamilyVkl;
	  n_Step = 1;
	}
	UpdateData(FALSE);
}

void CLineDlg::InitStatusDop()
{
	UpdateData();
	if (m_iCircuitDop==1 && m_iKnotDop==1)
	{ //база
	  m_iDopSource = 0;	
	  m_iDopMeas = 0;//1;
	  m_bDop = false;
	  m_uStepDop = 0;
	  m_bFamilyVkl = false;
	  m_iFamilyVkl = 0;
	  m_bFamilyVklOld = m_bFamilyVkl;
	  n_Step = 1;
	}
	else
	{
	  m_bDop = false;
	  m_uStepDop = 0;
	  m_bFamilyVkl = false;
	  m_iFamilyVkl = 0;
	  m_bFamilyVklOld = m_bFamilyVkl;
	  n_Step = 1;
	}
	UpdateData(FALSE);
}

void CLineDlg::SetStringsSourceMeas()
{ CWnd* pwnd;
  //наименоания узлов и измеряемых и генерируемых величин
  //======================================================
  TRACE(_T("SetStringsSourceMeas()\n"));

	CString uzel_C =_T("");
	CString uzel_B =_T("");
	CString uzel_S =_T("");
	CString uzel_E =_T("");
	n_obryv = 0;	//количество обрывов при внешней коммутации
	b_InitAxis=true;

  if (m_nelement!=-1)
  {
	if (m_iKommutator==0)
	{//внутренняя коммутация=> прямое подключение электродов
		uzel_C = strKnot[m_nelement][COLLECTOR]; m_iKnotKol=1;
		uzel_B = strKnot[m_nelement][BAZA];      m_iKnotBaza=1;
		uzel_S = strKnot[m_nelement][DOP];       m_iKnotDop=1;
		uzel_E = strKnot[m_nelement][EMITTER];   m_iKnotEm=1;
	}
	else
	{//внешняя коммутация => возможно обратное подключение электродов
	   //всё стереть
       ShowControls(bmpRejKnotExtern,SIZEOF_ARRAY(bmpRejKnotExtern),FALSE);	

		if (m_nelement==DIOD || m_nelement==RES)
		{ 
			pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
			if (pwnd){ pwnd->MoveWindow(&RectBitKol);
					   pwnd->ShowWindow(SW_SHOW);}
			pwnd=GetDlgItem(IDC_STATICBITKNOT_E);
		    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
					   pwnd->ShowWindow(SW_SHOW);}
			switch(m_iGnezdoC)
			{  
				case 0:		uzel_C = _T("a"); uzel_E = _T("k");	break;	
				case 1:	    uzel_C = _T("k"); uzel_E = _T("a");	break;	
			}
		}

		if ((m_nelement==NPN) || (m_nelement==PNP) || (m_nelement==NPN4) || (m_nelement==PNP4)) 
		{	
		  switch(m_iGnezdoC)
		  { case 0: uzel_C = _T("c"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;
		    case 1: uzel_B = _T("c"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;
		    case 2: uzel_E = _T("c"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;
			case 3: n_obryv++;
				    pwnd=GetDlgItem(IDC_STATUS_KOL_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;

		  }
		  switch(m_iGnezdoB)
		  { case 0: uzel_C = _T("b");
		  			pwnd=GetDlgItem(IDC_STATUS_BAZA_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("b"); 
		  			pwnd=GetDlgItem(IDC_STATUS_BAZA_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;
		    case 2: uzel_E = _T("b");
		  			pwnd=GetDlgItem(IDC_STATUS_BAZA_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;
			case 3: pwnd=GetDlgItem(IDC_STATUS_BAZA_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
					break;
		  }
		  switch(m_iGnezdoE)
		  { case 0: uzel_C = _T("e"); 
					pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("e"); 
				    pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 2: uzel_E = _T("e"); 
				    pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
			case 3: pwnd=GetDlgItem(IDC_STATUS_EM_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
					break;
		  }
		}
		if ((m_nelement==NMOS) || (m_nelement==PMOS) || (m_nelement==NMOS4) || (m_nelement==PMOS4)) 
		{	
		  switch(m_iGnezdoC)
		  { case 0: uzel_C = _T("d"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("d");
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;
		    case 2: uzel_E = _T("d"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
				    break;
			case 3: 
				    pwnd=GetDlgItem(IDC_STATUS_KOL_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
				    break;
		  }
		  switch(m_iGnezdoB)
		  { case 0: uzel_C = _T("g"); 
		  			pwnd=GetDlgItem(IDC_STATUS_GATE_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("g");
		  			pwnd=GetDlgItem(IDC_STATUS_GATE_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 2: uzel_E = _T("g");
		  			pwnd=GetDlgItem(IDC_STATUS_GATE_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
			case 3: pwnd=GetDlgItem(IDC_STATUS_GATE_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
					break;

		  }
		  switch(m_iGnezdoE)
		  { case 0: uzel_C = _T("s");
					pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("s");
					pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 2: uzel_E = _T("s");
					pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
			case 3: pwnd=GetDlgItem(IDC_STATUS_EM_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
					break;
		  }
		}
		if (m_nelement==TS)
		{	
		  switch(m_iGnezdoC)
		  { case 0: uzel_C = _T("a"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("a"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 2: uzel_E = _T("a"); 
					pwnd=GetDlgItem(IDC_STATUS_KOL_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
			case 3: 
				    pwnd=GetDlgItem(IDC_STATUS_KOL_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitKol);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
				    break;
		  }
		  switch(m_iGnezdoB)
		  { case 0: uzel_C = _T("g"); 
		  			pwnd=GetDlgItem(IDC_STATUS_BAZA_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("g"); 
		  			pwnd=GetDlgItem(IDC_STATUS_BAZA_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 2: uzel_E = _T("g"); 
		  			pwnd=GetDlgItem(IDC_STATUS_BAZA_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
			case 3: pwnd=GetDlgItem(IDC_STATUS_BAZA_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitBaza);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
					break;
		  }
		  switch(m_iGnezdoE)
		  { case 0: uzel_C = _T("k"); 
					pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 1: uzel_B = _T("k"); 
					pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
		    case 2: uzel_E = _T("k"); 
					pwnd=GetDlgItem(IDC_STATICBITKNOT_E);//IDC_STATUS_EM_ON);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					break;
			case 3: pwnd=GetDlgItem(IDC_STATUS_EM_OFF);
				    if (pwnd){ pwnd->MoveWindow(&RectBitEm);
							   pwnd->ShowWindow(SW_SHOW);}
					n_obryv++;
					break;
		  }
		}

		if ((m_nelement==NPN4) || (m_nelement==PNP4)|| (m_nelement==TS)) uzel_S = _T("s");
		if ((m_nelement==NMOS4) || (m_nelement==PMOS4)) uzel_S = _T("b");
	

	}
	  if (uzel_C==_T("")) m_iKnotKol=0; else m_iKnotKol=1;
	  if (uzel_B==_T("")) m_iKnotBaza=0; else m_iKnotBaza=1;
	  if (uzel_S==_T("")) m_iKnotDop=0; else m_iKnotDop=1;// m_iKnotDop=0;
	  if (uzel_E==_T("")) m_iKnotEm=0; else m_iKnotEm=1;

	  if	((m_iGnezdoC==m_iGnezdoB && m_iGnezdoC!=3) || 
		 (m_iGnezdoC==m_iGnezdoE && m_iGnezdoC!=3) ||
		 (m_iGnezdoB==m_iGnezdoE && m_iGnezdoB!=3) ||
		 (m_nelement>PMOS && n_obryv>2) ||
 		 (m_nelement>RES && m_nelement<NPN4 && n_obryv>1) ||
		  (uzel_C==_T("") && m_iQue1==COLLECTOR) ||
		  (uzel_B==_T("") && m_iQue1==BAZA))

	{ SetTextInfo(_T("ПРОВЕРЬТЕ СХЕМУ ВКЛЮЧЕНИЯ!"));
	}
	else
	{//OK'
	  strTestName = m_EL + _T(" \\ ") + m_MOD;
	  SetTextInfo(_T("TEST: ") + strTestName);
	// коллектор
	//**********
	  m_sKnotKol = uzel_C;
      switch (m_iCircuitKol)
	  { case -1: case 0: case 1:
			    m_sKolMeasU   = _T("U") + uzel_C;	
			    m_sKolMeasI   = _T("I") + uzel_C;	
			    m_sSliderKolU = m_sKolMeasU;
				break;
	  }

	// база 
	//******
   	  m_sKnotBaza   = uzel_B;//strKnot[m_nelement][BAZA];
      if (m_iCircuitBaza==1)
	  {	
		switch(m_iBazaSource)
		{ case -1:  m_sBazaSource=_T(""); break;
		  case 0:	//источник U
		        m_sBazaSource=_T("U") + uzel_B; 
				m_sBazaMeasLimit=_T("I") + uzel_B; break;
	      case 1:   //источник I
		        m_sBazaSource=_T("I") + uzel_B; 
				m_sBazaMeasLimit=_T("U") + uzel_B; break;
		}

		if (m_sAxisY==m_sBazaMeasLimit)  
		{ m_bBaza = true;
		  m_iBazaMeas=!m_iBazaSource;
		}
		else
		{ m_bBaza = false;
		  m_iBazaMeas=m_iBazaSource;
		}

		switch(m_iBazaMeas)
		{ case -1:  m_sBazaMeas=_T("");	break;
		  case 0:	//измеритель U
					m_sBazaMeas=_T("U") + uzel_B; break;
		  case 1:   //измеритель I
					m_sBazaMeas=_T("I") + uzel_B; break;
		}
	  }
	// доп. канал 
	//***********
	if (m_iCircuitDop==1)
	{
	m_sKnotDop    = uzel_S;//strKnot[m_nelement][DOP];
	switch(m_iDopSource)
	{ case -1:  m_sDopSource=_T(""); break;
	  case 0:	//источник U
		        m_sDopSource=_T("U") + uzel_S; 
				m_sDopMeasLimit=_T("I") + uzel_S; break;
	  case 1:   //источник I
		        m_sDopSource=_T("I") + uzel_S; 
				m_sDopMeasLimit=_T("U") + uzel_S; break;
	}

    if (m_sAxisY==m_sDopMeasLimit)  
	{ m_bDop = true;
	  m_iDopMeas=!m_iDopSource;
	}
	else
	{ m_bDop = false;
	  m_iDopMeas=m_iDopSource;
	}

	switch(m_iDopMeas)
	{ case -1:  m_sDopMeas=_T("");	break;
	  case 0:	//измеритель U
				m_sDopMeas=_T("U") + uzel_S; break;
	  case 1:   //измеритель I
				m_sDopMeas=_T("I") + uzel_S; break;
	}
	}
	// эмиттер
	//********
	m_sKnotEm     = uzel_E;//strKnot[m_nelement][EMITTER];
	m_sRangMaxKolI = _T("О г р а н и ч е н и е  ")+ m_sSliderKolU+_T(", %");
	UpdateData(FALSE);	//вставила 29.11.17
//	ClearResults();
//вставила 29.11.17=>
	if(m_sPar==_T("Beta"))   m_bBeta = true;
						else m_bBeta = false;
	if(m_sPar==_T("Betai")) m_bBetai = true;
						else m_bBetai = false;
	if(m_sPar==_T("R")) m_bR	= true;
						else m_bR = false;
	if(m_sPar==_T("S")) m_bS	= true;
						else m_bS = false;
//<=					
	InitTablRes();

	}//end of OK'
}  
}


void CLineDlg::InitPage(int nPage)
{
	switch(nPage)
	{ 
	  case 0: //Схема включения
			  ShowControls(aPage0,SIZEOF_ARRAY(aPage0),TRUE);
			  VklRadioElement();
			  if (m_bFileOpen)
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
			  else
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),TRUE);
			  ShowControls(aPage1,SIZEOF_ARRAY(aPage1),FALSE);
			  ShowControls(aPage2,SIZEOF_ARRAY(aPage2),FALSE);
			  ShowControls(aPage3,SIZEOF_ARRAY(aPage3),FALSE);
			  ShowControls(aPage4,SIZEOF_ARRAY(aPage4),FALSE);
			  if (m_nelement==DIOD || m_nelement==RES)	//двухполюсник
			      ShowControls(aPage0_bipolar2,SIZEOF_ARRAY(aPage0_bipolar2),FALSE);
			  if (m_nelement>RES && m_nelement < NPN4)	//трёхполюсник
			      ShowControls(aPage0_bipolar3,SIZEOF_ARRAY(aPage0_bipolar3),FALSE);
			  SetListTest();
			  BuildCircuit();
			
			  ShowQue1(); 

		 	  ShowGnezdoKommutator();
			  SetTest();
			  ShowRejimChanel(); //7.12.17
		      break;
	  case 2:	//	  case 1: //Коллектор
			  ShowControls(aPage0,SIZEOF_ARRAY(aPage0),FALSE);
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
			  ShowControls(aElement_OFF,SIZEOF_ARRAY(aElement_OFF),FALSE);
			  ShowControls(aBmpCircuit_Off,SIZEOF_ARRAY(aBmpCircuit_Off),FALSE);	
			  ShowControls(aPage1,SIZEOF_ARRAY(aPage1),TRUE);
			  ShowControls(aPage2,SIZEOF_ARRAY(aPage2),FALSE);
			  ShowControls(aPage3,SIZEOF_ARRAY(aPage3),FALSE);
			  ShowControls(aPage4,SIZEOF_ARRAY(aPage4),FALSE);
			  if (m_iCircuitKol==0)				//коллектор в обрыве
			      DisableControls(aPage1_break,SIZEOF_ARRAY(aPage1_break),FALSE);
			  InitIconImp();			 
			  break;
	  case 3:	//case 2: //База
			  ShowControls(aPage0,SIZEOF_ARRAY(aPage0),FALSE);
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
			  ShowControls(aElement_OFF,SIZEOF_ARRAY(aElement_OFF),FALSE);
			  ShowControls(aBmpCircuit_Off,SIZEOF_ARRAY(aBmpCircuit_Off),FALSE);	
			  ShowControls(aPage1,SIZEOF_ARRAY(aPage1),FALSE);
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
				  ShowControls(aPage2,SIZEOF_ARRAY(aPage2),TRUE);

			  ShowControls(aPage3,SIZEOF_ARRAY(aPage3),FALSE);
			  ShowControls(aPage4,SIZEOF_ARRAY(aPage4),FALSE);
			  if (m_irazv==SYN_POS || m_irazv==SYN_NEG)	//---!
			  { if  (m_nelement<2)  GetDlgItem(IDC_CHECK_BAZA_VKL)->ShowWindow(SW_HIDE);//двухполюсники
			    if  (m_nelement>=2 && m_iCircuitBaza == 1 && m_iKnotBaza==1) 
					GetDlgItem(IDC_CHECK_BAZA_VKL)->ShowWindow(SW_SHOW);//трех- и четырехполюсники
				else GetDlgItem(IDC_CHECK_BAZA_VKL)->ShowWindow(SW_HIDE);
			  }
			  else  GetDlgItem(IDC_CHECK_BAZA_VKL)->ShowWindow(SW_HIDE);

			  break;
	  case 4:	//case 3: //Дополнительный канал
			  ShowControls(aPage0,SIZEOF_ARRAY(aPage0),FALSE);
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
			  ShowControls(aElement_OFF,SIZEOF_ARRAY(aElement_OFF),FALSE);
			  ShowControls(aBmpCircuit_Off,SIZEOF_ARRAY(aBmpCircuit_Off),FALSE);	
			  ShowControls(aPage1,SIZEOF_ARRAY(aPage1),FALSE);
			  ShowControls(aPage2,SIZEOF_ARRAY(aPage2),FALSE);
	if (m_iCircuitDop==1 && m_iKnotDop==1)
				  ShowControls(aPage3,SIZEOF_ARRAY(aPage3),TRUE);
			  ShowControls(aPage4,SIZEOF_ARRAY(aPage4),FALSE);
			  break;
	  case 1:	//case 4: //Расчёт
			  ShowControls(aPage0,SIZEOF_ARRAY(aPage0),FALSE);
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
			  ShowControls(aElement_OFF,SIZEOF_ARRAY(aElement_OFF),FALSE);
			  ShowControls(aBmpCircuit_Off,SIZEOF_ARRAY(aBmpCircuit_Off),FALSE);	
			  ShowControls(aPage1,SIZEOF_ARRAY(aPage1),FALSE);
			  ShowControls(aPage2,SIZEOF_ARRAY(aPage2),FALSE);
			  ShowControls(aPage3,SIZEOF_ARRAY(aPage3),FALSE);
			  ShowControls(aPage4,SIZEOF_ARRAY(aPage4),TRUE);
			  if (m_sUr1.IsEmpty() || m_sUr2.IsEmpty())
			  { DisableControls(aFormula_OFF,SIZEOF_ARRAY(aFormula_OFF),FALSE);	
			    m_iForm = -1;
			  }
			  else
			  { DisableControls(aFormula_OFF,SIZEOF_ARRAY(aFormula_OFF),TRUE);
			    if (m_iForm=-1) m_iForm=0;
			  }
			  if (m_sPar.IsEmpty() && m_sArg.IsEmpty())
			  { DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
			  }
			  else
			  { if (m_sUr1.IsEmpty() && m_sUr2.IsEmpty())
				   DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
			    else
				   DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),TRUE);	
			  }
		      break;
	}
}

void CLineDlg::ShowQue1() 
{	
    CString str;
	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBO_QUE1);
    pCb->ResetContent();
	str=_T(" Канал С");
	pCb->AddString(str);
	if (m_nelement>RES)
	{
	  str=_T(" Канал B");
	  pCb->AddString(str);
	}
	pCb->SetCurSel(m_iQue1);
}

void CLineDlg::ShowRejimChanel() 
{	
	CWnd* pwnd;
    CString str;
	TRACE(_T("ShowRejimChanel()\n"));
 if (m_iQue1==COLLECTOR)
 {
	//всё стереть
    ShowControls(aIconRejimChanelC,SIZEOF_ARRAY(aIconRejimChanelC),FALSE);
	switch(m_nelement)
	{//1 
	  case DIOD: case RES:  //канал С
							GetDlgItem(IDC_STATICCHANELC)->ShowWindow(SW_SHOW);
							pwnd=GetDlgItem(aIconRejimChanelC[m_irazv]);
							if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
										  pwnd->ShowWindow(SW_SHOW);
										}
							//канал В
							GetDlgItem(IDC_STATICCHANELB)->ShowWindow(SW_HIDE);
							break;
	  case NPN: case PNP: case NMOS: case PMOS:
	  case NPN4: case PNP4: case NMOS4: case PMOS4: case TS:
							//канал С
							if (m_iCircuitKol==1 && m_iKnotKol==1)
							{//2
							  GetDlgItem(IDC_STATICCHANELC)->ShowWindow(SW_SHOW);
							  switch(m_irazv)
							  { case IMP_POS: 
											  if (m_nelement==TS && !m_bFamilyVkl)
											  {
											    pwnd=GetDlgItem(IDC_STATICTIPIMP1);
							                    if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
									                          pwnd->ShowWindow(SW_SHOW);
															}
											  }
											  if (m_nelement!=TS)
											  { 
//											     pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
											     pwnd=GetDlgItem(IDC_STATICTIPIMP);
							                     if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
									                          pwnd->ShowWindow(SW_SHOW);
															}
											  }
											  break;
							    case IMP_NEG:	
											  if (m_nelement==TS && !m_bFamilyVkl)
											  {
											    pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
							                    if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
									                          pwnd->ShowWindow(SW_SHOW);
															}
											  }
											 
							                  if (m_nelement!=TS)
											  {
//											     pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
											     pwnd=GetDlgItem(IDC_STATICTIPIMPNEG);
											     if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
									                          pwnd->ShowWindow(SW_SHOW);
															}
											  }
									          break;
								case DC_POS: case SYN_POS: case AC:
								case DC_NEG: case SYN_NEG:
											  pwnd=GetDlgItem(aIconRejimChanelC[m_irazv]);
							                  if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
										                  pwnd->ShowWindow(SW_SHOW);
														}
											  break;

							  }
							}//2
							//канал В
//							if (m_iCircuitBaza==1 && m_iGnezdoB!=3)
							if (m_iCircuitBaza==1 && m_iKnotBaza==1)
							{//2
							   GetDlgItem(IDC_STATICCHANELB)->ShowWindow(SW_SHOW);
							  switch(m_irazv)
							  { //3
							    case IMP_POS:   
												if (m_bFamilyVkl)
												{ 
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1);
							                      if (pwnd)	{ if (m_nelement!=TS) pwnd->MoveWindow(&RectRejimChanelB);
												                       else		  pwnd->MoveWindow(&RectRejimChanelCB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												  if (m_nelement==TS)
												  { pwnd=GetDlgItem(IDC_STATICTIPIMP1);
							                        if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
										                          pwnd->ShowWindow(SW_SHOW);
																}
												  }
												}
												}
												else
												{
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP2);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP2NEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
											  break;
							    case IMP_NEG: 
												if (m_bFamilyVkl)
												{
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
							                      if (pwnd)	{ if (m_nelement!=TS) pwnd->MoveWindow(&RectRejimChanelB);
												                             else pwnd->MoveWindow(&RectRejimChanelCB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												  if (m_nelement==TS)
												  { pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
							                        if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
										                          pwnd->ShowWindow(SW_SHOW);
																}
												  }
												}
												}
												else
												{
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP2NEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP2);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
											  break;
								case DC_POS:  
												if (m_bFamilyVkl)
												{
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelCB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);//CB
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
												else
												{
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
											  break;
								 case DC_NEG: 
												if (m_bFamilyVkl)
												{
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelCB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);//CB
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
												else
												{
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
											  break;
								
								case SYN_POS: if (m_bBazaVkl)
											  { //4
												if (m_bFamilyVkl)
												{
												//импульс включен, семейстово включено => в базе IMP
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
												else
												{
												//импульс включен, семейстово выключено => в базе IMP2
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP2);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP2NEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
											  }//4
											  else
											  {//4
												if (m_bFamilyVkl)
												{
												  //импульс выключен, семейстово включено => в базе DC
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
												else
												{//5
												  //импульс выключен, семейстово выключено => в базе СONST
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}//5
											  }//4
											  break;
								case SYN_NEG: if (m_bBazaVkl)
											  { //4
												 //импульс включен => в базе IMP
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPIMP1);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											  }//4
											  else
											  { //4
												if (m_bFamilyVkl)
												{
												  //импульс выключен => в базе DC
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
												else
												{
												  //импульс выключен => в базе DC
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
												}
											  }//4
												
											  break;
								case AC:      if (m_nelement!=TS ||
												 (m_nelement==TS && !m_bFamilyVkl))
											  {
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPCONSTNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											  }
											  if (m_nelement==TS && m_bFamilyVkl)
											  {
												if (m_iPolarityStepBaza==0) 
												{ pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											    else
												{ pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
							                      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
										                      pwnd->ShowWindow(SW_SHOW);
															}
												}
											  }
												break;
							
							}//3
			}//2 end of if (m_iCircuitBaza==1 && m_iGnezdoB!=4)
			break;
	}//1
	}//end if (m_iQue1==COLLECTOR)
	
	if (m_iQue1==BAZA)
	{
	//всё стереть
    ShowControls(aIconRejimChanelC,SIZEOF_ARRAY(aIconRejimChanelC),FALSE);
		//канал С
		GetDlgItem(IDC_STATICCHANELC)->ShowWindow(SW_SHOW);
	   if (m_irazv<SYN_POS)
	   { pwnd=GetDlgItem(IDC_STATICTIPCONSTPOS);
	     if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
				      pwnd->ShowWindow(SW_SHOW);
					}
	   }
	   if (m_irazv>SYN_NEG)
	   { pwnd=GetDlgItem(IDC_STATICTIPCONSTNEG);
	     if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelC);
		              pwnd->ShowWindow(SW_SHOW);
					}
		}
		//канал В
		GetDlgItem(IDC_STATICCHANELB)->ShowWindow(SW_SHOW);
	    switch(m_irazv)
		{//1
			case IMP_NEG:	if (m_iPolarityStepBaza==0) 
							{ pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
						      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
						                  pwnd->ShowWindow(SW_SHOW);
										}		
							}
							else
							{ pwnd=GetDlgItem(IDC_STATICTIPIMP1);
						      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
						                  pwnd->ShowWindow(SW_SHOW);
										}
							}
							break;
			case IMP_POS:	if (m_iPolarityStepBaza==0) 
							{ pwnd=GetDlgItem(IDC_STATICTIPIMP1);
						      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
						                  pwnd->ShowWindow(SW_SHOW);
										}		
							}
							else
							{ pwnd=GetDlgItem(IDC_STATICTIPIMP1NEG);
						      if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
						                  pwnd->ShowWindow(SW_SHOW);
										}
							}
							break;
			case DC_NEG:	if (m_iPolarityStepBaza==0) pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
									else	pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
						    if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
						                  pwnd->ShowWindow(SW_SHOW);
										}		
							break;
			case DC_POS:	if (m_iPolarityStepBaza==0) pwnd=GetDlgItem(IDC_STATICTIPDCPOS);
									else	pwnd=GetDlgItem(IDC_STATICTIPDCNEG);
						    if (pwnd)	{ pwnd->MoveWindow(&RectRejimChanelB);
						                  pwnd->ShowWindow(SW_SHOW);
										}		
							break;
		}//1

	}//end if (m_iQue1==BAZA)
}


void CLineDlg::InitIconImp() 
{//	UpdateData();
    CString str;
    GetDlgItem(IDC_CHECKPAUZA)->ShowWindow(SW_SHOW);
	switch(m_irazv)
	{
	  case IMP_POS:	case IMP_NEG: 
								  //длительность импульса
 								  GetDlgItem(IDC_EDITDLITIMP)->ShowWindow(SW_SHOW);
								  GetDlgItem(IDC_STATICIMP)->EnableWindow(true);
								  m_idlitimp = 5 - (m_iKolSourceU+bias_KolSourceU);
								  break;
	  case DC_POS:	case DC_NEG:
								  GetDlgItem(IDC_CHECKPAUZA)->ShowWindow(SW_HIDE);
								  //длительность импульса
								  GetDlgItem(IDC_STATICIMP)->EnableWindow(false);
								  GetDlgItem(IDC_EDITDLITIMP)->ShowWindow(SW_HIDE);
								  m_idlitimp = 0;	
								  break;
	  case SYN_POS:	case SYN_NEG:
								  //длительность импульса
								  if (m_bBazaVkl)
								  { GetDlgItem(IDC_STATICIMP)->EnableWindow(true);
 								    GetDlgItem(IDC_EDITDLITIMP)->ShowWindow(SW_SHOW);
								  }
								  else
								  {
								  GetDlgItem(IDC_STATICIMP)->EnableWindow(false);
								  GetDlgItem(IDC_EDITDLITIMP)->ShowWindow(SW_HIDE);
								  }
								  m_idlitimp = 0;
								  if (n_Switch==1 || n_Switch==2) m_idlitimp = 6;
								  break;
	  case AC:	
								  //длительность импульса
								  GetDlgItem(IDC_STATICIMP)->EnableWindow(false);
								  GetDlgItem(IDC_EDITDLITIMP)->ShowWindow(SW_HIDE);
								  m_idlitimp = 0;	
								  break;
	}


	//длительность импульса
    m_sDlitImp = mas_dlit_imp[m_idlitimp];

	//Скважность x 2 
	if (m_irazv==DC_POS || m_irazv==DC_NEG)	m_udlitpauza = 0;	//скважность х 2 выкл//---!
	else if (m_bCheckPauza) m_udlitpauza = 1;
	                   else m_udlitpauza = 0; 

	UpdateData(FALSE);
}


void CLineDlg::DefinitionDiapazon()
{	DefinitionDiapazonKol();
	DefinitionDiapazonBaza();
	DefinitionDiapazonDop();
}
void CLineDlg::InitTest() 
{   //режим включения узлов
	InitStatusKnot();
	InitGnezdo();
	//режим каналов
	InitStatusKol();
	InitStatusBaza();
	InitStatusDop();
	//наименования узлов, характеристик (коллектор, база, доп. источник, эмиттер)
	//начальная установка диапазонов 
	DefinitionDiapazon();
	InitDiapazonKol();
	InitDiapazonBaza();
	InitDiapazonDop();
	
}

void CLineDlg::SetTest() 
{ 
	SetGnezdo();
	//наименования узлов, характеристик (коллектор, база, доп. источник, эмиттер)
	//***************************************************************************
	SetStringsSourceMeas();
	//начальная установка диапазонов измерителя и ограничения по U и I
	//****************************************************************
	SetDiapazonKol();
	SetDiapazonBaza();
	SetDiapazonDop();
	ShowButtonLoopingCompensation();//16.04.2015
}

void CLineDlg::BuildCircuit() 
{//построение текущей схемы включения
 //**********************************
	CWnd* pwnd;
 
 if (m_nelement>=0)
 { 
	//всё стереть
    ShowControls(aBmpCircuit_Off,SIZEOF_ARRAY(aBmpCircuit_Off),FALSE);	

    //узпы
	//=====
 	//элемент
	//=======
	pwnd=GetDlgItem(bmpEl[m_nelement]);
	if (pwnd)	{ pwnd->MoveWindow(&RectBitElement);
				  pwnd->ShowWindow(SW_SHOW);
				}
	//схема включения
	//===============
	//коллектор
	//--------
	pwnd=GetDlgItem(bmpRejKnot[COLLECTOR][m_iCircuitKol+1]);
	if (pwnd)	{ pwnd->MoveWindow(&RectBitKol);
				  pwnd->ShowWindow(SW_SHOW);
				}
	//база
	//----
	switch(m_nelement)
	{ //case 0: case 1: case 2: case 3: case 6: case 7: //не NMOS и не PMOS
	case DIOD: case RES: case NPN: case PNP: case NPN4: case PNP4: case TS://не NMOS и не PMOS
				pwnd=GetDlgItem(bmpRejKnot[BAZA][m_iCircuitBaza+1]);
				if (pwnd)	{ pwnd->MoveWindow(&RectBitBaza);
								pwnd->ShowWindow(SW_SHOW);
							}
				break;
//	  case 4: case 5: case 8: case 9:	//для NMOS и PMOS
	  case NMOS: case PMOS: case NMOS4: case PMOS4:	//для NMOS и PMOS
				pwnd=GetDlgItem(bmpRejKnot[GATE][m_iCircuitBaza+1]);
				if (pwnd)	{ pwnd->MoveWindow(&RectBitBaza);
							  pwnd->ShowWindow(SW_SHOW);
							}
				break;
	}
	//доп.канал
	//----------
	pwnd=GetDlgItem(bmpRejKnot[DOP][m_iCircuitDop+1]);
	if (pwnd)	{ pwnd->MoveWindow(&RectBitDop);
				  pwnd->ShowWindow(SW_SHOW);
				}
	
	//эмиттер
	//--------
	if (m_iKommutator==0)
	{
	pwnd=GetDlgItem(bmpRejKnot[EMITTER][1]);//m_iCircuitEm+1]);
	if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
				  pwnd->ShowWindow(SW_SHOW);
				}
	}
	else
	{
	pwnd=GetDlgItem(bmpRejKnot[EMITTER][0]);
	if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
				  pwnd->ShowWindow(SW_SHOW);
				}

	}
	//уточнение схемы
	//===============
	switch (m_iCircuitBaza)
	{  case 2:	//(база-коллектор)
			    //изменить режим колектора
				pwnd=GetDlgItem(bmpRejKnot[COLLECTOR][m_iCircuitKol+1]);
				if (pwnd)	{ //pwnd->MoveWindow(&RectBitKol);
							  pwnd->ShowWindow(SW_HIDE);
							}
				if (m_iCircuitKol==1)
				{//база-коллектор
				pwnd=GetDlgItem(bmpRejKnot[COLLECTOR][2+1]);
				if (pwnd)	{ pwnd->MoveWindow(&RectBitKol);
						      pwnd->ShowWindow(SW_SHOW);
							}
				}
				else
				{//база-коллектор_off
				pwnd=GetDlgItem(bmpRejKnot[COLLECTOR][2+2]);
				if (pwnd)	{ pwnd->MoveWindow(&RectBitKol);
						      pwnd->ShowWindow(SW_SHOW);
							}
				}
				if (m_iCircuitDop==2)
							{ //(база-) + (доп.-эмиттер)
							  pwnd=GetDlgItem(bmpRejKnot[EMITTER][3]);//3
							  if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
										  pwnd->ShowWindow(SW_SHOW);
										}
							}
				break;
	  case 3:	//(база-эмиттер)
				//изменить режим эмиттера
				pwnd=GetDlgItem(bmpRejKnot[EMITTER][m_iCircuitEm+1]);
				if (pwnd)	{ //pwnd->MoveWindow(&RectBitEm);
							  pwnd->ShowWindow(SW_HIDE);
							}
				if (m_iCircuitDop==2)
							{ //(база-эмиттер) + (доп.-эмиттер)
							  pwnd=GetDlgItem(bmpRejKnot[EMITTER][3+1]);
							  if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
										  pwnd->ShowWindow(SW_SHOW);
										}
							}
				else		{ pwnd=GetDlgItem(bmpRejKnot[EMITTER][1+1]);
							  if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
										  pwnd->ShowWindow(SW_SHOW);
										}
							}
				break;
	  case 4:	//(база-эмиттер 1 kOm)
				//изменить режим эмиттера
				pwnd=GetDlgItem(bmpRejKnot[EMITTER][m_iCircuitEm+1]);
				if (pwnd)	{ //pwnd->MoveWindow(&RectBitEm);
							  pwnd->ShowWindow(SW_HIDE);
							}
				if (m_iCircuitDop==2)
							{ //(база-эмиттер) + (доп.-эмиттер)
							  pwnd=GetDlgItem(bmpRejKnot[EMITTER][5+1]);
							  if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
										  pwnd->ShowWindow(SW_SHOW);
										}
							}
				else		{ pwnd=GetDlgItem(bmpRejKnot[EMITTER][4+1]);
							  if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
										  pwnd->ShowWindow(SW_SHOW);
										}
							}
				break;

	}
	switch(m_iCircuitDop)
	{ 
	
	
	case 2: //доп.-эмиттер
				//изменить режим эмиттера
				pwnd=GetDlgItem(bmpRejKnot[EMITTER][m_iCircuitEm+1]);
				if (pwnd)	{ //pwnd->MoveWindow(&RectBitEm);
							  pwnd->ShowWindow(SW_HIDE);
							}
			  switch (m_iCircuitBaza)
			  { case 0: case 1: case 2:
						pwnd=GetDlgItem(bmpRejKnot[EMITTER][3]);//2+1
						if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
									  pwnd->ShowWindow(SW_SHOW);
									}
						break;
				case 3: pwnd=GetDlgItem(bmpRejKnot[EMITTER][3+1]);//3+1
						if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
									  pwnd->ShowWindow(SW_SHOW);
									}
					    break;
				case 4: pwnd=GetDlgItem(bmpRejKnot[EMITTER][5+1]);//5+1
						if (pwnd)	{ pwnd->MoveWindow(&RectBitEm);
									  pwnd->ShowWindow(SW_SHOW);
									} 
					    break;
			  }
		      break;
	}
	UpdateWindow();
 }
 else
	SetTextInfo(_T("Выберите элемент и уточните способ подсоединения узлов!"));

}

void CLineDlg::InitStatusKnot() 
{//инициализация состояния узлов (в зависимости от типа элемента)
 //**************************************************************
  UpdateData();
  switch(m_nelement)
  { case -1: 
			 m_iCircuitKol = -1;
			 m_iCircuitBaza = -1;
			 m_iCircuitDop = -1;
			 break;
	case DIOD:	 //0
	case RES:	 //1		//двухполюсники
	         DisableControls(aPage0_bipolar2,SIZEOF_ARRAY(aPage0_bipolar2),FALSE);
			 m_iCircuitKol = 1;
			 m_iCircuitBaza = -1;
			 m_iCircuitDop = -1;
			 break;
	case NPN:	 //2
	case PNP:	 //3		//трёхполюсники
	case NMOS:	 //4
	case PMOS:	 //5		//трёхполюсники
			 DisableControls(aPage0_bipolar3,SIZEOF_ARRAY(aPage0_bipolar3),FALSE);
			 m_iCircuitKol = 1;
			 m_iCircuitBaza = 1;//0;
			 m_iCircuitDop = -1;
			 break;
	case NPN4:	 //6
	case PNP4:	 //7		//четырёхполюсники
	case NMOS4:	 //8
	case PMOS4:	 //9	//четырёхполюсники
	case TS:	 //10
			 m_iCircuitKol = 1;
			 m_iCircuitBaza = 1;
			 m_iCircuitDop = 1;
			 break;
  }
 UpdateData(FALSE);
}


void CLineDlg::SetListTest() 
{//построение списка стандартных тестов
 //************************************
    CComboBox *pct = (CComboBox*) GetDlgItem(IDC_COMBOMOD);
	pct->ResetContent();

	if (pModSelected==NULL)
	{
	  BuildArrayTstStandart();
	  int ind = -1;
      int n_tst = array_tst_standart.GetSize();
      if(m_nelement==-1) pct -> SetCurSel(ind);
      else
	  {
	    pct->AddString(_T("_USER"));
	    for(int i=0;i<n_tst;i++)
		{ CString str = array_tst_standart.GetAt(i);		  
	      pct->AddString(str);
		}
        ind = pct->FindStringExact(0,m_MOD);
	    if (ind>=0) pct -> SetCurSel(ind);
	  }
	}
//  UpdateData(FALSE);	//убрала 29.11.17
}


void CLineDlg::SetTextInfo(LPCTSTR strMessage)//,LPCTSTR strFile,LPCTSTR strTest)
{ 
//	if (!nCheckParam)
	{
    ((CStatic*)GetDlgItem(IDC_STATICEDINFO))->SetFont(&fontMessage);
    CEdit* pEd = (CEdit*)GetDlgItem(IDC_STATICEDINFO);
//	pEd->MoveWindow(&RectEdInfo);
    CString str;
	str.Format(_T("%s\n%s"),strMessage,str_PRIM);//
//	pEd -> SetWindowText(strMessage);
	pEd -> SetWindowText(str);

/*    pEd = (CEdit*)GetDlgItem(IDC_EDFILE);
	pEd->MoveWindow(&RectEdFile);
    pEd -> SetWindowText(strFileName);
*/
/*    pEd = (CEdit*)GetDlgItem(IDC_EDMODEL);
	pEd->MoveWindow(&RectEdModel);
    pEd -> SetWindowText(strTestName);
*/	}
}

//void CLineDlg::SubGetAxes(const double& Min_data,const double& Max_data,double* Min_axes,double* Max_axes,double* Min_tic_axes,double* Maj_tic_axes)
void CLineDlg::SubGetAxes(const double& Min_data,const double& Max_data,int* m_iS,int* expo)
{ //вызов: SubGetAxes(X_min,X_max,&m_iX,&expo_X); или  SubGetAxes(Y_min,Y_max,&m_iY,&expo_Y);
   double data_min = 0.0;
   double data_max = 0.0;
   double data     = 0.0;
   *expo     = 0;
	int sign=1;
   switch(m_iButNull)
   { case LEFT:    data_max = Max_data;
				   data = data_max;
				   if (data<0.0) sign=-1;
				   break;
     case CENTER:  data_min = Min_data;
				   data_max = Max_data;			
				   data = data_max;
				   if (data<0.0) sign=-1;
				   break;
     case RIGHT:   data_min = Min_data;
				   if (data_min<0.0) sign=-1;
				   data = fabs(data_min);
				   break;
   }	
   //нормализуем data
  if (data)
  {
   if (data<1.0)
   { do { data = data*10.0;
	      *expo = *expo - 1;
		} while (data<1.0);
	}
   if (data>=10.0)
   { do { data = data/10.0;
	      *expo = *expo + 1;
		} while (data>=10.0);
   }
   //определение вида развёртки
//   CString str;
//   str.Format(_T("%f"),data);
//   int mant=atoi((const char*)str.GetBuffer(str.GetLength()+1));//мантисса
//   int mant = (int)data;
  // if (fabs(data - mant)>0.0) mant++;
	//mant_min=(int)(fabs(floor(min_d*sign_min)));//округляем мин в меньшую сторону
	//mant_max=(int)(fabs(ceil(max_d*sign_max))); //округляем мах в большую сторону

   int mant;
   if (sign>0) mant =(int)(fabs (ceil(data*sign)));
				else mant =(int)(fabs(floor(data*sign)));
   switch (mant)
   { case 1:   //развёртка по оси "1" ед.
				*m_iS = 0;
				break;
     case 2:  	//развёртка по оси "2" ед.
				*m_iS = 1;
		        break;
	 case 3:case 4: case 5:    //развёртка по оси "5" ед.
				*m_iS = 2;
		        break;
	 case 6: case 7: case 8: case 9: case 10:
		        *m_iS = 0;
				*expo = *expo+1;
				break;
   }
  }
}

void CLineDlg::DrawSetka()
{	//сетка
}

int CLineDlg::SubGetStrRazmer(int xy,int expo,const CString& sStr)
{ //xy: 0 - V, 1 - A, 2 - Om - режим измерения (0-U,1-I,2-R,3-Beta,4-Betai,5-S);
  //expo - показатель степени
  //sStr - наименование измеряемой характериcтики
  //strRazmer - искомая строка с размерностью
  //возвращает: p - порядок
 
  strRazmer   = _T("");
  
  int c = expo / 3;	//порядок
  int p = expo % 3;	//остаток
  CString prefix = _T("");
  CString ed_izm = _T("");
  if (expo>=0)
	switch(c)
	{ case 0:		prefix = _T("");  break;	
	  case 1:		prefix = _T("k"); break;	
	  case 2:		prefix = _T("M"); break;	
	  case 3:		prefix = _T("G"); break;
	  case 4:		prefix = _T("T"); break;
	  case 5:		prefix = _T("P"); break;
	  default:      prefix = _T("");  break;//_T("");  break;
	}
  else
  {
   if (p==0)
	switch(c)
	{ case 0:	prefix = _T(""); break;	
	  case -1:	prefix = _T("m"); break;//_T("m"); break;
	  case -2:	prefix = _T("u"); break;//_T("u"); break;
	  case -3:	prefix = _T("n"); break;//_T("n"); break;
	  case -4:	prefix = _T("p"); break;//_T("p"); break;
	  case -5:	prefix = _T("f"); break;//_T("p"); break;
	  case -6:	prefix = _T("a"); break;//_T("p"); break;
	  default:  prefix = _T(""); break;//_T("");  break;
	}
    else
	{ p=p+3;
	  switch(c)
	  { case 0:		prefix = _T("m"); break;	
	    case -1:	prefix = _T("u"); break;//_T("m"); break;
	    case -2:	prefix = _T("n"); break;//_T("u"); break;
	    case -3:	prefix = _T("p"); break;//_T("n"); break;
	    case -4:	prefix = _T("f"); break;//_T("p"); break;
	    case -5:	prefix = _T("a"); break;//_T("p"); break;
	    default:    prefix = _T("");  break;//_T("");  break;
	  }
	}
 }
  switch (xy)
  { case 0:		ed_izm = _T("V"); break;
    case 1:		ed_izm = _T("A"); break;
	case 2:		ed_izm = _T("Om");break;
	case 3:		ed_izm = _T("");break;
	case 4:		ed_izm = _T("");break;
	case 5:		ed_izm = _T("A/V");break;
	default:    ed_izm = _T("");  break;
  }
///  if (p==0) strRazmer.Format(_T("%s, %s%s"),sStr,prefix,ed_izm);
///       else strRazmer.Format(_T("%s, %s%s [E %+03d]"),sStr,prefix,ed_izm,p);
  
  strRazmer.Format(_T("%s, %s%s"),sStr,prefix,ed_izm);
  return p;
}


void CLineDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
//	HeapFree(GetProcessHeap(),0,m_ColorTable);
//	HeapFree(GetProcessHeap(),0,pX);
//	HeapFree(GetProcessHeap(),0,pY);

    while (m_modList.GetHeadPosition()) {
      delete m_modList.RemoveHead();
    }


    m_fontMin.DeleteObject();   
 	m_fontMax.DeleteObject();   

    brFon.DeleteObject();
//	brFonInfo.DeleteObject();

	m_brushBlack.DeleteObject();
	m_brushGray.DeleteObject();
	m_brushGrayLight.DeleteObject();
	m_brushWhite.DeleteObject();
	m_brushBlue.DeleteObject();
	m_brushYellowLight.DeleteObject();
	m_brushGreenLight.DeleteObject();
	
	m_penSetka_DASH.DeleteObject();
	m_penSetka_SOLID.DeleteObject();
	m_penTangent.DeleteObject();
	m_penFon.DeleteObject();
	m_penCurve.DeleteObject();
	m_penRef.DeleteObject();

	if (pModBuf) delete pModBuf;

	CDialog::PostNcDestroy();
}

void CLineDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;

	//График
	//------
/*	rect=m_RectWndGraf;
  	if(dc.RectVisible(&rect)) 
	{ 
	 TRACE(_T("OnPaint(); m_RectWndGraf\n"));
//	 int er =ControlParam();//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	 if (!er)				//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	 {
	  DrawSetka();
	  if (n_Result) DrawGraph();
	  if (nCheckParam) DisplayParam();
	  if (nCheckRef) DisplayRef();

   
		 CPoint p(0,0);
      MemDC.DPtoLP(&p);		
      dc.BitBlt(rect.left,rect.top,
	              rect.Width(),rect.Height(),&MemDC,p.x,p.y,SRCCOPY);

//	  InitAxis();
	 }
	}
*/	
	if (!nCheckParam)
	{//Информационное окно
	 //-------------------
/*	rect=RectInfoTest;
	if(dc.RectVisible(&rect)) 
	{
	 TRACE(_T("OnPaint(); RectInfoTest\n"));
      CPoint p(0,0);
      InfoDC.DPtoLP(&p);		
      dc.BitBlt(rect.left,rect.top,
	              rect.Width(),rect.Height(),&InfoDC,p.x,p.y,SRCCOPY);
		}
*/
//отображение состояния физической рукоятки
//-----------------------------------------
/*    if(dc.RectVisible(&rect)) 
	{
      rect = RectZona;
	  TRACE(_T("OnPaint(); RectZona\n"));
		CPoint p(0,0);
      InfoRukoyatkaDC.DPtoLP(&p);		
      dc.BitBlt(rect.left,rect.top,
	              rect.Width(),rect.Height(),&InfoRukoyatkaDC,p.x,p.y,SRCCOPY);
//	  DrawStatusRukoyatka();
	}
*/	
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CLineDlg::DrawGraph() 
{
//	iX		  = 0;		//канал - аргумент-источник 1-ой очереди: 0-коллектор, 1-база, 2-доп.канал
//	iY		  = 0;		//канал - функция: 0-коллектор, 1-база, 2-доп.канал 
//	rej_X	  = 0;		//характеристика, отображаемая по оси X: 0 - U, 1 - I
//	rej_Y	  = 1;		//характеристика, отображаемая по оси Y: 0 - U, 1 - I
//	pM1[] - источник коллектора (Uc)	(ACP1)
//	pM2[] - измеритель коллектора (Ic)	(ACP2)
//	pM3[] - измеритель базы				(ACP3)
//	pM4[] - измеритель доп. канала		(ACP4)
//	pM5[] - расчётный массив			(ACP5)
}


void CLineDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int nYear,nMonth,nDay,nHour,nMin,nSec;
	CTime timeCurrent;

    CWnd* pwnd=GetDlgItem(IDC_STATICEDDATE);
	CWnd* pwnp=GetDlgItem(IDC_PROGRESS_PROCESS);

//    CWnd* pwnt=GetDlgItem(IDC_STATICEDTIME);
//    CWnd* psh=GetDlgItem(IDC_STATICHIGHU);
//    CWnd* psnh=GetDlgItem(IDC_STATICNOHIGHU);

//  if (nIDEvent==1)//индикатор текущего времени
  if (nIDEvent==6)//индикатор текущего времени
  {	  timeCurrent = CTime::GetCurrentTime();
	  nYear=timeCurrent.GetYear();
	  nMonth=timeCurrent.GetMonth();
	  nDay=timeCurrent.GetDay();
	  nHour=timeCurrent.GetHour();
	  nMin=timeCurrent.GetMinute();
	  nSec=timeCurrent.GetSecond();
      strDateCurrent.Format(_T("%.2d.%.2d.%.2d"),nDay,nMonth,nYear);// = timeCurrent.Format(_T("%d.%m.%Y"));
      strTimeCurrent.Format(_T("%.2d:%.2d:%.2d"),nHour,nMin,nSec);// = timeCurrent.Format(_T("%H:%M:%S"));
	  if (n_IPPP) pwnd  -> SetWindowText(_T("ИППП-3/1\r\n")+strDateCurrent+_T("\r\n")+strTimeCurrent);
			else  pwnd  -> SetWindowText(_T("ИППП-3\r\n")+strDateCurrent+_T("\r\n")+strTimeCurrent);
//      pwnd  -> SetWindowText(strDateCurrent+_T("\r\n")+strTimeCurrent);
//	  pwnd  -> SetWindowText(_T("Анализатор ВАХ\r\n")+strDateCurrent+_T("\r\n")+strTimeCurrent);
      if (m_bModifiedMemory)
	  { 
	    WhatFreeMemory();	//индикатор памяти
		if (free_space>=90)
		{  m_bSignalMemoryFull =! m_bSignalMemoryFull;
		  switch(m_bSignalMemoryFull)
			{ case 0: ShowControls(aNoSignalMemoryFull,SIZEOF_ARRAY(aNoSignalMemoryFull),TRUE);
	   				  ShowControls(aSignalMemoryFull,SIZEOF_ARRAY(aSignalMemoryFull),FALSE); break;
			  case 1: ShowControls(aSignalMemoryFull,SIZEOF_ARRAY(aSignalMemoryFull),TRUE);
					  ShowControls(aNoSignalMemoryFull,SIZEOF_ARRAY(aNoSignalMemoryFull),FALSE); 
					  MyMessageBeep(MB_ICONASTERISK);
					  break;
			}
		}
		else
		{ m_bModifiedMemory = false;
		  m_bSignalMemoryFull = false;
		  ShowControls(aNoSignalMemoryFull,SIZEOF_ARRAY(aNoSignalMemoryFull),TRUE);
	   	  ShowControls(aSignalMemoryFull,SIZEOF_ARRAY(aSignalMemoryFull),FALSE);
		}
	  }
  }

	if (nIDEvent==2)//индикатор процесса
	{   if (bAutoSetNull)
		{ 
		  if (pwnp)	pwnp->ShowWindow(SW_SHOW);
		  m_Progress.StepIt();
	      m_ProgPos+=5;
	      if (m_ProgPos==100) 
		  { if (!bAutoSetNull) KillTimer(2);
		    m_ProgPos=0;
		    m_Progress.SetPos(0);
		  }
		}
		else
		{ KillTimer(2);
		  if (pwnp)	pwnp->ShowWindow(SW_HIDE);
		  m_ProgPos=0;
		  m_Progress.SetPos(0);
		}	
		
	}
	if (nIDEvent==4)//индикатор процесса
	{   if (bKalibr)
		{ 
		  if (pwnp)	pwnp->ShowWindow(SW_SHOW);
		  m_Progress.StepIt();
	      m_ProgPos+=5;
	      if (m_ProgPos==100) 
		  { if (!bKalibr) KillTimer(4);
		    m_ProgPos=0;
		    m_Progress.SetPos(0);
		  }
		}
		else
		{ KillTimer(4);
		  if (pwnp)	pwnp->ShowWindow(SW_HIDE);
		  m_ProgPos=0;
		  m_Progress.SetPos(0);
		}	
		
	}

  if (nIDEvent==3)//мигание светодиода "Высокое напряжение"
  { //if (!nCheckParam)
	{   if (bSignal)
		{ m_bSignal = !m_bSignal;
		  switch(m_bSignal)
			{ case 0: ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),TRUE);
	   				  ShowControls(aSignal,SIZEOF_ARRAY(aSignal),FALSE); break;
			  case 1: ShowControls(aSignal,SIZEOF_ARRAY(aSignal),TRUE);
					  ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),FALSE); 
					  MyMessageBeep(MB_ICONASTERISK);
					  break;
			}
		}
		else 
		{ m_bSignal = FALSE;
		  ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),TRUE);
	      ShowControls(aSignal,SIZEOF_ARRAY(aSignal),FALSE);
		}
	}
  }

  CDialog::OnTimer(nIDEvent);
}
  

HBRUSH CLineDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	//Зона информации
	//---------------
	if (pWnd -> GetDlgCtrlID() == IDC_STATICEDDATE) 
	{	hbr = m_brushGray;//hbr = m_brushGray;//Light;//hbr;//Light;//m_brushBlack;
		pDC->SetBkColor(RGB(203,203,203));//241,241,241));//192,192,192));//234,234,234));//213,213,213));//111,111,111));//0,0,0));
	//	pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));//145,72,72));//165,0,33));//
	}
	if  (pWnd -> GetDlgCtrlID() == IDC_STATICEDINFO)
	{	hbr = m_brushGray;//m_brushBlack;
		pDC->SetBkColor(RGB(203,203,203));//111,111,111));//0,0,0));//134,134,134));
		pDC->SetTextColor(RGB(145,72,72));//0,64,128));
	}
	//Установка параметров
	//=====================
	//Закладка "Схема включения ПП"
	//-----------------------------
	if (pWnd -> GetDlgCtrlID() == IDC_STATICTEXTPAGE0)
	{
		hbr = m_brushGrayLight;//White;//Black;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//255,255,255));//213,213,213));//111,111,111));//134,134,134));//0,0,0));
		pDC->SetTextColor(RGB(0,0,0));
	}
/*	if (pWnd -> GetDlgCtrlID() == IDC_TEXTCOMBOCHECKEM)
	{//	hbr = hbr;
		hbr = m_brushGrayLight;//Black;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//111,111,111));//134,134,134));//0,0,0));
		pDC->SetTextColor(RGB(0,0,0));
	}

	if ((pWnd -> GetDlgCtrlID() == IDC_STATIC_GNEZDO_S)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTCOMBOCHECKEM))
	{//	hbr = hbr;
		hbr = m_brushWhite;
		pDC->SetBkColor(RGB(255,255,255));//234,234,234));//213,213,213));//111,111,111));//134,134,134));//0,0,0));
		pDC->SetTextColor(RGB(0,0,0));
	}
*/
/*	if ((pWnd -> GetDlgCtrlID() == IDC_EDITKNOTKOL)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITKNOTBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITKNOTDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITKNOTEM))
	{//	hbr = hbr;
		hbr = m_brushGrayLight;//Black;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//111,111,111));//134,134,134));//0,0,0));
		pDC->SetTextColor(RGB(165,0,33));//119,0,0));
	}
*/	//Закладки "Коллектор", "База", "Доп. канал"
	//-------------------------------------------
/*	if ((pWnd -> GetDlgCtrlID() == IDC_EDITLIMITKOLI)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITLIMITKOLU)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITBAZABIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITLIMITBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITDOPBIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITLIMITDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITSTEPBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITSTEPDOP))
	{	hbr = m_brushBlack;
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(0,255,0));
	}
*/	if	((pWnd -> GetDlgCtrlID() == IDC_EDITDLITIMP)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITRANGBAZABIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITRANGDOPBIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_CHECKPAUZA)||
		(pWnd -> GetDlgCtrlID() == IDC_CHECK_INVERS_X)||
		(pWnd -> GetDlgCtrlID() == IDC_CHECK_INVERS_Y))
	{//	hbr = hbr;
		hbr = m_brushGrayLight;//Black;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//134,134,134));//0,0,0));
		pDC->SetTextColor(RGB(0,0,0));
	}
	if ((pWnd -> GetDlgCtrlID() == IDC_EDMEASBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_EDKOLMEASU)||		
		(pWnd -> GetDlgCtrlID() == IDC_EDKOLMEASI)||
		(pWnd -> GetDlgCtrlID() == IDC_EDBAZASOURCEUI)||
		(pWnd -> GetDlgCtrlID() == IDC_EDDOPSOURCEUI)||
		(pWnd -> GetDlgCtrlID() == IDC_EDSLIDERKOLU)||
//		(pWnd -> GetDlgCtrlID() == IDC_EDTEXTRANGMAXKOLI)||
		(pWnd -> GetDlgCtrlID() == IDC_EDMEASDOP))
	{//	hbr = hbr;
		hbr = m_brushWhite;//m_brushGrayLight;//GrayLight;//Black;
		pDC->SetBkColor(RGB(255,255,255));//241,241,241));//34,234,234));//255,255,255));//192,192,192));//134,134,134));//0,0,0));
//		pDC->SetTextColor(RGB(165,0,33));//0,255,0));
		pDC->SetTextColor(RGB(255,0,0));//0,255,0));
	}
	if	(pWnd -> GetDlgCtrlID() == IDC_ST_FREEMEMORY)
	{	hbr = hbr;
		pDC->SetTextColor(RGB(255,0,0));
	}
	if ((pWnd -> GetDlgCtrlID() == IDC_STATICLIMITBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITLIMITKOLI)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITLIMITKOLU)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITBAZABIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITLIMITBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITDOPBIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITLIMITDOP)||
//		(pWnd -> GetDlgCtrlID() == IDC_STATICLIMIT)||
//		(pWnd -> GetDlgCtrlID() == IDC_STATICKOLLECTORSOURCE)||//источники
		(pWnd -> GetDlgCtrlID() == IDC_STATICLIMITDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICRAZV)||//
		(pWnd -> GetDlgCtrlID() == IDC_STATICKOLLECTORSOURCE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICLIMIT)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICIMP))
//		(pWnd -> GetDlgCtrlID() == IDC_STATICDLITPAUZA))
//		(pWnd -> GetDlgCtrlID() == IDC_CHECKPAUZA))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));
	}
	if (
//		(pWnd -> GetDlgCtrlID() == IDC_STATICKOLLECTORSOURCE)||//источники
		(pWnd -> GetDlgCtrlID() == IDC_STATICSTOP)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICSTART)||
		(pWnd -> GetDlgCtrlID() == IDC_ZONACHARACTERISTIC)||
		(pWnd -> GetDlgCtrlID() == IDC_ZONAPARAM)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_FON))
//		(pWnd -> GetDlgCtrlID() == IDC_EDTEXTRANGMAXKOLI))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;//Black;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//0,0,0));
		pDC->SetTextColor(RGB(0,0,0));//255,0));
	}
	if	(//измерители
//		(pWnd -> GetDlgCtrlID() == IDC_EDSLIDERKOLI)||
//		(pWnd -> GetDlgCtrlID() == IDC_STATICKOLMEAS)||
		(pWnd -> GetDlgCtrlID() == IDC_SLIDERBAZABIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_SLIDERDOPBIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_SLIDERLIMITBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_SLIDERKOLU)||
		(pWnd -> GetDlgCtrlID() == IDC_SLIDERLIMITDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_SLIDERKOLI))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));
	}
	if	((pWnd -> GetDlgCtrlID() == IDC_TEXTAMPLITUDEBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_CHECK_BAZA_VKL)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTSTEPBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTPOLARITYSTEP)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTAMPLITUDEDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTSTEPDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTPOLARITYSTEPDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTPOLARITYBIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTPOLARITYBIASDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTRANGLIMITBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTRANGLIMITDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_CHECKBAZAREJHIGH))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));
	}
	if	(
//		(pWnd -> GetDlgCtrlID() == IDC_CHECKPAUZA)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIOBAZASOURCEU)||
//		(pWnd -> GetDlgCtrlID() == IDC_CHECKKOLREJHIGH)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIOBAZASOURCEI)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIODOPU)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIODOPI))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//203,203,203));//234,234,234));//213,213,213));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));
	}
	if ((pWnd -> GetDlgCtrlID() == IDC_TEXTRANGMINKOLU)||
//		(pWnd -> GetDlgCtrlID() == IDC_TEXTRANGMAXKOLU)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTRANGMINKOLI)||
//		(pWnd -> GetDlgCtrlID() == IDC_EDTEXTRANGMAXKOLI)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTMINBAZABIAS)||
//		(pWnd -> GetDlgCtrlID() == IDC_TEXTMAXBAZABIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTMINDOPBIAS)||
//		(pWnd -> GetDlgCtrlID() == IDC_TEXTMAXDOPBIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTMINLIMITBAZA)||
//		(pWnd -> GetDlgCtrlID() == IDC_TEXTMAXLIMITBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTMINLIMITDOP)||
//		(pWnd -> GetDlgCtrlID() == IDC_TEXTMAXLIMITDOP)||
//		(pWnd -> GetDlgCtrlID() == IDC_STATICTIPIMPNEG)||
//		(pWnd -> GetDlgCtrlID() == IDC_STATICTIPIMP1NEG)||
//		(pWnd -> GetDlgCtrlID() == IDC_STATICTIPIMP)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_VKL)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_ICON_INVERS)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_ICON_LUPA)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_ICON_LOOPING)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_ICON_FAMILY)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_TEXT_INVERSY))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));
	}
	if ((pWnd -> GetDlgCtrlID() == IDC_RADIO_RAZVIMPPOSITIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_RAZVIMPNEGATIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_RAZVDCPOSITIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_RAZVDCNEGATIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_RAZVAC)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_SYNPOSITIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO_SYNNEGATIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICIMPPOSITIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICIMPNEGATIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICICONDCPOS)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICICONDCNEGATIVE)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICIMP1POS)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICIMP1NEG)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICAC)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICBAZA)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICDOP)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICBAZABIAS)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICDOPBIAS)||
//		(pWnd -> GetDlgCtrlID() == IDC_RADIO_LEFT)||
//		(pWnd -> GetDlgCtrlID() == IDC_RADIO_CENTER)||
//		(pWnd -> GetDlgCtrlID() == IDC_RADIO_RIGHT)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_NULL_LEFT)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_NULL_CENTER)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_NULL_RIGHT))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));
	}
	//Закладка "Расчёт"
	//-----------------
/*	if ((pWnd -> GetDlgCtrlID() == IDC_AXISY)||
		(pWnd -> GetDlgCtrlID() == IDC_AXISX)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITNAMEPAR)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITNAMEARG)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITUR1)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITUR2)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITPOINT)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITMIN)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITMAX))
	{	hbr = m_brushBlack;
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(0,255,0));
	}
*/	if ((pWnd -> GetDlgCtrlID() == IDC_TEXTFIND)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTFOR)||
		(pWnd -> GetDlgCtrlID() == IDC_FORMULA)||
		(pWnd -> GetDlgCtrlID() == IDC_ENDFORMULA)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXT)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTUR1)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTUR2)||
//		(pWnd -> GetDlgCtrlID() == IDC_TEXTPOINT)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO1)||
		(pWnd -> GetDlgCtrlID() == IDC_ICONRADIO1)||
		(pWnd -> GetDlgCtrlID() == IDC_RADIO2)||
/*		(pWnd -> GetDlgCtrlID() == IDC_STATIC_MARKER1)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_MARKER1_ON)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_MARKER2)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_MARKER2_ON)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_BUTTON_TANGENT)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_BUTTON_TANGENT_ON)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_NAME_MARKER1)||
		(pWnd -> GetDlgCtrlID() == IDC_STATIC_NAME_MARKER2)||
*/		(pWnd -> GetDlgCtrlID() == IDC_ZONAFORMULA)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICPOROG)||
		(pWnd -> GetDlgCtrlID() == IDC_EDITPOINT)||
		(pWnd -> GetDlgCtrlID() == IDC_ICONRADIO2))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//111,111,111));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));
	}
/*	if	
	{	//hbr = hbr;
		hbr = m_brushGray;
		pDC->SetBkColor(RGB(111,111,111));//134,134,134));
		pDC->SetTextColor(RGB(0,255,255));
	}
	if	
	{	//hbr = hbr;
		hbr = m_brushGray;
		pDC->SetBkColor(RGB(111,111,111));//134,134,134));
		pDC->SetTextColor(RGB(119,0,0));
	}

	//Зона отображения положения рукоядки
	//====================================
	if ((pWnd -> GetDlgCtrlID() == IDC_STATICZONATEXT)||
		(pWnd -> GetDlgCtrlID() ==	IDC_STATIC_NAMEFILE))
	{	hbr = m_brushGrayLight;//Black;
		pDC->SetBkColor(RGB(192,192,192));//0,0,0));
		pDC->SetTextColor(RGB(119,0,0));//56,220,233));
	}
*/
	if (pWnd -> GetDlgCtrlID() == IDC_STATICZONATEXT)
	{//	hbr = m_brushGray;//Light;//Black;
//		pDC->SetBkColor(RGB(203,203,203));//234,234,234));//192,192,192));//0,0,0));
		pDC->SetTextColor(RGB(255,0,0));//56,220,233));
	}
	if	(pWnd -> GetDlgCtrlID() ==	IDC_STATIC_NAMEFILE)
	{//	hbr = m_brushGrayLight;//Black;
//		pDC->SetBkColor(RGB(234,234,234));//192,192,192));//0,0,0));
		pDC->SetTextColor(RGB(255,0,0));//56,220,233));
	}
	//Закладка "Дисплей"
	//==================
/*	if (pWnd -> GetDlgCtrlID() == IDC_CHECK_LOOPING)//IDC_STATIC_TEXT_MARKER2)
	{	//hbr = hbr;
		hbr = m_brushGray;
		pDC->SetBkColor(RGB(111,111,111));//134,134,134));
		pDC->SetTextColor(RGB(0,0,0));//255));
	}
*/	
//	(pWnd -> GetDlgCtrlID() == IDC_STATIC_TEXT_TANGENT)||
	if	((pWnd -> GetDlgCtrlID() == IDC_EDTEXTRANGMAXKOLI)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTMIN)||
		(pWnd -> GetDlgCtrlID() == IDC_TEXTMAX))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//111,111,111));//134,134,134));
		pDC->SetTextColor(RGB(119,0,0));
	}
	if ((pWnd -> GetDlgCtrlID() == IDC_MARKER1)||
		(pWnd -> GetDlgCtrlID() == IDC_MARKER2)||
		(pWnd -> GetDlgCtrlID() == IDC_TANGENT))
	{	//hbr = hbr;
		hbr = m_brushGrayLight;//White;////Black;//Gray;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//255,255,255));//13,213,213));//,111,111));//134,134,134));//
		pDC->SetTextColor(RGB(0,0,0));//230,240,96));
	}

	//Закладка "Ошибки"
	//==================
	if	(pWnd -> GetDlgCtrlID() == IDC_EDITERRORS)
	{
		hbr = m_brushYellowLight;
		pDC->SetBkColor(RGB(255,255,81));
		pDC->SetTextColor(RGB(0,0,0));
	}
	//Окно "Отчёт"
	//==================
	if (pWnd -> GetDlgCtrlID() == IDC_EDITOTCHT)
	{
		hbr = m_brushWhite;//m_brushGreenLight;
		pDC->SetBkColor(RGB(255,255,255));//128,255,128));
		pDC->SetTextColor(RGB(0,0,0));
	}

    //отображение параметров
	if ((pWnd -> GetDlgCtrlID() == IDC_STATIC_ELEMENT)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICKNOT)||
		(pWnd -> GetDlgCtrlID() == IDC_STATICMOD))
	{	hbr = m_brushGrayLight;//Black;
		pDC->SetBkColor(RGB(241,241,241));//234,234,234));//213,213,213));//111,111,111));//0,0,0));//128,128,128));//
		pDC->SetTextColor(RGB(0,0,0));
	}
/*	if (pWnd -> GetDlgCtrlID() == IDC_EDITDISPLAYMEAS)
	{	hbr = m_brushWhite;//Gray;
		pDC->SetBkColor(RGB(255,255,255));//111,111,111));//128,128,128));//
		pDC->SetTextColor(RGB(0,0,0));
	}
	if ((pWnd -> GetDlgCtrlID() == IDC_EDITDISPLAYREF)||
	   (pWnd -> GetDlgCtrlID() == IDC_EDITDISPLAYMEAS))
	{	hbr = m_brushWhite;//Gray;
//		pDC->SetBkColor(RGB(228,228,228));//
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
	}
*/	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

/*
void CLineDlg::OnViewTabl() 
{
	// TODO: Add your control notification handler code here
	bSignal=!bSignal;
	switch(bSignal)
	{ case 0: ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),TRUE);
			  ShowControls(aSignal,SIZEOF_ARRAY(aSignal),FALSE); break;
	  case 1: ShowControls(aSignal,SIZEOF_ARRAY(aSignal),TRUE);
			  ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),FALSE); break;
	}
}
*/
void CLineDlg::ShowControls(UINT* pControls,UINT cControls,BOOL fVisible)
{ for (UINT uIndex=0;uIndex<cControls;uIndex++)
	{ CWnd* pwnd=GetDlgItem(pControls[uIndex]);
	  if (pwnd)
	  { pwnd->ShowWindow(fVisible? SW_SHOW:SW_HIDE);
	    pwnd->EnableWindow(fVisible);
	  }
	}
}

void CLineDlg::DisableControls(UINT* pControls,UINT cControls,BOOL fVisible)
{ for (UINT uIndex=0;uIndex<cControls;uIndex++)
	{ CWnd* pwnd=GetDlgItem(pControls[uIndex]);
	  if (pwnd)
	  { //pwnd->ShowWindow(fVisible? SW_SHOW:SW_HIDE);
	    pwnd->EnableWindow(fVisible);
	  }
	}
}


void CLineDlg::OnButtonFile() 
{
	// TODO: Add your control notification handler code here
		
	  akt_button = 100;	
	  CMenu menu;
	  menu.LoadMenu(IDR_MENUFLOATINGFILE);
	  ASSERT(menu);
	  CPoint point;
	  point.x=RectWnd.left+10;
	  point.y=RectWnd.bottom-200;
/*	  if (!m_bModifiedFlag)
	  {//нет изменений - нечего сохранять
	    menu.GetSubMenu(0)->EnableMenuItem(IDR_FILE_SAVE,MF_GRAYED);
	    menu.GetSubMenu(0)->EnableMenuItem(IDR_FILE_SAVE_AS,MF_GRAYED);
	  }
*/	  if (!m_bFileOpen)
	  { menu.GetSubMenu(0)->EnableMenuItem(IDR_FILE_CLOSE,MF_GRAYED);//нет открытого файла - нечего закрывать
	    //нет открытого файла - нечего сохранять
	    menu.GetSubMenu(0)->EnableMenuItem(IDR_FILE_SAVE,MF_GRAYED);
	    menu.GetSubMenu(0)->EnableMenuItem(IDR_FILE_SAVE_AS,MF_GRAYED);
	  }
	  else
	  { //menu.GetSubMenu(0)->EnableMenuItem(IDC_FILE_DELETE,MF_GRAYED);
		menu.GetSubMenu(0)->EnableMenuItem(IDR_FILE_NEW,MF_GRAYED);
		menu.GetSubMenu(0)->EnableMenuItem(IDR_FILE_OPEN,MF_GRAYED);
	  }

	  	  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	
}

void CLineDlg::OnFileNew() 
{
	// TODO: Add your command handler code here
	if(m_bFileOpen) FileClose();
	else FileNew();
}

void CLineDlg::FileNew()
{ 
	  strFileName			= strFileUntitled;
	  m_bModifiedFlag		= FALSE;
	  m_bZaschitaParol	    = FALSE;	//защита паролем файла *.mod: 0-нет, 1-да
	  TreeUpdate();
	  TreeOpen();			//открываем окно просмотра дерева тестов
	  m_bFileOpen			= TRUE;
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
  pModSelected = NULL;
  i_EL			= 0;
  i_MOD			= 0;
  i_SelUrov		= 0;
//  m_EL			= _T("");
//  m_MOD			= _T("");
  str_EL		= _T("");
  str_MOD		= _T("");
  m_sNameFileMod= _T("Untitled.mod");
  UpdateData(false);
}

void CLineDlg::OnFileOpen() 
{
	// TODO: Add your command handler code here
	if (m_bFileOpen) { FileClose();FileOpen();}
	else FileOpen();
}

void CLineDlg::FileOpen()
{
	BOOL flo = FALSE;	//признак: 1 - файл открыт, 0 - нет
	CFile myFile;
	CFileException er;
	CString str;

	CFileDlg dlg;
	dlg.m_sTitle = _T("Открыть файл модели измерения");
	dlg.m_iWhatDo = FILE_LOAD;
	dlg.m_sFilter =_T("Line Files (\\Hard Disk\\Mod\\*.mod)");
	dlg.m_sCatalog =_T("\\Hard Disk\\Mod\\*.mod");
	dlg.m_sPath = _T("\\Hard Disk\\Mod\\");
	dlg.m_sName = _T("*.mod");
	dlg.m_itip = FILE_MOD;
	dlg.m_sFullNameFile = _T("");
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
	  strFileName = dlg.m_sFullNameFile;
/*	}
	
	CFileDialog dlgo(TRUE,_T("mod"),_T("\\Disk\\Mod\\*.mod"));
	dlgo.m_ofn.lpstrTitle=_T("Открыть");
	dlgo.m_ofn.lpstrFilter=_T("Line Files (\\Disk\\Mod\\*.mod)");
	if (dlgo.DoModal()==IDOK)
	{ strFileName = dlgo.GetPathName();
*/	  //открыть документ
	  if (!myFile.Open(strFileName,CFile::modeRead,&er)) 
	  { //TCHAR szError[1024];
        //ex.GetErrorMessage(szError, 1024);
	    MessageBox(_T("Невозможно открыть файл модели измерения %s!"),NULL,MB_OK|MB_ICONERROR);
	    flo = FALSE;
	  }
	  else
	  { CArchive arj(&myFile,CArchive::load);
	    Serialize(arj);
	    myFile.Close();
	    arj.Close();
	    flo = TRUE;
		TreeUpdate();
	    TreeOpen();		//открываем окно просмотра дерева тестов
//		m_sNameFileMod = dlgo.GetFileName();
		m_sNameFileMod = dlg.m_sName;
		UpdateData(false);
			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
	  }
	}//end of IDOK
	else flo = FALSE;	//нажали отмену при запросе пути для открытия документа

	m_bFileOpen = flo;	//TRUE - if отрытие прошло успешно, else - FALSE

}

void CLineDlg::OnFileSave() 
{
	// TODO: Add your command handler code here
	if (m_bFileOpen) FileSave();
}

BOOL CLineDlg::FileSave()
{  
	BOOL fl = FALSE;
	CParolDlg dlgp;
	if (m_bZaschitaParol==TRUE)
	{ //файл защищён паролем
    	dlgp.what_save = FILE_MOD;
    	UpdateData(false);
		if (dlgp.DoModal()==IDOK)
		{ UpdateData();
			if (m_sparol == dlgp.m_sparol) 
			{ //пароль верный
			  fl = FileWrite();
			}
		  else MessageBox(_T("Неверный пароль! Файл не сохранён!"),NULL,MB_OK|MB_ICONERROR);
		}
		else MessageBox(_T("Пароль не введен! Файл не сохранён!"),NULL,MB_OK|MB_ICONEXCLAMATION);
	}
	else 
	{ //файл не защищён паролем
	  fl = FileWrite();
	}
	
	if (fl) m_bModifiedFlag = FALSE;	//изменения сохранены => сбрасываеь флаг модификации

	return fl;
	
}


void CLineDlg::TreeOpen()
{ 
//	if (m_pTreeDlg==NULL)
//	{
//		m_pTreeDlg = new CTreeViewDlg(this);
//        if (m_pTreeDlg->Create()==TRUE)
//		ASSERT(m_pTreeDlg);
//	    m_pTreeDlg->SetActiveWindow();

	FConfig();
	
//=====================================================
  i_EL       = 0;     //текущий номер элемента
  i_MOD      = 0;     //текущий номер теста
  i_SelUrov  = 1;      //выделенный уровень:1-элемент
  CString str;
  pModSelected = NULL;
  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
  if (pModSelected)
  {  
	 if (!ControlSwitch())
	 {
	   ExtractParam();		//экстракция параметров
	   str_EL   = pModSelected->m_EL;
	   str_MOD  = pModSelected->m_MOD;
	   str_PRIM = pModSelected->m_PRIM;
	   strTestName = str_EL+_T(" \\ ")+str_MOD;	//_T(" Тест:  ")+текущая модель измерения (выбранный тест)
	   SetTextInfo(_T(" TEST:  ")+strTestName);//отобразить выбранный тест в информационном окне
//	   UpdateData(FALSE);		//отобразить параметры в окошках
		  SetStringsSourceMeas();//@ 11.12.17	
	   InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)
	   InitDiapazonBaza();	//инициализация диапазонов базы
	   InitDiapazonDop();	//инициализация диапазонов доп.канала

//	   InitGnezdo();//4.12.17
	   InitIconImp();
	   UpdateData(FALSE);		//отобразить параметры в окошках
	   
	   int nTab = 0;			//отобразить закладку "Схема включения"
	   m_TabCtrlParam.SetCurSel(nTab);
	   InitPage(nTab);
	   b_InitAxis=true;
	   int error = ControlParam();
/*	   if (error) 
	   {  //str.Format(_T("Входные параметры теста %s заданы неверно.\n\nОбнаружено ошибок: %d."),str_EL+_T("\\")+str_MOD,error);
	      //MessageBox(str,NULL,MB_ICONERROR);
	      int  nTabTuning = 1;//Error//0
	      m_TabCtrlTuning.SetCurSel(nTabTuning);
	      InitPageTuning(nTabTuning);	// отобразить панель "Ошибки"
		}
*/	 }//end of (!ControlSwitch())
	 else 
	 { //переключатель не в том положении
		 pModSelected = NULL;
	 }

  }
//=================================================
//	}
//	else
//	{ 
//	  m_pTreeDlg->SetActiveWindow();
//	}
//	m_Chart.SetWindowPos(&wndBottom,0,0,1,1,SWP_NOMOVE|SWP_NOSIZE);


}

void CLineDlg::TreeClose()
{
/*	m_pTreeDlg->DestroyWindow();
//	don't delete m_pTreeDlg; !
	m_pTreeDlg=NULL;
*/
	if (!(m_pList->IsEmpty())) 
	{ //удалить весь список тестов
		while (m_modList.GetHeadPosition())
		{
			delete m_modList.RemoveHead();
		}
		FConfig();
	}
}


BOOL CLineDlg::FileWrite()
{ //сохранение активного документа
	BOOL fl = FALSE;
	CFile myFile;
	CFileException er;
	CString str;
	CCmdTarget::BeginWaitCursor();
	if (!myFile.Open(strFileName,CFile::modeCreate |CFile::modeWrite,&er))// 
	{ TCHAR szError[1024];
      er.GetErrorMessage(szError, 1024);
	  MessageBox(_T("Невозможно сохранить активный файл модели измерения!\nНет свободного места на диске!"),NULL,MB_OK|MB_ICONERROR);
	  fl = FALSE;
	}
	else
	{ CArchive arj(&myFile,CArchive::store);
	  Serialize(arj);
	  arj.Close();
	  myFile.Close();
	  str.Format(_T("Файл активной модели измерения %s сохранён успешно."),strFileName);
	  MessageBox(str,NULL,MB_OK|MB_ICONINFORMATION);
	  fl = TRUE;
	  SetModifiedMemory();
	}
	EndWaitCursor();
 return fl;
}

BOOL CLineDlg::FileSaveAs() 
{ 	int p1,p2;
	BOOL fl_ok  = FALSE;
	CString str =_T("");
    CString strFileNameSave =_T("");
	CString str_temp=_T("");

	CFileDlg dlg;
	dlg.m_sTitle = _T("Сохранить как");
	dlg.m_iWhatDo = FILE_SAVE_AS;
	dlg.m_sFilter =_T("Line Files (\\Hard Disk\\Mod\\*.mod)");
	dlg.m_sCatalog =_T("\\Hard Disk\\Mod\\*.mod");
	dlg.m_sPath = _T("\\Hard Disk\\Mod\\");
	dlg.m_sName = _T("*.mod");
	dlg.m_sFullNameFile = _T("");
	dlg.m_itip = FILE_MOD;
	dlg.m_bZaschParol = m_bZaschitaParol;
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
	  strFileNameSave = dlg.m_sFullNameFile;
	  m_bZaschitaParol = dlg.m_bZaschParol;
/*	}

	CSaveAsDlg dlgFile;
	dlgFile.m_bZaschitaParol = m_bZaschitaParol;
	UpdateData(FALSE);
	if (dlgFile.DoModal()==IDOK)
	{ 
	  UpdateData();
	  strFileNameSave = dlgFile.m_sFileNameSave;
	  m_bZaschitaParol = dlgFile.m_bZaschitaParol;
*/	  if (strFileNameSave == strFileNoName)
	  { MessageBox(_T("Имя файла для сохранения не введено!"),NULL,MB_OK|MB_ICONERROR);
	    fl_ok = FALSE;
	  }
	  else
	  {//имя файла получено
		int l = strFileNameSave.GetLength();
		str = strFileNameSave.Mid(l-3,l);
		if (str!=_T("mod"))
		{ MessageBox(_T("Неверно указано расширение файла!"),NULL,MB_OK|MB_ICONERROR);
	      fl_ok = FALSE;
		}
		else
		{
		 if (strFileNameSave == strFileName)
		 {//имя файла для сохранения совпадает с прежним
			fl_ok = FileSave();
		 }
		 else
		 {//введено новое имя файла для сохранения
			str_temp = strFileName;	//временно сохранили прежнее имя документа
			strFileName = strFileNameSave;
			fl_ok = FileWrite();
			if (fl_ok) m_bModifiedFlag = FALSE;	//сохранение прошло успешно => сбрасываем флаг модификации
			else strFileName = str_temp;		//изменения не сохранены => восстанавливаем прежнее имя документа
		 }
		 if (fl_ok)
		 { m_bZaschitaParol = dlg.m_bZaschParol; //признак защиты сохранённого файла
		   //поменять имя текущего файла документа
//	       m_pTreeDlg->SetWindowText(strFileName);
			if (!strFileName.IsEmpty())
			{
			   p1=10;//пропускаем путь
			   p2=strFileName.Find(L'.',p1);
		       m_sNameFileMod = strFileName.Mid(p1,p2-p1+4);

//	       m_sNameFileMod=strFileName;
		   UpdateData(false);
		   //AfxGetApp()->WriteProfileString(_T("Settings"),_T("Last File"),(LPCTSTR)strFileName);
			}
		 }
		}
	  }//end if имя файла получено
	}//end if dlgFile IDOK
 return fl_ok;
}

void CLineDlg::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	if (m_bFileOpen) FileSaveAs();
}

void CLineDlg::OnFileClose() 
{
	// TODO: Add your command handler code here
/*	if (m_bFileOpen) FileClose();
    int nTab = 0;			//отобразить закладку "Схема включения"
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);
	pModSelected = NULL;
	m_sNameFileMod= _T("Файл модели");
     i_EL = 0;
	 i_MOD = 0;
	 i_SelUrov = 0;
	 m_EL = _T("");
  	 m_MOD = _T("");
	 m_PRIM = _T("");
	 str_EL		= _T("");
     str_MOD		= _T("");
     strTestName   = _T("");
	 str_PRIM = _T("");
     bAutomat = HAND; // "ручной"
    UpdateData(FALSE);
*/
  if (m_bFileOpen) 
  {
	if(	FileClose())
	{
 //   int nTab = 0;			//отобразить закладку "Схема включения"
//	m_TabCtrlParam.SetCurSel(nTab);
//	InitPage(nTab);
	pModSelected = NULL;
     i_EL = 0;
	 i_MOD = 0;
	 i_SelUrov = 0;
	 m_EL = _T("");
  	 m_MOD = _T("");
	 m_PRIM = _T("");
	 str_EL		= _T("");
     str_MOD		= _T("");
     strTestName   = _T("");
	 str_PRIM = _T("");
     bAutomat = HAND; // "ручной"

//	 InitNewSwitch();	//закомментировала 6.12.17
//добавила 6.12.17  ==>
	 if (m_bFileOpen)	
	    ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),FALSE);
	 else
	    ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),TRUE);
//<==

	 m_sNameFileMod= _T("Файл модели");
    UpdateData(FALSE);
	}
  }



}

BOOL CLineDlg::FileClose() 
{	
	BOOL fl=FALSE;	
	CString str;
	if (m_bModifiedFlag)
	{ 
	  str.Format(_T("Файл модели измерения %s был модифицирован! Сохранить изменения?"),strFileName);
	  int j= MessageBox(str,NULL,MB_YESNOCANCEL|MB_ICONQUESTION);
	  if (j==IDCANCEL)
	  { fl=FALSE;
	  }
	  else
	  { if (j==IDYES)
		{ //сохранить изменения
			if (strFileName == strFileUntitled) fl = FileSaveAs();
				else   fl = FileSave();
		}
		else fl=TRUE;	//закрыть документ без сохранения
	  }
	}
	else fl=TRUE;	//изменений нет => можно закрыть документ

	if (!fl)
	{ str.Format(_T("Файл модели измерения %s не закрыт."),strFileName);
      MessageBox(str,NULL,MB_OK|MB_ICONEXCLAMATION);
	}
	else 
	{ //закрыть активный документ
	  TreeClose();
	  m_bFileOpen = FALSE;
//			      ShowControls(aPage0_loadtst,SIZEOF_ARRAY(aPage0_loadtst),TRUE);
	  str.Format(_T("Файл модели измерения %s закрыт."),strFileName);
//    MessageBox(str,NULL,MB_OK|MB_ICONEXCLAMATION);
//		 m_sNameFileMod= _T("Файл модели");
//		 UpdateData(false);
	}
return fl;
}

void CLineDlg::OnFileExit() 
{
	// TODO: Add your command handler code here
//    AfxGetApp()->WriteProfileString(_T("Settings"),_T("Last File"),(LPCTSTR)strFileName);
/*	BOOL fl= FALSE;
if (MessageBox(_T("Вы уверены, что хотите завершить работу?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
{
	if (m_bFileOpen) fl = FileClose();
			   else  fl = TRUE;
	if (fl)
	{
*/
//	  ASSERT(AfxGetMainWnd()!=NULL);
//	  AfxGetMainWnd()->SendMessage(WM_CLOSE);
/*	}
}
*/	
}

void CLineDlg::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{
		// storing code
	  m_modList.Serialize(ar);
	  ar << m_bZaschitaParol;
	}
	else
	{
		// loading code
	  m_modList.Serialize(ar);
	  ar >> m_bZaschitaParol;
	}
}

void CLineDlg::TreeUpdate() 
{
  m_pList = GetList();
  m_position = GetPosition();
  *m_position = m_pList->GetHeadPosition();
/*
  pModSelected = NULL;
  i_EL			= 0;
  i_MOD			= 0;
  i_SelUrov		= 0;
  m_EL			= _T("");
  m_MOD			= _T("");
  strTestName   = _T("");
  str_EL		= _T("");
  str_MOD		= _T("");
*/
}
/*
void CLineDlg::OnButtonTabl() 
{
	// TODO: Add your control notification handler code here
	//пример, как управлять сигналом "Высокое напряжение"
	bSignal=!bSignal;
	switch(bSignal)
	{ case 0: ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),TRUE);
			  ShowControls(aSignal,SIZEOF_ARRAY(aSignal),FALSE); break;
	  case 1: ShowControls(aSignal,SIZEOF_ARRAY(aSignal),TRUE);
			  ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),FALSE); break;
	}
m_bModifiedFlag=TRUE;	
}
*/
void CLineDlg::OnFileReparol() 
{//Изменить пароль
	// TODO: Add your command handler code here
	CReparolDlg dlgr;
	UpdateData(FALSE);
	if (dlgr.DoModal()==IDOK)
	{
		UpdateData();
		if (m_sparol != dlgr.m_sparolold)
			MessageBox(_T("Старый пароль указан неверно."),NULL,MB_OK|MB_ICONEXCLAMATION);
		else
		{ if (dlgr.m_sparolnew != dlgr.m_sparolnew1)
			MessageBox(_T("Новый пароль введен некорректно."),NULL,MB_OK|MB_ICONEXCLAMATION);
		  else
		  { //изменение пароля
	        if (MessageBox(_T("Вы уверены, что хотите изменить пароль?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
			  m_sparol = dlgr.m_sparolnew;
//			  AfxGetApp()->WriteProfileString(_T("Settings"),_T("Line Password"),(LPCTSTR)m_sparol);
			  BOOL b = SetPassWordLine();
			  if (b) MessageBox(_T("Пароль изменён."),NULL,MB_OK|MB_ICONEXCLAMATION);
			  else 	
				     MessageBox(_T("Пароль изменить не удалось! Нет места на диске."),NULL,MB_OK|MB_ICONEXCLAMATION);

			}
		  }
		}
	}
	
}

void CLineDlg::OnButtonTest() 
{
	// TODO: Add your control notification handler code here
	  akt_button = 101;	
	  CMenu menu;
	  menu.LoadMenu(IDR_MENUFLOATINGTEST);
	  ASSERT(menu);
	  CPoint point;
	  point.x=RectButtonTest.left+10;
	  point.y=RectWnd.bottom-250;//180;

//	if (m_pTreeDlg)
	if (m_bFileOpen)
	{//1)
/*	  m_pTreeDlg->MoveWindow(&RectTree);
	  m_pTreeDlg->ShowWindow(SW_SHOW);
	  m_pTreeDlg->SetWindowText(strFileName);
	  CRect Rect;
	  m_pTreeDlg->GetClientRect(&Rect);
	  m_pTreeDlg->m_TreeCtrl.MoveWindow(&Rect);
	  m_pTreeDlg->SetActiveWindow();
*/	  if (m_pList->IsEmpty() || i_SelUrov==0) 
	  { //список тестов пуст => доступно только добавление
		   menu.GetSubMenu(0)->EnableMenuItem(ID_DEL,MF_GRAYED);
		   menu.GetSubMenu(0)->EnableMenuItem(ID_DEL_ALL,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_COPY,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_PASTE,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_TEST_RENAME,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_MODIFY,MF_GRAYED);
		   menu.GetSubMenu(0)->EnableMenuItem(ID_TST_STANDART,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_TST_USER,MF_GRAYED);
	  }
	  else
	  { if (i_SelUrov==1)
		{ //уровень элемента => недоступно сохранение и изменение(и переименование  - пока)
		   menu.GetSubMenu(0)->EnableMenuItem(ID_TST_STANDART,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_TST_USER,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_TEST_RENAME,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_MODIFY,MF_GRAYED);
		   //и вставка, если буфер пуст
		   if (!m_bCopy) menu.GetSubMenu(0)->EnableMenuItem(ID_PASTE,MF_GRAYED);
		}
	    if (i_SelUrov==2)
		{ //уровень теста => недоступно добавление
//	       menu.GetSubMenu(0)->EnableMenuItem(ID_ADDTREE1,MF_GRAYED);
		   //и вставка, если буфер пуст
		   if (!m_bCopy) menu.GetSubMenu(0)->EnableMenuItem(ID_PASTE,MF_GRAYED);
		}
	  }
	  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	}//end of 1)
	else
	{//2)
	 //файл модели измерения не загружен, возможно только сохранение в библиотеку активного теста
		   menu.GetSubMenu(0)->EnableMenuItem(ID_DEL,MF_GRAYED);
		   menu.GetSubMenu(0)->EnableMenuItem(ID_DEL_ALL,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_COPY,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_PASTE,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_TEST_RENAME,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_MODIFY,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_ADD_TREEFROMPANEL,MF_GRAYED);
	       menu.GetSubMenu(0)->EnableMenuItem(ID_ADD_TREEFROMMEMORY,MF_GRAYED);
 	  
	  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	}//end of 2)

}

int CLineDlg::FConfig() 
{//Конфигурирование дерева тестов загруженной модели измерения	
	POSITION pos;
	CString str;
	int i,j,k,m;
    BOOL fl = FALSE;
	
//	m_pTreeDlg->m_TreeCtrl.DeleteAllItems();
  TRACE(_T("FConfig()"));
	if (m_pList->IsEmpty()) 
	{ 
//      VERIFY(m_pTreeDlg->m_TreeCtrl.DeleteAllItems());
      VERIFY(m_TreeCtrl.DeleteAllItems());
      for(i=0;i<NE;i++) m_masStrEl[i]="";
      for(i=0;i<NE;i++)
		for(j=0;j<NT;j++)
			m_masStrMod[i][j]="";
	  strTestName=_T("");
	  str_PRIM=_T("");
	  SetTextInfo(_T("Список тестов файла модели пуст!\nВыберите категорию и тест для измерений!"));
	  return 0;
	}
	else
	{
	  //strTestName=_T("");
	  //if (i_SelUrov==0) 
		str_PRIM = _T("");
	  SetTextInfo(_T("Выберите тест для измерений из файла модели!"));
      for(i=0;i<NE;i++)
		for(j=0;j<NT;j++)
			m_masStrMod[i][j]="";
	  AddMasEl();
//	  VERIFY(m_pTreeDlg->m_TreeCtrl.DeleteAllItems());//m_pTreeDlg->m_TreeCtrl.DeleteAllItems();
	  VERIFY(m_TreeCtrl.DeleteAllItems());//m_pTreeDlg->m_TreeCtrl.DeleteAllItems();
//	  m_pTreeDlg->m_TreeCtrl.SetImageList(&m_imageList,TVSIL_NORMAL);
	  m_TreeCtrl.SetImageList(&m_imageList,TVSIL_NORMAL);
	  
	  TV_INSERTSTRUCT tvinsert;
	  tvinsert.hInsertAfter = TVI_LAST;
	  tvinsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
      tvinsert.item.hItem = NULL;
	  tvinsert.hParent = NULL;
	  tvinsert.item.state = 0;
	  tvinsert.item.stateMask = 0;
	  tvinsert.item.cchTextMax = 8;
	  tvinsert.item.iSelectedImage = 0;
	  tvinsert.item.cChildren = 0;
		m=0;
	  //верхний уровень - элемент
	  for (i=0;i<NE;i++)
	  { if(m_masStrEl[i]!="")
		{ 
			m++;		
		  tvinsert.hParent = NULL;
          tvinsert.item.pszText = m_masStrEl[i].GetBuffer(m_masStrEl[i].GetLength()+1);
	      tvinsert.item.iImage = 1;
	      tvinsert.item.lParam = i*1000+NT;//0xffff;
	      m_masStrEl[i].ReleaseBuffer();
//	      m_masHEl[i]= m_pTreeDlg->m_TreeCtrl.InsertItem(&tvinsert);
	      m_masHEl[i]= m_TreeCtrl.InsertItem(&tvinsert);
		}
	  }
		
	  if (m>=NE) MessageBox(_T("Число элементов максимально допустимое"),NULL,MB_OK|MB_ICONINFORMATION);

	  //тест - нижний уровень
	  *m_position=m_pList->GetHeadPosition();
	  do 
	  { pos=*m_position;
	    CMod* pMod =m_pList->GetNext(*m_position);
		str=pMod->m_EL;
	    for(i=0;i<NE;i++)
		  if (str==m_masStrEl[i])
		  { tvinsert.hParent = m_masHEl[i]; 
		    break;
		  }
		str=pMod->m_MOD;
		fl=FALSE;
		k=0;
		for (m=0;m<NT;m++)
		  if (str==m_masStrMod[i][m])
		  { fl=TRUE;
		    break;
		  }
		m=0;
		while (m_masStrMod[i][m]!="") m++;
		if (!fl) 
		{ if (m<NT)
			{
		  k=m;
		  m_masStrMod[i][k] = str.GetBuffer(str.GetLength()+1);
		  tvinsert.item.pszText = str.GetBuffer(str.GetLength()+1);
	      tvinsert.item.iImage = 2;
	      tvinsert.item.lParam = i*1000+k;		//i - элемент, k - тест
//	      m_pTreeDlg->m_TreeCtrl.InsertItem(&tvinsert);
	      m_TreeCtrl.InsertItem(&tvinsert);
		  k++; 
	      str.ReleaseBuffer();
			}
		else  MessageBox(_T("Число тестов для элемента ")+ m_masStrEl[i]+_T(" максимально допустимое"),NULL,MB_OK|MB_ICONINFORMATION);

		}
	  } while (*m_position);
	  return 1;
	}
}

void CLineDlg::AddMasEl()
{ //Формирование массива с перечнем тестируемых элементов
	POSITION pos;
	CString str;
	int i;
	BOOL fl=FALSE;
  

  if (!m_pList->IsEmpty())
  {
    for(i=0;i<NE;i++) m_masStrEl[i]="";
	*m_position=m_pList->GetHeadPosition();
	do 
	{ pos=*m_position;
      CMod* pMod =m_pList->GetNext(*m_position);
	  str=pMod->m_EL;
	  fl=FALSE;
	  for(i=0;i<NE;i++)
	   if(m_masStrEl[i]=="") break;
	   else if (m_masStrEl[i]==str) {fl=TRUE;break;}
			  else continue;
      
	  if (!fl)
	  { for(i=0;i<NE;i++)
		if (m_masStrEl[i]=="") break;
		m_masStrEl[i]=str.GetBuffer(str.GetLength()+1);
		str.ReleaseBuffer();
	  }
	
	}while (*m_position);
  }
}

void CLineDlg::Add() 
{	
	InsertEntry(*m_position);
    SetModifiedFlag();
	
}

void CLineDlg::SetModifiedFlag()
{	m_bModifiedFlag = TRUE;

}

void CLineDlg::SetModifiedMemory()
{	m_bModifiedMemory = TRUE;

}

void CLineDlg::InsertEntry(POSITION position)
{
	CString str;
	int p1,p2;
    int iStrLen;
    char* cc;//

/*не надо!!!
	if (pDlgTree->index==-1)
	{//добавляем тест с параметрами с экрана
	  pMod->m_nelement = m_nelement;
//	  pMod->m_iTIP  = m_iTIP;
	  //...
	}
*/	if (pDlgTree->index!=-1)
	{//выбран тест из списка
	 //добавить модель с данными из файла *.tst
	  if (!nameTst.IsEmpty())
	  { //выбор параметров из файла
		CFile file;

/*	   CFileDialog dl(TRUE);//,"tst","*.tst");
	
	   CString strd("Файлы стандартных параметров (*.tst)"); strd+=(TCHAR)NULL;
	   strd+="*.tst"; strd+=(TCHAR)NULL;
	   dl.m_ofn.lpstrFilter=strd;
	   dl.m_ofn.nFilterIndex=1;
	   TCHAR strName[_MAX_PATH];
	   strName[0]='*';
	   strName[1]='.';
	   strName[2]='t';
	   strName[3]='s';
	   strName[4]='t';
	   strName[5]=(TCHAR)NULL;
	   dl.m_ofn.lpstrFile=strName;
	   dl.m_ofn.lpstrTitle="Укажите файл для экстракции параметров";

	   if (dl.DoModal()==IDOK)
	   { CString name = dl.GetPathName();
	     CFile file;
		 CString str;
		 int p1,p2;
		 file.Open((LPCTSTR)name,CFile::modeRead);
*/
		 BOOL b = file.Open((LPCTSTR)nameTst,CFile::modeRead);
		 if (b)
		 {//if b
		 file.SeekToBegin();	
		 DWORD  l = file.GetLength();//
		 DWORD* lp = new DWORD[l];//
//		 ULONGLONG l = file.GetLength();
//		 BYTE* lp = new BYTE[l];
		 file.Read(lp,l);
		 file.Close();
		 CString strFile((LPCTSTR)&lp[0],l);
		 delete[] lp;
		 if (!strFile.IsEmpty())
		 {
          CMod* pMod = new CMod;
		   if(pMod){//!!!
	       pMod->m_EL       = pDlgTree->m_EL;
	       pMod->m_MOD      = pDlgTree->m_MOD;
//	       pMod->m_PRIM     = pDlgTree->m_PRIM;
			//коллектор
		   p1=strFile.Find(L'=',0);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_nelement = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   pMod->m_PRIM = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iCircuitKol = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
		   
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iCircuitBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
		   
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iCircuitDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iKommutator = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iGnezdoC = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iGnezdoB = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iGnezdoE = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_irazv = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_idlitimp = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iKolU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iKolI = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iKolSourceU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iLimitKolU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iLimitKolI = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();


		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_itipimp = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();


			//база
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iBazaSource = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();


		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iAmplitudeStepBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_uStepBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iPolarityStepBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iQue1 = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iBiasBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iPolarityBiasBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iBazaMeas = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iRangLimitBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iLimitBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();


			//доп.канал
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iDopSource = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iAmplitudeStepDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_uStepDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iPolarityStepDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iFamilyVkl = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iBiasDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   cc = new char[iStrLen];
		   iStrLen = str.GetLength();
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iPolarityBiasDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   cc = new char[iStrLen];
		   iStrLen = str.GetLength();
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iDopMeas = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iRangLimitDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iLimitDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

			//расчёт
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sAxisY = strFile.Mid(p1+1,p2-p1-2);

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sAxisX = strFile.Mid(p1+1,p2-p1-2);


		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sPar = strFile.Mid(p1+1,p2-p1-2);

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sArg = strFile.Mid(p1+1,p2-p1-2);

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_uTp = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sUr1 = strFile.Mid(p1+1,p2-p1-2);

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sUr2 = strFile.Mid(p1+1,p2-p1-2);

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_iForm = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sMin = strFile.Mid(p1+1,p2-p1-2);

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   pMod->m_sMax = strFile.Mid(p1+1,p2-p1-2);

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_bCheckKolRejHigh = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_bCheckPauza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_bBazaVkl = atoi(cc);
		   delete cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_bCheckBazaRejHigh = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

/*		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   pMod->m_bDopVkl = atoi(cc);
		   delete cc;
		   str.ReleaseBuffer();
*/
			*m_position = m_pList->InsertAfter(*m_position, pMod);
    
			POSITION pos = *m_position;
			FConfig();
			  //Expand
			HTREEITEM hitem = NULL;
			str=pMod->m_EL;
			for(int i=0;i<NE;i++)
			if (str==m_masStrEl[i])
			{ hitem = m_masHEl[i]; 
			  break;
			}
			if (hitem!=NULL)
			{
//			  m_pTreeDlg->m_TreeCtrl.Expand(hitem,TVE_EXPAND);
//			  m_pTreeDlg->m_TreeCtrl.EnsureVisible(hitem);
			  m_TreeCtrl.Expand(hitem,TVE_EXPAND);
			  m_TreeCtrl.EnsureVisible(hitem);
			}
			*m_position = pos;
			
//			delete pMod;	//надо закомментировать 29.11.17 !!!! //раскомментировала 29.11.17

			}//!!!
		 }//end of control strFile
		 }//end of if b
		 else
		  MessageBox(_T("Файл отсутствует в библиотеке тестов!"),NULL,MB_OK|MB_ICONERROR);
// 	   }//end of dl.DoModal()
	  }//end of nameTst.IsEmpty
	}//end of add param
}

CMod* CLineDlg::FindListName(LPCTSTR sEl,LPCTSTR sMod)
{
    POSITION pos;
	
	if (!m_pList->IsEmpty()) 
	{ *m_position = m_pList->GetHeadPosition();
	  do { pos=*m_position;
	       CMod* pMod =m_pList->GetNext(*m_position);
  	       if((!((pMod->m_EL).Compare(sEl)))&&((!((pMod->m_MOD).Compare(sMod)))))
		   {
        	pDlgTree->m_EL   = pMod->m_EL;
        	pDlgTree->m_MOD  = pMod->m_MOD;
//        	pDlgTree->m_PRIM = pMod->m_PRIM;
			*m_position = pos;
		    return pMod;
		   }
	  } while(*m_position);
   } 
   return NULL;
}

BOOL CLineDlg::CompareParam()
{
	BOOL b_ModifiedTest=FALSE;
	UpdateData();

   if ( 
        pModSelected->m_EL	 != m_EL ||
		pModSelected->m_MOD  != m_MOD ||
		pModSelected->m_PRIM != m_PRIM ||
		pModSelected->m_nelement    != m_nelement ||
	    pModSelected->m_iCircuitKol	!= m_iCircuitKol ||
	    pModSelected->m_iCircuitBaza!= m_iCircuitBaza ||
	    pModSelected->m_iCircuitDop	!= m_iCircuitDop ||
	    pModSelected->m_iKommutator	!= m_iKommutator ||
	    pModSelected->m_iGnezdoC	!= m_iGnezdoC ||
	    pModSelected->m_iGnezdoB	!= m_iGnezdoB ||
	    pModSelected->m_iGnezdoE	!= m_iGnezdoE ||
		pModSelected->m_irazv       != m_irazv ||
		pModSelected->m_idlitimp    != m_idlitimp ||
		pModSelected->m_iKolU       != m_iKolU ||
		pModSelected->m_iKolI       != m_iKolI ||
		pModSelected->m_iKolSourceU != m_iKolSourceU ||
		pModSelected->m_iLimitKolU  != m_iLimitKolU ||
		pModSelected->m_iLimitKolI  != m_iLimitKolI ||
		pModSelected->m_bCheckKolRejHigh != m_bCheckKolRejHigh ||
		pModSelected->m_itipimp     != m_itipimp ||
		pModSelected->m_bCheckPauza != m_bCheckPauza ||
		pModSelected->m_iBazaSource		   != m_iBazaSource ||
		pModSelected->m_bBazaVkl		   != m_bBazaVkl ||
		pModSelected->m_iAmplitudeStepBaza != m_iAmplitudeStepBaza ||
		pModSelected->m_uStepBaza		   != m_uStepBaza ||
		pModSelected->m_iPolarityStepBaza  != m_iPolarityStepBaza ||
		pModSelected->m_iQue1      != m_iQue1 ||
		pModSelected->m_iBiasBaza          != m_iBiasBaza ||		
		pModSelected->m_iPolarityBiasBaza  != m_iPolarityBiasBaza ||
		pModSelected->m_iBazaMeas          != m_iBazaMeas ||
		pModSelected->m_iRangLimitBaza     != m_iRangLimitBaza ||
		pModSelected->m_iLimitBaza         != m_iLimitBaza ||
		pModSelected->m_bCheckBazaRejHigh  != m_bCheckBazaRejHigh ||
		pModSelected->m_iDopSource         != m_iDopSource ||
//		pModSelected->m_bDopVkl		       != m_bDopVkl ||
		pModSelected->m_iAmplitudeStepDop  != m_iAmplitudeStepDop ||
		pModSelected->m_uStepDop           != m_uStepDop ||
		pModSelected->m_iPolarityStepDop   != m_iPolarityStepDop ||
		pModSelected->m_iFamilyVkl       != m_iFamilyVkl ||
		pModSelected->m_iBiasDop           != m_iBiasDop ||			
		pModSelected->m_iPolarityBiasDop   != m_iPolarityBiasDop ||
		pModSelected->m_iDopMeas           != m_iDopMeas ||
		pModSelected->m_iRangLimitDop      != m_iRangLimitDop ||
		pModSelected->m_iLimitDop          != m_iLimitDop ||	
		pModSelected->m_sAxisY != m_sAxisY ||
		pModSelected->m_sAxisX != m_sAxisX ||
		pModSelected->m_sPar   != m_sPar ||
		pModSelected->m_sArg   != m_sArg ||
		pModSelected->m_uTp    != m_uTp ||
		pModSelected->m_sUr1   != m_sUr1 ||
		pModSelected->m_sUr2   != m_sUr2 ||
		pModSelected->m_iForm  != m_iForm ||
		pModSelected->m_sMin   != m_sMin ||
		pModSelected->m_sMax   != m_sMax)
		
		b_ModifiedTest=TRUE;
	
return b_ModifiedTest;
}

//void CLineDlg::OnSelchangedTree()
void CLineDlg::SelchangedTree()
{	
	CString  str;
/* 
  //сохранить параметры предыдущего выбранного теста
  //------------------------------------------------
  if (pModSelected)
  { BOOL m_bModifiedFlagTest = CompareParam();
	  if (m_bModifiedFlagTest) 
	{  str.Format(_T("Параметры теста %s были изменены.\n\nСохранить изменения?"),str_EL+_T("\\")+str_MOD);
	   int j = MessageBox(str,NULL,MB_ICONEXCLAMATION|MB_YESNO);
	   if (j==IDYES) SaveParam();
	   m_bModifiedFlagTest = FALSE;
	}
  }
*/
  //поиск нового выбранного теста в списке
  //--------------------------------------
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
  if (pModSelected)
  {  
	 ExtractParam();		//экстракция параметров
//	 ExtractResults();		//экстракция последних результатов
	 str_EL   = pModSelected->m_EL;
	 str_MOD  = pModSelected->m_MOD;
	 str_PRIM = pModSelected->m_PRIM;
	 strTestName = str_EL+_T(" \\ ")+str_MOD;	//_T(" Тест:  ")+текущая модель измерения (выбранный тест)
	 SetTextInfo(_T(" TEST:  ")+strTestName);//отобразить выбранный тест в информационном окне
	 UpdateData(FALSE);		//отобразить параметры в окошках
	   InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)
	   InitDiapazonBaza();	//инициализация диапазонов базы
	   InitDiapazonDop();	//инициализация диапазонов доп.канала
//	   SetTest();
//	   if (!nCheckParam)
//	   {
	    int nTab = 0;			//отобразить закладку "Схема включения"
	     m_TabCtrlParam.SetCurSel(nTab);
	     InitPage(nTab);
//	   }
	 int error = ControlParam();
	 if (error) 
	 { //str.Format(_T("Входные параметры теста %s заданы неверно.\n\nОбнаружено ошибок: %d."),str_EL+_T("\\")+str_MOD,error);
	   //MessageBox(str,NULL,MB_ICONERROR);
	   DisplayError();
	 }
  }
  else 
  {  i_EL = 0;
	 i_MOD = 0;
	 i_SelUrov = 0;
	 str_EL		= _T("");
     str_MOD		= _T("");
     strTestName   = _T("");
		str_PRIM = _T("");
	 SetTextInfo(_T("Выберите тест для измерения!"));
     bAutomat = HAND; // "ручной"
  }
}
/*
void CLineDlg::SelchangedTree()
{	
	CString  str;
 
  //сохранить параметры предыдущего выбранного теста
  //------------------------------------------------
  if (pModSelected)
  { BOOL m_bModifiedFlagTest = CompareParam();
	  if (m_bModifiedFlagTest) 
	{  str.Format(_T("Параметры теста %s были изменены.\n\nСохранить изменения?"),str_EL+_T("\\")+str_MOD);
	   int j = MessageBox(str,NULL,MB_ICONEXCLAMATION|MB_YESNO);
	   if (j==IDYES) SaveParam();
	   m_bModifiedFlagTest = FALSE;
	}
  }

  //поиск нового выбранного теста в списке
  //--------------------------------------
  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
  if (pModSelected)
  {  
	 ExtractParam();		//экстракция параметров
	 ExtractResults();		//экстракция последних результатов
	 str_EL   = pModSelected->m_EL;
	 str_MOD  = pModSelected->m_MOD;
	 str_PRIM = pModSelected->m_PRIM;
	 strTestName = str_EL+_T(" \\ ")+str_MOD;	//_T(" Тест:  ")+текущая модель измерения (выбранный тест)
			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала
	 UpdateData(FALSE);		//отобразить параметры в окошках
	if (!nCheckParam)
	{
	 int nTab = 0;			//отобразить закладку "Схема включения"
	 m_TabCtrlParam.SetCurSel(nTab);
	 InitPage(nTab);
	}
	 UpdateData(FALSE);		//отобразить параметры в окошках
	 //отобразить выбранный тест в информационном окне
	 SetTextInfo(_T(" TEST:  ")+strTestName);
	 //контроль параметров
//     ClearError();
//	 InitTest();
	 int error = ControlParam();
	 if (error) 
	 { str.Format(_T("Входные параметры теста %s заданы неверно.\n\nОбнаружено ошибок: %d."),str_EL+_T("\\")+str_MOD,error);
	   MessageBox(str,NULL,MB_ICONERROR);
	   DisplayError();
	 }
  }
  else 
  {  i_EL = 0;
	 i_MOD = 0;
	 i_SelUrov = 0;
	 str_EL		= _T("");
     str_MOD		= _T("");
     strTestName   = _T("");
	 SetTextInfo(_T("Выберите тест для измерения!"));
     bAutomat = HAND; // "ручной"
  }
}
*/
UINT CLineDlg::CountnTest()
{ UINT k = 0;
  int  i = 0;

	 switch(i_SelUrov)
	 { case 1:  //выделенный уровень = 1 (элемент) =>
				bAutomat = AUTOMAT;  // "автоматический"
				//подсчет числа тестов для выбранного элемента
				for (i=0;i<NT;i++)
				{ if (m_masStrMod[i_EL][i]!="") k++;
				  else break;
				}
				break;
	   default: //выделенный уровень не определён или
	   case 0:	//не выбран или
	   case 2:  //выделенный уровень = 2 (тест) =>
				bAutomat = HAND; // "ручной"
				k = 1;
		        break;
	 }
  return k;
}

void CLineDlg::ExtractParam()
{
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);//???_???
	if (pModSelected)
	{
	strDateMeas = pModSelected->strDateMeas;
	strTimeMeas = pModSelected->strTimeMeas;
	m_EL        = pModSelected->m_EL;
	m_MOD       = pModSelected->m_MOD;
	m_PRIM      = pModSelected->m_PRIM;
	//схема включения
	m_nelement			 = pModSelected->m_nelement;
	m_iCircuitKol		 = pModSelected->m_iCircuitKol;
	m_iCircuitBaza	     = pModSelected->m_iCircuitBaza;
	m_iCircuitDop		 = pModSelected->m_iCircuitDop;
	    m_iKommutator	= pModSelected->m_iKommutator;
	    m_iGnezdoC	= pModSelected->m_iGnezdoC;
	    m_iGnezdoB	= pModSelected->m_iGnezdoB;
	    m_iGnezdoE	= pModSelected->m_iGnezdoE;
	//коллектор
	m_irazv              = pModSelected->m_irazv;
	m_idlitimp			 = pModSelected->m_idlitimp;
	m_iKolU				 = pModSelected->m_iKolU;
	m_iKolI				 = pModSelected->m_iKolI;
	m_iKolSourceU		 = pModSelected->m_iKolSourceU;
	m_iLimitKolU		 = pModSelected->m_iLimitKolU;
	m_iLimitKolI		 = pModSelected->m_iLimitKolI;
	m_bCheckKolRejHigh   = pModSelected->m_bCheckKolRejHigh;
	m_itipimp			 = pModSelected->m_itipimp;
	m_bCheckPauza		 = pModSelected->m_bCheckPauza;
	//база
	m_iBazaSource        = pModSelected->m_iBazaSource;
	m_bBazaVkl           = pModSelected->m_bBazaVkl;
	m_iAmplitudeStepBaza = pModSelected->m_iAmplitudeStepBaza;
	m_uStepBaza          = pModSelected->m_uStepBaza;
	m_iPolarityStepBaza  = pModSelected->m_iPolarityStepBaza;
	m_iQue1				 = pModSelected->m_iQue1;
    if (m_iQue1==-1) m_iQue1=COLLECTOR;
	m_iBiasBaza          = pModSelected->m_iBiasBaza;		
	m_iPolarityBiasBaza  = pModSelected->m_iPolarityBiasBaza;
	m_iBazaMeas			 = pModSelected->m_iBazaMeas;
	m_iRangLimitBaza     = pModSelected->m_iRangLimitBaza;
	m_iLimitBaza         = pModSelected->m_iLimitBaza;	
	m_bCheckBazaRejHigh  = pModSelected->m_bCheckBazaRejHigh;
	//доп.канал
	m_iDopSource		 = pModSelected->m_iDopSource;
//	m_bDopVkl            = pModSelected->m_bDopVkl;
	m_iAmplitudeStepDop  = pModSelected->m_iAmplitudeStepDop;
	m_uStepDop			 = pModSelected->m_uStepDop;
	m_iPolarityStepDop	 = pModSelected->m_iPolarityStepDop;
	m_iFamilyVkl		 = pModSelected->m_iFamilyVkl;
    if (m_iFamilyVkl==-1) m_iFamilyVkl = 0;		
	if (m_iFamilyVkl==1) m_bFamilyVkl=true;
				   else  m_bFamilyVkl=false;
		   m_bFamilyVklOld = m_bFamilyVkl;
	m_iBiasDop			 = pModSelected->m_iBiasDop;			
	m_iPolarityBiasDop	 = pModSelected->m_iPolarityBiasDop;
	m_iDopMeas			 = pModSelected->m_iDopMeas;
	m_iRangLimitDop		 = pModSelected->m_iRangLimitDop;
	m_iLimitDop			 = pModSelected->m_iLimitDop;		
	//расчёт
	m_sAxisY = pModSelected->m_sAxisY;
	m_sAxisX = pModSelected->m_sAxisX;
	m_sPar   = pModSelected->m_sPar;
	m_sArg   = pModSelected->m_sArg;
	m_uTp    = pModSelected->m_uTp;
	m_sUr1   = pModSelected->m_sUr1;
	m_sUr2   = pModSelected->m_sUr2;
	m_iForm  = pModSelected->m_iForm;
	m_sMin   = pModSelected->m_sMin;
	m_sMax   = pModSelected->m_sMax;
	}
   m_iPosTek = m_iLimitKolI;
   DrawStatusRukoyatka();	//индикация Старта по Uc
//	SetStringsSourceMeas();
//	BuildCircuit();
}

void CLineDlg::SaveParam()
{
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);//???_???
	UpdateData();

	if (pModSelected)
	{
	pModSelected->m_EL		  = m_EL;
	pModSelected->m_MOD		  = m_MOD;
	pModSelected->m_PRIM	  = m_PRIM;
	//схема включения
	pModSelected->m_nelement		 = m_nelement;
	pModSelected->m_iCircuitKol		 = m_iCircuitKol;
	pModSelected->m_iCircuitBaza	 = m_iCircuitBaza;
	pModSelected->m_iCircuitDop		 = m_iCircuitDop;
	    pModSelected->m_iKommutator	= m_iKommutator;
	    pModSelected->m_iGnezdoC	= m_iGnezdoC;
	    pModSelected->m_iGnezdoB	= m_iGnezdoB;
	    pModSelected->m_iGnezdoE	= m_iGnezdoE;
	//коллектор
	pModSelected->m_irazv			 = m_irazv;
	pModSelected->m_idlitimp		 = m_idlitimp;
	pModSelected->m_iKolU			 = m_iKolU;
	pModSelected->m_iKolI			 = m_iKolI;
	pModSelected->m_iKolSourceU		 = m_iKolSourceU;
	pModSelected->m_iLimitKolU		 = m_iLimitKolU;
	pModSelected->m_iLimitKolI		 = m_iLimitKolI;
	pModSelected->m_bCheckKolRejHigh = m_bCheckKolRejHigh;
	pModSelected->m_itipimp 		 = m_itipimp;
	pModSelected->m_bCheckPauza		 = m_bCheckPauza;

	//база
	pModSelected->m_iBazaSource		   = m_iBazaSource;
	pModSelected->m_bBazaVkl		   = m_bBazaVkl;
	pModSelected->m_iAmplitudeStepBaza = m_iAmplitudeStepBaza;
	pModSelected->m_uStepBaza		   = m_uStepBaza;
	pModSelected->m_iPolarityStepBaza  = m_iPolarityStepBaza;
	pModSelected->m_iQue1      = m_iQue1;
	pModSelected->m_iBiasBaza          = m_iBiasBaza;		
	pModSelected->m_iPolarityBiasBaza  = m_iPolarityBiasBaza;
	pModSelected->m_iBazaMeas          = m_iBazaMeas;
	pModSelected->m_iRangLimitBaza     = m_iRangLimitBaza;
	pModSelected->m_iLimitBaza         = m_iLimitBaza;	
	pModSelected->m_bCheckBazaRejHigh  = m_bCheckBazaRejHigh;
	//доп.канал
	pModSelected->m_iDopSource         = m_iDopSource;
//	pModSelected->m_bDopVkl		       = m_bDopVkl;
	pModSelected->m_iAmplitudeStepDop  = m_iAmplitudeStepDop;
	pModSelected->m_uStepDop           = m_uStepDop;
	pModSelected->m_iPolarityStepDop   = m_iPolarityStepDop;
	pModSelected->m_iFamilyVkl       = m_iFamilyVkl;
	pModSelected->m_iBiasDop           = m_iBiasDop;			
	pModSelected->m_iPolarityBiasDop   = m_iPolarityBiasDop;
	pModSelected->m_iDopMeas           = m_iDopMeas;
	pModSelected->m_iRangLimitDop      = m_iRangLimitDop;
	pModSelected->m_iLimitDop          = m_iLimitDop;	
	//расчёт
	pModSelected->m_sAxisY = m_sAxisY;
	pModSelected->m_sAxisX = m_sAxisX;
	pModSelected->m_sPar   = m_sPar;
	pModSelected->m_sArg   = m_sArg;
	pModSelected->m_uTp    = m_uTp;
	pModSelected->m_sUr1   = m_sUr1;
	pModSelected->m_sUr2   = m_sUr2;
	pModSelected->m_iForm  = m_iForm;
	pModSelected->m_sMin   = m_sMin;
	pModSelected->m_sMax   = m_sMax;
	
	SetModifiedFlag();
	}
}

void CLineDlg::SaveResults()
{   UINT m=0;
	UINT i=0;
	strDateMeas = strDateCurrent;
    strTimeMeas = strTimeCurrent;
	int n_point=NPOINT;
//	if (m_itipimp==TIP_IMP) n_point = 1;//---!
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_irazv==DC_POS || m_irazv==DC_NEG) n_point = n_Result;

	n_Result = n_Step * n_point;
	
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);//???_???
/*
	if (pModSelected)
	{
    pModSelected->n_Result = n_Result;		//n_Result = NPOINT * n_Step; (NPOINT = 250, n_Step от 1 до 100)
	pModSelected->n_Step = n_Step;
	pModSelected->n_Mas = n_Mas;
	for (m=0;m<n_Mas;m++)
	for (i=0;i<n_Result;i++)
	{ pModSelected->pM1[m][i]=pM1[m][i];
	  pModSelected->pM2[m][i]=pM2[m][i];
	  pModSelected->pM3[m][i]=pM3[m][i];
	  pModSelected->pM4[m][i]=pM4[m][i];
	  pModSelected->pM5[m][i]=pM5[m][i];

	pModSelected->strDateMeas = strDateCurrent;
	pModSelected->strTimeMeas = strTimeCurrent;
	}
 
	SetModifiedFlag();
	}
*/

}
/*
void CLineDlg::ExtractResults()
{
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);//???_???
	if (pModSelected)
	{	
    n_Result = pModSelected->n_Result;		//n_Result = NPOINT * n_Step; (NPOINT = 250, n_Step от 1 до 100)
	n_Step = pModSelected->n_Step;
	n_Mas  = pModSelected->n_Mas;
	for (UINT m=0;m<n_Mas;m++)
	for (UINT i=0;i<n_Result;i++)
	{ pM1[m][i]=pModSelected->pM1[m][i];
	  pM2[m][i]=pModSelected->pM2[m][i];
	  pM3[m][i]=pModSelected->pM3[m][i];
	  pM4[m][i]=pModSelected->pM4[m][i];
	  pM5[m][i]=pModSelected->pM5[m][i];
	}

	strDateMeas = pModSelected->strDateMeas;
	strTimeMeas = pModSelected->strTimeMeas;
	}

}
*/
CMod* CLineDlg::FindTest(LPCTSTR sEl,LPCTSTR sMod)
{
    POSITION pos;
	
	if (!m_pList->IsEmpty()) 
	{ *m_position = m_pList->GetHeadPosition();
	  do { pos=*m_position;
	       CMod* pMod =m_pList->GetNext(*m_position);
  	       if((!((pMod->m_EL).Compare(sEl)))&&((!((pMod->m_MOD).Compare(sMod)))))
		   {
			*m_position = pos;
		    return pMod;
		   }
	  } while(*m_position);
   } 
   return NULL;
}

int CLineDlg::ControlSwitch()
{   CString str;
	ClearError();
	int n_er=0;//array_error.GetSize();//счетчик ошибок: if 0 - no error

	if (n_IPPP==1 && pModSelected)
	{ 
		if (n_Switch==3)
		{ 
	      str=_T("ERROR 135: Переключатель в нейтральном положении\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}
		if (n_Switch==4)
		{ 
		  str=_T("ERROR 216: Переключатель неисправен\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}
		
		//проверка положения переключателя 5 V / 2000 V
//		if (n_Switch!=0 && pModSelected->m_irazv!= SYN_POS && pModSelected->m_irazv!= SYN_NEG && pModSelected->m_idlitimp<5)
		if (n_Switch!=0 && pModSelected->m_idlitimp<5)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне ±2000V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}
		if (n_Switch==0 && pModSelected->m_idlitimp>4)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне ±5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}
//		if (n_Switch==1 && pModSelected->m_irazv == IMP_NEG && pModSelected->m_idlitimp>4)
		if ((n_Switch==1 && pModSelected->m_irazv == IMP_NEG && pModSelected->m_idlitimp>4)||
		    (n_Switch==1 && pModSelected->m_irazv == SYN_NEG && pModSelected->m_idlitimp>4))
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне -5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}
//		if (n_Switch==2 && pModSelected->m_irazv == IMP_POS && pModSelected->m_idlitimp>4)
		if ((n_Switch==2 && pModSelected->m_irazv == IMP_POS && pModSelected->m_idlitimp>4)||
		    (n_Switch==2 && pModSelected->m_irazv == SYN_POS && pModSelected->m_idlitimp>4))
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне +5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}

	}
	if (n_er)
	{ 
	  str=_T("ERROR 136: Тест не выбран. Проверьте соответствие теста положению переключателя\r\n");
      array_error.SetAtGrow(n_er,str); 
      n_er++;
	  int   nTabTuning = 1;
	  m_TabCtrlTuning.SetCurSel(nTabTuning);
	  InitPageTuning(nTabTuning);	// отобразить панель "Ошибки"
	}
return n_er;
}
int CLineDlg::ControlParam()
{ //контроль входных параметров
	
	TRACE(_T("ControlParam()\n"));
	CString str;
	CString temp;
	double a = 0.0;
	double b = 0.0;
	double k = 0.0;
	int nTabTuning;
	int n_er;

//	UpdateData(TRUE);	//убрала	//29.11.17
	n_Step  = 1;					//значение по умолчанию
//	i_tipimp = m_itipimp;
//	bGenerationStup = false;
	m_bR = FALSE;					////рассчитать сопротивление
	m_bBeta = FALSE;				//рассчитать Beta
	m_bBetai= FALSE;				//рассчитать Betai
    m_bErli = FALSE;				//рассчитать напряжение Эрли
	m_bS	= false;				//рассчитать S
	m_uFunc = 0;
	m_bminusX = FALSE;
	m_bminusY = FALSE;

	m_sAxisX.TrimLeft();
    m_sAxisX.TrimRight();
	m_sAxisY.TrimLeft();
    m_sAxisY.TrimRight();
	m_sPar.TrimLeft();
    m_sPar.TrimRight();
	m_sArg.TrimLeft();
    m_sArg.TrimRight();
	m_sUr1.TrimLeft();
    m_sUr1.TrimRight();
	m_sUr2.TrimLeft();
    m_sUr2.TrimRight();
	m_sMin.TrimLeft();
    m_sMin.TrimRight();
	m_sMax.TrimLeft();
    m_sMax.TrimRight();

		ClearError();
		n_er=array_error.GetSize();//счетчик ошибок: if 0 - no error

		if (m_bFileOpen && pModSelected==NULL)
		{ 
		  str=_T("ERROR 136: Тест не выбран. Проверьте соответствие теста положению переключателя\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		  FConfig();
//		  return n_er;
		}
if (!n_er)
{//0.0 	if (!n_er)

	if (n_IPPP==1)
	{ 
		if (n_Switch==3)
		{ 
	      str=_T("ERROR 135: Переключатель в нейтральном положении\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
//		  return n_er;
		}
		if (n_Switch==4)
		{ 
		  str=_T("ERROR 216: Переключатель неисправен\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
//		  return n_er;
		}
		
		//проверка положения переключателя 5 V / 2000 V
//		if (n_Switch!=0 && m_irazv!= SYN_POS && m_irazv!= SYN_NEG && m_idlitimp<5)
		if (n_Switch!=0 && m_idlitimp<5)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне ±2000V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
//		  return n_er;
		}
		if (n_Switch==0 && m_idlitimp>4)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне ±5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
//		  return n_er;
		}
/*		if (n_Switch==1 && m_irazv!=IMP_POS)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне -5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
//		  return n_er;
		}
		if (n_Switch==2 && m_irazv!=IMP_NEG)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне +5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
//		  return n_er;
		}

*/	
		if (n_Switch==1 && m_irazv == IMP_NEG && m_idlitimp>4)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне -5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}
		if (n_Switch==2 && m_irazv == IMP_POS && m_idlitimp>4)
		{
	      str=_T("ERROR 135: Положение переключателя не в диапазоне +5V\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
		}
		if (pModSelected && i_SelUrov==1 && m_iKommutator==1)
		{
	      str=_T("ERROR 136: В автоматическом режиме не допустима внешняя коммутация\r\n");
	      array_error.SetAtGrow(n_er,str); 
	      n_er++;
//		  return n_er;
		}


	}
//=====================================	
	if (m_iKommutator==0)
	{
	if (((m_nelement!=DIOD) && (m_nelement!=RES))&&
		((m_iGnezdoC==m_iGnezdoB) || (m_iGnezdoC==m_iGnezdoE) ||(m_iGnezdoB==m_iGnezdoE)))
	{ 
	  str=_T("ERROR 128: Неверно указано подключение к гнездам\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	}
	else
	{ if ((m_nelement>RES && m_nelement< NPN4 && n_obryv>1) ||
		  (m_nelement>PMOS && n_obryv>2))
		{ 
		  str=_T("ERROR 128: Неверно указано подключение к гнездам\r\n");
		  array_error.SetAtGrow(n_er,str); 
		  n_er++;
		}
		if ((m_iGnezdoC==m_iGnezdoB && m_iGnezdoC!=3) ||
			(m_iGnezdoC==m_iGnezdoE && m_iGnezdoE!=3) ||
			(m_iGnezdoB==m_iGnezdoE && m_iGnezdoB!=3))// ||
//			(m_iGnezdoC==3 && m_iGnezdoB==3 && m_iGnezdoE==3))
		{ 
		  str=_T("ERROR 128: Неверно указано подключение к гнездам\r\n");
		  array_error.SetAtGrow(n_er,str); 
		  n_er++;
		}
	   if ((m_iKnotKol==0 && m_iQue1==COLLECTOR) ||
		  (m_iKnotBaza==0 && m_iQue1==BAZA))
		{ //коллектор в обрыве => 
			str=_T("ERROR 126: Источник 1-ой очереди в обрыве\r\n");
			array_error.SetAtGrow(n_er,str); 
			n_er++;
		}

	}

	if (m_iCircuitKol==0 && n_IPPP==1 && n_Switch) 
	{ //коллектор в обрыве => 
	  str=_T("ERROR 126: Коллектор в обрыве. Проверить переключатель ±2000V\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	
	if (m_nelement==-1) 
	{ str=_T("ERROR 100: Не указана категория тестируемого элемента\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
/*	if (m_MOD.IsEmpty()) 
	{ str=_T("ERROR 101: Не указано наименование теста.\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
*/	if ((m_iCircuitKol<0 && m_iCircuitBaza<0 && m_iCircuitDop<0) ||
	    (m_iCircuitKol==0 && m_iCircuitBaza==0 && m_iCircuitDop==0))
	{ str=_T("ERROR 102: Ошибка схемы включения тестируемого элемента\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	if (m_iCircuitKol==1 && m_irazv<0)
	{ str=_T("ERROR 103: Не указан вид развертки\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	if (m_iCircuitKol==1 && (m_irazv==IMP_POS || m_irazv==IMP_NEG) && m_idlitimp<0)
	{ str=_T("ERROR 104: Не указана длительность импульса\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	if (m_sAxisX.IsEmpty()) 
	{ str=_T("ERROR 105: Не указан аргумент для построения графика\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	else
	{ m_bminusX = FALSE;
	  temp = m_sAxisX;
	  if (temp.GetAt(0)==0x2D)
	  { m_bminusX=TRUE;			//признак: перевернуть шкалу X
		temp=temp.Mid(1,temp.GetLength());
	    m_sAxisX=temp;
	  }
	}
	if (m_sAxisY.IsEmpty()) 
	{ str=_T("ERROR 106: Не указана функция для построения графика\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	else
	{ m_bminusY = FALSE;
	  temp = m_sAxisY;
	  if (temp.GetAt(0)==0x2D)
	  { m_bminusY=TRUE;			//признак: перевернуть шкалу Y
		temp=temp.Mid(1,temp.GetLength());
	    m_sAxisY=temp;
	  }
	}
	if (m_iCircuitKol==1 && !m_sAxisX.IsEmpty() && m_sAxisX != m_sKolMeasU && m_irazv==AC) 
	{ str=_T("ERROR 107: Для заданной зависимости запрещена развертка вида ±AC\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	if ((!m_sAxisX.IsEmpty() && 
		  m_sAxisX != m_sKolMeasU && 
		  m_sAxisX != m_sKolMeasI &&
		  m_sAxisX != m_sBazaSource &&
		  m_sAxisX != m_sDopSource &&
		  m_sAxisX != m_sBazaMeas &&
		  m_sAxisX != m_sDopMeas &&
		  m_sAxisX != m_sBazaMeasLimit &&
		  m_sAxisX != m_sDopMeasLimit)  ||
	     (!m_sArg.IsEmpty() && m_sArg!= m_sAxisX && m_sArg!=m_sAxisY))
	{ str.Format(_T("ERROR 108: Неизвестная характеристика %s по оси Х\r\n"),m_sAxisX);
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
//	if (m_sAxisX == m_sKolMeasI ||
/*	if	 ((m_sAxisX == m_sBazaMeas  && m_iQue1==0 && m_iKnotBaza!=0) || //m_sAxisX!=m_sAxisY) ||
		  (m_sAxisX == m_sDopMeas  &&  m_iQue1==0) || //m_sAxisX!=m_sAxisY))
		  (m_sAxisX == m_sKolMeasU &&  m_iQue1==1) ||
		  (m_sAxisX == m_sKolMeasI && m_iQue1==1))
	{ //str=_T("ERROR 108: В качестве аргумента задана измеряемая характеристика\r\n");
	  str=_T("ERROR 109: По оси Х заданная величина не соответствует 1-ой очереди\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
*/
/*	if ((!m_sAxisX.IsEmpty() && m_sAxisX == m_sBazaSource) &&
		(!m_sAxisY.IsEmpty() && m_sAxisY == m_sAxisX)) m_bBaza=TRUE;//изменить режим измерения в базе

	if ((!m_sAxisX.IsEmpty() && m_sAxisX == m_sDopSource) &&
		(!m_sAxisY.IsEmpty() && m_sAxisY == m_sAxisX)) m_bDop=TRUE;//изменить режим измерения в доп.канале
*/
	if (!m_sAxisY.IsEmpty() && 
		 m_sAxisY != m_sKolMeasI && 
		 m_sAxisY != m_sKolMeasU && 
		 m_sAxisY != m_sBazaMeas &&
		 m_sAxisY != m_sDopMeas &&
		 m_sAxisY != m_sKolMeasU && 
		 m_sAxisY != m_sBazaSource &&
		 m_sAxisY != m_sDopSource && 
		 m_sAxisY != m_sBazaMeasLimit &&
		 m_sAxisY != m_sDopMeasLimit) 
	{ str.Format(_T("ERROR 108: Неизвестная характеристика %s по оси Y\r\n"),m_sAxisY);
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	if (m_sAxisX==m_sAxisY)
	{ str=_T("ERROR 110: По оси Х и Y заданы одноименные величины \r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	if (!m_sPar.IsEmpty() && m_sPar!= m_sAxisX && m_sPar!=m_sAxisY && 
		 m_sPar!=_T("Beta") && m_sPar!=_T("Betai") &&  m_sPar!=_T("R") &&  m_sPar!=_T("S"))
	{ str.Format(_T("ERROR 108: Неизвестная характеристика %s для расчета\r\n"),m_sPar);
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
/*	if (m_sPar==_T("R") && m_nelement>RES)
	{ str=_T("ERROR 132: Расчет R доступен только для двухполюсников\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
*/
	if ((m_sPar==_T("Beta") || m_sPar== _T("Betai")) && (m_nelement!= NPN && m_nelement!= PNP && m_nelement!= NPN4 && m_nelement!= PNP4))
	{ str=_T("ERROR 133: Расчет Beta доступен только для биполярных транзисторов\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}

	if (m_sPar==_T("S") && (m_nelement!= NMOS && m_nelement!= PMOS && m_nelement!= NMOS4 && m_nelement!= PMOS4))
	{ str=_T("ERROR 134: Расчет S доступен только для МОП-транзисторов\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}
	if (m_sPar==_T("S") && 
		(m_nelement== NMOS || m_nelement== PMOS|| m_nelement== NMOS4 || m_nelement== PMOS4)&&
		m_sArg==_T(""))	{ m_bS = true; m_uFunc = 5;}////m_uFunc>1--!
	if (m_sPar==_T("Beta") &&
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4)&&
		m_sArg==_T("")) { m_bBeta = true; m_uFunc = 5;}////m_uFunc>1--!
	if (m_sPar==_T("Betai") &&
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4)&&
		m_sArg==_T(""))	{ m_bBetai = true; m_uFunc = 5;}////m_uFunc>1--!

	if (m_iQue1==COLLECTOR && m_sPar==_T("S") &&
		(m_nelement== NMOS || m_nelement== PMOS|| m_nelement== NMOS4 || m_nelement== PMOS4))
	{
		if (m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && //m_sAxisX==_T("Id") && m_sAxisY==_T("Ug") && 
		m_iBazaSource== 0 && m_sArg==m_sAxisX &&// m_sBazaSource== _T("Ug") && m_sArg==m_sAxisX && 
		(m_irazv == IMP_POS || m_irazv==IMP_NEG || m_irazv == DC_POS || m_irazv==DC_NEG))
		{ m_bS = true; m_uFunc = 2;}////---!

		if (m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && //m_sAxisX==_T("Id") && m_sAxisY==_T("Ug") && 
		m_iBazaSource== 0 && m_sArg==m_sAxisY && //m_sBazaSource== _T("Ug") && m_sArg==m_sAxisY && 
		(m_irazv == IMP_POS || m_irazv==IMP_NEG || m_irazv == DC_POS || m_irazv==DC_NEG))
		{ m_bS = true; m_uFunc = 3;}////---!

	}

	if (m_iQue1==BAZA && m_sPar==_T("S")&&
		(m_nelement== NMOS || m_nelement== PMOS|| m_nelement== NMOS4 || m_nelement== PMOS4))
	{ 
		if (m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && //m_sAxisX==_T("Ug") && m_sAxisY==_T("Id") && 
		m_iBazaSource== 0 && m_sArg==m_sAxisX &&//m_sBazaSource== _T("Ug") && m_sArg==m_sAxisX &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG))  
		{ m_bS = true; m_uFunc = 2;}//---!

		if (m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && //m_sAxisX==_T("Ug") && m_sAxisY==_T("Id") && 
		m_iBazaSource== 0 && m_sArg==m_sAxisY &&//m_sBazaSource== _T("Ug") && m_sArg==m_sAxisY &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG))  
		{ m_bS = true; m_uFunc = 3;}//---!

	}
 	  if (m_sPar==_T("S") &&  m_iBazaSource==0 &&
 		 (m_sAxisX!=m_sKolMeasI || m_sAxisY!=m_sBazaSource))//if (m_sAxisX!=_T("Ug") && m_sAxisY!=_T("Id"))
	  { str=_T("ERROR 129: Неверно задан режим осей для расчета S\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }
    
	if (m_sPar==_T("S") && m_iBazaSource!=0)
	{ str=_T("ERROR 130: Неверно задан режим генератора ступенек по каналу B для расчета S\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}

	  if (m_sPar==_T("S") && (m_irazv != IMP_POS && m_irazv!=IMP_NEG && m_irazv != DC_POS && m_irazv!=DC_NEG))//---!
	  { str=_T("ERROR 131: Неверно задан вид развертки для расчета S\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }

	  if (m_iQue1==COLLECTOR && m_sPar==_T("Beta") && //(
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisX &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)) //||
	  { m_bBeta = true; m_uFunc = 2;}//---!
	  if (m_iQue1==COLLECTOR && m_sPar==_T("Beta") && //(
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisY &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)) //||
	  { m_bBeta = true; m_uFunc = 3;}//---!

	  if ((m_iQue1==BAZA && m_sPar==_T("Beta") && 
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisX &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)))
	  { m_bBeta = true; m_uFunc = 2;}//---!
	  if ((m_iQue1==BAZA && m_sPar==_T("Beta") && 
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisY &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)))
	  { m_bBeta = true; m_uFunc = 3;}//---!


 	  if (m_sPar==_T("Beta") &&  m_iBazaSource==1 &&
		 (m_sAxisX!=m_sKolMeasI || m_sAxisY!=m_sBazaSource))
	  { str=_T("ERROR 129: Неверно задан режим осей для расчета Beta\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }
     if (m_sPar==_T("Beta") && m_iBazaSource!=1)
	  { str=_T("ERROR 130: Неверно задан режим генератора ступенек по каналу B для расчета Beta\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }
	  if (m_sPar==_T("Beta") && (m_irazv != IMP_POS && m_irazv!=IMP_NEG && m_irazv != DC_POS && m_irazv!=DC_NEG))//---!
	  { str=_T("ERROR 131: Неверно задан вид развертки для расчета Beta\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }

	  if (m_iQue1==COLLECTOR && m_sPar==_T("Betai") && //(
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisX &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)) //||
	  { m_bBetai = true; m_uFunc = 2;}//---!
	  if (m_iQue1==COLLECTOR && m_sPar==_T("Betai") && //(
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisY &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)) //||
	  { m_bBetai = true; m_uFunc = 3;}//---!

	  if ((m_iQue1==BAZA && m_sPar==_T("Betai") && 
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisX &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)))
	  { m_bBetai = true; m_uFunc = 2;}//---!
	  if ((m_iQue1==BAZA && m_sPar==_T("Betai") && 
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 m_sAxisX==m_sKolMeasI && m_sAxisY==m_sBazaSource && m_iBazaSource==1 && m_sArg==m_sAxisY &&
		(m_irazv == IMP_POS || m_irazv==IMP_NEG  || m_irazv == DC_POS || m_irazv==DC_NEG)))
	  { m_bBetai = true; m_uFunc = 3;}//---!
	  if (m_iQue1==BAZA && m_sPar==_T("Betai") && 
		(m_nelement== NPN || m_nelement== PNP|| m_nelement== NPN4 || m_nelement== PNP4) &&
		 (m_sAxisX!=m_sKolMeasI || m_sAxisY!=m_sBazaSource))
	  { str=_T("ERROR 129: Неверно задан режим осей для расчета Betai\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }
      
	  if (m_sPar==_T("Betai") && m_iBazaSource!=1)
	  { str=_T("ERROR 130: Неверно задан режим генератора ступенек по каналу B для расчета Betai\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }
	  if (m_sPar==_T("Betai") && (m_irazv != IMP_POS && m_irazv!=IMP_NEG && m_irazv != DC_POS && m_irazv!=DC_NEG))//---!
	  { str=_T("ERROR 131: Неверно задан вид развертки для расчета Betai\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;
	  }

	if (!m_sPar.IsEmpty() && m_sPar==_T("R") && // m_iQue1==COLLECTOR &&
		(m_sAxisX!=m_sKolMeasU || m_sAxisY!=m_sKolMeasI) )
	{
		str=_T("ERROR 129: Неверно задан режим осей для расчета R\r\n");
	    array_error.SetAtGrow(n_er,str); 
	    n_er++;	
	}

	if (!m_sPar.IsEmpty() && m_sPar==_T("R")&& // m_iQue1==COLLECTOR &&
		m_sAxisX==m_sKolMeasU && m_sAxisY==m_sKolMeasI && 
		m_sArg==m_sAxisX) {m_bR = TRUE; m_uFunc = 4;}//R от pM1
	if (!m_sPar.IsEmpty() && m_sPar==_T("R")&&  //m_iQue1==COLLECTOR &&
		m_sAxisX==m_sKolMeasU && m_sAxisY==m_sKolMeasI && 
		m_sArg==m_sAxisY) {m_bR = TRUE; m_uFunc = 2;}//R от pM2

		if (!m_sPar.IsEmpty() && m_sPar==_T("R")&&  m_sArg.IsEmpty()) {m_bR = TRUE; m_uFunc = 5;}// не считать параметр
	if (!m_sPar.IsEmpty() && m_sPar==m_sAxisY && m_sArg==m_sAxisX) m_uFunc = 0;//найти значение функции по значению аргумента
	if (!m_sPar.IsEmpty() && m_sPar==m_sAxisX &&  m_sArg==m_sAxisY) m_uFunc = 1;  //найти значение аргумента по значению функции

	if (!m_sArg.IsEmpty() && !m_sPar.IsEmpty() && m_sUr1.IsEmpty() && m_sUr2.IsEmpty())
	{ str=_T("ERROR 111: Не заданы значения аргумента для расчета параметра\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}

	if (!m_sPar.IsEmpty() && (m_sPar==_T("S")||	m_sPar==_T("Beta")||m_sPar==_T("Betai")) &&
		m_iKommutator==1)
	{ str=_T("ERROR 108: При внешней коммутации расчет параметра запрещен\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}

	n_Step = 1;
	bGenerationStup = false;
	m_uTp = 0;
	str.Format(_T("на ступеньке № %u"),m_uTp);
	m_sStupN = str;
	if (m_uStepBaza>0 &&  m_uStepDop>0) 
	{ str=_T("ERROR 113: Запрещены ступеньки по каналу В и S одновременно\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	  n_Step = 1;
	  bGenerationStup = false;
	}
	else
	{ // нахождение n_Step и определение признака генерации ступенек
		if (m_iQue1==COLLECTOR)
		{
		if (m_uStepBaza==0 && m_uStepDop==0 && m_bFamilyVkl==true)
		{ str=_T("ERROR 112: Не заданы ступеньки по каналу В или S\r\n");
		  array_error.SetAtGrow(n_er,str); 
		  n_er++;
		  n_Step = 1; m_uTp = 0; bGenerationStup = false;
		}
		if (m_uStepBaza==0 && m_uStepDop==0 && m_bFamilyVkl==false) {n_Step = 1; m_uTp = 0; bGenerationStup = false;}
		if (m_uStepBaza>0 && m_uStepDop==0)
		{ 
			str.Format(_T("на ступеньке № %u"),m_uStepBaza);
			m_sStupN = str;
			if (m_bFamilyVkl) { n_Step = m_uStepBaza; m_uTp = n_Step; bGenerationStup = true;}
					   else { n_Step = 1; m_uTp = 1; bGenerationStup = false;}
		}
		if (m_uStepDop>0 && m_uStepBaza==0)
		{ 
			str.Format(_T("на ступеньке № %u"),m_uStepDop);
			m_sStupN = str;
			if (m_bFamilyVkl) { n_Step = m_uStepDop; m_uTp = n_Step; bGenerationStup = true;}
					   else { n_Step = 1; m_uTp = 1; bGenerationStup = false;}
		}
		}//end if (m_iQue1==COLLECTOR)
		
		if (m_iQue1==BAZA)
		{  n_Step = 1; m_uTp = 1; bGenerationStup = false;
			if (m_uStepBaza==0) m_uStepBaza=1;
			uNpointsLupa = m_uStepBaza * 10;
			m_sStupN = _T("");
		}//end if (m_iQue1==BAZA)
		else
		{//26.08.15 if (m_iQue1==COLLECTOR)
			if (m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG)
			 uNpointsLupa = uNpointsLupaDC;
			else if (m_bLooping) uNpointsLupa = 100;
								else uNpointsLupa = 200;
		}
	}


	Ur1 = GetNumberFromString(m_sUr1);
	Ur2 = GetNumberFromString(m_sUr2);
	Porog_min = GetNumberFromString(m_sMin);
	Porog_max = GetNumberFromString(m_sMax);
	
	if (!m_sMax.IsEmpty() && !m_sMin.IsEmpty() && Porog_max<=Porog_min)
	{ str=_T("ERROR 117: Неверно заданы пороговые значения MIN и MAX\r\n");
	  array_error.SetAtGrow(n_er,str);
	  n_er++;
	}

	if (m_sPar=="Ic" && m_sArg=="Uc" && 
	   (!m_sUr1.IsEmpty()) && m_uTp!=0 ) m_bErli=TRUE;
	
	int er = 0;
	if (!n_er)
	{ er = Argument();
	  if (er==1)
		{ str.Format(_T("ERROR 108: Неизвестная характеристика %s для расчета\r\n"),m_sArg);
		  //str=_T("ERROR 108: Неизвестная характеристика в качестве аргумента для расчёта\r\n");
		  array_error.SetAtGrow(n_er,str); 
		  n_er++;
		}
	  if (er==2)
		{ str=_T("ERROR 118: X_min = 0.0 и X_max = 0.0 \r\n");
		  array_error.SetAtGrow(n_er,str); 
		  n_er++;
		}
	}
	if (!n_er)
	{ er = Function();
	  if (er==1)
		{ str.Format(_T("ERROR 108: Неизвестная характеристика %s для расчета\r\n"),m_sPar);
		  //str=_T("ERROR 109: Неизвестная характеристика в качестве функции для расчёта\r\n");
		  array_error.SetAtGrow(n_er,str); 
		  n_er++;
		}
	  if (er==2)
		{ str=_T("ERROR 119: Y_min = 0.0 и Y_max = 0.0 \r\n");
		  array_error.SetAtGrow(n_er,str); 
		  n_er++;
		}
	}
	if (!n_er)
	{ if (bMeasDiff || bStatusDiff)
		{ if (iX!=0 || iY!=0 || m_bKolY!=false || m_bKolX!=false)
			{ str=_T("ERROR 116: Неверно заданы Х и Y для измерения разности ВАХ\r\n");
			  array_error.SetAtGrow(n_er,str);
			  n_er++;
			}
		}
	}
	//проверка на "крест" и установка бита Q18 (база и доп. канал)
	//=============================================================
	if (!n_er)
	{ 
	  if (m_iCircuitBaza==1)
	  { //база
		if (m_iBazaSource==1) a=mas_d_range_ampl_bazadop_I[m_iAmplitudeStepBaza];//valueAmplBaza;
		else a=mas_d_range_ampl_bazadop_U[m_iAmplitudeStepBaza];//valueAmplBaza;
		b=valueBiasBaza;
		if (sign_ampl_baza==1) {a=a*(-1.0); b=b*(-1.0);}
		if (m_bCheckBazaRejHigh) 
		{ if (m_iQue1==COLLECTOR)  k = fabs(a*m_uStepBaza*0.1+b);
							  else k = fabs(a*m_uStepBaza+b);	//m_iQue1==BAZA
		}
		else k = fabs(a*m_uStepBaza+b);//fabs(valueAmplBaza*m_uStepBaza+valueBiasBaza);
		if (m_iBazaSource==1)
		{ //источник I
		    if (fabs(valueLimitBaza)>8.0 && k > 0.110)
			{ str=_T("ERROR 120: Недопустимый уровень ограничения напряжения в канале В\r\n");
		      array_error.SetAtGrow(n_er,str); 
		      n_er++;
			}
		    if (fabs(valueLimitBaza)<=8 && k > 2.75)
			{ str=_T("ERROR 121: Недопустимый уровень тока в канале В\r\n");
		      array_error.SetAtGrow(n_er,str); 
		      n_er++;
			}

		    if (((m_irazv==IMP_NEG || m_irazv==IMP_POS) && m_iRangLimitBaza<3)||//if диапазон ограничения > 4 V 
			   ((m_irazv==SYN_NEG || m_irazv==SYN_POS) && m_bBazaVkl && m_iRangLimitBaza<3))
			{ str=_T("ERROR 137: Неверно задан диапазон ограничения напряжения в канале В в импульсном режиме\r\n");
		      array_error.SetAtGrow(n_er,str); 
		      n_er++;
			}


		}//end of I
		if (m_iBazaSource==0)
		{ //источник U
		  if (fabs(valueLimitBaza)>0.110 && k > 8)
		  { str=_T("ERROR 122: Недопустимый уровень ограничения тока в канале В\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		}//end of U
	  }//end of база

	if (m_iCircuitDop==1 && m_iKnotDop==1)
	  { //доп.канал
		  if (m_iDopSource==1)a=mas_d_range_ampl_bazadop_I[m_iAmplitudeStepDop];//valueAmplBaza;
		  else a=mas_d_range_ampl_bazadop_U[m_iAmplitudeStepDop]; 
		  b=valueBiasDop;

		  if (sign_ampl_dop==1) {a=a*(-1.0); b=b*(-1.0);}
		k = fabs(a*m_uStepDop+b);//fabs(valueAmplDop*m_uStepDop+valueBiasDop);
		if (m_iDopSource==0)
		{ //источник U

	if (valueLimitDop>0.012)
		  { str=_T("ERROR 123: Недопустимый уровень ограничения тока в канале S\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		}
	    else
		{ //источник I
/*		  if (k > 0.011)
		  { str=_T("ERROR 124: Недопустимый уровень тока в канале S\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }

*/		  if (valueLimitDop>44.0 && k > 0.010)
		  { str=_T("ERROR 124: Недопустимый уровень тока в канале S\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		}
	  }//end of доп.канал
	}//end проверка на "крест" (база и доп. канал)
	//============================================

	//проверка на высокое напряжение
	//===============================
m_dLimitKolU = mas_d_rangeKolSourceU[m_iKolSourceU+bias_KolSourceU];
	switch(m_irazv)//---!
	{ case IMP_POS: case DC_POS: case SYN_POS:break;
	  case AC: m_dLimitKolU/=2.0;break;
	  case DC_NEG: case IMP_NEG: case SYN_NEG: m_dLimitKolU*=(-1.0); break;
	}
	valueLimitKolU=(m_dLimitKolU/100)*m_iLimitKolU;
	if (fabs(valueLimitKolU)>=40.0) bSignal = TRUE;
							   else bSignal = FALSE;
	//========================
	//проверка на "крест" (коллектор)
	//===================================
	if (n_IPPP==0)
	{//ИППП-3
		if ( m_iKolSourceU==0 && (fabs(valueLimitKolI)>12.0))//20V
		  { str=_T("ERROR 125: Превышен порог ограничения тока(12 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==1 && (fabs(valueLimitKolI)>1.6))//100V
		  { str=_T("ERROR 125: Превышен порог ограничения тока(1.6 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==2 && (fabs(valueLimitKolI)>0.28))//500V
		  { str=_T("ERROR 125: Превышен порог ограничения тока(0.28 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==3 && (fabs(valueLimitKolI)>0.07))//2000V
		  { str=_T("ERROR 125: Превышен порог ограничения тока (0.07 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
	}//end of ИППП-3
	if ((n_IPPP==1) && (n_Switch==0))
	{//ИППП-3/1
//	  if (m_itipimp!=TIP_IMP)
	  if (m_irazv != IMP_POS && m_irazv!=IMP_NEG)
	  {	if ((m_iKolSourceU==0 || m_iKolSourceU==1) && (fabs(valueLimitKolI)>12.0))//10V или 20V
		  { str=_T("ERROR 125: Превышен порог ограничения тока (12 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==2 && (fabs(valueLimitKolI)>2.2))//100V
		  { str=_T("ERROR 125: Превышен порог ограничения тока (2.2 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==3 && (fabs(valueLimitKolI)>0.45))//500V
		  { str=_T("ERROR 125: Превышен порог ограничения тока (0.45 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==4 && (fabs(valueLimitKolI)>0.07))//2000V
		  { str=_T("ERROR 125: Превышен порог ограничения тока (0.07 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
	  }
//	  if (m_itipimp==TIP_IMP)
	  if (m_irazv == IMP_POS || m_irazv==IMP_NEG)
	  {	if (m_iKolSourceU==0 && (fabs(valueLimitKolI)>22.0))//20V
		  { str=_T("ERROR 125: Превышен порог ограничения тока (22 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==1 && (fabs(valueLimitKolI)>4.4))//100V
		  { str=_T("ERROR 125: Превышен порог ограничения тока(4.4 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==2 && (fabs(valueLimitKolI)>0.55))//500V
		  { str=_T("ERROR 125: Превышен порог ограничения тока(0.55 A) в канале С\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
		if ( m_iKolSourceU==3 && (fabs(valueLimitKolI)>0.11))//2000V
		  { str=_T("ERROR 125: Превышен порог ограничения тока(0.11 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
	  }
	}//end of ИППП-3/1
	if ((n_IPPP==1) && (n_Switch!=0))
	{ if (m_iKolSourceU==0 && (fabs(valueLimitKolI)>120.0))//10V
		  { str=_T("ERROR 125: Превышен порог ограничения тока (120 A) в канале C\r\n");
		    array_error.SetAtGrow(n_er,str); 
		    n_er++;
		  }
	}
	if (m_iQue1==COLLECTOR && m_iLimitKolI>0 && m_iLimitKolI>=m_iLimitKolU)
	{ str=_T("ERROR 127: Нижний порог ограничения в канале С превышает верхний\r\n");
	  array_error.SetAtGrow(n_er,str); 
	  n_er++;
	}

	//end проверка на "крест" (коллектор)
	//===================================

// K40 "Трехполюсник?" и К37
  if (m_nelement<= RES) { //двухполюсники
						  K40[1] = 0; //импульс в канале C
						  K37[1] = 0; //нет имульса в канале В
						}
				   else { //трех- четырех- полюсники
					   
					      K40[1]=0;

//						 if (m_nelement==TS) K40[1]=0; //импульс в канале C - тиристор
//									else     K40[1]=1; //импульс в канале В

						  if (m_irazv==DC_POS || m_irazv==DC_NEG)	K37[1] = 0;	//нет импульса в канале В
						  if( m_irazv==SYN_POS || m_irazv==SYN_NEG)
						  { //импульс по переключателю
							if (m_bBazaVkl) K37[1] = 1;	//импульс вкл.
									  else  K37[1] = 0; //имульс выкл.
						  }
						  if ((m_irazv==IMP_POS || m_irazv==IMP_NEG) && (!bAutoSetNull && !bKalibr))
						  { 
						     if (m_nelement==TS) K40[1]=0; //импульс в канале C - тиристор
							     		else     K40[1]=1; //импульс в канале В
 	                         if (m_iBazaSource==1)
							 { if (m_iAmplitudeStepBaza<=13) K37[1] = 1;	//источник I и Ib>=1mA => импульс вкл.
								else
								{ K37[1] = 0; //имульс выкл.
                                  str=_T("WARNING: В канале B установите амплитуду ступени 50 uА и выше\r\n");
								  array_error.SetAtGrow(n_er,str); 
								  n_er++;
								}
							 }
							 if (m_iBazaSource==0)
							 { if (m_iRangLimitBaza<=13) K37[1] = 1; //источник U и ограничение Ib>=1mA => импульс вкл.
								else 
								{ K37[1] = 0; //имульс выкл.
								  str=_T("WARNING: В канале B установите диапазон ограничения тока 1 mA и выше\r\n");
								  array_error.SetAtGrow(n_er,str); 
								  n_er++;
								}
							 }
						  }//end of if (m_irazv==IMP_POS || m_irazv==IMP_NEG)
						}//end of трех- четырех- полюсники
  
}//0.0 if (!n_er)	
	if (n_er) 
	{ 
	    nTabTuning = 1;
	    m_TabCtrlTuning.SetCurSel(nTabTuning);
	    InitPageTuning(nTabTuning);	// отобразить панель "Ошибки"
	}
	else 
	{ 
	 m_bCheckKolRejHigh = false;	//при изменении параметров и запуску измерения отключать лупу
	 UpdateData(false);
     if(b_InitAxis) 
		 InitAxis();
	 b_InitAxis=false;

	}
	  DisplayError();
	if (m_bminusX) m_sAxisX = _T("-") + m_sAxisX;
	if (m_bminusY) m_sAxisY = _T("-") + m_sAxisY;
return n_er;
}

void CLineDlg::DisplayError()
{  //индикация ошибок

	CString str = _T("");
	CString stre= _T("");

	CEdit* ep = (CEdit*) GetDlgItem(IDC_EDITERRORS);
/*	int nTab = m_TabCtrlTuning.GetCurSel();
    if (nTab!=2)
	{ nTab = 2;
	  m_TabCtrlTuning.SetCurSel(nTab);	//Закладка "Ошибки"
	  InitPageTuning(nTab);
	}
*/
	int n_er=array_error.GetSize();
	if (n_er)
	{ for(int i=0;i<n_er;i++)	
		{ str = array_error.GetAt(i);
		  stre=stre+str;
		}
	}
    str.Format(_T("Всего ошибок: %d\r\n"),n_er);
    stre=stre+str;
    ep ->SetWindowText(stre);

}

void CLineDlg::ClearError()
{	 
	CString str=_T("");
	array_error.RemoveAll();
}

int CLineDlg::Leksema()
{//в качестве функции расчётный массив (pM5[])
	iY = COUNT;
	int fl = 0;

return fl;
}

int CLineDlg::Function()
{//формирование данных для шкалы Y 
 //нахождение Y_min, Y_max и т. д.	  
  int n_e=0;	//признак: 1 - ошибка, 0 - нет
  int fl=0;		//флаг: 1 - аргумент найден, 0 - нет
  m_bKolY = false;
  m_bBaza = FALSE; m_iBazaMeas = m_iBazaSource;//!
  m_bDop  = FALSE; m_iDopMeas = m_iDopSource;//! 
 //поиск функции среди выходных параметров
 //---------------------------------------
//   if (m_iCircuitKol==1  && m_sAxisY==m_sKolMeasI)  {Fun_Y(COLLECTOR);fl=1;}
   if (m_sAxisY==m_sKolMeasI)  {m_bKolY= false; Fun_Y(COLLECTOR);fl=1;}
   if (m_sAxisY==m_sKolMeasU)  {m_bKolY= true; Fun_Y(COLLECTOR);fl=1;}
   if (m_iCircuitBaza==1 && m_sAxisY==m_sBazaMeasLimit)  {m_bBaza = true; Fun_Y(BAZA);fl=1;} //{m_bBaza = FALSE; Fun_Y(BAZA);fl=1;}	
   if (m_iCircuitDop==1  && m_sAxisY==m_sDopMeasLimit)  {m_bDop = true;  Fun_Y(DOP); fl=1;}//  {m_bDop = FALSE;  Fun_Y(DOP); fl=1;}	
 
   if (!fl)
   { //поиск функции среди входных параметров
     //--------------------------------------
	if (m_iCircuitBaza==1 && m_sAxisY==m_sBazaSource) {m_bBaza = false; Fun_Y(BAZA);fl=1;} //{m_bBaza = TRUE; Fun_Y(BAZA);fl=1;}	
	if (m_iCircuitDop==1  && m_sAxisY==m_sDopSource)  {m_bDop = false;  Fun_Y(DOP); fl=1;} //{m_bDop = TRUE;  Fun_Y(DOP); fl=1;}	
   }
//   if (!fl) fl = Leksema();	
   if (fl) 
   { if (Y_min==0.0 && Y_max==0.0) n_e = 2;
	 else SubGetAxes(Y_min,Y_max,&m_iY,&expo_Y);
   }
   else  n_e = 1;
return n_e;
}

void CLineDlg::Fun_Y(int Chanel)
{//нахождение iY, rej_Y, Y_min, Y_max
 //if m_bBaza || m_bDop ==0 => меряем то, что генерируем

   double Y_temp = 0.0;

   iY = Chanel;		//Chanel - с какого канала берётся результат измерения по оси Y (COLLECTOR,BAZA или DOP) 

   if (Chanel==COLLECTOR)
   {//-------------------
	 if (m_bKolY==false)
	 {
   	   rej_Y   = 1;	//функция - ток на коллекторе
	   switch(m_iButNull)
	   { case LEFT:   Y_min = 0.0;
				    Y_max = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI];//*2];
				    break;
	     case CENTER: Y_min = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI]*(-1.0);//*2] * (-1.0);
					Y_max = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI];//*2];
					break;
	     case RIGHT:  Y_min = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI]*(-1.0);//*2] *(-1.0);
					Y_max = 0.0;
					break;
	   }
	 }
	 else
	 {
	   rej_Y   = 0;	// U
	   switch(m_iButNull)
	   { case LEFT:   Y_min = 0.0;
				    Y_max = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU];//
				    break;
	     case CENTER: Y_min = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU] * (-1.0);//
					Y_max = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU];//
					break;
	     case RIGHT:  Y_min = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU] *(-1.0);//
					Y_max = 0.0;
					break;
	   }
	 }
   }//end of  if (Chanel==COLLECTOR)

   if (Chanel==BAZA)
   {//--------------
//	 if (m_bBaza) 
	 if (!m_bBaza) 
	 { //меряем то, что генерируем
	   rej_Y = m_iBazaSource;	
	   m_iBazaMeas = m_iBazaSource;
	 }
	 else 
	 { //меряем характеристику, противоположную генерируемой
	   m_iBazaMeas = !m_iBazaSource;
	   rej_Y = m_iBazaMeas;      
	 }
	 switch(rej_Y)
	 { case 0: //измеритель U
//				if (m_bBaza){//диапазон измерителя = диапазону источника
			   if (!m_bBaza){//диапазон измерителя = диапазону источника
					          Fun1_Y(); //Y_temp = mas_d_range_meas_bazadop_U[m_iAmplitudeStepBaza];
							  m_iRangMeasBaza = m_iAmplitudeStepBaza;
							}
						else{//диапазон измерителя = диапазону ограничения
							  Y_temp = mas_d_range_meas_bazadop_U[m_iRangLimitBaza];//+mas_d_range_meas_bazadop_U[m_iRangLimitBaza]/10  ???
							  m_iRangMeasBaza = m_iRangLimitBaza;
							}
				break;
	   case 1: //измеритель I
//				if (m_bBaza){//диапазон измерителя = диапазону источника
				if (!m_bBaza){//диапазон измерителя = диапазону источника
							  Fun1_Y(); //Y_temp = mas_d_range_meas_bazadop_I[m_iAmplitudeStepBaza];
							  m_iRangMeasBaza = m_iAmplitudeStepBaza;
							}
						else{//диапазон измерителя = диапазону ограничения
							  Y_temp = mas_d_range_meas_bazadop_I[m_iRangLimitBaza];//+mas_d_range_meas_bazadop_I[m_iRangLimitBaza]/10  ???
							  m_iRangMeasBaza = m_iRangLimitBaza;
							}
				break;
	 }
//	 if (!m_bBaza)
	 if (m_bBaza)
	 switch(m_iButNull)
	 { case LEFT:   Y_min = 0.0;
				    Y_max = Y_temp;
				    break;
	   case CENTER: Y_min = Y_temp * (-1.0);
					Y_max = Y_temp;
					break;
	   case RIGHT:  Y_max = 0.0;
					Y_min = Y_temp *(-1.0);
					break;
	  }

   }//end of if (Chanel==BAZA)

   if (Chanel==DOP)
   { //-------------
//	 if (m_bDop) 
	 if (!m_bDop) 
	 {//меряем то,что генерируем
	   rej_Y = m_iDopSource;	
	   m_iDopMeas = m_iDopSource;
	 }
	 else
	 { //меряем характеристику, противоположную генерируемой
	   m_iDopMeas = !m_iDopSource;
	   rej_Y = m_iDopMeas;
	 }
	 switch(rej_Y)
	 { case 0: //измеритель U
//				if (m_bDop) {//диапазон измерителя = диапазону источника
				if (!m_bDop) {//диапазон измерителя = диапазону источника
							  Fun2_Y(); //Y_temp = mas_d_range_meas_bazadop_U[m_iAmplitudeStepDop];
							  m_iRangMeasDop = m_iAmplitudeStepDop;
							}
						else{//диапазон измерителя = диапазону ограничения
							  Y_temp = mas_d_range_meas_bazadop_U[m_iRangLimitDop];//+mas_d_range_meas_bazadop_U[m_iRangLimitDop]/10  ???
							  m_iRangMeasDop = m_iRangLimitDop;
							}
				break;
	   case 1: //измеритель I
//				if (m_bDop) {//диапазон измерителя = диапазону источника
				if (!m_bDop) {//диапазон измерителя = диапазону источника
							  Fun2_Y();  //Y_temp = mas_d_range_meas_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
							  m_iRangMeasDop = m_iAmplitudeStepDop+bias_Dop_I;
							}
						else{//диапазон измерителя = диапазону ограничения
							  Y_temp = mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I];//+mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I]/10  ???
							  m_iRangMeasDop = m_iRangLimitDop+bias_Dop_I;
							}
				break;
	 }
//	 if (!m_bDop) 
	 if (m_bDop) 
	 switch(m_iButNull)
	  { case LEFT:   Y_min = 0.0;
					 Y_max = Y_temp;
				     break;
	    case CENTER: Y_min = Y_temp * (-1.0);
					 Y_max = Y_temp;
					 break;
	    case RIGHT:  Y_max = 0.0;
					 Y_min = Y_temp *(-1.0);
					 break;
	  }

   }//end of if (Chanel==DOP)
   if (m_bminusY)
   {
   }//end of (m_bminusY)
   	rej_Y_Meas = rej_Y;			//характеристика, отображаемая по оси Y: 0 - U, 1 - I при измерении

}

void CLineDlg::Fun1_Y()
{//Y_min, Y_max среди входных параметров базы
 
   UINT i = 0;
   double Y_sh = 0.0;
   
   switch(rej_Y)
	{ case 0: //источник базы - U
			    Y_sh = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepBaza];
				Y_axis[0]  = mas_d_range_bias_bazadop_U[m_iAmplitudeStepBaza];//valueBiasBaza;	//Y_min  
				break;
	  case 1: //источник базы - I
			    Y_sh = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepBaza];
				Y_axis[0]  = mas_d_range_bias_bazadop_I[m_iAmplitudeStepBaza];//valueBiasBaza;	//Y_min  
				break;
	}
//	if (m_irazv<=2) //---!
	if (m_irazv<=AC) //---!
	{ //развёртка "+"
	  if (m_iPolarityStepBaza==1) 
		{ //амплитуда: обратная (=> "-")
			Y_sh = Y_sh * (-1.0); 
	        if (m_iPolarityBiasBaza==0)//смещение: прямая (=> "-")
				Y_axis[0] = Y_axis[0] * (-1.0);
		}
		else
		{//амплитуда: прямая (т.е. +)
	        if (m_iPolarityBiasBaza==1)//смещение: обратное (=> "-")
				Y_axis[0] = Y_axis[0] * (-1.0);
		}
	}
//	if (m_irazv>2)//---!
	if (m_irazv>AC)//---!
	{ //развёртка "-"
	  if (m_iPolarityStepBaza==0) 
		{ //амплитуда: прямая (=> "-")
			Y_sh = Y_sh * (-1.0); 
	        if (m_iPolarityBiasBaza==0)//смещение: прямая (=> "-")
				Y_axis[0] = Y_axis[0] * (-1.0);
		}
		else
		{//амплитуда: обратная (т.е. +)
	        if (m_iPolarityBiasBaza==1)//смещение: обратное (=> "-")
				Y_axis[0] = Y_axis[0] * (-1.0);
		}
	}
//	for (i=1;i<=m_uStepBaza+1;i++) Y_axis[i]=Y_axis[i-1]+Y_sh;
	for (i=1;i<=11;i++) Y_axis[i]=Y_axis[i-1]+Y_sh;

	Y_max = Y_axis[0];
	Y_min = Y_axis[0];
//	for (i=1;i<=m_uStepBaza+1;i++)
	for (i=1;i<=11;i++)
	{ Y_max=max(Y_axis[i],Y_max);
	  Y_min=min(Y_axis[i],Y_min);
	}

	 if (Y_min<0.0 && Y_max<=0.0) 
	 { m_iButNull = RIGHT;
	   Y_max = 0.0;
	 }
	 if(Y_min<0.0 && Y_max>0.0)
	 { m_iButNull = CENTER;
	   double Y_temp = max(fabs(Y_min),fabs(Y_max));
	   Y_min = Y_temp * (-1.0);
	   Y_max = Y_temp;
	 }
	 if (Y_min>=0.0 && Y_max>0.0)
	 { m_iButNull = LEFT;
	   Y_min = 0.0;
	 }
//	 UpdateData(false);
/*	switch(m_iButNull)
	{ case LEFT:   Y_min = 0.0;
				   break;
	  case RIGHT:  Y_max = 0.0;
				   break;
	}
*/
}

void CLineDlg::Fun2_Y()
{//Y_min, Y_max среди входных параметров доп. канала
 
   UINT i = 0;
   double Y_sh = 0.0;
   
   switch(rej_Y)
	{ case 0: //источник доп. канала - U
			    Y_sh = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepDop];
				Y_axis[0]  = mas_d_range_bias_bazadop_U[m_iAmplitudeStepDop];//valueBiasBaza;	//Y_min  
				break;
	  case 1: //источник доп. канала - I
			    Y_sh = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
				Y_axis[0]  = mas_d_range_bias_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];//valueBiasBaza;	//Y_min  
				break;
	}
//	if (m_irazv<=2) //---!
	if (m_irazv<=AC) //---!
	{ //развёртка "+"
	  if (m_iPolarityStepDop==1) 
		{ //амплитуда: обратная (=> "-")
			Y_sh = Y_sh * (-1.0); 
	        if (m_iPolarityBiasDop==0)//смещение: прямая (=> "-")
				Y_axis[0] = Y_axis[0] * (-1.0);
		}
		else
		{//амплитуда: прямая (т.е. +)
	        if (m_iPolarityBiasDop==1)//смещение: обратное (=> "-")
				Y_axis[0] = Y_axis[0] * (-1.0);
		}
	}
//	if (m_irazv>2)//---!
	if (m_irazv>AC)//---!
	{ //развёртка "-"
	  if (m_iPolarityStepDop==0) 
		{ //амплитуда: прямая (=> "-")
			Y_sh = Y_sh * (-1.0); 
	        if (m_iPolarityBiasDop==0)//смещение: прямая (=> "-")
				Y_axis[0] = Y_axis[0] * (-1.0);
		}
		else
		{//амплитуда: обратная (т.е. +)
	        if (m_iPolarityBiasDop==1)//смещение: обратное (=> "-")
			    Y_axis[0] = Y_axis[0] * (-1.0);
		}
	}
	for (i=1;i<=11;i++) Y_axis[i]=Y_axis[i-1]+Y_sh;

	Y_max = Y_axis[0];
	Y_min = Y_axis[0];
	for (i=1;i<=11;i++)
	{ Y_max=max(Y_axis[i],Y_max);
	  Y_min=min(Y_axis[i],Y_min);
	}

	 if (Y_min<0.0 && Y_max<=0.0) 
	 { m_iButNull = RIGHT;
	   Y_max = 0.0;
	 }
	 if(Y_min<0.0 && Y_max>0.0)
	 { m_iButNull = CENTER;
	   double Y_temp = max(fabs(Y_min),fabs(Y_max));
	   Y_min = Y_temp * (-1.0);
	   Y_max = Y_temp;
	 }
	 if (Y_min>=0.0 && Y_max>0.0)
	 { m_iButNull = LEFT;
	   Y_min = 0.0;
	 }
/*
	switch(m_iButNull)
	{ case LEFT:   Y_min = 0.0;
				   break;
	  case RIGHT:  Y_max = 0.0;
				   break;
	}
*/
}


int CLineDlg::Argument()
{//формирование данных для шкалы X 
 //нахождение X_min, X_max и т. д.	  
  int n_e=0;	//признак: 1 - ошибка, 0 - нет
  int fl=0;		//флаг: 1 - аргумент найден, 0 - нет
  m_bKolX= false;//признак 0 - по оси X - Uc,1 - по оси X - Ic
 //поиск аргумента среди входных параметров
 //----------------------------------------
//   if (m_iCircuitKol==1 && m_sAxisX==m_sKolMeasU)   {Arg_X(COLLECTOR);fl=1;}
   if (m_sAxisX==m_sKolMeasU)   {m_bKolX= false; Arg_X(COLLECTOR);fl=1;}
   if (m_sAxisX==m_sKolMeasI)   {m_bKolX= true;  Arg_X(COLLECTOR);fl=1;}
   if (m_iCircuitBaza==1 && m_sAxisX==m_sBazaSource){Arg_X(BAZA);fl=1;}	
   if (m_iCircuitDop==1 && m_sAxisX==m_sDopSource)  {Arg_X(DOP); fl=1;}	
   if (fl) 
   { if (X_min==0.0 && X_max==0.0) n_e = 2;
	 else SubGetAxes(X_min,X_max,&m_iX,&expo_X);
   }
   else  n_e = 1;
return n_e;
}

void CLineDlg::Arg_X(int Chanel)
{//нахождение iX, rej_X, X_min, X_max
 
   UINT i = 0;
   UINT j = 0;
   double X_temp = 0.0;
   double X_sh = 0.0;
   
   iX     = Chanel;	//Chanel - канал с которого берётся результат по оси Х 
					//(он же - источник 1-ой очереди): 0-коллектор, 1-база, 2-доп.канал
					//if iX = 0 - массив результатов измерения напряжения на коллекторе,
					//if 1 или 2 - массив X_axis

//	 nStepQ1 = NSTEPQ1;	//1-ая очередь
//	 nStepQ2 = n_Step;	//2-ая очередь
	 if (m_iQue1==COLLECTOR) nStepQ1 = NSTEPQ1;	//1-ая очередь
	 if (m_iQue1==BAZA)      nStepQ1 = uNpointsLupa;
	 if (m_iQue1==DOP)       nStepQ1 = uNpointsLupa;
	 nStepQ2 = n_Step;	//2-ая очередь

   if (Chanel==COLLECTOR)
   {
	//-----------------------------------
	if (m_bKolX == 0)
	{//аргумент - напряжение на коллекторе
	 rej_X   = 0;	// U
	 switch(m_iButNull)
	 { case LEFT:   X_min = 0.0;
				    X_max = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU];//
				    break;
	   case CENTER: X_min = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU] * (-1.0);//
					X_max = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU];//
					break;
	   case RIGHT:  X_min = mas_d_rangeKolMeasU[bias_KolMeasU+m_iKolU] *(-1.0);//
					X_max = 0.0;
					break;
	 }
	}
	else
	{//аргумент - ток на коллекторе
	 rej_X   = 1;	//I
	 switch(m_iButNull)
	 { case LEFT:   X_min = 0.0;
				    X_max = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI];//*2];
				    break;
	   case CENTER: X_min = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI]*(-1.0);//*2] * (-1.0);
					X_max = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI];//*2];
					break;
	   case RIGHT:  X_min = mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI]*(-1.0);//*2] *(-1.0);
					X_max = 0.0;
					break;
	 }
	}
  }//end of  if (Chanel==COLLECTOR)

   if (Chanel==BAZA)
   {//аргумент - источник базы
	//---------------------------
	 rej_X   = m_iBazaSource;
//	 nStepQ1 = NSTEPQ1;				//m_uStepBaza+1;	//1-ая очередь
//	 nStepQ2 = m_uStepBaza+1;		//NSTEPQ1;		    //2-ая очередь
	 sign_ampl_baza = 0;	// +
	 sign_bias_baza = 0;	// +
	 switch(m_iBazaSource)
	 { case 0: //источник базы - U
			    X_sh = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepBaza];
				valueAmplBaza = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepBaza];
				break;
	   case 1: //источник базы - I
			    X_sh = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepBaza];
				valueAmplBaza = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepBaza];
				break;
	 }

	 if (m_iQue1==BAZA) 
	 { X_sh = X_sh * 0.1;
	   valueAmplBaza = valueAmplBaza *0.1;
	 }

	 valueBiasBaza=(m_dBiasBaza/1000.0)*m_iBiasBaza;

	 x_axis[0]  = valueBiasBaza;	//X_min  
//	 if (m_irazv<=2) //---!
	 if (m_irazv<=AC) //---!
	 { //развёртка "+"
		if (m_iPolarityStepBaza==1) 
		{ //амплитуда: полярность обратная (=> "-")
			sign_ampl_baza = 1;	// -
			X_sh = X_sh * (-1.0); 
	        if (m_iPolarityBiasBaza==0)//смещение: прямая (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_baza = 1;			
			}
		}
		else
		{//амплитуда: полярность прямая (т.е. +)
	        if (m_iPolarityBiasBaza==1)//смещение: обратное (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_baza = 1;			
			}
		}
	 }
//	 if (m_irazv>2)//---!
	 if (m_irazv>AC)//---!
	 { //развёртка "-"
		if (m_iPolarityStepBaza==0) 
		{ //амплитуда: прямая (=> "-")
			sign_ampl_baza = 1;	// -
			X_sh = X_sh * (-1.0); 
	        if (m_iPolarityBiasBaza==0)//смещение: прямая (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_baza = 1;			
			}
		}
		else
		{//амплитуда: обратная (т.е. +)
	        if (m_iPolarityBiasBaza==1)//смещение: обратное (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_baza = 1;			
			}
		}
	 }

	 
	 if (m_uStepBaza > 0)  x_axis[0] = x_axis[0] + X_sh;

//	 for (i=1;i<nStepQ1;i++) x_axis[i]=x_axis[i-1]+X_sh;
	 for (i=0;i<nStepQ1;i++) x_axis[i+1]=x_axis[i]+X_sh;
     if (m_bminusX)
	 { for (i=0;i<nStepQ1;i++) x_axis[i]=x_axis[i]*(-1.0);
	 }//end of (m_bminusX)


	 X_max = x_axis[0];
	 X_min = x_axis[0];
	 for (i=0;i<nStepQ1;i++)
	 { X_max=max(x_axis[i],X_max);
	   X_min=min(x_axis[i],X_min);
	 }
	 if (X_min<0.0 && X_max<=0.0) 
	 { m_iButNull = RIGHT;
	   X_max = 0.0;
	 }
	 if(X_min<0.0 && X_max>0.0)
	 { m_iButNull = CENTER;
	   X_temp = max(fabs(X_min),fabs(X_max));
	   X_min = X_temp * (-1.0);
	   X_max = X_temp;
	 }
	 if (X_min>=0.0 && X_max>0.0)
	 { m_iButNull = LEFT;
	   X_min = 0.0;
	 }

	 for (j=0;j<nStepQ2;j++)
		 for (i=0;i<nStepQ1;i++)
		X_axis[i] = x_axis[i];

   }//end of if (Chanel==BAZA)

   if (Chanel==DOP)
   {//аргумент - источник доп.канала
	//-------------------------------
	 rej_X   = m_iDopSource;
//	 nStepQ1 =	NSTEPQ1;		//m_uStepDop+1;	//1-ая очередь
//	 nStepQ2 = m_uStepDop+1;	//NSTEPQ1;		//2-ая очередь
	 sign_ampl_dop = 0;	// +
	 sign_bias_dop = 0;	// +
	 switch(m_iDopSource)
	 { case 0: //источник доп.канала - U
			    X_sh = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepDop];
				valueAmplDop = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepDop];
				break;
	   case 1: //источник доп.канала - I
			    X_sh = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
				valueAmplDop = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
				break;
	 }

	 if (m_iQue1==DOP) 
	 { X_sh = X_sh * 0.1;
	   valueAmplBaza = valueAmplBaza *0.1;
	 }
	valueBiasDop=(m_dBiasDop/1000.0)*m_iBiasDop;

	 x_axis[0]  = valueBiasDop;	//X_min  
//	 if (m_irazv<=2) //---!
	 if (m_irazv<=AC) //---!
	 { //развёртка "+"
		if (m_iPolarityStepDop==1) 
		{ //амплитуда: обратная (=> "-")
	        sign_ampl_dop = 1;	// -
			X_sh = X_sh * (-1.0); 
	        if (m_iPolarityBiasDop==0)//смещение: прямая (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_dop = 1;	// -
			}
		}
		else
		{//амплитуда: прямая (т.е. +)
	        if (m_iPolarityBiasDop==1)//смещение: обратное (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_dop = 1;	// -
			}
		}
	 }
//	 if (m_irazv>2)//---!
	 if (m_irazv>AC)//---!
	 { //развёртка "-"
		if (m_iPolarityStepDop==0) 
		{ //амплитуда: прямая (=> "-")
	        sign_ampl_dop = 1;	// -
			X_sh = X_sh * (-1.0); 
	        if (m_iPolarityBiasDop==0)//смещение: прямая (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_dop = 1;	// -
			}
		}
		else
		{//амплитуда: обратная (т.е. +)
	        if (m_iPolarityBiasDop==1)//смещение: обратное (=> "-")
			{ x_axis[0] = x_axis[0] * (-1.0);
			  sign_bias_dop = 1;	// -
			}
		}
	 }
	 
	 if (m_uStepDop > 0)  x_axis[0] = x_axis[0] + X_sh;

	 for (i=1;i<nStepQ1;i++) x_axis[i]=x_axis[i-1]+X_sh;

     if (m_bminusX)
	 { for (i=0;i<nStepQ1;i++) x_axis[i]=x_axis[i]*(-1.0);
	 }//end of (m_bminusX)

     X_max = x_axis[0];
	 X_min = x_axis[0];
	 for (i=1;i<=nStepQ1;i++)
	 { X_max=max(x_axis[i],X_max);
	   X_min=min(x_axis[i],X_min);
	 }

	 if (X_min<0.0 && X_max<=0.0) 
	 { m_iButNull = RIGHT;
	   X_max = 0.0;
	 }
	 if(X_min<0.0 && X_max>0.0)
	 { m_iButNull = CENTER;
	   X_temp = max(fabs(X_min),fabs(X_max));
	   X_min = X_temp * (-1.0);
	   X_max = X_temp;
	 }
	 if (X_min>=0.0 && X_max>0.0)
	 { m_iButNull = LEFT;
	   X_min = 0.0;
	 }

	 for (j=0;j<nStepQ2;j++)
		 for (i=0;i<nStepQ1;i++)
		X_axis[i] = x_axis[i];

  }//end of if (Chanel==DOP)

}

BOOL CLineDlg::StringToValue(const CString& sStr, double *value)
{   *value = 0.0;
	CString str;
	int n = 0;  //n-номер позиции в строке
	int fl = 0; //признак присутствия символа размерности: if 0 - V||A||s
	BOOL res = TRUE; //результат: 0 - ошибка в строке, 1 - ОК (результат в value)
				//вызов функции: например, i=StringToValue(m_sSt1,&f);
	str=sStr;
	str.TrimLeft();
	str.TrimRight();
	if (!str.IsEmpty())
	{ 
	unsigned short c=str.GetAt(0);
	if ((c<0x30 || c>0x39) && (c!=0x2B && c!=0x2D && c!=0x20)) 
	{//переменная
		//поиск переменной и получение ее значения 
		//...
		//*value=
		res=FALSE;//TRUE;
	}
	else
	{//значение с/без единиц имерения
	n=sStr.Find('p');
	if (n>0){ str=sStr.Left(n);
	          *value=atof((const char*)str.GetBuffer(str.GetLength()+1))/1000000000000.0;		  
			  str.ReleaseBuffer();
			  fl++;	
			}
	n=sStr.Find('n');
	if (n>0){ str=sStr.Left(n);
	          *value=atof((const char*)str.GetBuffer(str.GetLength()+1))/1000000000.0;		  
			  str.ReleaseBuffer();
			  fl++;	
			}
	n=sStr.Find('u');
	if (n>0){ str=sStr.Left(n);
	          *value=atof((const char*)str.GetBuffer(str.GetLength()+1))/1000000.0;		  
			  str.ReleaseBuffer();
			  fl++;	
			}
	n=sStr.Find('m');
	if (n>0){ str=sStr.Left(n);
	          *value=atof((const char*)str.GetBuffer(str.GetLength()+1))/1000.0;		  
			  str.ReleaseBuffer();
			  fl++;	
			}
	n=sStr.Find('k');
	if (n>0){ str=sStr.Left(n);
	          *value=atof((const char*)str.GetBuffer(str.GetLength()+1))*1000.0;		  
			  str.ReleaseBuffer();
			  fl++;	
			}
	n=sStr.Find('M');
	if (n>0){ str=sStr.Left(n);
	          *value=atof((const char*)str.GetBuffer(str.GetLength()+1))*1000000.0;		  
			  str.ReleaseBuffer();
			  fl++;	
			}
	n=sStr.Find('G');
	if (n>0){ str=sStr.Left(n);
	          *value=atof((const char*)str.GetBuffer(str.GetLength()+1))*1000000000.0;		  
			  str.ReleaseBuffer();
			  fl++;	
			}

	if (!fl) { 
				*value=atof((const char*)str.GetBuffer(str.GetLength()+1));		
			  str.ReleaseBuffer();
			  fl++;	
			 }
    if (fl>1) res=FALSE; //error
		 else res=TRUE;  //OK
	}
	}
return(res);    
}


void CLineDlg::OnDel() 
{
	// TODO: Add your command handler code here
	POSITION pos;
	CMod* pMod;

 m_pList     = GetList();
 m_position  = GetPosition();
 *m_position = m_pList->GetHeadPosition();

 if (!m_pList->IsEmpty())
 { 
//	if (pModSelected)//???_???
	{  
	 if (i_SelUrov==2)
		{ //уровень теста
			if (MessageBox(_T("Вы уверены, что хотите удалить тест ")+m_MOD+
		                   _T("\nдля элемента ")+m_EL+_T(" ?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
			{ if (FindTest(m_EL,m_MOD))
				{ //удалить выделенный тест 
				  Del();
		          SetModifiedFlag();
				  TreeUpdate();
				}
			  else MessageBox(_T("В списке не найден тест ")+m_MOD+
							  _T("\nдля элемента ")+m_EL+_T(" !"),NULL,MB_OK|MB_ICONEXCLAMATION);
			}
		}//end of уровень 2
		else
		{ if (i_SelUrov==1)
			{//уровень элемента
				if (MessageBox(_T("Вы уверены, что хотите удалить элемент ")+m_EL+
							   _T("\nсо всеми тестами?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
				{ //удалить все тесты для выделенного элемента
				  *m_position=m_pList->GetHeadPosition();
		          do { pMod  = m_pList->GetAt(*m_position);
		               if(!((pMod->m_EL).Compare(m_EL))) 
					   { 
				         pos=*m_position;
				         m_pList->RemoveAt(*m_position);
				         *m_position=m_pList->GetHeadPosition();
				         delete pMod;
					   }
				       else pMod =m_pList->GetNext(*m_position);
				  } while(*m_position);
		          SetModifiedFlag();
				  TreeUpdate();
				}//end of delete EL
			}//end of уровень 1
		}//end of else
	}//end of pModSelected
 }//end of !m_pList	
}

void CLineDlg::Del() 
{
	POSITION pos;
    if ((pos = *m_position) !=NULL) 
	{
      m_pList->GetNext(pos);
      if (pos == NULL) 
	  {
        pos = m_pList->GetHeadPosition();
        if (pos == *m_position) 
		{
        pos = NULL;
		}
	  }

	  CMod* ps = m_pList->GetAt(*m_position);
      m_pList->RemoveAt(*m_position);
	  //Expand
	  FConfig();
      CString str;
      int i;
	  HTREEITEM hitem=NULL;

	  str=ps->m_EL;
	  for(i=0;i<NE;i++)
	  if (str==m_masStrEl[i])
		{ hitem = m_masHEl[i]; 
		  break;
		}
	  if (hitem!=NULL)
	  { 
//		m_pTreeDlg->m_TreeCtrl.Expand(hitem,TVE_EXPAND);
//	    m_pTreeDlg->m_TreeCtrl.EnsureVisible(hitem);
		m_TreeCtrl.Expand(hitem,TVE_EXPAND);
	    m_TreeCtrl.EnsureVisible(hitem);
	  }
 
      delete ps;
	  *m_position = pos;
    }
}


void CLineDlg::OnDelAll() 
{
	// TODO: Add your command handler code here
	CString str;
	str.Format(_T("Вы уверены, что хотите удалить весь список тестов\nмодели %s ?"),strFileName);
	if (MessageBox(str,NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
	{//удалить весь список тестов
	  while (GetList()->GetHeadPosition())
	  {
        Del();
	  }
      SetModifiedFlag();
	}
}

void CLineDlg::DlgTreeUpdate()
{
    GetEntry(*m_position);
}

void CLineDlg::GetEntry(POSITION position)
{
    if (position)
	{
      CMod* pMod = m_pList->GetAt(position);
      pDlgTree->m_EL    = pMod->m_EL;
	}
    else 
	{
      ClearEntry();
    }
}

void CLineDlg::ClearEntry()
{
 	pDlgTree->m_EL    =_T("");
	pDlgTree->m_MOD   =_T("");
//	pDlgTree->m_PRIM  =_T("");
}

void CLineDlg::OnCopy() 
{
	// TODO: Add your command handler code here
	m_pList     = GetList();
	m_position  = GetPosition();
    *m_position = m_pList->GetHeadPosition();
 
 if (!m_pList->IsEmpty())
 { if (i_SelUrov==2)
	{ //копировать тест
	  if (FindTest(str_EL,str_MOD))
	  { m_bCopy = TRUE;
	    CopyInBuf(); //копировать в буфер
	  }
	  else MessageBox(_T("В списке не найден тест ")+str_MOD+_T(" для элемента ")+str_EL,NULL,MB_OK|MB_ICONERROR);
	}
   else
   { if (i_SelUrov==1)
	{ //копировать элемент
	   CopyEl(str_EL,str_EL+_T("_1"));
	}
   }
 }	
}

void CLineDlg::OnPaste() 
{
	// TODO: Add your command handler code here
	POSITION pos;
	CMod* pMod;

	m_pList     = GetList();
	m_position  = GetPosition();
    *m_position = m_pList->GetHeadPosition();
 
 if (!m_pList->IsEmpty())
 {
	if (i_SelUrov==1)//вставить тест в элемент
	{
		  *m_position=m_pList->GetHeadPosition();
		  do { pMod  = m_pList->GetAt(*m_position);
		       if(!((pMod->m_EL).Compare(str_EL))) 
				{ 
				  pos = *m_position;
				  pModBuf->m_EL = pMod->m_EL;
				  pModBuf->m_MOD = pModBuf->m_MOD +"_1";
				  CopyFromBuf();
				  break;
				}
				else pMod =m_pList->GetNext(*m_position);
			 } while(*m_position);
	}
 }
}

void CLineDlg::CopyInBuf() 
{
  if (*m_position)
	{
	  CMod* pMod = m_pList->GetAt(*m_position);
 	  pModBuf->strDateMeas = pMod->strDateMeas;
	  pModBuf->strTimeMeas = pMod->strTimeMeas;
	  pModBuf->m_EL    = pMod->m_EL;
	  pModBuf->m_MOD   = pMod->m_MOD;// + "_1";
 	  pModBuf->m_PRIM  = pMod->m_PRIM;//_T("");
	//коллектор
	pModBuf->m_nelement		      = pMod->m_nelement;
	pModBuf->m_iCircuitKol		  = pMod->m_iCircuitKol;
	pModBuf->m_iCircuitBaza	      = pMod->m_iCircuitBaza;
	pModBuf->m_iCircuitDop		  = pMod->m_iCircuitDop;
	    pModBuf->m_iKommutator	= pMod->m_iKommutator;
	    pModBuf->m_iGnezdoC	= pMod->m_iGnezdoC;
	    pModBuf->m_iGnezdoB	= pMod->m_iGnezdoB;
	    pModBuf->m_iGnezdoE	= pMod->m_iGnezdoE;
	pModBuf->m_irazv              = pMod->m_irazv;
	pModBuf->m_idlitimp			  = pMod->m_idlitimp;
	pModBuf->m_iKolU			  = pMod->m_iKolU;
	pModBuf->m_iKolI			  = pMod->m_iKolI;
	pModBuf->m_iKolSourceU		  = pMod->m_iKolSourceU;
	pModBuf->m_iLimitKolU		  = pMod->m_iLimitKolU;
	pModBuf->m_iLimitKolI		  = pMod->m_iLimitKolI;
	pModBuf->m_bCheckKolRejHigh   = pMod->m_bCheckKolRejHigh;
	pModBuf->m_itipimp			  = pMod->m_itipimp;
	pModBuf->m_bCheckPauza        = pMod->m_bCheckPauza;
	//база
	pModBuf->m_iBazaSource        = pMod->m_iBazaSource;
	pModBuf->m_bBazaVkl		      = pMod->m_bBazaVkl;
	pModBuf->m_iAmplitudeStepBaza = pMod->m_iAmplitudeStepBaza;
	pModBuf->m_uStepBaza          = pMod->m_uStepBaza;
	pModBuf->m_iPolarityStepBaza  = pMod->m_iPolarityStepBaza;
	pModBuf->m_iQue1      = pMod->m_iQue1;
	pModBuf->m_iBiasBaza          = pMod->m_iBiasBaza;		
	pModBuf->m_iPolarityBiasBaza  = pMod->m_iPolarityBiasBaza;
	pModBuf->m_iBazaMeas		  = pMod->m_iBazaMeas;
	pModBuf->m_iRangLimitBaza     = pMod->m_iRangLimitBaza;
	pModBuf->m_iLimitBaza         = pMod->m_iLimitBaza;	
	pModBuf->m_bCheckBazaRejHigh  = pMod->m_bCheckBazaRejHigh;
	//доп.канал
	pModBuf->m_iDopSource		 = pMod->m_iDopSource;
//	pModBuf->m_bDopVkl		     = pMod->m_bDopVkl;
	pModBuf->m_iAmplitudeStepDop = pMod->m_iAmplitudeStepDop;
	pModBuf->m_uStepDop			 = pMod->m_uStepDop;
	pModBuf->m_iPolarityStepDop	 = pMod->m_iPolarityStepDop;
	pModBuf->m_iFamilyVkl		 = pMod->m_iFamilyVkl;
	pModBuf->m_iBiasDop			 = pMod->m_iBiasDop;			
	pModBuf->m_iPolarityBiasDop	 = pMod->m_iPolarityBiasDop;
	pModBuf->m_iDopMeas			 = pMod->m_iDopMeas;
	pModBuf->m_iRangLimitDop	 = pMod->m_iRangLimitDop;
	pModBuf->m_iLimitDop		 = pMod->m_iLimitDop;		
	//расчёт
	pModBuf->m_sAxisY = pMod->m_sAxisY;
	pModBuf->m_sAxisX = pMod->m_sAxisX;
	pModBuf->m_sPar   = pMod->m_sPar;
	pModBuf->m_sArg   = pMod->m_sArg;
	pModBuf->m_uTp    = pMod->m_uTp;
	pModBuf->m_sUr1   = pMod->m_sUr1;
	pModBuf->m_sUr2   = pMod->m_sUr2;
	pModBuf->m_iForm  = pMod->m_iForm;
	pModBuf->m_sMin   = pMod->m_sMin;
	pModBuf->m_sMax   = pMod->m_sMax;

	}
}

void CLineDlg::CopyEl(LPCTSTR sEl,LPCTSTR sElNew)
{
	POSITION pos;
	*m_position=m_pList->GetHeadPosition();
    do 
	{ 
	  CMod* pMod = m_pList->GetAt(*m_position);
      if(!((pMod->m_EL).Compare(sEl))) 
	  { 
	    pos=*m_position;
		CopyTest(sElNew,pMod->m_MOD);
	    *m_position=pos;
	  }
	  pMod =m_pList->GetNext(*m_position);
	} while(*m_position);
    SetModifiedFlag();
	FConfig();
}

void CLineDlg::CopyTest(LPCTSTR sEl_new,LPCTSTR sMod_new) 
{	
    if (*m_position)
	{
	  CMod* pMod = m_pList->GetAt(*m_position);
	  CMod* pModNew = new CMod;
 	  pModNew->strDateMeas = pMod->strDateMeas;
	  pModNew->strTimeMeas = pMod->strTimeMeas;
	  pModNew->m_EL    = sEl_new;
	  pModNew->m_MOD   = sMod_new;
 	  pModNew->m_PRIM  = pMod->m_PRIM;//_T("");
	//коллектор
	pModNew->m_nelement		      = pMod->m_nelement;
	pModNew->m_iCircuitKol		  = pMod->m_iCircuitKol;
	pModNew->m_iCircuitBaza	      = pMod->m_iCircuitBaza;
	pModNew->m_iCircuitDop		  = pMod->m_iCircuitDop;
	    pModNew->m_iKommutator	= pMod->m_iKommutator;
	    pModNew->m_iGnezdoC	= pMod->m_iGnezdoC;
	    pModNew->m_iGnezdoB	= pMod->m_iGnezdoB;
	    pModNew->m_iGnezdoE	= pMod->m_iGnezdoE;
	pModNew->m_irazv              = pMod->m_irazv;
	pModNew->m_idlitimp			  = pMod->m_idlitimp;
	pModNew->m_iKolU			  = pMod->m_iKolU;
	pModNew->m_iKolI			  = pMod->m_iKolI;
	pModNew->m_iKolSourceU		  = pMod->m_iKolSourceU;
	pModNew->m_iLimitKolU		  = pMod->m_iLimitKolU;
	pModNew->m_iLimitKolI		  = pMod->m_iLimitKolI;
	pModNew->m_bCheckKolRejHigh   = pMod->m_bCheckKolRejHigh;
	pModNew->m_itipimp			  = pMod->m_itipimp;
	pModNew->m_bCheckPauza        = pMod->m_bCheckPauza;
	//база
	pModNew->m_iBazaSource        = pMod->m_iBazaSource;
	pModNew->m_bBazaVkl		      = pMod->m_bBazaVkl;
	pModNew->m_iAmplitudeStepBaza = pMod->m_iAmplitudeStepBaza;
	pModNew->m_uStepBaza          = pMod->m_uStepBaza;
	pModNew->m_iPolarityStepBaza  = pMod->m_iPolarityStepBaza;
	pModNew->m_iQue1      = pMod->m_iQue1;
	pModNew->m_iBiasBaza          = pMod->m_iBiasBaza;		
	pModNew->m_iPolarityBiasBaza  = pMod->m_iPolarityBiasBaza;
	pModNew->m_iBazaMeas		  = pMod->m_iBazaMeas;
	pModNew->m_iRangLimitBaza     = pMod->m_iRangLimitBaza;
	pModNew->m_iLimitBaza         = pMod->m_iLimitBaza;	
	pModNew->m_bCheckBazaRejHigh  = pMod->m_bCheckBazaRejHigh;
	//доп.канал
	pModNew->m_iDopSource		 = pMod->m_iDopSource;
//	pModNew->m_bDopVkl		     = pMod->m_bDopVkl;
	pModNew->m_iAmplitudeStepDop = pMod->m_iAmplitudeStepDop;
	pModNew->m_uStepDop			 = pMod->m_uStepDop;
	pModNew->m_iPolarityStepDop	 = pMod->m_iPolarityStepDop;
	pModNew->m_iFamilyVkl		 = pMod->m_iFamilyVkl;
	pModNew->m_iBiasDop			 = pMod->m_iBiasDop;			
	pModNew->m_iPolarityBiasDop	 = pMod->m_iPolarityBiasDop;
	pModNew->m_iDopMeas			 = pMod->m_iDopMeas;
	pModNew->m_iRangLimitDop	 = pMod->m_iRangLimitDop;
	pModNew->m_iLimitDop		 = pMod->m_iLimitDop;		
	//расчёт
	pModNew->m_sAxisY = pMod->m_sAxisY;
	pModNew->m_sAxisX = pMod->m_sAxisX;
	pModNew->m_sPar   = pMod->m_sPar;
	pModNew->m_sArg   = pMod->m_sArg;
	pModNew->m_uTp    = pMod->m_uTp;
	pModNew->m_sUr1   = pMod->m_sUr1;
	pModNew->m_sUr2   = pMod->m_sUr2;
	pModNew->m_iForm  = pMod->m_iForm;
	pModNew->m_sMin   = pMod->m_sMin;
	pModNew->m_sMax   = pMod->m_sMax;
//	  *m_position = m_pList->InsertAfter(*m_position,pModNew);
	  *m_position = m_pList->InsertAfter(m_pList->GetTailPosition(),pModNew);

//это не надо !!! 29.11.17  14.30 =>
//вставила 29.11.17 14.00 
//		delete pModNew;	
//		pModNew = NULL;
//<=

	}
}

void CLineDlg::CopyFromBuf()
{  //копирование из буфера
    m_bCopy = FALSE;
	CMod* pModNew = new CMod;
	m_pList     = GetList();
	m_position  = GetPosition();
 	pModNew->strDateMeas   = pModBuf->strDateMeas;
	pModNew->strTimeMeas   = pModBuf->strTimeMeas;
	pModNew->m_EL		   = pModBuf->m_EL;
	pModNew->m_MOD         = pModBuf->m_MOD;
 	pModNew->m_PRIM        = pModBuf->m_PRIM;
	//коллектор
	pModNew->m_nelement		      = pModBuf->m_nelement;
	pModNew->m_iCircuitKol		  = pModBuf->m_iCircuitKol;
	pModNew->m_iCircuitBaza	      = pModBuf->m_iCircuitBaza;
	pModNew->m_iCircuitDop		  = pModBuf->m_iCircuitDop;
	    pModNew->m_iKommutator	= pModBuf->m_iKommutator;
	    pModNew->m_iGnezdoC	= pModBuf->m_iGnezdoC;
	    pModNew->m_iGnezdoB	= pModBuf->m_iGnezdoB;
	    pModNew->m_iGnezdoE	= pModBuf->m_iGnezdoE;
	pModNew->m_irazv              = pModBuf->m_irazv;
	pModNew->m_idlitimp			  = pModBuf->m_idlitimp;
	pModNew->m_iKolU			  = pModBuf->m_iKolU;
	pModNew->m_iKolI			  = pModBuf->m_iKolI;
	pModNew->m_iKolSourceU		  = pModBuf->m_iKolSourceU;
	pModNew->m_iLimitKolU		  = pModBuf->m_iLimitKolU;
	pModNew->m_iLimitKolI		  = pModBuf->m_iLimitKolI;
	pModNew->m_bCheckKolRejHigh   = pModBuf->m_bCheckKolRejHigh;
	pModNew->m_itipimp			  = pModBuf->m_itipimp;
	pModNew->m_bCheckPauza        = pModBuf->m_bCheckPauza;
	//база
	pModNew->m_iBazaSource        = pModBuf->m_iBazaSource;
	pModNew->m_bBazaVkl		      = pModBuf->m_bBazaVkl;
	pModNew->m_iAmplitudeStepBaza = pModBuf->m_iAmplitudeStepBaza;
	pModNew->m_uStepBaza          = pModBuf->m_uStepBaza;
	pModNew->m_iPolarityStepBaza  = pModBuf->m_iPolarityStepBaza;
	pModNew->m_iQue1      = pModBuf->m_iQue1;
	pModNew->m_iBiasBaza          = pModBuf->m_iBiasBaza;		
	pModNew->m_iPolarityBiasBaza  = pModBuf->m_iPolarityBiasBaza;
	pModNew->m_iBazaMeas		  = pModBuf->m_iBazaMeas;
	pModNew->m_iRangLimitBaza     = pModBuf->m_iRangLimitBaza;
	pModNew->m_iLimitBaza         = pModBuf->m_iLimitBaza;	
	pModNew->m_bCheckBazaRejHigh  = pModBuf->m_bCheckBazaRejHigh;
	//доп.канал
	pModNew->m_iDopSource		 = pModBuf->m_iDopSource;
//	pModNew->m_bDopVkl		     = pModBuf->m_bDopVkl;
	pModNew->m_iAmplitudeStepDop = pModBuf->m_iAmplitudeStepDop;
	pModNew->m_uStepDop			 = pModBuf->m_uStepDop;
	pModNew->m_iPolarityStepDop	 = pModBuf->m_iPolarityStepDop;
	pModNew->m_iFamilyVkl		 = pModBuf->m_iFamilyVkl;
	pModNew->m_iBiasDop			 = pModBuf->m_iBiasDop;			
	pModNew->m_iPolarityBiasDop	 = pModBuf->m_iPolarityBiasDop;
	pModNew->m_iDopMeas			 = pModBuf->m_iDopMeas;
	pModNew->m_iRangLimitDop	 = pModBuf->m_iRangLimitDop;
	pModNew->m_iLimitDop		 = pModBuf->m_iLimitDop;		
	//расчёт
	pModNew->m_sAxisY = pModBuf->m_sAxisY;
	pModNew->m_sAxisX = pModBuf->m_sAxisX;
	pModNew->m_sPar   = pModBuf->m_sPar;
	pModNew->m_sArg   = pModBuf->m_sArg;
	pModNew->m_uTp    = pModBuf->m_uTp;
	pModNew->m_sUr1   = pModBuf->m_sUr1;
	pModNew->m_sUr2   = pModBuf->m_sUr2;
	pModNew->m_iForm  = pModBuf->m_iForm;
	pModNew->m_sMin   = pModBuf->m_sMin;
	pModNew->m_sMax   = pModBuf->m_sMax;


	*m_position = m_pList->InsertAfter(m_pList->GetTailPosition(),pModNew);
    SetModifiedFlag();

    POSITION pos=*m_position;
	FConfig();
	HTREEITEM hitem=NULL;

	CString str = pModNew->m_EL;
	for(int i=0;i<NE;i++)
	 if (str == m_masStrEl[i])
	 { hitem = m_masHEl[i]; 
	   break;
	 }
	if (hitem!=NULL)
	{ 
//	  m_pTreeDlg->m_TreeCtrl.Expand(hitem,TVE_EXPAND);
//	  m_pTreeDlg->m_TreeCtrl.EnsureVisible(hitem);
	  m_TreeCtrl.Expand(hitem,TVE_EXPAND);
	  m_TreeCtrl.EnsureVisible(hitem);
	}
    *m_position=pos;

//это не надо !!! 29.11.17  14.30 =>
//вставила 29.11.17  =>14.00
//		delete pModNew;	
//		pModNew = NULL;
//<=

}

void CLineDlg::OnEndLabelEdit() 
{
	// TODO: Add your specialized code here and/or call the base class

	CMod* pMod;
	m_pList     = GetList();
	m_position  = GetPosition();
    *m_position = m_pList->GetHeadPosition();

    
	if (i_SelUrov==2) //переименовать тест в списке
	{ pMod = FindTest(str_EL,str_MOD);
	  if (pMod)
		{ 
	      pMod->m_MOD = str_NewMOD;
		  str_MOD = str_NewMOD;
		  strTestName=str_EL+_T(" \\ ")+str_MOD;//_T("Test: ")+
//		  m_masStrMod[i_EL][i_MOD]=str_NewMOD.GetBuffer(str_NewMOD.GetLength()+1);
		}
	}
    if (i_SelUrov==1) //переименовать элемент в списке
	{ do { pMod  = m_pList->GetAt(*m_position);
	       if (!((pMod->m_EL).Compare(str_EL)))
			{ 
			  pMod->m_EL = str_NewEL;
			}
		   pMod =m_pList->GetNext(*m_position);
		} while(*m_position);
	  str_EL = str_NewEL;
//	  m_masStrEl[i_EL]=str_NewEL.GetBuffer(str_NewEL.GetLength()+1);
		  strTestName=_T("");
	}
	FConfig();
    SetModifiedFlag();
}


void CLineDlg::OnModify() 
{//0
	// TODO: Add your command handler code here
	CParolDlg dlgp;
//  if (pModSelected)//???_???
  {//1
	if (MessageBox(_T("Вы уверены, что хотите сохранить тест ")+str_MOD+_T(" для элемента ")+str_EL+
				 _T("\nс новыми параметрами?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
	{//1-1
	if (m_bZaschitaParol==TRUE)
	{//2 //файл защищён паролем
		dlgp.what_save = FILE_MOD;
		UpdateData(false);
		if (dlgp.DoModal()==IDOK)
		{//3 
			UpdateData();
			if (m_sparol == dlgp.m_sparol) 
			{//4 //пароль верный
				if (!m_pList->IsEmpty()) 
				{//5
					if (i_SelUrov==2)
					{//6 //модифицировать параметры теста
	
						
						if (FindTest(str_EL,str_MOD))
						{ SaveParam();
						  SetModifiedFlag();
						}
						else MessageBox(_T("В списке не найден тест ")+str_MOD+_T(" для элемента ")+str_EL+_T("!"),NULL,MB_OK|MB_ICONERROR);
					}//6
				    if (i_SelUrov==1)
					 MessageBox(_T("Выберите тест для изменения!"),NULL,MB_OK|MB_ICONERROR);	  	  
				}//5
			}//4
		    else MessageBox(_T("Неверный пароль! Файл не сохранён!"),NULL,MB_OK|MB_ICONERROR);
		}//3
		else MessageBox(_T("Пароль не введен! Файл не сохранён!"),NULL,MB_OK|MB_ICONEXCLAMATION);
	}//2
	else 
	{//2 //файл не защищён паролем
				if (!m_pList->IsEmpty()) 
				{//5
					if (i_SelUrov==2)
					{//6 //модифицировать параметры теста
						if (FindTest(str_EL,str_MOD))
						{ SaveParam();
						  SetModifiedFlag();
						}
						else MessageBox(_T("В списке не найден тест ")+str_MOD+_T(" для элемента ")+str_EL+_T("!"),NULL,MB_OK|MB_ICONERROR);
					}//6
				    if (i_SelUrov==1)
					 MessageBox(_T("Выберите тест для изменения!"),NULL,MB_OK|MB_ICONERROR);	  
				}//5

	}//2
	}//1-1
  }//1
//  else MessageBox(_T("Выберите тест для изменения!"),NULL,MB_OK|MB_ICONERROR);//???_???
	
}//0


void CLineDlg::InitGnezdo() 
{
  CComboBox *cbptr = (CComboBox*) GetDlgItem(IDC_COMBO_GNEZDO_C);
  cbptr->ResetContent();
  CComboBox *cbptrb = (CComboBox*) GetDlgItem(IDC_COMBO_GNEZDO_B);
  cbptrb->ResetContent();
  CComboBox *cbptre = (CComboBox*) GetDlgItem(IDC_COMBO_GNEZDO_E);
  cbptre->ResetContent();
	if (m_nelement==DIOD || m_nelement==RES)
	{ cbptr->AddString(_T("C"));
	  cbptr->AddString(_T("E"));
	  cbptre->AddString(_T("C"));
	  cbptre->AddString(_T("E"));
	  m_iGnezdoC=0;
	  m_iGnezdoE=1;
	}
	else 
	{ cbptr->AddString(_T("C"));
	  cbptr->AddString(_T("B"));
	  cbptr->AddString(_T("E"));
      cbptr->AddString(_T("обрыв"));
	  cbptrb->AddString(_T("C"));
	  cbptrb->AddString(_T("B"));
	  cbptrb->AddString(_T("E"));
	  cbptrb->AddString(_T("обрыв"));
	  cbptre->AddString(_T("C"));
	  cbptre->AddString(_T("B"));
	  cbptre->AddString(_T("E"));
 	  cbptre->AddString(_T("обрыв"));
	  m_iGnezdoC=0;
	  m_iGnezdoB=1;
	  m_iGnezdoE=2;
	}
	UpdateData(FALSE);
}


void CLineDlg::SetGnezdo() 
{//установить текущее состояние гнезд

  CComboBox *ptr = (CComboBox*) GetDlgItem(IDC_COMBO_KOMMUTATOR);
  ptr->SetCurSel(m_iKommutator);
  
  ptr = (CComboBox*) GetDlgItem(IDC_COMBOCHECKKOL);
  ptr->SetCurSel(m_iCircuitKol);


  CComboBox *cbptr = (CComboBox*) GetDlgItem(IDC_COMBO_GNEZDO_C);
  cbptr->ResetContent();
  CComboBox *cbptrb = (CComboBox*) GetDlgItem(IDC_COMBO_GNEZDO_B);
  cbptrb->ResetContent();
  CComboBox *cbptre = (CComboBox*) GetDlgItem(IDC_COMBO_GNEZDO_E);
  cbptre->ResetContent();
	if (m_nelement==DIOD || m_nelement==RES)
	{ cbptr->AddString(_T("C"));
	  cbptr->AddString(_T("E"));
	  cbptre->AddString(_T("C"));
	  cbptre->AddString(_T("E"));
	  cbptr->SetCurSel(m_iGnezdoC);
	  cbptre->SetCurSel(m_iGnezdoE);
	}
	else 
	{ cbptr->AddString(_T("C"));
	  cbptr->AddString(_T("B"));
	  cbptr->AddString(_T("E"));
      cbptr->AddString(_T("обрыв"));
	  cbptrb->AddString(_T("C"));
	  cbptrb->AddString(_T("B"));
	  cbptrb->AddString(_T("E"));
	  cbptrb->AddString(_T("обрыв"));
	  cbptre->AddString(_T("C"));
	  cbptre->AddString(_T("B"));
	  cbptre->AddString(_T("E"));
	  cbptre->AddString(_T("обрыв"));
	  cbptr->SetCurSel(m_iGnezdoC);
	  cbptrb->SetCurSel(m_iGnezdoB);
	  cbptre->SetCurSel(m_iGnezdoE);

	  ptr = (CComboBox*) GetDlgItem(IDC_COMBOCHECKBAZA);
      ptr->SetCurSel(m_iCircuitBaza);

	  if (m_nelement>PMOS)
	  {
	    ptr = (CComboBox*) GetDlgItem(IDC_COMBOCHECKDOP);
        ptr->SetCurSel(m_iCircuitDop);
	  }

	  
	}
}



//void CLineDlg::OnButtonDiod() 
void CLineDlg::ButtonDiod() 
{
	// TODO: Add your control notification handler code here
	m_nelement = DIOD;//0;
	m_EL =_T("D");
	m_MOD=_T("");//_T("Forvard");
//	m_irazv = AC;	//AC
	m_irazv = SYN_POS;//AC;	//AC//---!
	if (n_Switch==1) 	m_irazv = IMP_POS;//AC;	//AC//---!
	if (n_Switch==2) 	m_irazv = IMP_NEG;//AC;	//AC//---!

	m_sAxisX=_T("Ua");
	m_sAxisY=_T("Ia");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
/*    
	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioDiod);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioDiod);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();

	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}

//void CLineDlg::OnButtonNmos() 
void CLineDlg::ButtonNmos() 
{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==1)
{
	m_nelement = NMOS;//4;
	m_EL =_T("NMOS");
	m_MOD=_T("");//_T("IdVd");
//	m_irazv = AC;	//AC//---!
	m_irazv = SYN_POS;//AC;	//AC//---!
	if (n_Switch==1) 	m_irazv = IMP_POS;//AC;	//AC//---!
	m_sAxisX=_T("Ud");
	m_sAxisY=_T("Id");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
/*    
	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioNmos);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioNmos);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonNmos4() 
void CLineDlg::ButtonNmos4() 
{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==1)
{
	m_nelement = NMOS4;//8;
	m_EL =_T("NMOS4");
	m_MOD=_T("");//_T("IdVd_b");
//	m_irazv = AC;	//AC//---!
	m_irazv = SYN_POS;//AC;	//AC//---!
	if (n_Switch==1) 	m_irazv = IMP_POS;//AC;	//AC//---!
	m_sAxisX=_T("Ud");
	m_sAxisY=_T("Id");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
    
/*	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioNmos4);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioNmos4);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonNpn() 
void CLineDlg::ButtonNpn() 
{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==1)
{
	m_nelement = NPN;//2;
	m_EL =_T("NPN");
	m_MOD=_T("");//_T("IcVc");
//	m_irazv = AC;	//AC
	m_irazv = SYN_POS;//AC;	//AC//---!
	if (n_Switch==1) 	m_irazv = IMP_POS;//AC;	//AC//---!
	m_sAxisX=_T("Uc");
	m_sAxisY=_T("Ic");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
    
/*	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioNpn);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioNpn);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonNpn4() 
void CLineDlg::ButtonNpn4() 
{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==1)
{
	m_nelement = NPN4;//6;
	m_EL =_T("NPN4");
	m_MOD=_T("");//_T("IcVc_s");
//	m_irazv = AC;	//AC//---!
	m_irazv = SYN_POS;//AC;	//AC//---!
	if (n_Switch==1) 	m_irazv = IMP_POS;//AC;	//AC//---!
	m_sAxisX=_T("Uc");
	m_sAxisY=_T("Ic");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
    
/*	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioNpn4);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioNpn4);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonPmos() 
void CLineDlg::ButtonPmos() 
{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==2)
{
	m_nelement = PMOS;//5;
	m_EL =_T("PMOS");
	m_MOD=_T("");//_T("IdVd_p");
//	m_irazv = AC;	//AC//---!
	m_irazv = SYN_NEG;//AC;	//AC//---!
	if (n_Switch==2) 	m_irazv = IMP_NEG;//AC;	//AC//---!
	m_sAxisX=_T("Ud");
	m_sAxisY=_T("Id");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
/*    
	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioPmos);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioPmos);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonPmos4() 
void CLineDlg::ButtonPmos4() 

{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==2)
{
	m_nelement = PMOS4;//9;
	m_EL =_T("PMOS4");
	m_MOD=_T("");//_T("IdVd_b_p");
//	m_irazv = AC;	//AC//---!
	m_irazv = SYN_NEG;//AC;	//AC//---!
	if (n_Switch==2) 	m_irazv = IMP_NEG;//AC;	//AC//---!
	m_sAxisX=_T("Ud");
	m_sAxisY=_T("Id");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
/*    
	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioPmos4);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioPmos4);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	int InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonPnp() 
void CLineDlg::ButtonPnp() 
{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==2)
{
	m_nelement = PNP;//3;
	m_EL =_T("PNP");
	m_MOD=_T("");//_T("IcVc_p");
	m_irazv = SYN_NEG;//AC;	//AC//---!
	if (n_Switch==2) 	m_irazv = IMP_NEG;//AC;	//AC//---!
	m_sAxisX=_T("Uc");
	m_sAxisY=_T("Ic");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	UpdateData(FALSE);
    
/*	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioPnp);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioPnp);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonPnp4() 
void CLineDlg::ButtonPnp4() 
{
	// TODO: Add your control notification handler code here
if  (n_Switch==0 || n_Switch==2)
{
	m_nelement = PNP4;//7;
	m_EL =_T("PNP4");
	m_MOD=_T("");//_T("IcVc_s");
	m_irazv = SYN_NEG;//AC;	//AC//---!
//	m_irazv = AC;	//AC//---!
	if (n_Switch==2) 	m_irazv = IMP_NEG;//AC;	//AC//---!
	m_sAxisX=_T("Uc");
	m_sAxisY=_T("Ic");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
    
/*	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioPnp4);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioPnp4);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
}	
}

//void CLineDlg::OnButtonRes() 
void CLineDlg::ButtonRes() 
{
	// TODO: Add your control notification handler code here
	m_nelement = TS;//RES;//1;
	m_EL =_T("TS");	//R");
//	m_nelement = RES;//1;
//	m_EL =_T("R");
	m_MOD=_T("");//_T("Res");
//	m_irazv = AC;	//AC//---!
	m_irazv = SYN_POS;
	if (n_Switch==1) 	m_irazv = IMP_POS;//AC;	//AC//---!
	if (n_Switch==2) 	m_irazv = IMP_NEG;//AC;	//AC//---!
	m_sAxisX=_T("Ua");
	m_sAxisY=_T("Ia");
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMin=_T("");
	m_sMax=_T("");
	 m_bCheckKolRejHigh = false;
	 m_iKommutator = 0;
	 m_iQue1 = 0;
	UpdateData(FALSE);
    
/*	((CStatic*)GetDlgItem(IDC_STATIC_VKL))->MoveWindow(&RectRadioRes);
	CWnd* pwnd=GetDlgItem(IDI_ICON_VKL);
	if (pwnd)	{ pwnd->MoveWindow(&RectRadioRes);
				  pwnd->ShowWindow(SW_SHOW);
				}
*/
	InitTest();
//	InitNewSwitch();
//	InitPage(0);
    int nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	InitAxis();
	b_InitAxis=true; ControlParam();
	DrawStatusRukoyatka();
		str_PRIM = _T("");
    SetTextInfo(_T("Выберите тест и загрузите параметры."));
	
}
void CLineDlg::VklRadioElement()
{
	TRACE(_T(" VklRadioElement()\r\n"));
	CStatic* pst=(CStatic*)GetDlgItem(IDC_STATIC_VKL);
	switch(m_nelement)
	{ case RES:
	  case DIOD: pst->MoveWindow(&RectRadioDiod);
				 break;
	  case TS:   pst->MoveWindow(&RectRadioRes);
				 break;
	  case NPN:  pst->MoveWindow(&RectRadioNpn);
				 break;
	  case PNP:  pst->MoveWindow(&RectRadioPnp);
				 break;
	  case NMOS: pst->MoveWindow(&RectRadioNmos);
				 break;
	  case PMOS: pst->MoveWindow(&RectRadioPmos);
				 break;
	  case NPN4: pst->MoveWindow(&RectRadioNpn4);
				 break;
	  case PNP4: pst->MoveWindow(&RectRadioPnp4);
				 break;
	  case NMOS4:pst->MoveWindow(&RectRadioNmos4);
				 break;
	  case PMOS4:pst->MoveWindow(&RectRadioPmos4);
				 break;
	}
//	Invalidate();
}

BOOL CLineDlg::ExtractParamFromTst() 
{ 
	BOOL fl = FALSE;	//fl=1 - параметры загрузили из файла *.tst , fl=0 - нет
	int p1,p2;
	CString str=_T("");
    int iStrLen;
    char* cc;

	if (!nameTst.IsEmpty())
	{ //выбор параметров из файла *.tst (полный путь в nameTst)
	  CFile file;
	  BOOL b = file.Open((LPCTSTR)nameTst,CFile::modeRead);
	  if (b)
	  {//if b
	    file.SeekToBegin();	
	    DWORD  l = file.GetLength();
	    DWORD* lp = new DWORD[l];
	    file.Read(lp,l);
		file.Close();
		CString strFile((LPCTSTR)&lp[0],l);
		delete[] lp;
		if (!strFile.IsEmpty())
		{
/*		     m_nelement >> m_PRIM >>
			 m_iCircuitKol >> m_iCircuitBaza >>	m_iCircuitDop >>
			 m_irazv >>	m_idlitimp >> m_iKolU >> m_iKolI >>	m_iKolSourceU >>	
	         m_iLimitKolU >> m_iLimitKolI >> 
			 m_bCheckKolRejHigh >>m_itipimp >>m_bCheckPauza >>	
			 m_iBazaSource >> m_bBazaVkl >>m_iAmplitudeStepBaza >> m_uStepBaza >> m_iPolarityStepBaza >> 
			 m_iQue1 >> m_iBiasBaza >> 	m_iPolarityBiasBaza >> 
			 m_iBazaMeas >> m_iRangLimitBaza >> m_iLimitBaza >> 
			 m_bCheckBazaRejHigh >> 
			 m_iDopSource >> m_bDopVkl >> m_iAmplitudeStepDop >> m_uStepDop >> m_iPolarityStepDop >> 
			 m_iFamilyVkl >> m_iBiasDop >> m_iPolarityBiasDop >> 
			 m_iDopMeas >> m_iRangLimitDop >> m_iLimitDop >> 
			 m_sAxisY >> m_sAxisX >> m_sPar >> m_sArg >> m_uTp >> 
		 	 m_sUr1 >> m_sUr2 >> m_iForm >> m_sMin >> m_sMax;// >> 
*/
			//коллектор
		   p1=strFile.Find(L'=',0);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_nelement = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_PRIM = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();
			str_PRIM = m_PRIM;

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iCircuitKol = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
		   
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iCircuitBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
		   
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iCircuitDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iKommutator = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iGnezdoC = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iGnezdoB = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iGnezdoE = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_irazv = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_idlitimp = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iKolU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iKolI = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iKolSourceU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iLimitKolU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iLimitKolI = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
			   m_iPosTek = m_iLimitKolI;
			   DrawStatusRukoyatka();	//индикация Старта по Uc

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_itipimp = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();



			//база
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iBazaSource = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iAmplitudeStepBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_uStepBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iPolarityStepBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iQue1 = atoi(cc);
		   if (m_iQue1==-1) m_iQue1=COLLECTOR;
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iBiasBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iPolarityBiasBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iBazaMeas = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iRangLimitBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iLimitBaza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

			//доп.канал
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iDopSource = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();


		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iAmplitudeStepDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_uStepDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iPolarityStepDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iFamilyVkl = atoi(cc);
		   if (m_iFamilyVkl==-1) m_iFamilyVkl = 0;		
		   delete[] cc;
		   str.ReleaseBuffer();
		   if (m_iFamilyVkl==1) m_bFamilyVkl=true;
				          else  m_bFamilyVkl=false;
		   m_bFamilyVklOld = m_bFamilyVkl;

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iBiasDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iPolarityBiasDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iDopMeas = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iRangLimitDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iLimitDop = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

			//расчёт
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sAxisY = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sAxisX = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sPar = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sArg = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_uTp = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sUr1 = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sUr2 = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_iForm = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sMin = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   m_sMax = str;//strFile.Mid(p1+1,p2-p1-2);
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_bCheckKolRejHigh = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_bCheckPauza = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_bBazaVkl = atoi(cc);
		   delete cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_bCheckBazaRejHigh = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

/*		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_bDopVkl = atoi(cc);
		   delete cc;
		   str.ReleaseBuffer();
*/
//		   UpdateData(FALSE);	//убрала 29.11.17
		   fl = TRUE;
			

		}//end of if (!strFile.IsEmpty())
	  }//end of if (b)
	  else
	  { // b == FALSE;
		fl = FALSE;
		MessageBox(_T("Файл отсутствует в библиотеке тестов!"),NULL,MB_OK|MB_ICONERROR);
	  }
	}//end of if (!nameTst.IsEmpty())
	else
	{//nameTst.IsEmpty
		fl = FALSE;
	}
 return fl;
}

void CLineDlg::SetParamDefault()
{//установка параметров в состояние "Пусто"
	//коллектор
	InitStatusKnot();
	m_irazv = -1;
	m_idlitimp = 0;
	m_iKolU = -1;
	m_iKolI = -1;
	m_iKolSourceU = 0;
	m_iLimitKolU =100;		//текущее значение слайдера ограничения U коллектора (в %)
	m_iLimitKolI =0;		//текущее значение слайдера ограничения I коллектора
	   m_iPosTek = m_iLimitKolI;
	   DrawStatusRukoyatka();	//индикация Старта по Uc
	m_bCheckKolRejHigh = FALSE;
	m_bCheckPauza = FALSE;
	//база
	m_iBazaSource = -1;
	m_bBazaVkl = false;
	m_iAmplitudeStepBaza = -1;
	m_uStepBaza = 0;
	m_iPolarityStepBaza = -1;
	m_iBiasBaza = 0;		//текущее значение слайдера смещения базы
	m_iPolarityBiasBaza = -1;
	m_iBazaMeas = -1;
	m_iRangLimitBaza = -1;
	m_iLimitBaza = 100;		//текущее значение слайдера ограничения базы
	m_bCheckBazaRejHigh = FALSE;
	//доп.канал
	m_iDopSource = -1;
//	m_bDopVkl = false;
	m_iAmplitudeStepDop = -1;
	m_uStepDop = 0;
	m_iPolarityStepDop = -1;
	m_iBiasDop = 0;			//текущее значение слайдера смещения доп.канала
	m_iPolarityBiasDop = -1;
	m_iDopMeas = -1;
	m_iRangLimitDop = -1;
	m_iLimitDop = 100;		//текущее значение слайдера ограничения доп.канала
	//расчёт
	m_sAxisY = _T("");
	m_sAxisX = _T("");
	m_sPar = _T("");
	m_sArg = _T("");
	m_uTp = 0;
	m_sUr1 = _T("");
	m_sUr2 = _T("");
	m_iForm = -1;
	m_sMin = _T("");
	m_sMax = _T("");
	//резерв
	//m_iRangBiasBaza = 0;
	m_iFamilyVkl = 0;//m_iRangBiasDop = 0;
	m_itipimp = TIP_SYN;		

}

void CLineDlg::OnSelchangeTabctrlparam(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	  
	int nTab = m_TabCtrlParam.GetCurSel();
	InitPage(nTab);
	
	*pResult = 0;
}


void CLineDlg::OnButtonLoadtst() 
{
	// TODO: Add your control notification handler code here
    CString str;
	int  nTab;
	BOOL b=FALSE;
//	UpdateData(TRUE);	//убрала 29.11.17
	if (!m_MOD.IsEmpty())
	{
 	  CComboBox *pcb = (CComboBox*) GetDlgItem(IDC_COMBOMOD);
	  index=pcb->FindStringExact(0,m_MOD);
	  if (index==-1)
	  { //наименование теста не совпадает ни с одним из предлагаемого списка => 
		str_EL = m_EL;
	    str_MOD = m_MOD;
	    strTestName = str_MOD;//str_EL + _T(" \\ ")+str_MOD;
	    SetTextInfo(_T("TEST:  ") + strTestName);
//        if (MessageBox(_T("Обнаружено новое наименование теста.\nСбросить параметры?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
//		{ SetParamDefault();
//	      UpdateData(FALSE);
//		}
	  }
	  if (index>=1)
	  { //выбран стандартный тест
		//добавить тест со стандартными данными из
		//каталога Tst_standart
		  CCmdTarget::BeginWaitCursor();
		CString rStrTest;
		pcb->GetLBText(index,rStrTest);
		nameTst=strPassTstStandart+rStrTest+_T(".tst");//_T("\\Disk\\Tst_standart\\")_T("\\Line\\Tst\\")+
//		  m_EL=_T("");
		  str_EL = m_EL;
	      str_MOD = m_MOD;
//	      strTestName = m_MOD;
	      strTestName = str_EL + _T("\\ ") + str_MOD;
 	    b = ExtractParamFromTst();
		if (b) 
		{ 
	      SetTextInfo(_T("TEST: ") + str_MOD);//nameTst);
		  str_PRIM = m_PRIM;
			UpdateData(FALSE);		//отобразить параметры в окошках	//убрала 29.11.17 //раскоииентировада 28.11.17
		  SetStringsSourceMeas();//@ 7.12.17	
			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала
//	ChangeDiapazonKolSourceU();
//	ChangeDiapazonKolMeasU();
//	ChangeDiapazonKol();
	

//		  ControlParam();
    nTab = 0;
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);	
//	int  nTabTuning = 0;//Error//1
//	m_TabCtrlTuning.SetCurSel(nTabTuning);
//	InitPageTuning(nTabTuning);	// отобразить панель "Дисплей"
//		  InitPage(0);
//		  InitPageTuning(0);
		  b_InitAxis=true;
			ControlParam();
		  EndWaitCursor();
		}
		else
		{//файл отсутствует в каталоге 
            m_MOD=_T("");
		    str_EL = m_EL;
	        str_MOD = m_MOD;
		    strTestName = _T("");
	        SetTextInfo(_T("Введите наименование теста!"));
	        UpdateData(FALSE);
		}
	  }
	  if (index==0)
	  { //выбран пользовательский тест
		//добавить тест с пользовательскими данными из
		//каталога Tst_user
/*		nameTst=_T("\\Disk\\Tst_user\\");//_T("\\Line\\Tst\\");
		CFileDialog dl(TRUE);
	    CString strd(_T("\\Disk\\Tst_user\\ (*.tst)")); strd+=(TCHAR)NULL;
	    strd+=_T("*.tst"); strd+=(TCHAR)NULL;
	    dl.m_ofn.lpstrFilter=strd;
	    dl.m_ofn.nFilterIndex=1;
	    TCHAR strName[_MAX_PATH];
	    strName[0]='*';
	    strName[1]='.';
	    strName[2]='t';
	    strName[3]='s';
	    strName[4]='t';
	    strName[5]=(TCHAR)NULL;
	    dl.m_ofn.lpstrFile=strName;
	    dl.m_ofn.lpstrTitle=_T("Укажите файл для экстракции параметров");
*/
		CDelTstDlg dlg;
		dlg.what_directory = TST_USER;
		dlg.m_nelement = m_nelement;
		UpdateData(false);
//	    if (dl.DoModal()==IDOK)
	    if (dlg.DoModal()==IDOK)
		{			
//		  nameTst = dl.GetPathName();
		  //получение имени польз.теста из полного пути
//		  str=dl.GetFileTitle();
		  UpdateData();
		  CCmdTarget::BeginWaitCursor();
		  str = dlg.m_sname_tst;
		  nameTst = strPassTstUser + str;
		  int l=str.GetLength();
		  m_MOD=str.Mid(0,l-4);
//		  m_MOD=str;
		  str_EL = m_EL;
	      str_MOD = m_MOD;
	      strTestName = str_MOD;//str_EL + _T("\\ ") + str_MOD;
//	      UpdateData(FALSE);
 	      b = ExtractParamFromTst();
//	      UpdateData(FALSE);	//убрала 29.11.17  //вставила 28.11.17
		  if (b)
		  { 
	        SetTextInfo(_T("TEST:  ") + strTestName);
			str_PRIM = m_PRIM;
		  SetStringsSourceMeas();//@ 7.12.17	
			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)//=====
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала
//		    InitPage(0);
//		  InitPageTuning(0);
			nTab = 0;
			m_TabCtrlParam.SetCurSel(nTab);
			InitPage(nTab);	
		  b_InitAxis=true;
		  ControlParam();
		  EndWaitCursor();
		  }
		  else
		  {//файл отсутствует 
            m_MOD=_T("");
		    str_EL = m_EL;
	        str_MOD = m_MOD;
		    strTestName = _T("");
			str_PRIM = _T("");
	        SetTextInfo(_T("Введите наименование теста!"));
	        UpdateData(FALSE);
		  }
		}//end of IDOK
		else
		{ //нажали CANCEL=>восстановить наименование теста
		  if (m_bFileOpen) 
		  { if (str_MOD.IsEmpty())
	            SetTextInfo(_T("Укажите наименование теста!"));
		    else 
			{ m_MOD = str_MOD;
		      m_EL = str_EL;
		      strTestName = str_EL + _T(" \\ ") + str_MOD;
	          SetTextInfo(_T("TEST:  ") + strTestName);
			}
		  }
		  else
		  { if (strTestName.IsEmpty())
	            SetTextInfo(_T("Укажите наименование теста!"));
		    else 
			{ m_MOD=strTestName;
	          SetTextInfo(_T("TEST:  ") + strTestName);
			}
		  }
	      UpdateData(FALSE);
		}
	  }//end of index==0
	}//end of (!m_MOD.IsEmpty())
		
}

void CLineDlg::ResetDisplay(int pointNull)
{
	UpdateData(TRUE);
	// панель "Дисплей"
	m_iButNull      = pointNull;// положение координаты (0,0)
//	m_bButInversion = FALSE;	// сброс инверсии осей 
//	m_bButInversionY= FALSE;	// сброс инверсии осей 
	m_iMarker	    = MARKER0;	// сброс текущего маркера
/*	switch (nCheckRef)
	{ case 0:	break;
	  case 1:   nCheckRef = 0;	
			    ShowControls(aVid3,SIZEOF_ARRAY(aVid3),TRUE);
			    ShowControls(aVid4,SIZEOF_ARRAY(aVid4),FALSE);
		        break;
	}
*/	int nTabTuning = m_TabCtrlTuning.GetCurSel();
	if (nTabTuning>0)
	{ nTabTuning = 0;
	  m_TabCtrlTuning.SetCurSel(nTabTuning);
	}
	InitPageTuning(nTabTuning);	// отобразить панель "Дисплей"
	UpdateData(FALSE);
	
//	InvalidateRect(&m_RectWndGraf,FALSE);	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
}

void CLineDlg::OnRadioRazvimppositive() 
{
	// TODO: Add your control notification handler code here
	int irazv_old = m_irazv;
	UpdateData(TRUE);
	if (n_IPPP==1 && n_Switch==2) {m_irazv = IMP_NEG;}// m_itipimp=TIP_IMP;}//---!
	UpdateData(FALSE);
//    i_tipimp = m_itipimp;
//	m_nMas=1;
	InitIconImp();
///	DefinitionDiapazonKol();
	ChangeDiapazonKol();	//InitDiapazonKol();
	ChangeDiapazonKolMeasU();
//	if (irazv_old!=IMP_NEG) ChangeDiapazonKol();//m_iKolI = -1;
	ShowRejimChanel();
	UpdateData(FALSE);
///    SetDiapazonKol();
//	bGenerationStup = true;
	ShowButtonLoopingCompensation();
	UpdateData(FALSE);
	if(iX==COLLECTOR && iY==COLLECTOR) ResetDisplay(LEFT);
	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
}

void CLineDlg::OnRadioRazvimpnegative() 
{
	// TODO: Add your control notification handler code here
	int irazv_old = m_irazv;
	UpdateData(TRUE);
	if (n_IPPP==1 && n_Switch==1) {m_irazv = IMP_POS;}// m_itipimp=TIP_IMP;}//---!
	UpdateData(FALSE);
//    i_tipimp = m_itipimp;
//	m_nMas=1;
	InitIconImp();
///	DefinitionDiapazonKol();
	ChangeDiapazonKol();	//	InitDiapazonKol();
	ChangeDiapazonKolMeasU();
//	if (irazv_old!=IMP_POS) ChangeDiapazonKol();//m_iKolI = -1;
	ShowRejimChanel();
	UpdateData(FALSE);
///    SetDiapazonKol();
//	bGenerationStup = true;
	ShowButtonLoopingCompensation();
	UpdateData(FALSE);
	if(iX==COLLECTOR && iY==COLLECTOR) ResetDisplay(RIGHT);
	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
}

void CLineDlg::OnRadioRazvdcpositive() 
{
	// TODO: Add your control notification handler code here
	int irazv_old;
	  UpdateData(TRUE);
//	if (n_IPPP==1 && n_Switch==1) m_irazv = IMP_POS;
//	if (n_IPPP==1 && n_Switch==2) m_irazv = IMP_NEG;
	  if (n_IPPP==1 && n_Switch==1) {m_irazv = IMP_POS;}// m_itipimp=TIP_IMP;}//---!
	  if (n_IPPP==1 && n_Switch==2) {m_irazv = IMP_NEG;}// m_itipimp=TIP_IMP;}//---!
	UpdateData(FALSE);
//    if (n_IPPP==0 || (n_IPPP==1 && n_Switch==0))
//    i_tipimp = m_itipimp;
	irazv_old = m_irazv;
//	DisableControls(aImpuls_OFF,SIZEOF_ARRAY(aImpuls_OFF),FALSE);
	InitIconImp();
///	DefinitionDiapazonKol();
	ChangeDiapazonKol();	//	InitDiapazonKol();
	ChangeDiapazonKolMeasU();
//	if (irazv_old!=DC_NEG) ChangeDiapazonKol();//m_iKolI = -1;
	ShowRejimChanel();
	UpdateData(FALSE);
///    SetDiapazonKol();
	m_nMas=1;
//	bGenerationStup = false;
	ShowButtonLoopingCompensation();
	UpdateData(FALSE);
	if(iX==COLLECTOR && iY==COLLECTOR) ResetDisplay(LEFT);
	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
}

void CLineDlg::OnRadioRazvdcnegative() 
{
	// TODO: Add your control notification handler code here
	int irazv_old;
	  UpdateData(TRUE);
//	if (n_IPPP==1 && n_Switch==1) m_irazv = IMP_POS;
//	if (n_IPPP==1 && n_Switch==2) m_irazv = IMP_NEG;
//	if (n_IPPP==0 || (n_IPPP==1 && n_Switch==0))
	  if (n_IPPP==1 && n_Switch==1) {m_irazv = IMP_POS;}// m_itipimp=TIP_IMP;}//---!
	  if (n_IPPP==1 && n_Switch==2) {m_irazv = IMP_NEG;}// m_itipimp=TIP_IMP;}//---!
	UpdateData(FALSE);
//	i_tipimp = m_itipimp;
	irazv_old = m_irazv;
//	DisableControls(aImpuls_OFF,SIZEOF_ARRAY(aImpuls_OFF),FALSE);
	InitIconImp();
///	DefinitionDiapazonKol();
	ChangeDiapazonKol();	//	InitDiapazonKol();
	ChangeDiapazonKolMeasU();
//	if (irazv_old!=DC_POS) ChangeDiapazonKol();//m_iKolI = -1;
	ShowRejimChanel();
	UpdateData(FALSE);
///    SetDiapazonKol();
	m_nMas=1;
//	bGenerationStup = false;
	ShowButtonLoopingCompensation();
	if(iX==COLLECTOR && iY==COLLECTOR) ResetDisplay(RIGHT);
	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
	UpdateData(FALSE);
	


}

void CLineDlg::OnRadioRazvac() 
{
	// TODO: Add your control notification handler code here
	int irazv_old;
	irazv_old = m_irazv;
	UpdateData(TRUE);
	if (n_Switch==0 && m_iQue1==BAZA) {m_irazv = irazv_old;UpdateData(FALSE);}// m_itipimp=TIP_IMP;}//---!
	else
	{
	if (n_IPPP==1 && n_Switch==1) {m_irazv = IMP_POS;}// m_itipimp=TIP_IMP;}//---!
	if (n_IPPP==1 && n_Switch==2) {m_irazv = IMP_NEG;}// m_itipimp=TIP_IMP;}//---!
	UpdateData(FALSE);
//	i_tipimp = m_itipimp;
//	if (n_IPPP==0 || (n_IPPP==1 && n_Switch==0))
//	irazv_old = m_irazv;
//	DisableControls(aImpuls_OFF,SIZEOF_ARRAY(aImpuls_OFF),FALSE);
	InitIconImp();
///	DefinitionDiapazonKol();
//	ChangeDiapazonKol();	//	InitDiapazonKol();
	ChangeDiapazonKol();//m_iKolI = -1;
	ChangeDiapazonKolMeasU();
	ShowRejimChanel();
	UpdateData(FALSE);
///    SetDiapazonKol();
	ShowButtonLoopingCompensation();
	m_nMas=2;
//	bGenerationStup = true;
	}
	if(iX==COLLECTOR && iY==COLLECTOR) ResetDisplay(CENTER);
	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
	UpdateData(FALSE);
	
}

void CLineDlg::InitDiapazonBaza()
{ // инициализация амплитуды ступеньки, диапазона смещения, диапазона ограничения и диапазона измерения
  //---------------------------------------------------------------------------------------------------
	UINT i=0;
/*
	m_uStepBaza = 0;				// число ступенек
	m_bCheckBazaRejHigh = FALSE;	// реж.высокого разрешения
    m_iAmplitudeStepBaza=-1;		// индекс диапазона амплитуды ступени и 
									// индекс диапазона смещения if в базе источник - I 
    m_iRangLimitBaza=-1;
	m_iBiasBaza = 0;
	m_dBiasBaza = 0.0;
	valueBiasBaza = 0.0;			// значение смещения базы
	m_iLimitBaza = 100;
	m_dLimitBaza = 0.0;
	valueLimitBaza = 0.0;			// значение ограничения базы
	m_iRangMeasBaza = -1;			// индекс диапазона измерения в базе:
									// измеритель != источнику => диапазон измерителя = диапазону ограничения
									// if измеритель = источнику => диапазон измерителя = диапазону источника
*/
	CComboBox *cbptrampl  = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEBAZA);
	CComboBox *cbptrlimit = (CComboBox*) GetDlgItem(IDC_COMBOBAZALIMITMAXUI);
    cbptrampl->ResetContent();
    cbptrlimit->ResetContent();

//  if (m_iCircuitBaza==1)
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	{//база
	switch(m_iBazaSource)
	{ case -1:  // обрыв 
			    cbptrampl->AddString(_T(""));
			    cbptrlimit->AddString(_T(""));
                m_iAmplitudeStepBaza=-1;
                m_iRangLimitBaza=-1;
				m_iRangMeasBaza = -1;		
				str_razmer_BiasBaza=_T("");
				str_razmer_LimitBaza=_T("");
	            break;
	  case 0:	//источник U 
				for (i=0;i<6;i++)			// амплитуда ступеньки (U): от 2 V до 5 mV
				cbptrampl->AddString(mas_range_ampl_bazadop_U[i]);
				m_sRangBiasBaza = mas_range_bias_bazadop_U[m_iAmplitudeStepBaza];
			    m_dBiasBaza = mas_d_range_bias_bazadop_U[m_iAmplitudeStepBaza];
				str_razmer_BiasBaza=_T(" V");
				//ограничение (по I)
				for (i=0;i<27;i++)			// диапазон ограничения (I): от 20 A до 5 nA
				cbptrlimit->AddString(mas_range_meas_bazadop_I[i]);
			    m_dLimitBaza = mas_d_range_meas_bazadop_I[m_iRangLimitBaza]+mas_d_range_meas_bazadop_I[m_iRangLimitBaza]/10;
				str_razmer_LimitBaza=_T(" A");
				// измеритель U	//I
//				m_iRangMeasBaza = m_iRangLimitBaza;		// индекс диапазона измерителя в базе
				m_iRangMeasBaza = m_iAmplitudeStepBaza;//m_iRangLimitBaza;		// индекс диапазона измерителя в базе
				break;
	  case 1:	//источник I 
				for (i=0;i<27;i++)			// амплитуда ступеньки (I): от 1 A до 0.2 nA
				cbptrampl->AddString(mas_range_ampl_bazadop_I[i]);
				m_sRangBiasBaza = mas_range_bias_bazadop_I[m_iAmplitudeStepBaza];
			    m_dBiasBaza = mas_d_range_bias_bazadop_I[m_iAmplitudeStepBaza];
				str_razmer_BiasBaza = _T(" A");
				//ограничение (по U)
				for (i=0;i<6;i++)			// диапазон ограничения (U): от 100 mV до 40 V
				cbptrlimit->AddString(mas_range_meas_bazadop_U[i]);
			    m_dLimitBaza = mas_d_range_meas_bazadop_U[m_iRangLimitBaza]+mas_d_range_meas_bazadop_U[m_iRangLimitBaza]/10;
				str_razmer_LimitBaza=_T(" V");
				// измеритель I	//U
//				m_iRangMeasBaza = m_iRangLimitBaza;		// индекс диапазона измерителя в базе
				m_iRangMeasBaza = m_iAmplitudeStepBaza;//m_iRangLimitBaza;		// индекс диапазона измерителя в базе
				break;
	}
  }//end of if (m_iCircuitBaza==1)
  else
  {
/*	  m_iBazaSource = -1;
    m_iBazaMeas = -1;
	cbptrampl->AddString(_T(""));
	cbptrbias->AddString(_T(""));
	cbptrlimit->AddString(_T(""));
//    m_iAmplitudeStepBaza=-1;
//    m_iRangLimitBaza=-1;
	str_razmer_BiasBaza=_T("");
	str_razmer_LimitBaza=_T("");
*/  }

  UpdateData(FALSE);
}

void CLineDlg::SetDiapazonBaza()
{
	UINT i=0;
//	UpdateData();	//убрала 29.11.17
	
	if(m_iAmplitudeStepBaza==-1) DefinitionDiapazonBaza();

	//амплитуда ступеньки, диапазон смещения, диапазон ограничения
	CComboBox *cbptrampl  = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEBAZA);
	CComboBox *cbptrlimit = (CComboBox*) GetDlgItem(IDC_COMBOBAZALIMITMAXUI);

    cbptrampl  -> SetCurSel(m_iAmplitudeStepBaza);
    cbptrlimit -> SetCurSel(m_iRangLimitBaza);

	//слайдер смещения
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERBAZABIAS);
	pSl -> SetPos(m_iBiasBaza);
	//слайдер ограничения
	pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERLIMITBAZA);
	pSl -> SetPos(m_iLimitBaza);
	switch(m_iBazaSource)
	{ case 0: //U
			  m_dBiasBaza = mas_d_range_bias_bazadop_U[m_iAmplitudeStepBaza];
			  m_dLimitBaza = mas_d_range_meas_bazadop_I[m_iRangLimitBaza]+mas_d_range_meas_bazadop_I[m_iRangLimitBaza]/10;
			  m_sRangBiasBaza = mas_range_bias_bazadop_U[m_iAmplitudeStepBaza];
	          break;
	  case 1: //I
			  m_dBiasBaza = mas_d_range_bias_bazadop_I[m_iAmplitudeStepBaza];
			  m_dLimitBaza = mas_d_range_meas_bazadop_U[m_iRangLimitBaza]+mas_d_range_meas_bazadop_U[m_iRangLimitBaza]/10;
			  m_sRangBiasBaza = mas_range_bias_bazadop_I[m_iAmplitudeStepBaza];
		      break;
	}

	//смешение
	valueBiasBaza=(m_dBiasBaza/1000.0)*m_iBiasBaza;
//	m_sBiasBaza.Format(_T("%.2e"),valueBiasBaza);
//  m_sBiasBaza+=str_razmer_BiasBaza;
    ValueToString(m_iBazaSource,valueBiasBaza,m_sBiasBaza);
	//ограничение
	valueLimitBaza=(m_dLimitBaza/110)*m_iLimitBaza;
//	m_sLimitBaza.Format(_T("%.2e"),valueLimitBaza);
//  m_sLimitBaza+=str_razmer_LimitBaza;
    ValueToString(!m_iBazaSource,valueLimitBaza,m_sLimitBaza);

//	UpdateData(FALSE);	//убрала 29.11.17
}

void CLineDlg::InitDiapazonDop()
{ // инициализация амплитуды ступеньки, диапазона смещения, диапазона ограничения и диапазона измерения
  //---------------------------------------------------------------------------------------------------

	UINT i=0;
/*
	m_uStepDop = 0;				// число ступенек
    m_iAmplitudeStepDop=-1;		// индекс диапазона амплитуды ступени и 
								// индекс диапазона смещения if в доп.кананале источник - I 
    m_iRangLimitDop=-1;
	m_iBiasDop = 0;
	m_dBiasDop = 0.0;
	valueBiasDop = 0.0;			// значение смещения доп.канала
	m_iLimitDop = 100;
	m_dLimitDop = 0.0;
	valueLimitDop = 0.0;		// значение ограничения доп. канала
	m_iRangMeasDop = -1;		// индекс диапазона измерителя в доп.канале
								// измеритель != источнику => диапазон измерителя = диапазону ограничения
								// if измеритель = источнику => диапазон измерителя = диапазону источника
*/	bias_Dop_I = 10;//9;

	CComboBox *cbptrampl  = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEDOP);
	CComboBox *cbptrlimit = (CComboBox*) GetDlgItem(IDC_COMBODOPLIMITMAXUI);
    cbptrampl->ResetContent();
    cbptrlimit->ResetContent();

	if (m_iCircuitDop==1 && m_iKnotDop==1)
// if (m_iCircuitDop==1)
 {
	switch(m_iDopSource)
	{ case -1:  // обрыв 
			    cbptrampl->AddString(_T(""));
			    cbptrlimit->AddString(_T(""));
                m_iAmplitudeStepDop=-1;
                m_iRangLimitDop=-1;
				str_razmer_BiasDop=_T("");
				str_razmer_LimitDop=_T("");
	            break;
	  case 0:	//источник U 
				for (i=0;i<6;i++)			// амплитуда ступеньки (U): от 2 V до 5 mV
				cbptrampl->AddString(mas_range_ampl_bazadop_U[i]);
				m_sRangBiasDop = mas_range_bias_bazadop_U[m_iAmplitudeStepDop];
    		    m_dBiasDop = mas_d_range_bias_bazadop_U[m_iAmplitudeStepDop];
				str_razmer_BiasDop=_T(" V");
				//ограничение (по I)
				for (i=bias_Dop_I;i<27;i++)// диапазон ограничения (I): от 20 mA до 5 nA
				cbptrlimit->AddString(mas_range_meas_bazadop_I[i]);
			    m_dLimitDop = mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I]+mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I]/10;
				str_razmer_LimitDop=_T(" A");
//				// измеритель I
//				m_iRangMeasDop = m_iRangLimitDop+bias_Dop_I; // индекс диапазона измерителя в базе
				// измеритель U
				m_iRangMeasDop = m_iAmplitudeStepDop; // индекс диапазона измерителя в базе
				break;
	  case 1:	//источник I
				for (i=bias_Dop_I;i<27;i++)			// амплитуда ступеньки (I): от 1 mA до 0.2 nA
				cbptrampl->AddString(mas_range_ampl_bazadop_I[i]);
				m_sRangBiasDop = mas_range_bias_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
			    m_dBiasDop = mas_d_range_bias_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
				str_razmer_BiasDop = _T(" A");
				//ограничение (по U)
				for (i=0;i<6;i++)			// диапазон ограничения (U): от 40 V до 100 mV
				cbptrlimit->AddString(mas_range_meas_bazadop_U[i]);
			    m_dLimitDop = mas_d_range_meas_bazadop_U[m_iRangLimitDop]+mas_d_range_meas_bazadop_U[m_iRangLimitDop]/10;
				str_razmer_LimitDop=_T(" V");
//				// измеритель U
//				m_iRangMeasDop = m_iRangLimitDop;	// индекс диапазона измерителя в базе
//				// измеритель I
				m_iRangMeasDop = m_iAmplitudeStepDop+bias_Dop_I;	// индекс диапазона измерителя в базе
				break;
	}
 }
 else
 {  m_iDopSource = -1;
    m_iDopMeas = -1;
	cbptrampl->AddString(_T(""));
	cbptrlimit->AddString(_T(""));
    m_iAmplitudeStepDop=-1;
    m_iRangLimitDop=-1;
	str_razmer_BiasDop=_T("");
	str_razmer_LimitDop=_T("");
 }
	UpdateData(FALSE);

}

void CLineDlg::SetDiapazonDop()
{
	UINT i=0;
	
//	UpdateData();		//убрала 29.11.17
	if(m_iAmplitudeStepDop==-1) DefinitionDiapazonDop();

	//амплитуда ступеньки, диапазон смещения, диапазон ограничения
	CComboBox *cbptrampl  = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEDOP);
	CComboBox *cbptrlimit = (CComboBox*) GetDlgItem(IDC_COMBODOPLIMITMAXUI);

    cbptrampl  -> SetCurSel(m_iAmplitudeStepDop);
    cbptrlimit -> SetCurSel(m_iRangLimitDop);

	//слайдер смещения
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERDOPBIAS);
	pSl -> SetPos(m_iBiasDop);
	//слайдер ограничения
	pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERLIMITDOP);
	pSl -> SetPos(m_iLimitDop);
	switch(m_iDopSource)
	{ case 0: //U
				m_sRangBiasDop = mas_range_bias_bazadop_U[m_iAmplitudeStepDop];
    		    m_dBiasDop = mas_d_range_bias_bazadop_U[m_iAmplitudeStepDop];
			    m_dLimitDop = mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I]+mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I]/10;
	          break;
	  case 1: //I
				m_sRangBiasDop = mas_range_bias_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
			    m_dBiasDop = mas_d_range_bias_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
			    m_dLimitDop = mas_d_range_meas_bazadop_U[m_iRangLimitDop]+mas_d_range_meas_bazadop_U[m_iRangLimitDop]/10;
		      break;
	}

	//смешение
	valueBiasDop=(m_dBiasDop/1000.0)*m_iBiasDop;
//	m_sBiasDop.Format(_T("%.2e"),valueBiasDop);
//  m_sBiasDop+=str_razmer_BiasDop;
	ValueToString(m_iDopSource,valueBiasDop,m_sBiasDop);					   
	//ограничение
	valueLimitDop=(m_dLimitDop/110)*m_iLimitDop;
//	m_sLimitDop.Format(_T("%.2e"),valueLimitDop);
//  m_sLimitDop+=str_razmer_LimitDop;
	ValueToString(!m_iDopSource,valueLimitDop,m_sLimitDop);
//	UpdateData(FALSE);	//убрала 29.11.17

}

void CLineDlg::DefinitionDiapazonBaza()
{ //инициализация диапазонов коллектора (в зависимости от вида развёртки)
  //=====================================================================
//  UpdateData();
//	m_uStepBaza = 10;				// число ступенек
//	m_bCheckBazaRejHigh = FALSE;	// реж.высокого разрешения
    m_iAmplitudeStepBaza=-1;		// индекс диапазона амплитуды ступени и 
									// индекс диапазона смещения if в базе источник - I 
    m_iRangLimitBaza=-1;
	m_iBiasBaza = 0;
	m_dBiasBaza = 0.0;
	valueBiasBaza = 0.0;			// значение смещения базы
	m_iLimitBaza = 100;
	m_dLimitBaza = 0.0;
	valueLimitBaza = 0.0;			// значение ограничения базы
	m_iRangMeasBaza = -1;			// индекс диапазона измерения в базе:
									// измеритель != источнику => диапазон измерителя = диапазону ограничения
									// if измеритель = источнику => диапазон измерителя = диапазону источника
	switch(m_iBazaSource)
	{ case 0:  // ист.U
				m_iAmplitudeStepBaza = 4;	// 100 mV
	            m_iRangLimitBaza = 22;		// 1 uA
				break;
	  case 1:  //ист. I
		        m_iAmplitudeStepBaza = 12;//50 uA	18;	// 1 uA
	            m_iRangLimitBaza = 5;		// 1 V
				break;
	}
  UpdateData(false);

}
void CLineDlg::DefinitionDiapazonDop()
{ //инициализация диапазонов коллектора (в зависимости от вида развёртки)
  //=====================================================================
//  UpdateData();

//	m_uStepDop = 0;				// число ступенек
    m_iAmplitudeStepDop=-1;		// индекс диапазона амплитуды ступени и 
								// индекс диапазона смещения if в доп.кананале источник - I 
    m_iRangLimitDop=-1;
	m_iBiasDop = 0;
	m_dBiasDop = 0.0;
	valueBiasDop = 0.0;			// значение смещения доп.канала
	m_iLimitDop = 100;
	m_dLimitDop = 0.0;
	valueLimitDop = 0.0;		// значение ограничения доп. канала
	m_iRangMeasDop = -1;		// индекс диапазона измерителя в доп.канале
								// измеритель != источнику => диапазон измерителя = диапазону ограничения
								// if измеритель = источнику => диапазон измерителя = диапазону источника
	bias_Dop_I = 10;//9;

	switch(m_iDopSource)
	{ case 0:  // ист.U
				m_iAmplitudeStepDop = 4;	// 100 mV
	            m_iRangLimitDop = 12;		// 1 uA
				break;
	  case 1:  //ист I
	            m_iAmplitudeStepDop = 9;	// 10 uA
	            m_iRangLimitDop = 5;		// 1V
				break;
	}
  UpdateData(false);

}


void CLineDlg::InitDiapazonKol()
{ //инициализация диапазонов коллектора (в зависимости от вида развёртки)
  //=====================================================================
  int i = 0;
  int k = 0;
  CString str = _T("");

//  UpdateData();
  CComboBox *cbptrU = (CComboBox*) GetDlgItem(IDC_COMBOKOLU);	//измеритель U
  cbptrU->ResetContent();
  CComboBox *cbptrI = (CComboBox*) GetDlgItem(IDC_COMBOKOLI);	//измеритель I
  cbptrI->ResetContent();
  CComboBox *cbptrsU = (CComboBox*) GetDlgItem(IDC_COMBOKOLSOURU);	//источник U
  cbptrsU->ResetContent();
//    if (m_iCircuitKol==1)
	{//вкл.
			switch(m_irazv)
			{ default:	m_iKolSourceU = 0;
						m_iKolU = -1;
						m_iKolI = -1;
						bias_KolSourceU = 0;
						bias_KolMeasI = 0;
						break;
			  case IMP_POS: //+IMPULS
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  for(i=bias_KolSourceU;i<5;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 0;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  cbptrU->AddString(mas_rangeKolMeasU[i]);
						  //Измеритель I
						  switch(m_iKolSourceU)
						  { case 0: bias_KolMeasI = 5;break;	//4
						    case 1: bias_KolMeasI = 8;break;	//7
							case 2: bias_KolMeasI = 10;break;	//9
							case 3: bias_KolMeasI = 12;break;	//11
						  }
						  
//							if (m_itipimp==TIP_SYN) k = 22;		//21	//---!	
//										else  k = 18;			//17	
//						  for(i=bias_KolMeasI;i<=k;i=i+1)			
						  for(i=bias_KolMeasI;i<=18;i=i+1)			
						  cbptrI->AddString(mas_rangeKolMeasI[i]);
						}
					    if (n_IPPP==1 && n_Switch==1)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  for(i=0;i<1;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  cbptrU->AddString(mas_rangeKolMeasU[i]);
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) bias_KolMeasI = 0;//3;
//						  else  bias_KolMeasI = 0;
						  bias_KolMeasI = 0;//200А
						  for(i=bias_KolMeasI;i<=3;i=i+1)			
						  cbptrI->AddString(mas_rangeKolMeasI[i]);
						}
						break;
			  case SYN_POS: //
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  for(i=bias_KolSourceU;i<5;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 0;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  cbptrU->AddString(mas_rangeKolMeasU[i]);
						  //Измеритель I
						  switch(m_iKolSourceU)
						  { case 0: bias_KolMeasI = 5;break;//4
						    case 1: bias_KolMeasI = 8;break;	//7
							case 2: bias_KolMeasI = 10;break;	//9
							case 3: bias_KolMeasI = 12;break;	//11
						  }
						  
//							if (m_itipimp==TIP_SYN) k = 22;		//21	//---!	
//										else  k = 18;			//17	
//						  for(i=bias_KolMeasI;i<=k;i=i+1)			
						  for(i=bias_KolMeasI;i<=22;i=i+1)			
						  cbptrI->AddString(mas_rangeKolMeasI[i]);
						}
					    if (n_IPPP==1 && n_Switch==1)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  for(i=0;i<1;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  cbptrU->AddString(mas_rangeKolMeasU[i]);
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) bias_KolMeasI = 0;//3;
//						  else  bias_KolMeasI = 0;
						  bias_KolMeasI = 0;//200А
						  for(i=bias_KolMeasI;i<=3;i=i+1)			
						  cbptrI->AddString(mas_rangeKolMeasI[i]);
						}
						break;
			    case DC_POS: //+DC
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  for(i=bias_KolSourceU;i<5;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 0;
						  for(i=bias_KolMeasU;i<=12;i=i+1)						
						  cbptrU->AddString(mas_rangeKolMeasU[i]);
						  //Измеритель I
						  if (m_iCircuitKol==0) bias_KolMeasI = 5;//4;  
						  else
						  {
						   if (m_iLimitKolU==0) bias_KolMeasI = 8;	//7
						   else 	  //bias_KolMeasI = 11;			//10;		//16;
								switch(m_iKolSourceU)
								{ case 0: bias_KolMeasI = 11;break;	//20V
								  case 1: bias_KolMeasI = 13;break;	//100V
								  case 2: bias_KolMeasI = 16;break;	//500V
							      case 3: bias_KolMeasI = 16;break;	//2000V
								}
						  }
						  for(i=bias_KolMeasI;i<MAXRANGEIKOL;i=i+1)			//33
						  cbptrI->AddString(mas_rangeKolMeasI[i]);
						}
						break;
			    case AC: //± AC
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  for(i=bias_KolSourceU;i<5;i++)
					      cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 0;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("±")+mas_rangeKolMeasU[i];
						    cbptrU->AddString(str);
						  }
						  //Измеритель I
						  switch(m_iKolSourceU)
						  { case 0: bias_KolMeasI = 5;break;		//6
						    case 1: bias_KolMeasI = 8;break;		//7
							case 2: bias_KolMeasI = 10;break;		//9
							case 3: bias_KolMeasI = 12;break;		//11;
						  }

//							if (m_itipimp==TIP_SYN) k = 22;			//21
//										else  k = 18;				//17
						  k = 22;
						  for(i=bias_KolMeasI;i<=k;i=i+1)			
						  { str = _T("±")+mas_rangeKolMeasI[i];
						    cbptrI->AddString(str);
						  }
						}
						break;
				case DC_NEG: //-DC
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  for(i=bias_KolSourceU;i<5;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 0;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("-")+mas_rangeKolMeasU[i];
						    cbptrU->AddString(str);
						  }
						  //Измеритель I
						  if (m_iCircuitKol==0) bias_KolMeasI = 5;//4;  
						  else
						  {
						  if (m_iLimitKolU==0) bias_KolMeasI = 8;		//7
									else 	 // bias_KolMeasI = 11;		//10		//16;
								switch(m_iKolSourceU)
								{ case 0: bias_KolMeasI = 11;break;	//20V
								  case 1: bias_KolMeasI = 13;break;	//100V
								  case 2: bias_KolMeasI = 16;break;	//500V
							      case 3: bias_KolMeasI = 16;break;	//2000V
								}
						  }
						  for(i=bias_KolMeasI;i<MAXRANGEIKOL;i=i+1)				//33
						  { str = _T("-")+mas_rangeKolMeasI[i];
						    cbptrI->AddString(str);
						  }
						}
						break;
				case SYN_NEG: //
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  for(i=bias_KolSourceU;i<5;i++)
						   cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 0;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("-")+mas_rangeKolMeasU[i];
						    cbptrU->AddString(str);
						  }
						  //Измеритель I
						  switch(m_iKolSourceU)
						  { case 0: bias_KolMeasI = 5;break;//4
						    case 1: bias_KolMeasI = 8;break;		//7
							case 2: bias_KolMeasI = 10;break;		//9
							case 3: bias_KolMeasI = 12;break;		//11
						  }

//							if (m_itipimp==TIP_SYN) k = 22;			//21//---!
//										else  k = 18;				//17
//						  for(i=bias_KolMeasI;i<=k;i=i+1)		
						  for(i=bias_KolMeasI;i<=22;i=i+1)		
						  { str=_T("-")+mas_rangeKolMeasI[i];
						    cbptrI->AddString(str);
						  }
						}
					    if (n_IPPP==1 && n_Switch==2)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  for(i=0;i<1;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("-")+mas_rangeKolMeasU[i];
						    cbptrU->AddString(str);
						  }
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) bias_KolMeasI = 0;//3;
//						  else bias_KolMeasI = 0;
						  bias_KolMeasI = 0;//200А
						  for(i=bias_KolMeasI;i<=3;i=i+1)			
						  { str=_T("-")+mas_rangeKolMeasI[i];
						    cbptrI->AddString(str);
						  }
						}
						break;
				case IMP_NEG: //-IMPULS
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  for(i=bias_KolSourceU;i<5;i++)
						   cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 0;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("-")+mas_rangeKolMeasU[i];
						    cbptrU->AddString(str);
						  }
						  //Измеритель I
						  switch(m_iKolSourceU)
						  { case 0: bias_KolMeasI = 5;break;//4
						    case 1: bias_KolMeasI = 8;break;		//7
							case 2: bias_KolMeasI = 10;break;		//9
							case 3: bias_KolMeasI = 12;break;		//11
						  }

//							if (m_itipimp==TIP_SYN) k = 22;			//21//---!
//										else  k = 18;				//17
//						  for(i=bias_KolMeasI;i<=k;i=i+1)		
						  for(i=bias_KolMeasI;i<=18;i=i+1)		
						  { str=_T("-")+mas_rangeKolMeasI[i];
						    cbptrI->AddString(str);
						  }
						}
					    if (n_IPPP==1 && n_Switch==2)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  for(i=0;i<1;i++)
						  cbptrsU->AddString(mas_rangeKolSourceU[i]);
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("-")+mas_rangeKolMeasU[i];
						    cbptrU->AddString(str);
						  }
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) bias_KolMeasI = 0;//3;
//						  else bias_KolMeasI = 0;
						  bias_KolMeasI = 0;//200А
						  for(i=bias_KolMeasI;i<=3;i=i+1)			
						  { str=_T("-")+mas_rangeKolMeasI[i];
						    cbptrI->AddString(str);
						  }
						}
						break;
			}//end of switch(m_irazv)
	}//end of if(m_iCircuitKol==1)
	UpdateData(FALSE);
}



void CLineDlg::DefinitionDiapazonKol()
{ //инициализация диапазонов коллектора (в зависимости от вида развёртки)
  //=====================================================================

 // UpdateData();
  m_iKolSourceU = 0;	//10V или 20V
  m_iLimitKolU  = 50;	//50 %
  m_iLimitKolI  = 0;	//100 %
	   m_iPosTek = m_iLimitKolI;
	   DrawStatusRukoyatka();	//индикация Старта по Uc
//    if (m_iCircuitKol==1)
	{//вкл.
			switch(m_irazv)
			{ default:	m_iKolSourceU = 0;
						m_iKolU = -1;
						m_iKolI = -1;
						bias_KolSourceU = 0;
						bias_KolMeasU = 0;
						bias_KolMeasI = 0;
						break;
			  case IMP_POS: //+IMPULS
			  case SYN_POS:
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  //Измеритель U
						  bias_KolMeasU = 0;
						  m_iKolU = 8;//7;								//10 V
						  //Измеритель I
						  bias_KolMeasI = 5;//4;
						  m_iKolI = 13;	//1 mA			//8; //50 mA
						}
					    if (n_IPPP==1 && n_Switch==1)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  m_iKolU = 0;//1;	5V
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 0; m_iKolI = 0;}//3;
//						  else  { bias_KolMeasI = 0; m_iKolI = 3;}	//20 A
						  bias_KolMeasI = 0;//200А
						  m_iKolI = 3;	//20 A
						}
						break;
			    case DC_POS: //+DC
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  //Измеритель U
						  bias_KolMeasU = 0;
						  m_iKolU = 8;//7;								//10 V
						  //Измеритель I
						  bias_KolMeasI = 11;	//200 mA	//16;
						  m_iKolI = 7;	//1;	//1 mA				//5;	// 50uA
						}
/*					    if (n_IPPP==1 && n_Switch==1)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  m_iKolU = 0;//1;	5V
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 3; m_iKolI = 0;}
//						  else  { bias_KolMeasI = 0; m_iKolI = 3;}	//20 A
						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 3; m_iKolI = 0;}
						  else  { bias_KolMeasI = 1; m_iKolI = 2;}	//20 A
						}
*/						break;
			    case AC: //± AC
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  //Измеритель U
						  bias_KolMeasU = 0;
					      m_iKolU = 8;//7;								//±10 V
						  //Измеритель I
						  bias_KolMeasI = 5;//4;
						  m_iKolI = 13;	//1 mA				//17;	//± 50 uA 
						}
//					    if (n_IPPP==1 && n_Switch==1)
/*					    if (n_IPPP==1 && n_Switch)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  m_iKolU = 0;//1;5V
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 3; m_iKolI = 0;}
//						  else  { bias_KolMeasI = 0; m_iKolI = 3;}	//20 A
						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 3; m_iKolI = 0;}
						  else  { bias_KolMeasI = 1; m_iKolI = 2;}	//20 A
						}
*/						break;
				case DC_NEG: //-DC
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  //Измеритель U
						  bias_KolMeasU = 0;
						  m_iKolU = 8;//7;								//-10 V
						  //Измеритель I
						  bias_KolMeasI = 11;	//200 mA	//16;
						  m_iKolI = 7;	//1;	//1 mA				//5;	// -50uA
						}
//					    if (n_IPPP==1 && n_Switch==1)
/*					    if (n_IPPP==1 && n_Switch==2)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  m_iKolU = 0;//1;5V
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 3; m_iKolI = 0;}
//						  else  { bias_KolMeasI = 0; m_iKolI = 3;}	//20 A
						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 3; m_iKolI = 0;}
						  else  { bias_KolMeasI = 1; m_iKolI = 2;}	//20 A
						}
*/						break;
				case IMP_NEG: //-IMPULS
				case SYN_NEG:
					    if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
						{
						  //Источник U
						  bias_KolSourceU = 1;
						  //Измеритель U
						  bias_KolMeasU = 0;
						  m_iKolU = 8;//7;								//-10 V
						  //Измеритель I
						  bias_KolMeasI = 5;//4;
						  m_iKolI = 13;	//1 mA					//9;//-20 mA
						}
//					    if (n_IPPP==1 && n_Switch==1)
					    if (n_IPPP==1 && n_Switch==2)
						{
						  //Источник U
						  bias_KolSourceU = 0;
						  //Измеритель U
						  bias_KolMeasU = 9;//6;
						  m_iKolU = 0;//1;5V
						  //Измеритель I
//						  if (m_itipimp==TIP_SYN) { bias_KolMeasI = 0; m_iKolI = 0;}//3;
//						  else  { bias_KolMeasI = 0; m_iKolI = 3;}	//20 A
						  bias_KolMeasI = 0;//200А
						  m_iKolI = 3;	//20 A
						}
						break;
			}//end of switch(m_irazv)
	}//end of if(m_iCircuitKol==1)
/*	else
	{ m_irazv = -1;
      m_iKolSourceU = 0;
	  m_iKolU = -1;
      m_iKolI = -1;
	  bias_KolMeasU = 0;
	  bias_KolMeasI = 0;
	  m_idlitimp = 0;
	}
*/
  m_idlitimp = 0;
  if (n_Switch==1 || n_Switch==2) m_idlitimp = 5 - (m_iKolSourceU+bias_KolSourceU);
	UpdateData(FALSE);
}


void CLineDlg::SetDiapazonKol()
{	
	int i = 0;
	CString str   = _T("");
	
//	UpdateData();	//убрала 29.11.17
	CComboBox *cbptrU = (CComboBox*) GetDlgItem(IDC_COMBOKOLU);	//измеритель U
	CComboBox *cbptrI = (CComboBox*) GetDlgItem(IDC_COMBOKOLI);	//измеритель I
	CComboBox *cbptrsU = (CComboBox*) GetDlgItem(IDC_COMBOKOLSOURU);	//источник U
	//источник U
	//**********
	cbptrsU->SetCurSel(m_iKolSourceU);
	//измеритель U
	//************
	cbptrU -> SetCurSel(m_iKolU);	//10 V
	//измеритель I
	//************
	cbptrI -> SetCurSel(m_iKolI);	//+-50 uA (для DC, AC), +-20 mA (для IMP)
	//ограничение по U 
	//****************
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLU);
	pSl -> SetPos(m_iLimitKolU);

//вместо =>
/*  
	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBOKOLU);
	m_iKOLU = pCb->GetCurSel();
	m_dLimitKolU = mas_range_kol_U[m_iKOLU];
	switch(m_irazv)
	{ case 0: case 1: break;
	  case 2: m_dLimitKolU/=2.0;break;
	  case 3: case 4: m_dLimitKolU*=(-1.0); break;
	}
	m_sRangMaxKolU.Format("%.2e V",m_dLimitKolU);
	valueLimitKolU=(m_dLimitKolU/110)*m_iLimitKolU;
	m_sLimitKolU.Format("%.2e V",valueLimitKolU); 
*/
//надо =>
	m_sLimitKolU.Format(_T(" %3d"),pSl->GetPos());
	m_sLimitKolU+=_T(" %");

	//ограничение по I 
	//****************
	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBOKOLI);
    pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLI);
//	m_iKolI = pCb->GetCurSel();//11.12.17 не понятно
	pSl -> SetPos(m_iLimitKolI);

	m_sLimitKolI.Format(_T(" %3d"),pSl->GetPos());
	m_sLimitKolI+=_T(" %");
//	UpdateData(FALSE);	//убоала 29.11.17
}

void CLineDlg::ValueToString(int xy, double value, CString& sStr)
{//вход:  xy - основные единицы измерения: 0-V, 1-I, 2-Om и т. д.;
 //       value - значение (напр., value = 0.000005)
 //выход: sStr - строка   (sStr = 5.00 u)
	bool flag = false;
	if (value<0.0) {value=value*(-1.0);flag=true;}//по модулю
//нормализуем value:
 	int n = 0;
 if (value!=0.0)
  {
    if (value<1.0)
    { do { value = value*10.0;
	       n = n - 1;
		} while (value<1.0);
	}
    if (value>=10.0)
    { do { value = value/10.0;
	       n = n + 1;
		} while (value>=10.0);
    }
  }
 
  int c = n / 3;	//порядок
  int p = n % 3;	//остаток
  CString prefix = _T("");
  CString ed_izm = _T("");
  if (n>=0)
	switch(c)
	{ case 0:		prefix = _T("");  break;	
	  case 1:		prefix = _T("k"); break;	
	  case 2:		prefix = _T("M"); break;	
	  case 3:		prefix = _T("G"); break;
	  case 4:		prefix = _T("T"); break;
	  case 5:		prefix = _T("P"); break;
	  default:      prefix = _T("");  break;//_T("");  break;
	}
  else
  {
   if (p==0)
	switch(c)
	{ case 0:	prefix = _T(""); break;	
	  case -1:	prefix = _T("m"); break;//_T("m"); break;
	  case -2:	prefix = _T("u"); break;//_T("u"); break;
	  case -3:	prefix = _T("n"); break;//_T("n"); break;
	  case -4:	prefix = _T("p"); break;//_T("p"); break;
	  case -5:	prefix = _T("f"); break;//_T("p"); break;
	  case -6:	prefix = _T("a"); break;//_T("p"); break;
	  default:  prefix = _T(""); break;//_T("");  break;
	}
    else
	{ p=p+3;
	  switch(c)
	  { case 0:		prefix = _T("m"); break;	
	    case -1:	prefix = _T("u"); break;//_T("m"); break;
	    case -2:	prefix = _T("n"); break;//_T("u"); break;
	    case -3:	prefix = _T("p"); break;//_T("n"); break;
	    case -4:	prefix = _T("f"); break;//_T("p"); break;
	    case -5:	prefix = _T("a"); break;//_T("p"); break;
	    default:    prefix = _T("");  break;//_T("");  break;
	  }
	}
 }
  switch (xy)
  { case 0:		ed_izm = _T("V"); break;
    case 1:		ed_izm = _T("A"); break;
	case 2:		ed_izm = _T("Om");break;
	case 3:		ed_izm = _T("");break;
	case 4:		ed_izm = _T("");break;
	default:    ed_izm = _T("");  break;
  }
  if (flag) value=value*(-1.0);//восстанавливаем знак
  switch(p)
  { default:	break;
    case 0:		sStr.Format(_T("%.3f %s%s"),value,prefix,ed_izm);
		        break;
	case 1:     value = value * 10.0; 
				sStr.Format(_T("%.2f %s%s"),value,prefix,ed_izm);
				break;
	case 2:     value = value * 100.0;
				sStr.Format(_T("%.1f %s%s"),value,prefix,ed_izm);
				break;
  }
}

void CLineDlg::OnSelchangeCombokolu() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

//    CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLU);
//	m_iLimitKolU=100;
//	pSl -> SetPos(m_iLimitKolU);
//	m_sLimitKolU.Format(_T("%d"),pSl->GetPos());
//	m_sLimitKolU+=_T(" %");
	UpdateData(FALSE);
	if (iX==COLLECTOR) {b_InitAxis=true;int er = ControlParam();}
}

void CLineDlg::OnSelchangeCombokoli() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBOKOLI);
    CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLI);
	m_iKolI = pCb->GetCurSel();
/*	m_iLimitKolI=0;
	pSl -> SetPos(m_iLimitKolI);

	int ind = bias_KolMeasI+m_iKolI;//*2;
	CString str = _T("");

	m_dLimitKolI = mas_rangeMaxKolMeasI[ind];
	m_sRangMaxKolI = mas_RangMaxKolMeasI[ind];
	switch(m_irazv)
	{ case IMP_POS: 
	  case DC_POS: 
	          valueLimitKolI=(m_dLimitKolI/110)*m_iLimitKolI;
//			  m_sLimitKolI.Format(_T("%.2eA"),valueLimitKolI);
	          ValueToString(1,valueLimitKolI,m_sLimitKolI);
			  break;
	  case AC: 
		      valueLimitKolI=(m_dLimitKolI/110)*m_iLimitKolI;
//	          m_sLimitKolI.Format(_T("±%.2eA"),valueLimitKolI);
	          ValueToString(1,valueLimitKolI,m_sLimitKolI);
		      m_sLimitKolI=_T("±")+m_sLimitKolI;
			  break;	
	  case DC_NEG: 
	  case IMP_NEG:
			  str = _T("-") + m_sRangMaxKolI;
			  m_sRangMaxKolI = 	str;
	          m_dLimitKolI*=(-1.0); 
			  valueLimitKolI=(m_dLimitKolI/110)*m_iLimitKolI;
//	          m_sLimitKolI.Format(_T("%.2eA"),valueLimitKolI);
	          ValueToString(1,valueLimitKolI,m_sLimitKolI);
		      break;
	}
*/	UpdateData(FALSE);
//	if (iY==COLLECTOR) 
	b_InitAxis=true;	int er = ControlParam();
//	if (iY==COLLECTOR) { InvalidateRect(&m_RectStr_Y,FALSE);
//						 InvalidateRect(&m_RectRazmer_Y,FALSE);
//						}
}

void CLineDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int k=0;
    int iLimitKolU_old = m_iLimitKolU;
	UpdateData();
	CSliderCtrl* pSl = (CSliderCtrl*)pScrollBar;
	CSliderCtrl* pSld = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLI);
	switch(pScrollBar->GetDlgCtrlID())
	{
      case IDC_SLIDERKOLU: //max ограничение по U (коллектор) - Стоп
						   m_iLimitKolU = pSl->GetPos();
						   m_sLimitKolU.Format(_T(" %3d"),pSl->GetPos());
						   m_sLimitKolU+=_T(" % ");

						   if (m_iQue1==BAZA) 
						   { 
							   m_iLimitKolI = m_iLimitKolU; 
							   pSld -> SetPos(m_iLimitKolI);
						   }
						   if (m_iQue1==COLLECTOR) 
						   {  
							    if ((m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG) && (m_bBeta==true || m_bBetai == true)) 
								{
								  if (m_iLimitKolU<=0) m_iLimitKolU = 1;
							      m_iLimitKolI = m_iLimitKolU - 1; 
								  pSld->SetPos(m_iLimitKolI);
																  
								}
							    else 
								{
//							      if ( m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG) 
							      if ((m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG) && (m_bBeta==false && m_bBetai == false)) 
								  { m_iLimitKolI = pSld->GetPos();}
								  else 
								  { m_iLimitKolI = 0; pSld -> SetPos(m_iLimitKolI);}
								}
							}
							m_sLimitKolI.Format(_T(" %3d"),m_iLimitKolI);
							m_sLimitKolI+=_T(" % ");
							m_iPosTek = m_iLimitKolI;
							DrawStatusRukoyatka();	//индикация Старта по Uc
//						   if (m_iQue1==COLLECTOR && (m_irazv==DC_POS || m_irazv==DC_NEG) && (m_iLimitKolU==0 ||iLimitKolU_old ==0))//(m_iLimitKolU==0)
//						    ChangeDiapazonKol();
						   break;
	  case IDC_SLIDERKOLI: //min ограничение по U (коллектор) - Старт
/*						   if (m_iQue1==COLLECTOR)
						   { if (m_irazv==IMP_POS || m_irazv==IMP_NEG ||
						         m_irazv==DC_POS || m_irazv==DC_NEG)
								{ m_iLimitKolI = pSl->GetPos();}
						     else 
								{ m_iLimitKolI = 0; pSl -> SetPos(m_iLimitKolI);}
						   }
*/
						   if (m_iQue1==COLLECTOR) 
						   {  
							    if ((m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG) && (m_bBeta==true || m_bBetai == true)) 
								{
								  if (m_iLimitKolU<=0) m_iLimitKolU = 1;
							      m_iLimitKolI = m_iLimitKolU - 1; 
								  pSld->SetPos(m_iLimitKolI);
																  
								}
							    else 
								{
//							      if ( m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG) 
							      if ((m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG) && (m_bBeta==false && m_bBetai == false)) 
								  { m_iLimitKolI = pSld->GetPos();}
								  else 
								  { m_iLimitKolI = 0; pSld -> SetPos(m_iLimitKolI);}
								}
							}
							if (m_iQue1==BAZA)
							{ m_iLimitKolI = m_iLimitKolU; 
							  pSl -> SetPos(m_iLimitKolI);
							}
						     m_sLimitKolI.Format(_T(" %3d"),m_iLimitKolI);//pSl->GetPos());
						     m_sLimitKolI+=_T(" % ");
						     m_iPosTek = m_iLimitKolI;
						     DrawStatusRukoyatka();	//индикация Старта по Uc
//						    ChangeDiapazonKol();
						   break;
	  case IDC_SLIDERBAZABIAS:
						   k=pSl->GetPos();
						   m_iBiasBaza = k;
		                   valueBiasBaza=(m_dBiasBaza/1000.0)*k;
/*						   switch(m_iBazaSource)
						   { case 0: // источник U
									 m_sBiasBaza.Format(_T("%.2e V"),valueBiasBaza);
									 break;
							 case 1: // источник I
						             m_sBiasBaza.Format(_T("%.2e A"),valueBiasBaza);
									 break;
						   }
*/
						   ValueToString(m_iBazaSource,valueBiasBaza,m_sBiasBaza);
						   
						   break;
	  case IDC_SLIDERLIMITBAZA:
						   k=pSl->GetPos();
						   m_iLimitBaza = k;
		                   valueLimitBaza=(m_dLimitBaza/110)*k;
/*						   switch(m_iBazaSource)
						   { case 0: // измеритель I
						             m_sLimitBaza.Format(_T("%.2e A"),valueLimitBaza);
									 break;
							 case 1: // измеритель U 
									 m_sLimitBaza.Format(_T("%.2e V"),valueLimitBaza);
									 break;
						   }
*/
						   ValueToString(!m_iBazaSource,valueLimitBaza,m_sLimitBaza);
						   break;
	  case IDC_SLIDERDOPBIAS:
						   k=pSl->GetPos();
						   m_iBiasDop = k;
		                   valueBiasDop=(m_dBiasDop/1000.0)*k;
/*						   switch(m_iDopSource)
						   { case 0: // источник U
									 m_sBiasDop.Format(_T("%.2e V"),valueBiasDop);
									 break;
							 case 1: // источник I
						             m_sBiasDop.Format(_T("%.2e A"),valueBiasDop);
						   }
*/
						   ValueToString(m_iDopSource,valueBiasDop,m_sBiasDop);
						   break;
//						   break;
	  case IDC_SLIDERLIMITDOP:
						   k=pSl->GetPos();
						   m_iLimitDop = k;
		                   valueLimitDop=(m_dLimitDop/110)*k;
/*						   switch(m_iDopSource)
						   { case 0: // измеритель I
						             m_sLimitDop.Format(_T("%.2e A"),valueLimitDop);
									 break;
							 case 1: // измеритель U 
									 m_sLimitDop.Format(_T("%.2e V"),valueLimitDop);
									 break;
						   }
*/
						   ValueToString(!m_iDopSource,valueLimitDop,m_sLimitDop);
						   break;
	}
	UpdateData(FALSE);
	ControlParam();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLineDlg::DrawStatusRukoyatka()
{
   ((CStatic*)GetDlgItem(IDC_STATICZONATEXT))->SetFont(&fontRukoyatka);
   ::WaitForSingleObject(hMutexRukoyatka,INFINITE);
	//=>для отладки!!!!!!!!!!!!!
//	strValueRukoyatka=m_sLimitKolU;//<= для отладки!!!!!!!!!!!!!
	strValueRukoyatka.Format(_T(" %s %3d"),m_sKolMeasU,(short)(m_iPosTek));	//m_sKolSourceU
    strValueRukoyatka+=_T("%");
	((CStatic*)GetDlgItem(IDC_STATICZONATEXT))->SetWindowText(strValueRukoyatka);
  TRACE(_T("DrawStatusRukoyatka = %s\n"),strValueRukoyatka);
   ::ReleaseMutex(hMutexRukoyatka);

//	UpdateWindow();
//	InvalidateRect(RectZonaText);
	Invalidate();
}

void CLineDlg::OnSelchangeCombokolsouru() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBOKOLSOURU);
	m_iKolSourceU = pCb->GetCurSel();
	if ((m_iKolSourceU+bias_KolSourceU)>=2) bSignal = TRUE;
										else bSignal = FALSE;
	switch(bSignal)
	{ case 0: ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),TRUE);
			  ShowControls(aSignal,SIZEOF_ARRAY(aSignal),FALSE); break;
	  case 1: ShowControls(aSignal,SIZEOF_ARRAY(aSignal),TRUE);
			  ShowControls(aNoSignal,SIZEOF_ARRAY(aNoSignal),FALSE); break;
	}
	InitIconImp();
	ChangeDiapazonKol();
//	SetDiapazonKol();
	UpdateData(false);

	b_InitAxis=true;
	ControlParam();
}

void CLineDlg::ChangeDiapazonKolSourceU()
{	int i;
    UpdateData();
	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBOKOLSOURU);
    pCb->ResetContent();
	if ((n_IPPP==0)||(n_IPPP==1 && n_Switch==0))
	{
	  //Источник U
	  bias_KolSourceU = 1;
	  for(i=bias_KolSourceU;i<5;i++)
	  pCb->AddString(mas_rangeKolSourceU[i]);//от 20 V до 2000 V
	}
//    if (n_IPPP==1 && n_Switch==1)
    if (n_IPPP==1 && n_Switch!=0)
	{
	  //Источник U
	  bias_KolSourceU = 0;
	  for(i=0;i<1;i++)
	  pCb->AddString(mas_rangeKolSourceU[i]);//5 V
	}
	pCb->SetCurSel(m_iKolSourceU);
    UpdateData(false);
}

void CLineDlg::ChangeDiapazonKolMeasU()
{	int i;
    CString str;

    UpdateData();
	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBOKOLU);
    pCb->ResetContent();
	if ((n_IPPP==0)||(n_IPPP==1 && n_Switch==0))
	{ switch(m_irazv)
		{//Измеритель U
			case IMP_POS: case DC_POS: case SYN_POS: 
						  bias_KolMeasU = 0;
						  for(i=0;i<=12;i=i+1)			
						  pCb->AddString(mas_rangeKolMeasU[i]);
						  m_iKolU = 8;//7;								//10 V
						  break;
			case AC:	
						  bias_KolMeasU = 0;
						  for(i=0;i<=12;i=i+1)			
						  { str = _T("±")+mas_rangeKolMeasU[i];
						    pCb->AddString(str);
						  }
						  m_iKolU = 8;//7;								//±10 V
						  break;
			case IMP_NEG: case DC_NEG: case SYN_NEG: 
						  bias_KolMeasU = 0;
						  for(i=0;i<=12;i=i+1)		
						  { str = _T("-")+mas_rangeKolMeasU[i];
						    pCb->AddString(str);
						  }
						  m_iKolU = 8;//7;								//10 V
						  break;
		}
	}
    if (n_IPPP==1 && n_Switch==1 || n_Switch==2)
//    if (n_IPPP==1 && n_Switch!=0)
	{ switch(m_irazv)
		{
			case IMP_POS: case SYN_POS: //case DC_POS: 
						  bias_KolMeasU = 9;//6;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  pCb->AddString(mas_rangeKolMeasU[i]);
						  m_iKolU = 0;								//5 V
						  break;
/*			case AC:	
						  bias_KolMeasU = 9;//6;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("±")+mas_rangeKolMeasU[i];
						    pCb->AddString(str);
						  }
						  m_iKolU = 0;								//±5 V
						  break;
*/			case IMP_NEG:  case SYN_NEG://case DC_NEG:
						  bias_KolMeasU = 9;//6;
						  for(i=bias_KolMeasU;i<=12;i=i+1)			
						  { str = _T("-")+mas_rangeKolMeasU[i];
						    pCb->AddString(str);
						  }
						  m_iKolU = 0;								//5 V
						  break;
		}
	}
	pCb->SetCurSel(m_iKolU);
    UpdateData(false);
}

void CLineDlg::ChangeDiapazonKol() 
{ int i=0;
  int k=0;
  int m=0;
  CString str=_T("");

    CComboBox *cbptrI = (CComboBox*) GetDlgItem(IDC_COMBOKOLI);	//измеритель I
    cbptrI->ResetContent();
	CComboBox *pCb = (CComboBox*) GetDlgItem(IDC_COMBOKOLSOURU);
	m_iKolSourceU = pCb->GetCurSel();
  if (m_iKolSourceU==0)//20V(ИППП3) или 5V(ИППП3/1)
  {
	if ((n_IPPP==0)||(n_IPPP==1 && n_Switch==0))
		{ switch(m_irazv)
			{ case IMP_POS: case IMP_NEG: 
				            bias_KolMeasI = 5;//4;				
											k = 18;		//17
							m = 13;//14;							//13
							break;
 			  case SYN_POS: case SYN_NEG: 
				            bias_KolMeasI = 5;//4;				
								k = 22;	//21
							m = 13;//14;							//13
							break;

			  case AC:	    bias_KolMeasI = 5;//6;				//4;			
							k = 22;							//21
							m = 13;							//13
				            break;
			  case DC_POS:  case DC_NEG:
							if (m_iCircuitKol==0)	//обрыв
							{ bias_KolMeasI = 5;//4;		
							  m = 13;						//13
							}
							else
							{  if (m_iLimitKolU==0)	//ограничение Uc = 0%
								{ bias_KolMeasI = 8;		//7
							      m = 10; }					//10
							   else 
							   { bias_KolMeasI = 11;		//10
							      m = 7; }					//7
							}
							k = MAXRANGEIKOL-1;							//33
							break;
			}
		}
    if (n_IPPP==1 && n_Switch==1)
		{ switch(m_irazv)
			{ case IMP_POS:  
							{bias_KolMeasI = 0; k = 3; m = 3;}	
							break;
			  case SYN_POS: 
							{bias_KolMeasI = 0; k = 3; m = 3;}	//3;
							break;
		}  
    if (n_IPPP==1 && n_Switch==2)
		{ switch(m_irazv)
			{  case IMP_NEG:
							{bias_KolMeasI = 0; k = 3; m = 3;}	
							break;
			   case SYN_NEG:
							{bias_KolMeasI = 0; k = 3; m = 3;}	//3;
							break;
		}  
  }
	if (m_iKolSourceU==1)//100V
	{ if (n_IPPP==0)
		{ switch(m_irazv)
			{ case IMP_POS: case IMP_NEG: 
				            bias_KolMeasI = 8;				//7
//							if (m_itipimp==TIP_SYN) k = 22;	//21
//										else
											k = 18;		//17
							m = 10;							//10
							break;
			  case SYN_POS: case SYN_NEG: 
				            bias_KolMeasI = 8;				//7
//							if (m_itipimp==TIP_SYN) 
								k = 22;	//21
//										else  k = 18;		//17
							m = 10;							//10
							break;
			  case AC:	    bias_KolMeasI = 8;				//7
							k = 22;							//21
							m = 10;							//10
				            break;
			  case DC_POS:  case DC_NEG:
							if (m_iLimitKolU==0) {bias_KolMeasI = 8; m = 10;}	//7; 10
							else 	  { bias_KolMeasI = 13; m = 5;} //11; m = 7;}	//10; 7
							k = MAXRANGEIKOL-1;							//33
							break;
			}
		}
      if (n_IPPP==1 && n_Switch==0)
		{ switch(m_irazv)
			{ case IMP_POS: case IMP_NEG: 
//							if (m_itipimp==TIP_SYN)
//							{bias_KolMeasI = 8; k = 22; m= 10;}	//7; 21; 10
//							else
							{bias_KolMeasI = 8; k = 18; m= 10;}	//7; 17; 10
							break;
			  case SYN_POS: case SYN_NEG: 
//							if (m_itipimp==TIP_SYN)
							{bias_KolMeasI = 8; k = 22; m= 10;}	//7; 21; 10
//							else
//							{bias_KolMeasI = 8; k = 18; m= 10;}	//7; 17; 10
							break;
			  case AC:		bias_KolMeasI = 8;					//7
							k = 22;								//21							
							m = 10;								//10						
				            break;
			  case DC_POS:  case DC_NEG:
							if (m_iLimitKolU==0) { bias_KolMeasI = 8; m=10;}	//7; 10
							else 	  {bias_KolMeasI = 13; m = 5;}	//11; m= 7;}	//10; 7
							k = MAXRANGEIKOL-1;								//33
							break;
			}
		}
	}
	if (m_iKolSourceU==2)//500V
	{ if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
		{ switch(m_irazv)
			{ case IMP_POS: case IMP_NEG: 
				            bias_KolMeasI = 10;				//9
//							if (m_itipimp==TIP_SYN) k = 22;	//21
//										else
							k = 18;		//17
							m = 8;							//8
							break;
			  case SYN_POS: case SYN_NEG: 
				            bias_KolMeasI = 10;				//9
//							if (m_itipimp==TIP_SYN) 
							k = 22;	//21
//										else  k = 18;		//17
							m = 8;							//8
							break;
			  case AC:		bias_KolMeasI = 10;				//9
							k = 22;							//21
							m = 8;							//12
				            break;
			  case DC_POS:  case DC_NEG:
							if (m_iLimitKolU==0) {bias_KolMeasI = 8; m= 10;}	//7; 10
							else 	  {bias_KolMeasI = 16; m = 2;}	//11; m= 7;}	//10; 7
							k = MAXRANGEIKOL-1;							//33
							break;
			}
		}
	}
	if (m_iKolSourceU==3)//2000V
	{ if ((n_IPPP==0) || (n_IPPP==1 && n_Switch==0))
		{ switch(m_irazv)
			{ case IMP_POS: case IMP_NEG: 
				            bias_KolMeasI = 12;				//11
//							if (m_itipimp==TIP_SYN) k = 22; //21
//										else 
							k = 18;		//17
							m = 6;	//6							
							break;
			  case SYN_POS: case SYN_NEG: 
				            bias_KolMeasI = 12;				//11
//							if (m_itipimp==TIP_SYN) 
							k = 22; //21
//										else  k = 18;		//17
							m = 6;	//6							
							break;
			  case AC:		bias_KolMeasI = 12;				//11
							k = 22;		//21							
							m = 6;		//6							
				            break;
			  case DC_POS:  case DC_NEG:
							if (m_iLimitKolU==0) {bias_KolMeasI = 8; m= 10;}	//7; 10
							else   {bias_KolMeasI = 16; m=2;}	//11; m=7;}	//10; 7
							k = MAXRANGEIKOL-1;							//33
							break;
			}
		}
	}
	if (m_irazv<AC) str=_T("");
	if (m_irazv==AC) str=_T("±");
	if (m_irazv>AC) str=_T("-");
	for(i=bias_KolMeasI;i<=k;i=i+1)			 
	cbptrI->AddString(str+mas_rangeKolMeasI[i]);
	m_iKolI = m;
	//ограничение по U (Старт)
	//****************========
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLI);
//    if (m_itipimp==TIP_IMP || m_irazv==DC_POS || m_irazv==DC_NEG)

	if (m_iQue1==BAZA)
	{ m_iLimitKolI = m_iLimitKolU; 
	  pSl -> SetPos(m_iLimitKolI);
	}
    if (m_iQue1==COLLECTOR)
    {  if ( m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG)
		{ m_iLimitKolI = pSl->GetPos();}
		else 
		{ m_iLimitKolI = 0; pSl -> SetPos(m_iLimitKolI);}
	}
    m_sLimitKolI.Format(_T(" %3d"),m_iLimitKolI);
    m_sLimitKolI+=_T(" % ");
	m_iPosTek = m_iLimitKolI;
	DrawStatusRukoyatka();	//индикация Старта по Uc
	
	UpdateData(false);
}


void CLineDlg::OnRadiobazasourceu() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
//	m_iBazaMeas = 1;//источник U => по умолчанию измеритель I
	m_iBazaMeas = 0;//источник U => по умолчанию измеритель U
	  m_bBaza = false;
	m_uStepBaza = 0;				// число ступенек
	DefinitionDiapazonBaza();
	InitDiapazonBaza();
	SetDiapazonBaza();
	SetStringsSourceMeas();
	UpdateData(FALSE);
	ControlParam();
	
}

void CLineDlg::OnRadiobazasourcei() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
//	m_iBazaMeas=0;	//источник I => по умолчанию измеритель U
	m_iBazaMeas=1;	//источник I => по умолчанию измеритель I
	  m_bBaza = false;
	m_uStepBaza = 0;				// число ступенек
	DefinitionDiapazonBaza();
	InitDiapazonBaza();
	SetDiapazonBaza();
	SetStringsSourceMeas();
	UpdateData(FALSE);
	ControlParam();
	
}
/*
void CLineDlg::OnRadiodopsourceu() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iDopMeas = 1;	//источник U => по умолчанию измеритель I
	m_uStepDop = 0;				// число ступенек
	DefinitionDiapazonDop();
	InitDiapazonDop();
	SetDiapazonDop();
	SetStringsSourceMeas();
	UpdateData(FALSE);
	ControlParam();
	
}

void CLineDlg::OnRadiodopsourcei() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iDopMeas=0;	//источник I => по умолчанию измеритель U
	m_uStepDop = 0;				// число ступенек
	DefinitionDiapazonDop();
	InitDiapazonDop();
	SetDiapazonDop();
	SetStringsSourceMeas();
	UpdateData(FALSE);
	ControlParam();
	
}
*/
void CLineDlg::OnUpdateEditur1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_sUr1.TrimLeft();
	m_sUr1.TrimRight();
	m_sUr2.TrimLeft();
	m_sUr2.TrimRight();
	if (m_sUr1.IsEmpty() || m_sUr2.IsEmpty())
	{ DisableControls(aFormula_OFF,SIZEOF_ARRAY(aFormula_OFF),FALSE);	
	  m_iForm = -1;
	}
	else
	{ DisableControls(aFormula_OFF,SIZEOF_ARRAY(aFormula_OFF),TRUE);
	  if (m_iForm=-1) m_iForm=0;
	}

    if (m_sPar.IsEmpty() || m_sArg.IsEmpty())
	{ DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	}
	else
	{ if (m_sUr1.IsEmpty() && m_sUr2.IsEmpty())
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	  else
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),TRUE);	
	}

	UpdateData(FALSE);
		
}

void CLineDlg::OnUpdateEditur2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_sUr1.TrimLeft();
	m_sUr1.TrimRight();
	m_sUr2.TrimLeft();
	m_sUr2.TrimRight();
	if (m_sUr1.IsEmpty() || m_sUr2.IsEmpty())
	{ DisableControls(aFormula_OFF,SIZEOF_ARRAY(aFormula_OFF),FALSE);	
	  m_iForm = -1;
	}
	else
	{ DisableControls(aFormula_OFF,SIZEOF_ARRAY(aFormula_OFF),TRUE);
	  if (m_iForm=-1) m_iForm=0;
	}

    if (m_sPar.IsEmpty() || m_sArg.IsEmpty())
	{ DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	}
	else
	{ if (m_sUr1.IsEmpty() && m_sUr2.IsEmpty())
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	  else
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),TRUE);	
	}

	UpdateData(FALSE);
	
}

void CLineDlg::OnUpdateEditnamepar() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_sPar.TrimLeft();
	m_sPar.TrimRight();
	m_sArg.TrimLeft();
	m_sArg.TrimRight();
    if (m_sPar.IsEmpty() || m_sArg.IsEmpty())
	{ DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	}
	else
	{ if (m_sUr1.IsEmpty() && m_sUr2.IsEmpty())
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	  else
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),TRUE);	
	}

	UpdateData(FALSE);
	
	
}

void CLineDlg::OnUpdateEditnamearg() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_sPar.TrimLeft();
	m_sPar.TrimRight();
	m_sArg.TrimLeft();
	m_sArg.TrimRight();
    if (m_sPar.IsEmpty() || m_sArg.IsEmpty())
	{ DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	}
	else
	{ if (m_sUr1.IsEmpty() && m_sUr2.IsEmpty())
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),FALSE);	
	  else
	  DisableControls(aPorog_OFF,SIZEOF_ARRAY(aPorog_OFF),TRUE);	
	}

	UpdateData(FALSE);
	
}

void CLineDlg::InitPageTuning(int nPage)
{ //  CWnd* pwnd;
//	UpdateData();
	switch(nPage)
	{ case 0: //Шкала
			  ShowControls(aTuningPage1,SIZEOF_ARRAY(aTuningPage1),FALSE);
			  ShowControls(aTuningPage2,SIZEOF_ARRAY(aTuningPage2),FALSE);
			  ShowControls(aTuningPage0,SIZEOF_ARRAY(aTuningPage0),TRUE);
			  if (nCheckParam) GetDlgItem(IDC_BUTTON_MARKER_RESET)->ShowWindow(SW_SHOW);
			             else  GetDlgItem(IDC_BUTTON_MARKER_RESET)->ShowWindow(SW_HIDE);
			  ShowButtonLoopingCompensation();
			  break;
	case 1: //Ошибки
			  ShowControls(aTuningPage0,SIZEOF_ARRAY(aTuningPage0),FALSE);
			  ShowControls(aTuningPage1,SIZEOF_ARRAY(aTuningPage1),FALSE);
			  ShowControls(aTuningPage2,SIZEOF_ARRAY(aTuningPage2),TRUE);
		      DisplayError();
			  break;
	}
//	UpdateData(FALSE);
}

void CLineDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	CClientDC dc(this);

	CPoint mpoint;
	mpoint=point;
    dc.DPtoLP(&mpoint);

	if (RgnExit.PtInRegion(mpoint))
	{//скрытый выход из программы в WinCE
		BOOL fl= FALSE;
	if (MessageBox(_T("Вы уверены, что хотите завершить работу?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		if (m_bFileOpen) fl = FileClose();
				   else  fl = TRUE;
		if (fl)
		{
		  ASSERT(AfxGetMainWnd()!=NULL);
		 AfxGetMainWnd()->SendMessage(WM_CLOSE);
		}
	}
	}
	int nTab = m_TabCtrlParam.GetCurSel();
	if (nTab==0)
	{
		if (RgnDiod.PtInRegion(mpoint)) ButtonDiod();
		if (RgnRes.PtInRegion(mpoint))  ButtonRes();
		if (RgnNpn.PtInRegion(mpoint))	ButtonNpn();
		if (RgnPnp.PtInRegion(mpoint))	ButtonPnp();
		if (RgnNmos.PtInRegion(mpoint))	ButtonNmos();
		if (RgnPmos.PtInRegion(mpoint))	ButtonPmos();
		if (RgnNpn4.PtInRegion(mpoint)) ButtonNpn4();
		if (RgnPnp4.PtInRegion(mpoint)) ButtonPnp4();
		if (RgnNmos4.PtInRegion(mpoint))ButtonNmos4();
		if (RgnPmos4.PtInRegion(mpoint))ButtonPmos4();

	}//end of nTab==0

//	int nTabTuning = m_TabCtrlTuning.GetCurSel();

//	if (nTabTuning==1)
//	{   
		if (nCheckParam)
		{
	      if (RgnButtonMarker1.PtInRegion(mpoint))
		  {//Маркер1
		    m_iMarker=MARKER1;
//		    InitPageTuning(1);
		    m_Chart.SetActiveMarker(0);
		    DisplayMarker();
		  }
		  if (RgnButtonMarker2.PtInRegion(mpoint))
		  {//Маркер2
		    m_iMarker=MARKER2;
// 		    InitPageTuning(1);
           m_Chart.SetActiveMarker(1);
		    DisplayMarker();
		  }
		  if (RgnButtonTangent.PtInRegion(mpoint))
		  {//образующая
		    m_bTangent=!m_bTangent;
//		    InitPageTuning(1);
		    if (!m_bTangent)
			{ m_Chart.DrawSecant(false);
		      (CWnd*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT_ON)->ShowWindow(SW_HIDE);
			  (CWnd*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT)->ShowWindow(SW_SHOW);
			}
		    else
			{ m_Chart.DrawSecant(true);
		      (CWnd*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT)->ShowWindow(SW_HIDE);
			  (CWnd*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT_ON)->ShowWindow(SW_SHOW);
			}
		    DisplaySecant();
		  }
		}
//	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CLineDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    if(m_bCapt) 
	{ ReleaseCapture();
      m_bCapt=FALSE;
	}	
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CLineDlg::ReverseAxis() 
{ // поменять местами соответственно: m_iRejX, m_iX, rej_X, expo_X, m_sAxisX, iX. X_min, X_max и
								   // m_iRejY, m_iY, rej_Y, expo_Y, m_sAxisY, iY. Y_min, Y_max
	int temp = 0;
	double d = 0.0;
	CString str = _T("");

	temp = m_iX;
	m_iX = m_iY;
	m_iY = temp;
	
	temp = rej_X;
	rej_X = rej_Y;
	rej_Y = temp;
	
	temp = expo_X;
	expo_X = expo_Y;
	expo_Y = temp;

	str = m_sAxisX;
	m_sAxisX = m_sAxisY;
	m_sAxisY = str;

	temp = iX;
	iX = iY;
	iY = temp;

	d = X_min;
	X_min = Y_min;
	Y_min = d;

	d = X_max;
	X_max = Y_max;
	Y_max = d;


}

void CLineDlg::OnSelchangeTabctrltuning(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nTab = m_TabCtrlTuning.GetCurSel();
	InitPageTuning(nTab);
	
	*pResult = 0;
}


void CLineDlg::ResetMarkers()
{
	m_Chart.DisableRefresh(true); 
			  m_uN = 0;
			  m_bTangent = false;
			  m_iMarker=MARKER1;
			  m_Chart.CreateMarker(m_uN);//№ кривой
//			  m_Chart.SetActiveMarker(MARKER0);
			  m_Chart.CreateMarker(m_uN);//№ кривой
//			  m_Chart.m_pMarkersList[0]->SetColor(RGB(0,0,0));//цвет контура маркера
			  m_Chart.m_pMarkersList[1]->SetFillColor(RGB(0,0,255));//цвет заливки маркера
			  m_Chart.m_pMarkersList[1]->SetPosition(0.0, 0.0);
			  m_Chart.m_pMarkersList[0]->SetFillColor(RGB(255,0,0));//цвет заливки маркера
			  m_Chart.m_pMarkersList[0]->SetPosition(0.0, 0.0);
				(CWnd*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT_ON)->ShowWindow(SW_HIDE);
				(CWnd*)GetDlgItem(IDC_STATIC_BUTTON_TANGENT)->ShowWindow(SW_SHOW);
			    m_Chart.DrawSecant(false);
			  
			  ((CStatic*)GetDlgItem(IDC_MARKER2))->SetWindowText(_T(""));

			  m_Chart.SetActiveMarker(MARKER0);

			  m_Chart.SetRefreshMarkerOnMouseMove(false);			  
			  
			  DisplayMarker();
			  DisplaySecant();
    m_Chart.DisableRefresh(false); 
}

void CLineDlg::SetVidMemDC()
{// установить текущий контект экрана
 // в зависимости от nCheckParam и nCheckRef
}

void CLineDlg::DisplayMarker() 
{//отображение положения маркера1
	CString text;
	double XValue, YValue, RValue;
	CString str_x,str_y,str_r;

  if (bStatusCalc == false) rej_Y = rej_Y_Meas;

//  if (!nCheckRef)
//  {
	switch(m_iMarker)
	{ case MARKER0: 
			        ShowControls(aMarker_OFF,SIZEOF_ARRAY(aMarker_OFF),TRUE);
			        ShowControls(aMarker_ON,SIZEOF_ARRAY(aMarker_ON),FALSE);
		            break;
	  
	  case MARKER1:
					((CWnd*)GetDlgItem(aMarker_OFF[m_iMarker]))->ShowWindow(SW_HIDE);
					((CWnd*)GetDlgItem(aMarker_ON[m_iMarker]))->ShowWindow(SW_SHOW);
					((CWnd*)GetDlgItem(aMarker_OFF[m_iMarker+1]))->ShowWindow(SW_SHOW);
					((CWnd*)GetDlgItem(aMarker_ON[m_iMarker+1]))->ShowWindow(SW_HIDE);
		            break;
	  case MARKER2: 
					((CWnd*)GetDlgItem(aMarker_OFF[m_iMarker]))->ShowWindow(SW_HIDE);
					((CWnd*)GetDlgItem(aMarker_ON[m_iMarker]))->ShowWindow(SW_SHOW);
					((CWnd*)GetDlgItem(aMarker_OFF[m_iMarker-1]))->ShowWindow(SW_SHOW);
					((CWnd*)GetDlgItem(aMarker_ON[m_iMarker-1]))->ShowWindow(SW_HIDE);
		            break;
	} 
//  }


	m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->GetValues(XValue, YValue);
	XValue = XValue * pow(10.0,expo_X-po_X);
	YValue = YValue * pow(10.0,expo_Y-po_Y);
//	if (fabs(XValue)<EPSILON) XValue = EPSILON; //чтобы избежать деления на 0
//	if (fabs(YValue)<EPSILON) YValue = EPSILON;

	if (rej_X==0 && rej_Y==1 && YValue!=0.0) RValue = XValue / YValue;
	else if (rej_Y==0 && rej_X==1 && XValue!=0.0) RValue = YValue / XValue;
			else  	if (rej_Y==2 || rej_Y==3 || rej_Y==4) RValue =  YValue; //по Y-ку R, Beta или Betai
					else  RValue = 0.0;
	int iMarker = m_Chart.m_iActiveMarkerIndex + 1; 

	ValueToString(rej_X,XValue,str_x);
	ValueToString(rej_Y,YValue,str_y);
	ValueToString(2,RValue,str_r);

//	text.Format(_T(" marker%d:  X = % .3e,   Y = % .3e,   R%d = % .3e Om"),iMarker,XValue,YValue,iMarker,RValue);
	if (rej_Y==2 || rej_Y==3 || rej_Y==4) text.Format(_T(" marker%d:  X%d = %s,   Y%d = %s"),iMarker,iMarker,str_x,iMarker,str_y);
	         else text.Format(_T(" marker%d:  X%d = %s,   Y%d = %s,   R%d = %s"),iMarker,iMarker,str_x,iMarker,str_y,iMarker,str_r);
	if(m_Chart.m_iActiveMarkerIndex==0)
	 ((CStatic*)GetDlgItem(IDC_MARKER1))->SetWindowText(text);
	else
	 ((CStatic*)GetDlgItem(IDC_MARKER2))->SetWindowText(text);
	
	
}

void CLineDlg::DisplaySecant()
{   
	CString text,str_x,str_y,str_a,str_b;
	double X1Value, Y1Value;
	double X2Value, Y2Value;
	double AValue, BValue;
	double XValue, YValue;

	m_Chart.m_pMarkersList[0]->GetValues(X1Value, Y1Value);
	X1Value = X1Value * pow(10.0,expo_X-po_X);
	Y1Value = Y1Value * pow(10.0,expo_Y-po_Y);
	if (fabs(X1Value)<EPSILON) X1Value = 0.0;
	if (fabs(Y1Value)<EPSILON) Y1Value = 0.0;
	m_Chart.m_pMarkersList[1]->GetValues(X2Value, Y2Value);
	X2Value = X2Value * pow(10.0,expo_X-po_X);
	Y2Value = Y2Value * pow(10.0,expo_Y-po_Y);
	if (fabs(X2Value)<EPSILON) X2Value = 0.0;
	if (fabs(Y2Value)<EPSILON) Y2Value = 0.0;

	if (fabs(X2Value - X1Value)>= EPSILON) AValue = (Y2Value - Y1Value) / (X2Value - X1Value);
	else   AValue = 0.0;

	BValue = Y1Value - AValue * X1Value;

	//x=0
	YValue = BValue;
	//y=0
	if (fabs(AValue) >= EPSILON) XValue = (-1.0) * (BValue / AValue);
	else  XValue = 0.0;

	ValueToString(7,XValue,str_x);
	ValueToString(7,YValue,str_y);
//	ValueToString(7,AValue,str_a);
//	ValueToString(7,BValue,str_b);
//	text.Format(_T(" tangent:    Y = % .2e * X +% .2e,   X = 0.0  Y = % .2e,   Y = 0.0  X = % .2e"),AValue,BValue,YValue,XValue);
	
//	text.Format(_T(" tangent:    Y = % .3f  *  X + % .3f,   X = 0.0  Y = %s,   Y = 0.0  X = %s"),AValue,BValue,str_y,str_x);
	text.Format(_T(" tangent:    X = 0.0  Y = %s,   Y = 0.0  X = %s"),str_y,str_x);

	if (m_bTangent) ((CStatic*)GetDlgItem(IDC_TANGENT))->SetWindowText(text);
			   else ((CStatic*)GetDlgItem(IDC_TANGENT))->SetWindowText(_T(" tangent:"));

}

void CLineDlg::DisplayParam() 
{//отображение параметров текущего теста
	CString stre;
	CString str1;
	CStringArray array_MEAS;	//
	array_MEAS.RemoveAll();
	int n_i = array_MEAS.GetSize();
    CString str = _T(" Meas:\t") + m_EL + _T(" \\ ") + m_MOD+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
    str=_T(" Дата:\t") +strDateMeas;//+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
    str=_T(" \t") +strTimeMeas+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	str = _T("       Условия измерения:\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
//	str = m_PRIM+_T("\r\n");
//	array_MEAS.SetAtGrow(n_i,str);
//	n_i++;
	if (m_iQue1==COLLECTOR) str = _T(" Канал С: 1-ая очередь\r\n");
		else str = _T(" Канал С:\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;

	UpdateData();
	CComboBox *ptrU = (CComboBox*) GetDlgItem(IDC_COMBOKOLU);
    int n = ptrU->GetLBTextLen(m_iKolU);
    ptrU->GetLBText( m_iKolU, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" ")+m_sKolMeasU+_T(": ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;

	CComboBox *ptrI = (CComboBox*) GetDlgItem(IDC_COMBOKOLI);
    n = ptrI->GetLBTextLen(m_iKolI);
    ptrI->GetLBText( m_iKolI, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" ")+m_sKolMeasI+_T(": ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	if (m_iCircuitBaza==1)
	{
	if (m_iQue1==BAZA) str = _T("\r\n Канал B: 1-ая очередь\r\n")+m_sBazaSource+_T("\r\n");
			else str = _T("\r\n Канал B:  ")+m_sBazaSource+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	CComboBox *ptrA = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEBAZA);
    n = ptrA->GetLBTextLen(m_iAmplitudeStepBaza);
    ptrA->GetLBText( m_iAmplitudeStepBaza, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" Амплитуда: ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	CComboBox *ptrP = (CComboBox*) GetDlgItem(IDC_COMBOPOLARITYSTEP);
    n = ptrP->GetLBTextLen(m_iPolarityStepBaza);
    ptrP->GetLBText( m_iPolarityStepBaza, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" Полярность: ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	str1.Format(_T("%d"),m_uStepBaza);
	str = _T(" Число ступенек: ")+str1+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	str = _T(" Смещение: ")+m_sBiasBaza+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	CComboBox *ptrPB = (CComboBox*) GetDlgItem(IDC_COMBOPOLARITYBAZABIAS);
    n = ptrPB->GetLBTextLen(m_iPolarityBiasBaza);
    ptrPB->GetLBText( m_iPolarityBiasBaza, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" Полярность: ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;

	}
	else
	{ if (m_iCircuitBaza!=-1)
	{
	CComboBox *ptrB = (CComboBox*) GetDlgItem(IDC_COMBOCHECKBAZA);
    n = ptrB->GetLBTextLen(m_iCircuitBaza);
    ptrB->GetLBText( m_iCircuitBaza, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T("\r\n Канал B:  ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	}
	}
	if (m_iCircuitDop==1)
	{
	str = _T("\r\n Канал S:  ")+m_sDopSource+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	CComboBox *ptrAD = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEDOP);
    n = ptrAD->GetLBTextLen(m_iAmplitudeStepDop);
    ptrAD->GetLBText( m_iAmplitudeStepDop, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" Амплитуда: ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	CComboBox *ptrPD = (CComboBox*) GetDlgItem(IDC_COMBOPOLARITYSTEPDOP);
    n = ptrPD->GetLBTextLen(m_iPolarityStepDop);
    ptrPD->GetLBText( m_iPolarityStepDop, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" Полярность: ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	str1.Format(_T("%d"),m_uStepDop);
	str = _T(" Число ступенек: ")+str1+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	str = _T(" Смещение: ")+m_sBiasDop+_T("\r\n");
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	CComboBox *ptrPBD = (CComboBox*) GetDlgItem(IDC_COMBOPOLARITYDOPBIAS);
    n = ptrPBD->GetLBTextLen(m_iPolarityBiasDop);
    ptrPBD->GetLBText( m_iPolarityBiasDop, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T(" Полярность: ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;

	}
	else
	{ if (m_iCircuitDop!=-1)
	{
	CComboBox *ptrBD = (CComboBox*) GetDlgItem(IDC_COMBOCHECKDOP);
    n = ptrBD->GetLBTextLen(m_iCircuitDop);
    ptrBD->GetLBText( m_iCircuitDop, str.GetBuffer(n) );
    str.ReleaseBuffer();
    str1.Format(_T("%s\r\n"), str.GetBuffer(0));
	str = _T("\r\n Канал S:  ")+str1;
	array_MEAS.SetAtGrow(n_i,str);
	n_i++;
	}
	}


    ((CStatic*)GetDlgItem(IDC_EDITDISPLAYMEAS))->SetFont(&fontParam);
    CEdit* ep = (CEdit*) GetDlgItem(IDC_EDITDISPLAYMEAS);
	n_i = array_MEAS.GetSize();
	for(int i=0;i<n_i;i++)	
	{ str = array_MEAS.GetAt(i);
	  stre=stre+str;
	}
	ep ->SetWindowText(stre);

}


void CLineDlg::DisplayRef()
{ //отображение параметров ссылки
    CString str,stre;
//	CString strNameRef;
	((CStatic*)GetDlgItem(IDC_EDITDISPLAYREF))->SetFont(&fontParam);
    CEdit* ep = (CEdit*) GetDlgItem(IDC_EDITDISPLAYREF);
	if (bStatusRef)
	{ 
      int m_ref = m_listRef.GetSelectionMark();
	  if (m_ref>=0)
	  { strNameRef = array_ref.GetAt(m_ref);	//имя файла с эталонным графиком
	    stre = strNameRef;
	  }
      else stre =_T("");
	}
	else stre =_T("");
	str = _T(" Ref: ") + stre;
	stre = _T("\r\n     Условия измерения:\r\n ")+strPrimRef;
	str = str + stre;
	ep ->SetWindowText(str);
}

void CLineDlg::ClearOtcht()
{
/*	int i;
	CString str;

    CEdit* ep = (CEdit*) GetDlgItem(IDC_EDITOTCHT);
	int n=array_otcht.GetSize();
	for(i=0;i<n;i++)
	{
	  array_otcht.SetAtGrow(i,_T("")); 
	  str = array_otcht.GetAt(i);
      ep -> SetWindowText(str);
	}
*/
	array_otcht.RemoveAll();
    m_sEL_old = _T("");		//элемент
    m_sPart_old = _T("");	//№ партии
    m_sPlast_old = _T("");	//№ пластины
	m_sPoint_old  = _T("");	//№ точки
}

void CLineDlg::DisplayOtcht(void)
{//индикация отчета
	int i,n;
	CString str,stre;

    CEdit* ep = (CEdit*) GetDlgItem(IDC_EDITOTCHT);
	n = array_otcht.GetSize();
	for(i=0;i<n;i++)	
	{ str = array_otcht.GetAt(i);
	  stre=stre+str;
	}
//	stre=_T("Это отчёт.");///!!!
	ep ->SetWindowText(stre);
	ep ->LineScroll(n+1,0);
}

void CLineDlg::OnButtonOtcht() 
{
	// TODO: Add your control notification handler code here
	  akt_button = 106;	

    CMenu menu;
    menu.LoadMenu(IDR_MENUFLOATINGOTCHT);
    ASSERT(menu);
    CPoint point;
    point.x=RectButtonOtcht.left+10;
    point.y=RectWnd.bottom-170;

//    if (bStateOtcht == stateMin) menu.GetSubMenu(0)->EnableMenuItem(ID_OTCHT_MIN,MF_GRAYED);
//                            else menu.GetSubMenu(0)->EnableMenuItem(ID_OTCHT_MAX,MF_GRAYED);

    menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	
}

void CLineDlg::OnOtchtNew() 
{
	// TODO: Add your command handler code here
	ClearOtcht();
	DisplayOtcht();
	
}

void CLineDlg::OnOtchtMax() 
{
	// TODO: Add your command handler code here
	bStateOtcht = stateMax;
	ClientToScreen(&RectOtcht);
//	RectOtcht.top=RectInfoTest.bottom;
//	RectOtcht.bottom=RectWnd.bottom-(RectWnd.bottom/20)-6;
	RectOtcht.top=RectInfoTest.bottom;
	ScreenToClient(&RectOtcht);
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_EDITOTCHT);
	pSt->MoveWindow(&RectOtcht);
	ShowControls(aVid5,SIZEOF_ARRAY(aVid5),TRUE);
	SetTextInfo(_T("\nОтчёт о проведенных измерениях\n"));
	DisplayOtcht();
}

void CLineDlg::OnOtchtMin() 
{
	// TODO: Add your command handler code here
	bStateOtcht = stateMin;
	ClientToScreen(&RectOtcht);
//	RectOtcht.top=RectWnd.top+43;//RectWnd.bottom/20+2;//RectButtons.top-RectWnd.bottom/20-20;//7;
//	RectOtcht.bottom=RectOtcht.top+33;//RectWnd.bottom-(RectWnd.bottom/20)-6;
	RectOtcht.top=m_RectWndGraf.bottom+3;
	ScreenToClient(&RectOtcht);
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_EDITOTCHT);
	pSt->MoveWindow(&RectOtcht);
	ShowControls(aVid5,SIZEOF_ARRAY(aVid5),TRUE);
	SetTextInfo(_T("TEST: ") + strTestName);
	DisplayOtcht();
}

void CLineDlg::Otcht() 
{ //формирование строки отчёта по окончании измерения
	// rej_X, rej_Y
    CString str  = _T(" ");
	CString strc = _T("");

//	ExtractResultsCalculate();
if (m_uFunc>1)
{
	if (m_bR)	 rej_Y = 2;		// R
    if (m_bBeta) rej_Y = 3;		// Beta
	if (m_bBetai) rej_Y = 4;	// Betai
	if (m_bS) rej_Y = 5;		// S
}
	CString ed_izm_x = _T("");
	switch(rej_X)
	{ case 0:		ed_izm_x = _T(" V"); break;
      case 1:		ed_izm_x = _T(" A"); break;
	  case 2:		ed_izm_x = _T(" Om");break;
	  case 3:		ed_izm_x = _T("");break;
	  case 4:		ed_izm_x = _T("");break;
	  case 5:		ed_izm_x = _T(" A/V");break;
	  default:      ed_izm_x = _T("");  break;
	}
	CString ed_izm_y = _T("");
	switch(rej_Y)
	{ case 0:		ed_izm_y = _T(" V"); break;
      case 1:		ed_izm_y = _T(" A"); break;
	  case 2:		ed_izm_y = _T(" Om");break;
	  case 3:		ed_izm_y = _T("");break;
	  case 4:		ed_izm_y = _T("");break;
	  case 5:		ed_izm_y = _T(" A/V");break;
	  default:      ed_izm_y = _T("");  break;
	}

	UINT n = array_otcht.GetSize();
//	UpdateData(TRUE);
	if (m_sEL_old    != m_EL || m_sPart_old  != m_sPart || m_sPlast_old != m_sPlast || m_sPoint_old != m_sPoint)
	{ m_sEL_old    = m_EL;	   	//тестируемый элемент
	  m_sPart_old  = m_sPart;	//№ партии
	  m_sPlast_old = m_sPlast;	//№ пластины
	  m_sPoint_old = m_sPoint;	//№ точки
	  str = str + strDateMeas+_T("\tПарт.№ ")+m_sPart+_T("\tПласт.№ ")+m_sPlast+_T("\tЭлемент: ")+m_EL+_T("\tТ.№ ")+m_sPoint+_T("\r\n");
//	  array_otcht.SetAtGrow(n,str); 
	  array_otcht.Add(str); 

	  uFileNumber=0;
    
    n++;
	}
	strc = m_MOD;
	str = strTimeMeas+_T("\t\t")+strc+_T(":\t");
/*	if (m_sPar.IsEmpty() && m_bR==0 &&	m_bBeta==0 && m_bBetai==0 && m_bS==0)
	{// strc.Format(_T("Параметры для расчета не заданы.\r\n"));
	 // str = str + strc;
     // array_otcht.SetAtGrow(n,str); 
     // n++;
	}
	else
	{//1
*/
	  if (!m_sPar.IsEmpty() && !m_sArg.IsEmpty())// && (m_uFunc==0 || m_uFunc==1 || ( m_uFunc==2 && bStatusCalc==true)))
	  { //2
		if (Flag_sboy && Flag)
		{ str = str+_T("Значение ")+ m_sPar+_T(" не найдено.");
		}
		else
		{//3
	    if (!Flag_sboy)
		{//4//if (!Flag_sboy)
		if (!m_sUr1.IsEmpty() && !m_sUr2.IsEmpty())
		{//5 //крутизна
		    if (m_uFunc==0)// ||  m_uFunc==2 ||m_uFunc==4)
			{ 
			  strc = _T("Xa = ") + m_sUr1 + ed_izm_x; 
		      str = str + strc;
			  strc.Format(_T("  Ya = % .3e"),r1);  
			  str = str + strc + ed_izm_y +_T("  Xb = ")+ m_sUr2 + ed_izm_x;
			  strc.Format(_T("  Yb = % .3e"),r2);
			  str = str + strc + ed_izm_y +_T("  ");
			}
			if (m_uFunc==1)//||  m_uFunc==3)
			{ strc.Format(_T("Xa = % .3e"),r1);
			  str = str + strc + ed_izm_x + _T("  Ya = ") + m_sUr1 + ed_izm_y;
			  strc.Format(_T("  Xb = % .3e"),r2);
			  str = str + strc + ed_izm_x + _T("  Yb = ") + m_sUr2 + ed_izm_y;
			}
		  if (m_iForm==0) strc.Format(_T("  (Yb-Ya)/(Xb-Xa) = % .3e"),Par);
		             else strc.Format(_T("  (Xb-Xa)/(Yb-Ya) = % .3e"),Par);
	      if (Flag)  strc = strc + _T(" (*)");
		        else strc = strc + _T("");
	      str = str + strc + _T("\t");
		}//5//end of крутизна
		else
		{//5) 
		  str = str + m_sPar + _T(" = ");
		  strc.Format(_T("% .3e"),Par);
		  switch(m_uFunc)
		  { case 0: case 4:	strc = strc + ed_izm_y; break;
			case 2:		 strc = strc + ed_izm_y;break;
			case 1:      strc = strc + ed_izm_x; break;
			case 3:      strc = strc + ed_izm_y; break;
			default:     break;
		  }
	      if (Flag) strc = strc + _T(" (*) при ");
		       else strc = strc + _T("     при ");
		  switch(m_uFunc)
		  { case 0:	case 4: strc = strc + m_sArg + _T(" = ") + m_sUr1 + ed_izm_x+_T("\t"); 
					break;
			case 2:	strc = strc + m_sArg + _T(" = ") + m_sUr1 +_T(" A\t"); 
					break;
			case 1: strc = strc + m_sArg + _T(" = ")+ m_sUr1 + ed_izm_y+_T("\t"); 
				    break;
			case 3:	
/*					if (m_iBazaSource==0 && m_bBaza==0)
						 strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" V\t");
					else strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" A\t");
*/					
					if (!m_bBaza)
					{ if (m_iBazaSource==1) strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" A\t");
						else strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" V\t");
					}
					else
					{ 
						if(m_sArg==m_sAxisY)
						{
						if (m_iBazaMeas==1) strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" A\t");
						else strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" V\t");
						}
					    if(m_sArg==m_sAxisX)
						{
						if (m_iBazaSource==1) strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" A\t");
						else strc = strc + m_sArg + _T(" = ")+ m_sUr1 +_T(" V\t");
						}
					}
	
					break;
			default:break;
		  }
	      str = str + strc + _T("\t");
		}//5)

		if (m_bBeta)  
		{ strc.Format(_T("Beta max = % .3e\tIb при Beta max = % .3e A\t"),Beta_max,Ib_Beta_max);
		  str = str + strc;
		}
		if (m_bBetai)
		{ strc.Format(_T("Betai max = % .3e\tIb при Betai max = % .3e A\t"),Betai_max,Ib_Betai_max);
		  str = str + strc;
		}
		    if (m_bLooping) strc.Format(_T("(Компенсация \"петли\" вкл.)."));
		               else if (m_iQue1!=BAZA && m_irazv!=DC_POS && m_irazv!=DC_NEG 
						     && m_irazv!=IMP_POS && m_irazv!=IMP_NEG) strc.Format(_T("(Компенсация \"петли\" выкл.)."));
					        else strc=_T("");

		}//4//end (!Flag_sboy)
		else
		{//4
		    if (m_bLooping) strc.Format(_T("(Компенсация \"петли\" нарушена)."));
		               else strc.Format(_T("(Превышен порог)."));
		}//4
	    str = str + strc;
		}//3//end of !Flag_sboy
	  } //end of m_sPar

/*
	}//1)
*/
	    str = str + _T("\r\n");
//        array_otcht.SetAtGrow(n,str); 
        array_otcht.Add(str); 
        n++;
	TRACE(L"Otch()!!!\n");
}

void CLineDlg::ExtractResultsCalculate()
{ //выборка результатов расчета
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);//???_???
}

void CLineDlg::OnPart() 
{
	// TODO: Add your command handler code here
	CPartDlg dlg;
	dlg.m_sProduct=m_sProduct;
	dlg.m_sPart=m_sPart;
	dlg.m_sPlast=m_sPlast;
	dlg.m_sPoint=m_sPoint;
	UpdateData(FALSE);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
	  m_sProduct=dlg.m_sProduct;
	  m_sPart=dlg.m_sPart;
	  m_sPlast=dlg.m_sPlast;
	  m_sPoint=dlg.m_sPoint;
	  BOOL b = SetPassWordLine();
	}
	
}

void CLineDlg::OnButtonTuning() 
{
	// TODO: Add your control notification handler code here
	  akt_button = 102;	
  CMenu menu;
  menu.LoadMenu(IDR_MENUFLOATINGTUNING);
  ASSERT(menu);
  CPoint point;
  point.x=RectButtonTuning.left+10;
  point.y=RectWnd.bottom-230;

  if (bZapusk) { menu.GetSubMenu(0)->CheckMenuItem(ID_ZAPUSK_EXTERNAL, MF_CHECKED | MF_BYCOMMAND);
				 menu.GetSubMenu(0)->CheckMenuItem(ID_ZAPUSK_INTERNAL, MF_UNCHECKED | MF_BYCOMMAND);}
          else { menu.GetSubMenu(0)->CheckMenuItem(ID_ZAPUSK_INTERNAL, MF_CHECKED | MF_BYCOMMAND);
				 menu.GetSubMenu(0)->CheckMenuItem(ID_ZAPUSK_EXTERNAL, MF_UNCHECKED | MF_BYCOMMAND);}
  if (bSound) { menu.GetSubMenu(0)->CheckMenuItem(ID_SOUND_ON, MF_CHECKED | MF_BYCOMMAND);
				menu.GetSubMenu(0)->CheckMenuItem(ID_SOUND_OFF, MF_UNCHECKED | MF_BYCOMMAND);}
		 else { menu.GetSubMenu(0)->CheckMenuItem(ID_SOUND_ON, MF_UNCHECKED | MF_BYCOMMAND);
				menu.GetSubMenu(0)->CheckMenuItem(ID_SOUND_OFF, MF_CHECKED | MF_BYCOMMAND);}
//  if (bAutoSetNull) menu.GetSubMenu(0)->EnableMenuItem(ID_AUTO_SETUP_NULL, MF_GRAYED);

 menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
}

void CLineDlg::OnButtonGraf() 
{
	// TODO: Add your control notification handler code here
	  akt_button = 104;	
  if (bStateListRef)
  { ShowControls(aVid6,SIZEOF_ARRAY(aVid6),false);
	bStateListRef=false;
	CString str;
	str.Format(_T("TEST:  %s \\ %s."),m_EL,m_MOD);//\tПроведите измерение или выберите другую модель!
	SetTextInfo(str);//_T("Проведите измерение или выберите другую модель!"));
  }
  CMenu menu;
  menu.LoadMenu(IDR_MENUFLOATINGGRAF);
  ASSERT(menu);
  CPoint point;
  point.x=RectButtonGraf.left+10;
  point.y=RectWnd.bottom-180;

  if (bStatusRef || bPusk)// || bStatusCalc)
  {//если на экране присутствует ссылка, менять стиль кривых нельзя
	  //или включено измерение
	    menu.GetSubMenu(0)->EnableMenuItem(ID_CURVE_LINE,MF_GRAYED);
	    menu.GetSubMenu(0)->EnableMenuItem(ID_CURVE_POINT,MF_GRAYED);
	    menu.GetSubMenu(0)->EnableMenuItem(ID_CURVE_LINEWITHPOINT,MF_GRAYED);
  }
 if (!bStatusMeas)
 {//если на экране нет измеренного графика, сохранять в качестве ссылки нечего
	    menu.GetSubMenu(0)->EnableMenuItem(ID_SAVE_REF,MF_GRAYED);
 }
 if (nCheckParam)
 {//если активны маркеры, менять стиль кривой нельзя
	    menu.GetSubMenu(0)->EnableMenuItem(ID_CURVE_LINE,MF_GRAYED);
	    menu.GetSubMenu(0)->EnableMenuItem(ID_CURVE_POINT,MF_GRAYED);
	    menu.GetSubMenu(0)->EnableMenuItem(ID_CURVE_LINEWITHPOINT,MF_GRAYED);
 }
 
 switch (m_iStyleCurve)
  { case STYLE_LINE:	menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_LINE, MF_CHECKED | MF_BYCOMMAND);
						menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_POINT, MF_UNCHECKED | MF_BYCOMMAND);
						menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_LINEWITHPOINT, MF_UNCHECKED | MF_BYCOMMAND);
						break;
	case STYLE_POINT:   menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_LINE, MF_UNCHECKED | MF_BYCOMMAND);
						menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_POINT, MF_CHECKED | MF_BYCOMMAND);
						menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_LINEWITHPOINT, MF_UNCHECKED | MF_BYCOMMAND);
						break;
	case STYLE_LINEPOINT: menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_LINE, MF_UNCHECKED | MF_BYCOMMAND);
						menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_POINT, MF_UNCHECKED | MF_BYCOMMAND);
						menu.GetSubMenu(0)->CheckMenuItem(ID_CURVE_LINEWITHPOINT, MF_CHECKED | MF_BYCOMMAND);
						break;
  }
  if (!bStatusRef)
  {//если на экране нет ссылки, то нечего сбрасывать
	menu.GetSubMenu(0)->EnableMenuItem(ID_REF_RESET,MF_GRAYED);
  }
  else menu.GetSubMenu(0)->EnableMenuItem(ID_SAVE_REF,MF_GRAYED);

  if (!nCheckParam) 
  { menu.GetSubMenu(0)->CheckMenuItem(ID_MARKER_AKT, MF_UNCHECKED | MF_BYCOMMAND);
	menu.GetSubMenu(0)->CheckMenuItem(ID_MARKER_NOAKT, MF_CHECKED | MF_BYCOMMAND); 
  }
  else 
  { menu.GetSubMenu(0)->CheckMenuItem(ID_MARKER_AKT, MF_CHECKED | MF_BYCOMMAND);
    menu.GetSubMenu(0)->CheckMenuItem(ID_MARKER_NOAKT, MF_UNCHECKED | MF_BYCOMMAND);
  }
/*  if(!m_bCheckKolRejHigh)
  { 
	menu.GetSubMenu(0)->EnableMenuItem(ID_GRAF_LUPA,MF_GRAYED);
  }
*/
	switch(size_point)
	{case 0:	menu.GetSubMenu(0)->CheckMenuItem(ID_POINT_SIZE_SMALL, MF_CHECKED | MF_BYCOMMAND);
				menu.GetSubMenu(0)->CheckMenuItem(ID_POINT_SIZE_BIG, MF_UNCHECKED | MF_BYCOMMAND);
				break;
	 case 1:	menu.GetSubMenu(0)->CheckMenuItem(ID_POINT_SIZE_SMALL, MF_UNCHECKED | MF_BYCOMMAND);
				menu.GetSubMenu(0)->CheckMenuItem(ID_POINT_SIZE_BIG, MF_CHECKED | MF_BYCOMMAND);
				break;
	}
  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	
	
}

void CLineDlg::OnButtonMemory() 
{
	// TODO: Add your control notification handler code here
	  akt_button = 103;	
  CMenu menu;
  menu.LoadMenu(IDR_MENUFLOATINGMEMORY);
  ASSERT(menu);
  CPoint point;
  point.x=RectButtonMemory.left+10;
  point.y=RectWnd.bottom-150;
  
  if (!bStatusRef)
  {//если на экране нет ссылки, то нечего сбрасывать
	menu.GetSubMenu(0)->EnableMenuItem(ID_RESET_REF,MF_GRAYED);
  }
  int i_ref = m_listRef.GetItemCount();
  if(!i_ref)
  {//если нет ни одной ссылки, то не надо очищать архив, нечего удалять и загружать
	menu.GetSubMenu(0)->EnableMenuItem(ID_CLEAR_MEMORY,MF_GRAYED);
//	menu.GetSubMenu(0)->EnableMenuItem(ID_COPY_REF_TO_FLASH,MF_GRAYED);
	menu.GetSubMenu(0)->EnableMenuItem(ID_DEL_MEMORY,MF_GRAYED);
	menu.GetSubMenu(0)->EnableMenuItem(ID_EXTRACT_REF,MF_GRAYED);

  }

  int m_ref = m_listRef.GetSelectionMark();
  if (m_ref==-1 || bStateListRef == false)
  {//если не выбрана ссылка, то нечего удалять и нечего загружать
	menu.GetSubMenu(0)->EnableMenuItem(ID_DEL_MEMORY,MF_GRAYED);
	menu.GetSubMenu(0)->EnableMenuItem(ID_EXTRACT_REF,MF_GRAYED);
  }
  
  if (bStateListRef == false) menu.GetSubMenu(0)->EnableMenuItem(ID_LISTREF_CLOSE,MF_GRAYED);
                        else menu.GetSubMenu(0)->EnableMenuItem(ID_LISTREF_OPEN,MF_GRAYED);
 
  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	
	
}

void CLineDlg::OnSaveTstUser() 
{
	// TODO: Add your command handler code here
	int where = TST_USER;
	BOOL b = SaveParamToTst(where);
	if (b)
	MessageBox(_T("Сохранение завершено успешно!"),NULL,MB_OK|MB_ICONINFORMATION);	
}

BOOL CLineDlg::SaveParamToTst(int where) 
{
	CString nameFileTst = _T("");
	CString str  = _T("");
	CString stre = _T("");
	CFile   file;
	int     l;
	BOOL fl = FALSE;
	CFileException er;
	int er_param = 0;
	int n=0;
	int i=0;
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);//???_???

/*	if (m_pTreeDlg && pModSelected==NULL)
	{ MessageBox(_T("Укажите тест в модели измерения для сохранения!"),NULL,MB_OK|MB_ICONEXCLAMATION);
	  return FALSE;
	}
*/
//	if (m_pTreeDlg==NULL && m_MOD.IsEmpty())
	if (pModSelected==NULL && m_MOD.IsEmpty())
	{ MessageBox(_T("Укажите наименование активного теста для сохранения!"),NULL,MB_OK|MB_ICONEXCLAMATION);
	  return FALSE;
	}
//	if (m_pTreeDlg==NULL && !m_MOD.IsEmpty())
	if (!m_MOD.IsEmpty())
	{ //0) Выбор входных параметров
	  //if (pModSelected)
	  ExtractParam();
      //ExtractParamGraf();
	  //1) Контроль параметров
	  TRACE (L"Контроль параметров\n");
	  er_param = ControlParam();
	  if (er_param)
	  { MessageBox(_T("В параметрах сохраняемого теста обнаружены ошибки!\nСохранение запрещено!"),NULL,MB_OK|MB_ICONERROR);//XCLAMATION);
	    return FALSE;
	  }
      //2) Построение пути
      if (pModSelected) { str = pModSelected->m_MOD;
						  m_PRIM = pModSelected->m_PRIM;
						}
				else    str = m_MOD;
	  TRACE (str);
	  switch(where)
	  { case TST_STANDART: //сохранение в библиотеке стандартных тестов
						   nameFileTst=_T("\\Hard Disk\\Tst_standart\\")+str+_T(".tst");//_T("\\Line\\Tst\\")+
						   BuildArrayTstStandart();
						   n = array_tst_standart.GetSize();
						   break;
	    case TST_USER:	   //сохранение в библиотеке пользовательских тестов
						   nameFileTst=_T("\\Hard Disk\\Tst_user\\")+str+_T(".tst");//  T(\\Line\\Tst\\)
						   BuildArrayTstUser();
						   n = array_tst_user.GetSize();
						   break;
	  }

	  CSaveTstDlg std;
	  std.m_uWhere = where;
	  std.m_snameTst = str;
	  std.m_sPrim = m_PRIM;
	  std.m_sPath = nameFileTst;
	  UpdateData(false);

	if (std.DoModal()==IDOK)
	{//%%%%
		UpdateData();
		str = std.m_snameTst;
		m_MOD = str;
	    nameFileTst= std.m_sPath;
		m_PRIM = std.m_sPrim;
      if (!m_MOD.IsEmpty())
	  {//3) 
		//контроль на совпадение
		BOOL fl1 = false;
/*		for(i=0;i<n;i++)
		{ if (where==TST_STANDART) str = array_tst_standart.GetAt(i);
							  else str = array_tst_user.GetAt(i);
		  str=str.Mid(0,str.GetLength()-4);
		  if (str == std.m_snameTst) { fl1 = true; break;}
		}
		 if (fl1)
		 { fl1 = true;
		    str.Format(_T("Файл с именем %s.tst уже существует. Перезаписать?"),std.m_snameTst);
			if (MessageBox(str,NULL,MB_YESNO|MB_ICONEXCLAMATION)==IDYES) fl1= false;
		 }
*/

	HANDLE hFileTst;
	WIN32_FIND_DATA fd;
	CString str_name_tst;

	
	hFileTst=::FindFirstFile(nameFileTst,&fd);	//
	
	if (hFileTst != INVALID_HANDLE_VALUE)
	{ BOOL b = true;
	  while (b)
	  { //массив имён файлов 
		str_name_tst.Format(L"%s",fd.cFileName);
		if ((str+_T(".tst"))==str_name_tst){ fl1 = true; break;}
	    //поиск всех файлов *.tst
		b=FindNextFile(hFileTst,&fd);
	  }	//while (b)
	}

		 if (fl1)
		 { fl1 = true;
		    str.Format(_T("Файл с именем %s.tst уже существует. Перезаписать?"),std.m_snameTst);
			if (MessageBox(str,NULL,MB_YESNO|MB_ICONEXCLAMATION)==IDYES) fl1= false;
		 }


		if (!fl1) 
		{ //архивирование
     BOOL b = file.Open((LPCTSTR)nameFileTst,CFile::modeCreate|CFile::modeWrite);
//	    if (!file.Open((LPCTSTR)nameFileTst,CFile::modeCreate|CFile::modeWrite),&er)
		if (!b)
		{ TCHAR szError[1024];
          er.GetErrorMessage(szError, 1024);
		  str.Format(_T("ERROR FILE №%s"),szError);
		  TRACE(str);
		  MessageBox(_T("Нет свободного места для сохранения!"),NULL,MB_OK|MB_ICONERROR);
	      return FALSE;
		}
	    else
		{//4) Сохранение параметров теста
/*		     m_nelement << m_PRIM <<
			 m_iCircuitKol << m_iCircuitBaza <<	m_iCircuitDop <<
			 m_irazv  << m_idlitimp << m_iKolU << m_iKolI << m_iKolSourceU <<	
	         m_iLimitKolU << m_iLimitKolI << 
			 m_bCheckKolRejHigh << m_itipimp << 	m_bCheckPauza <<
			 m_iBazaSource << m_bBazaVkl << m_iAmplitudeStepBaza << m_uStepBaza << m_iPolarityStepBaza << 
			 m_iQue1 << m_iBiasBaza << 	m_iPolarityBiasBaza << 
			 m_iBazaMeas << m_iRangLimitBaza << m_iLimitBaza << 
			 m_bCheckBazaRejHigh << 
			 m_iDopSource << m_bDopVkl <<m_iAmplitudeStepDop << m_uStepDop << m_iPolarityStepDop << 
			 m_iFamilyVkl << m_iBiasDop << m_iPolarityBiasDop << 
			 m_iDopMeas << m_iRangLimitDop << m_iLimitDop << 
			 m_sAxisY << m_sAxisX << m_iRejX << m_iRejY << m_sPar << m_sArg << m_uTp << 
		 	 m_sUr1 << m_sUr2 << m_iForm << m_sMin << m_sMax;// << 
*/
	    TRACE (L"Сохранение параметров теста\n");
		BeginWaitCursor();
		//коллектор
	    str.Format(_T("m_nelement=%d\r\n"),m_nelement);//m_nelement 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_PRIM=%s\r\n"),m_PRIM); // m_PRIM;// <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iCircuitKol=%d\r\n"),m_iCircuitKol);//m_iCircuitKol 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iCircuitBaza=%d\r\n"),m_iCircuitBaza);// m_iCircuitBaza 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iCircuitDop=%d\r\n"),m_iCircuitDop);//m_iCircuitDop 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
			     	
		str.Format(_T("m_iKommutator=%d\r\n"),m_iKommutator);//m_iKommutator
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
			     	
		str.Format(_T("m_iGnezdoC=%d\r\n"),m_iGnezdoC);//m_iGnezdoC
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
			     	
		str.Format(_T("m_iGnezdoB=%d\r\n"),m_iGnezdoB);//m_iGnezdoB
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
			     	
		str.Format(_T("m_iGnezdoE=%d\r\n"),m_iGnezdoE);//m_iGnezdoE
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
			     	
	    str.Format(_T("m_irazv=%d\r\n"),m_irazv);//m_irazv 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_idlitimp=%d\r\n"),m_idlitimp);// m_idlitimp
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iKolU=%d\r\n"),m_iKolU);// m_iKolU
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iKolI=%d\r\n"),m_iKolI);// m_iKolI
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iKolSourceU=%d\r\n"),m_iKolSourceU);// m_iKolSourceU <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iLimitKolU=%d\r\n"),m_iLimitKolU);// m_iLimitKolU 
		l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iLimitKolI=%d\r\n"),m_iLimitKolI);// m_iLimitKolI << 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_itipimp=%d\r\n"),m_itipimp);// m_itipimp 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
		//база
		str.Format(_T("m_iBazaSource=%d\r\n"),m_iBazaSource);// m_iBazaSource
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iAmplitudeStepBaza=%d\r\n"),m_iAmplitudeStepBaza);// m_iAmplitudeStepBaza
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_uStepBaza=%d\r\n"),(int)m_uStepBaza);// m_uStepBaza
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iPolarityStepBaza=%d\r\n"),m_iPolarityStepBaza);// m_iPolarityStepBaza //
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iQue1=%d\r\n"),m_iQue1);// m_iQue1//m_iRangBiasBaza 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iBiasBaza=%d\r\n"),m_iBiasBaza);// m_iBiasBaza
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iPolarityBiasBaza=%d\r\n"),m_iPolarityBiasBaza);//	m_iPolarityBiasBaza << 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iBazaMeas=%d\r\n"),m_iBazaMeas);// m_iBazaMeas 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iRangLimitBaza=%d\r\n"),m_iRangLimitBaza);// m_iRangLimitBaza
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iLimitBaza=%d\r\n"),m_iLimitBaza);// m_iLimitBaza <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
		//доп.канал
		str.Format(_T("m_iDopSource=%d\r\n"),m_iDopSource);// m_iDopSource
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iAmplitudeStepDop=%d\r\n"),m_iAmplitudeStepDop);//m_iAmplitudeStepDop 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_uStepDop=%d\r\n"),(int)m_uStepDop);// m_uStepDop 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iPolarityStepDop=%d\r\n"),m_iPolarityStepDop);// m_iPolarityStepDop <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iFamilyVkl=%d\r\n"),m_iFamilyVkl);// m_iFamilyVkl//m_iRangBiasDop
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iBiasDop=%d\r\n"),m_iBiasDop);// m_iBiasDop
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iPolarityBiasDop=%d\r\n"),m_iPolarityBiasDop); // m_iPolarityBiasDop <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iDopMeas=%d\r\n"),m_iDopMeas);//	 m_iDopMeas 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iRangLimitDop=%d\r\n"),m_iRangLimitDop);// m_iRangLimitDop 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iLimitDop=%d\r\n"),m_iLimitDop);// m_iLimitDop <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
		//расчёт
		str.Format(_T("m_sAxisY=%s\r\n"),m_sAxisY);// m_sAxisY
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_sAxisX=%s\r\n"),m_sAxisX);// m_sAxisX
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_sPar=%s\r\n"),m_sPar);//m_sPar
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_sArg=%s\r\n"),m_sArg);// m_sArg
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
		    
	    str.Format(_T("m_uTp=%d\r\n"),(int)m_uTp);// m_uTp <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
		   
	    str.Format(_T("m_sUr1=%s\r\n"),m_sUr1); //	 m_sUr1
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_sUr2=%s\r\n"),m_sUr2);// m_sUr2 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_iForm=%d\r\n"),m_iForm);// m_iForm
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_sMin=%s\r\n"),m_sMin);// m_sMin
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_sMax=%s\r\n"),m_sMax); // m_sMax;// <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_bCheckKolRejHigh=%d\r\n"),(int)m_bCheckKolRejHigh);// m_bCheckKolRejHigh 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_bCheckPauza=%d\r\n"),(int)m_bCheckPauza);//	m_bCheckPauza <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_bBazaVkl=%d\r\n"),m_bBazaVkl);// m_bBazaVkl 
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

		str.Format(_T("m_bCheckBazaRejHigh=%d\r\n"),(int)m_bCheckBazaRejHigh);// m_bCheckBazaRejHigh <<
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);

/*		str.Format(_T("m_bDopVkl=%d\r\n"),m_bDopVkl); // m_bDopVkl
	    l=str.GetLength();
        file.Write((LPCTSTR)str,l*2);
*/
        file.Close();
		SetModifiedMemory();
		EndWaitCursor();
		return TRUE;
	   }//end of 4)
	   }//архивирование
	   else return false;
	  }//end of 3)
	  else return FALSE;
	  }//%%%
	  else return false;
	}//end of 1)
	else return FALSE;
}

void CLineDlg::OnSaveTstStandart() 
{
	// TODO: Add your command handler code here
	CParolDlg dlgp;
	dlgp.what_save = FILE_TST;
	UpdateData(false);
	if (dlgp.DoModal()==IDOK)
	{   UpdateData();
		if (m_sparol == dlgp.m_sparol) 
		{ //пароль верный
			int where = TST_STANDART;
			BOOL b = SaveParamToTst(where);
			if (b) MessageBox(_T("Сохранение завершено успешно!"),NULL,MB_OK|MB_ICONINFORMATION);	
			else   MessageBox(_T("Тест не сохранён!"),NULL,MB_OK|MB_ICONERROR);
		}
		else MessageBox(_T("Неверный пароль! Тест не сохранён!"),NULL,MB_OK|MB_ICONERROR);
	}
	else MessageBox(_T("Пароль не введен! Тест не сохранён!"),NULL,MB_OK|MB_ICONEXCLAMATION);
}

void CLineDlg::OnAddTreefrompanel() 
{
	// TODO: Add your command handler code here

	CTreeAddFromPanelDlg dlg;
	POSITION position=*m_position;

    if (position)
	{ CMod* pMod = m_pList->GetAt(position);
      dlg.m_EL    = pMod->m_EL;
	  dlg.m_MOD   = m_MOD;
	  dlg.m_PRIM  = m_PRIM;//pMod->m_PRIM;
	}
    else 
	{ dlg.m_EL    = _T("");
	  dlg.m_MOD   = m_MOD;
	  dlg.m_PRIM  = m_PRIM;
    }
	UpdateData(FALSE);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
	  m_EL   = dlg.m_EL;
	  m_MOD  = dlg.m_MOD;
	  m_PRIM = dlg.m_PRIM;
	  if (FindTest(m_EL,m_MOD))
	     MessageBox(_T("В списке уже существует тест ")+m_MOD+_T(" для элемента ")+m_EL,NULL,MB_OK|MB_ICONEXCLAMATION);
	  else
	  {//добавление в модель теста с параметрами с панели
		CMod* pMod = new CMod;
		pMod->m_EL    = m_EL;
		pMod->m_MOD   = m_MOD;
		pMod->m_PRIM  = m_PRIM;
		//коллектор
	    pMod->m_nelement		 = m_nelement;
		pMod->m_iCircuitKol		 = m_iCircuitKol;
		pMod->m_iCircuitBaza	 = m_iCircuitBaza;
		pMod->m_iCircuitDop		 = m_iCircuitDop;
	    pMod->m_iKommutator	= m_iKommutator;
	    pMod->m_iGnezdoC	= m_iGnezdoC;
	    pMod->m_iGnezdoB	= m_iGnezdoB;
	    pMod->m_iGnezdoE	= m_iGnezdoE;
		pMod->m_irazv			 = m_irazv;
		pMod->m_idlitimp		 = m_idlitimp;
		pMod->m_iKolU			 = m_iKolU;
		pMod->m_iKolI			 = m_iKolI;
		pMod->m_iKolSourceU		 = m_iKolSourceU;
		pMod->m_iLimitKolU		 = m_iLimitKolU;
		pMod->m_iLimitKolI		 = m_iLimitKolI;
		pMod->m_bCheckKolRejHigh = m_bCheckKolRejHigh;
		pMod->m_itipimp			 = m_itipimp;
		pMod->m_bCheckPauza		 = m_bCheckPauza;
		//база
		pMod->m_iBazaSource		   = m_iBazaSource;
		pMod->m_bBazaVkl		   = m_bBazaVkl;
		pMod->m_iAmplitudeStepBaza = m_iAmplitudeStepBaza;
		pMod->m_uStepBaza		   = m_uStepBaza;
		pMod->m_iPolarityStepBaza  = m_iPolarityStepBaza;
		pMod->m_iQue1      = m_iQue1;
		pMod->m_iBiasBaza          = m_iBiasBaza;		
		pMod->m_iPolarityBiasBaza  = m_iPolarityBiasBaza;
		pMod->m_iBazaMeas          = m_iBazaMeas;
		pMod->m_iRangLimitBaza     = m_iRangLimitBaza;
		pMod->m_iLimitBaza         = m_iLimitBaza;	
		pMod->m_bCheckBazaRejHigh  = m_bCheckBazaRejHigh;
		//доп.канал
		pMod->m_iDopSource         = m_iDopSource;
//		pMod->m_bDopVkl			   = m_bDopVkl;
		pMod->m_iAmplitudeStepDop  = m_iAmplitudeStepDop;
		pMod->m_uStepDop           = m_uStepDop;
		pMod->m_iPolarityStepDop   = m_iPolarityStepDop;
		pMod->m_iFamilyVkl       = m_iFamilyVkl;
		pMod->m_iBiasDop           = m_iBiasDop;			
		pMod->m_iPolarityBiasDop   = m_iPolarityBiasDop;
		pMod->m_iDopMeas           = m_iDopMeas;
		pMod->m_iRangLimitDop      = m_iRangLimitDop;
		pMod->m_iLimitDop          = m_iLimitDop;	
		//расчёт
		pMod->m_sAxisY = m_sAxisY;
		pMod->m_sAxisX = m_sAxisX;
		pMod->m_sPar   = m_sPar;
		pMod->m_sArg   = m_sArg;
		pMod->m_uTp    = m_uTp;
		pMod->m_sUr1   = m_sUr1;
		pMod->m_sUr2   = m_sUr2;
		pMod->m_iForm  = m_iForm;
		pMod->m_sMin   = m_sMin;
		pMod->m_sMax   = m_sMax;
	
		SetModifiedFlag();
        *m_position = m_pList->InsertAfter(*m_position, pMod);
    
        POSITION pos = *m_position;
	    FConfig();
	    //Expand
	    HTREEITEM hitem = NULL;
	    CString str=pMod->m_EL;
	    for(int i=0;i<NE;i++)
	    if (str==m_masStrEl[i])
		{ hitem = m_masHEl[i]; 
	      break;
		}
	    if (hitem!=NULL)
		{
//		  m_pTreeDlg->m_TreeCtrl.Expand(hitem,TVE_EXPAND);
//	      m_pTreeDlg->m_TreeCtrl.EnsureVisible(hitem);
		  m_TreeCtrl.Expand(hitem,TVE_EXPAND);
	      m_TreeCtrl.EnsureVisible(hitem);
		}
	    *m_position = pos;

//это не надо !!! 29.11.17  14.30 =>
//вставила 29.11.17  =>14.00
//		delete pMod;	
//		pMod = NULL;
//<=
	  }//end of add
	}//end if IDOK
}

void CLineDlg::OnAddTreefrommemory() 
{
	// TODO: Add your command handler code here
	BuildArrayTstStandart();
	pDlgTree=new CTreeAddDlg(this);
	DlgTreeUpdate();
	pDlgTree->DoModal();
	delete pDlgTree;
	
}

void CLineDlg::OnSelchangeCombocheckkol() 
{
	// TODO: Add your control notification handler code here
 	UpdateData();
   if (n_IPPP==1 && (n_Switch==1 || n_Switch==2) && m_iCircuitKol==0)
   {//запретить обрыв на +- 5V
	 m_iCircuitKol=1;
	 UpdateData(false);
   }
	BuildCircuit();
	InitStatusKol();
	InitDiapazonKol();
//	SetDiapazonKol();
	ChangeDiapazonKol();
	SetStringsSourceMeas();
	ShowRejimChanel();
	UpdateData(false);
}

void CLineDlg::OnSelchangeCombocheckbaza() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	BuildCircuit();
	InitStatusBaza();
	InitDiapazonBaza();
	SetDiapazonBaza();
	SetStringsSourceMeas();
	ShowRejimChanel();
	UpdateData(false);
}

void CLineDlg::OnSelchangeCombocheckdop() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	BuildCircuit();
	InitStatusDop();
	InitDiapazonDop();
	SetDiapazonDop();
	SetStringsSourceMeas();
	UpdateData(false);
}


void CLineDlg::OnSelchangeCombobazalimitmaxui() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	//диапазон ограничения
	CComboBox *cbptrlimit = (CComboBox*) GetDlgItem(IDC_COMBOBAZALIMITMAXUI);
    m_iRangLimitBaza = cbptrlimit -> GetCurSel();
	//слайдер ограничения
	if (m_iRangLimitBaza>=0) m_iLimitBaza = 100;
//	else m_iLimitBaza = 0;
	else m_iLimitBaza = 10;
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERLIMITBAZA);
	pSl -> SetPos(m_iLimitBaza);
	//ограничение
	switch(m_iBazaSource)
	{ case 0: //U
			  m_dLimitBaza = mas_d_range_meas_bazadop_I[m_iRangLimitBaza]+mas_d_range_meas_bazadop_I[m_iRangLimitBaza]/10;
	          break;
	  case 1: //I
			  m_dLimitBaza = mas_d_range_meas_bazadop_U[m_iRangLimitBaza]+mas_d_range_meas_bazadop_U[m_iRangLimitBaza]/10;
		      break;
	}
	valueLimitBaza=(m_dLimitBaza/110)*m_iLimitBaza;
//	m_sLimitBaza.Format(_T("%.2e"),valueLimitBaza);
//  m_sLimitBaza+=str_razmer_LimitBaza;
	ValueToString(!m_iBazaSource,valueLimitBaza,m_sLimitBaza);
	UpdateData(FALSE);
	ControlParam();
	if (iY==BAZA) { 
	  InitAxis();
//					InvalidateRect(&m_RectStr_Y,FALSE);
//					InvalidateRect(&m_RectRazmer_Y,FALSE);
					}
	
}

void CLineDlg::OnSelchangeCombodoplimitmaxui() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	//диапазон ограничения
	CComboBox *cbptrlimit = (CComboBox*) GetDlgItem(IDC_COMBODOPLIMITMAXUI);
    m_iRangLimitDop = cbptrlimit -> GetCurSel();
	//слайдер ограничения
	if (m_iRangLimitDop>=0) m_iLimitDop = 100;
//	else m_iLimitDop = 0;
	else m_iLimitDop = 10;
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERLIMITDOP);
	pSl -> SetPos(m_iLimitDop);
	//ограничение
	switch(m_iDopSource)
	{ case 0: //U
			    m_dLimitDop = mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I]+mas_d_range_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I]/10;
	          break;
	  case 1: //I
			    m_dLimitDop = mas_d_range_meas_bazadop_U[m_iRangLimitDop]+mas_d_range_meas_bazadop_U[m_iRangLimitDop]/10;
		      break;
	}
	valueLimitDop=(m_dLimitDop/110)*m_iLimitDop;
	ValueToString(!m_iDopSource,valueLimitDop,m_sLimitDop);
	UpdateData(FALSE);
	ControlParam();
	if (iY==DOP)  { 
	  InitAxis();
//		InvalidateRect(&m_RectStr_Y,FALSE);
//					InvalidateRect(&m_RectRazmer_Y,FALSE);
	}
	
}
void CLineDlg::OnSelchangeComboamplitudebaza() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	//амплитуда ступеньки
	CComboBox *cbptrampl  = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEBAZA);
    m_iAmplitudeStepBaza = cbptrampl  -> GetCurSel();
	//число ступенек (база)
	m_uStepBaza = 0;
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINSTEPBAZA);
	pSpin -> SetPos(m_uStepBaza);
	//смешение
	m_iBiasBaza = 0;
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERBAZABIAS);
	pSl -> SetPos(m_iBiasBaza);

	switch(m_iBazaSource)
	{ case 0: //U
			  m_sRangBiasBaza = mas_range_bias_bazadop_U[m_iAmplitudeStepBaza];
			  m_dBiasBaza = mas_d_range_bias_bazadop_U[m_iAmplitudeStepBaza];
	          break;
	  case 1: //I
		      m_sRangBiasBaza = mas_range_bias_bazadop_I[m_iAmplitudeStepBaza];
			  m_dBiasBaza = mas_d_range_bias_bazadop_I[m_iAmplitudeStepBaza];
		      break;
	}


	valueBiasBaza=(m_dBiasBaza/1000.0)*m_iBiasBaza;
	ValueToString(m_iBazaSource,valueBiasBaza,m_sBiasBaza);
	UpdateData(FALSE);
	b_InitAxis=true;
	ControlParam();
	
}

void CLineDlg::OnSelchangeComboamplitudedop() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	//амплитуда ступеньки
	CComboBox *cbptrampl  = (CComboBox*) GetDlgItem(IDC_COMBOAMPLITUDEDOP);
    m_iAmplitudeStepDop = cbptrampl  -> GetCurSel();
	//число ступенек 
	m_uStepDop = 0;
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINSTEPDOP);
	pSpin -> SetPos(m_uStepDop);
	//смешение
	m_iBiasDop = 0;
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERDOPBIAS);
	pSl -> SetPos(m_iBiasDop);

	switch(m_iDopSource)
	{ case 0: //U
			  m_sRangBiasDop = mas_range_bias_bazadop_U[m_iAmplitudeStepDop];
			  m_dBiasDop = mas_d_range_bias_bazadop_U[m_iAmplitudeStepDop];
	          break;
	  case 1: //I
			  m_sRangBiasDop = mas_range_bias_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
			  m_dBiasDop = mas_d_range_bias_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
		      break;
	}


	valueBiasDop=(m_dBiasDop/1000.0)*m_iBiasDop;
	ValueToString(m_iDopSource,valueBiasDop,m_sBiasDop);
	UpdateData(FALSE);
	b_InitAxis=true;
	ControlParam();
//	if (iX==DOP)  { InvalidateRect(&m_RectStr_X,FALSE);
//					InvalidateRect(&m_RectRazmer_X,FALSE);
//					}
				
}

void CLineDlg::OnKillfocusAxisy() 
{
	// TODO: Add your control notification handler code here
//	CString temp=m_sAxisY;
	UpdateData();
//	if (temp!=m_sAxisY)
	b_InitAxis=true;
	{ int er=ControlParam();//Function();
		if (!er) { 
//			InitAxis();
//			InvalidateRect(&m_RectStr_Y,FALSE);
//				   InvalidateRect(&m_RectRazmer_Y,FALSE);
		}
	}
	UpdateData(FALSE);
}

void CLineDlg::OnKillfocusAxisx() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	b_InitAxis=true;
	int er = ControlParam();//Argument();
	if (!er) { 
//				InitAxis();
//				InvalidateRect(&m_RectStr_X,FALSE);
//			   InvalidateRect(&m_RectRazmer_X,FALSE);
			}
	UpdateData(FALSE);
}

void CLineDlg::BuildCommand() 
{
	CString str = _T("");
	double x = 0.0;
	int p = 0;
	int rez = 0;

	UpdateData();

 if (m_uCountTest==0)
 {
   //K35: режим измерения 
    K35[1] = n_Test;		// 1 - "ручной", >1 - "автоматический"
   //K34: вид запуска 
    K34[1] = bZapusk;		// вид запуска: 0 - "внутренний" (по кнопке "Пуск"), 1 - "внешний" (по внешнему сигналу)
    K34[2] = bMeas;			// 0 - "однократный", 1 - "многократный"
 }
 // K1: схема включения
	K1[1] = m_iCircuitKol;
//	if (m_iCircuitBaza==-1) K1[2] = 0;
	if (m_iCircuitBaza==-1 || m_iKnotBaza==0) K1[2] = 0;	//11.12.17
					   else K1[2] = m_iCircuitBaza;
//	if (m_iCircuitDop==-1 )  K1[3] = 0;
	if (m_iCircuitDop==-1 || m_iKnotDop==0)  K1[3] = 0;	//11.12.17
					   else K1[3] = m_iCircuitDop;
// if (m_iCircuitKol==1)
// {
 // K2
	K2[1] = m_irazv;
/*// вид развёртки
//	if (m_itipimp==TIP_IMP) K2[2] = TIP_IMP;	// IMP
//					   else K2[2] = TIP_SYN;	// не IMP
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) K2[2] = TIP_IMP;	// IMP
					                     else K2[2] = TIP_SYN;	// не IMP
*/
//	K2[2] = 0;//(1-ая очередь - канал С (пока))
	K2[2] = m_iQue1;//(1-ая очередь )

   // K3
/*	24.07.2013
	if (m_itipimp==TIP_IMP) 
		{ m_idlitimp = 5 - (m_iKolSourceU+bias_KolSourceU);
//		  if (m_bCheckPauza) m_udlitpauza = 7;	//160 ms
//				        else m_udlitpauza = 3;	//80 ms
		}
	else{ m_idlitimp = 0;}		//10ms					// m_udlitpauza = 3;

    if (m_idlitimp==-1) m_idlitimp=0;//на всякий случай
*/
	if (m_idlitimp==6) K3[1] = 0;//31.03.15 (SYN 10ms на +-5V)
    else   K3[1] = m_idlitimp;
	
	if (m_iQue1==BAZA)  K3[2] = 1;
	else
	{
//	if (bZapusk || K35[1]>1) { //06.07.15
	if (K35[1]>1) { 
		n_Step = 1; m_uTp = 1; bGenerationStup = false;
								}//В автомате генерация ступенек выкл.
	
	if (bGenerationStup) K3[2] = 1;
					else K3[2] = 0;// m_irejimp;
	}

	//	K3[2] = m_udlitpauza;
 // K4
//	if (m_iCircuitKol==0) K4[1] = 4; //коллектор в обрыве => диапазон источника (U) коллектора = 2000V
//	else 
		K4[1] = bias_KolSourceU + m_iKolSourceU;		// диапазон источника (U) коллектора
 // K5
	K5[1] = bias_KolMeasU + m_iKolU;	// диапазон измерителя U коллектора	
//	K5[1] = m_iKolU;	// диапазон измерителя U коллектора	
 // K6
//	K6[1] = bias_KolMeasI/2 +m_iKolI;	// диапазон измерителя I коллектора
	K6[1] = bias_KolMeasI  + m_iKolI;	// диапазон измерителя I коллектора
 // K7
/*	if (m_itipimp==TIP_IMP) K7[1] = (BYTE)(m_iLimitKolU/5);//№ точки (от 1 до 20), соответствующей ограничению Uc в % (Стоп)
					else 	K7[1] = m_iLimitKolU;	//ограничение Uc в % (Стоп)
*/
    K7[1] = m_iLimitKolU;	//ограничение Uc в % (Стоп)
// K8
/*	// m_iLimitKolI - ограничение измерителя коллектора (I)	в %
	//из расчёта: 100% - код 220, m_iLimitKolI % - x имеем:
	x = (m_iLimitKolI * 220.0)/100.0;
	rez = (int)x;
//	str.Format(_T("%.0f"),x);
//	rez=atoi((const char*)str.GetBuffer(str.GetLength()+1));		  
	p = rez & 0xff;
	K8[1] = p;
*/
/*	if (m_itipimp==TIP_IMP) K8[1] = (BYTE)(m_iLimitKolI/2);//   /5);//№ точки (от 1 до 20), соответствующей ограничению Uc в % (Старт)
					else 	K8[1] = m_iLimitKolI;	//ограничение Uc в % (Старт)
*/
	K8[1] = m_iLimitKolI;	//ограничение Uc в % (Старт)
// }
// if (m_iCircuitBaza==1)
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	{	PolaritySingBaza();
 // K9
	K9[1] = m_iBazaSource;
 // K39
//	if (m_bBazaVkl) K39[1]=1;
//	else K39[1] = 0;
 // K10
	K10[1] = m_iAmplitudeStepBaza;
 // K11
	if (m_iQue1==BAZA) K11[1] = m_uStepBaza*10;
	else 
		K11[1] = m_uStepBaza;	
 // K12
//	if (m_uStepBaza) valueAmplBaza = 1.0;//mas_d_range_ampl_bazadop_U[m_iAmplitudeStepBaza];
//				else  valueAmplBaza = 0.0;
	 switch(m_iBazaSource)
	 { case 0: //источник базы - U
				valueAmplBaza = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepBaza];
				break;
	   case 1: //источник базы - I
				valueAmplBaza = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepBaza];
				break;
	 }
	if (m_bCheckBazaRejHigh) valueAmplBaza = valueAmplBaza * (0.1); //амплитуду ступеньки базы уменьшить в 10 раз
	if (sign_ampl_baza==0) valueAmplBaza = valueAmplBaza * (-1.0);	//знак амплитуды  (инверсия)
	switch (m_iBazaSource)
	{ case 0:	// U
				x = valueAmplBaza*KSBU[m_iAmplitudeStepBaza]/ koeff_bazadop_U[m_iAmplitudeStepBaza];;//-BSABU[m_iAmplitudeStepBaza];
//				if (sign_ampl_baza==0) valueAmplBaza = valueAmplBaza * (-1.0);	//знак амплитуды  (инверсия)
				break;
	  case 1:	// I
				x = valueAmplBaza*KSBI[m_iAmplitudeStepBaza]/ koeff_bazadop_I[m_iAmplitudeStepBaza];;//-BSABI[m_iAmplitudeStepBaza];
				break;
	}
	rez = (int)x;
//	str.Format(_T("%.0f"),x);
//	rez=atoi((const char*)str.GetBuffer(str.GetLength()+1));		  
	p = rez & 0xff;
	K12[1] = p;
	p = rez & 0xff00;
	p = p >> 8;
	K12[2] = p;

 // K14	
	// valueBiasBaza - смещение генератора ступенек базы
	valueBiasBaza=(m_dBiasBaza/1000.0)*m_iBiasBaza;
	if (sign_bias_baza==0) valueBiasBaza = valueBiasBaza * (-1.0);	//знак смещения  (инверсия)
	switch (m_iBazaSource)
	{ case 0:	// U
				x = (valueBiasBaza *KSBU[m_iAmplitudeStepBaza]+BSBU[m_iAmplitudeStepBaza])/ koeff_bazadop_U[m_iAmplitudeStepBaza];//
//		    	if (sign_bias_baza==0) valueBiasBaza = valueBiasBaza * (-1.0);	//знак смещения  (инверсия)
				break;
	  case 1:	// I
				x = (valueBiasBaza *KSBI[m_iAmplitudeStepBaza]+BSBI[m_iAmplitudeStepBaza])/ koeff_bazadop_I[m_iAmplitudeStepBaza];
				break;
	}
	rez = (int)x;
//	str.Format(_T("%.0f"),x);
//	rez=atoi((const char*)str.GetBuffer(str.GetLength()+1));		  
	p = rez & 0xff;
	K14[1] = p;
	p = rez & 0xff00;
	p = p >> 8;
	K14[2] = p;
 
//	if (m_bBaza) m_iBazaMeas = m_iBazaSource;
//			else m_iBazaMeas = ! m_iBazaSource;
	if (m_bBaza) m_iBazaMeas = !m_iBazaSource;
			else m_iBazaMeas = m_iBazaSource;
	// K15
	K15[1] = m_iBazaMeas;//режим измерителя
//	if (m_bBaza)  m_iRangMeasBaza = m_iAmplitudeStepBaza;
//		    else  m_iRangMeasBaza = m_iRangLimitBaza;
	if (!m_bBaza)  m_iRangMeasBaza = m_iAmplitudeStepBaza;
		    else  m_iRangMeasBaza = m_iRangLimitBaza;
 // K16 - диапазон измерителя
	K16[1] =  m_iRangMeasBaza;
 // K17 - диапазон ограничения
	K17[1] = m_iRangLimitBaza;
 // K18
	//valueLimitBaza - ограничение измерителя базы
	switch(m_iBazaSource)
	{ case 0:	//ист. U => огр. I
				x = (valueLimitBaza *KLBI[m_iRangLimitBaza]+BLBI[m_iRangLimitBaza])/ koeff_limit_bazadop_I[m_iRangLimitBaza];
				break;
	  case 1:	//ист. I => огр. U
				x = (valueLimitBaza*KLBU[m_iRangLimitBaza]+BLBU[m_iRangLimitBaza]) / koeff_limit_bazadop_U[m_iRangLimitBaza];
				break;
	}
	rez = (int)x;
//	str.Format(_T("%.0f"),x);
//	rez=atoi((const char*)str.GetBuffer(str.GetLength()+1));		  
	p = rez & 0xff;
	K18[1] = p;
	p = rez & 0xff00;
	p = p >> 8;
	K18[2] = p;
 }
 if (m_iCircuitDop ==1)
 {	PolaritySingDop();
 // K19
	K19[1] = m_iDopSource;
 // K40
//	K40[1] = m_bDopVkl;
 // K20
	if (m_iDopSource==1) K20[1] = m_iAmplitudeStepDop+bias_Dop_I;
	               else  K20[1] = m_iAmplitudeStepDop;
 // K21
	K21[1] = m_uStepDop;
 // K22
//	if (m_uStepDop) valueAmplDop = 1.0;// mas_d_range_ampl_bazadop_U[m_iAmplitudeStepDop];
//			  else  valueAmplDop = 0.0;
	 switch(m_iDopSource)
	 { case 0: //источник доп.канала - U
				valueAmplDop = mas_d_range_ampl_bazadop_U[m_iAmplitudeStepDop];
				break;
	   case 1: //источник доп.канала - I
				valueAmplDop = mas_d_range_ampl_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
				break;
	 }
	if (sign_ampl_dop==0) valueAmplDop = valueAmplDop * (-1.0);	//знак амплитуды (инверсия)
//	valueAmplDop = valueAmplDop * AMPLITUDA;
 	switch (m_iDopSource)
	{ case 0:	// U
				x = valueAmplDop * KSDU[m_iAmplitudeStepDop]/ koeff_bazadop_U[m_iAmplitudeStepDop];//-BSADU[m_iAmplitudeStepDop];
//				if (sign_ampl_dop==0) valueAmplDop = valueAmplDop * (-1.0);	//знак амплитуды (инверсия)
				break;
	  case 1:	// I
				x = valueAmplDop *KSDI[m_iAmplitudeStepDop] / koeff_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];//-BSADI[m_iAmplitudeStepDop];
				break;
	}
	rez = (int)x;
//	str.Format(_T("%.0f"),x);
//	rez=atoi((const char*)str.GetBuffer(str.GetLength()+1));		  
	p = rez & 0xff;
	K22[1] = p;
	p = rez & 0xff00;
	p = p >> 8;
	K22[2] = p;
// K24	
	// valueBiasDop - смещение генератора ступенек доп.канала
	valueBiasDop=(m_dBiasDop/1000.0)*m_iBiasDop;
	if (sign_bias_dop==0) valueBiasDop = valueBiasDop * (-1.0);	//знак смещения  (инверсия)
	switch (m_iDopSource)
	{ case 0:	// U
				x = (valueBiasDop*KSDU[m_iAmplitudeStepDop]+BSDU[m_iAmplitudeStepDop]) / koeff_bazadop_U[m_iAmplitudeStepDop];
//				if (sign_bias_dop==0) valueBiasDop = valueBiasDop * (-1.0);	//знак смещения  (инверсия)
				break;
	  case 1:	// I
				x = (valueBiasDop*KSDI[m_iAmplitudeStepDop]+BSDI[m_iAmplitudeStepDop]) / koeff_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I];
				break;
	}
	rez = (int)x;
//	str.Format(_T("%.0f"),x);
//	rez=atoi((const char*)str.GetBuffer(str.GetLength()+1));		  
	p = rez & 0xff;
	K24[1] = p;
	p = rez & 0xff00;
	p = p >> 8;
	K24[2] = p;
 // K25
//	if (m_bDop)	 m_iDopMeas = m_iDopSource;
//	   else 	 m_iDopMeas = !m_iDopSource;
	if (m_bDop)	 m_iDopMeas = !m_iDopSource;
	   else 	 m_iDopMeas = m_iDopSource;
	K25[1] = m_iDopMeas;//режим измерителя
 // K26 - диапазон измерителя,
 //	K27 - диапазон ограничения
//	if (m_bDop) 
	if (!m_bDop) 
	{ //меряем то что генерируем => диапазон измерителя = диапазону источника
		if (m_iDopSource==0)
		{ //ист. U , изм. U, огр. по I
		  K26[1] = m_iAmplitudeStepDop;			//изм. по ист. U
	      K27[1] = m_iRangLimitDop + bias_Dop_I;//огр. по I
		}
		else
		{ //ист. I , изм. I , огр. по U
		  K26[1] = m_iAmplitudeStepDop + bias_Dop_I;//изм. по ист. I
	      K27[1] = m_iRangLimitDop;					//огр. по U
		}
	}
	else
	{ //диапазон измерителя = диапазону ограничения
		if (m_iDopSource==0)
		{ //ист. U , изм. I, огр. по I
		  K26[1] = m_iRangLimitDop + bias_Dop_I;	//изм. по огр. I
	      K27[1] = m_iRangLimitDop + bias_Dop_I;	//огр. по I
		}
		else
		{ //ист. I , изм. U, огр. по U
		  K26[1] = m_iRangLimitDop;					//изм. по огр. U
	      K27[1] = m_iRangLimitDop;					//огр. по U
		}
	}
 // K28
	//valueLimitDop - ограничение измерителя доп.канала
	switch(m_iDopSource)
	{ case 0:	//ист. U => огр. I
				x = (valueLimitDop*KLDI[m_iRangLimitDop]+BLDI[m_iRangLimitDop]) / koeff_limit_bazadop_I[m_iRangLimitDop+bias_Dop_I];
				break;
	  case 1:	//ист. I => огр. U
				x = (valueLimitDop*KLDU[m_iRangLimitDop]+BLDU[m_iRangLimitDop]) / koeff_limit_bazadop_U[m_iRangLimitDop];
				break;
	}
	rez = (int)x;
//	str.Format(_T("%.0f"),x);
//	rez=atoi((const char*)str.GetBuffer(str.GetLength()+1));		  
	p = rez & 0xff;
	K28[1] = p;
	p = rez & 0xff00;
	p = p >> 8;
	K28[2] = p;
 }
 // K30// "Число точек в DC и импульсном режиме"
//	if ((m_itipimp==TIP_IMP) || (m_irazv==DC_POS || m_irazv==DC_NEG))
	if ( m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG)
	K30[1] = uNpointsLupa;
	if (m_iQue1==BAZA) K30[1]=1;

	if (m_bBeta || m_bBetai || m_bS) K30[1] = 1;	//одно значение на каждой ступеньке при Uc = 5V

 // K32
	K32[1] = m_bCheckBazaRejHigh;
 // K37 "Состояние бита Q18"

// K36
	if (m_idlitimp==-1) m_idlitimp=0;
//	Nz_max = mas_Nz_max[m_irejimp][m_idlitimp];
	Nz_max = mas_Nz_max[m_idlitimp];
	K36[1] = Nz_max;

}

void CLineDlg::BuildTest()
{
	int i=0;
	int j=0;
	int k=0;		 //счетчик числа передаваемых байт теста
  ::WaitForSingleObject(hMutexO,INFINITE);
    NumO = k;	//кол-во байт для передачи
  ::ReleaseMutex(hMutexO);

	for(i=0;i<NBUF;i++)
	My_buf_Out[i]=0;
/*//Химия
if (m_MOD==_T("Ic_10n_Calibr"))
{
	memcpy(&My_buf_Out[k],&K31[0],nK[30]);
	k = k + nK[30];
}
*/
//0.команда K35, K34
 if (m_uCountTest==0)
 {
   //K35: режим измерения  K35[1] = n_Test (1 - "ручной", >1 - "автоматический")
	memcpy(&My_buf_Out[k],&K35[0],nK[34]);
	k = k + nK[34];
//   K34: вид запуска 	K34[1] = bZapusk;// (0 - "внутренний" (по кнопке "Пуск"), 1 - "внешний" (по внешнему сигналу))
//    K34[2] = bMeas;(0 - "однократный", 1 - "многократный")
	memcpy(&My_buf_Out[k],&K34[0],nK[33]);
	k = k + nK[33];
 }
//1.команда К1 "Схема включения"
	memcpy(&My_buf_Out[k],&K1[0],nK[0]);
	k = k + nK[0];
// if (m_iCircuitKol==1)
 {
//2.команда K2 "Вид развёртки" (K2[1] = m_irazv)
	memcpy(&My_buf_Out[k],&K2[0],nK[1]);
	k = k + nK[1];
//3.команда K3 "Импульс" (K3[1] = m_idlitimp)
//	if (m_irazv == IMP_POS || m_irazv == IMP_NEG)
	{//IMP
	   memcpy(&My_buf_Out[k],&K3[0],nK[2]);
	   k = k + nK[2];
	}
//4.команда K4 "Диапазон источника (U) коллектора" (K4[1] = m_iKolSourceU)
	memcpy(&My_buf_Out[k],&K4[0],nK[3]); 
	k = k + nK[3];
//5.команда K5 "Диапазон измерителя U коллектора"(K5[1] = bias_KolMeasU + m_iKolU;)
	memcpy(&My_buf_Out[k],&K5[0],nK[4]);
	k = k + nK[4];
//6.команда K6 "Диапазон измерителя I коллектора" (K6[1] = bias_KolMeasI +m_iKolI;)
	memcpy(&My_buf_Out[k],&K6[0],nK[5]);
	k = k + nK[5];
//7.команда K7 "Ограничение напряжения коллектора в %" (K7[1] = m_iLimitKolU;)
	memcpy(&My_buf_Out[k],&K7[0],nK[6]);
	k = k + nK[6];
//8.команда K8 "Порог ограничения измерителя коллектора (I)" 
	memcpy(&My_buf_Out[k],&K8[0],nK[7]);
	k = k + nK[7];
 }

// if (m_iCircuitBaza==1)
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
 {
    //9.команда K9 "Режим источника базы" (K9[1] = m_iBazaSource;)
	memcpy(&My_buf_Out[k],&K9[0],nK[8]);
	k = k + nK[8];
    //39.команда K39 "Вкл.генератора базы" (K39[1] = m_bBazaVkl;)
//	memcpy(&My_buf_Out[k],&K39[0],nK[38]);
//	k = k + nK[38];


    //11.команда K11 "Число ступеней базы" (K11[1] = m_uStepBaza;)
	memcpy(&My_buf_Out[k],&K11[0],nK[10]);
	k = k + nK[10];
    //32.команда K32 "Высокое разрещение по базе"( K32[1] = m_bCheckBazaRejHigh;)
	memcpy(&My_buf_Out[k],&K32[0],nK[31]);
	k = k + nK[31];
    //10.команда K10 "Диапазон амплитуды ступени базы" (K10[1] = m_iAmplitudeStepBaza;)
	memcpy(&My_buf_Out[k],&K10[0],nK[9]);
	k = k + nK[9];
    //12.команда K12 "Значение амплитуды ступени базы" 
	memcpy(&My_buf_Out[k],&K12[0],nK[11]);
	k = k + nK[11];

//    //13.команда K13 "Диапазон смещения базы" (	if U K13[1] = m_iRangBiasBaza; if I	K13[1] = m_iAmplitudeStepBaza;)
//	memcpy(&My_buf_Out[k],&K13[0],nK[12]);
//	k = k + nK[12];
    //14.команда K14 "Смещение Базы" 
	memcpy(&My_buf_Out[k],&K14[0],nK[13]);
	k = k + nK[13];

    //15.команда K15 "Режим измерителя базы" (K15[1] = m_iBazaMeas;)
	memcpy(&My_buf_Out[k],&K15[0],nK[14]);
	k = k + nK[14];
//   if (m_iBazaSource == m_iBazaMeas)
   { 
     //16.команда "Диапазон измерителя базы"
	 memcpy(&My_buf_Out[k],&K16[0],nK[15]);
	 k = k + nK[15];
   }
    //17.команда K17 "Диапазон ограничения базы" (K17[1] = m_iRangLimitBaza;)
	memcpy(&My_buf_Out[k],&K17[0],nK[16]);
	k = k + nK[16];
    //18.команда K18 "Порог ограничения измерителя базы" 
	memcpy(&My_buf_Out[k],&K18[0],nK[17]);
	k = k + nK[17];
 }
 if (m_iCircuitDop == 1)
 {
    //19.команда K19 "Режим источника доп.канала" (K19[1] = m_iDopSource;)
	memcpy(&My_buf_Out[k],&K19[0],nK[18]);
	k = k + nK[18];
/*    //40.команда K40 "Вкл.генератора подложки" (K40[1] = m_bDopVkl;)
	memcpy(&My_buf_Out[k],&K40[0],nK[39]);
	k = k + nK[39];
*/
    //21.команда K21 "Число ступеней доп.канала" (K21[1] = m_uStepDop;)
	memcpy(&My_buf_Out[k],&K21[0],nK[20]);
	k = k + nK[20];
//   if (m_uStepDop!=0)
   {
    //20.команда K20 "Диапазон амплитуды ступени доп.канала" (K20[1] = m_iAmplitudeStepDop;)
	memcpy(&My_buf_Out[k],&K20[0],nK[19]);
	k = k + nK[19];
    //22.команда K22 "Значение амплитуды ступени доп.канала" 
	memcpy(&My_buf_Out[k],&K22[0],nK[21]);
	k = k + nK[21];
   }
    //24.команда K24	"Смещение доп.канала" 
	memcpy(&My_buf_Out[k],&K24[0],nK[23]);
	k = k + nK[23];
    //25.команда K25 "Режим измерителя доп.канала" (K26[1] = m_iDopMeas;)
	memcpy(&My_buf_Out[k],&K25[0],nK[24]);
	k = k + nK[24];
//	if (m_iDopSource == m_iDopMeas)
	{ 
      //26.команда "Диапазон измерителя доп.канала"
	  memcpy(&My_buf_Out[k],&K26[0],nK[25]);
	  k = k + nK[25];
	}
    //27.команда K27 "Диапазон ограничения доп.канала" (K27[1] = m_iRangLimitDop;)
	memcpy(&My_buf_Out[k],&K27[0],nK[26]);
	k = k + nK[26];
    //28.команда K28 "Порог ограничения измерителя доп.канала" 
	memcpy(&My_buf_Out[k],&K28[0],nK[27]);
	k = k + nK[27];
 }
//30.команда K30 "Число измеряемых точек в реж. DC или в импульсном" (20 или 50)
	memcpy(&My_buf_Out[k],&K30[0],nK[29]);
	k = k + nK[29];

//36.команда K36 Команда "Максимальный код ЦАПа Dz"
	memcpy(&My_buf_Out[k],&K36[0],nK[35]);
	k = k + nK[35];

//37.команда K37 "Состояние бита Q18" (K37[1] = m_uBitQ18;)
	memcpy(&My_buf_Out[k],&K37[0],nK[36]);
	k = k + nK[36];

//40.команда K40 "Признак: биполярный транзистор + импульс = да или нет" 
	memcpy(&My_buf_Out[k],&K40[0],nK[39]);
	k = k + nK[39];

//-------------------------------
	My_buf_Out[k] = '\0';	//конец передачи
	k = k + 1;
//-------------------------------
  ::WaitForSingleObject(hMutexO,INFINITE);
//кол-во байт для передачи (NumO)
//--------------------------------
    NumO = k;
//кол-во байт для приёма массива (NumI)
//---------------------------------------
//  if (m_itipimp==TIP_SYN)
/*  if (m_itipimp==TIP_SYN  || m_itipimp==-1)
  {	m_nMas=1; n_Mas = 1;
    if (m_irazv==AC) m_nMas=2;
	if (m_nMas==1) NumI = NBUFE;	//1 массив из 1600 байт (200 точек)= 200 точек
	if (m_nMas==2) NumI = NBUFE;//_AC;	//2 массива по 800 байт(100 точек) = 200 точек
  }
*/
	if (m_idlitimp==-1) m_idlitimp=0;

	
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) 
  {	if (bMeas == MEAS_SINGLE) m_nMas = uNpointsLupa; //m_iLimitKolU/5;//№ точки, соответствующей ограничению Uc в %
                       else   m_nMas=1;//uNpointsLupa;
	if (m_bBeta || m_bBetai || m_bS)  m_nMas=1;	//одно значение на каждой ступеньке при Uc = 5V
    n_Mas = m_nMas;
	NumI = mas_n_points[m_idlitimp] * 8;//число точек * число байт	
  }
  else
  {	m_nMas=1; n_Mas = 1; //m_idlitimp=0;
    if (m_irazv==AC) m_nMas=2;
	if (m_nMas==1) NumI = NBUFE;	//1 массив из 1600 байт (200 точек)= 200 точек
	if (m_nMas==2) NumI = NBUFE;//_AC;	//2 массива по 800 байт(100 точек) = 200 точек
  }

  if (m_irazv==DC_POS || m_irazv==DC_NEG)
	{
	   if (bMeas == MEAS_SINGLE) m_nMas = uNpointsLupa; 
                           else  m_nMas=1;//1;
 //    if (m_bBeta || m_bBetai || m_bS)  m_nMas=1;	//одно значение на каждой ступеньке при Uc = 5V
	if (m_iQue1==COLLECTOR)
	{
	if (m_bBeta || m_bBetai || m_bS)  {m_nMas = 1;}//t_0=0;}	//одно значение на каждой ступеньке при Uc = 5V
	}
	   n_Mas=1;
	}
	NPOINT = mas_n_points[m_idlitimp];//число точек в снимаемом массиве
  if (m_nMas==0) m_nMas=1;
  ::ReleaseMutex(hMutexO);
}

UINT ThreadComIn(LPVOID pParam)
{ //int i; 
  for (;;)
  { //Switch_temp = 0;
	WaitCommEvent(hCom2,&dwEvtMask,&o);
	if (dwEvtMask & EV_RXCHAR)// Ожидание нажатия физической кнопки или рукоятки (по СОМ2)
	{ 
	  TRACE(_T("===EV_RXCHAR===\n"));
//	  dwRead2=2;
	  do
	  {
	  ::ReadFile(hCom2,bufComIn2,2,&dwRead2,NULL);
	  if (dwRead2==2)
	  {
//	  TRACE(_T("***********ThreadComIn dwRead2 = %d %d %d %d %d ***********\n"),dwRead2,bufComIn2[0],bufComIn2[1],bufComIn2[2],bufComIn2[3]);
	  TRACE(_T("***********ThreadComIn dwRead2 = %d %x %x %x %x ***********\n"),dwRead2,bufComIn2[0],bufComIn2[1],bufComIn2[2],bufComIn2[3]);
	  ::WaitForSingleObject(hMutexI,INFINITE);
	  bufIn2[0]=(BYTE)dwRead2;						//кол-во
	  memcpy(&bufIn2[1],&bufComIn2[0],dwRead2);		//данные
	  ::ReleaseMutex(hMutexI);
//	  TRACE(_T("***********ThreadComIn  %d %d ***********\n"),bufIn2[1],bufIn2[2]);
	  ::SendMessage((HWND)pParam,WM_BUTTON,0,0);	//код кнопки в bufIn2[1]	
	  }
	  } while (dwRead2==2);
	}
  }//end of for(;;)

return(0);
}


UINT ThreadComOut(LPVOID pParam)
{  // Передача команд Pic-контроллеру (по порту СОМ2)

	CString str;
  
 for(;;)
 {	
	::WaitForSingleObject(hEventComOut,INFINITE);
	::WaitForSingleObject(hMutexO,INFINITE);

	TRACE(_T("ThreadComOut\n"));
//for(;;)	::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);	//+1 байт, т.к. с кол-вом байт для передачи

	// послать команды в Pic
	//======================
	if(!::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL))	//+1 байт, т.к. с кол-вом байт для передачи
	{ 
      FlushFileBuffers(hCom2);
	  PurgeComm(hCom2,PURGE_TXCLEAR);
	  ::ReleaseMutex(hMutexO);
	  ::WaitForSingleObject(hMutexRes,INFINITE);
        bufRes[0]=1;	                
        bufRes[1]='L';	            //ERROR 201: ошибка записи команды в СОМ1
	  ::ReleaseMutex(hMutexRes);
	  ::PostMessage((HWND)pParam,WM_RESPONSE,0,0);
		TRACE(_T("ThreadComOut ERROR 201: ошибка записи команды в СОМ1 \n"));
	}
	else
	{ if (dwWrite != (DWORD)NumO+1)	//+1 байт, т.к. с кол-вом байт для передачи
		{ FlushFileBuffers(hCom2);
	      PurgeComm(hCom2,PURGE_TXCLEAR);
		  ::ReleaseMutex(hMutexO);
		  ::WaitForSingleObject(hMutexRes,INFINITE);
            bufRes[0]=1;	                
            bufRes[1]='W';	         //ERROR 202: передались не все команды в СОМ1-порт
	      ::ReleaseMutex(hMutexRes);
	      ::PostMessage((HWND)pParam,WM_RESPONSE,0,0);
		TRACE(_T("ThreadComOut ERROR 202: передались не все команды в СОМ1 \n"));
		}
	  else
	  {	//передана вся команда 
	    ::ReleaseMutex(hMutexO);
//надо ==================>следующее
		::WaitForSingleObject(hMutexRes,INFINITE);
        bufRes[0]=1;	                
        bufRes[1]='R';				// запись в порт СОМ2 прошла успешно
	    ::ReleaseMutex(hMutexRes);
	    ::PostMessage((HWND)pParam,WM_RESPONSE,0,0);
//	TRACE(_T("ThreadComOut OK!!!\n"));

//<==================это
//вместо следующего =>
//<======== это не надо
	  }
	}
 }//end for
 
return(0);
}

UINT ThreadEppIn(LPVOID pParam)
{ // Ожидание "готовности" и приём массива результатов из Pic-контроллера
  // через порт 0x78

//test
//=======================================
//    CCriticalSection MycriticalSection;
  CRITICAL_SECTION csMyCriticalSection;
  InitializeCriticalSection (&csMyCriticalSection);
  BYTE g = 0;
  int  i = 0;
  DWORD dwStop;
  int	ens;
  int   strobe;
//  CWnd	hWnd;

  for(;;)	
  {//for  
	//разрешить чтение массива данных (установить сигнал ENB_PCM <- 1)


		::WaitForSingleObject(hEventEppIn,INFINITE);
    TRACE(_T("ThreadEppIn\n"));
	
	__try
	{
		EnterCriticalSection (&csMyCriticalSection);//MyCriticalSection.Lock();
//		Sleep(10);

	//разрешить чтение массива данных (установить сигнал ENB_PCM <- 1)
	ens = _inp(0x79);		
	ens = ens | 0x02;
	_outp(0x79,ens);			//D1<-1 (ENB_PCM <- 1)
    TRACE(_T("ThreadEppIn (ENB_PCM <- 1)\n"));
	//ожидание готовности	
	do
	{ g =_inp(0x79);
	  g = g & 0x40;									
	  dwStop = ::WaitForSingleObject(hEventStop,0);//0
	} while (g == 0x00 && dwStop != WAIT_OBJECT_0);		//D6 0->1
    TRACE(_T("ThreadEppIn (0->1)\n"));
	do
	{ g =_inp(0x79);
	  g = g & 0x40;
	  dwStop = ::WaitForSingleObject(hEventStop,0);//0
	} while (g == 0x40 && dwStop != WAIT_OBJECT_0);		//D6 1->0
    TRACE(_T("ThreadEppIn (1->0)\n"));
	
	
	if (dwStop == WAIT_OBJECT_0)
	{ //"готовности" нет (экстренный стоп)

//			::WaitForSingleObject(hMutexEpp,INFINITE);
//			  bufIn_Epp[0]='S';				//error
//			  bufIn_Epp[1]='T';				//
//			ens = _inp(0x378+2);			//запретить установку сигнала ENS
//			ens = ens | 0x08;
// 			_outp(0x378+2,ens);	         //D3<-1 (ENS<-1)
//			::ReleaseMutex(hMutexEpp);
////     ::PostMessage((HWND)pParam,WM_MASSIV,1L,1L);
	}
	else
	{ //"готовность принята" => принять массив данных
			// Your code to access the shared resource goes here.
			::WaitForSingleObject(hMutexEpp,INFINITE);
			for(i=0;i<NumI;i++)
			{ //STROBE <- 0
				strobe = _inp(0x79);		
				strobe = strobe & 0xfe;
				_outp(0x79,strobe);			//D0<-0

			  //чтение байта данных
	   			bufEppIn[i] = _inp(0x78);

			  //STROBE <- 1
				strobe = _inp(0x79);		
				strobe = strobe | 0x01;
				_outp(0x79,strobe);			//D0<-1
				strobe = _inp(0x79);					//задержка 10us
				strobe = strobe | 0x01;
				_outp(0x79,strobe);			//D0<-1
				strobe = _inp(0x79);		
				strobe = strobe | 0x01;
				_outp(0x79,strobe);			//D0<-1
				
			}

			::ReleaseMutex(hMutexEpp);
			::PostMessage((HWND)pParam,WM_MASSIV,0,0);	
	}

	}
	__finally
	{
		// Release ownership of the critical section
		LeaveCriticalSection (&csMyCriticalSection);//MyCriticalSection.Unlock();
	}


  }//end for 

return (0);
}


void CLineDlg::Pusk()
{ 	CString str;


	if (!bMeasDiff && !bStatusDiff)
	{
	 str = _T("ИЗМЕРЕНИЕ!");
	}
	if (bMeasDiff && !bStatusDiff)
	{
			str = _T("РАЗНОСТЬ ВАХ\nИзмерение ВАХ дополнительного элемента");
	}
	if (!bMeasDiff && bStatusDiff)
	{
		switch(m_iDifference)
		{ case 1: str = _T("РАЗНОСТЬ ВАХ\nИзмерение с последовательным включением");//разности ВАХ");
				  break;
		  case 2: str = _T("РАЗНОСТЬ ВАХ\nИзмерение с параллельным включением");//разности ВАХ");
			      break;
		}
	}

 SetTextInfo(str);

  if (nCheckParam) {b_flagCheckParam = true; nCheckParam=0; MarkerNoakt();}
//  if (nCheckParam) { MarkerNoakt();}//nCheckParam=0;
 
//  	ResetRef();
//  if (bStatusRef) {b_flagStatusRef = true; ResetRef();}
  if (bStatusRef && m_sArg==_T("")) {b_flagStatusRef = true; ResetRef();}
  if (bStatusRef && m_sArg!=_T("")) {b_flagStatusRef = false; ResetRef();}


//  if (m_bBetai)
//  	MessageBox(_T("Установите инверсное включение транзистора!"),NULL,MB_OK|MB_ICONEXCLAMATION);
  ClearResults();
  FlagOLClear();
//  InitTablRes();//!!!
  ClearTablRes();
	m_Chart.DisableRefresh(true);

		int b = m_Chart.GetSeriesCount();
		TRACE(L"Pusk: до удаления GetSeriesCount = %d\n",b);
		for(int j=0;j<b;j++)
		m_Chart.DeleteSerie(0);

//		b = m_Chart.GetSeriesCount();
//		TRACE(L"Pusk: после удаления GetSeriesCount = %d\n",b);
		b = m_Chart.GetFamiliesCount();
		TRACE(L"Pusk: до удаления GetFamiliesCount = %d\n",b);
		for(j=0;j<b;j++)
	    m_Chart.DeleteFamily(0);
//		b = m_Chart.GetFamiliesCount();
//		TRACE(L"Pusk: после удаления GetFamiliesCount = %d\n",b);

	bStatusMeas = false;
	CChartSerie*  pGraph;
 for (j=0;j<n_Step;j++)
 { 	
    switch(m_iStyleCurve)
	{ case STYLE_LINE:	    pGraph = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraph = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraph = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}

 }
	CChartSerieFamily* pFamily = m_Chart.AddFamily();
	      pGraph->SetColor(m_colorCurve);
	pFamily->SetColor(m_colorCurve);
	pFamily->SetName("Meas");
	
	m_Chart.DisableRefresh(false);

	if (bMeas == MEAS_SINGLE) SetMassivNpoint(true);//+++	//real massiv
  
  BuildCommand();
  BuildTest();
  nCountPointDC = 1;//0;//для DC счетчик точек
  n_pointDC = 0;//1; № точки
  m_iPosTekOld = m_iPosTek;
  m_nCountMas=0;		
  stup  = 0;				// текущее значение №-ра ступеньки
//  nK41=0;
  ready = 0;
//  flag_G = 0;
//				res_propusk=true;
//				n_propusk=0;
  n_propusk = 1;
  if ((bias_KolMeasI+m_iKolI)>=29 && (bias_KolSourceU+m_iKolSourceU)<4) n_propusk=4;
  if ((bias_KolMeasI+m_iKolI)>=29 && (bias_KolSourceU+m_iKolSourceU)>3) n_propusk=9;

  // поместить в bufOut управляющие команды
  ::WaitForSingleObject(hMutexO,INFINITE);
  bufOut[0]=(BYTE)NumO+1;						//кол-во байт для передачи + 1 байт с количеством
  memcpy(&bufOut[1],&My_buf_Out[0],bufOut[0]);	//данные
  ::ReleaseMutex(hMutexO);
//  PurgeComm(hCom2,PURGE_TXCLEAR|PURGE_RXCLEAR);//23.02.16
//  SetEvent(hEventComOut);//++++++++++++++++++++
	::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);	//+1 байт, т.к. с кол-вом байт для передачи

  	CButton* pbut = (CButton*)GetDlgItem(IDC_BUTTON_MEAS);
		pbut -> SetWindowText(L"Сброс");

}

void CLineDlg::Stop()
{ int k=0;

  if (bAutoSetNull)
  { bAutoSetNull = 0;
    TRACE(_T("Stop()!!! K13\n"));
    // поместить в bufOut управляющие команды
   ::WaitForSingleObject(hMutexO,INFINITE);
    bufOut[0]=3;//4;							//кол-во байт для передачи + 1 байт с количеством
    bufOut[1]=K13[0];							//команда "Экстренный стоп"
    bufOut[2]='\0';							//конец передачи
    NumO = 2;
    ::ReleaseMutex(hMutexO);
    PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
    //  ::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);
    SetEvent(hEventComOut);//+++++++++++++++++++++
	
	if (hEventEppIn) SetEvent(hEventStop);
  }
  
  if (bKalibr)
  { bKalibr = 0;
//    TRACE(_T("Stop()!!! K13\n"));
    TRACE(_T("Stop()!!! K38\n"));
    // поместить в bufOut управляющие команды
   ::WaitForSingleObject(hMutexO,INFINITE);
    bufOut[0]=3;//4;							//кол-во байт для передачи + 1 байт с количеством
    bufOut[1]=K38[0];//K13[0];							//команда "Экстренный стоп"
    bufOut[2]='\0';							//конец передачи
    NumO = 2;
    ::ReleaseMutex(hMutexO);
    PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
    //  ::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);
    SetEvent(hEventComOut);//+++++++++++++++++++++
 	
	if (hEventEppIn) SetEvent(hEventStop);
 }

//  m_iNproxod=0;
  if (bPusk)
  {
	 if ((m_irazv!=SYN_POS && m_irazv!=SYN_NEG && bMeas == MEAS_SINGLE)||
		((m_irazv==SYN_POS || m_irazv==SYN_NEG) && (bMeas == MEAS_SINGLE && bStop==true))||
		 bMeas == MEAS_MULTIPLE||
		 bAutoSetNullIc_10nA)
		{
		 bPusk = false;
//		 bStop = true;
		 ready = 0;//24.06.16
		  TRACE(_T("Stop()!!! K38\n"));

		 // поместить в bufOut управляющие команды
		 ::WaitForSingleObject(hMutexO,INFINITE);
		 bufOut[0]=3;//4;							//кол-во байт для передачи + 1 байт с количеством
		  bufOut[1]=K38[0];							//команда "Экстренный стоп"
		 bufOut[2]='\0';							//конец передачи
		 NumO = 2;
		::ReleaseMutex(hMutexO);
		  PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
		//  ::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);
		 SetEvent(hEventComOut);//+++++++++++++++++++++

/*		Sleep(100);

		 if (!bAutoSetNullIc_10nA)
		 {
		  SetMassiv(true);
			 InitTablRes();
		 }
		FinishControlFlags();
*/	
		 if (!bAutoSetNullIc_10nA)
		{
		 if ((bMeas == MEAS_SINGLE && bStop==false) ||
			 (bMeas == MEAS_MULTIPLE && bStop==true))
		 {  //выполняем, если измерение выполнялось, т. е. if bStop==false
		    SetMassiv(true);
			InitTablRes();
					Otcht();			//подготовка отчета по тесту//28.01.2016
   					DisplayOtcht();		//индикация отчета
					if  (bSound) AutoSaveTablResult();
		 }
		}
//		 if ((bMeas == MEAS_SINGLE && bStop==false) ||
//			 (bMeas == MEAS_MULTIPLE && bStop==true))
//		FinishControlFlags();//выполняем, если измерение выполнялось, т. е. if bStop==false

		 int	ens = _inp(0x79);		//запретить подачу сигнала на объект исследования
				ens = ens & 0xfd;
 				_outp(0x79,ens);	         //D1<-0 (ENB_PCM <- 0)
    TRACE(_T("Stop (ENB_PCM <- 0)\n"));

		 
		bStop = true;

		 if (hEventEppIn) SetEvent(hEventStop);
	 

		}
	   //обнулить состояние рукоятки
	 if (bMeas == MEAS_MULTIPLE) 
	{ 
     ::WaitForSingleObject(hMutexRukoyatka,INFINITE);
	  m_iPosTek=0;
     ::ReleaseMutex(hMutexRukoyatka);
	  DrawStatusRukoyatka(); //обнулить состояние рукоятки
	}
 }
  	CButton* pbut = (CButton*)GetDlgItem(IDC_BUTTON_MEAS);
		pbut -> SetWindowText(L"Измерение");
}


void CLineDlg::ExtraStop(BYTE condition,LPCTSTR strMessage)
{ //экстренный останов измерения по условию  
	MyMessageBeep(MB_ICONASTERISK);
}


void CLineDlg::MyMessageBeep(UINT uType)
{ //if (bSound) MessageBeep(uType);
}

LONG CLineDlg::OnButton(WPARAM wParam,LPARAM lParam)
{ CString str;
  CClientDC dc(this);
  CRect rect;
//  BYTE maska_kod = 0x3c;
  BYTE maska_kod = 0x3d;
  BYTE kod;
	CButton* pbut = (CButton*)GetDlgItem(IDC_BUTTON_MEAS);
	//переписать данные из буфера bufIn2
	::WaitForSingleObject(hMutexI,INFINITE);
	  memcpy(&My_buf_But[0],&bufIn2[1],bufIn2[0]);	//данные
	::ReleaseMutex(hMutexI);	//23.02.16 
	  TRACE (_T("OnButton\n"));
	switch(My_buf_But[0])	//код физ.кнопки
	{ case 1:	// кнопка "Пуск" - ("Измерение однократное")
				  bMeas = MEAS_SINGLE;
				if (bAutoSetNullIc_10nA==true)
				 {
					MessageBox(_T("Отсоедините перемычку \".027\", замыкающую гнезда Uc и E'!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
					  Flag_sboy=true;
				      bPusk = TRUE;			//запускаем измерение
					  Stop();
					bAutoSetNullIc_10nA=false;
					  break;
				 }
				if (m_bKrishka==1 && (bias_KolSourceU+m_iKolSourceU)>=2)
					{ //при запуске
					  str = _T("ERROR 207: БЛОКИРОВКА НАРУШЕНА! ЗАКРОЙТЕ КРЫШКУ!"); 
					  SetTextInfo(str);
					  Flag_sboy=true;
				      bPusk = TRUE;			//запускаем измерение
					  Stop();
					  break;
					}
			    b_InitAxis=true;
				if (ControlParam()!=0) 
					{
					  Flag_sboy=true;
				      bPusk = TRUE;			//запускаем измерение
					  Stop();
					  break;
					}

                ::WaitForSingleObject(hMutexStop,INFINITE);
				if (bStop)// && !bZapusk)	//if "Стоп" и запуск "внутренний"
				{ 
				  CCmdTarget::BeginWaitCursor();	//запускаем измерение
				  bPusk = TRUE;			//запускаем измерение
				  bStop = FALSE;
				  Flag  = FALSE;
				  Flag_sboy  = FALSE;
				  Flag_All = FALSE;
				  m_uCountTest  = 0;
				  bMeas = MEAS_SINGLE;
				  n_Test = CountnTest();//посчитать количество тестов
				  TRACE(L"========= PUSK - 1 ========\n");
				  Pusk();
				} //end of (bStop)	
			    ::ReleaseMutex(hMutexStop);
				break;
	  case 2:	// "Рукоятка"(первое вращение) - ("Измерение многократное")
				      bMeas = MEAS_MULTIPLE; 
				if (bAutoSetNullIc_10nA==true)
				 {
					MessageBox(_T("Отсоедините перемычку \".027\", замыкающую гнезда Uc и E'!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
					  Flag_sboy=true;
				      bPusk = TRUE;			//запускаем измерение
					  Stop();
					bAutoSetNullIc_10nA=false;
					  break;
				 }
				if (m_bKrishka==1 && (bias_KolSourceU+m_iKolSourceU)>=2)
					{//при запуске
					  str = _T("ERROR 207: БЛОКИРОВКА НАРУШЕНА! ЗАКРОЙТЕ КРЫШКУ!"); 
					  SetTextInfo(str);
					  Flag_sboy=true;
				      bPusk = TRUE;			//запускаем измерение
					  Stop();
					  break;
					}
				 b_InitAxis=true;
				if (ControlParam()!=0) 
					{//при запуске
				      bPusk = TRUE;			//запускаем измерение
					  Flag_sboy=true;
					  Stop();
					  break;
					}
			  
				if ((m_irazv==DC_NEG || m_irazv==DC_POS) && bGenerationStup==true) 
				{
				   bGenerationStup=false;//в DC многократном запретить ступеньки
				   n_Step = 1;
				}

				if ((m_irazv==IMP_NEG || m_irazv==IMP_POS) && bGenerationStup==true) 
				{  bGenerationStup=false;//в IMP многократном запретить ступеньки
				   n_Step = 1;
				}

                ::WaitForSingleObject(hMutexStop,INFINITE);
//				if (bStop) //&& !bAutomat && !bZapusk)
				if (bStop && !bPusk)	//23.02.16 
//				if (bStop && !bPusk && !ready)	//24.02.16 
				{ 
				  m_uCountTest  = 0;
				  n_Test = CountnTest();//посчитать количество тестов
				  if (n_Test>1)	
				  { 
//					  bMeas = MEAS_SINGLE; //Запуск измерения следует осуществлять кнопкой \"Пуск\"!
//				      TRACE(L"======== PUSK - 1 =======\n");
					  Flag_sboy=true;
				      bPusk = TRUE;			//запускаем измерение
					  Stop();
				  }
				  else 
				  {  
					  n_Test = 1;
				      bMeas = MEAS_MULTIPLE; 
					  if (m_iQue1==BAZA) bMeas = MEAS_SINGLE; 
				      TRACE(L"======== PUSK - 2 =======\n");
 				      CCmdTarget::BeginWaitCursor();	//запускаем измерение
				      bPusk = TRUE;
				      bStop = FALSE;
				      Flag_sboy  = FALSE;
				      Flag  = FALSE;
				      Flag_All = FALSE;
//				     cikl=0;
//				    ::WaitForSingleObject(hMutexRukoyatka,INFINITE);
//				    ::ReleaseMutex(hMutexRukoyatka);
				  // поместить в bufOut управляющие команды
				  // c командой "Измерение многократное"
				     Pusk();
				  }
				} //end of (bStop)	
			    ::ReleaseMutex(hMutexStop);
				break;
      case 3:	// "Новое состояние рукоятки"
                ::WaitForSingleObject(hMutexStop,INFINITE);
				if (!bStop && !bAutomat && !bZapusk)// && ready)
				{ //отобразить новое состояние рукоятки
				  ::WaitForSingleObject(hMutexRukoyatka,INFINITE);
				  m_iPosTek = My_buf_But[1];
				  if (m_iPosTek<m_iLimitKolI)m_iPosTek=m_iLimitKolI+m_iPosTek;
	
				  ::ReleaseMutex(hMutexRukoyatka);
				  DrawStatusRukoyatka();//
				}
			    ::ReleaseMutex(hMutexStop);
				break;
	  
	  case 5:	//1-в режиме DC многократный произошла смена ступеньки в первой очереди 
				//(пришла команда  5 от Pic-a)
				TRACE(_T("5555555555555555555555\n"));
				if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas==MEAS_MULTIPLE)
				{ ::WaitForSingleObject(hMutexDCres_gotov,INFINITE);
				  res_gotov=true;
				  ::ReleaseMutex(hMutexDCres_gotov);

					int	ens = _inp(0x79);	//запретить подачу сигнала на объект исследования
					ens = ens & 0xfd;		//D1<-0 (ENB_PCM <- 0)
 					_outp(0x79,ens);	         
					ens = _inp(0x79);		//Задержка 10us 
					ens = ens & 0xfd;
					_outp(0x79,ens);	    
					ens = _inp(0x79);		//Задержка 10us 
					ens = ens & 0xfd;
			 		_outp(0x79,ens);	    
					ens = _inp(0x79);		//Задержка 10us 
					ens = ens & 0xfd;
			 		_outp(0x79,ens);	    
				    TRACE(_T("ThreadEppIn (ENB_PCM <- 0)\n"));

					ens = _inp(0x79);		//разрешить подачу сигнала на объект исследования
					ens = ens | 0x02;
					_outp(0x79,ens);		//D1<-1 (ENB_PCM <- 1)
					TRACE(_T("ThreadEppIn (ENB_PCM <- 1)\n"));

				}
				if (!bStop)// && ready)
				{
				if ((m_irazv==IMP_POS || m_irazv==IMP_NEG) && bMeas==MEAS_MULTIPLE)
				{
				    m_nMas = My_buf_But[1];
			        n_Mas = m_nMas;
				}
				}
				break;

	  case 4:   // кнопка "Стоп"
                  TRACE(_T("STOP\n"));
				::WaitForSingleObject(hMutexStop,INFINITE);
/*				   if (bPusk) 
				   { bStop = TRUE;
//				     bPusk = FALSE;	//bPusk не сбрасывать, чтобы процесс измерения дошел до конца
				   }
				   else
				   { bStop = TRUE;
				     bPusk = FALSE;
			         ready = 0;
				   }
*/
//========================
				   if (bPusk && ready)      //измерение было 
				   { bStop = TRUE;
//				     bPusk = FALSE;			//bPusk не сбрасывать, чтобы процесс измерения дошел до конца
				   }
				   else
				   { if (bPusk && !ready)   //быстрый стоп после пуска
					{  //  str = _T("ИЗМЕРЕНИЕ ЗАКОНЧЕНО!");
					   //	SetTextInfo(str);
						Flag_sboy = true;
						Stop();				//экстренный стоп для завершения всех процессов
					}
				    else
					{  //!bPusk				//нажали кнопку Стоп
					   bStop = TRUE;
				       bPusk = FALSE;
			           ready = 0;
				       Flag  = false;
					   Flag_All = false;
					   Flag_sboy = true;
		               pbut -> SetWindowText(L"Измерение");
					}
//				   }
//===================
				   ::ReleaseMutex(hMutexStop);
				   if (Flag_sboy)
				   {
				     Flag_sboy=0;
				     //очистить сообщение об ошибке
				     SetTextInfo(_T(" TEST:  ")+strTestName);//отобразить выбранный тест в информационном окне
				   }
				   }
				break;

	  case 'G': //получена готовность => пошли читать результат (для синхронизации измерения)
				::WaitForSingleObject(hMutexStop,INFINITE);
				 TRACE(_T("=================G==================\n"));
//				 flag_ruk= 0;
				if (!ready)
				{
				 if (bPusk) {
					 ready=1;
					 SetEvent(hEventEppIn);
				 TRACE(_T("SetEvent(hEventEppIn);\n"));
				 }	//``````````````
				}

				if (bAutoSetNull)
				{//КОРРЕКЦИЯ СМЕЩЕНИЯ  НУЛЯ
					if (!ready)
					{ 
					  SetEvent(hEventEppIn);	//``````````````
				 TRACE(_T("SetEvent(hEventEppIn);\n"));
				 ready = 1;
					}
				}//end if (bAutoSetNull)
				if (bKalibr)
				{//КАЛИБРОВКА
					if (!ready)
					{ ready = 1;
					  SetEvent(hEventEppIn);	//``````````````
					}
				}//end if (bKalibr)
			    ::ReleaseMutex(hMutexStop);
				break;
	  case 'P': //n_Switch = 0 === Переключатель +-2000В
		        //n_Switch = 1 === Переключатель +5В
				//n_Switch = 2 === Переключатель -5В
				//n_Switch = 3 === Переключатель в нейтральном положении
				//n_Switch = 4 === Переключатель неисправен
				 TRACE(_T("=================P==================\n"));
				if (bPusk)
				{ //во время измерения переключили диапазон источника
				  //экстренный останов измерения
				  //SetEvent(hEventStop);
//				  str = _T("ИЗМЕНЕН ДИАПАЗОН ИСТОЧНИКА ВО ВРЕМЯ ИЗМЕРЕНИЯ!");
//				  SetTextInfo(str);
				  ready = 0;
				  bPusk = false;
				  bStop = true;
				  Flag_sboy = true;
				  Stop();
				} 
		        n_Switch = My_buf_But[1];
				 	m_MOD = _T("");
				if (n_Switch<3) InitNewSwitch();
				if (n_Switch==3)
				{ str=_T("ПЕРЕКЛЮЧАТЕЛЬ ±2000V/±5V В НЕЙТРАЛЬНОМ ПОЛОЖЕНИИ!");
				  SetTextInfo(str);
				  if(m_bFileOpen)
				  { 
//					SetModifiedFlag();
//					FileClose();
					pModSelected = NULL;
					i_EL = 0;
					i_MOD = 0;
					i_SelUrov = 0;
//					m_EL = _T("");
				 	m_MOD = _T("");
					m_PRIM = _T("");
//					str_EL		= _T("");
				    str_MOD		= _T("");
//				    strTestName   = _T("");
					str_PRIM = _T("");
				    bAutomat = HAND; // "ручной"
					m_sNameFileMod= _T("Файл модели");
					UpdateData(FALSE);
				  }
				}
				if (n_Switch==4)
				{ str=_T("ERROR 216: ПЕРЕКЛЮЧАТЕЛЬ ±2000V/±5V НЕИСПРАВЕН!");
				  SetTextInfo(str);
				  if(m_bFileOpen)
				  { 
//					SetModifiedFlag();
//					FileClose();
					pModSelected = NULL;
					i_EL = 0;
					i_MOD = 0;
					i_SelUrov = 0;
//					m_EL = _T("");
  					m_MOD = _T("");
					m_PRIM = _T("");
//					str_EL		= _T("");
					str_MOD		= _T("");
//					strTestName   = _T("");
					str_PRIM = _T("");
					bAutomat = HAND; // "ручной"
					m_sNameFileMod= _T("Файл модели");
					UpdateData(FALSE);
				  }
				}
		        break;
	  case 'F':	//сигнал (измерение не прерываем, все отключается аппаратно)
				 TRACE(_T("=================F==================\n"));
//		        if (bPusk)
				{ kod = My_buf_But[1];
				  kod = kod & maska_kod;
				  switch(kod)
				  { case 0x3d: //xx1111x1	case 0x3c: //xx1111xx
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)//   && (bias_KolSourceU+m_iKolSourceU)>=2)//&& !Flag_sboy
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
								break;
				    case 0x39: //xx1110x1	
					case 0x38: //xx1110xx
//					case 0x30: //xx1100x0
					case 0x28: //xx1010x0
				    case 0x18: //xx0110x0
					case 0x20: //xx1000x0						
					case 0x08: //xx0010x0
					case 0x10: //xx0100x0
					case 0x00: //xx0000x0
								TRACE(_T("COVER OPEN!\n"));
                               ::WaitForSingleObject(hMutexStop,INFINITE);
								if (!bPusk  && !m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)// && !Flag_sboy 
								{
								str = _T("КРЫШКА ОТКРЫТА!"); 
								SetTextInfo(str);
								}
								m_bKrishka = 1;
								if (bPusk && (bias_KolSourceU+m_iKolSourceU)>=2)
								{//в ходе измерения
								  str = _T("ERROR 207: БЛОКИРОВКА НАРУШЕНА! ЗАКРОЙТЕ КРЫШКУ!"); 
								  SetTextInfo(str);
								  Flag_sboy = true;	
								  Stop();
								}
								break;
					case 0x30: //xx1100x0
								TRACE(_T("COVER OPEN!\n"));
								if (!bPusk && !m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2) //&& !Flag_sboy 
								{ 
								str = _T("КРЫШКА ОТКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 1;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr))// && ready==1)// && m_iCircuitKol!=1)
								{
								str = _T("ERROR 208: ПРЕВЫШЕН ПОРОГ ПО ТОКУ В КАНАЛЕ С!");
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
					case 0x34: //xx1101x0
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2) //&& !Flag_sboy 
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr))// && ready==1)// && m_iCircuitKol!=1)
								{
								str = _T("ERROR 208: ПРЕВЫШЕН ПОРОГ ПО ТОКУ В КАНАЛЕ С!");
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
					case 0x2c: //xx1011x0
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)//&& Flag_sboy  
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;

//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr) && (m_iCircuitBaza>=1))// && ready==1))
								{
								str = _T("ERROR 209: ПРЕВЫШЕН ПОРОГ ОГРАНИЧЕНИЯ В КАНАЛЕ B!");
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
//				    case 0x18: //xx0110x0
				    case 0x1c: //xx0111x0
//				    case 0x1d: //xx0111x1
//				    case 0x19: //xx0110x1
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)// && !Flag_sboy 
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr) && (m_iCircuitDop>=1))//  && ready==1))
								{
								str = _T("ERROR 210: ПРЕВЫШЕН ПОРОГ ОГРАНИЧЕНИЯ В КАНАЛЕ S!");
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
//					case 0x20: //xx1000x0						
					case 0x24: //xx1001x0						
//					case 0x25: //xx1001x1						
//					case 0x21: //xx1000x1
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)// && !Flag_sboy 
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr) && (m_iCircuitBaza>=1))//  && ready==1))
								{
								str = _T("ERROR 211: ПРЕВЫШЕН ПОРОГ ОГРАНИЧЕНИЯ В КАНАЛАХ С И В!");					
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
//					case 0x08: //xx0010x0
					case 0x0c: //xx0011x0
//					case 0x0d: //xx0011x1
//					case 0x09: //xx0010x1
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)// && !Flag_sboy 
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr) && (m_iCircuitBaza>=1 && m_iCircuitDop>=1))// && ready==1))
								{
						        str = _T("ERROR 212: ПРЕВЫШЕН ПОРОГ ОГРАНИЧЕНИЯ В КАНАЛАХ В И S!");					
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
//					case 0x10: //xx0100x0
					case 0x14: //xx0101x0
//					case 0x15: //xx0101x1
//					case 0x11: //xx0100x1
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)// && !Flag_sboy 
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr) && (m_iCircuitDop>=1))//  && ready==1))
								{
								str = _T("ERROR 213: ПРЕВЫШЕН ПОРОГ ОГРАНИЧЕНИЯ В КАНАЛАХ С И S!");
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
//					case 0x00: //xx0000x0
					case 0x04: //xx0001x0
//					case 0x05: //xx0001x1
//					case 0x01: //xx0000x1
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)// && !Flag_sboy 
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr) && (m_iCircuitBaza>=1 && m_iCircuitDop>=1))// && ready==1))
								{
								str = _T("ERROR 214: ПРЕВЫШЕН ПОРОГ ОГРАНИЧЕНИЯ В КАНАЛАХ С, В И S!");					
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
					case 0x3c: //xx1111x0
//					case 0x38: //xx1111x0
								TRACE(_T("COVER CLOSE!\n"));
								if (!bPusk && m_bKrishka)// && (bias_KolSourceU+m_iKolSourceU)>=2)// && !Flag_sboy 
								{ 
								str = _T("КРЫШКА ЗАКРЫТА!"); 
								SetTextInfo(str);
//								Flag_sboy=0;
								}
								m_bKrishka = 0;
//								if (bPusk || bAutoSetNull || (bKalibr && m_iNsetK!=1))
								if ((bPusk || bAutoSetNull || bKalibr))//  && ready==1)// && m_iCircuitKol!=1)
								{
								str = _T("ERROR 215: ПРЕВЫШЕН ПОРОГ ПО МОЩНОСТИ В КАНАЛЕ С!");					
							    if (bAutoSetNull) str = str +_T(" КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");
							    if (bKalibr) str = str +_T(" КАЛИБРОВКА ПРЕРВАНА!");
								SetTextInfo(str);
								Flag_sboy = true;
								Stop();
								}
								break;
				  }
				}
				break;
	  case 'E': //ошибка при разборе команд Pic-ом
				if (bPusk)	
				{
//				  SetEvent(hEventStop);
				  Stop();
					str = _T("ERROR 204: ОШИБКА ПРИЕМА!\nИЗМЕРЕНИЕ ОСТАНОВЛЕНО!");//ошибка при разборе команд Pic-ом	
				  SetTextInfo(str);
				  bPusk = false;
				  bStop = true;
				  Flag_sboy = true;
				  ready = 0;
				}
				break;

	  default:  // ошибка: неопознанный код физической кнопки
				if (bPusk)
				{ //экстренный останов измерения
//				  SetEvent(hEventStop);
				  Stop();
				  str = _T("ERROR 200: ПРОИЗОШЁЛ СБОЙ!\nПОВТОРИТЕ ИЗМЕРЕНИЕ!");//неопознанный код физической кнопки
				  SetTextInfo(str);
				  bPusk = false;
				  bStop = true;
				  Flag_sboy = true;
				  ready = 0;
				}
				break;

	}//end switch(My_buf_But[0])

return 0L;
}


void CLineDlg::InitNewSwitch()
{ CString str;
if(pModSelected==NULL)
{	
	UpdateData();

	m_iCircuitKol=1;//выключить обрыв
	
	if (n_IPPP && n_Switch==1)
	{ str=_T("ПЕРЕКЛЮЧАТЕЛЬ В ПОЛОЖЕНИИ +5V");
		m_irazv = IMP_POS;
		m_iButNull = LEFT;
		if (m_nelement==PNP)   m_nelement=NPN;
		if (m_nelement==PNP4)  m_nelement=NPN4;
		if (m_nelement==PMOS)  m_nelement=NMOS;
		if (m_nelement==PMOS4) m_nelement=NMOS4;
		m_idlitimp=5;
	    bias_KolSourceU = 0;
	    bias_KolMeasU = 9;
		bias_KolMeasI = 0;
		m_iKolSourceU=0;
		m_iKolU = 0;
		m_iKolI = 3;
	}
	if (n_IPPP && n_Switch==2)
	{ str=_T("ПЕРЕКЛЮЧАТЕЛЬ В ПОЛОЖЕНИИ -5V");
		m_irazv = IMP_NEG;
		m_iButNull = RIGHT;
		if (m_nelement==NPN)   m_nelement=PNP;
		if (m_nelement==NPN4)  m_nelement=PNP4;
		if (m_nelement==NMOS)  m_nelement=PMOS;
		if (m_nelement==NMOS4) m_nelement=PMOS4;
		m_idlitimp=5;
	    bias_KolSourceU = 0;
	    bias_KolMeasU = 9;
		bias_KolMeasI = 0;
		m_iKolSourceU=0;
		m_iKolU = 0;
		m_iKolI = 3;
	}
	if (n_IPPP && n_Switch==0)
	{ str=_T("ПЕРЕКЛЮЧАТЕЛЬ В ПОЛОЖЕНИИ ±2000V");
		m_irazv = SYN_POS;//DC_POS;
		m_iButNull = CENTER;
		m_idlitimp=0;
		bias_KolSourceU = 1;
	    bias_KolMeasU = 0;
		bias_KolMeasI = 5;//4;
		m_iKolSourceU=0;
		m_iKolU = 8;
		m_iKolI = 13;//14;
		if (m_nelement==PNP || m_nelement==PNP4 ||
			m_nelement==PMOS ||m_nelement==PMOS4) m_irazv = SYN_NEG;
	}
	switch(m_nelement)
	{   case DIOD:   m_EL =_T("D");break;
		case TS:    m_EL =_T("TS");break;
		case NPN:	m_EL =_T("NPN");break;
		case NPN4:	m_EL =_T("NPN4");break;
		case PNP:	m_EL =_T("PNP");break;
		case PNP4:	m_EL =_T("PNP4");break;
		case NMOS:	m_EL =_T("NMOS");break;
		case NMOS4:	m_EL =_T("NMOS4");break;
		case PMOS:	m_EL =_T("PMOS");break;
		case PMOS4:	m_EL =_T("PMOS4");break;
	}


	m_MOD = _T("");
	str = str + _T("\nВыберите категорию и тест для измерений!");//добавила 6.12.17
	SetTextInfo(str);
//	  SetTextInfo(_T("Список тестов файла модели пуст!\nВыберите категорию и тест для измерений!"));
	
	UpdateData(false);
	
	ChangeDiapazonKolSourceU();
	ChangeDiapazonKolMeasU();
	ChangeDiapazonKol();
	
	SetDiapazonKol();
}

//			InitIconImp();
/*
	int nTab = 2;			//отобразить закладку "Канал С"
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);
*/
	int nTab = m_TabCtrlParam.GetCurSel();
	InitPage(nTab);

	int  nTabTuning = 0;//Error//1
	m_TabCtrlTuning.SetCurSel(nTabTuning);
	InitPageTuning(nTabTuning);	// отобразить панель "Дисплей"


	if (!bAutoSetNull && !bKalibr) ControlParam();//^^^^^^^^^^^^^^^^^^^^
	
	if (bStatusCalc) InitAxisCalc();
	else InitAxis();

}


LONG CLineDlg::OnResponse(WPARAM wParam,LPARAM lParam)
{	
  CString str;
  CClientDC dc(this);
  CRect rect;

	//переписать данные из буфера bufInRes
	::WaitForSingleObject(hMutexRes,INFINITE);
	  memcpy(&My_buf_Res[0],&bufRes[1],bufRes[0]);	//данные
	::ReleaseMutex(hMutexRes);
	//анализ отклика
      switch(My_buf_Res[0])
	  {   case 'R':	// команды переданы успешно => if включено измерение => принять массив
//					if (bPusk) SetEvent(hEventEppIn);	//``````````````
						TRACE(_T("ThreadComOut OK!!!\n"));
					if (bPusk) 
					{ 
						if (ready)
						{
							SetEvent(hEventEppIn);
							TRACE(_T("SetEvent(hEventEppIn);-R\n"));
						}
					}	//``````````````
					if (bAutoSetNull) 
					{ if (ready) 
					{
						SetEvent(hEventEppIn);}	//``````````````
							TRACE(_T("SetEvent(hEventEppIn);-R\n"));
					}
					if (bKalibr) 
					{ if (ready) SetEvent(hEventEppIn);}	//``````````````
					break;
	      case 'L':	// Ошибка записи команды в COM-порт
					if (bPusk)
					{ TRACE(_T("ERROR 201\n"));
                      str = _T("ERROR 201: ОШИБКА ПЕРЕДАЧИ!");	
			          SetTextInfo(str);	
					  Stop();	//экстренный останов
//					  ExtraStop(1,str);
					}
				    break;
	      case 'W':	// Ошибка тайм-аута при передаче команд в СОМ-порт
					if (bPusk) 
					{ TRACE(_T("ERROR 202\n"));
				      str = _T("ERROR 202: ТАЙМ-АУТ ПРИ ПЕРЕДАЧЕ!");
					  SetTextInfo(str);	
					  Stop();	//экстренный останов
//					  ExtraStop(1,str);
					}
				    break;
	  }//end switch(My_buf_Res[0])
//  }//end of (!bStop)
//	else ::ReleaseMutex(hMutexStop);

return 0L;
}

void CLineDlg::FlagOLClear()
{ for (int i=0;i<4;i++)
  FlagOL[i] = false;
}

void CLineDlg::FinishControlFlags()
{ 
//  CString str_acp=_T("");
//  CString str_mes=_T("");
  CString str=_T("");
 TRACE(_T("FinishControlFlags()!!!"));

  if (Flag_sboy==false)
  {//0
	if (FlagOL[0]==false && FlagOL[1]==false && FlagOL[2]==false && FlagOL[3]==false)
	{//1 
	  if (m_sMin.IsEmpty() && m_sMax.IsEmpty()) { str = _T("ИЗМЕРЕНИЕ ЗАКОНЧЕНО!\n");
												  OutLimit(0);//выдать сигнал "ЭЛЕМЕНТ ГОДЕН!"
												}
	  else
		if (!Flag) { str = _T("ИЗМЕРЕНИЕ ЗАКОНЧЕНО!\n ЭЛЕМЕНТ ГОДЕН!");
					  OutLimit(0);//выдать сигнал "ЭЛЕМЕНТ ГОДЕН!"
					}
		else   { str = _T("ИЗМЕРЕНИЕ ЗАКОНЧЕНО!\n ЭЛЕМЕНТ НЕ ГОДЕН!");
				 OutLimit(1);//выдать сигнал "ЭЛЕМЕНТ НЕ ГОДЕН!"
				}
	
	}//1
    SetTextInfo(str);
  }//0
  else
  { //2
	if (Flag)	{ str = _T("ИЗМЕРЕНИЕ ЗАКОНЧЕНО! ТРЕБУЕМОЕ ЗНАЧЕНИЕ АРГУМЕНТА ") + m_sArg+ _T(" НЕ ДОСТИГНУТО!");
			      OutLimit(2);//выдать сигнал "ТРЕБУЕМОЕ ЗНАЧЕНИЕ АРГУМЕНТА НЕ ДОСТИГНУТО"
				  SetTextInfo(str);
				}
	else
	OutLimit(3);//ВЫДАТЬ СИГНАЛ "ПРЕВЫШЕН ПОРОГ ИЛИ НАРУШЕНА БЛОКИРОВКА"
	
  }//2
   if (b_flagCheckParam && !bAutomat && !bZapusk)
	{ b_flagCheckParam=false; nCheckParam=1; MarkerAkt();}
    if (b_flagStatusRef && !bAutomat && !bZapusk)
	{ b_flagStatusRef=false; SetMassivRefNpoint(true); } 

int	ens = _inp(0x79);		//запретить подачу сигнала на объект исследования
		ens = ens & 0xfd;
 		_outp(0x79,ens);	         //D1<-0 (ENB_PCM <- 0)
    TRACE(_T("FinishControlFlag (ENB_PCM <- 0)\n"));

}

void CLineDlg::OutLimit(int kod)
{ //выдать сигнал (команда 33)
  // поместить в bufOut управляющие команды
 ::WaitForSingleObject(hMutexO,INFINITE);
  bufOut[0]=4;		//кол-во байт для передачи + 1 байт с количеством
  bufOut[1]=K33[0];	//команда "выдать сигнал"
  bufOut[2]=kod;	//код сигнала
  bufOut[3]='\0';	//конец передачи
  NumO = 3;
  ::ReleaseMutex(hMutexO);
//  PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
//  ::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);
 	TRACE(_T("OutLimit()!!! K33 %d\n"),kod);
 SetEvent(hEventComOut);//+++++++++++++++++++++
}

LONG CLineDlg::OnMassiv(WPARAM wParam,LPARAM lParam)
{ BOOL b = 0;
  int ens;
  	CButton* pbut = (CButton*)GetDlgItem(IDC_BUTTON_MEAS);
  CString str;
  CClientDC dc(this);
  CRect rect;
  int i,j,t;
  int k = 0;	//k - № байта в принятом массиве
  BYTE   mask[] =  {0xc0,0x30,0x0c,0x03};
  int n=0;

  TRACE(_T("OnMassiv!!!\n"));
if (bPusk)
{//ИЗМЕРЕНИЕ
// // SetTimer(5,1000,NULL);
if ((!bStop)||//(bStop && stup!=0)||
//	(bStop && (m_irazv==IMP_POS || m_irazv==IMP_NEG) && (m_nCountMas +1) <= m_nMas && (stup+1)<=n_Step))
	(bStop && (m_irazv==IMP_POS || m_irazv==IMP_NEG) && (m_nCountMas +1) <= m_nMas && (stup+1)<=n_Step) && bMeas == MEAS_MULTIPLE)
//	(bStop && (m_irazv==IMP_POS || m_irazv==IMP_NEG)))// && (m_nCountMas +1) <= m_nMas && (stup+1)<=n_Step))

{//%%%

  //переписать данные из буфера bufIn_Epp
  //-------------------------------------
  ::WaitForSingleObject(hMutexEpp,INFINITE);

  memcpy(&My_buf_Mas[0],&bufEppIn[0],NumI);	//данные
  ::ReleaseMutex(hMutexEpp);
  m_nCountMas++;
//		TRACE(_T("cikl=%d\n"),cikl);
        TRACE(_T("stup = %d \n"),stup);
        TRACE(_T("m_nMas = %d \n"),m_nMas);
        TRACE(_T("n_Mas = %d \n"),n_Mas);
        TRACE(_T("m_nCountMas = %d \n"),m_nCountMas);
        TRACE(_T("nCountPointDC = %d \n"),nCountPointDC);

  TRACE(_T("m_nCountMas = %d  My_buf_Mas[0] = %x  My_buf_Mas[1] = %x  My_buf_Mas[2] = %x  My_buf_Mas[3] = %x\n   My_buf_Mas[4] = %x	My_buf_Mas[5] = %x\n   My_buf_Mas[6]= %x  My_buf_Mas[7]= %x\n  My_buf_Mas[8]= %x  My_buf_Mas[9]= %x\n"),m_nCountMas,My_buf_Mas[0],My_buf_Mas[1],My_buf_Mas[2],My_buf_Mas[3],My_buf_Mas[4],My_buf_Mas[5],My_buf_Mas[6],My_buf_Mas[7],My_buf_Mas[8],My_buf_Mas[9]);


//	if (m_udlitpauza==1 && m_irazv!=AC) Sleep(100);	//скважность х 2 вкл.

//	if ((m_irazv!=AC) || (m_irazv==AC && m_nCountMas==2))//24.02.15
	{ 
		if (m_irazv==IMP_POS || m_irazv==IMP_NEG) Sleep(20);//19.06.17
	    ens = _inp(0x79);		//запретить подачу сигнала на объект исследования
		ens = ens & 0xfd;
 		_outp(0x79,ens);	         //D1<-0 (ENB_PCM <- 0)
    TRACE(_T("OnMassiv (ENB_PCM <- 0)\n"));
	}

  if (m_irazv==AC && m_nCountMas==1) Sleep(25);
  
//  if (m_irazv==IMP_POS || m_irazv==IMP_NEG) Sleep(20);//19.06.17
  
  if (m_udlitpauza==1 && m_irazv!=AC) Sleep(100);	//скважность х 2 вкл.

  for (j=0;j<4;j++)		//4 АЦП
     for (t=0;t<NPOINT;t++)
		MAS_RESULT[j][t] = 0;
  // обработка кодированного (реального) массива
  //--------------------------------------------
  int a;
  for (t=0;t<NPOINT;t++)	//t - № точки
  {	//раскладываем одну точку (8 байт (4 + 4)  по 4-м АЦП)
	//байты 1-ый - 4-ый
	//биты  D15 - D8
	for (i=0;i<4;i++)		//i - счётчик байт
	{ 
   	  for (j=0;j<4;j++)		//j - № АЦП
	  { a = My_buf_Mas[k];	//k - № байта в принятом массиве
	    a = a & mask[j];
	    a = a << (j * 2);
		a = a >> (i * 2);
		MAS_RESULT[j][t] = MAS_RESULT[j][t] | a;
	  }
	  k++;
	}
	for (j=0;j<4;j++)
	   MAS_RESULT[j][t] = (MAS_RESULT[j][t]<< 8);	//сдвигаем старший байт
	//байты 5-ый - 8-ой
	//биты  D7 - D0
	for (i=0;i<4;i++)	
	{ 
   	  for (j=0;j<4;j++)
	  { a = My_buf_Mas[k]; 
	    a = a & mask[j];
	    a = a << (j * 2);
		a = a >> (i * 2);
		MAS_RESULT[j][t] = MAS_RESULT[j][t] | a;
	  }
	  k++;
	}
  }//end for t
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //для АЦП2 (Ic) все точки сдвигаем на 1 бит влево
for (t=0;t<NPOINT;t++)	//t - № точки
	   MAS_RESULT[1][t] = (MAS_RESULT[1][t]<<1);	
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //обработка знака
  //---------------
  int tem=0;
  for (i=0;i<4;i++)
  {  for (t=0;t<NPOINT;t++)
		{ tem = MAS_RESULT[i][t] & 0x8000;		//знак
			{ if (tem==0)   { //+  
							  MAS_RESULT[i][t] = MAS_RESULT[i][t] & 0x7fff;
//							  if (MAS_RESULT[i][t]>0x7c50) FlagOL[i]=true;
							}     
					  else  { //-
							  MAS_RESULT[i][t] = MAS_RESULT[i][t] | 0xffff8000;
//							  if (MAS_RESULT[i][t]<0x8000) FlagOL[i]=true;
							} 
			}
		}
  }
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //для АЦП2 (Ic) инверсия знака
  for (t=0;t<NPOINT;t++)	//t - № точки
	   MAS_RESULT[1][t] = MAS_RESULT[1][t]*(-1);	
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //разложить по массивам и умножить на дискрет:
  //t+NPOINT*stup,	где t - № точки, stup - № ступеньки
  for (t=0;t<NPOINT;t++)
  { //АЦП1 - Uc
	    pM1[m_nCountMas-1][t+NPOINT*stup] = (MAS_RESULT[0][t]* koef_kol_U[bias_KolMeasU+m_iKolU])*KMCU[bias_KolMeasU+m_iKolU]-BMCU[bias_KolMeasU+m_iKolU];	
    //АЦП2 - Ic
	    pM2[m_nCountMas-1][t+NPOINT*stup] = (MAS_RESULT[1][t]* koef_kol_I[bias_KolMeasI+m_iKolI])*KMCI[bias_KolMeasI+m_iKolI]-BMCI[bias_KolMeasI+m_iKolI];		
//	    pM2[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[1][t]* koef_kol_I[bias_KolMeasI+m_iKolI])*KMCI[bias_KolMeasI+m_iKolI]-BMCI[bias_KolMeasI+m_iKolI];		
    //АЦП3 - Baza
	if (m_iBazaSource==0 && m_iBazaMeas==1)	//источник U - измеритель I
		pM3[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_I[m_iRangLimitBaza])*KMBI[m_iRangLimitBaza]-BMBI[m_iRangLimitBaza];//диапазон ограничения I		
	if (m_iBazaSource==1 && m_iBazaMeas==0)	//источник I - измеритель U
		pM3[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_U[m_iRangLimitBaza])*KMBU[m_iRangLimitBaza]-BMBU[m_iRangLimitBaza];//диапазон ограничения U		
	if (m_iBazaSource==1 && m_iBazaMeas==1)	//источник I - измеритель I
		pM3[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_I[m_iAmplitudeStepBaza])*KMBI[m_iAmplitudeStepBaza]-BMBI[m_iAmplitudeStepBaza];//диапазон источника I		
	if (m_iBazaSource==0 && m_iBazaMeas==0)	//источник U - измеритель U
		pM3[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_U[m_iAmplitudeStepBaza])*KMBU[m_iAmplitudeStepBaza]-BMBU[m_iAmplitudeStepBaza];//диапазон источника U		
	//АЦП4 - Dop
	if (m_iDopSource==0 && m_iDopMeas==1)	//источник U - измеритель I
		pM4[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_I[m_iRangLimitDop+bias_Dop_I])*KMDI[m_iRangLimitDop]-BMDI[m_iRangLimitDop];//диапазон ограничения I		
	if (m_iDopSource==1 && m_iDopMeas==0)	//источник I - измеритель U
		pM4[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_U[m_iRangLimitDop])*KMDU[m_iRangLimitDop]-BMDU[m_iRangLimitDop];//диапазон ограничения U		
	if (m_iDopSource==1 && m_iDopMeas==1)	//источник I - измеритель I
		pM4[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_I[m_iAmplitudeStepDop+bias_Dop_I])*KMDI[m_iAmplitudeStepDop]-BMDI[m_iAmplitudeStepDop];//диапазон источника I		
	if (m_iDopSource==0 && m_iDopMeas==0)	//источник U - измеритель U
		pM4[m_nCountMas-1][t+NPOINT*stup] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_U[m_iAmplitudeStepDop])*KMDU[m_iAmplitudeStepDop]-BMDU[m_iAmplitudeStepDop];//диапазон источника U		
  }
  //-------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------
 
 if (m_irazv == DC_POS || m_irazv == DC_NEG)
  {
	if (bMeas == MEAS_SINGLE)
	{ 
	  //16 - число усреднений в одной точке
	  //uNpointsLupa - число измеряемых точек (20 или 50)
	  //m_nCountMas - № текущей точки
/*				res_propusk=!res_propusk;
			  if (res_propusk==true) 
			  { 
				  m_nCountMas--;
//				res_propusk=false;
				SetEvent(hEventEppIn);
				return 0L;
			  }	  
*/
		 if (nCountPointDC < n_propusk)//9
		 {//пропустить 3 результата измерения
			nCountPointDC++;
				  m_nCountMas--;
			    // поместить в bufOut управляющие команды
/*			    ::WaitForSingleObject(hMutexO,INFINITE);
			    bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			    bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			    bufOut[2]='\0';		//конец передачи
			    NumO = 2;
			    ::ReleaseMutex(hMutexO);
			    nK41=nK41+1;
			    TRACE(_T("K41 nK41=%d\n"),nK41);
		        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
			    SetEvent(hEventComOut);//+++++++++++++++++++++
*/
				SetEvent(hEventEppIn);//`````````````````````
				return 0L;
		 }
		 else
		 {

//      if (nCountPointDC < uNpointsLupa)
      if (m_nCountMas <= m_nMas)
	  { 
//			nCountPointDC++;
			nCountPointDC=0;
			mM1[stup][m_nCountMas-1] =  0.0;
			mM2[stup][m_nCountMas-1] =  0.0;
			mM3[stup][m_nCountMas-1] =  0.0;
			mM4[stup][m_nCountMas-1] =  0.0;
		  
		//сняли 200 точек
		//для вычисления значения текущей точки усредняем значения точек со 150 до 166
		  for(i=0;i<16;i++)
		  {
		    mM1[stup][m_nCountMas-1] =  mM1[stup][m_nCountMas-1] + pM1[m_nCountMas-1][POINTDC+i+NPOINT*stup];
			mM2[stup][m_nCountMas-1] =  mM2[stup][m_nCountMas-1] + pM2[m_nCountMas-1][POINTDC+i+NPOINT*stup];
			mM3[stup][m_nCountMas-1] =  mM3[stup][m_nCountMas-1] + pM3[m_nCountMas-1][POINTDC+i+NPOINT*stup];
			mM4[stup][m_nCountMas-1] =  mM4[stup][m_nCountMas-1] + pM4[m_nCountMas-1][POINTDC+i+NPOINT*stup];
		  }
			mM1[stup][m_nCountMas-1] =  mM1[stup][m_nCountMas-1] / 16;
			mM2[stup][m_nCountMas-1] =  mM2[stup][m_nCountMas-1] / 16;
			mM3[stup][m_nCountMas-1] =  mM3[stup][m_nCountMas-1] / 16;
			mM4[stup][m_nCountMas-1] =  mM4[stup][m_nCountMas-1] / 16;

			for(j=0;j<n_Step;j++)
		    for (i=0;i<m_nMas;i++)
			{  pM1[0][i+NPOINT*j]= mM1[j][i];
	  	       pM2[0][i+NPOINT*j]= mM2[j][i];
		       pM3[0][i+NPOINT*j]= mM3[j][i];
		       pM4[0][i+NPOINT*j]= mM4[j][i];
			}
	          
//			if (nCountPointDC < uNpointsLupa)
			if (m_nCountMas < m_nMas)
			{
			  SetMassivNpoint(true);//+++	//real massiv
//		   n=array_otcht.GetSize();
			  // поместить в bufOut управляющие команды
			  ::WaitForSingleObject(hMutexO,INFINITE);
			  bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			  bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			  bufOut[2]='\0';		//конец передачи
			  NumO = 2;
			  ::ReleaseMutex(hMutexO);
// 			  nK41=nK41+1;
//			  TRACE(_T("K41 nK41=%d\n"),nK41);
		      PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);	  //---------------------------------------------------
			  SetEvent(hEventComOut);//+++++++++++++++++++++
			  return 0L;
			 }

		}//end of if (nCountPointDC < uNpointsLupa)
		 }
	}//end if DC is SINGLE

	if (bMeas == MEAS_MULTIPLE)
	{ //измерение "DC многократное"
      //if (!bStop): на каждом цикле измерения из 200 (.)-к выбираем одну с № 150, усредняем со150-ой по 166-ю,
      //выводим на график
      //if (bStop): по кнопке Стоп вытираем график
	  if (!bStop)
	  { //1)
/*			  ::WaitForSingleObject(hMutexDCres_gotov,INFINITE);
		
			  if (res_gotov==true) 
			  { 
//				res_gotov=false;
//				n_pointDC++;
				nCountPointDC++;
				if (nCountPointDC==1000) nCountPointDC=0;
			  }
			  ::ReleaseMutex(hMutexDCres_gotov);
				if (m_iLimitKolI==m_iLimitKolU) {nCountPointDC=1;n_pointDC=0;}
*/			  aM1[n_pointDC] =  0.0;
			  aM2[n_pointDC] =  0.0;
			  aM3[n_pointDC] =  0.0;
			  aM4[n_pointDC] =  0.0;

			  for(i=0;i<16;i++)
			  {
		      aM1[n_pointDC] =  aM1[n_pointDC] + pM1[m_nCountMas-1][POINTDC+i+NPOINT*stup];
			  aM2[n_pointDC] =  aM2[n_pointDC] + pM2[m_nCountMas-1][POINTDC+i+NPOINT*stup];
			  aM3[n_pointDC] =  aM3[n_pointDC] + pM3[m_nCountMas-1][POINTDC+i+NPOINT*stup];
			  aM4[n_pointDC] =  aM4[n_pointDC] + pM4[m_nCountMas-1][POINTDC+i+NPOINT*stup];
			  }
			  aM1[n_pointDC] =  aM1[n_pointDC] / 16;
			  aM2[n_pointDC] =  aM2[n_pointDC] / 16;
			  aM3[n_pointDC] =  aM3[n_pointDC] / 16;
			  aM4[n_pointDC] =  aM4[n_pointDC] / 16;


			  n_Result = nCountPointDC;//+1;
			  for (i=0;i<n_Result;i++)
			  {  pM1[0][i+NPOINT*stup]= aM1[i];
	  	         pM2[0][i+NPOINT*stup]= aM2[i];
		         pM3[0][i+NPOINT*stup]= aM3[i];
		         pM4[0][i+NPOINT*stup]= aM4[i];
			  }
			  ::WaitForSingleObject(hMutexDCres_gotov,INFINITE);
		
			  if (res_gotov==true) 
			  { 
/*				n_propusk++;
				if (n_propusk<3)//3
				{ SetEvent(hEventEppIn);
				  return 0L;
				}
				else
				{
*/				res_gotov=false;
				n_pointDC++;
				nCountPointDC++;
				if (m_iLimitKolI==m_iLimitKolU) {nCountPointDC=1;n_pointDC=0;}
				if (nCountPointDC==1000) {nCountPointDC=1;n_pointDC=0;}
//				}
			  }
			  ::ReleaseMutex(hMutexDCres_gotov);
		  

	  }//1)
	  else 
	  { //if (bStop)
//		nCountPointDC = 0;
//			  n_Result = nCountPointDC+1;
	  }
	}//end of if DC is MULTIPLE
//  }


 }//end of DC

  //-------------------------------------------------------------------------------
  if (m_irazv == AC)
  {//АС - I вариант (2 массива по 200 точек)
	if (m_nCountMas < m_nMas)
	{//1-ый массив  (положительная ветвь синусоиды)	 //из 200 точек делаем 100
		for (i=0;i<NPOINT;i++)
		{ bM1[i] = 0;
		  bM2[i] = 0;
		  bM3[i] = 0;
		  bM4[i] = 0;
		}//end for i
		j = 0;
		for (i=0;i<NPOINT100;i++)
		{ bM1[i] = pM1[m_nCountMas-1][j+NPOINT*stup];
		  bM2[i] = pM2[m_nCountMas-1][j+NPOINT*stup];
		  bM3[i] = pM3[m_nCountMas-1][j+NPOINT*stup];
		  bM4[i] = pM4[m_nCountMas-1][j+NPOINT*stup];
		  j=j+2;
		}//end for i
	 //пошли читать 2-ой массив

//		SetMassivNpoint(true);//+++	//real massiv
	  
		TRACE(_T("AC - 1)\n"));
	 SetEvent(hEventEppIn);			//`````````````````````
	 return 0L;
	}//end of 1-ый массив
	if (m_nCountMas == m_nMas)
	{//2-ой массив из 200 точек делаем 100
		for (i=0;i<NPOINT;i++)
		{ cM1[i] = 0;
		  cM2[i] = 0;
		  cM3[i] = 0;
		  cM4[i] = 0;
		}//end for i
		j = 0;
		for (i=0;i<NPOINT100;i++)
		{ cM1[i] = pM1[m_nCountMas-1][j+NPOINT*stup];//pM1[i+NPOINT*stup];//
		  cM2[i] = pM2[m_nCountMas-1][j+NPOINT*stup];//pM2[i+NPOINT*stup];//
		  cM3[i] = pM3[m_nCountMas-1][j+NPOINT*stup];//pM3[i+NPOINT*stup];//
		  cM4[i] = pM4[m_nCountMas-1][j+NPOINT*stup];//pM4[i+NPOINT*stup];//
		  j=j+2;
		}//end for i
	 //из двух массивов делаем один
		j = 50;
		for(i=0;i<50;i++)
		{ pM1[0][i+NPOINT*stup] = cM1[j];
		  pM2[0][i+NPOINT*stup] = cM2[j];
		  pM3[0][i+NPOINT*stup] = cM3[j];
		  pM4[0][i+NPOINT*stup] = cM4[j];
		  j=j+1;}
		j = 0;
		for(i=50;i<100;i++)
		{ pM1[0][i+NPOINT*stup] = bM1[j];
		  pM2[0][i+NPOINT*stup] = bM2[j];
		  pM3[0][i+NPOINT*stup] = bM3[j];
		  pM4[0][i+NPOINT*stup] = bM4[j];
		  j=j+1;}
		j = 50;
		for(i=100;i<150;i++)
		{ pM1[0][i+NPOINT*stup] = bM1[j];
		  pM2[0][i+NPOINT*stup] = bM2[j];
		  pM3[0][i+NPOINT*stup] = bM3[j];
		  pM4[0][i+NPOINT*stup] = bM4[j];
		  j=j+1;}
		j = 0;
		for(i=150;i<NPOINT;i++)
		{ pM1[0][i+NPOINT*stup] = cM1[j];
		  pM2[0][i+NPOINT*stup] = cM2[j];
		  pM3[0][i+NPOINT*stup] = cM3[j];
		  pM4[0][i+NPOINT*stup] = cM4[j];
		  j=j+1;}

	  TRACE(_T("AC - 2 )\n"));
	}//end of 2-ой массив
  }//end of AC

//---------------------------------------------------
//----------------------------------------------------

//---------------------------------------------------
//----------------------------------------------------
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG)
  {//Импульсная развёртка 
		UINT m=0;
 	if (m_nCountMas < m_nMas)
	{
		//отрабатываем очередной импульс
		for(i=0;i<NPOINT;i++)
		{ mM1[m_nCountMas-1][i+NPOINT*stup] = pM1[m_nCountMas-1][i+NPOINT*stup];
		  mM2[m_nCountMas-1][i+NPOINT*stup] = pM2[m_nCountMas-1][i+NPOINT*stup];
	      mM3[m_nCountMas-1][i+NPOINT*stup] = pM3[m_nCountMas-1][i+NPOINT*stup];
	      mM4[m_nCountMas-1][i+NPOINT*stup] = pM4[m_nCountMas-1][i+NPOINT*stup];}
		//из каждого массива из NPOINT точек берём n_t-последнюю (NPOINT+NPOINT*stup-n_t)
		//и помещаем её на место первой для текущей ступени
//	    for(m=0;m<m_nCountMas;m++)
	    for(m=0;m<m_nMas;m++)
		{ pM1[m][NPOINT*stup]=mM1[m][NPOINT+NPOINT*stup-n_t];
	      pM2[m][NPOINT*stup]=mM2[m][NPOINT+NPOINT*stup-n_t];
		  pM3[m][NPOINT*stup]=mM3[m][NPOINT+NPOINT*stup-n_t];
		  pM4[m][NPOINT*stup]=mM4[m][NPOINT+NPOINT*stup-n_t];}

		SetMassivNpoint(true);//+++	//real massiv

		// поместить в bufOut управляющие команды : отработать следующий импульс (ступеньку)
//		if (bMeas==MEAS_SINGLE)
		{
		::WaitForSingleObject(hMutexO,INFINITE);
		bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
		bufOut[1]=K41[0];	//команда "Cледующая ступенька"
        bufOut[2]='\0';		//конец передачи
		NumO = 2;
        ::ReleaseMutex(hMutexO);
// 		nK41=nK41+1;
		TRACE(_T("K41 nK41=%d\n"),nK41);
       PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);	  //---------------------------------------------------
        SetEvent(hEventComOut);//+++++++++++++++++++++
	    return 0L;
		}
/*		else
		{
	      //пошли читать очередной массив

		::WaitForSingleObject(hMutexO,INFINITE);
		bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
		bufOut[1]=K41[0];	//команда "Cледующая ступенька"
        bufOut[2]='\0';		//конец передачи
		NumO = 2;
//		nK41=nK41+1;
//		TRACE(_T("K41 nK41=%d\n"),nK41);
        ::ReleaseMutex(hMutexO);
        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
        SetEvent(hEventComOut);//+++++++++++++++++++++
	      return 0L;
		}
*/	}
	if (m_nCountMas == m_nMas)//%%%%%
// 	if (m_nCountMas == n_Mas)
	{ 
	  //отработали все импульсы 
		for(i=0;i<NPOINT;i++)
		{ mM1[m_nCountMas-1][i+NPOINT*stup] = pM1[m_nCountMas-1][i+NPOINT*stup];
		  mM2[m_nCountMas-1][i+NPOINT*stup] = pM2[m_nCountMas-1][i+NPOINT*stup];
	      mM3[m_nCountMas-1][i+NPOINT*stup] = pM3[m_nCountMas-1][i+NPOINT*stup];
	      mM4[m_nCountMas-1][i+NPOINT*stup] = pM4[m_nCountMas-1][i+NPOINT*stup];}
	    
	    //из каждого массива из NPOINT точек берём n_t-последнюю (NPOINT+NPOINT*stup-n_t)
		//и помещаем её на место первой для текущей ступени
	    for(m=0;m<m_nMas;m++)
//	    for(m=0;m<m_nCountMas;m++)
		{ pM1[m][NPOINT*stup]=mM1[m][NPOINT+NPOINT*stup-n_t];
	      pM2[m][NPOINT*stup]=mM2[m][NPOINT+NPOINT*stup-n_t];
		  pM3[m][NPOINT*stup]=mM3[m][NPOINT+NPOINT*stup-n_t];
		  pM4[m][NPOINT*stup]=mM4[m][NPOINT+NPOINT*stup-n_t];}
//			SetMassivNpoint(true);//+++	//real massiv
	 }

  }//end of TIP_IMP

//---------------------------------------------------

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  //"Компенсация петли"
  if (m_bLooping && !Flag_sboy) LoopingCompensation();
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//---------------------------------------------------
/*  if (m_nCountMas == m_nMas)
  { // отобразить график
	m_nCountMas = 0;
    SetMassivNpoint(true);	//real massiv
  }
  // перейти к очередной ступеньке
  stup++;
  if (stup==n_Step)	stup=0;
  TRACE(_T("stup = %d \n"),stup);
*/
  //---------------------------------------------------
}//%%%
  ::WaitForSingleObject(hMutexStop,INFINITE);
if (!bStop)
{//0
	::ReleaseMutex(hMutexStop);
	if (bMeas == MEAS_MULTIPLE)
	{ //измерение многократное
	  if (m_nCountMas >= m_nMas)
//	  if (m_nCountMas == n_Mas)
	  { // отобразить график
		m_nCountMas = 0;
		SetMassivNpoint(true);//+++	//real massiv
        // перейти к очередной ступеньке
        stup++;
		if (stup==n_Step)
		{ //конец полного цикла измерения
//			cikl++;
//			TRACE(_T("cikl=%d\n"),cikl);
		  stup=0; 
//		  m_nCountMas = 0;
		}
//       TRACE(_T("stup = %d \n"),stup);
		::WaitForSingleObject(hMutexO,INFINITE);
		bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
		bufOut[1]=K41[0];	//команда "Cледующая ступенька"
        bufOut[2]='\0';		//конец передачи
		NumO = 2;
//		nK41=nK41+1;
//		TRACE(_T("stup= %d\n"),stup);
		TRACE(_T("K41\n"));
        ::ReleaseMutex(hMutexO);
        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
        SetEvent(hEventComOut);//+++++++++++++++++++++
	  }
	  //---------------------------------------------------
	} //конец измерения многократного
	else
	{//1 //измерение однократное
	  if (stup<n_Step-1 && bGenerationStup)
	  {	// поместить в bufOut управляющие команды : отработать ступеньку
		::WaitForSingleObject(hMutexO,INFINITE);
		bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
		bufOut[1]=K41[0];	//команда "Cледующая ступенька"
        bufOut[2]='\0';		//конец передачи
		NumO = 2;
// 		nK41=nK41+1;
		TRACE(_T("K41 nK41=%d\n"),nK41);
       ::ReleaseMutex(hMutexO);
        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
        SetEvent(hEventComOut);//+++++++++++++++++++++
	  //---------------------------------------------------
		if (m_nCountMas >= m_nMas)
//		if (m_nCountMas == m_nMas)
		{ // отобразить график
			m_nCountMas = 0;
			SetMassivNpoint(true);	//real massiv
        // перейти к очередной ступеньке
        stup++;
      if (stup==n_Step)
		{ stup=0; 
		}
//        TRACE(_T("stup = %d \n"),stup);
		}
	  //---------------------------------------------------
	  }
	  else
	  {//^^^
//		if (m_nCountMas == m_nMas)
//		{ // отобразить график
//			m_nCountMas = 0;
//			SetMassivNpoint(true);//+++	//real massiv
			SetMassiv(true);			//вывод окончательного графика и поиск измеренного параметра 
		  UpdateWindow();
//			// перейти к очередной ступеньке
//        stup++;
//        if (stup==n_Step)	stup=0;
//        TRACE(_T("stup = %d \n"),stup);
//		}
	  //---------------------------------------------------
		m_uCountTest++;
		//сохранить результаты измерения
		SaveResults();
		//Измерение разности ВАХ?
		if (bStatusDiff)	{ CalculateDifference();
							  SetMassivDiffNpoints();
							}
		//Измерение вычитаемой Diff?
		if (bMeasDiff)		{//да
							  m_uFunc = 0; //игнорировать параметр, его расчет, проверку и выдачу графика								
							  bMeasDiff = false;
							  bStatusDiff = true;
							}
		else
		{
		if (!m_sPar.IsEmpty() && m_uFunc > 1  && !Flag_sboy) Calculate();// рассчитать рассчитываемый массив
		
		// проверить параметр на допуск
		ControlPar();
		}
		
		if (m_uCountTest==n_Test && n_Test==1)
		{//3-2 //измерение закончено
//		  n=array_otcht.GetSize();
		  InitTablRes();//!!!
//		  n=array_otcht.GetSize();
		  if (!Flag_sboy)
		  {//if (!Flag_sboy)
//		  if (m_uFunc == 2 && !bZapusk)
		  if (m_uFunc >1  && !bZapusk)//&&  m_uFunc < 5
		  { if (MessageBox(_T("Отобразить расчетный график?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
			{ 
//22.12.15              m_iButNull=CENTER;
	          UpdateData(false);
			  InitAxisCalc();
			  SetMassivCalcNpoints();	//вывод расчетного графика и рассчет параметра 
			  str_PRIM = _T("");
			}
		  }
//		// проверить параметр на допуск
//		ControlPar();
		  
		  }//end of if (!Flag_sboy)

		// отобразить отчёт по тесту
		Otcht();			//подготовка отчета по тесту
		DisplayOtcht();		//индикация отчета

		// однократное измерение закончено
		// автоматическое сохранение таблицы результатов измерения
		if  (bSound) AutoSaveTablResult();

		FinishControlFlags();
		  ::WaitForSingleObject(hMutexStop,INFINITE);
		  bPusk = false;
		  bStop = true;
		  TRACE(_T("bPusk = false\n"));
		  ::ReleaseMutex(hMutexStop);
		  EndWaitCursor();	//Измерение закончено
		pbut -> SetWindowText(L"Измерение");
		
		}//2
	    else
		{//3 измерение однократное, режим "Автомат"
		  InitTablRes();
		// проверить параметр на допуск
		ControlPar();
		// сохранить результаты расчёта
//		SaveResultsCalculate();
		// отобразить отчёт по тесту
		Otcht();			//подготовка отчета по тесту
		DisplayOtcht();		//индикация отчета

		// автоматическое сохранение результатов измерения
			if  (bSound) AutoSaveTablResult();
		  // перейти к очередному тесту
	      i_MOD++;
		  Flag_All = Flag_All | Flag;
		  if (m_masStrMod[i_EL][i_MOD]==_T(""))
		  {//4 //измерение однократное, режим "Автомат" закончено
            ::WaitForSingleObject(hMutexStop,INFINITE);
            bPusk = false;
	        bStop = true;
			::ReleaseMutex(hMutexStop);
		
			if (Flag_All) 
			{
			  str = _T("ИЗМЕРЕНИЕ ЗАКОНЧЕНО! ЭЛЕМЕНТ НЕ ГОДЕН!");
		      SetTextInfo(str);
			  OutLimit(0);//выдать сигнал "ЭЛЕМЕНТ НЕ ГОДЕН!"
			}
		    else FinishControlFlags();
		   //восстановить i_MOD и pModSelected
		    i_MOD=0;
			Flag = false;
			Flag_All = false;
			if (m_bFileOpen)
			{
		    pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
		    ExtractParam();
			}
	        UpdateData(FALSE);		//отобразить параметры в окошках
		    EndWaitCursor();	//Измерение закончено
		    pbut -> SetWindowText(L"Измерение");
		  }//4
          else
		  {//5 //поиск очередного теста
		    pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
            if (pModSelected!=NULL)
			{//6 //экстракция параметров
			  if (!ControlSwitch())
			  {//6.1
/*			      ExtractParam();		
		 UpdateData(FALSE);		//отобразить параметры в окошках
		      str_EL   = pModSelected->m_EL;
		      str_MOD  = pModSelected->m_MOD;
		      str_PRIM = pModSelected->m_PRIM;
				  Flag  = false;
//				  Flag_All = false;

			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала

//			InitGnezdo();
			InitIconImp();
*/
			ExtractParam();		//экстракция параметров
//			UpdateData(FALSE);		//отобразить параметры в окошках
			str_EL   = pModSelected->m_EL;
			str_MOD  = pModSelected->m_MOD;
			str_PRIM = pModSelected->m_PRIM;
				  Flag  = false;
			strTestName = str_EL+_T(" \\ ")+str_MOD;	//_T(" Тест:  ")+текущая модель измерения (выбранный тест)
			SetTextInfo(_T(" TEST:  ")+strTestName);//отобразить выбранный тест в информационном окне
//			InitGnezdo();	//4.12.17
		  SetStringsSourceMeas();//@ 11.12.17	
			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала
			UpdateData(FALSE);		//отобразить параметры в окошках
//			SetTest();
				  int nTab = 0;			//отобразить закладку "Схема включения"
			 m_TabCtrlParam.SetCurSel(nTab);
			 InitPage(nTab);

			 b_InitAxis=true;


		      // провести контроль параметров
				  
		      if (!ControlParam()) Pusk();	//запуск очередного теста
		      else
			  {//7 
                ::WaitForSingleObject(hMutexStop,INFINITE);
				bPusk = FALSE;
		        bStop = TRUE;
				EndWaitCursor();	//Измерение закончено
		        pbut -> SetWindowText(L"Измерение");
			    ::ReleaseMutex(hMutexStop);
		        str.Format(_T("ERROR 203: В параметрах теста %s \\ %s обнаружены ошибки."),str_EL,str_MOD);
		        SetTextInfo(str);
	int  nTabTuning = 1;//Error//0
	m_TabCtrlTuning.SetCurSel(nTabTuning);
	InitPageTuning(nTabTuning);	// отобразить панель "Дисплей"
//				Stop();	
//				ExtraStop(1,str);//экстренный останов
		        //восстановить i_MOD и pModSelected
		        i_MOD=0;
				  Flag  = false;
				  Flag_All = false;
		        pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
			    ExtractParam();
		        UpdateData(FALSE);		//отобразить параметры в окошках
			  }//7
			  }//6.1
			  else
			  {
               ::WaitForSingleObject(hMutexStop,INFINITE);
				bPusk = FALSE;
		        bStop = TRUE;
				EndWaitCursor();	//Измерение закончено
		        pbut -> SetWindowText(L"Измерение");
			    ::ReleaseMutex(hMutexStop);
		        str.Format(_T("ERROR 203: В параметрах теста %s \\ %s обнаружены ошибки."),str_EL,str_MOD);
		        SetTextInfo(str);
	int  nTabTuning = 1;//Error//0
	m_TabCtrlTuning.SetCurSel(nTabTuning);
	InitPageTuning(nTabTuning);	// отобразить панель "Дисплей"
//				Stop();	
//				ExtraStop(1,str);//экстренный останов
		        //восстановить i_MOD и pModSelected
		        i_MOD=0;
				  Flag  = false;
				  Flag_All = false;
		        pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
			    ExtractParam();
		 UpdateData(FALSE);		//отобразить параметры в окошках
			  }
			}//6//конец запуска очередного теста в режиме "Автомат"
		  }//5//конец поиска очередного теста
		}//3//конец перехода к очередному тесту
	  }//^^^
	}//1//конец измерения однократного
}//0//end of if (!bStop)
else
{ //была нажата кнопка "Стоп"
  ::ReleaseMutex(hMutexStop);
  if (bMeas == MEAS_MULTIPLE)
  { //измерение "многократное"
/*	  if (stup!=0 && bGenerationStup)
	  {	// поместить в bufOut управляющие команды : отработать ступеньку
		::WaitForSingleObject(hMutexO,INFINITE);
		bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
		bufOut[1]=K41[0];	//команда "Cледующая ступенька"
        bufOut[2]='\0';		//конец передачи
       ::ReleaseMutex(hMutexO);
		nK41=nK41+1;
 		TRACE(_T("K41-STOP\n"));
        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
        SetEvent(hEventComOut);//+++++++++++++++++++++
	  //---------------------------------------------------
		if (m_nCountMas == m_nMas)
		{ // отобразить график
			m_nCountMas = 0;
			SetMassivNpoint(true);//+++	//real massiv
//		    ::WaitForSingleObject(hMutexRukoyatka,INFINITE);
//			if (m_itipimp==TIP_IMP) m_nMas=m_iPosTek/5;
//			::ReleaseMutex(hMutexRukoyatka);
        // перейти к очередной ступеньке
        stup++;
        if (stup==n_Step)
		{ stup=0; 
		  if (nCountPointDC==1000) nCountPointDC = 0;
		}
        TRACE(_T("stup = %d \n"),stup);
		}
	  //---------------------------------------------------
	  }
	  else
	  {//&&&
	  //---------------------------------------------------
//		if (m_irazv!=DC_POS && m_irazv!=DC_NEG)
//		{
//		  if (m_nCountMas == m_nMas)
//		  { // отобразить график
//			m_nCountMas = 0;
//			SetMassivNpoint(true);//+++	//real massiv
*/			SetMassiv(true);//+++	//real massiv
//            // перейти к очередной ступеньке
//            stup++;
//            if (stup==n_Step)	stup=0;
            TRACE(_T("stup = %d \n"),stup);
//		  }
//		}
	  //---------------------------------------------------
		m_uCountTest++;
		//сохранить результаты измерения
		SaveResults();
		//Измерение разности ВАХ?
		if (bStatusDiff)	{ CalculateDifference();
							  SetMassivDiffNpoints();
							}
		//Измерение вычитаемой Diff?
		if (bMeasDiff)		{//да
							  m_uFunc=0;//игнорировать параметр, его расчет, проверку и выдачу графика
							  bMeasDiff = false;
							  bStatusDiff = true;
							}
		else
		{
		//рассчитать параметр
		if (!m_sPar.IsEmpty() && m_uFunc > 1 && !Flag_sboy) Calculate();	//рассчитать рассчитываемый массив
  		 
		// проверить параметр на допуск
		ControlPar();
		}
		
		InitTablRes();//+++
		
		if (!Flag_sboy)
		  {//if (!Flag_sboy)

			if (m_uFunc > 1 && !bZapusk)
		  { if (MessageBox(_T("Отобразить расчетный график?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
			{ 
 //22.12.15             m_iButNull=CENTER;
	          UpdateData(false);
			  InitAxisCalc();
			  SetMassivCalcNpoints();
			  str_PRIM=_T("");
			}
		  }
//		//проверить параметр на допуск
//		ControlPar();
		//сохранить результаты расчёта
//		SaveResultsCalculate();

		}//end of if (!Flag_sboy)
		
		//отобразить отчёт по тесту 
		Otcht();			//подготовка отчета по тесту
		DisplayOtcht();		//индикация отчета
	    //обнулить состояние рукоятки
	   ::WaitForSingleObject(hMutexRukoyatka,INFINITE);
	    //m_iPosTek = 0;
		m_iPosTek = m_iLimitKolI;//обнулить состояние рукоятки (задать равным старту)
		m_iPosTekOld = m_iPosTek;
	   ::ReleaseMutex(hMutexRukoyatka);
	    DrawStatusRukoyatka();//обнулить состояние рукоятки

		// многократное измерение закончилось 
		// автоматическое сохранение результатов измерения
		if  (bSound) AutoSaveTablResult();
				
		FinishControlFlags();
		  bPusk = false;
		  Flag  = false;
		  Flag_All = false;
		  EndWaitCursor();	//Измерение закончено
		pbut -> SetWindowText(L"Измерение");
//	  }//&&&
  }
  else 
  { // измерение "однократное"
	//ИЗМЕРЕНИЕ ПРЕРВАНО НАЖАТИЕМ КНОПКИ \"СТОП\"!

	InitTablRes();
	FinishControlFlags();
  if (hEventEppIn) SetEvent(hEventStop);

  if (Flag_sboy==0)
	{
    str = _T("ИЗМЕРЕНИЕ ПРЕРВАНО НАЖАТИЕМ КНОПКИ \"СТОП\"!");
    SetTextInfo(str);
	}
    bPusk = false;
	bStop = true;
    Flag  = false;
    Flag_All = false;
		pbut -> SetWindowText(L"Измерение");
//	SetEvent(hEventStop);

  }
}//end if была нажата кнопка "Стоп"
}//end if (bPusk)

if (bAutoSetNull)
{//КОРРЕКЦИЯ СМЕЩЕНИЯ  НУЛЯ
  //переписать данные из буфера bufIn_Epp
  //-------------------------------------
  ::WaitForSingleObject(hMutexEpp,INFINITE);

  memcpy(&My_buf_Mas[0],&bufEppIn[0],NumI);	//данные
  ::ReleaseMutex(hMutexEpp);
  m_nCountMas++;

  TRACE(_T("m_nCountMas = %d  My_buf_Mas[0] = %x  My_buf_Mas[1] = %x  My_buf_Mas[2] = %x  My_buf_Mas[3] = %x\n   My_buf_Mas[4] = %x	My_buf_Mas[5] = %x\n   My_buf_Mas[6]= %x  My_buf_Mas[7]= %x\n  My_buf_Mas[8]= %x  My_buf_Mas[9]= %x\n"),m_nCountMas,My_buf_Mas[0],My_buf_Mas[1],My_buf_Mas[2],My_buf_Mas[3],My_buf_Mas[4],My_buf_Mas[5],My_buf_Mas[6],My_buf_Mas[7],My_buf_Mas[8],My_buf_Mas[9]);

//   if (m_ir==0) Sleep(20);		//задержка сброса ENS для импульсной развертки

	    ens = _inp(0x79);		//запретить подачу сигнала на объект исследования
		ens = ens & 0xfd;
 		_outp(0x79,ens);	         //D1<-0 (ENB_PCM <- 0)
 
	if (m_ir==0) Sleep(20);		//задержка сброса ENS для импульсной развертки

  for (j=0;j<4;j++)		//4 АЦП
     for (t=0;t<NPOINT;t++)
		MAS_RESULT[j][t] = 0;
  // обработка кодированного (реального) массива
  //--------------------------------------------
  int a;
  for (t=0;t<NPOINT;t++)	//t - № точки
  {	//раскладываем одну точку (8 байт (4 + 4)  по 4-м АЦП)
	//байты D15 - D14
	for (i=0;i<4;i++)		//i - счётчик байт
	{ 
   	  for (j=0;j<4;j++)		//j - № АЦП
	  { a = My_buf_Mas[k];	//k - № байта в принятом массиве
	    a = a & mask[j];
	    a = a << (j * 2);
		a = a >> (i * 2);
		MAS_RESULT[j][t] = MAS_RESULT[j][t] | a;
	  }
	  k++;
	}
	for (j=0;j<4;j++)
	   MAS_RESULT[j][t] = (MAS_RESULT[j][t]<< 8);	//сдвигаем старший байт
	// байты D7 - D0
	for (i=0;i<4;i++)	
	{ 
   	  for (j=0;j<4;j++)
	  { a = My_buf_Mas[k]; 
	    a = a & mask[j];
	    a = a << (j * 2);
		a = a >> (i * 2);
		MAS_RESULT[j][t] = MAS_RESULT[j][t] | a;
	  }
	  k++;
	}
  }//end for t
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //для АЦП2 (Ic) все точки сдвигаем на 1 бит влево
  for (t=0;t<NPOINT;t++)	//t - № точки
	   MAS_RESULT[1][t] = (MAS_RESULT[1][t]<<1);	
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //обработка знака
  //---------------
  int tem=0;
  for (i=0;i<4;i++)
  {  for (t=0;t<NPOINT;t++)
		{ tem = MAS_RESULT[i][t] & 0x8000;		//знак
			{ if (tem==0)   { //+  
							  MAS_RESULT[i][t] = MAS_RESULT[i][t] & 0x7fff;
							  if (MAS_RESULT[1][t]>=0x7ff0) 
							  { FlagOL[1]=true;
							  //  MAS_RESULT[1][t]=0x7ff0;
							  }
							}     
					  else  { //-
							  MAS_RESULT[i][t] = MAS_RESULT[i][t] | 0xffff8000;
							  if (MAS_RESULT[1][t]<0x800f) 
							  { FlagOL[1]=true;
							  //  MAS_RESULT[1][t]=0x8000;
							  }
							} 
			}
		}
  }
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //для АЦП2 (Ic) инверсия знака
  for (t=0;t<NPOINT;t++)	//t - № точки
	   MAS_RESULT[1][t] = MAS_RESULT[1][t]*(-1);	
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //разложить по массивам и умножить на дискрет:
  //t - № точки 
  for (t=0;t<NPOINT;t++)
  { //АЦП1 - Uc
	    pM1[m_nCountMas-1][t] = (MAS_RESULT[0][t]* koef_kol_U[bias_Uc+m_iUc])*KMCU[bias_Uc+m_iUc]-BMCU[bias_Uc+m_iUc];	
    //АЦП2 - Ic
        pM2[m_nCountMas-1][t] = (MAS_RESULT[1][t]* koef_kol_I[bias_Ic+m_iIc])*KMCI[bias_Ic+m_iIc]-BMCI[bias_Ic+m_iIc];		
//	    pM2[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[1][t]* koef_kol_I[bias_Ic+m_iIc])*KMCI[bias_Ic+m_iIc]-BMCI[bias_Ic+m_iIc];		
    //АЦП3 - Baza
	if (m_iBs==0 && m_iBm==1)	//источник U - измеритель I
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_I[m_iRb])*KMBI[m_iRb]-BMBI[m_iRb];//диапазон ограничения I		
	if (m_iBs==1 && m_iBm==0)	//источник I - измеритель U
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_U[m_iRb])*KMBU[m_iRb]-BMBU[m_iRb];//диапазон ограничения U		
	if (m_iBs==1 && m_iBm==1)	//источник I - измеритель I
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_I[m_iAb])*KMBI[m_iAb]-BMBI[m_iAb];//диапазон источника I		
	if (m_iBs==0 && m_iBm==0)	//источник U - измеритель U
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_U[m_iAb])*KMBU[m_iAb]-BMBU[m_iAb];//диапазон источника U		
	//АЦП4 - Dop
	if (m_iDs==0 && m_iDm==1)	//источник U - измеритель I
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_I[m_iRd+bias_Id])*KMDI[m_iRd]-BMDI[m_iRd];//диапазон ограничения I		
	if (m_iDs==1 && m_iDm==0)	//источник I - измеритель U
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_U[m_iRd])*KMDU[m_iRd]-BMDU[m_iRd];//диапазон ограничения U		
	if (m_iDs==1 && m_iDm==1)	//источник I - измеритель I
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_I[m_iAd+bias_Id])*KMDI[m_iAd]-BMDI[m_iAd];//диапазон источника I		
	if (m_iDs==0 && m_iDm==0)	//источник U - измеритель U
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_U[m_iAd])*KMDU[m_iAd]-BMDU[m_iAd];//диапазон источника U		
  }
  //-------------------------------------------------------------------------------

  if (m_ir==1)//коррекция нуля
  {//DC (20 раз читаем результат, первые 4 пропускаем, остальные 16 усредняем=получаем 1 точку
		 if (nCountPointDC < 4)
		 {//пропустить 4 результата измерения
			nCountPointDC++;
			m_nCountMas=0;
			    // поместить в bufOut управляющие команды
			    ::WaitForSingleObject(hMutexO,INFINITE);
			    bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			    bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			    bufOut[2]='\0';		//конец передачи
			    NumO = 2;
			    ::ReleaseMutex(hMutexO);
			    nK41=nK41+1;
			    TRACE(_T("K41 nK41=%d\n"),nK41);
		        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
			    SetEvent(hEventComOut);//+++++++++++++++++++++

//				SetEvent(hEventEppIn);//`````````````````````
				return 0L;
		 }
		 else
		 {
			if (nCountPointDC < 19)//(4-пропущено + 16-усредняем = 20)
			{
			  aM1[0] =  aM1[0] + pM1[m_nCountMas-1][POINTDC];
			  aM2[0] =  aM2[0] + pM2[m_nCountMas-1][POINTDC];
			  aM3[0] =  aM3[0] + pM3[m_nCountMas-1][POINTDC];
			  aM4[0] =  aM4[0] + pM4[m_nCountMas-1][POINTDC];
			  nCountPointDC++;
			  m_nCountMas=0;
			    // поместить в bufOut управляющие команды
			    ::WaitForSingleObject(hMutexO,INFINITE);
			    bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			    bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			    bufOut[2]='\0';		//конец передачи
			    NumO = 2;
			    ::ReleaseMutex(hMutexO);
 			    nK41=nK41+1;
			    TRACE(_T("K41 nK41=%d\n"),nK41);
		        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
			    SetEvent(hEventComOut);//+++++++++++++++++++++

//				SetEvent(hEventEppIn);//`````````````````````
             return 0L;
			}
			else
			{
			  aM1[0] =  aM1[0] + pM1[m_nCountMas-1][POINTDC];
			  aM2[0] =  aM2[0] + pM2[m_nCountMas-1][POINTDC];
			  aM3[0] =  aM3[0] + pM3[m_nCountMas-1][POINTDC];
			  aM4[0] =  aM4[0] + pM4[m_nCountMas-1][POINTDC];
			  aM1[0] =  aM1[0] / 16;
			  aM2[0] =  aM2[0] / 16;
			  aM3[0] =  aM3[0] / 16;
			  aM4[0] =  aM4[0] / 16;//результат в aM1[0],aM2[0],aM3[0],aM4[0]
				nCountPointDC=0;
			  m_nCountMas=0;
			}
		  }
  }//end if m_ir==1 (DC)

  if (m_ir==0)//коррекция нуля
  {//Импульсная развёртка 
	//20 раз читаем результат из 5-ти точек, берем n_t-ю точку с конца
	//первые 4 измерения пропускаем, остальные 16 усредняем по 16-ти
		 if (nCountPointDC < 4)
		 {//пропустить 4 результата измерения
			nCountPointDC++;
			m_nCountMas=0;
			    // поместить в bufOut управляющие команды
			    ::WaitForSingleObject(hMutexO,INFINITE);
			    bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			    bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			    bufOut[2]='\0';		//конец передачи
			    NumO = 2;
			    ::ReleaseMutex(hMutexO);
// 			    nK41=nK41+1;
//			    TRACE(_T("K41 nK41=%d\n"),nK41);
		        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
			    SetEvent(hEventComOut);//+++++++++++++++++++++

//				SetEvent(hEventEppIn);//`````````````````````
			    return 0L;
		 }
		 else
		 {
//
			if (nCountPointDC < 19)//(4-пропущено + 16-усредняем = 20)
			{
			  aM1[0] =  aM1[0] + pM1[m_nCountMas-1][NPOINT-n_t];
			  aM2[0] =  aM2[0] + pM2[m_nCountMas-1][NPOINT-n_t];
			  aM3[0] =  aM3[0] + pM3[m_nCountMas-1][NPOINT-n_t];
			  aM4[0] =  aM4[0] + pM4[m_nCountMas-1][NPOINT-n_t];

			  nCountPointDC++;
			  m_nCountMas=0;
			    // поместить в bufOut управляющие команды
			    ::WaitForSingleObject(hMutexO,INFINITE);
			    bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			    bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			    bufOut[2]='\0';		//конец передачи
			    NumO = 2;
			    ::ReleaseMutex(hMutexO);
// 			    nK41=nK41+1;
//			    TRACE(_T("K41 nK41=%d\n"),nK41);
		        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
			    SetEvent(hEventComOut);//+++++++++++++++++++++

//				SetEvent(hEventEppIn);//`````````````````````
			    return 0L;
			}
			else
			{
			  aM1[0] =  aM1[0] + pM1[m_nCountMas-1][NPOINT-n_t];
			  aM2[0] =  aM2[0] + pM2[m_nCountMas-1][NPOINT-n_t];
			  aM3[0] =  aM3[0] + pM3[m_nCountMas-1][NPOINT-n_t];
			  aM4[0] =  aM4[0] + pM4[m_nCountMas-1][NPOINT-n_t];
			  aM1[0] =  aM1[0] / 16;
			  aM2[0] =  aM2[0] / 16;
			  aM3[0] =  aM3[0] / 16;
			  aM4[0] =  aM4[0] / 16;//результат в aM1[0],aM2[0],aM3[0],aM4[0]

			  nCountPointDC=0;
			  m_nCountMas=0;
			}
		  }
  }//end if m_ir==0

  int k=0;
  switch(m_iNset)
  { 
	  case 0: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИЗМЕРИТЕЛЯ Ic");
			  k = AutoSetupNullIc();
			  break;
	  case 10: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИЗМЕРИТЕЛЯ Ic");
			  k = AutoSetupNullIc_10nA(m_iRange);
			  break;
	  case 1: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИЗМЕРИТЕЛЯ Uc");
			  k = AutoSetupNullUc();
			  break;
	  case 2: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИЗМЕРИТЕЛЯ Ib");
			  k = AutoSetupNullIbm();
			  break;
	  case 3: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИЗМЕРИТЕЛЯ Ub");
			  k = AutoSetupNullUbm();
			  break;
	  case 4: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИСТОЧНИКА Ib");
			  k = AutoSetupNullIbs();
			  break;
	  case 5: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИСТОЧНИКА Ub");
			  k = AutoSetupNullUbs();
			  break;
	  case 6: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИЗМЕРИТЕЛЯ Is");
			  k = AutoSetupNullIsm();
			  break;
	  case 7: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИЗМЕРИТЕЛЯ Us");
			  k = AutoSetupNullUsm();
			  break;
	  case 8: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИСТОЧНИКА Is");
			  k = AutoSetupNullIss();
			  break;
	  case 9: //str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ ИСТОЧНИКА Us");
			  k = AutoSetupNullUss();
			  break;
  }//end switch(m_iNset)

  My_buf_Out[k] = '\0';	//конец передачи
  k = k + 1;
  ::WaitForSingleObject(hMutexO,INFINITE);
  if (bAutoSetNull==false && Flag_sboy)
  {
 						   k = 0;
						   memcpy(&My_buf_Out[k],&K13[0],nK[12]);
							k = k + nK[12];
						   ready = 1;
						   bAutoSetNull=false;
						   str = _T("СБОЙ КОРРЕКЦИИ!\n");
						   SetTextInfo(str);
 }
  // поместить в bufOut управляющие команды
  NumO = k;	//кол-во байт для передачи
  bufOut[0]=k+1;								//кол-во байт для передачи + 1 байт с количеством
  memcpy(&bufOut[1],&My_buf_Out[0],bufOut[0]);	//данные
  ::ReleaseMutex(hMutexO);
  PurgeComm(hCom2,PURGE_TXCLEAR|PURGE_RXCLEAR);
  SetEvent(hEventComOut);//++++//запускаем измерение
  TRACE(_T("SetEvent(hEventComOut)-OnMassiv()\n"));

//  if (m_iNset==10 && bAutoSetNull==false)
//  MessageBox(_T("Отсоедините перемычку \".027\", замыкающую гнезда Uc и E'!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);

}//end if (bAutoSetNull)

if (bKalibr)
{//КАЛИБРОВКА
  //переписать данные из буфера bufIn_Epp
  //-------------------------------------
  ::WaitForSingleObject(hMutexEpp,INFINITE);

  memcpy(&My_buf_Mas[0],&bufEppIn[0],NumI);	//данные
  ::ReleaseMutex(hMutexEpp);
  m_nCountMas++;

  TRACE(_T("m_nCountMas = %d  My_buf_Mas[0] = %x  My_buf_Mas[1] = %x  My_buf_Mas[2] = %x  My_buf_Mas[3] = %x\n   My_buf_Mas[4] = %x	My_buf_Mas[5] = %x\n   My_buf_Mas[6]= %x  My_buf_Mas[7]= %x\n  My_buf_Mas[8]= %x  My_buf_Mas[9]= %x\n"),m_nCountMas,My_buf_Mas[0],My_buf_Mas[1],My_buf_Mas[2],My_buf_Mas[3],My_buf_Mas[4],My_buf_Mas[5],My_buf_Mas[6],My_buf_Mas[7],My_buf_Mas[8],My_buf_Mas[9]);

   if (m_ir==0) Sleep(20);		//задержка сброса ENS для импульсной развертки

	    ens = _inp(0x79);		//запретить подачу сигнала на объект исследования
		ens = ens & 0xfd;
 		_outp(0x79,ens);	         //D1<-0 (ENB_PCM <- 0)

  for (j=0;j<4;j++)		//4 АЦП
     for (t=0;t<NPOINT;t++)
		MAS_RESULT[j][t] = 0;
  // обработка кодированного (реального) массива
  //--------------------------------------------
  int a;
  for (t=0;t<NPOINT;t++)	//t - № точки
  {	//раскладываем одну точку (8 байт (4 + 4)  по 4-м АЦП)
	//байты D15 - D14
	for (i=0;i<4;i++)		//i - счётчик байт
	{ 
   	  for (j=0;j<4;j++)		//j - № АЦП
	  { a = My_buf_Mas[k];	//k - № байта в принятом массиве
	    a = a & mask[j];
	    a = a << (j * 2);
		a = a >> (i * 2);
		MAS_RESULT[j][t] = MAS_RESULT[j][t] | a;
	  }
	  k++;
	}
	for (j=0;j<4;j++)
	   MAS_RESULT[j][t] = (MAS_RESULT[j][t]<< 8);	//сдвигаем старший байт
	// байты D7 - D0
	for (i=0;i<4;i++)	
	{ 
   	  for (j=0;j<4;j++)
	  { a = My_buf_Mas[k]; 
	    a = a & mask[j];
	    a = a << (j * 2);
		a = a >> (i * 2);
		MAS_RESULT[j][t] = MAS_RESULT[j][t] | a;
	  }
	  k++;
	}
  }//end for t
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //для АЦП2 (Ic) все точки сдвигаем на 1 бит влево
  for (t=0;t<NPOINT;t++)	//t - № точки
	   MAS_RESULT[1][t] = (MAS_RESULT[1][t]<<1);	
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //обработка знака
  //---------------
  int tem=0;
  for (i=0;i<4;i++)
  {  for (t=0;t<NPOINT;t++)
		{ tem = MAS_RESULT[i][t] & 0x8000;		//знак
			{ if (tem==0)   { //+  
							  MAS_RESULT[i][t] = MAS_RESULT[i][t] & 0x7fff;
//							  if (MAS_RESULT[i][t]>0x7c50) FlagOL[i]=true;
							}     
					  else  { //-
							  MAS_RESULT[i][t] = MAS_RESULT[i][t] | 0xffff8000;
//							  if (MAS_RESULT[i][t]<0x8000) FlagOL[i]=true;
							} 
			}
		}
  }
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //для АЦП2 (Ic) инверсия знака
//  for (t=0;t<NPOINT;t++)	//t - № точки
//	   MAS_RESULT[1][t] = MAS_RESULT[1][t]*(-1);	
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //разложить по массивам и умножить на дискрет:
  //t - № точки 
  for (t=0;t<NPOINT;t++)
  { //АЦП1 - Uc
	    pM1[m_nCountMas-1][t] = (MAS_RESULT[0][t]* koef_kol_U[bias_Uc+m_iUc])*KMCU[bias_Uc+m_iUc]-BMCU[bias_Uc+m_iUc];	
    //АЦП2 - Ic
//	    pM2[m_nCountMas-1][t] = (MAS_RESULT[1][t]* koef_kol_I[bias_Ic+m_iIc])*KMCI[bias_Ic+m_iIc]-BMCI[bias_Ic+m_iIc];		
	    pM2[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[1][t]* koef_kol_I[bias_Ic+m_iIc])*KMCI[bias_Ic+m_iIc]-BMCI[bias_Ic+m_iIc];		
    //АЦП3 - Baza
	if (m_iBs==0 && m_iBm==1)	//источник U - измеритель I
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_I[m_iRb])*KMBI[m_iRb]-BMBI[m_iRb];//диапазон ограничения I		
	if (m_iBs==1 && m_iBm==0)	//источник I - измеритель U
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_U[m_iRb])*KMBU[m_iRb]-BMBU[m_iRb];//диапазон ограничения U		
	if (m_iBs==1 && m_iBm==1)	//источник I - измеритель I
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_I[m_iAb])*KMBI[m_iAb]-BMBI[m_iAb];//диапазон источника I		
	if (m_iBs==0 && m_iBm==0)	//источник U - измеритель U
		pM3[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[2][t]* koeff_meas_bazadop_U[m_iAb])*KMBU[m_iAb]-BMBU[m_iAb];//диапазон источника U		
	//АЦП4 - Dop
	if (m_iDs==0 && m_iDm==1)	//источник U - измеритель I
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_I[m_iRd+bias_Id])*KMDI[m_iRd]-BMDI[m_iRd];//диапазон ограничения I		
	if (m_iDs==1 && m_iDm==0)	//источник I - измеритель U
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_U[m_iRd])*KMDU[m_iRd]-BMDU[m_iRd];//диапазон ограничения U		
	if (m_iDs==1 && m_iDm==1)	//источник I - измеритель I
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_I[m_iAd+bias_Id])*KMDI[m_iAd]-BMDI[m_iAd];//диапазон источника I		
	if (m_iDs==0 && m_iDm==0)	//источник U - измеритель U
		pM4[m_nCountMas-1][t] = (-1.0)*(MAS_RESULT[3][t]* koeff_meas_bazadop_U[m_iAd])*KMDU[m_iAd]-BMDU[m_iAd];//диапазон источника U		
  }
  //-------------------------------------------------------------------------------
  if (m_ir==1)//калибровка
  {//DC (20 раз читаем результат, первые 4 пропускаем, остальные 16 усредняем=получаем 1 точку
		 if (nCountPointDC < 4)
		 {//пропустить 4 результата измерения
			nCountPointDC++;
			m_nCountMas=0;
			    // поместить в bufOut управляющие команды
			    ::WaitForSingleObject(hMutexO,INFINITE);
			    bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			    bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			    bufOut[2]='\0';		//конец передачи
			    NumO = 2;
			    ::ReleaseMutex(hMutexO);
// 			    nK41=nK41+1;
//			    TRACE(_T("K41 nK41=%d\n"),nK41);
		        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
			    SetEvent(hEventComOut);//+++++++++++++++++++++

// 				SetEvent(hEventEppIn);//`````````````````````
			    return 0L;
		 }
		 else
		 {
			if (nCountPointDC < 19)//(4-пропущено + 16-усредняем = 20)
			{
			  aM1[0] =  aM1[0] + pM1[m_nCountMas-1][POINTDC];
			  aM2[0] =  aM2[0] + pM2[m_nCountMas-1][POINTDC];
			  aM3[0] =  aM3[0] + pM3[m_nCountMas-1][POINTDC];
			  aM4[0] =  aM4[0] + pM4[m_nCountMas-1][POINTDC];
			  nCountPointDC++;
			  m_nCountMas=0;
			    // поместить в bufOut управляющие команды
			    ::WaitForSingleObject(hMutexO,INFINITE);
			    bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
			    bufOut[1]=K41[0];	//команда "Cледующая ступенька"
			    bufOut[2]='\0';		//конец передачи
			    NumO = 2;
			    ::ReleaseMutex(hMutexO);
// 			    nK41=nK41+1;
//			    TRACE(_T("K41 nK41=%d\n"),nK41);
		        PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);
			    SetEvent(hEventComOut);//+++++++++++++++++++++

// 				SetEvent(hEventEppIn);//`````````````````````
			    return 0L;
			}
			else
			{
			  aM1[0] =  aM1[0] + pM1[m_nCountMas-1][POINTDC];
			  aM2[0] =  aM2[0] + pM2[m_nCountMas-1][POINTDC];
			  aM3[0] =  aM3[0] + pM3[m_nCountMas-1][POINTDC];
			  aM4[0] =  aM4[0] + pM4[m_nCountMas-1][POINTDC];
			  aM1[0] =  aM1[0] / 16;
			  aM2[0] =  aM2[0] / 16;
			  aM3[0] =  aM3[0] / 16;
			  aM4[0] =  aM4[0] / 16;//результат в aM1[0],aM2[0],aM3[0],aM4[0]
			  nCountPointDC=0;
			  m_nCountMas=0;
			}
		  }
  }//end if m_ir==1 (DC)

  if (m_ir==0)//калибровка
  {//Импульсная развёртка 
//Импульсная развёртка 
		UINT m=0;
 	if (m_nCountMas < m_nMas)
// 	if (m_nCountMas < n_Mas)
	{
		//отрабатываем очередной импульс
		for(i=0;i<NPOINT;i++)
		{ mM1[m_nCountMas-1][i] = pM1[m_nCountMas-1][i];
		  mM2[m_nCountMas-1][i] = pM2[m_nCountMas-1][i];
	      mM3[m_nCountMas-1][i] = pM3[m_nCountMas-1][i];
	      mM4[m_nCountMas-1][i] = pM4[m_nCountMas-1][i];}
		//из каждого массива из NPOINT точек берём n_t-последнюю (NPOINT+NPOINT*stup-n_t)
		//и помещаем её на место первой для текущей ступени
	    for(m=0;m<m_nCountMas;m++)
		{ pM1[m][0]=mM1[m][NPOINT-n_t];
	      pM2[m][0]=mM2[m][NPOINT-n_t];
		  pM3[m][0]=mM3[m][NPOINT-n_t];
		  pM4[m][0]=mM4[m][NPOINT-n_t];}

		// поместить в bufOut управляющие команды : отработать следующий импульс (ступеньку)
		::WaitForSingleObject(hMutexO,INFINITE);
		bufOut[0]=3;		//кол-во байт для передачи + 1 байт с количеством
		bufOut[1]=K41[0];	//команда "Cледующая ступенька"
        bufOut[2]='\0';		//конец передачи
		NumO = 2;
        ::ReleaseMutex(hMutexO);
// 		nK41=nK41+1;
//		TRACE(_T("K41 nK41=%d\n"),nK41);
       PurgeComm(hCom2,PURGE_TXCLEAR);//|PURGE_RXCLEAR);	  //---------------------------------------------------
			    SetEvent(hEventComOut);//+++++++++++++++++++++
	    return 0L;
	}
 	if (m_nCountMas == m_nMas)
// 	if (m_nCountMas == n_Mas)
	{ //отработали все импульсы 
		for(i=0;i<NPOINT;i++)
		{ mM1[m_nCountMas-1][i] = pM1[m_nCountMas-1][i];
		  mM2[m_nCountMas-1][i] = pM2[m_nCountMas-1][i];
	      mM3[m_nCountMas-1][i] = pM3[m_nCountMas-1][i];
	      mM4[m_nCountMas-1][i] = pM4[m_nCountMas-1][i];}
	    
	    //из каждого массива из NPOINT точек берём n_t-последнюю (NPOINT+NPOINT*stup-n_t)
		//и помещаем её на место первой для текущей ступени
	    for(m=0;m<m_nMas;m++)
		{ pM1[m][0]=mM1[m][NPOINT-n_t];
	      pM2[m][0]=mM2[m][NPOINT-n_t];
		  pM3[m][0]=mM3[m][NPOINT-n_t];
		  pM4[m][0]=mM4[m][NPOINT-n_t];}
  }
  }//end if m_ir==0

  int k=0;
	switch(m_iNsetK)		//№ программы
	{ case 0: k = KalibrIc();
			  break;
	  case 1: k = KalibrUc();
			  break;
	  case 2: k = KalibrIb();
			  break;
	  case 3: k = KalibrUb();
			  break;
	  case 4: k = KalibrIs();
			  break;
	  case 5: k = KalibrUs();
			  break;
	}
//-------------------------------
  if ((k==0) ||
	  (bKalibr==false && Flag_sboy==1))
  { //калибровка прервана
			//выключить калибровку
			k=0;
			memcpy(&My_buf_Out[k],&K13[0],nK[12]);
			k = k + nK[12];
			ready = 1;
			m_iNproxod=0;
			bKalibr=false;
			if (Flag_sboy==1) str = _T("СБОЙ КАЛИБРОВКИ!");
		    else str = _T("КАЛИБРОВКА ПРЕРВАНА!");
            SetTextInfo(str);
  }

   My_buf_Out[k] = '\0';	//конец передачи
   k = k + 1;
   ::WaitForSingleObject(hMutexO,INFINITE);
   NumO = k;	//кол-во байт для передачи
   // поместить в bufOut управляющие команды
   bufOut[0]=k+1;								//кол-во байт для передачи + 1 байт с количеством
   memcpy(&bufOut[1],&My_buf_Out[0],bufOut[0]);	//данные
   ::ReleaseMutex(hMutexO);
   PurgeComm(hCom2,PURGE_TXCLEAR|PURGE_RXCLEAR);
//   SetEvent(hEventComOut);//++++//
	::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);	//+1 байт, т.к. с кол-вом байт для передачи

}//end if (bKalibr)

return 0L;
//}//end of else //wParam lParam
}//end of OnMassiv()

//=======================================================


int CLineDlg::KalibrIb()
{  
  int k=0;
  CString str;
  CValEnter dl;
  
  switch(m_iNproxod)
  { case 0: t1 = aM3[0];
			//отобразить поле ввода
			value_enter = 0.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("A");
			dl.m_bWhat = false;
           	UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);
			BSBI[m_iAb] = BSBI[m_iAb] + value_enter;
			BMBI[m_iAb] = BMBI[m_iAb] + t1  - value_enter;
			k = PuskKalibrIb(1,m_iRange);
			m_iNproxod = 1;
			}
			break;
    case 1: t2 = aM3[0];
			//отобразить поле ввода
			value_enter = 1.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("A");
			dl.m_bWhat = false;
	        UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);

			double	a=mas_d_range_ampl_bazadop_I[m_iAb];
			double b = mas_d_range_bias_bazadop_I[m_iAb];
			  b = (b/1000.0)*m_iBb;
			double ksbi = a * m_uSb + b;			  

			  if ( value_enter!=0.0) KSBI[m_iAb] = KSBI[m_iAb] * ksbi / value_enter;
							  else KSBI[m_iAb] = KSBI[m_iAb];
//			  if ( value_enter!=0.0) KSBI[m_iAb] = KSBI[m_iAb] * mas_d_range_meas_bazadop_I[m_iAb] / value_enter;
//							  else KSBI[m_iAb] = KSBI[m_iAb];
			if (t2!=0.0) KMBI[m_iAb] = KMBI[m_iAb] * value_enter / t2;
					else KMBI[m_iAb] = KMBI[m_iAb];
						
			//выключить калибровку
			m_iNproxod=0;
			k=0;
			memcpy(&My_buf_Out[k],&K13[0],nK[12]);
			k = k + nK[12];
			ready = 1;
			bKalibr=false;
			str.Format(_T("\nДИАПАЗОН %s ОТКАЛИБРОВАН"),mas_range_meas_bazadop_I[m_iAb]);
			SetTextInfo(str);
			}
			break;
  }
return(k);
}
int CLineDlg::KalibrUb()
{  
  int k=0;
	CString str;
  CValEnter dl;
  
  switch(m_iNproxod)
  { case 0: t1 = aM3[0];
			//отобразить поле ввода
			value_enter = 0.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("V");
			dl.m_bWhat = false;
           	UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);
			BSBU[m_iAb] = BSBU[m_iAb] + value_enter;
			BMBU[m_iAb] = BMBU[m_iAb] + t1 - value_enter;
			k = PuskKalibrUb(1,m_iRange);
			m_iNproxod = 1;
			}
			break;
    case 1: t2 = aM3[0];
			//отобразить поле ввода
			value_enter = 1.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("V");
			dl.m_bWhat = false;
	        UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);

			if ( value_enter!=0.0) KSBU[m_iAb] = KSBU[m_iAb] * mas_d_range_meas_bazadop_U[m_iAb] / value_enter;
							  else KSBU[m_iAb] = KSBU[m_iAb];
			if (t2!=0.0) KMBU[m_iAb] = KMBU[m_iAb] * value_enter / t2;
					else KMBU[m_iAb] = KMBU[m_iAb];
						
			//выключить калибровку
			k=0;
			memcpy(&My_buf_Out[k],&K13[0],nK[12]);
			k = k + nK[12];
			ready = 1;
			m_iNproxod=0;
			bKalibr=false;
			str.Format(_T("\nДИАПАЗОН %s ОТКАЛИБРОВАН"),mas_range_meas_bazadop_U[m_iAb]);
			SetTextInfo(str);
			}
			break;
  }
return(k);
}
int CLineDlg::KalibrIs()
{  
  int k=0;
	CString str;
  CValEnter dl;
  
  switch(m_iNproxod)
  { case 0: t1 = aM4[0];
			//отобразить поле ввода
			value_enter = 0.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("A");
			dl.m_bWhat = false;
           	UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);
			BSDI[m_iAd] = BSDI[m_iAd] + value_enter;
			BMDI[m_iAd] = BMDI[m_iAd] + t1 - value_enter;
			k = PuskKalibrIs(1,m_iRange);
			m_iNproxod = 1;
			}
			break;
    case 1: t2 = aM4[0];
			//отобразить поле ввода
			value_enter = 1.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("A");
			dl.m_bWhat = false;
	        UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);

			double	a=mas_d_range_ampl_bazadop_I[m_iAd+bias_Id];
			double b = mas_d_range_bias_bazadop_I[m_iAd+bias_Id];
			  b = (b/1000.0)*m_iBd;
			double ksdi = a * m_uSd + b;			  

			  if ( value_enter!=0.0) KSDI[m_iAd] = KSDI[m_iAd] * ksdi / value_enter;
							  else KSBI[m_iAd] = KSBI[m_iAd];
//			if ( value_enter!=0.0) KSDI[m_iAd] = KSDI[m_iAd] * mas_d_range_meas_bazadop_I[m_iAd+bias_Id] / value_enter;
//							  else KSDI[m_iAd] = KSDI[m_iAd];
			if (t2!=0.0) KMDI[m_iAd] = KMDI[m_iAd] * value_enter / t2;
					else KMDI[m_iAd] = KMDI[m_iAd];
						
			//выключить калибровку
			k=0;
			memcpy(&My_buf_Out[k],&K13[0],nK[12]);
			k = k + nK[12];
			ready = 1;
			m_iNproxod=0;
			bKalibr=false;
			str.Format(_T("\nДИАПАЗОН %s ОТКАЛИБРОВАН"),mas_range_meas_bazadop_I[m_iAd+bias_Id]);
			SetTextInfo(str);
			}
			break;
  }
return(k);
}
int CLineDlg::KalibrUs()
{  
  int k=0;
	CString str;
  CValEnter dl;
  
  switch(m_iNproxod)
  { case 0: t1 = aM4[0];
			//отобразить поле ввода
			value_enter = 0.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("V");
			dl.m_bWhat = false;
           	UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);
			BSDU[m_iAd] = BSDU[m_iAd] + value_enter;
			BMDU[m_iAd] = BMDU[m_iAd] + t1 -  value_enter;
			k = PuskKalibrUs(1,m_iRange);
			m_iNproxod = 1;
			}
			break;
    case 1: t2 = aM4[0];
			//отобразить поле ввода
			value_enter = 1.0;
			dl.m_sValEnter = _T("");
			dl.m_sRazm =_T("V");
			dl.m_bWhat = false;
	        UpdateData(FALSE);
			if (dl.DoModal()==IDOK)
			{ UpdateData();
			  value_enter = GetNumberFromString(dl.m_sValEnter);

			if ( value_enter!=0.0) KSDU[m_iAd] = KSDU[m_iAd] * mas_d_range_meas_bazadop_U[m_iAd] / value_enter;
							  else KSDU[m_iAd] = KSDU[m_iAd];
			if (t2!=0.0) KMDU[m_iAd] = KMDU[m_iAd] * value_enter / t2;
					else KMDU[m_iAd] = KMDU[m_iAd];
						
			//выключить калибровку
			k=0;
			memcpy(&My_buf_Out[k],&K13[0],nK[12]);
			k = k + nK[12];
			ready = 1;
			m_iNproxod=0;
			bKalibr=false;
			str.Format(_T("\nДИАПАЗОН %s ОТКАЛИБРОВАН"),mas_range_meas_bazadop_U[m_iAd]);
			SetTextInfo(str);
			}
			break;
  }
return(k);
}

int CLineDlg::KalibrUc()
{ // результат в aM1[0] 
  int k=0;
	CString str;
  switch(m_iNproxod)
  { case 0: t1 = aM1[0];
			BMCU[m_iUc] = BMCU[m_iUc] + t1;
			m_iNproxod = 1;
			k = PuskKalibrUc(1,m_iRange);
			ready = 0;
			break;
    case 1: t2 = aM1[0];
		    if (t2!=0.0) KMCU[m_iUc] = KMCU[m_iUc] * value_necessary / t2;
			else KMCU[m_iUc] = KMCU[m_iUc];
						//выключить калибровку
						k=0;
						memcpy(&My_buf_Out[k],&K13[0],nK[12]);
						k = k + nK[12];
						ready = 1;
						m_iNproxod=0;
						bKalibr=false;
						str.Format(_T("\nДИАПАЗОН %s ОТКАЛИБРОВАН"),mas_rangeKolMeasU[m_iUc+bias_Uc]);
						SetTextInfo(str);
			break;
  }
return(k);
}

int CLineDlg::KalibrIc()
{ // результат в aM2[0] 
  int k=0;
  CString str;
  CValEnter dl;
		int range_Ic = bias_Ic + m_iIc;
//		if ((bias_Ic + m_iIc)>7)
		if (range_Ic > 7)
		{//Режим 1 и 2
			switch(m_iNproxod)
			  { case 0: //t1 - точка в нуле
						t1 = aM2[0];
						//отобразить поле ввода
						value_enter = 0.0;
						dl.m_sValEnter = _T("");
						dl.m_sRazm =_T("A");
						dl.m_bWhat = false;
	                 	UpdateData(FALSE);
						if (dl.DoModal()==IDOK)
						{ UpdateData();
						  value_enter = GetNumberFromString(dl.m_sValEnter);
						BMCI[bias_Ic+m_iIc] = BMCI[bias_Ic+m_iIc] + t1 - value_enter; // 
						k = PuskKalibrIc(1,m_iRange);
						m_iNproxod = 1;
						}
						
						break;
			    case 1: //t2 - предел диапазона
						t2 = aM2[0];
						//отобразить поле ввода
						value_enter = 1.0;
						dl.m_sValEnter = _T("");
						dl.m_sRazm =_T("A");
						dl.m_bWhat = false;
	                 	UpdateData(FALSE);
						if (dl.DoModal()==IDOK)
						{ UpdateData();
						  value_enter = GetNumberFromString(dl.m_sValEnter);
						if (t2!=0.0) KMCI[bias_Ic+m_iIc] = KMCI[bias_Ic+m_iIc] * value_enter / t2;
						else KMCI[bias_Ic+m_iIc] = KMCI[bias_Ic+m_iIc];
						
						//выключить калибровку
						k=0;
						memcpy(&My_buf_Out[k],&K13[0],nK[12]);
						k = k + nK[12];
						ready = 1;
						m_iNproxod=0;
						bKalibr=false;
						str.Format(_T("\nДИАПАЗОН %s ОТКАЛИБРОВАН"),mas_rangeKolMeasI[m_iRange+bias_Ic]);
						SetTextInfo(str);
						}
						break;
			  }
		}
		else
		{ //Режим 3
			switch(m_iNproxod)
			  { case 0: //t1 - точка в нуле
//						v1 = aM1[0];	//x1
//						t1 = aM2[0];	//y1
						v1 = pM1[2][0];	//x1
						t1 = pM2[2][0];	//y1
						v2 = pM1[18][0];	//x2
					    t2 = pM2[18][0];	//y2
/*						k = PuskKalibrIc(1,m_iRange);
						m_iNproxod = 1;
						break;
			    case 1: //t2 - предел диапазона
						v2 = aM1[0];	//x2
					    t2 = aM2[0];	//y2
*/					    
						//отобразить поле ввода
						value_R_nominal = 1.0;
						dl.m_sValEnter = _T("");
						dl.m_sRazm =_T("Om");
						dl.m_bWhat = true;
	                 	UpdateData(FALSE);
						if (dl.DoModal()==IDOK)
						{ UpdateData();
						  value_R_nominal = GetNumberFromString(dl.m_sValEnter);
						
						double b = 0.0;
							
						if ((v2-v1)!=0)	b = t1 - (v1*t2 - v1*t1) / (v2 - v1);
						
						BMCI[bias_Ic+m_iIc] = BMCI[bias_Ic+m_iIc] + b ;
						
						double  w = 1.0;
						
						if ((v2-v1)!=0) w =(t2-t1)/(v2-v1);
						w =  w * value_R_nominal;

						KMCI[bias_Ic+m_iIc] = KMCI[bias_Ic+m_iIc] / w;
												
						//выключить калибровку
						k=0;
						memcpy(&My_buf_Out[k],&K13[0],nK[12]);
						k = k + nK[12];
						ready = 1;
						m_iNproxod=0;
						bKalibr=false;
						str.Format(_T("\nДИАПАЗОН %s ОТКАЛИБРОВАН"),mas_rangeKolMeasI[m_iRange+bias_Ic]);
						SetTextInfo(str);
						}
						break;
			  }
		}
return(k);
}
int CLineDlg::AutoSetupNullIc_10nA(int m_iRange)
{ // результат в aM2[0] 
  int k=0;
  CString str;

			  switch(m_iNproxod)
			  {
				case 0:	//диапазоны 2uA - 10 nA
						BMCI[bias_Ic+m_iIc] = BMCI[bias_Ic+m_iIc] +  aM2[0];
						 //конец автоустановки нуля Ic
//					    MessageBox(_T("Отсоедините перемычку \".027\", замыкающую гнезда Uc и E'!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
								  //KillTimer(2);
								   //команда К13 "Bыкл. автоустановку нуля"
								   k = 0;
									memcpy(&My_buf_Out[k],&K13[0],nK[12]);
									k = k + nK[12];
						           ready = 1;
								   bAutoSetNull=false;
								   bAutoSetNullIc_10nA=true;
									TRACE(_T("K13\n"));
//								   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
								   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
								   SetTextInfo(str);
//								   MessageBox(_T("Коррекция смещения нуля измерителя I канала С закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
								   m_iNproxod=0;
						
						break;
			  }
return(k);

}


int CLineDlg::AutoSetupNullIc()
{ // результат в aM2[0] 
  int k=0;
  CString str;

			  switch(m_iNproxod)
			  { case 0: //t1 - точка 1
						t1 = aM2[0];
						m_iLstart= 20;//100;	//ограничение Uc min
						m_iLstop= 20;//100;	//ограничение Uc max
						memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIcm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIcm));
						k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIcm);
						k = CommandKol(k);
						ready = 0;
						m_iNproxod=1;
						break;
				case 1: //t2 - точка 2
						t2 = aM2[0];
						m_iLstart= 20;//100;	//ограничение Uc min
						m_iLstop= 20;//100;	//ограничение Uc max
						memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIcm_[0],SIZEOF_ARRAY(mas_ControlAutoSetupIcm_));
						k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIcm_);
						k = CommandKol(k);
						ready = 0;
						m_iNproxod=2;
						break;
				case 2: //t3 - точка 3
						t3 = aM2[0];
						//контроль
//						if ((fabs(t1-t2)>0.008) || (fabs(t1-t3)>0.008))
						if ((fabs(t1-t2)>0.025) || (fabs(t1-t3)>0.025))
						{ //KillTimer(2);
						  m_iNproxod=0;
						  bAutoSetNull=false;
							//0.команда К13 "Bыкл. автоустановку нуля"
						    k = 0;
							memcpy(&My_buf_Out[k],&K13[0],nK[12]);
							k = k + nK[12];
							ready = 1;
							TRACE(_T("K13\n"));
						  MessageBox(_T("Устраните утечку тока в канале С и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
						  str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
						  SetTextInfo(str);
						}
						else
						{//OK'
						 //начинаем измерение поправок BMCI для всех диапазонов
						 //устанавливаем начальный № диапазона в зависимости от модификации прибора
						   m_ir = 0;//импульсный
						   nCountPointDC = 0;
						   m_nCountMas=0;		
						   m_iLstart= 0;	//ограничение Uc min
						   m_iLstop= 0;	//ограничение Uc max
						   if (n_IPPP==1)
						   { 
povt0:					     MessageBox(_T("Установите переключатель в положение +5V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
						     if (n_Switch!=1) goto povt0;
							

						     bias_Uc = 9;
						     m_iUc = 0;		//5V измеритель
						     bias_Ic = 0;
						     m_iIc = 0;		//200A
							 bias_Ucs = 0;
							 m_iUcs = 0;  NPOINT = mas_n_points[5];//число точек в снимаемом массиве для источника Uc=20V
						     NumI = NPOINT*8;	//NPOINT точек по 8 байт
						str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \n ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						TRACE(_T("ДИАПАЗОН %s\n"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
							 memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_200A[0],SIZEOF_ARRAY(mas_AutoSetupIcm_200A));
						     k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_200A);
						     k = CommandKol(k);
						     m_iNproxod=3;
						     ready = 0;
						   }
						   else
						   { bias_Uc = 0;
						     m_iUc = 7;		//20V измеритель
						     bias_Ic = 5;//4;
							 m_iIc  = 0; //20A	//50A
						     NPOINT = mas_n_points[4];//число точек в снимаемом массиве для источника Uc=20V
						     NumI = NPOINT*8;	//NPOINT точек по 8 байт
						str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \n ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						TRACE(_T("ДИАПАЗОН %s\n"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
							 memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_50A[0],SIZEOF_ARRAY(mas_AutoSetupIcm_50A));
						     k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_50A);
						     k = CommandKol(k);
						     m_iNproxod=4;
						     ready = 0;
						   }
						}
						break;
				case 3: //диапазоны 200А - 20А импульс
						BMCI[bias_Ic+m_iIc] = BMCI[bias_Ic+m_iIc] + aM2[0];
						m_iIc++;
						if ((bias_Ic+m_iIc)<4)//<=3)
						{
						str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \n ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						TRACE(_T("ДИАПАЗОН %s\n"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						  memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_200A[0],SIZEOF_ARRAY(mas_AutoSetupIcm_200A));
						  k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_200A);
					      k = CommandKol(k);
						}
						else
						{  //m_iIc++;//пропуск 50A
povt:					   MessageBox(_T("Установите переключатель в положение ±2000V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
						   if (n_Switch!=0) goto povt; 
							bias_Ucs=1;
							m_iUcs= 0;	//	bias_Ucs + m_iUcs - диапазон источника (U) коллектора		
							NPOINT = mas_n_points[4];//число точек в снимаемом массиве для источника Uc=20V
						   NumI = NPOINT*8;	//NPOINT точек по 8 байт
						str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \n ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						TRACE(_T("ДИАПАЗОН %s\n"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						   memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_50A[0],SIZEOF_ARRAY(mas_AutoSetupIcm_50A));
						   k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_50A);
					       k = CommandKol(k);
						   m_iNproxod=4;
						}
						ready = 0;
						break;
				case 4: //диапазоны 50A - 100uA//10nA
						BMCI[bias_Ic+m_iIc] = BMCI[bias_Ic+m_iIc] +  aM2[0];
						m_iIc++;
						if ((bias_Ic+m_iIc)<11)//11
						{  
						str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \n ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						TRACE(_T("ДИАПАЗОН %s\n"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
							memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_50A[0],SIZEOF_ARRAY(mas_AutoSetupIcm_50A));
						   k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_50A);
					       k = CommandKol(k);
						     ready = 0;
						}
						else
						{ 
						str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \n ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						TRACE(_T("ДИАПАЗОН %s\n"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						  m_ir = 1; 
							bias_Ucs=1;
							m_iUcs= 0;	//	bias_Ucs + m_iUcs - диапазон источника (U) коллектора		
							bias_Ic=11;
							m_iIc = 0;
							bias_Uc = 0;
						     m_iUc = 7;		//20V измеритель
						  NPOINT = NPOINTS;	//массив из 200 точек
						  NumI = NPOINT*8;	//NPOINT точек по 8 байт
						  memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_200mA[0],SIZEOF_ARRAY(mas_AutoSetupIcm_200mA));
						  k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_200mA);
					      k = CommandKol(k);
						  ready = 0;
						  m_iNproxod=5;
						}
						break;
				case 5:	BMCI[bias_Ic+m_iIc] = BMCI[bias_Ic+m_iIc] +  aM2[0];
						m_iIc++;
					    if ((bias_Ic+m_iIc)<21)//200uA 	//MAXRANGEIKOL)	// или 33 или 36???
						{ 
						str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \n ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
						TRACE(_T("ДИАПАЗОН %s\n"),mas_rangeKolMeasI[m_iIc+bias_Ic]);
							memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_200mA[0],SIZEOF_ARRAY(mas_AutoSetupIcm_200mA));
						  k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_200mA);
					      k = CommandKol(k);
						  ready = 0;
						}
						else
						{ 
						  //конец автоустановки нуля Ic
						  //KillTimer(2);
						  //команда К13 "Bыкл. автоустановку нуля"
						   k = 0;
						   memcpy(&My_buf_Out[k],&K13[0],nK[12]);
							k = k + nK[12];
						   ready = 1;
						   bAutoSetNull=false;
//						   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
						   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
						   SetTextInfo(str);
//						   MessageBox(_T("Коррекция смещения нуля измерителя I канала С закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
						   m_iNproxod=0;
						}
						break;
			  }//end of switch (m_iNproxod)
//	   SetTextInfo(str);

return(k);
}

int  CLineDlg::AutoSetupNullUc()
{ // результат в aM1[0] 
  int k=0;
  CString str;

  switch(m_iNproxod)
	{ case 0:	//t1 - первая точка 
				t1 = aM1[0];
				TRACE(_T("=================t1===============\n"));
						m_iLstart= 6;	//ограничение Uc min
						m_iLstop= 6;	//ограничение Uc max
				memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUcm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUcm));
				k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUcm);
				k = CommandKol(k);
				ready = 0;
				m_iNproxod=1;
			    break;
	   case 1:	//t2 - вторая точка
				t2 = aM1[0];
				TRACE(_T("==================t2===============\n"));
				//-IMP
				memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUcm_[0],SIZEOF_ARRAY(mas_ControlAutoSetupUcm_));
				k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUcm_);
				k = CommandKol(k);
				ready = 0;
				m_iNproxod=2;
				break;
	   case 2:	//t3 - третья точка
				t3 = aM1[0];
				TRACE(_T("===================t3===============\n"));
				//контроль
				if ((fabs(t1-t2)>0.08) || (fabs(t1-t3)>0.08))//80 mV(4%)
				{ //KillTimer(2);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k = 0;
					memcpy(&My_buf_Out[k],&K13[0],nK[12]);
					k = k + nK[12];
				  ready = 1;
				  MessageBox(_T("Проверьте соединение гнезд и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				  str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				  SetTextInfo(str);
				  bAutoSetNull=false;
				  m_iNproxod=0;
				}
				else
				{ //OK'
				  //начинаем измерение поправок BMCU для всех диапазонов
				  //устанавливаем начальный № диапазона
					if (n_IPPP==1 && n_Switch!=0)
					{
povt4:				   MessageBox(_T("Установите переключатель в положение ±2000V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
					   if (n_Switch!=0) goto povt4; 
					}
					m_ir  = 1;	//DC
					m_iUc = 0;	//измеритель Uc - 5000V
					m_iIc = 11; //200mA      17;	//измеритель Ic - 2mA
					bias_Ic = 0;
					bias_Uc = 0;
						m_iLstart= 0;	//ограничение Uc min
						m_iLstop= 0;	//ограничение Uc max
					NPOINT = NPOINTS;	//массив из 200 точек
					NumI = NPOINT*8;	//NPOINT точек по 8 байт
					memcpy(&My_buf_Out[k],&mas_AutoSetupUcm[0],SIZEOF_ARRAY(mas_AutoSetupUcm));
					k = k + SIZEOF_ARRAY(mas_AutoSetupUcm);
				    k = CommandKol(k);
					m_iNproxod=3;
				    ready = 0;
				}
				break;
	   case 3:	// проходим все диапазоны по Uc от 5000V до 0.5V
				BMCU[bias_Uc+m_iUc] = BMCU[bias_Uc+m_iUc] +  aM1[0];
				m_iUc++;
/*				if ((bias_Uc+m_iUc)==9 && n_IPPP==1)
povt5:		    { MessageBox(_T("Установите переключатель в положение +5V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
				  if (n_Switch!=1) goto povt5; 
				  NPOINT = mas_n_points[5];//число точек в снимаемом массиве для источника Uc=20V
				  NumI = NPOINT*8;	//NPOINT точек по 8 байт
				  m_ir =0;		//IMP
				  bias_Uc = 9;
				  m_iUc = 0;
				  m_iIc = 3;	//20A
				  bias_Ucs = 0;
				  m_iUcs = 0;	//ист. Uc 5V
				}
*/				if ((bias_Uc+m_iUc)<13)
				{ //mas_AutoSetupUcm[17]=bias_Uc + m_iUc;
//					if (n_IPPP==1)
//				  memcpy(&My_buf_Out[k],&mas_AutoSetupUcm_5V[0],SIZEOF_ARRAY(mas_AutoSetupUcm));
//					else
				  memcpy(&My_buf_Out[k],&mas_AutoSetupUcm[0],SIZEOF_ARRAY(mas_AutoSetupUcm));
				  k = k + SIZEOF_ARRAY(mas_AutoSetupUcm);
				    k = CommandKol(k);
				   ready = 0;
				}
				else
				{ //установка нуля Uc закончена
				  //KillTimer(2);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
					memcpy(&My_buf_Out[k],&K13[0],nK[12]);
					k = k + nK[12];
				   ready = 1;
				   bAutoSetNull=false;
//				   MessageBox(_T("Коррекция смещения нуля измерителя U канала С закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
//				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
				}
				break;
	}
return(k);
}

int  CLineDlg::AutoSetupNullIbm()
{ //результат в aM3[0]	
	int k=0;
	CString str;

	switch(m_iNproxod)
	{ case 0: //t1 - точка 1
			  t1 = aM3[0];
			  m_iBb = 1000;	//смещение 20V
			  sign_Ab = 0;
			  sign_Bb = 0;	//полярность смещения - прямая
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbm);
			  k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM3[0];
			  m_iBb = 1000;	//смещение 20V
			  sign_Ab = 0;
			  sign_Bb = 1;	//полярность смещения - обратная
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbm);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  m_iNproxod=2;
			  ready = 0;
			  break;
	  case 2: //t3 - точка 3
			  t3 = aM3[0];
			  //контроль
			  if ((fabs(t1-t2)>0.0000000004) || (fabs(t1-t3)>0.0000000004))
			  { //KillTimer(2);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Устраните утечку тока в канале B и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BMBI для всех диапазонов
			   //устанавливаем начальный № диапазона
			    m_ir = 1;	//DC
			    m_iBb = 0;	//смещение 0V
			    sign_Ab = 0;
			    sign_Bb = 0;//полярность смещения - прямая
				m_iRb = 0;	//диапазон ограничения - 20 A
			    memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbm));
			    k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbm);
				    k = CommandKol(k);
			    k = CommandBazaUbsIbm(k);
			    m_iNproxod=3;
			    ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Ib от 20A до 100uA//40nA
				BMBI[m_iRb] = BMBI[m_iRb] +  aM3[0];
				m_iRb++;
				if ((m_iRb)<17)//27
				{ memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbm));
			      k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbm);
				    k = CommandKol(k);
			      k = CommandBazaUbsIbm(k);
				ready = 0;
				}
				else
				{ //установка нуля Ib закончена
				  //KillTimer(2);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
				   memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				   k = k + nK[12];
				   ready = 1;
				   bAutoSetNull=false;
//				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
//				   MessageBox(_T("Коррекция смещения нуля измерителя I канала B закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				}
				break;
	}
return(k);
}

int CLineDlg::AutoSetupNullUbm()
{//результат в aM3[0]
	int k=0;
	CString str;

	switch(m_iNproxod)
	{ case 0: //t1 - точка 1
			  t1 = aM3[0];
			  m_iBb = 1000;	//смещение 200mA
			  sign_Ab = 0;
			  sign_Bb = 0;	//полярность смещения - прямая
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbm);
				    k = CommandKol(k);
			  k = CommandBazaIbsUbm(k);
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM3[0];
			  m_iBb = 1000;	//смещение 200mA
			  sign_Ab = 0;
			  sign_Bb = 1;	//полярность смещения - обратная
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbm);
				    k = CommandKol(k);
			  k = CommandBazaIbsUbm(k);
			  m_iNproxod=2;
			  ready = 0;
			  break;
	  case 2: //t3 - точка 3
			  t3 = aM3[0];
			  //контроль
			  if (fabs(fabs(t2-t1) - fabs(t3-t1))>0.01)
			  { //KillTimer(2);
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Проверьте соединение гнезд и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BMBU для всех диапазонов
			   //устанавливаем начальный № диапазона 
			    m_ir = 1;	//DC
			    m_iBb = 0;	//смещение 0A
				m_iAb = 26; //2nA/cт.
			    sign_Ab = 0;
			    sign_Bb = 0;//полярность смещения - прямая
				m_iRb = 0;	//диапазон ограничения - 40V
			    memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbm));
			    k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbm);
				    k = CommandKol(k);
			    k = CommandBazaIbsUbm(k);
			    m_iNproxod=3;
			    ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Ub от 40V до 1V
				t3 = aM3[0];
				BMBU[m_iRb] = BMBU[m_iRb] +  t3;
				m_iRb++;
				if ((m_iRb)<6)
				{ 
			      memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbm));
			      k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbm);
				    k = CommandKol(k);
			      k = CommandBazaIbsUbm(k);
				  ready = 0;
				}
				else
				{ //установка нуля Ub закончена
				  //KillTimer(2);
				   bAutoSetNull=false;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
					memcpy(&My_buf_Out[k],&K13[0],nK[12]);
					k = k + nK[12];
				   ready = 1;
//				   MessageBox(_T("Коррекция смещения нуля измерителя U канала B закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				}
				break;
	}
return(k);
}

int  CLineDlg::AutoSetupNullIbs()
{//результат в aM3[0]
	int k=0;
	CString str;
	switch(m_iNproxod)
	{ case 0: //t1 - точка 1
			  t1 = aM3[0];
			  m_iBb = 1000;	//смещение 200mA
			  sign_Ab = 0;
			  sign_Bb = 0;	//полярность смещения - прямая
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbs[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbs));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbs);
			  k = CommandBazaIbsUbm(k);
				    k = CommandKol(k);
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM3[0];
			  m_iBb = 1000;	//смещение 200mA
			  sign_Ab = 0;
			  sign_Bb = 1;	//полярность смещения - обратная
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbs[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbs));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbs);
				    k = CommandKol(k);
			  k = CommandBazaIbsUbm(k);
			  m_iNproxod=2;
			  ready = 0;
			  break;
	  case 2: //t3 - точка 3
			  t3 = aM3[0];
			  //контроль
			  if (fabs(fabs(t2-t1) - fabs(t3-t1))>0.01)
			  { //KillTimer(2);
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Проверьте соединение гнезд и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BSBI для всех диапазонов
			   //устанавливаем начальный № диапазона 
			    m_ir = 1;	//DC
			    m_iBs = 1;	//источник Ib
			    m_iBm = 1;	//измеритель Ib
				m_iAb = 0;  //амплитуда ступени - 1A/ст
				m_uSb = 0;
			    m_iBb = 0;	//смещение 0A
			    m_iRb = 5;	//диапазон ограничения - 1V
			    m_iLb = 100;//ограничение - 100%
			    sign_Ab = 0;
			    sign_Bb = 0;//полярность смещения - прямая
			    memcpy(&My_buf_Out[k],&mas_AutoSetupIbs[0],SIZEOF_ARRAY(mas_AutoSetupIbs));
			    k = k + SIZEOF_ARRAY(mas_AutoSetupIbs);
				    k = CommandKol(k);
			    k = CommandBazaIbsIbm(k);
			    m_iNproxod=3;
			  ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Ib от 1A/cт до 2nA/cт
		        t1 = aM3[0];
				BSBI[m_iAb] = BSBI[m_iAb] +  t1;
				m_iAb++;
				if ((m_iAb)<17)//27)
				{ 
			      memcpy(&My_buf_Out[k],&mas_AutoSetupIbs[0],SIZEOF_ARRAY(mas_AutoSetupIbs));
			      k = k + SIZEOF_ARRAY(mas_AutoSetupIbs);
				    k = CommandKol(k);
			      k = CommandBazaIbsIbm(k);
				ready = 0;
				}
				else
				{ //установка нуля Ub закончена
				  //KillTimer(2);
				   bAutoSetNull=false;
//				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
				   memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				   k = k + nK[12];
				   ready = 1;
//				   MessageBox(_T("Коррекция смещения нуля источника I канала B закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				}
				break;
	}
return(k);
}

int CLineDlg::AutoSetupNullUbs()
{//результат в aM3[0]
	int k=0;
	CString str;
	switch(m_iNproxod)
	{ case 0: //t1 - точка 1
			  t1 = aM3[0];
			  m_iBb = 1000;	//смещение 20V
			  sign_Ab = 0;
			  sign_Bb = 0;	//полярность смещения - прямая
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbs[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbs));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbs);
			  k = CommandBazaUbsIbm(k);
				    k = CommandKol(k);
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM3[0];
			  m_iBb = 1000;	//смещение 20V
			  sign_Ab = 0;
			  sign_Bb = 1;	//полярность смещения - обратная
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbs[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbs));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbs);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  m_iNproxod=2;
			  ready = 0;
			  break;
	  case 2: //t3 - точка 3
			  t3 = aM3[0];
			  //контроль
			  if ((fabs(t1-t2)>0.0000000004) || (fabs(t1-t3)>0.0000000004))
			  { //KillTimer(2);
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Устраните утечку тока в канале B и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BSBU для всех диапазонов
			   //устанавливаем начальный № диапазона
		       m_ir = 1;		//DC
			   m_iBs = 0;	//источник Ub
			   m_iBm = 0;	//измеритель Ub
			   m_iAb = 0;	//амплитуда ступени - 2B/ст
			   m_uSb = 0;
			   m_iRb = 26;	//диапазон ограничения - 40nA
			   m_iBb = 0;	//смещение - 0V
			   m_iLb = 100;	//ограничение - 100%
			   sign_Ab = 0;
			   sign_Bb = 0;	//полярность смещения - прямая
			   memcpy(&My_buf_Out[k],&mas_AutoSetupUbs[0],SIZEOF_ARRAY(mas_AutoSetupUbs));
			   k = k + SIZEOF_ARRAY(mas_AutoSetupUbs);
				    k = CommandKol(k);
			   k = CommandBazaUbsUbm(k);
			   m_iNproxod=3;
			  ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Ub от 40V до 1V
				t3 = aM3[0];
				BSBU[m_iAb] =  BSBU[m_iAb] + t3;
				m_iAb++;
				if ((m_iAb)<6)
				{ 
			      memcpy(&My_buf_Out[k],&mas_AutoSetupUbs[0],SIZEOF_ARRAY(mas_AutoSetupUbs));
			      k = k + SIZEOF_ARRAY(mas_AutoSetupUbs);
				    k = CommandKol(k);
			      k = CommandBazaUbsUbm(k);
				ready = 0;
				}
				else
				{ //установка нуля Ub закончена
				  //KillTimer(2);
				   bAutoSetNull=false;
//				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
					memcpy(&My_buf_Out[k],&K13[0],nK[12]);
					k = k + nK[12];
				   ready = 1;
//				   MessageBox(_T("Коррекция смещения нуля источника U канала B закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				}
				break;
	}
return(k);
}

int  CLineDlg::AutoSetupNullIsm()
{//результат в aM4[0]	
	int k=0;
	CString str;
	switch(m_iNproxod)
	{ case 0://t1 - точка 1
			  t1 = aM4[0];
			  m_iBd = 1000;	//смещение 20V
			  sign_Ad = 0;
			  sign_Bd = 0;	//полярность смещения - прямая
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIsm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIsm);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  k = CommandDopUssIsm(k); 
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM4[0];
			  m_iBd = 1000;	//смещение 20V
			  sign_Ad = 0;
			  sign_Bd = 1;	//полярность смещения - обратная
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIsm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIsm);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  k = CommandDopUssIsm(k); 
			  m_iNproxod=2;
			  ready = 0;
			  break;
	  case 2: //t3 - точка 3
			  t3 = aM4[0];
			  //контроль
			  if ((fabs(t1-t2)>0.0000000004) || (fabs(t1-t3)>0.0000000004))
			  { //KillTimer(2);
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Устраните утечку тока в канале S и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BMDI для всех диапазонов
			   //устанавливаем начальный № диапазона
			    m_ir = 1;	//DC
				m_iAd = 0;	//2V/ст
			    m_iBd = 0;	//смещение 0V
			    sign_Ad = 0;
			    sign_Bd = 0;//полярность смещения - прямая
				m_iRd = 0;	//диапазон ограничения - 10mA
				bias_Id = 10;
			    memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIsm));
			    k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIsm);
				    k = CommandKol(k);
			    k = CommandBazaUbsIbm(k);
			    k = CommandDopUssIsm(k); 
			    m_iNproxod=3;
			  ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Is от 10mA до 40nA
				t3 = aM4[0];
				BMDI[m_iRb] = BMDI[m_iRb] +  t3;
				m_iRd++;
				if ((m_iRd+bias_Id)<17)//27)
				{ memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIsm));
			      k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIsm);
				    k = CommandKol(k);
			      k = CommandBazaUbsIbm(k);
			      k = CommandDopUssIsm(k); 
				ready = 0;
				}
				else
				{ //установка нуля Is закончена
				  //KillTimer(2);
				   bAutoSetNull=false;
//				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
				   memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				   k = k + nK[12];
				   ready = 1;
//				   MessageBox(_T("Коррекция смещения нуля измерителя I канала S закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				}
				break;
	}
return(k);
}

int  CLineDlg::AutoSetupNullUsm()
{//результат в aM4[0]	
	int k=0;
	CString str;
	switch(m_iNproxod)
	{ case 0: //t1 - точка 1
			  t1 = aM4[0];
			  m_iBd = 1000;	//смещение 5mA
			  sign_Ad = 0;
			  sign_Bd = 0;	//полярность смещения - прямая
			  m_iDs = 1;	//источник Is
			  m_iDm = 0;	//измеритель Us
			  m_iAd = 0;	//амплитуда ступени - 0.5mA/ст
			  m_iRd = 5;	//диапазон ограничения - 1V
			  m_iLd = 100;	//ограничение - 100%
			  bias_Id = 10;
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUsm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUsm);
  				    k = CommandKol(k);
			  k = CommandBazaIbsUbm(k);
			  k = CommandDopIssUsm(k); 
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM4[0];
			  m_iBd = 1000;	//смещение 5mA
			  sign_Ad = 0;
			  sign_Bd = 1;	//полярность смещения - обратное
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUsm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUsm);
 				    k = CommandKol(k);
 			  k = CommandBazaIbsUbm(k);
			  k = CommandDopIssUsm(k); 
			  ready = 0;
			  m_iNproxod=2;
			  break;
	  case 2: //t3 - точка 3
			  t3 = aM4[0];
			  //контроль
			  if (fabs(fabs(t2-t1) - fabs(t3-t1))>0.01)
			  { //KillTimer(2);
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Проверьте соединение гнезд и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BMDU для всех диапазонов
			   //устанавливаем начальный № диапазона 
			    m_ir = 1;	//DC
			    m_iBd = 0;	//смещение 0A
			    sign_Ad = 0;
			    sign_Bd = 0;//полярность смещения - прямая
				m_iRd = 0;	//диапазон ограничения - 40V
				m_iAd = 16;	//амплитуда ступени - 2nA/ст
			    memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUsm));
			    k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUsm);
 				    k = CommandKol(k);
 			    k = CommandBazaIbsUbm(k);
			    k = CommandDopIssUsm(k); 
			    m_iNproxod=3;
			  ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Ub от 40V до 1V
		        t3 = aM4[0];
				BMDU[m_iRb] = BMDU[m_iRb] +  aM4[0];
				m_iRb++;
				if ((m_iRb)<6)
				{ 
			      memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUsm));
			      k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUsm);
				    k = CommandKol(k);
  			      k = CommandBazaIbsUbm(k);
			      k = CommandDopIssUsm(k); 
				ready = 0;
				}
				else
				{ //установка нуля Ub закончена
				  //KillTimer(2);
				   bAutoSetNull=false;
//				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
					memcpy(&My_buf_Out[k],&K13[0],nK[12]);
					k = k + nK[12];
				   ready = 1;
//				   MessageBox(_T("Коррекция смещения нуля измерителя U канала S закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				}
				break;
	}
return(k);
}

int  CLineDlg::AutoSetupNullIss()
{//результат в aM4[0]	
	int k=0;
	CString str;
	switch(m_iNproxod)
	{ case 0: //t1 - точка 1
			  t1 = aM4[0];
			  m_iBd = 1000;	//смещение 5mA
			  sign_Ad = 0;
			  sign_Bd = 0;	//полярность смещения - прямая
			  m_iDs = 1;	//источник Is
			  m_iDm = 0;	//измеритель Us
			  m_iAd = 0;	//амплитуда ступени - 0.5mA/ст
			  m_iRd = 5;	//диапазон ограничения - 1V
			  m_iLd = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIss[0],SIZEOF_ARRAY(mas_ControlAutoSetupIss));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIss);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  k = CommandDopIssUsm(k);
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM4[0];
			  m_iBd = 1000;	//смещение 5mA
			  sign_Ad = 0;
			  sign_Bd = 1;	//полярность смещения - обратная
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIss[0],SIZEOF_ARRAY(mas_ControlAutoSetupIss));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIss);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  k = CommandDopIssUsm(k);
			  ready = 0;
			  m_iNproxod=2;
			  break;	
	  case 2: //t3 - точка 3
			  t3 = aM4[0];
			  //контроль
			  if (fabs(fabs(t2-t1) - fabs(t3-t1))>0.01)
			  { //KillTimer(2);
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Проверьте соединение гнезд и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BSDI для всех диапазонов
			   //устанавливаем начальный № диапазона 
			    m_ir = 1;	//DC
			    m_iDs = 1;	//источник Is
			    m_iDm = 1;	//измеритель Is
			    m_iBd = 0;	//смещение 0A
			    sign_Ad = 0;
			    sign_Bd = 0;//полярность смещения - прямая
				m_iAd = 0;	//амплитуда ступени - 0.5mA/ст
				m_iRd = 1;	//диапазон ограничения - 20V
			    memcpy(&My_buf_Out[k],&mas_AutoSetupIss[0],SIZEOF_ARRAY(mas_AutoSetupIss));
			    k = k + SIZEOF_ARRAY(mas_AutoSetupIss);
			    k = CommandBazaUbsIbm(k);
			    k = CommandDopIssIsm(k);
			    m_iNproxod=3;
			  ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Is от 10mA до 40nA
				t3 = aM4[0];
				BSDI[m_iAd] = BSDI[m_iAd] +  aM4[0];
				m_iAd++;
				if ((m_iAd+bias_Id)<17)//27)
				{ 
			      memcpy(&My_buf_Out[k],&mas_AutoSetupIss[0],SIZEOF_ARRAY(mas_AutoSetupIss));
			      k = k + SIZEOF_ARRAY(mas_AutoSetupIss);
				    k = CommandKol(k);
			      k = CommandBazaUbsIbm(k);
			      k = CommandDopIssIsm(k);
				ready = 0;
				}
				else
				{ //установка нуля Ub закончена
				  //KillTimer(2);
				   MessageBox(_T("Коррекция смещения измерителя U канала S закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				   bAutoSetNull=false;
				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   SetTextInfo(str);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
					memcpy(&My_buf_Out[k],&K13[0],nK[12]);
					k = k + nK[12];
				   ready = 1;
				}
				break;
	}
return(k);
}

int  CLineDlg::AutoSetupNullUss()
{//результат в aM4[0]		
	int k=0;
	CString str;
	switch(m_iNproxod)
	{ case 0: //t1 - точка 1
			  t1 = aM4[0];
			  m_iBd = 1000;	//смещение - 20V
			  sign_Ad = 0;
			  sign_Bd = 0;	//полярность смещения - прямая
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUss[0],SIZEOF_ARRAY(mas_ControlAutoSetupUss));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUss);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  k = CommandDopUssIsm(k);
			  ready = 0;
			  m_iNproxod=1;
			  break;
	  case 1: //t2 - точка 2
			  t2 = aM4[0];
			  m_iBd = 1000;	//смещение 20V
			  sign_Ad = 0;
			  sign_Bd = 1;	//полярность смещения - обратная
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUss[0],SIZEOF_ARRAY(mas_ControlAutoSetupUss));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUss);
				    k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  k = CommandDopUssIsm(k);
			  m_iNproxod=2;
			  ready = 0;
			  break;
	  case 2: //t3 - точка 3
			  t3 = aM4[0];
			  //контроль
			  if ((fabs(t1-t2)>0.0000000004) || (fabs(t1-t3)>0.0000000004))
			  { //KillTimer(2);
			    m_iNproxod=0;
			    bAutoSetNull=false;
			    MessageBox(_T("Устраните утечку тока в канале S и повторите установку нуля!"),NULL,MB_OK|MB_ICONEXCLAMATION);
			    str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
			    SetTextInfo(str);
				//0.команда К13 "Bыкл. автоустановку нуля"
				k=0;
				memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				k = k + nK[12];
				ready = 1;
			  }
			  else
			  {//OK'
			   //начинаем измерение поправок BSDU для всех диапазонов
			   //устанавливаем начальный № диапазона
			    m_ir = 1;	//DC
			    m_iDs = 0;	//источник Us
			    m_iDm = 0;	//измеритель Us
			    m_iAd = 0;	//амплитуда ступени - 2B/ст
			    m_iRd = 16;	//диапазон ограничения - 40nA
			    m_iBd = 0;	//смещение - 0V
			    m_iLd = 100;	//ограничение - 100%
			    bias_Id = 10;
			    sign_Ad = 0;
			    sign_Bd = 0;//полярность смещения - прямая
			    memcpy(&My_buf_Out[k],&mas_AutoSetupUss[0],SIZEOF_ARRAY(mas_AutoSetupUss));
			    k = k + SIZEOF_ARRAY(mas_AutoSetupUss);
				    k = CommandKol(k);
			    k = CommandBazaUbsIbm(k);
			    k = CommandDopUssUsm(k);
			    m_iNproxod=3;
			  ready = 0;
			  }
			  break;
	   case 3:	// проходим все диапазоны по Is от 10mA до 40nA
				t3 = aM4[0];
				BSDU[m_iAd] = BSDU[m_iAd] + t3;
				m_iAd++;
				if ((m_iAd)<6)
				{ 
			      memcpy(&My_buf_Out[k],&mas_AutoSetupUss[0],SIZEOF_ARRAY(mas_AutoSetupUss));
			      k = k + SIZEOF_ARRAY(mas_AutoSetupUss);
				    k = CommandKol(k);
			      k = CommandBazaUbsIbm(k);
			      k = CommandDopUssUsm(k);
				ready = 0;
				}
				else
				{ //установка нуля Is закончена
				  //KillTimer(2);
				   bAutoSetNull=false;
//				   str = _T("TEST: ") + m_EL + _T("\\") + m_MOD;
				   str = _T("КОРРЕКЦИЯ ЗАКОНЧЕНА!\n");
				   SetTextInfo(str);
				   //команда К13 "Bыкл. автоустановку нуля"
				   k=0;
				   memcpy(&My_buf_Out[k],&K13[0],nK[12]);
				   k = k + nK[12];
			       k = k + nK[30];
				   ready = 1;
//				   MessageBox(_T("Коррекция смещения нуля измерителя I канала S закончена!"),NULL,MB_OK|MB_ICONEXCLAMATION);
				   m_iNproxod=0;
				}
				break;
	}
return(k);
}


void CLineDlg::Calculate()
{	int m=0;
	int i=0;
	int j=0;
    double c=0.001;//0.00000000000001;	// эпсилон
	double r1=0.0;
	double r2=0.0;
	double dprecigion = 0.0001;//0.0001;

	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}

  pM[0]=&pM1[0][0];	// U коллектора
  pM[1]=&pM2[0][0];	// I коллектора
  pM[2]=&pM3[0][0];	// U или I базы
  pM[3]=&pM4[0][0];	// U или I доп.канала
  pM[4]=&pM5[0][0];	// рассчётный массив

  if (m_uFunc >1)
  {//
  //расчёт рассчитываемого массива
 if (m_bS)	
 { 
	for (j=0;j<n_Step;j++)
	{
	  for (m=0;m<n_Mas;m++)
	  {
	    for (i=0;i<n_point;i++)
		{
				if (fabs(pM3[m][i+1+NPOINT*j] - pM3[m][i+NPOINT*j])<c*mas_d_range_meas_bazadop_I[m_iRangLimitBaza]) 
				pM5[m][i+1+NPOINT*j] = (pM2[m][i+1+NPOINT*j] - pM2[m][i+NPOINT*j])/(c*mas_d_range_meas_bazadop_I[m_iRangLimitBaza]);

			else
			pM5[m][i+1+NPOINT*j] = fabs(pM2[m][i+1+NPOINT*j] - pM2[m][i+NPOINT*j]) / fabs(pM3[m][i+1+NPOINT*j]-pM3[m][i+NPOINT*j]);
		}//end for i
	  }//end for j
	}//end for m
  pM5[0][0] = 0.0;
 }

 if (m_bR)
 {
 //  rej_Y = 2;		// R
	for (j=0;j<n_Step;j++)
	{
	  for (m=0;m<n_Mas;m++)
	  {
	    for (i=0;i<n_point;i++)
		{
			if (fabs(pM2[m][i+NPOINT*j])< c*mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI])
			{ 
			pM5[m][i+NPOINT*j] = pM1[m][i+NPOINT*j] / (c*mas_d_rangeKolMeasI[bias_KolMeasI+m_iKolI]);
			}
			else
			{
			pM5[m][i+NPOINT*j] = pM1[m][i+NPOINT*j] / pM2[m][i+NPOINT*j];
			}
		}//end for i
	  }//end for j
	}//end for m
 }//end of  if (m_bR && m_iQue1==COLLECTOR)

  if (m_bBeta || m_bBetai)
 {
	for (j=0;j<n_Step;j++)
	{
	  for (m=0;m<n_Mas;m++)
	  {
	    for (i=0;i<n_point;i++)
		{
			if (fabs(pM3[m][i+NPOINT*j])<c*mas_d_range_meas_bazadop_I[m_iAmplitudeStepBaza])
			pM5[m][i+NPOINT*j] = pM2[m][i+NPOINT*j] / (c*mas_d_range_meas_bazadop_I[m_iAmplitudeStepBaza]);
			else
			pM5[m][i+NPOINT*j] = pM2[m][i+NPOINT*j] / pM3[m][i+NPOINT*j];
		}//end for i
	  }//end for j
	}//end for m
		
 }
	CalculatePar();//расчет Par и поиск min и max для построения осей расчетного графика
	if (m_bBeta) CalculateBetaMax();
	if (m_bBetai) CalculateBetaiMax();
  }//end of if m_uFunc >1
}

void CLineDlg::CalculateBetaMax()
{ //расчет Beta	(массив результатов в pM5[])
  //найти Beta max
	int index = 0;
	int index_m = 0;
	int index_j = 0;
	int i=0;
	int j=0;
	int m=0;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	if (m_bBeta || m_bBetai || m_bS)  n_point =1;	//одно значение на каждой ступеньке при Uc = 5V
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}

	Beta_max = pM5[0][0];
	 for (j=0;j<n_Step;j++)
	   for (m=0;m<n_Mas;m++)
	    for (i=0;i<n_point;i++)
	  if (pM5[m][i+NPOINT*j]>Beta_max) 
	  { Beta_max = pM5[m][i+NPOINT*j];
	    index = i;
		index_m=m;
	    index_j = j;
	  }
	  else continue;
  //найти Ib при Beta max
	if (index<=n_point) Ib_Beta_max = pM3[index_m][index+NPOINT*index_j];
}

void CLineDlg::CalculateBetaiMax()
{ //расчет Betai	
  //найти Betai max
	int index = 0;
	int index_m = 0;
	int index_j = 0;
	int i,m;
	int j=0;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	if (m_bBeta || m_bBetai || m_bS)  n_point =1;	//одно значение на каждой ступеньке при Uc = 5V
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}


	Betai_max = pM5[0][0];
	for (j=0;j<n_Step;j++)
	 for (m=0;m<n_Mas;m++)
	    for (i=0;i<n_point;i++)
	  if (pM5[m][i+NPOINT*j]>Betai_max) 
	  { Betai_max = pM5[m][i+NPOINT*j];
	    index = i;
		index_m = m;
	    index_j = j;
	  }
	  else continue;
  //найти Ib при Betai max
	if (index<=n_point) Ib_Betai_max = pM3[index_m][index+NPOINT*index_j];
}

void CLineDlg::CalculatePar()
{ 
	int index = 0;
	int index_m = 0;
	int index_j = 0;
	int i,j,m;
	int n_step=n_Step;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	int t_0 = 2;//0;//выводим расчетный массив, начиная с 20-ой (.)-ки
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) {n_point = 1; t_0 = 0;}
	int t_s = 0;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	if (m_bBeta || m_bBetai || m_bS)  n_point =1;	//одно значение на каждой ступеньке при Uc = 5V
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}

	
	if (m_bS==1) {t_s = 1;}//t_0=0;}
	double delta_min =0;

if (m_bR && m_uFunc==4)
{//R ot Uc (pM1)
	//найти Par при Uc близким к Ur1
	delta_min = fabs(pM1[0][0]- Ur1);
	for (j=0;j<n_Step;j++)
	  for (m=0;m<n_Mas;m++)
	    for (i=0;i<n_point;i++)
		 if (fabs(pM1[m][i+NPOINT*j]- Ur1)<=delta_min) 
			{ delta_min = fabs(pM1[m][i+NPOINT*j]-Ur1);
			  index_j = j;
			  index_m = m;
			  index = i;
			}
		 else continue;
}


if ((m_bR || m_bBeta || m_bBetai || m_bS) && m_uFunc==2)
{
	//найти Par при Ic близким к Ur1
	delta_min = fabs(pM2[0][0]- Ur1);
	for (j=0;j<n_Step;j++)
	  for (m=0;m<n_Mas;m++)
	    for (i=0;i<n_point;i++)
		 if (fabs(pM2[m][i+NPOINT*j]- Ur1)<=delta_min) 
			{ delta_min = fabs(pM2[m][i+NPOINT*j]-Ur1);
			  index_j = j;
			  index_m = m;
			  index = i;
			}
		 else continue;
}


if ((m_bBeta || m_bBetai || m_bS) && m_uFunc==3)
{
	//найти S при Ug близким к Ur1
	delta_min = fabs(pM3[0][0]- Ur1);
	for (j=0;j<n_Step;j++)
	  for (m=0;m<n_Mas;m++)
	    for (i=0;i<n_point;i++)
		 if (fabs(pM3[m][i+NPOINT*j]- Ur1)<=delta_min) 
			{ delta_min = fabs(pM3[m][i+NPOINT*j]-Ur1);
			  index_j = j;
			  index_m = m;
			  index = i;
			}
		 else continue;
}
//	if (index<=n_Result) Par = pM5[index_m][index+NPOINT*index_j];
	if (index<=n_point) Par = pM5[index_m][index+NPOINT*index_j];

//поиск min и max по Y для рассчётного графика
	//находим минимум pM5[]
	Y_min = pM5[0][0];

 for(j=0;j<n_step;j++)
	for (UINT m=0;m<n_Mas;m++)
		for(i=0;i<n_point;i++)
	  if (pM5[m][i+NPOINT*j]<=Y_min) 
	   Y_min = pM5[m][i+NPOINT*j];
	  else continue;
	//находим максимум pM5[]
	Y_max = pM5[0][0];
 for(j=0;j<n_step;j++)
	for (UINT m=0;m<n_Mas;m++)
		for(i=0;i<n_point;i++)
	  if (pM5[m][i+NPOINT*j]>Y_max) 
	  { Y_max = pM5[m][i+NPOINT*j];
	    index = i;
	  }
	  else continue;
//   double Y_temp = max(fabs(Y_min),fabs(Y_max));//22.12.15
//	   Y_min = Y_temp * (-1.0);
//	   Y_max = Y_temp;
if (Y_max>0.0&& Y_min<0.0) m_iButNull=CENTER;
if (Y_max<=0.0) m_iButNull=RIGHT;
if (Y_min>=0.0) m_iButNull=LEFT;
  UpdateData(false);

}


void	CLineDlg::ControlPar()
{//контроль на пороги параметра 1
if(!m_sPar.IsEmpty() && !m_sArg.IsEmpty())
{
  Flag=FALSE;
	int n_step=n_Step;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
//	if (m_itipimp==TIP_IMP) {n_point = 1;}
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
//	if (m_irazv==DC_POS || m_irazv==DC_NEG) {n_point = n_Result;}//n_step=1;}
//	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE){n_point = uNpointsLupa;}//n_Result;
//	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) {n_point = n_Result;}
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}

	  double x_min=refX[0][0]* pow(10.0,expo_X-po_X);	//в refX, refY координаты точек графика (измеренного)
	  double x_max=refX[0][0]* pow(10.0,expo_X-po_X);
	  double y_min=refY[0][0]* pow(10.0,expo_Y-po_Y);
	  double y_max=refY[0][0]* pow(10.0,expo_Y-po_Y);

//if(!m_sPar.IsEmpty())
//{
//  if (!Flag)
	{//контроль на достижение требуемой величины
    for(int j=0;j<n_step;j++)
    for(int m=0;m<n_Mas;m++)
    for(int i=0;i<n_point;i++)
	{   x_min= min(x_min,refX[m][i+n_point*j]* pow(10.0,expo_X-po_X));
	    x_max= max(x_max,refX[m][i+n_point*j]* pow(10.0,expo_X-po_X));
	    y_min= min(y_min,refY[m][i+n_point*j]* pow(10.0,expo_Y-po_Y));
	    y_max= max(y_max,refY[m][i+n_point*j]* pow(10.0,expo_Y-po_Y));
	}  
/*	switch(m_uFunc)
	{ case 0:  case 2: case 4:
	            if (!m_sUr1.IsEmpty() &&  ((Ur1 < x_min) || (Ur1 > x_max))) Flag = true;
				if (!m_sUr2.IsEmpty() &&  ((Ur2 < x_min) || (Ur2 > x_max))) Flag = true;
				break;
	  case 1:	case 3:
		        if (!m_sUr1.IsEmpty() && ((Ur1 < y_min) || (Ur1 > y_max))) Flag = true;
				if (!m_sUr2.IsEmpty() &&  ((Ur2 < y_min) || (Ur2 > y_max))) Flag = true;
				break;
	  
	}
*/
	switch(m_uFunc)
	{ case 0:   case 4: 
				if (!m_sUr1.IsEmpty() &&  ((Ur1 < x_min) || (Ur1 > x_max))) Flag = true;
				if (!m_sUr2.IsEmpty() &&  ((Ur2 < x_min) || (Ur2 > x_max))) Flag = true;
				break;
	  case 1:
		        if (!m_sUr1.IsEmpty() && ((Ur1 < y_min) || (Ur1 > y_max))) Flag = true;
				if (!m_sUr2.IsEmpty() &&  ((Ur2 < y_min) || (Ur2 > y_max))) Flag = true;
				break;
	  case 3:  case 2:
		  		if (m_sArg==m_sAxisX)
				{
				if (!m_sUr1.IsEmpty() &&  ((Ur1 < x_min) || (Ur1 > x_max))) Flag = true;
				if (!m_sUr2.IsEmpty() &&  ((Ur2 < x_min) || (Ur2 > x_max))) Flag = true;
				}
				if (m_sArg==m_sAxisY)
				{
		        if (!m_sUr1.IsEmpty() && ((Ur1 < y_min) || (Ur1 > y_max))) Flag = true;
				if (!m_sUr2.IsEmpty() &&  ((Ur2 < y_min) || (Ur2 > y_max))) Flag = true;
				}
				break;
	  
	}
	if (Flag) Flag_sboy=true;
  }
  if (!m_sMin.IsEmpty())
  { if (Par<Porog_min) Flag=TRUE;
  }  
  if (!m_sMax.IsEmpty())
  { if (Par>Porog_max) Flag=TRUE;
  }
}
}


void	CLineDlg::InitAxisCalc()
{
	TRACE (L"InitAxisCalc();\n");
//	DWORD tick = ::GetTickCount();
	m_Chart.DisableRefresh(true);
	
	if (m_bR)	 rej_Y = 2;		// R
    if (m_bBeta) rej_Y = 3;		// Beta
	if (m_bBetai) rej_Y = 4;	// Betai
	if (m_bS) rej_Y = 5;		// S

	CChartSerieFamily* pFamily = m_Chart.GetFamily(0);
			pFamily->SetName("Calc");
	if(!m_bCheckKolRejHigh)
	{ 


	m_Chart.m_pAxisBottom->SetAutomatic(false);
	  m_Chart.m_pAxisLeft->SetAutomatic(false);

	  uPrecigion = 1;
//	if ((m_iX==1 || m_iY==1) && (m_iButNull==LEFT || m_iButNull==RIGHT)) uPrecigion = 2;
//      m_iButNull=CENTER;
//	  UpdateData(false);
	  po_X = SubGetStrRazmer(rej_X,expo_X,m_sAxisX);
	  m_Chart.m_pAxisBottom->SetLabelText(strRazmer);

	  ScaleX = strRazmer;
      
      m_Chart.m_pAxisBottom->SetMinMax(cifra_axes[m_iButNull][m_iX][0] * pow(10.0,po_X),
										cifra_axes[m_iButNull][m_iX][1] * pow(10.0,po_X));
	  MinX = cifra_axes[m_iButNull][m_iX][0] * pow(10.0,po_X);
	  MaxX = cifra_axes[m_iButNull][m_iX][1] * pow(10.0,po_X);
      m_Chart.m_pAxisBottom->SetBigTickIncrement(big_tick[m_iButNull][m_iX] * pow(10.0,po_X));
	  StepX = big_tick[m_iButNull][m_iX] * pow(10.0,po_X);
	  
	  SubGetAxes(Y_min,Y_max,&m_iY,&expo_Y);
	  po_Y = SubGetStrRazmer(rej_Y,expo_Y,m_sPar);
	  m_Chart.m_pAxisLeft->SetLabelText(strRazmer);
	  
	  ScaleY = strRazmer;

	  m_Chart.m_pAxisLeft->SetMinMax(cifra_axes[m_iButNull][m_iY][0] * pow(10.0,po_Y),
									cifra_axes[m_iButNull][m_iY][1] * pow(10.0,po_Y));
	  MinY = cifra_axes[m_iButNull][m_iY][0] * pow(10.0,po_Y);
	  MaxY = cifra_axes[m_iButNull][m_iY][1] * pow(10.0,po_Y);
	  m_Chart.m_pAxisLeft->SetBigTickIncrement(big_tick[m_iButNull][m_iY] * pow(10.0,po_Y));
	  StepY = big_tick[m_iButNull][m_iY] * pow(10.0,po_Y);
	}
  if(m_bCheckKolRejHigh)
  {
      m_Chart.m_pAxisBottom->SetMinMax(MinX,MaxX);
      m_Chart.m_pAxisBottom->SetBigTickIncrement(StepX);
	  m_Chart.m_pAxisLeft->SetMinMax(MinY,MaxY);
	  m_Chart.m_pAxisLeft->SetBigTickIncrement(StepY);
	  m_Chart.m_pAxisBottom->SetAutomatic(true);
	  m_Chart.m_pAxisLeft->SetAutomatic(true);
  }

      
	
//	tick = ::GetTickCount() - tick;
	m_Chart.DisableRefresh(false);
}


void	CLineDlg::SetMassivCalcNpoints()
{//отобразить рассчётный массив
  //iX - канал, с которого берётся результат по оси Х 
  //(он же - источник 1-ой очереди): 0-коллектор, 1-база, 2-доп.канал
  //if iX = 0 - массив результатов измерения напряжения на коллекторе,
  //if 1 или 2 - массив X_axis
  //pM5[t] - рассчётный массив, с которого берётся результат для оси Y 
  // pM1[t] - АЦП1 - Uc
  // pM2[t] - АЦП2 - Ic
  // pM3[t] - АЦП3 - Baza
  // pM4[t] - АЦП4 - Dop
	int i=0;
	int j=0;
	UINT m=0;
	int b =0;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	int t_0 = 0;
//	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) {n_point = 1; t_0 = 0;}

//	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE){n_point = uNpointsLupa;t_0 = 1;}//n_Result;
//	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) {n_point = n_Result;t_0 = 1;}
	int t_s = 0;
	if (m_iQue1==COLLECTOR)
	{
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) {n_point = 1; t_0 = 0;}
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) {n_point = uNpointsLupa;t_0 = 1;}//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) {n_point = n_Result;t_0 = 1;}
//	if (m_bBeta || m_bBetai || m_bS)  n_point =1;	//одно значение на каждой ступеньке при Uc = 5V
	if (m_bBeta || m_bBetai || m_bS)  {n_point = 1;t_0=0;}	//одно значение на каждой ступеньке
	}
	if (m_iQue1==BAZA)
	{
		n_point = m_uStepBaza*10;t_0=0;
//		if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) {n_point = m_uStepBaza*10;t_0=0;}//uNpointsLupa;//n_Result;
//		if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) {n_point = n_Result;t_0=0;}
	}
//	if (m_bS==1) {t_s = 1;}//t_0=0;}

//	if (m_bBeta || m_bBetai || m_bS)  {n_point = 1;t_0=0;}	//одно значение на каждой ступеньке
//	if (m_bR) t_0=20;//выводим расчетный массив, начиная с 20-ой (.)-ки
//	int t_k = t_0;

	
	m_Chart.DisableRefresh(true);
 if (bStatusMeas)
 {
	b = m_Chart.GetSeriesCount();
	for(j=0;j<b;j++)
	m_Chart.DeleteSerie(0);
    m_Chart.DeleteFamily(MEAS);
	bStatusMeas = false;
 }
 if (bStatusRef)
 { b = m_Chart.GetSeriesCount();
	for(j=0;j<b;j++)
	m_Chart.DeleteSerie(0);
    m_Chart.DeleteFamily(REF);
	bStatusRef = false;
 }
  if (bStatusDiff)
 { b = m_Chart.GetSeriesCount();
	for(j=0;j<b;j++)
	m_Chart.DeleteSerie(0);
    m_Chart.DeleteFamily(DIFF);
//	bStatusDiff = false;
 }

 if (bStatusCalc)
 {
	b = m_Chart.GetSeriesCount();
	for(j=0;j<b;j++)
	m_Chart.DeleteSerie(0);
    m_Chart.DeleteFamily(CALC);
	bStatusCalc = false;
 }
	CChartSerie*  pGraph;

 for(j=0;j<n_Step;j++)//+t_s
 {  
//    double* pX = new double[n_point*n_Step*n_Mas];
//    double* pY = new double[n_point*n_Step*n_Mas];

	switch(m_iStyleCurve)
	{ case STYLE_LINE:	    pGraph = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraph = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraph = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}
//	if (m_itipimp!=TIP_IMP) pGraph->SetSortPoints(false);
//	if (m_irazv!=IMP_POS && m_irazv!=IMP_NEG) pGraph->SetSortPoints(false);
//	if ((m_irazv!=IMP_POS && m_irazv!=IMP_NEG)||
//		m_nelement==TS) pGraph->SetSortPoints(false);

//	if (m_nelement==TS) 
//	pGraph->SetSortPoints(false);
//        else  pGraph->SetSortPoints(true);

	if (((m_irazv==SYN_POS || m_irazv==SYN_NEG) && !m_bLooping) ||
		(m_irazv==AC && !m_bLooping) ||
		(m_nelement==TS))	pGraph->SetSortPoints(false);
    else  pGraph->SetSortPoints(true);
	
	for (m=0;m<n_Mas;m++)
	{
	for(i=0;i<n_point;i++)//-t_0
	{
	  switch(iX)
	  { case 0: 
				if (m_bKolX==false) pX[i+n_point*m] = pM1[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);//t_0
						     else   pX[i+n_point*m] = pM2[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);//t_0

				break;
//		case 1: 
//		case 2: pX[i+n_point*m] = X_axis[i+t_0+t_s] / pow(10.0,expo_X-po_X); 
//			    break;
		case 1: if(iX == iY) pX[i+n_point*m] = X_axis[i] / pow(10.0,expo_X-po_X); //+t_0
					    else pX[i+n_point*m] = pM3[m][i+NPOINT*j] / pow(10.0,expo_X-po_X); //+t_0
			    break;
		case 2: if(iX == iY) pX[i+n_point*m] = X_axis[i] / pow(10.0,expo_X-po_X);// +t_0
					    else pX[i+n_point*m] = pM4[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);//+t_0
			    break;
	  }
	  pY[i+n_point*m] = pM5[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y);// +t_0
				
/*	if (m_bBeta||m_bBetai||m_bS||m_bR)
	{
	  refX[m][i+n_point*j] = pX[i+n_point*m];//t_0+
	  refY[m][i+n_point*j] = pY[i+n_point*m];//+t_0
	}
*/	}
	}
	pGraph->SetColor(m_colorCurve);							//цвет кривой
	pGraph->SetPoints(pX,pY,(n_point)*n_Mas);//-t_0
//	if (bInversion) 
//	{
//	 TRACE(L"inverted\n");
//	 pGraph->SetXYInverted(true);
//	}

	pGraph->SetName("j-ая кривая");
//	delete[] pX;
//	delete[] pY;
 }//end of for j

	CChartSerieFamily* pFamily = m_Chart.AddFamily();
//	CChartSerieFamily* pFamily = m_Chart.GetFamily(0);
	pFamily->SetColor(m_colorCurve);
	pFamily->SetName("Calc");
	bStatusCalc = true;
	bStatusMeas = true;
	m_Chart.DisableRefresh(false);

}



void CLineDlg::OnKillfocusCombomod() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!m_MOD.IsEmpty())
	{
 	  CComboBox *pcb = (CComboBox*) GetDlgItem(IDC_COMBOMOD);
	  index=pcb->FindStringExact(0,m_MOD);
	  if (index==-1)
	  { //наименование теста не совпадает ни с одним из предлагаемого списка =>
		  str_EL = m_EL;
		  str_MOD = m_MOD;
	    strTestName = str_EL + _T(" \\ ") + str_MOD;
		CString str;
		str.Format(_T("TEST: %s. "),strTestName);
	    SetTextInfo(str);
//        if (MessageBox(_T("Обнаружено новое наименование теста.\nСбросить параметры?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
//		{ SetParamDefault();
//	      UpdateData(FALSE);
//		}
	  }
	}
	
}


void CLineDlg::PolaritySingBaza() 
{
	sign_ampl_baza = 0;	// -
	sign_bias_baza = 0;			
	
	UpdateData();
//	 if (m_irazv<=2) //---!
	 if (m_irazv<=AC) //---!
	 { //развёртка "+"
		if (m_iPolarityStepBaza==1) 
		{ //амплитуда: полярность обратная (=> "-")
			sign_ampl_baza = 1;	// -
	        if (m_iPolarityBiasBaza==0)//смещение: прямая (=> "-")
			  sign_bias_baza = 1;			
		}
		else
		{//амплитуда: полярность прямая (т.е. +)
	        if (m_iPolarityBiasBaza==1)//смещение: обратное (=> "-")
			  sign_bias_baza = 1;			
		}
	 }
//	 if (m_irazv>2)
	 if (m_irazv>AC)
	 { //развёртка "-"
		if (m_iPolarityStepBaza==0) 
		{ //амплитуда: прямая (=> "-")
			sign_ampl_baza = 1;	// -
	        if (m_iPolarityBiasBaza==0)//смещение: прямая (=> "-")
			  sign_bias_baza = 1;			
		}
		else
		{//амплитуда: обратная (т.е. +)
	        if (m_iPolarityBiasBaza==1)//смещение: обратное (=> "-")
			  sign_bias_baza = 1;			
		}
	 }

	UpdateData(FALSE);
}
void CLineDlg::OnSelendokCombopolaritystep() 
{
	// TODO: Add your control notification handler code here
	PolaritySingBaza();
	UpdateData(TRUE);
	ShowRejimChanel();
	UpdateData(FALSE);
	ControlParam();
}

void CLineDlg::OnSelendokCombopolaritystepdop() 
{
	// TODO: Add your control notification handler code here
	PolaritySingDop();
	ControlParam();
	
}

void CLineDlg::PolaritySingDop()
{
    sign_ampl_dop = 0;	// -
	sign_bias_dop = 0;	// -
	UpdateData();
//	 if (m_irazv<=2) //---!
	 if (m_irazv<=AC) //---!
	 { //развёртка "+"
		if (m_iPolarityStepDop==1) 
		{ //амплитуда: обратная (=> "-")
	        sign_ampl_dop = 1;	// -
	        if (m_iPolarityBiasDop==0)//смещение: прямая (=> "-")
			  sign_bias_dop = 1;	// -
		}
		else
		{//амплитуда: прямая (т.е. +)
	        if (m_iPolarityBiasDop==1)//смещение: обратное (=> "-")
			  sign_bias_dop = 1;	// -
		}
	 }
//	 if (m_irazv>2)
	 if (m_irazv>AC)
	 { //развёртка "-"
		if (m_iPolarityStepDop==0) 
		{ //амплитуда: прямая (=> "-")
	        sign_ampl_dop = 1;	// -
	        if (m_iPolarityBiasDop==0)//смещение: прямая (=> "-")
			  sign_bias_dop = 1;	// -
		}
		else
		{//амплитуда: обратная (т.е. +)
	        if (m_iPolarityBiasDop==1)//смещение: обратное (=> "-")
			  sign_bias_dop = 1;	// -
		}
	 }
	UpdateData(FALSE);
}

void CLineDlg::OnSelendokCombopolaritybazabias() 
{
	// TODO: Add your control notification handler code here
	PolaritySingBaza();
	ControlParam();
}

void CLineDlg::OnSelendokCombopolaritydopbias() 
{
	// TODO: Add your control notification handler code here
	PolaritySingDop();
	ControlParam();
}



void CLineDlg::OnKillfocusEditnamepar() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ControlParam();
	UpdateData(FALSE);
	
}

void CLineDlg::OnKillfocusEditnamearg() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ControlParam();
	UpdateData(FALSE);
	
}

void CLineDlg::OnTestRename() 
{
	// TODO: Add your command handler code here
  CTestRenameDlg dlg;
//  pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);//???_???
  if (pModSelected)
  {
	if (!m_pList->IsEmpty()) 
	{ if (i_SelUrov==2)
		{//переименовать тест
//		  if (FindTest(m_EL,m_MOD))
//		  { 
		    dlg.m_EL = m_EL;
		    dlg.m_MOD = m_MOD;
		    dlg.m_PRIM = m_PRIM;
		    UpdateData(FALSE);
		    if (dlg.DoModal()==IDOK)
			{ UpdateData();
			  m_EL = dlg.m_EL;
			  m_MOD = dlg.m_MOD;
			  m_PRIM = dlg.m_PRIM;
			  str_PRIM = m_PRIM;
//			  SaveParam();
			  pModSelected->m_EL		  = m_EL;
			  pModSelected->m_MOD		  = m_MOD;
			  pModSelected->m_PRIM	  = m_PRIM;
			  str_EL = m_EL;
			  str_MOD = m_MOD;
			  strTestName = str_EL + _T(" \\ ") + str_MOD;
			  CString str;
			  str.Format(_T("TEST: %s."),strTestName);//\tПроведите измерение или выберите другой тест!
			  SetTextInfo(str);
			  UpdateData(FALSE);
			  SetModifiedFlag();
		      FConfig();

			}
//		  }
//		  else MessageBox(_T("В списке не найден тест ")+m_MOD+_T(" для элемента ")+m_EL+_T("!"),NULL,MB_OK|MB_ICONERROR);
		}
	  else if (i_SelUrov==1)
	  {//переименовать элемент во всём списке
		MessageBox(_T("Выберите тест для переименования!"),NULL,MB_OK|MB_ICONERROR);
	  }
	}
  }
  else MessageBox(_T("Выберите тест для переименования!"),NULL,MB_OK|MB_ICONERROR);
	
}

void CLineDlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here

	// Кнопка "Таблица"
	//================
//	OpenTabl();
	  akt_button = 105;	
  CMenu menu;
  menu.LoadMenu(IDR_MENUFLOATINGTABL);
  ASSERT(menu);
  CPoint point;
  point.x=RectButtonReset.left+10;
  point.y=RectWnd.bottom-130;
  
 if (bPusk)
  {//если включено измерение, запретить просмотр таблицы результатов и её сохранение
	menu.GetSubMenu(0)->EnableMenuItem(ID_OPENTABL,MF_GRAYED);
	menu.GetSubMenu(0)->EnableMenuItem(ID_TABL_MIN,MF_GRAYED);
	menu.GetSubMenu(0)->EnableMenuItem(ID_TABLRES_SAVE,MF_GRAYED);
  }
    if (bStatusTablRes == stateMin) menu.GetSubMenu(0)->EnableMenuItem(ID_TABL_MIN,MF_GRAYED);
                            else menu.GetSubMenu(0)->EnableMenuItem(ID_OPENTABL,MF_GRAYED);
  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);

/*
//Эмуляция	
  CValEnter dl;

  //отобразить поле ввода
  akt_button = 102;
  value_enter = 0.0;
  dl.m_sValEnter = _T("0.0");
  dl.m_sRazm =_T("A");
  dl.m_bWhat = false;
  UpdateData(FALSE);
  if (dl.DoModal()==IDOK)
  { UpdateData();
    value_enter = GetNumberFromString(dl.m_sValEnter);
  }
*/
}

void CLineDlg::ClearResults()
{//обнулить результаты измерений
	int m=0;
	int i=0;
//	n_Step = 1;		//число шагов (ступенек) на одном цикле измерения
	n_Mas  = 1;		//число массивов (импульсов) на одном цикле измерения
	if (m_irazv==DC_POS || m_irazv==DC_NEG) n_Result = 0;
	else n_Result = mas_n_points[m_idlitimp]* n_Step;	//число измеренных точек
//	m_nCountMas=0;			// текущее значение №-ра массива
//    m_uCountTest  = 0;		// текущее значение №-ра теста
	stup  = 0;				// текущее значение №-ра ступеньки
//	nK41 = 0;					// (для отладки) число посланных команд K41 
    res_gotov = false;			//признак: в DC многократном произошла смена ступеньки (пришла команда  5 от Pic-a)
	for (m=0;m<NMAS;m++)
	{ for (i=0;i<NREZ;i++)
		{	pM1[m][i]=0.0;
			pM2[m][i]=0.0;
			pM3[m][i]=0.0;
			pM4[m][i]=0.0;
			pM5[m][i]=0.0; 
		}
	}
	for (m=0;m<NMAS;m++)
	{ for (i=0;i<NREZ;i++)
		{	mM1[m][i]=0.0;
			mM2[m][i]=0.0;
			mM3[m][i]=0.0;
			mM4[m][i]=0.0;
		}
	}
	for (i=0;i<NREZ;i++)
	{	aM1[i]=0.0;
		aM2[i]=0.0;
		aM3[i]=0.0;
		aM4[i]=0.0;
	}
	bStatusCalc = false;
}

void CLineDlg::ResetResults()
{//сбросить указатели на результаты измерений
	pM[0]=&pM1[0][0];
	pM[1]=&pM2[0][0];
	pM[2]=&pM3[0][0];
	pM[3]=&pM4[0][0];
	pM[4]=&pM5[0][0];
}

BOOL CLineDlg::GetPassWordLine()
{ CFile file;
  int p1,p2;
  CString str  = _T("");
  int iStrLen;
  char* cc;

  BOOL b = file.Open((LPCTSTR)filePassWord,CFile::modeCreate|CFile::modeNoTruncate |CFile::modeReadWrite);
	if (b)
	{//if b
	 file.SeekToBegin();	
	 DWORD  l = file.GetLength();
	 DWORD* lp = new DWORD[l];
	 file.Read(lp,l);
	 CString strFile((LPCTSTR)&lp[0],l);
	 delete[] lp;
	 if (!strFile.IsEmpty())
	 {	   //m_sparol	
		   p1=strFile.Find(L'=',0);
		   p2=strFile.Find(L'\n',p1);
		   m_sparol = strFile.Mid(p1+1,p2-p1-2);
		   //m_sProduct	
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   m_sProduct = strFile.Mid(p1+1,p2-p1-2);
		   //m_sPart	
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   m_sPart = strFile.Mid(p1+1,p2-p1-2);
		   //m_sPlast	
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   m_sPlast = strFile.Mid(p1+1,p2-p1-2);
		   //m_sPoint	
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   m_sPoint = strFile.Mid(p1+1,p2-p1-2);

		   //m_colorFon,m_colorSetka,m_colorText,m_colorTangent,m_colorCurve,m_colorRef;

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_colorFon = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_colorSetka = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();

		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_colorText = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
		   
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_colorTangent = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
		   
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_colorCurve = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
		   
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   m_colorRef = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
	 }
	 else 
	 { m_sparol = _T("admin");
       str.Format(_T("m_sparol=%s\r\n"),m_sparol);
	   DWORD l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
		m_sProduct = _T("");
       str.Format(_T("m_sProduct=%s\r\n"),m_sProduct);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
		m_sPart = _T("");
       str.Format(_T("m_sPart=%s\r\n"),m_sPart);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
		m_sPlast = _T("");
       str.Format(_T("m_sPlast=%s\r\n"),m_sPlast);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
		m_sPoint = _T("");
       str.Format(_T("m_sPoint=%s\r\n"),m_sPoint);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   //m_colorFon,m_colorSetka,m_colorText,m_colorTangent,m_colorCurve,m_colorRef;
	   str.Format(_T("m_colorFon=%d\r\n"),m_colorFon);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorSetka=%d\r\n"),m_colorSetka);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorText=%d\r\n"),m_colorText);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorTangent=%d\r\n"),m_colorTangent);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorCurve=%d\r\n"),m_colorCurve);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorRef=%d\r\n"),m_colorRef);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	 }
	 file.Close();
	}
  return b;
}

BOOL CLineDlg::GetKoefPsw()
{ BOOL b;
  CFile file;
  int p1,p2;
  CString str  = _T("");
  int iStrLen;
  char* cc;
  DWORD l;
  int i;

   b = file.Open((LPCTSTR)fileKoefPsw,CFile::modeCreate|CFile::modeNoTruncate |CFile::modeReadWrite);
//   b = file.Open((LPCTSTR)fileKoefPsw,CFile::modeNoTruncate |CFile::modeRead);
	if (b)
	{//if b
	 file.SeekToBegin();	
	 l = file.GetLength();
	 DWORD* lp = new DWORD[l];
	 file.Read(lp,l);
	 CString strFile((LPCTSTR)&lp[0],l);
	 delete[] lp;
	 if (!strFile.IsEmpty())
	 {//заводской номер
		   p1=strFile.Find(L'=',0);
		   p2=strFile.Find(L';',p1);
		   strpsw = strFile.Mid(p1+1,p2-p1-1); 
               strpsw.TrimLeft();
			   strpsw.TrimRight();
	  //модификация ИППП:
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L';',p1);
		   str=strFile.Mid(p1+1,p2-p1-1);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   n_IPPP = (BYTE)atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
	  //дата последней калибровки
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L';',p1);
           strdatekalibr=strFile.Mid(p1+1,p2-p1-1);
		       strdatekalibr.TrimLeft();
			   strdatekalibr.TrimRight();
	  //===============================================
      TRACE(_T("Загрузка калибровочных коэффициентов.\n"));
	  //коллектор (источник)
	  //номинальное значение кода сети (1 байт) - общее для всех диапазонов
		   p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L';',p1);
		   str=strFile.Mid(p1+1,p2-p1-1);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   nominal = (BYTE)atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
	  //максимальный код ЦАПа Da для каждого диапазона (5 диапазонов) 
	  //максимальный код ЦАПа Dz (1 байт) 
	  //калибровочные коэффициенты
		   //КОЛЛЕКТОР
		   for (i=0;i<13;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BMCU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<13;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KMCU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<MAXRANGEIKOL;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BMCI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<MAXRANGEIKOL;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BMCI_DUBL[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<MAXRANGEIKOL;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KMCI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<MAXRANGEIKOL;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KMCI_DUBL[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   //БАЗА
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BSBU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KSBU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<27;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BSBI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<27;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KSBI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BLBU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KLBU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<27;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BLBI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<27;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KLBI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BMBU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KMBU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<27;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BMBI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<27;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KMBI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   
		   //ПОДЛОЖКА
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BSDU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KSDU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<17;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BSDI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<17;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KSDI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BLDU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KLDU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<17;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BLDI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<17;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KLDI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BMDU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<6;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KMDU[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<17;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 BMDI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
		   for (i=0;i<17;i++)
		   { p1=strFile.Find('=',p2);
             p2=strFile.Find(';',p1);
             str=strFile.Mid(p1+1,p2-p1-1);
		     iStrLen = str.GetLength();
		     cc = new char[iStrLen];
		     WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
			 KMDI[i]=atof(cc);
		     delete[] cc;
		     str.ReleaseBuffer();
		   }
	   //......
	 }
	 else 
	 { 
		CModificationDlg dlg;
		 strpsw = "000";
		 n_IPPP = 0;
		 dlg.m_sSerNum = strpsw;
		 dlg.m_inIPPP = n_IPPP;
		 UpdateData(FALSE);
		if (dlg.DoModal()==IDOK)
		{ UpdateData();
		 strpsw = dlg.m_sSerNum;
		 n_IPPP = dlg.m_inIPPP;
		}
		 //заводской номер прибора
       str.Format(_T("strpsw=%s;\r\n"),strpsw);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //модификация ИППП:
	   str.Format(_T("n_IPPP=%d;\r\n"),n_IPPP);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //дата последней калибровки
       str.Format(_T("strdatekalibr=%s;\r\n"),strdatekalibr);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //коллектор (источник)
	   //номинальное значение кода сети (1 байт) - общее для всех диапазонов
	   str.Format(_T("nominal=%d;\r\n"),nominal);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
   
	 //калибровочные коэффициенты
	   //КОЛЛЕКТОР
	   str=_T("[COLLECTOR]\r\n[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<13;i++)
	   { str.Format(_T("BMCU[%d]=%.5e;\r\n"),i,BMCU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<13;i++)
	   { str.Format(_T("KMCU[%d]=%.5e;\r\n"),i,KMCU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("BMCI[%d]=%.5e;\r\n"),i,BMCI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("BMCI_DUBL[%d]=%.5e;\r\n"),i,BMCI_DUBL[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("KMCI[%d]=%.5e;\r\n"),i,KMCI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("KMCI_DUBL[%d]=%.5e;\r\n"),i,KMCI_DUBL[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   //БАЗА
	   str=_T("[BASE]\r\n[SOURCE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BSBU[%d]=%.5e;\r\n"),i,BSBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KSBU[%d]=%.5e;\r\n"),i,KSBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[SOURCE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BSBI[%d]=%.5e;\r\n"),i,BSBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KSBI[%d]=%.5e;\r\n"),i,KSBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION U]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BLBU[%d]=%.5e;\r\n"),i,BLBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KLBU[%d]=%.5e;\r\n"),i,KLBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION I]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BLBI[%d]=%.5e;\r\n"),i,BLBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KLBI[%d]=%.5e;\r\n"),i,KLBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BMBU[%d]=%.5e;\r\n"),i,BMBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KMBU[%d]=%.5e;\r\n"),i,KMBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BMBI[%d]=%.5e;\r\n"),i,BMBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KMBI[%d]=%.5e;\r\n"),i,KMBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   //ПОДЛОЖКА
	   str=_T("[SUBSTRATE]\r\n[SOURCE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BSDU[%d]=%.5e;\r\n"),i,BSDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KSDU[%d]=%.5e;\r\n"),i,KSDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[SOURCE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BSDI[%d]=%.5e;\r\n"),i,BSDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KSDI[%d]=%.5e;\r\n"),i,KSDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION U]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BLDU[%d]=%.5e;\r\n"),i,BLDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KLDU[%d]=%.5e;\r\n"),i,KLDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION I]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BLDI[%d]=%.5e;\r\n"),i,BLDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KLDI[%d]=%.5e;\r\n"),i,KLDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BMDU[%d]=%.5e;\r\n"),i,BMDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KMDU[%d]=%.5e;\r\n"),i,KMDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BMDI[%d]=%.5e;\r\n"),i,BMDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KMDI[%d]=%.5e;\r\n"),i,KMDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }

	 }
	 file.Close();
	}
  return b;
}

BOOL CLineDlg::WriteKoefToPsw()
{ BOOL b;
  CFile file;
  CString str  = _T("");
  DWORD l;
  int i;

   b = file.Open((LPCTSTR)fileKoefPsw,CFile::modeCreate|CFile::modeWrite);
	if (b)
	{//if b
	 file.SeekToBegin();	
	   //заводской номер прибора
       str.Format(_T("strpsw=%s;\r\n"),strpsw);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //модификация ИППП:
	   str.Format(_T("n_IPPP=%d;\r\n"),n_IPPP);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //дата последней калибровки
       str.Format(_T("strdatekalibr=%s;\r\n"),strdatekalibr);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   //коллектор (источник)
	   //номинальное значение кода сети (1 байт) - общее для всех диапазонов
	   str.Format(_T("nominal=%d;\r\n"),nominal);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	 //калибровочные коэффициенты
	   //КОЛЛЕКТОР
	   str=_T("[COLLECTOR]\r\n[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<13;i++)
	   { str.Format(_T("BMCU[%d]=%.5e;\r\n"),i,BMCU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<13;i++)
	   { str.Format(_T("KMCU[%d]=%.5e;\r\n"),i,KMCU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("BMCI[%d]=%.5e;\r\n"),i,BMCI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("BMCI_DUBL[%d]=%.5e;\r\n"),i,BMCI_DUBL[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("KMCI[%d]=%.5e;\r\n"),i,KMCI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<MAXRANGEIKOL;i++)
	   { str.Format(_T("KMCI_DUBL[%d]=%.5e;\r\n"),i,KMCI_DUBL[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   //БАЗА
	   str=_T("[BASE]\r\n[SOURCE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BSBU[%d]=%.5e;\r\n"),i,BSBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KSBU[%d]=%.5e;\r\n"),i,KSBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[SOURCE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BSBI[%d]=%.5e;\r\n"),i,BSBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KSBI[%d]=%.5e;\r\n"),i,KSBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION U]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BLBU[%d]=%.5e;\r\n"),i,BLBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KLBU[%d]=%.5e;\r\n"),i,KLBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION I]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BLBI[%d]=%.5e;\r\n"),i,BLBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KLBI[%d]=%.5e;\r\n"),i,KLBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BMBU[%d]=%.5e;\r\n"),i,BMBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KMBU[%d]=%.5e;\r\n"),i,KMBU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("BMBI[%d]=%.5e;\r\n"),i,BMBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<27;i++)
	   { str.Format(_T("KMBI[%d]=%.5e;\r\n"),i,KMBI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   //ПОДЛОЖКА
	   str=_T("[SUBSTRATE]\r\n[SOURCE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BSDU[%d]=%.5e;\r\n"),i,BSDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KSDU[%d]=%.5e;\r\n"),i,KSDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[SOURCE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BSDI[%d]=%.5e;\r\n"),i,BSDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KSDI[%d]=%.5e;\r\n"),i,KSDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION U]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BLDU[%d]=%.5e;\r\n"),i,BLDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KLDU[%d]=%.5e;\r\n"),i,KLDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[LIMITATION I]\r\n[BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BLDI[%d]=%.5e;\r\n"),i,BLDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KLDI[%d]=%.5e;\r\n"),i,KLDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE U][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("BMDU[%d]=%.5e;\r\n"),i,BMDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<6;i++)
	   { str.Format(_T("KMDU[%d]=%.5e;\r\n"),i,KMDU[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[MEASURE I][BIAS]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("BMDI[%d]=%.5e;\r\n"),i,BMDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	   str=_T("[COEFFICIENT]\r\n");
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
	   for (i=0;i<17;i++)
	   { str.Format(_T("KMDI[%d]=%.5e;\r\n"),i,KMDI[i]);
	     l = str.GetLength();
	     file.Write((LPCTSTR)str,l*2);
	   }
	 file.Close();
	}
	return b;
}

void CLineDlg::SetKoefPswToPic() 
{
  TRACE(_T("##############SetKoefPswToPic()###############!\n"));
	int i=0;
	int j=1;		 //индекс в команде K29
	int k=0;		 //счетчик числа передаваемых байт теста
//	int p = 0;

	::WaitForSingleObject(hMutexO,INFINITE);
    NumO = k;		//кол-во байт для передачи
	::ReleaseMutex(hMutexO);

	for(i=0;i<NBUF;i++)
	My_buf_Out[i]=0;
  
	K29[j]= n_IPPP;		//модификация прибора
	j++;
	K29[j]= nominal;	//номинальное значение кода сети (1 байт) - общее для всех диапазонов
	j++;
//-------------------------------
	memcpy(&My_buf_Out[k],&K29[0],nK[28]);//пока 13 байт//15
	k = nK[28];

	K34[1] = bZapusk;		// вид запуска: 0 - "внутренний" (по кнопке "Пуск"), 1 - "внешний" (по внешнему сигналу)
	memcpy(&My_buf_Out[k],&K34[0],nK[33]);
	k = k + nK[33];

	My_buf_Out[k] = '\0';	//конец передачи
	k = k + 1;
//-------------------------------
  ::WaitForSingleObject(hMutexO,INFINITE);
    NumO = k;									//кол-во байт для передачи
    bufOut[0]=(BYTE)NumO+1;						//кол-во байт для передачи + 1 байт с количеством
    memcpy(&bufOut[1],&My_buf_Out[0],bufOut[0]);//данные
  ::ReleaseMutex(hMutexO);
  PurgeComm(hCom2,PURGE_TXCLEAR|PURGE_RXCLEAR);
  SetEvent(hEventComOut);//+++++++++++++++++++++
}

BOOL CLineDlg::SetPassWordLine()
{ CFile file;
  CString str  = _T("");
  CString stre = _T("");
	BOOL b = file.Open((LPCTSTR)filePassWord,CFile::modeCreate|CFile::modeWrite);
	if (b)
	{ 
       str.Format(_T("m_sparol=%s\r\n"),m_sparol);
	   DWORD l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

       str.Format(_T("m_sProduct=%s\r\n"),m_sProduct);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

       str.Format(_T("m_sPart=%s\r\n"),m_sPart);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

       str.Format(_T("m_sPlast=%s\r\n"),m_sPlast);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

       str.Format(_T("m_sPoint=%s\r\n"),m_sPoint);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorFon=%d\r\n"),m_colorFon);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorSetka=%d\r\n"),m_colorSetka);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorText=%d\r\n"),m_colorText);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorTangent=%d\r\n"),m_colorTangent);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorCurve=%d\r\n"),m_colorCurve);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);

	   str.Format(_T("m_colorRef=%d\r\n"),m_colorRef);
	   l = str.GetLength();
	   file.Write((LPCTSTR)str,l*2);
 
      file.Close();
	}
	return b;
}
		
void CLineDlg::SetZapuskToPic() 
{
	  int k=0;
	  K34[1] = bZapusk;		// вид запуска: 0 - "внутренний" (по кнопке "Пуск"), 1 - "внешний" (по внешнему сигналу)
	  memcpy(&My_buf_Out[k],&K34[0],nK[33]);
	  k = k + nK[33];

	  My_buf_Out[k] = '\0';	//конец передачи
	  k = k + 1;
     ::WaitForSingleObject(hMutexO,INFINITE);
      NumO = k;									//кол-во байт для передачи
      bufOut[0]=(BYTE)NumO+1;						//кол-во байт для передачи + 1 байт с количеством
      memcpy(&bufOut[1],&My_buf_Out[0],bufOut[0]);//данные
     ::ReleaseMutex(hMutexO);
      PurgeComm(hCom2,PURGE_TXCLEAR|PURGE_RXCLEAR);
	  ready = 0;
      SetEvent(hEventComOut);//+++++++++++++++++++++
}

void CLineDlg::OnZapuskInternal() 
{
	// TODO: Add your command handler code here
//  if (bPusk) 
//	  SetTextInfo(_T("WARNING:	Идёт измерение! Дождитесь окончания или нажмите \"Стоп\"."));
//  else 
  {   bZapusk = FALSE;
//	  SetPassWordLine();
//-------------------------------
	  SetZapuskToPic();
//-------------------------------
  }
}

void CLineDlg::OnZapuskExternal() 
{
	// TODO: Add your command handler code here
//  if (bPusk)  
//	  SetTextInfo(_T("WARNING:	Идёт измерение! Дождитесь окончания или нажмите \"Стоп\"."));
//  else
  {  bZapusk = TRUE;
     bMeas = false;
// 	 SetPassWordLine();
//-------------------------------
	 SetZapuskToPic();
//-------------------------------
  }
	
}

void CLineDlg::OnCurveLine() 
{
	// TODO: Add your command handler code here
 if (!bStatusRef && !bStatusCalc)
 {
    m_iStyleCurve = STYLE_LINE;
	if (nCheckParam)
	{ if (m_bTangent)
	  { m_bTangent=false;
	    m_Chart.DrawSecant(false);
	  }
	  m_Chart.DeleteMarker(m_uN);
	  m_Chart.DeleteMarker(m_uN);
	  m_iMarker = MARKER0;
	}
		int b = m_Chart.GetSeriesCount();
		TRACE(L"Pusk: до удаления GetSeriesCount = %d\n",b);
		for(int j=0;j<b;j++)
		m_Chart.DeleteSerie(0);

//		b = m_Chart.GetSeriesCount();
//		TRACE(L"Pusk: после удаления GetSeriesCount = %d\n",b);
		b = m_Chart.GetFamiliesCount();
		TRACE(L"Pusk: до удаления GetFamiliesCount = %d\n",b);
		for(j=0;j<b;j++)
	    m_Chart.DeleteFamily(0);
//		b = m_Chart.GetFamiliesCount();
//		TRACE(L"Pusk: после удаления GetFamiliesCount = %d\n",b);

	bStatusMeas = false;
	CChartSerie*  pGraph;
 for (j=0;j<n_Step;j++)
 { 	
    switch(m_iStyleCurve)
	{ case STYLE_LINE:	    pGraph = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraph = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraph = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}

 }
	CChartSerieFamily* pFamily = m_Chart.AddFamily();
	pFamily->SetColor(m_colorCurve);
	pFamily->SetName("Meas");


//	if (m_bButInversion) SetMassivNpoint(true);
//					else SetMassivNpoint(false);
	if (nCheckParam) 
	{ ResetMarkers();
//	  DisplayMarker();
//	  DisplaySecant();
	}
	if (!bPusk) SetMassivNpoint(true);
 }	
}

void CLineDlg::OnCurveLinewithpoint() 
{
	// TODO: Add your command handler code here
 if (!bStatusRef && !bStatusCalc)
 {
 	m_iStyleCurve = STYLE_LINEPOINT;
	if (nCheckParam)
	{ if (m_bTangent)
	  { m_bTangent=false;
	    m_Chart.DrawSecant(false);
	  }
	  m_Chart.DeleteMarker(m_uN);
	  m_Chart.DeleteMarker(m_uN);
	  m_iMarker = MARKER0;
	}
		int b = m_Chart.GetSeriesCount();
		TRACE(L"Pusk: до удаления GetSeriesCount = %d\n",b);
		for(int j=0;j<b;j++)
		m_Chart.DeleteSerie(0);

//		b = m_Chart.GetSeriesCount();
//		TRACE(L"Pusk: после удаления GetSeriesCount = %d\n",b);
		b = m_Chart.GetFamiliesCount();
		TRACE(L"Pusk: до удаления GetFamiliesCount = %d\n",b);
		for(j=0;j<b;j++)
	    m_Chart.DeleteFamily(0);
//		b = m_Chart.GetFamiliesCount();
//		TRACE(L"Pusk: после удаления GetFamiliesCount = %d\n",b);

	bStatusMeas = false;
	CChartSerie*  pGraph;
 for (j=0;j<n_Step;j++)
 { 	
    switch(m_iStyleCurve)
	{ case STYLE_LINE:	    pGraph = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraph = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraph = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}

 }
	CChartSerieFamily* pFamily = m_Chart.AddFamily();
	pFamily->SetColor(m_colorCurve);
	pFamily->SetName("Meas");

//	if (m_bButInversion) SetMassivNpoint(true);
//					else SetMassivNpoint(false);
	if (nCheckParam) 
	{ ResetMarkers();
//	  DisplayMarker();
//	  DisplaySecant();
	}
	if (!bPusk) SetMassivNpoint(true);
 }
}

void CLineDlg::OnCurvePoint() 
{
	// TODO: Add your command handler code here
 if (!bStatusRef && !bStatusCalc)
 {
	if (nCheckParam)
	{ if (m_bTangent)
	  { m_bTangent=false;
	    m_Chart.DrawSecant(false);
	  }
	  m_Chart.DeleteMarker(m_uN);
	  m_Chart.DeleteMarker(m_uN);
	  m_iMarker = MARKER0;
	}
	m_iStyleCurve = STYLE_POINT;
		int b = m_Chart.GetSeriesCount();
		TRACE(L"Pusk: до удаления GetSeriesCount = %d\n",b);
		for(int j=0;j<b;j++)
		m_Chart.DeleteSerie(0);

//		b = m_Chart.GetSeriesCount();
//		TRACE(L"Pusk: после удаления GetSeriesCount = %d\n",b);
		b = m_Chart.GetFamiliesCount();
		TRACE(L"Pusk: до удаления GetFamiliesCount = %d\n",b);
		for(j=0;j<b;j++)
	    m_Chart.DeleteFamily(0);
//		b = m_Chart.GetFamiliesCount();
//		TRACE(L"Pusk: после удаления GetFamiliesCount = %d\n",b);

	bStatusMeas = false;
	CChartSerie*  pGraph;
 for (j=0;j<n_Step;j++)
 { 	
    switch(m_iStyleCurve)
	{ case STYLE_LINE:	    pGraph = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraph = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraph = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}

 }
	CChartSerieFamily* pFamily = m_Chart.AddFamily();
	pFamily->SetColor(m_colorCurve);
	pFamily->SetName("Meas");

//	if (m_bButInversion) SetMassivNpoint(true);
//					else SetMassivNpoint(false);
	if (nCheckParam) 
	{ ResetMarkers();
//	  DisplayMarker();
//	  DisplaySecant();
	}
	if (!bPusk) SetMassivNpoint(true);
 }
}

void CLineDlg::OnColorFon() 
{
	// TODO: Add your command handler code here
	CColorDialog ColorDialog;
	if (ColorDialog.DoModal()==IDOK)
	{
	  m_colorFon = ColorDialog.GetColor();
	  SetPassWordLine();
	  m_Chart.DisableRefresh(true); 
	  m_Chart.SetBackColor(m_colorFon);
	  m_Chart.DisableRefresh(false); 
	}	
	
}

void CLineDlg::OnColorCurve() 
{
	// TODO: Add your command handler code here
	CColorDialog ColorDialog;
	CChartSerie*  pGraph;
	if (ColorDialog.DoModal()==IDOK)
	{
	  m_colorCurve = ColorDialog.GetColor();
	  SetPassWordLine();
	
	  m_Chart.DisableRefresh(true);
	  CChartSerieFamily* pFamily = m_Chart.GetFamily(MEAS);
	    int n = m_Chart.GetSeriesCount();
	    int f = m_Chart.GetFamiliesCount();
		int k = (int)(n/f);
		for (int i=0;i<k;i++)
		{ pGraph = m_Chart.GetSerie(i);
	      pGraph->SetColor(m_colorCurve);
		}
	    pFamily->SetColor(m_colorCurve);
	  m_Chart.DisableRefresh(false); 

//	  m_penCurve.DeleteObject();
//    m_penCurve.CreatePen(PS_SOLID,1,m_colorCurve);
//    InvalidateRect(m_RectWndGraf,FALSE);
	if (nCheckParam)
	{ if (m_bTangent)
	  { m_bTangent=false;
	    m_Chart.DrawSecant(false);
	  }
	  m_Chart.DeleteMarker(m_uN);
	  m_Chart.DeleteMarker(m_uN);
	  m_iMarker = MARKER0;
	}
//	if (m_bButInversion) SetMassivNpoint(true);
//					else SetMassivNpoint(false);
	if (nCheckParam) 
	{ ResetMarkers();
//	  DisplayMarker();
//	  DisplaySecant();
	}
	}	
	
}

void CLineDlg::OnColorFont() 
{
	// TODO: Add your command handler code here
	CColorDialog ColorDialog;
	if (ColorDialog.DoModal()==IDOK)
	{
	  m_colorText = ColorDialog.GetColor();
	  SetPassWordLine();
	  m_Chart.DisableRefresh(true); 
      m_Chart.m_pAxisBottom->SetLabelTextColor(m_colorText);  //цвет надписи по оси Х
	  m_Chart.m_pAxisLeft->SetLabelTextColor(m_colorText);	//цвет надписи по оси Y
	  m_Chart.DisableRefresh(false); 
//    InvalidateRect(m_RectWndGraf,FALSE);
	}	
	
}

void CLineDlg::OnColorRef() 
{
	// TODO: Add your command handler code here
	CColorDialog ColorDialog;
 if (!bStatusRef)
 {	if (ColorDialog.DoModal()==IDOK)
	{
	  m_colorRef = ColorDialog.GetColor();
	  SetPassWordLine();
	}
 }
 else //if (bStatusRef)
 {
	if (ColorDialog.DoModal()==IDOK)
	{
	  m_colorRef = ColorDialog.GetColor();
	  SetPassWordLine();
	  m_Chart.DisableRefresh(true); 
	  CChartSerieFamily* pFamily = m_Chart.GetFamily(REF);
	    int n = m_Chart.GetSeriesCount();
	    int f = m_Chart.GetFamiliesCount();//f=2
		int k = (int)(n/f);
		for (int i=k;i<k+k;i++)
		{ pGraphRef = m_Chart.GetSerie(i);
	      pGraphRef->SetColor(m_colorRef);
		}
	    pFamily->SetColor(m_colorRef);

	  m_Chart.DisableRefresh(false); 

//    InvalidateRect(m_RectWndGraf,FALSE);
	if (nCheckParam)
	{ if (m_bTangent)
	  { m_bTangent=false;
	    m_Chart.DrawSecant(false);
	  }
	  m_Chart.DeleteMarker(m_uN);
	  m_Chart.DeleteMarker(m_uN);
	  m_iMarker = MARKER0;
	}
	if (nCheckParam) 
	{ ResetMarkers();
	}
	}	
 }
}

void CLineDlg::OnColorSetka() 
{
	// TODO: Add your command handler code here
	CColorDialog ColorDialog;
	if (ColorDialog.DoModal()==IDOK)
	{
  	  m_colorSetka = ColorDialog.GetColor();
	  SetPassWordLine();
	  m_Chart.DisableRefresh(true); 
	  m_Chart.m_pAxisBottom->SetGridColor(m_colorSetka);	    //цвет сетки по оси Х
	  m_Chart.m_pAxisLeft->SetGridColor(m_colorSetka);		//цвет сетки по оси Y
	  m_Chart.m_pAxisBottom->SetTextColor(m_colorSetka);
	  m_Chart.m_pAxisLeft->SetTextColor(m_colorSetka);
	  m_Chart.SetFrameColor(m_colorSetka);
	  m_Chart.DisableRefresh(false); 
//	  m_penSetka_DASH.DeleteObject();
//    m_penSetka_DASH.CreatePen(PS_DASH,1,m_colorSetka);
//	  m_penSetka_SOLID.DeleteObject();
//    m_penSetka_SOLID.CreatePen(PS_SOLID,1,m_colorSetka);
//    InvalidateRect(m_RectWndGraf,FALSE);
	}	
	
}

void CLineDlg::OnColorTangent() 
{
	// TODO: Add your command handler code here
	CColorDialog ColorDialog;
	if (ColorDialog.DoModal()==IDOK)
	{
	  m_colorTangent = ColorDialog.GetColor();
	  SetPassWordLine();
	  m_Chart.DisableRefresh(true); 
	  m_Chart.m_pSecant->SetColor(m_colorTangent);			//цвет секущей
	  m_Chart.DisableRefresh(false); 
//	  m_penTangent.DeleteObject();
//    m_penTangent.CreatePen(PS_SOLID,1,m_colorTangent);
//    InvalidateRect(m_RectWndGraf,FALSE);
	}	
	
}

LRESULT CLineDlg::OnRefresh(WPARAM wParam,LPARAM lParam)
{
	    	    m_Chart.DisableRefresh(true);
		  int NearestSerieIndex = m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->GetRelatedSerieIndex();
	DisplayMarker();
	DisplaySecant();
////	    	    m_Chart.RefreshCtrl();
//		    m_Chart.DrawMarkers();
	    	    m_Chart.DisableRefresh(false);

	return NULL;
}


void CLineDlg::OnButtonLeft() 
{//текущий маркер влево
	// TODO: Add your control notification handler code here
 double XMarker, YMarker;
 CPoint p;
 CPoint point;
 double X=0.0;
 double Y=0.0;

 if (nCheckParam)
 {
	m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->GetValues(XMarker, YMarker);
	XMarker = XMarker * pow(10.0,expo_X-po_X);
	YMarker = YMarker * pow(10.0,expo_Y-po_Y);
	if (XMarker>X_min && YMarker>Y_min)
	{	m_Chart.MarkerMoveLeft();
		DisplayMarker();
		if (m_bTangent) DisplaySecant();
	}
	else
	{ if (n_Step>1)
		{
		  int NearestSerieIndex = m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->GetRelatedSerieIndex();
		  if (NearestSerieIndex > 0)
		  { NearestSerieIndex = NearestSerieIndex-1;
		    m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->SetRelatedSerie(NearestSerieIndex);		
			//устанавливаем позицию маркера
		    CChartSerie* pS = (CChartSerie*)m_Chart.GetSerie(NearestSerieIndex);
			pS ->ValueToScreen(X,Y,point);
			p = m_Chart.GetNearestSeriePoint(NearestSerieIndex, point,X,Y);	
			m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->SetPosition(X,Y);		
		    m_Chart.m_pSecant->SetPoints(X,Y);
    	    m_Chart.RefreshCtrl();
		    m_Chart.DrawMarkers();
			DisplayMarker();
			if (m_bTangent) DisplaySecant();
		  }
		}
	}
 }
}

void CLineDlg::OnButtonRight() 
{//текущий маркер вправо
	// TODO: Add your control notification handler code here
 double XMarker, YMarker;
 CPoint p;
 CPoint point;
 double X=0.0;
 double Y=0.0;

 if (nCheckParam)
 {
	m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->GetValues(XMarker, YMarker);
	XMarker = XMarker * pow(10.0,expo_X-po_X);
	YMarker = YMarker * pow(10.0,expo_Y-po_Y);
	if (XMarker<X_max && YMarker<Y_max)
	{	m_Chart.MarkerMoveRight();
		DisplayMarker();
		if (m_bTangent) DisplaySecant();
	}
	else
	{
		if (n_Step>1)
		{
		  int NearestSerieIndex = m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->GetRelatedSerieIndex();
		  if (NearestSerieIndex < n_Step)
		  { NearestSerieIndex = NearestSerieIndex+1;
		    m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->SetRelatedSerie(NearestSerieIndex);		
			//устанавливаем позицию маркера
		    CChartSerie* pS = (CChartSerie*)m_Chart.GetSerie(NearestSerieIndex);
			pS ->ValueToScreen(X,Y,point);
			p = m_Chart.GetNearestSeriePoint(NearestSerieIndex, point,X,Y);	
			m_Chart.m_pMarkersList[m_Chart.m_iActiveMarkerIndex]->SetPosition(X,Y);		
		    m_Chart.m_pSecant->SetPoints(X,Y);
    	    m_Chart.RefreshCtrl();
		    m_Chart.DrawMarkers();
			DisplayMarker();
			if (m_bTangent) DisplaySecant();
		  }
		}
	}
 }	
}

void CLineDlg::OnButtonMarkerReset() 
{//сброс маркеров
	// TODO: Add your control notification handler code here
 if (nCheckParam)
 {
	m_Chart.DeleteMarker(m_uN);
	m_Chart.DeleteMarker(m_uN);
	ResetMarkers();
 }
}

void CLineDlg::OnSaveRef() 
{	// сохранить график в памяти (в файле *.ref)
	// TODO: Add your command handler code here
 if (bStatusMeas)//n_Result)
 {//0
	BOOL b = SaveRef();
	if (!b) MessageBox(_T("Нет свободного места для сохранения!"),NULL,MB_OK|MB_ICONERROR);
    else DisplayListRef();
 }//0 end if n_Result
// else MessageBox(_T("Проведите измерение!"),NULL,MB_OK|MB_ICONERROR);

}

BOOL CLineDlg::SaveRef() 
{
	CString str_Name_Ref = _T("");
	CString str_Prim_Ref = _T("");
	CString strFullRef = _T("");
	CString str,strDate,strMonth,strYear;
	CString	strHour,strMin,strSek;
	CFile file;
	int l;
	int i,j;
	CFileException er;
//	int nResultRef;
	int nStepRef;//
	int nMasRef;
	int nPointRef;

	nPointRef = NPOINT;
	if (m_bLooping) nPointRef = NPOINT100;
//	if (m_itipimp==TIP_IMP) nPointRef = 1;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) nPointRef=1;
//	if (m_irazv==DC_POS || m_irazv==DC_NEG) nPointRef = n_Result;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) nPointRef = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) nPointRef = n_Result;
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) nPointRef = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) nPointRef = n_Result;
	}

	if (m_bBeta || m_bBetai || m_bS)  nPointRef =1;	//одно значение на каждой ступеньке при Uc = 5V

	
	nStepRef=n_Step;
//	nResultRef = n_Step * nPointRef ;
	nMasRef	 = n_Mas;

	if(strDateMeas.IsEmpty())
	{ strDateMeas = strDateCurrent;
	  strTimeMeas = strTimeCurrent;
	}
	strDate=strDateMeas.Mid(0,2);
	strMonth=strDateMeas.Mid(3,2);
	strYear=strDateMeas.Mid(8,2);
	strHour=strTimeMeas.Mid(0,2);
	strMin=strTimeMeas.Mid(3,2);
	strSek=strTimeMeas.Mid(6,2);

//	strNameRef = m_MOD+_T("-")+strDate+_T("_")+
//				 strMonth+_T("_")+strYear+_T("_")+strTimeMeas+_T(".ref");
	str = _T("_")+strDate+strMonth+strYear+strHour+strMin+strSek+_T(".ref");
	str_Name_Ref = m_MOD+str;//_T("_")+strDate+strMonth+strYear+strHour+strMin+strSek+_T(".ref");
	
	strFullRef = strPassRef + str_Name_Ref;
	

	CSaveRefDlg srfd;
	srfd.m_sNameRef = m_MOD;
	srfd.m_sTimeRef = str;
	srfd.m_sNameFileRef = str_Name_Ref;
	srfd.m_sPrimRef = m_PRIM;
	UpdateData(FALSE);
	if (srfd.DoModal()==IDOK)
	{//2
		UpdateData();
		str_Prim_Ref = srfd.m_sPrimRef;
		str_Name_Ref = srfd.m_sNameFileRef;
		
		strFullRef = strPassRef + str_Name_Ref;
	    
		BOOL b = file.Open((LPCTSTR)strFullRef,CFile::modeCreate|CFile::modeWrite);
	    if (!b)
		{ TCHAR szError[1024];
          er.GetErrorMessage(szError, 1024);
	      str.Format(_T("Ошибка сохранения файла REF %s"),szError);
	      TRACE(str);
          return FALSE;
		}
        else
		{//1 
		//сохранение данных
		CCmdTarget::BeginWaitCursor();
		CString stre=_T("");

		str = str_Prim_Ref+_T(";\r\n");			//комментарий
		stre = stre + str;
		
	    str.Format(_T("m_iStyleCurve=%d;\r\n"),m_iStyleCurve);	//стиль кривой Ref
		stre = stre + str;
	    
		str.Format(_T("nStepRef=%d;\r\n"),nStepRef);		//число ступенек
		stre = stre + str;
		
		str.Format(_T("nMasRef=%d;\r\n"),nMasRef);		//число массивов
		stre = stre + str;
		
		str.Format(_T("nPointRef=%d;\r\n"),nPointRef);		//число точек
		stre = stre + str;

		for (j=0;j<nStepRef;j++)
		for (UINT m=0;m<nMasRef;m++)
		for (i=0;i<nPointRef;i++)				//точки
		{  str.Format(_T("refX[%d]=%f;\r\n"),i,refX[m][i+nPointRef*j]);
		   stre = stre + str;
	       
		   str.Format(_T("refY[%d]=%f;\r\n"),i,refY[m][i+nPointRef*j]);
	       stre = stre + str;
		}
		l=stre.GetLength();
        file.Write((LPCTSTR)stre,l*2);

		file.Close();
	    SetModifiedMemory();
		//архивирование
	    int n_ref = array_ref.GetSize();
		//контроль на совпадение
		BOOL fl = false;
		for(i=0;i<n_ref;i++)
		{ str = array_ref.GetAt(i);
			if (str == str_Name_Ref) { fl = true; break; }
		}
		if (!fl) 
		{ array_ref.SetAtGrow(n_ref,str_Name_Ref);
		}
		EndWaitCursor();
		MessageBox(_T("Сохранение завершено успешно!"),NULL,MB_OK|MB_ICONINFORMATION);

		return TRUE;
	}//1
	}//2
	else return TRUE;	//выход по кнопке "Cancel"
}

void CLineDlg::BuildArrayRef() 
{
	HANDLE hFileRef;
	WIN32_FIND_DATA fd;
	CString str_name_ref;

	array_ref.RemoveAll();
    int n_ref = array_ref.GetSize();
	
	hFileRef=::FindFirstFile(strFileRef,&fd);	//
	
	if (hFileRef != INVALID_HANDLE_VALUE)
	{ BOOL b = true;
	  while (b)
	  { //массив имён файлов ссылок
		str_name_ref.Format(L"%s",fd.cFileName);
		array_ref.SetAtGrow(n_ref,str_name_ref);		  
		n_ref++;
	    //поиск всех файлов *.ref
		b=FindNextFile(hFileRef,&fd);
	  }	//while (b)
	}

}
void CLineDlg::BuildArrayTstUser() 
{
	HANDLE hFileTst;
	WIN32_FIND_DATA fd;
	CString str_name_tst;

	array_tst_user.RemoveAll();
    int n_tst = array_tst_user.GetSize();
	
	hFileTst=::FindFirstFile(strFileTstUser,&fd);	//
	
	if (hFileTst != INVALID_HANDLE_VALUE)
	{ BOOL b = true;
	  while (b)
	  { //массив имён файлов 
		str_name_tst.Format(L"%s",fd.cFileName);
		array_tst_user.SetAtGrow(n_tst,str_name_tst);		  
		n_tst++;
	    //поиск всех файлов *.tst
		b=FindNextFile(hFileTst,&fd);
	  }	//while (b)
	}

}


void CLineDlg::BuildArrayTstStandart() 
{
	HANDLE hFileTst;
	WIN32_FIND_DATA fd;
	CString str_pass_tst,str_name_tst,str;
	CFile file;
	int iStrLen;
	CFileException er;
	int p1,p2;
    char* cc;
	//int nelement,
	int razv,dlitimp,iKolSourceU;
	
	array_tst_standart.RemoveAll();
    int n_tst = array_tst_standart.GetSize();
	
	hFileTst=::FindFirstFile(strFileTstStandart,&fd);	//
	
	if (hFileTst != INVALID_HANDLE_VALUE)
	{ BOOL g = true;
	  while (g)
	  { //массив имён файлов ссылок
		str_name_tst.Format(L"%s",fd.cFileName);
	    str_pass_tst = strPassTstStandart+str_name_tst;
     	BOOL b = file.Open((LPCTSTR)str_pass_tst,CFile::modeRead);
		  if (!b)
		  { TCHAR szError[1024];
			er.GetErrorMessage(szError, 1024);
			str.Format(_T("ERROR FILE TST %s"),szError);
			  TRACE(str);
		  }
          else
		  {//1 
		   //чтение файла
	       file.SeekToBegin();	
	       DWORD  l = file.GetLength();
	       DWORD* lp = new DWORD[l];
	       file.Read(lp,l);
		   file.Close();
		   CString strFile((LPCTSTR)&lp[0],l);
	       delete[] lp;
		   if (!strFile.IsEmpty())
		   {//2
			//категория
		    p1=strFile.Find(L'=',0);
		    p2=strFile.Find(L'\n',p1);
		    str=strFile.Mid(p1+1,p2-p1-2);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    int nelement = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			//--------------------------------------------------------------------------------
			//пропускаем 8 параметров
			for(int i=0;i<8;i++)	
			{ p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L'\n',p1);
		      str=strFile.Mid(p1+1,p2-p1-2);
		      iStrLen = str.GetLength();
		      cc = new char[iStrLen];
		      WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		      delete[] cc;
		      str.ReleaseBuffer();
			}
			//развертка
		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L'\n',p1);
		    str=strFile.Mid(p1+1,p2-p1-2);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    razv = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			//длительность импульса
		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L'\n',p1);
		    str=strFile.Mid(p1+1,p2-p1-2);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    dlitimp = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			//------------------------------------------------------------------------------
			//пропускаем 2 параметрa
			for( i=0;i<2;i++)	
			{ p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L'\n',p1);
		      str=strFile.Mid(p1+1,p2-p1-2);
		      iStrLen = str.GetLength();
		      cc = new char[iStrLen];
		      WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		      delete[] cc;
		      str.ReleaseBuffer();
			}
			//источник Uc
			p1=strFile.Find(L'=',p2);
		   p2=strFile.Find(L'\n',p1);
		   str=strFile.Mid(p1+1,p2-p1-2);
		   iStrLen = str.GetLength();
		   cc = new char[iStrLen];
		   WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		   iKolSourceU = atoi(cc);
		   delete[] cc;
		   str.ReleaseBuffer();
			//------------------------------------------------------------------------------
//			UpdateData();
			if (nelement == m_nelement)
			{ //проверка на соответствие категории и положению переключателя 2000V/5V
			  if ((n_Switch==0 && dlitimp<=4)||							//+-2000V
				  (n_IPPP==1 && n_Switch==1 && razv==IMP_POS && dlitimp > 4) ||
				  (n_IPPP==1 && n_Switch==1 && razv==SYN_POS && dlitimp == 6) ||
				  (n_IPPP==1 && n_Switch==2 && razv==SYN_NEG && dlitimp == 6)||	//-5V
				  (n_IPPP==1 && n_Switch==2 && razv==IMP_NEG && dlitimp > 4))	//+5V
			  { //тест
			    l = str_name_tst.GetLength();
			    str = str_name_tst.Mid(0,l-4);
			    array_tst_standart.SetAtGrow(n_tst,str);		  
				n_tst++;
			  }
			}
		   }//2
		  }//1
		  //поиск всех файлов *.tst
	      g=FindNextFile(hFileTst,&fd);
	  }	//while (g)
	}

}



void CLineDlg::DisplayListRef() 
{
//ITEMDATA* m_pDataFileRef[NREF];
	int i;
	int p1;
	CString str_name_ref,str_Prim_Ref;
	CString strTestRef,strDateRef,strMonthRef,strYearRef,strHourRef,strMinRef,strSekRef;
	CString str;
	CFile file;
	int iStrLen;
	CFileException er;

	BuildArrayRef();

	CListCtrl* pmyListCtrl = (CListCtrl*)GetDlgItem(IDC_LISTREF);
	pmyListCtrl->DeleteAllItems();// Delete all of the items from the list view control.

    int n_ref = array_ref.GetSize();
	for(i=0;i<n_ref;i++)
	{//0 
	  str_name_ref = array_ref.GetAt(i);	//<name>= <test><_><date><month><year><hour><min><sek>.ref

	  p1 = str_name_ref.Find(L'_',0);
	  strTestRef =str_name_ref.Mid(0,p1);
	  p1++;
	  strDateRef = str_name_ref.Mid(p1,2);
	  strMonthRef = str_name_ref.Mid(p1+2,2);
	  strYearRef = str_name_ref.Mid(p1+4,2);
	  
	  strHourRef = str_name_ref.Mid(p1+6,2);
	  strMinRef = str_name_ref.Mid(p1+8,2);
	  strSekRef = str_name_ref.Mid(p1+10,2);

	  str_Prim_Ref=str_name_ref.Mid(p1+12,30);
	  
	  //формируем строку списка файлов эталонных графиков
	  str.Format(_T("%d"),i+1);
	  strDataFileRef[i][0]=str;
	  strDataFileRef[i][1]=strTestRef;
	  str.Format(_T("%s.%s.%s"),strDateRef,strMonthRef,strYearRef);
	  strDataFileRef[i][2]=str;
	  str.Format(_T("%s:%s:%s"),strHourRef,strMinRef,strSekRef);
	  strDataFileRef[i][3]=str;

	  str_name_ref = strPassRef+str_name_ref;

    	  BOOL b = file.Open((LPCTSTR)str_name_ref,CFile::modeRead);
		  if (!b)
		  { TCHAR szError[1024];
			er.GetErrorMessage(szError, 1024);
			str.Format(_T("ERROR FILE REF %s"),szError);
			  TRACE(str);
		    str_Prim_Ref = _T("");
		  }
          else
		  {//1 
		   //чтение файла
	       file.SeekToBegin();	
	       DWORD  l = file.GetLength();
	       DWORD* lp = new DWORD[l];
	       file.Read(lp,l);
		   file.Close();
		   CString strFile((LPCTSTR)&lp[0],l);
	       delete[] lp;
		   if (!strFile.IsEmpty())
		   {//2
			//чтение данных
		    p1=strFile.Find(L'\r',0);
			str=strFile.Mid(0,p1);
		    iStrLen = str.GetLength();
		    str_Prim_Ref=str;
		   }//2
		   else str_Prim_Ref = _T("");
		  }//1
	  
	  str.Format(_T("%s"),str_Prim_Ref);
	  strDataFileRef[i][4]=str;
	//таблица
	//========
		pmyListCtrl->InsertItem(i, strDataFileRef[i][0]);
		pmyListCtrl->SetItemText(i, 1, strDataFileRef[i][1]);
		pmyListCtrl->SetItemText(i, 2, strDataFileRef[i][2]);
		pmyListCtrl->SetItemText(i, 3, strDataFileRef[i][3]);
		pmyListCtrl->SetItemText(i, 4, strDataFileRef[i][4]);
		pmyListCtrl->SetItemData(i,i);
	} 
}

void CLineDlg::OnExtractRef() 
{
	// TODO: Add your command handler code here
	CString str;
	CString strNameRef;
	CString strFullRef;
	CFile file;
	char* cc;
	int p1,p2;
	int iStrLen;
	CFileException er;

    int n_ref = array_ref.GetSize();
	int i_ref = m_listRef.GetItemCount();
	if(i_ref)
	{
		int m_ref = m_listRef.GetSelectionMark();
		if (m_ref==-1) MessageBox(_T("Выберите наименование теста для загрузки из списка!"),NULL,MB_OK|MB_ICONERROR);
		else 
		{ //0
		  strNameRef = array_ref.GetAt(m_ref);	//имя файла с эталонным графиком
		  strFullRef = strPassRef + strNameRef;
    	  BOOL b = file.Open((LPCTSTR)strFullRef,CFile::modeRead);
		  if (!b)
		  { TCHAR szError[1024];
			er.GetErrorMessage(szError, 1024);
			str.Format(_T("ERROR FILE REF %s"),szError);
			  TRACE(str);
		  }
          else
		  {//1 
		   //чтение файла
	       file.SeekToBegin();	
	       DWORD  l = file.GetLength();
	       DWORD* lp = new DWORD[l];
	       file.Read(lp,l);
		   file.Close();
		   CString strFile((LPCTSTR)&lp[0],l);
	       delete[] lp;
		   if (!strFile.IsEmpty())
		   {//2
			//чтение данных
		    p1=0;//strFile.Find(L'=',0);
		    p2=strFile.Find(L';',p1);
		    strPrimRef = strFile.Mid(p1,p2-p1); 
               strPrimRef.TrimLeft();
			   strPrimRef.TrimRight();

		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L';',p1);
		    str=strFile.Mid(p1+1,p2-p1-1);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    m_iStyleCurveRef = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
		    
		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L';',p1);
		    str=strFile.Mid(p1+1,p2-p1-1);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
//		    m_iResultRef = atoi(cc);
		    m_iStepRef = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			
		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L';',p1);
		    str=strFile.Mid(p1+1,p2-p1-1);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    m_iMasRef = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();
			
		    p1=strFile.Find(L'=',p2);
		    p2=strFile.Find(L';',p1);
		    str=strFile.Mid(p1+1,p2-p1-1);
		    iStrLen = str.GetLength();
		    cc = new char[iStrLen];
		    WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		    m_iPointRef = atoi(cc);
		    delete[] cc;
		    str.ReleaseBuffer();

			for (int j=0;j<m_iStepRef;j++)
			for (int m=0;m<m_iMasRef;m++)
			for(int i=0;i<m_iPointRef;i++)
			{//3
		      p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L';',p1);
		      str=strFile.Mid(p1+1,p2-p1-1);
		      iStrLen = str.GetLength();
		      cc = new char[iStrLen];
		      WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		      refX[m][i+m_iPointRef*j] = atof(cc);
		      delete[] cc;
		      str.ReleaseBuffer();

		      p1=strFile.Find(L'=',p2);
		      p2=strFile.Find(L';',p1);
		      str=strFile.Mid(p1+1,p2-p1-1);
		      iStrLen = str.GetLength();
		      cc = new char[iStrLen];
		      WideCharToMultiByte(CP_ACP,NULL,str.GetBuffer(iStrLen),iStrLen,cc,iStrLen,NULL,NULL);
		      refY[m][i+m_iPointRef*j] = atof(cc);
		      delete[] cc;
		      str.ReleaseBuffer();
			}//3 end of for i
			SetMassivRefNpoint(m_bButInversion);
			//закрыть архив
			ShowControls(aVid6,SIZEOF_ARRAY(aVid6),false);
			bStateListRef=false;
			CString str;
			str.Format(_T("TEST:  %s \\ %s."),m_EL,m_MOD);
			SetTextInfo(str);

		   }//2 if (!strFile.IsEmpty())
		  }//1
		}//0
//		if (nCheckRef) DisplayRef();
	}//end if (i_ref)
	else  MessageBox(_T("Список эталонных графиков пуст!"),NULL,MB_OK|MB_ICONERROR);

}
void CLineDlg::SetMassivRefNpoint(BOOL bInversion)
{
	int i,j;
 int nPointRef = m_iPointRef;
 int nStepRef = m_iStepRef;//m_iResultRef / m_iPointRef;
 int nMasRef = m_iMasRef;

 if (bStatusRef)
 {
   for(j=0;j<nStepRef;j++)
   m_Chart.DeleteSerie(n_Step);

   m_Chart.DeleteFamily(REF);
   bStatusRef = false;
 }
 for(j=0;j<nStepRef;j++)
 {  
	switch(m_iStyleCurveRef)
	{ case STYLE_LINE:	    pGraphRef = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraphRef = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraphRef = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}

	if (((m_irazv==SYN_POS || m_irazv==SYN_NEG) && !m_bLooping) ||
		(m_irazv==AC && !m_bLooping) ||
		(m_nelement==TS))	pGraphRef->SetSortPoints(false);
    else  pGraphRef->SetSortPoints(true);

   for (UINT m=0;m<nMasRef;m++)
   {
	for(i=0;i<nPointRef;i++)
	{
	  pX[i+nPointRef*m] = refX[m][i+nPointRef*j];
	  pY[i+nPointRef*m] = refY[m][i+nPointRef*j];
	}
   }
	pGraphRef->SetColor(m_colorRef);							//цвет кривой
	pGraphRef->SetPoints(pX,pY,nPointRef*nMasRef);
	pGraphRef->SetName("j-ая кривая");
 }
	CChartSerieFamily* pFamilyRef = m_Chart.AddFamily();
	pFamilyRef->SetColor(m_colorRef);
	pFamilyRef->SetName("Ref");

	m_Chart.RefreshCtrl();
	bStatusRef = true;
}

void CLineDlg::SetMassivDiffNpoints()
{
      r1=0.0;
	  r2=0.0;
	  double x  = 0.0;	//искомое значение аргумента при значении 1 или 2 параметра
	  double y  = 0.0;	//искомое значение функции при значении 1 или 2 параметра
	int i=0;
	int j=0;
	int m=0;
	int n_step=n_Step;
	int n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
//	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_iQue1==COLLECTOR)
	{
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	if (m_iQue1==BAZA)
	{
		if (m_irazv==IMP_POS || m_irazv==IMP_NEG) {n_point = 1; n_Mas=m_uStepBaza*10;}
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}

	m_Chart.DisableRefresh(true);
	int b=0;
 if (bStatusMeas)
 {
	b = m_Chart.GetSeriesCount();
	for(j=0;j<b;j++)
	m_Chart.DeleteSerie(0);
    m_Chart.DeleteFamily(MEAS);
	bStatusMeas = false;
 }
 if (bStatusRef)
 { b = m_Chart.GetSeriesCount();
	for(j=0;j<b;j++)
	m_Chart.DeleteSerie(0);
    m_Chart.DeleteFamily(REF);
	bStatusRef = false;
 }
 if (bStatusCalc)
 {
	b = m_Chart.GetSeriesCount();
	for(j=0;j<b;j++)
	m_Chart.DeleteSerie(0);
    m_Chart.DeleteFamily(CALC);
	bStatusCalc = false;
 }
 
 if (bStatusDiff)
 {
	b = m_Chart.GetSeriesCount();
   for(j=0;j<b;j++)
   m_Chart.DeleteSerie(0);

   m_Chart.DeleteFamily(DIFF);
   bStatusDiff = false;
 }

	CChartSerie*  pGraph;

 for(j=0;j<n_Step;j++)
 {  
	switch(m_iStyleCurve)
	{ case STYLE_LINE:	    pGraph = m_Chart.AddSerie(CChartSerie::stLine); break;
	  case STYLE_POINT:	    pGraph = m_Chart.AddSerie(CChartSerie::stPoints); break;
	  case STYLE_LINEPOINT:	pGraph = m_Chart.AddSerie(CChartSerie::stMixed); break;
	}
//	if (m_nelement==TS)
//	pGraph->SetSortPoints(false);
//        else  pGraph->SetSortPoints(true);

	if (((m_irazv==SYN_POS || m_irazv==SYN_NEG) && !m_bLooping) ||
		(m_irazv==AC && !m_bLooping) ||
		(m_nelement==TS))	pGraph->SetSortPoints(false);
    else  pGraph->SetSortPoints(true);

	for (m=0;m<n_Mas;m++)
	{
	for(i=0;i<n_point;i++)
	{
	  pX[i+n_point*m] = pM1[m][i+NPOINT*j] / pow(10.0,expo_X-po_X);
	  pY[i+n_point*m] = pM2[m][i+NPOINT*j] / pow(10.0,expo_Y-po_Y); 
				
		  if (!b_flagStatusRef)
		  {//копия для архива
		  refX[m][i+n_point*j] = pX[i+n_point*m];
		  refY[m][i+n_point*j] = pY[i+n_point*m];
		  }
	}
	}
	pGraph->SetColor(m_colorCurve);							//цвет кривой
	pGraph->SetPoints(pX,pY,n_point*n_Mas);
//	if (bInversion) 
//	{
//	 TRACE(L"inverted\n");
//	 pGraph->SetXYInverted(true);
//	}

	pGraph->SetName("j-ая кривая");
//	delete[] pX;
//	delete[] pY;
 }//end of for j

	CChartSerieFamily* pFamily = m_Chart.AddFamily();
	pFamily->SetColor(m_colorCurve);
	pFamily->SetName("Diff");
	bStatusDiff = true;
	bStatusMeas = true;
	m_Chart.DisableRefresh(false);

	// поиск параметра на ступеньке c № m_uTp = j-ой кривой
	//--------------------------------------------------------
	if (!m_sPar.IsEmpty() && m_uFunc<2)

//	if ((!m_sPar.IsEmpty() && m_uTp >0 && (m_uTp-1) == j && m_uFunc!=2)||
//		(!m_sPar.IsEmpty() && m_uTp ==0 && m_uTp == j && m_uFunc!=2))
	{ 
		//расчет параметра 1
	  // из уравнения прямой по 2-м точкам (x1,y1) (x2,y2): 
	  // (y-y1)/(y2-y1)=(x-x1)/(x2-x1) получаем: 
	  // y = (y2-y1)*(x-x1)/(x2-x1)+y1;
	  // x = ((y-y1)*(x2-x1))/(y2-y1)+x1;
	  if (m_uFunc==0)	// найти значение функции по значению аргумента 
		{ if (!m_sUr1.IsEmpty())
			{ 
		      Ur1 = Ur1 / pow(10.0,expo_X-po_X);

		      y = GetYbyX(Ur1,m_uTp);
			  
			  Ur1 = Ur1 * pow(10.0,expo_X-po_X);
			  r1 = y * pow(10.0,expo_Y-po_Y);
			}//end of if (!m_sUr1.IsEmpty())

		 if (!m_sUr2.IsEmpty())
			{
			  Ur2 = Ur2 / pow(10.0,expo_X-po_X);
			  y  = GetYbyX(Ur2,m_uTp);

			  Ur2 = Ur2 * pow(10.0,expo_X-po_X);
			  r2 = y * pow(10.0,expo_Y-po_Y);
			}// end of if (!m_sUr2.IsEmpty())

		  if (!m_sUr1.IsEmpty() && !m_sUr2.IsEmpty()) 
		  { switch(m_iForm)
			{ case 0: if ((Ur2-Ur1)!= 0.0) Par = (r2-r1)/(Ur2-Ur1);
					  break;
			  case 1: if ((r2-r1)!=0.0) Par = (Ur2-Ur1)/(r2-r1);
					  break;
			}
		  }
	      else Par = r1;
		}//end of m_uFunc==0

	  if (m_uFunc==1)	//найти значение аргумента по значению функции
		{ if (!m_sUr1.IsEmpty()) 
			{
		      Ur1 = Ur1 / pow(10.0,expo_Y-po_Y);
		      x = GetXbyY(Ur1,m_uTp);

			  Ur1 = Ur1 * pow(10.0,expo_Y-po_Y);
			  r1 = x * pow(10.0,expo_X-po_X);
			}//end of if (!m_sUr1.IsEmpty())

		  if (!m_sUr2.IsEmpty())
			{
			  Ur2 = Ur2 / pow(10.0,expo_Y-po_Y);
			  x  = GetXbyY(Ur2,m_uTp);

			  Ur2 = Ur2 * pow(10.0,expo_Y-po_Y);
			  r2 = x * pow(10.0,expo_X-po_X);
			}
		  if (!m_sUr1.IsEmpty() && !m_sUr2.IsEmpty()) 
		  { switch(m_iForm)
			{ case 0: if ((r2-r1)!=0.0) Par = (Ur2-Ur1)/(r2-r1);
					  break;
			  case 1: if ((Ur2-Ur1)!=0.0) Par = (r2-r1)/(Ur2-Ur1);
					  break;
			}
		  }
	      else Par = r1;
		}//end of m_uFunc==1
	}//end of поиск параметра

}


void CLineDlg::ResetRef() 
{
	int j;
	BOOL flag=false;

 if (bStatusRef)
 {	
	m_Chart.DisableRefresh(true); 
	if (nCheckParam)
	{ flag = true;
		nCheckParam = false;	//!;
	//маркеры неактивны отoбражение панели задания параметров измерения				
//	  ShowControls(aVid2,SIZEOF_ARRAY(aVid2),FALSE);
//	  ShowControls(aVid1,SIZEOF_ARRAY(aVid1),TRUE);
	  MarkerNoakt();
	}

   for(j=0;j<m_iStepRef;j++)
   m_Chart.DeleteSerie(n_Step);
   m_Chart.DeleteFamily(REF);
   bStatusRef = false;
   
   m_listRef.SetSelectionMark(0);
   strNameRef = _T("");
   strPrimRef = _T("");
//   if (nCheckRef) DisplayRef();
   if (flag)
   {		nCheckParam = true;	//!;
			MarkerAkt();
   }
	m_Chart.DisableRefresh(false); 
 }
}

void CLineDlg::ResetMeas() 
{
	int j;

 if (bStatusMeas)
 {	
   
   for(j=0;j<n_Step;j++)
   m_Chart.DeleteSerie(0);

   m_Chart.DeleteFamily(MEAS);
   bStatusMeas = false;
   
   m_Chart.RefreshCtrl();
 }
}

void CLineDlg::OnGetdispinfoListref(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CLineDlg::OnDelMemory() 
{
	// TODO: Add your command handler code here
	CString str,str_Name_Ref,strFullRef;
	CFile file;

    int n_ref = array_ref.GetSize();
	int i_ref = m_listRef.GetItemCount();
	if(i_ref)
	{
		int m_ref = m_listRef.GetSelectionMark();
		if (m_ref==-1) MessageBox(_T("Выберите наименование теста с эталонным графиком!"),NULL,MB_OK|MB_ICONERROR);
		else 
		{//0)
		  str_Name_Ref = array_ref.GetAt(m_ref);	//имя файла с эталонным графиком
		  if (str_Name_Ref!=_T(""))
		  { strFullRef = strPassRef + str_Name_Ref;
		    str.Format(_T("Вы уверены, что хотите удалить тест %s из архива эталонных графиков?"),str_Name_Ref);
		    if (MessageBox(str,NULL,MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{//удалить файл
		      file.Remove((LPCTSTR)strFullRef);
	          DisplayListRef();
			  SetModifiedMemory();
			}
		  }
		}//end of 0)
	}//end if (i_ref)
}

void CLineDlg::OnClearMemory() 
{
	// TODO: Add your command handler code here
	CString str_Name_Ref,strFullRef;
	CFile file;

	int i_ref = m_listRef.GetItemCount();
	if(i_ref)
	{ if (MessageBox(_T("Вы уверены, что хотите очистить весь архив?"),NULL,MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		{//удалить все файлы *.ref
          int n_ref = array_ref.GetSize();
		  if (n_ref)
		  { for (int i=0;i<n_ref;i++)
			{
			  str_Name_Ref = array_ref.GetAt(i);	//имя файла с эталонным графиком
			  if (str_Name_Ref!=_T(""))
			  { strFullRef = strPassRef + str_Name_Ref;
		        file.Remove((LPCTSTR)strFullRef);//удалить файл
			  }
			}//end for
	        DisplayListRef();
		    SetModifiedMemory();
		  }//end if (n_ref)
		}//end if IDYES
	}// end if (i_ref)
}

void CLineDlg::OnOtchtMail() 
{
			    ShowControls(aVid5,SIZEOF_ARRAY(aVid5),FALSE);
				SetTextInfo(_T("TEST:  ") + strTestName);
					nCheckOtcht= 0;	
}

void CLineDlg::OnListrefOpen() 
{
	ShowControls(aVid6,SIZEOF_ARRAY(aVid6),TRUE);
	bStateListRef=true;
	SetTextInfo(_T("\nАрхив эталонных графиков\n"));
}

void CLineDlg::OnListrefClose() 
{
	ShowControls(aVid6,SIZEOF_ARRAY(aVid6),false);
	bStateListRef=false;

	CString str;
	str.Format(_T("TEST:  %s \\ %s."),m_EL,m_MOD);//\tПроведите измерение или выберите другую модель!
	SetTextInfo(str);//_T("Проведите измерение или выберите другую модель!"));
}


void CLineDlg::WhatFreeMemory() 
{
	// TODO: Add your command handler code here
	CString strFreeSpace;
	ULARGE_INTEGER  ulTotalSpace;
    ULARGE_INTEGER  ulUserSpace;
    ULARGE_INTEGER  ulFreeSpace;
	BOOL b=TRUE;
	CFreeMemoryDlg fdlg;
	    LPCWSTR   lpDirectoryName =_T("\\Hard Disk");
		if (!GetDiskFreeSpaceEx(lpDirectoryName, &ulUserSpace, &ulTotalSpace, &ulFreeSpace))
			{
			DWORD d_error=GetLastError();
			if (d_error==ERROR_PATH_NOT_FOUND)
				{
				m_st_flash.Format(_T("Нет"));
				m_progress_flash.ShowWindow(SW_HIDE);
				}
			}
		else
			{
			free_space=(int)((ulFreeSpace.QuadPart*100)/ulTotalSpace.QuadPart);
			
			if ((!b)||(free_space==0)) strFreeSpace.Format(_T("0%%"));//100%% "));
			else strFreeSpace.Format(_T("%u%%  "),100-free_space);//100-free_space);
			m_progress_flash.SetPos(100-free_space);//100-free_space);
			m_st_flash=strFreeSpace;

		UpdateData(false);
			}
}

void CLineDlg::OnRefReset() 
{
	// TODO: Add your command handler code here
	ResetRef();
	SetMassiv(true);	//восстанавливаем в refX и refY измеренный график
}

void CLineDlg::OpenTabl() 
{
	//отображение таблицы измеренных данных
	//=====================================
	CTablDlg tabdlg;
	CString  str;
	UpdateData();
	switch(m_nelement)
	{ case DIOD:  str = _T("D");break;
	  case RES:   str = _T("R");break;
	  case NPN:   str = _T("NPN");break;
	  case PNP:   str = _T("PNP");break;
	  case NMOS:  str = _T("NMOS");break;
	  case PMOS:  str = _T("PMOS");break;
      case NPN4:  str = _T("NPN4");break;
	  case PNP4:  str = _T("PNP4");break;
	  case NMOS4: str = _T("NMOS4");break;
	  case PMOS4: str = _T("PMOS4");break;
	}

 if (!bPusk)
 {
	tabdlg.m_sKolMeasU = m_sKolMeasU;
	tabdlg.m_sKolMeasI = m_sKolMeasI;
	tabdlg.m_sBazaSource = m_sBazaSource;
	tabdlg.m_sBazaMeas = m_sBazaMeas;
	tabdlg.m_sDopSource = m_sDopSource;
	tabdlg.m_sDopMeas = m_sDopMeas;
	tabdlg.m_iCircuitBaza = m_iCircuitBaza;
	tabdlg.m_iCircuitDop = m_iCircuitDop;
	tabdlg.m_iBazaMeas = m_iBazaMeas;
	tabdlg.m_iDopMeas = m_iDopMeas;
	tabdlg.m_bBaza = m_bBaza;
	tabdlg.m_bDop = m_bDop;
	tabdlg.m_bR = m_bR;
	tabdlg.m_bS = m_bS;
	tabdlg.m_bBeta = m_bBeta;
	tabdlg.m_bBetai= m_bBetai;
	tabdlg.rej_Y= rej_Y;
	tabdlg.strDateMeas = strDateMeas;
	tabdlg.strTimeMeas = strTimeMeas;
	tabdlg.strElement = str;
	tabdlg.strPrim = str_PRIM;
	tabdlg.m_itipimp = m_itipimp;

	UpdateData(FALSE);
	tabdlg.DoModal();
 }
}

void CLineDlg::OnOpentabl() 
{
	bStatusTablRes = stateMax;
	ClientToScreen(&RectTablRes);
	RectTablRes.top=RectInfoTest.bottom;
	ScreenToClient(&RectTablRes);
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_LIST_RESULT);
	pSt->MoveWindow(&RectTablRes);
	InitTablRes();
}


void CLineDlg::OnRadioLeft() 
{
	//(0,0) слева
	m_iButNull=LEFT;
	InitPageTuning(0);
	if (bStatusCalc) InitAxisCalc();
	else InitAxis();
}

void CLineDlg::OnRadioCenter() 
{
	//(0,0) в центре
	m_iButNull=CENTER;
	InitPageTuning(0);
	if (bStatusCalc) InitAxisCalc();
	else 	InitAxis();
}

void CLineDlg::OnRadioRight() 
{
	//(0,0) справа
	m_iButNull=RIGHT;
	InitPageTuning(0);
	if (bStatusCalc) InitAxisCalc();
	else InitAxis();
}

void CLineDlg::OnCheckInversX() 
{
	// TODO: Add your control notification handler code here
		//инверсия X <=> -X
		  m_bButInversion = !m_bButInversion;
		  m_Chart.m_pAxisBottom->SetInverted(m_bButInversion);
		  InitPageTuning(0);
}

void CLineDlg::OnCheckInversY() 
{
	// TODO: Add your control notification handler code here
		//инверсия Y <=> -Y
		  m_bButInversionY = !m_bButInversionY;
		  m_Chart.m_pAxisLeft->SetInverted(m_bButInversionY);
		  InitPageTuning(0);
	
}

void CLineDlg::OnRadiodopu() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
//	m_iDopMeas = 1;	//источник U => по умолчанию измеритель I
	m_iDopMeas = 0;	//источник U => по умолчанию измеритель U
	  m_bDop = false;
	DefinitionDiapazonDop();
	InitDiapazonDop();
	SetDiapazonDop();
	SetStringsSourceMeas();
	UpdateData(FALSE);
	ControlParam();
	
}

void CLineDlg::OnRadiodopi() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
//	m_iDopMeas=0;	//источник I => по умолчанию измеритель U
	m_iDopMeas=1;	//источник I => по умолчанию измеритель I
	  m_bDop = false;
	DefinitionDiapazonDop();
	InitDiapazonDop();
	SetDiapazonDop();
	SetStringsSourceMeas();
	UpdateData(FALSE);
	ControlParam();
}


void CLineDlg::OnCheckpauza() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_bCheckPauza) m_udlitpauza = 1;	//Скважность х 2 вкл.
				  else m_udlitpauza = 0;	//выкл.
	if (m_irazv==DC_POS || m_irazv==DC_NEG)	m_udlitpauza = 0;	//скважность х 2 выкл
	UpdateData(FALSE);
	
}

void CLineDlg::OnSoundOn() 
{
	// TODO: Add your command handler code here
	//включить автоматическое сохранение результатов измерения
	CString str,strDate,strMonth,strYear;
	CFile file;
	ULONGLONG dwLength = 0;

	strDate=strDateCurrent.Mid(0,2);
	strMonth=strDateCurrent.Mid(3,2);
	strYear=strDateCurrent.Mid(8,2);

	
	do
	{
//	str.Format(_T("%s_%s_%s_%s%s%s.xls"),m_sProduct,m_sPart,m_sPlast,strDate,strMonth,strYear);//strDateMeas);
	  str.Format(_T("%s_%s_%s_%s%s%s_%u.xls"),m_sProduct,m_sPart,m_sPlast,strDate,strMonth,strYear,uFileNumber);
//	  nameXLS = _T("\\Disk\\Xls\\")+str;
	  nameXLS = _T("\\Hard Disk\\FTPRoot\\Xls\\")+str;
//	  nameXLS = _T("\\Hard Disk\\Cosmos\\Xls\\")+str;
	  file.Open((LPCTSTR)nameXLS,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead);
	  dwLength = file.GetLength();
	  file.Close();
	  uFileNumber++;
	} while (dwLength>=2000000);//MAX_FILE_LENGTH)

	uFileNumber--;
		
	bSound = true;
//	SetPassWordLine();
}

void CLineDlg::OnSoundOff() 
{
	// TODO: Add your command handler code here
	//выключить автоматическое сохранение результатов измерения	
	bSound = false;
//	SetPassWordLine();
}

void CLineDlg::ClearTablRes() 
{
	// TODO: Add your command handler code here
	UINT i,k;
	CString str;
	UINT j=0,s=0;
	CRect rect;
 
//	pM[4]= &pM5[0][0];

		rect.left = RectTablRes.left;
		rect.top = RectTablRes.top;
		rect.right = RectTablRes.right;
		rect.bottom  = RectTablRes.bottom;

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	pListCtrl->DeleteAllItems();// Delete all of the items from the list view control.
	for (i=0;i<7;i++)
	pListCtrl->DeleteColumn(0);
	pListCtrl->SetExtendedStyle(m_listRes.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//заголовки колонок
	k= 0;
	str =_T("Точка");
	pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/8);
	strNameColumn=str+_T("\t");
	k++;
	str =m_sKolMeasU+_T(", V");
	pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	strNameColumn=strNameColumn+str+_T("\t");
	k++;
	str = m_sKolMeasI+_T(", A");
	pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	strNameColumn=strNameColumn+str+_T("\t");
//	strNameColumn=strNameColumn+str;//+_T("\t");
	k++;
//	if (m_iCircuitBaza>0)
//	if (m_iCircuitBaza==1)
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	{ 
/*		if (m_bBaza)
		{ if (m_iBazaSource) str = m_sBazaSource +_T(", A");
				else str = m_sBazaSource+_T(", V");
		}
		else
		{ if (m_iBazaMeas) str = m_sBazaMeas+_T(", A");
				else str = m_sBazaMeas+_T(", V");
		}
*/
		if (!m_bBaza)
		{ if (m_iBazaSource==1) str = m_sBazaSource +_T(", A");
				else str = m_sBazaSource+_T(", V");
		}
		else
		{ if (m_iBazaMeas==1) str = m_sBazaMeasLimit+_T(", A");
				else str = m_sBazaMeasLimit+_T(", V");
		}
		pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
//	if (m_iCircuitDop>0)
	if (m_iCircuitDop==1 && m_iKnotDop==1)
//	if (m_iCircuitDop==1)
	{ 
/*		if (m_bDop)
		{ if (m_iDopSource)  str = m_sDopSource+_T(", A");
				 else str = m_sDopSource+_T(", V");
		}
		else
		{ if (m_iDopMeas)  str = m_sDopMeas+_T(", A");
				else  str = m_sDopMeas+_T(", V");
		}
*/
		if (!m_bDop)
		{ if (m_iDopSource==1)  str = m_sDopSource+_T(", A");
				 else str = m_sDopSource+_T(", V");
		}
		else
		{ if (m_iDopMeas==1)  str = m_sDopMeasLimit+_T(", A");
				else  str = m_sDopMeasLimit+_T(", V");
		}
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
	if (m_bR)
	{ str = _T("R, Om");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
	if (m_bS)
	{ str = _T("S, A/V");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}

	if (m_bBeta)
	{ str = _T("Beta");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
	if (m_bBetai)
	{ str = _T("Betai");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
	//заполнение до 10-ти узлов (чтобы в строке наименований узлов было 10 табуляций)
	str=_T("");
	for (i=k; i<10; i++)
	str = str+_T("\t");
    strNameColumn=strNameColumn+str;//_T("\t");

	strNameColumn=strNameColumn+_T("\r\n");//+ 2 табуляции == всего 12

	//заполнение данными строк таблицы
	  int p = 0;//	остаток от деления
	int m=0;
	int n_point=1;
	for (j=0;j<1;j++)
	for (s=0;s<1;s++)
	{
//	for (i=0;i<n_point;i++)
	for (i=0;i<1;i++)
	{ k=0;//k-номер колонки
	  //m-номер строки в таблице,j-номер очереди,i-номер точки в очереди,
	  //для импульсной развёртки:m-номер строки в таблице,j-номер очереди,s-номер точки в очереди,k-номер колонки
	   str=_T("1 \\ 1 \\ ");
	  strData[s][i+n_point*j][k]=str;
	  k++;
	  //данные Uc
	  //=========
	  str.Format(_T("%7.4e"),pM1[s][i+NPOINT*j]);
	  strData[s][i+n_point*j][k]=str;
	  k++;
	  //данные Ic
	  //=========
	  str.Format(_T("%7.4e"),pM2[s][i+NPOINT*j]);
	  strData[s][i+n_point*j][k]=str;
	  k++;
	  
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	  {//данные по базе
	   //==============
	     str.Format(_T("%7.4e"),pM3[s][i+NPOINT*j]);
	     strData[s][i+n_point*j][k]=str;
	  k++;
	  }
	  
	if (m_iCircuitDop==1 && m_iKnotDop==1)
	  {//данные по доп.каналу
	   //====================
	     str.Format(_T("%7.4e"),pM4[s][i+NPOINT*j]);
	     strData[s][i+n_point*j][k]=str;
	  k++;
	  }

	  if (m_bR || m_bBeta || m_bBetai || m_bS)
	  {//рассчитанный массив
//	     str.Format(_T("%7.4e"),pM5[s][i+NPOINT*j]);
	     str.Format(_T("%7.3e"),pM5[s][i+NPOINT*j]);
	     strData[s][i+n_point*j][k]=str;
	  k++;
	  }
	  m++;
	}
	}
  
	k=0;//№ строки
	for (j=0; j<1;j++)
	for (s=0; s<1;s++)
	for (i=0; i<1; i++)
	{m=0;//№ колонки
		pListCtrl->InsertItem(k, strData[s][i+n_point*j][m]);m++;
		pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;
		pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;
		if (m_iCircuitBaza>0){pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;}
		if (m_iCircuitDop>0) {pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;}
		if (m_bR || m_bBeta || m_bBetai ||m_bS) pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;
//		pListCtrl->SetItemText(k, 6, strData[s][i+n_point*j][6]);
		pListCtrl->SetItemData(k, (LPARAM)m_pData[k]);
		k++;
	}
}

void CLineDlg::InitTablRes() 
{
	// TODO: Add your command handler code here
	UINT i,k;
	CString str;
	UINT j=0,s=0;
	CRect rect;
 
		rect.left = RectTablRes.left;
		rect.top = RectTablRes.top;
		rect.right = RectTablRes.right;
		rect.bottom  = RectTablRes.bottom;
	int n_t0 = 0;
	int n_step = n_Step;
	int n_point = NPOINT;
	UINT n_mas = 1;
	if (m_bLooping) n_point = NPOINT100;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) { n_point = 1; n_mas = n_Mas;}
	if (m_irazv==DC_POS || m_irazv==DC_NEG) 
	{ if (n_Result==0) n_Result=1;
	  if ( bMeas == MEAS_SINGLE) n_point = uNpointsLupa;
							else n_point = n_Result;
//	  if (m_iLimitKolU==0) n_point=1;//для вывода в таблицу только одной точки
	}
	if (m_iQue1==COLLECTOR)
	{
	if (m_bBeta || m_bBetai || m_bS)  {n_point = 1;}//t_0=0;}	//одно значение на каждой ступеньке при Uc = 5V
	}

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_RESULT);
	pListCtrl->DeleteAllItems();// Delete all of the items from the list view control.
	for (i=0;i<7;i++)
	pListCtrl->DeleteColumn(0);
	pListCtrl->SetExtendedStyle(m_listRes.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//заголовки колонок
	k= 0;
	str =_T("Точка");
	pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/8);
	strNameColumn=str+_T("\t");
	k++;
	str =m_sKolMeasU+_T(", V");
	pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	strNameColumn=strNameColumn+str+_T("\t");
	k++;
	str = m_sKolMeasI+_T(", A");
	pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	strNameColumn=strNameColumn+str+_T("\t");
//	strNameColumn=strNameColumn+str;//+_T("\t");
	k++;
//	if (m_iCircuitBaza>0)
//	if (m_iCircuitBaza==1)
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	{ 
/*		if (m_bBaza)
		{ if (m_iBazaSource) str = m_sBazaSource +_T(", A");
				else str = m_sBazaSource+_T(", V");
		}
		else
		{ if (m_iBazaMeas) str = m_sBazaMeas+_T(", A");
				else str = m_sBazaMeas+_T(", V");
		}
*/
		if (!m_bBaza)
		{ if (m_iBazaSource==1) str = m_sBazaSource +_T(", A");
				else str = m_sBazaSource+_T(", V");
		}
		else
		{ if (m_iBazaMeas==1) str = m_sBazaMeasLimit+_T(", A");
				else str = m_sBazaMeasLimit+_T(", V");
		}
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
//	if (m_iCircuitDop>0)
	if (m_iCircuitDop==1 && m_iKnotDop==1)
//	if (m_iCircuitDop==1)
	{ 
		if (!m_bDop)
		{ if (m_iDopSource)  str = m_sDopSource+_T(", A");
				 else str = m_sDopSource+_T(", V");
		}
		else
		{ if (m_iDopMeas==1)  str = m_sDopMeasLimit+_T(", A");
				else  str = m_sDopMeasLimit+_T(", V");
		}
		pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
	if (m_bR)
	{ str = _T("R, Om");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}
	if (m_bS)
	{ str = _T("S, A/V");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
	  k++;
	}

	if (m_bBeta)
	{ str = _T("Beta");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
	if (m_bBetai)
	{ str = _T("Betai");
	  pListCtrl->InsertColumn(k,str,LVCFMT_LEFT,rect.Width()/5);
	  strNameColumn=strNameColumn+str+_T("\t");
//	  strNameColumn=strNameColumn+_T("\t")+str;
	  k++;
	}
	//заполнение до 10-ти узлов (чтобы в строке наименований узлов было 10 табуляций)
	str=_T("");
	for (i=k; i<10; i++)
	str = str+_T("\t");
    strNameColumn=strNameColumn+str;//_T("\t");

	strNameColumn=strNameColumn+_T("\r\n");//+ 2 табуляции == всего 12

	//заполнение данными строк таблицы
	  int p = 0;//	остаток от деления
	int m=0;
	m=n_t0;
	for (j=0;j<n_step;j++)
	for (s=0;s<n_mas;s++)
	{
//	for (i=0;i<n_point;i++)
	for (i=n_t0;i<n_point;i++)
	{ k=0;//k-номер колонки
	  //m-номер строки в таблице,j-номер очереди,i-номер точки в очереди,
	  //для импульсной развёртки:m-номер строки в таблице,j-номер очереди,s-номер точки в очереди,k-номер колонки
//	  if (m_itipimp==TIP_IMP) str.Format(_T("%d \\ %d \\ %d"),m+1-n_t0,j+1,s+1);
	  if (m_irazv==IMP_POS || m_irazv==IMP_NEG)
		                      str.Format(_T("%d \\ %d \\ %d"),m+1-n_t0,j+1,s+1);
						else  str.Format(_T("%d \\ %d \\ %d"),m+1-n_t0,j+1,i+1-n_t0);
	  strData[s][i+n_point*j][k]=str;
	  k++;
	  //данные Uc
	  //=========
	  str.Format(_T("%7.4e"),pM1[s][i+NPOINT*j]);
	  strData[s][i+n_point*j][k]=str;
	  k++;
	  //данные Ic
	  //=========
	  str.Format(_T("%7.4e"),pM2[s][i+NPOINT*j]);
	  strData[s][i+n_point*j][k]=str;
	  k++;
	  
	if (m_iCircuitBaza==1 && m_iKnotBaza==1)
	  {//данные по базе
	   //==============
	     str.Format(_T("%7.4e"),pM3[s][i+NPOINT*j]);
	     strData[s][i+n_point*j][k]=str;
	  k++;
	  }
	  
	if (m_iCircuitDop==1 && m_iKnotDop==1)
//	  if (m_iCircuitDop==1)
	  {//данные по доп.каналу
	   //====================
	     str.Format(_T("%7.4e"),pM4[s][i+NPOINT*j]);
/*		 p = m_iRangMeasDop % 3;
		 if (m_iDopMeas==1)
		 {//I
		   if (p==2) str.Format(_T("%7.4e"),pM4[s][i+NPOINT*j]);
		   else      str.Format(_T("%7.5e"),pM4[s][i+NPOINT*j]);
		 }
		 else
		 {//U
		   if (p==2) str.Format(_T("%7.5e"),pM4[s][i+NPOINT*j]);
		   else      str.Format(_T("%7.4e"),pM4[s][i+NPOINT*j]);
		 }
*/	     strData[s][i+n_point*j][k]=str;
	  k++;
	  }

	  if (m_bR || m_bBeta || m_bBetai || m_bS)
	  {//рассчитанный массив
//	     str.Format(_T("%7.4e"),pM5[s][i+NPOINT*j]);
	     str.Format(_T("%7.3e"),pM5[s][i+NPOINT*j]);
	     strData[s][i+n_point*j][k]=str;
	  k++;
	  }
	  m++;
	}
	}
  
	k=0;//№ строки
	for (j=0; j<n_step;j++)
	for (s=0; s<n_mas;s++)
	for (i=n_t0; i<n_point; i++)
	{m=0;//№ колонки
		pListCtrl->InsertItem(k, strData[s][i+n_point*j][m]);m++;
		pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;
		pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;
		if (m_iCircuitBaza>0){pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;}
		if (m_iCircuitDop>0) {pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;}
		if (m_bR || m_bBeta || m_bBetai ||m_bS) pListCtrl->SetItemText(k, m, strData[s][i+n_point*j][m]);m++;
//		pListCtrl->SetItemText(k, 6, strData[s][i+n_point*j][6]);
		pListCtrl->SetItemData(k, (LPARAM)m_pData[k]);
		k++;
	}
}



void CLineDlg::OnSelchangedTreeTest(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UINT i=0;
	UINT k=0;

	HTREEITEM hSelected = pNMTreeView -> itemNew.hItem;
	if (hSelected != NULL)
	{  char text[15];
	   TV_ITEM item;
	   item.mask = TVIF_HANDLE |TVIF_TEXT|TVIF_PARAM;	
	   item.hItem = hSelected;
	   item.pszText = (unsigned short*)text;
	   item.cchTextMax = 14;
	   VERIFY(m_TreeCtrl.GetItem(&item));
       DWORD dw=(DWORD)(m_TreeCtrl.GetItemData(hSelected));
	   if (dw!=0xffff)	     //выбрали уровень теста => разрешить измерение
	   { 
	     i = dw/1000;		 //i - элемент
	     k = dw%1000;		 //k - тест
		 if (k < NT)
		 { i_SelUrov = 2;    //выделенный уровень:2-тест
		 }
	     else 
		 { i_SelUrov = 1;    //выделенный уровень:1-элемент
		   k = 0;
		 }
         i_EL       = i;     //текущий номер элемента
         i_MOD      = k;     //текущий номер теста

//	     AfxGetMainWnd()->SendMessage(WM_COMMAND,IDC_TREE1,0);
//		 if (i_SelUrov) { m_bEditCut = TRUE; }

//поиск нового выбранного теста в списке
//--------------------------------------
//		 SelchangedTree(); 
		 CString str;
		 pModSelected = NULL;
		 pModSelected = FindTest(m_masStrEl[i_EL],m_masStrMod[i_EL][i_MOD]);
		 if (pModSelected)
		 {  
		  if (!ControlSwitch())
		  {
//==========
			ExtractParam();		//экстракция параметров
//			UpdateData(FALSE);		//отобразить параметры в окошках
			str_EL   = pModSelected->m_EL;
			str_MOD  = pModSelected->m_MOD;
			str_PRIM = pModSelected->m_PRIM;
			strTestName = str_EL+_T(" \\ ")+str_MOD;	//_T(" Тест:  ")+текущая модель измерения (выбранный тест)
			SetTextInfo(_T(" TEST:  ")+strTestName);//отобразить выбранный тест в информационном окне
//			InitGnezdo();	//4.12.17
		  SetStringsSourceMeas();//@ 11.12.17	
			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала
			UpdateData(FALSE);		//отобразить параметры в окошках
//			SetTest();

//==========			  
			  
			  int nTab = 0;			//отобразить закладку "Схема включения"
			 m_TabCtrlParam.SetCurSel(nTab);
			 InitPage(nTab);

			 b_InitAxis=true;
			 int error = ControlParam();
		  }
		  else 
		  { pModSelected = NULL;
		  }
		 }
		 else 
		 {  
			i_EL = 0;
			i_MOD = 0;
			i_SelUrov = 0;
			str_EL		= _T("");
			str_MOD		= _T("");
			strTestName   = _T("");
			str_PRIM = _T("");
			SetTextInfo(_T("Выберите тест для измерения из файла модели!"));
			bAutomat = HAND; // "ручной"
		 }
	   }
	}	
	*pResult = 0;
}

void CLineDlg::OnTablMin() 
{
	// TODO: Add your command handler code here
	bStatusTablRes = stateMin;
	ClientToScreen(&RectTablRes);
	RectTablRes.top=m_RectWndGraf.bottom+3;
	ScreenToClient(&RectTablRes);
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_LIST_RESULT);
	pSt->MoveWindow(&RectTablRes);
	InitTablRes();
}

void CLineDlg::OnTablresSave() 
{
	// TODO: Add your command handler code here
	SaveTablResult();
}

void CLineDlg::SaveTablResult()
{

	UINT s,i;
	int j;
	int	m_isposob=0;
	CString str,strOtcht;

	int n_point = NPOINT;
	if (m_bLooping) n_point = NPOINT100;
//	if (m_itipimp==TIP_IMP) n_point = 1;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
//	if (m_irazv==DC_POS || m_irazv==DC_NEG) n_point = n_Result;
	if (m_irazv==DC_POS || m_irazv==DC_NEG) 
	{ if (n_Result==0) n_Result=1;
	  if ( bMeas == MEAS_SINGLE) n_point = uNpointsLupa;
							else n_point = n_Result;
	  if (m_iLimitKolU==0) n_point=1;//для вывода в таблицу только одной точки
	}
	UINT n_mas = 1;
//	if (m_itipimp==TIP_IMP) n_mas = n_Mas;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_mas = n_Mas;
	UpdateData();
// if (MessageBox(_T("Сохранение займёт некоторое время!\nВы уверены, что хотите сохранить таблицу результатов измерения?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDYES)
// {//0)

	CFileDlg dlg;
	dlg.m_sTitle = _T("Сохранить таблицу результатов");
	dlg.m_iWhatDo = FILE_SAVE;
	dlg.m_sFilter =_T("Files (\\Hard Disk\\Xls\\*.xls)");
	dlg.m_sCatalog =_T("\\Hard Disk\\Xls\\*.xls");
	dlg.m_sPath = _T("\\Hard Disk\\Xls\\");
	dlg.m_sName = _T("*.xls");
	dlg.m_itip = FILE_XLS;
	dlg.m_sFullNameFile = _T("");
	dlg.m_isposob = m_isposob;
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
      m_isposob = dlg.m_isposob;
	  CString name = dlg.m_sFullNameFile;
			CFile file;
			CString stre=_T("");
			int l;
			CCmdTarget::BeginWaitCursor();		
			if (m_isposob)
			{ //обновить данные
			  file.Open((LPCTSTR)name,CFile::modeCreate|CFile::modeWrite);//|CFile::typeBinary);
			  file.SeekToBegin();
			  //заголовок таблицы	
//			  str.Format(_T("Результаты измерения\r\nФайл:%s\r\n"),name);
//			  stre=stre+str;
			}
		    else
			{ //добавить данные
			  file.Open((LPCTSTR)name,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);//|CFile::typeBinary);
			  file.SeekToEnd();
			}
			//формирование данных
		    //1) дата проведения измерения 
//			str.Format(_T("Дата:%s\r\nВремя:%s\r\nЭлемент:%s\r\nТест:%s\r\n%s\r\n"),strDateMeas,strTimeMeas,m_EL,m_MOD,m_PRIM);
			str.Format(_T("%s\\%s\t%s\t%s\t"),m_EL,m_MOD,strDateMeas,strTimeMeas);
			stre=stre+str;
			int n = array_otcht.GetSize();
			if (n) strOtcht = array_otcht.GetAt(n-1);	//строка из отчёта
//????		stre=stre+strOtcht;//m_sInfoTest;//strTestName;
			//2) рассчетные данные из отчета
    		if (!strOtcht.IsEmpty())
			{ int p1=strOtcht.Find(_T(":"),9);	//9 - пропустили время с ":"
			  int p2=strOtcht.Find(_T("\n"),p1);
			  str = strOtcht.Mid(p1+2,p2-p1-1);	//рассчетные данные
			  stre = stre + str+_T("\r\n");
			}
			//3) наименование колонок 
			str=strNameColumn;//+_T("\r\n");		//строка с наименованиями колонок
			stre = stre +str;
			//4) измеренные данные

			for (j=0;j<n_Step;j++)
			for (s=0;s<n_mas;s++)
			for (i=0;i<n_point;i++)
			{  str =  strData[s][i+n_point*j][0]+_T("\t")+	
				      strData[s][i+n_point*j][1]+_T("\t")+
					  strData[s][i+n_point*j][2]+_T("\t")+
					  strData[s][i+n_point*j][3]+_T("\t")+
					  strData[s][i+n_point*j][4]+_T("\t")+
					  strData[s][i+n_point*j][5]+_T("\r\n");	//данные + переход к следующей строке
			   str.Replace('.', ',');
			   stre = stre +str;
			}
//			str=_T("\n");
//			  stre = stre + str;
			l=stre.GetLength()+1;
			BYTE* buf_byte = new BYTE[l];
			FromUnicodeToAscii(buf_byte,stre);
			file.Write(buf_byte,l);

			file.Close();
			delete[] buf_byte;
		    SetModifiedMemory();
		    EndWaitCursor();
			MessageBox(_T("Сохранение завершено успешно!"),NULL,MB_OK|MB_ICONINFORMATION);	
	}
// }//0)
}
//возвращает (число символов в строке +1(\0) +1(индекс для след.записи)
int CLineDlg::FromUnicodeToAscii(BYTE* c_ascii,CString strUnicode) 
{
		for (int i=0;i<strUnicode.GetLength();i++)
		{
		if (strUnicode.GetAt(i) == L'№') c_ascii[i]=(BYTE)0xb9;
		else if ((strUnicode.GetAt(i) >= L'А')&& (strUnicode.GetAt(i) <= L'я'))
			c_ascii[i]=(BYTE)(strUnicode.GetAt(i)-0x350);
		else
			c_ascii[i]=(BYTE)strUnicode.GetAt(i);
		}
		c_ascii[i]='\0';
		return i+1;
}

void CLineDlg::AutoSaveTablResult()
{
	TRACE(_T("\n AutoSaveTablResult()!!!"));
	UINT s,i,j;
	CString str,strDate,strMonth,strYear;
//	CString strHour,strMin;
	CString stre=_T("");
	CFile file;
	ULONGLONG dwLength = 0;
	UINT l;
	CFileException e;

	strDate=strDateCurrent.Mid(0,2);
	strMonth=strDateCurrent.Mid(3,2);
	strYear=strDateCurrent.Mid(8,2);
//	strHour=strTimeCurrent.Mid(0,2);
//	strMin=strTimeCurrent.Mid(3,2);


	UINT n_point = NPOINT;
	if (m_bLooping) n_point = NPOINT100;
//	if (m_itipimp==TIP_IMP) n_point = 1;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point=1;
//	if (m_irazv==DC_POS || m_irazv==DC_NEG) n_point = n_Result;
	if (m_irazv==DC_POS || m_irazv==DC_NEG) 
	{ if (n_Result==0) n_Result=1;
	  if ( bMeas == MEAS_SINGLE) n_point = uNpointsLupa;
							else n_point = n_Result;
	  if (m_iLimitKolU==0) n_point=1;//для вывода в таблицу только одной точки
	}
	UINT n_mas = 1;
//	if (m_itipimp==TIP_IMP) n_mas = n_Mas;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_mas = n_Mas;

	UpdateData();
/*	strDate=strDateMeas.Mid(0,2);
	strMonth=strDateMeas.Mid(3,2);
	strYear=strDateMeas.Mid(8,2);

	str.Format(_T("%s_%s_%s_%s%s%s.xls"),m_sProduct,m_sPart,m_sPlast,strDate,strMonth,strYear);//strDateMeas);
	CString name = _T("\\Disk\\Xls\\")+str;
*/	
	  str.Format(_T("%s_%s_%s_%s%s%s_%u.xls"),m_sProduct,m_sPart,m_sPlast,strDate,strMonth,strYear,uFileNumber);
//	  nameXLS = _T("\\Disk\\Xls\\")+str;
	  nameXLS = _T("\\Hard Disk\\FTPRoot\\Xls\\")+str;
//	  nameXLS = _T("\\Hard Disk\\Cosmos\\Xls\\")+str;


//	file.Open((LPCTSTR)nameXLS,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	if (!file.Open((LPCTSTR)nameXLS,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
	{
#ifdef _DEBUG
  afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
	}
	else
	{
	file.SeekToEnd();
	//формирование данных
	strDateMeas = strDateCurrent;//28.01.2016
    strTimeMeas = strTimeCurrent;
	//1) дата проведения измерения 
	str.Format(_T("%s\\%s\t%s\t%s\t%s\r\n"),m_EL,m_MOD,strDateMeas,strTimeMeas,m_sPoint);
	stre=stre+str;
	//2) наименование колонок 
	str=strNameColumn;//+_T("\r\n");		//строка с наименованиями колонок
	stre = stre +str;
	//3) измеренные данные
	for (j=0;j<n_Step;j++)
	for (s=0;s<n_mas;s++)
	for (i=0;i<n_point;i++)
	{  str =  strData[s][i+n_point*j][0]+_T("\t")+	
	   strData[s][i+n_point*j][1]+_T("\t")+
	   strData[s][i+n_point*j][2]+_T("\t")+
	   strData[s][i+n_point*j][3]+_T("\t")+
	   strData[s][i+n_point*j][4]+_T("\t")+
	   strData[s][i+n_point*j][5]+_T("\r\n");	//данные + переход к следующей строке
	   str.Replace('.', ',');
	   stre = stre +str;
	}
	l=stre.GetLength()+1;
	BYTE* buf_byte = new BYTE[l];
	FromUnicodeToAscii(buf_byte,stre);
	file.Write(buf_byte,l);
	stre.ReleaseBuffer();

	delete[] buf_byte;

	dwLength = file.GetLength();

	file.Close();

	if (dwLength>=2000000)//MAX_FILE_LENGTH)
	{
	  //сохранение соотв. отчета
	  str.Format(_T("%s_%s_%s_%s%s%s_%u.txt"),m_sProduct,m_sPart,m_sPlast,strDate,strMonth,strYear,uFileNumber);
	  nameOtch = _T("\\Hard Disk\\FTPRoot\\Txt\\")+str;
//	  nameOtch = _T("\\Hard Disk\\Cosmos\\Txt\\")+str;
	  file.Open((LPCTSTR)nameOtch,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	  file.SeekToBegin();
	  stre =_T("");
	  if (uFileNumber==0)
	  {
			if (m_sFIO!=_T("")) stre = stre + _T("Измерения проводил:\t ")+m_sFIO+_T("\r\n");
			if (m_sPost!=_T("")) stre = stre + _T("Должность: ")+m_sPost+_T("\r\n");
	  }
	  UINT n = array_otcht.GetSize();
	  for(i=0;i<n;i++)	
	  { str = array_otcht.GetAt(i);
	    stre=stre+str;
	  }
	  l=stre.GetLength()+1;
	  BYTE*  buf_byte_o = new BYTE[l];
	  FromUnicodeToAscii(buf_byte_o,stre);
	  file.Write(buf_byte_o,l);
	  stre.ReleaseBuffer();
 	  file.Close();
	  delete[] buf_byte_o;

	  array_otcht.RemoveAll();


	  uFileNumber++;
	  str.Format(_T("%s_%s_%s_%s%s%s_%u.xls"),m_sProduct,m_sPart,m_sPlast,strDate,strMonth,strYear,uFileNumber);
//	  nameXLS = _T("\\Disk\\Xls\\")+str;
	  nameXLS = _T("\\Hard Disk\\FTPRoot\\Xls\\")+str;
//	  nameXLS = _T("\\Hard Disk\\Cosmos\\Xls\\")+str;
	}	

    SetModifiedMemory();
	}

TRACE(_T("AutoSaveTablResult()!!!"));

}


void CLineDlg::OnMarkerAkt() 
{
	// TODO: Add your command handler code here
  if (!nCheckParam)
  {
	nCheckParam = true;	//!nCheckParam;
	//маркеры активны  отображение параметров измерения
	ShowControls(aVid1,SIZEOF_ARRAY(aVid1),FALSE);
	ShowControls(aVid2,SIZEOF_ARRAY(aVid2),TRUE);
	MarkerAkt();
  }
}

void CLineDlg::MarkerNoakt() 
{ 
// int nTab = m_TabCtrlParam.GetCurSel();
  
  if (m_bTangent)
  { m_bTangent=false;
    m_Chart.DrawSecant(false);
  }
  m_Chart.DeleteMarker(m_uN);//№ кривой
  m_Chart.DeleteMarker(m_uN);//№ кривой
  m_iMarker = MARKER0;
  m_Chart.RefreshCtrl();
}

void CLineDlg::MarkerAkt() 
{
	ResetMarkers();
}

void CLineDlg::OnMarkerNoakt() 
{
	// TODO: Add your command handler code here
  if (nCheckParam)
  {
	nCheckParam = false;	//!;
	//маркеры неактивны отoбражение панели задания параметров измерения				
  ShowControls(aVid2,SIZEOF_ARRAY(aVid2),FALSE);
  ShowControls(aVid1,SIZEOF_ARRAY(aVid1),TRUE);
	MarkerNoakt();
  }
}

void CLineDlg::OnDelTstUser() 
{//загрузить тест из библиотеки пользовательских тестов
	CDelTstDlg dlg;
	dlg.what_directory = TST_USER;
	dlg.m_nelement = m_nelement;
	dlg.bFileOpen = m_bFileOpen;
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{
		  UpdateData();
		//загрузить тест
		  CString str = dlg.m_sname_tst;
		  nameTst = strPassTstUser + str;
		  int l=str.GetLength();
		  m_MOD=str.Mid(0,l-4);
 	      BOOL b = ExtractParamFromTst();
		  if (b)
		  { 
	        UpdateData(FALSE);
		  SetStringsSourceMeas();//@ 11.12.17	
	switch(m_nelement)
	{   case DIOD:   m_EL =_T("D");break;
		case TS:    m_EL =_T("TS");break;
		case NPN:	m_EL =_T("NPN");break;
		case NPN4:	m_EL =_T("NPN4");break;
		case PNP:	m_EL =_T("PNP");break;
		case PNP4:	m_EL =_T("PNP4");break;
		case NMOS:	m_EL =_T("NMOS");break;
		case NMOS4:	m_EL =_T("NMOS4");break;
		case PMOS:	m_EL =_T("PMOS");break;
		case PMOS4:	m_EL =_T("PMOS4");break;
	}
		    str_EL = m_EL;
	        str_MOD = m_MOD;
	        strTestName = str_EL + _T(" \\ ") + str_MOD;//str_MOD;
	        SetTextInfo(_T(" TEST:  ") + strTestName);
			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала

			int  nTab = 0;
			m_TabCtrlParam.SetCurSel(nTab);
			InitPage(nTab);	
			b_InitAxis=true;
			ControlParam();
		  }
		  else
		  {//файл отсутствует 
			str_EL		= _T("");
			str_MOD		= _T("");
			strTestName   = _T("");
			str_PRIM = _T("");
			SetTextInfo(_T("Выберите тест для измерения!"));
		  }
	}
	else SetModifiedMemory();
	
}

void CLineDlg::OnDelTstStandart() 
{//загрузить тест из библиотеки стандартных тестов
	CDelTstDlg dlg;
	dlg.what_directory = TST_STANDART;
	dlg.m_nelement = m_nelement;
	dlg.bFileOpen = m_bFileOpen;
	UpdateData(false);
	if (dlg.DoModal()==IDOK)
	{
	   UpdateData();
		//загрузить тест
		  CString str = dlg.m_sname_tst;
		  nameTst = strPassTstStandart + str;
		  int l=str.GetLength();
		  m_MOD=str.Mid(0,l-4);
 	      BOOL b = ExtractParamFromTst();
		  if (b)
		  { 
	        UpdateData(FALSE);
		  SetStringsSourceMeas();//@ 11.12.17	
	switch(m_nelement)
	{   case DIOD:   m_EL =_T("D");break;
		case TS:    m_EL =_T("TS");break;
		case NPN:	m_EL =_T("NPN");break;
		case NPN4:	m_EL =_T("NPN4");break;
		case PNP:	m_EL =_T("PNP");break;
		case PNP4:	m_EL =_T("PNP4");break;
		case NMOS:	m_EL =_T("NMOS");break;
		case NMOS4:	m_EL =_T("NMOS4");break;
		case PMOS:	m_EL =_T("PMOS");break;
		case PMOS4:	m_EL =_T("PMOS4");break;
	}

		    str_EL = m_EL;
	        str_MOD = m_MOD;
	        strTestName = str_EL + _T(" \\ ") + str_MOD;//str_MOD;
	        SetTextInfo(_T(" TEST:  ") + strTestName);

			InitDiapazonKol();	//инициализация диапазонов коллектора (в зависимости от вида развёртки)
			InitDiapazonBaza();	//инициализация диапазонов базы
			InitDiapazonDop();	//инициализация диапазонов доп.канала

	        int  nTab = 0;
        	m_TabCtrlParam.SetCurSel(nTab);
	        InitPage(nTab);	
			b_InitAxis=true;
			ControlParam();
		  }
		  else
		  {//файл отсутствует 
			str_EL		= _T("");
			str_MOD		= _T("");
			strTestName   = _T("");
			str_PRIM = _T("");
			SetTextInfo(_T("Выберите тест для измерения!"));
		  }
	}
	else SetModifiedMemory();
}


void CLineDlg::OnFileDelete() 
{
	CParolDlg dlgp;
	dlgp.what_save = FILE_MOD;
	UpdateData(false);
	if (dlgp.DoModal()==IDOK)
	{   UpdateData();
		if (m_sparol == dlgp.m_sparol) 
		{ //пароль верный
			CFileDlg* pdlg = new CFileDlg(this);
			pdlg->m_sTitle = _T("Укажите имя удаляемого файла");
			pdlg->m_iWhatDo = FILE_DELETE;
			pdlg->m_sFilter =_T("Line Files (\\Hard Disk\\Mod\\*.mod)");
			pdlg->m_sCatalog =_T("\\Hard Disk\\Mod\\*.mod");
			pdlg->m_sPath = _T("\\Hard Disk\\Mod\\");
			pdlg->m_sName = _T("*.mod");
			pdlg->m_itip = FILE_MOD;
			pdlg->m_sFullNameFile = _T("");
			UpdateData(false);
			pdlg->DoModal();
			delete pdlg;
			m_bModifiedMemory = true;
		}
		else MessageBox(_T("Неверный пароль!"),NULL,MB_OK|MB_ICONERROR);
	}
	else MessageBox(_T("Пароль не введен!"),NULL,MB_OK|MB_ICONEXCLAMATION);

}

void CLineDlg::OnTablDelete() 
{
	// TODO: Add your command handler code here
			CFileDlg* pdlg = new CFileDlg(this);
			pdlg->m_sTitle = _T("Укажите имя удаляемого файла");
			pdlg->m_iWhatDo = FILE_DELETE;
			pdlg->m_sFilter =_T("Files (\\Hard Disk\\Xls\\*.xls)");
			pdlg->m_sCatalog =_T("\\Hard Disk\\Xls\\*.xls");
			pdlg->m_sPath = _T("\\Hard Disk\\Xls\\");
			pdlg->m_sName = _T("*.xls");
			pdlg->m_itip = FILE_XLS;
			pdlg->m_sFullNameFile = _T("");
			UpdateData(false);
			pdlg->DoModal();
			delete pdlg;
			m_bModifiedMemory = true;
}

void CLineDlg::OnOtchtSave() 
{
	// TODO: Add your command handler code here

	CString	strFullPathOtch;
	CString strDate,strMonth,strYear;
	CString	strHour,strMin;
	CString str,stre;
	CFile   file;
	int     n=0;
	int		l=0;
	int		i=0;
	CFileException er;
	
	strDate=strDateCurrent.Mid(0,2);
	strMonth=strDateCurrent.Mid(3,2);
	strYear=strDateCurrent.Mid(8,2);
	strHour=strTimeCurrent.Mid(0,2);
	strMin=strTimeCurrent.Mid(3,2);
	
	str = _T("_")+strDate+strMonth+strYear+_T("_")+strHour+strMin+_T(".txt");

	CSaveOtchDlg dlg;
	dlg.m_isposob = 1;
	dlg.m_sName = m_sProduct;
	dlg.m_sData = str;
	dlg.m_sFile = m_sProduct + str;
	strFullPathOtch = strPassOtch + m_sProduct + str;
	dlg.m_sFIO = m_sFIO;
	dlg.m_sPost = m_sPost;
	UpdateData(FALSE);
	if (dlg.DoModal()==IDOK)
	{ UpdateData();
	  m_sNameOtch = dlg.m_sFile;
      int isposob = dlg.m_isposob;
	  m_sFIO = dlg.m_sFIO;
	  m_sPost = dlg.m_sPost;
	  strFullPathOtch = strPassOtch + m_sNameOtch;
	  BOOL b = false;
	  if (isposob)
	  { //обновить данные
	    b = file.Open((LPCTSTR)strFullPathOtch,CFile::modeCreate|CFile::modeWrite);//|CFile::typeBinary);
	    if (b)
	    file.SeekToBegin();
	  }
	  else
	  { //добавить данные
	    b = file.Open((LPCTSTR)strFullPathOtch,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);//|CFile::typeBinary);
	    if (b) file.SeekToEnd();
	  }
	    if (!b)
		{ TCHAR szError[1024];
          er.GetErrorMessage(szError, 1024);
	      str.Format(_T("Ошибка сохранения файла отчёта %s"),szError);
	      TRACE(str);
   		}
        else
		{//1 
			//сохранение 
			CCmdTarget::BeginWaitCursor();
			stre.Format(_T("Файл отчета:\t%s\r\n"),m_sNameOtch);
			if (dlg.m_sPrim!=_T("")) stre = stre + _T("Комментарий:\t")+dlg.m_sPrim+_T("\r\n");
			if (m_sFIO!=_T("")) stre = stre + _T("Измерения проводил:\t ")+m_sFIO+_T("\r\n");
			if (m_sPost!=_T("")) stre = stre + _T("Должность: ")+m_sPost+_T("\r\n");
			n = array_otcht.GetSize();
			for(i=0;i<n;i++)	
			{ str = array_otcht.GetAt(i);
			  stre=stre+str;
			}
			l=stre.GetLength()+1;
			  BYTE*  buf_byte = new BYTE[l];
			  FromUnicodeToAscii(buf_byte,stre);
			  file.Write(buf_byte,l);
			stre.ReleaseBuffer();
 
			file.Close();
			delete[] buf_byte;

			SetModifiedMemory();

			EndWaitCursor();
			MessageBox(_T("Сохранение отчёта завершено успешно!"),NULL,MB_OK|MB_ICONINFORMATION);	
		}//1
	}
}

void CLineDlg::OnDeleteOtch() 
{
	// TODO: Add your command handler code here
			CFileDlg* pdlg = new CFileDlg(this);
			pdlg->m_sTitle = _T("Укажите имя удаляемого файла");
			pdlg->m_iWhatDo = FILE_DELETE;
			pdlg->m_sFilter =_T("Files (\\Hard Disk\\Txt\\*.txt)");
			pdlg->m_sCatalog =_T("\\Hard Disk\\Txt\\*.txt");
			pdlg->m_sPath = _T("\\Hard Disk\\Txt\\");
			pdlg->m_sName = _T("*.txt");
			pdlg->m_itip = FILE_TXT;
			pdlg->m_sFullNameFile = _T("");
			UpdateData(false);
			pdlg->DoModal();
			delete pdlg;
			m_bModifiedMemory = true;

}

void CLineDlg::OnTuningCopyfile() 
{
	// TODO: Add your command handler code here
			CCopyFileDlg* pdlg = new CCopyFileDlg(this);
			UpdateData(false);
			pdlg->DoModal();
			delete pdlg;
			m_bModifiedMemory = true;
	
}

void CLineDlg::OnDeltaposSpinstepbaza(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
//	UpdateData();
//	b_InitAxis=true;

//	ControlParam();
	*pResult = 0;
}

void CLineDlg::OnDeltaposSpinstepdop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
//	UpdateData();
//	ControlParam();
	
	*pResult = 0;
}

void CLineDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CLineDlg::OnCheckLooping() 
{
	// TODO: Add your control notification handler code here
	m_bLooping =! m_bLooping;
	m_bLoopingOld = m_bLooping;
	UpdateData(false);
	
}


void CLineDlg::ShowButtonLoopingCompensation()
{
	TRACE(_T("ShowButtonLoopingCompensation()\n"));
//	UpdateData();		//убрала 29.11.17
//  if ((m_itipimp==TIP_IMP) || (m_irazv==DC_POS || m_irazv==DC_NEG))
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG)
  { GetDlgItem(IDC_CHECK_LOOPING)->EnableWindow(false);//ShowWindow(SW_HIDE);//компенсация "петли" не доступна
//	GetDlgItem(IDC_STATIC_LOOPING)->EnableWindow(false);
//	GetDlgItem(IDC_STATIC_LOOPING)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECKKOLREJHIGH)->EnableWindow(true);//ShowWindow(SW_SHOW);//"Экранная лупа" доступна
//	GetDlgItem(IDC_STATIC_ICON_LUPA)->ShowWindow(SW_SHOW);
    m_bLoopingOld = m_bLooping;
	m_bLooping = false;
    if (m_bCheckKolRejHigh)
	{
	  GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(false);
	  GetDlgItem(IDC_RADIO_CENTER)->EnableWindow(false);
	  GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(false);
	}
	else
	{
	  GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(true);
	  GetDlgItem(IDC_RADIO_CENTER)->EnableWindow(true);
	  GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(true);
	}
  }
  else
  {
	int nTab = m_TabCtrlTuning.GetCurSel();
	if (nTab==0)
	{
	GetDlgItem(IDC_CHECK_LOOPING)->EnableWindow(true);//ShowWindow(SW_SHOW);//компенсация "петли" доступна
//	GetDlgItem(IDC_STATIC_LOOPING)->EnableWindow(true);
//	GetDlgItem(IDC_STATIC_LOOPING)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECKKOLREJHIGH)->EnableWindow(false);//ShowWindow(SW_HIDE);//"Экранная лупа" не доступна
//	GetDlgItem(IDC_STATIC_ICON_LUPA)->ShowWindow(SW_HIDE);
	}
	m_bLooping = m_bLoopingOld;
	m_bCheckKolRejHigh = false;
	  GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(true);
	  GetDlgItem(IDC_RADIO_CENTER)->EnableWindow(true);
	  GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(true);
  }
  if ((m_irazv == AC && m_nelement!= TS) || 
	  m_nelement== DIOD ||
	  m_iQue1==BAZA) 
  {//генерация ступенек не доступна
	  GetDlgItem(IDC_CHECK_FAMILY)->EnableWindow(false);
	  m_bFamilyVkl = false;
	  m_iFamilyVkl = 0;
	  m_bFamilyVklOld = m_bFamilyVkl;
  }
  else
  {//генерация ступенек доступна
	  GetDlgItem(IDC_CHECK_FAMILY)->EnableWindow(true);
	  m_bFamilyVkl = m_bFamilyVklOld;
	  if (m_bFamilyVkl) m_iFamilyVkl=1;
				  else  m_iFamilyVkl=0;
  }
//  UpdateData(false);	//убрала 29.11.17
}


void CLineDlg::LoopingCompensation() 
{
  double temp=0.0;
  int j = stup;

	for(int i=0;i<NPOINT100;i++)
	{  
	  temp = ( pM1[0][i+NPOINT*j] + pM1[0][(199-i)+NPOINT*j] ) / 2.0;
	  pM1[0][i+NPOINT*j] = temp;
	  temp = ( pM2[0][i+NPOINT*j] + pM2[0][(199-i)+NPOINT*j] ) / 2.0;
	  pM2[0][i+NPOINT*j] = temp;
	  temp = ( pM3[0][i+NPOINT*j] + pM3[0][(199-i)+NPOINT*j] ) / 2.0;
	  pM3[0][i+NPOINT*j] = temp;
	  temp = ( pM4[0][i+NPOINT*j] + pM4[0][(199-i)+NPOINT*j] ) / 2.0;
	  pM4[0][i+NPOINT*j] = temp;
	}
}
int CLineDlg::PushKlava(CString strSymbol) 
{
	CString strtitle;
	switch (akt_button)
	{
	case 100: break;
	case 101: break;
	case 102: break;
	case 103: break;
	case 104: break;
	case 105: break;
	case 107: break;
	case 106: 
			  if (strSymbol.GetLength() > 10) strSymbol=strSymbol.Left(10);
			  m_pSaveOtchDlg->m_sName = strSymbol;
		      break;
	}
	return strSymbol.GetLength();
}
//получение строки символов из соотв. строки ввода
CString CLineDlg::GetStringFromEdit() 
{
	CString strEdit;
	switch (akt_button)
		{
		case 106:
			strEdit=((CSaveOtchDlg*)m_pSaveOtchDlg)->m_sName;
		break;
		}
	return strEdit;
}
/*
void CLineDlg::OnSelchangeComboSwitch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	n_Switch = m_iSwitchKol;
//				SetPassWordLine();
	if (n_IPPP && n_Switch==1)
	{
		m_irazv = IMP_POS;
//		m_itipimp = TIP_IMP;
	}
	if (n_IPPP && n_Switch==2)
	{
		m_irazv = IMP_NEG;
//		m_itipimp = TIP_IMP;
	}
	if ((n_IPPP==0) || (n_IPPP && n_Switch==0))
	{
//		m_irazv = IMP_POS;
//		m_itipimp = TIP_SYN;
		m_irazv = SYN_POS;
	}
		UpdateData(false);
		InitIconImp();

				ChangeDiapazonKolSourceU();
				ChangeDiapazonKolMeasU();
				ChangeDiapazonKol();
}
*/

void CLineDlg::OnKillfocusEditur1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ControlParam();
	UpdateData(FALSE);
	
}

void CLineDlg::OnCheckkolrejhigh() 
{
	// TODO: Add your control notification handler code here
	m_bCheckKolRejHigh=!m_bCheckKolRejHigh;
	if(!m_bCheckKolRejHigh)
	{ 
	  GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(true);
	  GetDlgItem(IDC_RADIO_CENTER)->EnableWindow(true);
	  GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(true);
	}
	else
	{ 
	  GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(false);
	  GetDlgItem(IDC_RADIO_CENTER)->EnableWindow(false);
	  GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(false);
	}
//	InitAxis();
	if (bStatusCalc) InitAxisCalc();
	else InitAxis();
}

void CLineDlg::OnGrafLupa() 
{
	// TODO: Add your command handler code here
	
	CLupaDlg lupa;
	m_Chart.m_pAxisBottom->GetMinMaxTickIncrement(MinX,MaxX,StepX);
	m_Chart.m_pAxisLeft->GetMinMaxTickIncrement(MinY,MaxY,StepY);

	lupa.m_sScaleX=ScaleX;//m_sAxisX;
	lupa.m_sScaleY=ScaleY;//m_sAxisY;
	lupa.X_min = MinX;
	lupa.X_max = MaxX;
	lupa.Y_min = MinY;
	lupa.Y_max = MaxY;
	lupa.StepX = StepX;
	lupa.StepY = StepY;
	lupa.m_uPrecision = uPrecigion;//
	lupa.m_bCheckKolRejHigh = m_bCheckKolRejHigh;
	lupa.m_bCheckLooping = 	m_bLooping;
	lupa.m_uNpoints	= uNpointsLupa;
	lupa.m_irazv = m_irazv;
	lupa.m_iQue1 = m_iQue1;

	UpdateData(false);
	if (lupa.DoModal()==IDOK)
	{//переустановить шкалу

		UpdateData(true);

		lupa.X_min= GetNumberFromString(lupa.m_sXmin);
		MinX = lupa.X_min;

		lupa.X_max =  GetNumberFromString(lupa.m_sXmax);
		MaxX = lupa.X_max;

		lupa.Y_min = GetNumberFromString(lupa.m_sYmin);
		MinY = lupa.Y_min;

		lupa.Y_max = GetNumberFromString(lupa.m_sYmax);
	    MaxY = lupa.Y_max;

		lupa.StepX = GetNumberFromString(lupa.m_sStepX);
	    StepX = lupa.StepX;

		lupa.StepY = GetNumberFromString(lupa.m_sStepY);
	    StepY = lupa.StepY;
	 
	    uPrecigion = lupa.m_uPrecision;//

	  if (m_irazv==IMP_POS || m_irazv==IMP_NEG || m_irazv==DC_POS || m_irazv==DC_NEG)
	  {	if (m_iQue1==COLLECTOR)
		{
		  lupa.m_uNpoints = GetNumberFromString(lupa.m_sNpoints);
		  uNpointsLupa = lupa.m_uNpoints;
		  uNpointsLupaDC = lupa.m_uNpoints;
		}
  		  
	  }

	m_Chart.DisableRefresh(true);
      m_Chart.m_pAxisBottom->SetMinMax(MinX,MaxX);
      m_Chart.m_pAxisBottom->SetBigTickIncrement(StepX);
	  m_Chart.m_pAxisLeft->SetMinMax(MinY,MaxY);
	  m_Chart.m_pAxisLeft->SetBigTickIncrement(StepY);
	m_Chart.DisableRefresh(false);
//	 InitAxis();

	}
}

double CLineDlg::GetNumberFromString(CString str)
{
	double num;
	char* lpsz=new char[str.GetLength()+1];
	for (int i=0;i<str.GetLength();i++)
	{
	lpsz[i]=(char)str.GetAt(i);
	}
	lpsz[i]=0;
	num=atof(lpsz);
	delete []lpsz;
	return num;
}

void CLineDlg::OnKalibr() 
{
	// TODO: Add your command handler code here
	CString str;
if (bZapusk)
  MessageBox(_T("Отключите внешний запуск!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
else
{//0
  if (bAutoSetNullIc_10nA==true)
  {
  MessageBox(_T("Отсоедините перемычку \".027\", замыкающую гнезда Uc и E'!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
  bAutoSetNullIc_10nA=false;
  }
  else
  {//1
		  str = _T("КАЛИБРОВКА");
		  SetTextInfo(str);
		CKalibrDlg dlgk;
		dlgk.m_sserial = strpsw;
		dlgk.m_sdatek = strdatekalibr;
		dlgk.m_iNsetK = m_iNsetK;
		dlgk.m_iRange = m_iRange;
		UpdateData(FALSE);
		if (dlgk.DoModal()==IDOK)
		{//2
	      UpdateData();
		  strdatekalibr  = dlgk.m_sdatek ;
		  m_iNsetK = dlgk.m_iNsetK;
		  m_iRange = dlgk.m_iRange;


/*		  if ((n_IPPP==1 && m_iNsetK==0 && m_iRange>=4 && (n_Switch==1 || n_Switch==2))||
			  (n_IPPP==1 && m_iNsetK>0 && (n_Switch==1 || n_Switch==2)))
povt1:	  {  MessageBox(_T("Установите переключатель в положение ±3000V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
		     if (n_Switch!=0) goto povt1;
		  }
		  else
		  { if ((n_IPPP==1 && m_iNsetK==0 && m_iRange<4 && n_Switch==0)||
				(n_IPPP==1 && m_iNsetK==1 && m_iRange>8 && n_Switch==0))
povt2:	    {  MessageBox(_T("Установите переключатель в положение +5V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
			   if (n_Switch==0) goto povt2;
			}
		  }
*/
			if (n_IPPP==0 || (n_IPPP==1 && n_Switch==0))
			{
				bias_Ucs = 1;	//bias_KolSourceU;
				bias_Uc =  0;	//bias_KolMeasU;
				bias_Ic =  5;//4;//bias_KolMeasI;	//0;
			}
			if (n_IPPP==1 && n_Switch!=0)
			{
				bias_Ucs = 0;	//bias_KolSourceU;
				bias_Uc =  0;//9;	//bias_KolMeasU;
				bias_Ic =  0;	//bias_KolMeasI;
			}
			  if (n_IPPP==1)
			  {   //калибровка Uc и канала B и S
			  //-----------------------------
				  if (m_iNsetK>0 && (n_Switch==1 || n_Switch==2))
povt1:			  {  MessageBox(_T("Установите переключатель в положение ±2000V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
					 if (n_Switch!=0) goto povt1;
				  }
			  //калибровка Ic
			  //-------------
				  if (m_iNsetK==0)
				  { if (m_iRange+bias_Ic<4)//переключатель д. б. +5V
					{ if (n_Switch==0 || n_Switch==2)
povt2:					{ MessageBox(_T("Установите переключатель в положение +5V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
					  if (n_Switch!=1) goto povt2;
						}
					}
				    if (m_iRange+bias_Ic>=4)//переключатель д. б. ±2000V
					{ if (n_Switch==1 || n_Switch==2)
povt3:					{ MessageBox(_T("Установите переключатель в положение ±2000V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
						  if (n_Switch!=0) goto povt3;
						}
					}
				  }//end of if (m_iNsetK==0)
			  }//end of if (n_IPPP==1)

			 bKalibr = true;	
			 m_iNproxod = 0;		//№ прохода
		  
			  int k = 0;
			  k = PuskKalibr();	//k- число байт в массиве команд
			  if (k>0)
			  {
			  // поместить в bufOut управляющие команды
			  ::WaitForSingleObject(hMutexO,INFINITE);
			  NumO = k;
			  bufOut[0]=k+1;								//кол-во байт для передачи + 1 байт с количеством
			  memcpy(&bufOut[1],&My_buf_Out[0],bufOut[0]);	//данные
			  ::ReleaseMutex(hMutexO);
	
			  ResetRef();
			  if (nCheckParam) {nCheckParam=0; MarkerNoakt();}

			  SetTimer(4,250,NULL);
			  ready =0;
			  PurgeComm(hCom2,PURGE_TXCLEAR|PURGE_RXCLEAR);
//			  SetEvent(hEventComOut);//+++++++++++++++++++++//запускаем калибровку
	::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);	//+1 байт, т.к. с кол-вом байт для передачи
			  }
			  else
			  { 
			    bKalibr=false;
			    m_iNproxod=0;
			    str = _T("Калибровка прервана!");
	            SetTextInfo(str);
			  }
			}//2
			else
			{ 
			  bKalibr=false;
			  m_iNproxod=0;
			  str = _T("TEST: ") +m_EL + _T(" \\ ") + m_MOD;
	         SetTextInfo(str);
			}
   }//1
}//0
}

int CLineDlg::PuskKalibr() 
{   int k=0;			//число байт в массиве команд
	double x = 0.0;
	int p = 0;
	int rez = 0;
	CString str;

    Flag_sboy  = FALSE;

//  bias_Ucs = bias_KolSourceU;
//	bias_Uc =  bias_KolMeasU;
//  bias_Ic = bias_KolMeasI;

	bias_Id = bias_Dop_I;
	if (n_IPPP==0 || (n_IPPP==1 && n_Switch==0))

	{
		bias_Ucs = 1;	//bias_KolSourceU;
		bias_Uc =  0;	//bias_KolMeasU;
		bias_Ic =  5;//bias_KolMeasI;	//0;
	}
	if (n_IPPP==1 && n_Switch!=0)
	{
		bias_Ucs = 0;	//bias_KolSourceU;
		bias_Uc =  0;//9;	//bias_KolMeasU;
		bias_Ic =  0;	//bias_KolMeasI;
	}

	m_iBs=-1;	//канал B - обрыв
	m_iBm=-1;	
	m_iDs=-1;	//канал S - обрыв
	m_iDm=-1;	
	
	switch(m_iNsetK)		//№ программы
	{ case 0: str.Format(_T("КАЛИБРОВКА КАНАЛ С\nИЗМЕРИТЕЛЬ I ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iRange+bias_Ic]);
			  k = PuskKalibrIc(0,m_iRange);
			  break;
	  case 1: str.Format(_T("КАЛИБРОВКА КАНАЛ С\nИЗМЕРИТЕЛЬ U ДИАПАЗОН %s"),mas_rangeKolMeasU[m_iRange+bias_Uc]);
			  k = PuskKalibrUc(0,m_iRange);
			  break;
	  case 2: str.Format(_T("КАЛИБРОВКА КАНАЛ B\nИСТОЧНИК I/ИЗМЕРИТЕЛЬ I ДИАПАЗОН %s"),mas_range_meas_bazadop_I[m_iRange]);
			  k = PuskKalibrIb(0,m_iRange);
			  break;
	  case 3: str.Format(_T("КАЛИБРОВКА КАНАЛ B\nИСТОЧНИК U/ИЗМЕРИТЕЛЬ U ДИАПАЗОН %s"),mas_range_meas_bazadop_U[m_iRange]);
			  k = PuskKalibrUb(0,m_iRange);
			  break;
	  case 4: str.Format(_T("КАЛИБРОВКА КАНАЛ S\nИСТОЧНИК I/ИЗМЕРИТЕЛЬ I ДИАПАЗОН %s"),mas_range_meas_bazadop_I[m_iRange+bias_Id]);
			  k = PuskKalibrIs(0,m_iRange);
			  break;
	  case 5: str.Format(_T("КАЛИБРОВКА КАНАЛ S\nИСТОЧНИК U/ИЗМЕРИТЕЛЬ U ДИАПАЗОН %s"),mas_range_meas_bazadop_U[m_iRange]);
			  k = PuskKalibrUs(0,m_iRange);
			  break;
	}
	if (k>0)
	{
//-------------------------------
	My_buf_Out[k] = '\0';	//конец передачи
	k = k + 1;
//-------------------------------
    SetTextInfo(str);
	
	NPOINT = NPOINTS;	//массив из 200 точек
	if (m_ir==0) {NPOINT = mas_n_points[4];//число точек в снимаемом массиве для источника Uc=20V
					m_nMas = 20;
				}
	NumI = NPOINT*8;	//NPOINT точек по 8 байт
	}
  return k;
}

int CLineDlg::PuskKalibrUb(BOOL where, int range) 
{ int k=0;	//k - число байт в массиве команд
			//where = 0 - калибровка в нуле, = 1 - калибровка на краю диапазона
			//range - калибруемый диапазон
	CString str;
	for(int i=0;i<NBUF;i++)
	My_buf_Out[i]=0;	//массив команд
	ClearResults();
    
	nCountPointDC = 0;  //счетчик числа точек для усреднения (20)
	m_nCountMas=0;		//счетчик числа массивов

	   m_ir = 1;		//DC

	   m_iUcs = 0;	//диапазон источника Uc 20V
	   m_iUc = 7;	//20 V
	   m_iIc = 13;	//2 mA
   	   m_iLstart= 0;//ограничение Uc min
	   m_iLstop= 0;	//ограничение Uc max

	   m_iBs = 0;	//ист. Ub
	   m_iBm = 0;	//изм. Ub
	   m_iAb = range;	//калибруемый диапазон
	   if (where==false)  { m_uSb = 0; m_iBb = 0;} //в нуле
					else  { m_uSb = 10; m_iBb = 1000;} //предел диапазона
	   sign_Ab = 0;
	   sign_Bb = 0;	//полярность прямая
	   m_iRb = 12;	//диапазон ограничения = 2 mA
	   m_iLb = 100;	//ограничение - 100%
	   
	   memcpy(&My_buf_Out[k],&mas_Kalibr_Baza[0],SIZEOF_ARRAY(mas_Kalibr_Baza));
	   k = k + SIZEOF_ARRAY(mas_Kalibr_Baza);
	   k = CommandKol(k);
	   k = CommandBazaUbsUbm(k);
	   ready = 0;
return(k);
}

int CLineDlg::PuskKalibrIb(BOOL where, int range) 
{ int k=0;	//k - число байт в массиве команд
			//where = 0 - калибровка в нуле, = 1 - калибровка на краю диапазона
			//range - калибруемый диапазон
	CString str;
	for(int i=0;i<NBUF;i++)
	My_buf_Out[i]=0;	//массив команд
	ClearResults();
    
	nCountPointDC = 0;  //счетчик числа точек для усреднения (20)
	m_nCountMas=0;		//счетчик числа массивов

	   m_ir = 1;		//DC

	   m_iUcs = 0;	//диапазон источника Uc 20V
	   m_iUc = 7;	//20 V
	   m_iIc = 13;	//2 mA
   	   m_iLstart= 0;//ограничение Uc min
	   m_iLstop= 0;	//ограничение Uc max

	   m_iBs = 1;	//ист. Ib
	   m_iBm = 1;	//изм. Ib
	   m_iAb = range;	//калибруемый диапазон
	   if (where==false)  { m_uSb = 0; m_iBb = 0;} //в нуле
					else  { 
							if (m_iAb==0) { m_uSb = 1; m_iBb = 150;}//диапазон 20А
							if (m_iAb==1) { m_uSb = 2; m_iBb = 300;}//диапазон 10А
							if (m_iAb==2) { m_uSb = 6; m_iBb = 650;}//диапазон 4А
							if  (m_iAb>2) { m_uSb = 10; m_iBb = 1000;} //предел диапазона
							}
	   sign_Ab = 0;
	   sign_Bb = 0;	//полярность прямая
	   m_iRb = 0;	//диапазон ограничения - 40 V
	   m_iLb = 100;	//ограничение - 100%
	   
	   memcpy(&My_buf_Out[k],&mas_Kalibr_Baza[0],SIZEOF_ARRAY(mas_Kalibr_Baza));
	   k = k + SIZEOF_ARRAY(mas_Kalibr_Baza);
	   k = CommandKol(k);
	   k = CommandBazaIbsIbm(k);
	   ready = 0;
return(k);
}

int CLineDlg::PuskKalibrUs(BOOL where, int range) 
{ int k=0;	//k - число байт в массиве команд
			//where = 0 - калибровка в нуле, = 1 - калибровка на краю диапазона
			//range - калибруемый диапазон
	CString str;
	for(int i=0;i<NBUF;i++)
	My_buf_Out[i]=0;	//массив команд
	ClearResults();
    
	nCountPointDC = 0;  //счетчик числа точек для усреднения (20)
	m_nCountMas=0;		//счетчик числа массивов

	   m_ir = 1;		//DC

	   m_iUcs = 0;	//диапазон источника Uc 20V
	   m_iUc = 7;	//20 V
	   m_iIc = 13;	//2 mA
   	   m_iLstart= 0;//ограничение Uc min
	   m_iLstop= 0;	//ограничение Uc max

	   m_iBs = 0;	//ист. Ub
	   m_iBm = 1;	//изм. Ib
	   m_iAb = 5;	//диапазон 0.5V
	   m_uSb = 0;
	   m_iBb = 0;
	   sign_Ab = 0;
	   sign_Bb = 0;	//полярность прямая
	   m_iRb = 12;	//диапазон ограничения = 2 mA
	   m_iLb = 100;	//ограничение - 100%

	   bias_Id = 10;
	   m_iDs = 0;	//ист. U
	   m_iDm = 0;	//изм. U
	   m_iAd = range;	//калибруемый диапазон
	   if (where==false)  { m_uSd = 0; m_iBd = 0;} //в нуле
					else  { m_uSd = 10; m_iBd = 1000;} //предел диапазона
	   sign_Ad = 0;
	   sign_Bd = 0;	//полярность прямая
	   bias_Id = 10;
	   m_iRd = 2;	//bias_Id+m_iRd = 12 => диапазон ограничения - 2 mA
	   m_iLd = 100;	//ограничение - 100%
	   
	   memcpy(&My_buf_Out[k],&mas_Kalibr_Dop[0],SIZEOF_ARRAY(mas_Kalibr_Dop));
	   k = k + SIZEOF_ARRAY(mas_Kalibr_Dop);
	   k = CommandKol(k);
	   k = CommandBazaUbsIbm(k);
	   k = CommandDopUssUsm(k);
	   ready = 0;
return(k);
}

int CLineDlg::PuskKalibrIs(BOOL where, int range) 
{ int k=0;	//k - число байт в массиве команд
			//where = 0 - калибровка в нуле, = 1 - калибровка на краю диапазона
			//range - калибруемый диапазон
	CString str;
	for(int i=0;i<NBUF;i++)
	My_buf_Out[i]=0;	//массив команд
	ClearResults();
    
	nCountPointDC = 0;  //счетчик числа точек для усреднения (20)
	m_nCountMas=0;		//счетчик числа массивов

	   m_ir = 1;		//DC

	   m_iUcs = 0;	//диапазон источника Uc 20V
	   m_iUc = 7;	//20 V
	   m_iIc = 13;	//2 mA
   	   m_iLstart= 0;//ограничение Uc min
	   m_iLstop= 0;	//ограничение Uc max

	   m_iBs = 0;	//ист. Ub
	   m_iBm = 1;	//изм. Ib
	   m_iAb = 5;	//диапазон 0.5V
	   m_uSb = 0;
	   m_iBb = 0;
	   sign_Ab = 0;
	   sign_Bb = 0;	//полярность прямая
	   m_iRb = 12;	//диапазон ограничения = 2m
	   m_iLb = 100;	//ограничение - 100%

	   m_iDs = 1;	//ист. I
	   m_iDm = 1;	//изм. I
	   bias_Id = 10;
	   m_iAd = range;	//калибруемый диапазон
	   if (where==false)  { m_uSd = 0; m_iBd = 0;} //в нуле
					else  { m_uSd = 10; m_iBd = 1000;} //предел диапазона
	   sign_Ad = 0;
	   sign_Bd = 0;	//полярность прямая
	   m_iRd = 0;	//диапазон ограничения - 40 V
	   m_iLd = 100;	//ограничение - 100%
	   
	   memcpy(&My_buf_Out[k],&mas_Kalibr_Dop[0],SIZEOF_ARRAY(mas_Kalibr_Dop));
	   k = k + SIZEOF_ARRAY(mas_Kalibr_Dop);
	   k = CommandKol(k);
	   k = CommandBazaUbsIbm(k);
	   k = CommandDopIssIsm(k);
	   ready = 0;
return(k);
}


int CLineDlg::PuskKalibrUc(BOOL where, int range) 
{ int k=0;	//k - число байт в массиве команд
			//where = 0 - калибровка в нуле, = 1 - калибровка на краю диапазона
			//range - калибруемый диапазон
	CString str;

	for(int i=0;i<NBUF;i++)
	My_buf_Out[i]=0;	//массив команд
	ClearResults();
    
	nCountPointDC = 0;  //счетчик числа точек для усреднения (20)
	m_nCountMas=0;		//счетчик числа массивов
	ready = 0;			//готовность
	
	m_iUc = range;
    m_ir = 1;//DC
    m_iUcs = 0;	 //диапазон источника Uc 20V
    m_iIc = 7;//bias_Ic+m_iIc=11 => 200mA //13;  //bias_Ic+m_iIc=17 => 2mA
    m_iLstart= 0;//ограничение Uc min
    m_iLstop= 0;	//ограничение Uc max
    
	int b;

	if (where==false)
	{ value_necessary = 0.0;
	  b = MessageBox(_T("Подайте нулевое напряжение от В1-12!"),_T("  Внимание!"),MB_OKCANCEL|MB_ICONEXCLAMATION);
	}
	else
	{   if (m_iUc>1)
		{
			value_necessary = mas_d_rangeKolMeasU[m_iUc];
			str.Format(_T("Подайте напряжение +%s от В1-12!"),mas_rangeKolMeasU[m_iUc]);
			b = MessageBox(str,_T("  Внимание!"),MB_OKCANCEL|MB_ICONEXCLAMATION);
		}
		else
		{  if (m_iUc==1)//диапазон 2000 V => середина диапазона
			value_necessary = mas_d_rangeKolMeasU[m_iUc]/2;
		   if (m_iUc==0)//диапазон 5000 V => 1/5 диапазона
			value_necessary = mas_d_rangeKolMeasU[m_iUc]/5;

			b = MessageBox(_T("Подайте напряжение +1000 В от В1-12!"),_T("  Внимание!"),MB_OKCANCEL|MB_ICONEXCLAMATION);
		}
	}
	if (b==IDOK)
	{
	 memcpy(&My_buf_Out[k],&mas_KalibrUc[0],SIZEOF_ARRAY(mas_KalibrUc));
	 k = k + SIZEOF_ARRAY(mas_KalibrUc);
	 k = CommandKol(k);
	 ready = 0;
	}
return(k);
}

int CLineDlg::PuskKalibrIc(BOOL where, int range) 
{   int k=0;	//k - число байт в массиве команд
		 	    //where = 0 - калибровка в нуле, = 1 - калибровка на краю диапазона
			    //range - калибруемый диапазон

	for(int i=0;i<NBUF;i++)
	My_buf_Out[i]=0;	//массив команд
	ClearResults();
    
	nCountPointDC = 0;  //счетчик числа точек для усреднения (20)
	m_nCountMas=0;		//счетчик числа массивов

	m_iIc = range;

	if ((bias_Ic+m_iIc)>20)		//((bias_KolMeasI + range)>20)	
	{//0  //Условие 1 (калибровка Ic от 10nA - 100uA)
	   //============================================
	   m_ir = 1;		//DC

	   m_iUcs = 0;	//диапазон источника Uc 20V
	   m_iUc = 0;	//20 V
	   m_iIc = range;
   	   m_iLstart= 0;	//ограничение Uc min
	   m_iLstop= 0;	//ограничение Uc max

	   m_iBs = 0;	//ист. Ub
	   m_iBm = 1;	//изм. Ib
	   m_iAb = mas_Ab_Ic[bias_Ic+m_iIc];//диапазон амплитуды ступени базы по U, соответсв. диапазону Ic
	   if (where==false)  { m_uSb = 0; m_iBb = 0;} //в нуле
					else  { 
//							if (((bias_Ic+m_iIc)>22 && (bias_Ic+m_iIc)<26) ||
//								((bias_Ic+m_iIc)>28 && (bias_Ic+m_iIc)<32))
//							{
								m_uSb = 10; 
								m_iBb = mas_bias_Ub_for_KalibrIc[bias_Ic+m_iIc];//1000;
//							}
//							else{m_uSb = 10; m_iBb = 0;
							
//							}
					} //предел диапазона
	   sign_Ab = 0;
	   sign_Bb = 0;	//полярность прямая
	   m_iRb = 9;//20mA	//13;	//диапазон ограничения - 1 mA
	   m_iLb = 100;	//ограничение - 100%
	   
	   memcpy(&My_buf_Out[k],&mas_KalibrIc_100uA[0],SIZEOF_ARRAY(mas_KalibrIc_100uA));
	   k = k + SIZEOF_ARRAY(mas_KalibrIc_100uA);
	   k = CommandKol(k);
	   k = CommandBazaUbsIbm(k);
	   ready = 0;
	}//0
	else
	{//1 
		if ((bias_Ic+m_iIc)>7)		//if ((bias_KolMeasI + range)>7)
		{//2  //Условие 2 (калибровка Ic от 2A - 200uA)
			//=========================================
			m_ir = 1;		//DC	//Режим 2 

//	       bias_Ucs = bias_KolSourceU;
	       m_iUcs = 0;	//диапазон источника Uc 20V
//	       bias_Uc = bias_KolMeasU;
	       m_iUc = 0;	//20 V
//           bias_Ic = biasKolMeasI;
	       m_iIc = range;
   	       m_iLstart= 0;	//ограничение Uc min
	       m_iLstop= 0;	//ограничение Uc max

	       m_iBs = 1;	//ист. Ib
	       m_iBm = 0;	//изм. Ub
	       m_iAb = mas_Ab_Ic[bias_Ic+m_iIc];//диапазон амплитуды ступени базы по I, соответсв. диапазону Ic
	       if (where==false)  { m_uSb = 0; m_iBb = 0;} //в нуле
					    else  { m_uSb = 10; m_iBb = 0;} //предел диапазона
	       sign_Ab = 1;
	       sign_Bb = 0;
	       m_iRb = 3;	//диапазон ограничения - 4 V
	       m_iLb = 100;	//ограничение - 100%
		   memcpy(&My_buf_Out[k],&mas_KalibrIc_5A[0],SIZEOF_ARRAY(mas_KalibrIc_5A));
		   k = k + SIZEOF_ARRAY(mas_KalibrIc_5A);
	       k = CommandKol(k);
	       k = CommandBazaIbsUbm(k);
	       ready = 0;
		}//2
	    else
		{//2
			  m_ir = 0;		//IMP   	//Режим 3 
//			m_nMas = 20;
//			NPOINT = 5;
//		    bias_Ucs = bias_KolSourceU;
//	        bias_Uc = bias_KolMeasU;
//          bias_Ic = bias_KolMeasI;
	        m_iUcs = 0;		//m_iUcs + bias_Ucs - диапазон источника Uc (20V-ИППП3 или 5V-ИППП3-1)
	        m_iIc = range;	//m_iIc + bias_Ic - калибруемый диапазон
			//m_iUc + bias_Uc - диапазон измерителя Uc:
	        m_iUc = mas_range_meas_Uc[m_iIc+bias_Ic]-bias_Uc;	//m_iUc = 9;	//5 V
//			if (where==false){ //во 2-ой (.) 
  		    m_iLstart= 8;//4;	//ограничение Uc min
//			m_iLstop= 25;   //ограничение Uc max
			m_iLstop= mas_Lstop_Uc_forKalibrIc[m_iIc+bias_Ic];   //ограничение Uc max
/*							} 
					   else { //середина диапазона
   							   m_iLstart= 0;//50;	//ограничение Uc min
							   m_iLstop= 40;//50;	//ограничение Uc max
							
*/
			m_iBs=-1;	//канал B - обрыв
		    m_iBm=-1;	
	        memcpy(&My_buf_Out[k],&mas_KalibrIc_50A[0],SIZEOF_ARRAY(mas_KalibrIc_50A));
	        k = k + SIZEOF_ARRAY(mas_KalibrIc_50A);
	        k = CommandKol(k);
	        ready = 0;
		}//2
	}//1
 return (k);	
}

void CLineDlg::OnUpdateKalibr(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bPusk && !bAutoSetNull && bStop);
	
}

void CLineDlg::OnAutoSetupNull() 
{
	// TODO: Add your command handler code here
	CString str;
	if (bZapusk)
		  MessageBox(_T("Отключите внешний запуск!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
	else
	if (!bAutoSetNull && !bPusk && !bKalibr)
	{
			str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ");
			SetTextInfo(str);
	AutoSetupNull();	
	}
}

void CLineDlg::AutoSetupNull() 
{
	// TODO: Add your command handler code here
	CString str;
		  if (bAutoSetNullIc_10nA==true)//m_iNset!=10 && 
		  {
		  MessageBox(_T("Отсоедините перемычку \".027\", замыкающую гнезда Uc и E'!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
		  bAutoSetNullIc_10nA=false;
		  }
		  else
		  {

	CAutoSetNull dasn;
	dasn.m_iNset = m_iNset;
	dasn.m_iRange = 0;
		UpdateData(FALSE);
		if (dasn.DoModal()==IDOK)
		{
	      UpdateData();
		  m_iNset = dasn.m_iNset;	//№ программы автоустановки нуля
		  m_iRange = dasn.m_iRange;
 


		  if (n_IPPP==1 && n_Switch==1 || n_Switch==2)
povt1:	  {  MessageBox(_T("Установите переключатель в положение ±2000V!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
		     if (n_Switch!=0) goto povt1;
		  }
		  bAutoSetNull = true;
		  nK41=0;
		  
//	CButton* pbut = (CButton*)GetDlgItem(IDC_BUTTON_MEAS);
//		pbut -> SetWindowText(L"Сброс");

		int k = PuskAutoSetupNull();	//k- число байт в массиве команд

		  // поместить в bufOut управляющие команды
		  ::WaitForSingleObject(hMutexO,INFINITE);
		  NumO = k;
		  bufOut[0]=k+1;								//кол-во байт для передачи + 1 байт с количеством
		  memcpy(&bufOut[1],&My_buf_Out[0],bufOut[0]);	//данные
		  ::ReleaseMutex(hMutexO);
		  
		  SetTimer(2,250,NULL);
		  
		  PurgeComm(hCom2,PURGE_TXCLEAR|PURGE_RXCLEAR);

//		  SetEvent(hEventComOut);//+++++++++++++++++++++//запускаем автоустановку нуля
		  	::WriteFile(hCom2,bufOut,(DWORD)NumO+1,&dwWrite,NULL);	//+1 байт, т.к. с кол-вом байт для передачи

TRACE(_T("AutoSetupNull()-WriteFile();\n"));
		}
		else
		{ 
		  bAutoSetNull=false;
		  m_iNproxod=0;
		  str = _T("TEST: ") +m_EL + _T(" \\ ") + m_MOD;
          SetTextInfo(str);
		}
		  }
}


int CLineDlg::PuskAutoSetupNull() 
{   int k=0;			//число байт в массиве команд
	double x = 0.0;
	int p = 0;
	int rez = 0;
	CString str;

	for(int i=0;i<NBUF;i++)
	My_buf_Out[i]=0;	//массив команд
	ClearResults();
	FlagOLClear();
    Flag_sboy  = FALSE;
	m_iNproxod = 0;		//№ прохода
	nCountPointDC = 0;  //счетчик числа точек для усреднения (20)
	m_nCountMas=0;		//счетчик числа массивов
	ready = 0;			//готовность
	sign_Ab = 0;
	sign_Bb = 0;
    sign_Ad = 0;
    sign_Bd = 0;//полярность смещения - прямая
	m_uSb = 0;
	m_uSd = 0;

	bias_Ucs = 0;
    m_iUcs = 1;	//диапазон источника Uc 20V
	bias_Uc = 0;
	m_iUc = 7;	//измеритель Uc - 20V
	bias_Ic = 0;
	m_iIc = 17;	//измеритель Ic - 2mA
	m_iLstart= 0;	//ограничение Uc min
	m_iLstop= 0;	//ограничение Uc max

	m_iBs=-1;	//канал B - обрыв
	m_iBm=-1;	
	m_iDs=-1;	//канал S - обрыв
	m_iDm=-1;	

	switch(m_iNset)		//№ программы
	{ case 0: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИЗМЕРИТЕЛЬ I КАНАЛ С (I > 0.1 mA)");
//			  m_ir = 1;		//DC
			  m_ir = 0;		//IMP
		   	  m_iUc = 7;	//измеритель Uc - 20V
	          m_iIc = 11;//0.2 A		23;	//20 uA		33;	//измеритель Ic - 10 nA   24;	//10uA   
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIcm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIcm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIcm);
			  k = CommandKol(k);
			  break;
	  case 10:// str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИЗМЕРИТЕЛЬ I КАНАЛ С (");
			  bias_Ic = 21;
			  str.Format(_T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ \nИЗМЕРИТЕЛЬ I КАНАЛ С ДИАПАЗОН %s"),mas_rangeKolMeasI[m_iRange+bias_Ic]);
			  m_ir = 1;		//DC
		   	  m_iUc = 8;	//измеритель Uc - 10V
	          m_iIc = m_iRange;   
//		  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIcm_10nA[0],SIZEOF_ARRAY(mas_ControlAutoSetupIcm_10nA));
//			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIcm_10nA);
		  memcpy(&My_buf_Out[k],&mas_AutoSetupIcm_10nA[0],SIZEOF_ARRAY(mas_AutoSetupIcm_10nA));
			  k = k + SIZEOF_ARRAY(mas_AutoSetupIcm_10nA);
			  k = CommandKol(k);
			  break;
	  
	  case 1: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИЗМЕРИТЕЛЬ U КАНАЛ С");
			  m_ir = 0;		//IMP
			  m_iUc = 10;	//2 V		12;	//измеритель Uc - 0.5V
			  m_iIc = 5;	//измеритель Ic - 20 A 	//4;	//50 A		
		      memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUcm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUcm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUcm);
			  k = CommandKol(k);
			  break;
	  case 2: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИЗМЕРИТЕЛЬ I КАНАЛ В");
		      m_ir = 1;		//DC
			  m_iBs = 0;	//источник Ub
			  m_iBm = 1;	//измеритель Ib
			  m_iAb = 0;	//амплитуда ступени - 2B/ст
			  m_iRb = 26;	//диапазон ограничения - 40nA
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbm);
			  k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  break;
	  case 3: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИЗМЕРИТЕЛЬ U КАНАЛ В");
		      m_ir = 1;		//DC
			  m_iBs = 1;	//источник Ib
			  m_iBm = 0;	//измеритель Ub
			  m_iAb = 5;	//диапазон амплитуды ступени - 20mA/ст
			  m_iRb = 5;	//диапазон ограничения - 1V
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbm);
			  k = CommandKol(k);
			  k = CommandBazaIbsUbm(k);
			  break;
	  case 4: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИСТОЧНИК I КАНАЛ В");
		      m_ir = 1;		//DC
			  m_iBs = 1;	//источник Ib
			  m_iBm = 0;	//измеритель Ub
			  m_iAb = 5;	//амплитуда ступени - 20mA/ст
			  m_iRb = 5;	//диапазон ограничения - 1V
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIbs[0],SIZEOF_ARRAY(mas_ControlAutoSetupIbs));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIbs);
			  k = CommandKol(k);
			  k = CommandBazaIbsUbm(k);
			  break;
	  case 5: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИСТОЧНИК U КАНАЛ В");
		      m_ir = 1;		//DC
			  m_iBs = 0;	//источник Ub
			  m_iBm = 1;	//измеритель Ib
			  m_iAb = 0;	//амплитуда ступени - 2B/ст
			  m_iRb = 26;	//диапазон ограничения - 40nA
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUbs[0],SIZEOF_ARRAY(mas_ControlAutoSetupUbs));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUbs);
			  k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  break;
	  case 6: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИЗМЕРИТЕЛЬ I КАНАЛ S");
		      m_ir = 1;		//DC
			  m_iBs = 0;	//источник Ub
			  m_iBm = 1;	//измеритель Ib
			  m_iAb = 0;	//амплитуда ступени - 2B/ст
			  m_iRb = 26;	//диапазон ограничения - 40nA
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupIsm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIsm);
			  k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  m_iDs = 0;	//источник Us
			  m_iDm = 1;	//измеритель Is
			  m_iAd = 0;	//амплитуда ступени - 2B/ст
			  m_iRd = 26;	//диапазон ограничения - 40nA
			  m_iBd = 0;	//смещение - 0V
			  m_iLd = 100;	//ограничение - 100%
			  bias_Id = 10;
			  k = CommandDopUssIsm(k); 
			  break;
	  case 7: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИЗМЕРИТЕЛЬ U КАНАЛ S");
		      m_ir = 1;		//DC
			  m_iBs = 1;	//источник Ib
			  m_iBm = 0;	//измеритель Ub
			  m_iAb = 17;	//амплитуда ступени - 2uA/ст
			  m_iRb = 5;	//диапазон ограничения - 1V
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUsm[0],SIZEOF_ARRAY(mas_ControlAutoSetupUsm));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUsm);
			  k = CommandKol(k);
  			  k = CommandBazaIbsUbm(k);
			  m_iDs = 1;	//источник Is
			  m_iDm = 0;	//измеритель Us
			  m_iAd = 0;	//амплитуда ступени - 0.5mA/ст
			  m_iRd = 5;	//диапазон ограничения - 1V
			  m_iBd = 0;	//смещение - 0V
			  m_iLd = 100;	//ограничение - 100%
			  bias_Id = 10;
			  k = CommandDopIssUsm(k); 
			  break;
	  case 8: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИСТОЧНИК I КАНАЛ S");
		      m_ir = 1;		//DC
			  m_iBs = 0;	//источник Ub
			  m_iBm = 1;	//измеритель Ib
			  m_iAb = 0;	//амплитуда ступени - 2B/ст
				m_uSb = 0;
			  m_iRb = 26;	//диапазон ограничения - 40nA
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupIss[0],SIZEOF_ARRAY(mas_ControlAutoSetupIss));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupIss);
			  k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  m_iDs = 1;	//источник Is
			  m_iDm = 0;	//измеритель Us
			  m_iAd = 0;	//амплитуда ступени - 0.5mA/ст
			  m_iRd = 5;	//диапазон ограничения - 1V
			  m_iBd = 0;	//смещение - 0V
			  m_iLd = 100;	//ограничение - 100%
			  bias_Id = 10;
			  k = CommandDopIssUsm(k);
			  break;
	  case 9: str = _T("КОРРЕКЦИЯ СМЕЩЕНИЯ НУЛЯ\n ИСТОЧНИК U КАНАЛ S");
		      m_ir = 1;		//DC
			  m_iBs = 0;	//источник Ub
			  m_iBm = 1;	//измеритель Ib
			  m_iAb = 0;	//амплитуда ступени - 2B/ст
			  m_iRb = 26;	//диапазон ограничения - 40nA
			  m_iBb = 0;	//смещение - 0V
			  m_iLb = 100;	//ограничение - 100%
			  memcpy(&My_buf_Out[k],&mas_ControlAutoSetupUss[0],SIZEOF_ARRAY(mas_ControlAutoSetupUss));
			  k = k + SIZEOF_ARRAY(mas_ControlAutoSetupUss);
			  k = CommandKol(k);
			  k = CommandBazaUbsIbm(k);
			  m_iDs = 0;	//источник Us
			  m_iDm = 1;	//измеритель Is
			  m_iAd = 0;	//амплитуда ступени - 2B/ст
			  m_iRd = 16;	//диапазон ограничения - 40nA
			  m_iBd = 0;	//смещение - 0V
			  m_iLd = 100;	//ограничение - 100%
			  bias_Id = 10;
			  k = CommandDopUssIsm(k);
			  break;
	}
//-------------------------------
	My_buf_Out[k] = '\0';	//конец передачи
	k = k + 1;
//-------------------------------
    SetTextInfo(str);
	
	NPOINT = NPOINTS;	//массив из 200 точек
	if (m_ir==0) NPOINT = mas_n_points[4];//число точек в снимаемом массиве для источника Uc=20V
	NumI = NPOINT*8;	//NPOINT точек по 8 байт

  return k;
}

int CLineDlg::CommandKol(int k) 
{	
	for (int i=0;i<NREZ;i++)
	{	aM1[i]=0.0;
		aM2[i]=0.0;
		aM3[i]=0.0;
		aM4[i]=0.0;
	}
	//k - количество байт в массиве команд My_buf_Out
		//KK4 - диапазон источника Uc
		KK4[1] = bias_Ucs + m_iUcs;		// диапазон источника (U) коллектора
		memcpy(&My_buf_Out[k],&KK4[0],SIZEOF_ARRAY(KK4));
		k = k + SIZEOF_ARRAY(KK4);
		//KK5 - диапазон измерителя Uc
		KK5[1] = bias_Uc + m_iUc;
		memcpy(&My_buf_Out[k],&KK5[0],SIZEOF_ARRAY(KK5));
		k = k + SIZEOF_ARRAY(KK5);
		//KK6 - диапазон измерителя Ic
	    KK6[1] = bias_Ic + m_iIc;	//диапазон измерителя Ic
	    memcpy(&My_buf_Out[k],&KK6[0],SIZEOF_ARRAY(KK6));
	    k = k + SIZEOF_ARRAY(KK6);
		//KK7 - стоп
	    KK7[1] = m_iLstop;	
	    memcpy(&My_buf_Out[k],&KK7[0],SIZEOF_ARRAY(KK7));
	    k = k + SIZEOF_ARRAY(KK7);
		//KK8 - старт
	    KK8[1] = m_iLstart;
	    memcpy(&My_buf_Out[k],&KK8[0],SIZEOF_ARRAY(KK8));
	    k = k + SIZEOF_ARRAY(KK8);
return(k);
}
	
int CLineDlg::CommandBazaUbsIbm(int k) 
{	//источник-Ub , измеритель-Ib
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;
			  //KK9 - режим источника базы
			  KK9[1] = m_iBs;
			  memcpy(&My_buf_Out[k],&KK9[0],nK[8]);
			  k = k + nK[8];
			  //KK10 - "Диапазон амплитуды ступени базы"
			  KK10[1] = m_iAb;
			  memcpy(&My_buf_Out[k],&KK10[0],nK[9]);
			  k = k + nK[9];
			  // KK11 - число ступенек
			  KK11[1] = m_uSb;
			  memcpy(&My_buf_Out[k],&KK11[0],nK[10]);
			  k = k + nK[10];
			  //KK12 - valueAmplBaza
			  x = mas_d_range_ampl_bazadop_U[m_iAb];
			  if (sign_Ab==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSBU[m_iAb]/ koeff_bazadop_U[m_iAb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK12[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK12[2] = p;
			  memcpy(&My_buf_Out[k],&KK12[0],nK[11]);
			  k = k + nK[11];
			  //KK14 - valueBiasBaza 
			  x = mas_d_range_bias_bazadop_U[m_iAb];
			  x = (x/1000.0)*m_iBb;
			  if (sign_Bb==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSBU[m_iAb]+BSBU[m_iAb])/ koeff_bazadop_U[m_iAb];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK14[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK14[2] = p;
			  memcpy(&My_buf_Out[k],&KK14[0],nK[13]);
			  k = k + nK[13];
			  // K15 - режим измерителя
			  KK15[1] = m_iBm;
			  memcpy(&My_buf_Out[k],&KK15[0],nK[14]);
			  k = k + nK[14];
			  //KK17 - диапазон ограничения
			  KK17[1]=m_iRb;
			  memcpy(&My_buf_Out[k],&KK17[0],nK[16]);
			  k = k + nK[16];
			  //KK18 - valueLimitBaza - ограничение измерителя базы
			  x = mas_d_range_meas_bazadop_I[m_iRb]+mas_d_range_meas_bazadop_I[m_iRb]/10.0;
			  x=(x/110)*m_iLb;
			  x = (x *KLBI[m_iRb]+BLBI[m_iRb])/ koeff_limit_bazadop_I[m_iRb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK18[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK18[2] = p;
			  memcpy(&My_buf_Out[k],&KK18[0],nK[17]);
			  k = k + nK[17];
return(k);
}

int CLineDlg::CommandBazaUbsUbm(int k) 
{	//источник-Ub , измеритель-Ub
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;
			  //KK9 - режим источника базы
			  KK9[1] = m_iBs;
			  memcpy(&My_buf_Out[k],&KK9[0],nK[8]);
			  k = k + nK[8];
			  //KK10 - "Диапазон амплитуды ступени базы"
			  KK10[1] = m_iAb;
			  memcpy(&My_buf_Out[k],&KK10[0],nK[9]);
			  k = k + nK[9];
			  // KK11 - число ступенек
			  KK11[1] = m_uSb;
			  memcpy(&My_buf_Out[k],&KK11[0],nK[10]);
			  k = k + nK[10];
			  //KK12 - valueAmplBaza
			  x = mas_d_range_ampl_bazadop_U[m_iAb];
			  if (sign_Ab==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSBU[m_iAb]/ koeff_bazadop_U[m_iAb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK12[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK12[2] = p;
			  memcpy(&My_buf_Out[k],&KK12[0],nK[11]);
			  k = k + nK[11];
			  //KK14 - valueBiasBaza 
			  x = mas_d_range_bias_bazadop_U[m_iAb];
			  x = (x/1000.0)*m_iBb;
			  if (sign_Bb==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSBU[m_iAb]+BSBU[m_iAb])/ koeff_bazadop_U[m_iAb];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK14[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK14[2] = p;
			  memcpy(&My_buf_Out[k],&KK14[0],nK[13]);
			  k = k + nK[13];
			  // K15 - режим измерителя
			  KK15[1] = m_iBm;
			  memcpy(&My_buf_Out[k],&KK15[0],nK[14]);
			  k = k + nK[14];
			  //KK16 диапазон измерителя
			  KK16[1]=m_iAb;
			  memcpy(&My_buf_Out[k],&KK16[0],nK[15]);
			  k = k + nK[15];
			  //KK17 диапазон ограничения
			  KK17[1]=m_iRb;
			  memcpy(&My_buf_Out[k],&KK17[0],nK[16]);
			  k = k + nK[16];
			  //KK18 - valueLimitBaza - ограничение измерителя базы
			  x = mas_d_range_meas_bazadop_I[m_iRb]+mas_d_range_meas_bazadop_I[m_iRb]/10.0;
			  x=(x/110)*m_iLb;
			  x = (x *KLBI[m_iRb]+BLBI[m_iRb])/ koeff_limit_bazadop_I[m_iRb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK18[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK18[2] = p;
			  memcpy(&My_buf_Out[k],&KK18[0],nK[17]);
			  k = k + nK[17];
return(k);
}


int CLineDlg::CommandBazaIbsUbm(int k) 
{	//источник-Ib , измеритель-Ub
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;

			  //KK9 - режим источника базы
			  KK9[1] = m_iBs;
			  memcpy(&My_buf_Out[k],&KK9[0],nK[8]);
			  k = k + nK[8];
			  //KK10 - "Диапазон амплитуды ступени базы"
			  KK10[1] = m_iAb;
			  memcpy(&My_buf_Out[k],&KK10[0],nK[9]);
			  k = k + nK[9];
			  // KK11 - число ступенек
			  KK11[1] = m_uSb;
			  memcpy(&My_buf_Out[k],&KK11[0],nK[10]);
			  k = k + nK[10];
			  //KK12 - valueAmplBaza
			  x = mas_d_range_ampl_bazadop_I[m_iAb];
			  if (sign_Ab==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSBI[m_iAb]/ koeff_bazadop_I[m_iAb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK12[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK12[2] = p;
			  memcpy(&My_buf_Out[k],&KK12[0],nK[11]);
			  k = k + nK[11];
			  //KK14 - valueBiasBaza 
			  x = mas_d_range_bias_bazadop_I[m_iAb];
			  x = (x/1000.0)*m_iBb;
			  if (sign_Bb==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSBI[m_iAb]+BSBI[m_iAb])/ koeff_bazadop_I[m_iAb];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK14[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK14[2] = p;
			  memcpy(&My_buf_Out[k],&KK14[0],nK[13]);
			  k = k + nK[13];
			  // K15 - режим измерителя
			  KK15[1] = m_iBm;
			  memcpy(&My_buf_Out[k],&KK15[0],nK[14]);
			  k = k + nK[14];
			  //KK17диапазон ограничения
			  KK17[1]=m_iRb;
			  memcpy(&My_buf_Out[k],&KK17[0],nK[16]);
			  k = k + nK[16];
			  //KK18 - valueLimitBaza - ограничение измерителя базы
			  x = mas_d_range_meas_bazadop_U[m_iRb]+mas_d_range_meas_bazadop_U[m_iRb]/10.0;
			  x=(x/110)*m_iLb;
			  x = (x *KLBU[m_iRb]+BLBU[m_iRb])/ koeff_limit_bazadop_U[m_iRb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK18[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK18[2] = p;
			  memcpy(&My_buf_Out[k],&KK18[0],nK[17]);
			  k = k + nK[17];
return(k);
}

int CLineDlg::CommandBazaIbsIbm(int k) 
{	//источник-Ib , измеритель-Ib
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;

			  //KK9 - режим источника базы
			  KK9[1] = m_iBs;
			  memcpy(&My_buf_Out[k],&KK9[0],nK[8]);
			  k = k + nK[8];
			  //KK10 - "Диапазон амплитуды ступени базы"
			  KK10[1] = m_iAb;
			  memcpy(&My_buf_Out[k],&KK10[0],nK[9]);
			  k = k + nK[9];
			  // KK11 - число ступенек
			  KK11[1] = m_uSb;
			  memcpy(&My_buf_Out[k],&KK11[0],nK[10]);
			  k = k + nK[10];
			  //KK12 - valueAmplBaza
			  x = mas_d_range_ampl_bazadop_I[m_iAb];
			  if (sign_Ab==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSBI[m_iAb]/ koeff_bazadop_I[m_iAb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK12[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK12[2] = p;
			  memcpy(&My_buf_Out[k],&KK12[0],nK[11]);
			  k = k + nK[11];
			  //KK14 - valueBiasBaza 
			  x = mas_d_range_bias_bazadop_I[m_iAb];
			  x = (x/1000.0)*m_iBb;
			  if (sign_Bb==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSBI[m_iAb]+BSBI[m_iAb])/ koeff_bazadop_I[m_iAb];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK14[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK14[2] = p;
			  memcpy(&My_buf_Out[k],&KK14[0],nK[13]);
			  k = k + nK[13];
			  // K15 - режим измерителя
			  KK15[1] = m_iBm;
			  memcpy(&My_buf_Out[k],&KK15[0],nK[14]);
			  k = k + nK[14];
			  //KK16 диапазон измерителя
			  KK16[1]=m_iAb;
			  memcpy(&My_buf_Out[k],&KK16[0],nK[15]);
			  k = k + nK[15];
			  //KK17 диапазон ограничения
			  KK17[1]=m_iRb;
			  memcpy(&My_buf_Out[k],&KK17[0],nK[16]);
			  k = k + nK[16];
			  //KK18 - valueLimitBaza - ограничение измерителя базы
			  x = mas_d_range_meas_bazadop_U[m_iRb]+mas_d_range_meas_bazadop_U[m_iRb]/10.0;
			  x=(x/110)*m_iLb;
			  x = (x *KLBU[m_iRb]+BLBU[m_iRb])/ koeff_limit_bazadop_U[m_iRb];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK18[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK18[2] = p;
			  memcpy(&My_buf_Out[k],&KK18[0],nK[17]);
			  k = k + nK[17];
return(k);
}

int CLineDlg::CommandDopUssIsm(int k) 
{	//источник-Us , измеритель-Is
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;

			  //KK19 - режим источника доп.кан.
			  KK19[1] = m_iDs;
			  memcpy(&My_buf_Out[k],&KK19[0],nK[18]);
			  k = k + nK[18];
			  //KK20 - "Диапазон амплитуды ступени доп.кан."
			  KK20[1] = m_iAd;
			  memcpy(&My_buf_Out[k],&KK20[0],nK[19]);
			  k = k + nK[19];
			  // KK21 - число ступенек доп.кан.
			  KK21[1] = m_uSd;
			  memcpy(&My_buf_Out[k],&KK21[0],nK[20]);
			  k = k + nK[20];
			  //KK22 - valueAmplDop
			  x = mas_d_range_ampl_bazadop_U[m_iAd];
			  if (sign_Ad==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSDU[m_iAd]/ koeff_bazadop_U[m_iAd];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK22[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK22[2] = p;
			  memcpy(&My_buf_Out[k],&KK22[0],nK[21]);
			  k = k + nK[21];
			  //KK24 - valueBiasDop 
			  x = mas_d_range_bias_bazadop_U[m_iAd];
			  x = (x/1000.0)*m_iBd;
			  if (sign_Bd==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSDU[m_iAd]+BSDU[m_iAd])/ koeff_bazadop_U[m_iAd];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK24[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK24[2] = p;
			  memcpy(&My_buf_Out[k],&KK24[0],nK[23]);
			  k = k + nK[23];
			  // K25 - режим измерителя
			  KK25[1] = m_iDm;
			  memcpy(&My_buf_Out[k],&KK25[0],nK[24]);
			  k = k + nK[24];
			  //KK27
			  KK27[1]=m_iRd+bias_Id;
			  memcpy(&My_buf_Out[k],&KK27[0],nK[26]);
			  k = k + nK[26];
			  //KK28 - valueLimitDop - ограничение измерителя канала S
			  x = mas_d_range_meas_bazadop_I[m_iRd+bias_Id]+mas_d_range_meas_bazadop_I[m_iRd+bias_Id]/10.0;
			  x=(x/110)*m_iLd;
			  x = (x *KLDI[m_iRd]+BLDI[m_iRd])/ koeff_limit_bazadop_I[m_iRd+bias_Id];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK28[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK28[2] = p;
			  memcpy(&My_buf_Out[k],&KK28[0],nK[27]);
			  k = k + nK[27];
return(k);
}

int CLineDlg::CommandDopUssUsm(int k) 
{	//источник-Us , измеритель-Us
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;

			  //KK19 - режим источника доп.кан.
			  KK19[1] = m_iDs;
			  memcpy(&My_buf_Out[k],&KK19[0],nK[18]);
			  k = k + nK[18];
			  //KK20 - "Диапазон амплитуды ступени доп.кан."
			  KK20[1] = m_iAd;
			  memcpy(&My_buf_Out[k],&KK20[0],nK[19]);
			  k = k + nK[19];
			  // KK21 - число ступенек доп.кан.
			  KK21[1] = m_uSd;
			  memcpy(&My_buf_Out[k],&KK21[0],nK[20]);
			  k = k + nK[20];
			  //KK22 - valueAmplDop
			  x = mas_d_range_ampl_bazadop_U[m_iAd];
			  if (sign_Ad==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSDU[m_iAd]/ koeff_bazadop_U[m_iAd];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK22[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK22[2] = p;
			  memcpy(&My_buf_Out[k],&KK22[0],nK[21]);
			  k = k + nK[21];
			  //KK24 - valueBiasDop 
			  x = mas_d_range_bias_bazadop_U[m_iAd];
			  x = (x/1000.0)*m_iBd;
			  if (sign_Bd==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSDU[m_iAd]+BSDU[m_iAd])/ koeff_bazadop_U[m_iAd];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK24[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK24[2] = p;
			  memcpy(&My_buf_Out[k],&KK24[0],nK[23]);
			  k = k + nK[23];
			  // K25 - режим измерителя
			  KK25[1] = m_iDm;
			  memcpy(&My_buf_Out[k],&KK25[0],nK[24]);
			  k = k + nK[24];
			  //KK26 диапазон измерителя
			  KK26[1]=m_iAd;
			  memcpy(&My_buf_Out[k],&KK26[0],nK[25]);
			  k = k + nK[25];
			  //KK27 диапазон ограничения
			  KK27[1]=m_iRd+bias_Id;
			  memcpy(&My_buf_Out[k],&KK27[0],nK[26]);
			  k = k + nK[26];
			  //KK28 - valueLimitDop - ограничение измерителя канала S
			  x = mas_d_range_meas_bazadop_I[m_iRd+bias_Id]+mas_d_range_meas_bazadop_I[m_iRd+bias_Id]/10.0;
			  x=(x/110)*m_iLd;
			  x = (x *KLDI[m_iRd]+BLDI[m_iRd])/ koeff_limit_bazadop_I[m_iRd+bias_Id];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK28[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK28[2] = p;
			  memcpy(&My_buf_Out[k],&KK28[0],nK[27]);
			  k = k + nK[27];
return(k);
}

int CLineDlg::CommandDopIssUsm(int k) 
{	//источник-Is , измеритель-Us
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;

			  //KK19 - режим источника доп.кан.
			  KK19[1] = m_iDs;
			  memcpy(&My_buf_Out[k],&KK19[0],nK[18]);
			  k = k + nK[18];
			  //KK20 - "Диапазон амплитуды ступени доп.кан."
			  KK20[1] = m_iAd+bias_Id;
			  memcpy(&My_buf_Out[k],&KK20[0],nK[19]);
			  k = k + nK[19];
			  // KK21 - число ступенек доп.кан.
			  KK21[1] = m_uSd;
			  memcpy(&My_buf_Out[k],&KK21[0],nK[20]);
			  k = k + nK[20];
			  //KK22 - valueAmplDop
			  x = mas_d_range_ampl_bazadop_I[m_iAd+bias_Id];
			  if (sign_Ad==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSDI[m_iAd]/ koeff_bazadop_I[m_iAd+bias_Id];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK22[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK22[2] = p;
			  memcpy(&My_buf_Out[k],&KK22[0],nK[21]);
			  k = k + nK[21];
			  //KK24 - valueBiasDop 
			  x = mas_d_range_bias_bazadop_I[m_iAd+bias_Id];
			  x = (x/1000.0)*m_iBd;
			  if (sign_Bd==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSDI[m_iAd]+BSDI[m_iAd])/ koeff_bazadop_I[m_iAd+bias_Id];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK24[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK24[2] = p;
			  memcpy(&My_buf_Out[k],&KK24[0],nK[23]);
			  k = k + nK[23];
			  // K25 - режим измерителя
			  KK25[1] = m_iDm;
			  memcpy(&My_buf_Out[k],&KK25[0],nK[24]);
			  k = k + nK[24];
			  //KK27
			  KK27[1]=m_iRd;
			  memcpy(&My_buf_Out[k],&KK27[0],nK[26]);
			  k = k + nK[26];
			  //KK28 - valueLimitDop - ограничение измерителя канала S
			  x = mas_d_range_meas_bazadop_U[m_iRd]+mas_d_range_meas_bazadop_U[m_iRd]/10.0;
			  x=(x/110)*m_iLd;
			  x = (x *KLDU[m_iRd]+BLDU[m_iRd])/ koeff_limit_bazadop_U[m_iRd];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK28[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK28[2] = p;
			  memcpy(&My_buf_Out[k],&KK28[0],nK[27]);
			  k = k + nK[27];
return (k);
}

int CLineDlg::CommandDopIssIsm(int k) 
{	//источник-Is , измеритель-Is
	//k - количество байт в массиве команд My_buf_Out
	double x = 0.0;
	int p = 0;
	int rez = 0;

			  //KK19 - режим источника доп.кан.
			  KK19[1] = m_iDs;
			  memcpy(&My_buf_Out[k],&KK19[0],nK[18]);
			  k = k + nK[18];
			  //KK20 - "Диапазон амплитуды ступени доп.кан."
			  KK20[1] = m_iAd+bias_Id;
			  memcpy(&My_buf_Out[k],&KK20[0],nK[19]);
			  k = k + nK[19];
			  // KK21 - число ступенек доп.кан.
			  KK21[1] = m_uSd;
			  memcpy(&My_buf_Out[k],&KK21[0],nK[20]);
			  k = k + nK[20];
			  //KK22 - valueAmplDop
			  x = mas_d_range_ampl_bazadop_I[m_iAd+bias_Id];
			  if (sign_Ad==0) x = x * (-1.0);	//знак амплитуды  (инверсия)
			  x = x * KSDI[m_iAd]/ koeff_bazadop_I[m_iAd+bias_Id];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK22[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK22[2] = p;
			  memcpy(&My_buf_Out[k],&KK22[0],nK[21]);
			  k = k + nK[21];
			  //KK24 - valueBiasDop 
			  x = mas_d_range_bias_bazadop_I[m_iAd+bias_Id];
			  x = (x/1000.0)*m_iBd;
			  if (sign_Bd==0) x = x *(-1.0);	//знак смещения  (инверсия)
			  x = (x *KSDI[m_iAd]+BSDI[m_iAd])/ koeff_bazadop_I[m_iAd+bias_Id];
		      rez = (int)x;
			  p = rez & 0xff;
			  KK24[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK24[2] = p;
			  memcpy(&My_buf_Out[k],&KK24[0],nK[23]);
			  k = k + nK[23];
			  // K25 - режим измерителя
			  KK25[1] = m_iDm;
			  memcpy(&My_buf_Out[k],&KK25[0],nK[24]);
			  k = k + nK[24];
			  //KK26 - диапазон измерителя
			  KK26[1]=m_iAd+bias_Id;
			  memcpy(&My_buf_Out[k],&KK26[0],nK[25]);
			  k = k + nK[25];
			  //KK27 - диап. огр.
			  KK27[1]=m_iRd;
			  memcpy(&My_buf_Out[k],&KK27[0],nK[26]);
			  k = k + nK[26];
			  //KK28 - valueLimitDop - ограничение измерителя канала S
			  x = mas_d_range_meas_bazadop_U[m_iRd]+mas_d_range_meas_bazadop_U[m_iRd]/10.0;
			  x=(x/110)*m_iLd;
			  x = (x *KLDU[m_iRd]+BLDU[m_iRd])/ koeff_limit_bazadop_U[m_iRd];
			  rez = (int)x;
			  p = rez & 0xff;
			  KK28[1] = p;
			  p = rez & 0xff00;
			  p = p >> 8;
			  KK28[2] = p;
			  memcpy(&My_buf_Out[k],&KK28[0],nK[27]);
			  k = k + nK[27];
return (k);
}


void CLineDlg::OnUpdateAutoSetupNull(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!bPusk && !bAutoSetNull && bStop);
	
}

void CLineDlg::OnSelchangeComboKommutator() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
						
	InitTest();
	SetStringsSourceMeas();
	BuildCircuit();
	ShowGnezdoKommutator();
	ShowRejimChanel();
//	UpdateData(false);
//	SetStringsSourceMeas();
	m_sAxisX = m_sKolMeasU;
//	if (m_iQue1==COLLECTOR) m_sAxisX = m_sKolMeasU;
//	if (m_iQue1==BAZA)      m_sAxisX = m_sBazaMeas;
	m_sAxisY = m_sKolMeasI;
	UpdateData(false);
	b_InitAxis=true;
	int n = ControlParam();
	if(!n) DrawStatusRukoyatka();
}

void CLineDlg::ShowGnezdoKommutator() 
{
	if (m_iKommutator==0)
	{//внутренний
	  GetDlgItem(IDC_COMBO_GNEZDO_C)->EnableWindow(false);
	  GetDlgItem(IDC_COMBO_GNEZDO_B)->EnableWindow(false);
	  GetDlgItem(IDC_COMBO_GNEZDO_E)->EnableWindow(false);
	  GetDlgItem(IDC_STATIC_GNEZDO_S)->EnableWindow(false);

	  GetDlgItem(IDC_COMBOCHECKKOL)->EnableWindow(true);
	  GetDlgItem(IDC_COMBOCHECKBAZA)->EnableWindow(true);
	  GetDlgItem(IDC_COMBOCHECKDOP)->EnableWindow(true);
//	  GetDlgItem(IDC_TEXTCOMBOCHECKEM)->EnableWindow(true);
	}
	else
	{//внешний
	  GetDlgItem(IDC_COMBO_GNEZDO_C)->EnableWindow(true);
	  GetDlgItem(IDC_COMBO_GNEZDO_B)->EnableWindow(true);
	  GetDlgItem(IDC_COMBO_GNEZDO_E)->EnableWindow(true);
	  GetDlgItem(IDC_STATIC_GNEZDO_S)->EnableWindow(true);

	  GetDlgItem(IDC_COMBOCHECKKOL)->EnableWindow(false);
	  GetDlgItem(IDC_COMBOCHECKBAZA)->EnableWindow(false);
	  GetDlgItem(IDC_COMBOCHECKDOP)->EnableWindow(false);
//	  GetDlgItem(IDC_TEXTCOMBOCHECKEM)->EnableWindow(false);

	}
}

void CLineDlg::OnSelchangeComboGnezdoC() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SetStringsSourceMeas();
	if (m_iQue1==COLLECTOR) m_sAxisX = m_sKolMeasU;
	if (m_iQue1==BAZA)      m_sAxisX = m_sBazaMeas;
	m_sAxisY = m_sKolMeasI;
	ShowRejimChanel();
	UpdateData(false);
	int n = ControlParam();
	if(!n) DrawStatusRukoyatka();
}

void CLineDlg::OnSelchangeComboGnezdoB() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SetStringsSourceMeas();
	if (m_iQue1==COLLECTOR) m_sAxisX = m_sKolMeasU;
	if (m_iQue1==BAZA)      m_sAxisX = m_sBazaMeas;
	m_sAxisY = m_sKolMeasI;
	ShowRejimChanel();
	UpdateData(false);
	int n = ControlParam();
	if(!n) DrawStatusRukoyatka();
	
}

void CLineDlg::OnSelchangeComboGnezdoE() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SetStringsSourceMeas();
	if (m_iQue1==COLLECTOR) m_sAxisX = m_sKolMeasU;
	if (m_iQue1==BAZA)      m_sAxisX = m_sBazaMeas;
	m_sAxisY = m_sKolMeasI;
	ShowRejimChanel();
	UpdateData(false);
	int n = ControlParam();
	if(!n) DrawStatusRukoyatka();
	
}

void CLineDlg::OnTablCoef() 
{
	// TODO: Add your command handler code here
	int i;
	CKoefDlg kdlg;
	m_iNset = 0;
	kdlg.m_iNset = m_iNset;
	for(i=0;i<MAXRANGEIKOL;i++)
	{ kdlg.bmci[i] = BMCI[i];
	  kdlg.kmci[i] = KMCI[i];
	}
	for(i=0;i<13;i++)
	{ kdlg.bmcu[i] = BMCU[i];
	  kdlg.kmcu[i] = KMCU[i];
	}
	for(i=0;i<27;i++)
	{ kdlg.bmbi[i] = BMBI[i];
	  kdlg.kmbi[i] = KMBI[i];
	  kdlg.bsbi[i] = BSBI[i];
	  kdlg.ksbi[i] = KSBI[i];
	  kdlg.blbi[i] = BLBI[i];
	  kdlg.klbi[i] = KLBI[i];
	}
	for(i=0;i<17;i++)
	{ kdlg.bmdi[i] = BMDI[i];
	  kdlg.kmdi[i] = KMDI[i];
	  kdlg.bsdi[i] = BSDI[i];
	  kdlg.ksdi[i] = KSDI[i];
	  kdlg.bldi[i] = BLDI[i];
	  kdlg.kldi[i] = KLDI[i];
	}
	for(i=0;i<6;i++)
	{
	  kdlg.bmbu[i] = BMBU[i];
	  kdlg.kmbu[i] = KMBU[i];
	  kdlg.bsbu[i] = BSBU[i];
	  kdlg.ksbu[i] = KSBU[i];
	  kdlg.blbu[i] = BLBU[i];
	  kdlg.klbu[i] = KLBU[i];
	  kdlg.bmdu[i] = BMDU[i];
	  kdlg.kmdu[i] = KMDU[i];
	  kdlg.bsdu[i] = BSDU[i];
	  kdlg.ksdu[i] = KSDU[i];
	  kdlg.bldu[i] = BLDU[i];
	  kdlg.kldu[i] = KLDU[i];
	}
	UpdateData(FALSE);
	if (kdlg.DoModal()==IDOK)
	{
      UpdateData();
		for(i=0;i<MAXRANGEIKOL;i++)
		{ BMCI[i] = kdlg.bmci[i];
		  KMCI[i] = kdlg.kmci[i];
		}
		for(i=0;i<13;i++)
		{ BMCU[i] = kdlg.bmcu[i];
		  KMCU[i] = kdlg.kmcu[i];
		}
		for(i=0;i<27;i++)
		{ BMBI[i] = kdlg.bmbi[i];
		  KMBI[i] = kdlg.kmbi[i];
		  BSBI[i] = kdlg.bsbi[i];
		  KSBI[i] = kdlg.ksbi[i];
		  BLBI[i] = kdlg.blbi[i];
		  KLBI[i] = kdlg.klbi[i];
		}
		for(i=0;i<17;i++)
		{ BMDI[i] = kdlg.bmdi[i] ;
		  KMDI[i] = kdlg.kmdi[i];
		  BSDI[i] = kdlg.bsdi[i];
		  KSDI[i] = kdlg.ksdi[i];
		  BLDI[i] = kdlg.bldi[i];
		  KLDI[i] = kdlg.kldi[i];
		}
		for(i=0;i<6;i++)
		{
		  BMBU[i] = kdlg.bmbu[i];
		  KMBU[i] = kdlg.kmbu[i];
		  BSBU[i] = kdlg.bsbu[i];
		  KSBU[i] = kdlg.ksbu[i];
		  BLBU[i] = kdlg.blbu[i];
		  KLBU[i] = kdlg.klbu[i];
		  BMDU[i] = kdlg.bmdu[i];
		  KMDU[i] = kdlg.kmdu[i];
		  BSDU[i] = kdlg.bsdu[i];
		  KSDU[i] = kdlg.ksdu[i];
		  BLDU[i] = kdlg.bldu[i];
		  KLDU[i] = kdlg.kldu[i];
		}
	}
	else
	{
	}

	
}

void CLineDlg::OnButtonMeas() 
{
	// TODO: Add your control notification handler code here

// Кнопка "Измерение\Сброс" - ("Измерение однократное")
	CString str;
	CButton* pbut = (CButton*)GetDlgItem(IDC_BUTTON_MEAS);
	if (bAutoSetNullIc_10nA==true && !bZapusk)
	{
	  MessageBox(_T("Отсоедините перемычку \".027\", замыкающую гнезда Uc и E'!"),_T("  Внимание!"),MB_OK|MB_ICONEXCLAMATION);
	  bAutoSetNullIc_10nA=false;
	}
	else
	{

	  akt_button = 107;	
 if (!bPusk && !bKalibr && !bAutoSetNull && bStop && !bZapusk)
 {	
	if (m_bKrishka==1 && (bias_KolSourceU+m_iKolSourceU)>=2)
	{ //при запуске
		str = _T("ERROR 207: БЛОКИРОВКА НАРУШЕНА! ЗАКРОЙТЕ КРЫШКУ!"); 
		SetTextInfo(str);
	}
	else
	{ b_InitAxis=true;
	if (ControlParam()==0)
	{ ::WaitForSingleObject(hMutexStop,INFINITE);
	  if (bStop && !bZapusk)	//if "Стоп" и запуск "внутренний"
	  { 
	    CCmdTarget::BeginWaitCursor();	//запускаем измерение
	    bPusk = TRUE;			//запускаем измерение
	    bStop = FALSE;
	
		pbut -> SetWindowText(L"Сброс");

		Flag  = FALSE;
	    Flag_sboy  = FALSE;
		Flag_All = FALSE;
//		i_Error = 0;
//		m_nCountMas = 0;
		m_uCountTest  = 0;
		bMeas = MEAS_SINGLE;
		n_Test = CountnTest();//посчитать количество тестов
		TRACE(L"========= PUSK - 1 ========\n");
		Pusk();
	  } //end of (bStop)	
	  ::ReleaseMutex(hMutexStop);
	}
	}
// }
 }//end if (!bPusk)
 else
// if (bPusk || bKalibr || bAutoSetNull)
 {
	if (hEventEppIn) SetEvent(hEventStop);
	
	if (bPusk) 	str = _T("ИЗМЕРЕНИЕ ПРЕРВАНО!");//произощла рассинхронизация с Рic-ом
	if (bKalibr) 	str = _T("КАЛИБРОВКА ПРЕРВАНА!");//произощла рассинхронизация с Рic-ом
	if (bAutoSetNull) 	str = _T("КОРРЕКЦИЯ НУЛЯ ПРЕРВАНА!");//произощла рассинхронизация с Рic-ом
	SetTextInfo(str);
	Flag_sboy = true;
	ready = 0;//9.06.15
	Stop();
//	bPusk = false;
//	bStop = true;
//	ready = 0;//9.06.15
	
	pbut -> SetWindowText(L"Измерение");

 }
	}
}

void CLineDlg::OnModifyIpAddress() 
{
	// TODO: Add your command handler code here
	CIpInfo dlg;
	UpdateData(FALSE);
	if (dlg.DoModal()==IDOK)
	{
	UpdateData();
	}
	
}

void CLineDlg::OnPointSizeSmall() 
{
	// TODO: Add your command handler code here
	size_point=0;
}

void CLineDlg::OnPointSizeBig() 
{
	// TODO: Add your command handler code here
	size_point=1;
	
}

void CLineDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINSTEPBAZA);
	CSpinButtonCtrl* pSpin1 = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINSTEPBAZA);
	UpdateData();
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SPINSTEPBAZA: 	m_uStepBaza = pSpin -> GetPos();
							break;
	case IDC_SPINSTEPDOP: 	m_uStepDop = pSpin1 -> GetPos();
							break;

	}
	ControlParam();
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CLineDlg::OnCancel()
{

//	return;
	if (bPusk || bKalibr || bAutoSetNull) return;
if (MessageBox(_T("Вы уверены, что хотите завершить работу?"),NULL,MB_YESNO|MB_ICONQUESTION)==IDNO)
{
	return;
}
	if (m_bFileOpen) FileClose();

	ShowTaskBar(true);


	CDialog::OnCancel();

}


void CLineDlg::OnHibernate() 
{
	// TODO: Add your message handler code here
	
}

void CLineDlg::OnCheckbazarejhigh() 
{
	// TODO: Add your control notification handler code here
if (m_iQue1==BAZA) m_bCheckBazaRejHigh=true;
else UpdateData();
	UpdateData(false);
	ControlParam();
	
}

void CLineDlg::OnRadioSynpositive() 
{
	// TODO: Add your control notification handler code here
	int irazv_old = m_irazv;
	UpdateData(TRUE);
//	  if (n_IPPP==1 && n_Switch==1) {m_irazv = IMP_POS;}// m_itipimp=TIP_IMP;}//---!
//	  if (n_IPPP==1 && n_Switch==2) {m_irazv = IMP_NEG;}// m_itipimp=TIP_IMP;}//---!
	  if (n_Switch==0 && m_iQue1==BAZA) {m_irazv = irazv_old;UpdateData(FALSE);}// m_itipimp=TIP_IMP;}//---!
	  else
	  {
	  if (n_IPPP==1 && n_Switch==2) {m_irazv = SYN_NEG;}// m_itipimp=TIP_IMP;}//---!
	UpdateData(FALSE);
//    i_tipimp = m_itipimp;
	m_nMas=1;
	InitIconImp();
///	DefinitionDiapazonKol();
	ChangeDiapazonKol();	//InitDiapazonKol();
	ChangeDiapazonKolMeasU();
//	if (irazv_old!=IMP_NEG) ChangeDiapazonKol();//m_iKolI = -1;
	ShowRejimChanel();
	UpdateData(FALSE);
///    SetDiapazonKol();
//	bGenerationStup = true;
	ShowButtonLoopingCompensation();
	UpdateData(FALSE);
	  }
	if(iX==COLLECTOR && iY==COLLECTOR) ResetDisplay(LEFT);
	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
	
}

void CLineDlg::OnRadioSynnegative() 
{
	// TODO: Add your control notification handler code here
	int irazv_old = m_irazv;
	UpdateData(TRUE);
//	if (n_IPPP==1 && n_Switch==1) {m_irazv = IMP_POS;}// m_itipimp=TIP_IMP;}//---!
//	  if (n_IPPP==1 && n_Switch==2) {m_irazv = IMP_NEG;}// m_itipimp=TIP_IMP;}//---!
	  if (n_Switch==0 && m_iQue1==BAZA) {m_irazv = irazv_old;UpdateData(FALSE);}// m_itipimp=TIP_IMP;}//---!
else
{
	if (n_IPPP==1 && n_Switch==1) {m_irazv = SYN_POS;}// m_itipimp=TIP_IMP;}//---!
	UpdateData(FALSE);
//    i_tipimp = m_itipimp;
	m_nMas=1;
	InitIconImp();
///	DefinitionDiapazonKol();
	ChangeDiapazonKol();	//	InitDiapazonKol();
	ChangeDiapazonKolMeasU();
//	if (irazv_old!=IMP_POS) ChangeDiapazonKol();//m_iKolI = -1;
	ShowRejimChanel();
	UpdateData(FALSE);
///    SetDiapazonKol();
//	bGenerationStup = true;
	ShowButtonLoopingCompensation();
	UpdateData(FALSE);
}
	if(iX==COLLECTOR && iY==COLLECTOR) ResetDisplay(RIGHT);
	int er = ControlParam();//^^^^^^^^^^^^^^^^^^^^
	
}

void CLineDlg::OnCheckBazaVkl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowRejimChanel();
	UpdateData(FALSE);
	
}

void CLineDlg::OnCheckFamily() 
{
	// TODO: Add your control notification handler code here
	m_bFamilyVkl =! m_bFamilyVkl;
	m_bFamilyVklOld = m_bFamilyVkl;
	if (m_bFamilyVkl) m_iFamilyVkl=1;
				else  m_iFamilyVkl=0;
	ShowRejimChanel();
	UpdateData(false);
	ControlParam();
	
}

void CLineDlg::OnSelendokComboQue1() 
{
	// TODO: Add your control notification handler code here
	CString str=_T("");

    UpdateData();
	switch(m_iQue1)
	{ case COLLECTOR:
/*						if (n_IPPP==1 && n_Switch==2 && m_irazv != IMP_NEG) m_irazv = IMP_NEG;
						if (n_IPPP==1 && n_Switch==1 && m_irazv != IMP_POS) m_irazv = IMP_POS;
						if (n_Switch==0 && m_irazv != DC_POS && 
							m_irazv != IMP_POS && m_irazv != IMP_NEG)  m_irazv = DC_POS;
*/
 ///						m_iKommutator = 0;
						m_bFamilyVkl=m_bFamilyVklOld;
						m_bLooping = m_bLoopingOld;
					    m_sAxisX = m_sKolMeasU;
						m_sAxisY = m_sKolMeasI;
						m_bCheckBazaRejHigh = false;
//						uNpointsLupa =uNpointsLupaOld;
//						m_iLimitKolI = 0;
						UpdateData(false);
						str=_T("1-ая очередь - КАНАЛ С");
						break;

      case BAZA:/*		if (n_IPPP==1 && n_Switch==2 && m_irazv != IMP_NEG) m_irazv = IMP_NEG;
						if (n_IPPP==1 && n_Switch==1 && m_irazv != IMP_POS) m_irazv = IMP_POS;
						if (n_Switch==0 && m_irazv != DC_POS && 
							m_irazv != IMP_POS && m_irazv != IMP_NEG)  m_irazv = DC_POS;
*/
  ///						m_iKommutator = 0;
//					    m_bFamilyVklOld	= m_bFamilyVkl;
//						m_bFamilyVkl=false;
//						m_bLoopingOld = m_bLooping;
///						m_bLooping = false;
	m_iLimitKolU =1;		//текущее значение слайдера ограничения U коллектора (в %)
	m_iLimitKolI = m_iLimitKolU;
	//ограничение по U 
	//****************
	CSliderCtrl* pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLU);
	pSl -> SetPos(m_iLimitKolU);
	pSl = (CSliderCtrl*)GetDlgItem(IDC_SLIDERKOLI);
	pSl -> SetPos(m_iLimitKolI);
	m_sLimitKolU.Format(_T(" %3d"),pSl->GetPos());
	m_sLimitKolU+=_T(" %");
						m_bCheckBazaRejHigh = true;
						m_sAxisX = m_sBazaMeas;
						m_sAxisY = m_sKolMeasI;
//						uNpointsLupaOld = uNpointsLupa;
						bGenerationStup = false;
						UpdateData(false);
						str=_T("1-ая очередь - КАНАЛ B");
						break;
	
	}
	m_sPar=_T("");
	m_sArg=_T("");
	m_sUr1=_T("");
	m_sUr2=_T("");
	m_sMax=_T("");
	m_sMin=_T("");
	UpdateData(false);

	InitStatusKol();
	ShowGnezdoKommutator();
    int nTab = 2;			//отобразить закладку "Канал С"
	m_TabCtrlParam.SetCurSel(nTab);
	InitPage(nTab);
	InitIconImp();
	ChangeDiapazonKol();	
	ChangeDiapazonKolMeasU();
	ShowRejimChanel();
	UpdateData(FALSE);
	ShowButtonLoopingCompensation();
	UpdateData(false);
	b_InitAxis=true;
	int n = ControlParam();
	if(!n) DrawStatusRukoyatka();
	SetTextInfo(str);
	
	
}

void CLineDlg::OnKillfocusEditmin() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ControlParam();
	UpdateData(FALSE);
	
}

void CLineDlg::OnKillfocusEditmax() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ControlParam();
	UpdateData(FALSE);
	
}

void CLineDlg::OnDifferenceVax() 
{
	// TODO: Add your command handler code here
	CString str;
			str = _T("РАЗНОСТЬ ВАХ\nИзмерение ВАХ дополнительного элемента");
			SetTextInfo(str);
			
			PuskDifferenceVax();	
}

void CLineDlg::PuskDifferenceVax()
{ 
	CString str;
	CButton* pbut = (CButton*)GetDlgItem(IDC_BUTTON_MEAS);
	
	CDiffDlg dlg;
 
	dlg.m_iDifference = m_iDifference;
		UpdateData(FALSE);
		if (dlg.DoModal()==IDOK)
		{
	      UpdateData();
	      m_iDifference = dlg.m_iDifference;
		  switch(m_iDifference)
		  {
			case 0:  bStatusDiff = false;	// вычитание ВАХ-ов не производить
					 bMeasDiff = false;		//измерение ВАХ доп. эл-та выключено
					 str = _T("ИЗМЕРЕНИЕ РАЗНОСТИ ВАХ ВЫКЛЮЧЕНО!\n");
					 SetTextInfo(str);
					 break;
			case 1: case 2:
				     bStatusDiff = false;	// вычитание ВАХ-ов не производить
					 bMeasDiff = true;		//измерение ВАХ доп. эл-та включить
					
					 if (!bPusk && !bKalibr && !bAutoSetNull)
					 {	
						if (m_bKrishka==1 && (bias_KolSourceU+m_iKolSourceU)>=2)
						{ //при запуске
							str = _T("ERROR 207: БЛОКИРОВКА НАРУШЕНА! ЗАКРОЙТЕ КРЫШКУ!"); 
							SetTextInfo(str);
						}
						else
						{ 	b_InitAxis=true;
							if (ControlParam()==0)
							{ ::WaitForSingleObject(hMutexStop,INFINITE);
							  if (bStop && !bZapusk)	//if "Стоп" и запуск "внутренний"
							  { 
								//CCmdTarget::BeginWaitCursor();	//запускаем измерение
							    bPusk = TRUE;			//запускаем измерение
							    bStop = FALSE;
	
								pbut -> SetWindowText(L"Сброс");

								Flag  = FALSE;
							    Flag_sboy  = FALSE;
								Flag_All = FALSE;
								m_uCountTest  = 0;
								bMeas = MEAS_SINGLE;

								bAutomat  = HAND;
								n_Test = 1;
								bSound = false;
								bZapusk = false;

								TRACE(L"========= PUSK - Diff ========\n");
								Pusk();
							  } //end of (bStop)	
							  ::ReleaseMutex(hMutexStop);
							}//end of !ControlParam()
						}//end of else
					}//end if (!bPusk)

					 break;

		  }//end of  switch(m_iDifference)
		}//end of if (dlg.DoModal()==IDOK)
}
void CLineDlg::ResetDiff()
{	int i,j,b;
 if (bStatusDiff)
 {	
 BOOL flag = false;
	m_Chart.DisableRefresh(true); 
	if (nCheckParam)
	{ flag = true;
		nCheckParam = false;	//!;
	  MarkerNoakt();
	}
   if (bStatusRef)
   {	
	b = m_Chart.GetSeriesCount();
     for(j=0;j<b;j++)
     m_Chart.DeleteSerie(n_Step);
     m_Chart.DeleteFamily(REF);
   }
   
   b = m_Chart.GetSeriesCount();
   for(j=0;j<b;j++)
   m_Chart.DeleteSerie(n_Step);
   m_Chart.DeleteFamily(DIFF);
 }
   
    bStatusDiff = false;	// вычитание ВАХ-ов не производить
    bMeasDiff = false;		//измерение ВАХ доп. эл-та выключено
	m_iDifference = 0;		//разность ВАХ выключена
	for (UINT m=0;m<NMAS;m++)
	for(i=0;i<NREZ;i++)
	{ diffX[m][i]=0.0;
	  diffY[m][i]=0.0;
	}
	m_Chart.DisableRefresh(false); 

}
void CLineDlg::CalculateDifference()
{	
	double delta;
	double Ur_y,Ur_y1,Ur_y2;
	double Ur_x,Ur_x1,Ur_x2;
	  // из уравнения прямой по 2-м точкам (x1,y1) (x2,y2): 
	  // (y-y1)/(y2-y1)=(x-x1)/(x2-x1) получаем: 
	  // y = (y2-y1)*(x-x1)/(x2-x1)+y1;
	  // x = ((y-y1)*(x2-x1))/(y2-y1)+x1;
	UINT i=0;
	UINT j=0;
	UINT m=0;
	UINT k=0;
	UINT index_j = j;
	UINT index_m = m;
	UINT index_k = k;
	UINT n_step=n_Step;
	UINT n_point=NPOINT;
	if (m_bLooping) n_point = NPOINT100;
	if (m_irazv==IMP_POS || m_irazv==IMP_NEG) n_point = 1;
	if (m_iQue1==COLLECTOR)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}
	if (m_iQue1==BAZA)
	{
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_SINGLE) n_point = m_uStepBaza*10;//uNpointsLupa;//n_Result;
	if ((m_irazv==DC_POS || m_irazv==DC_NEG) && bMeas == MEAS_MULTIPLE) n_point = n_Result;
	}


  if (m_iDifference==1)
  {//последовательное включение (внешний резистор)(GetXbyY)
	 //i-ая точка Xi: Uci пп = Uci общ. - Uci доп.,
	 //           Yi: Ici общ.
	for (j=0;j<n_Step;j++)
	  for (m=0;m<n_Mas;m++)
	    for (i=0;i<n_point;i++)
		{	Ur_y =pM2[m][i+NPOINT*j];
			delta = fabs(Ur_y - diffY[m][0+NPOINT*j]);
			for (k=0;k<n_point;k++)
			if (fabs( Ur_y - diffY[m][k+NPOINT*j])<=delta)
			{ delta = fabs(Ur_y - diffY[m][k+NPOINT*j]);
			  index_j = j;
			  index_m = m;
			  index_k = k;
			}
		    else continue;
			Ur_x = 0.0;
//	        if (index_k<=n_point)
//			{  index_k=index_k+1;
//			   index_k=n_point-1;
			  //Ur_y  = diffY[index_m][index_k+NPOINT*index_j];
			  if (index_k==0)
			  { Ur_y1 = diffY[index_m][(index_k)+NPOINT*index_j];
				Ur_x1 = diffX[index_m][(index_k)+NPOINT*index_j];
			  }
			  else 
			  { Ur_y1 = diffY[index_m][(index_k-1)+NPOINT*index_j];
			    Ur_x1 = diffX[index_m][(index_k-1)+NPOINT*index_j];
			  }
			  if (index_k==n_point) 
			  { Ur_y2 = diffY[index_m][(index_k)+NPOINT*index_j];
				Ur_x2 = diffX[index_m][(index_k)+NPOINT*index_j];
			  }
			  else 
			  { Ur_y2 = diffY[index_m][(index_k+1)+NPOINT*index_j];
			    Ur_x2 = diffX[index_m][(index_k+1)+NPOINT*index_j];
			  }

			  // x = ((y-y1)*(x2-x1))/(y2-y1)+x1;
			  Ur_x  = ((Ur_y-Ur_y1)*(Ur_x2-Ur_x1))/(Ur_y2-Ur_y1)+Ur_x1;
//			}
			
//			Ur_x = diffX[index_m][index_k+NPOINT*index_j];
			double temp = pM1[m][i+NPOINT*j] - Ur_x;
//		    pM1[m][i+NPOINT*j] = pM1[m][i+NPOINT*j] - Ur_x;
		    pM1[m][i+NPOINT*j] = temp;
		}

  }//end of m_iDifference==1

  if (m_iDifference==2)
  {//параллельное включение (изоляция) (GetYbyX)
	 //i-ая точка Xi: Uci общ., 
	 //           Yi: Ici пп = Ici общ. - Ici доп.
	for (j=0;j<n_Step;j++)
	  for (m=0;m<n_Mas;m++)
	    for (i=0;i<n_point;i++)
		{	Ur_x =pM1[m][i+NPOINT*j];
			delta = fabs(diffX[m][0+NPOINT*j]- Ur_x);
			for (k=0;k<n_point;k++)
			if (fabs(diffX[m][k+NPOINT*j]- Ur_x)<=delta)
			{ delta=fabs(diffX[m][k+NPOINT*j] - Ur_x);
			  index_j = j;
			  index_m = m;
			  index_k = k;
			}
		    else continue;
			Ur_y = 0.0;
//	        if (index_k<=(n_point-1))
//			{ 
			  if (index_k==0)
			  {
			  Ur_x1 = diffX[index_m][(index_k)+NPOINT*index_j];
			  Ur_y1 = diffY[index_m][(index_k)+NPOINT*index_j];
			  }
			  else
			  {
			  Ur_x1 = diffX[index_m][(index_k-1)+NPOINT*index_j];
			  Ur_y1 = diffY[index_m][(index_k-1)+NPOINT*index_j];
			  }
			  if (index_k==n_point) 
			  {
			  Ur_x2 = diffX[index_m][(index_k)+NPOINT*index_j];
			  Ur_y2 = diffY[index_m][(index_k)+NPOINT*index_j];
			  }
			  else
			  {
			  Ur_x2 = diffX[index_m][(index_k+1)+NPOINT*index_j];
			  Ur_y2 = diffY[index_m][(index_k+1)+NPOINT*index_j];
			  }
			  // y = (y2-y1)*(x-x1)/(x2-x1)+y1;
			  Ur_y  = ((Ur_y2-Ur_y1)*(Ur_x-Ur_x1))/(Ur_x2-Ur_x1)+Ur_y1;
//			}
			
		    pM2[m][i+NPOINT*j] = pM2[m][i+NPOINT*j] - Ur_y;
		}

  }//end of m_iDifference==2
}

void CLineDlg::OnKillfocusEditstepbaza() 
{
	// TODO: Add your control notification handler code here
	if(m_iQue1==BAZA)
	{
	b_InitAxis=true;
	ControlParam();
	}
}

void CLineDlg::ShowTaskBar(BOOL flagShow) 
{
	CRect rectWorkArea=CRect(0,0,0,0);
	CRect rectTaskBar=CRect(0,0,0,0);
	CWnd* hWnd=FindWindow(_T("HHTaskBar"),_T(""));

	if (flagShow)
	{
		//Показываем панель задач
		SystemParametersInfo(SPI_GETWORKAREA,0,(LPVOID)&rectWorkArea,0);
		if (hWnd)
		{
			hWnd->GetWindowRect(&rectTaskBar);
			rectWorkArea.bottom = RectWnd.bottom - rectTaskBar.Height();
			SystemParametersInfo(SPI_SETWORKAREA,0,(LPVOID)&rectWorkArea,0);
			hWnd->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		//Скрываем панель задач
		SystemParametersInfo(SPI_GETWORKAREA,0,(LPVOID)&rectWorkArea,0);
		if (hWnd)
		{
			hWnd->GetWindowRect(&rectTaskBar);
			rectWorkArea.bottom = RectWnd.bottom;
			SystemParametersInfo(SPI_SETWORKAREA,0,(LPVOID)&rectWorkArea,0);
			hWnd->ShowWindow(SW_HIDE);
		}
	}
}
