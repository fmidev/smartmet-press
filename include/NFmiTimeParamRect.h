// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiTimeParamRect
 */
// ======================================================================
/*!
 * \class NFmiTimeParamRect
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMITIMEPARAMRECT_H
#define NFMITIMEPARAMRECT_H

#include "NFmiPressTime.h"
#include "NFmiStationPoint.h"
#include "NFmiTextParamRect.h"

//! Undocumented
enum NFmiTimeParamRectObjects
{
  dTimeFormat = 190,
  dParamRectLanguage,
  dFinnishTimezone
};

//! Undocumented
class NFmiTimeParamRect : public NFmiTextParamRect
{
 public:
  virtual ~NFmiTimeParamRect(void);
  NFmiTimeParamRect(void);
  NFmiTimeParamRect(const NFmiTimeParamRect& theTimeParamRect);

  virtual bool ReadDescription(NFmiString& retString);
  virtual NFmiParamRect* Clone(void) const;
  virtual int ConvertDefText(NFmiString& object);
  void SetLanguage(FmiLanguage theLanguage);

  virtual bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
                       NFmiFastQueryInfo* theQI,
                       std::ofstream& theDestinationFile,
                       FmiPressOutputMode theOutput);

 protected:
  bool PointParam(NFmiFastQueryInfo* theQI);
  virtual NFmiTime TimeToWrite(NFmiFastQueryInfo* theQI);

  unsigned long itsFormat;
  unsigned long itsOrigFormat;
  NFmiString itsOrigFont;
  NFmiStationPoint itsStationPoint;
  bool fIsValidTime;

 private:
  FmiLanguage itsLanguage;
  bool fFinnishTimezone;
};  // class NFmiTimeParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiTimeParamRect::NFmiTimeParamRect(void)
    : NFmiTextParamRect(),
      itsFormat(kHHdMM),
      fIsValidTime(true),
      itsLanguage(kFinnish),
      fFinnishTimezone(false)
{
}

#endif  // NFMITIMEPARAMRECT_H

// ======================================================================
