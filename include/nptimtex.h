// © Ilmatieteenlaitos/Lasse.
// Original 10.3.1998
// 
//********************************************************
// Ps-aikatekstien (tekstien erikoistapauksena (NFmiPressText)) 
// abstrakti luokka. Aikaformaatti j‰senen‰. T‰st‰ perit‰‰n edelleen 
// datanAika, koneenAika ja annettuAika.
//********************************************************
//
// Muutettu 020498/LW perint‰ PressDataObjectista katkaistu, t‰m‰n lapsella
//                   NFmiPressDataTimeText:lla on vasta data
//                   t‰m‰ on abstrakti josta periytyy NFmiDataTimeText ja
//                                                    NFmiGivenTimeText
//Muutettu 150300/LW erilaisia output-formaatteja mahdollistettu

//---------------------------------------------------------------------------

#ifndef __NPTIMTEX_H__
#define __NPTIMTEX_H__

typedef enum
{
	 dTimeTextFormat	= 160
	,dTimeTextDays
}NFmiTimeTextObjects;

#include "npretext.h"
//#include "npredata.h"
#include "npretime.h" 

class _FMI_DLL NFmiPressTimeText : public NFmiPressText//2.4 , public NFmiPressDataObject
{
	public:
		NFmiPressTimeText(void) :  NFmiPressText()
		            //              ,NFmiPressDataObject()  //2.4
		                          ,itsFormat(kWwsHH)
		                          {};

		NFmiPressTimeText(const NFmiPressText& theTextParamRect); 

		virtual ~NFmiPressTimeText();

		virtual FmiBoolean		ReadDescription(NFmiString& retString); 
//		virtual NFmiParamRect*	Clone(void) const; 
        virtual int ConvertDefText(NFmiString & object);

		virtual FmiBoolean	WritePS(FmiPressOutputMode theOutput) =0; //15.3.00  
//		void SetValueDepending(float theValue); ei viel‰ t‰‰ss‰, jossain lapsessa jossa on Value()
//		void SetColor(float theC, float theM, float theY, float theK);
		void SetLanguage(FmiLanguage newLanguage); //24.3.00		
       
	protected:               
		
		unsigned long itsFormat;
		unsigned long itsOrigFormat;  //24.3.00
		NFmiString itsOrigFont;

};

#endif //__NPRETEXT_H__
