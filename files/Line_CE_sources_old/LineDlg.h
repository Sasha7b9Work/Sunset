// LineDlg.h : header file
//

#if !defined(AFX_LINEDLG_H__074D2EB6_E91A_42F1_806D_0D35CE8EAD6F__INCLUDED_)
#define AFX_LINEDLG_H__074D2EB6_E91A_42F1_806D_0D35CE8EAD6F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Mod.h"
#include "TreeViewDlg.h"
#include "TreeAddDlg.h"
#include "ChartCtrl.h"
#include "GradBtn.h"
#include "SaveOtchDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CLineDlg dialog

class CLineDlg : public CDialog
{
// Construction
public:
	CLineDlg(CWnd* pParent = NULL);	// standard constructor

	CModList      m_modList;
	POSITION	  m_modposition;
public:
	CModList* GetList() { return &m_modList;}
	POSITION* GetPosition() { return &m_modposition;}

  	BOOL		  m_bCapt;
	BOOL		  m_bModifiedFlag;	//признак: 1 - файл документа был модификацирован, 0 - нет
	BOOL		  m_bZaschitaParol;	//защита паролем активного файла *.mod: 0-нет, 1-да
	BOOL		  m_bFileOpen;		//признак: 1 - файл документа уже загружен, 0 - нет	
	BOOL	      m_bCopy;			//признак: 1 - буфер содержит копируемую информацию, 0 - нет
	BOOL		  m_bModifiedMemory;//признак: 1 - была модификацирована память, 0 - нет
	BOOL		  m_bKrishka;		//0- крышка закрыта, 1 - открыта

	CModList*     m_pList;
	POSITION*     m_position;

	CMod*	      pModSelected;	
	CMod*	      pModBuf;

	CTreeAddDlg*  pDlgTree;
//	CTreeViewDlg* m_pTreeDlg;

	CFont		  m_fontMin;   
 	CFont		  m_fontMax;   

//	CString		  strDateCurrent;
    CString		  strTimeCurrent;
	CString		  strNameColumn;	//строка наименований колонок измеренных данных
	CString		  m_sNameOtch;
	CString		  m_sFIO;
	CString		  m_sPost;
	CString		  m_sProduct;

	int			  index;
	int			  m_ProgPos;     //состояние индикатора
//
//	int			  m_irejimp;
	int			  m_itipimp;	 //тип импульса TIP_SYN или TIP_IMP
//резерв - заменить
//	int			  m_iRangBiasBaza;//m_iQue1
	int			  m_iFamilyVkl;//m_iRangBiasDop;
//конец резерв - заменить

    CBitmapButton m_LEFT;
    CBitmapButton m_RIGHT;
	CBitmapButton m_RESET;

	//палитра
	//=======
	WORD		  m_nColorPalette;
	LPLOGPALETTE  logpalette;
	CPalette      m_palette;
	RGBQUAD       RGBQuad;
	BYTE*         m_ColorTable;
	
	CBrush        brFon;
	CBrush	      brFonCircuit;
	CBrush        m_brushBlack;
	CBrush        m_brushGray;
	CBrush        m_brushGrayLight;
	CBrush	      m_brushWhite;
	CBrush		  m_brushBlue;
	CBrush		  m_brushYellowLight;
	CBrush		  m_brushGreenLight;

	CSaveOtchDlg* m_pSaveOtchDlg;
	//график
	//------
	CRect         m_RectWndGraf;
//	CChartSerie*  pGraph;
//	CChartSerieFamily* pFamily;

	CChartSerie*  pGraphRef;
//	CChartSerieFamily* pFamilyRef;

/*	CDC           MemDC;
	CBitmap       m_bmpWndGraf;
	CRect         m_RectGraf;
	CRect         m_RectStr_X;		//оцифровка оси Х
	CRect         m_RectStr_Y;		//оцифровка оси Y
	CRect         m_RectRazmer_X;	//оцифровка оси Х: характеристика, единицы измерения и порядок
	CRect         m_RectRazmer_Y;	//оцифровка оси Y: характеристика, единицы измерения и порядок
*/
	UINT          Height, Width;

	CPen		  m_penSetka_DASH,m_penSetka_SOLID,m_penFon,m_penTangent,m_penCurve,m_penRef;
	COLORREF	  m_colorBMP,m_colorFon,m_colorSetka,m_colorText,m_colorTangent,m_colorCurve,m_colorRef;

