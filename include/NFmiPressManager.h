// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressManager
 */
// ======================================================================
/*!
 * \class NFmiPressManager
 *
 * Manageroi lehtisäätuotantoa ylemmältä tasolta eli suorittaa
 * NFmiPressProduct-luokan WritePs-metodia toistuvasti muuttamalla aina
 * määrittelyssä annettua esim. aikaa tai paikkaa. Manageria käytetään
 * kun Managerit-hakemistosta löytyy samanniminen .prem-tiedosto
 * (=Manageri) kuin itse .pre-tiedosto.
 *
 */
// ======================================================================

#ifndef NFMIPRESSMANAGER_H
#define NFMIPRESSMANAGER_H

#include "NFmiPressDescription.h"
#include "NFmiPressProduct.h"

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
  dManActivateFirstPassive,
  dManDeactivateFirstActive,
  dManActivateFirstSegment,
  dManDeactivateFirstSegment,
  dManMaskNumber,
  dManPressMaskNumber
};

//! Undocumented
class NFmiPressManager : public NFmiPressTimeDescription
{
 public:
  virtual ~NFmiPressManager(void);
  NFmiPressManager(void);

  virtual bool ReadDescription(NFmiString& retString);
  bool ReadDescriptionAndWrite(NFmiPressProduct& pressProduct,
                               bool& theManagerReadFailed,
                               FmiPressOutputMode theOutMode = kPostScript);

  virtual int ConvertDefText(NFmiString& object);

 protected:
 private:
  bool PreProcessManager(const NFmiFileString& theInputFile);

};  // class NFmiPressManager

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiPressManager::~NFmiPressManager(void) {}
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressManager::NFmiPressManager(void) {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString
 */
// ----------------------------------------------------------------------

inline bool NFmiPressManager::ReadDescription(NFmiString& retString)
{
  return true;
}
#endif  // NFMIPRESSMANAGER_H

// ======================================================================
