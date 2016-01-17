// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressProduct
 */
// ======================================================================
/*!
 * \class NFmiPressProduct
 *
 * Tämä on lehtisääohjelman pääluokka. Segmentit (NFmiPressParam)
 * ja monet muut oliot ovat tämän jäseniä. Määrittelytiedoston
 * lukeminen ja eps-tiedoston kirjoitus aloitetaan täältä
 * kontrollin siirtyessä jäsenille mahd. useammankin tason kautta.
 * Yleiset asiat kuten pohjat, tiedostonimet, data hoidetaan
 * täällä (Windowsin ympäristömuuttujat luetaan). Määrittelytiedosto
 * preprosessoidaan ennen käsittelyä (kommentit pois, includet mukaan,
 * direktiivikäsittely). Managerikäytössä löytyy metodit erilaisiin
 * (pikku) muutoksiin.
 *
 */
// ======================================================================

#ifndef NFMIPRESSPRODUCT_H
#define NFMIPRESSPRODUCT_H

// namespace fs = boost::filesystem;
// using namespace boost;
// namespace boost

// press
#include "NFmiValueCorrection.h"
#include "NFmiCmykColorBag.h"
#include "NFmiHyphenationString.h"
#include "NFmiNamedQueryData.h"
#include "NFmiPressArea.h"
#include "NFmiPressImage.h"
#include "NFmiPressParam.h"
#include "NFmiPressSameSymbols.h"
#include "NFmiPressStationText.h"
#include "NFmiPressTimeDescription.h"
#include "NFmiRectScale.h"
// newbase
#include "NFmiFileString.h"
#include "NFmiVoidPtrList.h"
// system
#include <boost/shared_ptr.hpp>
#include <queue>
#include <vector>
#include <map>
#include <locale>

class NFmiLocationFinder;
class NFmiNameDay;
class NFmiFastQueryInfo;

//! Undocumented
enum NFmiPressProductObjects
{
  dProduct = 20,
  dEncoding,
  dOutDir,
  dMapFile,
  dDataFile,
  dYearDataFile,
  dNewestDataFile,
  dDataFileTimeStamp,
  dMandatoryDataFile,
  dMapIsReduced,
  dPlottingView,
  dMapDefSize,
  dClippingRectangle,
  dBoundingBorder,
  dUniBoundingBorder,
  dSubViews,
  dMargin,
  dGap,
  dPressParam,
  dSymbolPlaces,
  dPalette,
  dSubImage = 1020,
  dPageSize,
  dDate,
  dProductWithTimeStamp,
  dProductWithPressTimeStamp,

  dProductWithDataTimeStamp,
  dTimeStampFormat,
  dProductSizeFactor,
  dProductPlaceMove,
  dRelativeHours,
  dChangeScandinavian = 2020,
  dNameDay,
  dProductNameFormat,
  dProductNameTimeFormat,
  dSecondProductNameTimeFormat,

  dOutputMode,
  dNumberAddingToName,
  dMagicSavePath,
  dMapArea,
  dMakeElementsAfterSegments,
  dMaskFile = 3020,
  dWeekdayDir,
  dAllDataFilesCritical,
  dDataFilesNotCritical,
  dCVversion
};

//! Undocumented
struct FmiElementStatus
{
  bool symbol;
  bool number;
  bool text;
};
/*
struct FmiSubstituteMappingValue
{
  float oldValue;
  float newValue;
};
*/

//! Undocumented
class _FMI_DLL NFmiPressProduct : public NFmiPressTimeDescription
{
 public:
  virtual ~NFmiPressProduct(void);
  NFmiPressProduct(void);

  bool WritePS(FmiPressOutputMode = kPostScript);

