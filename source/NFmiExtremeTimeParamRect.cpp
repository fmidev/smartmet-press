// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiExtremeTimeParamRect
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiExtremeTimeParamRect.h"
#include "NFmiPressParam.h"

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiExtremeTimeParamRect::~NFmiExtremeTimeParamRect(void) = default;
// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theExtremeTimeParamRect Undocumented
 * \todo Remove the implementation, let the compiled do it
 */
// ----------------------------------------------------------------------

NFmiExtremeTimeParamRect::NFmiExtremeTimeParamRect(
    const NFmiExtremeTimeParamRect& theExtremeTimeParamRect) = default;

// ----------------------------------------------------------------------
/*!
 * Clone the object
 *
 * \return Clone of this
 */
// ----------------------------------------------------------------------

NFmiParamRect* NFmiExtremeTimeParamRect::Clone() const
{
  return new NFmiExtremeTimeParamRect(*this);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiTime NFmiExtremeTimeParamRect::TimeToWrite(NFmiFastQueryInfo* theQI)
{
  return itsPressParam->GetOptionTime();
}

// ======================================================================
