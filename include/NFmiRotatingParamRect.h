//© Ilmatieteenlaitos/Lasse.
//Original 20.11.1997
//
//****************************************************************
// T‰m‰ luokka hallitsee dataohjattujen k‰‰ntyvien symbolien tuottamisen
// m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
// Symboli k‰‰nnet‰‰n data-arvon mukaan, jonka oletetaan olevan tuulen-
// suunta (0-360). Symbolin oletetaan lis‰ksi olevan kuvatiedostossa
// k‰‰nnettyn‰ arvoon 270 (l‰nsi). Voidaan parametrisoida myˆhemmin 
// jos tarvitaan.
//****************************************************************
//  
// Muutettu 110298/LW pois WritePS()  
// Muutettu 041198/LW +itsRotatingDataIdent
// Muutettu 231198/LW +DoPostReading()
// Muutettu 241198/LW +itsNotRotatingMinValue,itsNotRotatingMaxValue
// Muutettu 080199/LW +GetSecondDataIdent(),PointSecondParam(), SetProducer()
// Muutettu 110400/LW +itsVerticalLong
// Muutettu 071100/LW +peritty nyt uudesta NFmi2SymbolParamRectist‰
//---------------------------------------------------------------------------

#ifndef __NROTPARR_H__
#define __NROTPARR_H__

#include "NFmi2SymbolParamRect.h"

typedef enum
{
//	 dRotatingPar = 107
	 dNotRotInterval = 109    
	,dVerticalLong       //110400

}NFmiRotatingParamRectObjects;

class _FMI_DLL NFmiRotatingParamRect : public NFmi2SymbolParamRect  //7.11.00
{
	public:
		NFmiRotatingParamRect(void): NFmi2SymbolParamRect(){
				itsNotRotatingMinValue = 10.;
			    itsNotRotatingMaxValue =-10.;
				itsVerticalLong=kFloatMissing;}; 
/*
		NFmiRotatingParamRect ( NFmiDataIdent theParam
							, NFmiRect theRelativeSize	)
							:  NFmiSymbolParamRect(theParam
							                      ,theRelativeSize)
														  {}; 
*/		
		NFmiRotatingParamRect(const NFmiRotatingParamRect& theSymbolRect);

		virtual bool      ReadRemaining(void);
		
		virtual ~NFmiRotatingParamRect();
        virtual int ConvertDefText(NFmiString & object);
		virtual NFmiParamRect*	Clone(void) const;
	protected:
//        bool      ReadValues( NFmiQueryInfo* theQI); //23.8.99
//		NFmiDataIdent	GetSecondDataIdent(void) const; //080199
//		virtual void    AddSymbolMetaOptions(void); //8.11.00
		bool      Rotate(void) const; //241198
		virtual void    DoPostReading(void); //231198   
	private:
		bool CopyShortSymbol2Dest(NFmiString* symbolFile, std::ofstream& theDestinationFile); //11.2
		float AdjustToMap(float theDirection)const;    //110400
		virtual void WriteMetaCode(NFmiString* symbolFile, NFmiPoint realPos, std::ofstream & os); //14.11.00

	private:
//		float itsSecondParamValue;             
//		NFmiDataIdent itsRotatingDataIdent;  
		double itsNotRotatingMinValue;         
		double itsNotRotatingMaxValue;         
		double itsVerticalLong;             

};

//inline NFmiDataIdent NFmiRotatingParamRect::GetSecondDataIdent() const { return itsRotatingDataIdent;} //080199

#endif //__NROTPARR_H__
