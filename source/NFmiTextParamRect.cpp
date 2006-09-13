// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiTextParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiTextParamRect.h"
#include "NFmiPressParam.h"  
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal mappings
 * \todo The if is unnecessary
 */
// ----------------------------------------------------------------------

NFmiTextParamRect::~NFmiTextParamRect(void)
{
  if(itsMapping)
	delete static_cast<NFmiParamMapping *>(itsMapping);
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theTextParamRect The object to copy
 */
// ----------------------------------------------------------------------

NFmiTextParamRect::NFmiTextParamRect(const NFmiTextParamRect & theTextParamRect)
  : NFmiParamRect(theTextParamRect)
  , itsAddInFront(theTextParamRect.itsAddInFront)
  , itsAddAfter(theTextParamRect.itsAddAfter)
  , fUseSelectLatinFont(theTextParamRect.fUseSelectLatinFont)
  , fParenthesis(theTextParamRect.fParenthesis)
  , itsMapping(theTextParamRect.itsMapping ? new NFmiParamMapping(*theTextParamRect.itsMapping) : 0)
  , itsCurrentNumOfColMaps(theTextParamRect.itsCurrentNumOfColMaps)
  , itsRelCharWidth(theTextParamRect.itsRelCharWidth)
  , itsWidthFactor(theTextParamRect.itsWidthFactor)
  , fAddStationName(theTextParamRect.fAddStationName)
{
  for(unsigned int ind =0; ind < maxNumOfColMaps; ind++)
	{
	  itsColorMapping[ind] = theTextParamRect.itsColorMapping[ind];
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiTextParamRect::ReadRemaining(void)
{
  double r1,r2;
  NFmiValueString helpValueString;
  NFmiString helpString;
  double intervalEps = 0.00001;
  
  switch(itsIntObject)
	{
		case dParamSize:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				SetTextSize(r1);
			  }

			ReadNext();
			break;
		  }
		case dAlignment:
		  {
			if (!ReadEqualChar())
			  break;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;

			FmiDirection dir = String2FmiDirection(itsString);

			if(dir != kNoDirection)
			{
				//itsAlignment = dir;
				SetTextAlignment(dir);
			}
			else
			  *itsLogFile << "*** ERROR: Tuntematon kohdistus: "
						  << static_cast<char *>(itsObject)
						  << endl;
			ReadNext();
			break;
		  }
		case dFont:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;

			SetFont(itsObject);

			ReadNext();
			break;
		  }
	case dTextMapping:
		{
		  NFmiMappingInterval interval;
		  if (!ReadEqualChar())
			break;
		  if(ReadDouble(r1))
			{
			  *itsDescriptionFile >> itsObject;
			  helpValueString = NFmiValueString(itsObject);
			  if(helpValueString.IsNumeric())
				{
				  r2 = static_cast<double>(helpValueString);
				  *itsDescriptionFile >> itsObject;
				  helpString = itsObject;
				}
			  else
				{
				  r2 = r1;
				}
			  
			  
			  interval.itsBottomValue = r1 - intervalEps;
			  interval.itsTopValue = r2 + intervalEps;
			  
			  NFmiString* mappingName = new NFmiString(itsObject);
			  interval.itsSymbol = *mappingName;
			  itsMapping->AddMappingInterval(interval);
			}
		  
		  ReadNext();
		  break;
		}
	case dRelCharWidth:
	  {
		if (!ReadEqualChar())
		  break;
		
		if(ReadOne(r1))
		  itsRelCharWidth = r1;
		
		ReadNext();
		break;
	  }
	case dWidthFactor:
	{
		SetOne(itsWidthFactor);
		break;
	  }
	case dAddTextInFront:
	  {
		if (!ReadEqualChar())
		  break;
		*itsDescriptionFile >> itsObject;
		itsAddInFront = itsObject;
		
		ReadNext();
		break;
	  }
	case dAddTextAfter:
	  {
		if (!ReadEqualChar())
		  break;
		*itsDescriptionFile >> itsObject;
		itsAddAfter = itsObject;
		
		ReadNext();
		break;
	  }
	case dAddStationName:
	  {
		fAddStationName = true;
		
		ReadNext();
		break;
	  }
	case dRGBMapping:
	  {
		itsCurrentNumOfColMaps ++;
		if(itsCurrentNumOfColMaps > maxNumOfColMaps)
		  {
			itsCurrentNumOfColMaps = maxNumOfColMaps;
			*itsLogFile << "*** ERROR: v‰rimuunosten maxlkm ylitetty"  << endl;
			}
		if (SetFive(itsColorMapping[itsCurrentNumOfColMaps-1].minValue,
					itsColorMapping[itsCurrentNumOfColMaps-1].maxValue,
					itsColorMapping[itsCurrentNumOfColMaps-1].color.color.rgb.red,
					itsColorMapping[itsCurrentNumOfColMaps-1].color.color.rgb.green,
					itsColorMapping[itsCurrentNumOfColMaps-1].color.color.rgb.blue))
		  {
			itsColorMapping[itsCurrentNumOfColMaps-1].color.isRgb = true;
		  }
		else
		  {
			itsCurrentNumOfColMaps--;
			*itsLogFile << "*** ERROR: v‰rimuunoksen lukeminen ep‰onnistui"  << endl;
		  }
		
		break;
	  }
	case dCMYKMapping:
	  {
		itsCurrentNumOfColMaps ++;
		if(itsCurrentNumOfColMaps > maxNumOfColMaps)
		  {
			itsCurrentNumOfColMaps = maxNumOfColMaps;
			*itsLogFile << "*** ERROR: v‰rimuunosten maxlkm ylitetty"  << endl;
		  }
		if (SetSix(itsColorMapping[itsCurrentNumOfColMaps-1].minValue,
				   itsColorMapping[itsCurrentNumOfColMaps-1].maxValue,
				   itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.cyan,
				   itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.magenta,
				   itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.yellow,
				   itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.black))
		  {
			itsColorMapping[itsCurrentNumOfColMaps-1].color.isRgb = false;
		  }
		else
		  {
			itsCurrentNumOfColMaps--;
			*itsLogFile << "*** ERROR: v‰rimuunoksen lukeminen ep‰onnistui"  << endl;
		  }
		
		break;
	  }
	default:
	  {
		return NFmiParamRect::ReadRemaining();
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

int NFmiTextParamRect::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if(lowChar==NFmiString("alignment") ||
	 lowChar==NFmiString("kohdistus"))
	return dAlignment;

  else if(lowChar==NFmiString("font") ||
		  lowChar==NFmiString("kirjasin") ||
		  lowChar==NFmiString("fontti"))
	return dFont;

  else if(lowChar==NFmiString("size") ||
		  lowChar==NFmiString("koko"))
	return dParamSize;

  else if(lowChar==NFmiString("parenthesis") ||
		  lowChar==NFmiString("sulkuihin"))
	return dParenthesis;

  else if(lowChar==NFmiString("addafter") ||
		  lowChar==NFmiString("lis‰‰per‰‰n"))
	return dAddTextAfter;

  else if(lowChar==NFmiString("addinfront") ||
		  lowChar==NFmiString("lis‰‰eteen"))
	return dAddTextInFront;

  else if(lowChar==NFmiString("mapping") ||
		  lowChar==NFmiString("muunnos") ||
		  lowChar==NFmiString("tekstimuunnos"))
	return dTextMapping;

  else if(lowChar==NFmiString("rgbmapping") ||
		  lowChar==NFmiString("rgbmuunnos"))
	return dRGBMapping;

  else if(lowChar==NFmiString("cmykmapping") ||
		  lowChar==NFmiString("cmykmuunnos") ||
		  lowChar==NFmiString("v‰rimuunnos"))
	return dCMYKMapping;

  else if(lowChar==NFmiString("charspacejustification") ||
		  lowChar==NFmiString("merkkiv‰lins‰‰tˆ"))
	return dRelCharWidth;
  
  else if(lowChar==NFmiString ("widthfactor") ||
		  lowChar==NFmiString ("leveyskerroin"))
	return dWidthFactor;

  else if(lowChar==NFmiString("addstationname") ||
		  lowChar==NFmiString("lis‰‰asemannimi"))
	return dAddStationName;

  else
	return NFmiParamRect::ConvertDefText(object);

}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 * \param AbsoluteRectOfSymbolGroup Undocumented
 * \param theDestinationFile Undocumented
 * \param theObjectName Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiTextParamRect::WriteCode(const NFmiString & theText,
								  const NFmiRect & AbsoluteRectOfSymbolGroup,
								  ofstream & theDestinationFile,
								  const NFmiString & theObjectName,
								  FmiPressOutputMode theOutput)

{
  NFmiString longMinus ("\\226");

  NFmiRect numberRect = AbsoluteRectOfSymbolGroup.ToAbs(NFmiRect(TopLeft(),BottomRight()));


  NFmiPoint topLeft = numberRect.TopLeft(); //TopLeft on alaVasen! johtuuko RectScale:sta
  NFmiPoint bottomRight = numberRect.BottomRight();
  double centerX = (bottomRight.X()-topLeft.X())/2. + topLeft.X();
  // tarvitaan jotta vanha OsaAlue-menetelm‰ toimisi
  if(!fNewScaling && GetTextAlignment() == kRight )
	{
	  centerX = bottomRight.X();
	}
  else if (!fNewScaling && GetTextAlignment() == kLeft)
	{
	  centerX = topLeft.X();
	}
  //laskettu fontille LucideConsole 5.1.98
  // 13.10 universaali fontti-riippumaton paikan m‰‰ritys
  // vain keskikohta voi heitt‰‰ v‰h‰n, saishan senkin paikalleen

  // double x = centerX + numberRect.Height() * (-.31f - .302f*(theText.GetLen()-1));
  double x = centerX;
  double y = topLeft.Y() + .147f * numberRect.Height(); // oli .125; nostaa v‰h‰n mutta = illu

  if(theOutput == kPlainText)
	{
	  theDestinationFile << "\t" << static_cast<char *>(theText);
	}
  else if(theOutput == kXml)
	{
	  theDestinationFile << "<Text>" << endl;
	  theDestinationFile << "  <Set>Tekstit" << endl;
	  theDestinationFile << "  <Place>" << x << " " << y << endl;
	  
	}
  else if(theOutput == kMetaLanguage)
	{
	  theDestinationFile << endl
						 << "Font "
						 << static_cast<char *>(GetFont())
						 << endl;
	  theDestinationFile << "TextAlign "
						 << static_cast<char *>(itsPsWriting.AlignmentToMeta(GetTextAlignment()))
						 << endl;
	  theDestinationFile << "FontSize "
						 << numberRect.Height()
						 << endl;
	  
	  itsPsWriting.WriteColor(MapColor(), theOutput, theDestinationFile);
	  theDestinationFile << "Text \""
						 << static_cast<char *>(theText)
						 << "\" "
						 << x
						 << " "
						 << y
						 << endl;
	}
  else
	{
	  theDestinationFile << "%*** "
						 << static_cast<char *>(theObjectName)
						 << " ALKAA ***"
						 << endl;
	  theDestinationFile << "gsave" << endl;
	  
	  if(IsPureBlack())
		theDestinationFile << "true setoverprint" << endl;
	  else
		theDestinationFile << "false setoverprint" << endl;
	  
	  itsPsWriting.WriteColor(MapColor(), theOutput, theDestinationFile);
	  
	  /* Uusi makro selectlatinfont k‰yttˆˆn jotta skandit tulisivat ok
		 Mika Heiskasen tutkima asia
		 Mikan makrolla er‰it‰ puutteita:
		 - ei tule miinus
		 - ei sovi er‰ille fonteille, ainakin ZapfDingbats
		 mutta tarvitaan:
		 + skandeihin (muuten kuin sanan viimeisin‰)
	  */

	  if (fUseSelectLatinFont)
		{
		  theDestinationFile << "/"
							 << static_cast<char *>(GetFont())
							 << " /" << static_cast<char *>(GetFont())
							 << "_"
							 << endl;
		  theDestinationFile << numberRect.Height()
							 << " selectlatinfont"
							 << endl;
		}
	  else
		{
		  theDestinationFile << "/"
							 << static_cast<char *>(GetFont())
							 << " "
							 << numberRect.Height()
							 << " selectfont"
							 << endl;
		}

	  bool firstIs226 = theText.GetLen() > 3
		&& NFmiString(theText.GetCharsPtr(1,4)).IsEqual(longMinus);

	  bool boldFont = false;
	  unsigned long lenFont = GetFont().GetLen();
	  if(GetFont() == NFmiString("FranklinGothic-Roman") ||
         GetFont().GetChars(lenFont-3, 4) == NFmiString("Bold") ||
         GetFont().GetChars(lenFont-4, 5) == NFmiString("Black"))
		 boldFont = true;

	  bool secondIs226 = theText.GetLen() > 4
		&& NFmiString(theText.GetCharsPtr(2,4)).IsEqual(longMinus); //suluissa pitk‰ miinus tai numerov‰li 
	                                                                //tyyppi‰ 6-10, kuitenkin vaikeuksia joten
	                                                                //poistettu alla -> k‰yt‰ lyhytt‰ miinusta
	  NFmiString  restString, emptyString, widthString, minusWidth;
/*
	  if(!(firstIs226 || secondIs226)) //ei ongelmia
		{
		 // WriteShowString(x, y, theText, theText, theDestinationFile);
		  WriteShowString(x, y, emptyString, theText, theDestinationFile);
		}
	  else if(firstIs226)
		{
		  if(boldFont)
			//widthString = NFmiString("W"); //boldit v‰h‰n enemm‰n irti, levein kirjain
			widthString = NFmiString("B"); //boldit v‰h‰n enemm‰n irti, levein kirjain
		  else
			widthString = NFmiString("n"); //sama leveys kuin pitk‰ll‰ miinuksella kun kerran se itse ei k‰y stringwidth:iin
		  NFmiString onlyMinus(widthString);
		  
		  if(theText.GetLen()>4)
			widthString += theText.GetCharsPtr(5,theText.GetLen()-4);

		  if(GetTextAlignment() == kLeft)
			 WriteShowString(x, y, emptyString, longMinus, theDestinationFile);
		  else
			 WriteShowString(x, y, widthString, longMinus, theDestinationFile);

		  if(theText.GetLen()>4)
			  restString = theText.GetCharsPtr(5,theText.GetLen()-4);
			  if(GetTextAlignment() == kLeft)
			  {
				WriteShowString(x, y, onlyMinus, restString, theDestinationFile);
			  }
			  else
			  {
				WriteShowString(x, y, restString, restString, theDestinationFile);
			  }		  
		}
*/
	  if(!(firstIs226 || secondIs226)) //ei ongelmia
		{
		 // WriteShowString(x, y, theText, theText, theDestinationFile);
		  WriteShowString(x, y, theText, theDestinationFile);
		}
	  else if(firstIs226)
		{
		  minusWidth = NFmiString("B"); 

		  if(theText.GetLen()>4)
			  restString = theText.GetCharsPtr(5,theText.GetLen()-4);
		  WriteShowStringLongMinus(x, y, longMinus, minusWidth
			                          ,restString, theDestinationFile);		  
		  WriteShowStringText     (x, y, longMinus, minusWidth
			                          ,restString, theDestinationFile);		  
		}

	  else //secondIs226 eli suluissa (AL) 
	  {  // PS:ss‰ pit‰‰ olla parilliset sulut, muuten kenon kanssa
		 // miten k‰‰nt‰j‰st‰ saisi l‰pi NFmiString("\("); 
		/*
		  NFmiString widthStringAll = NFmiString("\(");
		  if(boldFont)
			widthString = NFmiString("W");
		  else
			widthString = NFmiString("n"); 

		  if(theText.GetLen()>4)
			widthString += theText.GetCharsPtr(6,theText.GetLen()-6);
		  widthString += NFmiString("\)");

		  widthStringAll += widthString;
		  //NFmiString first = theText.GetCharsPtr(1,1);
		  WriteShowString(x, y, widthStringAll, NFmiString("\("), theDestinationFile);
		  WriteShowString(x, y, widthString, longMinus, theDestinationFile);
			
		  if(theText.GetLen()>5)
			{
			  restString = theText.GetCharsPtr(6,theText.GetLen()-6);
		      restString += NFmiString("\)");
			  WriteShowString(x, y, restString, restString, theDestinationFile);
			}
*/		
		}
	  
	  theDestinationFile << "false setoverprint" << endl;
	  
	  theDestinationFile << "grestore" << endl;
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

FmiGenericColor NFmiTextParamRect::MapColor(void) const
{
  return GetColor();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiTextParamRect::Construct(NFmiString * theString) const
{
  NFmiString str = NFmiString(*theString);
  NFmiString retString;
  if(itsAddInFront.IsValue())
	retString += itsAddInFront;
  retString += str;
  if(fAddStationName)
	  retString += itsPressParam->GetCurrentStation().GetName();
  if(itsAddAfter.IsValue())
	retString += itsAddAfter;
  
  retString.ReplaceChars(NFmiString("_"), NFmiString(" "));
  
  return retString;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param x Undocumented
 * \param y Undocumented
 * \param theWidthString Undocumented
 * \param theShowString Undocumented
 * \param os Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiTextParamRect::WriteShowString(double x,
										double y,
										const NFmiString & theShowString,
										ofstream & os) const
{
  bool widthScaling = itsWidthFactor != 1.;
  float xScaled = x;
  if(widthScaling)
  {
		os << itsWidthFactor << " 1 scale" << endl;
		xScaled = x/itsWidthFactor;
  }

  os << xScaled << " " << y << " moveto" << endl;
  if(GetTextAlignment() == kRight )
	{
	  os << "(" << static_cast<char *>(theShowString) << ") " << "stringwidth" << endl;
	  os << "neg exch neg exch" << endl;
	  os  << " rmoveto" << endl;
	}
  else if (GetTextAlignment() == kCenter)
  {
	  if(theShowString.GetLen()>0)
		{
		  os << "(" << static_cast<char *>(theShowString) << ") " << "stringwidth" << endl;
		  os << "-2. div exch -2. div exch" << endl; //puolikas matka
		  os  << " rmoveto" << endl;
		}
  }
  else if (GetTextAlignment() == kLeft && theShowString.GetLen()>0)
	{
	 // os << "(" << static_cast<char *>(theWidthString) << ") " << "stringwidth" << endl;
	 // os  << " rmoveto" << endl;
	}
  if(itsRelCharWidth == 0.) // merkkiv‰lien levennys/kavennus
	os << "(" << static_cast<char *>(theShowString) << ") show" << endl;
  else
	os << itsRelCharWidth << " 0. (" << static_cast<char *>(theShowString) << ") ashow" << endl;
  
  return true;
}
// ----------------------------------------------------------------------

bool NFmiTextParamRect::WriteShowStringLongMinus(double x,
										double y,
										const NFmiString & theMinusString,
										const NFmiString & theWidthMinus,
										const NFmiString & theShowString,
										ofstream & os) const
{
  os << x << " " << y << " moveto" << endl;
  if(GetTextAlignment() == kRight )
	{
	  NFmiString totalWidth(theWidthMinus);
	  totalWidth += theShowString; 
	  os << "(" << static_cast<char *>(totalWidth) << ") " << "stringwidth" << endl;
	  os << "neg exch neg exch" << endl;
	  os  << " rmoveto" << endl;
	}
  else if (GetTextAlignment() == kCenter)
  {
	  if(theShowString.GetLen()>0)
		{
		  os << "(" << static_cast<char *>(theShowString) << ") " << "stringwidth" << endl;
		  os << "-2. div exch -2. div exch" << endl; //puolikas matka
		  os  << " rmoveto" << endl;
		}
	  if(theWidthMinus.GetLen()>0)
		{
		  os << "(" << static_cast<char *>(theWidthMinus) << ") " << "stringwidth" << endl;
		  os << "-2. div exch -2. div exch" << endl; //puolikas matka
		  os  << " rmoveto" << endl;
		}
  }
/*
  else if (GetTextAlignment() == kLeft && theWidthMinus.GetLen()>0)
	{
	  os << "(" << static_cast<char *>(theWidthMinus) << ") " << "stringwidth" << endl;
	  os  << " rmoveto" << endl;
	}
*/
  if(itsRelCharWidth == 0.) // merkkien levennys/kavennus
	os << "(" << static_cast<char *>(theMinusString) << ") show" << endl;
  else
	os << itsRelCharWidth << " 0. (" << static_cast<char *>(theMinusString) << ") ashow" << endl;
  
  return true;
}

// ----------------------------------------------------------------------

bool NFmiTextParamRect::WriteShowStringText(double x,
										double y,
										const NFmiString & theMinusString,
										const NFmiString & theWidthMinus,
										const NFmiString & theShowString,
										ofstream & os) const
{
  os << x << " " << y << " moveto" << endl;
  if(GetTextAlignment() == kRight )
	{
	  os << "(" << static_cast<char *>(theShowString) << ") " << "stringwidth" << endl;
	  os << "neg exch neg exch" << endl;
	  os  << " rmoveto" << endl;
	}
  else if (GetTextAlignment() == kCenter)
  {
	  if(theShowString.GetLen()>0)
		{
		  NFmiString totalWidth(theWidthMinus);
		  totalWidth += theShowString; 
		  os << "(" << static_cast<char *>(totalWidth) << ") " << "stringwidth" << endl;
		  os << "-2. div exch -2. div exch" << endl; //puolikas matka
		  os  << " rmoveto" << endl;
		  os << "(" << static_cast<char *>(theWidthMinus) << ") " << "stringwidth" << endl;
		  os  << " rmoveto" << endl;
		}
  }
  else if (GetTextAlignment() == kLeft && theWidthMinus.GetLen()>0)
	{
	  os << "(" << static_cast<char *>(theWidthMinus) << ") " << "stringwidth" << endl;
	  os  << " rmoveto" << endl;
	}
  if(itsRelCharWidth == 0.) // merkkien levennys/kavennus
	os << "(" << static_cast<char *>(theShowString) << ") show" << endl;
  else
	os << itsRelCharWidth << " 0. (" << static_cast<char *>(theShowString) << ") ashow" << endl;
  
  return true;
}

// ======================================================================
