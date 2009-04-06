// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressTimeText
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressTimeText.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressTimeText::~NFmiPressTimeText(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param newLanguage Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressTimeText:: SetLanguage(FmiLanguage newLanguage)		
{
  if(newLanguage == kChinese &&
	 (itsOrigFormat == kShortWeekDay || itsOrigFormat == kI))
	{
	  itsFormat = kI;
	  SetFont("Cviikko");
	  //itsFont = NFmiString("Cviikko");
	}
  else if(itsLanguage == kChinese) //mutta uusi ei
	{
	  itsFormat = itsOrigFormat;
	  SetFont(itsOrigFont);
	  //itsFont = itsOrigFont;
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

bool NFmiPressTimeText::ReadDescription(NFmiString & retString)
{
  NFmiValueString valueString;
  NFmiString helpString;
  double r1,r2;
  
  //itsFont = NFmiString("Times-Roman");
  
  *itsDescriptionFile >> itsObject;
  itsString = itsObject;                     
  itsIntObject = ConvertDefText(itsString);

  itsRectSize.Y(GetTextSize());
  
  while(itsIntObject != dEnd || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel)
		itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #AikaTekstiss‰" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:	  
		  {    
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #AikaTekstiss‰: "
						  << static_cast<char *>(itsObject)
						  << endl;  
			ReadNext();
			break;
		  }
		case dComment:	  
		case dEndComment:	  
		  {
			ReadNext();
			break;
		  }
		case dPrintOnce:
		  {
			SetPrintOnceOn();
			
			ReadNext();
			break;
		  }
		case dSymbolPlace:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read2Double(r1,r2))
			  {
				Place(NFmiPoint(r1,r2));
				//riitt‰‰ kun antaa joko Paikan tai RotPaikan
				itsRotatingPoint = NFmiPoint(r1,r2);
			  }
			ReadNext();
			break;
		  }
		case dPsPlaceMove:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read2Double(r1,r2))
			  {
				Move(NFmiPoint(r1,r2));
			  }
			ReadNext();
			break;
		  }
		case dCharSpace:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				itsCharSpace = r1;
			  }
			ReadNext();
			break;
		}
		case dTimeTextFormat:
		  {
			if (!ReadEqualChar())
			  break;
			
			itsFormat = ReadTimeFormat();
			
			ReadNext();
			break;
		  }
		case dSymbolSize:
		  {
			if (!ReadEqualChar())
			  break;
			
			if(ReadDouble(r1))       
			  {	
				itsRectSize.Y(r1);
			  }
			
			ReadNext();
			break;
		  }
		case dUpperCase:
		  {
			fUpperCase = true;
			
			ReadNext();
			break;
		  }
		case dLowerCase:
		{
		  fLowerCase = true;
		  
		  ReadNext();
		  break;
		}
		case dAddInFront:
		  {
			if (!ReadEqualChar())
			  break;
			itsAddInFront = ReadString();
			
			ReadNext();
			break;
		  }
		case dAddAfter:
		  {
			if (!ReadEqualChar())
				break;
			itsAddAfter = ReadString();
			
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
  
  itsOrigFormat = itsFormat;
  itsOrigFont = GetFont();
  if(itsLanguage == kChinese &&
	 (itsFormat == kShortWeekDay || itsFormat == kI))
	{
	  itsFormat = kI;
	  SetFont(NFmiString("Cviikko"));
	}
  
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

int NFmiPressTimeText::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase(); // kaikille pit‰isi sallia vapaa isot/pienet kirj.
  if(lowChar==NFmiString("format") || lowChar==NFmiString("formaatti"))
	return dTimeTextFormat;
  else
	return NFmiPressText::ConvertDefText(object);
}

// ======================================================================
