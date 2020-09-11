// ======================================================================
/*!
 * \file
 * \brief NFmiValueCorrection.h
 */
// ======================================================================
/*!
 * \class NFmiValueCorrection
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

#ifndef NFMIVALUECORRECTION_H
#define NFMIVALUECORRECTION_H

#include "NFmiMetTime.h"
#include "NFmiString.h"
#include <vector>

struct FmiTempCorrections
{
  NFmiString stationName;
  float dayCorr;
  float nightCorr;
};
//! Undocumented
class NFmiValueCorrection
{
 public:
  NFmiValueCorrection(void);
  virtual ~NFmiValueCorrection(void);
  void SetFiles(const std::string& theFile, std::ofstream* theLogFile);
  bool IsRead(void);
  bool IsMissing(void);
  bool CorrectValue(float& thevalue, const NFmiString& theStation, bool dayTime);

 private:
  // bool ReadFile(const NFmiString & theFileName);
  bool ReadFile(void);

 private:
  std::string itsFile;
  std::ofstream* itsLogFile;
  std::vector<FmiTempCorrections> itsTempCorrections;
  bool fRead;
  bool fMissing;

};  // class NFmiValueCorrection

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiValueCorrection::NFmiValueCorrection(void)
{
  fRead = false;
  fMissing = false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiValueCorrection::IsRead(void) { return fRead; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiValueCorrection::IsMissing(void) { return fMissing; }
#endif  // NFMIVALUECORRECTION_H

// ======================================================================
