// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiRectScale
 */
// ======================================================================

#include "NFmiRectScale.h"

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiRectScale::~NFmiRectScale(void) {}
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

NFmiRectScale::NFmiRectScale(void)
{
  SetScales(NFmiRect(0., 0., 1., 1.), NFmiRect(0., 0., 1., 1.));
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theStartRect Undocumented
 * \param theEndRect Undocumented
 * \todo Should take const references as input
 */
// ----------------------------------------------------------------------

NFmiRectScale::NFmiRectScale(NFmiRect theStartRect, NFmiRect theEndRect)
    : itsXStartScale(static_cast<float>(theStartRect.Left()),
                     static_cast<float>(theStartRect.Right())),
      itsYStartScale(static_cast<float>(theStartRect.Top()),
                     static_cast<float>(theStartRect.Bottom())),
      itsXEndScale(static_cast<float>(theEndRect.Left()), static_cast<float>(theEndRect.Right())),
      itsYEndScale(static_cast<float>(theEndRect.Top()), static_cast<float>(theEndRect.Bottom()))
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStartRect Undocumented
 * \param theEndRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::SetScales(const NFmiRect& theStartRect, const NFmiRect& theEndRect)
{
  SetStartScales(theStartRect);
  SetEndScales(theEndRect);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param withFactor Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::RescaleEndScales(double withFactor)
{
  double deltaX = withFactor * (itsXEndScale.Difference()) / 2.f;
  double deltaY = withFactor * (itsYEndScale.Difference()) / 2.f;

  SetEndScales(NFmiRect(GetEndCenter().X() - deltaX,
                        GetEndCenter().Y() + deltaY,
                        GetEndCenter().X() + deltaX,
                        GetEndCenter().Y() - deltaY));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::SetXStartScale(const NFmiScale& theScale) { itsXStartScale = theScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::SetYStartScale(const NFmiScale& theScale) { itsYStartScale = theScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::SetXEndScale(const NFmiScale& theScale) { itsXEndScale = theScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::SetYEndScale(const NFmiScale& theScale) { itsYEndScale = theScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStartRect Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::SetStartScales(const NFmiRect& theStartRect)
{
  itsXStartScale.Set(static_cast<float>(theStartRect.Left()),
                     static_cast<float>(theStartRect.Right()));
  itsYStartScale.Set(static_cast<float>(theStartRect.Top()),
                     static_cast<float>(theStartRect.Bottom()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStartRect Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::SetEndScales(const NFmiRect& theStartRect)
{
  itsXEndScale.Set(static_cast<float>(theStartRect.Left()),
                   static_cast<float>(theStartRect.Right()));
  itsYEndScale.Set(static_cast<float>(theStartRect.Top()),
                   static_cast<float>(theStartRect.Bottom()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDelta Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::MoveEndScales(const NFmiPoint& theDiff)
{
  SetEndScales(NFmiRect(
      NFmiPoint(itsXEndScale.StartValue() + theDiff.X(), itsYEndScale.StartValue() + theDiff.Y()),
      NFmiPoint(itsXEndScale.EndValue() + theDiff.X(), itsYEndScale.EndValue() + theDiff.Y())));
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDelta Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRectScale::MoveXEndScale(double theDelta)
{
  SetXEndScale(NFmiScale(itsXEndScale.StartValue() + (float)theDelta,
                         itsXEndScale.EndValue() + (float)theDelta));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiRect NFmiRectScale::GetStartScales(void) const
{
  return NFmiRect(NFmiPoint(itsXStartScale.StartValue(), itsYStartScale.StartValue()),
                  NFmiPoint(itsXStartScale.EndValue(), itsYStartScale.EndValue()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiRect NFmiRectScale::GetEndScales(void) const
{
  return NFmiRect(NFmiPoint(itsXEndScale.StartValue(), itsYEndScale.StartValue()),
                  NFmiPoint(itsXEndScale.EndValue(), itsYEndScale.EndValue()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiScale NFmiRectScale::GetXEndScale(void) const { return itsXEndScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiScale NFmiRectScale::GetYEndScale(void) const { return itsYEndScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiScale NFmiRectScale::GetXStartScale(void) const { return itsXStartScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiScale NFmiRectScale::GetYStartScale(void) const { return itsYStartScale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiRectScale::GetStartStartPoint(void) const
{
  return NFmiPoint(itsXStartScale.StartValue(), itsYStartScale.StartValue());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiRectScale::GetEndStartPoint(void) const
{
  return NFmiPoint(itsXEndScale.StartValue(), itsYEndScale.StartValue());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiRectScale::GetStartCenter(void) const
{
  return NFmiPoint(itsXStartScale.CenterValue(), itsYStartScale.CenterValue());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiRectScale::GetEndCenter(void) const
{
  return NFmiPoint(itsXEndScale.CenterValue(), itsYEndScale.CenterValue());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

double NFmiRectScale::GetXScaling(void) const
{
  return itsXEndScale.Difference() / itsXStartScale.Difference();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

double NFmiRectScale::GetYScaling(void) const
{
  return itsYEndScale.Difference() / itsYStartScale.Difference();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiRectScale::GetScaling(void) const { return NFmiPoint(GetXScaling(), GetYScaling()); }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePoint Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiRectScale::Scale(const NFmiPoint& thePoint) const
{
  return NFmiPoint(ScaleX(thePoint.X()), ScaleY(thePoint.Y()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiRect NFmiRectScale::ScaleRect(const NFmiRect& theRect) const
{
  return NFmiRect(Scale(theRect.TopLeft()), Scale(theRect.BottomRight()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

double NFmiRectScale::ScaleX(double value) const
{
  return itsXEndScale.Location(itsXStartScale.RelLocation(static_cast<float>(value)));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

double NFmiRectScale::ScaleY(double value) const
{
  return itsYEndScale.Location(itsYStartScale.RelLocation(static_cast<float>(value)));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

double NFmiRectScale::UnScaleX(double value) const
{
  return itsXStartScale.Location(itsXEndScale.RelLocation(static_cast<float>(value)));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

double NFmiRectScale::UnScaleY(double value) const
{
  return itsYStartScale.Location(itsYEndScale.RelLocation(static_cast<float>(value)));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePoint Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiRectScale::UnScale(const NFmiPoint& thePoint) const
{
  return NFmiPoint(UnScaleX(thePoint.X()), UnScaleY(thePoint.Y()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiRect NFmiRectScale::UnScaleRect(const NFmiRect& theRect) const
{
  return NFmiRect(UnScale(theRect.TopLeft()), UnScale(theRect.BottomRight()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiRectScale NFmiRectScale::Scale(const NFmiRectScale& theScale) const
{
  // kaksi peräkkäistä rectskaalausta
  return NFmiRectScale(GetStartScales(), theScale.ScaleRect(ScaleRect(GetStartScales())));
}

// ======================================================================
