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
class _FMI_DLL NFmiPressParamText : public NFmiPressText,
									public NFmiPressDataObject
{
public:

  virtual ~NFmiPressParamText(void);

  NFmiPressParamText(void);
  NFmiPressParamText(const NFmiPressText & theTextParamRect); 

  virtual bool WritePS(FmiPressOutputMode theOutput);  
  virtual bool IsDataObject(void);
      
}; // class NFmiPressParamText

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline
NFmiPressParamText::~NFmiPressParamText(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressParamText::NFmiPressParamText(void)
  :  NFmiPressText()
  , NFmiPressDataObject()
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
bool NFmiPressParamText::IsDataObject(void)
{
  return true;
}

#endif // NFMIPRESSPARAMTEXT_H

// ======================================================================
