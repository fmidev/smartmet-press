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
  dImageShear
};


//! Undocumented
class _FMI_DLL NFmiPressImage : public NFmiPressScaling
{
public:

  virtual ~NFmiPressImage(void);
  NFmiPressImage(void);
  NFmiPressImage(const NFmiPressImage & theOtherImage); 
  
  virtual bool ReadDescription(NFmiString & retString); 
  int ConvertDefText(NFmiString & object);

  using NFmiPressScaling::WritePS;
  virtual bool WritePS(FmiPressOutputMode theOutput);
  void ScalePlotting(void);
  void SetPath(const NFmiString & thePath);
  NFmiFileString GetPath(void) const;
  virtual unsigned long ClassId(void);
  void SetPressProduct(NFmiPressProduct*  thePressProductOwner);
  NFmiPressProduct* GetPressProduct(void)const;
		
protected:               
			 		
  NFmiFileString itsPath;
  NFmiRectScale itsImageScale;
  NFmiRect itsClippingRect;
  //to be able to add recursively objects to the pressProducts object list: 
  NFmiPressProduct* itsPressProduct; //not owner;
  void SetTempImageFile (const NFmiString& theFile);
  void SetImageScale(const NFmiRectScale& theScale);
  void SetClippingRect(const NFmiRect& theRect);
      
private:
	//tempit tarvitaan nyt rekursion takia
  NFmiString  itsTempImageFile;
  NFmiString  itsTempImagePath;
  NFmiString  itsTempImageDir;
  float itsShear;

}; // class NFmiPressImage



// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressImage::SetPressProduct(NFmiPressProduct* thePressProductOwner)
{
  itsPressProduct=thePressProductOwner;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePath Undocumented
 */
// ----------------------------------------------------------------------

inline
void  NFmiPressImage::SetTempImageFile (const NFmiString& theFile)
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

inline
void  NFmiPressImage::SetImageScale (const NFmiRectScale& theScale)
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

inline
void  NFmiPressImage::SetClippingRect (const NFmiRect& theRect)
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

inline
NFmiPressProduct* NFmiPressImage::GetPressProduct(void)const
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

inline
void NFmiPressImage::SetPath(const NFmiString & thePath)
{
  itsPath=thePath;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiFileString NFmiPressImage::GetPath(void) const
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

inline
unsigned long NFmiPressImage::ClassId(void)
{
  return kNFmiPressImage;
}

#endif // NFMIPRESSIMAGE_H

// ======================================================================
