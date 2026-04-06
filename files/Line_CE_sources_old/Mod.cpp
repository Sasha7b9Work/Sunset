// Mod.cpp: implementation of the CMod class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Line.h"
#include "Mod.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMod, CObject, 0)

CMod::CMod()
{
	strDateMeas = _T("");		// дата
	strTimeMeas = _T("");		// время
	//элемент
	m_EL = _T("");				// наименование элемента (уровень 0 узла в дереве)
	m_MOD = _T("");				// наименование теста (уровень 1 узла в дереве)
	m_PRIM= _T("");				// комментарии
	//схема включения
	m_nelement     = -1;		// Вид тестируемого элемента:-1-не задано,0-D,1-R,
								//							  2-NPN,3-PNP,4-NMOS,5-PMOS,
								//							  6-NPN4,7-PNP4,8-NMOS4,9-PMOS4
	m_iCircuitKol  = 1;			// вкл
	m_iCircuitBaza = 0;			// обрыв
	m_iCircuitDop  = 0;			// обрыв
	m_iKommutator  = 0;
	m_iGnezdoC     = 0;
	m_iGnezdoB     = 1;
	m_iGnezdoE     = 2;
	//коллектор
	m_irazv = -1;
	m_idlitimp = 0;
	m_iKolU = -1;
	m_iKolI = -1;
	m_iKolSourceU = 0;
	m_iLimitKolU =100;		//текущее значение слайдера ограничения U коллектора (в %)
	m_iLimitKolI =0;		//текущее значение слайдера ограничения I коллектора
	m_bCheckKolRejHigh = FALSE;
	m_itipimp = TIP_SYN;
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

	m_iQue1 = 0;//m_iRangBiasBaza = 0;
	m_iFamilyVkl = 0;//m_iRangBiasDop = 0;

//	strDateMeas = _T("");
//	strTimeMeas = _T("");
/*	n_Result = 200;
	n_Step = 1;
	n_Mas  = 1;
	
	for (int m=0;m<NMAS;m++)
	for (int i=0;i<NREZ;i++)
	{	pM1[m][i]=0.0;
		pM2[m][i]=0.0;
		pM3[m][i]=0.0;
		pM4[m][i]=0.0;
		pM5[m][i]=0.0;
	}
	pM[0]=&pM1[0][0];
	pM[1]=&pM2[0][0];
	pM[2]=&pM3[0][0];
	pM[3]=&pM4[0][0];
	pM[4]=&pM5[0][0];
	*/
}

CMod::~CMod()
{

}

