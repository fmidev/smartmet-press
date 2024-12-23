// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressNameDay
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressNameDay.h"
#include "NFmiSettings.h"
#include <iostream>
using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressNameDay::~NFmiPressNameDay(void) {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressNameDay::ReadRemaining(void)
{
  unsigned long long1;
  FmiCounter counter;
  switch (itsIntObject)
  {
    case dMaxLength:
    {
      if (!ReadEqualChar())
        break;

      if (ReadOne(counter))
      {
        itsMaxLength = counter;
      }

      ReadNext();
      break;
    }
    case dMaxNumber:
    {
      if (!ReadEqualChar())
        break;

      if (ReadOne(counter))
      {
        itsMaxNumber = counter;
      }

      ReadNext();
      break;
    }
    case dRelDay:
    {
      if (!ReadEqualChar())
        break;

      if (ReadOne(long1))
      {
        itsFirstPlotTime = NFmiMetTime();
        itsFirstPlotTime.ChangeByDays(long1 + itsEnvironment.GetDayAdvance());
      }

      ReadNext();
      break;
    }

    default:
    {
      NFmiPressText::ReadRemaining();
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

int NFmiPressNameDay::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();
  if (lowChar == NFmiString("maxnumber") || lowChar == NFmiString("maksimimäärä"))
    return dMaxNumber;
  else if (lowChar == NFmiString("maxlength") || lowChar == NFmiString("maksimipituus"))
    return dMaxLength;
  else
    return NFmiPressText::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressNameDay::WritePS(FmiPressOutputMode theOutput)
{
  if (!itsNameDay)
  {
    *itsLogFile << "*** ERROR: Nameday Object missing, must be program bug" << endl;
    return false;
  }
  ScalePlotting();

  NFmiString str;

  if (!itsNameDay->IsRead())
  {
#ifndef UNIX
    NFmiString fileName = GetHome();
    fileName += kFmiDirectorySeparator;
    fileName += NFmiString("Muut");
    fileName += kFmiDirectorySeparator;
#else
    NFmiString fileName = static_cast<NFmiString>(NFmiSettings::Require<string>("press::incpath"));
    fileName += "/";
#endif
    if (itsLanguage == kLatvian)
    {
      fileName += NFmiString("namedays_lat.txt");
    }
    else if (itsLanguage == kSwedish)
    {
      fileName += NFmiString("nimipäivätRuotsi.txt");
    }
    else
    {
      fileName += NFmiString("nimipäivät.txt");
    }
    if (!itsNameDay->ReadFile(fileName))
    {
      *itsLogFile << "*** ERROR: Reading of nameday file failed" << endl;
      return false;
    }
  }

  if (itsNameDay->IsValue())
  {
    str = itsNameDay->GetName(itsFirstPlotTime, itsMaxNumber, itsMaxLength);
    SetText(str);
    *itsLogFile << "  Nameday: " << static_cast<char *>(str) << endl;

    if (itsLanguage != kFinnish && str == "Karkauspäivä")
      SetText("Leap day");

    if (itsLanguage == kSwedish && str == "Karkauspäivä")
      SetText("Skottdag");

    return WriteString(NFmiString("NameDay"), theOutput);
  }
  else
    return false;
}

// ======================================================================
