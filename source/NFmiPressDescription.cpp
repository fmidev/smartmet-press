// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressDescription
 */
// ======================================================================

#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressDescription.h"
#include "NFmiEnumConverter.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressDescription::ReadRemaining(void)
{
  //tässä tulisi olla kaikki mitä ConvertDefText:ssäkin
  FmiRGBColor rgb;
  double r1,r2,r3,r4;
  unsigned long helpLong;
  switch(itsIntObject)
	{
	  // tänne vaikka vain Produktissa ja segmentissä käytössä
	case dPressMaskNumber: //muut kuin tiealueet
	  {
		if (!ReadEqualChar())
		  break;

		NFmiString maskName = ReadString();
		string stdString(maskName);
		NFmiEnumConverter converter(kPressRegions);
		helpLong = converter.ToEnum(stdString);

		if(helpLong == 0)
		  *itsLogFile << "*** ERROR: Not valid PressMask: "
					  << static_cast<char *>(maskName)
					  << endl;

		else
		{
		  itsEnvironment.SetMaskNumber(helpLong);
		  itsEnvironment.SetEnumSpace(kPressRegions);
		}

		ReadNext();

		break;
	  }
	case dMaskNumber: //vain tiealueet
	  {
		if (!ReadEqualChar())
		  break;

		NFmiString maskName = ReadString();
		string stdString(maskName);
		NFmiEnumConverter converter(kRoadRegions);
		helpLong = converter.ToEnum(stdString);

		if(helpLong == 0)
		  *itsLogFile << "*** ERROR: Not valid RoadMask: "
					  << static_cast<char *>(maskName)
					  << endl;

		else
		{
		  itsEnvironment.SetMaskNumber(helpLong);
		  itsEnvironment.SetEnumSpace(kRoadRegions);
		}

		ReadNext();

		break;
	  }

	case dLanguage:
	  {
		if (!ReadEqualChar())
		  break;

		itsLanguage = ReadLanguage();

		ReadNext();
		break;
	  }
	case dLongNumberMinus:
	  {
		itsEnvironment.SetLongNumberMinus();

		ReadNext();
		break;
	  }
	case dShortNumberMinus:
	  {
		itsEnvironment.SetShortNumberMinus();

		ReadNext();
		break;
	  }
	case dRGBColor:
	  {
		if (SetThree (rgb.red, rgb.green,rgb.blue))
		  itsEnvironment.SetRGB(rgb);
		break;
	  case dColor:
		{
		  if (!ReadEqualChar())
			break;
		  if(Read4Double(r1,r2,r3,r4))
			{
			  FmiGenericColor color;
			  color.color.cmyk.cyan = r1;
			  color.color.cmyk.magenta = r2;
			  color.color.cmyk.yellow = r3;
			  color.color.cmyk.black = r4;
			  color.isRgb = false;
			  SetColor(color);
			}
		  ReadNext();
		  break;
		}

	  }
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

FmiLanguage NFmiPressDescription::ReadLanguage(void)
{
  // pitää vielä ylläpitää myös PressProduktissa(25.8.00)
  NFmiString helpString;
  FmiLanguage language = kFinnish;

  *itsDescriptionFile >> itsObject;
  helpString = itsObject;
  helpString.LowerCase();
  if (helpString == NFmiString ("suomi") ||
	  helpString == NFmiString ("finnish"))
	language = kFinnish;
  else if (helpString == NFmiString ("ruotsi") ||
		   helpString == NFmiString ("swedish"))
	language = kSwedish;
  else if (helpString == NFmiString ("englanti") ||
		   helpString == NFmiString ("english"))
	language = kEnglish;
  else if (helpString == NFmiString ("saksa") ||
		   helpString == NFmiString ("germany"))
	language = kGermany;
  else if (helpString == NFmiString ("espanja") ||
		   helpString == NFmiString ("spanish"))
	language = kSpanish;
  else if (helpString == NFmiString ("ranska") ||
		   helpString == NFmiString ("french"))
	language = kFrench;
  else if (helpString == NFmiString ("kiina") ||
		   helpString == NFmiString ("chinese"))
	language = kChinese;
  else if (helpString == NFmiString ("saame")) 
	language = kSaame;
  else
	*itsLogFile << "*** ERROR: Tuntematon kieli: "
				<< static_cast<char *>(helpString)
				<< endl;
  return language;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiPressDescription::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();	// kaikille pitäisi sallia vapaa isot/pienet kirj.

  short len = lowChar.GetLen();
  if (!itsCommentLevel &&
	  lowChar.GetChars(1,1) == NFmiString("#") &&
	  ((len > 8 && lowChar.GetChars(len-4,5) == NFmiString("loppu")) ||
	   (len > 6 && lowChar.GetChars(len-2,3) == NFmiString("end"))))
	{
	  ReadNext();
	  return dEnd;
	}
  else if(lowChar==NFmiString("mask") ||
		  lowChar==NFmiString("maski"))
	return dMaskNumber; //roadregion
  else if(lowChar==NFmiString("areamask") ||
		  lowChar==NFmiString("aluemaski"))
	return dPressMaskNumber; //other region
  else if(lowChar==NFmiString("producer") ||
		  lowChar==NFmiString("tuottaja"))
	return dProducer;
  else if(lowChar==NFmiString("longminus") ||
		  lowChar==NFmiString("numerotpitkälläviivalla") ||
		  lowChar==NFmiString("pitkälläviivalla"))
	return dLongNumberMinus;
  else if(lowChar==NFmiString("shortminus") ||
		  lowChar==NFmiString("numerotlyhyelläviivalla") ||
		  lowChar==NFmiString("lyhyelläviivalla"))
	return dShortNumberMinus;
  else if(lowChar==NFmiString("language") ||
		  lowChar==NFmiString("kieli"))
	return dLanguage;
  else if(lowChar==NFmiString("rgbcolor") ||
		  lowChar==NFmiString("rgbväri"))
	return dRGBColor;
  else if(lowChar==NFmiString("color") ||
		  lowChar==NFmiString("väri") ||
		  lowChar==NFmiString("cmykväri"))
	return dColor;
  else
	return NFmiDescription :: ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressDescription::IsPureBlack(void)
{
  return (GetColor().color.cmyk.cyan <= 0. &&
		  GetColor().color.cmyk.magenta <= 0. &&
		  GetColor().color.cmyk.yellow <= 0. &&
		  GetColor().color.cmyk.black >= 1.);
}

// ======================================================================

