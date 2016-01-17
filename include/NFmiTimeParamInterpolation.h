// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiTimeParamInterpolation
 */
// ======================================================================
/*!
 * \class NFmiTimeParamInterpolation
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMITIMEPARAMINTERPOLATION_H
#define NFMITIMEPARAMINTERPOLATION_H

class NFmiFastQueryInfo;
class NFmiWeightTimeScale;

//! Undocumented
class NFmiTimeParamInterpolation
{
 public:
  virtual ~NFmiTimeParamInterpolation(void);
  NFmiTimeParamInterpolation(void);
  NFmiTimeParamInterpolation(NFmiFastQueryInfo* theInfo, NFmiWeightTimeScale* theWeights);

 protected:
  NFmiFastQueryInfo* itsInfo;
  NFmiWeightTimeScale* itsWeightScale;

};  // class NFmiTimeParamInterpolation

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theInfo Undocumented
 * \param theWeights Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiTimeParamInterpolation::NFmiTimeParamInterpolation(NFmiFastQueryInfo* theInfo,
                                                              NFmiWeightTimeScale* theWeights)
    : itsInfo(theInfo), itsWeightScale(theWeights)
{
}

#endif  // NFMITIMEPARAMINTERPOLATION_H

// ======================================================================
