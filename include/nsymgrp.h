//© Ilmatieteenlaitos/Salla.
//Original 21.10.1997
// 
//****************************************************************
// T‰m‰ luokka toimii datariippuvien olioiden (symbolit, numerot) 
// listana yhden segmentin (NFmiPresParam) puitteissa. Luokalla on
// suorakaide piirroksia varten, jonka sis‰ll‰ eri olioille annetaan
// suhteellinen ala k‰ytett‰v‰ksi.  
//****************************************************************
//  
//Viimeinen Muutos	24.10.1997	WritePS(const NFmiPoint), ReadDescription();
//			Muutos	18.11.1997	Add
//			Muutos				itsSize dataosa -> itsRectSize	
//			Muutos				Convert(NFmiString&)
//Muutos 201197/LW itsSymbolDir pois
//Muutos 121297/LW peritty NFmiDescription:sta
//Muutettu 120198/LW perint‰ NFmiPressDescriptionista, +itsProducer
//Muutettu 160198/LW ReadDescription():iin palautusStringi
//Muutettu 040298/LW +NumOfMissing()
//Muutettu 240298/LW +itsPsSymbol
//Muutettu 290698/LW peritty NFmiPressTimeDescription:sta eik‰ NFmiPressDescription:sta
//Muutettu 080998/LW +itsPressScalingObjects
//Muutettu 99/LW +itsPressParam
//Muutettu 130899/LW pressparam-v‰litet‰‰n t‰nne ja edelleen piirto-olioille
//Muutettu 150300/LW metakieli ja xml output eps:n lis‰ksi
//Muutettu 270600/LW +InitMissing()
//---------------------------------------------------------------------------
#ifndef __NSYMGRP_H__
#define __NSYMGRP_H__

#include "NFmiSize.h"
#include "nparrect.h"
#include "nrectsca.h"
#include "NFmiQueryData.h"
#include "nstatpnt.h"
#include "nprtides.h"
#include "NFmiExtremeTimeParamRect.h"
  
class NFmiPressParam;


class NFmiPsSymbol;

class _FMI_DLL NFmiSymbolGroup : public NFmiSize, public NFmiPressTimeDescription //29.6
{
	public:
		NFmiSymbolGroup(void) :  itsPressParam(0)
		                        ,itsProducer(230)   
								,itsPsSymbol(0)
								,itsParamRects(0)
								,itsRectSize(40,40)  
								,itsRectScale(0)
								,itsQueryData(0)
								,itsQueryDataIter(0)
								{};
		NFmiSymbolGroup(NFmiPressParam* pressParam
			            ,NFmiParamRect** theSymbolList
						,int num);
		
		~NFmiSymbolGroup(void);

		long NumOfMissing(void) const; 
		void InitMissing(void); //27.6.00
		FmiBoolean  ReadDescription(NFmiString& retString); 
		                      //25.2 v‰liaikaista:
		FmiBoolean	WritePS(const NFmiStationPoint &theStationPoint,FmiPressOutputMode theOutput); //15.3.00

		FmiBoolean SetPressParam(NFmiPressParam* param) 
		                {itsPressParam = param;
		                 return isTrue;};
		FmiBoolean	SetProducer(unsigned int producer)
		                {itsProducer = producer;
		                 return isTrue;};
		
		FmiBoolean	Set(NFmiRectScale &theRectScale
						,NFmiFastQueryInfo* theQueryDataIter 
						,std::ofstream& theDestinationFile);

		FmiBoolean	Add(const NFmiParamRect & theParamRect);
		int			ConvertDefText(NFmiString & object);
		FmiBoolean SetAllTimes(const NFmiMetTime& theTime); //22.3.00   
		FmiBoolean SetAllLanguages(FmiLanguage theLanguage);//24.3.00 

	private:
		void SetScalingMode(void); 

		NFmiPressParam* itsPressParam; 
		FmiBoolean      fNewScaling;  
		unsigned long   itsProducer;  
		NFmiPsSymbol*   itsPsSymbol; 
		NFmiVoidPtrList itsPressScalingObjects; 
		NFmiParamRect**	itsParamRects;
		NFmiPoint		itsRectSize; 

		NFmiRectScale*	itsRectScale;
		NFmiQueryData*	itsQueryData;
//		NFmiSuperSmartInfo*	itsQueryDataIter;
		NFmiFastQueryInfo*	itsQueryDataIter;
//		std::ofstream	itsOutFile;
//Mikan ehdotus
		std::ofstream	* itsOutFile;
};

#endif //__NSYMGRP_H__