	int			  m_iX;			//развёртка по оси X : 0-"1" ед.,1-"2" ед.,2-"5" ед.
	int			  m_iY;			//развёртка по оси Y : 0-"1" ед.,1-"2" ед.,2-"5" ед.
	UINT		  nStepQ1;		//число шагов 1-ой очереди
	UINT          nStepQ2;		//число шагов 2-ой очереди
	int 		  iX;			//канал, с которого берётся результат по оси Х 
								//(он же - источник 1-ой очереди): 0-коллектор, 1-база, 2-доп.канал
								//if iX = 0 - массив результатов измерения напряжения на коллекторе,
								//if 1 или 2 - массив X_axis
	int 		  iY;			//канал, с которого берётся результат измерения по оси Y (0-коллектор, 1-база, 2-доп.канал) 
	double		  X_min;
	double		  X_max;
	double		  Y_min;
	double		  Y_max;
	int			  rej_X;		//характеристика, отображаемая по оси X: 0 - U, 1 - I
	int			  rej_Y;		//характеристика, отображаемая по оси Y: 0 - U, 1 - I
	int			  rej_Y_Meas;	//характеристика, отображаемая по оси Y: 0 - U, 1 - I при измерении
	int			  expo_X;
	int			  expo_Y;
	int			  po_X;
	int			  po_Y;
	double        mashtab_X;
	double        mashtab_Y;
	double		  Ur1;
	double		  Ur2;
	double		  Porog_min;	//пороги
	double		  Porog_max;
	CString		  strRazmer;
	CString		  strRazmer_X;
	BOOL		  m_bminusX;
	BOOL		  m_bminusY;
	BOOL		  m_bBeta;
	BOOL		  m_bBetai;
	BOOL		  m_bErli; 
	BOOL		  m_bR;	
	BOOL		  m_bS;
	BOOL		  m_bKolX;		//признак:	0 - по оси Х - Uc, 1 - Ic
	BOOL		  m_bKolY;		//признак:	0 - по оси Y - Ic, 1 - Uc
	BOOL		  m_bBaza;		//признак: 0 - меряем то, что генерируем, 
								//		   1 - измеряем то, что ограничиваем
	BOOL		  m_bDop;		//признак: 1 - изменить режим измерения в доп.канале
	UINT		  m_uFunc;		//признак: 0 - найти значение функции по значению аргумента
								//		   1 - найти значение аргумента по значению функции
								//		   2 - рассчитать рассчётный массив (pM5[]) и найти параметр по значению аргумента
								//		   3 - рассчитать рассчётный массив (pM5[]) и найти параметр по значению функции
	int			  m_iStyleCurve;	//0 - линия, 1 - точки, 2 - линия с точками

	int			  m_iStyleCurveRef;
	int			  m_iResultRef;
	int				m_iStepRef;
	int			  m_iPointRef;
	int			  m_iMasRef;
	int			  m_iNset;		//№ программы автостановки нуля
	int			  m_iNproxod;	//№ прохода автостановки нуля
	int			  m_iNsetK;		//№ программы калибровки
	int			  m_iRange;		//№ калибруемого диапазона

	int           m_iDifference;//режим измерения разности ВАХ

	//информационное окно
	//-------------------
/*	CDC			  InfoDC;
	CBitmap		  m_bmpWndInfo;
	UINT          Height_Info, Width_Info;
	CBrush        brFonInfo;
*/
    CFont		  fontMessage;
    CFont		  fontParam;
    CFont		  fontStr;
	//кнопки
	//-------
//	BOOL		  m_bButInversion;	//0 - выкл., 1 - вкл.
//	BOOL		  m_bButInversionY;	//0 - выкл., 1 - вкл.
	int			  m_iMarker;		//номер текущего маркера: 1 - маркер1, 2 - маркер2, 0 - недоступен
	BOOL		  m_bTangent;		//1 - рисовать секущую, 0 - нет

