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

#include "NFmiGlobals.h"
#include "NFmiPoint.h"
#include "NFmiPressTypes.h"
#include "NFmiString.h"

class NFmiPressArea;

struct FmiPressTextAttributes
{
  NFmiString font;
  double size;
  FmiDirection alignment;
  FmiGenericColor color;  // käyttöön myöhemmin
  bool fLongNumberMinus;  // käyttöön myöhemmin
};

//! Undocumented
class _FMI_DLL NFmiPressEnvironment
{
 public:
  ~NFmiPressEnvironment(void);
  NFmiPressEnvironment(void);
  NFmiPressEnvironment(const NFmiPressEnvironment& aTime);

  void SetMaskNumber(unsigned long theMaskNumber);
  void SetEnumSpace(FmiEnumSpace theEnumSpace);
  void SetMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace);
  //! use change if to be performed only when already set, otherwise Set
  bool ChangeMaskNumber(unsigned long theMaskNumber);
  bool ChangeMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace);

  unsigned long GetMaskNumber(void) const;
  FmiEnumSpace GetEnumSpace(void) const;
  bool IsRGB(void) const;
  bool IsCMYK(void) const;
  FmiGenericColor GetColor(void) const;
  bool GetLongNumberMinus(void) const;
  void SetDayAdvance(long theDayAdvance);
  long GetDayAdvance(void);
  int GetVersion(void) const;
  NFmiString GetSymbolSet(void) const;
  void SetSymbolSet(const NFmiString& symbolSet);
  float GetSymbolSizeFactor(void) const;
  void SetSymbolSizeFactor(float symbolSet);

  NFmiString GetFont(void) const;
  void SetFont(const NFmiString& font);
  double GetTextSize(void) const;
  void SetTextSize(double size);
  FmiDirection GetTextAlignment(void) const;
  void SetTextAlignment(FmiDirection alignment);

  void SetLongNumberMinus(void);
  void SetShortNumberMinus(void);
  void SetRGB(FmiRGBColor theColor);
  void SetCMYK(FmiCMYK theColor);
  void SetColor(const FmiGenericColor& theColor);
  void SetVersion(int theVersion);
  bool IsSegmentMove(void) const;
  void SetSegmentMove(const NFmiPoint& theMove);
  void SetCV(bool theCV);
  bool GetCV(void) const;
  NFmiPoint GetSegmentMove(void) const;
  void AvoidOrphanSyllables(bool theMode);
  bool AvoidOrphanSyllables(void) const;
  void UseBackupPreviousDay(bool theMode);
  bool UseBackupPreviousDay(void) const;
  void SetEncoding(std::string theEncoding);
  std::string GetEncoding(void) const;

 private:
  unsigned long itsMaskNumber;
  FmiEnumSpace itsEnumSpace;
  bool fLongNumberMinus;
  FmiGenericColor itsGenericColor;
  long itsAdditionalDayAdvance;
  FmiPressTextAttributes itsTextAttributes;
  int itsVersion;
  NFmiPoint itsSegmentMove;
  NFmiString itsSymbolSet;
  float itsSymbolSizeFactor;
  bool fCV;
  bool fAvoidOrphanSyllables;
  bool fUseBackupPreviousDay;
  std::string itsEncoding;
};  // class NFmiPressEnvironment

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressEnvironment::NFmiPressEnvironment(void)
    : itsMaskNumber(0),
      itsEnumSpace(kNoneEnumSpace),
      fLongNumberMinus(false),
      itsAdditionalDayAdvance(0),
      fCV(false),
      itsEncoding("latin")
{
  // HUOM vain jompikumpi värijärjestelmä
  itsGenericColor.color.cmyk.cyan = 0.;
  itsGenericColor.color.cmyk.magenta = 0.;
  itsGenericColor.color.cmyk.yellow = 0.;
  itsGenericColor.color.cmyk.black = 1.;
  itsGenericColor.isRgb = false;

  itsTextAttributes.font = NFmiString("Helvetica");
  itsTextAttributes.size = 12.;
  itsTextAttributes.alignment = kCenter;
  // itsTextAttributes.color;                    //käyttöön myöhemmin
  itsTextAttributes.fLongNumberMinus = false;  // käyttöön myöhemmin
  itsVersion = 1;
  itsSegmentMove = NFmiPoint(0., 0.);
  itsSymbolSet = NFmiString("Kymi");
  itsSymbolSizeFactor = 1.;
  fAvoidOrphanSyllables = false;
  fUseBackupPreviousDay = false;
}
// ----------------------------------------------------------------------
/*!
 * Sets the encoding of the parameter in case it is needed as text
 *
 * \param theEncoding The given encoding
 */
