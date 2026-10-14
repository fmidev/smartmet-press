// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressGivenTimeText
 */
// ======================================================================
/*!
 * \class NFmiPressGivenTimeText
 *
 * Luokalle annetaan aika jonka voi kirjoittaa ps-oliona
 * output-tiedostoon. Emo on abstrakti NFmiPressTimeText ja
 * sisaruksia NFmiPressDataTimeText ja NFmiPressComputerTimeText.
 *
 */
// ======================================================================

#ifndef NFMIPRESSGIVENTIMETEXT_H
#define NFMIPRESSGIVENTIMETEXT_H

#include "NFmiPressTimeText.h"

//! Undocumented
class NFmiPressGivenTimeText : public NFmiPressTimeText
{
 public:
  ~NFmiPressGivenTimeText() override;
  NFmiPressGivenTimeText();

  bool ReadRemaining() override;
#ifdef OLD_MSC
  using NFmiPressTimeText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  bool WritePS(FmiPressOutputMode theOutput) override;
  bool SetText() override;
  void SetText(NFmiString theText) override;
  void SetTime(const NFmiMetTime& theTime) override;

};  // class NFmiPressGivenTimeText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressGivenTimeText::NFmiPressGivenTimeText() {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressGivenTimeText::SetText(NFmiString theText)
{
  NFmiPressTimeText::SetText(theText);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressGivenTimeText::SetTime(const NFmiMetTime& theTime)
{
  itsFirstPlotTime = theTime;
}

#endif  // NFMIPRESSGIVENTIMETEXT_H

// ======================================================================
