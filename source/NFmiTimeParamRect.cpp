// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiTimeParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiTimeParamRect.h"
#include "NFmiPressParam.h"
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
	  itsFont = NFmiString("Cviikko");
	}
  else if(itsLanguage == kChinese) //mutta uusi ei
	{
	  itsFormat = itsOrigFormat;
	  itsFont = itsOrigFont;
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

  itsFont = NFmiString("Helvetica");
  itsAlignment = kCenter;

  SetPreReadingTimes();

  ReadNext();

  // kopsattu vain numerosta,jotta oletussuht.paikka = (0,0) toimii
  itsRelRect -= NFmiPoint(1., 1.);

  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Numerossa" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #AikaTekstissä: "
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
		case dAlignment:
		  {
			if (!ReadEqualChar())
			  break;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
			if (itsString == NFmiString ("Center") ||
				itsString == NFmiString ("KeskiPiste") ||
				itsString == NFmiString ("Keski"))
			  itsAlignment = kCenter;
			else if (itsString == NFmiString ("Right") ||
					 itsString == NFmiString ("OikeaLaita") ||
					 itsString == NFmiString ("Oikea"))
			  itsAlignment = kRight;
			else if (itsString == NFmiString ("Left") ||
					 itsString == NFmiString ("VasenLaita") ||
					 itsString == NFmiString ("Vasen"))
			  itsAlignment = kLeft;

			ReadNext();
			break;
		  }
		case dFont:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;
			itsFont = itsObject;

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

		  *itsDescriptionFile >> itsObject;
		  itsString = itsObject;
		  itsIntObject = ConvertDefText(itsString);
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: Päiviä ei voi asettaa #AikaParamissa"  << endl;
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

  //flush viimeinen takaisin sreamiin! Miten?

  itsOrigFormat = itsFormat;
  itsOrigFont = itsFont;
  if(itsLanguage == kChinese && (itsFormat == kShortWeekDay || itsFormat == kI))
	{
	  itsFormat = kI;
	  itsFont = NFmiString("Cviikko");
	}

  SetPostReadingTimes();

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
  if(object==NFmiString("Format") || object==NFmiString("Formaatti"))
	return dTimeFormat;
  else if(object==NFmiString("Language") || object==NFmiString("Kieli"))
	return dParamRectLanguage;
  else
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
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiTimeParamRect::WritePS(const NFmiRect & AbsoluteRectOfSymbolGroup,
								NFmiFastQueryInfo * theQI ,
								ofstream & theDestinationFile,
								FmiPressOutputMode theOutput)
{
  if(!ActiveTimeIndex(itsPressParam->GetCurrentStep()))
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
  if(!SetRelativeHour(theQI,NFmiString("#AikaTeksti")))
	return isFalse;

  NFmiPressTime pTime = TimeToWrite(theQI);

  //aina local time piirtoalkioissa (vrt segmentti ja tuote missä ei)
  int errCode;
  itsStationPoint.LocalTime(pTime, errCode);
  if(errCode > 0  && !itsPressParam->GetErrorReported(errCode))
	{
	  *itsLogFile << "  WARNING: possible problems to calculate local time, first: "
				  << static_cast<char *>(static_cast<const NFmiString &>(itsStationPoint.GetName()))
				  << endl;

	  if(errCode == 1)
		*itsLogFile << "  ->ei Wmo-numeroa eikä longitudia" << endl;
	  else if(errCode == 2)
		*itsLogFile << "  ->Wmo-nro puuttuu Asema-määrittelyltä(longitudi käytetty)" << endl;
	  else if(errCode == 3)
		*itsLogFile << "  ->Wmo-nro puuttuu ohjelmasta eikä longitudia ole"  << endl;
	  else if(errCode == 4)
		*itsLogFile << "  ->Wmo-nro puuttuu ohjelmasta (longitudi käytetty)" << endl;
	  itsPressParam->SetErrorReported(errCode);
	}
  NFmiString timeString; // = pTime.ToStr(itsFormat,itsLanguage);
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
				   AbsoluteRectOfSymbolGroup,
				   theDestinationFile,
				   NFmiString("AIKATEKSTI"),
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
	  double latitude = itsStationPoint.GetLatitude();//Testiin
	  double longitude = itsStationPoint.GetLongitude();//Testiin
	  if(fabs(longitude) <= .001 && fabs(latitude) <= .001)
		*itsLogFile << "*** ERROR: longitudi puuttuu "
					<< static_cast<char *>(itsStationPoint.GetName())
					<< ":lta paikalliseen aikaan"
					<< endl;
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
