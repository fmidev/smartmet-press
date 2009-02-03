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
  // # = not allowed to hyphenate, tarvitaanko
  itsIrregularHyphens.push_back("pohjois-os"); //osa, osissa
  itsIrregularHyphens.push_back("etelä-os");
  itsIrregularHyphens.push_back("itä-os");
  itsIrregularHyphens.push_back("länsi-os");
  itsIrregularHyphens.push_back("maan-os");
  itsIrregularHyphens.push_back("kaakkois-os");
  itsIrregularHyphens.push_back("lounais-os");
  itsIrregularHyphens.push_back("luoteis-os");
  itsIrregularHyphens.push_back("koillis-os");
  itsIrregularHyphens.push_back("sää-ennuste");
  itsIrregularHyphens.push_back("sade-alue");
  itsIrregularHyphens.push_back("kuuro-alue");

  itsIrregularHyphens.push_back("syd-ost");
  itsIrregularHyphens.push_back("syd-östra");
  itsIrregularHyphens.push_back("nord-ost");
  itsIrregularHyphens.push_back("nord-östra");
  itsIrregularHyphens.push_back("nord-sjö");
  itsIrregularHyphens.push_back("öster-sjö");  
  itsIrregularHyphens.push_back("öster-i-från");  
  itsIrregularHyphens.push_back("väster-i-från");  
  itsIrregularHyphens.push_back("fin-ska "); 
  itsIrregularHyphens.push_back("nor-ska "); 
  itsIrregularHyphens.push_back("om-kring");
  itsIrregularHyphens.push_back("snö-blandat");
  itsIrregularHyphens.push_back("halv-klar");
  itsIrregularHyphens.push_back("annor-städes");
  itsIrregularHyphens.push_back("annan-stans");
  itsIrregularHyphens.push_back("regn-skur");
  itsIrregularHyphens.push_back("åsk-skur");
  itsIrregularHyphens.push_back("gan-ska");
  itsIrregularHyphens.push_back("land-skape");
  itsIrregularHyphens.push_back("meller-sta");
  itsIrregularHyphens.push_back("skogs-brand");
  itsIrregularHyphens.push_back("väder-utsikt");
  itsIrregularHyphens.push_back("väder-prognos");
  itsIrregularHyphens.push_back("kust-område");
  itsIrregularHyphens.push_back("havs-område");
  itsIrregularHyphens.push_back("land-område");
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
  unsigned long posCharNot = 0;
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
			//posCharNot = word.find("#");
			//if(posCharNot!= string::npos)
			//	word.erase(posCharNot, 1);

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
		//sama isoille alkukirjaimille, 
		word[0] = toupper(word[0]);
 		hyphWord[0] = toupper(hyphWord[0]);
  		posChar = stdString.find(word);
 		while(posChar!= string::npos)
		{
            stdString.replace(posChar,word.size(),hyphWord);
			posChar = stdString.find(word);
			word[0] = toupper(word[0]);
 		    hyphWord[0] = toupper(hyphWord[0]);
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
  
	  if(GetChars(itsCurrentCharPos-2, 1) != NFmiString(" ") &&  //ei sanan alkuun
		 GetChars(itsCurrentCharPos-3, 1) != NFmiString(" ") &&  // eikä jätetä yksi kirjain alkuun
		                                    // annetun epäsäännöllisen jälkeen väh. 2 merkkiä
		 GetChars(itsCurrentCharPos-2, 1) != NFmiString(theHyphenationMark) && 
		 GetChars(itsCurrentCharPos-3, 1) != NFmiString(theHyphenationMark) && 
//		 GetChars(itsCurrentCharPos-2, 1) != NFmiString("#") &&  //kieltomerkki, tarvitaanko
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





