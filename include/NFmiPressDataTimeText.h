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
class _FMI_DLL NFmiPressDataTimeText : public NFmiPressTimeText,
									   public NFmiPressDataObject
{
public:

  virtual ~NFmiPressDataTimeText(void);
  NFmiPressDataTimeText(void);

  bool ReadRemaining(void);
  int ConvertDefText(NFmiString & object);
		
  virtual bool WritePS(FmiPressOutputMode theOutput);
  virtual bool WritePSUpdatingSubText(FmiPressOutputMode theOutput);
  virtual unsigned long ClassId(void);

protected:

  bool fUseOriginTime;
		
}; // class NFmiPressDataTimeText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressDataTimeText::NFmiPressDataTimeText(void)
  :  NFmiPressTimeText()
  , NFmiPressDataObject()
  , fUseOriginTime(false)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
unsigned long NFmiPressDataTimeText::ClassId(void)
{
  return kNFmiPressDataTimeText;
}

#endif // NFMIPRESSDATATIMETEXT_H

// ======================================================================
