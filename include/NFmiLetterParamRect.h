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
class _FMI_DLL NFmiLetterParamRect : public NFmiTextParamRect
{
 public:
  virtual ~NFmiLetterParamRect(void);

  NFmiLetterParamRect(void);
  NFmiLetterParamRect(const NFmiLetterParamRect &theLetterParamRect);

  virtual bool ReadDescription(NFmiString &retString);
  virtual NFmiParamRect *Clone(void) const;
  int ConvertDefText(NFmiString &object);

  virtual bool WritePS(const NFmiRect &AbsoluteRectOfSymbolGroup,
                       NFmiFastQueryInfo *theQI,
                       std::ofstream &theDestinationFile,
                       FmiPressOutputMode theOutput);

  FmiGenericColor MapColor(void) const;

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

inline NFmiLetterParamRect::NFmiLetterParamRect(void) : NFmiTextParamRect()
{
  itsMapping = 0;
  fUseSelectLatinFont = true;
  itsColumnText = 0;
}

#endif  // NFMILETTERPARAMRECT_H

// ======================================================================
