// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressManager
 */
// ======================================================================
/*!
 * \class NFmiPressManager
 *
 * Manageroi lehtis‰‰tuotantoa ylemm‰lt‰ tasolta eli 
 * k‰skee NFmiPressProductia (xxx.pre-tiedostoja).
 * Mahdollistaa yksinkertaisten muutosten teon (esim aseman vaihto)
 * ja taas uuden eps-(tai muun) tiedoston kirjoittamisen
 *
 */
// ======================================================================

#ifndef NFMIPRESSMANAGER_H
#define NFMIPRESSMANAGER_H

#include "NFmiPressProduct.h"
#include "NFmiPressDescription.h"


//! Undocumented
enum NFmiPressManagerObjects
{
  dManWritePs = 3000,
  dManProduct,
  dManProductFormat,
  dManStation,
  dManStationAndImages,
  dManStationNewName,
  dManCloseLog,
  dManOpenLog,
  dManChangeTime,
  dManRelativeHours,
  dManAddHours,
  dManOutputMode,
  dManData,
  dManLanguage,
  dManNumberAddingToName,
  dManDeleteNumberAddingToName,
  dManActivateFirst,
  dManDeactivateFirst,
  dManMaskNumber
};


//! Undocumented
class _FMI_DLL NFmiPressManager : public NFmiPressDescription 
{

public:

  virtual ~NFmiPressManager(void);
  NFmiPressManager(void);
  
  virtual bool ReadDescription(NFmiString & retString);
  bool ReadDescriptionAndWrite(NFmiPressProduct & pressProduct,
							   FmiPressOutputMode theOutMode=kPostScript);   

  virtual int ConvertDefText(NFmiString & object);

}; // class NFmiPressManager

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline
NFmiPressManager::~NFmiPressManager(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressManager::NFmiPressManager(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressManager::ReadDescription(NFmiString & retString)
{
  return true;
}

#endif // NFMIPRESSMANAGER_H

// ======================================================================
