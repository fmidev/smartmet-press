// ======================================================================
/*
 * \file
 * \brief Implementation of class NFmiPressTimeDescription
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

// press
#include "NFmiPressTimeDescription.h"
#include "NFmiPressTime.h"
// newbase
#include "NFmiFastQueryInfo.h"
#include "NFmiValueString.h"
// system
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Tarkistaa onko viikonpäiväehdot tähän vuodenaikaan voimassa, esim.
 * lumikartta ei piirretä torstaisin jos snow-kausi ei ole päällä
 * oletus tietysti että viikonpäivädirrit on aktiivisia.
 *
 * \param theDefinition Undocumented
 * \param theSeasons Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::WeekdayDirectiveActive(const string & theDefinition,
											  const FmiPressSeasons* theSeasons) const
{
  string::size_type  start1, end1, start2, end2, pos;
  const string delims(" \t\n");
  pos = theDefinition.find("#Viikonpäivä");
  if(pos == string::npos)
	{
	  pos = theDefinition.find("#viikonpäivä");
	  if(pos == string::npos)
		{
		  pos = theDefinition.find("#Weekday");
		  if(pos == string::npos)
			{
			  pos = theDefinition.find("#weekday");
			}
		}
	}
  if(pos != string::npos)
	{
	  pos = theDefinition.find_first_of(delims, pos);
	  start1 = theDefinition.find_first_not_of(delims, pos);
	  end1 = theDefinition.find_first_of(delims, start1);
	  start2 = theDefinition.find_first_not_of(delims, end1);
	  end2 = theDefinition.find_first_of(delims, start2);
	  string str1 = theDefinition.substr(start1, end1-start1);
	  string str2 = theDefinition.substr(start2, end2-start2);
	  bool in;
	  NFmiString str = str1;
	  str.LowerCase();
	  if(str == "onlyin" || str == "vainkun")
		in = true;
	  else if(str == "onlyoutside" || str == "vainkunei")
		in = false;
	  else
		{
		  *itsLogFile << "***ERROR: on #Weekday line"<< endl;
		  return true;
		}
	  str = str2;
	  str.LowerCase();
	  if(str == "summer" || str == "kesä")
		{
		  *itsLogFile << "viikonpäiväohjauksen riippuvuus: kesä"<< endl;
		  return in == theSeasons->summer;
		}
	  else if(str == "snowperiod" || str == "lumikausi")
		{
		  *itsLogFile << "viikonpäiväohjauksen riippuvuus: lumikausi"<< endl;
		  return in == theSeasons->snow;
		}
	  else if(str == "pollenperiod" || str == "siitepölykausi")
		{
		  *itsLogFile << "viikonpäiväohjauksen riippuvuus: siitepölykausi"<< endl;
		  return in == theSeasons->pollen;
		}
	  else if(str == "wintertime" || str == "talviaika")
		{
		  *itsLogFile << "viikonpäiväohjauksen riippuvuus: talviaika"<< endl;
		  return in == theSeasons->wintertime;
		}
	  else if(str == "summertime" || str == "kesäaika")
		{
		  *itsLogFile << "viikonpäiväohjauksen riippuvuus: kesäaika"<< endl;
		  return in != theSeasons->wintertime; //HUOM
		}
	  else
		{
		  *itsLogFile << "***ERROR: on #weekday line"<< endl;
		  return true;
		}
	}

   //oletus: ei löydy tiedostoa -> ei kielletä viikonloppudirektiivien käyttöä
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePrepr Undocumented
 * \param theCondValue Undocumented
 * \param theConditionalBeginDirective Undocumented
 * \param theConditionalNotBeginDirective Undocumented
 * \param theConditionalEndDirective Undocumented
 * \param theConditionalElseDirective Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::PreProcessConditionally(NFmiPreProcessor & thePrepr,
											   bool theCondValue,
											   const string & theConditionalBeginDirective,
											   const string & theConditionalNotBeginDirective,
											   const string & theConditionalEndDirective,
											   const string & theConditionalElseDirective )
{
  bool res1, res2;
  res1 = thePrepr.SetConditionalStripping(theCondValue,
										  theConditionalBeginDirective,
										  theConditionalNotBeginDirective,
										  theConditionalEndDirective,
										  theConditionalElseDirective);
  res2 = thePrepr.Strip();
  if(!res1 || !res2)
	{
	  *itsLogFile << "*** ERROR: Preprocessing failed: "
				  << theConditionalBeginDirective
				  << endl;
	  string message = thePrepr.GetMessage();
	  if(!message.empty())
		*itsLogFile << "*** "  << message << endl;
	  return false;
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param inFileName Undocumented
 * \param outFileName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::PreProcessDefinition(const string & inFileName,
											const string & outFileName)
// Tämä korvaa PreProcessProduct:n tehden kaikki siivoushommat: //-kommentit,
// /* */-kommentit, if/else/endif-direktiivit ja Includet
// nyt voi koodin käsittelystä poistaa kommenttien käsittelyn, entäs prem???

