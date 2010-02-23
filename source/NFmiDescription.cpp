// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiDescription
 */
// ======================================================================

#include "NFmiDescription.h"
#include "NFmiFileString.h"
#include <iostream>
#include <list>
#include <string>

#ifdef UNIX
	extern std::list<std::string> errors;
#else
	std::list<std::string> errors;
#endif

using namespace std;

//! Undocumented
static unsigned char kSecondC = '\"';

 // ----------------------------------------------------------------------
/*!
 * \param category W=Warning, E=Error, other->no prefix
 * \param finReport report in Finnish
 * \param engReport report in English
 * \param variable failing variable, optional
 */
// ----------------------------------------------------------------------
void NFmiDescription::OutputLog(const char category, const NFmiString & finReport,
								const NFmiString & engReport, NFmiString * variable) 
{
	  bool finnish = true; //tee globaali muuttuja t‰lle
	  NFmiFileString logString; //jotta ‰‰kkˆset saadaan pois
	  if(category == 'W')
		  logString = NFmiString("WARNING: ");
	  else if(category == 'E')
		  logString = NFmiString("*** ERROR: ");

	  if(finnish)
		  logString.Add(finReport);
	  else
		  logString.Add(engReport);

	  if(variable)
	  {
			NFmiString shortString;
			if(variable->GetLen() > 20)
			{
				shortString = variable->GetChars(1, 20);
				logString.Add(shortString);
				logString.Add(NFmiString("..."));
			}
			else
			{
				shortString = *variable;
				logString.Add(shortString);
			}
	  }
	  char* charString =  static_cast<char *>(logString);
  	  *itsLogFile << charString << endl;
	  if(category == 'W' || category == 'E')
	  {
		  logString.ChangeScandinavian();
		  cout << static_cast<char *>(logString) << endl;
	  }
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param retValue1 Undocumented
 * \param retValue2 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiDescription::ReadMaxTwoFloatToDelimiter(float & retValue1,
												 float & retValue2)
{
  // false kun lˆytynyt muu stringi kuin pilkku eli tiedoston nimi,
  // jolloin kutsuva ohjelma lopettaa luupin

  retValue1 = kFloatMissing;
  retValue2 = kFloatMissing;

  *itsDescriptionFile >> itsObject;
  itsValueHelpString = itsObject;
  if(itsValueHelpString.IsNumeric())
	{
	  retValue1 = static_cast<float>(itsValueHelpString);
	}
  else 
	{
	  if(itsValueHelpString == NFmiString(","))
		return true;
	  else if(itsValueHelpString == NFmiString("-,"))
		{
		  return true;
		}
	  else if(itsValueHelpString == NFmiString("-"))
		{
		  //floatmissing, on jo
		  *itsDescriptionFile >> itsObject;
		  itsString = itsObject;
		  if(itsString == NFmiString(","))
			return true;
		  else
			return false;
		}
	  else //tiedostonimi
			return false;
	}
  
  *itsDescriptionFile >> itsObject;
  itsValueHelpString = itsObject;
  if(itsValueHelpString.IsNumeric())
	{
	  retValue2 = static_cast<float>(itsValueHelpString);
	  // luetaan aina seuraava stringi itsObjektiin	  
	  *itsDescriptionFile >> itsObject;
	  itsString = itsObject;
	  if(itsString == NFmiString(","))
		return true;
	  else
		return false;
	}
  else if(itsValueHelpString == NFmiString("-,"))
	{
	  return true;
	}
  else if(itsObject == NFmiString("-"))
	{
	  //floatmissing, on jo
	  *itsDescriptionFile >> itsObject;
	  itsString = itsObject;
	  if(itsString == NFmiString(","))
		return true;
	  else
		return false;
	}
  else if(itsValueHelpString == NFmiString(","))
	return true;
  else
	return false;
  
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiDescription::Set1Double(double & theMember)
{
  bool ok = false;
  double aDouble;
  if(ReadEqualChar())
	{
	  if(ReadDouble(aDouble))
		{
		  theMember = aDouble;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiDescription::Set1Long(long & theMember)
{
  bool ok = false;
  long aLong;
  if(ReadEqualChar())
	{
	  if(ReadLong(aLong))
		{
		  theMember = aLong;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiDescription::Set1UnsignedLong(unsigned long & theMember)
{
  bool ok = false;
  unsigned long aLong;
  if(ReadEqualChar())
	{
	  if(ReadUnsignedLong(aLong))
		{
		  theMember = aLong;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiDescription::ReadNext(void)
{
  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// mahdollistaa myˆs "-merkeill‰ ymp‰rˆidyn merkkijonon -> v‰lilyˆnti
// voi sis‰lty‰

NFmiString NFmiDescription::ReadString(void)
{
  *itsDescriptionFile >> itsObject;

  // etsit‰‰n "-merkin pari, kaikki luettuun muuttujaan
  if(itsObject[0] == kSecondC)
	{
	  char helpChar [255];
	  NFmiString helpObject;
	  
	  helpObject = itsObject;
	  helpObject = helpObject.GetChars(2, helpObject.GetLen()-1);
	  short loop = 0;

	  // korkeintaan 10 (erillist‰)v‰lilyˆnti
	  // tarkistus jotta ei jouduta luuppiin

	  while (helpObject.Search(&kSecondC) <=0 && loop < 11)
		{
		  *itsDescriptionFile >> helpChar;
		  loop++;
		  helpObject += NFmiString(" "); // HUOM vain yksi vˆlilyˆnti aina
		  helpObject += helpChar;
		}

	  if(loop >= 11)
		{
		  string msg = string("lainausmerkilt‰ puuttuu pari: ")+static_cast<char *>(helpObject);
		  errors.push_back(msg);
		  *itsLogFile << "*** ERROR: " << msg << endl;
		}

	  return helpObject.GetChars(1, helpObject.GetLen()-1);

	}
  else
	return itsObject;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 * \todo Use '=' instead of 61
 */
// ----------------------------------------------------------------------

bool NFmiDescription::ReadEqualChar(void)
{
  //tarkistetaan ett‰ todella on annettu "=" -merkki
  *itsDescriptionFile >> itsObject;
  if(itsObject[0] == 61)
	{
	  return true;
	}
  itsValueHelpString = itsObject;

  string msg = string("Yht‰kuin-merkki (=) puuttuu: ")+static_cast<char *>(itsValueHelpString);
  errors.push_back(msg);
  *itsLogFile << "*** ERROR: " << msg << endl;
  ReadNext();
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue Undocumented
 * \return Undocumented
 * \todo Remove the strange cast sequence
 */
// ----------------------------------------------------------------------

bool NFmiDescription::ReadUnsignedLong(unsigned long & retValue)
{
  *itsDescriptionFile >> itsObject;
  itsValueHelpString = itsObject;
  if(itsValueHelpString.IsNumeric())
	{
	  retValue = static_cast<unsigned long>(static_cast<int>(itsValueHelpString));
	  return true;
	}
  retValue = 0;

  if(itsLogFile)
	{
	  string msg = string("Pit‰‰ olla luku: ")+static_cast<char *>(itsValueHelpString);
	  errors.push_back(msg);
	  *itsLogFile << "*** ERROR: " << msg << endl;
	}

  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue Undocumented
 * \param errorReport Undocumented
 * \return Undocumented
 *
 * \todo Remove The strange cast sequence
 */
// ----------------------------------------------------------------------

bool NFmiDescription::ReadLong(long & retValue, bool errorReport)
{
  *itsDescriptionFile >> itsObject;
  itsValueHelpString = itsObject;
  if(itsValueHelpString.IsNumeric())
	{
	  retValue = static_cast<long>(static_cast<int>(itsValueHelpString));
	  return true;
	}
  retValue = 0;

  if(errorReport)
	{
	  string msg = string("Pit‰‰ olla luku: ")+static_cast<char *>(itsValueHelpString);
	  errors.push_back(msg);
	  *itsLogFile << "*** ERROR: " << msg << endl;
	}

  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiDescription::ReadDouble(double & retValue)
{
  *itsDescriptionFile >> itsObject;
  itsValueHelpString = itsObject;
  if(itsValueHelpString.IsNumeric())
	{
	  retValue = static_cast<double>(itsValueHelpString);
	  return true;
	}
  retValue = 0.;
  
  if(itsLogFile)
	{
	  string msg = string("Pit‰‰ olla luku: ")+static_cast<char *>(itsValueHelpString);
	  errors.push_back(msg);
	  *itsLogFile << "*** ERROR: " << msg << endl;
	}

  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue1 Undocumented
 * \param retValue2 Undocumented
 * \param retValue3 Undocumented
 * \param retValue4 Undocumented
 * \return Undocumented
 *
 * \todo Remove the unnecessary parenthesis
 */
// ----------------------------------------------------------------------

bool NFmiDescription::Read4Double(double & retValue1,
								  double & retValue2,
								  double & retValue3,
								  double & retValue4)
{
  // Sulut ovat takaavinaan oikean j‰rjestyksen, mutta eikˆ se
  // ole kuitenkin vasemmalta oikealle standardin mukaan? (Mika)
  if((((ReadDouble(retValue1)) &&
	   ReadDouble(retValue2)) &&
	  ReadDouble(retValue3)) &&
	 ReadDouble(retValue4))
	{
	  return true;
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue1 Undocumented
 * \param retValue2 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiDescription::Read2Double(double & retValue1, double & retValue2)
{
  if((ReadDouble(retValue1)) && ReadDouble(retValue2))
	{
	  return true;
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 *†\return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiDescription::ConvertDefText(NFmiString & object) 
{
  unsigned long len = object.GetLen(); 
  if(len >= 2 && 
	 NFmiString(object.GetCharsPtr(1,2)) == NFmiString("/*"))
	{
	  if(NFmiString(object.GetCharsPtr(len-1,2)) != NFmiString("*/"))
		itsCommentLevel++;
	  return dComment;
	}
  else if(len >= 2 && 
		  NFmiString(object.GetCharsPtr(len-1,2)) == NFmiString("*/"))
	{
	  itsCommentLevel--;
	  return dEndComment;
	}
  else if(len >= 2 &&
		  NFmiString(object.GetCharsPtr(1,2)) == NFmiString("//"))
	return dComment; 

  else if(len >= 1 && NFmiString(object.GetCharsPtr(1,1)) == NFmiString("#"))
	{
	  return dEnd;
	}
  else
	{
	  return dOther;
	}
}

// ======================================================================


