// ======================================================================
/*!
 * \file
 * \brief NFmiLetterParamRect.h
 */
// ======================================================================
/*!
 * \class NFmiLetterParamRect
 *
 * T‰m‰ luokka hallitsee dataohjattujen sanojen tuottamisen
 * m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
 * Sanat voivat olla yhden merkin mittaisia ja valinta tapahtuu
 * "mappaustekniikan" avulla (Muunnos, NFmiParamMapping).
 *
 */
// ======================================================================

#ifndef NFMILETTERPARAMRECT_H
#define NFMILETTERPARAMRECT_H

#include "NFmiPressText.h"
#include "NFmiTextParamRect.h"

//! Undocumented
enum NFmiLetterParamRectObjects
{
  dLetterMapping = 180,
  dColumnText
};

//! Undocumented
class NFmiLetterParamRect : public NFmiTextParamRect
{
 public:
  ~NFmiLetterParamRect() override;

  NFmiLetterParamRect();
  NFmiLetterParamRect(const NFmiLetterParamRect &theLetterParamRect);

  bool ReadDescription(NFmiString& retString) override;
  NFmiParamRect* Clone() const override;
  int ConvertDefText(NFmiString& object) override;

  bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
               NFmiFastQueryInfo* theQI,
               std::ofstream& theDestinationFile,
               FmiPressOutputMode theOutput) override;

  FmiGenericColor MapColor() const override;

 protected:
 private:
  bool ModifyTextBySeason(NFmiString &theString);
  NFmiPressText *itsColumnText;

};  // class NFmiLetterParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiLetterParamRect::NFmiLetterParamRect()
{
  itsMapping = nullptr;
  fUseSelectLatinFont = true;
  itsColumnText = nullptr;
}

#endif  // NFMILETTERPARAMRECT_H

// ======================================================================
