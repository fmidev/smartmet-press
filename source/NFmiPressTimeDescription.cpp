// ======================================================================
/*
 *†\file
 * \brief Implementation of class NFmiPressTimeDescription
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiPressTimeDescription.h"
#include "NFmiPressTime.h"
// newbase
#include "NFmiFastQueryInfo.h"
#include "NFmiValueString.h"
// system
#include <iostream>
#include <list>
#include <macgyver/StringConversion.h>

using namespace std;
#include "NFmiPressErrors.h"

// ----------------------------------------------------------------------
/*!
 * Tarkistaa onko viikonp‰iv‰ehdot t‰h‰n vuodenaikaan voimassa, esim.
 * lumikartta ei piirret‰ torstaisin jos snow-kausi ei ole p‰‰ll‰
 * oletus tietysti ett‰ viikonp‰iv‰dirrit on aktiivisia.
 *
 * \param theDefinition Undocumented
 * \param theSeasons Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::WeekdayDirectiveActive(const string& theDefinition,
                                                      const FmiPressSeasons* theSeasons) const
{
  string::size_type start1 = 0;
  string::size_type end1 = 0;
  string::size_type start2 = 0;
  string::size_type end2 = 0;
  string::size_type pos = 0;
  const string delims(" \t\n");
  pos = theDefinition.find("#Viikonp‰iv‰");
  if (pos == string::npos)
  {
    pos = theDefinition.find("#viikonp‰iv‰");
    if (pos == string::npos)
    {
      pos = theDefinition.find("#Weekday");
      if (pos == string::npos)
      {
        pos = theDefinition.find("#weekday");
      }
    }
  }
  if (pos != string::npos)
  {
    pos = theDefinition.find_first_of(delims, pos);
    start1 = theDefinition.find_first_not_of(delims, pos);
    end1 = theDefinition.find_first_of(delims, start1);
    start2 = theDefinition.find_first_not_of(delims, end1);
    end2 = theDefinition.find_first_of(delims, start2);
    string str1 = theDefinition.substr(start1, end1 - start1);
    string str2 = theDefinition.substr(start2, end2 - start2);
    bool in = false;
    NFmiString str = str1;
    str.LowerCase();
    if (str == "onlyin" || str == "vainkun")
      in = true;
    else if (str == "onlyoutside" || str == "vainkunei")
      in = false;
    else
    {
      *itsLogFile << "***ERROR: on #Weekday line" << endl;
      return true;
    }
    str = str2;
    str.LowerCase();
    if (str == "summer" || str == "kes‰")
    {
      *itsLogFile << "weekday dependency: summer" << endl;
      return in == theSeasons->summer;
    }
    else if (str == "snowperiod" || str == "lumikausi")
    {
      *itsLogFile << "weekday dependency: snowperiod" << endl;
      return in == theSeasons->snow;
    }
    else if (str == "pollenperiod" || str == "siitepˆlykausi")
    {
      *itsLogFile << "weekday dependency: pollen" << endl;
      return in == theSeasons->pollen;
    }
    else if (str == "wintertime" || str == "talviaika")
    {
      *itsLogFile << "weekday dependency: winter time" << endl;
      return in == theSeasons->wintertime;
    }
    else if (str == "summertime" || str == "kes‰aika")
    {
      *itsLogFile << "weekday dependency: summer time" << endl;
      return in != theSeasons->wintertime;  // HUOM
    }
    else
    {
      *itsLogFile << "***ERROR: on #weekday line" << endl;
      return true;
    }
  }

  // oletus: ei lˆydy tiedostoa -> ei kiellet‰ viikonloppudirektiivien k‰yttˆ‰
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

bool NFmiPressTimeDescription::PreProcessConditionally(
    NFmiPreProcessor& thePrepr,
    bool theCondValue,
    const string& theConditionalBeginDirective,
    const string& theConditionalNotBeginDirective,
    const string& theConditionalEndDirective,
    const string& theConditionalElseDirective)
{
  bool res1 = false;
  bool res2 = false;
  bool res3 = false;
  res1 = thePrepr.SetConditionalStripping(theCondValue,
                                          theConditionalBeginDirective,
                                          theConditionalNotBeginDirective,
                                          theConditionalEndDirective,
                                          theConditionalElseDirective);
  res3 = thePrepr.SetReplaceMap(itsReplaceMap);
  res2 = thePrepr.Strip();
  if (!res1 || !res2 || !res3)
  {
    *itsLogFile << "*** ERROR: Preprocessing failed: " << theConditionalBeginDirective << endl;
    string message = thePrepr.GetMessage();
    if (!message.empty())
      *itsLogFile << "*** " << message << endl;
    return false;
  }
  int num = thePrepr.NumOfReplacesDone();
  if (num > 0)
    *itsLogFile << "  " << num << " variables substituted in include round" << endl;

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

bool NFmiPressTimeDescription::PreProcessDefinition(const string& inFileName,
                                                    const string& outFileName)
// T‰m‰ korvaa PreProcessProduct:n tehden kaikki siivoushommat: //-kommentit,
// /* */-kommentit, if/else/endif-direktiivit ja Includet
// nyt voi koodin k‰sittelyst‰ poistaa kommenttien k‰sittelyn, ent‰s prem???

