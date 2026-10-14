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
  ~NFmiNumberParamRect() override;

  NFmiNumberParamRect();
  NFmiNumberParamRect(const NFmiNumberParamRect& theNumberParamRect);

  bool ReadDescription(NFmiString& retString) override;
  bool ReadRemaining() override;
  NFmiParamRect* Clone() const override;
  int ConvertDefText(NFmiString& object) override;

  bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
               NFmiFastQueryInfo* theQI,
               std::ofstream& theDestinationFile,
               FmiPressOutputMode theOutput) override;

 private:
  NFmiString DetachSign(const NFmiString& theString, float theValue) const;

 protected:
  FmiGenericColor MapColor() const override;

  NFmiString itsFormat;
  bool fZeroMinus = false;
  bool fDetachSign = false;
  bool fDotToComma = false;

};  // class NFmiNumberParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiNumberParamRect::NFmiNumberParamRect() : itsFormat(NFmiString("%1.f")) {}

#endif  // NFMINUMBERPARAMRECT_H

// ======================================================================
