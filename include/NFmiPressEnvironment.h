// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressEnvironment
 */
// ======================================================================
/*!
 * \class NFmiPressEnvironment
 *
 * T‰h‰n voidaan ker‰t‰ lehtis‰‰ohjelman sellaisia piirteit‰, joita
 * voi antaa hierarkian eri tasoilla. Esim ylimm‰ll‰ tuotetasolla ja
 * alemmilla ylikirjoittaa. Ekaksi t‰h‰n otetaan uusi pitk‰/lyhyt
 * numeromiinus.
 *
 */
// ======================================================================

#ifndef NFMIPRESSENVIRONMENT_H
#define NFMIPRESSENVIRONMENT_H

#include <utility>

#include "NFmiGlobals.h"
#include "NFmiPoint.h"
#include "NFmiPressTypes.h"
#include "NFmiString.h"

class NFmiPressArea;

struct FmiPressTextAttributes
{
  NFmiString font;
  double size = 0.0;
  FmiDirection alignment;
  FmiGenericColor color{};  // k‰yttˆˆn myˆhemmin
  bool fLongNumberMinus = false;  // k‰yttˆˆn myˆhemmin
};

//! Undocumented
class NFmiPressEnvironment
{
 public:
  ~NFmiPressEnvironment();
  NFmiPressEnvironment();
  NFmiPressEnvironment(const NFmiPressEnvironment& aTime);
  NFmiPressEnvironment& operator=(const NFmiPressEnvironment&) = default;

  void SetMaskNumber(unsigned long theMaskNumber);
  void SetEnumSpace(FmiEnumSpace theEnumSpace);
  void SetMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace);
  //! use change if to be performed only when already set, otherwise Set
  bool ChangeMaskNumber(unsigned long theMaskNumber);
  bool ChangeMask(unsigned long theMaskNumber, FmiEnumSpace theEnumSpace);

  unsigned long GetMaskNumber() const;
  FmiEnumSpace GetEnumSpace() const;
  bool IsRGB() const;
  bool IsCMYK() const;
  FmiGenericColor GetColor() const;
  bool GetLongNumberMinus() const;
  void SetDayAdvance(long theDayAdvance);
  long GetDayAdvance() const;
  int GetVersion() const;
  NFmiString GetSymbolSet() const;
  void SetSymbolSet(const NFmiString& symbolSet);
  float GetSymbolSizeFactor() const;
  void SetSymbolSizeFactor(float symbolSet);

  NFmiString GetFont() const;
  void SetFont(const NFmiString& font);
  double GetTextSize() const;
  void SetTextSize(double size);
  FmiDirection GetTextAlignment() const;
  void SetTextAlignment(FmiDirection alignment);

  void SetLongNumberMinus();
  void SetShortNumberMinus();
  void SetRGB(FmiRGBColor theColor);
  void SetCMYK(FmiCMYK theColor);
  void SetColor(const FmiGenericColor& theColor);
  void SetVersion(int theVersion);
  bool IsSegmentMove() const;
  void SetSegmentMove(const NFmiPoint& theMove);
  void SetCV(bool theCV);
  bool GetCV() const;
  NFmiPoint GetSegmentMove() const;
  void AvoidOrphanSyllables(bool theMode);
  bool AvoidOrphanSyllables() const;
  void UseBackupPreviousDay(bool theMode);
  bool UseBackupPreviousDay() const;
  void SetEncoding(std::string theEncoding);
  std::string GetEncoding() const;

 private:
  unsigned long itsMaskNumber = 0;
  FmiEnumSpace itsEnumSpace = kNoneEnumSpace;
  bool fLongNumberMinus = false;
  FmiGenericColor itsGenericColor{};
  long itsAdditionalDayAdvance = 0;
  FmiPressTextAttributes itsTextAttributes;
  int itsVersion;
  NFmiPoint itsSegmentMove;
  NFmiString itsSymbolSet;
  float itsSymbolSizeFactor;
  bool fCV = false;
  bool fAvoidOrphanSyllables;
  bool fUseBackupPreviousDay;
  std::string itsEncoding;
};  // class NFmiPressEnvironment

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressEnvironment::NFmiPressEnvironment() : itsVersion(1), itsEncoding("latin")
{
  // HUOM vain jompikumpi v‰rij‰rjestelm‰
  itsGenericColor.color.cmyk.cyan = 0.;
  itsGenericColor.color.cmyk.magenta = 0.;
  itsGenericColor.color.cmyk.yellow = 0.;
  itsGenericColor.color.cmyk.black = 1.;
  itsGenericColor.isRgb = false;

  itsTextAttributes.font = NFmiString("Helvetica");
  itsTextAttributes.size = 12.;
  itsTextAttributes.alignment = kCenter;
  // itsTextAttributes.color;                    //k‰yttˆˆn myˆhemmin
  itsTextAttributes.fLongNumberMinus = false;  // k‰yttˆˆn myˆhemmin

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
  itsEncoding = std::move(theEncoding);
}
// ----------------------------------------------------------------------
/*!
 * Sets the encoding of the parameter in case it is needed as text
 *
 * \param theEncoding The given encoding
 */
