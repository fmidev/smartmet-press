// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressTime
 */
// ======================================================================
/*!
 * \class NFmiPressTime
 * 
 * T�ss� on lis�tty emon formaatteihin aika monta uutta 
 * aikaformaattia, mitk� lehtituotteissa ovat k�ytt�kelpoisia.
 * ToStr-muunnoksessa voidaan parametrein ohjata
 * k�ytett�v�� kielt�. Tarvittaessa voidaan uusia  formaatteja
 * lis�t�. 
 * On j�senen� NFmiPressTimeText-luokassa. eik� ole?
 *
 */
// ======================================================================
 
#ifndef NFMIPRESSTIME_H
#define NFMIPRESSTIME_H
 
#include  "NFmiMetTime.h" 

const unsigned long kUndefined  = 0;
const unsigned long kShortWeekDay  = 128;	// pienikirjain=teksti
const unsigned long kLongWeekDay   = 256;
const unsigned long kShortMonthName = 512;
const unsigned long kLongMonthName = 1024;
const unsigned long kSpace      = 2048;	
const unsigned long kDot        = 4096;
const unsigned long kShortDay   = 8192;
const unsigned long kShortMonth = 16384;
const unsigned long kShortHour  = 32768;
const unsigned long kI		    = 65536;
const unsigned long kDdMd       = kShortDay+kDot+kShortMonth;
const unsigned long kNnnnsYYYY  = kLongMonthName+kSpace+kLongYear;	
const unsigned long kWwHH		= kHour+kShortWeekDay;
const unsigned long kWwsHH		= kHour+kSpace+kShortWeekDay;
const unsigned long kWwsDDdMMd	= kShortWeekDay+kSpace+kDay+kDot+kMonth;
const unsigned long kWwsDdMd	= kShortWeekDay+kSpace+kShortDay+kDot+kShortMonth;
const unsigned long kHHdMM	    = kHour+kDot+kMinute;
const unsigned long kDDdMMdYYYY	= kMonth+kDot+kDay+kLongYear;
const unsigned long kDdMdYYYY	= kShortMonth+kDot+kShortDay+kLongYear;
const unsigned long kHdMM	    = kShortHour+kDot+kMinute;
const unsigned long kH		    = kShortHour;


//! Undocumented
class _FMI_DLL NFmiPressTime : public NFmiTime
{ 
 
public: 

  NFmiPressTime(void);
  NFmiPressTime(const NFmiTime & theTime);
 
  NFmiString ToStr(const unsigned long theTimeMask,
				   const FmiLanguage theLanguage=kFinnish) const;

  NFmiString InterpretToStr(const NFmiString theTimeCode,
							const FmiLanguage theLanguage=kFinnish) const;

 protected: 

  NFmiString RelativeDay(FmiLanguage theLanguage,
						 NFmiString theStr6,
						 int & thePlusInd) const;

 private: 

}; // class NFmiPressTime
 
//! Undocumented, should be removed
typedef NFmiPressTime * PTFmiPressTime; 

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressTime::NFmiPressTime(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiPressTime::NFmiPressTime(const NFmiTime & theTime)
  : NFmiTime(theTime)
{
}
 
#endif //__NFMISTATICTIME_H__


