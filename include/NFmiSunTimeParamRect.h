// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiSunTimeParamRect
 */
// ======================================================================
/*!
 * \class NFmiSunTimeParamRect
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMISUNTIMEPARAMRECT_H
#define NFMISUNTIMEPARAMRECT_H

#include "NFmiTimeParamRect.h"

//! Undocumented
enum NFmiSunTimeParamRectObjects
{
  dSunRise = 535,
  dSunSet

};

//! Undocumented
class NFmiSunTimeParamRect : public NFmiTimeParamRect
{
 public:
  ~NFmiSunTimeParamRect(void) override;
  NFmiSunTimeParamRect(void);
  NFmiSunTimeParamRect(const NFmiSunTimeParamRect& theTimeParamRect);

  bool ReadRemaining(void) override;
  NFmiParamRect* Clone(void) const override;
  int ConvertDefText(NFmiString& object) override;

 protected:
  NFmiTime TimeToWrite(NFmiFastQueryInfo* theQI) override;

 private:
  bool fIsSunRise;

};  // class NFmiSunTimeParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiSunTimeParamRect::NFmiSunTimeParamRect(void) : NFmiTimeParamRect(), fIsSunRise(true)
{
  itsFormat = kHHdMM;
}

#endif  // NFMISUNTIMEPARAMRECT_H

// ======================================================================