// ----------------------------------------------------------------------
inline std::string NFmiPressEnvironment::GetEncoding() const
{
  return itsEncoding;
}
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

inline bool NFmiPressEnvironment::UseBackupPreviousDay() const
{
  return fUseBackupPreviousDay;
}
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

inline bool NFmiPressEnvironment::AvoidOrphanSyllables() const
{
  return fAvoidOrphanSyllables;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetCV(bool theCV)
{
  fCV = theCV;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::GetCV() const
{
  return fCV;
}
// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

inline int NFmiPressEnvironment::GetVersion() const
{
  return itsVersion;
}
// ----------------------------------------------------------------------
/*!
 * versio vaikuttaa:
 * 20+:
 * mitta/sijoitsualue + kartta: koordinaatit skaalataan niill‰ ennen lat/long
 *   vanhemmissa skaalataan j‰lkeen lat/long
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetVersion(int theVersion)
{
  itsVersion = theVersion;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPressEnvironment::GetSymbolSet() const
{
  return itsSymbolSet;
}
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

inline float NFmiPressEnvironment::GetSymbolSizeFactor() const
{
  return itsSymbolSizeFactor;
}
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

inline FmiDirection NFmiPressEnvironment::GetTextAlignment() const
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

inline double NFmiPressEnvironment::GetTextSize() const
{
  return itsTextAttributes.size;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetTextSize(double size)
{
  itsTextAttributes.size = size;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetFont(const NFmiString& font)
{
  itsTextAttributes.font = font;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPressEnvironment::GetFont() const
{
  return itsTextAttributes.font;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetLongNumberMinus()
{
  fLongNumberMinus = true;
  itsTextAttributes.fLongNumberMinus = true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressEnvironment::SetShortNumberMinus()
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
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline unsigned long NFmiPressEnvironment::GetMaskNumber() const
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

inline FmiEnumSpace NFmiPressEnvironment::GetEnumSpace() const
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

inline bool NFmiPressEnvironment::IsRGB() const
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

inline bool NFmiPressEnvironment::IsCMYK() const
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

inline FmiGenericColor NFmiPressEnvironment::GetColor() const
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

inline bool NFmiPressEnvironment::GetLongNumberMinus() const
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

inline long NFmiPressEnvironment::GetDayAdvance() const
{
  return itsAdditionalDayAdvance;
}
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

inline NFmiPoint NFmiPressEnvironment::GetSegmentMove() const
{
  return itsSegmentMove;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressEnvironment::IsSegmentMove() const
{
  return itsSegmentMove.X() != 0. || itsSegmentMove.Y() != 0.;
}

#endif  // NFMIPRESSENVIRONMENT_H

// ======================================================================
