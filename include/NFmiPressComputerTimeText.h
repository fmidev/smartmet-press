// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressComputerTimeText
 */
// ======================================================================
/*!
 * \class NFmiPressComputerTimeText
 *
 * Luokka tuottaa koneen ajan ps-oliona output-
 * tiedostoon. Toistaiseksi k‰ytet‰‰n Suomen aikaa.
 * Emo on abstrakti NFmiPressTimeText ja sisaruksia ovat
 * NFmiPressDataTimeText ja NFmiPressComputerTimeText.
 *
 */
// ======================================================================

#ifndef NFMIPRESSCOMPUTERTIMETEXT_H
#define NFMIPRESSCOMPUTERTIMETEXT_H

#include "NFmiPressTimeText.h"

//! Undocumented
class NFmiPressComputerTimeText : public NFmiPressTimeText
{
 public:
  ~NFmiPressComputerTimeText(void) override;
  NFmiPressComputerTimeText(void);

  bool ReadRemaining(void) override;
  bool SetText(void) override;

  void SetText(NFmiString theText) override;
  int ConvertDefText(NFmiString& object) override;

#ifdef OLD_MSC
  using NFmiPressTimeText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  bool WritePS(FmiPressOutputMode theOutput) override;

 protected:
  short itsDeltaDays;
  short itsDeltaHours;

};  // class NFmiPressComputerTimeText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressComputerTimeText::NFmiPressComputerTimeText(void)
    : NFmiPressTimeText(), itsDeltaDays(0), itsDeltaHours(0)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressComputerTimeText::SetText(NFmiString theText)
{
  NFmiPressTimeText::SetText(theText);
}

#endif  // NFMIPRESSCOMPUTERTIMETEXT_H

// ======================================================================
