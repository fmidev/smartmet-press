// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressArea
 */
// ======================================================================
/*!
 * \class NFmiPressArea
 *
 * Area-luokka, joka m��ritelt�viss�. Voidaan k�ytt�� asemien
 * sijoituksessa tuotteeseen. Kun area on annettu riitt�� pelk�n
 * (x,y) paikan antaminen, jolloin ohjelmaa osaa ratkaista lon/latin.
 * Tai p�invastoin voidaan antaa lon/lat, jolloin ohjelma sijoittaa
 * oikealle paikalle tuotteessa. My�s paikan nimi kelpaa jos se on
 * asematLonLat-tiedostossa.
 *
 * Areaa voidaan my�s k�ytt�� aluekeskiarvojen, summien ym
 * laskemisessa. T�ll�in ei tarvitse antaa xy-mittoja (jos
 * ylemm�ll� tasolla  tiedet��n ett� t�h�n k�ytet��n)
 *
 */
// ======================================================================

#ifndef NFMIPRESSAREA_H
#define NFMIPRESSAREA_H

#include "NFmiPressDescription.h"
#include "NFmiStereographicArea.h"
#include "NFmiYKJArea.h"
class NFmiPressProduct;

//! Undocumented
enum NFmiAreaObjects
{
  dLonLatCorners = 400,
  dLonLatCornerNames,
  dXYCorners,
  dWorldXYCorners,
  dProjection,
  dOrientation,
  dTrueLat
};

//! Undocumented
class NFmiPressArea : public NFmiPressDescription
{
 public:
  ~NFmiPressArea(void);
  NFmiPressArea(void);
  NFmiPressArea(const NFmiPressArea& anArea);

  virtual int ConvertDefText(NFmiString& object);
  virtual bool ReadDescription(NFmiString& retString);

  NFmiArea* GetArea(void) const;
  void SetArea(NFmiArea* theArea);
  void SetProduct(NFmiPressProduct* theProduct);
  void SetXyRequest(bool value);

 private:
  NFmiArea* itsArea;
  NFmiPressProduct* itsPressProduct;  // ei omista
  bool fIsXyRequest;

};  // class NFmiPressArea

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressArea::NFmiPressArea(void) : itsArea(0), itsPressProduct(0), fIsXyRequest(true) {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiArea* NFmiPressArea::GetArea(void) const
{
  return itsArea;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theArea Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressArea::SetArea(NFmiArea* theArea)
{
  itsArea = theArea;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theArea Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressArea::SetProduct(NFmiPressProduct* theProduct)
{
  itsPressProduct = theProduct;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressArea::SetXyRequest(bool value)
{
  fIsXyRequest = value;
}
#endif  // NFMIPRESSAREA_H

// ======================================================================
