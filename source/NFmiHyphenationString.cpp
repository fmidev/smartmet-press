/*---------------------------------------------------------------------------*/ 
//© Ilmatieteenlaitos/Lasse.               
// 
//Originaali 6.3.1998 
// 
// Joitakin lisäyksiä tekstiksi-koodaukseen
//
//Muutettu 290698/LW +merkin korvaus stringillä (- tavuviivaksi kun illu ei ota "-"merkkiä??)
//Muutettu 290698/LW +kielletyt jakokohdat oct/136 (esim -5)
//Muutettu 010399/LW +DropChar()
//Muutettu 051099/LW +NextSubString()
/*---------------------------------------------------------------------------NFmiHyphenationString.cpp*/

 #include  "NFmiHyphenationString.h"
#include <string> //STL 27.8.01 
 
//---------------------------------------------------------------

NFmiHyphenationString :: NFmiHyphenationString  (void)
{
	ResetPosition();
}

//---------------------------------------------------------------
NFmiHyphenationString :: NFmiHyphenationString  (const char *theText)
:NFmiString(theText) 
{
	ResetPosition();
}

//---------------------------------------------------------------
NFmiHyphenationString :: NFmiHyphenationString  (const NFmiString &theText)
:NFmiString(theText) 
{
	ResetPosition();
}

//---------------------------------------------------------------
NFmiHyphenationString :: NFmiHyphenationString  (const NFmiHyphenationString &theText)
:NFmiString(theText) 
{

}
//---------------------------------------------------------------
NFmiHyphenationString :: ~NFmiHyphenationString  (void)
{
}

//---------------------------------------------------------------
NFmiString NFmiHyphenationString::CreateHyphens(const char *theHyphenationMark)
{
	NFmiString newString;
	ResetPosition();
	while(NextConsonant())
	{
		newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);
		itsCurrentCharPos++;
/*		if(IsLastCharPosition())  //LW
		{
			newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition);
		}
		else */
		if(GetChars(itsCurrentCharPos-2, 1) != NFmiString(" ") &&
		   GetChars(itsCurrentCharPos-3, 1) != NFmiString(" ") && //070199 estetty yhden vokaalin jääminen ed. riville
			    IsVowel(GetChars(itsCurrentCharPos, 1)))
		{
			newString += NFmiString(theHyphenationMark);
;
		}
		itsLastCharPosition = --itsCurrentCharPos;

	}
	if (GetLen() - itsLastCharPosition > 0)            //LW
	   newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition);

	return newString;
}

//---------------------------------------------------------------
NFmiString NFmiHyphenationString::ReplaceChar (const NFmiString &theChar, const NFmiString& withString)
{   //palvelee tavutusta, ei yleinen
	//lisää myös oct=136 kun ei saa jakaa

	NFmiString newString;
	ResetPosition();
	while(NextChar(theChar))
	{
		newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);
		if(NextIsNumeric())        //ei jaeta esim -5 mutta kyllä itä-uusimaa
             newString += NFmiString("\\136");
                                    
		newString += NFmiString(withString);
                                  //16.10 eikö pitäisi ynnätä enemmän jos += NFmiString("\\136")??
		itsCurrentCharPos += 2;

		itsLastCharPosition = --itsCurrentCharPos;

	}
	if (GetLen() - itsLastCharPosition > 0)            //LW
	   newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition+1);

	return newString;

}
// 1.3.99 ---------------------------------------------------------------
NFmiString NFmiHyphenationString:: DropChar (const NFmiString &theChar)
{ 
	//29.3.00 nyt aloittaa ekasta ja muutenkin ok

	NFmiString newString;
//	ResetPosition();  //ekaa ei tarkisteta mutta lehtisäässä ei tarvikaan
	SuperResetPosition();  //16.3.00 tarkistetaan

	while(NextChar(theChar))
	{
		newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);
                                  
//		itsCurrentCharPos += 2;
//		itsLastCharPosition = --itsCurrentCharPos;
		itsLastCharPosition = itsCurrentCharPos+1;

	}
