// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmi2SymbolParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmi2SymbolParamRect.h"

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmi2SymbolParamRect::~NFmi2SymbolParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theSymbolRect The object beging copied
 */
// ----------------------------------------------------------------------

NFmi2SymbolParamRect::NFmi2SymbolParamRect(const NFmi2SymbolParamRect & theSymbolRect)
  : NFmiSymbolParamRect(theSymbolRect)
{
  itsSecondDataIdent = theSymbolRect.itsSecondDataIdent;    
}


// ----------------------------------------------------------------------
/*!
 * Clone the object
 *
 * \return The clone of the object
 * \todo Should return an auto_ptr to indicate transfer of ownership
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmi2SymbolParamRect::Clone(void) const
{
  return new NFmi2SymbolParamRect(*this);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 * \todo Why is there a break after a return in the default switch?
 */
// ----------------------------------------------------------------------

bool NFmi2SymbolParamRect::ReadRemaining(void)  
{
  long long1;

  switch(itsIntObject)
	{
	case dSecondPar:	  
	  {
		if (!ReadEqualChar())
		  break;
		
		if(ReadLong(long1))
		  itsSecondDataIdent.SetParam(NFmiParam(long1));
		
		ReadNext();
		break;
	  }

	default: 
	  {
		return NFmiSymbolParamRect::ReadRemaining();  
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

int NFmi2SymbolParamRect::ConvertDefText(NFmiString & object)  
{
  NFmiString lowChar = object;

  // kaikille pit‰isi sallia vapaa isot/pienet kirj.
  lowChar.LowerCase();

  if(lowChar==NFmiString("secondparameter") ||
	 lowChar==NFmiString("toinenparametri"))
	return dSecondPar;
  else
	return NFmiSymbolParamRect::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \param hearDummy Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmi2SymbolParamRect::ReadValues(NFmiFastQueryInfo * theQI, bool hearDummy)
{
  if(!PointOnParam(theQI, GetSecondDataIdent().GetParam()) ||
	 !PointOnLevel(theQI))
	return false;
   
  // Vain tunnit voi muuttaa t‰ll‰ tasolla, segmentin aikaluuppi
  // menee muuten sekaisin

  if(!SetRelativeHour(theQI,NFmiString("K‰‰ntyv‰Symboli")))
	return isFalse;

  FloatValue(theQI, itsSecondParamValue); 

  // ei SetRelativeHour toistamiseen
  return NFmiSymbolParamRect::ReadValues(theQI, false);
}

// ======================================================================
