// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiHyphenationString
 */
// ======================================================================
/*!
 * \class NFmiHyphenationString
 *
 * Luokka tavuttaa tekstin suomen kielen tavutussäännön
 * (se yksi joka kattaa 96% ? tapauksista) mukaan sijoittamalla
 * sovitun merkin tavatuskohtiin. Käytetään sääsivujen tekstien
 * palstoituksessa. Sisältää mm metodit konsonanttien, vokaalien
 * ja numeeristen merkkien löytämiseen.
 *
 * Myös muita hyödyllisiä meteodeja:
 * ReplaceChar(merkki), NextSubString(erotin)
 *
 */
// ======================================================================

#ifndef NFMIHYPHENATIONSTRING_H
#define NFMIHYPHENATIONSTRING_H

#include "NFmiString.h"

#include <string>
#include <vector>
/*
struct FmiPressHyphens
{
        std::string word;
        int pos;
};
*/

//! Undocumented
class _FMI_DLL NFmiHyphenationString : public NFmiString
{
 public:
  ~NFmiHyphenationString(void);

  NFmiHyphenationString(void);
  NFmiHyphenationString(const char* theText);
  NFmiHyphenationString(const NFmiString& theText);
  NFmiHyphenationString(const NFmiHyphenationString& theText);

  NFmiString CreateHyphens(const char* theHyphenationMark);
  NFmiString DeleteShortSyllables(const char* theHyphenationMark);
  bool NextChar(const NFmiString& theChar);
  bool NextIsNumeric(void);
  NFmiString ReplaceChar(const NFmiString& theChar, const NFmiString& withString);
  NFmiString DropChar(const NFmiString& theChar);
  bool NextConsonant(void);
  bool NextPosition(void);
  bool BackPosition(void);
  void ResetPosition(void);
  void SuperResetPosition(void);
  bool IsConsonant(const NFmiString& theChar) const;
  bool IsVowel(const NFmiString& theChar) const;
  bool IsNumeric(const NFmiString& theChar) const;
  bool IsLastCharPosition(void) const;
  unsigned long CurrentCharPosition(void) const;
  bool NextSubString(const NFmiString& toDelimiter, NFmiString& resString);
  bool NextSubString(NFmiString& resString);
  void SetNarrowColumn(bool theStatus);

 private:
  bool IsPunctuation(unsigned short theChar);
  bool InitIrregularHyphens(void);
  void CreateIrregularHyphens(const char* theHyphenationMark);
  bool EnoughOnRow(void) const;

  unsigned short itsCurrentCharPos;
  unsigned short itsLastCharPosition;
  bool fIrregularHyphensInited;
  std::vector<std::string> itsIrregularHyphens;
  bool fNarrowColumn;

};  // class NFmiHyphenationString

#endif  // NFMIHYPHENATIONSTRING_H

// ======================================================================
