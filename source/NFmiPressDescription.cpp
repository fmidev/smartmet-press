// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressDescription
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ kääntäjän varoitusta
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

FmiDirection NFmiPressDescription::String2FmiDirection(const NFmiString &theString) const
{
  NFmiString lowChar = theString;
  lowChar.LowerCase();

  if (lowChar == NFmiString("center") || lowChar == NFmiString("centre") ||
      lowChar == NFmiString("middle") || lowChar == NFmiString("keskipiste") ||
      lowChar == NFmiString("keski"))
    return kCenter;

  else if (lowChar == NFmiString("right") || lowChar == NFmiString("oikealaita") ||
           lowChar == NFmiString("oikea"))
    return kRight;

  else if (lowChar == NFmiString("left") || lowChar == NFmiString("vasenlaita") ||
           lowChar == NFmiString("vasen"))
    return kLeft;
  else
    return kNoDirection;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressDescription::ReadRemaining(void)
{
  // tässä tulisi olla kaikki mitä ConvertDefText:ssäkin
  FmiRGBColor rgb = FmiRGBColor();
  double r1, r2, r3, r4;
  float f1;
  unsigned int i1;
  unsigned long helpLong;
  FmiEnumSpace helpEnumSpace = kPressRegions;
  switch (itsIntObject)
  {
    // tänne vaikka vain Produktissa ja segmentissä käytössä
    case dMaskNumber:  // vain tiealueet
      helpEnumSpace = kRoadRegions;
    case dPressMaskNumber:  // muut kuin tiealueet
    {
      if (!ReadEqualChar()) break;

      NFmiString maskName = ReadString();
      string stdString(maskName);
      NFmiEnumConverter converter(helpEnumSpace);
      helpLong = converter.ToEnum(stdString);

      if (helpLong == 0)
      {
        *itsLogFile << "*** ERROR: Not valid PressMask: " << static_cast<char *>(maskName)
                    << " in space " << static_cast<int>(helpEnumSpace) << endl;
      }

      else
      {
        itsEnvironment.SetMask(helpLong, helpEnumSpace);
      }

      ReadNext();

      break;
    }
    case dProducer:
    {
      SetOne(helpLong);  // luetaan vain pois koska tuottaja hylätty
      break;
    }

    case dLanguage:
    {
      if (!ReadEqualChar()) break;

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
      if (SetThree(rgb.red, rgb.green, rgb.blue)) itsEnvironment.SetRGB(rgb);
      break;
      case dColor:
      {
        if (!ReadEqualChar()) break;
        if (Read4Double(r1, r2, r3, r4))
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
      case dDescFont:
      {
        if (!ReadEqualChar()) break;

        // itsOutFileName = ReadString();
        itsEnvironment.SetFont(ReadString());

        ReadNext();
        break;
      }
      case dDescSymbolSet:
      {
        if (!ReadEqualChar()) break;

        itsEnvironment.SetSymbolSet(ReadString());

        ReadNext();
        break;
      }
      case dSymbolSizeFactor:
      {
        if (!ReadEqualChar()) break;

        ReadOne(f1);
        itsEnvironment.SetSymbolSizeFactor(f1);

        ReadNext();
        break;
      }
      case dDescTextSize:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(r1)) itsEnvironment.SetTextSize(r1);

        ReadNext();
        break;
      }
      case dDescTextAlignment:
      {
        if (!ReadEqualChar()) break;

        *itsDescriptionFile >> itsObject;
        itsString = itsObject;

        FmiDirection dir = String2FmiDirection(itsString);

        if (dir != kNoDirection) itsEnvironment.SetTextAlignment(dir);
        // itsAlignment = dir;
        else
          *itsLogFile << "*** ERROR: Unknown alignment: " << static_cast<char *>(itsObject) << endl;
        ReadNext();
        break;
      }
      case dMissingPrecedingImage:
      {
        SetOnlyForMissingPrecedingElementFlag(true);

        ReadNext();
        break;
      }
      case dVersion:
      {
        SetOne(i1);
        itsEnvironment.SetVersion(i1);
        *itsLogFile << "version: " << i1 << endl;

        break;
      }
      case dSegmentMove:
      {
        SetTwo(r1, r2);
        itsEnvironment.SetSegmentMove(NFmiPoint(r1, r2));

        break;
      }
      case dAvoidOrphanSyllables:
      {
        itsEnvironment.AvoidOrphanSyllables(true);
        ReadNext();

        break;
      }
      case dUseBackupPreviousDay:
      {
        itsEnvironment.UseBackupPreviousDay(true);
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
  if (helpString == NFmiString("suomi") || helpString == NFmiString("finnish"))
    language = kFinnish;
  else if (helpString == NFmiString("ruotsi") || helpString == NFmiString("swedish"))
    language = kSwedish;
  else if (helpString == NFmiString("englanti") || helpString == NFmiString("english"))
    language = kEnglish;
  else if (helpString == NFmiString("saksa") || helpString == NFmiString("germany"))
    language = kGermany;
  else if (helpString == NFmiString("espanja") || helpString == NFmiString("spanish"))
    language = kSpanish;
  else if (helpString == NFmiString("ranska") || helpString == NFmiString("french"))
    language = kFrench;
  else if (helpString == NFmiString("kiina") || helpString == NFmiString("chinese"))
    language = kChinese;
  else if (helpString == NFmiString("saame"))
    language = kSaame;
  else if (helpString == NFmiString("latvia") || helpString == NFmiString("latvian") ||
           helpString == NFmiString("lettish"))
    language = kLatvian;
  else
    *itsLogFile << "*** ERROR: Unknown language: " << static_cast<char *>(helpString) << endl;
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

int NFmiPressDescription::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();  // kaikille pitäisi sallia vapaa isot/pienet kirj.

  unsigned long len = lowChar.GetLen();
  if (!itsCommentLevel && lowChar.GetChars(1, 1) == NFmiString("#") &&
      ((len > 8 && lowChar.GetChars(len - 4, 5) == NFmiString("loppu")) ||
       (len > 6 && lowChar.GetChars(len - 2, 3) == NFmiString("end"))))
  {
    ReadNext();
    return dEnd;
  }
  else if (lowChar == NFmiString("mask") || lowChar == NFmiString("maski"))
    return dMaskNumber;  // roadregion
  else if (lowChar == NFmiString("areamask") || lowChar == NFmiString("aluemaski"))
    return dPressMaskNumber;  // other region
  else if (lowChar == NFmiString("producer") || lowChar == NFmiString("tuottaja"))
    return dProducer;
  else if (lowChar == NFmiString("longminus") || lowChar == NFmiString("numerotpitkälläviivalla") ||
           lowChar == NFmiString("pitkälläviivalla"))
    return dLongNumberMinus;
  else if (lowChar == NFmiString("shortminus") ||
           lowChar == NFmiString("numerotlyhyelläviivalla") ||
           lowChar == NFmiString("lyhyelläviivalla"))
    return dShortNumberMinus;
  else if (lowChar == NFmiString("language") || lowChar == NFmiString("kieli"))
    return dLanguage;
  else if (lowChar == NFmiString("rgbcolor") || lowChar == NFmiString("rgbväri"))
    return dRGBColor;
  else if (lowChar == NFmiString("cmykcolor") || lowChar == NFmiString("color") ||
           lowChar == NFmiString("väri") || lowChar == NFmiString("cmykväri"))
    return dColor;
  else if (lowChar == NFmiString("symbolset") || lowChar == NFmiString("symbolisetti") ||
           lowChar == NFmiString("kuvakansio") || lowChar == NFmiString("symbolikansio"))
    return dDescSymbolSet;
  else if (lowChar == NFmiString("additionsymbolscale") ||
           lowChar == NFmiString("additionalsymbolscale") ||
           lowChar == NFmiString("kuvienuudelleenskaalaus") ||
           lowChar == NFmiString("symbolienuudelleenskaalaus"))
    return dSymbolSizeFactor;
  else if (lowChar == NFmiString("font") || lowChar == NFmiString("fontti") ||
           lowChar == NFmiString("kirjasin"))
    return dDescFont;
  else if (lowChar == NFmiString("textsize") || lowChar == NFmiString("tekstikoko"))
    return dDescTextSize;
  else if (lowChar == NFmiString("textalignment") || lowChar == NFmiString("tekstikohdistus"))
    return dDescTextAlignment;
  else if (lowChar == NFmiString("onlyformissingprecedingimage") ||
           lowChar == NFmiString("vainedellisenkuvanpuuttuessa"))
    return dMissingPrecedingImage;
  else if (lowChar == NFmiString("version") || lowChar == NFmiString("versio"))
    return dVersion;
  else if (lowChar == NFmiString("segmentmove") || lowChar == NFmiString("segmentinsiirto"))
    return dSegmentMove;
  else if (lowChar == NFmiString("avoidorphansyllables") ||
           lowChar == NFmiString("vältäorpotavuja") || lowChar == NFmiString("vältäorpojatavuja"))
    return dAvoidOrphanSyllables;

  else if (lowChar == NFmiString("usebackuppreviousday") ||
           lowChar == NFmiString("käytäeilistätarvittaessa"))
    return dUseBackupPreviousDay;
  else
    return NFmiDescription::ConvertDefText(object);
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
  return (GetColor().color.cmyk.cyan <= 0. && GetColor().color.cmyk.magenta <= 0. &&
          GetColor().color.cmyk.yellow <= 0. && GetColor().color.cmyk.black >= 1.);
}

// ======================================================================
