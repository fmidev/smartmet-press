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
class _FMI_DLL NFmiRectScale
{
 public:
  virtual ~NFmiRectScale(void);
  NFmiRectScale(void);
  NFmiRectScale(NFmiRect theStartRect, NFmiRect theEndRect);

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
  double GetXScaling(void) const;
  double GetYScaling(void) const;
  NFmiRect GetStartScales(void) const;
  NFmiRect GetEndScales(void) const;
  NFmiScale GetXStartScale(void) const;
  NFmiScale GetYStartScale(void) const;
  NFmiScale GetXEndScale(void) const;
  NFmiScale GetYEndScale(void) const;
  NFmiPoint GetEndStartPoint(void) const;
  NFmiPoint GetStartStartPoint(void) const;
  NFmiPoint GetStartCenter(void) const;
  NFmiPoint GetEndCenter(void) const;
  NFmiPoint GetScaling(void) const;
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
