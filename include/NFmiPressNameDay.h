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

#include "NFmiMetTime.h"
#include "NFmiNameDay.h"
#include "NFmiPressText.h"

//! Undocumented
enum NFmiPressNameDayObjects
{
  dMaxLength = 2800,
  dMaxNumber
};

//! Undocumented
class NFmiPressNameDay : public NFmiPressText
{
 public:
  ~NFmiPressNameDay() override;
  NFmiPressNameDay();

  bool ReadRemaining() override;
  int ConvertDefText(NFmiString& object) override;
#ifdef OLD_MSC
  using NFmiPressText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  bool WritePS(FmiPressOutputMode theOutput) override;
  void SetMaxNumber(FmiCounter maxNumber);
  void SetMaxLength(FmiCounter maxLength);
  void SetNameDay(NFmiNameDay* theNameDay);
  void SetTime(const NFmiMetTime& theTime) override;
  void ChangeByHours(long hours) override;

 protected:
  NFmiNameDay* itsNameDay = nullptr;
  FmiCounter itsMaxNumber;
  FmiCounter itsMaxLength;  // voisi olla textill‰

};  // class NFmiPressNameDay

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressNameDay::NFmiPressNameDay()
    : itsMaxNumber(kShortMissing), itsMaxLength(kShortMissing)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressNameDay::SetTime(const NFmiMetTime& theTime)
{
  itsFirstPlotTime = theTime;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param maxNumber Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressNameDay::ChangeByHours(long hours)
{
  itsFirstPlotTime.ChangeByHours(hours);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param maxNumber Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressNameDay::SetMaxNumber(FmiCounter maxNumber)
{
  itsMaxNumber = maxNumber;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param maxLength Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressNameDay::SetMaxLength(FmiCounter maxLength)
{
  itsMaxLength = maxLength;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theNameDay Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressNameDay::SetNameDay(NFmiNameDay* theNameDay)
{
  itsNameDay = theNameDay;
}
#endif  // NFMIPRESSNAMEDAY_H

// ======================================================================
