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
  NFmiCmykColor color{};
  NFmiString name;
};

// NFmiParamMapping: organized series of mapping intervals; knows how to add
// new mapping interval; when given a value, finds the right interval and returns
// associated symbol.

//! Undocumented
class NFmiCmykColorBag : public NFmiSize
{
 public:
  ~NFmiCmykColorBag() override;
  NFmiCmykColorBag();
  NFmiCmykColorBag(const NFmiCmykColorBag& theCmykColorBag);
  NFmiCmykColorBag& operator=(const NFmiCmykColorBag&) = default;

  void AddColor(const NFmiNamedCmykColor& theColor);
  NFmiCmykColor* GetColor(const NFmiString& theName) const;
  NFmiString GetColorString(const NFmiString& theName) const;

 private:
  NFmiNamedCmykColor* itsColors = nullptr;

};  // class NFmiCmykColorBag

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiCmykColorBag::NFmiCmykColorBag() : NFmiSize(0) {}
#endif  // NFMICMYKCOLORBAG_H

// ======================================================================