  void ReadPalette(void);
  virtual bool ReadDescriptionFile(NFmiString inputFileName);
  virtual bool ReadDescription(NFmiString& retString);
  int ConvertDefText(NFmiString& object);
  bool SetSegmentData(const NFmiString& theDataName);
  bool PreProcessProduct(std::ifstream& origInput, std::ofstream& output);
  bool SetAllLanguages(FmiLanguage theLanguage);
  NFmiStationPoint FirstParamLocation();
  NFmiQueryData* DataByName(NFmiString givenName, bool& isYearData);
  NFmiQueryData* FirstData(void);
  NFmiNamedQueryData* FirstNamedData(void);
  NFmiString FirstDataName(void);
  NFmiString FirstSegmentName(void);
  bool SetMaskNumber(unsigned long theNumber, FmiEnumSpace theEnumSpace);
  bool SetFirstStation(const NFmiLocation& theLocation);
  bool SetStationRename(const NFmiRenaming& theRename);
  bool SetImagePreNames(const NFmiLocation& theLocation);
  bool SetProductName(const NFmiString& theName);
  bool SetProductNameFormat(const NFmiString& theName);
  bool SetLogFile(bool OnOff);
  bool ReadData(void);
  bool DefinePar(const NFmiString& theString);
  bool ReplacePar(NFmiString& theString);
  bool ReadNameToLonLatList(void);
  bool FindLonLatFromList(NFmiString& theStationName, NFmiPoint& theLonLat);
  unsigned long FindWmoFromList(const NFmiString& theStationName);
  bool Close(void);
  NFmiString GetInFileName(void) const;
  bool SetAllTimes(const NFmiMetTime& theTime);
  void ActivateNumberToName(long theStartNumber);
  void DeActivateNumberToName(void);
  bool SetFirstObjectActivity(bool theActivity);
  bool ChangeFirstPossibleObject(bool toNewActivity);
  bool SetFirstSegmentActivity(bool theActivity);
  void PutInStorage(const float value, int queueNum = 1);
  float UseFromStorage(int queueNum = 1, bool errorReport = true);
  bool GetElementsAfterSegments(void) const;
  bool ReadSeasonsStatus(void);
  bool SetSegmentTimeStatus(int theSegmentNum, bool theStatus);
  bool GetSegmentTimeStatus(int theSegmentNum) const;
  void SetSupplementMode(bool theMode);
  bool GetSupplementMode(void);
  bool AddSubstituteMappingValue(float theOldValue, float theNewValue);
  NFmiString CutOffString(NFmiString& theString, int toMaxChars) const;

 protected:
  void StepMap(void);
  void Scale(void);

 private:
  void SetEncoding(std::string theEncoding);
  bool ReadQueryData(NFmiQueryData* theQD, char* fileName);
  bool ConstructOutFileName(void);
  bool WriteScalingObjects(bool doPreSegments, FmiPressOutputMode theOutput);
  bool WriteSameSymbols(bool doPreSegments, FmiPressOutputMode theOutput);
  bool WriteMetaInit(void);

 public:
  NFmiLocationFinder* itsNameToLonLat;
  std::queue<float> itsFloatQueue;
  std::queue<float> itsFloatQueue2;
  NFmiVoidPtrList itsObjects;  // osakuvat voivat tuoda rekursiivisesti lisäkuvia
  bool IsSummerWeather(const NFmiString& theCountryPart);
  std::vector<bool> itsSegmentDoneTimes;
  void SetNameDay(NFmiNameDay* nameDay, FmiLanguage = kFinnish);
  NFmiNameDay* GetNameDay(FmiLanguage = kFinnish) const;
  NFmiArea* itsMainArea;  // jonkun PressArean pressProduktissa tai Paramissa, joka omistaa

  bool LastSymbolStatus(void) const;
  bool LastNumberStatus(void) const;
  bool LastTextStatus(void) const;
  void SetLastSymbolStatus(bool theBool);
  void SetLastNumberStatus(bool theBool);
  void SetLastTextStatus(bool theBool);
  std::map<float, float> itsSubstituteMappingValues;
  FmiPageSize GetPageSize(void) const;
  NFmiValueCorrection* GetTempCorrection(void) const;

