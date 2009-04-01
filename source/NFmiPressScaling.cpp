// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressScaling
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressScaling.h"
#include "NFmiPressTime.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressScaling::~NFmiPressScaling(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param thePsSymbol The object to copy
 */
// ----------------------------------------------------------------------

NFmiPressScaling::NFmiPressScaling(const NFmiPressScaling & thePsSymbol)
  : NFmiPressTimeDescription(thePsSymbol)
  , NFmiPsWriting(thePsSymbol)
  , itsRectScale(thePsSymbol.itsRectScale)
  , itsRelArea(thePsSymbol.itsRelArea)
  , itsRectSize(thePsSymbol.itsRectSize)
  , itsPlace(thePsSymbol.itsPlace)
  , fPrintOnce(thePsSymbol.fPrintOnce)
  , fScaleNotPlace(thePsSymbol.fScaleNotPlace)
  , itsTimestampDayGap(thePsSymbol.itsTimestampDayGap)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \param theDestinationFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressScaling::Set(NFmiRectScale & theScale,
						   ofstream & theDestinationFile)
{
  itsRectScale = theScale;
  itsOutFile = &theDestinationFile;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressScaling::SetScale(const NFmiRectScale & theScale)
{
  itsRectScale = theScale;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressScaling::ReadRemaining(void)
{
  double r1, r2, r3;
  NFmiValueString valueString;
  switch(itsIntObject)
	{
	case dMakeAsLast:
	  {
		SetWriteLast(true);
		ReadNext();
		break;
	  }
	case dRotate:  //VakioKuvalla oma
	  {
		if(!ReadEqualChar())
		  return false;
		
		if(ReadDouble(r1))
		  {
			if(r1 != 90.)
			  {
				*itsLogFile << "WARNING: vain 90 asteen kierto sallittu, ei "
							<< r1
							<< endl;
				r1 = 90.;
			  }
			itsRotatingAngle = r1;
			
			*itsDescriptionFile >> itsObject;
			valueString = itsObject;
			if(valueString.IsNumeric())
			  {
				r2 = static_cast<double>(valueString);
				
				if(ReadDouble(r3))
				  {
					Place(NFmiPoint(r2,r3));  //riitt‰‰ kun antaa joko Paikan tai RotPaikan
					itsRotatingPoint = NFmiPoint(r2,r3);
				  }
				ReadNext();
			  }
			else
			  {
				itsString = valueString;
				itsIntObject = ConvertDefText(itsString);
			  }
		  }
		else
		  ReadNext();
		break;
	  }
	case dFileTimestamp:
	  {
		// toista avainsanaa pit‰isi ohjelmoida jos vaikka eilist‰
		// teksti‰ (muuttuja kelpaa edlleen)
		itsTimestampDayGap = 0;
		ReadNext();
		break;
	  }
	case dNoLineFeed:
	  {
		SetLineFeed(false);
		ReadNext();
		break;
	  }
	default:
	  {
		NFmiPressTimeDescription:: ReadRemaining();
		break;
	  }
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param object Undocumented
 *†\return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiPressScaling::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if(lowChar==NFmiString("size") ||
	 lowChar==NFmiString("koko"))
	return dSymbolSize;

  else if(lowChar==NFmiString("makeaftersegments") ||
		  lowChar==NFmiString("makeatend") ||
		  lowChar==NFmiString("teesegmenttienj‰lkeen")||
		  lowChar==NFmiString("teelopussa"))
	return dMakeAsLast;

  else if(lowChar==NFmiString("relplace") ||
		  lowChar==NFmiString("osaalue"))
	return dRelSymbolSize;

  else if(lowChar==NFmiString("printOnce") ||
		  lowChar==NFmiString("tulostakerran"))
	return dPrintOnce;

  else if(lowChar==NFmiString("place") ||
		  lowChar==    NFmiString("paikka"))
	return dSymbolPlace;

  else if(lowChar==NFmiString("placemove") ||
		  lowChar==NFmiString("paikansiirto") ||
		  lowChar==NFmiString("suhtpaikka") ||
		  lowChar==NFmiString("suhteellinenpaikka"))
	return dPsPlaceMove;

  else if(lowChar==NFmiString("rotate") ||
		  lowChar==NFmiString("kierto"))
	return dRotate;

  // t‰m‰ ei ole oikea tapa, mutta karsii aika useita virheit‰
  // jotka aihetuvat m‰‰rittelyn v‰‰r‰st‰ j‰rjestyksest‰
  else if(lowChar==NFmiString("stationlist") ||
 		  lowChar==NFmiString("asemataulukko") ||
		  lowChar==NFmiString("asemiensijoitusalue"))
	return dEnd;

  else if(lowChar==NFmiString("timestamp") ||
		  lowChar==NFmiString ("aikaleima"))
	return dFileTimestamp;

  else if(lowChar==NFmiString("nolinefeed") ||
		  lowChar==NFmiString ("eirivinvaihto")||
		  lowChar==NFmiString ("eirivinsiirto"))
	return dNoLineFeed;

  else
	return NFmiPressTimeDescription::ConvertDefText(object);
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFile Undocumented
 *†\param theFormat Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressScaling:: AddValidTimeTimeStamp(NFmiString & theFile,
									 const NFmiString & theFormat,
									 const NFmiPressTime & theValidTime) const
{
  if(theFormat.IsValue())
	{
	  NFmiPressTime pressTime(theValidTime);
	  NFmiString string1(pressTime.InterpretToStr(theFormat));
	  NFmiString string2(theFile);
	  
	  theFile = string1+string2;
	} 
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFile Undocumented
 *†\param theFormat Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressScaling:: AddTimeStamp(NFmiString & theFile,
									 const NFmiString & theFormat) const
{ 
  if(theFormat.IsValue())
	{
	  NFmiPressTime pressTime;
	  NFmiString string1(pressTime.InterpretToStr(theFormat));

#ifndef UNIX
	  unsigned long nSep = theFile.SearchLast(&kFmiDirectorySeparator);
#else
	  unsigned long nSep = static_cast<string>(theFile).find("/");
#endif
	  if(nSep >0 && nSep != string::npos )
	  {
		NFmiString string2;
		string2 = theFile.GetChars(1,nSep);
		string2 += string1;
		string2 += theFile.GetChars(nSep+1,theFile.GetLen()-nSep+1);
        theFile = string2;
	  }
	  else
	  {
		NFmiString string2(theFile);		  
		theFile = string1+string2;
	  }
	}
  
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param defDir Undocumented
 * \param givenPath Undocumented
 * \param givenDir Undocumented
 * \param givenFile Undocumented
 * \param theExtension Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiFileString NFmiPressScaling::CreatePath(NFmiString defDir,
											NFmiString givenPath,
											NFmiString givenDir,
											NFmiString givenFile,
											NFmiString theExtension)
{
  NFmiFileString fileStr;
  if (!givenPath.IsValue())
	{
	  if(!defDir.IsValue() || defDir[1ul] != kFmiDirectorySeparator)
	  {
		  fileStr = GetHome();
		  fileStr += kFmiDirectorySeparator;
	  }
#ifdef UNIX
	  if (static_cast<string>(givenFile).find("/") == string::npos)
#else
	  if (givenFile.Search(&(kFmiDirectorySeparator)) <=0) //sis‰ltyykˆ hekemisto nimeen
#endif
	  {
		fileStr += defDir;
		fileStr += kFmiDirectorySeparator;
	  }
	  fileStr += givenFile;
	}
  else
	{
	  fileStr = givenPath;
	  fileStr.NormalizeDelimiter();
	  if(givenFile.IsValue())
	  {
		// kenoviiva loppuun jos ei m‰‰rittelyss‰
		if(!(fileStr.GetChars(givenPath.GetLen(),1) == NFmiString(kFmiDirectorySeparator)))
			{
			fileStr += kFmiDirectorySeparator;
			}
		  
		fileStr += givenFile;
	  }
	}
  if(givenDir.IsValue())
	{
	  fileStr.ReplaceDirectory(givenDir);
	}
  
  if(theExtension.IsValue() && !fileStr.HasExtension())
	fileStr.Extension(static_cast<char *>(theExtension));
  
  return fileStr;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param place Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressScaling::WritePS(const NFmiPoint & place,
							   FmiPressOutputMode theOutput)
{
  itsPlace = place;
  return WritePS(theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressScaling::	ScalePlotting(void)
{
  NFmiPoint scaledPlace;
  NFmiPoint sizeScaling = itsRectScale.GetScaling();
  if(!fScaleNotPlace)
	scaledPlace = itsRectScale.Scale(itsPlace);
  else
	scaledPlace = itsPlace;
  
  double scaledXSizeHalf = itsRectSize.X() * sizeScaling.X() / 2.;
  double scaledYSizeHalf = itsRectSize.Y() * sizeScaling.Y() / 2.;
  NFmiRect absRect(scaledPlace.X() - scaledXSizeHalf,	//left
				   scaledPlace.Y() + scaledYSizeHalf,	//top
				   scaledPlace.X() + scaledXSizeHalf,	//right
				   scaledPlace.Y() - scaledYSizeHalf);	//bottom
  NFmiRect absRelRect = absRect.ToAbs(itsRelArea);
  itsWriteScale.SetEndScales(absRelRect);
}

// ======================================================================

