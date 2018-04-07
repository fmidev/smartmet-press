// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressParam
 */
// ======================================================================
/*!
 * \class NFmiPressParam
 *
 * T‰m‰ luokka vastaa m‰‰rittelyn #Segmentti:‰. Luokka hoitaa
 * asema- ja aikasilmukat. J‰senin‰ on sek‰ datariippuvia
 * (NFmiSymbolGroupin kautta) ett‰ muita olioita.
 *
 */
// ======================================================================

#ifndef NFMIPRESSPARAM_H
#define NFMIPRESSPARAM_H

// press
//#include "NFmiPressProduct.h"
#include "NFmiPressArea.h"
#include "NFmiPressStationText.h"
#include "NFmiPressTimeDescription.h"
#include "NFmiPsWriting.h"
#include "NFmiSymbolGroup.h"
// newbase
#include "NFmiFastQueryInfo.h"
#include "NFmiFileString.h"
#include "NFmiInfoAreaMask.h"
#include "NFmiQueryData.h"
#include "NFmiRectScale.h"
//#include "NFmiText.h"

#include <boost/shared_ptr.hpp>

//#include <vector>
#include <list>

class NFmiPressProduct;
class NFmiArea;

//! Undocumented
enum NFmiPressParamObjects
{
  dSymbolGroup = 40,
  dStation,
  dRisuStation,
  dStationDefArea,
  dStationPlotArea,
  dStationRelocation,
  dParamSubViews,
  dParamStepSize,
  dStationTable,
  dTimeTable,
  dTimeTableRelative = 1040,
  dSegmentDataFile,
  dSegmentPrimaryDataFile,
  dStationNameReplace,
  dMultiParams,
  dMultiMapping,
  dGridMode,
  dStationsLocalTime,
  dLevelTable,
  dSegmentLevel,
  dLevelPlace,
  dTimePlace = 2040,
  dTimePlaceRelative,
  dLogText,
  dLogTextAdd,
  dStationLonLatOnMap,
  dStationNameOnMap,
  dStationPlaceOnMap,
  dStationPlaceMatrixOnMap,
  dSegmentMapArea,
  dMainArea,
  dCoordinatesFromMainMap = 3040,
  dStationsFromData,
  dStationsFromDataCropping,
  dDataNotNeeded,
  dAreaOperation,
  dDistanceCheck,
  dSupplement,
  dSegmentNameDay,
  dBackupStation,
  dDataCoordinatesMoved,
  dStationNamesAfterParams = 5040,
  dDayChangeText,
  dOptimizeGlobalObs,
  dSegmentHybridLevel,
  dHybridLevelPlace,
  dAlternatingSizeFactor
};

struct FmiValuePoint
{
  long value;
  NFmiPoint point;
};
struct FmiMaxMinPoint
{
  float value;
  bool isMax;
  unsigned long index;
  NFmiPoint point;
  float significance;
};

//! Undocumented
const unsigned short kMaxNumOfTableElements = 25;

//! Undocumented
class _FMI_DLL NFmiPressParam : public NFmiPressTimeDescription
{
 public:
  virtual ~NFmiPressParam(void);

  NFmiPressParam(void);

  NFmiPressParam(const NFmiRectScale& scale,
                 const short firstDeltaDays,
                 const short firstPlotHours,
                 const short firstPlotMinutes,
                 const short hourStep,
                 const unsigned long producer,
                 NFmiPressProduct* pressProduct);

  bool WritePS(NFmiRectScale theScale, std::ofstream& theFile, FmiPressOutputMode theOutput);

