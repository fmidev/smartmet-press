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
#include "NFmiPsSymbol.h"
#include "NFmiRectScale.h"
#include "NFmiPressDescription.h"
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
class _FMI_DLL NFmiPressSameSymbols : public NFmiPressDescription
{
public:

  virtual ~NFmiPressSameSymbols(void);

  NFmiPressSameSymbols(void);
  NFmiPressSameSymbols(const NFmiRectScale & scale);

  bool WritePS(std::ofstream & theOutFile);
  
  void SetScale(const NFmiRectScale & scale);        
  virtual bool ReadDescription(NFmiString & retString); 
  int ConvertDefText(NFmiString & object);
  void SetOutFile(std::ofstream & outFile);
  NFmiPsSymbol * GetPsSymbol(void);

protected:
  
  void NextPoint(void);

private:

  FmiCounter itsNumberOfSteps;
  NFmiPoint itsStepSize;   
  NFmiRectScale itsCurrentScale; 
  NFmiRectScale itsScale;
  NFmiVoidPtrList itsPlaces;
  NFmiPsSymbol itsPsSymbol;

}; // class NFmiPressSameSymbols

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline
NFmiPressSameSymbols::~NFmiPressSameSymbols (void)
{
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressSameSymbols::NFmiPressSameSymbols(void)
  : itsNumberOfSteps(1)
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param scale Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiPressSameSymbols::NFmiPressSameSymbols(const NFmiRectScale & scale)    
  : itsNumberOfSteps(1)
  , itsScale(scale)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param outFile Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressSameSymbols::SetOutFile(std::ofstream & outFile)
{
  itsPsSymbol.SetFile(outFile);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiPsSymbol * NFmiPressSameSymbols::GetPsSymbol(void)
{
  return &itsPsSymbol;
}

#endif // NFMIPRESSSAMESYMBOLS_H

// ======================================================================
