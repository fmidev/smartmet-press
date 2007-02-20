// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiSubstituteParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiSubstituteParamRect.h"
#include "NFmiPressParam.h"
#include "NFmiPressProduct.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiSubstituteParamRect::~NFmiSubstituteParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theSubstituteParamRect Undocumented
 */
// ----------------------------------------------------------------------

NFmiSubstituteParamRect::NFmiSubstituteParamRect(const NFmiSubstituteParamRect & theSubstituteParamRect)
  : NFmiTextParamRect(theSubstituteParamRect)
  , itsSubstituteMappingValue(theSubstituteParamRect.itsSubstituteMappingValue)

{
}

// ----------------------------------------------------------------------
/*!
 * Clone the object
 *
 * \result A new copy of this
 * \todo Should return an auto_ptr
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmiSubstituteParamRect::Clone(void) const
{
  return new NFmiSubstituteParamRect(*this);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSubstituteParamRect::ReadDescription(NFmiString & retString)
{			 
  NFmiString helpString;
  NFmiValueString helpValueString;
  long long1;
  double r1;//,r2,r3,r4;

  itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));
  itsMapping = new NFmiParamMapping;

  SetPreReadingTimes();

  itsMultiMapping = 0;
  itsModifier = kNoneModifier;
  // bool relPlace = false;

  itsRelRect -= NFmiPoint(1., 1.);
  ReadNext();
	
  while(itsIntObject != 9999 || itsCommentLevel) 
	{
	  if (itsIntObject != dEndComment && itsCommentLevel)
		itsIntObject = dComment; 

	  if(itsLoopNum > itsMaxLoopNum)  
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Tekstisss‰" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:	  
		  {    
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #Tekstisss‰: "
						  << static_cast<char *>(itsObject)
						  << endl;  
			ReadNext();
			break;
		  }
		case dComment:	  
		  {
			ReadNext();
			break;
		  }
		case dEndComment:	  
		  {
			ReadNext();
			break;
		  }
		  
		case dRelDay:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadLong(long1))
			  itsFirstDeltaDays = static_cast<unsigned short>(long1+ itsEnvironment.GetDayAdvance());

			ReadNext();
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: P‰ivi‰ ei voi asettaa #Tekstiss‰"
						  << endl;  
			break;
		  }
		case dHour:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadLong(long1))
			  itsFirstPlotHours = static_cast<unsigned short>(long1);
			
			ReadNext();
			break;
		  }
		case dMappingSubstituteValue:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadOne(r1))
			   itsSubstituteMappingValue = static_cast<float>(r1);

			ReadNext();
			break;
		  }
		default:
		  {
			ReadRemaining();  
			break;
		  }
		}
	} //while
	
  //flush viimeinen takaisin streamiin! Miten?
  SetPostReadingTimes();

  Set(NFmiDataIdent(NFmiParam(itsIdentPar),240), NFmiRect(itsRelRect));

  retString = itsString;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

int NFmiSubstituteParamRect::ConvertDefText(NFmiString & object)
{
	NFmiString lowChar = object;
	lowChar.LowerCase();
	
	if(lowChar==NFmiString("mappingvaluetosubstitute") ||
		lowChar==NFmiString("korvattavamuunnosarvo"))
		return dMappingSubstituteValue;
	else 
	    return NFmiTextParamRect::ConvertDefText(object);	
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theAbsoluteRectOfSymbolGroup Undocumented
 * \param theQI Undocumented
 * \param theDestinationFile Undocumented
 * \param theOutput Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSubstituteParamRect::WritePS(const NFmiRect & theAbsoluteRectOfSymbolGroup,
								  NFmiFastQueryInfo * theQI,
								  ofstream & theDestinationFile,
								  FmiPressOutputMode theOutput)
{
  bool lastElementStatus = GetPressProduct()->LastTextStatus();
  GetPressProduct()->SetLastTextStatus(true);
  if(itsPressParam->IsBackupStation())
  {
	  if(lastElementStatus)
	  {
			*itsLogFile << "  vara-asemaa ei tarvita tekstielementille" << endl;
		    return true;
	  }
	  else
			*itsLogFile << "  vara-asemaa k‰ytet‰‰n tekstielementille" << endl;
  }

  itsCurrentSegmentTime = (static_cast<NFmiQueryInfo *>(theQI))->Time();
  itsCurrentTime = itsCurrentSegmentTime;

  NFmiString hString;
  float value = 0;
  
  if(!SetRelativeHour(theQI,NFmiString("#Teksti"))) 
	return isFalse;
  
  if(itsMultiMapping) 
	{
	  if(!ReadCurrentValueArray (theQI))
		return false;
	}
  else
	{
	  if(!PointOnParam(theQI, GetDataIdent().GetParam())) 
		{
		  return false;
		}
	  if(!ReadCurrentValue(theQI, itsCurrentParamValue))
		{
		  return false;        
		}
	  value = itsCurrentParamValue;
	}
	if(itsSubstituteMappingValue >0)
		itsPressParam->GetPressProduct()->AddSubstituteMappingValue(itsSubstituteMappingValue, value);
	else
		*itsLogFile << "***ERROR: #Korvaus-oliolta puuttuu KorvattavaMuunnosArvo" << endl;

	return true;
  }
// ======================================================================