 private:
  bool fMakeElementsAfterSegments;
  NFmiNameDay* itsNameDayFi;
  NFmiNameDay* itsNameDaySw;
  bool fChangeScandinavian;
  bool fDataRead;
  FmiPageSize itsPageSize;
  NFmiString itsPaletteName;
  NFmiCmykColorBag* itsPalette;
  unsigned long itsProducer;
  unsigned short itsHourStep;
  NFmiString itsPath;
  NFmiVoidPtrList itsEpsFileNames;
  NFmiString itsDataFileName;
  NFmiString itsMaskFileName;
  NFmiString itsMagicSavePath;
  NFmiString itsOutDir;
  NFmiHyphenationString itsProductNameFormat;
  bool fTimeStamp;
  bool fDataTimeStamp;
  NFmiFileString itsOutFile;
  NFmiString itsOutFileName;
  std::ofstream outFile;
  NFmiString itsInFileName;
  bool fMapIsEps;
  double itsMapGap;
  double itsMargin;
  FmiCounter itsNumberOfMaps;
  FmiCounter itsCurrentMap;
  NFmiRectScale itsScale;
  NFmiRect itsPlottingRect;
  NFmiRect itsClippingRect;
  NFmiRect itsBoundingBorder;
  NFmiFastQueryInfo* itsCurrentDataIter;
  boost::shared_ptr<NFmiSuperSmartInfo> itsMaskIter;
  NFmiVoidPtrList itsDatas;
  NFmiVoidPtrList itsParams;
  NFmiVoidPtrList itsSameSymbols;
  NFmiString itsLogFileName;
  unsigned long itsNameTimeFormat;
  unsigned long itsSecondNameTimeFormat;
  FmiPressOutputMode itsOutputMode;
  long itsCurrentNumberToName;
  unsigned long itsNumOfWritePS;
  NFmiPressArea itsArea;
  bool fNewestDataMode;
  bool fSupplementMode;
  FmiElementStatus itsLastElementStatus;
  NFmiValueCorrection* itsTempCorrection;
};  // class NFmiPressProduct

// ----------------------------------------------------------------------
/*!
 * Sets the encoding of the parameter in case it is needed as text
 *
 * \param theEncoding The given encoding
 */
// ----------------------------------------------------------------------
inline void NFmiPressProduct::SetEncoding(std::string theEncoding)
{
  itsEnvironment.SetEncoding(theEncoding);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiValueCorrection* NFmiPressProduct::GetTempCorrection(void) const
{
  return itsTempCorrection;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiPageSize NFmiPressProduct::GetPageSize(void) const { return itsPageSize; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressProduct::SetLastSymbolStatus(bool theBool)
{
  itsLastElementStatus.symbol = theBool;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressProduct::SetLastNumberStatus(bool theBool)
{
  itsLastElementStatus.number = theBool;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressProduct::SetLastTextStatus(bool theBool)
{
  itsLastElementStatus.text = theBool;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressProduct::LastSymbolStatus(void) const { return itsLastElementStatus.symbol; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressProduct::LastNumberStatus(void) const { return itsLastElementStatus.number; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressProduct::LastTextStatus(void) const { return itsLastElementStatus.text; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressProduct::SetSupplementMode(bool theMode) { fSupplementMode = theMode; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressProduct::GetSupplementMode(void) { return fSupplementMode; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressProduct::GetElementsAfterSegments(void) const
{
  return fMakeElementsAfterSegments;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiNameDay* NFmiPressProduct::GetNameDay(FmiLanguage theLanguage) const
{
  if (theLanguage == kFinnish)
    return itsNameDayFi;
  else
    return itsNameDaySw;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressProduct::SetNameDay(NFmiNameDay* nameDay, FmiLanguage theLanguage)
{
  if (theLanguage == kFinnish)
    itsNameDayFi = nameDay;
  else
    itsNameDaySw = nameDay;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPressProduct::GetInFileName(void) const { return itsInFileName; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStartNumber Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressProduct::ActivateNumberToName(long theStartNumber)
{
  itsCurrentNumberToName = theStartNumber;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressProduct::DeActivateNumberToName(void)
{
  itsCurrentNumberToName = kLongMissing;
}

#endif  // NFMIPRESSPRODUCT_H

// ======================================================================
