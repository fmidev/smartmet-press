// © Ilmatieteenlaitos/Salla.
// Original 24.10.1997
// 
// Changed	xx.xx.97
// Muutettu 211197/LW +ConvertDefText()	
// Muutettu 291297/LW Producer ja param nyt myös yhdellä rivillä
// Muutettu 070198/LW Määrittelyt suomennettu
// Muutettu 120198/LW Producer saadaan PressDescriptionista
// Muutettu 190198/LW 
// Muutettu 040298/LW +NumOfMissing()
// Muutettu 050398/LW copyKonstruktori ym
// Muutettu 200398/LW +PointParam()
// Muutettu 190899/LW +itsCombinedIdent eli parametri voi olla TotalWindissä tai WeatherAndCluodiness:ssa
// Muutettu 190899/LW TUOTTAJA POISTETTU DATAN HAUSSA, pelkkä parametri jää, koko dataIdentti edelleen jäsen
// Muutettu 020999/LW +dWeightedMean
// Muutettu 230999/LW multiParamMapping ja kaikki mikä siihen kuuluu:PointOnMultiParam() jne
// Muutettu 290200/LW copy-konstr. siivottu
// Muutettu 280300/LW auringonKorkeuden käsittely
// Muutettu 020500/LW PointOnParam(,+NFmiParam)
// Muutettu 090500/LW + summa
// Muutettu 180900/LW lokin aikatekstit selvennetty
// Muutettu 131000/LW +itsStringNameTimeFormat copyContr:iin
// Muutettu 171000/LW +topWeatherAndCloudiness huomioitu
// Muutettu 291100/LW +jaksoissa mahdollistettu vuosien asetus
// Muutettu 080201/LW +WaWa-koodin käsittely

#include "npresspa.h"

#include "NFmiWeatherAndCloudiness.h"
#include "NFmiDataModifierClasses.h"
#include "NFmiDataModifierWindChill.h"
#include "NFmiMetMath.h"
#include "NFmiBitMask.h"
#include "NFmiCalculator.h"
#include "NFmiDataIterator.h"
#include "NFmiInfoAreaMask.h"
#include "NFmiDataModifierDescriptor.h"
#include "NFmiParamDataModifierList.h"
#include "NFmiDataModifierVariance.h"
#include "NFmiRelativeDataIterator.h"
#include "NFmiDataModifierProb.h"
#include "NFmiDataModifierCombinedParam.h"
#include "NFmiMaskedDataIterator.h"
#include "NFmiRelativeTimeIntegrationIterator.h"
//#include "NFmiTiesaaAlueet.h"
//#include "NFmiStreamQueryData.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01


//---------------------------------------------------------------------------
#include "nparrect.h"
//---------------------------------------------------------------------------
NFmiParamRect :: NFmiParamRect(const NFmiParamRect& theRect)  
:NFmiRect(NFmiRect(theRect.TopLeft(), theRect.BottomRight()))
,NFmiPressTimeDescription()
		,itsValueIntervalMin(theRect.itsValueIntervalMin) //2.11.01
		,itsValueIntervalMax(theRect.itsValueIntervalMax) //2.11.01
//		,itsLoopActivity(theRect.itsLoopActivity)    //2.10.01 -> PressTimeDescriptioniin
		,fAllowMissing(theRect.fAllowMissing)      //21.9.01
		,fUseFromStorage(theRect.fUseFromStorage)   //18.9.01
		,fUseFromStorageConditionally(theRect.fUseFromStorageConditionally)//28.9.01
		,fPutInStorage(theRect.fPutInStorage)     //18.9.01
		,itsStorageQueue(theRect.itsStorageQueue) //28.9.01
		,fIsProbability(theRect.fIsProbability)     //18.9.01
		,itsLevel(theRect.itsLevel)            
		,itsCurrentSegmentTime(theRect.itsCurrentSegmentTime) 
		,itsCurrentTime(theRect.itsCurrentTime)       
		,itsCurrentMultiParNum(theRect.itsCurrentMultiParNum) 
		,itsCombinedIdent(theRect.itsCombinedIdent)
		,itsRealPar(theRect.itsRealPar)       //31.5.01
 ,itsCurrentParamValue(theRect.itsCurrentParamValue)
 ,itsValueFactor(theRect.itsValueFactor)
		,itsRelRect(theRect.itsRelRect)
 ,fNewScaling(theRect.fNewScaling)
 ,fParamErrorReported(theRect.fParamErrorReported)  
 ,fLevelErrorReported(theRect.fLevelErrorReported)   
 ,fTimeErrorReported(theRect.fTimeErrorReported)      //3.5.02  
 ,itsNumOfMissing(theRect.itsNumOfMissing)     
 ,itsPressParam(theRect.itsPressParam)  
		,itsIntegrationPeriod(theRect.itsIntegrationPeriod)
		,itsFirstExtremRelHour(theRect.itsFirstExtremRelHour) //27.9.01
	    ,itsLastExtremRelHour(theRect.itsLastExtremRelHour)  //27.9.01
		,itsFirstExtremHour(theRect.itsFirstExtremHour)  
	    ,itsLastExtremHour(theRect.itsLastExtremHour)  
		,itsFirstExtremYear(theRect.itsFirstExtremYear)  
	    ,itsLastExtremYear(theRect.itsLastExtremYear)  
		,itsModifier(theRect.itsModifier)      
		,itsAreaModifier(theRect.itsAreaModifier)      //18.5.2001
		,itsValueOption(theRect.itsValueOption)//1.2.01
		,itsIdentPar(theRect.itsIdentPar)     //1.2.01
 ,itsDataIdent(theRect.itsDataIdent)
{
 SetEnvironment(theRect.GetEnvironment());     
 if(theRect.itsMultiMapping)
 {
	 itsMultiMapping = new NFmiMultiParamMapping(*theRect.itsMultiMapping);
	 itsCurrentParamArray = new float[FmiMaxNumOfMappingParams+1]; //vikaa ei arvoille tarvita
	 itsMultiParams = new FmiParameterName[FmiMaxNumOfMappingParams+1];
	 for (int i=0;i<=FmiMaxNumOfMappingParams;i++)
	 {
		itsCurrentParamArray[i] = theRect.itsCurrentParamArray[i];
		itsMultiParams[i] = theRect.itsMultiParams[i];
	 }
 }
 else
	 itsMultiMapping = 0;

 itsLogFile  = theRect.GetLogFile();   //nämä perityt jäsenet kun eivät ole emon copy-
 itsMaxLoopNum = theRect.GetMaxLoopNum(); //konstruktorissa niin olkoon tässä
 itsFirstPlotHours = theRect.itsFirstPlotHours; 
 itsFirstDeltaDays = theRect.itsFirstDeltaDays;  
 itsFirstPlotTime = theRect.itsFirstPlotTime;
 fGivenHoursAreRelative = theRect.fGivenHoursAreRelative; //4.5.00
 itsStringNameTimeFormat = theRect.itsStringNameTimeFormat; //13.10.00
 itsLoopActivity = theRect.itsLoopActivity;
}
//---------------------------------------------------------------------------
NFmiParamRect ::~NFmiParamRect(void)
{
	if(itsMultiMapping)
	{
		delete [] itsCurrentParamArray;
		delete [] itsMultiParams;
		delete itsMultiMapping;
	}
}