  bool SetData(const NFmiString& dataName);
  bool SetPrimaryData(const NFmiString& dataName);
  void SetStationScale(const NFmiRectScale& scale);
  void SetScale(const NFmiRectScale& scale);
  virtual bool ReadDescription(NFmiString& retString);
  int ConvertDefText(NFmiString& object);
  void SetData(NFmiQueryData* data);
  void SetPrimaryData(NFmiQueryData* data);
  void SetMaskIter(boost::shared_ptr<NFmiFastQueryInfo> info);
  boost::shared_ptr<NFmiFastQueryInfo> GetMaskIter(void);
  NFmiInfoAreaMask* GetAreaMask(void);
  void SetDataName(NFmiString name);
  void SetPrimaryDataName(NFmiString name);
  void SetLanguage(FmiLanguage theLanguage);
  FmiLanguage GetLanguage(void);
  NFmiString GetName(void);
  NFmiPoint GetFirstPoint(void);
  NFmiStation GetCurrentStation(void) const;
  NFmiStationPoint GetFirstStationPoint(void);
  bool SetFirstStation(const NFmiLocation& theLocation);
  bool SetStationRename(const NFmiRenaming& theRenaming);
  bool SetAllTimes(const NFmiMetTime& theTime);
  bool SetAllLanguages(FmiLanguage theLanguage);

  bool IsStationLocalTimeMode(void) const;
  bool IsFirstStation(void) const;
  void SetErrorReported(unsigned short ind);
  bool GetErrorReported(unsigned short ind);
  NFmiString MakeLogComment(void) const;
  void SetNewGeoArea(const NFmiPressArea& theArea);
  NFmiTime GetOptionTime(void) const;
  void SetOptionTime(NFmiTime theTime);
  NFmiLocation* GetOptionLocation(void);
  void SetOptionLocation(NFmiLocation theLocation);
  void SetEncoding(std::string theEncoding);
  void DeleteOptionLocation(void);
  bool IsAreaOperation(void);
  NFmiArea* GetGeoArea(void);
  bool ChangeMaskNumber(unsigned long theMask, FmiEnumSpace theEnumSpace);
  void PutInStorage(const float value, int storageQueue);
  float UseFromStorage(int storageQueue);
  FmiCounter GetCurrentStep(void);
  bool IsActive(void) const;
  void SetActivity(bool theActivity);
  bool IsDistanceCheck(void) const;
  bool CheckAndSetDistance(long theValue, const NFmiPoint& point);
  bool SetMaxMinPoints(void);
  bool IsMaxMin(bool& theIsMax, NFmiPoint& theCorrectedPoint);
  bool InterruptSymbolGroup(void) const;
  unsigned long GetCurrentStationStep(void) const;
  NFmiPressProduct* GetPressProduct(void) const;
  bool IsSupplementary(void) const;
  bool SetSegmentCurrentTimeStatus(bool theStatus);
  void SetYearData(bool theStatus);
  bool IsBackupStation(void) const;
  bool SetStationNotNeeded(void);
  NFmiPoint GetCurrentUnscaledPoint(void) const;
  void SetCurrentStationLonLat(NFmiPoint& theLonLat);
  void SetStationNamesAfterParams(bool theStatus);
  bool IsStationNamesAfterParams(void) const;
  NFmiFastQueryInfo* GetPrimaryDataIter(void) const;
  unsigned long GetPrimaryDataNum(void) const;
  void AddPrimaryDataNum(void);
  bool PrimaryDataOk(void) const;
  bool HasPrimaryData(void) const;
  bool AddNumberOfBackupTimeUsed(void);
  void SetDayChanged(void);
  bool IsDayChanged(void) const;
  bool IsOptimizeGlobalObs(void) const;
  void SetBackupDayReported(void);
  bool IsBackupDayReported(void) const;
  NFmiStationPoint* GetCurrentStationPoint(void) const;
  void SetReportProseccedNumber(bool theValue);

 protected:
  void UnsetAllErrorReported(void);
  void NextStation(void);
  bool SetLonLat(NFmiStationPoint& theStation);
  void SetDistanceCheck(const NFmiPoint& theDistances);

 private:
  void StepTimeDependent(long theHourStep);
  bool DoTimeDependent(bool isLastLoop,
                       FmiCounter theCurrentStepInd,
                       ofstream& theFile,
                       FmiPressOutputMode theOutput,
                       NFmiPressScaling*& theSaveObject);
  bool FindQDStationName(const NFmiStationPoint& theStation);
  bool CreateAreaMask(void);
  bool CreateStationFromAreaMask(void);

