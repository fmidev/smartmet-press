//© Ilmatieteenlaitos/Lasse.
//Original 7.11.2000
//
//****************************************************************
// T‰m‰ luokka hallitsee dataohjattujen skaalattavien symbolien tuottamisen
//****************************************************************
//  
// Muutettu xxxxxx/LW 
//---------------------------------------------------------------------------

#ifndef __NFMISCALINGPARAMRECT_H__
#define __NFMISCALINGPARAMRECT_H__

#include "NFmi2SymbolParamRect.h"

typedef struct       //17.10.00
{
	double		noneValue;  //t‰ll‰ j‰‰ pois, normaalisti nolla
	double		symbolValue;//talletettua symbolia vastaava arvo
	double	    maxValue;   //max-arvo, jonka mukaan skaalataan
}FmiValueScaling;

typedef enum
{
	 dHeightScale            
	,dWidthScale            
}NFmiScalingParamRectObjects;

class _FMI_DLL NFmiScalingParamRect : public NFmi2SymbolParamRect  
{
	public:
		NFmiScalingParamRect(void): NFmi2SymbolParamRect(){
								 {itsXValueScaling.symbolValue=kFloatMissing;
								  itsYValueScaling.symbolValue=kFloatMissing;}; 

		};
/*
		NFmiRotatingParamRect ( NFmiDataIdent theParam
							, NFmiRect theRelativeSize	)
							:  NFmiSymbolParamRect(theParam
							                      ,theRelativeSize)
														  {}; 
*/		
		NFmiScalingParamRect(const NFmiScalingParamRect& theSymbolRect);

		virtual bool      ReadRemaining(void);
		
		virtual ~NFmiScalingParamRect();
        virtual int ConvertDefText(NFmiString & object);
		virtual NFmiParamRect*	Clone(void) const;

	protected:
		virtual void    DoPostReading(void); 
		bool ReadSymbolScale(FmiValueScaling& theScale); //201000
		void       ScaleByValue();             //7.11.00
		FmiValueScaling     itsXValueScaling;  //17.10.00
		FmiValueScaling     itsYValueScaling;  //17.10.00
//		double				itsHeightScale;     //16.10.00
//		double				itsHeightScaleBase;  //16.10.00


	private:
//		bool CopyShortSymbol2Dest(NFmiString* symbolFile, ofstream theDestinationFile); //11.2

	private:

};


#endif //__NROTPARR_H__
