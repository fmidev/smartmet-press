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
  ~NFmiNamedQueryData();
  NFmiNamedQueryData();
  NFmiNamedQueryData(NFmiQueryData* data,
                     NFmiString name,
                     bool mandatory = false,
                     bool newest = false,
                     bool yearData = false);
  NFmiNamedQueryData(const NFmiNamedQueryData& theNQD);
  NFmiNamedQueryData& operator=(const NFmiNamedQueryData&) = default;

  void SetData(NFmiQueryData* theData);
  NFmiQueryData* GetData();
  NFmiString GetName();
  bool IsMandatory() const;
  bool IsNewest() const;
  bool IsYearData() const;

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

inline NFmiNamedQueryData::NFmiNamedQueryData()
    : itsData(nullptr), fMandatory(false), fNewest(false), fYearData(false)
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

inline NFmiQueryData* NFmiNamedQueryData::GetData()
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

inline NFmiString NFmiNamedQueryData::GetName()
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

inline bool NFmiNamedQueryData::IsMandatory() const
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

inline bool NFmiNamedQueryData::IsNewest() const
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

inline bool NFmiNamedQueryData::IsYearData() const
{
  return fYearData;
}
#endif  // NFMINAMEDQUERYDATA_H

// ======================================================================
