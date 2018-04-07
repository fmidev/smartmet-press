// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressText
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressText.h"
#include "NFmiFileString.h"
#include "NFmiFileSystem.h"
#include "NFmiHyphenationString.h"
#include "NFmiPressComputerTimeText.h"
#include "NFmiPressDataTimeText.h"
#include "NFmiPressGivenTimeText.h"
#include <iostream>

extern std::list<std::string> errors;

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes some internal dynamic variables
 * \todo Remove the ifs as unnecessary
 */
// ----------------------------------------------------------------------

NFmiPressText::~NFmiPressText(void)
{
  if (itsText) delete itsText;
  if (itsSubText) delete itsSubText;
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 *†\param thePressText The object to copy
 * \todo Do everything in the initialization list
 */
// ----------------------------------------------------------------------

NFmiPressText::NFmiPressText(const NFmiPressText &thePressText)
    //: NFmiPressScaling()
    : NFmiPressScaling(thePressText),
      itsLastLineStep(thePressText.itsLastLineStep),
      itsLineStep(thePressText.itsLineStep),
      itsLineStepFactor(thePressText.itsLineStepFactor),
      itsTopMargin(thePressText.itsTopMargin),
      itsRightMargin(thePressText.itsRightMargin),
      itsLeftMargin(thePressText.itsLeftMargin),
      itsIndent(thePressText.itsIndent),
      fRightJustification(thePressText.fRightJustification),
      fLoopErrorReported(thePressText.fLoopErrorReported),
      fInParagraph(thePressText.fInParagraph),
      fInFreeArea(thePressText.fInFreeArea),
      itsParagraphMove(thePressText.itsParagraphMove),
      fUpperCase(thePressText.fUpperCase),
      fLowerCase(thePressText.fLowerCase),
      fFirstUpperCase(thePressText.fFirstUpperCase),
      fAddLocalTime(thePressText.fAddLocalTime),
      itsCharSpace(thePressText.itsCharSpace),
      itsMaxLen(thePressText.itsMaxLen),
      itsWidthFactor(thePressText.itsWidthFactor),
      itsFreePath(thePressText.itsFreePath),
      itsNextTexts(thePressText.itsNextTexts),
      fNarrowColumn(thePressText.fNarrowColumn)
{
  itsText = thePressText.itsText ? new NFmiString(*thePressText.itsText) : 0;
  itsSubText = thePressText.itsSubText ? new NFmiPressText(*thePressText.itsSubText) : 0;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressText::ReadDescription(NFmiString &retString)
{
  NFmiValueString valueString;
  NFmiString textFile, textPath, textDir;
  double r1, r2, r3, x;
  NFmiPoint point;

  // itsFont = NFmiString("Times-Roman");
  itsRectSize.Y(GetTextSize());

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);
  bool oneMarginSet = false;
  itsText = new NFmiString("ERROR");
  // bool textGiven = false;
  NFmiString headerFont("None");
  double headerSize(0.);
  NFmiString mainHeaderFont("None");
  double mainHeaderSize(0.);

  while (itsIntObject != dEnd || itsCommentLevel)
  {
    if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
    if (itsLoopNum > itsMaxLoopNum)
    {
      if (itsLogFile) *itsLogFile << "*** ERROR: max file length exceeded in #Text" << endl;
      retString = itsString;
      return isFalse;
    }
    itsLoopNum++;
    switch (itsIntObject)
    {
      case dOther:
      {
        if (itsLogFile)
          *itsLogFile << "*** ERROR: Unknown word in #Text: " << static_cast<char *>(itsObject)
                      << endl;
        ReadNext();
        break;
      }
      case dComment:
      {
        ReadNext();
        break;
      }
      case dEndComment:
      {
        ReadNext();
        break;
      }
      case dAddLocalTime:
      {
        fAddLocalTime = true;

        ReadNext();
        break;
      }
      case dSymbolPlace:
      {
        if (!ReadEqualChar()) break;
        if (Read2Double(r1, r2))
        {
          Place(NFmiPoint(r1, r2));
          itsRotatingPoint = NFmiPoint(r1, r2);
        }
        ReadNext();
        break;
      }
      case dPsPlaceMove:
      {
        if (!ReadEqualChar()) break;
        if (Read2Double(r1, r2))
        {
          NFmiPoint point(r1, r2);
          itsMovePlace = point;
          Move(point);
        }
        ReadNext();
        break;
      }
      case dCharSpace:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          itsCharSpace = r1;
        }
        ReadNext();
        break;
      }
      case dTextString:
      {
        if (!ReadEqualChar()) break;
        // itsText = new NFmiString(ReadString());
        *itsText = ReadString();
        // textGiven = true;

        ReadNext();
        break;
      }
      case dSymbolSize:
      {
        if (!ReadEqualChar()) break;

        if (ReadDouble(r1))
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
      case dFirstUpperCase:
      {
        fFirstUpperCase = true;

        ReadNext();
        break;
      }
      case dAddInFront:
      {
        if (!ReadEqualChar()) break;
        *itsDescriptionFile >> itsObject;
        itsAddInFront = itsObject;

        ReadNext();
        break;
      }
      case dAddAfter:
      {
        if (!ReadEqualChar()) break;
        *itsDescriptionFile >> itsObject;
        itsAddAfter = itsObject;

        ReadNext();
        break;
      }
      case dTopMargin:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          itsTopMargin = r1;
          fInParagraph = true;
          fInFreeArea = false;
        }
        ReadNext();
        break;
      }
      case dLeftMargin:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          if (oneMarginSet) fInParagraph = true;
          oneMarginSet = true;
          fInFreeArea = false;
          itsLeftMargin = r1;
        }
        ReadNext();
        break;
      }
      case dRightMargin:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          if (oneMarginSet) fInParagraph = true;
          oneMarginSet = true;
          fInFreeArea = false;
          itsRightMargin = r1;
        }
        ReadNext();
        break;
      }
      case dLineStep:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          itsLineStep = r1;
        }
        ReadNext();
        break;
      }
      case dIndent:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          itsIndent = r1;
        }
        ReadNext();
        break;
      }
      case dLF:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          itsLineStep = r1;
        }
        ReadNext();
        break;
      }
      case dLFFactor:
      {
        if (!ReadEqualChar()) break;
        if (ReadDouble(r1))
        {
          itsLineStepFactor = r1;
        }
        ReadNext();
        break;
      }
      case dParagraphBorders:
      {
        if (!ReadEqualChar()) break;

        if (Read2Double(r1, r2) && ReadDouble(r3))
        {
          if (r3 > r1 + 10. && r2 > -100.)
          {
            itsLeftMargin = r1;
            itsTopMargin = r2;
            itsRightMargin = r3;
            fInParagraph = true;
            fInFreeArea = false;
          }
          else
          {
            *itsLogFile << "*** ERROR: Virheelliset palstanrajat: " << r1 << " " << r2 << " " << r3
                        << endl;
          }
        }
        ReadNext();
        break;
      }
      case dTextBorders:
      {
        if (!ReadEqualChar()) break;

        if (fInParagraph)
          *itsLogFile << "*** ERROR: Paragraph and AreaPath confusion in Text" << endl;

        fInParagraph = false;
        fInFreeArea = true;

        int numPoints = 0;

        valueString = ReadString();
        while (valueString.IsNumeric())
        {
          x = static_cast<float>(valueString);
          valueString = ReadString();
          if (valueString.IsNumeric())
          {
            numPoints++;
            if (numPoints > 100)
            {
              *itsLogFile << "*** ERROR: Probable leakage in Text AreaBorders (>100 points?) "
                          << endl;
              fInFreeArea = false;
            }
            point.X(x);
            point.Y(static_cast<float>(valueString));
            itsFreePath.push_back(point);
            valueString = ReadString();
          }
          else
          {
            *itsLogFile << "*** ERROR: y-coord. missing in Text AreaBorders" << endl;
            fInFreeArea = false;
          }
        }
        if (numPoints < 3)
        {
          *itsLogFile << "*** ERROR: Text AreaBorders only " << numPoints << " points" << endl;
          fInFreeArea = false;
        }
        itsString = valueString;
        itsIntObject = ConvertDefText(valueString);

        break;
      }
      case dFile:
      {
        if (!ReadEqualChar()) break;
        textFile = ReadString();
        // textGiven = true;

        ReadNext();
        break;
      }
      case dTextDir:
      {
        if (!ReadEqualChar()) break;

        textDir = ReadString();
        // textGiven = true;

        ReadNext();
        break;
      }

      case dTextPath:
      {
        if (!ReadEqualChar()) break;

        textPath = ReadString();
        // textGiven = true;

        ReadNext();
        break;
      }
      case dHeaderFont:
      {
        if (!ReadEqualChar()) break;

        headerFont = ReadString();
        ReadNext();
        break;
      }
      case dHeaderSize:
      {
        SetOne(headerSize);
        break;
      }
      case dMainHeaderFont:
      {
        if (!ReadEqualChar()) break;

        mainHeaderFont = ReadString();
        ReadNext();
        break;
      }
      case dMainHeaderSize:
      {
        SetOne(mainHeaderSize);
        break;
      }

      default:
      {
        ReadRemaining();
        break;
      }
    }
  }
  fNarrowColumn = false;
  double height = itsRectSize.Y();
  if (fInParagraph && height > 0.)
    fNarrowColumn = ((itsRightMargin - itsLeftMargin) / height) < 12.;

  if (textFile.IsValue())
  {
    delete itsText;
    itsText = new NFmiString;

    NFmiHyphenationString nextString;

    if (GetTimestampDayGap() != kShortMissing) AddTimeStamp(textFile);

#ifndef UNIX
    NFmiFileString dataFile =
        CreatePath(NFmiString("Tekstit"), textPath, textDir, textFile, NFmiString("txt"));
#else
    NFmiFileString dataFile = textDir + "/" + textFile + ".txt";
#endif

    *itsLogFile << "#Text: " << static_cast<char *>(dataFile);

    std::fstream in(dataFile, ios::in | ios::in);
    const short lineSize = 2800;
    char inBuf[lineSize];
    unsigned long tagBeg, tagBegH, tagEnd;
    bool firstText = true;
    std::string stdNextStr;
    unsigned long posDuobleSpace, numDuobleSpace;

    std::string string0, string1, string2, string3;

    // int textNum = -1;
    // int numCR = 0;
    if (headerFont == NFmiString("None")) headerFont = itsEnvironment.GetFont();
    if (headerSize == 0.) headerSize = GetHeight();

    if (mainHeaderFont == NFmiString("None")) mainHeaderFont = headerFont;
    if (mainHeaderSize == 0.) mainHeaderSize = headerSize;

    bool isHeader, isMainHeader;
    NFmiString baseFont = GetFont();
    double baseSize = itsRectSize.Y();
    double baseLineStep = GetLineStep();
    double baseLineStepFactor = GetLineStepFactor();
    if (in.good())
    {
      while (in.getline(inBuf, lineSize, '\n'))
      {
        isHeader = false;
        isMainHeader = false;
        stdNextStr = inBuf;
        tagBeg = stdNextStr.find(NFmiString("<b"));
        tagBegH = stdNextStr.find(NFmiString("<h"));
        tagEnd = stdNextStr.find(NFmiString(">"));
        if (tagBeg != string::npos && tagEnd != string::npos)
        {
          stdNextStr.erase(tagBeg, tagEnd - tagBeg + 1);
          isHeader = true;

          tagBeg = stdNextStr.find(NFmiString("<"));
          tagEnd = stdNextStr.find(NFmiString(">"));
          if (tagBeg != string::npos && tagEnd != string::npos)
          {
            stdNextStr.erase(tagBeg, tagEnd - tagBeg + 1);
          }
          // else
          //	pot. virhe (lehtohjelmalle k‰y muttei html:n‰)
        }
        if (tagBegH != string::npos && tagEnd != string::npos)
        {
          stdNextStr.erase(tagBegH, tagEnd - tagBegH + 1);
          isMainHeader = true;

          tagBegH = stdNextStr.find(NFmiString("<"));
          tagEnd = stdNextStr.find(NFmiString(">"));
          if (tagBegH != string::npos && tagEnd != string::npos)
          {
            stdNextStr.erase(tagBegH, tagEnd - tagBegH + 1);
          }
          // else
          //	pot. virhe (lehtohjelmalle k‰y muttei html:n‰)
        }
        numDuobleSpace = 0;
        posDuobleSpace = stdNextStr.find("  ");
        while (posDuobleSpace != string::npos)
        {
          stdNextStr.erase(posDuobleSpace, 1);
          posDuobleSpace = stdNextStr.find("  ");
          numDuobleSpace++;
        }
        if (numDuobleSpace > 0)
          *itsLogFile << endl << "  INFO: " << numDuobleSpace << " double spaces removed" << endl;

        nextString = NFmiHyphenationString(stdNextStr);
        if (nextString.GetLen() > itsMaxLen)
        {
          *itsLogFile << endl
                      << "  *** ERROR: teksti katkaistu sallittuun maxpituuteen: " << itsMaxLen
                      << endl
                      << "             menetetty " << nextString.GetLen() - itsMaxLen << " merkki‰"
                      << endl;

          nextString = nextString.GetChars(1, itsMaxLen);
        }

        if (firstText)
        {
          *itsText = nextString;
          if (isHeader)
          {
            SetFont(headerFont);
            SetHeight(headerSize);
            SetLineStep(0.);
            SetLineStepFactor(0.);
          }
          else if (isMainHeader)
          {
            SetFont(mainHeaderFont);
            SetHeight(mainHeaderSize);
            SetLineStep(0.);
            SetLineStepFactor(0.);
          }
          else
          {
            SetFont(baseFont);
            SetHeight(baseSize);
            SetLineStep(baseLineStep);
            SetLineStepFactor(baseLineStepFactor);
          }

          firstText = false;
        }
        else
        {
          NFmiPressText newText(*this);
          newText.SetText(nextString);
          if (isHeader)
          {
            newText.SetFont(headerFont);
            newText.SetHeight(headerSize);
            newText.SetLineStep(0.);
            newText.SetLineStepFactor(0.);
          }
          else if (isMainHeader)
          {
            newText.SetFont(mainHeaderFont);
            newText.SetHeight(mainHeaderSize);
            newText.SetLineStep(0.);
            newText.SetLineStepFactor(0.);
          }
          else
          {
            newText.SetFont(baseFont);
            newText.SetHeight(baseSize);
            newText.SetLineStep(baseLineStep);
            newText.SetLineStepFactor(baseLineStepFactor);
          }

          itsNextTexts.push_back(newText);
        }
      }
      *itsLogFile << "  read" << endl;
      in.close();
    }
    else
    {
      string msg = string("Text not found: ") + dataFile.CharPtr();
      *itsLogFile << endl << "  *** ERROR: " << msg << endl;
      errors.push_back(msg);
      retString = itsString;
      return false;
    }
  }

  retString = itsString;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressText::ReadRemaining(void)
{
  double r1;
  NFmiString helpString;
  NFmiValueString valueString;
  switch (itsIntObject)
  {
    case dTextLanguage:
    {
      if (!ReadEqualChar()) break;

      itsLanguage = ReadLanguage();

      ReadNext();
      break;
    }
    case dParagraphMove:
    {
      if (!ReadEqualChar()) break;
      if (ReadDouble(r1))
      {
        fInParagraph = true;
        fInFreeArea = false;
        *itsDescriptionFile >> itsObject;
        valueString = itsObject;
        if (valueString.IsNumeric())
        {
          itsParagraphMove = NFmiPoint(r1, static_cast<double>(valueString));
          ReadNext();
        }
        else
        {
          itsParagraphMove = NFmiPoint(0, r1);
          itsString = valueString;
          itsIntObject = ConvertDefText(itsString);
        }
      }
      else
        ReadNext();
      break;
    }
    case dInParagraph:
    {
      fInParagraph = true;
      fInFreeArea = false;
      ReadNext();

      break;
    }

    case dSubTextObject:
    {
      if (itsSubText)  // onko mahdollista
        delete itsSubText;
      itsSubText = new NFmiPressText;
      itsSubText->SetHome(GetHome());
      itsSubText->SetEnvironment(itsEnvironment);
      itsSubText->SetLogFile(itsLogFile);
      itsSubText->SetLanguage(itsLanguage);
      itsSubText->SetDescriptionFile(itsDescriptionFile);  // miten on voinut toimia ilman
      if (!itsSubText->ReadDescription(itsString))
      {
        delete itsSubText;
        itsSubText = 0;
      }
      itsIntObject = ConvertDefText(itsString);
      break;
    }
    case dSubTimeTextObject:
    {
      if (itsSubText) delete itsSubText;
      itsSubText = new NFmiPressGivenTimeText;
      itsSubText->SetHome(GetHome());
      itsSubText->SetEnvironment(itsEnvironment);
      itsSubText->SetLogFile(itsLogFile);
      itsSubText->SetTime(itsFirstPlotTime);
      itsSubText->SetLanguage(itsLanguage);
      itsSubText->SetDescriptionFile(itsDescriptionFile);  // miten on voinut toimia ilman
      if (!itsSubText->ReadDescription(itsString))
      {
        delete itsSubText;
        itsSubText = 0;
      }

      itsIntObject = ConvertDefText(itsString);
      break;
    }

    case dSubComputerTimeTextObject:
    {
      if (itsSubText) delete itsSubText;
      itsSubText = new NFmiPressComputerTimeText;
      itsSubText->SetHome(GetHome());
      itsSubText->SetEnvironment(itsEnvironment);
      itsSubText->SetLogFile(itsLogFile);
      itsSubText->SetLanguage(itsLanguage);
      itsSubText->SetDescriptionFile(itsDescriptionFile);  // miten on voinut toimia ilman
      if (!itsSubText->ReadDescription(itsString))
      {
        delete itsSubText;
        itsSubText = 0;
      }

      itsIntObject = ConvertDefText(itsString);
      break;
    }
    case dMaxTextLength:
    {
      SetOne(itsMaxLen);
      break;
    }

    case dWidthFactor:
    {
      SetOne(itsWidthFactor);
      break;
    }

    case dNotRightJustification:
    {
      fRightJustification = false;
      ReadNext();
      break;
    }

    default:
    {
      NFmiPressScaling::ReadRemaining();
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

int NFmiPressText::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();
  if (lowChar == NFmiString("size") || lowChar == NFmiString("textsize") ||
      lowChar == NFmiString("koko") || lowChar == NFmiString("tekstikoko"))
    return dSymbolSize;

  else if (lowChar == NFmiString("language") || lowChar == NFmiString("kieli"))
    return dTextLanguage;

  else if (lowChar == NFmiString("#subtext") || lowChar == NFmiString("#subconsttext") ||
           lowChar == NFmiString("#jatkovakioteksti"))
    return dSubTextObject;

  else if (lowChar == NFmiString("#subtimetext") || lowChar == NFmiString("#jatkoaikateksti"))
    return dSubTimeTextObject;

  else if (lowChar == NFmiString("#subcomputertime") || lowChar == NFmiString("#jatkokoneenaika"))
    return dSubComputerTimeTextObject;

  else if (lowChar == NFmiString("paragraphmove") || lowChar == NFmiString("kappaleensiirto"))
    return dParagraphMove;

  else if (lowChar == NFmiString("indent") || lowChar == NFmiString("sisennys"))
    return dIndent;

  else if (lowChar == NFmiString("linefeed") || lowChar == NFmiString("rivinv‰li") ||
           lowChar == NFmiString("riviv‰li"))
    return dLF;

  else if (lowChar == NFmiString("linefeedfactor") || lowChar == NFmiString("rivinv‰likerroin") ||
           lowChar == NFmiString("riviv‰likerroin"))
    return dLFFactor;

  else if (lowChar == NFmiString("inparagraph") || lowChar == NFmiString("palstaan"))
    return dInParagraph;

  else if (lowChar == NFmiString("addafter") || lowChar == NFmiString("lis‰‰per‰‰n"))
    return dAddAfter;

  else if (lowChar == NFmiString("addinfront") || lowChar == NFmiString("lis‰‰eteen"))
    return dAddInFront;

  else if (lowChar == NFmiString("lowercase") || lowChar == NFmiString("pienill‰kirjaimilla") ||
           lowChar == NFmiString("pienetkirjaimet"))
    return dLowerCase;

  else if (lowChar == NFmiString("uppercase") || lowChar == NFmiString("isoillakirjaimilla") ||
           lowChar == NFmiString("isotkirjaimet"))
    return dUpperCase;

  else if (lowChar == NFmiString("capitalize") || lowChar == NFmiString("firstuppercase") ||
           lowChar == NFmiString("isollaalkukirjaimella") ||
           lowChar == NFmiString("isoalkukirjain"))
    return dFirstUpperCase;

  else if (lowChar == NFmiString("addlocaltime") || lowChar == NFmiString("lis‰‰paikallinenaika"))
    return dAddLocalTime;

  else if (lowChar == NFmiString("alignment") || lowChar == NFmiString("kohdistus") ||
           lowChar == NFmiString("tasaus"))
    return dDescTextAlignment;

  else if (lowChar == NFmiString("text") || lowChar == NFmiString("teksti"))
    return dTextString;

  else if (lowChar == NFmiString("charspacejustification") ||
           lowChar == NFmiString("merkkiv‰lins‰‰tˆ"))
    return dCharSpace;

  if (lowChar == NFmiString("file") || lowChar == NFmiString("tiedosto"))
    return dFile;

  else if (lowChar == NFmiString("directory") || lowChar == NFmiString("hakemisto"))
    return dTextDir;

  else if (lowChar == NFmiString("path") || lowChar == NFmiString("polku"))
    return dTextPath;

  else if (lowChar == NFmiString("topmargin") || lowChar == NFmiString("yl‰raja"))
    return dTopMargin;

  else if (lowChar == NFmiString("leftmargin") || lowChar == NFmiString("vasenraja"))
    return dLeftMargin;

  else if (lowChar == NFmiString("rightmargin") || lowChar == NFmiString("oikearaja"))
    return dRightMargin;

  else if (lowChar == NFmiString("paragraphborders") || lowChar == NFmiString("palstanrajat"))
    return dParagraphBorders;

  else if (lowChar == NFmiString("areapath") || lowChar == NFmiString("areaborders") ||
           lowChar == NFmiString("aluerajat"))
    return dTextBorders;

  else if (lowChar == NFmiString("timestamp") || lowChar == NFmiString("aikaleima"))
    return dFileTimestamp;

  else if (lowChar == NFmiString("maxlength") || lowChar == NFmiString("maksimipituus") ||
           lowChar == NFmiString("maxpituus"))
    return dMaxTextLength;

  else if (lowChar == NFmiString("widthfactor") || lowChar == NFmiString("leveyskerroin"))
    return dWidthFactor;

  else if (lowChar == NFmiString("noalignment") || lowChar == NFmiString("nojustification") ||
           lowChar == NFmiString("notrightjustification") || lowChar == NFmiString("liehureuna") ||
           lowChar == NFmiString("eioikeatasaus"))
    return dNotRightJustification;

  else if (lowChar == NFmiString("headerfont") || lowChar == NFmiString("otsikkokirjasin") ||
           lowChar == NFmiString("otsikkofontti"))
    return dHeaderFont;

  else if (lowChar == NFmiString("headersize") || lowChar == NFmiString("otsikkokoko"))
    return dHeaderSize;

  else if (lowChar == NFmiString("mainheaderfont") || lowChar == NFmiString("p‰‰otsikkokirjasin") ||
           lowChar == NFmiString("p‰‰otsikkofontti"))
    return dMainHeaderFont;

  else if (lowChar == NFmiString("mainheadersize") || lowChar == NFmiString("p‰‰otsikkokoko"))
    return dMainHeaderSize;

  else
    return NFmiPressScaling::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressText::WritePS(FmiPressOutputMode theOutput)
{
  //  if(itsPlace.X() == 0. && itsPlace.Y() == 0.)
  //      OutputLog('E', "Tekstin paikka antamatta: ", "Text place not given for: ", itsText);

  ScalePlotting();
  return WriteString(NFmiString("#TEXT"), theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param commentString Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressText::WriteString(const NFmiString &commentString, FmiPressOutputMode theOutput)
{
  bool fCV = itsEnvironment.GetCV();
  NFmiString text = Construct(itsText);
  bool fInArea = fInParagraph || fInFreeArea;

  if (theOutput != kPlainText && (itsPlace.X() == 0. && itsPlace.Y() == 0. && !fInArea))
    OutputLog('E', "Tekstin paikka antamatta: ", "Text place not given for: ", itsText);

  if (text == NFmiString("ERROR"))
    OutputLog('E', "Teksti puuttuu tekstioliolta", "Text missing from text object");

  NFmiHyphenationString hypString, helpString;
  bool firstParagraph = fInParagraph && itsTopMargin > -100.;
  bool nParagraph = fInParagraph && !firstParagraph;
  bool widthScaling = itsWidthFactor != 1.;
  text.ReplaceChars(NFmiString("_"), NFmiString(" "));

  NFmiRect rect = itsWriteScale.GetEndScales();
  NFmiPoint topLeft = rect.TopLeft();
  double centerX = itsWriteScale.GetEndCenter().X();

  if (itsRightMargin - itsLeftMargin < rect.Height() * 1.2 && fInParagraph &&
      theOutput == kPostScript)
  {
    *itsLogFile << "*** ERROR: too narrow text column: " << itsRightMargin - itsLeftMargin << endl;
    return false;
  }

  double lineStep = itsLineStep;
  double lineStepFactor = itsLineStepFactor;
  if (lineStepFactor <= 0.) lineStepFactor = 1.2;
  if (lineStep <= 0.) lineStep = rect.Height() * lineStepFactor;

  itsLineStep = lineStep;  // pressProduct kysyy

  double lineStepAdd = lineStep - itsLastLineStep;
  if (firstParagraph || fInFreeArea) lineStepAdd = 0;

  //*******************************************************************
  // laskettu fontille LucideConsole 5.1.98
  // 14.10 alkaen k‰ytet‰‰n PS:n stringwidth -> oikea sijoitus kaikilla fonteilla
  // y-koordinaatinkin saisi paikalleen samoin keinoin
  //*******************************************************************

  double heightHalf = .147f * rect.Height();   // kokeellinen vakio
  double heightHalfX = .345f * rect.Height();  // kokeellinen vakio
  double heightHalfY = .5 * rect.Height();     // kokeellinen vakio, t‰m‰ sijoittaa samoin kuin
  // entinen itsRotatingPoint ainakin yhdell‰ koolla

  double x, y;
  if (itsRotatingAngle == 90.)
  {
    x = centerX + heightHalfX;
    y = topLeft.Y() + heightHalfY;
  }
  else
  {
    x = centerX;
    y = topLeft.Y() + heightHalf;
  }

  if (theOutput == kMetaLanguage)
  {
    *itsOutFile << endl << "# TEKSTI" << endl;
    *itsOutFile << "font " << static_cast<char *>(GetFont()) << endl;
    *itsOutFile << "fontsize " << rect.Height() << endl;
    *itsOutFile << "textalign " << static_cast<char *>(AlignmentToMeta(GetTextAlignment())) << endl;

    WriteColor(GetColor(), theOutput, *itsOutFile);

    *itsOutFile << "text (" << static_cast<char *>(text) << ") " << x << " " << y << endl;
  }
  else if (theOutput == kPlainText)
  {
    if (IsLineFeed())
      *itsOutFile << endl << static_cast<char *>(text);
    else
      *itsOutFile << static_cast<char *>(text);
  }
  else if (theOutput == kXml)
  {
    *itsOutFile << "<text>" << static_cast<char *>(text) << ") " << x << " " << y << endl;
  }
  else  // possu
  {
    *itsOutFile << "%*** " << static_cast<char *>(commentString) << " BEGINS ***" << endl;

    // skandit vaatii erikoisk‰sittelyn:
    // ‰,Â,ˆ,÷ onnistuvat vain sanan lopussa, ƒ,≈ ei silloinkaan
    // ON 5.98 RATKAISTU MIKAN KANSSA

    // WritePSConcatText(!nParagraph, IsPureBlack());
    if (!nParagraph) *itsOutFile << "gsave" << endl;
    if (widthScaling) *itsOutFile << itsWidthFactor << " 1 scale" << endl;
    if (IsPureBlack())
      *itsOutFile << "true setoverprint" << endl;
    else
      *itsOutFile << "false setoverprint" << endl;

    WriteColor(GetColor(), theOutput, *itsOutFile);

    if (fInArea)
    {
      text += NFmiString(" ");
      hypString = NFmiHyphenationString(text);
      hypString.SetNarrowColumn(fNarrowColumn);
      helpString = hypString.CreateHyphens("~");
      if (itsEnvironment.AvoidOrphanSyllables()) helpString = helpString.DeleteShortSyllables("~");

      text = helpString.ReplaceChar(NFmiString("-"),
                                    NFmiString("\\255"));  // Illussa "-" ei mene l‰pi ??
    }

    // Jostain syyst‰ Mikan makrot ei l‰p‰ise miinusta,
    // joten ei k‰ytet‰ jos on tavuviivaa (silloin taas ei skandit tule!, mit‰ tehd‰)

    // bool isHyphen =    text.Search(NFmiString("-"))     != 0;
    bool isHyphen = false;
    bool isLongMinus = text.Search(NFmiString("\\226")) != 0;
    NFmiString lastFont;
    double lastHeight = 0;
    double lastLeading = 0;

    if (fInArea || (!isHyphen && !isLongMinus))
    {
      // bool deleteHeader = true;
      *itsOutFile << "/" << static_cast<char *>(GetFont()) << " /" << static_cast<char *>(GetFont())
                  << "_" << endl;
      //	  if(fInParagraph)              EI AUTA
      //		  text += NFmiString(" "); //metkut ei aina laita rivinsiirtoa loppuun
      hypString.Set(text, text.GetLen());
      unsigned long lastHead = hypString.SearchLast(NFmiString(">"));
      unsigned long firstHead = hypString.SearchLast(NFmiString("<"));
      bool isHeader = lastHead > 0 && firstHead > 0;
      if (isHeader && fInArea)
        hypString = hypString.GetChars(lastHead + 2, hypString.GetLen() - lastHead - 1);

      if (fCV ||
          hypString ==
              NFmiString("*"))  //!!!!!!!!!!!!!!! p‰iv‰Vaihtoteksti vaati, pit‰‰ tutkia; ks alla
        text = hypString;
      else
        text = hypString.ReplaceChar(NFmiString("-"),
                                     NFmiString("\\255"));  // Illu8ssa "-" ei mene l‰pi ??
                                                            // h‰vitt‰‰ yhden merkin pituiset

      *itsOutFile << rect.Height() << " select" << GetEncoding() << "font" << endl;
    }
    else
    {
      *itsOutFile << "/" << static_cast<char *>(GetFont()) << " " << rect.Height() << " selectfont"
                  << endl;
    }

    lastFont = GetFont();
    lastHeight = rect.Height();

    if (firstParagraph)
    {
      *itsOutFile << "{" << endl;

      double leftScaled = itsLeftMargin;
      double rightScaled = itsRightMargin;
      if (widthScaling)
      {
        leftScaled = itsLeftMargin / itsWidthFactor;
        rightScaled = itsRightMargin / itsWidthFactor;
      }
      // alaraja tarpeeksi alas, jos ei koko teksti mahdu alueeseen j‰‰ PS ilmeisesti
      // luuppiin

      *itsOutFile << leftScaled << " " << 0 << " moveto" << endl;
      *itsOutFile << leftScaled << " " << itsTopMargin << " lineto" << endl;
      *itsOutFile << rightScaled << " " << itsTopMargin << " lineto" << endl;
      *itsOutFile << rightScaled << " " << 0 << " lineto" << endl;
      *itsOutFile << "closepath" << endl;
      *itsOutFile << "}" << endl;
      *itsOutFile << "/TextPath exch def" << endl;
      *itsOutFile << "/Indent " << itsIndent << " def" << endl;
      if (!fRightJustification) *itsOutFile << "/Justification false def" << endl;
      *itsOutFile << "/Leading " << lineStep << " def" << endl;
      lastLeading = lineStep;
      *itsOutFile << "SetFirstText" << endl;
    }
    if (fInFreeArea)
    {
      *itsOutFile << "{" << endl;
      std::vector<NFmiPoint>::iterator pos;
      NFmiString command("moveto");
      for (pos = itsFreePath.begin(); pos != itsFreePath.end(); ++pos)
      {
        *itsOutFile << (*pos).X()  // widthScaling??
                    << " " << (*pos).Y() << " " << static_cast<char *>(command) << endl;
        command = "lineto";
      }

      *itsOutFile << "closepath" << endl;
      *itsOutFile << "}" << endl;
      *itsOutFile << "/TextPath exch def" << endl;
      *itsOutFile << "/Indent " << itsIndent << " def" << endl;
      if (!fRightJustification) *itsOutFile << "/Justification false def" << endl;
      *itsOutFile << "/Leading " << lineStep << " def" << endl;
      lastLeading = lineStep;
      *itsOutFile << "SetFirstText" << endl;
    }
    if (!fInArea)
    {
      double xScaled = x;
      if (widthScaling) xScaled = x / itsWidthFactor;

      *itsOutFile << xScaled << " " << y << " moveto" << endl;
      if (GetTextAlignment() == kRight)
      {
        if (itsRotatingAngle == 90.)
        {
          *itsOutFile << "(" << static_cast<char *>(text) << ") "
                      << "stringwidth" << endl;
          *itsOutFile << "neg exch neg" << endl;
          *itsOutFile << " rmoveto" << endl;
        }
        else
        {
          *itsOutFile << "(" << static_cast<char *>(text) << ") "
                      << "stringwidth" << endl;
          *itsOutFile << "neg exch neg exch" << endl;
          *itsOutFile << " rmoveto" << endl;
        }
      }
      else if (GetTextAlignment() == kCenter)
      {
        if (itsRotatingAngle == 90.)
        {
          *itsOutFile << "(" << static_cast<char *>(text) << ") "
                      << "stringwidth" << endl;
          *itsOutFile << "-2. div exch -2. div " << endl;  // puolikas matka
          *itsOutFile << " rmoveto" << endl;
        }
        else
        {
          *itsOutFile << "(" << static_cast<char *>(text) << ") "
                      << "stringwidth" << endl;
          *itsOutFile << "-2. div exch -2. div exch" << endl;  // puolikas matka
          *itsOutFile << " rmoveto" << endl;
        }
      }
    }

    if (itsRotatingAngle == 90. && !fInArea)
    {
      *itsOutFile << itsRotatingPoint.X() << " " << itsRotatingPoint.Y() << " translate" << endl;
      *itsOutFile << itsRotatingAngle << " rotate" << endl;
      *itsOutFile << -itsRotatingPoint.X() << " " << -itsRotatingPoint.Y() << " translate" << endl;
    }

    if (fInArea)
    {
      NFmiString actualFont;
      double actualHeight;
      double yMove = itsParagraphMove.Y() - lineStepAdd;
      *itsOutFile << "/Leading " << lineStep << " def" << endl;
      *itsOutFile << itsParagraphMove.X() << " " <<  // EI ONNAA X
          yMove << " rmoveto" << endl;
      *itsOutFile << "(" << static_cast<char *>(text) << ") Paragraph" << endl;
      if (itsNextTexts.size() >> 0)
      {
        std::vector<NFmiPressText>::iterator pos;
        NFmiString nextString;

        for (pos = itsNextTexts.begin(); pos != itsNextTexts.end(); ++pos)
        {
          nextString = *((*pos).GetText());
          if (nextString.GetLen() > 0)
          {
            hypString.Set(nextString, nextString.GetLen());

            hypString += NFmiString(" ");  // tarvitaan ekstra merkki??
            helpString = hypString.CreateHyphens("~");
            if (itsEnvironment.AvoidOrphanSyllables())
              helpString = helpString.DeleteShortSyllables("~");

            if (!fCV)  // Illu8:ssa "-" ei mene l‰pi ??
              helpString = helpString.ReplaceChar(NFmiString("-"), NFmiString("\\255"));
            actualFont = (*pos).GetFont();
            actualHeight = (*pos).GetHeight();
            if (actualFont != lastFont || actualHeight != lastHeight)
            {
              *itsOutFile << "/" << static_cast<char *>(actualFont) << " /"
                          << static_cast<char *>(actualFont) << "_" << endl;
              *itsOutFile << actualHeight << " select" << GetEncoding() << "font" << endl;
              lastFont = actualFont;
              lastHeight = actualHeight;
            }
            lineStep = (*pos).GetLineStep();
            lineStepFactor = (*pos).GetLineStepFactor();
            if (lineStepFactor <= 0.) lineStepFactor = 1.2;
            if (lineStep <= 0.) lineStep = (*pos).GetHeight() * lineStepFactor;

            if (lineStep != lastLeading)
            {
              *itsOutFile << "/Leading " << lineStep << " def" << endl;
              double stepDiff = lineStep - lastLeading;
              *itsOutFile << "0 " << -stepDiff << " rmoveto" << endl;
            }
            lastLeading = lineStep;
            *itsOutFile << "(" << static_cast<char *>(helpString) << ") Paragraph" << endl;
          }
          else
            *itsOutFile << "0 " << -lineStep << " rmoveto" << endl;
        }
      }
    }
    else
    {
      if (itsCharSpace == 0.)  // merkkiv‰lin s‰‰tˆ
        *itsOutFile << "(" << static_cast<char *>(text) << ") show" << endl;
      else  // HUOM ashow
        *itsOutFile << itsCharSpace << " 0. (" << static_cast<char *>(text) << ") ashow" << endl;
    }

    *itsOutFile << "false setoverprint" << endl;
    // HUOM useimpiin ylim‰‰r‰inen
    //*itsOutFile << "grestore" << endl;
  }  // loppu eps-moodi
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiPressText::Construct(NFmiString *theString) const
{
  NFmiString str = NFmiString(*theString);
  if (fUpperCase) str.UpperCase();
  if (fLowerCase) str.LowerCase();
  NFmiString retString;
  if (itsAddInFront.IsValue()) retString += itsAddInFront;
  retString += str;
  if (itsAddAfter.IsValue())
  {
    if (retString == "-" || retString == "\226")  // long dash
    {
      // Do now show "- C" or "- mm", just "-"
      if (itsAddAfter == "m" || itsAddAfter == "_m" || itsAddAfter == "cm" ||
          itsAddAfter == "_cm" || itsAddAfter == "mm" || itsAddAfter == "_mm" ||
          itsAddAfter == "\260" ||  // deg
          itsAddAfter == "m/s")
      {
      }
      else
      {
        retString += itsAddAfter;
      }
    }
    else
      retString += itsAddAfter;
  }

  if (itsSubText)
  {
    if (itsSubText->SetText())  // tarvitaan NFmiGiventextiss‰
      retString += itsSubText->Construct(itsSubText->GetText());
  }
  return retString;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressText::ScalePlotting(void)
{
  itsRightMargin = itsRectScale.ScaleX(itsRightMargin);
  itsLeftMargin = itsRectScale.ScaleX(itsLeftMargin);
  itsTopMargin = itsRectScale.ScaleY(itsTopMargin);
  NFmiPressScaling::ScalePlotting();  // tarvitaankohan kaikki
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param addPlace Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressText::Move(const NFmiPoint &addPlace)
{
  itsTopMargin += addPlace.Y();
  itsRightMargin += addPlace.X();
  itsLeftMargin += addPlace.X();
  NFmiPressScaling::Move(addPlace);
}

// ======================================================================
