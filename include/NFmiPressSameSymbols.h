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
 * Toimivuutta viimeisess� versiossa ei ole testattu.
 * K�ytt�tarve aika pient�, joten kannattaako vied� k�ytt�oppaaseen?
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
  virtual ~NFmiPressSameSymbols(void);

  NFmiPressSameSymbols(void);
  NFmiPressSameSymbols(const NFmiRectScale& scale);

  bool WritePS(std::ofstream& theOutFile);

  void SetScale(const NFmiRectScale& scale);
  virtual bool ReadDescription(NFmiString& retString);
  int ConvertDefText(NFmiString& object);
  void SetOutFile(std::ofstream& outFile);
  NFmiPsSymbol* GetPsSymbol(void);
  void SetPressProduct(NFmiPressProduct* thePressP);

 protected:
  void NextPoint(void);

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

inline NFmiPressSameSymbols::~NFmiPressSameSymbols(void) {}
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressSameSymbols::NFmiPressSameSymbols(void) : itsNumberOfSteps(1) {}
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

inline NFmiPsSymbol* NFmiPressSameSymbols::GetPsSymbol(void) { return &itsPsSymbol; }
#endif  // NFMIPRESSSAMESYMBOLS_H

// ======================================================================
