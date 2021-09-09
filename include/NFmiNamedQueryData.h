// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiNamedQueryData.h
 */
// ======================================================================
/*!
 * \class NFmiNamedQueryData
 *
 * Tässä on vain QueryData yhdistetty stringiin jolla saadaan
 * useammat QD:t yksilöityä tarvittaessa. Tarvitaan kun
 * lehtituotteisiin halutaan useampi QD samaan tuotteeseen.
 *
 */
// ======================================================================

#ifndef NFMINAMEDQUERYDATA_H
#define NFMINAMEDQUERYDATA_H

#include "NFmiQueryData.h"

//! Undocumented
class NFmiNamedQueryData
{
 public:
  ~NFmiNamedQueryData(void);
  NFmiNamedQueryData(void);
  NFmiNamedQueryData(NFmiQueryData* data,
                     NFmiString name,
                     bool mandatory = false,
                     bool newest = false,
                     bool yearData = false);
  NFmiNamedQueryData(const NFmiNamedQueryData& theNQD);

  void SetData(NFmiQueryData* theData);
  NFmiQueryData* GetData(void);
  NFmiString GetName(void);
  bool IsMandatory(void);
  bool IsNewest(void);
  bool IsYearData(void);

 private:
  NFmiQueryData* itsData;
  NFmiString itsName;
  bool fMandatory;
  bool fNewest;
  bool fYearData;

};  // class NFmiNamedQueryData

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiNamedQueryData::NFmiNamedQueryData(void)
    : itsData(0), fMandatory(false), fNewest(false), fYearData(false)
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param data Undocumented
 * \param name Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiNamedQueryData::NFmiNamedQueryData(
    NFmiQueryData* data, NFmiString name, bool mandatory, bool newest, bool yearData)
    : itsData(data), itsName(name), fMandatory(mandatory), fNewest(newest), fYearData(yearData)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theNQD The object being copied
 */
// ----------------------------------------------------------------------

inline NFmiNamedQueryData::NFmiNamedQueryData(const NFmiNamedQueryData& theNQD)
{
  itsData = theNQD.itsData;
  itsName = theNQD.itsName;
  fMandatory = theNQD.fMandatory;
  fNewest = theNQD.fNewest;
  fYearData = theNQD.fYearData;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theData Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiNamedQueryData::SetData(NFmiQueryData* theData)
{
  itsData = theData;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiQueryData* NFmiNamedQueryData::GetData(void)
{
  return itsData;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiNamedQueryData::GetName(void)
{
  return itsName;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiNamedQueryData::IsMandatory(void)
{
  return fMandatory;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiNamedQueryData::IsNewest(void)
{
  return fNewest;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiNamedQueryData::IsYearData(void)
{
  return fYearData;
}
#endif  // NFMINAMEDQUERYDATA_H

// ======================================================================
