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
class _FMI_DLL NFmi2SymbolParamRect : public NFmiSymbolParamRect
{
 public:
  virtual ~NFmi2SymbolParamRect();
  NFmi2SymbolParamRect(void);
  NFmi2SymbolParamRect(const NFmi2SymbolParamRect& theSymbolRect);

  virtual bool ReadRemaining(void);

  virtual int ConvertDefText(NFmiString& object);
  virtual NFmiParamRect* Clone(void) const;
  void SetSecondDataIdent(unsigned long theSecondPar);

 protected:
  bool ReadValues(NFmiFastQueryInfo* theQI, bool SetRelHour = true);
  NFmiDataIdent GetSecondDataIdent(void) const;

  float itsSecondParamValue;
  NFmiDataIdent itsSecondDataIdent;

 private:
};  // class NFmi2SymbolParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmi2SymbolParamRect::NFmi2SymbolParamRect(void) : NFmiSymbolParamRect() {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiDataIdent NFmi2SymbolParamRect::GetSecondDataIdent(void) const
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
