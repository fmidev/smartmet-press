// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressParamText
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressParamText.h"
#include "NFmiFastQueryInfo.h"

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParamText::WritePS(FmiPressOutputMode theOutput)
{
  ScalePlotting();
  
  SetText(itsData->Param().GetParamName());
  
  return WriteString(NFmiString("PARAMETRITEKSTI"), theOutput);
}

// ======================================================================

