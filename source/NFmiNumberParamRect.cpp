// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiNumberParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiNumberParamRect.h"
#include "NFmiHyphenationString.h"
#include "NFmiPressParam.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiNumberParamRect::~NFmiNumberParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theNumberParamRect The object to copy
 */
// ----------------------------------------------------------------------

NFmiNumberParamRect::NFmiNumberParamRect(const NFmiNumberParamRect & theNumberParamRect)
  : NFmiTextParamRect(theNumberParamRect)
  , itsFormat(theNumberParamRect.itsFormat)
  , fZeroMinus(theNumberParamRect.fZeroMinus)
  , fDetachSign(theNumberParamRect.fDetachSign)
  , fDotToComma(theNumberParamRect.fDotToComma)
{
}

// ----------------------------------------------------------------------
/*!
 * Make a copy of this
 *
 * \return The clone
 * \todo Should return an auto_ptr
 * \todo The static_cast is most likely unnecessary
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmiNumberParamRect::Clone(void) const
{
	return static_cast<NFmiParamRect *>(new NFmiNumberParamRect(*this));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiNumberParamRect::ReadDescription(NFmiString & retString)
{
  long long1;
  double r1,r2,r3,r4;
  itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));

  itsFont = NFmiString("Helvetica");
  itsAlignment = kCenter;
  itsFormat = NFmiString("%.f");

  SetPreReadingTimes();
  itsMapping = new NFmiParamMapping;
  itsModifier = kNoneModifier;

  ReadNext();

  itsRelRect -= NFmiPoint(1., 1.);

  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Numerossa" << endl;
		  retString = itsString;
		  return false;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #Numerossa: "
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
		case dAlignment:
		  {
			if (!ReadEqualChar())
			  break;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
			if (itsString == NFmiString ("Center") ||
				itsString == NFmiString ("KeskiPiste") ||
				itsString == NFmiString ("Keski"))
			  itsAlignment = kCenter;
			else if (itsString == NFmiString ("Right") ||
					 itsString == NFmiString ("OikeaLaita") ||
					 itsString == NFmiString ("Oikea"))
			  itsAlignment = kRight;
			else if (itsString == NFmiString ("Left") ||
					 itsString == NFmiString ("VasenLaita") ||
					 itsString == NFmiString ("Vasen"))
			  itsAlignment = kLeft;
			else
			  *itsLogFile << "*** ERROR: Tuntematon kohdistus #Numerossa: "
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
			itsFont = itsObject;

			ReadNext();
			break;
		  }
		case dFormat:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;
			itsFormat = itsObject;

			ReadNext();
			break;
		  }
		case dStyle:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;
			itsStyle = itsObject;

			ReadNext();
			break;
		  }
		case dPlaceMove:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read2Double(r1,r2))
			  {
				itsRelRect += NFmiPoint(r1/c40, r2/c40);
			  }

			ReadNext();
			break;
		  }
		case dRelPlace:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(r1,r2,r3,r4))
			  {
				itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
			  }
			fNewScaling = false;
			ReadNext();
			break;
		  }
		case dParamSize:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(r1))
			  {
				itsRelRect.Inflate(-(c40-r1)/(c40*2));
			  }

			ReadNext();
			break;
		  }

		case dProducer:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadLong(long1))
			  long1=1;  //dummy koska tuottaja hylätty

			ReadNext();
			break;
		  }
		case dColorValueDependent: //poistettu
		  {
			ReadNext();
			break;
		  }

		case dParenthesis:
		  {
			fParenthesis = true;

			ReadNext();
			break;
		  }

		case dZeroMinus:
		  {
			fZeroMinus = true;

			ReadNext();
			break;
		  }

		case dDetachSign:
		  {
			fDetachSign = true;

			ReadNext();
			break;
		  }
		case dRelDay:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadLong(long1))
			  itsFirstDeltaDays = static_cast<unsigned short>(long1+ itsEnvironment.GetDayAdvance());

			ReadNext();
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Päiviä ei voi asettaa #Numerossa"  << endl;
			break;
		  }
		case dHour:
		  {
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
			  itsFirstPlotHours = static_cast<unsigned short>(long1);

			ReadNext();
			break;
		  }
		case dDotToComma:
		  {
			fDotToComma = true;

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

  SetPostReadingTimes();

  if(fNewScaling)
	itsRelRect += NFmiPoint(1.,1.);
  Set(NFmiDataIdent(NFmiParam(itsIdentPar),240),NFmiRect(itsRelRect));

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

bool NFmiNumberParamRect::ReadRemaining(void)
{
  return NFmiTextParamRect::ReadRemaining();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiNumberParamRect::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if(lowChar==NFmiString("format") || lowChar==NFmiString("formaatti"))
	return dFormat;
  else if(lowChar==NFmiString("zerowithminus") || lowChar==NFmiString("nollaviivalla"))
	return dZeroMinus;
  else if(lowChar==NFmiString("detachsign") || lowChar==NFmiString("irrotaetumerkki"))
	return dDetachSign;
  else if(lowChar==NFmiString("dottocomma") || lowChar==NFmiString("pistepilkuksi"))
	return dDotToComma;
  else
	return NFmiTextParamRect::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param AbsoluteRectOfSymbolGroup Undocumented
 * \param theQI Undocumented
 * \param theDestinationFile Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiNumberParamRect::WritePS(const NFmiRect & AbsoluteRectOfSymbolGroup,
								  NFmiFastQueryInfo * theQI,
								  ofstream & theDestinationFile,
								  FmiPressOutputMode theOutput)
{
  float value;
  itsCurrentSegmentTime = (static_cast<NFmiQueryInfo *>(theQI))->Time();
  itsCurrentTime = itsCurrentSegmentTime;

  if(fUseFromStorage)
	{
	  value = itsPressParam->UseFromStorage(itsStorageQueue);
	}
  else
	{
	  if(!PointOnParam(theQI, GetDataIdent().GetParam()))
		return false;

	  if(!SetRelativeHour(theQI,NFmiString("#Numero")))
		return false;

	  if(!ReadCurrentValue(theQI, itsCurrentParamValue))
		return false;

	  value = itsCurrentParamValue;
	}

  if(!ActiveTimeIndex(itsPressParam->GetCurrentStep()))
	{
	  if(itsLoopActivity.bypassWithValue)
		value = itsLoopActivity.bypassValue;
	  else
		return true;
	}

  if(fPutInStorage)
	{
	  itsPressParam->PutInStorage(value, itsStorageQueue);
	  return true;
	}

  NFmiString format = itsFormat;

  NFmiHyphenationString hypString(itsFormat);
  bool isParen = false;
  // esim formaatti=%(+)1.f -> kaksinumeroiset ilman plussaa; pelkkä ( tai ) riittää
  if(itsFormat.Search(NFmiString("(+")) > 0 || itsFormat.Search(NFmiString("+)")) > 0)
	{
	  if (value >= 9.5)
		hypString = hypString.DropChar(NFmiString("+"));
	  isParen = true;
	}
  // esim formaatti=%.(1)f -> alle yhden desimaalin kanssa, muuten ilman
  if(itsFormat.Search(NFmiString(".(")) > 0 || itsFormat.Search(NFmiString(")f")) > 0)
	{
	  if (fabs(value) >= .95)
		hypString = hypString.ReplaceChar(NFmiString("1"), (NFmiString("0")));
	  isParen = true;
	}

  if(isParen)
	{
	  hypString = hypString.DropChar(NFmiString("("));
	  hypString = hypString.DropChar(NFmiString(")"));
	  format = hypString;
	}

  // tasan nolla pirretään ilman desimaaliosia
  // tehty maaseudun tulevaisuutta varten, häiritseeköhän jossain

  if(value == 0.) format = NFmiString("%d");

  NFmiValueString str;
  NFmiString* mapString =0;

  if(itsMapping)  //on aina luotu vaikkei mapattu, mapString==0 ilmaisee puuttuvaa mappausta
	{
	  mapString = itsMapping->Map(value);
	}

  if(mapString) // mappingmahdollisuus myös numerolle
	{
	  if(*mapString == NFmiString("None"))
		return false;

	  NFmiString helpS(*mapString);  // mappauksessa olevat miinukset pitkinä haluttaessa
	  if(helpS == NFmiString("-") && itsEnvironment.GetLongNumberMinus())
		helpS = NFmiString("\\226");
	  return WriteCode(Construct(&helpS), AbsoluteRectOfSymbolGroup,
					   theDestinationFile, NFmiString("NUMERO"), theOutput);
	}
  else
	{

	  if(fParenthesis)
		{
		  str = NFmiString("(");
		}
	  if(fUseFromStorageInFrontOf)
		{
		  float inFrontOfValue = itsPressParam->UseFromStorage(itsStorageQueue);
	      if(inFrontOfValue == kFloatMissing)  // NORMAALI POS. TAI NEG. ARVO
		  {
			  if(!itsEnvironment.GetLongNumberMinus())
				{
				  str += NFmiString ("-");
				}
			  else
				{
				  str += NFmiString("\\226");

				}
		  }
		  else
			str += NFmiValueString(inFrontOfValue,format);
		}
	  if(value == 0 && fZeroMinus)   //NOLLA PUUTTUVANA
		{
		  if(!itsEnvironment.GetLongNumberMinus())
			{
			  str += NFmiString ("-");
			}
		  else
			{
			  str += NFmiString("\\226");

			}
		}
	  else if(value != kFloatMissing)  // NORMAALI POS. TAI NEG. ARVO
		if(value >= 0. || !itsEnvironment.GetLongNumberMinus())
		  {
			str += NFmiValueString (value,format);
		  }
		else
		  {
			//str += NFmiString("\\226");
			str += NFmiString("\\226 ");
			NFmiHyphenationString helpFormat = format;
			NFmiString hFormat = helpFormat.DropChar("+");
			//tähän välilyönti
			str += NFmiValueString (static_cast<float>(fabs(value)),hFormat);
		  }
	  else      // PUUTTUVA
		{
		  if(!itsEnvironment.GetLongNumberMinus())
			str += NFmiString("-");
		  else
			str += NFmiString("\\226");

		}
	  if(fParenthesis)
		{
		  str += NFmiString(")");
		}
	  if(fDotToComma)
	  {
		str.ReplaceChars(NFmiString("."), NFmiString(","));
	  }
	  
	} //ei-mappi luuppi

  str = DetachSign(str, value);

  fUseSelectLatinFont = value >= 0.  // astemerkki edes positiivisille
	&& value != kFloatMissing
	|| value == kFloatMissing       // pitkät miinukset näkyviin
	&& itsEnvironment.GetLongNumberMinus();

  return WriteCode(Construct(&str), AbsoluteRectOfSymbolGroup,
				   theDestinationFile, NFmiString("NUMERO"), theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theString Undocumented
 * \param theValue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

//tehty Hesaria varten, joka haluaa välilyönnin etumerkin jälkeen jos yksinumeroinen luku
//myös kaksinumeroisessa pitää (pitkä) miinus siirtää jottei päällekkäin
//ei universaali vielä, esim jos sulkuja mukana tms

NFmiString NFmiNumberParamRect::DetachSign(const NFmiString & theString, float theValue) const
{
  NFmiString newString;

// myös HS lehti saa luvan hyväksyä välilyönti kaksinumeroisiin pakkasiin
// nyt laitoin mukaan koska webbiautomatiikka edellyttää
//  if(fDetachSign && theValue < 9.5 && theValue > -9.5)
  if(fDetachSign && theValue < 9.5 && theValue)
	{
	  NFmiString firstChar(theString.GetChars(1,1));
	  NFmiString numString = theString.GetChars(2, theString.GetLen()-1);
	  if(firstChar == NFmiString("+"))
		{
		  newString = NFmiString("+ ");
		  newString += numString;
		}
	  else if(firstChar == NFmiString("-"))
		{
		  newString = NFmiString("- ");
		  newString += numString;
		}
	  else if(theString.GetChars(1, 4) == NFmiString("\\226"))
		{
		  newString = NFmiString("\\226  ");
		  newString += theString.GetChars(5, theString.GetLen()-4);
		}
	  else
		newString = theString;
	}
  else
	newString = theString;

  return newString;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

FmiGenericColor NFmiNumberParamRect::MapColor(void) const
{
  for(unsigned long ind=0; ind < itsCurrentNumOfColMaps; ind++)
	{
	  if(itsCurrentParamValue >= itsColorMapping[ind].minValue &&
		 itsCurrentParamValue <= itsColorMapping[ind].maxValue)
		{
		  return itsColorMapping[ind].color;
		}
	}
  return GetColor();
}

// ======================================================================
