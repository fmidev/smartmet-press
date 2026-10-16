// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressStationText
 */
// ======================================================================
/*!
 * \class NFmiPressStationText
 *
 * Kirjoittaa QD:n (current)asemannimen ps:na tai muuna koodina
 * output-tiedostoon ja lukee alussa m‰‰rittelyst‰ omat asetuksensa.
 * Nimi voidaan haluttaessa muuttaa, esim Kˆˆpenhamina->K:hamina.
 * Nimenmuutoslista tuodaan segmentista, joka saa nimenvaihdot
 * m‰‰rityksest‰.
 *
 */
// ======================================================================

#ifndef NFMIPRESSSTATIONTEXT_H
#define NFMIPRESSSTATIONTEXT_H

#include "NFmiPressDataObject.h"
#include "NFmiPressText.h"

//! Undocumented
struct NFmiRenaming
{
  NFmiString originalName;
  NFmiString newName;
};

class NFmiPressParam;

//! Undocumented
class NFmiPressStationText : public NFmiPressText, public NFmiPressDataObject
{
 public:
  ~NFmiPressStationText() override;
  NFmiPressStationText();

  void SetNewNames(NFmiVoidPtrList* theNames);
  bool SetNewName(const NFmiRenaming& theNames);
  void SetPressParam(NFmiPressParam* pressParam);
#ifdef OLD_MSC
  using NFmiPressText::WritePS;
#else
  using NFmiPressScaling::WritePS;
#endif
  bool WritePS(FmiPressOutputMode theOutput) override;
  NFmiString StationName();
  unsigned long ClassId() override;
  void SetStationNumberMode();

 protected:
  NFmiVoidPtrList* itsNewNames = nullptr;
  NFmiPressParam* itsPressParam = nullptr;

 private:
  bool fStationNumberMode = false;

};  // class NFmiPressStationText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressStationText::NFmiPressStationText() = default;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theNames Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressStationText::SetStationNumberMode()
{
  fStationNumberMode = true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theNames Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressStationText::SetNewNames(NFmiVoidPtrList* theNames)
{
  itsNewNames = theNames;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param pressParam Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressStationText::SetPressParam(NFmiPressParam* pressParam)
{
  itsPressParam = pressParam;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline unsigned long NFmiPressStationText::ClassId()
{
  return kNFmiPressStationText;
}
#endif  // NFMIPRESSSTATIONTEXT_H

// ======================================================================
