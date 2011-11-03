// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiHyphenationString
 */
// ======================================================================

#include "NFmiHyphenationString.h"
//#include <vector>
#include <cstring>
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
  fNarrowColumn = false;
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
  fNarrowColumn = false;
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
  fNarrowColumn = false;
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
	fIrregularHyphensInited = theText.fIrregularHyphensInited;
    itsIrregularHyphens = theText.itsIrregularHyphens;
	fNarrowColumn = theText.fNarrowColumn;
}
// ----------------------------------------------------------------------
/*!
 *
 * \param 
 */
// ----------------------------------------------------------------------

void NFmiHyphenationString::SetNarrowColumn(bool theStatus) 
{
	fNarrowColumn = theStatus;
}

// ----------------------------------------------------------------------
/*!
 *
 * upper case starting char need not be listed separately
 * 
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::InitIrregularHyphens(void)
{
  // # = not allowed to hyphenate, needed??
  itsIrregularHyphens.push_back("pohjois-os"); //osa, osissa
  itsIrregularHyphens.push_back("etel�-os");
  itsIrregularHyphens.push_back("it�-os");
  itsIrregularHyphens.push_back("l�nsi-os");
  itsIrregularHyphens.push_back("maan-os");
  itsIrregularHyphens.push_back("kaakkois-os");
  itsIrregularHyphens.push_back("lounais-os");
  itsIrregularHyphens.push_back("luoteis-os");
  itsIrregularHyphens.push_back("koillis-os");
  itsIrregularHyphens.push_back("keski-os");
  itsIrregularHyphens.push_back("s��-ennuste");
  itsIrregularHyphens.push_back("sade-alue");
  itsIrregularHyphens.push_back("kuuro-alue");
  itsIrregularHyphens.push_back("meri-alue");
  itsIrregularHyphens.push_back("alu-ei");
  itsIrregularHyphens.push_back("pakkas-aste");
  itsIrregularHyphens.push_back("l�mp�-aste");
  itsIrregularHyphens.push_back("p��-osin");
  itsIrregularHyphens.push_back("pakkas-aamu");
  itsIrregularHyphens.push_back("pakkas-y�");
  itsIrregularHyphens.push_back("pakkas-�i");
  itsIrregularHyphens.push_back("halla-y�");
  itsIrregularHyphens.push_back("tai-y�");
  itsIrregularHyphens.push_back("tai-aamu");
  itsIrregularHyphens.push_back("tai-il");  //ilta, illalla
  itsIrregularHyphens.push_back("huomis-aamu");
  itsIrregularHyphens.push_back("huomis-il");
  itsIrregularHyphens.push_back("huomis-y�");

  itsIrregularHyphens.push_back("syd-ost");
  itsIrregularHyphens.push_back("syd-�stra");
  itsIrregularHyphens.push_back("nord-ost");
  itsIrregularHyphens.push_back("nord-�stra");
  itsIrregularHyphens.push_back("nord-sj�");  
  itsIrregularHyphens.push_back("�ster-sj�");  
  itsIrregularHyphens.push_back("�ster-i-fr�n");  
  itsIrregularHyphens.push_back("v�ster-i-fr�n");  
  itsIrregularHyphens.push_back("fin-ska "); 
  itsIrregularHyphens.push_back("nor-ska "); 
  itsIrregularHyphens.push_back("sven-ska "); 
  itsIrregularHyphens.push_back("om-kring");
  itsIrregularHyphens.push_back("sn�-blandat");
  itsIrregularHyphens.push_back("halv-klar");
  itsIrregularHyphens.push_back("annor-st�des");
  itsIrregularHyphens.push_back("annan-stans");
  itsIrregularHyphens.push_back("regn-skur");
  itsIrregularHyphens.push_back("�sk-skur");
  itsIrregularHyphens.push_back("gan-ska");
  itsIrregularHyphens.push_back("land-skape");
  itsIrregularHyphens.push_back("meller-sta");
  itsIrregularHyphens.push_back("skogs-brand");
  itsIrregularHyphens.push_back("v�der-utsikt");
  itsIrregularHyphens.push_back("v�der-prognos");
  itsIrregularHyphens.push_back("kust-omr�de");
  itsIrregularHyphens.push_back("havs-omr�de");
  itsIrregularHyphens.push_back("land-omr�de");
  itsIrregularHyphens.push_back("regn-omr�de");
  itsIrregularHyphens.push_back("b�rds-omr�de");
  itsIrregularHyphens.push_back("falls-omr�de");
  itsIrregularHyphens.push_back("k�ld-grad");
  itsIrregularHyphens.push_back("v�rme-grad");
  itsIrregularHyphens.push_back("�st-gr�ns");
  itsIrregularHyphens.push_back("v�st-gr�ns");
  itsIrregularHyphens.push_back("sol-sken");
  //latvia; some cases from the paper received from Latvia
  itsIrregularHyphens.push_back("ru-dzi");
  itsIrregularHyphens.push_back("kau-dze");
  itsIrregularHyphens.push_back("rk-st");
  itsIrregularHyphens.push_back("nk-st");
  itsIrregularHyphens.push_back("n-gr");
  itsIrregularHyphens.push_back("k-st");
  itsIrregularHyphens.push_back("pa-dsmit");
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

//NFmiString NFmiHyphenationString::CreateIrregularHyphens(const char * theHyphenationMark)
void NFmiHyphenationString::CreateIrregularHyphens(const char * theHyphenationMark)
{
  if(!fIrregularHyphensInited)
		InitIrregularHyphens();

  std::string stdString(*this); 
  unsigned long posChar = 0;
  unsigned long posReturn = 0;
  unsigned long posLast = 0;
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
			//posCharNot = word.find("#"); //need this option??
			//if(posCharNot!= string::npos)
			//	word.erase(posCharNot, 1);

			hyphWord.replace(posChar+posDiff, 1, theHyphenationMark); 
			posChar = word.find("-");
			posDiff++;
		}
  		posChar = stdString.find(word);
 		while(posChar!= string::npos)
		{
			posLast = posChar;
	//       avoid very short orphane text fragments  
			posReturn = stdString.find('\r', posChar);
			int len = stdString.length();
			int len2 = word.length();
			if (!(len-posChar-len2 < 6 || (posReturn != string::npos && posReturn-posChar-len2 < 6)))
	//		if(posReturn != string::npos && posReturn-posChar > 11 
	//			          && stdString.length()-posChar > 11)
			{
				stdString.replace(posChar,word.size(),hyphWord);
			}
			posChar = stdString.find(word, posLast+1);
		}
		//same for starting upper case
		//toupper do not function with skandinavian chars
		word[0] = toupper(word[0]);
 		hyphWord[0] = toupper(hyphWord[0]);
		if(word[0] == '�')
		{
			word[0] = '�';
			hyphWord[0] = '�';
		}
		if(word[0] == '�')
		{
			word[0] = '�';
			hyphWord[0] = '�';
		}
 		if(word[0] == '�')
		{
			word[0] = '�';
			hyphWord[0] = '�';
		}
 		posChar = stdString.find(word);
 		while(posChar!= string::npos)
		{
			posLast = posChar;
	//       avoid very short orphane text fragments  
 			int len = stdString.length();
			int len2 = word.length();
			if (!(len-posChar-len2 < 6 || (posReturn != string::npos && posReturn-posChar-len2 < 6)))
			{
				stdString.replace(posChar,word.size(),hyphWord);
				word[0] = toupper(word[0]);
 				hyphWord[0] = toupper(hyphWord[0]);
			}
			posChar = stdString.find(word, posLast+1);
		}
	}
    NFmiString fmiString(stdString);
    Set(fmiString, fmiString.GetLen());
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theHyphenationMark Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiHyphenationString::DeleteShortSyllables(const char * theHyphenationMark)
{
  NFmiString newString;
  ResetPosition();
  while(NextChar(theHyphenationMark))
	{
	  newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);
      if(itsCurrentCharPos <= 4  ||
		 IsPunctuation(itsCurrentCharPos-3) || IsPunctuation(itsCurrentCharPos-4) ||
		 IsPunctuation(itsCurrentCharPos+3) || IsPunctuation(itsCurrentCharPos+4) ||
		 GetLen()-itsCurrentCharPos <= 4)
	  {
         itsLastCharPosition = itsCurrentCharPos+1;	 	  
	  }
	  else
	  {
         itsLastCharPosition = itsCurrentCharPos;	 	  
	  }
  }
  if (GetLen() - itsLastCharPosition > 0)
	newString += GetChars(itsLastCharPosition, GetLen() - itsLastCharPosition+1);

  return newString; 
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theHyphenationMark Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiHyphenationString::IsPunctuation(unsigned short theChar)
{
	return GetChars(theChar, 1) == NFmiString(" ") ||
           GetChars(theChar, 1) == NFmiString(",") ||
		   GetChars(theChar, 1) == NFmiString(".") ||
		   GetChars(theChar, 1) == NFmiString("-") ||
		   GetChars(theChar, 1) == NFmiString(":");
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
  CreateIrregularHyphens(theHyphenationMark);
  ResetPosition();
  while(NextConsonant())
	{
	  newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);
	  itsCurrentCharPos++;
  
	  if(GetChars(itsCurrentCharPos-2, 1) != NFmiString(" ") &&  //ei sanan alkuun
		 GetChars(itsCurrentCharPos-3, 1) != NFmiString(" ") &&  // eik� j�tet� yksi kirjain alkuun
		                                    // annetun ep�s��nn�llisen j�lkeen v�h. 2 merkki�
		 GetChars(itsCurrentCharPos-2, 1) != NFmiString(theHyphenationMark) && 
		 GetChars(itsCurrentCharPos-3, 1) != NFmiString(theHyphenationMark) &&
		
		   !(GetChars(itsCurrentCharPos-4, 1) == NFmiString(" ") && // kolme kons. sanan alussa  
			IsConsonant(GetChars(itsCurrentCharPos-3, 1)) &&        // esim ei "p� dagen sp-ricker"
			IsConsonant(GetChars(itsCurrentCharPos-2, 1)))
		 &&		 
		 GetChars(itsCurrentCharPos-2, 1) != NFmiString("-") && //muuten tuplana jaettaessa
		 EnoughOnRow() &&
		/* GetChars(itsCurrentCharPos+3, 1) != NFmiString('\r') && //omalle riville v�h. 8 merkki
		 GetChars(itsCurrentCharPos+4, 1) != NFmiString('\r') && 
		 GetChars(itsCurrentCharPos+5, 1) != NFmiString('\r') && 
		 GetChars(itsCurrentCharPos+6, 1) != NFmiString('\r') && 
		 GetChars(itsCurrentCharPos+7, 1) != NFmiString('\r') &&
		 */
		 //itsCurrentCharPos < GetLen()-6   &&

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

