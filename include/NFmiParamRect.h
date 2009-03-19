// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiParamRect
 */
// ======================================================================
/*!
 * \class NFmiParamRect
 *
 * Tämä on perusluokka datariippuvien ps-olioiden tuottamiselle.
 * Sääsymboli- numeroluokat peritään tästä. Tämän luokan valmistumisen
 * jälkeen on osittain rinnakkainen luokkahierarkia lähtenyt kehittymään
 * (jossa NFmiPressScaling keskeinen). Ps-koodin kirjoittaminen 
 * tapahtuu tässä erillisillä extern-funktioilla kun taas NFmiPressScaling
 * on peritty asiaa hoitavasta NFmiPsWriting-luokasta. 
 * Määrittelytiedoston lukeminen sentään periytyy yhteisestä emosta.
 * NFmiRect:stä periytymisellä on hoidettu skaala-asioita. 
 *
 */
// ======================================================================

#ifndef NFMIPARAMRECT_H
#define NFMIPARAMRECT_H

// press
#include "NFmiCopyFile.h"
#include "NFmiMultiParamMapping.h"
#include "NFmiPressTimeDescription.h"  
#include "NFmiPsWriting.h"
#include "NFmiRect.h"
#include "NFmiRectScale.h"
// newbase
#include "NFmiDataIdent.h"
#include "NFmiSaveBase.h"
#include "NFmiSuperSmartInfo.h"

class _FMI_DLL NFmiPressParam;
class _FMI_DLL NFmiPressProduct;


//! Undocumented
enum NFmiParamRectObjects
{
  dRelPlace = 60,
  dPlaceMove,
  dParam,
  dValueFactor,
  dIntegrationPeriod,
  dMaximum,
  dMinimum,
  dMean,
  dWeightedMean,
  dSum,

  dLevel = 1060,
  dYearPeriod,
  dWindCill,
  dDegreeDays,
  dFahrenheit,
  dAreaMaximum,
  dAreaMinimum,
  dAreaMean,
  dAreaSum,
  dAreaUnion,

  dPutInStorage = 5060,
  dUseFromStorage,
  dUseFromStorageInFrontOf,
  dProbability,
  dAllowMissing,
  dUseFromStorageCond,
  dStorageQueue,
  dAcceptanceInterval,
  dInterval2Number,
  dRandomInterval,
  dRandomModifying = 6060,
  dEquiDistanceMarking,
  dEquiDistanceJust,
  dStationTableActive,
  dFromMeanWindToMax,
  dRounding,
  dSupplementForMissing,
  dMissingValueString,
  dPlaceMoveAlternating,
  dTempNotMean,
  dRotatingAngle = 7060,
  dExtremePlotting,
  dTempMaxCorrection,
  dTempMinCorrection
};

//! Undocumented
struct FmiIntegrationPeriod
{
  unsigned long period;
  float startWeight;
  float centreWeight;
  float endWeight;
};

//! Undocumented
enum FmiModifier
{
  kNoneModifier,
  kMinimum,
  kMaximum,
  kMean,
  kWeightedMean,
  kSum,
  kUnion
};

//! Undocumented
enum FmiPressValueOption
{
  kNoneValueOption,
  kFahrenheit,
  kDegreeDays
};

//! Undocumented
class NFmiParamRect : public NFmiRect, public NFmiPressTimeDescription  
{

public:

  virtual ~NFmiParamRect(void);

  NFmiParamRect(void);
  NFmiParamRect(const NFmiParamRect & theRect);  
  NFmiParamRect(NFmiDataIdent theParam,
				NFmiRect theRect,
				std::ofstream * theLogFile,
				unsigned short theMaxLoopNum);
  

  virtual NFmiParamRect * Clone(void) const = 0; 
  
  virtual bool WritePS(const NFmiRect & AbsoluteRectOfSymbolGroup,
					   NFmiFastQueryInfo * theQueryInfo,
					   std::ofstream & theDestinationFile,
					   FmiPressOutputMode theOutput) = 0;

  int NumOfMappingParams(void);
  NFmiDataIdent	GetDataIdent(void) const;        