	//рукоятка
	//========
	BOOL		  m_bMigSignal;
	UINT          m_iPosTek;
	UINT          m_iPosTekOld;
    CFont		  fontRukoyatka;
	CString		  strValueRukoyatka;
/*	CDC			  InfoRukoyatkaDC;
	CBitmap		  m_bmpWndInfoRukoyatka;
	UINT          Height_InfoRukoyatka, Width_InfoRukoyatka;
	CBrush        brFonInfoRukoyatka;
*/
	//измерение
	//=========
	UINT		  m_nCountMas;	// счётчик числа принятых массивов результатов
	UINT		  m_nMas;		// число массивов, которое необходимо принять (зависит от вида развёртки)
//	UINT		  m_uBitQ18;		//состояние бита Q14 ( 1- импульсный, 0- нет)
	//коллектор
	double		  valueLimitKolU;	// ограничение источника коллектора (U)
    double		  m_dLimitKolU;		// максимальное значение диапазона изменения ограничения по U 
	double		  valueLimitKolI;	// ограничение измерителя коллектора (I)
    double		  m_dLimitKolI;		// максимальное значение диапазона изменения ограничения по I 
	int			  bias_KolMeasI;	// смещение индекса в массиве диапазонов измерителя I(коллектор)
	int			  bias_KolMeasU;	// смещение индекса в массиве диапазонов измерителя U(коллектор)
	int			  bias_KolSourceU;	// смещение индекса в массиве диапазонов источника U(коллектор)//измерителя U(коллектор)
	BOOL		  m_bSignal;		// признак: 1 - поджечь сигнал "Высокое напряжение", 0 - погасить
	BOOL		  m_bSignalMemoryFull;// признак: 1 - поджечь сигнал "Память переполнена", 0 - погасить
	//база
	double		  valueAmplBaza;	// амплитуда ступени базы
	double		  valueBiasBaza;	// смещение генератора ступенек базы
    double		  m_dBiasBaza;		// максимальное значение диапазона изменения смещения
	double		  valueLimitBaza;	// ограничение измерителя базы
    double		  m_dLimitBaza;		// максимальное значение диапазона изменения ограничения 
	int			  m_iRangMeasBaza;	// индекс диапазона измерителя в базе
	int		      sign_ampl_baza;	// знак амплитуды ступеньки в базе (0 - "+", 1 - "-")
	int			  sign_bias_baza;	// знак смещения в базе (0 - "+", 1 - "-")
//	BOOL		  m_bBazaVkl;		// признак: импульс в базе: 0 - sin (10ms), 1 - (0.2ms,0.5ms,2ms)
	CString		  m_sBazaMeas;
		//доп.канал
	double		  valueAmplDop;		// амплитуда ступени доп.канала
	double		  valueBiasDop; 	// смещение генератора ступенек доп.канала 
    double		  m_dBiasDop;		// максимальное значение диапазона изменения смещения
	double		  valueLimitDop;	// ограничение измерителя доп.канала 
    double		  m_dLimitDop;		// максимальное значение диапазона изменения ограничения   
	int			  m_iRangMeasDop;	// индекс диапазона измерителя в доп.канале
	int			  bias_Dop_I;		// смещение индекса в массиве диапазонов измерителя I(доп.канал)
	int		      sign_ampl_dop;	// знак амплитуды ступеньки в доп.канале (0 - "+", 1 - "-")
	int			  sign_bias_dop;	// знак смещения в доп.канале (0 - "+", 1 - "-")
	CString		  m_sDopMeas;

	// импульс
	int		m_idlitimp;

	//параметры текущего теста
	//-------------------------
	CString       strDateMeas;		// Дата проведения измерения
	CString       strTimeMeas;		// Время проведения измерения
	CString		  m_EL;				// Элемент (наименование)
	CString       m_PRIM;			// Комментарии (краткое описание теста)
//	int		      m_nelement;		// Вид тестируемого элемента:-1-не задано,0-D,1-R,
									//							  2-NPN,3-PNP,4-NMOS,5-PMOS,
									//							  6-NPN4,7-PNP4,8-NMOS4,9-PMOS4
	int			  m_nelement;
	int			  m_iCircuitEm;
	int			  m_iLimitKolU;		//текущее значение слайдера ограничения U коллектора (в %)
	int			  m_iLimitKolI;		//текущее значение слайдера ограничения I коллектора
	int			  m_iBiasBaza;		//текущее значение слайдера смещения базы
	int			  m_iLimitBaza;		//текущее значение слайдера ограничения базы
	int			  m_iBiasDop;		//текущее значение слайдера смещения доп.канала
	int			  m_iLimitDop;		//текущее значение слайдера ограничения доп.канала
	int			  m_iBazaMeas;
	int		      m_iDopMeas;