// ----------------------------------------------------------------------
inline void NFmiPressEnvironment::SetEncoding(std::string theEncoding)
{
  itsEncoding = theEncoding;
}
// ----------------------------------------------------------------------
/*!
 * Sets the encoding of the parameter in case it is needed as text
 *
 * \param theEncoding The given encoding
 */
// ----------------------------------------------------------------------
inline std::string NFmiPressEnvironment::GetEncoding(void) const { return itsEncoding; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressEnvironment::UseBackupPreviousDay(bool theMode)
{
  fUseBackupPreviousDay = theMode;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::UseBackupPreviousDay(void) const { return fUseBackupPreviousDay; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressEnvironment::AvoidOrphanSyllables(bool theMode)
{
  fAvoidOrphanSyllables = theMode;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::AvoidOrphanSyllables(void) const { return fAvoidOrphanSyllables; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetCV(bool theCV) { fCV = theCV; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::GetCV(void) const { return fCV; }
// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

inline int NFmiPressEnvironment::GetVersion(void) const { return itsVersion; }
// ----------------------------------------------------------------------
/*!
 * versio vaikuttaa:
 * 20+:
 * mitta/sijoitsualue + kartta: koordinaatit skaalataan niillä ennen lat/long
 *   vanhemmissa skaalataan jälkeen lat/long
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetVersion(int theVersion) { itsVersion = theVersion; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPressEnvironment::GetSymbolSet(void) const { return itsSymbolSet; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetSymbolSet(const NFmiString& theSymbolSet)
{
  itsSymbolSet = theSymbolSet;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline float NFmiPressEnvironment::GetSymbolSizeFactor(void) const { return itsSymbolSizeFactor; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetSymbolSizeFactor(float theSizeFactor)
{
  itsSymbolSizeFactor = theSizeFactor;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline FmiDirection NFmiPressEnvironment::GetTextAlignment(void) const
{
  return itsTextAttributes.alignment;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetTextAlignment(FmiDirection alignment)
{
  itsTextAttributes.alignment = alignment;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiPressEnvironment::GetTextSize(void) const { return itsTextAttributes.size; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetTextSize(double size) { itsTextAttributes.size = size; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetFont(const NFmiString& font) { itsTextAttributes.font = font; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPressEnvironment::GetFont(void) const { return itsTextAttributes.font; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetLongNumberMinus(void)
{
  fLongNumberMinus = true;
  itsTextAttributes.fLongNumberMinus = true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetShortNumberMinus(void)
{
  fLongNumberMinus = false;
  itsTextAttributes.fLongNumberMinus = false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetRGB(FmiRGBColor theColor)
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

inline void NFmiPressEnvironment::SetCMYK(FmiCMYK theColor)
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

inline void NFmiPressEnvironment::SetColor(const FmiGenericColor& theColor)
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

inline void NFmiPressEnvironment::SetMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace)
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

inline void NFmiPressEnvironment::SetMaskNumber(unsigned long theMaskNumber)
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

inline void NFmiPressEnvironment::SetEnumSpace(FmiEnumSpace theEnumSpace)
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

inline bool NFmiPressEnvironment::ChangeMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace)
{
  if (itsMaskNumber > 0)
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

inline bool NFmiPressEnvironment::ChangeMaskNumber(unsigned long theMaskNumber)
{
  if (itsMaskNumber > 0)
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

inline unsigned long NFmiPressEnvironment::GetMaskNumber(void) const { return itsMaskNumber; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiEnumSpace NFmiPressEnvironment::GetEnumSpace(void) const { return itsEnumSpace; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::IsRGB(void) const { return itsGenericColor.isRgb; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::IsCMYK(void) const { return !itsGenericColor.isRgb; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiGenericColor NFmiPressEnvironment::GetColor(void) const { return itsGenericColor; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::GetLongNumberMinus(void) const { return fLongNumberMinus; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDayAdvance Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetDayAdvance(long theDayAdvance)
{
  itsAdditionalDayAdvance = theDayAdvance;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline long NFmiPressEnvironment::GetDayAdvance(void) { return itsAdditionalDayAdvance; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetSegmentMove(const NFmiPoint& theMove)
{
  itsSegmentMove = theMove;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPoint NFmiPressEnvironment::GetSegmentMove(void) const { return itsSegmentMove; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::IsSegmentMove(void) const
{
  return itsSegmentMove.X() != 0. || itsSegmentMove.Y() != 0.;
}

#endif  // NFMIPRESSENVIRONMENT_H

// ======================================================================
