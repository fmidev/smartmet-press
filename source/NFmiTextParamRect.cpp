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
  , itsFont(theTextParamRect.itsFont)
  , itsAlignment(theTextParamRect.itsAlignment)
  , itsStyle(theTextParamRect.itsStyle)
  , fParenthesis(theTextParamRect.fParenthesis)
  , itsMapping(theTextParamRect.itsMapping ? new NFmiParamMapping(*theTextParamRect.itsMapping) : 0)
  , itsCurrentNumOfColMaps(theTextParamRect.itsCurrentNumOfColMaps)
  , itsRelCharWidth(theTextParamRect.itsRelCharWidth)
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

  else if(lowChar==NFmiString("style") ||
		  lowChar==NFmiString("tyyli"))
	return dStyle;

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
  if(!fNewScaling && itsAlignment == kRight )
	{
	  centerX = bottomRight.X();
	}
  else if (!fNewScaling && itsAlignment == kLeft)
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
						 << static_cast<char *>(itsFont)
						 << endl;
	  theDestinationFile << "TextAlign "
						 << static_cast<char *>(itsPsWriting.AlignmentToMeta(itsAlignment))
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
							 << static_cast<char *>(itsFont)
							 << " /" << static_cast<char *>(itsFont)
							 << "_"
							 << endl;
		  theDestinationFile << numberRect.Height()
							 << " selectlatinfont"
							 << endl;
		}
	  else
		{
		  theDestinationFile << "/"
							 << static_cast<char *>(itsFont)
							 << " "
							 << numberRect.Height()
							 << " selectfont"
							 << endl;
		}

	  bool firstIs226 = theText.GetLen() > 3
		&& NFmiString(theText.GetCharsPtr(1,4)).IsEqual(longMinus);

	  bool secondIs226 = theText.GetLen() > 4
		&& NFmiString(theText.GetCharsPtr(2,4)).IsEqual(longMinus); //suluissa pitk‰ miinus

	  NFmiString widthString, restString;
	  if(!(firstIs226 || secondIs226)) //ei ongelmia
		{
		  WriteShowString(x, y, theText, theText, theDestinationFile);
		}
	  else if(firstIs226)
		{
		  widthString = NFmiString("n"); //HUOM sama leveys kuin pitk‰ll‰ miinuksella kun kerran se itse ei k‰y
		  if(theText.GetLen()>4)
			widthString += theText.GetCharsPtr(5,theText.GetLen()-4);
		  WriteShowString(x, y, widthString, longMinus, theDestinationFile);
		  if(theText.GetLen()>4)
			{
			  restString = theText.GetCharsPtr(5,theText.GetLen()-4);
			  WriteShowString(x, y, restString, restString, theDestinationFile);
			}		  
		}
	  else //suluissa AL 
		{
		  widthString += NFmiString("(");
		  widthString += NFmiString("n"); //HUOM sama leveys kuin pitk‰ll‰ miinuksella kun kerran se itse ei k‰y
		  if(theText.GetLen()>4)
			widthString += theText.GetCharsPtr(6,theText.GetLen()-4);
		  NFmiString first = theText.GetCharsPtr(1,1);
		  WriteShowString(x, y, widthString, first, theDestinationFile);
		  WriteShowString(x, y, widthString, longMinus, theDestinationFile);
		  if(theText.GetLen()>4)
			{
			  restString = theText.GetCharsPtr(5,theText.GetLen()-4);
			  WriteShowString(x, y, restString, restString, theDestinationFile);
			}		  
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
										const NFmiString & theWidthString,
										const NFmiString & theShowString,
										ofstream & os) const
{
  os << x << " " << y << " moveto" << endl;
  if(itsAlignment == kRight )
	{
	  os << "(" << static_cast<char *>(theWidthString) << ") " << "stringwidth" << endl;
	  os << "neg exch neg exch" << endl;
	  os  << " rmoveto" << endl;
	}
  else if (itsAlignment == kCenter)
	{
	  os << "(" << static_cast<char *>(theWidthString) << ") " << "stringwidth" << endl;
	  os << "-2. div exch -2. div exch" << endl; //puolikas matka
	  os  << " rmoveto" << endl;
	}
  if(itsRelCharWidth == 0.) // merkkien levennys/kavennus
	os << "(" << static_cast<char *>(theShowString) << ") show" << endl;
  else
	os << itsRelCharWidth << " 0. (" << static_cast<char *>(theShowString) << ") ashow" << endl;
  
  return true;
}

// ======================================================================