  bool SetPressParam (NFmiPressParam * pressParam);
  virtual bool Set(NFmiDataIdent theParam, NFmiRect theRect);
  bool SetIdent(NFmiDataIdent theParam); 
  bool SetRect(NFmiRect theRect);        
  long NumOfMissing(void) const;
  void InitMissing(void);
  int ConvertDefText(NFmiString & object);

  bool IsNewScaling(void);
  void SetNewScaling(bool newScaling);
  virtual void SetLanguage(FmiLanguage language);
  bool UpdateModifierTimes(void);
  unsigned long GetOrder(void)const; 
  void SetOrder(unsigned long theOrder); 
  void SetMaxCorrection(void);
  void SetMinCorrection(void);
  void SetIdentPar(unsigned long theIdentPar); 
  unsigned long GetIdentPar(void); 
  unsigned long GetPrimaryDataNum(void)const;

protected:

  bool SetRelModifierTimes(long startRelHour, long endRelHour);
  long RelativeHour(void) const;
  bool SetRelativeHour(NFmiFastQueryInfo * data,
					   const NFmiString & calledFrom); 
  NFmiMetTime CalculatePeriodTime(long theHour); 
  bool SetStationLocalTime(NFmiFastQueryInfo * theQI);
  virtual FmiInterpolationMethod InterpolationMethod(NFmiFastQueryInfo * theQueryInfo);   
  bool ReadCurrentValueArray(NFmiFastQueryInfo * theQueryInfo); 
  bool ReadCurrentValue(NFmiFastQueryInfo * theQueryInfo,
						float & value,
						bool localTimeSet=false); 
  bool FloatValue(NFmiFastQueryInfo * theQueryInfo,
				  float & value); 
  NFmiPressProduct* GetPressProduct (void)const;
  virtual bool PointOnParam(NFmiFastQueryInfo * theQI,
							NFmiParam * theParam);
  virtual bool PointOnMultiParam(NFmiFastQueryInfo * theQI,
								 short theNum);
  virtual bool PointOnLevel(NFmiFastQueryInfo * theQI);  
  virtual bool ReadRemaining(void);  
  float SunElevation(NFmiFastQueryInfo * theQueryInfo);
  float GetRandomInterval(void)const{return itsRandomInterval;};
  bool IsRandom(void)const {return GetRandomInterval() > .00001f;};
  bool IsEquiDistanceMode(void) const;
  bool IsEquiDistanceAndCorrMode(void) const;
  bool ActiveStationIndex(int currentInd) const; //vain TimeParamRect toistaiseksi
  bool CompleteMultiMapping(void); 
  bool IsMaxMinPlotting(void) const;
  bool IsMissingLonLat(void);

private:

  int WaWa2PresentWeather(int value, NFmiFastQueryInfo * theData);
  int PreWeaWithPrecForm(int value, NFmiFastQueryInfo * theData);
  bool Randomize(float& theValue, float theInterval, float theMin, float theMax) const;
  bool RandomizeRelatively(float& theValue, float theInterval, float theMin, float theMax) const;
  bool RandomModify(float& theValue, unsigned long theParIdent) const;
  void JustifyConturPlace(NFmiFastQueryInfo * theQueryInfo, float& value);

protected:
  bool fModifierUsed;
  float itsInterval2NumberMin;
  float itsInterval2NumberMax;
  float itsInterval2NumberValue;
  float itsValueIntervalMin;
  float itsValueIntervalMax;
  bool fAllowMissing;
  bool fUseFromStorage;
  bool fUseFromStorageInFrontOf;
  bool fUseFromStorageConditionally;
  bool fPutInStorage;
  int itsStorageQueue;
  bool fIsProbability;
  NFmiLevel itsLevel; 
  NFmiMetTime itsCurrentSegmentTime; 
  NFmiMetTime itsCurrentTime;        
  unsigned long itsCurrentMultiParNum; 
  unsigned long itsCombinedIdent;
  unsigned long itsCurrentPar;
  unsigned long itsRealPar;
  float itsCurrentParamValue; 
  double itsValueFactor;       
  NFmiRect itsRelRect; //Suhteellisen alueen muutokset kerätään tähän
  bool fNewScaling;    
  bool fParamErrorReported;  
  bool fLevelErrorReported;  
  bool fTimeErrorReported;
  long itsNumOfMissing;     
  NFmiPressParam * itsPressParam; 
  FmiIntegrationPeriod itsIntegrationPeriod; 

