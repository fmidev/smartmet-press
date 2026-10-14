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
  dMappingSubstituteValue = 185
};

//! Undocumented
class NFmiSubstituteParamRect : public NFmiTextParamRect
{
 public:
  ~NFmiSubstituteParamRect() override;

  NFmiSubstituteParamRect();
  NFmiSubstituteParamRect(const NFmiSubstituteParamRect& theSubstituteParamRect);

  bool ReadDescription(NFmiString& retString) override;
  NFmiParamRect* Clone() const override;
  int ConvertDefText(NFmiString& object) override;

  bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
               NFmiFastQueryInfo* theQI,
               std::ofstream& theDestinationFile,
               FmiPressOutputMode theOutput) override;

 protected:
 private:
  float itsSubstituteMappingValue;

};  // class NFmiSubstituteParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiSubstituteParamRect::NFmiSubstituteParamRect()
{
  itsMapping = nullptr;
  fUseSelectLatinFont = true;
  itsSubstituteMappingValue = 0.;
  itsIdentPar = 0;
}

#endif  // NFMISUBSTITUTEPARAMRECT_H

// ======================================================================
