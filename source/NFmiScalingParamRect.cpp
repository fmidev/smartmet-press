// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiScalingParamRect
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiScalingParamRect.h"
#include "NFmiPressParam.h"
#include <iostream>
#include <algorithm>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiScalingParamRect::~NFmiScalingParamRect(void) {}
// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theSymbolRect The object to copy
 */
// ----------------------------------------------------------------------

NFmiScalingParamRect::NFmiScalingParamRect(const NFmiScalingParamRect& theSymbolRect)
    : NFmi2SymbolParamRect(theSymbolRect),
      itsXValueScaling(theSymbolRect.itsXValueScaling),
      itsYValueScaling(theSymbolRect.itsYValueScaling)
{
}

// ----------------------------------------------------------------------
/*!
 * Create a new copy of this
 *
 * \return The clone
 * \todo Should return an auto_ptr
 */
// ----------------------------------------------------------------------

NFmiParamRect* NFmiScalingParamRect::Clone(void) const { return new NFmiScalingParamRect(*this); }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiScalingParamRect::ReadRemaining(void)
{
  switch (itsIntObject)
  {
    case dHeightScale:
    {
      ReadSymbolScale(itsYValueScaling);
      break;
    }
    case dWidthScale:
    {
      ReadSymbolScale(itsXValueScaling);
      break;
    }
    default:
    {
      return NFmi2SymbolParamRect::ReadRemaining();
    }
  }
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiScalingParamRect::ReadSymbolScale(FmiValueScaling& theScale)
{
  if (SetThree(theScale.noneValue, theScale.symbolValue, theScale.maxValue))
  {
    if (theScale.noneValue == theScale.symbolValue)
    {
      *itsLogFile << "*** ERROR: First two values of SymbolScale cannot be identical" << endl;
      theScale.symbolValue = kFloatMissing;
      return false;
    }
    return true;
  }
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiScalingParamRect::ConvertDefText(NFmiString& object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if (lowChar == NFmiString("heightscale") || lowChar == NFmiString("korkeusskaala"))
    return dHeightScale;

  else if (lowChar == NFmiString("widthscale") || lowChar == NFmiString("leveysskaala"))
    return dWidthScale;

  else if (lowChar == NFmiString("scalingparameter") || lowChar == NFmiString("skaalaavaparametri"))
    return dSecondPar;

  else
    return NFmi2SymbolParamRect::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScalingParamRect::DoPostReading(void)
{
  if (!itsSecondDataIdent.IsDataParam()) itsSecondDataIdent.SetParam(*GetDataIdent().GetParam());
  itsSecondDataIdent.SetProducer(*GetDataIdent().GetProducer());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiScalingParamRect::ScaleByValue()
{
  if (itsYValueScaling.symbolValue != kFloatMissing)
  {
    NFmiScale endYScale = itsDefToProductScale.GetYEndScale();
    double valueScale =
        (FmiMin(static_cast<double>(itsSecondParamValue), itsYValueScaling.maxValue) -
         itsYValueScaling.noneValue) /
        (itsYValueScaling.symbolValue - itsYValueScaling.noneValue);
    endYScale.SetEndValue((endYScale.EndValue() - endYScale.StartValue()) *
                              static_cast<float>(valueScale) +
                          endYScale.StartValue());
    itsDefToProductScale.SetYEndScale(endYScale);
  }
  if (itsXValueScaling.symbolValue != kFloatMissing)
  {
    NFmiScale endXScale = itsDefToProductScale.GetXEndScale();
    double valueScale =
        (FmiMin(static_cast<double>(itsSecondParamValue), itsXValueScaling.maxValue) -
         itsXValueScaling.noneValue) /
        (itsXValueScaling.symbolValue - itsXValueScaling.noneValue);
    endXScale.SetEndValue((endXScale.EndValue() - endXScale.StartValue()) *
                              static_cast<float>(valueScale) +
                          endXScale.StartValue());
    itsDefToProductScale.SetXEndScale(endXScale);
  }
}

// ======================================================================
