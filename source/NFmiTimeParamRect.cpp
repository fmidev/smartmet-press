// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiTimeParamRect
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressProduct.h"
#include "NFmiTimeParamRect.h"
#include "NFmiPressParam.h"
#include "NFmiLocationFinder.h" 
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiTimeParamRect::~NFmiTimeParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theTimeParamRect The object being copied
 */
// ----------------------------------------------------------------------

NFmiTimeParamRect::NFmiTimeParamRect(const NFmiTimeParamRect & theTimeParamRect)
  : NFmiTextParamRect(theTimeParamRect)
  , itsFormat(theTimeParamRect.itsFormat)
  , itsOrigFormat(theTimeParamRect.itsOrigFormat)
  , itsOrigFont(theTimeParamRect.itsOrigFont)
  , itsStationPoint(theTimeParamRect.itsStationPoint)
  , fIsValidTime(theTimeParamRect.fIsValidTime)
  , itsLanguage(theTimeParamRect.itsLanguage)
  , fFinnishTimezone(theTimeParamRect.fFinnishTimezone)
{
}

// ----------------------------------------------------------------------
/*!
 * Make a clone of this
 *
 * \return The clone
 * \todo Should return an auto_ptr
 * \todo The static_cast is unnecessary
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmiTimeParamRect::Clone(void) const
{
  return static_cast<NFmiParamRect *>(new NFmiTimeParamRect(*this));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param newLanguage Undocumented
 */
// ----------------------------------------------------------------------

void NFmiTimeParamRect:: SetLanguage(FmiLanguage newLanguage)
{
  if(newLanguage == kChinese && (itsOrigFormat == kShortWeekDay || itsOrigFormat == kI))
	{
	  itsFormat = kI;
	  SetFont("Cviikko");
	}
  else if(itsLanguage == kChinese) //mutta uusi ei
	{
	  itsFormat = itsOrigFormat;
	  SetFont(itsOrigFont);
	}
  itsLanguage = newLanguage;

}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiTimeParamRect::ReadDescription(NFmiString & retString)
{
  NFmiString helpString;
  NFmiValueString helpValueString;
  long long1;
  double r1,r2,r3,r4;
  itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));

  SetPreReadingTimes();

  ReadNext();

  // kopsattu vain numerosta,jotta oletussuht.paikka = (0,0) toimii
  itsRelRect -= NFmiPoint(1., 1.);
  bool relPlace = false;

  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: max file length exceeded in #Number" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Unknown word in #TimeText: "
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
		case dParamRectLanguage:
		{
		  if (!ReadEqualChar())
			break;
		  itsLanguage = ReadLanguage();
		  ReadNext();
		  break;
		}

		case dTimeFormat:
		  {
			if (!ReadEqualChar())
			  break;                   //kI

			itsFormat = ReadTimeFormat();

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

		  *itsDescriptionFile >> itsObject;
		  itsString = itsObject;
		  itsIntObject = ConvertDefText(itsString);
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: You cannot set Day in #Parameters"  << endl;
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
		case dFinnishTimezone:
		  {
			SetTrue(fFinnishTimezone);
			break;
		  }
		default:
		  {
			ReadRemaining();
			break;
		  }
		}
	} //while

  //flush viimeinen takaisin sreamiin! Miten?

  itsOrigFormat = itsFormat;
  itsOrigFont = GetFont();
  if(itsLanguage == kChinese && (itsFormat == kShortWeekDay || itsFormat == kI))
	{
	  itsFormat = kI;
	  SetFont("Cviikko");
	}

  SetPostReadingTimes();

  if(!relPlace)
	  itsRelRect.Inflate(-(c40 - GetTextSize())/(c40*2));

  if(fNewScaling)
	itsRelRect += NFmiPoint(1.,1.);
  Set(NFmiDataIdent(NFmiParam(itsIdentPar),240/*=dummy NFmiProducer(itsProducer)*/)
	  ,NFmiRect(itsRelRect));

  retString = itsString;
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

