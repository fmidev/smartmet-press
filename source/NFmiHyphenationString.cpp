// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiHyphenationString
 */
// ======================================================================

#include "NFmiHyphenationString.h"
#include <string>
 
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

NFmiString NFmiHyphenationString::CreateHyphens(const char * theHyphenationMark)
{
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
  // lis�� my�s oct=136 kun ei saa jakaa

  NFmiString newString;
  ResetPosition();
  while(NextChar(theChar))
	{
	  newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);

	  // ei jaeta esim -5 mutta kyll� it�-uusimaa	  
	  if(NextIsNumeric())
		newString += NFmiString("\\136");
	  
	  newString += NFmiString(withString);

	  // eik� pit�isi ynn�t� enemm�n jos += NFmiString("\\136")??
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
  itsCurrentCharPos = 0;	// jotta Nextill� p��st��n luuppaamaan
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
  NFmiString consonants("bdfghjklmnprstv"); 
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
  NFmiString vowels("aeiouy��");  
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
  //ei toimi ilman casteja vaikkei k��nt�j� varoita
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
  // seuraava v�lily�ntien erottama 
  // ei toimi ilman casteja vaikkei k��nt�j� varoita

  if(!IsValue() ||
	 static_cast<int>(GetLen())-static_cast<int>(itsCurrentCharPos) <= -1)
	{
	  resString = NFmiString();
	  return false;
	}
  
  //alkuv�lily�nnit pois
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





