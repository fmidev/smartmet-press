/*---------------------------------------------------------------------------*/ 
//© Ilmatieteenlaitos/Lasse.               
// 
// Originaali 6.3.1998 
//********************************************************
// 
// Tässä on lisätty emon formaatteihin aika monta uutta 
// aikaformaattia, mitkä lehtituotteissa ovat käyttö-
// kelpoisia. ToStr-muunnoksessa voidaan parametrein ohjata
// käytettävää kieltä. Tarvittaessa voidaan uusia  formaatteja
// lisätä. 
// On jäsenenä NFmiPressTimeText-luokassa. eikä ole?
//
//********************************************************
//
// 
//
// Muutettu 020300/perintä muutettu NFmiTime:ksi, oli MetTime
// Muutettu 020300 +InterpretToStr()
//
/*---------------------------------------------------------------------------*/
 
#ifndef __NPRETIME_H__ 
#define __NPRETIME_H__ 
 
#include  "NFmiMetTime.h" 
/*
const unsigned long kShortYear	= 1;	// YY
const unsigned long kLongYear	= 2;	// YYYY
const unsigned long kMonth		= 4;	// MM
const unsigned long kDay		= 8;	// DD

const unsigned long kHour		= 16;	// HH
const unsigned long kMinute		= 32;	// MM
const unsigned long kSecond		= 64;	// SS
*/
const unsigned long kUndefined  = 0;	// 
const unsigned long kShortWeekDay  = 128;	//  pienikirjain=teksti
const unsigned long kLongWeekDay   = 256; //    //1.4
const unsigned long kShortMonthName = 512; //
const unsigned long kLongMonthName = 1024; //
const unsigned long kSpace      = 2048;	
const unsigned long kDot        = 4096;
const unsigned long kShortDay   = 8192;    // D
const unsigned long kShortMonth = 16384;   // M
const unsigned long kShortHour  = 32768;   // H   29.2.2000
const unsigned long kI		    = 65536;   //15.3.2000
const unsigned long kDdMd       = kShortDay+kDot+kShortMonth;      //1.4
const unsigned long kNnnnsYYYY  = kLongMonthName+kSpace+kLongYear;	
const unsigned long kWwHH		= kHour+kShortWeekDay;
const unsigned long kWwsHH		= kHour+kSpace+kShortWeekDay;
const unsigned long kWwsDDdMMd	= kShortWeekDay+kSpace+kDay+kDot+kMonth; //19.3 vika piste tulee aina
const unsigned long kWwsDdMd	= kShortWeekDay+kSpace+kShortDay+kDot+kShortMonth; //19.3 vika piste tulee aina
const unsigned long kHHdMM	    = kHour+kDot+kMinute; //7.4
const unsigned long kDDdMMdYYYY	= kMonth+kDot+kDay+kLongYear; //080299
const unsigned long kDdMdYYYY	= kShortMonth+kDot+kShortDay+kLongYear; //080299
const unsigned long kHdMM	    = kShortHour+kDot+kMinute; //29.2.2000
const unsigned long kH		    = kShortHour;              //29.2.2000

/*
const unsigned long kYYMMDD			= kShortYear+kMonth+kDay;
const unsigned long kYYYYMMDD		= kLongYear+kMonth+kDay;
const unsigned long kYYMMDDHH		= kShortYear+kMonth+kDay+kHour;
const unsigned long kYYYYMMDDHH		= kLongYear+kMonth+kDay+kHour;
const unsigned long kYYMMDDHHMMSS	= kShortYear+kMonth+kDay+kHour+kMinute;
const unsigned long kYYYYMMDDHHMM	= kLongYear+kMonth+kDay+kHour+kMinute;
const unsigned long kYYMMDDHHMM		= kShortYear+kMonth+kDay+kHour+kMinute+kSecond;
const unsigned long kYYYYMMDDHHMMSS	= kLongYear+kMonth+kDay+kHour+kMinute+kSecond;
const unsigned long kMMDDHH			= kMonth+kDay+kHour;
*/
class _FMI_DLL NFmiPressTime : public NFmiTime //2.3.2000 uusi perintä, oli metTimestä
{ 
 
 public: 
    NFmiPressTime (void){};
    NFmiPressTime (const NFmiTime& time)
		:NFmiTime(time){};
/*    NFmiPressTime (const NFmiTime& time)    //1.3.2000
	{   
	   SetYear(time.GetYear()); 
	   SetMonth(time.GetMonth()); 
	   SetDay(time.GetDay()); 
	   SetHour(time.GetHour()); 
	   SetMin(time.GetMin()); 
	   SetSec(time.GetSec()); 
	};
	*/
//    NFmiPressTime (const NFmiStringTime&); 
//    NFmiPressTime (const short year, const short month, const short day); 
//    NFmiPressTime (const short year, const short month, const short day 
//		   ,const short hour, const short minute=0, const short sec=0); 
 
//    virtual FmiBoolean IsEqual (const NFmiSortable& aFmiTest) const; 
//    virtual FmiBoolean IsLessThan (const NFmiSortable& aFmiTest) const; 
 
 
    NFmiString ToStr(const unsigned long theTimeMask, const FmiLanguage theLanguage=kFinnish) const;
    NFmiString InterpretToStr(const NFmiString theTimeCode, const FmiLanguage theLanguage=kFinnish) const; //3.10.00
//   void	   FromStr(const NFmiString theTimeString,const unsigned long theTimeMask = kYYYYMMDDHHMMSS);
 
 protected: 
	NFmiString RelativeDay(FmiLanguage theLanguage, NFmiString theStr6, int& thePlusInd) const; //5.1.01 

 private: 
}; 
 
typedef NFmiPressTime* PTFmiPressTime; 
 
#endif //__NFMISTATICTIME_H__


