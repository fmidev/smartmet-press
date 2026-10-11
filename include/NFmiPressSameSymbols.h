// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressSameSymbols
 */
// ======================================================================
/*!
 * \class NFmiPressSameSymbols
 *
 * Luokalla on tarkoitus tuottaa samaa vakio-"lyhytsymbolia"
 * useampiin paikkoihin ilman datariippuvuutta. Paikat
 * talletetaan voidPtrListaan.
 * Toimivuutta viimeisessä versiossa ei ole testattu.
 * Käyttötarve aika pientä, joten kannattaako viedä käyttöoppaaseen?
 *
 */
// ======================================================================

#ifndef NFMIPRESSSAMESYMBOLS_H
#define NFMIPRESSSAMESYMBOLS_H

// press
#include "NFmiPressProduct.h"
#include "NFmiPsSymbol.h"
// newbase
#include "NFmiVoidPtrList.h"

//! Undocumented
enum NFmiPressSameSymbolsObjects
{
  dPlace = 200,
  dPlaceSymbol,
  dPlaceDefArea,
  dPlacePlotArea,
  dPlaceSubViews,
  dPlaceStepSize,
  dPlaceTable
};

//! Undocumented
class NFmiPressSameSymbols : public NFmiPressDescription
{
 public:
  ~NFmiPressSameSymbols() override;

  NFmiPressSameSymbols();
  NFmiPressSameSymbols(const NFmiRectScale& scale);

  bool WritePS(std::ofstream& theOutFile);

  void SetScale(const NFmiRectScale& scale);
  bool ReadDescription(NFmiString& retString) override;
  int ConvertDefText(NFmiString& object) override;
  void SetOutFile(std::ofstream& outFile);
  NFmiPsSymbol* GetPsSymbol();
  void SetPressProduct(NFmiPressProduct* thePressP);

 protected:
  void NextPoint();

 private:
  FmiCounter itsNumberOfSteps;
  NFmiPoint itsStepSize;
  NFmiRectScale itsCurrentScale;
  NFmiRectScale itsScale;
  NFmiVoidPtrList itsPlaces;
  NFmiPsSymbol itsPsSymbol;
  NFmiPressProduct* itsPressProduct;  // no owner

};  // class NFmiPressSameSymbols

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiPressSameSymbols::~NFmiPressSameSymbols() = default;
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressSameSymbols::NFmiPressSameSymbols() : itsNumberOfSteps(1) {}
// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param scale Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressSameSymbols::NFmiPressSameSymbols(const NFmiRectScale& scale)
    : itsNumberOfSteps(1), itsScale(scale)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param outFile Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressSameSymbols::SetOutFile(std::ofstream& outFile)
{
  itsPsSymbol.SetFile(outFile);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePressP Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressSameSymbols::SetPressProduct(NFmiPressProduct* thePressP)
{
  itsPressProduct = thePressP;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPsSymbol* NFmiPressSameSymbols::GetPsSymbol()
{
  return &itsPsSymbol;
}
#endif  // NFMIPRESSSAMESYMBOLS_H

// ======================================================================