  bool fActivity;
  bool fStationNotNeeded;
  bool fErrorReported[4];
  bool fIsFirstStation;
  bool fStationsAreLocalTime;
  bool fGridMode;
  NFmiStation itsCurrentStation;
  NFmiString itsName;
  bool fLonLatMode;
  NFmiPressProduct* itsPressProduct;
  unsigned long itsProducer;
  short itsHourStep;
  NFmiString itsDataName;
  NFmiString itsPrimaryDataName;
  NFmiFastQueryInfo* itsDataIter;
  NFmiFastQueryInfo* itsPrimaryDataIter;
  boost::shared_ptr<NFmiFastQueryInfo> itsMaskIter;
  NFmiInfoAreaMask* itsAreaMask;
  FmiCounter itsCurrentStep;
  FmiCounter itsNumberOfSteps;
  unsigned long itsCurrentStationIndex;
  NFmiLevel itsLevels[kMaxNumOfTableElements];
  NFmiPoint itsSteps[kMaxNumOfTableElements];
  NFmiPoint itsUnscaledSteps[kMaxNumOfTableElements];
  long itsTimeSteps[kMaxNumOfTableElements];
  bool fIsTimeLoop;
  bool fIsLevelLoop;
  bool fIsPureRegTimeLoop;
  NFmiRectScale itsCurrentStationScale;
  NFmiRectScale itsScale;
  NFmiLocationBag itsStations;
  NFmiVoidPtrList itsTimeDepObjects;
  NFmiVoidPtrList itsStationDepObjects;
  NFmiSymbolGroup itsSymbols;
  bool fTwoPartLogComment;
  NFmiString itsLogComment;
  NFmiPressArea itsArea;
  bool fIsAreaOperation;
  NFmiTime itsOptionTime;           // max/min-olio tuo t‰nne NFmiExtremeTimeParamRectille
  NFmiLocation* itsOptionLocation;  // max/min-olio tuo t‰nne NFmiExtremePlaceParamRectille
  NFmiPoint itsCheckDistance;
  vector<FmiValuePoint> itsCheckLocations;
  list<FmiMaxMinPoint> itsMaxMinLocations;
  bool fInterruptSymbolGroup;
  bool fSupplementary;
  bool fCoordinatesFromMainMap;
  bool fYearData;
  bool fCurrentStationBackup;
  NFmiPoint itsCurrentUnscaledPoint;
  bool fMaxMinSearched;
  bool fDataCoordinatesMoved;
  bool fStationNamesAfterParams;
  unsigned long itsPrimaryDataNum;
  bool fPrimaryDataOk;
  NFmiPressText* itsDayChangeText;
  bool fDayChanged;
  bool fOptimizeGlobalObs;
  bool fBackupDayReported;
  NFmiStationPoint* itsCurrentStationPoint;
  bool fReportProseccedNumber;
};
// ----------------------------------------------------------------------
/*!
 * Destructor
 *
 * \todo According to the standard the ifs are not necessary, delete 0 is safe
 */
// ----------------------------------------------------------------------