//---------------------------------------------------------------------------
FmiBoolean NFmiParamRect::Set(NFmiDataIdent theParam, NFmiRect theRect)
{
	itsDataIdent = theParam; 
	NFmiRect::Set(theRect.TopLeft(), theRect.BottomRight());
	return kTrue;
};
//---------------------------------------------------------------------------
FmiBoolean NFmiParamRect::SetIdent(NFmiDataIdent theParam)
{
	itsDataIdent = theParam; 
	return kTrue;
};
//---------------------------------------------------------------------------
FmiBoolean NFmiParamRect::SetRect(NFmiRect theRect)
{
	NFmiRect::Set(theRect.TopLeft(), theRect.BottomRight());
	return kTrue;
};
//240999---------------------------------------------------------------------------
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
//----------------------------------------------------------------------------
FmiBoolean NFmiParamRect::ReadRemaining(void)  //090299
{
//	float float1, float2;
	long long1, long2;
	NFmiValueString valueString;
	switch(itsIntObject)
	{
		case dParam:  //2.2.01 
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
               itsIdentPar = long1;

			if(itsIdentPar == kFmiDegreeDays)
			{  //tälläkin tavalla sen lisäksi että vain ilmoittaa LämmitystarveLuku
				itsValueOption = kDegreeDays;
				itsIdentPar = kFmiTemperature;
				SetRelModifierTimes(-2, 22); 
				itsModifier = kMean;       
			}

			if(itsIdentPar == 362)  //31.5.01
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
			   *itsLogFile << "*** ERROR: MoniMuunnoksen parametrit toiseen kertaan, hylätään " << endl;
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
				if(ReadLong(long1, kFalse))
					itsMultiParams[i] = (FmiParameterName) long1;
				else
					break;
			}

			itsMultiMapping->NumOfParams(i);

			itsString = itsObject; //Huom uusi tapa käyttää ReadLongia (stringi joka lopettaa ja samalla on seuraava) 
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
		case dSum:	//9.5.2000  
		{
			if(!SetTwo(long1, long2)
				|| long1 > long2)
					*itsLogFile << "*** ERROR: Summa-ajat väärin " << endl;
			else
			{
				SetRelModifierTimes(long1, long2);
				itsModifier = kSum;
			}
			break;
		}
		case dMinimum:	//3.2.2000  
		{
			if(!SetTwo(long1, long2)
				|| long1 > long2)
					*itsLogFile << "*** ERROR: Minimiajat väärin " << endl;
			else
			{
				SetRelModifierTimes(long1, long2);
				itsModifier = kMinimum;
			}
			break;
		}
		case dMaximum:	//3.2.2000  
		{
			if(!SetTwo(long1, long2)
				|| long1 > long2)
					*itsLogFile << "*** ERROR: Maksimiajat väärin " << endl;
			else
			{
				SetRelModifierTimes(long1, long2);
				itsModifier = kMaximum;
			}
			break;
		}
		case dMean:	//3.2.2000  
		{
			if(!SetTwo(long1, long2)
				|| long1 > long2)
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
		case dWeightedMean:	//3.2.2000  
		{
			if(!SetFive(long1, long2
			   ,itsIntegrationPeriod.startWeight,itsIntegrationPeriod.centreWeight,itsIntegrationPeriod.endWeight)
				|| long1 > long2)
					*itsLogFile << "*** ERROR: Painotettu keskiarvo annettu väärin " << endl;
			else
			{
				SetRelModifierTimes(long1, long2);
				itsModifier = kWeightedMean;
			}

			break;
		}
		case dAreaUnion:	//24.9.01  
		{
			itsAreaModifier = kUnion;
			ReadNext();
			break;
		}
		case dAreaSum:	//18.5.01  
		{
			itsAreaModifier = kSum;
			ReadNext();
			break;
		}
		case dAreaMaximum:	//18.5.01  
		{
			itsAreaModifier = kMaximum;
			ReadNext();
			break;
		}
		case dAreaMinimum:	//18.5.01  
		{
			itsAreaModifier = kMinimum;
			ReadNext();
			break;
		}
		case dAreaMean:	//18.5.01  
		{
			itsAreaModifier = kMean;
			ReadNext();
			break;
		}
		case dWindCill:	//31.1.01  
		{
			itsIdentPar = kFmiChillFactor;
			ReadNext();
			break;
		}
		case dDegreeDays:	//31.1.01  
		{
			itsValueOption = kDegreeDays;
			itsIdentPar = kFmiTemperature;//ei tartte erikseen kutsua
			SetRelModifierTimes(-2, 22); // -"-    SA-vrk
			itsModifier = kMean;        //  -"-
			ReadNext();
			break;
		}
		case dFahrenheit:	//31.1.01  
		{
			itsValueOption = kFahrenheit;
//			itsIdentPar = kFmiTemperature; EI, pitää muistakin lämmöistä saada Fahr.(oletus 4)

			ReadNext();
			break;
		}
		case dPutInStorage:	//18.9.01  
		{
			fPutInStorage = true;

			ReadNext();
			break;
		}
		case dUseFromStorage:	//18.9.01  
		{
			fUseFromStorage = true;

			ReadNext();
			break;
		}
		case dStorageQueue:	//28.9.01  
		{
			SetOne(itsStorageQueue);
			if(itsStorageQueue < 1 || itsStorageQueue > 10)
			{
				*itsLogFile << "*** ERROR: Invalid storage queue: " << itsStorageQueue << endl;
				itsStorageQueue = 1;
			}

			break;
		}
		case dUseFromStorageCond:	//28.9.01  
		{
			fUseFromStorageConditionally = true;

			ReadNext();
			break;
		}
		case dProbability:	//18.9.01  
		{
			fIsProbability = true;

			ReadNext();
			break;
		}
		case dAllowMissing:	//21.9.01  
		{
			fAllowMissing = true;

			ReadNext();
			break;
		}
		case dYearPeriod:	//28.11.2000  
		{
			if(!SetTwo(long1, long2) 
			   || long1 > long2	)
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
		case dLevel:  //12.4.00	  
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
		case dIntegrationPeriod:  //14.9.99
		{
			if (!ReadEqualChar())
				break;

			if(ReadOne(itsIntegrationPeriod.period))      
			{
			   *itsDescriptionFile >> itsObject;
			   valueString = itsObject;
			   if(valueString.IsNumeric())  
			   {
				  itsIntegrationPeriod.startWeight = (float)valueString;

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
		case dAcceptanceInterval: //2.11.01
		{
			SetTwo(itsValueIntervalMin, itsValueIntervalMax);
			break;
		}
		case dRelHour:   //8.2.01 vain PressParamille ja Segmentille	  
		{
			*itsLogFile << "*** ERROR: Ei sallittu dataelementeille: " << (char*)itsString << endl;
			ReadNext();
			break;
		}
		/*
		case dTimeTableActive:  //26.9.01	  
		{
			if (!ReadEqualChar())
				break;

			long3 = 1000;
			if(ReadTwo(long1, long2))
			{
			   *itsDescriptionFile >> itsObject;
			   valueString = itsObject;
			   if(valueString.IsNumeric())  
			   {
				  long3 = (long)valueString;

				  *itsDescriptionFile >> itsObject;
				  itsString = itsObject;
				}
				else
				{
				   itsString = valueString;
				}

				if(long1 > 0 && long2 > 0  && long3 > 0  && long1 < long3)
				{
					itsLoopActivity.startIndex = long1;
					itsLoopActivity.step = long2;
					itsLoopActivity.stopIndex = long3;
				}
				else
					*itsLogFile << "*** ERROR: Not valid looping (start,step,stop): "<< long1<< long2<< long3<< endl;
			}


			itsIntObject = ConvertDefText(itsString);

			break;
		}
		case dBypassValue:  //2.10.01	  
		{
			SetOne(itsLoopActivity.bypassValue);
			itsLoopActivity.bypassWithValue = true;

			break;
		}
		*/
		default:
		{
			return NFmiPressTimeDescription::ReadRemaining();  
			break;
		}
	}
	return kTrue;
}
//3.6.02 voidaan käyttää managerin kanssa myös ---------------------------------------------------------------------------
bool NFmiParamRect::UpdateModifierTimes(void)
{
//	itsFirstPlotHours = itsFirstPlotTime.GetHour(); //4.6.02 EI KÄY
	long firstPlotHours = itsFirstPlotTime.GetHour(); //4.6.02
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
//3.5.00---------------------------------------------------------------------------
FmiBoolean NFmiParamRect::SetRelModifierTimes(long startHour, long endHour)
{
	itsFirstExtremRelHour = startHour; //27.9.01 Uudet metodit tarttee
	itsLastExtremRelHour = endHour;  

	return UpdateModifierTimes();  //3.6.02
/* 
	if(fGivenHoursAreRelative)
	{
		itsFirstExtremHour = itsFirstPlotHours + startHour;
		itsLastExtremHour = itsFirstPlotHours + endHour;
	}
	else
	{
		itsFirstExtremHour = startHour;
		itsLastExtremHour = endHour;
	}
	return kTrue;
*/
  }

//---------------------------------------------------------------------------
int NFmiParamRect::ConvertDefText(NFmiString & object) 
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //1.5.00 kaikille sallitaan vapaasti isot/pienet kirjaimet 
	if(lowChar==NFmiString("´relplace") ||            lowChar==NFmiString("osaalue")) //4.2
		return dRelPlace;
	else if(lowChar==NFmiString("level") ||           lowChar==NFmiString("painepinta"))
		return dLevel;
	else if(lowChar==NFmiString("valuefactor")||      lowChar==NFmiString("arvokerroin"))
		return dValueFactor;
	else if(lowChar==NFmiString("placemove") ||       lowChar==NFmiString("paikansiirto")
		         ||lowChar==NFmiString("suhtpaikka")||lowChar==NFmiString("suhteellinenpaikka")) //1.10.01
		return dPlaceMove;
	else if(lowChar==NFmiString("param")  ||          lowChar==NFmiString("parametri"))
		return dParam;
	else if(lowChar==NFmiString("integrationperiod")||lowChar==NFmiString("integrointijakso"))
		return dIntegrationPeriod;
	else if(lowChar==NFmiString("sum")||              lowChar==NFmiString("summa"))//9.2.2000
		return dSum;
	else if(lowChar==NFmiString("maximum")||          lowChar==NFmiString("maksimi"))//3.2.2000
		return dMaximum;
	else if(lowChar==NFmiString("mean")   ||          lowChar==NFmiString("keskiarvo"))
		return dMean;
	else if(lowChar==NFmiString("weightedmean")||     lowChar==NFmiString("painotettukeskiarvo"))
		return dWeightedMean;
	else if(lowChar==NFmiString("minimum")||          lowChar==NFmiString("minimi"))//3.2.2000
		return dMinimum;
	else if(lowChar==NFmiString("areaunion")||        lowChar==NFmiString("alueunioni")
		                                    ||        lowChar==NFmiString("alueyhdistelmä"))//24.9.01
		return dAreaUnion;
	else if(lowChar==NFmiString("areasum")||          lowChar==NFmiString("aluesumma"))//18.5.01
		return dAreaSum;
	else if(lowChar==NFmiString("areamaximum")||      lowChar==NFmiString("aluemaksimi"))//18.5.01
		return dAreaMaximum;
	else if(lowChar==NFmiString("areamean")   ||      lowChar==NFmiString("aluekeskiarvo"))//18.5.01
		return dAreaMean;
	else if(lowChar==NFmiString("areaminimum")||      lowChar==NFmiString("alueminimi"))////18.5.01
		return dAreaMinimum;
	else if(lowChar==NFmiString("yearperiod")||       lowChar==NFmiString("vuosijakso"))//28.11.2000
		return dYearPeriod;
	else if(lowChar==NFmiString("multiparams")||      lowChar==NFmiString("moniparametrit"))
		return dMultiParams;
	else if(lowChar==NFmiString("multimapping")||     lowChar==NFmiString("monimuunnos"))
		return dMultiMapping;
	else if(lowChar==NFmiString("windchill")||     lowChar==NFmiString("pakkasenpurevuus"))
		return dWindCill;
	else if(lowChar==NFmiString("degreeday")||     lowChar==NFmiString("lämmitystarve")
		                                     ||     lowChar==NFmiString("lämmitystarveluku"))
		return dDegreeDays;
	else if                                         (lowChar==NFmiString("fahrenheit") )
		return dFahrenheit;
	else if(lowChar==NFmiString("putinstorage")||     lowChar==NFmiString("laitavarastoon")) //18.9.01
		return dPutInStorage;
	else if(lowChar==NFmiString("usefromstorage")||     lowChar==NFmiString("otavarastosta"))  //18.9.01
		return dUseFromStorage;
	else if(lowChar==NFmiString("storagequeue")||     lowChar==NFmiString("varastostojono"))  //28.9.01
		return dStorageQueue;
	else if(lowChar==NFmiString("usefromstorageconditionally")||lowChar==NFmiString("otavarastostaehdolla")) //28.9.01
		return dUseFromStorageCond;
	else if(lowChar==NFmiString("probability")||     lowChar==NFmiString("todennäköisyys"))  //18.9.01
		return dProbability;
	else if(lowChar==NFmiString("allowmissing")||     lowChar==NFmiString("sallipuuttuvia"))  //21.9.01
		return dAllowMissing;
	else if(lowChar==NFmiString("acceptanceinterval")||     lowChar==NFmiString("hyväksymisväli"))  //2.11.01
		return dAcceptanceInterval;
	else 
		return NFmiPressTimeDescription :: ConvertDefText(object);  
}
//-20.3---------------------------------------------------------------------------
FmiBoolean NFmiParamRect:: PointOnParam(NFmiFastQueryInfo* theQI, NFmiParam* theParam) 
{	//2.5.00 yhdistetty PointOnSecondParam tähän
	//27.3.00 jos kysytään auringon korkeutta se lasketaan eikä haeta datasta
//	itsCurrentPar = GetDataIdent().GetParam()->GetIdent();
	itsCurrentPar = theParam->GetIdent();
	if(itsCurrentPar == kFmiSolarElevation || itsCurrentPar == kFmiChillFactor) //31.1.01
	{
		theQI->FirstParam(); //varmuuden vuoksi
		return kTrue;
	}

    fParamErrorReported = kFalse;  
    itsCombinedIdent = kFmiLastParameter;  // !eli olevinaan puuttuva
//	if (!theQI->Param(*GetDataIdent().GetParam())) //19.8.99 TUOTTAJA POISTETTU
	if (!theQI->Param(*theParam)) 
	{

	// 19.8.99
    // jos ei pelkkänä parametrina niin löytyisikö CombinedParametrina?

/*		NFmiDataIdent dataIdent = theQD->Param();
		unsigned long identNum = dataIdent.GetParam()->GetIdent();
		if (identNum == 20 || identNum == 21)
			itsCombinedParam = 22;
		else if(identNum == 338) 
			itsCombinedParam = 326;
		
		ident.Set
		
		if (!theQD->Param(itsCombinedIdent))
*/
		{
			if(itsLogFile && !fParamErrorReported)
			{
//			   long paramIdent = GetDataIdent().GetParam()->GetIdent();
			   long paramIdent = theParam->GetIdent();
			   *itsLogFile << "  *** ERROR: Parametria ei löydy: " << paramIdent 
						/*<< "/" << prodIdent*/ << endl;
			   fParamErrorReported = kTrue;
			}
			return kFalse;
		}
		return kTrue;
	}
	return kTrue;
}
//-23.9.99---------------------------------------------------------------------------
FmiBoolean NFmiParamRect:: PointOnMultiParam(NFmiFastQueryInfo* theQI, short theNum) 
{ 
	//28.3.00 jos kysytään auringon korkeutta se lasketaan eikä haeta datasta
	itsCurrentPar = itsMultiParams[theNum]; //280300
	if(itsCurrentPar == kFmiSolarElevation || itsCurrentPar == kFmiChillFactor)
	{
		theQI->FirstParam(); //varmuuden vuoksi
		return kTrue;
	}

    fParamErrorReported = kFalse;  
    itsCombinedIdent = kFmiLastParameter;  // onko tarkoitettu tähän?
//	NFmiWeatherAndCloudiness* testComParam = new NFmiWeatherAndCloudiness(theQI->Param(326));
	if (!theQI->Param(NFmiParam((unsigned long) itsMultiParams[theNum]))) 
	{
		{
			if(itsLogFile && !fParamErrorReported)
			{
//			   long paramIdent = GetDataIdent().GetParam()->GetIdent();
			   long paramIdent = (unsigned long) itsMultiParams[theNum]; //12.9.99
			   *itsLogFile << "  *** ERROR: multiParametria ei löydy: " << paramIdent << endl;
			   fParamErrorReported = kTrue;
			}
			return kFalse;
		}
		return kTrue;
	}
	return kTrue;
}
//-12.4.2000---------------------------------------------------------------------------
FmiBoolean NFmiParamRect:: PointOnLevel(NFmiFastQueryInfo* theQI) 
{   //segmentillä voi myös olla level-taulukko, jolloin parametriin ei pidä laittaa mitään
	if(itsLevel.LevelValue() == kFloatMissing)
		return kTrue; 

	//huonoa kun tämä aurinko-testi useammassa paikassa
	itsCurrentPar = GetDataIdent().GetParam()->GetIdent();
	if(itsCurrentPar == kFmiSolarElevation)
	{
		theQI->FirstLevel(); //varmuuden vuoksi
		return kTrue;
	}

    fParamErrorReported = kFalse;  
    itsCombinedIdent = kFmiLastParameter;  
	if (!theQI->Level(itsLevel)) 
	{
//		{
			if(itsLogFile && !fLevelErrorReported)
			{
			   *itsLogFile << "*** ERROR: Painepintaa ei löydy: " << itsLevel.LevelValue() << endl;
			   fLevelErrorReported = kTrue;
			}
			return kFalse;
//		}
//		return kTrue;
	}
	return kTrue;
}
//-090299---------------------------------------------------------------------------
                                                          // 30.8.99 value parametrisoitu
FmiBoolean NFmiParamRect:: ReadCurrentValue(NFmiFastQueryInfo* theQueryInfo, float& value, FmiBoolean localTimeSet)  
{

//	NFmiMetTime saveTime = theQueryInfo->Time(); //16.2.2000 pois, eiköhän alkuasetus groupissa riitä
	if(!localTimeSet)                 //multimappingissä jo asetettu
	{
		 if(!SetStationLocalTime(theQueryInfo)) //1.2.2000 väliaikaisesti jos optio päällä
																 
		 {
//			theQueryInfo->Time(saveTime);  
			return kFalse; 
		 }
	}
//	16.2.2000 ****** muualla ei enää kosketa QInfon aikaan
	if(!theQueryInfo->TimeToNearestStep (itsCurrentTime, kCenter, theQueryInfo->TimeResolution()/2))
	{
		if(!fTimeErrorReported) //3.5.02 tänne joudutaan jos piirtoelementissä muutetaan olemattomaan aikaan
		{
			*itsLogFile << "    WARNING: invalid time: " << itsCurrentTime << " (changed in data element)" << endl;
			fTimeErrorReported = true;
		}
	}

	if(itsPressParam->IsFirstStation() && !(itsMultiMapping && itsCurrentMultiParNum > 1)
		&& itsPressParam->IsStationLocalTimeMode())
	{
		if(itsMultiMapping)
		{
			*itsLogFile << "    eka asema: aika paikan päällä= " << itsCurrentTime << " utc; par=moni" << endl;
		}
		else
		{
			unsigned long par = theQueryInfo->ParamDescriptor().Param().GetParam()->GetIdent();
			*itsLogFile << "    eka asema: aika paikan päällä= " << itsCurrentTime << " utc; par=" << par << endl;
		}
		*itsLogFile << "      ->lähin data-aika= " << ((NFmiFastQueryInfo*)theQueryInfo)->Time() << " utc" << endl;
	}

	FloatValue(theQueryInfo, value);

	if(value != kFloatMissing && itsIdentPar == kFmiPresentWeather //8.2.01 WaWa koodin pot. muunto
	  && value >= 100 && value < 200)
	{
		value = (float)WaWa2PresentWeather((int)value, theQueryInfo);
	}

	if(value == kFloatMissing)                 
	{
		if(!itsMultiMapping)  //24.9.99
			itsNumOfMissing++;

		return kTrue; //kFalse;
	}
	else
	{
		value = (float)(value * itsValueFactor);
		
		return kTrue;
	}
}
//----------------------------------------------------------------------------
int NFmiParamRect::WaWa2PresentWeather(int value, NFmiFastQueryInfo* theData)
{  //Kaisalta kopsattu
	// int intValue = value-100;
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
//----------------------------------------------------------------------------
int NFmiParamRect:: PreWeaWithPrecForm(int value, NFmiFastQueryInfo* theData) 
{ //WaWa koodia varten lämpötila
	int newValue;
	if(!theData->Param(kFmiTemperature))
	{
	  *itsLogFile << "  WARNING: Lämpöä ei ole WaWalle" << endl;
	  return value;
	}
	float temp = theData->FloatValue();
	theData->Param(itsDataIdent);    //Takaisin varsinainen param
	newValue = value;   //7.9.01 alustamaton, OLI BUGI
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
//----------------------------------------------------------------------------
FmiBoolean NFmiParamRect:: FloatValue(NFmiFastQueryInfo* theQueryInfo, float& value)  
{    //1.11.99 piti erottaa jotta kääntäväarvo ei tarttis käyttää itsValueFactor:ia
	// 2.2000 qInfon CalcTimeData(modif..) käyttöön

//	itsPressParam->PutInStorage(20);
//	float testval = itsPressParam->UseFromStorage();

	/*
	if(fUseFromStorage) //18.9.01
	{
		value = itsPressParam->UseFromStorage();
		return kTrue;
	}
*/
	unsigned long par = theQueryInfo->ParamDescriptor().Param().GetParam()->GetIdent();

	if(itsCurrentPar == kFmiSolarElevation) //28.3.00 auringon korkeus metodilla eikä datasta 
	{
		value = SunElevation(theQueryInfo); 
		return kTrue;
	}

	if(itsCurrentPar == kFmiChillFactor)
		par = kFmiChillFactor;

//	NFmiMetTime firstTime = ((NFmiFastQueryInfo*)theQueryInfo)->Time(); //4.9.01
	NFmiMetTime firstTime = ((NFmiFastQueryInfo*)theQueryInfo)->Time(); //16.5.02
	NFmiMetTime lastTime = firstTime;             //2.2.01
	NFmiDataModifier* modif=0;            //4.1.01
	NFmiDataModifier* areaModif=0;       //18.5.01
	bool isExtremeModifier = false;
	unsigned long period = itsIntegrationPeriod.period;
	// unsigned long origPeriod = period;
	   float startWeight;
	   float centreWeight;
	   float endWeight;


	FmiBoolean isWeightedMean = period > 0 && period != kUnsignedLongMissing || itsModifier == kWeightedMean;

	if(itsModifier != kNoneModifier )  //4.2.2000
	{
		firstTime = CalculatePeriodTime(itsFirstExtremHour);
		lastTime = CalculatePeriodTime(itsLastExtremHour);
		period = lastTime.DifferenceInHours(firstTime);  //18.2.2000

		if(itsFirstExtremYear != kLongMissing)    //29.11.00
		{
			firstTime.SetYear(itsFirstExtremYear);
			lastTime.SetYear(itsLastExtremYear);
			//periodilla ei pitäisi olla väliä ??
		}

		if(itsPressParam->IsFirstStation() && !(itsMultiMapping && itsCurrentMultiParNum > 1))
		{
//		    unsigned long ownPar = GetDataIdent().GetParam()->GetIdent(); //ei hyvä

		   *itsLogFile << "      ->aikajakso= " << firstTime << " - " << lastTime << " utc" << endl; 
//		else
//		   *itsLogFile << "    aikajakso: " << firstTime << " - " << lastTime << " utc" << endl; 
		}
		switch(itsModifier)
		{
			case kMinimum:
/*				if(fAllowMissing)          //2.4.02
					modif = new NFmiDataModifierMin(kFmiAdd,true);
				else
		*/			modif = new NFmiDataModifierMin;

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
					modif = new NFmiDataModifierAllValidSum;  //16.5.01 +AllValid
//				if(itsValueIntervalMin != kFloatMissing)  //13.12.01 en uskaltanut laittaa yhteiseksi/Lasse
//					modif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);
				break;
			case kMean:
				if(fAllowMissing)                         //27.3.02
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
		modif = new NFmiDataModifierDummy; //2.2.01


	if(itsValueIntervalMin != kFloatMissing) //27.3.02 yhteiseksi 
		modif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);

	switch(itsAreaModifier)   //18.5.01
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
				areaModif = new NFmiDataModifierSum;     //27.3.02
			else
				areaModif = new NFmiDataModifierAllValidSum;  
//			if(itsValueIntervalMin != kFloatMissing)  //13.12.01 en uskaltanut laittaa yhteiseksi/Lasse
//				areaModif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);
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
//			areaModif = new NFmiDataModifierDummy; 
			break;
	}

	if(areaModif && itsValueIntervalMin != kFloatMissing) //27.3.02 yhteiseksi //18.4.+areaModif
		areaModif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);
	if(modif && itsValueIntervalMin != kFloatMissing) //18.4.02 tarviiko  molemmat modifierit
		modif->SetLimits(itsValueIntervalMin, itsValueIntervalMax);


// 22.2.2000 HUOM: OLETUS ON HETKELLINEN EIKÄ JAKSO 
   if(period == kUnsignedLongMissing /*&& (par == 4 || par == 19 || par == 20 || par == 21 )*/) 
		period = 0;
//	if (isWeightedMean /*period != 0 */) //painotettu keskiarvo 
//   {                                                
	   if (period == kUnsignedLongMissing) //jätetty ohjelman päätettäväksi
	   {
		   if (theQueryInfo->TimeDescriptor().Resolution() == 360)
			   period = 12;
		   else if (theQueryInfo->TimeDescriptor().Resolution() == 180)
			   period = 6;          //3.12.99
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
//		   if(period > 7)
//			   centreWeight = 3;
//		   else
			   centreWeight = 1;   //9.5.00 aina 1, joka tapauksessa vanhentunut
	   }
//	}

	if(period !=0 && itsModifier == kNoneModifier) //18.2.00 kNoneModifier lis. //yhteensopivuus vanhan kanssa
	{
		firstTime = itsCurrentSegmentTime;
		lastTime = itsCurrentSegmentTime;
		firstTime.ChangeByHours((-(long)period)/2);
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
				*itsLogFile << "*** ERROR: longitudi puuttuu " << (char*)itsPressParam->GetCurrentStation().GetName()
						<< ":lta paikallista aikaa varten"<< endl; 

			firstTime = firstTime.LocalTime(-(float)longitude);
			lastTime = lastTime.LocalTime(-(float)longitude);
		}
	}

    //27.9.01 uudet metodit mahdollistava
	bool varEtc = fIsProbability || itsAreaModifier != kNoneModifier && itsModifier != kNoneModifier
		|| itsCurrentPar == 353 && itsAreaModifier == kMaximum /*|| itsAreaModifier == kUnion*/; //HUOM w&c

	if(varEtc)
	{
//		*itsLogFile << " ERROR: varEtc " << endl;
//	}

		NFmiSuperSmartInfo ssinfo(*theQueryInfo);
		ssinfo.AreaUnCertaintyStart(2);//0-> 0 tai 100; olivat 1 3
		ssinfo.AreaUnCertaintyEnd(2);//Nämä ovat vain kokeellisia arvoja, pitäisi tulla oikeasti datan mukana meteorologilta
	//	ssinfo.First();
	//	ssinfo.Location(NFmiLocation(25,60.5));
		NFmiDataModifierMin min;
		NFmiDataModifierMax areaMax;
		NFmiDataModifierMax variationMax;
		NFmiDataModifierAvg mean;
		NFmiDataModifierSum sum; //(minVal, maxVal);
		if (itsValueIntervalMin != kFloatMissing)
			sum.SetLimits(itsValueIntervalMin, itsValueIntervalMax);

		if(fIsProbability) // **** sateen tn *** /
//         ===============
		{
			NFmiDataModifierProb prob(kValueGreaterThanLimit, .11);
	//					NFmiWeatherAndCloudiness weather;
	//					weather.SetPrecipitationProbabilityLimits(0.1f);
	//					NFmiDataModifierCombinedParam combine(weather);

			NFmiRelativeDataIterator variationArea(&ssinfo, 4, 4, 0); //EI  VAIKUTUSTA
			NFmiMaskedDataIterator geographicalArea(&ssinfo); //comb/vai ei (ei tartte)  //tiealueet,vika aikajakso

	//					NFmiCalculator areaMinCalc(&geographicalArea, &min);
	//					NFmiCalculator areaMeanCalc(&geographicalArea, &mean);
			NFmiCalculator areaMaxCalc(&geographicalArea, &areaMax);
	
	//		NFmiCalculator minCalc(&variationArea, &min);
	//		NFmiCalculator maxCalc(&variationArea, &max);
	//		NFmiCalculator meanCalc(&variationArea, &mean);
			
			NFmiCalculator probCalc(&variationArea, &prob);

	//					NFmiCalculator combineCalc(&variationArea, &combine);
			bool useVariation = true;
			bool useCalculator = true;
			bool createVariance = true;
			bool useTimeIntegration = true;
			variationArea.SetDimensions(ssinfo.CalcAreaUnCertainty(), ssinfo.CalcAreaUnCertainty(), 0); //aikavar. vika
	//					ssinfo.Param(NFmiParam(kFmiPrecipitation1h kFmiWindSpeedMS  kFmiPrecipitation1h ));
			ssinfo.Param(NFmiParam(itsCurrentPar));

			ssinfo.AreaMask(itsPressParam->GetAreaMask());
			ssinfo.UseAreaMask(true);
	//					ssinfo.Location(NFmiLocation(25,60.5));

			ssinfo.VariationCalculator(&probCalc);
	//					float RRProb = ssinfo.FloatValue(useVariation, !useCalculator, !createVariance);
	//		ssinfo.SetCalculator(&areaMinCalc);
	//		float areaRRProbMinCalc = ssinfo.FloatValue(useVariation, useCalculator, !createVariance);
	//		ssinfo.SetCalculator(&areaMeanCalc);
	//		float areaRRProbMeanCalc = ssinfo.FloatValue(useVariation, useCalculator, !createVariance);
	
			ssinfo.SetCalculator(&areaMaxCalc);
			float areaRRProbMaxCalc = ssinfo.FloatValue(useVariation, useCalculator,!useTimeIntegration,!createVariance);
			value = areaRRProbMaxCalc;
		}
		// **** tiehallinnon sateen int epävarmuusalueella laajennettuna *** /
		else if(itsCurrentPar == 353 && itsAreaModifier == kMaximum && itsModifier == kNoneModifier) // || itsAreaModifier == kUnion   eikä todenn.) 
//         ===============
        {
//			ssinfo.AreaUnCertaintyStart(0);//0-> 0 tai 100
//			ssinfo.AreaUnCertaintyEnd(0);//Nämä ovat vain kokeellisia arvoja, pitäisi tulla oikeasti datan mukana meteorologilta
	//		NFmiDataModifierProb prob(kValueGreaterThanLimit, 0.);
	//					NFmiWeatherAndCloudiness weather;
	//					weather.SetPrecipitationProbabilityLimits(0.1f);
	//					NFmiDataModifierCombinedParam combine(weather);

			NFmiRelativeDataIterator variationArea(&ssinfo, 4, 4, 0);
			NFmiMaskedDataIterator geographicalArea(&ssinfo); //comb/vai ei (ei tartte)  //tiealueet,vika aikajakso

	//					NFmiCalculator areaMinCalc(&geographicalArea, &min);
	//					NFmiCalculator areaMeanCalc(&geographicalArea, &mean);
			NFmiCalculator areaMaxCalc(&geographicalArea, &areaMax);
			NFmiCalculator variationMaxCalc(&variationArea, &variationMax);
	
	//		NFmiCalculator minCalc(&variationArea, &min);
	//		NFmiCalculator maxCalc(&variationArea, &max);
	//		NFmiCalculator meanCalc(&variationArea, &mean);
			
//			NFmiCalculator probCalc(&variationArea, &prob);

	//					NFmiCalculator combineCalc(&variationArea, &combine);
			bool useVariation = true;
			bool useCalculator = true;
			bool createVariance = false;
			bool useTimeIntegration = false;

	//		NFmiRelativeTimeIntegrationIterator timeIterator(&ssinfo, itsLastExtremRelHour-itsFirstExtremRelHour+1, -itsLastExtremRelHour);
	//		NFmiCalculator sumCalc(&timeIterator, &sum);
			variationArea.SetDimensions(ssinfo.CalcAreaUnCertainty(), ssinfo.CalcAreaUnCertainty(), 0); //aikavar. vika
	//					ssinfo.Param(NFmiParam(kFmiPrecipitation1h kFmiWindSpeedMS kFmiPrecipitation1h));
			ssinfo.Param(NFmiParam(itsCurrentPar));  //paramhan on asetettu???

			ssinfo.AreaMask(itsPressParam->GetAreaMask());
			ssinfo.UseAreaMask(true);
	//					ssinfo.Location(NFmiLocation(25,60.5));

//			ssinfo.VariationCalculator(&probCalc);
			ssinfo.VariationCalculator(&variationMaxCalc);
	//					float RRProb = ssinfo.FloatValue(useVariation, !useCalculator, !createVariance);
	//		ssinfo.SetCalculator(&areaMinCalc);
	//		float areaRRProbMinCalc = ssinfo.FloatValue(useVariation, useCalculator, !createVariance);
	//		ssinfo.SetCalculator(&areaMeanCalc);
	//		float areaRRProbMeanCalc = ssinfo.FloatValue(useVariation, useCalculator, !createVariance);
	
			ssinfo.SetCalculator(&areaMaxCalc);
//			float areaRRProbMaxCalc = ssinfo.FloatValue(useVariation, useCalculator,useTimeIntegration,!createVariance);

//			ssinfo.TimeIntegrationCalculator(&sumCalc);
			value = ssinfo.FloatValue(useVariation, useCalculator,useTimeIntegration,createVariance);
		}

		// **** tiehallinnon sadesummat **** /
		else if (itsAreaModifier != kNoneModifier && itsModifier != kNoneModifier ) 
		{
//			ssinfo.AreaUnCertaintyStart(0);//0-> 0 tai 100
//			ssinfo.AreaUnCertaintyEnd(0);//Nämä ovat vain kokeellisia arvoja, pitäisi tulla oikeasti datan mukana meteorologilta
			NFmiDataModifierProb prob(kValueGreaterThanLimit, 0.);
	//					NFmiWeatherAndCloudiness weather;
	//					weather.SetPrecipitationProbabilityLimits(0.1f);
	//					NFmiDataModifierCombinedParam combine(weather);

			NFmiRelativeDataIterator variationArea(&ssinfo, 4, 4, 0);
			NFmiMaskedDataIterator geographicalArea(&ssinfo); //comb/vai ei (ei tartte)  //tiealueet,vika aikajakso

	//					NFmiCalculator areaMinCalc(&geographicalArea, &min);
	//					NFmiCalculator areaMeanCalc(&geographicalArea, &mean);
			NFmiCalculator areaMaxCalc(&geographicalArea, &areaMax);
			NFmiCalculator variationMaxCalc(&variationArea, &variationMax);
	
	//		NFmiCalculator minCalc(&variationArea, &min);
	//		NFmiCalculator maxCalc(&variationArea, &max);
	//		NFmiCalculator meanCalc(&variationArea, &mean);
			
			NFmiCalculator probCalc(&variationArea, &prob);

	//					NFmiCalculator combineCalc(&variationArea, &combine);
			bool useVariation = true;
			bool useCalculator = true;
			bool createVariance = true;
			bool useTimeIntegration = itsModifier != kNoneModifier; //oik. aikamodifieri

			NFmiRelativeTimeIntegrationIterator timeIterator(&ssinfo, itsLastExtremRelHour-itsFirstExtremRelHour+1, -itsLastExtremRelHour);
			NFmiCalculator sumCalc(&timeIterator, &sum);
			variationArea.SetDimensions(ssinfo.CalcAreaUnCertainty(), ssinfo.CalcAreaUnCertainty(), 0); //aikavar. vika
	//					ssinfo.Param(NFmiParam(kFmiPrecipitation1h kFmiWindSpeedMS kFmiPrecipitation1h));
			ssinfo.Param(NFmiParam(itsCurrentPar));  //paramhan on asetettu???

			ssinfo.AreaMask(itsPressParam->GetAreaMask());
			ssinfo.UseAreaMask(true);
	//					ssinfo.Location(NFmiLocation(25,60.5));

//			ssinfo.VariationCalculator(&probCalc);
			ssinfo.VariationCalculator(&variationMaxCalc);
	//					float RRProb = ssinfo.FloatValue(useVariation, !useCalculator, !createVariance);
	//		ssinfo.SetCalculator(&areaMinCalc);
	//		float areaRRProbMinCalc = ssinfo.FloatValue(useVariation, useCalculator, !createVariance);
	//		ssinfo.SetCalculator(&areaMeanCalc);
	//		float areaRRProbMeanCalc = ssinfo.FloatValue(useVariation, useCalculator, !createVariance);
	
			ssinfo.SetCalculator(&areaMaxCalc);
//			float areaRRProbMaxCalc = ssinfo.FloatValue(useVariation, useCalculator,useTimeIntegration,!createVariance);

			ssinfo.TimeIntegrationCalculator(&sumCalc);
			value = ssinfo.FloatValue(!useVariation, useCalculator,useTimeIntegration,!createVariance);
		}

	}
	

	else
	{
	if(theQueryInfo->IsGrid())
	{
	    if (isWeightedMean  //ajan suhteen
			|| period > 0 &&
//			|| origPeriod > 0 && origPeriod != kLongMissing &&
			  (par == 336 || par == 326 || par == 19 || par == 271 || par == 57
			|| par == 353 || par == 375)    //17.10.00 topWeatherAndCloudiness
			&& (itsModifier == kNoneModifier  //9.5.00
			    || itsModifier == kMean)) //8.12.00 olomuodon ka tulee flotarina modifierista
	   {                                                
		   	value = (float)theQueryInfo->InterpolatedTimePeriodFloatValue(itsPressParam->GetCurrentStation().GetLocation(),
			                                 firstTime,lastTime,startWeight,centreWeight,endWeight); 

//		   value = (float)theQueryInfo->InterpolatedTimePeriodFloatValue(itsPressParam->GetCurrentStation().GetLocation(),
//			                                 period,startWeight,centreWeight,endWeight); 
	   }
	   else //ei painotettu aikakeskiarvo 
	   {
		  if(itsModifier != kNoneModifier)  
		  {        //pitäis ehkä olla optio Interpoloi/älä interp.
			 if(par == kFmiChillFactor)
			 {    //pitää olla modif
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
					((NFmiSuperSmartInfo*)theQueryInfo)->AreaMask(itsPressParam->GetAreaMask());
					((NFmiSuperSmartInfo*)theQueryInfo)->UseAreaMask(true);

					//theQueryInfo->AreaMask(itsPressParam->GetAreaMask());
					//theQueryInfo->UseAreaMask(true);
				 }

				theQueryInfo->CalcInterpolatedTimeData(modif, firstTime, lastTime,itsPressParam->GetCurrentStation().GetLocation());
				if(isExtremeModifier)
					itsPressParam->SetOptionTime(((NFmiDataModifierExtreme*)modif)->GetTime());
				value = modif->CalculationResult();
			 }
		  }
		  else
			if(par == kFmiChillFactor)
			{    
				NFmiDataModifierWindChill wcMod(theQueryInfo, modif,itsPressParam->GetCurrentStation().GetLocation()); 
//				NFmiTime time = theQueryInfo->Time();
				theQueryInfo->CalcTimeData(&wcMod, firstTime, lastTime);
				value = wcMod.CalculationResult();
			}
			else
			{
/*			
				// TESTI ALKAA
				NFmiBitMask newMask(2100);
			    newMask.Mask(1, kTrue);
			    newMask.Mask(2, kTrue);
			    newMask.Mask(3, kTrue);
			    newMask.Mask(4, kTrue);
			    newMask.Mask(31, kTrue);
			    newMask.Mask(32, kFalse); //HUOM
			    newMask.Mask(33, kTrue);
			    newMask.Mask(34, kTrue);
				FmiBoolean test1 = newMask.IsMasked(1215);
				FmiBoolean test2 = newMask.IsMasked(1218);
	   			theQueryInfo->StaticDataMask(newMask, kTrue);
//				theQueryInfo->UseStaticMask(kTrue);
//				theQueryInfo->Location(itsPressParam->GetCurrentStation().GetLocation());

//				NFmiPolSterArea area(NFmiPoint(25.46,67.97),NFmiPoint(26.85,67.58));
				NFmiPolSterArea area(NFmiPoint(19.23,57.85),NFmiPoint(20.16,58.3),25.);

				modif = new NFmiDataModifierAllValidSum;  
//				theQueryInfo->CalcLocationData(modif);
				theQueryInfo->CalcLocationData(modif, &area);
				value = modif->CalculationResult();

				NFmiDataIterator dataIter(theQueryInfo);				
				NFmiCalculator calc(&dataIter, modif);
				float value2 = calc.FloatValue();
				int iii = 1;
				
//			    value = theQueryInfo->FloatValue();
			    // TESTI LOPPU
*/				

				if(fIsProbability)  //20.9.01
				{

				}
				else if(itsPressParam->IsAreaOperation())  //18.5.01
				{
					if(areaModif)
					{
						if(itsPressParam->GetAreaMask() > 0)  //13.9.01
						{ 
							//oikeastaan turha testi nyt jos aina SupersmartInfo
							if (NFmiString(theQueryInfo->ClassName()) != NFmiString("NFmiSuperSmartInfo"))
							{    // VAATISI ETTÄ INFO VÄLITETTÄISIIN KOKO KETJUN LÄPI POINTTERIREFERENSSINÄ
								//NFmiFastQueryInfo* ssInfo = new NFmiSuperSmartInfo(*theQueryInfo);
								//delete theQueryInfo;
								//theQueryInfo = ssInfo;
								*itsLogFile << "***ERROR: data should be in SuperSmartInfo" << endl;
							}
							else
							
							{
								((NFmiSuperSmartInfo*)theQueryInfo)->AreaMask(itsPressParam->GetAreaMask());
								((NFmiSuperSmartInfo*)theQueryInfo)->UseAreaMask(true);
								((NFmiSuperSmartInfo*)theQueryInfo)->CalcLocationData(areaModif); 
								((NFmiSuperSmartInfo*)theQueryInfo)->UseAreaMask(false);
							}

//							if(theQueryInfo->AreaMask() == 0)
/*								 theQueryInfo->AreaMask(itsPressParam->GetAreaMask());
							theQueryInfo->UseAreaMask(true);
							theQueryInfo->CalcLocationData(areaModif); 
							theQueryInfo->UseAreaMask(false); 
							*/
						}
						else
						{            //meneekö tvalliseen QI:oon
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
	{  /***** HUOM 9.12.00: pitäisi varmaan estää modifierin käyttö aikakeskiarvon kanssa
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
			if(itsModifier != kNoneModifier)  //4.2.2000
			{    //2.3.00
				theQueryInfo->CalcTimeData(modif, firstTime, lastTime);
				if(isExtremeModifier)
					itsPressParam->SetOptionTime(((NFmiDataModifierExtreme*)modif)->GetTime());

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
		if(/*value == kFloatMissing && */ storedValue != kFloatMissing && storedValue < 105) //HUOM niinkauan kun pohjoisin suomi puuttuu tutkasateista
		{
			value = storedValue;
			*itsLogFile << "   * INFO: "<< "stored value used instead of actual"  << endl; 
        }
	}

	if(itsValueOption == kFahrenheit)
		value = FmiCelsius2Fahrenheit(value);
	else if(itsValueOption == kDegreeDays)
		//value = FmiDegreeDays(value, ((NFmiFastQueryInfo*)theQueryInfo)->Time().GetMonth()); //4.9.01 ??
		value = FmiDegreeDays(value, (theQueryInfo)->Time().GetMonth()); //16.5.02
	if(itsRealPar == 362) // 300301 minimituuli
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

	return kTrue;
}
/*
//-1.2.01---------------------------------------------------------------------------
float NFmiParamRect:: Celsius2Fahrenheit(float value)
{
	return value == kFloatMissing ? kFloatMissing : value * 1.8f + 32.f; 
}
//-1.2.01---------------------------------------------------------------------------
float NFmiParamRect:: DegreeDays(float value, int month)
//sisään vrk:n keskilämpötila
// huom eri kynnysarvo alku/loppuvuodelle (säteilyn vaikutus)
{
	if(value == kFloatMissing)
		return kFloatMissing;
	else if(value  > 12. || value  > 10. && month < 7)
		return 0; 
	else 
		return 17.f - value;
}
*/	
//-280300---------------------------------------------------------------------------
float NFmiParamRect:: SunElevation(NFmiFastQueryInfo* theQueryInfo)
{
	NFmiLocation location;
	if(theQueryInfo->IsGrid())  //30.3.00
	{
		location = itsPressParam->GetCurrentStation();
	}
	else
		location = *(theQueryInfo->Location());

	return (float)location.ElevationAngle(((NFmiFastQueryInfo*)theQueryInfo)->Time());
}
//-090200---------------------------------------------------------------------------
NFmiMetTime NFmiParamRect:: CalculatePeriodTime(long theHour)
{
	NFmiMetTime time(itsPressParam->GetFirstPlotTime()); //8.1.01 NFmiTime->MetTime (LocalTime(long) ei toiminut)
//	NFmiMetTime time(itsPressParam->GetFirstPlotTime()); 
							           
	long hour = theHour;

	if(theHour > 23)
	{
//		time.ChangeByDays(1);
//		hour -= 24;
		long days = theHour/24; //9.5.00 aikasarjat ulotettu yli yhden vuorokauden siirtymistä
		time.ChangeByDays(days);
		hour -= days*24;
	}
	else if(theHour < 0)
	{
//		time.ChangeByDays(-1);
//		hour += 24;
/*		long days = theHour/24; 
		time.ChangeByDays(-days);
		hour += days*24;
*/
		long days = (-23+theHour)/24; 
		time.ChangeByDays(days);
		hour = 24 + hour; //hour on neg.

	}
	time.SetHour((short)hour);
	long diff = itsCurrentSegmentTime.DifferenceInHours(itsPressParam->GetFirstPlotTime());
	time.ChangeByHours(diff); //16.2.2000

	if(itsPressParam->IsStationLocalTimeMode())
	{
		double longitude = itsPressParam->GetCurrentStation().GetLongitude();
		double latitude = itsPressParam->GetCurrentStation().GetLatitude();//Testiin
		if(fabs(longitude) <= .001 && fabs(latitude) <= .001)   //24.5.00 abs lisätty
			*itsLogFile << "*** ERROR: longitudi puuttuu " << (char*)itsPressParam->GetCurrentStation().GetName()
					<< ":lta paikallista aikaa varten"<< endl; 

		time = time.LocalTime(-(float)longitude);

	}
	return time;
}
//-230999---------------------------------------------------------------------------
FmiBoolean NFmiParamRect:: ReadCurrentValueArray(NFmiFastQueryInfo* theQI)
{
//	NFmiCombinedParam* combinedParam = 0; //Yritetään ottaa kaikki parametrit tästä
	                                      // kun kerran paljon aikaa on mennyt sen laskemiseen

	if(!SetStationLocalTime(theQI)) //1.2.2000 väliaikaisesti jos optio päällä
	{
		return kFalse;
	}
//	NFmiCombinedParam* combPar;
//	FmiBoolean singleTime = itsIntegrationPeriod.period == 0 || itsIntegrationPeriod.period == kLongMissing;
	for (int i = 0; (int)itsMultiParams[i] != (int)kFmiLastParameter; i++) 
	{
		itsCurrentMultiParNum = i+1; //15.2.2000

    //Yritetään ottaa kaikki parametrit kerran lasketusta combinedParamista
	// kun kerran paljon aikaa on mennyt sen kostruoimiseen
	// ei kannata käyttää ristiin tuuli ja weatherAndCloudiness-parametreja monimuunnos-määrittelyssä 
    // 29.5.00 combpar ei ok jos esim ei aikainterp ja ensin pilvet ja sitten ukkonen
//		combPar = const_cast<NFmiCombinedParam*>(theQI->CombinedParam());
//		if(i > 0 && combPar && combPar->IsMemberParam(itsMultiParams[i])
//			&& !singleTime)   //29.5.00 jos yksittäistä aikaa QD:ssä ei vältt. combinedPar
//		{
//			itsCurrentParamArray[i] = (float)combPar->SubValue(itsMultiParams[i]);
//		}

//		else 
//		{
			if(!PointOnMultiParam(theQI, i))
			{
				return kFalse;
			}

			float value;

	// osaparametrit saavat puuttua
	// jos signifikantti puuttuu multiMapping ilmoittaa

			ReadCurrentValue(theQI, value, kTrue); //15.2.00 =local time jo asetettu

			itsCurrentParamArray[i] = value;

	//		itsCombinedParam = new NFmiCombinedParam(*theQI->CombinedParam()); //30.3.00

			if(i>FmiMaxNumOfMappingParams) //varmuuden vuoksi
			{
				*itsLogFile << "  *** ERROR: moniParametriluvussa ylivuoto" << endl;
				return kFalse;
			}
//		}
	}
//	theQI->Time(saveTime); //1.2.2000 takaisin utc-ajaksi 
	return kTrue;
}
//-130899---------------------------------------------------------------------------

//EI TARVITA
FmiInterpolationMethod NFmiParamRect:: InterpolationMethod(NFmiFastQueryInfo* theQueryInfo)
{
	unsigned long ident = theQueryInfo->Param().GetParam()->GetIdent();
	if(ident == 338 || ident == 20)
		return kNearestPoint;
	return kLinearly;
}

//-1.2.2000---------------------------------------------------------------------------
FmiBoolean NFmiParamRect:: SetStationLocalTime(NFmiFastQueryInfo* theQI)
{  //optio
	if(itsPressParam->IsStationLocalTimeMode())  
	{
      //HUOM sama koodi ylläpisettävä NFmiTimeParamRect:ssä
		double longitude = itsPressParam->GetCurrentStation().GetLongitude();
		double latitude = itsPressParam->GetCurrentStation().GetLatitude();//Testiin
		if(fabs(longitude) <= .001 && fabs(latitude) <= .001) //24.5.00 +fabs 
			*itsLogFile << "*** ERROR: longitudi puuttuu " << (char*)itsPressParam->GetCurrentStation().GetName()
			            << ":lta paikallista aikaa varten"<< endl; 

		itsCurrentTime = itsCurrentTime.LocalTime(-(float)longitude);
	}
	return kTrue;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiParamRect::SetRelativeHour(NFmiFastQueryInfo* data, const NFmiString& calledFrom) //23.8.99 oli QD
// 16.2.2000 tällä hoidetaan se että alle vuorokaudenkin aikaluupit onnistuvat alkioiden omilla tunneilla
{
   long test = RelativeHour(); //HUOM
   if(RelativeHour() != 0)
   {
	   itsCurrentTime.ChangeByHours(RelativeHour());
	   if(itsPressParam->IsFirstStation())
	   {
		   if(itsPressParam->IsStationLocalTimeMode())  //18.9.00
	         *itsLogFile << "    tunti muutettu " << (char*)calledFrom << ":ssa: " << itsFirstPlotHours << " pa" << endl;
		   else
		     *itsLogFile << "    tunti muutettu " << (char*)calledFrom << ":ssa: " << itsFirstPlotHours << " utc" << endl;
	   }
	   }
   return kTrue;
}
//---------------------------------------------------------------------------
long NFmiParamRect::RelativeHour(void)const //16.5.2000
// 16.2.2000 tällä hoidetaan se että alle vuorokaudenkin aikaluupit onnistuvat alkioiden omilla tunneilla
{
   return itsFirstPlotHours - itsPressParam->GetFirstPlotHours();
}
/*
//---------------------------------------------------------------------------
bool NFmiParamRect::ActiveTimeIndex(int currentInd)const
{   //halutessa olioita voidaan piirtää esim joka n:teen aika-askeleeseen
	if(itsLoopActivity.startIndex < 1 || (currentInd-itsLoopActivity.startIndex)
		      % itsLoopActivity.step == 0
			  && currentInd >= itsLoopActivity.startIndex
			  && currentInd <= itsLoopActivity.stopIndex
//			  && itsPressParam->GetCurrentStep() <= itsLoopActivity.stopIndex
	  ) return true;
	return false;
}
*/
/* ei kun writePS jäseneksi ja sieltä käyttöön
//---------------------------------------------------------------------------
//24.10.00 tuli myös WritePS:ään !!!
NFmiString NFmiParamRect::AlignmentToMeta (const FmiDirection& thePressAlignment)const  //24.10.00
{
	NFmiString retString;
	switch (thePressAlignment)
	{
	case kCenter:
		{
			retString = NFmiString("Center");
			break;
		}
	case kRight:
		{
			retString = NFmiString("East");
			break;
		}
	case kLeft:
		{
			retString = NFmiString("West");
			break;
		}
	}
	return retString;
}
*/
