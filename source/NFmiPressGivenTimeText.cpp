// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressGivenTimeText
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressGivenTimeText.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
//----------------------------------------------------------------------

NFmiPressGivenTimeText::~NFmiPressGivenTimeText(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressGivenTimeText::ReadRemaining(void)  
{
  unsigned long long1;
  switch(itsIntObject)
	{	
	case dRelDay:     
	  {
		if (!ReadEqualChar())
		  break;
		
		if(ReadOne(long1))
		  {			
			itsFirstPlotTime = NFmiMetTime();
			itsFirstPlotTime.ChangeByDays(long1+itsEnvironment.GetDayAdvance());
		  }
		
		ReadNext();
		break;
	  }
	case dHour:       
	  {
		if (!ReadEqualChar())
		  break;
		
		if(ReadOne(long1))    
		  itsFirstPlotTime.SetHour(static_cast<short>(long1));
		
		ReadNext();
		break;
	  }
	default:
	  {
		NFmiPressTimeText:: ReadRemaining();
		break;
	  }
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressGivenTimeText::WritePS(FmiPressOutputMode theOutput)
{
   ScalePlotting();

  SetText();
  return WriteString(NFmiString("AnnettuAikaTeksti"), theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressGivenTimeText::SetText(void)
{
  if(itsStringNameTimeFormat.IsValue())
	{
	  if(fWriteAsUtc)
		SetText((static_cast<NFmiPressTime>(itsFirstPlotTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
	  else
		SetText((static_cast<NFmiPressTime>(itsFirstPlotTime.LocalTime(25.f))).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
	}
  else
	{
	  if(fWriteAsUtc)
		SetText((static_cast<NFmiPressTime>(itsFirstPlotTime)).ToStr(itsFormat,itsLanguage));
	  else
		SetText((static_cast<NFmiPressTime>(itsFirstPlotTime.LocalTime(25.f))).ToStr(itsFormat,itsLanguage));
	}
  return true;
}

// ----------------------------------------------------------------------
