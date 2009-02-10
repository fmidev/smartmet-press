// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressNameDay
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressNameDay.h"
#include "NFmiSettings.h"
#include <iostream>
using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressNameDay::~NFmiPressNameDay(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressNameDay::ReadRemaining(void)  
{
  unsigned long long1;
  FmiCounter counter;
  switch(itsIntObject)
	{	
	case dMaxLength:   
	  {
		if (!ReadEqualChar())
		  break;
		
		if (ReadOne(counter))
		  {
			itsMaxLength = counter;
		  }
		
		ReadNext();
		break;
	  }
	case dMaxNumber:    
	  {
		if (!ReadEqualChar())
		  break;
		
		if (ReadOne(counter))
		  {
			itsMaxNumber = counter;
		  }
		
		ReadNext();
		break;
	  }
	case dRelDay:     
	  {
		if (!ReadEqualChar())
		  break;
		
		if(ReadOne(long1))
		  {			
			itsFirstPlotTime = NFmiMetTime();
			itsFirstPlotTime.ChangeByDays(long1+ itsEnvironment.GetDayAdvance());
		  }
		
		ReadNext();
		break;
	  }

	default:
	  {
		NFmiPressText::ReadRemaining();
		break;
	  }
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiPressNameDay::ConvertDefText(NFmiString & object)
{
  if(object==NFmiString("MaxNumber") || object==NFmiString("MaksimiMäärä"))
	return dMaxNumber;
  else if(object==NFmiString("MaxLength") || object==NFmiString("MaksimiPituus"))
	return dMaxLength;
  else
	return NFmiPressText::ConvertDefText(object);
  
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressNameDay::WritePS(FmiPressOutputMode theOutput)
{
  if(!itsNameDay)
	{
	  *itsLogFile << "*** ERROR: NimiPäiväOlio puuttuu, ohjelmointivirhe"
				  << endl;
	  return false;
	}
  ScalePlotting();
  
  NFmiString str;
  
  if(!itsNameDay->IsRead())
	{
#ifndef UNIX
	  NFmiString fileName = GetHome();
	  fileName += kFmiDirectorySeparator;
	  fileName += NFmiString("Muut");
	  fileName += kFmiDirectorySeparator;
#else
	  NFmiString fileName = static_cast<NFmiString>(NFmiSettings::Require<string>("press::incpath"));
	  fileName += "/";
#endif
	  if(itsLanguage == kFinnish)
		{
		  fileName += NFmiString("nimipäivät.txt"); 
		}
	  else
		{
		  fileName += NFmiString("nimipäivätRuotsi.txt"); 
		}
	  if(!itsNameDay->ReadFile(fileName))
		{
		  *itsLogFile << "*** ERROR: Nimipäivien lukeminen epäonnistui" << endl;
		  return false; 
		}
	}
  
  if(itsNameDay->IsValue())
	{
	  str = itsNameDay->GetName(itsFirstPlotTime, itsMaxNumber, itsMaxLength);
	  SetText(str);
	  return WriteString(NFmiString("NIMIPÄIVÄ"), theOutput);
	}
  else
	return false;
}

// ======================================================================

