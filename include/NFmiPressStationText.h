// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressStationText
 */
// ======================================================================
/*!
 * \class NFmiPressStationText
 *
 * Kirjoittaa QD:n (current)asemannimen ps-oliona output-tiedostoon.
 * Nimi voidaan haluttaessa muuttaa, esim Kˆˆpenhamina->K:hamina.
 * Nimenmuutoslista tuodaan segmentista, joka lukee nimenvaihdot
 * m‰‰rityksest‰.
 *
 */
// ======================================================================

#ifndef NFMIPRESSSTATIONTEXT_H
#define NFMIPRESSSTATIONTEXT_H

#include "NFmiPressText.h"
#include "NFmiPressDataObject.h"


//! Undocumented
struct NFmiRenaming
{
  NFmiString originalName;
  NFmiString newName;
};

class NFmiPressParam;


//! Undocumented
class _FMI_DLL NFmiPressStationText : public NFmiPressText,
									  public NFmiPressDataObject
{
public:

  virtual ~NFmiPressStationText(void);
  NFmiPressStationText(void);

  void SetNewNames(NFmiVoidPtrList * theNames);
  bool SetNewName(const NFmiRenaming & theNames);
  void SetPressParam(NFmiPressParam * pressParam);
  virtual bool WritePS(FmiPressOutputMode theOutput);
  NFmiString StationName(void);
  virtual unsigned long ClassId(void);
		
protected:

  NFmiVoidPtrList * itsNewNames;
  NFmiPressParam * itsPressParam;

}; // class NFmiPressStationText


// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressStationText::NFmiPressStationText(void)
  : NFmiPressText()
  , NFmiPressDataObject()
  , itsNewNames(0)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theNames Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressStationText::SetNewNames(NFmiVoidPtrList * theNames)
{
  itsNewNames=theNames;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param pressParam Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressStationText::SetPressParam(NFmiPressParam * pressParam)
{
  itsPressParam=pressParam;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
unsigned long NFmiPressStationText::ClassId(void)
{
  return kNFmiPressStationText;
}

#endif // NFMIPRESSSTATIONTEXT_H

// ======================================================================

