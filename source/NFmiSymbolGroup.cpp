// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiSymbolGroup
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

// press
#include "NFmiSymbolGroup.h"
#include "NFmiSymbolParamRect.h"
#include "NFmiNumberParamRect.h"
#include "NFmiRotatingParamRect.h"
#include "NFmiLetterParamRect.h"
#include "NFmiTimeParamRect.h"
#include "NFmiSubstituteParamRect.h"
#include "NFmiPressText.h"
#include "NFmiPsSymbol.h"
#include "NFmiSunTimeParamRect.h"
#include "NFmiScalingParamRect.h"
#include "NFmiPressParam.h"
#include "NFmiExtremeTimeParamRect.h"
#include "NFmiExtremePlaceParamRect.h"
// newbase
#include "NFmiSuperSmartInfo.h"
// system
#include <iostream>

using namespace std;

//! Undocumented
enum NFmiSymbolGroupObjects
{
  dGroupSize = 120,
  dSymbol,
  dConstSymbol,
  dConstText,
  dNumber,		
  dRotating,
  dWindDirection,
  dWindSpeed,
  dLetter,
  dTime,
  dExtremeTime = 1120,
  dSunTime,
  dScaling,
  dSubstitute,
  dExtremePlace
};


// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal variables
 * \todo The ifs are unnecessary
 */
// ----------------------------------------------------------------------

