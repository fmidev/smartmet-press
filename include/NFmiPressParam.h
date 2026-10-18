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

#include <memory>

//#include <vector>
#include <list>
#include <utility>

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
  long value = 0;
  NFmiPoint point;
};
struct FmiMaxMinPoint
{
  float value = 0.0F;
  bool isMax = false;
  unsigned long index = 0;
  NFmiPoint point;
  float significance = 0.0F;
};

//! Undocumented
const unsigned short kMaxNumOfTableElements = 25;

//! Undocumented
class NFmiPressParam : public NFmiPressTimeDescription
{
 public:
  ~NFmiPressParam() override;

  NFmiPressParam();

  NFmiPressParam(const NFmiRectScale& scale,
                 short firstDeltaDays,
                 short firstPlotHours,
                 short firstPlotMinutes,
                 short hourStep,
                 unsigned long producer,
                 NFmiPressProduct* pressProduct);
  NFmiPressParam(const NFmiPressParam&) = delete;
  NFmiPressParam& operator=(const NFmiPressParam&) = delete;

  bool WritePS(const NFmiRectScale& theScale, std::ofstream& theFile, FmiPressOutputMode theOutput);

  bool SetData(const NFmiString& dataName);
  bool SetPrimaryData(const NFmiString& dataName);
  void SetStationScale(const NFmiRectScale& scale);
  void SetScale(const NFmiRectScale& scale);
  bool ReadDescription(NFmiString& retString) override;
  int ConvertDefText(NFmiString& object) override;
  void SetData(NFmiQueryData* data);
  void SetPrimaryData(NFmiQueryData* data);
  void SetMaskIter(std::shared_ptr<NFmiFastQueryInfo> info);
  std::shared_ptr<NFmiFastQueryInfo> GetMaskIter();
  NFmiInfoAreaMask* GetAreaMask();
  void SetDataName(const NFmiString& name);
  void SetPrimaryDataName(const NFmiString& name);
  void SetLanguage(FmiLanguage theLanguage);
  FmiLanguage GetLanguage();
  NFmiString GetName();
  NFmiPoint GetFirstPoint();
  NFmiStation GetCurrentStation() const;
  NFmiStationPoint GetFirstStationPoint();
  bool SetFirstStation(const NFmiLocation& theLocation);
  bool SetStationRename(const NFmiRenaming& theRenaming);
  bool SetAllTimes(const NFmiMetTime& theTime);
  bool SetAllLanguages(FmiLanguage theLanguage);

  bool IsStationLocalTimeMode() const;
  bool IsFirstStation() const;
  void SetErrorReported(unsigned short ind);
  bool GetErrorReported(unsigned short ind);
  NFmiString MakeLogComment() const;
  void SetNewGeoArea(const NFmiPressArea& theArea);
  NFmiTime GetOptionTime() const;
  void SetOptionTime(const NFmiTime& theTime);
  NFmiLocation* GetOptionLocation();
  void SetOptionLocation(const NFmiLocation& theLocation);
  void SetEncoding(std::string theEncoding);
  void DeleteOptionLocation();
  bool IsAreaOperation() const;
  NFmiArea* GetGeoArea();
  bool ChangeMaskNumber(unsigned long theMask, FmiEnumSpace theEnumSpace);
  void PutInStorage(float value, int storageQueue);
  float UseFromStorage(int storageQueue);
  FmiCounter GetCurrentStep() const;
  bool IsActive() const;
  void SetActivity(bool theActivity);
  bool IsDistanceCheck() const;
  bool CheckAndSetDistance(long theValue, const NFmiPoint& point);
  bool SetMaxMinPoints();
  bool IsMaxMin(bool& theIsMax, NFmiPoint& theCorrectedPoint);
  bool InterruptSymbolGroup() const;
  unsigned long GetCurrentStationStep() const;
  NFmiPressProduct* GetPressProduct() const;
  bool IsSupplementary() const;
  bool SetSegmentCurrentTimeStatus(bool theStatus);
  void SetYearData(bool theStatus);
  bool IsBackupStation() const;
  bool SetStationNotNeeded();
  NFmiPoint GetCurrentUnscaledPoint() const;
  void SetCurrentStationLonLat(const NFmiPoint& theLonLat);
  void SetStationNamesAfterParams(bool theStatus);
  bool IsStationNamesAfterParams() const;
  NFmiFastQueryInfo* GetPrimaryDataIter() const;
  unsigned long GetPrimaryDataNum() const;
  void AddPrimaryDataNum();
  bool PrimaryDataOk() const;
  bool HasPrimaryData() const;
  bool AddNumberOfBackupTimeUsed();
  void SetDayChanged();
  bool IsDayChanged() const;
  bool IsOptimizeGlobalObs() const;
  void SetBackupDayReported();
  bool IsBackupDayReported() const;
  NFmiStationPoint* GetCurrentStationPoint() const;
  void SetReportProseccedNumber(bool theValue);

