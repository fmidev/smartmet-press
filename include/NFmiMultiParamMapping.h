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
  FmiMappingInterval mappingInterval[FmiMaxNumOfMappingParams]{};
  NFmiString symbol;
};

//! Undocumented
class NFmiMultiParamMapping : public NFmiSize
{
 public:
  ~NFmiMultiParamMapping() override;

  NFmiMultiParamMapping();
  NFmiMultiParamMapping(const NFmiMultiParamMapping &theParamMapping);
  NFmiMultiParamMapping& operator=(const NFmiMultiParamMapping&) = default;

  FmiMultiMapping ReadOneMapping(std::ifstream *inFile);

  void AddMappingInterval(const FmiMultiMapping &theInterval);

  NFmiString *Map(const std::vector<float> &values, bool &missingFound);
  void SetMissing(const NFmiString &theString);
  NFmiString* GetMissing() const;

  void NumOfParams(short num);
  short NumOfParams() const;
  bool IsIncomplete() const;
  bool Complete(float theOldValue, float theNewValue);
  void SetComplete();
  short CheckIfIncomplete();

 private:
  FmiMultiMapping* itsMappingIntervals = nullptr;
  short itsNumOfParams = 0;
  bool fIncomplete = false;

};  // class NFmiMultiParamMapping

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiMultiParamMapping::NFmiMultiParamMapping() : NFmiSize(0) {}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param num Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiMultiParamMapping::IsIncomplete() const
{
  return fIncomplete;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param num Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiMultiParamMapping::SetComplete()
{
  fIncomplete = false;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param num Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiMultiParamMapping::NumOfParams(short num)
{
  itsNumOfParams = num;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline short NFmiMultiParamMapping::NumOfParams() const
{
  return itsNumOfParams;
}
#endif  // NFMIMULTIPARAMMAPPING_H

// ======================================================================