//	int helpLast = FmiMax(itsLastCharPosition,1); //28.3.00 yllä oleva SuperResetPosition ilmeisesti vaatii
//	int helpLast = itsLastCharPosition; 

	if (GetLen() >= itsLastCharPosition)            
	   newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition+1);

	return newString;
}
//---------------------------------------------------------------
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
//---------------------------------------------------------------
bool NFmiHyphenationString::NextChar(const NFmiString &theChar)
{
	while(NextPosition())
	{
//		if(strcmp(GetCharsPtr(itsCurrentCharPos, 1), theChar))
		if(GetChars(itsCurrentCharPos, 1) == theChar)
			return true;
	}
	return false;

}
//---------------------------------------------------------------
bool NFmiHyphenationString::NextConsonant(void)
{
	while(NextPosition())
	{
		if(IsConsonant(GetChars(itsCurrentCharPos, 1)))
			return true;
	}
	return false;

}
//---------------------------------------------------------------
bool NFmiHyphenationString::BackPosition(void)
{
  // Mika: itsCurrentCharPos is unsigned, hence this always fails
  // return itsCurrentCharPos-- < 0;
  // aivan oikein Mika, mutta oleellista on nyt tuo -- eli sitten näin
  // (sitäpaitsi tuo < pitää olla päinvastoin (ei käytetä missään))
  // Mika: Ei seuraavakaan toimi järkevästi, vaihdoin else-haaran
  // return itsCurrentCharPos-- >= 0;

  if(itsCurrentCharPos == 0)
	  return false;
  else
	{
	  --itsCurrentCharPos;
	  return true;
	}
}
//---------------------------------------------------------------
bool NFmiHyphenationString::NextPosition(void)
{
	return itsCurrentCharPos++ < fLength;
}
//---------------------------------------------------------------
void NFmiHyphenationString::ResetPosition(void)
{
	itsCurrentCharPos = 1;
	itsLastCharPosition = 1;
}
//---------------------------------------------------------------
void NFmiHyphenationString::SuperResetPosition(void)
{//16.3.00 jotta Nextillä päästään luuppaamaan
	itsCurrentCharPos = 0;
	itsLastCharPosition = 1; //29.3.00
}
//---------------------------------------------------------------
bool NFmiHyphenationString::IsConsonant(const NFmiString &theChar) const
{
	NFmiString consonants("bdfghjklmnprstv"); 
	for(int i = 1; i <= static_cast<int>(consonants.GetLen()); i++)  
	{
		if(!strcmp(consonants.GetCharsPtr(i,1), theChar))
			return true;
	}
	return false;
}
//---------------------------------------------------------------
bool NFmiHyphenationString::IsNumeric(const NFmiString &theChar) const
{
	NFmiString numerics("0123456789"); 
	for(int i = 1; i <= static_cast<int>(numerics.GetLen()); i++)  
	{
		if(!strcmp(numerics.GetCharsPtr(i,1), theChar))
			return true;
	}
	return false;
}
//---------------------------------------------------------------
bool NFmiHyphenationString::IsVowel(const NFmiString &theChar) const
{
	NFmiString vowels("aeiouyäö");  
	for(int i = 1; i <= static_cast<int>(vowels.GetLen()); i++)
	{
		if(!strcmp(vowels.GetCharsPtr(i,1), theChar))
			return true;
	}
	return false;
}
//---------------------------------------------------------------
unsigned long NFmiHyphenationString::CurrentCharPosition(void) const
{
	return itsCurrentCharPos;
}
//---------------------------------------------------------------
bool NFmiHyphenationString::IsLastCharPosition(void) const
{
	return itsCurrentCharPos >= GetLen();
}
//---------------------------------------------------------------
bool NFmiHyphenationString :: NextSubString (const NFmiString &toDelimiter, NFmiString& resString) // 5.10.1999/Lasse
{
  //ei toimi ilman casteja vaikkei kääntäjä varoita
  if(!IsValue() || static_cast<int>(GetLen())-static_cast<int>(itsCurrentCharPos) <= -1)
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
//---------------------------------------------------------------
bool NFmiHyphenationString :: NextSubString (NFmiString& resString) // 19.10.1999/Lasse
{ //seuraava välilyöntien erottama 
  //ei toimi ilman casteja vaikkei kääntäjä varoita
  if(!IsValue() || static_cast<int>(GetLen())-static_cast<int>(itsCurrentCharPos) <= -1)
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




