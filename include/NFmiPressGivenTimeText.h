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
class _FMI_DLL NFmiPressGivenTimeText : public NFmiPressTimeText
{
public:

  virtual ~NFmiPressGivenTimeText(void);
  NFmiPressGivenTimeText(void);

  virtual bool ReadRemaining(void);  
  using NFmiPressScaling::WritePS;
  virtual bool WritePS(FmiPressOutputMode theOutput); 
  virtual bool SetText(void);
  virtual void SetText(NFmiString theText);
  void SetTime(const NFmiMetTime & theTime);

}; // class NFmiPressGivenTimeText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressGivenTimeText::NFmiPressGivenTimeText(void)
  : NFmiPressTimeText()
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressGivenTimeText::SetText(NFmiString theText)   
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

inline
void NFmiPressGivenTimeText::SetTime(const NFmiMetTime & theTime)
{
  itsFirstPlotTime=theTime;
}

#endif // NFMIPRESSGIVENTIMETEXT_H

// ======================================================================
