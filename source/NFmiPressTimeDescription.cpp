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
  bool res1, res2, res3;
  res1 = thePrepr.SetConditionalStripping(theCondValue,
										  theConditionalBeginDirective,
										  theConditionalNotBeginDirective,
										  theConditionalEndDirective,
										  theConditionalElseDirective);
  res3 = thePrepr.SetReplaceMap(itsReplaceMap);
  res2 = thePrepr.Strip();
  if(!res1 || !res2 || !res3)
	{
	  *itsLogFile << "*** ERROR: Preprocessing failed: "
				  << theConditionalBeginDirective
				  << endl;
	  string message = thePrepr.GetMessage();
	  if(!message.empty())
		*itsLogFile << "*** "  << message << endl;
	  return false;
	}
  int num = thePrepr.NumOfReplacesDone();
  if (num>0)
	  *itsLogFile << "  muuttujakorvauksia include-kierroksella "
				  << num
				  << " kpl"
				  << endl;
/*
  unsigned long pos = oldString.find("$#");
  if (pos != string::npos)
  {
	  string newString += oldString.substr(0, pos);

	  *itsLogFile << "*** ERROR: Unknown variables: "
				  << theConditionalBeginDirective
				  << endl;
  }
*/
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
	//res = prePr.ReadFile(inFileName);
	set<string> optinalDirectives;
	//HUOM kaikki optiot esiteltävä päätasolla, vaikka includeissa käyetettäisiin
	res = prePr.ReadFileCheckingOptions(inFileName, "#ifCondition", optinalDirectives);

	bool firstLoop = true;
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
//		if(!PreProcessConditionally(prePr, GetSeasonsStatus()->afternoon, "#ifAfternoon", "#ifNotAfternoon", "#afternoonEndif", "#afternoonElse"))  
//		  return false;

		set<string>::const_iterator pos;
		string condition, conditionBody, conditionBody2, conditionBodyLow, firstToLower;
		string conditionMode;
		string notDir, elseDir, endDir, numStr;
		string numStr1, numStr2, numStr3, numStr4;
		bool hourCondition;
		NFmiMetTime now;
		int hour = now.GetHour();
		for(pos = optinalDirectives.begin(); pos!= optinalDirectives.end(); ++pos)
		{
			condition = *pos;
			if(firstLoop)
				*itsLogFile << "  vapaavalintainen direktiivi: "  
						        <<  condition << endl;
			conditionMode = condition.substr(12, 4);
			hourCondition = conditionMode == "Hour";
			if(hourCondition)
			{
				conditionBody = condition.substr(3, condition.size()-3);
				conditionBody2 = condition.substr(4, condition.size()-4);
				firstToLower = condition.substr(3, 1);
				transform (conditionBody.begin(), conditionBody.end(), firstToLower.begin(), tolower); 
				notDir = "#ifNot" + conditionBody; //ei kuitenkaan voi käyttää koska esittely ei onnistu
				elseDir = "#" + firstToLower + conditionBody2 + "Else";
				endDir = "#" + firstToLower + conditionBody2 + "Endif";
				numStr = condition.substr(condition.size()-2, 2);
				if(numStr.find_first_not_of("0123456789") != string::npos)
				{
					numStr = condition.substr(condition.size()-1, 1); //miksi toisen kerran
					if(numStr.find_first_not_of("0123456789") != string::npos)
					{
						*itsLogFile << "*** ERROR: kielletty tuntidirektiivi: "  
									<<  condition << endl;
						continue;
					}
				}
				int dirHour = atoi(numStr.c_str());
				if(!PreProcessConditionally(prePr, GetSeasonsStatus()->hour >= dirHour, condition, notDir, endDir, elseDir))  
					return false;
			}
			else //DataConversion; else ja end-komentoihin kuitenkin ajan toisto mukaan, muuten  
				 //eri aikaehdot käyttävät toistensa else/endejä  
			{
				conditionBody = condition.substr(3, condition.size()-3);
				conditionBody2 = condition.substr(4, condition.size()-4);
			//	firstToLower = condition.substr(3, 1); //EI TOIMI YHTÄKKIÄ VAIKKA YLLÄ OK??
			//	transform (conditionBody.begin(), conditionBody.end(), firstToLower.begin(), tolower); 
				notDir = "#ifNot" + conditionBody; //ei kuitenkaan voi käyttää koska esittely ei onnistu
				firstToLower = "c";
				elseDir = "#" + firstToLower + conditionBody2 + "Else";
				endDir = "#" + firstToLower + conditionBody2 + "Endif";
				numStr1 = condition.substr(condition.size()-9, 2);
				numStr2 = condition.substr(condition.size()-7, 2);
				numStr3 = condition.substr(condition.size()-4, 2);
				numStr4 = condition.substr(condition.size()-2, 2);
				if(   numStr1.find_first_not_of("0123456789") != string::npos
					||numStr2.find_first_not_of("0123456789") != string::npos
					||numStr3.find_first_not_of("0123456789") != string::npos
					||numStr4.find_first_not_of("0123456789") != string::npos)
				{
						*itsLogFile << "*** ERROR: kielletty päivädirektiivi: "  
									<<  condition << endl;
						continue;
				}

				int day1 = atoi(numStr1.c_str());
				int month1 = atoi(numStr2.c_str());
				int day2 = atoi(numStr3.c_str());
				int month2 = atoi(numStr4.c_str());
				if(day1 < 1 || day1 >31 || day2 < 1 || day2 >31
				   ||month1 < 1 || month1 >12 || month2 < 1 || month2 >12)
				{
						*itsLogFile << "*** ERROR: virheellinen aika: "  
									<<  condition << endl;
						continue;
				}
				NFmiMetTime dirTime1, dirTime2;
				dirTime1.SetDay(day1);
				dirTime1.SetMonth(month1);
				dirTime2.SetDay(day2);
				dirTime2.SetMonth(month2);

				NFmiMetTime today;
				int julToday = today.GetJulianDay();
				int jul1 = dirTime1.GetJulianDay();
				int jul2 = dirTime2.GetJulianDay();
				bool julCond;
				if(jul2 > jul1)
					julCond = julToday >= jul1 && julToday <= jul2;
				else
					julCond = julToday >= jul1 || julToday <= jul2;

				if(!PreProcessConditionally(prePr, julCond, condition, notDir, endDir, elseDir))  
					return false;
			}
	}
		if(!prePr.IncludeFiles("#Include", includePath, "inc"))
		  {
			*itsLogFile << "*** ERROR: Preprocessing failed to include file:" << endl;
			string message = prePr.GetMessage();
			if(!message.empty())
			  *itsLogFile << "    "  << message << endl;
			return false;
		  }
		  firstLoop = false;
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

	  unsigned long pos = outString.find("$#");
	  if (pos != string::npos)
	  {
		  string newString = outString.substr(pos, 3);

		  *itsLogFile << "*** ERROR: Määrittelemätön muuttuja: "
					  << newString
					  << endl;
	  }

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
	case dDayAddition: //toimii vain Segmentin päätasolla
		{
		if (!ReadEqualChar())
			break;
			
		if(ReadOne(long1))
		{
			itsFirstDeltaDays = static_cast<short>(JustifyByLeaps(itsFirstDeltaDays + long1));
		}

		ReadNext();
		break;
		}
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
			
			if(long1 > 0 && long2 > 0  && long3 > 0  && long1 <= long3)
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
// image osannee käyttää täältä Päivän ja Tunnin vaikka muilla oma
	case dRelDay:
		{
		if (!ReadEqualChar())
			break;
		if(ReadLong(long1))
			itsFirstDeltaDays = static_cast<unsigned short>(long1+ itsEnvironment.GetDayAdvance());

		ReadNext();
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
		return NFmiPressDescription::ReadRemaining();  
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
  if(lowChar==NFmiString("dayaddition") ||
	 lowChar==NFmiString("päivälisäys"))
	return dDayAddition;
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
  compTime.SetHour(static_cast<short>(hourDelta));
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
