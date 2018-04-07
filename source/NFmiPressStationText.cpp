// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressStationText
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k��nt�j�n varoitusta
#endif

#include "NFmiPressStationText.h"
#include "NFmiPressParam.h"
#include "NFmiStationPoint.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressStationText::~NFmiPressStationText(void) {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRename Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// originalName avaimena:
// - vaihdetaan newName jos l�ytyy
// - tai tehd��n uusi original/new -pari

bool NFmiPressStationText::SetNewName(const NFmiRenaming &theRename)
{
  if (itsNewNames)
  {
    NFmiVoidPtrIterator iter(itsNewNames);
    iter.Reset();
    NFmiRenaming *renaming = static_cast<NFmiRenaming *>(iter.Next());
    while (renaming)
    {
      if (renaming->originalName == theRename.originalName)
      {
        renaming->newName = theRename.newName;
        return true;
      }
      renaming = static_cast<NFmiRenaming *>(iter.Next());
    }
    NFmiRenaming *newRenaming = new NFmiRenaming;
    newRenaming->originalName = theRename.originalName;
    newRenaming->newName = theRename.newName;
    itsNewNames->Add(newRenaming);
  }
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressStationText::WritePS(FmiPressOutputMode theOutput)
{
  if (!itsData) return isFalse;

  ScalePlotting();

  NFmiString str;
  str = StationName();
  if (itsPressParam->IsDayChanged())  // OPTIOKSI
    str += NFmiString("*");

  if (fAddLocalTime)
  {
    // v�h�n mutkan kautta
    if (itsData->IsGrid())
    {
      if (!fLoopErrorReported)
      {
        *itsLogFile << "  *** ERROR: cannot get local times from GRID" << endl;
        fLoopErrorReported = true;
      }
    }
    else
    {
      NFmiStationPoint stationP;
      const NFmiLocation loc(*itsData->Location());
      stationP.SetIdent(loc.GetIdent());
      NFmiString tString = stationP.LocalWmoTime(itsPressParam->GetFirstPlotTime().GetHour());
      str += (" ");
      str += ("(");
      str += tString;
      if (tString == NFmiString("-"))
        *itsLogFile << "  *** ERROR: the station " << static_cast<char *>(stationP.GetName())
                    << " has no local time" << endl;
      str += (")");
    }
  }

  SetText(str);

  return WriteString(NFmiString("StationName"), theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiPressStationText::StationName(void)
{
  NFmiString name;
  if (itsData->IsGrid())
    name = itsPressParam->GetCurrentStation().GetName();
  else if (fStationNumberMode)
    name = NFmiValueString(static_cast<long>(itsData->Location()->GetIdent()));
  else
    name = itsData->Location()->GetName();

  if (itsNewNames)
  {
    NFmiVoidPtrIterator iter(itsNewNames);
    iter.Reset();
    NFmiRenaming *renaming = static_cast<NFmiRenaming *>(iter.Next());
    while (renaming)
    {
      if (renaming->originalName == name) return renaming->newName;

      renaming = static_cast<NFmiRenaming *>(iter.Next());
    }
  }
  return name;
}

// ======================================================================
