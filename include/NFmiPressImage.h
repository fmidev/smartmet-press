// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressImage
 */
// ======================================================================
/*!
 * \class NFmiPressImage
 *
 * Valmiiden EPS-kuvien litt‰minen ps-tiedostoon. Peritty
 * NFmiScaling:st‰ mutta jouduttu lis‰‰m‰‰n skaala-k‰sittely‰
 * ja alueen leikkausta (clip).
 *
 */
// ======================================================================

#ifndef NFMIPRESSIMAGE_H
#define NFMIPRESSIMAGE_H

#include "NFmiFileString.h"
#include "NFmiPressScaling.h"
class NFmiPressProduct;

//! Undocumented
enum NFmiPressImageObjects
{
  dImageFile = 260,
  dSummerWinterImageFile,
  dImagePath,
  dImageDir,
  dImageDefSize,
  dImagePlottingView,
  dImageClippingRectangle,
  dImageSizeFactor,
  dImagePlaceMove,
  dNewImage,
  dNewImageName = 1260,
  dNewImageRel,
  dImageShear,
  dImageClippingPath,
  dImageFileWithTimeStamp,
  dNewImageRelWithTimeStamp,
  dImagePlottingAndClipping,
  dSummerWinterChoise  // almost = dSummerWinterImageFile
};

//! Undocumented
class NFmiPressImage : public NFmiPressScaling
{
 public:
  ~NFmiPressImage() override;
  NFmiPressImage();
  NFmiPressImage(const NFmiPressImage& theOtherImage);

  bool ReadDescription(NFmiString& retString) override;
  int ConvertDefText(NFmiString& object) override;

  using NFmiPressScaling::WritePS;
  bool WritePS(FmiPressOutputMode theOutput) override;
  void ScalePlotting() override;
  void SetPath(const NFmiString& thePath);
  NFmiFileString GetPath() const;
  unsigned long ClassId() override;
  void SetPressProduct(NFmiPressProduct* thePressProductOwner);
  NFmiPressProduct* GetPressProduct() const;
  void SetRelHoursFromFirst(int theRelHoursFromFirst);
  int GetRelHoursFromFirst() const;

 protected:
  NFmiFileString itsPath;
  NFmiRectScale itsImageScale;
  NFmiRect itsClippingRect;
  std::vector<NFmiPoint> itsClippingPoints;
  // to be able to add recursively objects to the pressProducts object list:
  NFmiPressProduct* itsPressProduct = nullptr;  // not owner;
  void SetTempImageFile(const NFmiString& theFile);
  void SetImageScale(const NFmiRectScale& theScale);
  void SetClippingRect(const NFmiRect& theRect);
  void SetClippingPoints(const std::vector<NFmiPoint> thePoints);

 private:
  // tempit tarvitaan nyt rekursion takia
  NFmiString itsTempImageFile;
  NFmiString itsFileWithoutTimeStamp;
  NFmiString itsTempImagePath;
  NFmiString itsTempImageDir;
  int itsRelHoursFromFirst = 0;
  float itsShear = 0.;

};  // class NFmiPressImage

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressImage::SetRelHoursFromFirst(int theRelHoursFromFirst)
{
  itsRelHoursFromFirst = theRelHoursFromFirst;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline int NFmiPressImage::GetRelHoursFromFirst() const
{
  return itsRelHoursFromFirst;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressImage::SetPressProduct(NFmiPressProduct* thePressProductOwner)
{
  itsPressProduct = thePressProductOwner;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressImage::SetTempImageFile(const NFmiString& theFile)
{
  itsTempImageFile = theFile;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressImage::SetImageScale(const NFmiRectScale& theScale)
{
  itsImageScale = theScale;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressImage::SetClippingRect(const NFmiRect& theRect)
{
  itsClippingRect = theRect;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressImage::SetClippingPoints(const std::vector<NFmiPoint> thePoints)
{
  itsClippingPoints = thePoints;
}  // ----------------------------------------------------------------------
   /*!
    * Undocumented
    *
    * \param thePath Undocumented
    */
// ----------------------------------------------------------------------

inline NFmiPressProduct* NFmiPressImage::GetPressProduct() const
{
  return itsPressProduct;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressImage::SetPath(const NFmiString& thePath)
{
  itsPath = thePath;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiFileString NFmiPressImage::GetPath() const
{
  return itsPath;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline unsigned long NFmiPressImage::ClassId()
{
  return kNFmiPressImage;
}
#endif  // NFMIPRESSIMAGE_H

// ======================================================================