int NFmiTimeParamRect::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase(); // kaikille sallitaan vapaasti isot/pienet kirjaimet

  if(lowChar==NFmiString("format") || lowChar==NFmiString("formaatti"))
	return dTimeFormat;
  else if(lowChar==NFmiString("language") || lowChar==NFmiString("kieli"))
	return dParamRectLanguage;
  else if(lowChar==NFmiString("finnishtimezone") || lowChar==NFmiString("suomenaikavyöhyke"))
	return dFinnishTimezone;
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
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiTimeParamRect::WritePS(const NFmiRect & theAbsoluteRectOfSymbolGroup,
								NFmiFastQueryInfo * theQI ,
								ofstream & theDestinationFile,
								FmiPressOutputMode theOutput)
{
  if(!ActiveTimeIndex(itsPressParam->GetCurrentStep()) ||
	 !ActiveStationIndex(itsPressParam->GetCurrentStationStep()))
    {
	  return true;
    }

  itsCurrentSegmentTime = (static_cast<NFmiQueryInfo *>(theQI))->Time();
  itsCurrentTime = itsCurrentSegmentTime;

  //2.3.2000 StationPoint hallitsee lokaaliajat (wmo-nro ja longitudi)
  // joita myös tarvitaan auringon lasku/nousuissa
  itsStationPoint = NFmiStationPoint(itsPressParam->GetCurrentStation(), NFmiPoint(0.,0.));
  
  if(!PointParam(theQI))
	return isFalse;
  if(!SetRelativeHour(theQI,NFmiString("#TimeText")))
	return isFalse;

  NFmiPressTime pTime = TimeToWrite(theQI);
  if(pTime.IsMissing())
		return false;

  NFmiString timeString; // = pTime.ToStr(itsFormat,itsLanguage);

	int errCode;

	NFmiStationPoint tempStationPoint(itsStationPoint);
    
	//aina pitäisi olla localTime piirtoalkioissa (vrt segmentti ja tuote missä ei)
	if(tempStationPoint.Station()->GetIdent() < 1000)  //oletuksena on numeroitu 1->
	{
 		long wmo = itsPressParam->GetPressProduct()->itsNameToLonLat
			        ->FindWmo(tempStationPoint.Station()->GetName()); 
		tempStationPoint.SetIdent(wmo);
	}

	if(fFinnishTimezone)
			tempStationPoint.SetLongitude(25.);

	tempStationPoint.LocalTime(pTime, errCode);

	if(!fFinnishTimezone && errCode > 0  && !itsPressParam->GetErrorReported(errCode))
		{
		*itsLogFile << "  WARNING: possible problems to calculate local time, first: "
					<< static_cast<char *>(static_cast<const NFmiString &>(itsStationPoint.GetName()))
					<< endl;

		if(errCode == 1)
			*itsLogFile << "  ->no Wmo-number or longitude" << endl;
		else if(errCode == 2)
			*itsLogFile << "  ->Wmo-number missing from Station definition(longitude used)" << endl;
		else if(errCode == 3)
			*itsLogFile << "  ->Wmo-number missing from program and no longitude exists"  << endl;
		else if(errCode == 4)
			*itsLogFile << "  ->Wmo-number missing from program (longitude used)" << endl;
		itsPressParam->SetErrorReported(errCode);
		}
	if(fIsValidTime)
		{
		if(itsStringNameTimeFormat.IsValue())
			timeString = pTime.InterpretToStr(itsStringNameTimeFormat,itsLanguage);
		else
			timeString = pTime.ToStr(itsFormat,itsLanguage);
		}
	else
		timeString = NFmiString(" -   "); // puuttuva auringon nousu/lasku

  
  NFmiString str;

  if(fParenthesis)
	{
	  str = NFmiString("(");
	  str += NFmiString (timeString);
	  str += NFmiString(")");
	}
  else
	{
	  str = NFmiString (timeString);
	}

  return WriteCode(Construct(&str),
				   theAbsoluteRectOfSymbolGroup,
				   theDestinationFile,
				   NFmiString("#TimeText"),
				   theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiTime NFmiTimeParamRect::TimeToWrite(NFmiFastQueryInfo * theQI)
{
  // aina local time piirtoalkioissa (vrt segmentti ja tuote missä ei)
  // pitäisi mahdollistaa UTC-ajassa piirto

  NFmiMetTime tim(itsCurrentTime,1); //localTimea varten
  if(itsPressParam->IsStationLocalTimeMode())
	{
	  //HUOM tämä koodi ylläpidettävä samanlaisena myös NFmiParamRectissä
	  double longitude;
	  if(IsMissingLonLat())
	  {
	     NFmiPoint lonLat;
		 NFmiString name = itsPressParam->GetCurrentStation().GetName();
		 GetPressProduct()->FindLonLatFromList(name, lonLat);
		 itsPressParam->SetCurrentStationLonLat(lonLat);
	  }
	  if(IsMissingLonLat()) //eli taulukostakaan ei löytynyt
		*itsLogFile << "*** ERROR: longitudu missing from "
					<< static_cast<char *>(itsStationPoint.GetName())
					<< "to local time"
					<< endl;

	  longitude = itsPressParam->GetCurrentStation().GetLongitude();
	  NFmiTime localTime = tim.LocalTime(-static_cast<float>(longitude));

	  //Hullua näin
	  NFmiMetTime saveTime = (static_cast<NFmiQueryInfo *>(theQI))->Time();
	  theQI->TimeToNearestStep (localTime, kCenter, theQI->TimeResolution()/2);
	  tim =  (static_cast<NFmiQueryInfo *>(theQI))->Time();
	  theQI->Time(saveTime);
	}
  return tim;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiTimeParamRect:: PointParam(NFmiFastQueryInfo * theQI)
{
  //aika ei tarvitse parametriä
  fParamErrorReported = false;
  return true;
}

// ======================================================================
