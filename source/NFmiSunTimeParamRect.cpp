// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiSunTimeParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k��nt�j�n varoitusta
#endif

#include "NFmiSunTimeParamRect.h"
#include <iostream>

using namespace std;


// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiSunTimeParamRect::~NFmiSunTimeParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theSunTimeParamRect The object being copied
 */
// ----------------------------------------------------------------------

NFmiSunTimeParamRect::NFmiSunTimeParamRect(const NFmiSunTimeParamRect & theSunTimeParamRect)
  : NFmiTimeParamRect(theSunTimeParamRect)
  , fIsSunRise(theSunTimeParamRect.fIsSunRise)
{
}

// ----------------------------------------------------------------------
/*!
 * Create a copy of this
 *
 * \return The clone
 * \todo Should return an auto_ptr
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmiSunTimeParamRect::Clone(void) const
{
  return new NFmiSunTimeParamRect(*this);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSunTimeParamRect::ReadRemaining(void) 
{			 
  switch(itsIntObject)
	{
	case dSunRise:	  
	  {
		fIsSunRise = true;
		ReadNext();
		
		break;
	  }
	case dSunSet:	  
	  {
		fIsSunRise = false;
		ReadNext();
		
		break;
	  }
	default: 
	  {
		return NFmiTimeParamRect::ReadRemaining();  
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

int NFmiSunTimeParamRect::ConvertDefText(NFmiString & object) 
{
  if(object==NFmiString("SunRise") ||
	 object==NFmiString("AuringonNousu") ||
	 object==NFmiString("Auringonnousu") ||
	 object==NFmiString("Nousu"))
	return dSunRise;

  else if(object==NFmiString("SunSet") ||
		  object==NFmiString("AuringonLasku") ||
		  object==NFmiString("Auringonlasku") ||
		  object==NFmiString("Lasku")) 
	return dSunSet;

  else
	return NFmiTimeParamRect::ConvertDefText(object);
	
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \return Undocumented
 */
//----------------------------------------------------------------------

//aina local time piirtoalkioissa (vrt segmentti ja tuote miss� ei)
NFmiTime NFmiSunTimeParamRect::TimeToWrite(NFmiFastQueryInfo * theQI) 
{
  NFmiMetTime tim = itsCurrentSegmentTime; 
		
  double latitude = itsStationPoint.GetLatitude();
  double longitude = itsStationPoint.GetLongitude();
  if(fabs(longitude) <= .001 && fabs(latitude) <= .001) //ei pit�isi voida tapahtua
	*itsLogFile << "*** ERROR: aurinkoajan longitudi puuttuu "
				<< static_cast<char *>(itsStationPoint.GetName())
				<< ":lta paikalliseen aikaan"
				<< endl;
		
  if(fIsSunRise)
	{
	  // fIsValidTime; false jos ei laske/nouse
	  return itsStationPoint.TimeOfSunrise(tim, static_cast<bool &>(fIsValidTime));
	}
  else
	{
	  return itsStationPoint.TimeOfSunset(tim, static_cast<bool &>(fIsValidTime));
	}
}

// ======================================================================
