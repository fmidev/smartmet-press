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

// press
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
#include "NFmiPreProcessor.h"
#include "NFmiVoidPtrList.h"
// system
#include <queue>

class NFmiLocationFinder;
class NFmiNameDay;
class NFmiFastQueryInfo;

//! Undocumented
enum NFmiPressProductObjects
{
  dProduct = 20,
  dOutDir,
  dMapFile,
  dDataFile,
  dMandatoryDataFile,
  dMapIsReduced,
  dPlottingView,
  dMapDefSize,
  dClippingRectangle,
  dBoundingBorder,
  dSubViews,
  dMargin,
  dGap,
  dPressParam,
  dSymbolPlaces,
  dPalette,
  dSubImage,
  dPageSize,
  dDate,
  dProductWithTimeStamp,

  dProductWithDataTimeStamp = 1020,
  dTimeStampFormat ,
  dProductSizeFactor,
  dProductPlaceMove,
  dRelativeHours,
  dChangeScandinavian,
  dNameDay,
  dProductNameFormat,
  dProductNameTimeFormat,
  dSecondProductNameTimeFormat,

  dOutputMode = 2020,
  dNumberAddingToName,
  dMagicSavePath,
  dMapArea,
  dMakeElementsAfterSegments,
  dMaskFile,
  dWeekdayDir,
  dAllDataFilesCritical
};


//! Undocumented
struct FmiPressSeasons
{
  bool wintertime;
  bool summer;
  bool pollen;
  bool snow;
  bool pollenOrSnow;
  int weekday;
  int dayAdvance;
};


//! Undocumented
class _FMI_DLL NFmiPressProduct : public NFmiPressTimeDescription
{

public:

  virtual ~NFmiPressProduct(void);
  NFmiPressProduct(void);

  bool WritePS(FmiPressOutputMode=kPostScript);

  void ReadPalette(void);
  virtual bool ReadDescriptionFile(NFmiString inputFileName);
  virtual bool ReadDescription(NFmiString & retString);
  int ConvertDefText(NFmiString & object);
  bool SetSegmentData(const NFmiString & theDataName);
  bool PreProcessDefinition(const std::string & inFileName,
							const std::string & outFileName);
  bool PreProcessProduct(std::ifstream & origInput, std::ofstream & output);
  bool SetAllLanguages(FmiLanguage theLanguage);
  NFmiStationPoint FirstParamLocation();
  NFmiQueryData * DataByName(NFmiString givenName);
  NFmiQueryData * FirstData(void);
  NFmiString FirstDataName(void);
  NFmiString FirstSegmentName(void);
  bool SetMaskNumber(unsigned long theNumber, FmiEnumSpace theEnumSpace); 
  bool SetFirstStation(const NFmiLocation & theLocation);
  bool SetStationRename(const NFmiRenaming & theRename);
  bool SetImagePreNames(const NFmiLocation & theLocation);
  bool SetProductName(const NFmiString & theName);
  bool SetProductNameFormat(const NFmiString & theName);
  bool SetLogFile(bool OnOff);
  bool ReadData(void);
  bool DefinePar(const NFmiString & theString);
  bool ReplacePar(NFmiString & theString);
  bool ReadNameToLonLatList(void); 
  bool Close(void); 
  NFmiString GetInFileName(void) const;
  bool SetAllTimes(const NFmiMetTime & theTime);
  void ActivateNumberToName(long theStartNumber);
  void DeActivateNumberToName(void);
  bool SetFirstObjectActivity(bool theActivity);
  void PutInStorage(const float value, int queueNum=1);
  float UseFromStorage(int queueNum=1, bool errorReport = true);
  bool GetElementsAfterSegments(void) const;
 protected:

  void StepMap(void);
  void Scale(void);

 private:

  bool ReadQueryData(NFmiQueryData * theQD, char * fileName);
  bool ConstructOutFileName(void);
  bool WriteScalingObjects(bool doPreSegments, FmiPressOutputMode theOutput);
  bool WriteSameSymbols(bool doPreSegments, FmiPressOutputMode theOutput);
  bool WriteMetaInit(void);
  bool GetSeasonsStatus(FmiPressSeasons& theSeasonsStatus);
  bool WeekdayDirectiveActive(const std::string & theDefinition,
							  const FmiPressSeasons & theSeasons) const;
  bool PreProcessConditionally(NFmiPreProcessor & thePrepr,
							   bool theCondValue,
							   const std::string & theConditionalBeginDirective,
							   const std::string & theConditionalNotBeginDirective,
							   const std::string & theConditionalEndDirective,
							   const std::string & theConditionalElseDirective);
public:

  NFmiLocationFinder * itsNameToLonLat;
  std::queue<float> itsFloatQueue;
  std::queue<float> itsFloatQueue2;

private:

  bool fMakeElementsAfterSegments;
  NFmiNameDay * itsNameDay;
  bool fChangeScandinavian;
  bool fDataRead;
  FmiPageSize itsPageSize;
  NFmiString itsPaletteName;
  NFmiCmykColorBag * itsPalette;
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
  NFmiFastQueryInfo * itsCurrentDataIter;
  NFmiSuperSmartInfo * itsMaskIter;
  NFmiVoidPtrList itsDatas;
  NFmiVoidPtrList itsParams;
  NFmiVoidPtrList itsSameSymbols;
  NFmiVoidPtrList itsObjects;
  NFmiString itsLogFileName;
  unsigned long itsNameTimeFormat;
  unsigned long itsSecondNameTimeFormat;
  FmiPressOutputMode itsOutputMode;
  long itsCurrentNumberToName;
  unsigned long itsNumOfWritePS;
  NFmiPressArea itsArea;

}; // class NFmiPressProduct

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressProduct::GetElementsAfterSegments(void) const
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

inline
NFmiString NFmiPressProduct::GetInFileName(void) const
{
  return itsInFileName;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStartNumber Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressProduct::ActivateNumberToName(long theStartNumber)
{
  itsCurrentNumberToName = theStartNumber;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressProduct::DeActivateNumberToName(void)
{
  itsCurrentNumberToName = kLongMissing;
}

#endif // NFMIPRESSPRODUCT_H

// ======================================================================

