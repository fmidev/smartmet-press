/*---------------------------------------------------------------------------*/ 
//© Ilmatieteenlaitos/Lasse.               NFmiPressEnvironment.h 
// 
//Originaali 17.11.98 
// 
//Tähän voidaan kerätä lehtisääohjelman sellaisia piirteitä, joita
// voi antaa hierarkian eri tasoilla. Esim ylimmällä tuotetasolla ja
// alemmilla ylikirjoittaa. Ekaksi tähän otetaan uusi pitkä/lyhyt 
// numeromiinus.
//
// muutos 181000/LW +rgbColor, cmyk ollut alusta (ei tosin tässä)
// muutos 070901/LW +itsMaskNumber
// muutos 230502/LW +itsAdditionalDayAdvance
//
/*---------------------------------------------------------------------------*/
 
#ifndef __NPRENVIR_H__ 
#define __NPRENVIR_H__ 
 
#include  "NFmiGlobals.h"
//#include  "fcoltyp.h" //ei sekoiteta nviewtä tähän
#include "NFmiPressTypes.h"
class NFmiPressArea;
 
class _FMI_DLL NFmiPressEnvironment  
{ 
 public: 
	 NFmiPressEnvironment (void) :
	   itsMaskNumber(0) //12.11.01
	   ,itsAdditionalDayAdvance(0) //23.5.02
	   ,fLongNumberMinus(false)
	   {                         //HUOM vain jompikumpi värijärjestelmä
		   itsGenericColor.color.cmyk.cyan = 0.;
		   itsGenericColor.color.cmyk.magenta = 0.;
		   itsGenericColor.color.cmyk.yellow = 0.;
		   itsGenericColor.color.cmyk.black = 1.;
		   itsGenericColor.isRgb = false;
	   };
    NFmiPressEnvironment (const NFmiPressEnvironment& aTime);
    ~NFmiPressEnvironment(); 

	void       SetLongNumberMinus(void) {fLongNumberMinus=true;};
	void       SetShortNumberMinus(void) {fLongNumberMinus=false;};
	void       SetRGB(FmiRGBColor theColor) {itsGenericColor.color.rgb = theColor;
											itsGenericColor.isRgb = true;}; //vain toinen kerralla
	void       SetCMYK(FmiCMYK theColor) {itsGenericColor.color.cmyk = theColor;
											itsGenericColor.isRgb = false;};  //vain toinen kerralla
	void SetColor  (const FmiGenericColor& theColor){itsGenericColor = theColor;}; //3.11.00 
	void SetMaskNumber  (unsigned long theMaskNumber){itsMaskNumber = theMaskNumber;}; //6.9.01 
	bool ChangeMaskNumber  (unsigned long theMaskNumber){if(itsMaskNumber > 0){ itsMaskNumber = theMaskNumber; return true;}
																				else return false;}
	unsigned long GetMaskNumber  (void){return itsMaskNumber;};                      //6.9.01 
	bool IsRGB(void) const {return itsGenericColor.isRgb;};
	bool IsCMYK(void) const {return !itsGenericColor.isRgb;};
	FmiGenericColor GetColor(void)const {return itsGenericColor;};
	bool  GetLongNumberMinus(void)const {return fLongNumberMinus;};
	void SetDayAdvance (long theDayAdvance) {itsAdditionalDayAdvance=theDayAdvance;};
	long GetDayAdvance (void) {return itsAdditionalDayAdvance;};

 private:
	unsigned long itsMaskNumber;     //7.9.01
	bool fLongNumberMinus;
	FmiGenericColor itsGenericColor; 
	long itsAdditionalDayAdvance;  //23.5.02
}; 
#endif //__NPRENVIR_H__


