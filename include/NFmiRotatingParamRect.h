// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiRotatingParamRect
 */
// ======================================================================
/*!
 * \class NFmiRotatingParamRect
 *
 * T�m� luokka hallitsee dataohjattujen k��ntyvien symbolien tuottamisen
 * m��rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
 * Symboli k��nnet��n data-arvon mukaan, jonka oletetaan olevan tuulen-
 * suunta (0-360). Symbolin oletetaan lis�ksi olevan kuvatiedostossa
 * k��nnettyn� arvoon 270 (l�nsi). Voidaan parametrisoida my�hemmin
 * jos tarvitaan.
 */
// ======================================================================

#ifndef NFMIROTATINGPARAMRECT_H
#define NFMIROTATINGPARAMRECT_H

#include "NFmi2SymbolParamRect.h"

//! Undocumented
enum NFmiRotatingParamRectObjects
{
  dNotRotInterval = 109,
  dVerticalLong
};

//! Undocumented
class NFmiRotatingParamRect : public NFmi2SymbolParamRect
{
 public:
  virtual ~NFmiRotatingParamRect(void);
  NFmiRotatingParamRect(void);
  NFmiRotatingParamRect(const NFmiRotatingParamRect& theSymbolRect);

  virtual bool ReadRemaining(void);
  virtual int ConvertDefText(NFmiString& object);
  virtual NFmiParamRect* Clone(void) const;
  void SetNotRotatingMin(double notRotatingMin);
  void SetNotRotatingMax(double notRotatingMin);

 protected:
  bool Rotate(void) const;
  virtual void DoPostReading(void);

 private:
  bool CopyShortSymbol2Dest(NFmiString* symbolFile,
                            std::ofstream& theDestinationFile,
                            float theRotating);

  float AdjustToMap(float theDirection) const;

  virtual void WriteMetaCode(NFmiString* symbolFile, NFmiPoint realPos, std::ofstream& os);

 private:
  double itsNotRotatingMinValue;
  double itsNotRotatingMaxValue;
  double itsVerticalLong;

};  // class NFmiRotatingParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiRotatingParamRect::NFmiRotatingParamRect(void) : NFmi2SymbolParamRect()
{
  itsNotRotatingMinValue = 10.;
  itsNotRotatingMaxValue = -10.;
  itsVerticalLong = kFloatMissing;
}
// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

inline void NFmiRotatingParamRect::SetNotRotatingMin(double notRotatingMin)
{
  itsNotRotatingMinValue = notRotatingMin;
}
// ----------------------------------------------------------------------
/*!
 *
 */
// ----------------------------------------------------------------------

inline void NFmiRotatingParamRect::SetNotRotatingMax(double notRotatingMax)
{
  itsNotRotatingMaxValue = notRotatingMax;
}

#endif  // NFMIROTATINGPARAMRECT_H

// ======================================================================