NFmiSymbolGroup::~NFmiSymbolGroup(void)
{
  if(itsParamRects) delete [] static_cast<NFmiParamRect **>(itsParamRects);
  if(itsRectScale) delete static_cast<NFmiRectScale *>(itsRectScale);
  if(itsPsSymbol) delete itsPsSymbol;
  itsPressScalingObjects.Clear(isTrue);
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param pressParam Undocumented
 * \param theItemList Undocumented
 * \param numOfItems Undocumented
 */
// ----------------------------------------------------------------------

NFmiSymbolGroup::NFmiSymbolGroup(NFmiPressParam * pressParam,
								 NFmiParamRect ** theItemList,
								 int numOfItems)
  : NFmiSize(numOfItems)
  , itsPressParam(pressParam)
  , itsPsSymbol(0)
  , itsRectSize(NFmiPoint(40,40))
  , itsRectScale(0)
  , itsQueryData(0)
  , itsQueryDataIter(0)
{
  itsParamRects = new NFmiParamRect*[numOfItems];
  for(int i=0; i < static_cast<int>(GetSize()); i++)
	itsParamRects[i] = theItemList[i];
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRectScale Undocumented
 * \param theQueryDataIter Undocumented
 * \param theDestinationFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolGroup::Set(NFmiRectScale & theRectScale,
						  NFmiFastQueryInfo * theQueryDataIter ,
						  ofstream & theDestinationFile)
{
  if(itsRectScale) delete static_cast<NFmiRectScale *>(itsRectScale);
  itsRectScale = new NFmiRectScale(theRectScale);
  itsQueryDataIter = theQueryDataIter;
  itsOutFile = &theDestinationFile;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolGroup::SetAllTimes(const NFmiMetTime & theTime)
{
  SetTime(theTime); //varmaan turha
  
  for(int i=0; i < static_cast<int>(GetSize()); i++)
	{
	  itsParamRects[i]->SetTime(theTime);
	  itsParamRects[i]->UpdateModifierTimes();
	}
  
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolGroup::SetAllLanguages(FmiLanguage theLanguage)
{
  itsLanguage = theLanguage;
  
  for(int i=0; i < static_cast<int>(GetSize()); i++)
	{
	  itsParamRects[i]->SetLanguage(theLanguage);
	}
  
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theParamRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolGroup::Add(const NFmiParamRect & theParamRect)
{
  NFmiParamRect ** tempRects;
  tempRects = new NFmiParamRect*[itsSize + 1];
  int j;
  for(j=0; j<static_cast<int>(itsSize); j++)
	tempRects[j] = itsParamRects[j];
  tempRects[j] = theParamRect.Clone();
  
  if(itsParamRects)
	delete [] static_cast<NFmiParamRect **>(itsParamRects);
  
  itsParamRects = new NFmiParamRect*[itsSize+1];
  itsSize = itsSize+1;
  for(j=0; j< static_cast<int>(itsSize); j++)
	itsParamRects[j] = tempRects[j];
  
  delete [] static_cast<NFmiParamRect **>(tempRects);
  return true; //doesn't compare if already in the list ->true is returned as a default.

}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

long NFmiSymbolGroup::NumOfMissing(void) const
{
  long num = 0;
  for(int i=0; i < static_cast<int>(GetSize()); i++)
	{
	  num += itsParamRects[i]->NumOfMissing();
	}
  
  return num;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSymbolGroup::InitMissing(void)
{
	for(int i=0; i < static_cast<int>(GetSize()); i++)
	  {
		itsParamRects[i]->InitMissing();
	  }
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool  NFmiSymbolGroup::ReadDescription(NFmiString & retString)
{
  NFmiValueString valueString;
  double x, y;

  NFmiPressScaling * psObject;
  bool sizeGiven = false;
  bool isWindSpeed, isWindDirection;

  ReadNext();
  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: max file length exceeded in #Parameters" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum ++;
	  if (itsIntObject != dEndComment && itsCommentLevel)
		itsIntObject = dComment;

	  isWindSpeed = isWindDirection = false;
	  
	  switch(itsIntObject)
		{
		case dOther:	  //ylimääräistä roinaa, End.. lopettaa
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Unknown word in #Parameters: "
						  << static_cast<char *>(itsObject)
						  << endl;
			ReadNext();
			break;
		  }
		case dComment:
		  {
			ReadNext();
			break;
		  }
		case dEndComment:
		  {
			ReadNext();
			break;
		  }
		case dGroupSize:
		  {
			if (!ReadEqualChar())
			  break;
			
			if(ReadDouble(x))
			  {
				*itsDescriptionFile >> itsObject;
				valueString = itsObject;
				if(valueString.IsNumeric())
				  {
					y = static_cast<double>(valueString);
					*itsDescriptionFile >> itsObject;
					itsString = itsObject;
				  }
				else
				  {
					y = x;
					itsString = valueString;
				  }
				
				itsRectSize.Set(x,y);
			  }
			else
			  {
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
			  }
			
			itsIntObject = ConvertDefText(itsString);
			sizeGiven = true;
			break;
		  }
		case dSymbol:
		  {
			NFmiSymbolParamRect tempSPar;
			tempSPar.SetEnvironment(itsEnvironment);
			tempSPar.SetPressParam(itsPressParam);
			tempSPar.SetHome(GetHome());
			tempSPar.SetLogFile(itsLogFile);
			tempSPar.SetDescriptionFile(itsDescriptionFile);
			tempSPar.SetTime(itsFirstPlotTime);
			tempSPar.SetHourLoop(IsHourLoop());
			tempSPar.SetNewScaling(!sizeGiven);
			if(tempSPar.ReadDescription(itsString))
			  Add(tempSPar);
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dConstSymbol:
		  {
			// #VakioSymboli tekee nyt SymbolParamRect jossa ohitetaan data ja mappaus
			NFmiSymbolParamRect tempCSPar;
			tempCSPar.SetConstSymbol(true);  //HUOM
			tempCSPar.SetPressParam(itsPressParam);
			tempCSPar.SetHome(GetHome());
			tempCSPar.SetLogFile(itsLogFile);
			tempCSPar.SetDescriptionFile(itsDescriptionFile);
			tempCSPar.SetTime(itsFirstPlotTime);
			tempCSPar.SetHourLoop(IsHourLoop());
			tempCSPar.SetNewScaling(!sizeGiven);
			if(tempCSPar.ReadDescription(itsString))
			  Add(tempCSPar);
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dConstText:
		  {
			psObject = new NFmiPressText;
			psObject->SetHome(GetHome());
			psObject->SetLogFile(itsLogFile);
			psObject->SetDescriptionFile(itsDescriptionFile);
			psObject->SetSize(itsRectSize);
			if(psObject->ReadDescription(itsString))
			  itsPressScalingObjects.Add(psObject);
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dWindSpeed:
              isWindSpeed = true;
		case dNumber: 
			{
			  NFmiNumberParamRect tempNPar;
			  if(isWindSpeed)
			  {
				tempNPar.SetIdentPar(kFmiWindSpeedMS);
			  }
			  tempNPar.SetPressParam(itsPressParam);
			  tempNPar.SetEnvironment(itsEnvironment);
			  if(isWindSpeed)
			  {
			    tempNPar.SetTextAlignment(kCenter);
			  }
			  tempNPar.SetHome(GetHome());
			  tempNPar.SetLogFile(itsLogFile);
			  tempNPar.SetDescriptionFile(itsDescriptionFile);
			  tempNPar.SetTime(itsFirstPlotTime);
			  tempNPar.SetHourLoop(IsHourLoop());
			  tempNPar.SetNewScaling(!sizeGiven);
			  if(tempNPar.ReadDescription(itsString))
				Add(tempNPar);
			  itsIntObject = ConvertDefText(itsString);
			  break;
			}
		case dTime:
		  {
			NFmiTimeParamRect tempTPar;
			tempTPar.SetPressParam(itsPressParam);
			tempTPar.SetEnvironment(itsEnvironment);
			tempTPar.SetHome(GetHome());
			tempTPar.SetLanguage(itsLanguage);
			tempTPar.SetLogFile(itsLogFile);
			tempTPar.SetTime(itsFirstPlotTime);
			tempTPar.SetDescriptionFile(itsDescriptionFile);
			tempTPar.SetHourLoop(IsHourLoop());
			tempTPar.SetNewScaling(!sizeGiven);
			if(tempTPar.ReadDescription(itsString))
			  Add(tempTPar);
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dExtremeTime:
		  {
			NFmiExtremeTimeParamRect tempETPar;
			tempETPar.SetPressParam(itsPressParam);
			tempETPar.SetEnvironment(itsEnvironment);
			tempETPar.SetHome(GetHome());
			tempETPar.SetLanguage(itsLanguage);
			tempETPar.SetLogFile(itsLogFile);
			tempETPar.SetDescriptionFile(itsDescriptionFile);
			tempETPar.SetTime(itsFirstPlotTime);
			tempETPar.SetHourLoop(IsHourLoop());
			tempETPar.SetNewScaling(!sizeGiven);
			if(tempETPar.ReadDescription(itsString))
			  Add(tempETPar);
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dSunTime:
		  {
			NFmiSunTimeParamRect tempSTPar;
			tempSTPar.SetPressParam(itsPressParam);
			tempSTPar.SetEnvironment(itsEnvironment);
			tempSTPar.SetHome(GetHome());
			tempSTPar.SetLanguage(itsLanguage);
			tempSTPar.SetLogFile(itsLogFile);
			tempSTPar.SetDescriptionFile(itsDescriptionFile);
			tempSTPar.SetTime(itsFirstPlotTime);
			tempSTPar.SetHourLoop(IsHourLoop());
			tempSTPar.SetNewScaling(!sizeGiven);
			if(tempSTPar.ReadDescription(itsString))
			  Add(tempSTPar);
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dLetter:
		  {
			NFmiLetterParamRect tempLPar;
			tempLPar.SetPressParam(itsPressParam);
			tempLPar.SetEnvironment(itsEnvironment);
			tempLPar.SetHome(GetHome());
			tempLPar.SetLogFile(itsLogFile);
			tempLPar.SetDescriptionFile(itsDescriptionFile);
			tempLPar.SetTime(itsFirstPlotTime);
			tempLPar.SetHourLoop(IsHourLoop());
			tempLPar.SetNewScaling(!sizeGiven);
			if(tempLPar.ReadDescription(itsString))
			  Add(tempLPar);
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dExtremePlace:
		  {
			NFmiExtremePlaceParamRect tempEPPar;
			tempEPPar.SetPressParam(itsPressParam);
			tempEPPar.SetEnvironment(itsEnvironment);
			tempEPPar.SetHome(GetHome());
			tempEPPar.SetLogFile(itsLogFile);
			tempEPPar.SetDescriptionFile(itsDescriptionFile);
			tempEPPar.SetTime(itsFirstPlotTime);
			tempEPPar.SetHourLoop(IsHourLoop());
			tempEPPar.SetNewScaling(!sizeGiven);
			if(tempEPPar.ReadDescription(itsString))
			{
			  Add(tempEPPar);
			  itsPressParam->SetStationNotNeeded();
			  itsPressParam->SetReportProseccedNumber(false);
			}
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dSubstitute:
		  {
			NFmiSubstituteParamRect tempSPar;
			tempSPar.SetPressParam(itsPressParam);
			tempSPar.SetEnvironment(itsEnvironment);
			tempSPar.SetHome(GetHome());
			tempSPar.SetLogFile(itsLogFile);
			tempSPar.SetDescriptionFile(itsDescriptionFile);
			tempSPar.SetTime(itsFirstPlotTime);
			tempSPar.SetHourLoop(IsHourLoop());
			//tempSPar.SetNewScaling(!sizeGiven);
			if(tempSPar.ReadDescription(itsString))
			  Add(tempSPar);
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
	    case dWindDirection: 
			isWindDirection = true;
	    case dRotating:
		  {
			NFmiRotatingParamRect tempRPar;
			if(isWindDirection)
			{
				tempRPar.SetNotRotatingMin(-1.);
				tempRPar.SetNotRotatingMax(0.9);
				tempRPar.SetIdentPar(kFmiWindSpeedMS);
				tempRPar.SetSecondDataIdent(kFmiWindDirection);
			}
			tempRPar.SetPressParam(itsPressParam);
			tempRPar.SetEnvironment(itsEnvironment);
			tempRPar.SetHome(GetHome());
			tempRPar.SetLogFile(itsLogFile);
			tempRPar.SetDescriptionFile(itsDescriptionFile);
			tempRPar.SetTime(itsFirstPlotTime);
			tempRPar.SetHourLoop(IsHourLoop());
			tempRPar.SetNewScaling(!sizeGiven);
			if(tempRPar.ReadDescription(itsString))
			  Add(tempRPar);
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
	   case dScaling:
		  {
			NFmiScalingParamRect tempScPar;
			tempScPar.SetPressParam(itsPressParam);
			tempScPar.SetEnvironment(itsEnvironment);
			tempScPar.SetHome(GetHome());
			tempScPar.SetLogFile(itsLogFile);
			tempScPar.SetDescriptionFile(itsDescriptionFile);
			tempScPar.SetTime(itsFirstPlotTime);
			tempScPar.SetHourLoop(IsHourLoop());
			tempScPar.SetNewScaling(!sizeGiven);
			if(tempScPar.ReadDescription(itsString))
			  Add(tempScPar);
			
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		default:
		  {
			ReadRemaining();
			break;
		  }
		}
	  
	} // end of while
  SetScalingMode();
  retString = itsString;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 * \todo pitäisi kai tarkistaa myös muut kuin datariippuvat
 */
// ----------------------------------------------------------------------

void NFmiSymbolGroup::SetScalingMode(void)
{
  // pitäisi kai tarkistaa myös muut kuin datariippuvat
  fNewScaling = false;
  for(int i=0; i < static_cast<int>(GetSize()); i++)
	{
	  if(itsParamRects[i]->IsNewScaling())
		fNewScaling = true;
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiSymbolGroup::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if(lowChar==NFmiString("size") ||
	 lowChar==NFmiString("koko"))
	return dGroupSize;

  else if(lowChar==NFmiString("#symbol") ||
		  lowChar==NFmiString("#symboli") ||
		  lowChar==NFmiString("#kuva"))
	return dSymbol;

  else if(lowChar==NFmiString("#fixedsymbol") ||
		  lowChar==NFmiString("#constantsymbol") ||
		  lowChar==NFmiString("#kiinteäsymboli") ||
		  lowChar==NFmiString("#vakiosymboli") ||
		  lowChar==NFmiString("#vakiokuva"))
	return dConstSymbol;

  else if(lowChar==NFmiString("#text") ||    
	      lowChar==NFmiString("#constanttext") ||
		  lowChar==NFmiString("#vakioteksti"))
	return dConstText;

  else if(lowChar==NFmiString("#number") ||
		  lowChar==NFmiString("#numero"))
	return dNumber;

  else if(lowChar==NFmiString("#letter") ||
		  //lowChar==NFmiString("#text") || = #(constant)text
		  lowChar==NFmiString("#teksti") ||
		  lowChar==NFmiString("#kirjain"))
	return dLetter;

  else if(lowChar==NFmiString("#timetext") ||
		  lowChar==NFmiString("#time") ||
		  lowChar==NFmiString("#aikateksti") ||
		  lowChar==NFmiString("#aika") )
	return dTime;

  else if(lowChar==NFmiString("#extremetime") ||
		  lowChar==NFmiString("#ääriarvoaika"))
	return dExtremeTime;

  else if(lowChar==NFmiString("#extremeplace") ||
		  lowChar==NFmiString("#ääriarvopaikka"))
	return dExtremePlace;

  else if(lowChar==NFmiString("#suntime") ||
		  lowChar==NFmiString("#aurinkoaika") ||
		  lowChar==NFmiString("#aurinko"))
	return dSunTime;

  else if(lowChar==NFmiString("#rotatingsymbol") ||
	      lowChar==NFmiString("#rotating") ||
		  lowChar==NFmiString("#kääntyväsymboli") ||
		  lowChar==NFmiString("#kääntyväkuva"))
	return dRotating;

  else if(lowChar==NFmiString("#winddirection") ||
		  lowChar==NFmiString("#tuulensuunta"))
	return dWindDirection;
  
  else if(lowChar==NFmiString("#windspeed") ||
		  lowChar==NFmiString("#tuulennopeus"))
	return dWindSpeed;

  else if(lowChar==NFmiString("#substitute") ||
	      lowChar==NFmiString("#korvaus"))
	return dSubstitute;

  else if(lowChar==NFmiString("#scaledsymbol") ||
		  lowChar==NFmiString("#skaalautuvasymboli"))
	return dScaling;
  else
	return NFmiPressDescription::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStationPoint Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolGroup::WritePS(NFmiStationPoint* theStationPoint,
							  FmiPressOutputMode theOutput)
{

  NFmiPoint sizeScaling = itsRectScale->GetScaling();
  double scaledXSize = itsRectSize.X() * sizeScaling.X();
  double scaledYSize = itsRectSize.Y() * sizeScaling.Y();
  
  NFmiRect absRectOfGroup;

  absRectOfGroup = NFmiRect(theStationPoint->X() -  scaledXSize/2,	// left
							theStationPoint->Y() + scaledYSize/2,	// top
							theStationPoint->X() + scaledXSize/2,	// right
							theStationPoint->Y() - scaledYSize/2);	// bottom

  NFmiVoidPtrIterator objectIter(itsPressScalingObjects);
  NFmiPressScaling * object;

  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while (object)
	{
	  NFmiRectScale rectScale = NFmiRectScale(*itsRectScale);
	  NFmiRect rect = NFmiRect(NFmiPoint(0.,0.),itsRectSize);
	  rectScale.SetStartScales(rect);
	  rectScale.SetEndScales(absRectOfGroup);
	  object->Set(*itsRectScale, *itsOutFile);
	  
	  object->ScaleNotPlace(); // koska jo kerran skaalat
	  if (!(object->WritePS(theStationPoint->Point(),theOutput)))
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: (timeDep)object->WritePS() in NFmiSymbolGroup" << endl;
		  return false;
		}
	  object = static_cast<NFmiPressScaling *>(objectIter.Next());
	}
  
  NFmiMetTime currentSegmentTime = (static_cast<NFmiQueryInfo *>(itsQueryDataIter))->Time();

  for(int i=0; i < static_cast<int>(GetSize()) && !(itsPressParam->InterruptSymbolGroup()); i++)
	{
	  itsQueryDataIter->Time(currentSegmentTime); // piirtoalkiot saattavat muuttaa (ainakin tuntia)
	  itsParamRects[i]->SetOrder(i+1);
      itsParamRects[i]->SetAlternatingSizeFactor(theStationPoint->GetAlternatingSizeFactor());
	  itsParamRects[i]->WritePS(absRectOfGroup,
								itsQueryDataIter,
								*itsOutFile,
								theOutput);
	}

  itsQueryDataIter->Time(currentSegmentTime);
  return true;
}

//---------------------------------------------------------------------------
