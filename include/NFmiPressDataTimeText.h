// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressDataTimeText
 */
// ======================================================================
/*!
 * \class NFmiPressDataTimeText
 *
 * Luokka tuottaa datan aktuellin ajan ps-oliona output-
 * tiedostoon. Emo on abstrakti NFmiPressTimeText ja sisaruksia
 * NFmiPressDataTimeText ja NFmiPressComputerTimeText.
 *
 */
// ======================================================================

#ifndef NFMIPRESSDATATIMETEXT_H
#define NFMIPRESSDATATIMETEXT_H

#include "NFmiPressDataObject.h"
#include "NFmiPressTimeText.h"
#include "NFmiStationPoint.h"

//! Undocumented
enum NFmiPressDataTimeTextObjects
{
  dUseOriginTime = 600
};

//! Undocumented
class NFmiPressDataTimeText : public NFmiPressTimeText, public NFmiPressDataObject
{
 public:
  ~NFmiPressDataTimeText(void) override;
  NFmiPressDataTimeText(void);

  bool ReadRemaining(void) override;
  int ConvertDefText(NFmiString& object) override;

#ifdef OLD_MSC
  using NFmiPressTimeText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  bool WritePS(FmiPressOutputMode theOutput) override;
  bool WritePSUpdatingSubText(FmiPressOutputMode theOutput) override;
  unsigned long ClassId(void) override;

 protected:
  bool fUseOriginTime;

};  // class NFmiPressDataTimeText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressDataTimeText::NFmiPressDataTimeText(void)
    : NFmiPressTimeText(), NFmiPressDataObject(), fUseOriginTime(false)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline unsigned long NFmiPressDataTimeText::ClassId(void)
{
  return kNFmiPressDataTimeText;
}
#endif  // NFMIPRESSDATATIMETEXT_H

// ======================================================================
