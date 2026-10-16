// ======================================================================
/*!
 * \file
 * \brief NFmiNameDay.h
 */
// ======================================================================
/*!
 * \class NFmiNameDay
 *
 *
 * Tekee annetusta tiedostosta nimip‰iv‰taulukon. Tiedoston tulee olla muotoa:
 *
 * \code
 *      ....
 *		9.tammi	Veijo
 *		9.tammi	Veikko
 *		9.tammi	Veli
 *		10.tammi	Nyyrikki
 *		11.tammi	Kari
 *		11.tammi	Karri
 *      ....
 * \endcode
 *
 * Oleellista on kaksi stringi‰ per rivi. Ekan tulee olla erilainen
 * (sis‰llˆll‰ ei v‰li‰) jokaiselle uudelle p‰iv‰lle, ja tokassa tulee
 * itse nimi. Samalle p‰iv‰lle voi antaa 1-n kappaletta
 * nimi‰. Nimip‰iv‰ kysyt‰‰n oliolta MetTime:lla tuloksen ollessa
 * muotoa "Kari, Karri". Tuloksen voi rajoittaa nimien lukum‰‰r‰n tai
 * merkkim‰‰r‰n mukaan. Karkausp‰iv‰lle ei voi antaa arvoa.
 *
 * Soveltuu myˆs muihin samankaltaisiin teht‰viin.
 *
 * K‰ytet‰‰n aluksi lehtis‰‰ohjelmasta.
 *
 */
// ======================================================================

#ifndef NFMINAMEDAY_H
#define NFMINAMEDAY_H

#include "NFmiMetTime.h"
#include "NFmiString.h"

//! Undocumented
class NFmiNameDay
{
 public:
  virtual ~NFmiNameDay();
  NFmiNameDay();

  bool IsRead() const;
  bool IsValue() const;

  bool ReadFile(const NFmiString& theFileName);

  NFmiString GetName(const NFmiMetTime& theTime,
                     FmiCounter maxNumber = kShortMissing,
                     FmiCounter maxLength = kShortMissing);

 private:
  NFmiString itsNames[366];  // 0 ei k‰ytet‰
  bool fRead{false};
  bool fValue;

};  // class NFmiNameDay

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiNameDay::NFmiNameDay()
{

  fValue = false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiNameDay::IsRead() const
{
  return fRead;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiNameDay::IsValue() const
{
  return fValue;
}
#endif  // NFMINAMEDAY_H

// ======================================================================
