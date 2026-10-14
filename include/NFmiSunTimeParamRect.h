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
  ~NFmiSunTimeParamRect() override;
  NFmiSunTimeParamRect();
  NFmiSunTimeParamRect(const NFmiSunTimeParamRect& theTimeParamRect);

  bool ReadRemaining() override;
  NFmiParamRect* Clone() const override;
  int ConvertDefText(NFmiString& object) override;

 protected:
  NFmiTime TimeToWrite(NFmiFastQueryInfo* theQI) override;

 private:
  bool fIsSunRise = true;

};  // class NFmiSunTimeParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiSunTimeParamRect::NFmiSunTimeParamRect()
{
  itsFormat = kHHdMM;
}

#endif  // NFMISUNTIMEPARAMRECT_H

// ======================================================================
