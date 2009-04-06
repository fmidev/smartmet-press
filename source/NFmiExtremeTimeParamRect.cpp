// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiExtremeTimeParamRect
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiExtremeTimeParamRect.h"
#include "NFmiPressParam.h"

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiExtremeTimeParamRect::~NFmiExtremeTimeParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theExtremeTimeParamRect Undocumented
 * \todo Remove the implementation, let the compiled do it
 */
// ----------------------------------------------------------------------

NFmiExtremeTimeParamRect::NFmiExtremeTimeParamRect(const NFmiExtremeTimeParamRect & theExtremeTimeParamRect)
  : NFmiTimeParamRect(theExtremeTimeParamRect)
{
}

// ----------------------------------------------------------------------
/*!
 * Clone the object
 *
 * \return Clone of this
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmiExtremeTimeParamRect::Clone(void) const
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

NFmiTime NFmiExtremeTimeParamRect::TimeToWrite(NFmiFastQueryInfo * theQI)
{
  return itsPressParam->GetOptionTime();
}

// ======================================================================