    CString		  m_sEL_old;		//элемент
    CString		  m_sPart_old;		//№ партии
    CString		  m_sPlast_old;		//№ пластины
    CString		  m_sPart;			//№ партии
    CString		  m_sPlast;			//№ пластины
	CString		  m_sPoint;			//№ точки
	CString		  m_sPoint_old;		//№ точки

	UINT		m_uN;

	UINT		m_uCountTest;	    //счётчик числа тестов
	UINT		m_uTp;				//номер линии в семействе кривых, на которой осущ. поиск параметра

	double  Beta_max;
	double  Betai_max;
	double  Ib_Beta_max;
	double  Ib_Betai_max;
	double  r1;
	double  r2;
	double  Par;
	BOOL    Flag;		//флаг: 0 - параметр входит в допустимый диапазон
						//      1 - параметр превышает допустимый порог
	BOOL    Flag_All;	//флаг: 0 - элемент "годен" при выполнении всех тестов в реж. "Автомат"
						//      1 - элемент "не годен" при выполнении всех тестов в реж. "Автомат"
//	BOOL	IsDiskA;		//FALSE-отсутствует флэш-карта с архивами, TRUE-присутствует
	BOOL	m_bButInversion;
	BOOL	m_bButInversionY;
	BOOL	m_bLoopingOld;
	BOOL	m_bFamilyVklOld;
	int		free_space;


	//КОРРЕКЦИЯ СМЕЩЕНИЯ нуля и калибровка
	//-------------------------------
	int	m_ir;
	int m_iUc;		//Диапазон измерителя Uc
	int m_iIc;
	int m_iUcs;		//диапазон источника Uc
	int m_iLstart;	//ограничение Uc min
	int m_iLstop;	//ограничение Uc max
	int m_iBs;
	int m_iBm;
	int m_iAb;
	UINT m_uSb;
	int m_iRb;
	int m_iDs;
	int m_iDm;
	int m_iAd;
	UINT m_uSd;
	int m_iRd;
	int m_iBb;	//смещение 
	int m_iLb;	//ограничение 
	int m_iBd;	//смещение 
	int m_iLd;	//ограничение 
	int sign_Ab;
	int sign_Bb;
	int sign_Ad;
	int sign_Bd;

	int bias_Ucs;
	int bias_Uc;
	int bias_Ic;
	int bias_Id;
	double t1,t2,t3;
	double v1,v2;
	double value_enter;
	double value_necessary;
	double value_R_nominal;
	int			ControlSwitch();
	void		AutoSaveTablResult();
	int			CommandKol(int k); 
	int			CommandBazaUbsIbm(int k); 
	int			CommandBazaIbsUbm(int k) ;
	int			CommandBazaUbsUbm(int k); 
	int			CommandBazaIbsIbm(int k) ;
	int			CommandDopIssUsm(int k); 
	int			CommandDopUssIsm(int k); 
	int			CommandDopIssIsm(int k); 
	int			CommandDopUssUsm(int k); 
	int			AutoSetupNullIc();
	int			AutoSetupNullIc_10nA(int m_iRange);
	int		  AutoSetupNullUc();
	int		  AutoSetupNullIbm();
	int		  AutoSetupNullUbm();
	int		  AutoSetupNullIbs();
	int		  AutoSetupNullUbs();
	int		  AutoSetupNullIsm();
	int		  AutoSetupNullUsm();
	int		  AutoSetupNullIss();
	int		  AutoSetupNullUss();
	int			KalibrIc();
	int			KalibrUc();
	int			KalibrIb();
	int			KalibrUb();
	int			KalibrIs();
	int			KalibrUs();
	void		AutoSetupNull();
	void		SetModifiedMemory();
	void		WhatFreeMemory();
	void		SetTextInfo(LPCTSTR strMessage);//,LPCTSTR strFile,LPCTSTR strTest);
	void		DrawSetka();
	void		DrawStatusRukoyatka();
	void		ShowControls(UINT* pControls,UINT cControls,BOOL fVisible);
	void		DisableControls(UINT* pControls,UINT cControls,BOOL fVisible);
	void		InitPage(int nPage);
	void        InitPageTuning(int nPage);
	void		InitIconImp(); 

