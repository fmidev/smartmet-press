//© Ilmatieteenlaitos/Lasse.
//Original 6.11.2000
//
//****************************************************************
// T‰m‰ luokka tuo toisen parametrin Symboliparametreihi, jota 
// lapsissa k‰ytet‰‰n symbolin k‰‰nt‰miseen(NFmiRotatingParamRect) tai 
// skaalaamiseen(NFmiScalingParamRect)
//****************************************************************
//  
// Muutettu 110298/LW pois WritePS()  
// Muutettu 041198/LW +itsRotatingDataIdent
// Muutettu 231198/LW +DoPostReading()
// Muutettu 241198/LW +itsNotRotatingMinValue,itsNotRotatingMaxValue
// Muutettu 080199/LW +GetSecondDataIdent(),PointSecondParam(), SetProducer()
// Muutettu 110400/LW +itsVerticalLong
// Muutettu 030502/LW ReadValues() toka par
//---------------------------------------------------------------------------

#ifndef __N2SYMBOLPARAMRECT_H__
#define __N2SYMBOLPARAMRECT_H__

#include "nsymparr.h"


typedef enum
{
	 dSecondPar = 107
//	,dNotRotInterval = 109    
//	,dVerticalLong       //110400

}NFmi2SymbolParamRectObjects;

class _FMI_DLL NFmi2SymbolParamRect : public NFmiSymbolParamRect
{
	public:
		NFmi2SymbolParamRect(void): NFmiSymbolParamRect(){;}; 
/*
		NFmiRotatingParamRect ( NFmiDataIdent theParam
							, NFmiRect theRelativeSize	)
							:  NFmiSymbolParamRect(theParam
							                      ,theRelativeSize)
														  {}; 
*/		
		NFmi2SymbolParamRect(const NFmi2SymbolParamRect& theSymbolRect);

		virtual FmiBoolean      ReadRemaining(void); 
		
		virtual ~NFmi2SymbolParamRect();
        virtual int ConvertDefText(NFmiString & object);
		virtual NFmiParamRect*	Clone(void) const;

	protected:
        FmiBoolean      ReadValues( NFmiFastQueryInfo* theQI, bool SetRelHour=true); //3.5.02 toka par ) 
//        FmiBoolean      PointOnSecondParam(NFmiQueryInfo* theQI); 
		NFmiDataIdent	GetSecondDataIdent(void) const; 
//		virtual void    DoPostReading(void); 

		float itsSecondParamValue;              
		NFmiDataIdent itsSecondDataIdent;
		
	private:
//		FmiBoolean CopyShortSymbol2Dest(NFmiString* symbolFile, ofstream theDestinationFile); //11.2
//		float AdjustToMap(float theDirection) const;    

	private:

};

inline NFmiDataIdent NFmi2SymbolParamRect::GetSecondDataIdent() const { return itsSecondDataIdent;} 

#endif //__NROTPARR_H__
