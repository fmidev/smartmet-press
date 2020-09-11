// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiCmykColorBag
 */
// ======================================================================
/*!
 * \class NFmiCmykColorBag
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMICMYKCOLORBAG_H
#define NFMICMYKCOLORBAG_H

#include "NFmiSize.h"
#include "NFmiValueString.h"

//! Representation of a CMYK color
struct NFmiCmykColor
{
  double c;  //!< Cyan part of CMYK
  double m;  //!< Magenta part of CMYK
  double y;  //!< Yellow part of CMYK
  double k;  //!< K part of CMYK
};

//! Representation of a named CMYK color
struct NFmiNamedCmykColor
{
  NFmiCmykColor color;
  NFmiString name;
};

// NFmiParamMapping: organized series of mapping intervals; knows how to add
// new mapping interval; when given a value, finds the right interval and returns
// associated symbol.

//! Undocumented
class NFmiCmykColorBag : public NFmiSize
{
 public:
  virtual ~NFmiCmykColorBag(void);
  NFmiCmykColorBag(void);
  NFmiCmykColorBag(const NFmiCmykColorBag& theCmykColorBag);

  void AddColor(const NFmiNamedCmykColor& theColor);
  NFmiCmykColor* GetColor(const NFmiString& theName) const;
  NFmiString GetColorString(const NFmiString& theName) const;

 private:
  NFmiNamedCmykColor* itsColors;

};  // class NFmiCmykColorBag

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiCmykColorBag::NFmiCmykColorBag(void) : NFmiSize(0), itsColors(0) {}
#endif  // NFMICMYKCOLORBAG_H

// ======================================================================