	void	    FileNew();
	void        FileOpen();
	BOOL		FileWrite();
	BOOL        FileSave();
	BOOL        FileSaveAs();
	BOOL        FileClose(); 

	void		TreeOpen();
	void		TreeClose();
	void		TreeUpdate();
	int         FConfig(); 
	void        AddMasEl();

	void        SetModifiedFlag();
	void        DlgTreeUpdate();
	void        GetEntry(POSITION position);
	void        ClearEntry();
	void        InsertEntry(POSITION position);
	void		Add();
	void        Del(); 
	void        CopyInBuf(); 
	void        CopyFromBuf();
	void        CopyEl(LPCTSTR sEl,LPCTSTR sElNew);
	void        CopyTest(LPCTSTR sEl_new,LPCTSTR sMod_new); 
	CMod*       FindListName(LPCTSTR sEl,LPCTSTR sMod);
	CMod*       FindTest(LPCTSTR sEl,LPCTSTR sMod);

	void        ExtractParam();
	void		SaveParam();
//	void        ExtractResults();
	void		SaveResults();
	BOOL		ExtractParamFromTst();
	BOOL        SaveParamToTst(int where);		//where: 0 - сохранение в ...\Tst_standart 
												//		 1 - в ...\Tst_user
	BOOL		SaveRef();
	void        BuildArrayRef(); 
	void		DisplayListRef(); 
	void		SetMassivRefNpoint(BOOL bInversion);
	void		ResetRef(); 
	void		ResetMeas();

	int         ControlParam();
	void        ClearError();
	void        DisplayError();
	void        ClearOtcht();
	void        DisplayOtcht();
	void		Otcht();				//формирование строки отчёта по окончании измерения
	void		DisplayParam(); 
	void	    DisplayRef();

BOOL	InitDialog();
	void	    InitTest();				//включает:
	void		InitStatusKnot(); 
	void        InitStatusKol();
	void        InitStatusBaza();
	void        InitStatusDop();
	void        InitDiapazonKol();
	void        InitDiapazonBaza();
	void        InitDiapazonDop();

	void		ChangeDiapazonKol();

	void	    SetTest();				//включает:
	void		SetStringsSourceMeas();
	void        SetDiapazonKol();
	void        SetDiapazonBaza();
	void        SetDiapazonDop();
	void		SetGnezdo(); 

	void        SetListTest();
	void        BuildCircuit();

	void        SetParamDefault();
	void		SetVidMemDC();			 // установить текущий контекст экрана
										 // в зависимости от nCheckParam и nCheckRef
														 
	BOOL		StringToValue(const CString& sStr, double *value);
	int			Argument();				//поиск аргумента
	void		Arg_X(int Chanel);		//нахождение iX, rej_X, X_min, X_max
	int			Function();				//поиск функции
	void        Fun_Y(int Chanel);		//нахождение iY, rej_Y, Y_min, Y_max среди выходных параметров
    void        Fun1_Y();				//Y_min, Y_max среди входных параметров базы
    void        Fun2_Y();				//Y_min, Y_max среди входных параметров доп. канала

	void		ResetDisplay(int pointNull);	//pointNull - положение координаты (0,0)
	void		SubGetAxes(const double& Min_data,const double& Max_data,int* m_iS,int* expo);
	int         SubGetStrRazmer(int xy,int expo,const CString& sStr);

	void		BuildCommand();
	void		BuildTest();
	BOOL		CompareParam();
	BOOL		GetPassWordLine();
	BOOL		SetPassWordLine();
	BOOL		GetKoefPsw();
	void		SetKoefPswToPic();
	void		SetZapuskToPic();

	void        ClearResults();			//обнулить результаты измерений
	void        ResetResults();			//сбросить указатели на результаты измерений
	UINT		CountnTest();			//подсчет числа тестов для выбранного элемента
	void		Pusk();
	void	    Stop();
	int			Leksema();
	void		ReverseAxis();

	void		DrawGraph();
	void		InitAxis();
	void        SetMassivNpoint(BOOL bInversion);
	void		DisplayMarker();
	void		DisplaySecant();
	void		ResetMarkers();
	
