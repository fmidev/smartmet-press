// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiNumberParamRect.h
 */
// ======================================================================
/*!
 * \class NFmiNumberParamRect
 *
 * T‰m‰ luokka hallitsee dataohjattujen numeroiden tuottamisen
 * m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
 *
 */
// ======================================================================

#ifndef NFMINUMBERPARAMRECT_H
#define NFMINUMBERPARAMRECT_H

#include "NFmiTextParamRect.h"

//! Undocumented
enum NFmiNumberParamRectObjects
{
  dFormat = 27,
  dZeroMinus,
  dDetachSign,
  dDotToComma = 1027
};

//! Undocumented
class NFmiNumberParamRect : public NFmiTextParamRect
{
 public:
  virtual ~NFmiNumberParamRect(void);

  NFmiNumberParamRect(void);
  NFmiNumberParamRect(const NFmiNumberParamRect& theNumberParamRect);

  virtual bool ReadDescription(NFmiString& retString);
  virtual bool ReadRemaining(void);
  virtual NFmiParamRect* Clone(void) const;
  int ConvertDefText(NFmiString& object);

  virtual bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
                       NFmiFastQueryInfo* theQI,
                       std::ofstream& theDestinationFile,
                       FmiPressOutputMode theOutput);

 private:
  NFmiString DetachSign(const NFmiString& theString, float theValue) const;

 protected:
  FmiGenericColor MapColor(void) const;

 protected:
  NFmiString itsFormat;
  bool fZeroMinus;
  bool fDetachSign;
  bool fDotToComma;

};  // class NFmiNumberParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiNumberParamRect::NFmiNumberParamRect(void)
    : NFmiTextParamRect(),
      itsFormat(NFmiString("%1.f")),
      fZeroMinus(false),
      fDetachSign(false),
      fDotToComma(false)
{
}

#endif  // NFMINUMBERPARAMRECT_H

// ======================================================================