bool NFmiHyphenationString::EnoughOnRow(void)const
{
   if(fNarrowColumn)
	   return true;
   else
	   return
		 GetChars(itsCurrentCharPos+3, 1) != NFmiString('\r') && //omalle riville v�h. 8 merkki
		 GetChars(itsCurrentCharPos+4, 1) != NFmiString('\r') && 
		 GetChars(itsCurrentCharPos+5, 1) != NFmiString('\r') && 
		 GetChars(itsCurrentCharPos+6, 1) != NFmiString('\r') && 
		 GetChars(itsCurrentCharPos+7, 1) != NFmiString('\r');		 
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
  //ResetPosition();
  SuperResetPosition();
  while(NextChar(theChar))
	{
	  newString += GetChars(itsLastCharPosition, itsCurrentCharPos - itsLastCharPosition);

	  // ei jaeta esim -5 mutta kyll� it�-uusimaa	  
	  if(NextIsNumeric())
		newString += NFmiString("\\136");
	  
	  newString += NFmiString(withString);

	  // eik� pit�isi ynn�t� enemm�n jos += NFmiString("\\136")??
	  //itsCurrentCharPos++;
	  itsLastCharPosition = itsCurrentCharPos+1;

	  //itsLastCharPosition = itsCurrentCharPos-1;

	}
  if (GetLen() >= itsLastCharPosition )
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
 * \param theChar Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------
/*
bool NFmiHyphenationString::PreviousChar(const NFmiString & theChar)
{
  while(BackPosition())
	{
	  if(GetChars(itsCurrentCharPos, 1) == theChar)
		return true;
	}
  return false;
}
*/
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
  //return itsCurrentCharPos++ < fLength;
  return ++itsCurrentCharPos <= fLength;
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
  NFmiString consonants("bcdfghjklmnpqrstvwxzBCDFGHJKLMNPQRSTVWXZ"); 
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
  NFmiString vowels("aeiouy���AEIOUY���");  
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