	double		GetXbyY(double Ur_y, int index_j);
	double		GetYbyX(double Ur_x, int index_j);

	
	void		Calculate();			//рассчитать массив
	void		CalculatePar();			//рассчитать параметр
	void		CalculateBetaMax();		//рассчитать Max Beta
	void		CalculateBetaiMax();	//рассчитать Max Betai
	void		InitAxisCalc();
	void		SetMassivCalcNpoints();	//отобразить рассчётный массив
	void		ControlPar();			//контроль параметра на допуск
//	void		SaveResultsCalculate();
	void		ExtractResultsCalculate();

	void		ValueToString(int xy, double value, CString& sStr);
	void		OpenTabl();
	void		ExtraStop(BYTE condition,LPCTSTR strMessage);
//	BOOL		ControlLimit(int cause);			//контроль крышки (1) или срабатывания защиты (2)
	void		MyMessageBeep(UINT uType);
	void		InitTablRes();
	void		ClearTablRes();
	void		SelchangedTree();
//	void		SetOnePoint();
	void		DefinitionDiapazonBaza();		//инициализация диапазонов
	void		DefinitionDiapazonDop();		//инициализация диапазонов
	void		DefinitionDiapazonKol();
	void		DefinitionDiapazon();
	void		MarkerNoakt();
	void		MarkerAkt();
	void		BuildArrayTstUser();
	void		BuildArrayTstStandart();
	int			FromUnicodeToAscii(BYTE* c_ascii,CString strUnicode); 
	void		SaveTablResult();
	void		PolaritySingBaza();
	void		PolaritySingDop();
	void		LoopingCompensation();
	void		ShowButtonLoopingCompensation();
	int			PushKlava(CString strSymbol);
	CString		GetStringFromEdit();
	void		SetMassiv(BOOL bInversion); 
	void		ChangeDiapazonKolSourceU();
	void		ChangeDiapazonKolMeasU();
	BOOL		WriteKoefToPsw();
	void		FlagOLClear();
	void		FinishControlFlags();
	void        InitNewSwitch();
	double		GetNumberFromString(CString str);
	void		InitGnezdo();
	void		ShowGnezdoKommutator();
	int			PuskAutoSetupNull();
	int			PuskKalibr();
	int			PuskKalibrIc(BOOL where, int range); 
	int         PuskKalibrUc(BOOL where, int range);
	int			PuskKalibrUb(BOOL where, int range);
	int			PuskKalibrIb(BOOL where, int range);
	int			PuskKalibrUs(BOOL where, int range);
	int			PuskKalibrIs(BOOL where, int range);

	void ButtonDiod();
	void ButtonNmos();
	void ButtonNmos4();
	void ButtonNpn();
	void ButtonNpn4();
	void ButtonPmos();
	void ButtonPmos4();
	void ButtonPnp();
	void ButtonPnp4();
	void ButtonRes();
	void VklRadioElement();
	
	void ShowRejimChanel(); 
	void ShowQue1();
	void OutLimit(int kod);		//выдать сигнал (команда 33)
	void PuskDifferenceVax();	//Разность ВАХ
	void CalculateDifference();	//Разность ВАХ
	void SetMassivDiffNpoints();
	void ResetDiff();

	void ShowTaskBar(BOOL flagShow); 

