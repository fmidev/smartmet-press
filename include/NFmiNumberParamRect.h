// © Ilmatieteenlaitos/Lasse.
// Original 5.3 1998
// Erotettu yleisestä tekstioliosta numeroita varten
// 
//****************************************************************
// Tämä luokka hallitsee dataohjattujen numeroiden tuottamisen
// määrittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
// 
//****************************************************************
//
//Muutettu 260398/LW -fColorValueDependent koska emolla on jo
//Muutettu 000299/LW +fZeroMinus: nolla merkitään miinuksena (puuttuvana)
//Muutettu 150300/LW metakieli ja xml output eps:n lisäksi
//Muutettu 100402/LW +fDetachSign

//---------------------------------------------------------------------------

#ifndef __NNUMPARR_H__
#define __NNUMPARR_H__


typedef enum
{
	 dFormat = 27  //10.4.02 28->27
	,dZeroMinus     
	,dDetachSign  //10.4.02
}NFmiNumberParamRectObjects;


#include "NFmiTextParamRect.h"

class _FMI_DLL NFmiNumberParamRect : public NFmiTextParamRect
{
	public:
		NFmiNumberParamRect(void) :  NFmiTextParamRect()
		                          ,itsFormat(NFmiString("%1.f"))
								  ,fZeroMinus(false)
								  ,fDetachSign(false)
		                          {};
		NFmiNumberParamRect(const NFmiNumberParamRect& theNumberParamRect); 

		
		virtual ~NFmiNumberParamRect();

		virtual bool		ReadDescription(NFmiString& retString); 
        virtual bool ReadRemaining(void); //171198  
		virtual NFmiParamRect*	Clone(void) const; 
        int ConvertDefText(NFmiString & object);

		virtual bool	WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
									,NFmiFastQueryInfo* theQI
									,std::ofstream& theDestinationFile
									,FmiPressOutputMode theOutput); //15.3.00
//		void SetValueDepending(float theValue);
//		void SetColor(float theC, float theM, float theY, float theK);
	private:
		NFmiString DetachSign(const NFmiString& theString, float theValue) const;
	protected:
		FmiGenericColor MapColor(void) const; //3.11.00 

	protected:              
		
		NFmiString itsFormat;
		bool fZeroMinus;  //110299
		bool fDetachSign; //10.4.02
};

#endif //__NNUMPARR_H__
