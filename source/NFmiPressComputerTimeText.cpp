// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressComputerTimeText
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressComputerTimeText.h"

#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressComputerTimeText::~NFmiPressComputerTimeText(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressComputerTimeText::SetText(void)
{
  NFmiMetTime computerTime = NFmiMetTime(1);
  NFmiMetTime useTime = TimeToWrite(computerTime);
  useTime.ChangeByDays(itsDeltaDays);  
  if(itsStringNameTimeFormat.IsValue())
	SetText((static_cast<NFmiPressTime>(useTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
  else
	SetText((static_cast<NFmiPressTime>(useTime)).ToStr(itsFormat,itsLanguage));
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressComputerTimeText::ReadRemaining(void)  
{
  long long1;
  
  switch(itsIntObject)
	{
	case dTimeTextDays:	  
	  {
		if (!ReadEqualChar())
		  break;
		
		if(ReadOne(long1))    
		  itsDeltaDays =  static_cast<short>(long1);
		
		ReadNext();
		break;
	  }
	default: 
	  {
		return NFmiPressTimeText::ReadRemaining();
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

int NFmiPressComputerTimeText::ConvertDefText(NFmiString & object)
{
  if(object==NFmiString("Day") || object==NFmiString("P‰iv‰"))
	return dTimeTextDays;
  else
	return NFmiPressTimeText::ConvertDefText(object);
	
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressComputerTimeText::WritePS(FmiPressOutputMode theOutput)
{
  ScalePlotting();
  
  SetText();
  
  return WriteString(NFmiString("KoneenAika"),theOutput);
}

// ======================================================================
