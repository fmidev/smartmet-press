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
class _FMI_DLL NFmiPressComputerTimeText : public NFmiPressTimeText
{
public:

  virtual ~NFmiPressComputerTimeText(void);
  NFmiPressComputerTimeText(void);

  virtual bool ReadRemaining(void);
  virtual bool SetText(void);

  virtual void SetText(NFmiString theText);
  int ConvertDefText(NFmiString & object);

  using NFmiPressScaling::WritePS;
  virtual bool WritePS(FmiPressOutputMode theOutput);
       
protected:

  short itsDeltaDays;

}; // class NFmiPressComputerTimeText


// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressComputerTimeText::NFmiPressComputerTimeText(void)
  : NFmiPressTimeText()
  , itsDeltaDays(0)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param theText Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressComputerTimeText::SetText(NFmiString theText)
{
  NFmiPressTimeText::SetText(theText);
}

#endif // NFMIPRESSCOMPUTERTIMETEXT_H

// ======================================================================
