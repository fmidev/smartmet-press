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


//! Undocumented
enum NFmiPressImageObjects
{
  dImageFile = 260,
  dImagePath,
  dImageDir,
  dImageDefSize,
  dImagePlottingView,
  dImageClippingRectangle,
  dImageSizeFactor,
  dImagePlaceMove
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

  virtual bool WritePS(FmiPressOutputMode theOutput);
  void ScalePlotting(void);
  void SetPath(const NFmiString & thePath);
  NFmiFileString GetPath(void) const;
  virtual unsigned long ClassId(void);
		
protected:               
			 		
  NFmiFileString itsPath;
  NFmiRectScale itsImageScale;
  NFmiRect itsClippingRect;

}; // class NFmiPressImage


// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressImage::NFmiPressImage(void)
  : NFmiPressScaling()
{
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