  // yhteinen symbolille ja letterille vaikka tav mappi ilmeisesti erikseen
  NFmiMultiParamMapping * itsMultiMapping;

  float * itsCurrentParamArray;   
  FmiParameterName * itsMultiParams;
  long itsFirstExtremRelHour;
  long itsLastExtremRelHour;  
  long itsFirstExtremHour;  
  long itsLastExtremHour;  
  long itsFirstExtremYear;
  long itsLastExtremYear;
  FmiModifier itsModifier;     
  FmiModifier itsAreaModifier;
  FmiPressValueOption itsValueOption;
  NFmiPsWriting itsPsWriting;
  unsigned long itsIdentPar;
  float itsRandomInterval;
  bool fRandomModifying;
  float itsEquiDistance;
  float itsEquiDistanceHalfInterval;
  float itsEquiRadius;
  bool fMarkingValue;
  unsigned long itsSymbolGroupOrder;
  FmiLoopActivity itsStationLoopActivity;
  bool fMeanWindToMax;
  long itsRoundingNumber;
  bool fSupplementForMissing;
  NFmiString * itsMissingString;
  NFmiPoint itsAlternating;
  bool fTempNotMean;
  float itsRotatingAngle;
  NFmiPoint itsCorrPoint;
  NFmiString itsMaxText;
  NFmiString itsMinText;
  bool fTempMaxCorrection;
  bool fTempMinCorrection;
private:

  NFmiDataIdent	itsDataIdent;

}; // class NFmiParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiParamRect::NFmiParamRect(void)
  : NFmiRect()  
  , fModifierUsed(false)
  , itsInterval2NumberMin(kFloatMissing)
  , itsValueIntervalMin(kFloatMissing)
  , fAllowMissing(false)
  , fUseFromStorage(false)
  , fUseFromStorageInFrontOf(false)
  , fUseFromStorageConditionally(false)
  , fPutInStorage(false)
  , itsStorageQueue(1)
  , fIsProbability(false)
  , itsRealPar(0)
  , itsValueFactor(1.)
  , fParamErrorReported(false)
  , fLevelErrorReported(false)
  , fTimeErrorReported(false)
  , itsNumOfMissing(0)
  , itsMultiMapping(0)
  , itsCurrentParamArray(0)
  , itsMultiParams(0)
  , itsFirstExtremRelHour(kLongMissing)
  , itsFirstExtremYear(kLongMissing)
  , itsModifier(kNoneModifier)
  , itsAreaModifier(kNoneModifier)
  , itsValueOption(kNoneValueOption)
  , itsIdentPar(kFmiTemperature)
  , itsRandomInterval(0.)
  , fRandomModifying(false)
  , itsEquiDistance(0.)
  , itsEquiDistanceHalfInterval(0.)
  , itsEquiRadius(0.)
  , fMarkingValue(true)
  , itsSymbolGroupOrder(0)
  , fMeanWindToMax(false)
  , itsRoundingNumber(kLongMissing)
  , fSupplementForMissing(false)
  , itsMissingString(0)
  , itsAlternating(NFmiPoint(0.,0.))
  , fTempNotMean(false)
  , itsRotatingAngle(kFloatMissing)
  , itsMaxText(NFmiString("None"))
  , itsMinText(NFmiString("None"))
  , fTempMaxCorrection(false)
  , fTempMinCorrection(false)

