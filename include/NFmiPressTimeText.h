// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressTimeText
 */
// ======================================================================
/*!
 * \class NFmiPressTimeText
 *
 * Ps-aikatekstien (tekstien erikoistapauksena (NFmiPressText))
 * abstrakti luokka. Aikaformaatti j‰senen‰. T‰st‰ perit‰‰n edelleen
 * datanAika, koneenAika ja annettuAika.
 *
 */
// ======================================================================

#ifndef NFMIPRESSTIMETEXT_H
#define NFMIPRESSTIMETEXT_H

#include "NFmiPressText.h"
#include "NFmiPressTime.h"

//! Undocumented
enum NFmiTimeTextObjects
{
  dTimeTextFormat = 160,
  dTimeTextDays,
  dTimeTextHours
};

//! Undocumented
class NFmiPressTimeText : public NFmiPressText
{
 public:
  ~NFmiPressTimeText() override;
  NFmiPressTimeText();
  NFmiPressTimeText(const NFmiPressText& theTextParamRect);

  bool ReadDescription(NFmiString& retString) override;
  int ConvertDefText(NFmiString& object) override;

#ifdef OLD_MSC
  using NFmiPressText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  bool WritePS(FmiPressOutputMode theOutput) override = 0;

  void SetLanguage(FmiLanguage newLanguage) override;

 protected:
  unsigned long itsFormat;
  unsigned long itsOrigFormat;
  NFmiString itsOrigFont;

};  // class NFmiPressTimeText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressTimeText::NFmiPressTimeText() : itsFormat(kWwsHH) {}
#endif  // NFMIPRESSTIMETEXT_H

// ======================================================================
