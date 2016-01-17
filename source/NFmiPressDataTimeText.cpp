// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressDataTimeText
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressDataTimeText.h"
#include "NFmiFastQueryInfo.h"

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressDataTimeText::~NFmiPressDataTimeText(void) {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressDataTimeText::ReadRemaining(void)
{
  NFmiString helpString;
  NFmiValueString valueString;
  switch (itsIntObject)
  {
    case dUseOriginTime:
    {
      fUseOriginTime = true;

      ReadNext();
      break;
    }
    default:
    {
      NFmiPressTimeText::ReadRemaining();
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

int NFmiPressDataTimeText::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();  // kaikille pit‰isi sallia vapaa isot/pienet kirj.
  if (lowChar == NFmiString("hour") || lowChar == NFmiString("tunti") ||
      lowChar == NFmiString("day") || lowChar == NFmiString("p‰iv‰"))
    *itsLogFile << "WARNING: Day or hour moved from #time to #pressparam level" << endl;

  if (lowChar == NFmiString("analysistime") || lowChar == NFmiString("analyysiaika"))
    return dUseOriginTime;
  else
    return NFmiPressTimeText::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * Kutsutaan segmentist‰: GivenTimeText pit‰‰ p‰ivitt‰‰ oikealla ajalla
 * ainakin aikataulukosas
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
//---------------------------------------------------------------------------

bool NFmiPressDataTimeText::WritePSUpdatingSubText(FmiPressOutputMode theOutput)
{
  ScalePlotting();

  NFmiMetTime utcTime;
  NFmiMetTime localTime;

  if (fUseOriginTime)
  {
    utcTime = (static_cast<NFmiMetTime>(itsData->OriginTime()));
    localTime = (static_cast<NFmiMetTime>(itsData->OriginTime())).LocalTime();
  }
  else
  {
    utcTime = (static_cast<NFmiMetTime>((static_cast<NFmiQueryInfo *>(itsData))->Time()));
    localTime =
        (static_cast<NFmiMetTime>((static_cast<NFmiQueryInfo *>(itsData))->Time())).LocalTime();
  }

  if (itsStringNameTimeFormat.IsValue())
  {
    if (fWriteAsUtc)
      SetText((static_cast<NFmiPressTime>(utcTime))
                  .InterpretToStr(itsStringNameTimeFormat, itsLanguage));
    else
      SetText((static_cast<NFmiPressTime>(localTime))
                  .InterpretToStr(itsStringNameTimeFormat, itsLanguage));
  }
  else
  {
    if (fWriteAsUtc)
      SetText((static_cast<NFmiPressTime>(utcTime)).ToStr(itsFormat, itsLanguage));
    else
      SetText((static_cast<NFmiPressTime>(localTime)).ToStr(itsFormat, itsLanguage));
  }

  if (itsSubText) itsSubText->SetTime(NFmiMetTime(utcTime));

  return WriteString(NFmiString("AIKATEKSTI"), theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressDataTimeText::WritePS(FmiPressOutputMode theOutput)
{
  ScalePlotting();

  // Mika suspects a bug in the reinterpret cast,
  // since the cast is from NFmiFastQueryInfo * to NFmiQueryData *
  NFmiTime tim = (static_cast<NFmiMetTime>((reinterpret_cast<NFmiQueryData *>(itsData))->Time()))
                     .LocalTime();  // oletus Suomen aika
  SetText((static_cast<NFmiPressTime>(tim)).ToStr(itsFormat, itsLanguage));

  return WriteString(NFmiString("AIKATEKSTI"), theOutput);
}

// ----------------------------------------------------------------------
