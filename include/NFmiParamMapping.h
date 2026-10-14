// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiParamMapping
 */
// ======================================================================
/*!
 * \class NFmiParamMapping
 *
 * Information of single mapping interval and associated symbol.
 *
 * Oorganized series of mapping intervals; knows how to add
 * new mapping interval; when given a value, finds the right
 * interval and returns associated symbol.
 */
// ======================================================================

#ifndef NFMIPARAMMAPPING_H
#define NFMIPARAMMAPPING_H

#include "NFmiSize.h"
#include "NFmiString.h"

//! Undocumented
struct NFmiMappingInterval
{
  double itsBottomValue;
  double itsTopValue;
  NFmiString itsSymbol;
  bool fIsScaled;
};

//! Undocumented
class NFmiParamMapping : public NFmiSize
{
 public:
  ~NFmiParamMapping() override;
  NFmiParamMapping();
  NFmiParamMapping(const NFmiParamMapping& theParamMapping);

  void AddMappingInterval(const NFmiMappingInterval& theInterval);
  void AddMappingInterval(double theBottomValue, double theTopValue, const NFmiString& theSymbol);

  bool IsInFirst(const double theValue);

  NFmiString* Map(const double theValue);
  NFmiString* Map(const double theValue, bool& outIsScaled);

 private:
  NFmiMappingInterval* itsMappingIntervals = nullptr;

};  // class NFmiParamMapping

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiParamMapping::NFmiParamMapping() : NFmiSize(0) {}
#endif  // NFMIPARAMMAPPING_H

// ======================================================================
