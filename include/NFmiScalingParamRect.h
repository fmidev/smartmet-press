// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiScalingParamRect
 */
// ======================================================================
/*!
 * \class NFmiScalingParamRect
 *
 * Tämä luokka hallitsee dataohjattujen skaalattavien symbolien
 * tuottamisen.
 *
 */
// ======================================================================

#ifndef NFMISCALINGPARAMRECT_H
#define NFMISCALINGPARAMRECT_H

#include "NFmi2SymbolParamRect.h"

//! Undocumented
struct FmiValueScaling
{
  double noneValue;    //!< tällä jää pois, normaalisti nolla
  double symbolValue;  //!< talletettua symbolia vastaava arvo
  double maxValue;     //!< max-arvo, jonka mukaan skaalataan
};

//! Undocumented
enum NFmiScalingParamRectObjects
{
  dHeightScale,
  dWidthScale
};

//! Undocumented
class NFmiScalingParamRect : public NFmi2SymbolParamRect
{
 public:
  ~NFmiScalingParamRect(void) override;
  NFmiScalingParamRect(void);
  NFmiScalingParamRect(const NFmiScalingParamRect& theSymbolRect);

  bool ReadRemaining(void) override;

  int ConvertDefText(NFmiString& object) override;
  NFmiParamRect* Clone(void) const override;

 protected:
  void DoPostReading(void) override;
  bool ReadSymbolScale(FmiValueScaling& theScale);
  void ScaleByValue() override;

  FmiValueScaling itsXValueScaling;
  FmiValueScaling itsYValueScaling;

 private:
};  // class NFmiScalingParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiScalingParamRect::NFmiScalingParamRect(void) : NFmi2SymbolParamRect()
{
  itsXValueScaling.symbolValue = kFloatMissing;
  itsYValueScaling.symbolValue = kFloatMissing;
}

#endif  // NFMISCALINGPARAMRECT_H

// ======================================================================
