// © Ilmatieteenlaitos/Lasse.
// Original 21.1.2000
// 
//********************************************************
//
// Nimip‰iv‰(t) lehtituotteeseen
// k‰ytt‰‰ yleist‰ nimip‰iv‰luokkaa 
//
//********************************************************
//
//Muutettu xxxxxx/LW 

//---------------------------------------------------------------------------

#ifndef __NFMIPRESSNAMEDAY_H__
#define __NFMIPRESSNAMEDAY_H__

#include "NFmiPressText.h"
#include "NFmiNameDay.h"
#include "NFmiMetTime.h"

typedef enum
{
	 dMaxLength	= 2800
	,dMaxNumber
}NFmiPressNameDayObjects;

class _FMI_DLL NFmiPressNameDay : public NFmiPressText 
{
	public:
		NFmiPressNameDay(void) : NFmiPressText()
							,itsNameDay(0)
							,itsMaxNumber(kShortMissing)
							,itsMaxLength(kShortMissing){};

		virtual ~NFmiPressNameDay();

		virtual bool ReadRemaining(void);  
		virtual int ConvertDefText(NFmiString & object); 
		virtual bool	WritePS(FmiPressOutputMode theOutput);
		void SetMaxNumber(FmiCounter maxNumber) {itsMaxNumber=maxNumber;};
 		void SetMaxLength(FmiCounter maxLength) {itsMaxLength=maxLength;};
  		void SetNameDay(NFmiNameDay* theNameDay) {itsNameDay=theNameDay;};
     
	protected:
		NFmiNameDay* itsNameDay;
		FmiCounter itsMaxNumber;
		FmiCounter itsMaxLength;  //voisi olla textill‰
};

#endif //__NFMIPRESSNAMEDAY_H__
