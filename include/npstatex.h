// © Ilmatieteenlaitos/Lasse.
// Original 16.3.1998
// 
//********************************************************
// 
// Kirjoittaa QD:n (current)asemannimen ps-oliona output-
// tiedostoon. Nimi voidaan haluttaessa muuttaa, esim
// Kˆˆpenhamina->K:hamina. Nimenmuutoslista tuodaan 
// segmentista, joka lukee nimenvaihdot m‰‰rityksest‰.
//
//********************************************************
//
//Muutettu 051198/LW +NFmiRenaming, itsNewNames: nimen vaihdot tarvittaessa  
//Muutettu 250898/LW +itsPressParam, tarvitaan grideiss‰
//Muutettu 070600/LW +ChangeNewName()  
//Muutettu 040900/LW +ClassId()

//---------------------------------------------------------------------------

#ifndef __NPSTATEX_H__
#define __NPSTATEX_H__

//#include "npstatex.h" 
#include "npretext.h"
#include "npredata.h"

typedef struct  //051198
{
	NFmiString		originalName;
	NFmiString		newName;

}NFmiRenaming;

class NFmiPressParam;

class _FMI_DLL NFmiPressStationText : public NFmiPressText, public NFmiPressDataObject
{
	public:
		NFmiPressStationText(void) :  NFmiPressText()
		                          ,NFmiPressDataObject()
								  ,itsNewNames(0)
		                          {};

//		NFmiPressStationText(const NFmiPressText& theTextParamRect); 

		virtual ~NFmiPressStationText();

		void SetNewNames(NFmiVoidPtrList* theNames){itsNewNames=theNames;};
		FmiBoolean SetNewName(const NFmiRenaming& theNames); //7.6.00
		void SetPressParam(NFmiPressParam* pressParam){itsPressParam=pressParam;};//25.8.99
		virtual FmiBoolean	WritePS(FmiPressOutputMode theOutput); //15.3.00
		NFmiString StationName(void);  //12.6.00 muutettu public		
	    virtual unsigned long ClassId(void){return kNFmiPressStationText;}; //4.9.00 
		
		
       
	protected:
		NFmiVoidPtrList* itsNewNames; //051198
		NFmiPressParam*  itsPressParam; //25.8.99 tarvitaan grideissa aseman nimen hakuun
		                                // -> luokka ei en‰‰ yleinen
};

#endif //__NPRETEXT_H__
