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
  ~NFmiTimeParamRect() override;
  NFmiTimeParamRect();
  NFmiTimeParamRect(const NFmiTimeParamRect& theTimeParamRect);

  bool ReadDescription(NFmiString& retString) override;
  NFmiParamRect* Clone() const override;
  int ConvertDefText(NFmiString& object) override;
  void SetLanguage(FmiLanguage theLanguage) override;

  bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
               NFmiFastQueryInfo* theQI,
               std::ofstream& theDestinationFile,
               FmiPressOutputMode theOutput) override;

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

inline NFmiTimeParamRect::NFmiTimeParamRect()
    : NFmiTextParamRect(),
      itsFormat(kHHdMM),
      fIsValidTime(true),
      itsLanguage(kFinnish),
      fFinnishTimezone(false)
{
}

#endif  // NFMITIMEPARAMRECT_H

// ======================================================================
