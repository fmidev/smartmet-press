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

//! Undocumented
struct FmiMappingInterval
{
  float lowBorder;
  float highBorder;
};


//! Undocumented
const int FmiMaxNumOfMappingParams = 8;		// saisko dynaamiseksi


//! Undocumented
struct FmiMultiMapping
{
  FmiMappingInterval mappingInterval[FmiMaxNumOfMappingParams];
  NFmiString symbol;
};


//! Undocumented
class _FMI_DLL NFmiMultiParamMapping : public NFmiSize
{
public:

  virtual ~NFmiMultiParamMapping(void);

  NFmiMultiParamMapping(void);
  NFmiMultiParamMapping(const NFmiMultiParamMapping & theParamMapping);

  FmiMultiMapping ReadOneMapping(std::ifstream * inFile);
  
  void AddMappingInterval(const FmiMultiMapping & theInterval);
  
  NFmiString * Map(float values[FmiMaxNumOfMappingParams], bool & missingFound);
  void SetMissing (const NFmiString &theString);
  NFmiString * GetMissing (void)const;

  void NumOfParams(short num);
  short NumOfParams(void); 

private:

  FmiMultiMapping * itsMappingIntervals;

  short itsNumOfParams;
  NFmiString *itsMissingString;


}; // class NFmiMultiParamMapping

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiMultiParamMapping::NFmiMultiParamMapping(void)
  : NFmiSize(0)
  , itsMappingIntervals(0)
  , itsNumOfParams(0)
  , itsMissingString(0)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param num Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiMultiParamMapping::NumOfParams(short num)
{
  itsNumOfParams=num;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
short NFmiMultiParamMapping::NumOfParams(void)
{
  return itsNumOfParams;
}

#endif // NFMIMULTIPARAMMAPPING_H

// ======================================================================


