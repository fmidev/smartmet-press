// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressStationText
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressStationText.h"
#include "NFmiStationPoint.h"
#include "NFmiPressParam.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressStationText::~NFmiPressStationText(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRename Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// originalName avaimena:
// - vaihdetaan newName jos löytyy
// - tai tehdään uusi original/new -pari

bool NFmiPressStationText::SetNewName(const NFmiRenaming & theRename)
{
  if(itsNewNames)
	{
	  NFmiVoidPtrIterator iter(itsNewNames);
	  iter.Reset();
	  NFmiRenaming * renaming = static_cast<NFmiRenaming *>(iter.Next());
	  while (renaming)
		{
		  if(renaming->originalName == theRename.originalName)
			{
			  renaming->newName = theRename.newName;
			  return true;
			}
		  renaming = static_cast<NFmiRenaming *>(iter.Next());
		}
	  NFmiRenaming * newRenaming = new NFmiRenaming;
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
  if(!itsData)
	return isFalse;

  ScalePlotting();

  NFmiString str;
  str = StationName();

  if(fAddLocalTime)
	{
	  // vähän mutkan kautta
	  if(itsData->IsGrid())
		{
		  if(!fLoopErrorReported)
			{
			  *itsLogFile << "  *** ERROR: gridistä ei saa paikallisia aikoja"
						  << endl;
			  fLoopErrorReported = true;
			}
		}
	  else
		{
		  NFmiStationPoint stationP;
		  const NFmiLocation loc(*itsData->Location());
		  stationP.SetIdent(loc.GetIdent());
		  NFmiString tString = stationP.LocalWmoTime(12);
		  str += (" ");
		  str += ("(");
		  str += tString;
		  if(tString == NFmiString("-"))
			*itsLogFile << "  *** ERROR: asemalta "
						<< static_cast<char *>(stationP.GetName())
						<< " puuttuu paik.aika"
						<< endl;
		  str += (")");
		}
	}

  SetText(str);

  return WriteString(NFmiString("ASEMATEKSTI"),theOutput);
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
  if(itsData->IsGrid())
	name = itsPressParam->GetCurrentStation().GetName();
  else
	name = itsData->Location()->GetName();

  if(itsNewNames)
	{
	  NFmiVoidPtrIterator iter(itsNewNames);
	  iter.Reset();
	  NFmiRenaming* renaming = static_cast<NFmiRenaming *>(iter.Next());
	  while (renaming)
		{
		  if(renaming->originalName == name)
			return renaming->newName;

		  renaming = static_cast<NFmiRenaming *>(iter.Next());
		}
	}
  return name;
}

// ======================================================================