 protected:
  void UnsetAllErrorReported();
  void NextStation();
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
  bool CreateAreaMask();
  bool CreateStationFromAreaMask();

  bool fActivity;
  bool fStationNotNeeded;
  bool fErrorReported[4]{};
  bool fIsFirstStation = false;
  bool fStationsAreLocalTime;
  bool fGridMode;
  NFmiStation itsCurrentStation;
  NFmiString itsName;
  bool fLonLatMode = false;
  NFmiPressProduct* itsPressProduct;
  unsigned long itsProducer = 0;
  short itsHourStep;
  NFmiString itsDataName;
  NFmiString itsPrimaryDataName;
  NFmiFastQueryInfo* itsDataIter;
  NFmiFastQueryInfo* itsPrimaryDataIter;
  std::shared_ptr<NFmiFastQueryInfo> itsMaskIter;
  NFmiInfoAreaMask* itsAreaMask;
  FmiCounter itsCurrentStep = 0;
  FmiCounter itsNumberOfSteps;
  unsigned long itsCurrentStationIndex = 0;
  NFmiLevel itsLevels[kMaxNumOfTableElements];
  NFmiPoint itsSteps[kMaxNumOfTableElements];
  NFmiPoint itsUnscaledSteps[kMaxNumOfTableElements];
  long itsTimeSteps[kMaxNumOfTableElements]{};
  bool fIsTimeLoop = false;
  bool fIsLevelLoop = false;
  bool fIsPureRegTimeLoop;
  NFmiRectScale itsCurrentStationScale;
  NFmiRectScale itsScale;
  NFmiLocationBag itsStations;
  NFmiVoidPtrList itsTimeDepObjects;
  NFmiVoidPtrList itsStationDepObjects;
  NFmiSymbolGroup itsSymbols;
  bool fTwoPartLogComment = false;
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
  NFmiVoidPtrList* itsNewNames = nullptr;  // owns the renamings shared by the station texts
  bool fReportProseccedNumber;
};
// ----------------------------------------------------------------------
/*!
 * Destructor
 *
 * \todo According to the standard the ifs are not necessary, delete 0 is safe
 */
// ----------------------------------------------------------------------

