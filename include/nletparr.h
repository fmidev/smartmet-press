// © Ilmatieteenlaitos/Lasse.
// Original 10.2.98
// 
//****************************************************************
// T‰m‰ luokka hallitsee dataohjattujen sanojen tuottamisen
// m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
// Sanat voivat olla yhden merkin mittaisia ja valinta tapahtuu
// "mappaustekniikan" avulla (Muunnos, NFmiParamMapping).
// 
//****************************************************************
//
// 
// Muutos 121198  
// Muutettu 230899/LW +kirjoituspuolelle (WritePS jne) QD korvatti QI:ll‰
//Muutettu 150300/LW metakieli ja xml output eps:n lis‰ksi
//Muutettu 061100/LW +MapColor()

//---------------------------------------------------------------------------

#ifndef __NLETPARR_H__
#define __NLETPARR_H__

//#include "nparamap.h"

typedef enum
{
	dLetterMapping = 180

}NFmiLetterParamRectObjects;


#include "ntexparr.h"

class _FMI_DLL NFmiLetterParamRect : public NFmiTextParamRect
{
	public:
		NFmiLetterParamRect(void) :  NFmiTextParamRect() {itsMapping=0;
														fUseSelectLatinFont=kTrue;}; //121198
		NFmiLetterParamRect(const NFmiLetterParamRect& theLetterParamRect); 

		virtual ~NFmiLetterParamRect();

		virtual FmiBoolean		ReadDescription(NFmiString& retString); //16.1
		virtual NFmiParamRect*	Clone(void) const; 
        int ConvertDefText(NFmiString & object);

		virtual FmiBoolean	WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
									,NFmiFastQueryInfo* theQI   //23.8.99 oli QD
									,std::ofstream& theDestinationFile
									,FmiPressOutputMode theOutput); //15.3.00
		FmiGenericColor MapColor(void) const; //6.11.00
//		void SetValueDepending(float theValue);
//		void SetColor(float theC, float theM, float theY, float theK);

	private:
//		NFmiParamMapping*	itsMapping;
};

#endif //__NLETPARR_H__
