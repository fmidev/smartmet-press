// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiScale
 */
// ======================================================================
/*!
 * \class NFmiScale
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMISCALE_H
#define NFMISCALE_H

// #include "NFmiSaveBase.h"
#include "NFmiGlobals.h"

//! Undocumented
class _FMI_DLL NFmiScale
{
 public:
  virtual ~NFmiScale(void);
  NFmiScale(void);
  NFmiScale(float theStartValue, float theEndValue);
  NFmiScale(const NFmiScale& anOtherScale);

  NFmiScale& operator=(const NFmiScale& anOtherScale);
  NFmiScale& operator+=(const NFmiScale& anOtherScale);
  NFmiScale& operator-=(const NFmiScale& anOtherScale);

  float StartValue(void) const;
  float EndValue(void) const;
  double CenterValue(void) const;
  void SetStartValue(float theValue);
  void SetEndValue(float theValue);
  void Set(float theStartValue, float theEndValue);
  float RelLocation(float theValue) const;
  float Location(float theRelValue) const;
  bool Inside(float theValue) const;
  virtual void StartFromZeroOptionally(float theFactor = 1.);
  virtual void ExpandIfNotZero(float theFactor = .05);
  virtual void ExpandIfEqual(float theAdditionToBothEnds = 1.);
  virtual void Expand(float theAdditionToBothEnds = 1.);
  void LimitCheck(bool theCheck);
  virtual void ExcludePositive(void);
  virtual void ExcludeNegative(void);
  bool Ok(void) const;
  float Difference(void) const;

 protected:
  void Check(void);
  bool IsMissing(void) const;

  float itsStartValue;
  float itsEndValue;
  float itsEpsilon;
  bool itsDataOk = false;
  bool itsLimitCheck = false;

 private:
};  // class NFmiScale

// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline float NFmiScale::StartValue(void) const { return itsStartValue; }
// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------
inline float NFmiScale::EndValue(void) const { return itsEndValue; }
// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \param theValue Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiScale::SetStartValue(float theValue)
{
  itsStartValue = theValue;
  Check();
}

// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \param theValue Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiScale::SetEndValue(float theValue)
{
  itsEndValue = theValue;
  Check();
}

// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \param theStartValue Undocumented
 * \param theEndValue Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiScale::Set(float theStartValue, float theEndValue)
{
  itsStartValue = theStartValue;
  itsEndValue = theEndValue;
  Check();
}

// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline float NFmiScale::Difference(void) const { return itsEndValue - itsStartValue; }
// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiScale::CenterValue(void) const { return (itsStartValue + itsEndValue) / 2.; }
// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \param theCheck Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiScale::LimitCheck(bool theCheck) { itsLimitCheck = theCheck; }
// ----------------------------------------------------------------------
/*!
 * Undcoumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiScale::Ok(void) const { return itsDataOk; }
#endif  // NFMISCALE_H

// ======================================================================
