// © Ilmatieteenlaitos/Lasse.
// Original 6.3.1998
// 
//********************************************************
// Palstakirjoittamisen luokka. Tostaiseksi vain suorakaiteen 
// muotoisia palstoja. Ps-koodiin tehdyt makrot mahdollistaisivat
// kyll‰ ihan vapaamuotoisia palstoja (esim ympyr‰) mutta
// Illu ei osanut esitt‰‰ niit‰.  
//********************************************************
//
//Muutettu 190398/LW +itsLanguage    
//Muutettu 111298/LW +ScalePlot    

//*****************************
//
// POISTETTU 181298
// yhdistetty NFmiPressText:iin
//
//*****************************

//---------------------------------------------------------------------------

#ifndef __NCOLTEXT_H__
#define __NCOLTEXT_H__

typedef enum
{
	 dTopMargin	= 210
	,dLeftMargin
	,dRightMargin
	,dLineStep

}NFmiPressColumnTextObjects;

#include "npretext.h"

class _FMI_DLL NFmiColumnText : public NFmiPressText
{
	public:
		NFmiColumnText(void) :  NFmiPressText()
			                      ,itsLineStep(-1.)
		                          {};
		NFmiColumnText(const NFmiColumnText& theTextParamRect); 

		virtual ~NFmiColumnText();

		virtual FmiBoolean		ReadDescription(NFmiString& retString); 
        int ConvertDefText(NFmiString & object);
        void ScalePlotting(void); //111298

		virtual FmiBoolean	WritePS(void);  
        FmiBoolean WriteString(const NFmiString& commentString);																					
	protected:               
			 		
		double itsLineStep;
		double itsTopMargin;
		double itsRightMargin;
		double itsLeftMargin;
};

#endif //__NPRETEXT_H__
