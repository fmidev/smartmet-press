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
 * tiedostoon. Toistaiseksi k�ytet��n Suomen aikaa.
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
  virtual ~NFmiPressComputerTimeText(void);
  NFmiPressComputerTimeText(void);

  virtual bool ReadRemaining(void);
  virtual bool SetText(void);

  virtual void SetText(NFmiString theText);
  int ConvertDefText(NFmiString& object);

#ifdef OLD_MSC
  using NFmiPressTimeText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  virtual bool WritePS(FmiPressOutputMode theOutput);

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
 *�\param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressComputerTimeText::SetText(NFmiString theText)
{
  NFmiPressTimeText::SetText(theText);
}

#endif  // NFMIPRESSCOMPUTERTIMETEXT_H

// ======================================================================
