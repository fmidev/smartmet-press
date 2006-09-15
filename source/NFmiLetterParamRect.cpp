// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiLetterParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiLetterParamRect.h"
#include "NFmiPressParam.h"
#include "NFmiPressProduct.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiLetterParamRect::~NFmiLetterParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theLetterParamRect Undocumented
 */
// ----------------------------------------------------------------------

NFmiLetterParamRect::NFmiLetterParamRect(const NFmiLetterParamRect & theLetterParamRect)
  : NFmiTextParamRect(theLetterParamRect)
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

NFmiParamRect * NFmiLetterParamRect::Clone(void) const
{
  return new NFmiLetterParamRect(*this);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiLetterParamRect::ReadDescription(NFmiString & retString)
{			 
  NFmiString helpString;
  NFmiValueString helpValueString;
  long long1;
  double r1,r2,r3,r4;

  itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));
  itsMapping = new NFmiParamMapping;

  SetPreReadingTimes();

  itsMultiMapping = 0;
  itsModifier = kNoneModifier;
  bool relPlace = false;

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
		case dPlaceMove:
		  {
			if (!ReadEqualChar())
			  break;
			
			if(Read2Double(r1,r2))
			{ 
			  itsRelRect += NFmiPoint(r1/c40, r2/c40);
			}
			
			ReadNext();
			break;
		  }
		case dRelPlace:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(r1,r2,r3,r4))
			  {
				itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
			  }
			relPlace = true;
			ReadNext();
			break;
		  }

		case dColorValueDependent: 
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
		default:
		  {
			ReadRemaining();  
			break;
		  }
		}
	} //while
	
  //flush viimeinen takaisin streamiin! Miten?
  SetPostReadingTimes();

  if(!relPlace)
	  itsRelRect.Inflate(-(c40 - GetTextSize())/(c40*2));
  
  if(fNewScaling)
	itsRelRect += NFmiPoint(1.,1.);
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

int NFmiLetterParamRect::ConvertDefText(NFmiString & object)
{
  //NFmiString lowChar = object;
  //lowChar.LowerCase();

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

bool NFmiLetterParamRect::WritePS(const NFmiRect & theAbsoluteRectOfSymbolGroup,
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

  if(!ActiveTimeIndex(itsPressParam->GetCurrentStep()))
   {
	 if(itsLoopActivity.bypassWithValue)
	   value = itsLoopActivity.bypassValue;
	 else
	   return true;
   }
	if(value == kFloatMissing)
	  itsPressParam->GetPressProduct()->SetLastTextStatus(false);
	else
	  itsPressParam->GetPressProduct()->SetLastTextStatus(true);

  if(!fMarkingValue)
	  return true;

  MapColor();
  NFmiString * mapString;

  if (itsMultiMapping)
	{
	  bool missingFound;
	  CompleteMultiMapping();

	  mapString = itsMultiMapping->Map(itsCurrentParamArray, missingFound);
	  if(missingFound)
		{
		  itsNumOfMissing++;
		  if(!mapString)
		  {
			if(!itsMissingString)
			{
              GetPressProduct()->SetLastTextStatus(false);
			  return false;
			}
			else
				mapString = itsMissingString;
		  }
		}
	}
  else
  {
	mapString = itsMapping->Map(value);
	if(mapString == 0 && itsMissingString != 0)
		mapString = itsMissingString;
  }

  if (itsPressParam->IsDistanceCheck() && GetOrder() <= 1)
	{
	  float keyValue = itsCurrentParamValue;
	  if(itsMultiMapping)
		  keyValue = itsCurrentParamArray[0];

		if (!itsPressParam->CheckAndSetDistance(static_cast<long>(FmiRound(keyValue)), theAbsoluteRectOfSymbolGroup.Place()))
			return false;
	}

  if(mapString && !(*mapString == NFmiString("None"))) 
	{
	  NFmiString str;  

	  hString = NFmiString (*mapString);	// pit‰isi olla yleisempi
	  if(itsEnvironment.GetLongNumberMinus() && hString == NFmiString("-"))
		hString = NFmiString("\\226");
	  str += hString;

	  return WriteCode(Construct(&str),
					   theAbsoluteRectOfSymbolGroup, 
					   theDestinationFile,
					   NFmiString("TEKSTI datasta"),
					   theOutput); 
	}
  else if(!mapString)
	{
	  *itsLogFile << "WARNING: arvolle "
				  << value 
				  << " ei muunnosta (#Teksti datasta)"
				  << endl;
      GetPressProduct()->SetLastTextStatus(false);
	  return isFalse;
	}
  else
	{
	  return isFalse;
	}
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \result Undocumented
 */
// ----------------------------------------------------------------------
FmiGenericColor NFmiLetterParamRect::MapColor(void) const 
{
  for(unsigned long ind=0; ind < itsCurrentNumOfColMaps; ind++)
	{
	  if(itsCurrentParamValue >= itsColorMapping[ind].minValue &&
		 itsCurrentParamValue <= itsColorMapping[ind].maxValue)
		{
		  return itsColorMapping[ind].color;
		}
	}
  return GetColor();
}

// ======================================================================
