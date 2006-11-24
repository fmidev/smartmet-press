// ======================================================================
/*!
 * \file
 * \brief NFmiSubstituteParamRect.h
 */
// ======================================================================
/*!
 * \class NFmiSubstituteParamRect
 *
 * Tämä luokka määrittelee korvattavan arvon muunnostaulukoissa,
 * esim lämpötilan tietty fraktiili ja mikä arvo sillä koravtaan.
 * Peritty NFmiParamRectistä jolloin kaikki datankäsittelyominaisuudet.
 *
 */
// ======================================================================

#ifndef NFMISUBSTITUTEPARAMRECT_H
#define NFMISUBSTITUTEPARAMRECT_H

#include "NFmiTextParamRect.h"

//! Undocumented
enum NFmiSubstituteParamRectObjects
{
    dMappingSubstituteValue = 185,
};

//! Undocumented
class _FMI_DLL NFmiSubstituteParamRect : public NFmiTextParamRect
{

public:

  virtual ~NFmiSubstituteParamRect(void);

  NFmiSubstituteParamRect(void);
  NFmiSubstituteParamRect(const NFmiSubstituteParamRect & theSubstituteParamRect); 

  virtual bool ReadDescription(NFmiString & retString);
  virtual NFmiParamRect * Clone(void) const; 
  int ConvertDefText(NFmiString & object);

  virtual bool WritePS(const NFmiRect & AbsoluteRectOfSymbolGroup,
					   NFmiFastQueryInfo * theQI,
					   std::ofstream & theDestinationFile,
					   FmiPressOutputMode theOutput);

protected:

private:
  float itsSubstituteMappingValue;

}; // class NFmiSubstituteParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiSubstituteParamRect::NFmiSubstituteParamRect(void)
  : NFmiTextParamRect()
{
  itsMapping=0;
  fUseSelectLatinFont=true;
  itsSubstituteMappingValue = 0.;
  itsIdentPar = 0;
}

#endif // NFMISUBSTITUTEPARAMRECT_H

// ======================================================================
