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
class _FMI_DLL NFmiPressTimeText : public NFmiPressText
{
 public:
  virtual ~NFmiPressTimeText(void);
  NFmiPressTimeText(void);
  NFmiPressTimeText(const NFmiPressText& theTextParamRect);

  virtual bool ReadDescription(NFmiString& retString);
  virtual int ConvertDefText(NFmiString& object);

#ifdef OLD_MSC
  using NFmiPressText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  virtual bool WritePS(FmiPressOutputMode theOutput) = 0;

  void SetLanguage(FmiLanguage newLanguage);

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

inline NFmiPressTimeText::NFmiPressTimeText(void) : NFmiPressText(), itsFormat(kWwsHH) {}
#endif  // NFMIPRESSTIMETEXT_H

// ======================================================================
