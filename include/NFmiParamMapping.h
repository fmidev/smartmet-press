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
class _FMI_DLL NFmiParamMapping : public NFmiSize
{
 public:
  virtual ~NFmiParamMapping(void);
  NFmiParamMapping(void);
  NFmiParamMapping(const NFmiParamMapping& theParamMapping);

  void AddMappingInterval(const NFmiMappingInterval& theInterval);
  void AddMappingInterval(double theBottomValue, double theTopValue, const NFmiString& theSymbol);

  bool IsInFirst(const double theValue);

  NFmiString* Map(const double theValue);
  NFmiString* Map(const double theValue, bool& outIsScaled);

 private:
  NFmiMappingInterval* itsMappingIntervals;

};  // class NFmiParamMapping

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiParamMapping::NFmiParamMapping(void) : NFmiSize(0), itsMappingIntervals(0) {}
#endif  // NFMIPARAMMAPPING_H

// ======================================================================
