// © Ilmatieteenlaitos/Lasse.
// Original 6.3.1998
// 
//********************************************************
// 
// Luokka tavuttaa tekstin suomen kielen tavutussäännön
// (se yksi joka kattaa 96% ? tapauksista) mukaan sijoittamalla
// sovitun merkin tavatuskohtiin. 
// Käytetään sääsivujen tekstien palstoituksessa. 
// Sisältää mm metodit konsonanttien, vokaalien ja numeeristen
// merkkien löytämiseen.
// Myös muita hyödyllisiä meteodeja: ReplaceChar(merkki), NextSubString(erotin)   
//
//********************************************************
//
//Muutettu 190398/LW +itsLanguage    
//Muutettu 010399/LW +DropChar    
//Muutettu 051099/LW NextSubString(toDelimiter)
//Muutettu 191099/LW NextSubString() välilyöntiin
//Muutettu 160300/LW +SuperResetPosition
//---------------------------------------------------------------------------nhyphstr.h

#ifndef __NHYPHSTR_H__
#define __NHYPHSTR_H__

#include "NFmiString.h"

class _FMI_DLL NFmiHyphenationString : public NFmiString

{
	public:
	  NFmiHyphenationString  (void);
	  NFmiHyphenationString  (const char *theText);
	  NFmiHyphenationString  (const NFmiString &theText);
	  NFmiHyphenationString  (const NFmiHyphenationString &theText);

     ~NFmiHyphenationString (void);

	NFmiString CreateHyphens(char *theHyphenationMark);
	FmiBoolean NextChar(const NFmiString &theChar);
	FmiBoolean NextIsNumeric(void);
	NFmiString ReplaceChar(const NFmiString &theChar, const NFmiString& withString);
    NFmiString DropChar (const NFmiString &theChar); //1.3.99
	FmiBoolean NextConsonant(void);
	FmiBoolean NextPosition(void);
	FmiBoolean BackPosition(void);
	void ResetPosition(void);
	void SuperResetPosition(void); //16.3.00
	FmiBoolean IsConsonant(const NFmiString &theChar) const;
	FmiBoolean IsVowel(const NFmiString &theChar) const;
	FmiBoolean IsNumeric(const NFmiString &theChar) const;
	FmiBoolean IsLastCharPosition(void) const;
	unsigned long CurrentCharPosition(void) const;
	FmiBoolean NextSubString (const NFmiString &toDelimiter, NFmiString& resString); // 5.10.1999/Lasse
	FmiBoolean NextSubString (NFmiString& resString); // 19.10.1999/Lasse


	private:
		unsigned short itsCurrentCharPos;
		unsigned short itsLastCharPosition;

};

#endif //__NHYPHSTR_H__


