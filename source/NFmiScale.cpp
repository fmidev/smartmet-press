// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiScale
 */
// ======================================================================

#include "NFmiScale.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiScale::~NFmiScale(void) {}
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

NFmiScale::NFmiScale(void)
    : itsStartValue(kFloatMissing),
      itsEndValue(kFloatMissing),
      itsEpsilon(0.f),
      itsDataOk(false),
      itsLimitCheck(false)
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theStartValue Undocumented
 * \param theEndValue Undocumented
 */
// ----------------------------------------------------------------------

NFmiScale::NFmiScale(float theStartValue, float theEndValue)
    : itsStartValue(theStartValue), itsEndValue(theEndValue), itsEpsilon(0.f), itsLimitCheck(false)
{
  Check();
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param anOtherScale The object being copied
 */
// ----------------------------------------------------------------------

NFmiScale::NFmiScale(const NFmiScale& anOtherScale)
    : itsStartValue(anOtherScale.itsStartValue),
      itsEndValue(anOtherScale.itsEndValue),
      itsEpsilon(anOtherScale.itsEpsilon),
      itsDataOk(anOtherScale.itsDataOk),
      itsLimitCheck(anOtherScale.itsLimitCheck)
{
}

// ----------------------------------------------------------------------
/*!
 * Assignment operator
 *
 * \param anOtherScale The object being copied
 * \return The object assigned to
 */
// ----------------------------------------------------------------------

NFmiScale& NFmiScale::operator=(const NFmiScale& anOtherScale)
{
  itsStartValue = anOtherScale.itsStartValue;
  itsEndValue = anOtherScale.itsEndValue;
  itsDataOk = anOtherScale.itsDataOk;
  itsLimitCheck = anOtherScale.itsLimitCheck;
  itsEpsilon = anOtherScale.itsEpsilon;

  return *this;
}

// ----------------------------------------------------------------------
/*
 * Addition in-place operator
 *
 * \param anOtherScale The scale to add
 * \return The scale added to
 */
// ----------------------------------------------------------------------

NFmiScale& NFmiScale::operator+=(const NFmiScale& anOtherScale)
{
  // oletetaan että start < end
  if (itsStartValue == kFloatMissing)
  {
    itsStartValue = anOtherScale.itsStartValue;
  }
  else if (anOtherScale.itsStartValue != kFloatMissing)
  {
    itsStartValue = std::min(anOtherScale.itsStartValue, itsStartValue);
  }
  if (itsEndValue == kFloatMissing)
  {
    itsEndValue = anOtherScale.itsEndValue;
  }
  else if (anOtherScale.itsEndValue != kFloatMissing)
  {
    itsEndValue = std::max(anOtherScale.itsEndValue, itsEndValue);
  }
  Check();
  return *this;
}

// ----------------------------------------------------------------------
/*!
 * Substraction in-place operator
 *
 * \param anOtherScale The scale to substract
 * \return The scale substracted from
 */
// ----------------------------------------------------------------------

NFmiScale& NFmiScale::operator-=(const NFmiScale& anOtherScale)
{
  // oletetaan että start < end
  if (Ok() && anOtherScale.Ok())
  {
    if (Inside(anOtherScale.itsEndValue))
    {
      itsStartValue = anOtherScale.itsEndValue;
    }
    else if (Inside(anOtherScale.itsStartValue))
    {
      itsEndValue = anOtherScale.itsStartValue;
    }
    else if (anOtherScale.Inside(itsStartValue) && anOtherScale.Inside(itsEndValue))
    {
      // vanha sisältyy täysin vähennettävään
      itsStartValue = kFloatMissing;
      itsEndValue = kFloatMissing;
    }
  }
  Check();
  return *this;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScale::ExcludePositive(void)
{
  if (!IsMissing())
  {
    {
      if (itsStartValue > 0.) itsStartValue = 0.f;
      if (itsEndValue > 0.) itsStartValue = 0.f;
    }
    Check();
  }
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScale::ExcludeNegative(void)
{
  if (!IsMissing())
  {
    {
      if (itsStartValue < 0.) itsStartValue = 0.f;
      if (itsEndValue < 0.) itsStartValue = 0.f;
    }
    Check();
  }
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFactor Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScale::StartFromZeroOptionally(float theFactor)
{
  // ei toimi laskevalle skaalalle
  if (!IsMissing())
  {
    // Lassen kääntäjä ei selviä  std::min:n sisäisistä funktioista
    float fabsEnd = fabs(itsEndValue);
    float fabsStart = fabs(itsStartValue);
    //	  if ((itsEndValue-itsStartValue)*theFactor > std::min(fabs(itsEndValue),
    // fabs(itsStartValue))
    if ((itsEndValue - itsStartValue) * theFactor > std::min(fabsEnd, fabsStart) && !Inside(0.f))
    {
      if (itsStartValue > 0.)
        itsStartValue = 0.f;
      else
        itsEndValue = 0.f;
    }
  }
}

// ----------------------------------------------------------------------
/*!
 * Test whether the scale is valid (does not contain missing values)
 *
 * \return True, if the scale has missing values
 */
// ----------------------------------------------------------------------

bool NFmiScale::IsMissing(void) const
{
  return (itsStartValue == kFloatMissing || itsEndValue == kFloatMissing);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScale::Check(void)
{
  itsDataOk = (itsStartValue != kFloatMissing && itsEndValue != kFloatMissing);
  if (itsDataOk)
    itsEpsilon = (itsEndValue - itsStartValue) * .001f;
  else
    itsEpsilon = .000001f;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFactor Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScale::ExpandIfNotZero(float theFactor)
{
  if (itsDataOk)
  {
    float expansion = (itsEndValue - itsStartValue) * theFactor;
    if (itsStartValue != 0.) itsStartValue += -expansion;
    if (itsEndValue != 0.) itsEndValue += expansion;
  }
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theInterval Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScale::ExpandIfEqual(float theInterval)
{
  if (itsStartValue == itsEndValue) Expand(theInterval);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theInterval Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScale::Expand(float theInterval)
{
  if (!IsMissing())
  {
    itsStartValue -= theInterval;
    itsEndValue += theInterval;
    itsDataOk = true;
    itsEpsilon = (itsEndValue - itsStartValue) * .001f;
  }
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theValue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiScale::Inside(float theValue) const
{
  // epsilon näyttää tuovan yhden merkitsevän numeron lisää

  return (itsDataOk && theValue + itsEpsilon >= itsStartValue &&
          theValue - itsEpsilon <= itsEndValue);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theValue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiScale::RelLocation(float theValue) const
{
  float value = kFloatMissing;
  if (itsDataOk && theValue != kFloatMissing)
  {
    if (Difference() > 0.f)
      value = (theValue - itsStartValue) / Difference();
    else
      value = 0.f;
  }

  return (value == kFloatMissing || (!Inside(theValue) && itsLimitCheck)) ? kFloatMissing : value;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRelValue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiScale::Location(float theRelValue) const
{
  return itsDataOk ? Difference() * theRelValue + itsStartValue : kFloatMissing;
}

// ======================================================================