void CMod::Serialize(CArchive& ar)
{ int i = 0;
  UINT m = 0;
    if (ar.IsStoring())
	{//storing
	   ar << strDateMeas << 
		   strTimeMeas << 
		   m_EL  << 
		   m_MOD << 
		   m_PRIM << 
		     m_nelement << 
			 m_iCircuitKol << m_iCircuitBaza <<	m_iCircuitDop <<
			 m_iKommutator << m_iGnezdoC << m_iGnezdoB <<	m_iGnezdoE <<
			 m_irazv  << m_idlitimp << m_iKolU << m_iKolI << m_iKolSourceU <<	
	         m_iLimitKolU << m_iLimitKolI << 
			 m_bCheckKolRejHigh <<	 m_itipimp <<
			 m_bCheckPauza <<
			 m_iBazaSource << 	m_bBazaVkl <<
			 m_iAmplitudeStepBaza << m_uStepBaza << m_iPolarityStepBaza << m_iQue1 << //m_iRangBiasBaza << 
			 m_iBiasBaza << 	m_iPolarityBiasBaza << 
			 m_iBazaMeas << m_iRangLimitBaza << m_iLimitBaza << 
			 m_bCheckBazaRejHigh << 
			 m_iDopSource << 	//m_bDopVkl << 
			 m_iAmplitudeStepDop << m_uStepDop << m_iPolarityStepDop << m_iFamilyVkl << //m_iRangBiasDop <<
			 m_iBiasDop << m_iPolarityBiasDop << 
			 m_iDopMeas << m_iRangLimitDop << m_iLimitDop << 
			 m_sAxisY << m_sAxisX <<  m_sPar << m_sArg << m_uTp << 
		 	 m_sUr1 << m_sUr2 << m_iForm << m_sMin << m_sMax ;//<<
//			 n_Result << n_Step << n_Mas <<Beta_max << Betai_max << Ib_Beta_max << Ib_Betai_max << Par;//  
/*		for (m=0;m<n_Mas;m++)
		for (i=0;i<NREZ;i++)
		  ar <<	pM1[m][i];
		for (m=0;m<n_Mas;m++)
		for (i=0;i<NREZ;i++)
		  ar <<	pM2[m][i];
		for (m=0;m<n_Mas;m++)
		for (i=0;i<NREZ;i++)
		  ar <<	pM3[m][i];
		for (m=0;m<n_Mas;m++)
		for (i=0;i<NREZ;i++)
		  ar <<	pM4[m][i];
		for (m=0;m<n_Mas;m++)
		for (i=0;i<NREZ;i++)
		  ar <<	pM5[m][i];

*/	}
    else 
	{//loading
       ar >> strDateMeas >>  strTimeMeas >> 
		   m_EL  >>
		   m_MOD  >>
		   m_PRIM >> 
		     m_nelement >>
			 m_iCircuitKol >> m_iCircuitBaza >>	m_iCircuitDop >>
			 m_iKommutator >> m_iGnezdoC >> m_iGnezdoB >>	m_iGnezdoE >>
			 m_irazv >>	m_idlitimp >> m_iKolU >> m_iKolI >>	m_iKolSourceU >>	
	         m_iLimitKolU >> m_iLimitKolI >> 
			 m_bCheckKolRejHigh >>	m_itipimp >>
			 m_bCheckPauza >>
			 m_iBazaSource >> m_bBazaVkl >>
			 m_iAmplitudeStepBaza >> m_uStepBaza >> m_iPolarityStepBaza >> m_iQue1 >> //m_iRangBiasBaza >>
			 m_iBiasBaza >> 	m_iPolarityBiasBaza >> 
			 m_iBazaMeas >> m_iRangLimitBaza >> m_iLimitBaza >> 
			 m_bCheckBazaRejHigh >> 
			 m_iDopSource >> 	//m_bDopVkl >> 
			 m_iAmplitudeStepDop >> m_uStepDop >> m_iPolarityStepDop >> m_iFamilyVkl >> //m_iRangBiasDop >>
			 m_iBiasDop >> m_iPolarityBiasDop >> 
			 m_iDopMeas >> m_iRangLimitDop >> m_iLimitDop >> 
			 m_sAxisY >> m_sAxisX >>  m_sPar >> m_sArg >> m_uTp >> 
		 	 m_sUr1 >> m_sUr2 >> m_iForm >> m_sMin >> m_sMax;// >>
//			 n_Result >> n_Step >> n_Mas >> Beta_max >> Betai_max >> Ib_Beta_max >> Ib_Betai_max >> Par;//  ;// >> 
/*		for (m=0;m<n_Mas;m++)
		 for (i=0;i<NREZ;i++)
		  ar >>	pM1[m][i];
		for (m=0;m<n_Mas;m++)
		 for (i=0;i<NREZ;i++)
		  ar >>	pM2[m][i];
		for (m=0;m<n_Mas;m++)
		 for (i=0;i<NREZ;i++)
		  ar >>	pM3[m][i];
		for (m=0;m<n_Mas;m++)
		 for (i=0;i<NREZ;i++)
		  ar >>	pM4[m][i];
		for (m=0;m<n_Mas;m++)
		 for (i=0;i<NREZ;i++)
		  ar >>	pM5[m][i];
 */  }
}
