// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiHyphenationString
 */
// ======================================================================

#include "NFmiHyphenationString.h"
//#include <vector>
//#include <string>
using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiHyphenationString::~NFmiHyphenationString(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

NFmiHyphenationString::NFmiHyphenationString(void)
{
  ResetPosition();
  fIrregularHyphensInited = false;
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

NFmiHyphenationString::NFmiHyphenationString(const char * theText)
  : NFmiString(theText) 
{
  ResetPosition();
  fIrregularHyphensInited = false;
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

NFmiHyphenationString::NFmiHyphenationString(const NFmiString & theText)
  : NFmiString(theText) 
{
  ResetPosition();
  fIrregularHyphensInited = false;
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theText The object being copied
 */
// ----------------------------------------------------------------------

NFmiHyphenationString::NFmiHyphenationString(const NFmiHyphenationString & theText)
  : NFmiString(theText) 
{
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theHyphenationMark Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::InitIrregularHyphens(void)
{
  itsIrregularHyphens.push_back("syd-ostlig");
  itsIrregularHyphens.push_back("syd-östra");
  itsIrregularHyphens.push_back("nord-ostlig");
  itsIrregularHyphens.push_back("nord-östra");
  itsIrregularHyphens.push_back("ster-sjö"); //Östersjön,Östersjöområdet 
  itsIrregularHyphens.push_back("in-ska "); // Finska viken
  itsIrregularHyphens.push_back("pohjois-osa");
  itsIrregularHyphens.push_back("etelä-osa");
  itsIrregularHyphens.push_back("itä-osa");
  itsIrregularHyphens.push_back("länsi-osa");
  itsIrregularHyphens.push_back("om-kring");
  itsIrregularHyphens.push_back("snö-blandat");
  itsIrregularHyphens.push_back("halv-klar");
  itsIrregularHyphens.push_back("annor-städes");
  fIrregularHyphensInited = true;
  return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theHyphenationMark Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiHyphenationString::CreateIrregularHyphens(const char * theHyphenationMark)
{
  if(!fIrregularHyphensInited)
		InitIrregularHyphens();

  std::string stdString(*this);
  
  unsigned long posText = 0;
  unsigned long posChar = 0;
  std::string hyphWord, word; 
  std::vector<std::string>::iterator posWord;
  for(posWord=itsIrregularHyphens.begin(); posWord != itsIrregularHyphens.end(); ++posWord)
  {
		hyphWord = *posWord;
		word = hyphWord;
		posChar = word.find("-");
		int posDiff = 0;
		while(posChar!= string::npos)
		{
            word.erase(posChar, 1);
			hyphWord.replace(posChar+posDiff, 1, theHyphenationMark); 
			posChar = word.find("-");
			posDiff++;
		}
  		posChar = stdString.find(word);
 		while(posChar!= string::npos)
		{
            stdString.replace(posChar,word.size(),hyphWord);
			posChar = stdString.find(word);
		}
 }
  return stdString;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theHyphenationMark Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiHyphenationString::CreateHyphens(const char * theHyphenationMark)
{
  //NFmiString newString = CreateIrregularHyphens(theHyphenationMark);
	
  NFmiString newString;
  ResetPosition();
  while(NextConsonant())
	{
	  newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);
	  itsCurrentCharPos++;

	  if(GetChars(itsCurrentCharPos-2, 1) != NFmiString(" ") &&
		 GetChars(itsCurrentCharPos-3, 1) != NFmiString(" ") &&
		 IsVowel(GetChars(itsCurrentCharPos, 1)))
		{
		  newString += NFmiString(theHyphenationMark);
		}
	  itsLastCharPosition = --itsCurrentCharPos;
	  
	}
  if (GetLen() - itsLastCharPosition > 0)
	newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition);

  return newString;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theChar Undocumented
 * \param withString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiHyphenationString::ReplaceChar(const NFmiString & theChar,
											  const NFmiString & withString)
{
  // palvelee tavutusta, ei yleinen
  // lisää myös oct=136 kun ei saa jakaa

  NFmiString newString;
  ResetPosition();
  while(NextChar(theChar))
	{
	  newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);

	  // ei jaeta esim -5 mutta kyllä itä-uusimaa	  
	  if(NextIsNumeric())
		newString += NFmiString("\\136");
	  
	  newString += NFmiString(withString);

	  // eikö pitäisi ynnätä enemmän jos += NFmiString("\\136")??
	  itsCurrentCharPos += 2;

	  itsLastCharPosition = --itsCurrentCharPos;

	}
  if (GetLen() - itsLastCharPosition > 0)
	newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition+1);

  return newString;

}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theChar Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiHyphenationString::DropChar(const NFmiString & theChar)
{ 
  // nyt aloittaa ekasta ja muutenkin ok

  NFmiString newString;
  SuperResetPosition();

  while(NextChar(theChar))
	{
	  newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);
                                  
	  itsLastCharPosition = itsCurrentCharPos+1;

	}

  if (GetLen() >= itsLastCharPosition)            
	newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition+1);

  return newString;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::NextIsNumeric(void)
{
  NextPosition();
  if(IsNumeric(GetChars(itsCurrentCharPos, 1)))
	{
	  BackPosition();
	  return isTrue;
	}
  BackPosition();
  return isFalse;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theChar Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::NextChar(const NFmiString & theChar)
{
  while(NextPosition())
	{
	  if(GetChars(itsCurrentCharPos, 1) == theChar)
		return true;
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::NextConsonant(void)
{
  while(NextPosition())
	{
	  if(IsConsonant(GetChars(itsCurrentCharPos, 1)))
		return true;
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::BackPosition(void)
{
  if(itsCurrentCharPos == 0)
	return false;
  else
	{
	  --itsCurrentCharPos;
	  return true;
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::NextPosition(void)
{
  return itsCurrentCharPos++ < fLength;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiHyphenationString::ResetPosition(void)
{
  itsCurrentCharPos = 1;
  itsLastCharPosition = 1;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiHyphenationString::SuperResetPosition(void)
{
  itsCurrentCharPos = 0;	// jotta Nextillä päästään luuppaamaan
  itsLastCharPosition = 1;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theChar Undocumented
 * \return Undocumented
 * \todo Make loop index unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::IsConsonant(const NFmiString & theChar) const
{
  NFmiString consonants("bcdfghjklmnpqrstvwxz"); 
  for(int i = 1; i <= static_cast<int>(consonants.GetLen()); i++)  
	{
	  if(!strcmp(consonants.GetCharsPtr(i,1), theChar))
		return true;
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theChar Undocumented
 * \return Undocumented
 * \todo Make loop index unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::IsNumeric(const NFmiString & theChar) const
{
  NFmiString numerics("0123456789"); 
  for(int i = 1; i <= static_cast<int>(numerics.GetLen()); i++)  
	{
	  if(!strcmp(numerics.GetCharsPtr(i,1), theChar))
		return true;
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theChar Undocumented
 * \return Undocumented
 * \todo Make loop index unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::IsVowel(const NFmiString & theChar) const
{
  NFmiString vowels("aeiouyäöå");  
  for(int i = 1; i <= static_cast<int>(vowels.GetLen()); i++)
	{
	  if(!strcmp(vowels.GetCharsPtr(i,1), theChar))
		return true;
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

unsigned long NFmiHyphenationString::CurrentCharPosition(void) const
{
  return itsCurrentCharPos;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::IsLastCharPosition(void) const
{
  return itsCurrentCharPos >= GetLen();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param toDelimiter Undocumented
 * \param resString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::NextSubString(const NFmiString & toDelimiter,
										  NFmiString & resString)
{
  //ei toimi ilman casteja vaikkei kääntäjä varoita
  if(!IsValue() ||
	 static_cast<int>(GetLen())-static_cast<int>(itsCurrentCharPos) <= -1)
	{
	  resString = NFmiString();
	  return false;
	}
  
  unsigned long first = itsCurrentCharPos;
  while(NextPosition())
	{
	  if(GetChars(itsCurrentCharPos, 1) == toDelimiter)
		break;
	}
  resString = NFmiString(GetCharsPtr(first, itsCurrentCharPos-first));
  NextPosition(); //erotin pois
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param resString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::NextSubString(NFmiString & resString)
{
  // seuraava välilyöntien erottama 
  // ei toimi ilman casteja vaikkei kääntäjä varoita

  if(!IsValue() ||
	 static_cast<int>(GetLen())-static_cast<int>(itsCurrentCharPos) <= -1)
	{
	  resString = NFmiString();
	  return false;
	}
  
  //alkuvälilyönnit pois
  while(GetChars(itsCurrentCharPos, 1) == NFmiString(" "))
	{
	  if(!NextPosition())
		{
		  resString = NFmiString();
		  return false;
		}
	}
  
  unsigned long first = itsCurrentCharPos;
  while(NextPosition())
	{
	  if(GetChars(itsCurrentCharPos, 1) == NFmiString(" "))
		break;
	}
  resString = NFmiString(GetCharsPtr(first, itsCurrentCharPos-first));
  //  NextPosition(); //erotin pois
  return true;
}

// ======================================================================





