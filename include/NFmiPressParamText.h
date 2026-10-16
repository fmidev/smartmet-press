// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressParamText
 */
// ======================================================================
/*!
 * \class NFmiPressParamText
 *
 * Kirjoittaa (current) parametrinimen ps-oliona output-tiedostoon.
 * Ainoastaan ylitetty PressText:n WritePs-metodi. Toinen emo on
 * NFmiPressDataObject, jossa parametri.
 *
 */
// ======================================================================

#ifndef NFMIPRESSPARAMTEXT_H
#define NFMIPRESSPARAMTEXT_H

#include "NFmiPressDataObject.h"
#include "NFmiPressText.h"

//! Undocumented
class NFmiPressParamText : public NFmiPressText, public NFmiPressDataObject
{
 public:
  ~NFmiPressParamText() override;

  NFmiPressParamText();
  NFmiPressParamText(const NFmiPressText& theTextParamRect);

#ifdef OLD_MSC
  using NFmiPressText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  bool WritePS(FmiPressOutputMode theOutput) override;
  virtual bool IsDataObject();

};  // class NFmiPressParamText

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiPressParamText::~NFmiPressParamText() = default;
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressParamText::NFmiPressParamText() = default;
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParamText::IsDataObject()
{
  return true;
}
#endif  // NFMIPRESSPARAMTEXT_H

// ======================================================================
