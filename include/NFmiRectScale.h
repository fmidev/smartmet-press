// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiRectScale
 */
// ======================================================================
/*!
 * \class NFmiRectScale
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMIRECTSCALE_H
#define NFMIRECTSCALE_H

#include "NFmiRect.h"
#include "NFmiScale.h"

//! Undocumented
class NFmiRectScale
{
 public:
  virtual ~NFmiRectScale();
  NFmiRectScale();
  NFmiRectScale(const NFmiRect& theStartRect, const NFmiRect& theEndRect);
  NFmiRectScale(const NFmiRectScale&) = default;
  NFmiRectScale& operator=(const NFmiRectScale&) = default;

  void SetStartScales(const NFmiRect& startRect);
  void SetXStartScale(const NFmiScale& scale);
  void SetYStartScale(const NFmiScale& scale);
  void SetXEndScale(const NFmiScale& scale);
  void SetYEndScale(const NFmiScale& scale);
  void MoveEndScales(const NFmiPoint& theDiff);
  void MoveXEndScale(double theDelta);
  void SetEndScales(const NFmiRect& endRect);
  void RescaleEndScales(double withFactor);
  void SetScales(const NFmiRect& startRect, const NFmiRect& endRect);
  double GetXScaling() const;
  double GetYScaling() const;
  NFmiRect GetStartScales() const;
  NFmiRect GetEndScales() const;
  NFmiScale GetXStartScale() const;
  NFmiScale GetYStartScale() const;
  NFmiScale GetXEndScale() const;
  NFmiScale GetYEndScale() const;
  NFmiPoint GetEndStartPoint() const;
  NFmiPoint GetStartStartPoint() const;
  NFmiPoint GetStartCenter() const;
  NFmiPoint GetEndCenter() const;
  NFmiPoint GetScaling() const;
  NFmiPoint Scale(const NFmiPoint& fromPoint) const;
  NFmiRectScale Scale(const NFmiRectScale& rectScale) const;
  NFmiRect ScaleRect(const NFmiRect& fromRect) const;
  double ScaleX(double fromValue) const;
  double ScaleY(double fromValue) const;
  NFmiPoint UnScale(const NFmiPoint& fromPoint) const;
  NFmiRect UnScaleRect(const NFmiRect& fromRect) const;
  double UnScaleX(double fromValue) const;
  double UnScaleY(double fromValue) const;

 protected:
 private:
  NFmiScale itsXStartScale;
  NFmiScale itsYStartScale;
  NFmiScale itsXEndScale;
  NFmiScale itsYEndScale;

};  // class NFmiRectScale

#endif  // NFMIRECTSCALE_H

// ======================================================================
