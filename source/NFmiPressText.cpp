// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressText
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressText.h"
#include "NFmiHyphenationString.h"
#include "NFmiPressGivenTimeText.h"
#include "NFmiPressComputerTimeText.h"
#include "NFmiPressDataTimeText.h"
#include "NFmiFileString.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes some internal dynamic variables
 * \todo Remove the ifs as unnecessary
 */
// ----------------------------------------------------------------------

NFmiPressText::~NFmiPressText(void)
{
  if(itsText) delete itsText;
  if(itsSubText) delete itsSubText;
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 *†\param thePressText The object to copy
 * \todo Do everything in the initialization list
 */
// ----------------------------------------------------------------------

NFmiPressText::NFmiPressText(const NFmiPressText & thePressText)
  : NFmiPressScaling()
  , itsLastLineStep(thePressText.itsLastLineStep)
  , itsLineStep(thePressText.itsLineStep)
  , itsLineStepFactor(thePressText.itsLineStepFactor)
  , itsTopMargin(thePressText.itsTopMargin)
  , itsRightMargin(thePressText.itsRightMargin)
  , itsLeftMargin(thePressText.itsLeftMargin)
  , itsIndent(thePressText.itsIndent)
  , fLoopErrorReported(thePressText.fLoopErrorReported)
  , fInParagraph(thePressText.fInParagraph)
  , itsParagraphMove(thePressText.itsParagraphMove)
  , fUpperCase (thePressText.fUpperCase)
  , fLowerCase (thePressText.fLowerCase)
  , fAddLocalTime(thePressText.fAddLocalTime)
//  , itsFont(thePressText.itsFont)
//  , itsAlignment(thePressText.itsAlignment)
  , itsCharSpace(thePressText.itsCharSpace)
  , itsMaxLen(thePressText.itsMaxLen)
  , itsWidthFactor(thePressText.itsWidthFactor)
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

bool NFmiPressText::ReadDescription(NFmiString & retString)
{
  NFmiValueString valueString;
  NFmiString  textFile, textPath, textDir;
  double r1,r2,r3;

  //itsFont = NFmiString("Times-Roman");
  itsRectSize.Y(GetTextSize());

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);
  bool oneMarginSet = false;

  while(itsIntObject != dEnd || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Tekstiss‰" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #Tekstiss‰: "
						  << static_cast<char *>(itsObject)
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
			if (!ReadEqualChar())
			  break;
			if(Read2Double(r1,r2))
			  {
				Place(NFmiPoint(r1,r2));
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
		case dTextString:
		  {
			if (!ReadEqualChar())
			  break;
			itsText = new NFmiString(ReadString());

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
			*itsDescriptionFile >> itsObject;
			itsAddInFront = itsObject;

			ReadNext();
			break;
		  }
		case dAddAfter:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;
			itsAddAfter = itsObject;

			ReadNext();
			break;
		  }
		case dTopMargin:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				itsTopMargin = r1;
				fInParagraph = true;
			  }
		    ReadNext();
			break;
		  }
		case dLeftMargin:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				if (oneMarginSet)
					fInParagraph = true;
				oneMarginSet = true;
				itsLeftMargin = r1;
			  }
		    ReadNext();
			break;
		  }
		case dRightMargin:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				if (oneMarginSet)
					fInParagraph = true;
				oneMarginSet = true;
				itsRightMargin = r1;
			  }
		    ReadNext();
			break;
		  }
		case dLineStep:
		{
		  if (!ReadEqualChar())
			break;
		  if(ReadDouble(r1))
			{
			  itsLineStep = r1;
			}
		  ReadNext();
		  break;
		}
		case dIndent:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				itsIndent = r1;
			  }
		    ReadNext();
			break;
		  }
		case dLF:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				itsLineStep = r1;
			  }
		    ReadNext();
			break;
		  }
		case dLFFactor:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				itsLineStepFactor = r1;
			  }
		    ReadNext();
			break;
		  }
		case dParagraphBorders:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read2Double(r1,r2) && ReadDouble(r3))
			  {
				if(r3 > r1 + 10. && r2 > -100.)
				  {
					itsLeftMargin = r1;
					itsTopMargin =  r2;
					itsRightMargin = r3;
					fInParagraph = true;
				  }
				else
				  {
					*itsLogFile << "*** ERROR: Virheelliset palstanrajat: "
								<< r1 << " " << r2 << " " << r3 << endl;
				  }
			  }
		    ReadNext();
			break;
		  }
		case dFile:
		  {
			if (!ReadEqualChar())
			  break;
			textFile = ReadString();

		    ReadNext();
			break;
		  }
		case dTextDir:
		  {
			if (!ReadEqualChar())
			  break;

            textDir = ReadString();

			ReadNext();
			break;
		  }

		case dTextPath:
		  {
			if (!ReadEqualChar())
			  break;

            textPath = ReadString();

			ReadNext();
			break;
		  }


		default:
		  {
			ReadRemaining();
			break;
		  }

		}
	}

  if(textFile.IsValue())
	{
	  delete itsText;
	  itsText = new NFmiString;

	  NFmiHyphenationString aString;

	  if(GetTimestampDayGap() != kShortMissing)
		AddTimeStamp(textFile);

	  NFmiFileString dataFile = CreatePath(NFmiString("Tekstit"),
										   textPath,
										   textDir,
										   textFile,
										   NFmiString("txt"));

	  *itsLogFile << "VakioTeksti: "<< static_cast<char *>(dataFile) ;

	  std::fstream in(dataFile, ios::in|ios::in);
	  if(in.good())
		{
		  while( !in.eof())
			{
			  char text[120];
			  in >> text;
			  if(!in.eof())	// est‰‰ viimeisen sanan kahdentumisen jos loppuu pelkk‰‰n rivinsiirtoon
				aString += NFmiHyphenationString(text) += NFmiHyphenationString(" ");
			}
		  in.close();
			  if(aString.GetLen() > itsMaxLen)
			  {
		          *itsLogFile << endl << "  *** ERROR: teksti katkaistu sallittuun maxpituuteen: " 
					          << itsMaxLen
							  << endl
	                          << "             menetetty "
							  << aString.GetLen()-itsMaxLen
							  << " merkki‰"
							  << endl;
				
				  aString = aString.GetChars(1, itsMaxLen); 
			  }
			  
		  *itsText = aString; //ReplaceChar(NFmiString("-"), NFmiString("\\255")); //29.6 Illussa "-" ei mene l‰pi ??
          *itsLogFile << " luettu" << endl;
		}
	  else
		{
		  *itsLogFile << endl << "  *** ERROR: teksti ei lˆydy" << endl;
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
  switch(itsIntObject)
	{
	case dTextLanguage:
	  {
		if (!ReadEqualChar())
		  break;

		itsLanguage = ReadLanguage();

		ReadNext();
		break;
	  }
	case dParagraphMove:
	  {
		if (!ReadEqualChar())
		  break;
		if(ReadDouble(r1))
		  {
			fInParagraph = true;
			*itsDescriptionFile >> itsObject;
			valueString = itsObject;
			if(valueString.IsNumeric())
			  {
				itsParagraphMove = NFmiPoint(r1,static_cast<double>(valueString));
				ReadNext();
			  }
			else
			  {
				itsParagraphMove = NFmiPoint(0,r1);
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
		ReadNext();

		break;
	  }

	case dSubTextObject:
	  {
		if (itsSubText)    //onko mahdollista
		  delete itsSubText;
		itsSubText = new NFmiPressText;
		itsSubText->SetHome(GetHome());
        itsSubText->SetEnvironment(itsEnvironment);
		itsSubText->SetLogFile(itsLogFile);
		itsSubText->SetLanguage(itsLanguage);
		itsSubText->SetDescriptionFile(itsDescriptionFile);	// miten on voinut toimia ilman
		if(!itsSubText->ReadDescription(itsString))
		  {
			delete itsSubText;
			itsSubText = 0;
		  }
		itsIntObject = ConvertDefText(itsString);
		break;
	  }
	case dSubTimeTextObject:
	  {
		if (itsSubText)
		  delete itsSubText;
		itsSubText = new NFmiPressGivenTimeText;
		itsSubText->SetHome(GetHome());
        itsSubText->SetEnvironment(itsEnvironment);
		itsSubText->SetLogFile(itsLogFile);
		itsSubText->SetTime(itsFirstPlotTime);
		itsSubText->SetLanguage(itsLanguage);
		itsSubText->SetDescriptionFile(itsDescriptionFile);	// miten on voinut toimia ilman
		if(!itsSubText->ReadDescription(itsString))
		  {
			delete itsSubText;
			itsSubText = 0;
		  }

		itsIntObject = ConvertDefText(itsString);
		break;
	  }

	case dSubComputerTimeTextObject:
	  {
		if (itsSubText)
		  delete itsSubText;
		itsSubText = new NFmiPressComputerTimeText;
		itsSubText->SetHome(GetHome());
        itsSubText->SetEnvironment(itsEnvironment);
		itsSubText->SetLogFile(itsLogFile);
		itsSubText->SetLanguage(itsLanguage);
		itsSubText->SetDescriptionFile(itsDescriptionFile);	// miten on voinut toimia ilman
		if(!itsSubText->ReadDescription(itsString))
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

	default:
	  {
		NFmiPressScaling:: ReadRemaining();
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

int NFmiPressText::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if(lowChar==NFmiString("size") ||
	 lowChar==NFmiString("textsize") ||
	 lowChar==NFmiString("koko") ||
	 lowChar==NFmiString("tekstikoko"))
	return dSymbolSize;

  else if(lowChar==NFmiString("language") ||
	 lowChar==NFmiString("kieli"))
	return dTextLanguage;

  else if(lowChar==NFmiString("#subconsttext") ||
		  lowChar==NFmiString("#jatkovakioteksti"))
	return dSubTextObject;

  else if(lowChar==NFmiString("#subtimetext") ||
		  lowChar==NFmiString("#jatkoaikateksti"))
	return dSubTimeTextObject;

  else if(lowChar==NFmiString("#subcomputertime") ||
		  lowChar==NFmiString("#jatkokoneenaika"))
	return dSubComputerTimeTextObject;

  else if(lowChar==NFmiString("paragraphmove") ||
		  lowChar==NFmiString("kappaleensiirto"))
	return dParagraphMove;

  else if(lowChar==NFmiString("indent") ||
		  lowChar==NFmiString("sisennys"))
	return dIndent;

  else if(lowChar==NFmiString("linefeed") ||
		  lowChar==NFmiString("rivinv‰li") ||
		  lowChar==NFmiString("riviv‰li"))
	return dLF;

  else if(lowChar==NFmiString("linefeedfactor") ||
		  lowChar==NFmiString("rivinv‰likerroin") ||
		  lowChar==NFmiString("riviv‰likerroin"))
	return dLFFactor;

  else if(lowChar==NFmiString("inparagraph") ||
		  lowChar==NFmiString("palstaan"))
	return dInParagraph;

  else if(lowChar==NFmiString("addafter") ||
		  lowChar==NFmiString("lis‰‰per‰‰n"))
	return dAddAfter;

  else if(lowChar==NFmiString("addinfront") ||
		  lowChar==NFmiString("lis‰‰eteen"))
	return dAddInFront;

  else if(lowChar==NFmiString("lowercase") ||
		  lowChar==NFmiString("pienill‰kirjaimilla") ||
		  lowChar==NFmiString("pienetkirjaimet"))
	return dLowerCase;

  else if(lowChar==NFmiString("uppercase") ||
		  lowChar==NFmiString("isoillakirjaimilla") ||
		  lowChar==NFmiString("isotkirjaimet"))
	return dUpperCase;

  else if(lowChar==NFmiString("addlocaltime") ||
		  lowChar==NFmiString("lis‰‰paikallinenaika"))
	return dAddLocalTime;

  else if(lowChar==NFmiString("alignment") ||
		  lowChar==NFmiString("kohdistus") ||
		  lowChar==NFmiString("tasaus"))
	return dDescTextAlignment;

  else if(lowChar==NFmiString("text") ||
		  lowChar==NFmiString("teksti"))
	return dTextString;

  else if(lowChar==NFmiString("charspacejustification") ||
		  lowChar==NFmiString("merkkiv‰lins‰‰tˆ"))
	return dCharSpace;

  if(lowChar==NFmiString("file") ||
	 lowChar==NFmiString("tiedosto"))
	return dFile;

  else if(lowChar==NFmiString("directory") ||
		  lowChar==NFmiString("hakemisto"))
	return dTextDir;

  else if(lowChar==NFmiString("path") ||
		  lowChar==NFmiString("polku"))
	return dTextPath;

  else if(lowChar==NFmiString("topmargin") ||
		  lowChar==NFmiString("yl‰raja"))
	return dTopMargin;

  else if(lowChar==NFmiString("leftmargin") ||
		  lowChar==NFmiString("vasenraja"))
	return dLeftMargin;

  else if(lowChar==NFmiString("rightmargin") ||
		  lowChar==NFmiString("oikearaja"))
	return dRightMargin;

  else if(lowChar==NFmiString("paragraphborders") ||
		  lowChar==NFmiString ("palstanrajat"))
	return dParagraphBorders;

  else if(lowChar==NFmiString("timestamp") ||
		  lowChar==NFmiString ("aikaleima"))
	return dFileTimestamp;

  else if(lowChar==NFmiString("maxlength") ||
		  lowChar==NFmiString ("maksimipituus") ||
		  lowChar==NFmiString ("maxpituus"))
	return dMaxTextLength;

  else if(lowChar==NFmiString ("widthfactor") ||
		  lowChar==NFmiString ("leveyskerroin"))
	return dWidthFactor;

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

  ScalePlotting();
  return WriteString(NFmiString("VAKIOTEKSTI"), theOutput);
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

bool NFmiPressText::WriteString(const NFmiString & commentString,
								FmiPressOutputMode theOutput)
{

  NFmiString text = Construct(itsText);
  NFmiHyphenationString hypString, helpString;
  bool firstParagraph = fInParagraph && itsTopMargin > -100.;
  bool nParagraph = fInParagraph && !firstParagraph;
  bool widthScaling = itsWidthFactor != 1.;

  text.ReplaceChars(NFmiString("_"), NFmiString(" "));

  NFmiRect rect = itsWriteScale.GetEndScales();
  NFmiPoint topLeft = rect.TopLeft();
  double centerX = itsWriteScale.GetEndCenter().X();

  if(itsRightMargin-itsLeftMargin < rect.Height()*1.2 && fInParagraph && theOutput == kPostScript)
	{
	  *itsLogFile << "*** ERROR: too narrow text column: " << itsRightMargin-itsLeftMargin << endl;
	  return false;
	}

  double lineStep = itsLineStep;
  double lineStepFactor = itsLineStepFactor;
  if(lineStepFactor <= 0.)
	lineStepFactor = 1.2;
  if(lineStep <= 0.)
	lineStep = rect.Height() * lineStepFactor;

  itsLineStep = lineStep; //pressProduct kysyy

  double lineStepAdd = lineStep - itsLastLineStep;
  if(firstParagraph) lineStepAdd = 0;

  //*******************************************************************
  //laskettu fontille LucideConsole 5.1.98
  // 14.10 alkaen k‰ytet‰‰n PS:n stringwidth -> oikea sijoitus kaikilla fonteilla
  // y-koordinaatinkin saisi paikalleen samoin keinoin
  //*******************************************************************

  double heightHalf = .147f * rect.Height(); //kokeellinen vakio
  double heightHalfX =.345f * rect.Height(); //kokeellinen vakio
  double heightHalfY =.5 * rect.Height(); //kokeellinen vakio, t‰m‰ sijoittaa samoin kuin
		                                        // entinen itsRotatingPoint ainakin yhdell‰ koolla

  double x, y;
  if(itsRotatingAngle == 90.)
	{
	  x = centerX + heightHalfX;
	  y = topLeft.Y() + heightHalfY;
	}
  else
	{
	  x = centerX;
	  y = topLeft.Y() + heightHalf;
	}

  if(theOutput == kMetaLanguage)
	{
	  *itsOutFile << endl << "# TEKSTI" << endl;
	  *itsOutFile << "font " << static_cast<char *>(GetFont())  << endl;
	  *itsOutFile << "fontsize " << rect.Height()  << endl;
	  *itsOutFile << "textalign " << static_cast<char *>(AlignmentToMeta(GetTextAlignment()))  << endl;

	  WriteColor(GetColor(), theOutput, *itsOutFile);

	  *itsOutFile << "text (" << static_cast<char *>(text) << ") " << x << " " << y << endl;
	}
  else if(theOutput == kPlainText)
	{
	  if(IsLineFeed())
		  *itsOutFile << endl << static_cast<char *>(text);
	  else
		  *itsOutFile << static_cast<char *>(text);
	}
  else if(theOutput == kXml)
	{
	  *itsOutFile << "<text>" << static_cast<char *>(text) << ") " << x << " " << y << endl;
	}
  else
	{
	  *itsOutFile << "%*** " << static_cast<char *>(commentString) << " ALKAA ***" << endl;


	  // skandit vaatii erikoisk‰sittelyn:
	  // ‰,Â,ˆ,÷ onnistuvat vain sanan lopussa, ƒ,≈ ei silloinkaan
	  // ON 5.98 RATKAISTU MIKAN KANSSA

	 // WritePSConcatText(!nParagraph, IsPureBlack());
	  if(!nParagraph)
		*itsOutFile << "gsave" << endl;
	  if(widthScaling)
		*itsOutFile << itsWidthFactor << " 1 scale" << endl;
	  if(IsPureBlack())
		*itsOutFile << "true setoverprint" << endl;
	  else
		*itsOutFile << "false setoverprint" << endl;

	  WriteColor(GetColor(), theOutput, *itsOutFile);
	
	  if(fInParagraph)
		{
		  text += NFmiString(" ");
		  hypString = NFmiHyphenationString(text);
		  helpString = hypString.CreateHyphens("~");
		  text = helpString.ReplaceChar(NFmiString("-"), NFmiString("\\255")); // Illussa "-" ei mene l‰pi ??
			}

	  // Jostain syyst‰ Mikan makrot ei l‰p‰ise miinusta,
	  // joten ei k‰ytet‰ jos on tavuviivaa (silloin taas ei skandit tule!, mit‰ tehd‰)

	  bool isHyphen = text.Search(NFmiString("-"));
	  bool isLongMinus = text.Search(NFmiString("\\226"));

	  if(fInParagraph || (!isHyphen && !isLongMinus))
		{
		  *itsOutFile << "/"
					  << static_cast<char *>(GetFont())
					  << " /" << static_cast<char *>(GetFont())
					  << "_" << endl;
		  *itsOutFile << rect.Height() << " selectlatinfont" << endl;
		}
	  else

		*itsOutFile << "/"
					<< static_cast<char *>(GetFont())
					<< " " << rect.Height()
					<< " selectfont"
					<< endl;

	  if(firstParagraph)
		{
		  *itsOutFile << "{" << endl;

		  double leftScaled = itsLeftMargin;
		  double rightScaled = itsRightMargin;
		  if (widthScaling)
		  {
			  leftScaled = itsLeftMargin/itsWidthFactor;
			  rightScaled = itsRightMargin/itsWidthFactor;
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
		  *itsOutFile << "/Leading " << lineStep << " def" << endl;
		  *itsOutFile << "SetFirstText" << endl;
		}
	  if(!fInParagraph)
		{
		  double xScaled = x;
		  if (widthScaling)
			  xScaled = x/itsWidthFactor;

		  *itsOutFile << xScaled << " " << y << " moveto" << endl;

		  if(GetTextAlignment() == kRight )
			{
			  if(itsRotatingAngle == 90.)
				{
				  *itsOutFile << "(" << static_cast<char *>(text) << ") " << "stringwidth" << endl;
				  *itsOutFile << "neg exch neg" << endl;
				  *itsOutFile  << " rmoveto" << endl;
				}
			  else
				{
				  *itsOutFile << "(" << static_cast<char *>(text) << ") " << "stringwidth" << endl;
				  *itsOutFile << "neg exch neg exch" << endl;
				  *itsOutFile  << " rmoveto" << endl;
				}
			}
		  else if (GetTextAlignment() == kCenter)
			{
			  if(itsRotatingAngle == 90.)
				{
				  *itsOutFile << "(" << static_cast<char *>(text) << ") " << "stringwidth" << endl;
				  *itsOutFile << "-2. div exch -2. div " << endl; //puolikas matka
				  *itsOutFile  << " rmoveto" << endl;
				}
			  else
				{
				  *itsOutFile << "(" << static_cast<char *>(text) << ") " << "stringwidth" << endl;
				  *itsOutFile << "-2. div exch -2. div exch" << endl; //puolikas matka
				  *itsOutFile  << " rmoveto" << endl;
				}
			}
		}

	  if(itsRotatingAngle == 90. && !fInParagraph)
		{
		  *itsOutFile << itsRotatingPoint.X() << " " << itsRotatingPoint.Y() << " translate" << endl;
		  *itsOutFile << itsRotatingAngle << " rotate" << endl;
		  *itsOutFile << -itsRotatingPoint.X() << " " << -itsRotatingPoint.Y() << " translate" << endl;
		}

	  if(fInParagraph)
		{
		  double yMove = itsParagraphMove.Y() - lineStepAdd;
		  *itsOutFile << "/Leading " << lineStep << " def" << endl;
		  *itsOutFile << itsParagraphMove.X() << " " <<             // EI ONNAA X
			yMove << " rmoveto" << endl;
		  *itsOutFile << "(" << static_cast<char *>(text) << ") Paragraph" << endl;
		}
	  else
		{
		  if(itsCharSpace == 0.) // merkkiv‰lin s‰‰tˆ
			*itsOutFile << "(" << static_cast<char *>(text) << ") show" << endl;
		  else    //HUOM ashow
			*itsOutFile << itsCharSpace << " 0. (" << static_cast<char *>(text) << ") ashow" << endl;
		}

	  *itsOutFile << "false setoverprint" << endl;
	  //HUOM useimpiin ylim‰‰r‰inen
	  //*itsOutFile << "grestore" << endl; 
	} //loppu eps-moodi
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

NFmiString NFmiPressText::Construct(NFmiString * theString) const
{
  NFmiString str = NFmiString(*theString);
  if(fUpperCase) str.UpperCase();
  if(fLowerCase) str.LowerCase();
  NFmiString retString;
  if(itsAddInFront.IsValue())
	retString += itsAddInFront;
  retString += str;
  if(itsAddAfter.IsValue())
	retString += itsAddAfter;

  if(itsSubText)
	{
	  if(itsSubText->SetText())  //tarvitaan NFmiGiventextiss‰
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
  NFmiPressScaling::ScalePlotting(); //tarvitaankohan kaikki
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param addPlace Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressText::Move(const NFmiPoint & addPlace)
{
  itsTopMargin += addPlace.Y();
  itsRightMargin += addPlace.X();
  itsLeftMargin += addPlace.X();
  NFmiPressScaling::Move(addPlace);
}

// ======================================================================
