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
  virtual ~NFmiSunTimeParamRect(void);
  NFmiSunTimeParamRect(void);
  NFmiSunTimeParamRect(const NFmiSunTimeParamRect& theTimeParamRect);

  virtual bool ReadRemaining(void);
  virtual NFmiParamRect* Clone(void) const;
  int ConvertDefText(NFmiString& object);

 protected:
  NFmiTime TimeToWrite(NFmiFastQueryInfo* theQI);

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
