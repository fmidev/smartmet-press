// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressNameDay
 */
// ======================================================================
/*!
 * \class NFmiPressNameDay
 *
 * Nimip‰iv‰(t) lehtituotteeseen. K‰ytt‰‰ yleist‰ nimip‰iv‰luokkaa
 * (NFmiNameDay) mutta osaa press-luokkana lukea m‰‰rittelyns‰ ja 
 * kirjoittaa eps-koodinsa.
 *
 */
// ======================================================================

#ifndef NFMIPRESSNAMEDAY_H
#define NFMIPRESSNAMEDAY_H

#include "NFmiPressText.h"
#include "NFmiNameDay.h"
#include "NFmiMetTime.h"


//! Undocumented
enum NFmiPressNameDayObjects
{
  dMaxLength = 2800,
  dMaxNumber
};


//! Undocumented
class _FMI_DLL NFmiPressNameDay : public NFmiPressText 
{
public:

  virtual ~NFmiPressNameDay(void);
  NFmiPressNameDay(void);

  virtual bool ReadRemaining(void);  
  virtual int ConvertDefText(NFmiString & object); 
  virtual bool WritePS(FmiPressOutputMode theOutput);
  void SetMaxNumber(FmiCounter maxNumber);
  void SetMaxLength(FmiCounter maxLength);
  void SetNameDay(NFmiNameDay * theNameDay);
     
protected:

  NFmiNameDay * itsNameDay;
  FmiCounter itsMaxNumber;
  FmiCounter itsMaxLength;  //voisi olla textill‰

}; // class NFmiPressNameDay


// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressNameDay::NFmiPressNameDay(void)
  : NFmiPressText()
  , itsNameDay(0)
  , itsMaxNumber(kShortMissing)
  , itsMaxLength(kShortMissing)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param maxNumber Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressNameDay::SetMaxNumber(FmiCounter maxNumber)
{
  itsMaxNumber=maxNumber;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param maxLength Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressNameDay::SetMaxLength(FmiCounter maxLength)
{
  itsMaxLength=maxLength;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theNameDay Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressNameDay::SetNameDay(NFmiNameDay * theNameDay)
{
itsNameDay=theNameDay;
}

#endif // NFMIPRESSNAMEDAY_H

// ======================================================================

