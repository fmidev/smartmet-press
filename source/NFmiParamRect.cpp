// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiParamRect.h"
#include "NFmiPressParam.h"

#include "NFmiBitMask.h"
#include "NFmiCalculator.h"
#include "NFmiDataIterator.h"
#include "NFmiDataModifierClasses.h"
#include "NFmiDataModifierCombinedParam.h"
#include "NFmiDataModifierDescriptor.h"
#include "NFmiDataModifierProb.h"
#include "NFmiDataModifierVariance.h"
#include "NFmiDataModifierWindChill.h"
#include "NFmiInfoAreaMask.h"
#include "NFmiMaskedDataIterator.h"
#include "NFmiMetMath.h"
#include "NFmiParamDataModifierList.h"
#include "NFmiRelativeDataIterator.h"
#include "NFmiRelativeTimeIntegrationIterator.h"
#include "NFmiWeatherAndCloudiness.h"

#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes some internal variables
 */
// ----------------------------------------------------------------------

NFmiParamRect::~NFmiParamRect(void)
{
  if(itsMultiMapping)
	{
	  delete [] itsCurrentParamArray;
	  delete [] itsMultiParams;
	  delete itsMultiMapping;
	}
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theRect The object to copy
 */
// ----------------------------------------------------------------------

NFmiParamRect::NFmiParamRect(const NFmiParamRect & theRect)
  : NFmiRect(NFmiRect(theRect.TopLeft(), theRect.BottomRight()))
  , NFmiPressTimeDescription()
  , itsValueIntervalMin(theRect.itsValueIntervalMin)
  , itsValueIntervalMax(theRect.itsValueIntervalMax)
  , fAllowMissing(theRect.fAllowMissing)
  , fUseFromStorage(theRect.fUseFromStorage)
  , fUseFromStorageConditionally(theRect.fUseFromStorageConditionally)
  , fPutInStorage(theRect.fPutInStorage)
  , itsStorageQueue(theRect.itsStorageQueue)
  , fIsProbability(theRect.fIsProbability)
  , itsLevel(theRect.itsLevel)
  , itsCurrentSegmentTime(theRect.itsCurrentSegmentTime)
  , itsCurrentTime(theRect.itsCurrentTime)
  , itsCurrentMultiParNum(theRect.itsCurrentMultiParNum)
  , itsCombinedIdent(theRect.itsCombinedIdent)
  , itsRealPar(theRect.itsRealPar)
  , itsCurrentParamValue(theRect.itsCurrentParamValue)
  , itsValueFactor(theRect.itsValueFactor)
  , itsRelRect(theRect.itsRelRect)
  , fNewScaling(theRect.fNewScaling)
  , fParamErrorReported(theRect.fParamErrorReported)
  , fLevelErrorReported(theRect.fLevelErrorReported)
  , fTimeErrorReported(theRect.fTimeErrorReported)
  , itsNumOfMissing(theRect.itsNumOfMissing)
  , itsPressParam(theRect.itsPressParam)
  , itsIntegrationPeriod(theRect.itsIntegrationPeriod)
  , itsFirstExtremRelHour(theRect.itsFirstExtremRelHour)
  , itsLastExtremRelHour(theRect.itsLastExtremRelHour)
  , itsFirstExtremHour(theRect.itsFirstExtremHour)
  , itsLastExtremHour(theRect.itsLastExtremHour)
  , itsFirstExtremYear(theRect.itsFirstExtremYear)
  , itsLastExtremYear(theRect.itsLastExtremYear)
  , itsModifier(theRect.itsModifier)
  , itsAreaModifier(theRect.itsAreaModifier)
  , itsValueOption(theRect.itsValueOption)
  , itsIdentPar(theRect.itsIdentPar)
  , itsDataIdent(theRect.itsDataIdent)
{
  SetEnvironment(theRect.GetEnvironment());
  if(theRect.itsMultiMapping)
	{
	  itsMultiMapping = new NFmiMultiParamMapping(*theRect.itsMultiMapping);
	  //vikaa ei arvoille tarvita
	  itsCurrentParamArray = new float[FmiMaxNumOfMappingParams+1];
	  itsMultiParams = new FmiParameterName[FmiMaxNumOfMappingParams+1];
	  for (int i=0;i<=FmiMaxNumOfMappingParams;i++)
		{
		  itsCurrentParamArray[i] = theRect.itsCurrentParamArray[i];
		  itsMultiParams[i] = theRect.itsMultiParams[i];
		}
	}
  else
	itsMultiMapping = 0;
  
  //nämä perityt jäsenet kun eivät ole emon copykonstruktorissa niin olkoon tässä
  itsLogFile  = theRect.GetLogFile();
  itsMaxLoopNum = theRect.GetMaxLoopNum();
  itsFirstPlotHours = theRect.itsFirstPlotHours;
  itsFirstDeltaDays = theRect.itsFirstDeltaDays;
  itsFirstPlotTime = theRect.itsFirstPlotTime;
  fGivenHoursAreRelative = theRect.fGivenHoursAreRelative;
  itsStringNameTimeFormat = theRect.itsStringNameTimeFormat;
  itsLoopActivity = theRect.itsLoopActivity;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theParam Undocumented
 * \param theRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::Set(NFmiDataIdent theParam, NFmiRect theRect)
{
  itsDataIdent = theParam;
  NFmiRect::Set(theRect.TopLeft(), theRect.BottomRight());
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theParam Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::SetIdent(NFmiDataIdent theParam)
{
  itsDataIdent = theParam;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::SetRect(NFmiRect theRect)
{
  NFmiRect::Set(theRect.TopLeft(), theRect.BottomRight());
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiParamRect::NumOfMappingParams(void)
{
  if(!itsMultiMapping)
	return 0;
  
  int i;
  for(i=0; i<FmiMaxNumOfMappingParams; i++)
	{
	  if (itsMultiParams[i] == kFmiLastParameter)
		break;
	}
  return i;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::ReadRemaining(void)
{
  
  long long1, long2;
  NFmiValueString valueString;
  switch(itsIntObject)
	{
	case dParam:
	  {
		if (!ReadEqualChar())
		  break;
		if(ReadLong(long1))
		  itsIdentPar = long1;
		
		if(itsIdentPar == kFmiDegreeDays)
		  {
			//tälläkin tavalla sen lisäksi että vain ilmoittaa LämmitystarveLuku
			itsValueOption = kDegreeDays;
			itsIdentPar = kFmiTemperature;
			SetRelModifierTimes(-2, 22);
			itsModifier = kMean;
		  }
		
		if(itsIdentPar == 362)
		  {
			itsIdentPar = 21;
			itsRealPar = 362;
		  }
		ReadNext();
		break;
	  }
	case dMultiParams:
	  {
		if (!ReadEqualChar())
		  break;
		
		if(itsMultiMapping)
		  {
			*itsLogFile << "*** ERROR: MoniMuunnoksen parametrit toiseen kertaan, hylätään "
						<< endl;
			break;
		  }
		
		itsMultiMapping = new NFmiMultiParamMapping;
		itsMultiParams = new FmiParameterName[FmiMaxNumOfMappingParams+1];
		itsCurrentParamArray = new float[FmiMaxNumOfMappingParams];
		
		int i;
		for(i=0; i<FmiMaxNumOfMappingParams; i++)
		  {
			itsMultiParams[i] = kFmiLastParameter;
		  }
		
		for(i=0; i<FmiMaxNumOfMappingParams; i++)
		  {
			if(ReadLong(long1, false))
			  itsMultiParams[i] = FmiParameterName(long1);
			else
			  break;
		  }
		
		itsMultiMapping->NumOfParams(i);
		
		// Huom uusi tapa käyttää ReadLongia (stringi joka lopettaa ja samalla on seuraava)
		itsString = itsObject;
		itsIntObject = ConvertDefText(itsString);
		break;
	  }
	case dMultiMapping:
	  {
		if (!ReadEqualChar())
		  break;
		if(itsMultiMapping)
		  itsMultiMapping->AddMappingInterval(itsMultiMapping->ReadOneMapping(itsDescriptionFile));
		else
		  *itsLogFile << "*** ERROR: MoniMuunnos ilman parametrilistaa " << endl;
		
		ReadNext();
		break;
	  }
	case dSum:
	  {
		if(!SetTwo(long1, long2)  ||  long1 > long2)
		  *itsLogFile << "*** ERROR: Summa-ajat väärin " << endl;
		else
		  {
			SetRelModifierTimes(long1, long2);
			itsModifier = kSum;
		  }
		break;
	  }
	case dMinimum:
	  {
		if(!SetTwo(long1, long2)  ||  long1 > long2)
		  *itsLogFile << "*** ERROR: Minimiajat väärin " << endl;
		else
		  {
			SetRelModifierTimes(long1, long2);
			itsModifier = kMinimum;
		  }
		break;
	  }
	case dMaximum:
	  {
		if(!SetTwo(long1, long2)  ||  long1 > long2)
		  *itsLogFile << "*** ERROR: Maksimiajat väärin " << endl;
		else
		  {
			SetRelModifierTimes(long1, long2);
			itsModifier = kMaximum;
		  }
		break;
	  }
	case dMean:
	  {
		if(!SetTwo(long1, long2)  ||  long1 > long2)
		  {
			*itsLogFile << "*** ERROR: Keskiarvoajat väärin " << endl;
		  }
		else
		  {
			SetRelModifierTimes(long1, long2);
			itsModifier = kMean;
		  }
		
		break;
	  }
	case dWeightedMean:
	  {
		if(!SetFive(long1, long2, itsIntegrationPeriod.startWeight,itsIntegrationPeriod.centreWeight,itsIntegrationPeriod.endWeight)
		   || long1 > long2)
		  *itsLogFile << "*** ERROR: Painotettu keskiarvo annettu väärin " << endl;
		else
		  {
			SetRelModifierTimes(long1, long2);
			itsModifier = kWeightedMean;
		  }
		
		break;
	  }
	case dAreaUnion:
	  {
		itsAreaModifier = kUnion;
		ReadNext();
		break;
	  }
	case dAreaSum:
	  {
		itsAreaModifier = kSum;
		ReadNext();
		break;
	  }
	case dAreaMaximum:
	  {
		itsAreaModifier = kMaximum;
		ReadNext();
		break;
	  }
	case dAreaMinimum:
	  {
		itsAreaModifier = kMinimum;
		ReadNext();
		break;
	  }
	case dAreaMean:
	  {
		itsAreaModifier = kMean;
		ReadNext();
		break;
	  }
	case dWindCill:
	  {
		itsIdentPar = kFmiChillFactor;
		ReadNext();
		break;
	  }
	case dDegreeDays:
	  {
		itsValueOption = kDegreeDays;
		itsIdentPar = kFmiTemperature;	//ei tartte erikseen kutsua
		SetRelModifierTimes(-2, 22);	// -"-    SA-vrk
		itsModifier = kMean;			//  -"-
		ReadNext();
		break;
	  }
	case dFahrenheit:
	  {
		itsValueOption = kFahrenheit;
		
		
		ReadNext();
		break;
	  }
	case dPutInStorage:
	  {
		fPutInStorage = true;
		
		ReadNext();
		break;
	  }
	case dUseFromStorage:
	  {
		fUseFromStorage = true;
		
		ReadNext();
		break;
	  }
	case dStorageQueue:
	  {
		SetOne(itsStorageQueue);
		if(itsStorageQueue < 1 || itsStorageQueue > 10)
		  {
			*itsLogFile << "*** ERROR: Invalid storage queue: "
						<< itsStorageQueue
						<< endl;
			itsStorageQueue = 1;
		  }
		
		break;
	  }
	case dUseFromStorageCond:
	  {
		fUseFromStorageConditionally = true;
		
		ReadNext();
		break;
	  }
	case dProbability:
	  {
		fIsProbability = true;
		
		ReadNext();
		break;
	  }
	case dAllowMissing:
	  {
		fAllowMissing = true;
		
		ReadNext();
		break;
		}
	case dYearPeriod:
	  {
		if(!SetTwo(long1, long2) || long1 > long2	)
		  {
			*itsLogFile << "*** ERROR: Vuosijakso annettu väärin " << endl;
		  }
		else
		  {
			itsFirstExtremYear = long1;
			itsLastExtremYear = long2;
		  }
		break;
	  }
	case dLevel:
	  {
		if(SetOne(long1))
		  itsLevel = NFmiLevel(kFmiPressureLevel, long1);
		
		break;
	  }
	case dValueFactor:
	  {
		SetOne(itsValueFactor);
		
		break;
	  }
	case dIntegrationPeriod:
	  {
		if (!ReadEqualChar())
		  break;
		
		if(ReadOne(itsIntegrationPeriod.period))
		  {
			*itsDescriptionFile >> itsObject;
			valueString = itsObject;
			if(valueString.IsNumeric())
			  {
				itsIntegrationPeriod.startWeight = static_cast<float>(valueString);
				
				ReadTwo(itsIntegrationPeriod.centreWeight,itsIntegrationPeriod.endWeight);
				
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
			  }
			else
			  {
				itsIntegrationPeriod.startWeight = kFloatMissing;
				itsIntegrationPeriod.centreWeight = kFloatMissing;
				itsIntegrationPeriod.endWeight = kFloatMissing;
				
				itsString = valueString;
			  }
		  }
		else
		  {
			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
		  }
		itsIntObject = ConvertDefText(itsString);
		break;
	  }
	case dAcceptanceInterval:
	  {
		SetTwo(itsValueIntervalMin, itsValueIntervalMax);
		break;
	  }
	case dRelHour:
	  {
		*itsLogFile << "*** ERROR: Ei sallittu dataelementeille: "
					<< static_cast<char *>(itsString)
					<< endl;
		ReadNext();
		break;
	  }
	default:
	  {
		return NFmiPressTimeDescription::ReadRemaining();
		break;
	  }
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// voidaan käyttää managerin kanssa myös

bool NFmiParamRect::UpdateModifierTimes(void)
{
  long firstPlotHours = itsFirstPlotTime.GetHour();
  if(fGivenHoursAreRelative)
	{
	  itsFirstExtremHour = firstPlotHours + itsFirstExtremRelHour;
	  itsLastExtremHour = firstPlotHours + itsLastExtremRelHour;
	}
  else
	{
	  itsFirstExtremHour = itsFirstExtremRelHour;
	  itsLastExtremHour = itsLastExtremRelHour;
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param startHour Undocumented
 * \param endHour Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::SetRelModifierTimes(long startHour, long endHour)
{
  itsFirstExtremRelHour = startHour;
  itsLastExtremRelHour = endHour;
  
  return UpdateModifierTimes();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiParamRect::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase(); // kaikille sallitaan vapaasti isot/pienet kirjaimet

  if(lowChar==NFmiString("´relplace") ||
	 lowChar==NFmiString("osaalue"))
	return dRelPlace;

  else if(lowChar==NFmiString("level") ||
		  lowChar==NFmiString("painepinta"))
	return dLevel;

  else if(lowChar==NFmiString("valuefactor") ||
		  lowChar==NFmiString("arvokerroin"))
	return dValueFactor;

  else if(lowChar==NFmiString("placemove") ||
		  lowChar==NFmiString("paikansiirto") ||
		  lowChar==NFmiString("suhtpaikka") ||
		  lowChar==NFmiString("suhteellinenpaikka"))
	return dPlaceMove;

  else if(lowChar==NFmiString("param") ||
          lowChar==NFmiString("parametri"))
	return dParam;

  else if(lowChar==NFmiString("integrationperiod") ||
		  lowChar==NFmiString("integrointijakso"))
	return dIntegrationPeriod;

  else if(lowChar==NFmiString("sum") ||
		  lowChar==NFmiString("summa"))
	return dSum;

  else if(lowChar==NFmiString("maximum") ||
          lowChar==NFmiString("maksimi"))
	return dMaximum;

  else if(lowChar==NFmiString("mean") ||
          lowChar==NFmiString("keskiarvo"))
	return dMean;

  else if(lowChar==NFmiString("weightedmean") ||
		  lowChar==NFmiString("painotettukeskiarvo"))
	return dWeightedMean;

  else if(lowChar==NFmiString("minimum") ||
          lowChar==NFmiString("minimi"))
	return dMinimum;

  else if(lowChar==NFmiString("areaunion") ||
		  lowChar==NFmiString("alueunioni") ||
		  lowChar==NFmiString("alueyhdistelmä"))
	return dAreaUnion;

  else if(lowChar==NFmiString("areasum") ||
          lowChar==NFmiString("aluesumma"))
	return dAreaSum;

  else if(lowChar==NFmiString("areamaximum") ||
		  lowChar==NFmiString("aluemaksimi"))
	return dAreaMaximum;

  else if(lowChar==NFmiString("areamean") ||
		  lowChar==NFmiString("aluekeskiarvo"))
	return dAreaMean;

  else if(lowChar==NFmiString("areaminimum") ||
		  lowChar==NFmiString("alueminimi"))
	return dAreaMinimum;

  else if(lowChar==NFmiString("yearperiod") ||
		  lowChar==NFmiString("vuosijakso"))
	return dYearPeriod;

  else if(lowChar==NFmiString("multiparams") ||
		  lowChar==NFmiString("moniparametrit"))
	return dMultiParams;

  else if(lowChar==NFmiString("multimapping") ||
		  lowChar==NFmiString("monimuunnos"))
	return dMultiMapping;

  else if(lowChar==NFmiString("windchill") ||
		  lowChar==NFmiString("pakkasenpurevuus"))
	return dWindCill;

  else if(lowChar==NFmiString("degreeday") ||
		  lowChar==NFmiString("lämmitystarve") ||
		  lowChar==NFmiString("lämmitystarveluku"))
	return dDegreeDays;

  else if(lowChar==NFmiString("fahrenheit") )
	return dFahrenheit;

  else if(lowChar==NFmiString("putinstorage") ||
		  lowChar==NFmiString("laitavarastoon"))
	return dPutInStorage;

  else if(lowChar==NFmiString("usefromstorage") ||
		  lowChar==NFmiString("otavarastosta"))
	return dUseFromStorage;

  else if(lowChar==NFmiString("storagequeue") ||
		  lowChar==NFmiString("varastostojono"))
	return dStorageQueue;

  else if(lowChar==NFmiString("usefromstorageconditionally") ||
		  lowChar==NFmiString("otavarastostaehdolla"))
	return dUseFromStorageCond;

  else if(lowChar==NFmiString("probability") ||
		  lowChar==NFmiString("todennäköisyys"))
	return dProbability;

  else if(lowChar==NFmiString("allowmissing") ||
		  lowChar==NFmiString("sallipuuttuvia"))
	return dAllowMissing;

  else if(lowChar==NFmiString("acceptanceinterval") ||
		  lowChar==NFmiString("hyväksymisväli"))
	return dAcceptanceInterval;

  else
	return NFmiPressTimeDescription :: ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \param theParam Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect:: PointOnParam(NFmiFastQueryInfo * theQI, NFmiParam * theParam)
{
  // jos kysytään auringon korkeutta se lasketaan eikä haeta datasta

  itsCurrentPar = theParam->GetIdent();
  if(itsCurrentPar == kFmiSolarElevation || itsCurrentPar == kFmiChillFactor)
	{
	  theQI->FirstParam(); //varmuuden vuoksi
	  return true;
	}
  
  fParamErrorReported = false;
  itsCombinedIdent = kFmiLastParameter;  // !eli olevinaan puuttuva
  
  if (!theQI->Param(*theParam))
	{
	  
	  {
		if(itsLogFile && !fParamErrorReported)
		  {
			
			long paramIdent = theParam->GetIdent();
			*itsLogFile << "  *** ERROR: Parametria ei löydy: "
						<< paramIdent
						<< endl;
			fParamErrorReported = true;
		  }
		return false;
	  }
	  return true;
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \param theNum Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::PointOnMultiParam(NFmiFastQueryInfo * theQI, short theNum)
{
  // jos kysytään auringon korkeutta se lasketaan eikä haeta datasta
  itsCurrentPar = itsMultiParams[theNum];
  if(itsCurrentPar == kFmiSolarElevation || itsCurrentPar == kFmiChillFactor)
	{
	  theQI->FirstParam(); //varmuuden vuoksi
		return true;
	}
  
  fParamErrorReported = false;
  itsCombinedIdent = kFmiLastParameter;  // onko tarkoitettu tähän?
  if (!theQI->Param(NFmiParam(static_cast<unsigned long>(itsMultiParams[theNum]))))
	{
	  {
		if(itsLogFile && !fParamErrorReported)
		  {
			long paramIdent = static_cast<unsigned long>(itsMultiParams[theNum]);
			*itsLogFile << "  *** ERROR: multiParametria ei löydy: "
						<< paramIdent
						<< endl;
			fParamErrorReported = true;
		  }
		return false;
	  }
	  return true;
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::PointOnLevel(NFmiFastQueryInfo * theQI)
{
  //segmentillä voi myös olla level-taulukko, jolloin parametriin ei pidä laittaa mitään
  if(itsLevel.LevelValue() == kFloatMissing)
	return true;

  //huonoa kun tämä aurinko-testi useammassa paikassa
  itsCurrentPar = GetDataIdent().GetParam()->GetIdent();
  if(itsCurrentPar == kFmiSolarElevation)
	{
	  theQI->FirstLevel(); //varmuuden vuoksi
	  return true;
	}
  
  fParamErrorReported = false;
  itsCombinedIdent = kFmiLastParameter;
  if (!theQI->Level(itsLevel))
	{
	  
	  if(itsLogFile && !fLevelErrorReported)
		{
		  *itsLogFile << "*** ERROR: Painepintaa ei löydy: "
					  << itsLevel.LevelValue()
					  << endl;
		  fLevelErrorReported = true;
		}
	  return false;
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQueryInfo Undocumented
 * \param value Undocumented
 * \param localTimeSet Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect:: ReadCurrentValue(NFmiFastQueryInfo * theQueryInfo,
									  float & value,
									  bool localTimeSet)
{
  
  if(!localTimeSet)                 //multimappingissä jo asetettu
	{
	  if(!SetStationLocalTime(theQueryInfo)) // väliaikaisesti jos optio päällä
		{
		  return false;
		}
	}
  // muualla ei enää kosketa QInfon aikaan

  if(!theQueryInfo->TimeToNearestStep(itsCurrentTime,kCenter,theQueryInfo->TimeResolution()/2))
	{
	   // tänne joudutaan jos piirtoelementissä muutetaan olemattomaan aikaan
	  if(!fTimeErrorReported)
		{
		  *itsLogFile << "    WARNING: invalid time: "
					  << itsCurrentTime
					  << " (changed in data element)"
					  << endl;
		  fTimeErrorReported = true;
		}
	}
  
  if(itsPressParam->IsFirstStation() &&
	 !(itsMultiMapping && itsCurrentMultiParNum > 1) &&
	 itsPressParam->IsStationLocalTimeMode())
	{
	  if(itsMultiMapping)
		{
		  *itsLogFile << "    eka asema: aika paikan päällä= "
					  << itsCurrentTime
					  << " utc; par=moni"
					  << endl;
		}
	  else
		{
		  unsigned long par = theQueryInfo->ParamDescriptor().Param().GetParam()->GetIdent();
		  *itsLogFile << "    eka asema: aika paikan päällä= "
					  << itsCurrentTime
					  << " utc; par="
					  << par
					  << endl;
		}
	  *itsLogFile << "      ->lähin data-aika= "
				  << (static_cast<NFmiFastQueryInfo *>(theQueryInfo))->Time()
				  << " utc"
				  << endl;
	}

  FloatValue(theQueryInfo, value);
  
  if(value != kFloatMissing &&
	 itsIdentPar == kFmiPresentWeather &&
	 value >= 100 &&
	 value < 200)
	{
	  value = static_cast<float>(WaWa2PresentWeather(static_cast<int>(value), theQueryInfo));
	}
  
  if(value == kFloatMissing)
	{
	  if(!itsMultiMapping)
		itsNumOfMissing++;
	  
	  return true;
	}
  else
	{
	  value = static_cast<float>(value * itsValueFactor);
	  
	  return true;
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \param theData Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiParamRect::WaWa2PresentWeather(int value, NFmiFastQueryInfo * theData)
{
  switch(value-100)
	{
	case 0:
	case 1:
	case 2:
	case 3:
      return 1;    //poutaa
	case 4:
	case 5:
      return 5;     //auerta
	case 10:
      return 10;		// utua
 	case 20:
      return 28;		// sumua
 	case 21:
	  return PreWeaWithPrecForm(63, theData);// sadetta (olomuoto tuntematon)
 	case 22:
      return 20;		// tihkusad
 	case 23:
      return 21;		// vesisad
 	case 24:
      return 22;		// lumisad
 	case 25:
      return 24;		// jäät.vesisade
	  
 	case 30:
 	case 31:
 	case 32:
 	case 33:
 	case 34:
      return 45;		// sumua
	  
	case 40:
	  return PreWeaWithPrecForm(63, theData);// sadetta (olomuoto tuntematon)
	case 41:
	  return PreWeaWithPrecForm(61, theData);// heikk.sadetta (olomuoto tuntematon)
	case 42:
	  return PreWeaWithPrecForm(65, theData);// kov.sadetta (olomuoto tuntematon)
	  
 	case 50:
 	case 51:
 	case 52:
 	case 53:
	  return 50;	// tihkusad
	case 54:
 	case 55:
  	case 56:
	  return 56;	// jäät.tihkusad
	  
 	case 60:
 	case 61:
	  return 60;	// heik.vesisad
	case 62:
	  return 62;	// vesisad
 	case 63:
	  return 64;	// kov.vesisad
 	case 64:
 	case 65:
 	case 66:
	  return 66;	// jäät.vesisad
 	case 67:
 	case 68:
	  return 68;	// räntää
	  
 	case 70:
	  return 72;	// lumisad
 	case 71:
	  return 70;	// heik.lumisad
 	case 72:
	  return 72;	// lumisad
 	case 73:
	  return 74;	// kov.lumisad
 	case 74:
	case 75:
 	case 76:
	  return 79;	// jäjyväsiä
 	case 77:
	  return 77;	// lumijyväsiä
 	case 78:
	  return 78;	// lumikiteitä
	  
	case 80:
 	case 81:
	  return 80;	// heik.vesikuur
 	case 82:
	  return 81;	// vesikuur
	case 83:
	  return 82;	// kov.vesikuur
	case 84:
	  return 98;	// ankar.vesikuur
	  
	case 85:
	  return 85;	// heik.lumikuur
 	case 86:
	  return 86;	// lumikuur
 	case 87:
	  return 99;	// kov.lumikuur
 	case 89:
	  return 90;	// raekuuroja ???
	  
    default:
	  *itsLogFile << "  WARNING: Tuntematon WaWa-koodi: " << value << endl;
      return 0;
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \param theData Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiParamRect:: PreWeaWithPrecForm(int value, NFmiFastQueryInfo * theData)
{
  //WaWa koodia varten lämpötila
  int newValue;
  if(!theData->Param(kFmiTemperature))
	{
	  *itsLogFile << "  WARNING: Lämpöä ei ole WaWalle" << endl;
	  return value;
	}
  float temp = theData->FloatValue();
  theData->Param(itsDataIdent);    //Takaisin varsinainen param
  newValue = value;
  if(temp < 0.)
	newValue += 10; //lumi
  else if(temp < 2.5 && temp  > -0.1)
	{
	  if(value == 61)
		return 68;
	  else
		return 69;
	}
  return newValue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQueryInfo Undocumented
 * \param value Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect::FloatValue(NFmiFastQueryInfo * theQueryInfo, float& value)
{
  unsigned long par = theQueryInfo->ParamDescriptor().Param().GetParam()->GetIdent();
  
  if(itsCurrentPar == kFmiSolarElevation) // auringon korkeus metodilla eikä datasta
	{
	  value = SunElevation(theQueryInfo);
	  return true;
	}
  
  if(itsCurrentPar == kFmiChillFactor)
	par = kFmiChillFactor;
  
  NFmiMetTime firstTime = (static_cast<NFmiFastQueryInfo *>(theQueryInfo))->Time();
  NFmiMetTime lastTime = firstTime;
  NFmiDataModifier * modif=0;
  NFmiDataModifier * areaModif=0;
  bool isExtremeModifier = false;
  unsigned long period = itsIntegrationPeriod.period;
  float startWeight;
  float centreWeight;
  float endWeight;
  
  
  bool isWeightedMean = period > 0 &&
	period != kUnsignedLongMissing ||
	itsModifier == kWeightedMean;

  if(itsModifier != kNoneModifier )
	{
	  firstTime = CalculatePeriodTime(itsFirstExtremHour);
	  lastTime = CalculatePeriodTime(itsLastExtremHour);
	  period = lastTime.DifferenceInHours(firstTime);
	  
	  if(itsFirstExtremYear != kLongMissing)
		{
		  firstTime.SetYear(itsFirstExtremYear);
		  lastTime.SetYear(itsLastExtremYear);
		  //periodilla ei pitäisi olla väliä ??
		}
	  
	  if(itsPressParam->IsFirstStation() && !(itsMultiMapping && itsCurrentMultiParNum > 1))
		{
		  
		  *itsLogFile << "      ->aikajakso= "
					  << firstTime
					  << " - "
					  << lastTime
					  << " utc"
					  << endl;
		}
	  switch(itsModifier)
		{
		case kMinimum:
		  isExtremeModifier = true;
		  break;
		case kMaximum:
		  modif = new NFmiDataModifierMax;
		  isExtremeModifier = true;
		  break;
		case kSum:
		  if(fAllowMissing)
			modif = new NFmiDataModifierSum(kFmiAdd, true);
		  else
			modif = new NFmiDataModifierAllValidSum;
		  break;
		case kMean:
		  if(fAllowMissing)
			modif = new NFmiDataModifierAvg;
		  else
			modif = new NFmiDataModifierAllValidAvg;
		  break;
		default:
		  modif = new NFmiDataModifierDummy;
		  break;
		}
	}
  else
	modif = new NFmiDataModifierDummy;
  
  
  if(itsValueIntervalMin != kFloatMissing)
	modif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);
  
  switch(itsAreaModifier)
	{
	case kMinimum:
	  areaModif = new NFmiDataModifierMin;
	  isExtremeModifier = true;
	  break;
	case kMaximum:
	  areaModif = new NFmiDataModifierMax;
	  isExtremeModifier = true;
	  break;
	case kSum:
	  if(fAllowMissing)
		areaModif = new NFmiDataModifierSum;
	  else
		areaModif = new NFmiDataModifierAllValidSum;
	  break;
	case kUnion:
	  areaModif = new NFmiDataModifierPrecFormUnion;
	  if(itsValueIntervalMin != kFloatMissing)
		areaModif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);
	  break;
	case kMean:
	  if(fAllowMissing)
		areaModif = new NFmiDataModifierAvg;
	  else
		areaModif = new NFmiDataModifierAllValidAvg;
	  break;
	default:
	  break;
	}
  
  if(areaModif && itsValueIntervalMin != kFloatMissing)
	areaModif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);
  if(modif && itsValueIntervalMin != kFloatMissing)
	modif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);
  
  //  HUOM: OLETUS ON HETKELLINEN EIKÄ JAKSO

  if(period == kUnsignedLongMissing)
	period = 0;
  if (period == kUnsignedLongMissing) //jätetty ohjelman päätettäväksi
	{
	  if (theQueryInfo->TimeDescriptor().Resolution() == 360)
		period = 12;
	  else if (theQueryInfo->TimeDescriptor().Resolution() == 180)
		period = 6;
	  else  //kun tunnin välein
		period = 2;
	}
  startWeight = itsIntegrationPeriod.startWeight;
  centreWeight = itsIntegrationPeriod.centreWeight;
  endWeight = itsIntegrationPeriod.endWeight;
  if(startWeight == kFloatMissing)
	{
	  startWeight = 1;
	  endWeight = 1;
	  centreWeight = 1;   // aina 1, joka tapauksessa vanhentunut
	}
  
  if(period !=0 && itsModifier == kNoneModifier)
	{
	  firstTime = itsCurrentSegmentTime;
	  lastTime = itsCurrentSegmentTime;
	  firstTime.ChangeByHours((-static_cast<long>(period))/2);
	  lastTime.ChangeByHours(period/2);
	  
	  
	  if(itsModifier != kWeightedMean)
		{
		  startWeight = 1.;
		  centreWeight = 1.;
		  endWeight = 1.;
		}
	  if(itsPressParam->IsStationLocalTimeMode())
		{
		  double longitude = itsPressParam->GetCurrentStation().GetLongitude();
		  double latitude = itsPressParam->GetCurrentStation().GetLatitude();//Testiin
		  if(longitude <= .001 && latitude <= .001)
			*itsLogFile << "*** ERROR: longitudi puuttuu "
						<< static_cast<char *>(itsPressParam->GetCurrentStation().GetName())
						<< ":lta paikallista aikaa varten"
						<< endl;
		  
		  firstTime = firstTime.LocalTime(-static_cast<float>(longitude));
		  lastTime = lastTime.LocalTime(-static_cast<float>(longitude));
		}
	}
  
  bool varEtc = (fIsProbability ||
				 itsAreaModifier != kNoneModifier &&
				 itsModifier != kNoneModifier ||
				 itsCurrentPar == 353 &&
				 itsAreaModifier == kMaximum); //HUOM w&c

  if(varEtc)
	{
	  
	  NFmiSuperSmartInfo ssinfo(*theQueryInfo);
	  ssinfo.AreaUnCertaintyStart(2);	//0-> 0 tai 100; olivat 1 3
	  ssinfo.AreaUnCertaintyEnd(2);	//Nämä ovat vain kokeellisia arvoja, pitäisi tulla oikeasti datan mukana meteorologilta
	  NFmiDataModifierMin minvalue;
	  NFmiDataModifierMax areaMax;
	  NFmiDataModifierMax variationMax;
	  NFmiDataModifierAvg mean;
	  NFmiDataModifierSum sum; //(minVal, maxVal);
	  if (itsValueIntervalMin != kFloatMissing)
		sum.SetLimits(itsValueIntervalMin, itsValueIntervalMax);
	  
	  if(fIsProbability) // **** sateen tn *** /
		{
		  NFmiDataModifierProb prob(kValueGreaterThanLimit, .11);
		  
		  NFmiRelativeDataIterator variationArea(&ssinfo, 4, 4, 0); //EI  VAIKUTUSTA
		  NFmiMaskedDataIterator geographicalArea(&ssinfo); //comb/vai ei (ei tartte)  //tiealueet,vika aikajakso

		  NFmiCalculator areaMaxCalc(&geographicalArea, &areaMax);
		  NFmiCalculator probCalc(&variationArea, &prob);
		  bool useVariation = true;
		  bool useCalculator = true;
		  bool createVariance = true;
		  bool useTimeIntegration = true;
		  variationArea.SetDimensions(ssinfo.CalcAreaUnCertainty(), ssinfo.CalcAreaUnCertainty(), 0); //aikavar. vika
		  ssinfo.Param(NFmiParam(itsCurrentPar));
		  ssinfo.AreaMask(itsPressParam->GetAreaMask());
		  ssinfo.UseAreaMask(true);
		  ssinfo.VariationCalculator(&probCalc);
		  ssinfo.SetCalculator(&areaMaxCalc);
		  float areaRRProbMaxCalc = ssinfo.FloatValue(useVariation, useCalculator,!useTimeIntegration,!createVariance);
		  value = areaRRProbMaxCalc;
		}
	  // **** tiehallinnon sateen int epävarmuusalueella laajennettuna *** /
	  else if(itsCurrentPar == 353 && itsAreaModifier == kMaximum && itsModifier == kNoneModifier)
        {
		  NFmiRelativeDataIterator variationArea(&ssinfo, 4, 4, 0);
		  NFmiMaskedDataIterator geographicalArea(&ssinfo); //comb/vai ei (ei tartte)  //tiealueet,vika aikajakso
		  NFmiCalculator areaMaxCalc(&geographicalArea, &areaMax);
		  NFmiCalculator variationMaxCalc(&variationArea, &variationMax);
		  bool useVariation = true;
		  bool useCalculator = true;
		  bool createVariance = false;
		  bool useTimeIntegration = false;
		  variationArea.SetDimensions(ssinfo.CalcAreaUnCertainty(), ssinfo.CalcAreaUnCertainty(), 0); //aikavar. vika
		  ssinfo.Param(NFmiParam(itsCurrentPar));  //paramhan on asetettu???
		  ssinfo.AreaMask(itsPressParam->GetAreaMask());
		  ssinfo.UseAreaMask(true);
		  ssinfo.VariationCalculator(&variationMaxCalc);
		  ssinfo.SetCalculator(&areaMaxCalc);
		  value = ssinfo.FloatValue(useVariation, useCalculator,useTimeIntegration,createVariance);
		}
	  
		// **** tiehallinnon sadesummat **** /
	  else if (itsAreaModifier != kNoneModifier && itsModifier != kNoneModifier )
		{
		  NFmiDataModifierProb prob(kValueGreaterThanLimit, 0.);
		  NFmiRelativeDataIterator variationArea(&ssinfo, 4, 4, 0);
		  NFmiMaskedDataIterator geographicalArea(&ssinfo); //comb/vai ei (ei tartte)  //tiealueet,vika aikajakso
		  NFmiCalculator areaMaxCalc(&geographicalArea, &areaMax);
		  NFmiCalculator variationMaxCalc(&variationArea, &variationMax);
		  NFmiCalculator probCalc(&variationArea, &prob);
		  bool useVariation = true;
		  bool useCalculator = true;
		  bool createVariance = true;
		  bool useTimeIntegration = itsModifier != kNoneModifier; //oik. aikamodifieri
		  
		  NFmiRelativeTimeIntegrationIterator timeIterator(&ssinfo, itsLastExtremRelHour-itsFirstExtremRelHour+1, -itsLastExtremRelHour);
		  NFmiCalculator sumCalc(&timeIterator, &sum);
		  variationArea.SetDimensions(ssinfo.CalcAreaUnCertainty(), ssinfo.CalcAreaUnCertainty(), 0); //aikavar. vika
		  ssinfo.Param(NFmiParam(itsCurrentPar));  //paramhan on asetettu???
		  ssinfo.AreaMask(itsPressParam->GetAreaMask());
		  ssinfo.UseAreaMask(true);
		  ssinfo.VariationCalculator(&variationMaxCalc);
		  ssinfo.SetCalculator(&areaMaxCalc);
		  ssinfo.TimeIntegrationCalculator(&sumCalc);
		  value = ssinfo.FloatValue(!useVariation, useCalculator,useTimeIntegration,!createVariance);
		}
	  
	}
  
  
  else
	{
	  if(theQueryInfo->IsGrid())
		{
		  if (isWeightedMean || //ajan suhteen 
			  period > 0 &&
			  (par == 336 || par == 326 || par == 19 || par == 271 || par == 57 ||
			   par == 353 || par == 375) &&
			  (itsModifier == kNoneModifier || itsModifier == kMean))
			{
			  value = static_cast<float>(theQueryInfo->InterpolatedTimePeriodFloatValue(itsPressParam->GetCurrentStation().GetLocation(),
																						firstTime,lastTime,startWeight,centreWeight,endWeight));
			  
			}
		  else //ei painotettu aikakeskiarvo
			{
			  if(itsModifier != kNoneModifier)
				{
				  //pitäis ehkä olla optio Interpoloi/älä interp.
				  if(par == kFmiChillFactor)
					{
					  //pitää olla modif
					  NFmiDataModifierWindChill wcMod(theQueryInfo, modif,itsPressParam->GetCurrentStation().GetLocation());
					  // tämä nyt kai ei interpoloi
					  theQueryInfo->CalcTimeData(&wcMod, firstTime, lastTime);
					  value = wcMod.CalculationResult();
					}
				  else
					{
					  if (itsPressParam->IsAreaOperation())
						{
						  //oikeastaan turha testi nyt jos aina SupersmartInfo
						  if (NFmiString(theQueryInfo->ClassName()) != NFmiString("NFmiSuperSmartInfo"))
							{
							  *itsLogFile << "***ERROR: data should be in SuperSmartInfo" << endl;
							}
						  (static_cast<NFmiSuperSmartInfo *>(theQueryInfo))->AreaMask(itsPressParam->GetAreaMask());
						  (static_cast<NFmiSuperSmartInfo *>(theQueryInfo))->UseAreaMask(true);
						  
						}
					  
					  theQueryInfo->CalcInterpolatedTimeData(modif, firstTime, lastTime,itsPressParam->GetCurrentStation().GetLocation());
					  if(isExtremeModifier)
						itsPressParam->SetOptionTime((static_cast<NFmiDataModifierExtreme *>(modif))->GetTime());
					  value = modif->CalculationResult();
					}
				}
			  else
				if(par == kFmiChillFactor)
				  {
					NFmiDataModifierWindChill wcMod(theQueryInfo, modif,itsPressParam->GetCurrentStation().GetLocation());
					theQueryInfo->CalcTimeData(&wcMod, firstTime, lastTime);
					value = wcMod.CalculationResult();
				  }
				else
				  {
					
					if(fIsProbability)
					  {
						
					  }
					else if(itsPressParam->IsAreaOperation())
					  {
						if(areaModif)
						  {
							if(itsPressParam->GetAreaMask() > 0)
							  {
								//oikeastaan turha testi nyt jos aina SupersmartInfo
								if (NFmiString(theQueryInfo->ClassName()) != NFmiString("NFmiSuperSmartInfo"))
								  {
									// VAATISI ETTÄ INFO VÄLITETTÄISIIN KOKO KETJUN LÄPI POINTTERIREFERENSSINÄ
									*itsLogFile << "***ERROR: data should be in SuperSmartInfo" << endl;
								  }
								else
								  
								  {
									(static_cast<NFmiSuperSmartInfo *>(theQueryInfo))->AreaMask(itsPressParam->GetAreaMask());
									(static_cast<NFmiSuperSmartInfo *>(theQueryInfo))->UseAreaMask(true);
									(static_cast<NFmiSuperSmartInfo *>(theQueryInfo))->CalcLocationData(areaModif);
									(static_cast<NFmiSuperSmartInfo *>(theQueryInfo))->UseAreaMask(false);
								  }
								
							  }
							else
							  {
								//meneekö tvalliseen QI:oon
								theQueryInfo->CalcLocationData(areaModif, itsPressParam->GetGeoArea());
							  }
							value = areaModif->CalculationResult();
						  }
						else
						  {
							*itsLogFile << "*** ERROR: "<< "Modifier missing from areaoperation (sum,max...)"  << endl;
							value = kFloatMissing;
						  }
					  }
					else
					  
					  value = theQueryInfo->InterpolatedValue(itsPressParam->GetCurrentStation().GetLocation());
				  }
			}
		}
	  else //ei gridi
		{
		  /***** HUOM 9.12.00: pitäisi varmaan estää modifierin käyttö aikakeskiarvon kanssa
				 kuten gridissä tehtiin (esim olomuoto tulee flotarina ja muutenkin se kai olisi se oikea tapa)
		  */
		  if(par == kFmiChillFactor)
			{
			  NFmiDataModifierWindChill wcMod(theQueryInfo, modif, itsPressParam->GetCurrentStation().GetLocation());
			  theQueryInfo->CalcTimeData(&wcMod, firstTime, lastTime);
			  value = wcMod.CalculationResult();
			}
		  else
			{
			  if(itsModifier != kNoneModifier)
				{
				  theQueryInfo->CalcTimeData(modif, firstTime, lastTime);
				  if(isExtremeModifier)
					itsPressParam->SetOptionTime((static_cast<NFmiDataModifierExtreme *>(modif))->GetTime());
				  
				  value = modif->CalculationResult();
				}
			  else
				value = theQueryInfo->FloatValue();
			}
		} //vanhat tavat loppu
	} //vanhat+uudet loppu
  
  if(fUseFromStorageConditionally && value == kFloatMissing)  //4.10.01 olkoon ainoa ehto nyt aluksi että arvo puuttuu
	{
	  float storedValue = itsPressParam->UseFromStorage(itsStorageQueue);
	  if(storedValue != kFloatMissing && storedValue < 105) //HUOM niinkauan kun pohjoisin suomi puuttuu tutkasateista
		{
		  value = storedValue;
		  *itsLogFile << "   * INFO: "<< "stored value used instead of actual"  << endl;
        }
	}
  
  if(itsValueOption == kFahrenheit)
	value = FmiCelsius2Fahrenheit(value);
  else if(itsValueOption == kDegreeDays)
	value = FmiDegreeDays(value, (theQueryInfo)->Time().GetMonth());
  if(itsRealPar == 362) // minimituuli
	{                   //väliaikaisesti
	  if (value < 400) //muuten 32700 tai 999
		{
		  if (value <= 0.5)
			value += - 0;
		  else if(value <= 1.5)
			value += - 1;
		  else if(value <= 2.5)
			value += - 2;
		  else if(value <= 4.5)
			value += - 2.25;
		  else if(value <= 6.5)
			value += - 2;
		  else if(value <= 12.5)
			value += - 1.75;
		  else if(value <= 17.5)
			value += - 2;
		  else if(value <= 20.5)
			value += - 2.25;
		  else if(value <= 23.5)
			value += - 2.5;
		  else if(value <= 26.5)
			value += - 2.75;
		  else if(value <= 29.5)
			value += - 3;
		  else
			value += - 3.25;
		}
	  
	}
  
  if(modif)
	delete modif;
  if(areaModif)
	delete areaModif;
  
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQueryInfo Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiParamRect:: SunElevation(NFmiFastQueryInfo * theQueryInfo)
{
  NFmiLocation location;
  if(theQueryInfo->IsGrid())
	{
	  location = itsPressParam->GetCurrentStation();
	}
  else
	location = *(theQueryInfo->Location());
  
  return static_cast<float>(location.ElevationAngle((static_cast<NFmiFastQueryInfo *>(theQueryInfo))->Time()));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theHour Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiMetTime NFmiParamRect::CalculatePeriodTime(long theHour)
{
  NFmiMetTime tim(itsPressParam->GetFirstPlotTime());
  
  long hour = theHour;
  
  if(theHour > 23)
	{
	  long days = theHour/24; // aikasarjat ulotettu yli yhden vuorokauden siirtymistä
	  tim.ChangeByDays(days);
	  hour -= days*24;
	}
  else if(theHour < 0)
	{
	  long days = (-23+theHour)/24;
	  tim.ChangeByDays(days);
	  hour = 24 + hour; //hour on neg.
	  
	}
  tim.SetHour(static_cast<short>(hour));
  long diff = itsCurrentSegmentTime.DifferenceInHours(itsPressParam->GetFirstPlotTime());
  tim.ChangeByHours(diff);
  
  if(itsPressParam->IsStationLocalTimeMode())
	{
	  double longitude = itsPressParam->GetCurrentStation().GetLongitude();
	  double latitude = itsPressParam->GetCurrentStation().GetLatitude();//Testiin
	  if(fabs(longitude) <= .001 && fabs(latitude) <= .001)
		*itsLogFile << "*** ERROR: longitudi puuttuu "
					<< static_cast<char *>(itsPressParam->GetCurrentStation().GetName())
					<< ":lta paikallista aikaa varten"
					<< endl;

	  tim = tim.LocalTime(-static_cast<float>(longitude));
	  
	}
  return tim;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect:: ReadCurrentValueArray(NFmiFastQueryInfo * theQI)
{
	if(!SetStationLocalTime(theQI)) // väliaikaisesti jos optio päällä
	  {
		return false;
	  }
	for (int i = 0; static_cast<int>(itsMultiParams[i]) != static_cast<int>(kFmiLastParameter); i++)
	  {
		itsCurrentMultiParNum = i+1;
		
		if(!PointOnMultiParam(theQI, i))
		  {
			return false;
		  }
		
		float value;

		// osaparametrit saavat puuttua
		// jos signifikantti puuttuu multiMapping ilmoittaa
		
		ReadCurrentValue(theQI, value, true); // =local time jo asetettu
		
		itsCurrentParamArray[i] = value;
		
		if(i>FmiMaxNumOfMappingParams) //varmuuden vuoksi
		  {
			*itsLogFile << "  *** ERROR: moniParametriluvussa ylivuoto" << endl;
			return false;
		  }
	  }
	
	return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQueryInfo Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// EI TARVITA
FmiInterpolationMethod NFmiParamRect::InterpolationMethod(NFmiFastQueryInfo * theQueryInfo)
{
  unsigned long ident = theQueryInfo->Param().GetParam()->GetIdent();
  if(ident == 338 || ident == 20)
	return kNearestPoint;
  return kLinearly;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamRect:: SetStationLocalTime(NFmiFastQueryInfo * theQI)
{
  //optio
  if(itsPressParam->IsStationLocalTimeMode())
	{
      //HUOM sama koodi ylläpisettävä NFmiTimeParamRect:ssä
	  double longitude = itsPressParam->GetCurrentStation().GetLongitude();
	  double latitude = itsPressParam->GetCurrentStation().GetLatitude();//Testiin
	  if(fabs(longitude) <= .001 && fabs(latitude) <= .001)
		*itsLogFile << "*** ERROR: longitudi puuttuu "
					<< static_cast<char *>(itsPressParam->GetCurrentStation().GetName())
					<< ":lta paikallista aikaa varten"
					<< endl;

	  itsCurrentTime = itsCurrentTime.LocalTime(-static_cast<float>(longitude));
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param data Undocumented
 * \param calledFrom Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// tällä hoidetaan se että alle vuorokaudenkin aikaluupit onnistuvat
// alkioiden omilla tunneilla

bool NFmiParamRect::SetRelativeHour(NFmiFastQueryInfo * data,
									const NFmiString & calledFrom)
{
  if(RelativeHour() != 0)
	{
	  itsCurrentTime.ChangeByHours(RelativeHour());
	  if(itsPressParam->IsFirstStation())
		{
		  if(itsPressParam->IsStationLocalTimeMode())
			*itsLogFile << "    tunti muutettu "
						<< static_cast<char *>(calledFrom)
						<< ":ssa: "
						<< itsFirstPlotHours
						<< " pa"
						<< endl;
		  else
			*itsLogFile << "    tunti muutettu "
						<< static_cast<char *>(calledFrom)
						<< ":ssa: "
						<< itsFirstPlotHours
						<< " utc"
						<< endl;
		}
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// tällä hoidetaan se että alle vuorokaudenkin aikaluupit
// onnistuvat alkioiden omilla tunneilla

long NFmiParamRect::RelativeHour(void) const
{
  return itsFirstPlotHours - itsPressParam->GetFirstPlotHours();
}

// ======================================================================
