// © Ilmatieteenlaitos/Lasse.
// Original 2.4.1998
// 
//*************************************************************
// 
// Luokka tuottaa datan aktuellin ajan ps-oliona output-
// tiedostoon. Emo on abstrakti NFmiPressTimeText ja sisaruksia
// NFmiPressDataTimeText ja NFmiPressComputerTimeText.  . 
//
//*************************************************************
//
//Muutettu 280100/LW +itsStation, +fLocalTime; PressParam:n StationList:n eka asema, ei
//                   siis datan jossa myös ei-piirrettäviä. Tarvitaan jos
//                   aika piirretään localina
//                   +ReadRemaining(),+ConvertDefText()
//    EI TULLUTKAAN TÄNNE VAAN HOIDETAAN NFmiParamRectissä 
//Muutettu 160800/LW +WritePSUpdatingSubText() tarvitaan kun segmentissä aikataulukossa
//                   on jatkoaikateksti
//Muutettu 040900/LW +ClassId()
//Muutettu 260201/LW +fUseAnalyzingTime
//---------------------------------------------------------------------------

#ifndef __NPTIDTEX_H__
#define __NPTIDTEX_H__


#include "nptimtex.h"
#include "npredata.h"
#include "nstatpnt.h"

typedef enum
{
	 dUseOriginTime         =600   //25.2.01
}NFmiPressDataTimeTextObjects;

class _FMI_DLL NFmiPressDataTimeText : public NFmiPressTimeText, public NFmiPressDataObject
{
	public:
		NFmiPressDataTimeText(void) :  NFmiPressTimeText()
		                          ,NFmiPressDataObject()
								  ,fUseOriginTime(kFalse)
		                          {};

//		NFmiPressDataTimeText(const NFmiPressText& theTextParamRect); 

		virtual ~NFmiPressDataTimeText();
		FmiBoolean ReadRemaining(void);         //25.2.01
		int ConvertDefText(NFmiString & object); //25.2.01
		
		virtual FmiBoolean	WritePS(FmiPressOutputMode theOutput); //15.3.00
		virtual FmiBoolean	WritePSUpdatingSubText(FmiPressOutputMode theOutput); //16.8.00									
	    virtual unsigned long ClassId(void){return kNFmiPressDataTimeText;}; //4.9.00 

	protected:
		FmiBoolean fUseOriginTime; //26.2.01
		
};

#endif //__NPTIDTEX_H__
