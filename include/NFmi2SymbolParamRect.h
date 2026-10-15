// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmi2SymbolParamRect
 */
// ======================================================================
/*!
 * \class NFmi2SymbolParamRect
 *
 * T‰m‰ luokka tuo toisen parametrin Symboliparametreihi, jota
 * lapsissa k‰ytet‰‰n symbolin k‰‰nt‰miseen(NFmiRotatingParamRect) tai
 * skaalaamiseen(NFmiScalingParamRect)
 */
// ======================================================================

#ifndef NFMI2SYMBOLPARAMRECT_H
#define NFMI2SYMBOLPARAMRECT_H

#include "NFmiSymbolParamRect.h"

//! Undocumented

enum NFmi2SymbolParamRectObjects
{
  dSecondPar = 107
};

//! Undocumented
class NFmi2SymbolParamRect : public NFmiSymbolParamRect
{
 public:
  ~NFmi2SymbolParamRect() override;
  NFmi2SymbolParamRect();
  NFmi2SymbolParamRect(const NFmi2SymbolParamRect& theSymbolRect);
  NFmi2SymbolParamRect& operator=(const NFmi2SymbolParamRect&) = default;

  bool ReadRemaining() override;

  int ConvertDefText(NFmiString& object) override;
  NFmiParamRect* Clone() const override;
  void SetSecondDataIdent(unsigned long theSecondPar);

 protected:
  bool ReadValues(NFmiFastQueryInfo* theQI, bool SetRelHour = true) override;
  NFmiDataIdent GetSecondDataIdent() const;

  float itsSecondParamValue = 0.0F;
  NFmiDataIdent itsSecondDataIdent;

 private:
};  // class NFmi2SymbolParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmi2SymbolParamRect::NFmi2SymbolParamRect() {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiDataIdent NFmi2SymbolParamRect::GetSecondDataIdent() const
{
  return itsSecondDataIdent;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmi2SymbolParamRect::SetSecondDataIdent(unsigned long theSecondPar)
{
  itsSecondDataIdent.SetParam(NFmiParam(theSecondPar));
}
#endif  // NFMI2SYMBOLPARAMRECT_H

// ======================================================================
