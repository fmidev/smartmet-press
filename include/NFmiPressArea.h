// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressArea
 */
// ======================================================================
/*!
 * \class NFmiPressArea
 *
 * Area-luokka, joka määriteltävissä. Voidaan käyttää asemien
 * sijoituksessa tuotteeseen. Kun area on annettu riittää pelkän
 * (x,y) paikan antaminen, jolloin ohjelmaa osaa ratkaista lon/latin.
 * Tai päinvastoin voidaan antaa lon/lat, jolloin ohjelma sijoittaa
 * oikealle paikalle tuotteessa. Myös paikan nimi kelpaa jos se on
 * asematLonLat-tiedostossa.
 *
 * Areaa voidaan myös käyttää aluekeskiarvojen, summien ym
 * laskemisessa. Tällöin ei tarvitse antaa xy-mittoja (jos
 * ylemmällä tasolla  tiedetään että tähän käytetään)
 *
 */
// ======================================================================

#ifndef NFMIPRESSAREA_H
#define NFMIPRESSAREA_H

#include "NFmiPressDescription.h"
class NFmiPressProduct;
class NFmiArea;

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
  ~NFmiPressArea() override;
  NFmiPressArea();
  NFmiPressArea(const NFmiPressArea& anArea);

  int ConvertDefText(NFmiString& object) override;
  bool ReadDescription(NFmiString& retString) override;

  NFmiArea* GetArea() const;
  void SetArea(NFmiArea* theArea);
  void SetProduct(NFmiPressProduct* theProduct);
  void SetXyRequest(bool value);

 private:
  NFmiArea* itsArea = nullptr;
  NFmiPressProduct* itsPressProduct = nullptr;  // ei omista
  bool fIsXyRequest = true;

};  // class NFmiPressArea

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressArea::NFmiPressArea() {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiArea* NFmiPressArea::GetArea() const
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
