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
	if(itsColumnText)
		delete itsColumnText;
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
  , itsColumnText(theLetterParamRect.itsColumnText ? new NFmiPressText(*theLetterParamRect.itsColumnText) : 0)
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
		  return false;
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
		case dColumnText:
		  {
		  	//text->SetWriteLast(fMakeElementsAfterSegments);
            //text->SetEnvironment(itsEnvironment);
            //text->SetHome(GetHome());
		    //text->SetLogFile(itsLogFile);
			//text->SetDescriptionFile(itsDescriptionFile);
			//text->SetLanguage(itsLanguage);

			itsColumnText = new NFmiPressText;
            itsColumnText->SetEnvironment(itsEnvironment);
			itsColumnText->SetHome(GetHome());
			itsColumnText->SetLogFile(itsLogFile);
			itsColumnText->SetDescriptionFile(itsDescriptionFile);
		    itsColumnText->SetFont(GetFont());
			//itsColumnText->SetSize(itsRelRect);
			if(!itsColumnText->ReadDescription(itsString))
			{
			  delete itsColumnText;
			  itsColumnText = 0;
			}
			
			itsIntObject = ConvertDefText(itsString);
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
  NFmiString lowChar = object;
  lowChar.LowerCase();
  if    (lowChar == "#columntext"
	  || lowChar == "#palstateksti")
	  	return dColumnText;
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

  bool fMaxMinPlotting = true;
  bool isMax;
  if(fMaxMinPlotting)
  {
	itsPressParam->SetMaxMinPoints(); //vain ekalla kerralla
	if(!itsPressParam->IsMaxMin(isMax))
		return true;
  }


  itsCurrentSegmentTime = (static_cast<NFmiQueryInfo *>(theQI))->Time();
  itsCurrentTime = itsCurrentSegmentTime;

  NFmiString hString;
  float value = 0;
  
  if(!SetRelativeHour(theQI,NFmiString("#Teksti"))) 
	return false;
  
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
	  if(mapString)
		  ModifyTextBySeason(*mapString);
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
    if(mapString)
		ModifyTextBySeason(*mapString);
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

	  if(itsColumnText)
	  {
		    itsColumnText->SetText(str);
//		    itsColumnText->SetFont(GetFont());
			//itsColumnText->SetRect(theAbsoluteRectOfSymbolGroup);
//            itsColumnText->SetSize(itsRelativeRectOfSymbolGroup.Size());
  //bool SetScale(const NFmiRectScale & theRectScale);
			itsColumnText->SetFile(theDestinationFile);
			// return itsColumnText->WritePS(theAbsoluteRectOfSymbolGroup.Place(),theOutput);
			//return itsColumnText->WritePS(theOutput);
			// OK return itsColumnText->WritePS(NFmiPoint(100,50), theOutput);
		      return itsColumnText->WritePS(theAbsoluteRectOfSymbolGroup.Place(), theOutput);

	  }
	  else
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
	  return false;
	}
  else
	{
	  return false;
	}
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 * Vuodenajasta riippuen vaihdetaan joitakin sanoja, esim l‰mmint‰ -> lauhaa
 * Suomenkieli pit‰‰ pysy‰ 22 merkiss‰, k‰ytet‰‰n Vartissa, Kellokkaassa, Seiskassa.
 * Ruotsinkieli voi olla v‰h‰n pidempi, HBL:n takasivu = kaksi lyhytt‰ rivi‰
 *
 */
// ----------------------------------------------------------------------

bool NFmiLetterParamRect::ModifyTextBySeason(NFmiString & theString)
{
	NFmiStaticTime time;
	short mon = time.GetMonth();
	string stdString(theString);
	string::size_type startInd;

	startInd = stdString.find("l‰mmint‰");
    if(startInd != string::npos)
	{
		if(mon > 10 || mon <4)
//		if(mon > 4)
		{
			stdString.replace(startInd, 8, "lauhaa");
		}
	}

	startInd = stdString.find("varmt");
    if(startInd != string::npos)
	{
		if(mon > 10 || mon <4)
		{
			stdString.replace(startInd, 5, "milt");
		}
	}

	startInd = stdString.find("kylm‰‰");
    if(startInd != string::npos)
	{
		if(mon > 4 && mon <9)
		{
			stdString.replace(startInd, 6, "koleaa");
		}
	}

	startInd = stdString.find("kallt");
    if(startInd != string::npos)
	{
		if(mon > 4 && mon <9)
		{
			stdString.replace(startInd, 5, "kyligt");
		}
	}

	startInd = stdString.find("syyss‰‰t‰");
    if(startInd != string::npos)
	{
		if(mon == 12 || mon <3)
	//	if(mon >3)
			stdString.replace(startInd, 4, "talvi");
		else if(mon == 4)
			stdString.replace(startInd, 4, "kev‰t");
		else if(mon >=6 && mon <9)
			stdString.replace(startInd, 4, "kes‰");
		else if(mon == 11 || mon == 3 || mon==5)
			stdString.replace(startInd, 4, "pouta");
	}

	startInd = stdString.find("hˆstv‰der");
    if(startInd != string::npos)
	{
		if(mon == 12 || mon <3)
			stdString.replace(startInd, 4, "vinter");
		else if(mon == 4)
			stdString.replace(startInd, 4, "vÂr");
		else if(mon >=6 && mon <9)
			stdString.replace(startInd,4, "sommar");
		else if(mon == 11 || mon == 3 || mon==5)
			stdString.replace(startInd, 4, "uppehÂlls");
	}
	startInd = stdString.find("kallt_fˆr_Ârstiden");
    if(startInd != string::npos)
	{
		if(mon == 12 || mon == 1 || mon == 2)
			stdString.replace(startInd, 18, "kallt");
	}
	startInd = stdString.find("varmt_fˆr_Ârstiden");
    if(startInd != string::npos)
	{
		if(mon >= 6 || mon <= 8)
			stdString.replace(startInd, 18, "varmt");
	}

	theString = stdString;
	return true;
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
