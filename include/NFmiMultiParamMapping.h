// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiMultiParamMapping.h
 */
// ======================================================================
/*!
 * \class NFmiMultiParamMapping
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMIMULTIPARAMMAPPING_H
#define NFMIMULTIPARAMMAPPING_H

#include "NFmiParameterName.h"
#include "NFmiSize.h"
#include "NFmiString.h"
#include <vector>

//! Undocumented
struct FmiMappingInterval
{
  float lowBorder;
  float highBorder;
};

const float FmiStartOfIncompleteValues = 31690.;

//! Undocumented
const int FmiMaxNumOfMappingParams = 9;  // pitääkö olla yksi enemmän kuin tarve

//! Undocumented
struct FmiMultiMapping
{
  FmiMappingInterval mappingInterval[FmiMaxNumOfMappingParams];
  NFmiString symbol;
};

//! Undocumented
class NFmiMultiParamMapping : public NFmiSize
{
 public:
  virtual ~NFmiMultiParamMapping(void);

  NFmiMultiParamMapping(void);
  NFmiMultiParamMapping(const NFmiMultiParamMapping &theParamMapping);

  FmiMultiMapping ReadOneMapping(std::ifstream *inFile);

  void AddMappingInterval(const FmiMultiMapping &theInterval);

  NFmiString *Map(const std::vector<float> &values, bool &missingFound);
  void SetMissing(const NFmiString &theString);
  NFmiString *GetMissing(void) const;

  void NumOfParams(short num);
  short NumOfParams(void);
  bool IsIncomplete(void) const;
  bool Complete(float theOldValue, float theNewValue);
  void SetComplete(void);
  short CheckIfIncomplete(void);

 private:
  FmiMultiMapping *itsMappingIntervals;
  short itsNumOfParams;
  bool fIncomplete;

};  // class NFmiMultiParamMapping

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiMultiParamMapping::NFmiMultiParamMapping(void)
    : NFmiSize(0), itsMappingIntervals(0), itsNumOfParams(0), fIncomplete(false)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param num Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiMultiParamMapping::IsIncomplete(void) const { return fIncomplete; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param num Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiMultiParamMapping::SetComplete(void) { fIncomplete = false; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param num Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiMultiParamMapping::NumOfParams(short num) { itsNumOfParams = num; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline short NFmiMultiParamMapping::NumOfParams(void) { return itsNumOfParams; }
#endif  // NFMIMULTIPARAMMAPPING_H

// ======================================================================
