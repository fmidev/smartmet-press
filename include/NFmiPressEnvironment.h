// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressEnvironment
 */
// ======================================================================
/*!
 * \class NFmiPressEnvironment
 * 
 * Tähän voidaan kerätä lehtisääohjelman sellaisia piirteitä, joita
 * voi antaa hierarkian eri tasoilla. Esim ylimmällä tuotetasolla ja
 * alemmilla ylikirjoittaa. Ekaksi tähän otetaan uusi pitkä/lyhyt 
 * numeromiinus.
 *
 */
// ======================================================================
 
#ifndef NFMIPRESSENVIRONMENT_H
#define NFMIPRESSENVIRONMENT_H
 
#include  "NFmiGlobals.h"
#include "NFmiPressTypes.h"

class NFmiPressArea;

/*
struct FmiPressTextAttributes
{
	long value;
	NFmiPoint point;
};
*/
//! Undocumented 
class _FMI_DLL NFmiPressEnvironment  
{ 

public: 

  ~NFmiPressEnvironment(void); 
  NFmiPressEnvironment(void);
  NFmiPressEnvironment(const NFmiPressEnvironment & aTime);

  void SetLongNumberMinus(void);
  void SetShortNumberMinus(void);
  void SetRGB(FmiRGBColor theColor);
  void SetCMYK(FmiCMYK theColor);
  void SetColor(const FmiGenericColor & theColor);
  void SetMaskNumber(unsigned long theMaskNumber);
  void SetEnumSpace(FmiEnumSpace theEnumSpace);
  void SetMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace);
  //! use change if to be performed only when already set, otherwise Set
  bool ChangeMaskNumber(unsigned long theMaskNumber);
  bool ChangeMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace);

  unsigned long GetMaskNumber(void)const;
  FmiEnumSpace GetEnumSpace(void)const;
  bool IsRGB(void) const;
  bool IsCMYK(void) const;
  FmiGenericColor GetColor(void) const;
  bool GetLongNumberMinus(void) const;
  void SetDayAdvance(long theDayAdvance);
  long GetDayAdvance(void);

private:

  unsigned long itsMaskNumber;
  FmiEnumSpace  itsEnumSpace;
  bool fLongNumberMinus;
  FmiGenericColor itsGenericColor; 
  long itsAdditionalDayAdvance;

}; // class NFmiPressEnvironment

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressEnvironment::NFmiPressEnvironment(void)
  : itsMaskNumber(0)
  , itsEnumSpace(kNoneEnumSpace)
  , fLongNumberMinus(false)
  , itsAdditionalDayAdvance(0)
{
  //HUOM vain jompikumpi värijärjestelmä
  itsGenericColor.color.cmyk.cyan = 0.;
  itsGenericColor.color.cmyk.magenta = 0.;
  itsGenericColor.color.cmyk.yellow = 0.;
  itsGenericColor.color.cmyk.black = 1.;
  itsGenericColor.isRgb = false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetLongNumberMinus(void)
{
  fLongNumberMinus=true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetShortNumberMinus(void)
{
  fLongNumberMinus=false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetRGB(FmiRGBColor theColor)
{
  itsGenericColor.color.rgb = theColor;
  itsGenericColor.isRgb = true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetCMYK(FmiCMYK theColor)
{
  itsGenericColor.color.cmyk = theColor;
  itsGenericColor.isRgb = false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetColor(const FmiGenericColor & theColor)
{
  itsGenericColor = theColor;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMaskNumber Undocumented
 * \param theEnumSpace Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace)
{
  itsMaskNumber = theMaskNumber;
  itsEnumSpace = theEnumSpace;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMaskNumber Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetMaskNumber(unsigned long theMaskNumber)
{
  itsMaskNumber = theMaskNumber;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theEnumSpace the new EnumSpace
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetEnumSpace(FmiEnumSpace theEnumSpace)
{
  itsEnumSpace = theEnumSpace;
}

// ----------------------------------------------------------------------
/*!
 * Only performed if already set (as opposite to SetMask)
 *
 * \param theMaskNumber Undocumented
 * \param theEnumSpace Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressEnvironment::ChangeMask(unsigned long theMaskNumber,
									  FmiEnumSpace theEnumSpace)
{
  if(itsMaskNumber > 0)
	{
	  itsMaskNumber = theMaskNumber;
	  itsEnumSpace = theEnumSpace;
	  return true;
	}
  else
	return false;
}
// ----------------------------------------------------------------------
/*!
 * Only performed if already set (as opposite to SetMaskNumber)
 *
 * \param theMaskNumber Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressEnvironment::ChangeMaskNumber(unsigned long theMaskNumber)
{
  if(itsMaskNumber > 0)
	{
	  itsMaskNumber = theMaskNumber;
	  return true;
	}
  else
	return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
unsigned long NFmiPressEnvironment::GetMaskNumber(void) const
{
  return itsMaskNumber;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
FmiEnumSpace NFmiPressEnvironment::GetEnumSpace(void) const
{
  return itsEnumSpace;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressEnvironment::IsRGB(void) const
{
  return itsGenericColor.isRgb;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressEnvironment::IsCMYK(void) const
{
  return !itsGenericColor.isRgb;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
FmiGenericColor NFmiPressEnvironment::GetColor(void) const
{
  return itsGenericColor;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressEnvironment::GetLongNumberMinus(void) const
{
  return fLongNumberMinus;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDayAdvance Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressEnvironment::SetDayAdvance(long theDayAdvance)
{
  itsAdditionalDayAdvance=theDayAdvance;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
long NFmiPressEnvironment::GetDayAdvance(void)
{
  return itsAdditionalDayAdvance;
}

#endif // NFMIPRESSENVIRONMENT_H

// ======================================================================



