// © Ilmatieteenlaitos/Lasse.
// Original 6.3.98
//
// Aikatekstejä
// 
// Muutos 050399 +PointParam() oli kyllä emolla 
// Muutettu 230899/LW +kirjoituspuolelle (WritePS jne) QD korvatti QI:llä
// Muutettu 020200/LW +itsLanguage ja muita korjauksia
// Muutettu 290200/LW +TimeToWrite()
// Muutettu 010300/LW +LocalTime()
// Muutettu 010300/LW +itsStationPoint()
//Muutettu 150300/LW metakieli ja xml output eps:n lisäksi
//Muutettu 220500/LW pois oma ReadRemaining()
//Muutettu 220500/LW +fIsValidTime 
//---------------------------------------------------------------------------

#ifndef __NTIMPARR_H__
#define __NTIMPARR_H__

//#include "NFmiParamMapping.h"

typedef enum
{
	dTimeFormat = 190
	,dParamRectLanguage //2.2.2000

}NFmiTimeParamRectObjects;


#include "NFmiTextParamRect.h"
#include "NFmiPressTime.h"
#include "NFmiStationPoint.h"

class _FMI_DLL NFmiTimeParamRect : public NFmiTextParamRect
{
	public:
		NFmiTimeParamRect(void) :  NFmiTextParamRect()
						,itsFormat(kHHdMM)
						,fIsValidTime(true)
						,itsLanguage(kFinnish) {}

		NFmiTimeParamRect(const NFmiTimeParamRect& theTimeParamRect); 

		virtual ~NFmiTimeParamRect();

		virtual bool		ReadDescription(NFmiString& retString); //16.1
//		virtual bool ReadRemaining(void){return true;};  //290200 //pois 22.5.00
		virtual NFmiParamRect*	Clone(void) const; 
        virtual int ConvertDefText(NFmiString & object); //27.2.00 virtual
		void SetLanguage(FmiLanguage theLanguage) ;//23.3.00 

		virtual bool	WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
									,NFmiFastQueryInfo* theQI
									,std::ofstream& theDestinationFile
									,FmiPressOutputMode theOutput); //15.3.00
	protected:
//		bool ToLocalTime(NFmiPressTime& theTime) const;
        bool PointParam(NFmiFastQueryInfo* theQI);
		virtual NFmiTime	TimeToWrite(NFmiFastQueryInfo* theQI);

		unsigned long itsFormat;
		unsigned long itsOrigFormat;  //24.3.00
		NFmiString itsOrigFont;
		NFmiStationPoint itsStationPoint; //2.3.2000
		bool fIsValidTime; //2.6.00 puuttuva auringonnousu/lasku

	private:
		FmiLanguage itsLanguage; //2.2.2000
};

#endif //__NLETPARR_H__
