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
//---------------------------------------------------------------------------NFmiHyphenationString.h

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
	bool NextChar(const NFmiString &theChar);
	bool NextIsNumeric(void);
	NFmiString ReplaceChar(const NFmiString &theChar, const NFmiString& withString);
    NFmiString DropChar (const NFmiString &theChar); //1.3.99
	bool NextConsonant(void);
	bool NextPosition(void);
	bool BackPosition(void);
	void ResetPosition(void);
	void SuperResetPosition(void); //16.3.00
	bool IsConsonant(const NFmiString &theChar) const;
	bool IsVowel(const NFmiString &theChar) const;
	bool IsNumeric(const NFmiString &theChar) const;
	bool IsLastCharPosition(void) const;
	unsigned long CurrentCharPosition(void) const;
	bool NextSubString (const NFmiString &toDelimiter, NFmiString& resString); // 5.10.1999/Lasse
	bool NextSubString (NFmiString& resString); // 19.10.1999/Lasse


	private:
		unsigned short itsCurrentCharPos;
		unsigned short itsLastCharPosition;

};

#endif //__NHYPHSTR_H__