{
  itsStationLoopActivity.startIndex=0;
  itsIntegrationPeriod.period = kUnsignedLongMissing;
  itsIntegrationPeriod.startWeight = kFloatMissing;
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theParam Undocumented
 * \param theRect Undocumented
 * \param theLogFile Undocumented
 * \param theMaxLoopNum Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiParamRect::NFmiParamRect(NFmiDataIdent theParam,
							 NFmiRect theRect,
							 std::ofstream * theLogFile,
							 unsigned short theMaxLoopNum)
  : NFmiRect(theRect)
  , fModifierUsed(false)
  , itsValueIntervalMin(kFloatMissing)
  , fAllowMissing(false)
  , fUseFromStorage(false)
  , fUseFromStorageInFrontOf(false)
  , fUseFromStorageConditionally(false)
  , fPutInStorage(false)
  , fIsProbability(false)
  , itsRealPar(0)
  , itsCurrentParamValue (kFloatMissing)
  , itsValueFactor (1.)
  , fParamErrorReported(false)
  , fLevelErrorReported(false)
  , fTimeErrorReported(false)
  , itsNumOfMissing(0)
  , itsMultiMapping(0)
  , itsCurrentParamArray(0)
  , itsMultiParams(0)
  , itsFirstExtremYear(kLongMissing)
  , itsModifier(kNoneModifier)
  , itsAreaModifier(kNoneModifier)
  , itsValueOption(kNoneValueOption)
  , itsIdentPar(kFmiTemperature)
  , itsRandomInterval(0.)
  , fRandomModifying(false)
  , itsEquiDistance(0.)
  , itsEquiDistanceHalfInterval(0.)
  , itsEquiRadius(0.)
  , fMarkingValue(true)
  , itsSymbolGroupOrder(0)
  , fMeanWindToMax(false)
  , itsRoundingNumber(kLongMissing)
  , fSupplementForMissing(false)
  , itsMissingString(0)
  , itsAlternating(NFmiPoint(0.,0.))
  , fTempNotMean(false)
  , itsRotatingAngle(kFloatMissing)
  , itsMaxText(NFmiString("None"))
  , itsMinText(NFmiString("None"))
  , fTempMaxCorrection(false)
  , fTempMinCorrection(false)
  , itsDataIdent(theParam)
{
  itsLogFile = theLogFile;
  itsMaxLoopNum = theMaxLoopNum;
  itsIntegrationPeriod.period = kUnsignedLongMissing;
  itsIntegrationPeriod.startWeight = kFloatMissing;
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline
void NFmiParamRect::SetMaxCorrection(void)
{
  fTempMaxCorrection = true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------
inline
void NFmiParamRect::SetMinCorrection(void)
{
  fTempMinCorrection = true;
}
// ----------------------------------------------------------------------
/*!
 * Set processing order in SymbolGroup
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiParamRect::IsMaxMinPlotting(void) const
{
	return itsMaxText != NFmiString("None");
}
// ----------------------------------------------------------------------
/*!
 * Set processing order in SymbolGroup
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiParamRect::SetIdentPar(unsigned long theIdentPar) 
{
	itsIdentPar = theIdentPar;
}
// ----------------------------------------------------------------------
/*!
 * Set processing order in SymbolGroup
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
unsigned long NFmiParamRect::GetIdentPar(void) 
{
	return itsIdentPar;
}
// ----------------------------------------------------------------------
/*!
 * Set processing order in SymbolGroup
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiParamRect::SetOrder(unsigned long theOrder) 
{
	itsSymbolGroupOrder = theOrder;
}
// ----------------------------------------------------------------------
/*!
 * Get processing order in SymbolGroup
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
unsigned long NFmiParamRect::GetOrder(void)const 
{
	return itsSymbolGroupOrder;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiDataIdent NFmiParamRect::GetDataIdent(void) const
{
  return itsDataIdent;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param pressParam Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiParamRect::SetPressParam(NFmiPressParam * pressParam) 
{
  itsPressParam = pressParam;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
long NFmiParamRect::NumOfMissing(void) const
{
  return itsNumOfMissing;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiParamRect::InitMissing(void)
{
  itsNumOfMissing=0;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiParamRect::IsNewScaling(void)
{
  return fNewScaling;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param newScaling Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiParamRect::SetNewScaling(bool newScaling)
{
  fNewScaling=newScaling;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param newLanguage Undocumented, unused
 */
// ----------------------------------------------------------------------

inline
void NFmiParamRect::SetLanguage(FmiLanguage newLanguage)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param newLanguage Undocumented, unused
 */
// ----------------------------------------------------------------------

inline
bool NFmiParamRect::IsEquiDistanceMode(void) const
{
	return itsEquiDistanceHalfInterval > 0.;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param newLanguage Undocumented, unused
 */
// ----------------------------------------------------------------------

inline
bool NFmiParamRect::IsEquiDistanceAndCorrMode(void) const
{
	return itsEquiRadius > 0.;
}
#endif // NFMIPARAMRECT_H

// ======================================================================

