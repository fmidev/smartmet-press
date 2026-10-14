// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPsSymbol
 */
// ======================================================================
/*!
 * \class NFmiPsSymbol
 *
 * "Lyhytymbolien" tuottaminen suoraan ps-koodiin ilman dataohjausta.
 * Tarkoittaa Symbolit-hakemiston kuvia, joista tehd‰‰n tiiviit esitykset
 * LyhytSymbolit-hakemistoon.  Datariippuvat symbolit ovat toisessa
 * luokkahierarkiassa.
 *
 */
// ======================================================================

#ifndef NFMIPSSYMBOL_H
#define NFMIPSSYMBOL_H

#include "NFmiPressScaling.h"
#include <fstream>

//! Undocumented
enum NFmiPsSymbolObjects
{
  dSymbolDir = 150,
  dConstantSymbol,
  dPsRelSize
};

//! Undocumented
class NFmiPsSymbol : public NFmiPressScaling
{
 public:
  ~NFmiPsSymbol() override;
  NFmiPsSymbol();
  NFmiPsSymbol(const NFmiPsSymbol& thePsSymbol);

  int ConvertDefText(NFmiString& object) override;
  bool ReadDescription(NFmiString& retString) override;
  virtual NFmiPsSymbol* Clone() const;
  virtual bool MakeAndWritePS(NFmiPoint place);  // t‰n voi sitten joskus hyl‰t‰
  using NFmiPressScaling::WritePS;
  bool WritePS(FmiPressOutputMode theOutput) override;
  NFmiString GetSymbol() const;
  bool WritePSUpdatingSubText(FmiPressOutputMode theOutput) override;

 protected:
  virtual bool CopyShortSymbol2Dest();
  virtual bool ConvertOrig2Short();

  NFmiString itsShortDir;
  NFmiString itsOrigDir;
  NFmiString itsSymbol;

};  // class NFmiPsSymbol

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPsSymbol::NFmiPsSymbol() {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPsSymbol::GetSymbol() const
{
  return itsSymbol;
}
#endif  // NFMIPSSYMBOL_H

// ======================================================================
