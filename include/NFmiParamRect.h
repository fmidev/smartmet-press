// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiParamRect
 */
// ======================================================================
/*!
 * \class NFmiParamRect
 *
 * T‰m‰ on perusluokka datariippuvien ps-olioiden tuottamiselle.
 * S‰‰symboli- numeroluokat perit‰‰n t‰st‰. T‰m‰n luokan valmistumisen
 * j‰lkeen on osittain rinnakkainen luokkahierarkia l‰htenyt kehittym‰‰n
 * (jossa NFmiPressScaling keskeinen). Ps-koodin kirjoittaminen 
 * tapahtuu t‰ss‰ erillisill‰ extern-funktioilla kun taas NFmiPressScaling
 * on peritty asiaa hoitavasta NFmiPsWriting-luokasta. 
 * M‰‰rittelytiedoston lukeminen sent‰‰n periytyy yhteisest‰ emosta.
 * NFmiRect:st‰ periytymisell‰ on hoidettu skaala-asioita. 
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
  dProbability,
  dAllowMissing,
  dUseFromStorageCond,
  dStorageQueue,
  dAcceptanceInterval,
  dInterval2Number};

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
  virtual bool PointOnParam(NFmiFastQueryInfo * theQI,
							NFmiParam * theParam);
  virtual bool PointOnMultiParam(NFmiFastQueryInfo * theQI,
								 short theNum);
  virtual bool PointOnLevel(NFmiFastQueryInfo * theQI);  
  virtual bool ReadRemaining(void);  
  float SunElevation(NFmiFastQueryInfo * theQueryInfo); 

private:

  int WaWa2PresentWeather(int value, NFmiFastQueryInfo * theData);
  int PreWeaWithPrecForm(int value, NFmiFastQueryInfo * theData);
  
protected:
  bool fModifierUsed;
  float itsInterval2NumberMin;
  float itsInterval2NumberMax;
  float itsInterval2NumberValue;
  float itsValueIntervalMin;
  float itsValueIntervalMax;
  bool fAllowMissing;
  bool fUseFromStorage;
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
  NFmiRect itsRelRect; //Suhteellisen alueen muutokset ker‰t‰‰n t‰h‰n
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
{
  fModifierUsed = false;
  fParamErrorReported = false;
  fLevelErrorReported = false;
  fTimeErrorReported = false;
  itsNumOfMissing = 0;
  itsValueFactor = 1.;
  itsIntegrationPeriod.period = kUnsignedLongMissing;
  itsIntegrationPeriod.startWeight = kFloatMissing;
  itsMultiMapping = 0;
  itsCurrentParamArray = 0;
  itsMultiParams = 0;
  itsModifier = kNoneModifier;
  itsAreaModifier = kNoneModifier;
  itsValueOption = kNoneValueOption;
  itsFirstExtremYear = kLongMissing;
  itsIdentPar = kFmiTemperature;
  itsRealPar = 0;
  fUseFromStorage = false;
  fUseFromStorageConditionally = false;
  fPutInStorage = false;
  itsStorageQueue = 1;
  fIsProbability = false;
  fAllowMissing = false;
  itsFirstExtremRelHour = kLongMissing;
  itsValueIntervalMin = kFloatMissing;
  itsInterval2NumberMin = kFloatMissing;
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


#endif // NFMIPARAMRECT_H

// ======================================================================

