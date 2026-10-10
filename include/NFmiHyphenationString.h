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
class NFmiHyphenationString : public NFmiString
{
 public:
  ~NFmiHyphenationString() override;

  NFmiHyphenationString();
  NFmiHyphenationString(const char* theText);
  NFmiHyphenationString(const NFmiString& theText);
  NFmiHyphenationString(const NFmiHyphenationString& theText);

  NFmiString CreateHyphens(const char* theHyphenationMark);
  NFmiString DeleteShortSyllables(const char* theHyphenationMark);
  bool NextChar(const NFmiString& theChar);
  bool NextIsNumeric();
  NFmiString ReplaceChar(const NFmiString& theChar, const NFmiString& withString);
  NFmiString DropChar(const NFmiString& theChar);
  bool NextConsonant();
  bool NextPosition();
  bool BackPosition();
  void ResetPosition();
  void SuperResetPosition();
  bool IsConsonant(const NFmiString& theChar) const;
  bool IsVowel(const NFmiString& theChar) const;
  bool IsNumeric(const NFmiString& theChar) const;
  bool IsLastCharPosition() const;
  unsigned long CurrentCharPosition() const;
  bool NextSubString(const NFmiString& toDelimiter, NFmiString& resString);
  bool NextSubString(NFmiString& resString);
  void SetNarrowColumn(bool theStatus);

 private:
  bool IsPunctuation(unsigned short theChar);
  bool InitIrregularHyphens();
  void CreateIrregularHyphens(const char* theHyphenationMark);
  bool EnoughOnRow() const;

  unsigned short itsCurrentCharPos;
  unsigned short itsLastCharPosition;
  bool fIrregularHyphensInited;
  std::vector<std::string> itsIrregularHyphens;
  bool fNarrowColumn;

};  // class NFmiHyphenationString

#endif  // NFMIHYPHENATIONSTRING_H

// ======================================================================
