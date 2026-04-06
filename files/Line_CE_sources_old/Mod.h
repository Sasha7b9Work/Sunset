// Mod.h: interface for the CMod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOD_H__455C9078_5540_4110_825C_F066E8E7E1FD__INCLUDED_)
#define AFX_MOD_H__455C9078_5540_4110_825C_F066E8E7E1FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMod : public CObject  
{
	DECLARE_SERIAL(CMod)
public:
	CMod();
	virtual ~CMod();
    virtual void Serialize(CArchive& ar);

	CString strDateMeas;	// ƒата проведени€ измерени€
	CString strTimeMeas;	// ¬рем€ проведени€ измерени€

	CString	m_EL;			// Ёлемент
	CString m_MOD;			// “ест
	CString m_PRIM;			//  омментарии (краткое описание теста)
	//схема включени€
	int		m_nelement;		// ¬ид тестируемого элемента:-1-не задано,0-D,1-R,
							//							  2-NPN,3-PNP,4-NMOS,5-PMOS,
							//							  6-NPN4,7-PNP4,8-NMOS4,9-PMOS4
	int		m_iCircuitKol;	
	int		m_iCircuitBaza;
	int		m_iCircuitDop;
	int		m_iKommutator;
	int     m_iGnezdoC;
	int     m_iGnezdoB;
	int		m_iGnezdoE;
	//коллектор
	int		m_irazv;
	int		m_idlitimp;
	int		m_iKolU;
	int		m_iKolI;
	int		m_iKolSourceU;
	int		m_iLimitKolU;		//текущее значение слайдера ограничени€ U коллектора (в %)
	int		m_iLimitKolI;		//текущее значение слайдера ограничени€ I коллектора
	BOOL	m_bCheckKolRejHigh;
	int		m_itipimp;		
	BOOL	m_bCheckPauza;
	//база
	int		m_iBazaSource;
	BOOL	m_bBazaVkl;
	int		m_iAmplitudeStepBaza;
	UINT	m_uStepBaza;
	int		m_iPolarityStepBaza;
	int		m_iQue1;	//m_iRangBiasBaza;
	int		m_iBiasBaza;		//текущее значение слайдера смещени€ базы
	int		m_iPolarityBiasBaza;
	int		m_iBazaMeas;
	int		m_iRangLimitBaza;
	int		m_iLimitBaza;		//текущее значение слайдера ограничени€ базы
	BOOL	m_bCheckBazaRejHigh;
	//доп.канал
	int		m_iDopSource;
//	BOOL	m_bDopVkl;
	int		m_iAmplitudeStepDop;
	UINT	m_uStepDop;
	int		m_iPolarityStepDop;
	int		m_iFamilyVkl;	//m_iRangBiasDop;
	int		m_iBiasDop;			//текущее значение слайдера смещени€ доп.канала
	int		m_iPolarityBiasDop;
	int		m_iDopMeas;
	int		m_iRangLimitDop;
	int		m_iLimitDop;		//текущее значение слайдера ограничени€ доп.канала
	//расчЄт
	CString	m_sAxisY;
	CString	m_sAxisX;
	CString	m_sPar;
	CString	m_sArg;
	CString	m_sUr1;
	CString	m_sUr2;
	UINT	m_uTp;
	int		m_iForm;
	CString	m_sMin;
	CString	m_sMax;

/*	UINT	n_Result;
	UINT	n_Step;
	UINT	n_Mas;
	double  Beta_max;
	double  Betai_max;
	double  Ib_Beta_max;
	double  Ib_Betai_max;
	double  Par;
	double  pM1[NMAS][NREZ];		//массивы результатов (ACP1)
	double  pM2[NMAS][NREZ];		//(ACP2)
	double  pM3[NMAS][NREZ];		//(ACP3)
	double  pM4[NMAS][NREZ];		//(ACP4)
	double  pM5[NMAS][NREZ];		//(расчЄтный массив)
*/
};
typedef CTypedPtrList <CObList, CMod*> CModList;

#endif // !defined(AFX_MOD_H__455C9078_5540_4110_825C_F066E8E7E1FD__INCLUDED_)