	// Dialog Data
	//{{AFX_DATA(CLineDlg)
	enum { IDD = IDD_LINE_DIALOG };
	CGradBtn	m_btn_meas;
	CProgressCtrl	m_Progress;
	CProgressCtrl	m_progress_flash;
	CTreeCtrl	m_TreeCtrl;
	CListCtrl	m_listRes;
	CGradBtn	m_bst_loadtst;
	CGradBtn	m_btn_reset;
	CGradBtn	m_btn_otcht;
	CGradBtn	m_btn_graf;
	CGradBtn	m_btn_memory;
	CGradBtn	m_btn_tuning;
	CGradBtn	m_btn_test;
	CGradBtn	m_btn_file;
	CListCtrl	m_listRef;
	CTabCtrl	m_TabCtrlParam;
	CTabCtrl	m_TabCtrlTuning;
	CString	m_sInfoMeas;
	CString	m_sInfoSignal;
	CString	m_sKnotKol;
	CString	m_sKnotBaza;
	CString	m_sKnotDop;
	CString	m_sKnotEm;
	int		m_iCircuitKol;
	int		m_iCircuitBaza;
	int		m_iCircuitDop;
	CString	m_MOD;
	CString	m_sKolMeasU;
	CString	m_sKolMeasI;
	int		m_irazv;                // Канал С - развёртка
	CString	m_sSliderKolU;
	int		m_iKolU;
	int		m_iKolI;
	int		m_iKolSourceU;
	BOOL	m_bCheckKolRejHigh;
	CString	m_sLimitKolI;
	CString	m_sLimitKolU;
	CString	m_sRangMaxKolI;
	CString	m_sBazaSource;
	int		m_iBazaSource;
	int		m_iAmplitudeStepBaza;
	UINT	m_uStepBaza;
	int		m_iPolarityStepBaza;
	int		m_iPolarityBiasBaza;
	int		m_iRangLimitBaza;
	CString	m_sBiasBaza;
	CString	m_sLimitBaza;
	BOOL	m_bCheckBazaRejHigh;
	CString	m_sDopSource;
	int		m_iAmplitudeStepDop;
	UINT	m_uStepDop;
	int		m_iPolarityStepDop;
	CString	m_sBiasDop;
	int		m_iPolarityBiasDop;
	int		m_iRangLimitDop;
	CString	m_sLimitDop;
	CString	m_sAxisY;
	CString	m_sAxisX;
	CString	m_sPar;
	CString	m_sArg;
	CString	m_sUr1;
	CString	m_sUr2;
	int		m_iForm;
	CString	m_sMin;
	CString	m_sMax;
	int		m_iButNull;
	int		m_iDopSource;
	CString	m_sDlitImp;
	BOOL	m_bCheckPauza;
	CString	m_sNameFileMod;
	CString	m_st_flash;
	CString	m_sRangBiasBaza;
	CString	m_sRangBiasDop;
	BOOL	m_bLooping;
	int		m_iGnezdoC;
	int		m_iGnezdoB;
	int		m_iGnezdoE;
	int		m_iKommutator;
	CString	m_sBazaMeasLimit;
	CString	m_sDopMeasLimit;
	BOOL	m_bBazaVkl;
	BOOL	m_bFamilyVkl;
	CString	m_sStupN;
	int		m_iQue1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineDlg)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CChartCtrl m_Chart;

	// Generated message map functions
	//{{AFX_MSG(CLineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonFile();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileClose();
	afx_msg void OnFileExit();
	afx_msg void OnButtonTest();
	afx_msg void OnDel();
	afx_msg void OnDelAll();
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnModify();
	afx_msg void OnButtonLoadtst();
	afx_msg void OnRadioRazvimppositive();
	afx_msg void OnRadioRazvimpnegative();
	afx_msg void OnRadioRazvdcpositive();
	afx_msg void OnRadioRazvdcnegative();
	afx_msg void OnRadioRazvac();
	afx_msg void OnSelchangeCombokoli();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeCombokolsouru();
	afx_msg void OnRadiobazasourceu();
	afx_msg void OnRadiobazasourcei();
	afx_msg void OnUpdateEditur1();
	afx_msg void OnUpdateEditur2();
	afx_msg void OnUpdateEditnamepar();
	afx_msg void OnUpdateEditnamearg();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeTabctrltuning(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOtcht();
	afx_msg void OnOtchtNew();
	afx_msg void OnOtchtMax();
	afx_msg void OnOtchtMin();
	afx_msg void OnPart();
	afx_msg void OnFileReparol();
	afx_msg void OnButtonTuning();
	afx_msg void OnButtonGraf();
	afx_msg void OnButtonMemory();
	afx_msg void OnSaveTstUser();
	afx_msg void OnSaveTstStandart();
	afx_msg void OnAddTreefrompanel();
	afx_msg void OnAddTreefrommemory();
	afx_msg void OnSelchangeCombocheckkol();
	afx_msg void OnSelchangeCombocheckbaza();
	afx_msg void OnSelchangeCombocheckdop();
	afx_msg void OnSelchangeTabctrlparam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombobazalimitmaxui();
	afx_msg void OnSelchangeCombodoplimitmaxui();
	afx_msg void OnSelchangeComboamplitudebaza();
	afx_msg void OnSelchangeComboamplitudedop();
	afx_msg void OnKillfocusAxisy();
	afx_msg void OnKillfocusAxisx();
	afx_msg void OnKillfocusCombomod();
	afx_msg void OnSelendokCombopolaritystep();
	afx_msg void OnSelendokCombopolaritystepdop();
	afx_msg void OnSelendokCombopolaritybazabias();
	afx_msg void OnSelendokCombopolaritydopbias();
	afx_msg void OnSelchangeCombokolu();
	afx_msg void OnKillfocusEditnamepar();
	afx_msg void OnKillfocusEditnamearg();
	afx_msg void OnTestRename();
	afx_msg void OnButtonReset();
	afx_msg void OnZapuskInternal();
	afx_msg void OnZapuskExternal();
	afx_msg void OnCurveLine();
	afx_msg void OnCurveLinewithpoint();
	afx_msg void OnCurvePoint();
	afx_msg void OnColorFon();
	afx_msg void OnColorCurve();
	afx_msg void OnColorFont();
	afx_msg void OnColorRef();
	afx_msg void OnColorSetka();
	afx_msg void OnColorTangent();
	afx_msg void OnButtonLeft();
	afx_msg void OnButtonRight();
	afx_msg void OnButtonMarkerReset();
	afx_msg void OnSaveRef();
	afx_msg void OnExtractRef();
	afx_msg void OnGetdispinfoListref(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelMemory();
	afx_msg void OnClearMemory();
	afx_msg void OnOtchtMail();
	afx_msg void OnListrefOpen();
	afx_msg void OnListrefClose();
	afx_msg void OnRefReset();
	afx_msg void OnOpentabl();
	afx_msg void OnRadioLeft();
	afx_msg void OnRadioCenter();
	afx_msg void OnRadioRight();
	afx_msg void OnCheckInversX();
	afx_msg void OnCheckInversY();
	afx_msg void OnRadiodopu();
	afx_msg void OnRadiodopi();
	afx_msg void OnCheckpauza();
	afx_msg void OnSoundOn();
	afx_msg void OnSoundOff();
	afx_msg void OnSelchangedTreeTest(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTablMin();
	afx_msg void OnTablresSave();
	afx_msg void OnMarkerAkt();
	afx_msg void OnMarkerNoakt();
	afx_msg void OnDelTstUser();
	afx_msg void OnDelTstStandart();
	afx_msg void OnFileDelete();
	afx_msg void OnTablDelete();
	afx_msg void OnOtchtSave();
	afx_msg void OnDeleteOtch();
	afx_msg void OnTuningCopyfile();
	afx_msg void OnDeltaposSpinstepbaza(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinstepdop(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnCheckLooping();
	afx_msg void OnKillfocusEditur1();
	afx_msg void OnCheckkolrejhigh();
	afx_msg void OnGrafLupa();
	afx_msg void OnKalibr();
	afx_msg void OnUpdateKalibr(CCmdUI* pCmdUI);
	afx_msg void OnAutoSetupNull();
	afx_msg void OnUpdateAutoSetupNull(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeComboKommutator();
	afx_msg void OnSelchangeComboGnezdoC();
	afx_msg void OnSelchangeComboGnezdoB();
	afx_msg void OnSelchangeComboGnezdoE();
	afx_msg void OnTablCoef();
	afx_msg void OnButtonMeas();
	afx_msg void OnModifyIpAddress();
	afx_msg void OnPointSizeSmall();
	afx_msg void OnPointSizeBig();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHibernate();
	afx_msg void OnCheckbazarejhigh();
	afx_msg void OnRadioSynpositive();
	afx_msg void OnRadioSynnegative();
	afx_msg void OnCheckBazaVkl();
	afx_msg void OnCheckFamily();
	afx_msg void OnSelendokComboQue1();
	afx_msg void OnKillfocusEditmin();
	afx_msg void OnKillfocusEditmax();
	afx_msg void OnDifferenceVax();
	afx_msg void OnKillfocusEditstepbaza();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnSelchangedTree();
	afx_msg void OnEndLabelEdit();
	afx_msg LONG OnButton(WPARAM wParam,LPARAM lParam);
//	afx_msg LONG OnError(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnResponse(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnMassiv(WPARAM wParam,LPARAM lParam);
//	afx_msg LONG OnMassiv_1(WPARAM wParam,LPARAM lParam);
//	afx_msg LONG OnErrorEpp(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRefresh(WPARAM wParam,LPARAM lParam);
	void OnCancel();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEDLG_H__074D2EB6_E91A_42F1_806D_0D35CE8EAD6F__INCLUDED_)
