// © Ilmatieteenlaitos/Lasse.
// Original 19.3.1998
// 
//********************************************************
// 
// Kirjoittaa (current) parametrinimen ps-oliona output-
// tiedostoon. Ainoastaan ylitetty PressText:n WritePs-
// metodi. Toinen emo on NFmiPressDataObject, jossa parametri. 
//
//********************************************************
//
//Muutettu 150300/LW    
//Muutettu 040900/LW +IsDataObject()

//---------------------------------------------------------------------------

#ifndef __NPPARTEX_H__
#define __NPPARTEX_H__

#include "npretext.h"
#include "npredata.h"

class _FMI_DLL NFmiPressParamText : public NFmiPressText, public NFmiPressDataObject
{
	public:
		NFmiPressParamText(void) :  NFmiPressText()
		                          ,NFmiPressDataObject()
		                          {};

		NFmiPressParamText(const NFmiPressText& theTextParamRect); 

		virtual ~NFmiPressParamText(){};

//		virtual FmiBoolean		ReadDescription(NFmiString& retString); 
//		virtual NFmiParamRect*	Clone(void) const; 
//      int ConvertDefText(NFmiString & object);

		virtual FmiBoolean	WritePS(FmiPressOutputMode theOutput);  
 	    virtual bool IsDataObject(void) {return true;};  //4.9.00 
      
	protected:               
		
};

#endif //__NPRETEXT_H__