{

  NFmiPreProcessor prePr;
  bool res, res2;
  NFmiTime tim;

  string includePath(itsHomePath);
  includePath += kFmiDirectorySeparator;
	includePath += "Include";
	res = prePr.ReadFile(inFileName);

	while (!prePr.NoFilesIncluded())
	  {
		if(!PreProcessConditionally(prePr, GetSeasonsStatus()->pollenOrSnow, "#ifPollenOrSnowPeriod", "#ifNotPollenOrSnowPeriod", "#pollenOrSnowPeriodEndif", "#pollenOrSnowPeriodElse"))  //4.9.02
		  return false;
		if(!PreProcessConditionally(prePr, GetSeasonsStatus()->wintertime, "#ifWinterTime", "#ifNotWinterTime", "#endif", "#else"))
		  return false;
		if(!PreProcessConditionally(prePr, GetSeasonsStatus()->snow, "#ifSnowPeriod", "#ifNotSnowPeriod", "#snowPeriodEndif", "#snowPeriodElse"))
		  return false;
		if(!PreProcessConditionally(prePr, GetSeasonsStatus()->summer, "#ifSummer", "#ifNotSummer", "#summerEndif", "#summerElse"))  //13.6.02
		  return false;
		if(!PreProcessConditionally(prePr, GetSeasonsStatus()->pollen, "#ifPollenPeriod", "#ifNotPollenPeriod", "#pollenPeriodEndif", "#pollenPeriodElse"))  //4.9.02
		  return false;
		if(!prePr.IncludeFiles("#Include", includePath, "inc"))
		  {
			*itsLogFile << "*** ERROR: Preprocessing failed to include file" << endl;
			string message = prePr.GetMessage();
			if(!message.empty())
			  *itsLogFile << "*** "  << message << endl;
			return false;
		  }
	  }

	//jokaisen viikonpäivän direktiivit tarkastetaan
	bool isTheDay; //= true;
	bool weekdayActive = WeekdayDirectiveActive(prePr.GetString(),GetSeasonsStatus());
	for (int day = 1; day <= 7; day++)
	  {
		isTheDay = tim.GetWeekday() == GetSeasonsStatus()->weekday;
		string weekday(tim.Weekday(kEnglish));
		string ifDir = "#if" + weekday;
		string ifNotDir = "#ifNot" + weekday;
		string elseDir = "#" + weekday + "Else";
		string endifDir = "#" + weekday + "Endif";
		res = prePr.SetConditionalStripping(isTheDay && weekdayActive, ifDir, ifNotDir, endifDir, elseDir);
		res2 = prePr.Strip();
		string message = prePr.GetMessage();
		if(!message.empty())
		  *itsLogFile << "*** ERROR: "  << message << endl;
		if(prePr.NumOfLiita() > 0)
		  *itsLogFile << "*** WARNING: LIITÄ käytetty"  << endl;
		if(!res || !res2)
		  {
			*itsLogFile << "*** ERROR: Preprocessing failed" << endl;
			return false;
		  }
		tim.ChangeByDays(1);
		//isToday = false;
	  }

	string outString = prePr.GetString();
	ofstream file(outFileName.c_str());
	file << outString ;
	file.close();
	file.clear();
	return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param isSecond Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

unsigned long NFmiPressTimeDescription::ReadTimeFormat(bool isSecond)
{
  NFmiString helpString;
  unsigned long format = kWwsHH; //oletus

  helpString = ReadString(); //tuo myös välilyönnit hipsuissa
 
  if (helpString == NFmiString ("HHdMM"))   
	format = kHHdMM;
  else if (helpString == NFmiString ("HdMM"))   
	format = kHdMM;
  else if (helpString == NFmiString ("wwsHH") || helpString == NFmiString ("WwsHH"))
	format = kWwsHH;
  else if (helpString == NFmiString ("DdMd"))
	format = kDdMd; 
  else if (helpString == NFmiString ("WwsDDdMMd"))
	format = kWwsDDdMMd; 
  else if (helpString == NFmiString ("WwsDdMd"))
	format = kWwsDdMd; 
  else if (helpString == NFmiString ("NnnnsYYYY"))
	format = kNnnnsYYYY; 
  else if (helpString == NFmiString ("DDdMMdYYYY")) 
	format = kDDdMMdYYYY; 
  else if (helpString == NFmiString ("DdMdYYYY"))   
	format = kDdMdYYYY; 
  else
	{
	  if(isSecond)
		itsSecondStringNameTimeFormat = helpString;
	  else
		itsStringNameTimeFormat = helpString;
	}
  return format;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::ReadRemaining(void)  
{ 
  long long1, long2, long3;
  NFmiValueString valueString;
  switch(itsIntObject)
	{	
	case dGivenRelHours:
	  {
		fGivenHoursAreRelative = true;
		
		ReadNext();
		break;
	  }
	case dMinute:  
	  {
		SetOne(itsFirstPlotMinutes);
		
		break;
	  }
	case dWriteTimeAsUtc:
	  {
		fWriteAsUtc = true;
		
		ReadNext();
		break;
	  }
	case dTimeTableActive:
	  {
		if (!ReadEqualChar())
		  break;
		
		long3 = 1000;
		if(ReadTwo(long1, long2))
		  {
			*itsDescriptionFile >> itsObject;
			valueString = itsObject;
			if(valueString.IsNumeric())  
			  {
				long3 = static_cast<long>(valueString);
				
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
			  }
			else
			  {
				itsString = valueString;
			  }
			
			if(long1 > 0 && long2 > 0  && long3 > 0  && long1 < long3)
			  {
				itsLoopActivity.startIndex = long1;
				itsLoopActivity.step = long2;
				itsLoopActivity.stopIndex = long3;
			  }
			else
			  *itsLogFile << "*** ERROR: Not valid looping (start,step,stop): "
						  << long1
						  << long2
						  << long3
						  << endl;
		  }
		
		
		itsIntObject = ConvertDefText(itsString);
		
		break;
	  }
	case dBypassValue:
	  {
		SetOne(itsLoopActivity.bypassValue);
		itsLoopActivity.bypassWithValue = true;
		
		break;
	  }
	default:
	  {
		return NFmiPressDescription::ReadRemaining();  
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

int NFmiPressTimeDescription::ConvertDefText(NFmiString & object) 
{
  NFmiString lowChar = object;
  lowChar.LowerCase(); 
  
  if(lowChar==NFmiString("day") ||
	 lowChar==NFmiString("päivä"))
	return dRelDay;
  else if(lowChar==NFmiString("minute") ||
		  lowChar==NFmiString("minuutti"))
	return dMinute;
  else if(lowChar==NFmiString("hour") ||
		  lowChar==NFmiString("tunti"))
	return dHour;
  else if(lowChar==NFmiString("relhour") ||
		  lowChar==NFmiString("suhteellinentunti"))
	return dRelHour;                     
  else if(lowChar==NFmiString("relhours") ||
		  lowChar==NFmiString("suhteellisettunnit")) // data-alkioissa
	return dGivenRelHours;                     
  else if(lowChar==NFmiString("hourstep") ||
		  lowChar==NFmiString("tuntiaskel") ||
		  lowChar==NFmiString("tuntiaskel"))
	return dHourStep;
  else if(lowChar==NFmiString("utcwriting") ||
		  lowChar==NFmiString("utctulostus")) // data-alkioissa
	return dWriteTimeAsUtc;                     
  else if(lowChar==NFmiString("timetableactive") ||
		  lowChar==NFmiString("aikataulukonaktiiviset"))
	return dTimeTableActive;
  else if(lowChar==NFmiString("bypassvalue") ||
		  lowChar==NFmiString("ohitusarvo"))
	return dBypassValue;
  else 
	return NFmiPressDescription::ConvertDefText(object);
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param currentInd Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::ActiveTimeIndex(int currentInd) const
{
  //halutessa olioita voidaan piirtää esim joka n:teen aika-askeleeseen
  // Mika: Kannattaisikohan laittaa sulkuja?
  if(itsLoopActivity.startIndex < 1 || (currentInd-itsLoopActivity.startIndex)
	 % itsLoopActivity.step == 0
	 && currentInd >= static_cast<int>(itsLoopActivity.startIndex)
	 && currentInd <= static_cast<int>(itsLoopActivity.stopIndex)
	 ) return true;
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param relHours Undocumented
 * \param hourRes Undocumented
 * \param hourDelta Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiMetTime NFmiPressTimeDescription::NextUseTime(long relHours, long hourRes, long hourDelta) 
{
  //MetTimeltä ei löydy palvelua, jolla saisi NextTimen kun resoluutio ei ala nollasta
  //     eli esim ajat ovat 2,8 14,20,2.... tai aina 12 (hourDelta)
  //esim nyt = 8.2. klo 8.30 utc ->
  //	         NextUseTime(4,24,12) -> 9.2. klo 12 utc
  //	         NextUseTime(3,24,12) -> 8.2. klo 12
  //	         NextUseTime(-3,6,1)  -> 8.2. klo 7   utc
  NFmiMetTime curTime(60); 
  curTime.ChangeByHours(relHours);
  NFmiMetTime compTime(60); 
  compTime.ChangeByDays(-2); //riittääkö
  compTime.SetHour(hourDelta);
  if (compTime >= curTime)
	*itsLogFile << "*** ERROR: suhteellinen aikaero liian suuri: "
				<< relHours
				<< endl;
  else
	{
	  while(compTime <= curTime)
		compTime.ChangeByHours(hourRes);
	}
  
  return compTime;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param dayDiff Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

long NFmiPressTimeDescription::JustifyByLeaps(long dayDiff) 
{
  //ei kyllä ihan tarkka, ja toimii vain yhdelle vuodelle toistaiseksi
  long ret = dayDiff;
  short add;
  NFmiTime now; 
  NFmiTime compDate = now;
  short sign = +1;        //onko c:ssä
  if (dayDiff < 0) sign = -1;
  compDate.SetYear(now.GetYear()+sign);
  if(abs(abs(dayDiff)-365) < 5)
	if(abs(now.DifferenceInDays(compDate)) == 365)
	  add = 0;
	else
	  add = 1;
  else
	add = 0;
  
  return ret + add*sign;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressTimeDescription::SetPreReadingTimes(void) 
{
  NFmiTime curDate;
  curDate.SetHour(0);
  curDate.SetMin(0);
  NFmiTime firstPlotDate = NFmiTime(itsFirstPlotTime);
  firstPlotDate.SetHour(0);
  firstPlotDate.SetMin(0);
  itsFirstDeltaDays = static_cast<short>(firstPlotDate.DifferenceInDays(curDate));
  
  itsFirstOriginalDeltaDays = itsFirstDeltaDays;  
  itsFirstPlotHours = itsFirstPlotTime.GetHour(); 
  itsFirstPlotMinutes = itsFirstPlotTime.GetMin();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressTimeDescription::SetPostReadingTimes(void)
{
  itsFirstPlotTime.ChangeByDays(itsFirstDeltaDays-itsFirstOriginalDeltaDays);
  itsFirstPlotTime.SetTime(itsFirstPlotHours);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param data Undocumented
 * \param calledFrom Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::SetDataHour(NFmiFastQueryInfo * data,
										   const NFmiString & calledFrom)
{
  // Kun luupataan päivän sisällä ei voi käyttää omaa tuntia
  // vaan käytetään pressparamin aikaluuppi-arvoja
  // pidemmissä (>vrk) luupeissa pitäisi oma arvo toimia tässäkin
  if(!IsHourLoop())
	{
	  NFmiMetTime saveTime = (static_cast<NFmiQueryInfo *>(data))->Time();
	  NFmiMetTime tim = saveTime;
	  if(!(tim.GetHour() == itsFirstPlotHours) )
		*itsLogFile << "    tunti muutettu pirtoalkiossa: "
					<< itsFirstPlotHours
					<< " utc"
					<< endl;

	  tim.SetTime(itsFirstPlotHours); 
	  if(!data->Time(tim))
		{
		  if(itsLogFile)      
			*itsLogFile << "*** ERROR: aikaa ei ole: "
						<< tim
						<< static_cast<char *>(calledFrom)
						<< endl;
		  data->Time(saveTime);  //pitää asettaa takaisin 
		  return false;
		}
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param givenTime Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiMetTime NFmiPressTimeDescription::TimeToWrite(NFmiMetTime givenTime) const
{
  if(fWriteAsUtc)
	return givenTime;
  else 
	return 	NFmiMetTime(givenTime.LocalTime(25.f),1);	
}

// ======================================================================