inline NFmiPressParam::~NFmiPressParam()
{
  itsStations.Destroy();
  itsTimeDepObjects.Clear(isTrue);
  itsStationDepObjects.Clear(isTrue);

  delete itsDataIter;

  delete itsPrimaryDataIter;

  delete itsAreaMask;

  delete itsOptionLocation;

  delete itsDayChangeText;

  delete itsCurrentStationPoint;

  if (itsNewNames)
  {
    NFmiVoidPtrIterator iter(itsNewNames);
    iter.Reset();
    auto* renaming = static_cast<NFmiRenaming*>(iter.Next());
    while (renaming)
    {
      delete renaming;
      renaming = static_cast<NFmiRenaming*>(iter.Next());
    }
    delete itsNewNames;
  }
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressParam::NFmiPressParam()
    : fActivity(true),
      fStationNotNeeded(false),
      fStationsAreLocalTime(false),
      fGridMode(false),
      itsPressProduct(nullptr),
      itsHourStep(24),
      itsDataIter(nullptr),
      itsPrimaryDataIter(nullptr),
      itsAreaMask(nullptr),
      itsNumberOfSteps(1),
      fIsPureRegTimeLoop(true),
      fIsAreaOperation(false),
      itsOptionLocation(nullptr),
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
      itsDayChangeText(nullptr),
      fDayChanged(false),
      fOptimizeGlobalObs(false),
      fBackupDayReported(false),
      itsCurrentStationPoint(nullptr),
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
      fStationsAreLocalTime(false),
      fGridMode(false),
      itsPressProduct(pressProduct),
      itsProducer(producer),
      itsHourStep(hourStep),
      itsDataIter(nullptr),
      itsPrimaryDataIter(nullptr),
      itsAreaMask(nullptr),
      itsNumberOfSteps(1),
      fIsPureRegTimeLoop(true),
      itsScale(scale),
      fIsAreaOperation(false),
      itsOptionLocation(nullptr),
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
      itsDayChangeText(nullptr),
      fDayChanged(false),
      fOptimizeGlobalObs(false),
      fBackupDayReported(false),
      itsCurrentStationPoint(nullptr),
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
inline NFmiStationPoint* NFmiPressParam::GetCurrentStationPoint() const
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
inline void NFmiPressParam::SetBackupDayReported()
{
  fBackupDayReported = true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::IsBackupDayReported() const
{
  return fBackupDayReported;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::IsOptimizeGlobalObs() const
{
  return fOptimizeGlobalObs;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::IsDayChanged() const
{
  return fDayChanged;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressParam::SetDayChanged()
{
  fDayChanged = true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline bool NFmiPressParam::PrimaryDataOk() const
{
  return fPrimaryDataOk;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline unsigned long NFmiPressParam::GetPrimaryDataNum() const
{
  return itsPrimaryDataNum;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline void NFmiPressParam::AddPrimaryDataNum()
{
  itsPrimaryDataNum++;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiFastQueryInfo* NFmiPressParam::GetPrimaryDataIter() const
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

inline bool NFmiPressParam::IsStationNamesAfterParams() const
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

inline NFmiPoint NFmiPressParam::GetCurrentUnscaledPoint() const
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

inline bool NFmiPressParam::SetStationNotNeeded()
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

inline bool NFmiPressParam::IsBackupStation() const
{
  return fCurrentStationBackup;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsSupplementary() const
{
  return fSupplementary;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressProduct* NFmiPressParam::GetPressProduct() const
{
  return itsPressProduct;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::InterruptSymbolGroup() const
{
  return fInterruptSymbolGroup;
}
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

inline bool NFmiPressParam::IsDistanceCheck() const
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

inline bool NFmiPressParam::IsActive() const
{
  return fActivity;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theActivity Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetActivity(bool theActivity)
{
  fActivity = theActivity;
}
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
  data ? itsDataIter = new NFmiSuperSmartInfo(data) : nullptr;
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
  data ? itsPrimaryDataIter = new NFmiSuperSmartInfo(data) : nullptr;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param info Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetMaskIter(std::shared_ptr<NFmiFastQueryInfo> info)
{
  itsMaskIter = std::move(info);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline std::shared_ptr<NFmiFastQueryInfo> NFmiPressParam::GetMaskIter()
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

inline NFmiInfoAreaMask* NFmiPressParam::GetAreaMask()
{
  return itsAreaMask;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param name Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetDataName(const NFmiString& name)
{
  itsDataName = name;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param name Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetPrimaryDataName(const NFmiString& name)
{
  itsPrimaryDataName = name;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param name Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::HasPrimaryData() const
{
  return itsPrimaryDataIter != nullptr;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetLanguage(FmiLanguage theLanguage)
{
  itsLanguage = theLanguage;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiLanguage NFmiPressParam::GetLanguage()
{
  return itsLanguage;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString NFmiPressParam::GetName()
{
  return itsName;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiStation NFmiPressParam::GetCurrentStation() const
{
  return itsCurrentStation;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsStationLocalTimeMode() const
{
  return fStationsAreLocalTime;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetYearData(bool theStatus)
{
  fYearData = theStatus;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsFirstStation() const
{
  return fIsFirstStation;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param ind Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetErrorReported(unsigned short ind)
{
  fErrorReported[ind - 1] = true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param ind Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::GetErrorReported(unsigned short ind)
{
  return fErrorReported[ind - 1];
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiTime NFmiPressParam::GetOptionTime() const
{
  return itsOptionTime;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetOptionTime(const NFmiTime& theTime)
{
  itsOptionTime = theTime;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiLocation* NFmiPressParam::GetOptionLocation()
{
  return itsOptionLocation;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLocation Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::SetOptionLocation(const NFmiLocation& theLocation)
{
  itsOptionLocation = new NFmiLocation(theLocation);
}

inline void NFmiPressParam::DeleteOptionLocation()
{
    delete itsOptionLocation;
  itsOptionLocation = nullptr;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressParam::IsAreaOperation() const
{
  return fIsAreaOperation;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiArea* NFmiPressParam::GetGeoArea()
{
  return itsArea.GetArea();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiCounter NFmiPressParam::GetCurrentStep() const
{
  return itsCurrentStep;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressParam::UnsetAllErrorReported()
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
inline void NFmiPressParam::SetCurrentStationLonLat(const NFmiPoint& theLonLat)
{
  itsCurrentStation.SetLatitude(theLonLat.Y());
  itsCurrentStation.SetLongitude(theLonLat.X());
}

#endif  // NFMIPRESSPARAM_H

// ======================================================================