{
  NFmiPreProcessor prePr;
  bool res = false;
  bool res2 = false;
  NFmiTime tim;
  bool isUnix = false;

#ifndef UNIX
  string includePath(itsHomePath);
  includePath += kFmiDirectorySeparator;
  includePath += "Include";
  isUnix = false;
#else
  string includePath = static_cast<string>(getIncPath());
  isUnix = true;
#endif
  // res = prePr.ReadFile(inFileName);
  set<string> optinalDirectives;
  // HUOM kaikki optiot esitelt‰v‰ p‰‰tasolla, vaikka includeissa k‰yetett‰isiin
  if (!prePr.ReadFileCheckingOptions(inFileName, "#ifCondition", optinalDirectives))
  {
    if (itsLogFile)
      *itsLogFile << "*** ERROR: Cannot read " << inFileName << endl;
    return false;
  }

  if (!PreProcessConditionally(prePr, isUnix, "#ifUnix", "#ifNotUnix", "#unixEndif", "#unixElse"))
    return false;

  bool firstLoop = true;
  while (!prePr.NoFilesIncluded())
  {
    if (!PreProcessConditionally(prePr,
                                 GetSeasonsStatus()->pollenOrSnow,
                                 "#ifPollenOrSnowPeriod",
                                 "#ifNotPollenOrSnowPeriod",
                                 "#pollenOrSnowPeriodEndif",
                                 "#pollenOrSnowPeriodElse"))  // 4.9.02
      return false;
    if (!PreProcessConditionally(prePr,
                                 GetSeasonsStatus()->wintertime,
                                 "#ifWinterTime",
                                 "#ifNotWinterTime",
                                 "#endif",
                                 "#else"))
      return false;
    if (!PreProcessConditionally(prePr,
                                 GetSeasonsStatus()->snow,
                                 "#ifSnowPeriod",
                                 "#ifNotSnowPeriod",
                                 "#snowPeriodEndif",
                                 "#snowPeriodElse"))
      return false;
    if (!PreProcessConditionally(prePr,
                                 GetSeasonsStatus()->summer,
                                 "#ifSummer",
                                 "#ifNotSummer",
                                 "#summerEndif",
                                 "#summerElse"))  // 13.6.02
      return false;
    if (!PreProcessConditionally(prePr,
                                 GetSeasonsStatus()->pollen,
                                 "#ifPollenPeriod",
                                 "#ifNotPollenPeriod",
                                 "#pollenPeriodEndif",
                                 "#pollenPeriodElse"))  // 4.9.02
      return false;
    //		if(!PreProcessConditionally(prePr, GetSeasonsStatus()->afternoon, "#ifAfternoon",
    //"#ifNotAfternoon", "#afternoonEndif", "#afternoonElse"))
    //		  return false;

    set<string>::const_iterator pos;
    // set<string>::const_iterator dayPos;
    string condition;
    string conditionBody;
    string conditionBody2;
    string conditionBodyLow;
    string firstToLower;
    string conditionMode;
    string notDir;
    string elseDir;
    string endDir;
    string numStr;
    string numStr1;
    string numStr2;
    string numStr3;
    string numStr4;
    string dayStr;
    string todayStr;
    bool hourCondition = false;
    bool multiDayCondition = false;
    NFmiMetTime now;
    // int hour = now.GetHour();
    for (pos = optinalDirectives.begin(); pos != optinalDirectives.end(); ++pos)
    {
      condition = *pos;
      string::size_type bad_character = condition.find('\t', 0);
      if (bad_character != string::npos)
        condition = condition.substr(0, bad_character);
#ifdef UNIX
      // For some reason, the Linux program sees \r characters in the ends
      // of some directives, e.g. #ifConditionDate. They must be removed.
      bad_character = condition.find('\r', 0);
      if (bad_character != string::npos)
        condition = condition.substr(0, bad_character);
#endif

      if (firstLoop)
        *itsLogFile << "  freestyle directive: " << condition << endl;
      conditionMode = condition.substr(12, 3);
      hourCondition = conditionMode == "Hou";
      multiDayCondition = conditionMode == "Day";
      if (hourCondition)
      {
        conditionBody = condition.substr(3, condition.size() - 3);
        conditionBody2 = condition.substr(4, condition.size() - 4);
        firstToLower = condition.substr(3, 1);
        transform(conditionBody.begin(), conditionBody.end(), firstToLower.begin(), ::tolower);
        notDir = "#ifNot" + conditionBody;  // ei kuitenkaan voi k‰ytt‰‰ koska esittely ei onnistu
        elseDir = "#" + firstToLower + conditionBody2 + "Else";
        endDir = "#" + firstToLower + conditionBody2 + "Endif";
        numStr = condition.substr(condition.size() - 2, 2);
        if (numStr.find_first_not_of("0123456789") != string::npos)
        {
          numStr = condition.substr(condition.size() - 1, 1);  // miksi toisen kerran
          if (numStr.find_first_not_of("0123456789") != string::npos)
          {
            *itsLogFile << "*** ERROR: kielletty tuntidirektiivi: " << condition << endl;
            continue;
          }
        }
        int dirHour = Fmi::stoi(numStr);
        if (!PreProcessConditionally(
                prePr, GetSeasonsStatus()->hour >= dirHour, condition, notDir, endDir, elseDir))
          return false;
      }
      else if (multiDayCondition)
      {
        conditionBody = condition.substr(15, condition.size() - 15);
        notDir = "#ifNot" + conditionBody;  // ei kuitenkaan voi k‰ytt‰‰ koska esittely ei onnistu
        elseDir = "#" + conditionBody + "Else";
        endDir = "#" + conditionBody + "Endif";
        bool isDay = false;
        int dayPos = 15;
        int weekday = GetSeasonsStatus()->weekday;
        int condSize = condition.size();
        while (dayPos + 2 <= condSize && !isDay)
        {
          dayStr = condition.substr(dayPos, 2);
          dayPos += 2;
          if (dayStr != "Mo" && dayStr != "Tu" && dayStr != "We" && dayStr != "Th" &&
              dayStr != "Fr" && dayStr != "Sa" && dayStr != "Su")
          {
            string msg = string("invalid day directive: ") + condition;
            errors.push_back(msg);
            *itsLogFile << "*** ERROR: " << msg << endl;
            continue;
          }
          isDay = Weekday2Num(NFmiString(dayStr)) == weekday;
        }
        if (!PreProcessConditionally(prePr, isDay, condition, notDir, endDir, elseDir))
          return false;
      }
      else  // DateCondition; else ja end-komentoihin kuitenkin ajan toisto mukaan, muuten
      // eri aikaehdot k‰ytt‰v‰t toistensa else/endej‰
      {
        conditionBody = condition.substr(3, condition.size() - 3);
        conditionBody2 = condition.substr(4, condition.size() - 4);
        //	firstToLower = condition.substr(3, 1); //EI TOIMI YHTƒKKIƒ VAIKKA YLLƒ OK??
        //	transform (conditionBody.begin(), conditionBody.end(), firstToLower.begin(),
        // tolower);
        notDir = "#ifNot" + conditionBody;  // ei kuitenkaan voi k‰ytt‰‰ koska esittely ei onnistu
        firstToLower = "c";
        elseDir = "#" + firstToLower + conditionBody2 + "Else";
        endDir = "#" + firstToLower + conditionBody2 + "Endif";
        numStr1 = condition.substr(condition.size() - 9, 2);
        numStr2 = condition.substr(condition.size() - 7, 2);
        numStr3 = condition.substr(condition.size() - 4, 2);
        numStr4 = condition.substr(condition.size() - 2, 2);
        if (numStr1.find_first_not_of("0123456789") != string::npos ||
            numStr2.find_first_not_of("0123456789") != string::npos ||
            numStr3.find_first_not_of("0123456789") != string::npos ||
            numStr4.find_first_not_of("0123456789") != string::npos)
        {
          string msg = string("kielletty p‰iv‰direktiivi: ") + condition;
          errors.push_back(msg);
          *itsLogFile << "*** ERROR: " << msg << endl;
          continue;
        }

        int day1 = Fmi::stoi(numStr1);
        int month1 = Fmi::stoi(numStr2);
        int day2 = Fmi::stoi(numStr3);
        int month2 = Fmi::stoi(numStr4);
        if (day1 < 1 || day1 > 31 || day2 < 1 || day2 > 31 || month1 < 1 || month1 > 12 ||
            month2 < 1 || month2 > 12)
        {
          string msg = string("incorrect time: ") + condition;
          errors.push_back(msg);
          *itsLogFile << "*** ERROR: " << msg << endl;
          continue;
        }
        NFmiMetTime dirTime1;
        NFmiMetTime dirTime2;
        dirTime1.SetDay(day1);
        dirTime1.SetMonth(month1);
        dirTime2.SetDay(day2);
        dirTime2.SetMonth(month2);

        NFmiMetTime today;
        today.SetMonth(GetSeasonsStatus()->month);
        today.SetDay(GetSeasonsStatus()->day);
        int julToday = today.GetJulianDay();
        int jul1 = dirTime1.GetJulianDay();
        int jul2 = dirTime2.GetJulianDay();
        bool julCond = false;
        if (jul2 > jul1)
          julCond = julToday >= jul1 && julToday <= jul2;
        else
          julCond = julToday >= jul1 || julToday <= jul2;

        if (!PreProcessConditionally(prePr, julCond, condition, notDir, endDir, elseDir))
          return false;
      }
    }

    if (!prePr.IncludeFiles("#Include", includePath, "inc"))
    {
      *itsLogFile << "*** ERROR: Preprocessing failed to include file:" << endl;
      string message = prePr.GetMessage();
      if (!message.empty())
        *itsLogFile << "    " << message << endl;
      return false;
    }
    firstLoop = false;
  }

  // jokaisen viikonp‰iv‰n direktiivit tarkastetaan
  bool isTheDay = false;  //= true;
  bool weekdayActive = WeekdayDirectiveActive(prePr.GetString(), GetSeasonsStatus());
  for (int day = 1; day <= 7; day++)
  {
    isTheDay = tim.GetWeekday() == GetSeasonsStatus()->weekday;
    string weekday(tim.Weekday(kEnglish));
    string ifDir = "#if" + weekday;
    string ifNotDir = "#ifNot" + weekday;
    string elseDir = "#" + weekday + "Else";
    string endifDir = "#" + weekday + "Endif";
    res = prePr.SetConditionalStripping(
        isTheDay && weekdayActive, ifDir, ifNotDir, endifDir, elseDir);
    res2 = prePr.Strip();
    string message = prePr.GetMessage();
    if (!message.empty())
      *itsLogFile << "*** ERROR: " << message << endl;
    if (!res || !res2)
    {
      *itsLogFile << "*** ERROR: Preprocessing failed" << endl;
      return false;
    }
    tim.ChangeByDays(1);
    // isToday = false;
  }

  string outString = prePr.GetString();
  ofstream file(outFileName.c_str());
  if (!file)
  {
    *itsLogFile << "*** ERROR: Failed to save preprocessed file to '" << outFileName.c_str() << "'"
                << std::endl;
    return false;
  }

  file << outString;
  file.close();
  file.clear();

  size_t pos = outString.find("$#");
  if (pos != string::npos)
  {
    string newString = outString.substr(pos, 3);

    *itsLogFile << "*** ERROR: Undefined variable: " << newString << endl;
  }

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theWeekday Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------
int NFmiPressTimeDescription::Weekday2Num(const NFmiString& theWeekday) const
{
  NFmiString theString(theWeekday);
  theString.LowerCase();
  int num = 0;
  if (theString == "mo" || theString == "ma")
    num = 1;
  else if (theString == "tu" || theString == "ti")
    num = 2;
  else if (theString == "we" || theString == "ke")
    num = 3;
  else if (theString == "th" || theString == "to")
    num = 4;
  else if (theString == "fr" || theString == "pe")
    num = 5;
  else if (theString == "sa" || theString == "la")
    num = 6;
  else if (theString == "su")
    num = 7;
  return num;
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
  unsigned long format = kWwsHH;  // oletus

  helpString = ReadString();  // tuo myˆs v‰lilyˆnnit hipsuissa

  if (helpString == NFmiString("HHdMM"))
    format = kHHdMM;
  else if (helpString == NFmiString("HdMM"))
    format = kHdMM;
  else if (helpString == NFmiString("wwsHH") || helpString == NFmiString("WwsHH"))
    format = kWwsHH;
  else if (helpString == NFmiString("DdMd"))
    format = kDdMd;
  else if (helpString == NFmiString("WwsDDdMMd"))
    format = kWwsDDdMMd;
  else if (helpString == NFmiString("WwsDdMd"))
    format = kWwsDdMd;
  else if (helpString == NFmiString("NnnnsYYYY"))
    format = kNnnnsYYYY;
  else if (helpString == NFmiString("DDdMMdYYYY"))
    format = kDDdMMdYYYY;
  else if (helpString == NFmiString("DdMdYYYY"))
    format = kDdMdYYYY;
  else
  {
    if (isSecond)
      itsSecondStringNameTimeFormat = helpString;
    else
      itsStringNameTimeFormat = helpString;
  }
  return format;
}

// ----------------------------------------------------------------------
/*!
 * Read the time table looping definition (start, step and optional stop)
 */
// ----------------------------------------------------------------------

void NFmiPressTimeDescription::ReadTimeTableActive()
{
  long long1 = 0;
  long long2 = 0;
  long long3 = 0;
  NFmiValueString valueString;

  if (!ReadEqualChar())
    return;

  long3 = 1000;
  if (ReadTwo(long1, long2))
  {
    *itsDescriptionFile >> itsObject;
    valueString = itsObject;
    if (valueString.IsNumeric())
    {
      long3 = static_cast<long>(valueString);

      *itsDescriptionFile >> itsObject;
      itsString = itsObject;
    }
    else
    {
      // NOLINTNEXTLINE(cppcoreguidelines-slicing): only the formatted text is wanted
      itsString = valueString;
    }

    if (long1 > 0 && long2 > 0 && long3 > 0 && long1 <= long3)
    {
      itsLoopActivity.startIndex = long1;
      itsLoopActivity.step = long2;
      itsLoopActivity.stopIndex = long3;
    }
    else
      *itsLogFile << "*** ERROR: Not valid looping (start,step,stop): " << long1 << long2
                  << long3 << endl;
  }

  itsIntObject = ConvertDefText(itsString);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::ReadRemaining()
{
  long long1 = 0;
  switch (itsIntObject)
  {
    case dDayAddition:  // toimii vain Segmentin p‰‰tasolla
    {
      if (!ReadEqualChar())
        break;

      if (ReadOne(long1))
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
      ReadTimeTableActive();

      break;
    }
    case dBypassValue:
    {
      SetOne(itsLoopActivity.bypassValue);
      itsLoopActivity.bypassWithValue = true;

      break;
    }
    // image osannee k‰ytt‰‰ t‰‰lt‰ P‰iv‰n ja Tunnin vaikka muilla oma
    case dRelDay:
    {
      if (!ReadEqualChar())
        break;
      if (ReadLong(long1))
        itsFirstDeltaDays = static_cast<unsigned short>(long1 + itsEnvironment.GetDayAdvance());

      ReadNext();
      break;
    }
    case dHour:
    {
      if (!ReadEqualChar())
        break;
      if (ReadLong(long1))
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

int NFmiPressTimeDescription::ConvertDefText(NFmiString& object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if (lowChar == NFmiString("day") || lowChar == NFmiString("staticday") ||
      lowChar == NFmiString("p‰iv‰") ||
      lowChar == NFmiString("j‰‰dytettyp‰iv‰"))  // ennakon k‰yttˆ ei muuta t‰t‰
    return dRelDay;
  if (lowChar == NFmiString("dayaddition") || lowChar == NFmiString("p‰iv‰lis‰ys"))
    return dDayAddition;
  if (lowChar == NFmiString("minute") || lowChar == NFmiString("minuutti"))
    return dMinute;
  if (lowChar == NFmiString("hour") || lowChar == NFmiString("tunti"))
    return dHour;
  else if (lowChar == NFmiString("relativehours") || lowChar == NFmiString("relativehour") ||
           lowChar == NFmiString("relhour") || lowChar == NFmiString("suhteellinentunti"))
    return dRelHour;
  else if (lowChar == NFmiString("relativetime") || lowChar == NFmiString("relhours") ||
           lowChar == NFmiString("suhteellisettunnit"))  // data-alkioissa
    return dGivenRelHours;
  else if (lowChar == NFmiString("hourstep") || lowChar == NFmiString("tuntiaskel") ||
           lowChar == NFmiString("tuntiaskel"))
    return dHourStep;
  else if (lowChar == NFmiString("utcoutput") || lowChar == NFmiString("utcwriting") ||
           lowChar == NFmiString("utctulostus"))  // data-alkioissa
    return dWriteTimeAsUtc;
  else if (lowChar == NFmiString("timeseriesactive") || lowChar == NFmiString("timetableactive") ||
           lowChar == NFmiString("aikataulukonaktiiviset"))
    return dTimeTableActive;
  else if (lowChar == NFmiString("bypassvalue") || lowChar == NFmiString("ohitusarvo"))
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
  // halutessa olioita voidaan piirt‰‰ esim joka n:teen aika-askeleeseen
  // Mika: Kannattaisikohan laittaa sulkuja?
  return itsLoopActivity.startIndex < 1 ||
         ((currentInd - itsLoopActivity.startIndex) % itsLoopActivity.step == 0 &&
          currentInd >= static_cast<int>(itsLoopActivity.startIndex) &&
          currentInd <= static_cast<int>(itsLoopActivity.stopIndex));
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
  // MetTimelt‰ ei lˆydy palvelua, jolla saisi NextTimen kun resoluutio ei ala nollasta
  //     eli esim ajat ovat 2,8 14,20,2.... tai aina 12 (hourDelta)
  // esim nyt = 8.2. klo 8.30 utc ->
  //	         NextUseTime(4,24,12) -> 9.2. klo 12 utc
  //	         NextUseTime(3,24,12) -> 8.2. klo 12
  //	         NextUseTime(-3,6,1)  -> 8.2. klo 7   utc
  NFmiMetTime curTime(60);
  curTime.ChangeByHours(relHours);
  NFmiMetTime compTime(60);
  compTime.ChangeByDays(-2);  // riitt‰‰kˆ
  compTime.SetHour(static_cast<short>(hourDelta));
  if (compTime >= curTime)
    *itsLogFile << "*** ERROR: too long time difference: " << relHours << endl;
  else
  {
    while (compTime <= curTime)
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
  // ei kyll‰ ihan tarkka, ja toimii vain yhdelle vuodelle toistaiseksi
  long ret = dayDiff;
  short add = 0;
  NFmiTime now;
  NFmiTime compDate = now;
  short sign = +1;  // onko c:ss‰
  if (dayDiff < 0)
    sign = -1;
  compDate.SetYear(now.GetYear() + sign);
  if (abs(abs(dayDiff) - 365) < 5)
    if (abs(now.DifferenceInDays(compDate)) == 365)
      add = 0;
    else
      add = 1;
  else
    add = 0;

  return ret + add * sign;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressTimeDescription::SetPreReadingTimes()
{
  NFmiTime curDate;
  curDate.SetHour(0);
  curDate.SetMin(0);
  // NOLINTNEXTLINE(cppcoreguidelines-slicing): only the time is wanted, not the time step
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

void NFmiPressTimeDescription::SetPostReadingTimes()
{
  itsFirstPlotTime.ChangeByDays(itsFirstDeltaDays - itsFirstOriginalDeltaDays);
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

bool NFmiPressTimeDescription::SetDataHour(NFmiFastQueryInfo* data, const NFmiString& calledFrom)
{
  // Kun luupataan p‰iv‰n sis‰ll‰ ei voi k‰ytt‰‰ omaa tuntia
  // vaan k‰ytet‰‰n pressparamin aikaluuppi-arvoja
  // pidemmiss‰ (>vrk) luupeissa pit‰isi oma arvo toimia t‰ss‰kin
  if (!IsHourLoop())
  {
    NFmiMetTime saveTime = (static_cast<NFmiQueryInfo*>(data))->Time();
    NFmiMetTime tim = saveTime;
    if (!(tim.GetHour() == itsFirstPlotHours))
      *itsLogFile << "    hour changed in plot element: " << itsFirstPlotHours << " utc" << endl;

    tim.SetTime(itsFirstPlotHours);
    if (!data->Time(tim))
    {
      if (itsLogFile)
        *itsLogFile << "*** ERROR: missing time: " << tim << static_cast<char*>(calledFrom) << endl;
      data->Time(saveTime);  // pit‰‰ asettaa takaisin
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

NFmiMetTime NFmiPressTimeDescription::TimeToWrite(const NFmiMetTime& givenTime) const
{
  if (fWriteAsUtc)
    return givenTime;
      return NFmiMetTime(givenTime.LocalTime(25.F), 1);
}

// ======================================================================
