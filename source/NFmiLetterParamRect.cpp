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

  itsFont = NFmiString("Helvetica");
  itsAlignment = kCenter;

  itsMultiMapping = 0;
  itsModifier = kNoneModifier;

  // kopsattu vain numerosta,jotta oletussuht.paikka = (0,0) toimii 
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
		case dAlignment:
		  {
			if (!ReadEqualChar())
			  break;
			
			*itsDescriptionFile >> itsObject;
			itsString = itsObject;

			if (itsString == NFmiString ("Center") ||
				itsString == NFmiString ("KeskiPiste") ||
				itsString == NFmiString ("Keski"))
			  {
				itsAlignment = kCenter;
			  }
			else if (itsString == NFmiString ("Right") ||
					 itsString == NFmiString ("OikeaLaita") ||
					 itsString == NFmiString ("Oikea"))
			  {
				itsAlignment = kRight;
			  }
			else if (itsString == NFmiString ("Left") ||
					 itsString == NFmiString ("VasenLaita") ||
					 itsString == NFmiString ("Vasen"))
			  {
				itsAlignment = kLeft;
			  }
			else
				*itsLogFile << "*** ERROR: Tuntematon kohdistus #Tekstiss‰: "
							<< static_cast<char *>(itsObject)
							<< endl;  

			ReadNext();
			break;
		  }
		case dFont:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;
			itsFont = itsObject;
			if (itsFont == NFmiString("ZapfDingbats"))
			  fUseSelectLatinFont = false;

			ReadNext();
			break;
		  }
		case dStyle:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;
			itsStyle = itsObject;
			
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
			ReadNext();
			break;
		  }
		case dParamSize:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {           
				itsRelRect.Inflate(-(c40-r1)/(c40*2));
			  }
			
			ReadNext();
			break;
		  }
		case dProducer: 
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadLong(long1))
			  long1 = 0; //dummy
			
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
  return NFmiTextParamRect::ConvertDefText(object);
	
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param AbsoluteRectOfSymbolGroup Undocumented
 * \param theQI Undocumented
 * \param theDestinationFile Undocumented
 * \param theOutput Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiLetterParamRect::WritePS(const NFmiRect & AbsoluteRectOfSymbolGroup,
								  NFmiFastQueryInfo * theQI,
								  ofstream & theDestinationFile,
								  FmiPressOutputMode theOutput)
{
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

  if(!fMarkingValue)
	  return true;

  MapColor();
  NFmiString * mapString;

  if (itsMultiMapping)
	{
	  bool missingFound;
	  mapString = itsMultiMapping->Map(itsCurrentParamArray, missingFound);
	  if(missingFound)
		{
		  itsNumOfMissing++;
		  return false;
		}
	}
  else 
	mapString = itsMapping->Map(value); 

  if (itsPressParam->IsDistanceCheck() && GetOrder() <= 1)
	{
	  float value = itsCurrentParamValue;
	  if(itsMultiMapping)
		  value = itsCurrentParamArray[0];

		if (!itsPressParam->CheckAndSetDistance(static_cast<long>(FmiRound(value)), AbsoluteRectOfSymbolGroup.Place()))
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
					   AbsoluteRectOfSymbolGroup, 
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