inline NFmiPressParam::~NFmiPressParam(void)
{
  itsStations.Destroy();
  itsTimeDepObjects.Clear(isTrue);
  itsStationDepObjects.Clear(isTrue);
  if (itsDataIter) delete itsDataIter;
  if (itsPrimaryDataIter) delete itsDataIter;
  if (itsAreaMask) delete itsAreaMask;
  if (itsOptionLocation) delete itsOptionLocation;
  if (itsDayChangeText) delete itsDayChangeText;
  if (itsCurrentStationPoint) delete itsCurrentStationPoint;
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressParam::NFmiPressParam(void)
    : NFmiPressTimeDescription(),
      fActivity(true),
      fStationNotNeeded(false),
      fStationsAreLocalTime(0),
      fGridMode(false),
      itsPressProduct(0),
      itsHourStep(24),
      itsDataIter(0),
      itsPrimaryDataIter(0),
      itsAreaMask(0),
      itsNumberOfSteps(1),
      fIsPureRegTimeLoop(true),
      fIsAreaOperation(false),
      itsOptionLocation(0),
      itsCheckDistance(NFmiPoint()),
      fInterruptSymbolGroup(false),
      fSupplementary(false),
      fCoordinatesFromMainMap(false),
      fYearData(false),
      fCurrentStationBackup(false),
      fMaxMinSearched(false),
      fDataCoordinatesMoved(false),
      fStationNamesAfterParams(false),
      itsPrimaryDataNum(0),
      fPrimaryDataOk(true),
      itsDayChangeText(0),
      fDayChanged(false),
      fOptimizeGlobalObs(false),
      fBackupDayReported(false),
      itsCurrentStationPoint(0),
      fReportProseccedNumber(true)
{
  itsLanguage = kFinnish;
  itsOptionTime.SetMissing();
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param scale Undocumented
 * \param firstDeltaDays Undocumented
 * \param firstPlotHours Undocumented
 * \param firstPlotMinutes Undocumented
 * \param hourStep Undocumented
 * \param producer Undocumented
 * \param pressProduct Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressParam::NFmiPressParam(const NFmiRectScale& scale,
                                      const short firstDeltaDays,
                                      const short firstPlotHours,
                                      const short firstPlotMinutes,
                                      const short hourStep,
                                      const unsigned long producer,
                                      NFmiPressProduct* pressProduct)
    : NFmiPressTimeDescription(firstDeltaDays, firstPlotHours, firstPlotMinutes),
      fActivity(true),
      fStationNotNeeded(false),
      fStationsAreLocalTime(0),
      fGridMode(false),
      itsPressProduct(pressProduct),
      itsProducer(producer),
      itsHourStep(hourStep),
      itsDataIter(0),
      itsPrimaryDataIter(0),
      itsAreaMask(0),
      itsNumberOfSteps(1),
      fIsPureRegTimeLoop(true),
      itsScale(scale),
      fIsAreaOperation(false),
      itsOptionLocation(0),
      itsCheckDistance(NFmiPoint()),
      fInterruptSymbolGroup(false),
      fSupplementary(false),
      fCoordinatesFromMainMap(false),
      fYearData(false),
      fCurrentStationBackup(false),
      fMaxMinSearched(false),
      fDataCoordinatesMoved(false),
      fStationNamesAfterParams(false),
      itsPrimaryDataNum(0),
      fPrimaryDataOk(true),
      itsDayChangeText(0),
      fDayChanged(false),
      fOptimizeGlobalObs(false),
      fBackupDayReported(false),
      itsCurrentStationPoint(0),
      fReportProseccedNumber(true)
{
  itsLanguage = kFinnish;
  itsOptionTime.SetMissing();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline NFmiStationPoint* NFmiPressParam::GetCurrentStationPoint(void) const
{
  return itsCurrentStationPoint;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressParam::SetReportProseccedNumber(bool theValue)
{
  fReportProseccedNumber = theValue;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressParam::SetBackupDayReported(void) { fBackupDayReported = true; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::IsBackupDayReported(void) const { return fBackupDayReported; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::IsOptimizeGlobalObs(void) const { return fOptimizeGlobalObs; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::IsDayChanged(void) const { return fDayChanged; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressParam::SetDayChanged(void) { fDayChanged = true; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::PrimaryDataOk(void) const { return fPrimaryDataOk; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline unsigned long NFmiPressParam::GetPrimaryDataNum(void) const { return itsPrimaryDataNum; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressParam::AddPrimaryDataNum(void) { itsPrimaryDataNum++; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiFastQueryInfo* NFmiPressParam::GetPrimaryDataIter(void) const
{
  return itsPrimaryDataIter;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetStationNamesAfterParams(bool theStatus)
{
  fStationNamesAfterParams = theStatus;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsStationNamesAfterParams(void) const
{
  return fStationNamesAfterParams;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPoint NFmiPressParam::GetCurrentUnscaledPoint(void) const
{
  return itsCurrentUnscaledPoint;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::SetStationNotNeeded(void)
{
  fStationNotNeeded = true;
  return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsBackupStation(void) const { return fCurrentStationBackup; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsSupplementary(void) const { return fSupplementary; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressProduct* NFmiPressParam::GetPressProduct(void) const { return itsPressProduct; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::InterruptSymbolGroup(void) const { return fInterruptSymbolGroup; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetDistanceCheck(const NFmiPoint& theDistances)
{
  itsCheckDistance = theDistances;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsDistanceCheck(void) const
{
  return itsCheckDistance.X() > 0. || itsCheckDistance.Y() > 0.;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsActive(void) const { return fActivity; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theActivity Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetActivity(bool theActivity) { fActivity = theActivity; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param data Undocumented
 * \todo Siivoa sijoitus selke‰mm‰ksi, ei temppuilla liikaa
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetData(NFmiQueryData* data)
{
  data ? itsDataIter = new NFmiSuperSmartInfo(data) : 0;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param data Undocumented
 * \todo Siivoa sijoitus selke‰mm‰ksi, ei temppuilla liikaa
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetPrimaryData(NFmiQueryData* data)
{
  data ? itsPrimaryDataIter = new NFmiSuperSmartInfo(data) : 0;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param info Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetMaskIter(boost::shared_ptr<NFmiFastQueryInfo> info)
{
  itsMaskIter = info;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline boost::shared_ptr<NFmiFastQueryInfo> NFmiPressParam::GetMaskIter(void)
{
  return itsMaskIter;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiInfoAreaMask* NFmiPressParam::GetAreaMask(void) { return itsAreaMask; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param name Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetDataName(NFmiString name) { itsDataName = name; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param name Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetPrimaryDataName(NFmiString name) { itsPrimaryDataName = name; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param name Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::HasPrimaryData(void) const
{
  return itsPrimaryDataIter == 0 ? false : true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetLanguage(FmiLanguage theLanguage) { itsLanguage = theLanguage; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiLanguage NFmiPressParam::GetLanguage(void) { return itsLanguage; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPressParam::GetName(void) { return itsName; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiStation NFmiPressParam::GetCurrentStation(void) const { return itsCurrentStation; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsStationLocalTimeMode(void) const { return fStationsAreLocalTime; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetYearData(bool theStatus) { fYearData = theStatus; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsFirstStation(void) const { return fIsFirstStation; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param ind Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetErrorReported(unsigned short ind) { fErrorReported[ind - 1] = true; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param ind Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::GetErrorReported(unsigned short ind) { return fErrorReported[ind - 1]; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiTime NFmiPressParam::GetOptionTime(void) const { return itsOptionTime; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetOptionTime(NFmiTime theTime) { itsOptionTime = theTime; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiLocation* NFmiPressParam::GetOptionLocation(void) { return itsOptionLocation; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLocation Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetOptionLocation(NFmiLocation theLocation)
{
  itsOptionLocation = new NFmiLocation(theLocation);
}

inline void NFmiPressParam::DeleteOptionLocation(void)
{
  if (itsOptionLocation) delete itsOptionLocation;
  itsOptionLocation = 0;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsAreaOperation(void) { return fIsAreaOperation; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiArea* NFmiPressParam::GetGeoArea(void) { return itsArea.GetArea(); }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiCounter NFmiPressParam::GetCurrentStep(void) { return itsCurrentStep; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::UnsetAllErrorReported(void)
{
  fErrorReported[0] = false;
  fErrorReported[1] = false;
  fErrorReported[2] = false;
  fErrorReported[3] = false;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param scale Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressParam::SetCurrentStationLonLat(NFmiPoint& theLonLat)
{
  itsCurrentStation.SetLatitude(theLonLat.Y());
  itsCurrentStation.SetLongitude(theLonLat.X());
}

#endif  // NFMIPRESSPARAM_H

// ======================================================================
