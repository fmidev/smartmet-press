// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressParam
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiPressParam.h"
#include "NFmiMultiParamMapping.h"
#include "NFmiPressArea.h"
#include "NFmiPressDataTimeText.h"
#include "NFmiPressProduct.h"
#include "NFmiPressStationText.h"
#include "NFmiPsSymbol.h"
#include "NFmiPressNameDay.h"
// newbase
#include "NFmiEnumConverter.h"
#include "NFmiEquidistArea.h"
#include "NFmiLocationFinder.h"
#include "NFmiTotalWind.h"
#include "NFmiYKJArea.h"
// system
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetSegmentCurrentTimeStatus (bool theStatus) 
{
	return itsPressProduct->SetSegmentTimeStatus(itsCurrentStep, theStatus);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \param storageQueue Undocumented
 */
// ----------------------------------------------------------------------

// ei saa inliniksi koska PressProduktin ja PressParamin headerit ristiss‰ ??

void NFmiPressParam::PutInStorage(const float value, int storageQueue)
{
  // the PressProduct has the storage
  itsPressProduct->PutInStorage(value, storageQueue);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param storageQueue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiPressParam::UseFromStorage(int storageQueue)
{
  return itsPressProduct->UseFromStorage(storageQueue);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theArea Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressParam::SetNewGeoArea(const NFmiPressArea & theArea)
{
  //HUOM GeoArea on NFmiArea joka PressArean j‰sen
  NFmiArea * newArea = 0;
  if (theArea.GetArea())
	{
	  newArea = theArea.GetArea()->Clone();
	}
  itsArea.SetArea(newArea);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMask Undocumented
 * \param theEnumSpace Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::ChangeMaskNumber(unsigned long theMask,
									  FmiEnumSpace theEnumSpace)
{
  if(!itsStations.IsLocations())
	return false;

  itsEnvironment.ChangeMask(theMask, theEnumSpace);

  NFmiEnumConverter converter(itsEnvironment.GetEnumSpace());
  string name = converter.ToString(itsEnvironment.GetMaskNumber());
  itsName = NFmiString(name);

  itsStations.First();

  NFmiLocation * firstLocation= const_cast<NFmiLocation*> (itsStations.Location());
  firstLocation->SetName(itsName);

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLocation Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------
bool NFmiPressParam::SetFirstStation(const NFmiLocation& theLocation)
{
  if(!itsStations.IsLocations())
	return false;

  itsStations.First();
  NFmiLocation * firstLocation = const_cast<NFmiLocation*>(itsStations.Location());
  firstLocation->SetLongitude(theLocation.GetLongitude());
  firstLocation->SetLatitude(theLocation.GetLatitude());

  firstLocation->SetName(theLocation.GetName());
  firstLocation->SetIdent(theLocation.GetIdent());

  // segmentin nimeen pit‰‰ ottaa asetettu j‰lkiosa (kaksoispisteen j‰lkeen tuleva)
  itsName = theLocation.GetName();

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRenaming Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetStationRename(const NFmiRenaming & theRenaming)
{
  if(!itsStations.IsLocations())
	return false;

  //ainoastaan asemanNimi‰
//  NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsStationDepObjects);
  NFmiVoidPtrIterator objectIter(itsStationDepObjects);
  objectIter.Reset();
  NFmiPressStationText * object = static_cast<NFmiPressStationText *>(objectIter.Next());
  while(object)
	{
	  object->SetNewName(theRenaming);
	  object = static_cast<NFmiPressStationText *>(objectIter.Next());
	}
  itsName = theRenaming.newName; // samalla muuttuu itse olion nimi

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiStationPoint NFmiPressParam::GetFirstStationPoint(void)
{
  if(itsStations.First())
	{
	  return *static_cast<const NFmiStationPoint *>(itsStations.Location());
	}
  else
	return NFmiStationPoint();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiPoint NFmiPressParam::GetFirstPoint(void)
{
  if(itsStations.First())
	{
	  const NFmiStationPoint* statPoint = static_cast<const NFmiStationPoint *>(itsStations.Location());
	  return statPoint->Point();
	}
  else
	return NFmiPoint();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param dataName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::CheckAndSetDistance(long theValue, const NFmiPoint& point)
{	
	std::vector<FmiValuePoint>::iterator pos;

	for(pos=itsCheckLocations.begin(); pos != itsCheckLocations.end(); ++pos)
	{
		if((*pos).value == theValue && 
			 abs((*pos).point.X() - point.X()) < itsCheckDistance.X() &&
			 abs((*pos).point.Y() - point.Y()) < itsCheckDistance.Y())
		{
			fInterruptSymbolGroup = true;
			return false;
		}
	}
	FmiValuePoint valuePoint;
	valuePoint.value = theValue;
	valuePoint.point = point;
	itsCheckLocations.push_back(valuePoint);
	return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param dataName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetData(const NFmiString & dataName)
{
  // t‰nne eik‰ writeps:‰‰n koska tarvitaan pressScaling-objektien initialisoinnissa
  if(itsDataIter)
	delete itsDataIter;

  NFmiQueryData * data = itsPressProduct->DataByName(dataName);
  if(data)
	{
	  // hidastaa noin 10% jos tehd‰‰n SuperSmartInfoja (muuten pit‰isi
	  // antaa m‰‰rittelyss‰ k‰ytet‰‰nkˆ SuperSmartia tai muuttaa data SuperSmartiksi
	  // kun havaitaan ett‰ sit‰ tarvitaan)
	  // kuitenkin noin 60% nopeutus kun SuperSmartit korvattu FastInfoilla
	  // parametreissa ja muualla
	  itsDataIter = new NFmiSuperSmartInfo(data);

	  itsDataIter->First();
	  itsDataName = dataName;
	}
  else
	itsDataIter = 0;

  // vain PressDataTimeText n‰ist‰ datariippuvainen
  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling * scaleObject;
  objectIter.Reset();
  scaleObject = static_cast<NFmiPressScaling *>(objectIter.Next());

  while(scaleObject)
	{
	  switch(scaleObject->ClassId())
		{
		case kNFmiPressDataTimeText:
		  {
			(static_cast<NFmiPressDataTimeText *>(scaleObject))->SetData(itsDataIter);
		  }
		case kNFmiNone:
		default:
		  {
		  }
		}
	  scaleObject = static_cast<NFmiPressScaling *>(objectIter.Next());
	}

  //ainoastaan asemanNimi‰
  NFmiPressStationText * object;
  NFmiVoidPtrIterator objectIter2(itsStationDepObjects);
  objectIter2.Reset();
  object = static_cast<NFmiPressStationText *>(objectIter2.Next());
  while(object)
	{
	  object->SetData(itsDataIter);
	  object = static_cast<NFmiPressStationText *>(objectIter2.Next());
	}
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

bool NFmiPressParam::SetAllTimes(const NFmiMetTime & theTime)
{
  SetTime(theTime);
  bool retCode;

  retCode = itsSymbols.SetAllTimes(theTime);

  return retCode;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetAllLanguages(FmiLanguage theLanguage)
{
  SetLanguage(theLanguage); //ensin oma

  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling * object;
  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while(object)
	{
	  object->SetLanguage(theLanguage);
	  object = static_cast<NFmiPressScaling *>(objectIter.Next());
	}

  itsSymbols.SetAllLanguages(theLanguage);

  return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

unsigned long NFmiPressParam::GetCurrentStationStep(void) const
{
	return itsCurrentStationIndex;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::ReadDescription(NFmiString & retString)
{
  NFmiPoint tableDirectPoints[kMaxNumOfTableElements];

  bool isRelativeTable = false;
  fTwoPartLogComment = false;
  NFmiString logName;

  bool symbolGroupCalled = false;
  bool helpBool;
  bool firstStation = true;
  fIsTimeLoop = false;
  fIsLevelLoop = false;
  NFmiPoint stationStepSize;
  NFmiString dataFileName;
  NFmiValueString valueString;
  long long1, long2, long3;
  unsigned short numOfTableStations, currentStationNum=0; //taulukoissa
  unsigned short currentStationNumOnMap = 0;
  NFmiString string1, string2;
  NFmiPoint point0, point1, point2,scaledTimeLast,unscaledTimeLast, firstUnscaledPoint;
  NFmiPoint unscaledTableMove, scaledTableMove;
  double tableX1=0,tableX2, tableY1=0, tableY2, x, y;
  double xmin,xmax,ymin,ymax;
  bool timeOrLevelTableSet = false;
  long statNum;
  double lat, lon;

  NFmiVoidPtrList * newNames = new NFmiVoidPtrList;

  SetPreReadingTimes();

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);

  numOfTableStations = 0;
  unsigned short currentLevelInd = 0;
  unsigned short currentTimeInd = 1;  // eka on segmentin aika
  long currentTotalTimeDiff = 0;
  unsigned short numOfTableLevels = 0;
  unsigned long numberOfTimeSteps = 1;
  unsigned long numberOfLevelSteps = 1;
  NFmiPoint stepSize = NFmiPoint(20.,0.);
  NFmiPoint unscaledStepSize = NFmiPoint(20.,0.);
  NFmiStationPoint newStation;
  bool dataTimeGiven = false;
  bool takeToMainArea = false;
  while(itsIntObject != dEnd || itsCommentLevel)
	{
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Segmentiss‰" << endl;
		  retString = itsString;
		  return false;
		}
	  itsLoopNum++;
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  switch(itsIntObject)
		{
		case dOther:	  //ylim‰‰r‰ist‰ roinaa,
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #Segmentiss‰: "
						  << static_cast<char *>(itsObject)
						  << endl;
			ReadNext();
			break;
		  }
		case dComment:
		case dEndComment:
		  {
			ReadNext();
			break;
		  }
		case dLogTextAdd:
		  {
			fTwoPartLogComment = true;
		  }  // huom jatkuu seuraavaan
		case dLogText:
		  {
			if (!ReadEqualChar())
			  break;

			itsLogComment = ReadString();

			ReadNext();
			break;
		  }
		case dParamSubViews:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(long1))
			  numberOfTimeSteps = static_cast<unsigned short>(long1);

			ReadNext();
			break;
		  }
		case dParamStepSize:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(x))
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
					y = 0.;
					itsString = valueString;
				  }
                point1.Set(x,y);
                stepSize = NFmiPoint(point1.X()*itsScale.GetScaling().X(),
									 point1.Y()*itsScale.GetScaling().Y());
				unscaledStepSize.Set(x,y);
			  }
			else
			  {
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
			  }

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dLevelTable:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read2Double(xmax,ymax))
			  {
				fIsLevelLoop = true;
				fIsTimeLoop = false;
				point1.Set(xmax,ymax);
				unscaledTimeLast.Set(xmax,ymax);
				scaledTimeLast = itsScale.Scale(point1);
				scaledTimeLast = itsCurrentStationScale.Scale(scaledTimeLast);
				timeOrLevelTableSet = true;

				if(ReadLong(long1))
				  {
					numberOfLevelSteps = std::min(kMaxNumOfTableElements,static_cast<unsigned short>(long1));
				  }
			  }
			// itsStepSize joudutaan asettamaan luupin j‰lkeen alla
			ReadNext();
			break;
		  }
		case dSegmentLevel:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(long1))
			  {
				if(++currentLevelInd > kMaxNumOfTableElements)
				  {
					*itsLogFile << "*** ERROR: sallittu painepintam‰‰r‰ ylitetty: "
								<< numOfTableLevels
								<< endl;
					--currentLevelInd;
				  }
				else
				  {
					itsLevels[currentLevelInd] = long1;
					fIsLevelLoop = true;    //HUOM vaikka yksi vain
					fIsTimeLoop = false;
					timeOrLevelTableSet = true;
				  }
			  }

			ReadNext();
			break;
		  }
		case dLevelPlace:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(long1))
			  {
				if(++currentLevelInd > kMaxNumOfTableElements)
				  {
					*itsLogFile << "*** ERROR: sallittu painepintam‰‰r‰ ylitetty: "
								<< numOfTableLevels
								<< endl;
					ReadTwo(x, y); //luetaan vaan pois
					--currentLevelInd;
				  }
				else
				  {
					itsLevels[currentLevelInd] = long1;
					if(ReadTwo(x, y))
					  {
						fIsLevelLoop = true;
						fIsTimeLoop = false;
						timeOrLevelTableSet = true;
						tableDirectPoints[currentLevelInd].Set(x,y);
					  }
				  }
			  }

			ReadNext();
			break;
		  }
		case dTimeTable:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read2Double(xmax,ymax))
			  {
				fIsLevelLoop = false;
				fIsTimeLoop = true;
                point1.Set(xmax,ymax);
				unscaledTimeLast.Set(xmax,ymax);
                scaledTimeLast = itsScale.Scale(point1);
				scaledTimeLast = itsCurrentStationScale.Scale(scaledTimeLast);

				if(ReadLong(long1))
				  {
					numberOfTimeSteps = static_cast<unsigned short>(long1);
					if(numberOfTimeSteps >= kMaxNumOfTableElements)
					  {
						*itsLogFile << "*** WARNING: taulukkoon varattu aika-askelm‰‰r‰ ylitetty: "
									<< numberOfTimeSteps
									<< "; hidastuu"
									<< endl;
					  }
					if(ReadLong(long1))
					  {
						itsHourStep = static_cast<unsigned short>(long1);
						timeOrLevelTableSet = true;
						if(itsHourStep < 24)
						  SetHourLoop(true);
						currentTotalTimeDiff = (numberOfTimeSteps-1) * itsHourStep;
						currentTimeInd = static_cast<short>(numberOfTimeSteps);
					  }
				  }
			  }
			// itsStepSize joudutaan asettamaan luupin j‰lkeen alla
			ReadNext();
			break;
		  }
		case dTimeTableRelative:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read2Double(xmax,ymax))
			  {
				isRelativeTable = true;
				fIsLevelLoop = false;
				fIsTimeLoop = true;

				point1.Set(xmax,ymax);
				unscaledTableMove.Set(xmax,ymax);
				scaledTableMove = point1;
				scaledTableMove = itsScale.Scale(point1); //globaali skaalaus

				// asemamittakaava ei saa vaikuttaa mutta koko tuotteen vaikuttaa
				// eli siirtym‰t on mitattava ilman globaalia skaalausta
				// scaledTableMove = itsCurrentStationScale.Scale(scaledTableMove);

				if(ReadLong(long1))
				  {
					numberOfTimeSteps = static_cast<unsigned short>(long1);
					if(numberOfTimeSteps >= kMaxNumOfTableElements)
					  {
						*itsLogFile << "*** WARNING: taulukkoon varattu aika-askelm‰‰r‰ ylitetty: "
									<< numberOfTimeSteps
									<< "; hidastuu"
									<< endl;
					  }
					if(ReadLong(long1))
					  {
						itsHourStep = static_cast<unsigned short>(long1);
						timeOrLevelTableSet = true;
						if(itsHourStep < 24)
						  SetHourLoop(true);
						currentTotalTimeDiff = (numberOfTimeSteps-1) * itsHourStep;
						currentTimeInd = static_cast<short>(numberOfTimeSteps);
					  }
				  }
			  }
			// itsStepSize joudutaan asettamaan luupin j‰lkeen alla
			ReadNext();
			break;
		  }
		case dTimePlace:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(long1))
			  {
				if(++currentTimeInd > kMaxNumOfTableElements)
				  {
					*itsLogFile << "*** ERROR: sallittu aika-askelm‰‰r‰ ylitetty segmentiss‰: "
								<< currentTimeInd
								<< endl;
					ReadTwo(x, y); //luetaan vaan pois
					--currentTimeInd;
				  }
				else
				{
				  //edellisen siitym‰ t‰h‰n!!
				  itsTimeSteps[currentTimeInd-1] = long1 - currentTotalTimeDiff;
				  currentTotalTimeDiff += itsTimeSteps[currentTimeInd-1];
				  if(ReadTwo(x, y))
					{
					  fIsLevelLoop = false;
					  fIsTimeLoop = true;
					  timeOrLevelTableSet = true;
					  fIsPureRegTimeLoop = false;
					  tableDirectPoints[currentTimeInd].Set(x,y); //miksi t‰m‰ ind ??? lasketaan alla
					}
				}
			  }

			ReadNext();
			break;
		}
		case dTimePlaceRelative: // EI VALMIS
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(long1))
			  {
				if(++currentTimeInd > kMaxNumOfTableElements)
				  {
					*itsLogFile << "*** ERROR: sallittu aika-askelm‰‰r‰ ylitetty segmentiss‰: "
								<< currentTimeInd
								<< endl;
					ReadTwo(x, y); //luetaan vaan pois
					--currentTimeInd;
				  }
				else
				  {
					//edellisen siirtym‰ t‰h‰n!!
					itsTimeSteps[currentTimeInd-1] = long1 - currentTotalTimeDiff;
					currentTotalTimeDiff += itsTimeSteps[currentTimeInd-1];
					if(ReadTwo(x, y))
					  {
						NFmiPoint point3, point5;

						fIsLevelLoop = false;
						fIsTimeLoop = true;
						timeOrLevelTableSet = true;
						fIsPureRegTimeLoop = false;

						point1 = itsCurrentStationScale.Scale(firstUnscaledPoint);
						point2 = itsScale.Scale(point1);

						point3 = itsCurrentStationScale.UnScale(NFmiPoint(x,y));
						point5 = firstUnscaledPoint + NFmiPoint(x,y);
						tableDirectPoints[currentTimeInd].Set(point5.X(),point5.Y());

					  }
				  }
			  }

			ReadNext();
			break;
		  }
		case dStationTable:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				*itsDescriptionFile >> itsObject;
			    valueString = itsObject;
				if(valueString.IsNumeric())
				  {
					tableX1 = xmin;
					tableX2 = xmax;
					tableY1 = ymin;
					tableY2 = ymax;
					numOfTableStations = static_cast<int>(valueString);
			        *itsDescriptionFile >> itsObject;
			        itsString = itsObject;
				  }
				else     //yhteensopivuus vanhan formaatin kanssa
				  {
					tableX1 = xmin;
					tableX2 = xmin;
					tableY1 = ymin;
					tableY2 = xmax;  //! ok
					numOfTableStations = static_cast<unsigned short>(ymax);
					itsString = itsObject;
				  }
				stationStepSize.Set((tableX2 - tableX1) / (numOfTableStations-1),
									(tableY2 - tableY1) / (numOfTableStations-1));

			  }
            currentStationNum = 0;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dSegmentDataFile:
		  {
			if (!ReadEqualChar())
			  break;

			itsDataName = ReadString();
			itsDataName.LowerCase();

			if(dataTimeGiven)
			  {
			    *itsLogFile << "*** ERROR: Data '"
							<< static_cast<char *>(itsDataName)
							<< "' segmentiss‰ pit‰‰ antaa ennen"
							<< endl;
			    *itsLogFile << "       'K‰yt‰DatanAlkuaikaa' ja 'SuhteellinenTuntiDatasta'"
							<< endl;
			    *itsLogFile << "       -> segmentiss‰ mahdollisesti v‰‰r‰ aika"
							<< endl;
			  }
			SetData(itsDataName);

			ReadNext();
			break;
		  }
		case dDataNotNeeded:
		  {
			fDataNotNeeded = true;

			ReadNext();
			break;
		  }

		case dStationRelocation:
		  {
			if (SetTwo(x, y))
			{
				float upRight = 100.;
				itsCurrentStationScale.SetStartScales(NFmiRect(NFmiPoint(0.,0.), NFmiPoint(upRight,upRight)));
				itsCurrentStationScale.SetEndScales(NFmiRect(NFmiPoint(x,y), NFmiPoint(upRight+x,upRight+y)));
			}
			else
				  *itsLogFile << "*** ERROR: AsemienSiirto-k‰skyn parametreiss‰"  << endl;

			break;
		  }
		case dStationDefArea:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: asemienMittaAlueen min == max"  << endl;
				else
				  itsCurrentStationScale.SetStartScales(NFmiRect(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax)));
			  }

			ReadNext();
			break;
		  }
		case dStationPlotArea:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: asemienSijoitusAlueen min == max"  << endl;
				else
				  itsCurrentStationScale.SetEndScales(NFmiRect(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax)));
			  }
			ReadNext();
			break;
		  }
		case dProducer:
		  {
			if (!ReadEqualChar())
			  break;

 			if(ReadOne(long1))
              itsProducer = long1;

			ReadNext();
			break;
		  }
		case dStationTextObject:
		  {
			if(!itsDataIter) // ilman data ei voi saada asemannimi‰
			  {
				*itsLogFile << "*** ERROR: AsemanNime‰ yritetty ilman dataa" << endl;
			  }
			NFmiPressStationText * text = new NFmiPressStationText;
            text->SetEnvironment(itsEnvironment);
			text->SetHome(GetHome());
			text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetData(itsDataIter);
			text->SetPressParam(this);
			text->SetLanguage(itsLanguage);
			text->SetNewNames(newNames);
			text->Place(firstUnscaledPoint);
			// tarvitaankohan
			point1 = itsCurrentStationScale.Scale(text->Place());
			text->Place(point1);

			if(text->ReadDescription(itsString))
			  {
				// PITƒISI KAI OLLA VAIN OPTIO, SAMOIN MUILLA JA AIKALUUPILLA
				point1 = itsCurrentStationScale.Scale(text->Place());
				text->Place(point1);
				itsStationDepObjects.Add(text);
			  }
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dImageObject:
		  {
			NFmiPsSymbol * image = new NFmiPsSymbol;
			image->SetHome(GetHome());
		    image->SetLogFile(itsLogFile);
			image->SetDescriptionFile(itsDescriptionFile);
			if(image->ReadDescription(itsString))
			  itsTimeDepObjects.Add(image);
			else
			  delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dTextObject:
		  {
			NFmiPressText * text = new NFmiPressText;
            text->SetEnvironment(itsEnvironment);
			text->SetHome(GetHome());
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetLanguage(itsLanguage);
			if(text->ReadDescription(itsString))
			  itsTimeDepObjects.Add(text);
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dTimeTextObject:
		  {
			if(!itsDataIter) // ilman data ei voi saada aikateksti‰
			  {
				*itsLogFile << "*** ERROR: AikaTeksti‰ yritetty ilman dataa segmentiss‰" << endl;

			  }
			NFmiPressDataTimeText * text = new NFmiPressDataTimeText;
            text->SetEnvironment(itsEnvironment);
			text->SetHome(GetHome());
			text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetData(itsDataIter);
			text->SetLanguage(itsLanguage);
			text->Place(firstUnscaledPoint);
			if(text->ReadDescription(itsString))
			  {
				text->ScalePlotting();
				itsTimeDepObjects.Add(text);
			  }
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dSegmentNameDay:
		  {
	//		if(!itsDataIter) // ilman data voi kai saada nimip‰ivi‰
	//		  {
	//			*itsLogFile << "*** ERROR: Nimip‰iv‰‰ yritetty ilman dataa segmentiss‰" << endl;
//
//			  }

			NFmiPressNameDay * text = new NFmiPressNameDay;
		/*	if(!itsPressProduct->GetNameDay())
			{
			  NFmiNameDay *nameDay = new NFmiNameDay;
			  itsPressProduct->SetNameDay(nameDay); //omistaja
			  text->SetNameDay(nameDay);
			}
			else
			  text->SetNameDay(itsPressProduct->GetNameDay());
*/
            text->SetEnvironment(itsEnvironment);
            text->SetHome(GetHome());
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetLanguage(itsLanguage);
			text->SetTime(itsFirstPlotTime);
			text->Place(firstUnscaledPoint);
			if(text->ReadDescription(itsString))
			  {
				text->ScalePlotting();
				if(!itsPressProduct->GetNameDay(text->GetLanguage()))
				{
				  NFmiNameDay *nameDay = new NFmiNameDay;
				  itsPressProduct->SetNameDay(nameDay, text->GetLanguage()); //omistaja
				  text->SetNameDay(nameDay);
				}
				else
				{
				  text->SetNameDay(itsPressProduct->GetNameDay(text->GetLanguage()));
				}
				itsTimeDepObjects.Add(text);
			  }
			else
			  delete text;


			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dSymbolGroup:
		  {
			if(symbolGroupCalled)
			  {
				*itsLogFile << "*** ERROR: Useamman #DataAlkoit-elementin k‰yttˆ kielletty #Segmentiss‰, ohitetaan"  << endl;
				NFmiSymbolGroup dummyGroup;
				dummyGroup.ReadDescription(itsString);
			  }
			else
			  {
				symbolGroupCalled = true;
				itsSymbols.SetPressParam(this);
				itsSymbols.SetEnvironment(itsEnvironment);
				itsSymbols.SetProducer(itsProducer);
				itsSymbols.SetHome(GetHome());
				itsSymbols.SetAllLanguages(itsLanguage);

				itsSymbols.SetLogFile(itsLogFile);
				itsSymbols.SetDescriptionFile(itsDescriptionFile);
				SetPostReadingTimes(); // aikamuutos pit‰isi antaa ennen piirtoryhmi‰
				itsSymbols.SetTime(itsFirstPlotTime);   // Ei k‰ytet‰ kuin tuntia, jota voi ylitt‰‰ j‰seniss‰(mutta p‰iv‰‰ ei)
				itsSymbols.SetHourLoop(IsHourLoop());


				itsSymbols.ReadDescription(itsString);
			  }

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dRelDay:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(long1))
			  itsFirstDeltaDays = static_cast<short>(JustifyByLeaps(long1+ itsEnvironment.GetDayAdvance()));

			ReadNext();
			break;
		  }
		case dHour:
		  {
			if (!ReadEqualChar())
			  break;

 			if(ReadOne(long1))
              itsFirstPlotHours = static_cast<unsigned short>(long1);

			ReadNext();
			break;
		  }
		case dHourStep:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(long1))
			  {
				itsHourStep = static_cast<unsigned short>(long1);
				if(itsHourStep < 25)
				  SetHourLoop(true);
			  }

			ReadNext();
			break;
		  }
		case dDataTime:
		  {
			itsPressProduct->ReadData();
			if(itsDataIter)
			  {
				itsDataIter->Reset();
				itsDataIter->NextTime();
				itsFirstPlotTime = (static_cast<NFmiQueryInfo *>(itsDataIter))->Time();
				itsFirstPlotHours = itsFirstPlotTime.GetHour();
			  }
			else
			  {
				*itsLogFile << "*** ERROR: Aikaa yritetty lukea olemattomasta datasta Segmentiss‰"  << endl;
			  }

			dataTimeGiven = true;

			ReadNext();
			break;
		  }
		case dRelativeDataTime:
		  {
			SetOne (long1);

			itsPressProduct->ReadData();

			if(itsDataIter)
			  {
				itsDataIter->Reset();
				itsDataIter->NextTime();
				itsFirstPlotTime = (static_cast<NFmiQueryInfo *>(itsDataIter))->Time();
				itsFirstPlotTime.ChangeByHours(long1);
				itsFirstPlotHours = itsFirstPlotTime.GetHour();
			  }
			else
			  {
				*itsLogFile << "*** ERROR: Aikaa yritetty lukea olemattomasta datasta Segmentiss‰"  << endl;
			  }
			dataTimeGiven = true;


			break;
		  }
		case dRelHour:     // ei PressTimeDscr:iin koska ei dataelementeill‰
		  {
			if (!ReadEqualChar())
			  break;
			ReadTwo(long1, long2);
			itsFirstPlotTime = NFmiMetTime(60); //huom tunnin res.
			itsFirstPlotTime.ChangeByHours(long1);
			itsFirstPlotTime.NextMetTime(long2*60);

			*itsDescriptionFile >> itsObject;
			valueString = itsObject;
		    if(valueString.IsNumeric())
			  {
				//lasketulla itsFirstPlotTime:ll‰ ei tee mit‰‰n
				long3 = static_cast<int>(valueString);
				itsFirstPlotTime = NextUseTime(long1, long2, long3);
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
			  }
			else
			  itsString = itsObject;

            itsFirstPlotHours = itsFirstPlotTime.GetHour();

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dStationsLocalTime:
		  {
			fStationsAreLocalTime = true;

			ReadNext();
			break;
		  }
		case  dDistanceCheck:
		{
			if(SetTwo(x, y))
			{
			    point1 = itsCurrentStationScale.Scale(NFmiPoint(x,y));
  			    point2 = itsScale.Scale(point1);
				SetDistanceCheck(point2);
			}
			break;
		}
		case dStationNameReplace: // stationiin verrattuna lis‰ykset: *** PrintName
		{
		  lon = 0.;
		  lat = 0.;
		  bool hasOwnPoint = false;  // talukkoEscapea varten

		  if (!ReadEqualChar())
			break;

		  helpBool = true;

		  valueString = ReadString();

		  if(valueString.IsNumeric())
			{
			  statNum = static_cast<int>(valueString);
			  *itsDescriptionFile >> itsObject;
			  string1 = itsObject;
			  *itsDescriptionFile >> itsObject;
			  string2 = itsObject;
			}
		  else
			{
			  statNum = itsLoopNum; // toimii satunnaislukuna,n‰in saadaan bagiin useita samannimisi‰
			  string1 = valueString; //Object;
			}
		  string2 = ReadString();

		  *itsDescriptionFile >> itsObject;
		  valueString = itsObject;         // parillinen m‰‰r‰ lukuja sallittu: 0 2 tai 4
		  if(valueString.IsNumeric())   // >= 1 luku
			{
			  lon = static_cast<double>(valueString);
			  if(ReadDouble(lat))         // >= 2 lukua
				{
			      *itsDescriptionFile >> itsObject;
			      valueString = itsObject;
			      if(valueString.IsNumeric())  // >= 3 lukua
					{
					  x = static_cast<short>(valueString);
					  if(!ReadDouble(y))
						helpBool = false;  // 3 lukua virhe
					  *itsDescriptionFile >> itsObject;            // 4 lukua
					  itsString = itsObject;
					  hasOwnPoint = true;
					}
			      else               //2 lukua
					{
					  if(!fLonLatMode)
						{
						  x = lon;
						  y = lat;
						  lon = 0.;
						  lat = 0.;
						  hasOwnPoint = true;
						}
					  else
						hasOwnPoint = false;

					  itsString = valueString;
					}
				}
			  else                // 1 luku
				{
				  itsString = valueString;
				  if(numOfTableStations < 1)
					{
					  *itsLogFile << "*** ERROR: "
								  << static_cast<char *>(string1)
								  << ": asemalla, joka ei taulukossa, tulisi olla paikka"
								  << endl;
					  helpBool = false;
					}
				}
			}
		  else                // 0 lukua
			{
			  itsString = valueString;
			  if(numOfTableStations < 1) helpBool = false;
			}

		  if(helpBool)
			{
			  if (numOfTableStations > 1 && !hasOwnPoint)
				{
				  currentStationNum++;


				  x = stationStepSize.X() * (currentStationNum-1) + tableX1;
				  y = stationStepSize.Y() * (currentStationNum-1) + tableY1;
				}
			  point1 = itsCurrentStationScale.Scale(NFmiPoint(x,y));
  			  point2 = itsScale.Scale(point1);
              NFmiStationPoint station
				= NFmiStationPoint(NFmiStation(statNum, string1, lon, lat), point2);

			  itsStations.AddLocation(station, false);
			  if(firstStation)
				{
				  //EI TOIMI
				  itsName = station.GetName();  //EI TOIMI vai toimiiko
                  firstUnscaledPoint = NFmiPoint(x,y);
				  firstStation = false;
				}

			  // *** PrintName

			  NFmiRenaming * renaming = new NFmiRenaming;
			  renaming->originalName = string1;
			  renaming->newName = string2;
			  newNames->Add(renaming);

			}

		  itsIntObject = ConvertDefText(itsString);
		  break;
		}
		case dStation:
		  {
			lon = 0.;
			lat = 0.;
			bool hasOwnPoint = false;  // talukkoEscapea varten

			if (!ReadEqualChar())
			  break;

			helpBool = true;

			valueString = ReadString();
		    if(valueString.IsNumeric())
			  {
				statNum = static_cast<int>(valueString);
				*itsDescriptionFile >> itsObject;
			    string1 = itsObject;

			  }
			else
			  {
				statNum = itsLoopNum; // toimii satunnaislukuna,n‰in saadaan bagiin useita samannimisi‰
				string1 = valueString;
			  }
			valueString = ReadString();
			// parillinen m‰‰r‰ lukuja sallittu: 0 2 tai 4
			if(valueString.IsNumeric())   // >= 1 luku
			  {
				lon = static_cast<double>(valueString);
				if(ReadOne(lat))         // >= 2 lukua
				  {
					*itsDescriptionFile >> itsObject;
					valueString = itsObject;
					if(valueString.IsNumeric())  // >= 3 lukua
					  {
						x = static_cast<double>(valueString);
						if(!ReadOne(y))
						  helpBool = false;  // 3 lukua virhe
						*itsDescriptionFile >> itsObject;            // 4 lukua
						itsString = itsObject;
						hasOwnPoint = true;
					  }
					else               //2 lukua
					  {
						if(!fLonLatMode)
						  {
							x = lon;
							y = lat;
							lon = 0.;
							lat = 0.;
							hasOwnPoint = true;
						  }
						else
						  hasOwnPoint = false;

						itsString = valueString;
					  }
				  }
				else                // 1 luku
				  {
					itsString = valueString;
					helpBool = false;
				  }
			  }
			else                // 0 lukua
			  {
				itsString = valueString;
				if(numOfTableStations < 1)
				  {
					*itsLogFile << "*** ERROR: "
								<< static_cast<char *>(string1)
								<< ": asemalla, joka ei taulukossa, tulisi olla paikka"
								<< endl;
					helpBool = false;
				  }
			  }

            if(helpBool)
			  {
				if (numOfTableStations > 1 && !hasOwnPoint)
				  {

					currentStationNum++;
					x = stationStepSize.X() * (currentStationNum-1) + tableX1;
					y = stationStepSize.Y() * (currentStationNum-1) + tableY1;
				  }
				point1 = itsCurrentStationScale.Scale(NFmiPoint(x,y));
				point2 = itsScale.Scale(point1);

				NFmiStationPoint station(NFmiStation(statNum, string1, lon, lat), point2);
				// t‰m‰ temppu jotta koordinaatit oikein PsSymbolille
				// voidaan tallettaa point1:een kun kaikki periytyy NFmiPsSymbolista
				// lat/loniahan ei tarvita mihink‰‰n

				itsStations.AddLocation(station, false);
				if(firstStation)
				  {
					itsName = station.GetName();  //EI TOIMI vai toimiiko
					firstUnscaledPoint = NFmiPoint(x,y);
					firstStation = false;
				  }
			  }

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dStationPlaceOnMap:
		  {
			if (!ReadEqualChar())
			  break;

			NFmiString name;
			if(ReadOneStringAndTwoNumbers(name,x,y))
			  {
				if(itsArea.GetArea())
				  {
					if(fCoordinatesFromMainMap)
					{
						NFmiArea *mainArea = itsPressProduct->itsMainArea;
						NFmiArea *ownArea = itsArea.GetArea();
						if(!mainArea || !ownArea)
						{			
							*itsLogFile << "*** ERROR: "<< "p‰‰- tai oma karttaa puuttuu"  << endl;
						}
						else
						{
							double xZoom = ownArea->Width()/mainArea->Width();
							x = (x - mainArea->Left()) * xZoom + ownArea->Left();
							double yZoom = ownArea->Height()/mainArea->Height();
							y = (y - mainArea->Bottom()) * yZoom + ownArea->Bottom();
						}
					}
					currentStationNumOnMap++;
					point0 = NFmiPoint(x,y);
					double bottom = (itsArea.GetArea())->Bottom();
					double top = (itsArea.GetArea())->Top();
					double y0 = point0.Y();
					point1.Set(point0.X(), bottom -(y0-top));

					NFmiPoint lonlat(itsArea.GetArea()->ToLatLon(point1));
					lon = lonlat.X();
					lat = lonlat.Y();
					point1 = itsCurrentStationScale.Scale(NFmiPoint(x,y));
					point2 = itsScale.Scale(point1);
					if(!name.IsValue())
					{
						name = NFmiString("As");
						name += NFmiValueString(static_cast<int>(currentStationNumOnMap)); //uniikki nimi jokaiselle
					}
					NFmiStationPoint station(NFmiStation(currentStationNumOnMap, name, lon, lat), point2);
					itsStations.AddLocation(station, false);
					if(firstStation)
					  {
						itsName = station.GetName();
						firstUnscaledPoint = NFmiPoint(x,y);
						firstStation = false;
					  }
					firstStation = false;
				  }
				else
				  *itsLogFile << "*** ERROR: "<< "karttaprojektio puuttuu"  << endl;

			  }

			ReadNext();
			break;
		  }
		case dStationNameOnMap:
		  {
			if (!ReadEqualChar())
			  break;

			NFmiString name = ReadString();
			{
			  if(itsArea.GetArea())
				{
				  currentStationNumOnMap++;
				  NFmiStationPoint statPoint(NFmiStation(currentStationNumOnMap,name),NFmiPoint(10,10));
				  if(SetLonLat(statPoint))
					{
					  point0 = (itsArea.GetArea())->ToXY(NFmiPoint(statPoint.GetLongitude(),statPoint.GetLatitude()));
					  double bottom = (itsArea.GetArea())->Bottom();
					  double top = (itsArea.GetArea())->Top();
					  double y0 = point0.Y();
					  point1 = NFmiPoint(point0.X(),bottom-y0+top);
					  point2 = itsScale.Scale(point1);
					  NFmiStationPoint station
						(NFmiStation(currentStationNumOnMap, name, lon, lat), point2);
					  itsStations.AddLocation(station, false);
					  if(firstStation)
						{
						  itsName = station.GetName();
						  firstUnscaledPoint = NFmiPoint(point1);
						  firstStation = false;
						}
					  firstStation = false;
					}
				  else
					*itsLogFile << "*** ERROR: "
								<< "lat/lon puuttuu: "
								<< static_cast<char *>(name)
								<< endl;
				}
			  else
				*itsLogFile << "*** ERROR: "<< "karttaprojektio puuttuu"  << endl;

			}

			ReadNext();
			break;
		  }
		case dStationLonLatOnMap:
		  {
			if (!ReadEqualChar())
			  break;

			NFmiString name;
			if(ReadOneStringAndTwoNumbers(name,lon,lat))
			  {
				if(itsArea.GetArea())
				  {
					currentStationNumOnMap++;
					NFmiPoint lonlat(lon,lat);
					point0 = itsArea.GetArea()->ToXY(lonlat);
					lon = lonlat.X();
					lat = lonlat.Y();
					double bottom = (itsArea.GetArea())->Bottom();
					double top = (itsArea.GetArea())->Top();
					double y0 = point0.Y();
					point1 = NFmiPoint(point0.X(), bottom -(y0-top));
					point2 = itsScale.Scale(point1);
					if(!name.IsValue())
					{
						name = NFmiString("As");
						name += NFmiValueString(static_cast<int>(currentStationNumOnMap)); //uniikki nimi jokaiselle
					}
					NFmiStationPoint station
					  (NFmiStation(currentStationNumOnMap, name, lon, lat), point2);
					itsStations.AddLocation(station, false);
					if(firstStation)
					  {
						itsName = station.GetName();
						firstUnscaledPoint = NFmiPoint(point1);
						firstStation = false;
					  }
					firstStation = false;
				  }
				else
				  *itsLogFile << "*** ERROR: "<< "karttaprojektio puuttuu"  << endl;

			}

			ReadNext();
			break;
		  }

		case dSegmentMapArea:
		  {
			itsArea.SetLogFile(itsLogFile);
			itsArea.SetDescriptionFile(itsDescriptionFile);
			itsArea.SetXyRequest(!fIsAreaOperation); // mittoja ei tarvita jos k‰ytet‰‰n alueoperaatioihin
			itsArea.SetProduct(itsPressProduct);
			itsArea.ReadDescription(itsString);

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dMainArea:
		  {
			takeToMainArea = true; //toteutetaan lopussa jotta j‰rjestyksell‰ ei v‰li‰

			ReadNext();
			break;
		  }
		case dCoordinatesFromMainMap:
		  {
			fCoordinatesFromMainMap = true;
			ReadNext();
			break;
		  }
		case dStationsFromData:
		  {
			if(itsArea.GetArea())
			  {
				if(!itsDataIter)
				{
					*itsLogFile << "*** ERROR: "<< "asemina datan pisteet, mutta data puuttuu "  << endl;
					return false;
				}
				itsDataIter->ResetLocation();
				double bottom = (itsArea.GetArea())->Bottom();
				double top = (itsArea.GetArea())->Top();
				NFmiPoint lonlat;
                int num = 0;
				while (itsDataIter->NextLocation())
				  {
					num++;
					currentStationNumOnMap++;
					lonlat = itsDataIter->LatLon();
					point0 = itsArea.GetArea()->ToXY(lonlat);
					double y0 = point0.Y();
					point1 = NFmiPoint(point0.X(), bottom -(y0-top));
					point2 = itsScale.Scale(point1);
					NFmiString name("As");
					name += NFmiValueString(static_cast<int>(currentStationNumOnMap)); //uniikki nimi jokaiselle
					NFmiStationPoint station
					  (NFmiStation(currentStationNumOnMap, name, lonlat.X(), lonlat.Y()), point2);
					itsStations.AddLocation(station, false);
					if(firstStation)
					  {
						itsName = station.GetName();
						firstUnscaledPoint = NFmiPoint(x,y);
						firstStation = false;
					  }
					firstStation = false;
				  }
			  }
			else
			  *itsLogFile << "*** ERROR: "<< "karttaprojektio puuttuu gridilt‰"  << endl;

			ReadNext();
			break;
		  }
		case dAreaOperation:
		{
		  fIsAreaOperation = true;

		  ReadNext();
		  break;
		}
		case dSupplement:
		{
		  fSupplementary = true; //t‰m‰ segmentti
		  itsPressProduct->SetSupplementMode(true); //muille segmenteille tiedoksi

		  ReadNext();
		  break;
		}
		default:
		  {
			ReadRemaining();
			break;
		  }
		}
	} //while

  CreateStationFromAreaMask();
  if(itsEnvironment.GetMaskNumber() > 0) // t‰nne, oli vikana
	fIsAreaOperation = true;
  short numberOfRegularSteps = static_cast<unsigned short>(std::max(numberOfTimeSteps,numberOfLevelSteps));
  itsNumberOfSteps = numberOfRegularSteps;
  if(fIsLevelLoop)
	itsNumberOfSteps = currentLevelInd;
  else if(fIsTimeLoop)
	itsNumberOfSteps = currentTimeInd;

  if(timeOrLevelTableSet) // ei voi asettaa yll‰ p‰‰luupissa koska ekan aseman paikkaa ei v‰ltt‰m‰tt‰ tied‰
	{
	  if(itsStations.GetSize() < 1 )
		{
		  *itsLogFile << "*** ERROR: "<< "station(s) required for segment with table"  << endl;
		  return false;
		}

	  itsStations.Reset();
	  itsStations.Next();
	  NFmiStationPoint statPoint = *static_cast<const NFmiStationPoint *>(itsStations.Location());
	  NFmiPoint point = statPoint.Point();

	  if(isRelativeTable)
		{
		  short factor = numberOfRegularSteps; //nollakin sallitaan
		  stepSize.Set(scaledTableMove.X(),scaledTableMove.Y());
		  unscaledStepSize.Set(unscaledTableMove.X(), unscaledTableMove.Y());
		  scaledTimeLast.Set(point.X()+stepSize.X()*factor
							 ,point.Y()+stepSize.Y()*factor);
		  unscaledTimeLast.Set(firstUnscaledPoint.X()+unscaledStepSize.X()*factor
							   ,firstUnscaledPoint.Y()+unscaledStepSize.Y()*factor);
		}
	  else
		{
		  short factor = (std::max(numberOfRegularSteps-1,1)); //12.4.00 oli ..Time..; nollalla jaon esto
		  stepSize.Set((scaledTimeLast.X()-point.X())/factor,
					   (scaledTimeLast.Y()-point.Y())/factor);
		  unscaledStepSize.Set((unscaledTimeLast.X()-firstUnscaledPoint.X())/factor,
							   (unscaledTimeLast.Y()-firstUnscaledPoint.Y())/factor);
		}

	  NFmiPoint previousNextScaled = scaledTimeLast;
	  NFmiPoint previousNextUnscaled = unscaledTimeLast;
	  if(numberOfRegularSteps <= 1)
		{
		  previousNextUnscaled = firstUnscaledPoint;
		  previousNextScaled = itsScale.Scale(firstUnscaledPoint);
		  previousNextScaled = itsCurrentStationScale.Scale(previousNextScaled);
		}
	  for (int ind=1; ind<=itsNumberOfSteps && ind < kMaxNumOfTableElements; ind++) //writePS:ss‰ indeksointi 1:st‰
		{
		  //itsSteps on siirtym‰ seuraavaan
		  if(ind == itsNumberOfSteps)
			{
			  itsSteps[ind] = NFmiPoint(0.,0.);
			  itsUnscaledSteps[ind] = NFmiPoint(0.,0.);
			}
		  else
			{
			  if(ind >= numberOfRegularSteps)
				{
				  NFmiPoint nextScaledP = itsScale.Scale(tableDirectPoints[ind+1]);
				  nextScaledP = itsCurrentStationScale.Scale(nextScaledP);
				  itsSteps[ind] = nextScaledP - previousNextScaled;

				  itsUnscaledSteps[ind] = tableDirectPoints[ind+1] - previousNextUnscaled;


				  previousNextScaled = nextScaledP;
				  previousNextUnscaled = tableDirectPoints[ind+1];
				}
			  else //s‰‰nnˆllinen taulukko (alussa, muttei vain lopussa)
				{
				  itsSteps[ind] = stepSize;
				  itsUnscaledSteps[ind] = unscaledStepSize;
				  itsTimeSteps[ind] =itsHourStep;
				}
			}
		}
	}

  if(takeToMainArea)
  {
		if(itsArea.GetArea() == 0)
			*itsLogFile << "*** ERROR: "<< "puuttuvaa karttaa ei saa p‰‰kartaksi"  << endl;
		else
			itsPressProduct->itsMainArea = itsArea.GetArea(); 
  }

  retString = itsString;

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiPressParam::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar(object);
  lowChar.LowerCase(); 

  if(lowChar==NFmiString("#symbolgroup") ||
	 lowChar==NFmiString("#symboliryhm‰") ||
	 lowChar==NFmiString("#dataalkiot") ||
	 lowChar==NFmiString("#parametrit") ||
	 lowChar==NFmiString("#symboliryhm‰"))
	return dSymbolGroup;

  else if(lowChar==NFmiString("#image") ||
		  lowChar==NFmiString("#kuva") ||
		  lowChar==NFmiString("#vakiosymboli") ||
		  lowChar==NFmiString("#vakiokuva"))
	return dImageObject;

  else if(lowChar==NFmiString("#text") ||
		  lowChar==NFmiString("#teksti") ||
		  lowChar==NFmiString("#vakioteksti"))
	return dTextObject;

  else if(lowChar==NFmiString("#time") ||
		  lowChar==NFmiString("#aika") ||
		  lowChar==NFmiString("#aikateksti"))
	return dTimeTextObject;

  else if(lowChar==NFmiString("#stationname") ||
		  lowChar==NFmiString("#asemannimi"))
	return dStationTextObject;

  else if(lowChar==NFmiString("datanotneeded") ||
		  lowChar==NFmiString("eidataa"))
	return dDataNotNeeded;

  else if(lowChar==NFmiString("logtext") ||
		  lowChar==NFmiString("lokiteksti"))
	return dLogText;

  else if(lowChar==NFmiString("logtextadd") ||
		  lowChar==NFmiString("lokitekstilis‰ys"))
	return dLogTextAdd;

  // seuraavat kaksi t‰‰ll‰ eik‰ PressTimeDescriptionissa, koska t‰m‰n kaikilla lapsilla ei sallittua

  else if(lowChar==NFmiString("usedatastarttime") ||
		  lowChar==NFmiString("k‰yt‰datanalkuaikaa"))
	return dDataTime;

  else if(lowChar==NFmiString("relativehoursfromdata") ||
		  lowChar==NFmiString("suhteellinentuntidatasta"))
	return dRelativeDataTime;

  else if(lowChar==NFmiString("datafile") ||
		  lowChar==NFmiString("data"))
	return dSegmentDataFile;

  else if(lowChar==NFmiString("grid") ||
		  lowChar==NFmiString("hila"))
	{
	  return dGridMode;    // ei koodattu viel‰, Tarvitaanko???
	}
  else if(lowChar==NFmiString("usestationslocaltime") ||
		  lowChar==NFmiString("k‰yt‰asemienpaikallistaaikaa"))
	{
	  return dStationsLocalTime;
	}

  // datan syˆttˆ gridein‰ vaatii muutoksia: pit‰‰ voida erottaa milloin kaksi lukua
  // tarkoittavat tuotepaikkaa ja milloin lonLatia (varsinkin taulukoissa ep‰selv‰‰),
  // tehty uusi j‰sen fLonLatMode, jota muutetaan uusilla avainsanoilla xxxxLonLatxxxx.

  else if(lowChar==NFmiString("station") ||
		  lowChar==NFmiString("asema"))
	{
	  fLonLatMode = false;
	  return dStation;
	}
  else if(lowChar==NFmiString("stationnewnaming") ||
		  lowChar==NFmiString("asemanime‰vaihtaen"))
	{
	  fLonLatMode = false;
	  return dStationNameReplace;
	}
  else if(lowChar==NFmiString("stationlonlat") ||
		  lowChar==NFmiString("asemalonlat"))
	{
	  fLonLatMode = true;
	  return dStation;
	}
  else if(lowChar==NFmiString("stationlonlatnewnaming") ||
		  lowChar==NFmiString("asemalonlatnime‰vaihtaen"))
	{
	  fLonLatMode = true;
	  return dStationNameReplace;
	}
  else if(lowChar==NFmiString("stationdefarea") ||
		  lowChar==NFmiString("asemienmittaalue"))
	return dStationDefArea;

  else if(lowChar==NFmiString("stationrelocation") ||
		  lowChar==NFmiString("asemiensiirto"))
	return dStationRelocation;

  else if(lowChar==NFmiString("stationplotarea") ||
		  lowChar==NFmiString("asemiensijoitusalue"))
	return dStationPlotArea;

  else if(lowChar==NFmiString("paramsubviews") ||
		  lowChar==NFmiString("siirtolukum‰‰r‰") ||
		  lowChar==NFmiString("aikoja"))
	return dParamSubViews;

  else if(lowChar==NFmiString("paramstepsize") ||
		  lowChar==NFmiString("siirrot"))
	return dParamStepSize;

  else if(lowChar==NFmiString("stationtable") ||
		  lowChar==NFmiString("asemataulukko"))
	return dStationTable;

  else if(lowChar==NFmiString("timetable") ||
		  lowChar==NFmiString("aikataulukko"))
	return dTimeTable;

  else if(lowChar==NFmiString("timetablerelative") ||
		  lowChar==NFmiString("aikataulukkosuhteellisena") ||
		  lowChar==NFmiString("aikataulukkosuht"))
	return dTimeTableRelative;

  else if(lowChar==NFmiString("leveltable") ||
		  lowChar==NFmiString("painepintataulukko"))
	return dLevelTable;

  else if(lowChar==NFmiString("level") ||
		  lowChar==NFmiString("painepinta"))
	return dSegmentLevel;

  else if(lowChar==NFmiString("levelplace") ||
		  lowChar==NFmiString("painepintapaikka"))
	return dLevelPlace;

  else if(lowChar==NFmiString("timeplace") ||
		  lowChar==NFmiString("aikapaikka"))
	return dTimePlace;

  else if(lowChar==NFmiString("timeplacerelative") ||
		  lowChar==NFmiString("aikapaikkasuhteellisena") ||
		  lowChar==NFmiString("aikapaikkasuht"))
	return dTimePlaceRelative;

  else if(lowChar==NFmiString("stationlonlatonmap") ||
		  lowChar==NFmiString("asemalonlatillakartalle"))
	return dStationLonLatOnMap;

  else if(lowChar==NFmiString("stationnameonmap") ||
		  lowChar==NFmiString("asemanimell‰kartalle"))
	return dStationNameOnMap;

  else if(lowChar==NFmiString("stationplaceonmap") ||
		  lowChar==NFmiString("asemapaikallakartalle") ||
		  lowChar==NFmiString("paikka"))
	return dStationPlaceOnMap;

  else if(lowChar==NFmiString("#map") ||
		  lowChar==NFmiString("#kartta"))
	return dSegmentMapArea;

  else if(lowChar==NFmiString("mainmap") ||
		  lowChar==NFmiString("t‰ss‰p‰‰kartta"))
	return dMainArea;

  else if(lowChar==NFmiString("coordinatesfrommainmap") ||
		  lowChar==NFmiString("paikkakoordinaatitp‰‰kartasta"))
	return dCoordinatesFromMainMap;

  else if(lowChar==NFmiString("stationsfromdata") ||
		  lowChar==NFmiString("aseminadatanpisteet"))
	return dStationsFromData;

  else if(lowChar==NFmiString("areaoperaion") ||
		  lowChar==NFmiString("alueoperaatio"))
	return dAreaOperation;
  
  else if(lowChar==NFmiString("distancecheck") ||
		  lowChar==NFmiString("l‰heisyysesto"))
	return dDistanceCheck;

  else if(lowChar==NFmiString("onlysupplement") ||
		  lowChar==NFmiString("vaint‰ydennys"))
	return dSupplement;

  else if(lowChar==NFmiString("#nameday") ||
		  lowChar==NFmiString("#nimip‰iv‰"))
	return dSegmentNameDay;

  else
	return NFmiPressTimeDescription::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::CreateAreaMask(void)
{
  if(itsAreaMask)
	delete itsAreaMask;

  if(itsMaskIter && itsEnvironment.GetMaskNumber() > 0)
	{
	  itsMaskIter->First();
	  unsigned long maskNr = itsEnvironment.GetMaskNumber();
	  NFmiCalculationCondition maskOperation(kFmiMaskEqual, maskNr);

	  // maski on siis info-tyyppinen (ei esim. bitmap tai lennossa
	  // laskettava kuten auringon-kulma-maski)
	  NFmiAreaMask::Type maskType = NFmiAreaMask::kInfo;

	  NFmiInfoData::Type dataType = NFmiInfoData::kStationary;
	  bool ownsInfo = false;
	  NFmiAreaMask::BinaryOperator postBinaryOperator = NFmiAreaMask::kAnd;
	  itsAreaMask = new NFmiInfoAreaMask(maskOperation, maskType, dataType, itsMaskIter, ownsInfo, postBinaryOperator);
	  *itsLogFile << "  areamask created: " <<  maskNr << endl;
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

bool NFmiPressParam::CreateStationFromAreaMask(void)
{
  //tarvitaan nimi vaikka tekstioutputiin
  if(itsMaskIter)
	{
	  NFmiEnumConverter converter(itsEnvironment.GetEnumSpace());
	  string name = converter.ToString(itsEnvironment.GetMaskNumber());
	  itsName = NFmiString(name);
	  NFmiStationPoint station
		(NFmiStation(100, itsName, 25, 60), NFmiPoint(100.,100.));
	  // if(itsStations.GetSize() < 1)  // pit‰isi tuhota entinen ettei kumuloidu; p‰‰asia ett‰ nimi ok
	  itsStations.AddLocation(station, false);
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \param theFile Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::WritePS(NFmiRectScale theScale,
							 ofstream & theFile,
							 FmiPressOutputMode theOutput)
{
  UnsetAllErrorReported();

  if(fSupplementary)
	  *itsLogFile << "  VAINTƒYDENNYS" << endl;

  if (!itsDataIter)
	{
	  *itsLogFile << "  *** ERROR: data lost: "
				  << static_cast<char *>(itsDataName)
				  << endl;
	  return true; //false lopettaa kaikki segmentit
	}
  itsDataIter->First(); // ainakin level pit‰‰ asettaa nollaksi eik‰ -1:ksi

  CreateAreaMask();

  NFmiPoint stationPoint;
  NFmiPoint stationPointMovement;
  NFmiPoint nameFromData;

  if(itsLogFile)
	if(itsDataName.IsValue())
	  *itsLogFile << "  data: " << static_cast<char *>(itsDataName) << endl;
	else
	  *itsLogFile << "  data: tuotteessa ekana oleva" << endl;

  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling * object;

  // jotta voidaan tehd‰ aika vasta asemannimen j‰lkeen (tiedostomuoto)
  NFmiPressScaling * saveObject;
  saveObject = 0;

  NFmiVoidPtrIterator stationObjectIter (itsStationDepObjects);

  itsSymbols.Set(theScale, itsDataIter, theFile);

  NFmiMetTime time = itsFirstPlotTime;

  itsCurrentStep = 1;
  stationPointMovement.Set(0., 0.);
  itsCurrentStationIndex = 0;
  FmiCounter statAll = 0;
  bool done, supplementLater;

  /********* AIKA/PAINEPINTA-luuppi ********/
  while(itsCurrentStep <= itsNumberOfSteps) 
	{
	  FmiCounter currentStepInd = std::min(static_cast<int>(itsCurrentStep),kMaxNumOfTableElements-1);

      if(!fSupplementary)  
		 itsPressProduct->SetSegmentTimeStatus(itsCurrentStep, false);

	  if(!itsDataIter)
		{
		  if(itsLogFile)
			*itsLogFile << "  *** ERROR: Dataa ei ole" << endl;
		}
	  else
		{
		  if(fIsLevelLoop)
			itsDataIter->Level(NFmiLevel(kFmiPressureLevel,itsLevels[currentStepInd]));

		  done = fSupplementary && itsPressProduct->GetSegmentTimeStatus(itsCurrentStep);
		  supplementLater = itsPressProduct->GetSupplementMode() && !fSupplementary;

		  // HUOM segmentin aika pit‰‰ olla datassa vaikka piirtoalkiossa muutettaisiin tuntia
          // ei prosessoida jos t‰ydennyssegmentti ja on jo tehty		 
 		  if((itsDataIter->Time(time) || fDataNotNeeded) && !done)
 		 {
		  if(itsCurrentStep == 1 || fIsTimeLoop)
		  {
			 bool writeLog = true;
			 if(itsNumberOfSteps > 6)  // lyhennet‰‰n lokiskriptia
			   {
				 if(itsCurrentStep == 1)
				   {
					 *itsLogFile << "  ekan ajan segmentti-aika: " << static_cast<char *>(time.ToStr("DD.MM.YYYY HH"));
				   }
				 else if (itsCurrentStep == itsNumberOfSteps)
				   {
					 *itsLogFile << "  vikan ajan segmentti-aika: " << static_cast<char *>(time.ToStr("DD.MM.YYYY HH"));
				   }
				 else
				   writeLog = false;
			   }
			 else
			   {
				 *itsLogFile << "  Segmentin aika: " << static_cast<char *>(time.ToStr("DD.MM.YYYY HH"));
			   }
			 if(IsStationLocalTimeMode() && writeLog)
			   *itsLogFile << " pa"<< endl;
			 else if(writeLog)
			   *itsLogFile << " utc"<< endl;
		   }
         if(!fSupplementary)  
			 itsPressProduct->SetSegmentTimeStatus(itsCurrentStep, true);

		 if(fIsLevelLoop)
		   *itsLogFile << "  Segmentin painepinta: " << itsLevels[currentStepInd] << endl;

		 // ************** AikaSidotutObjektit ensin (paitsi ne jotka pyydetty lopussa) ***********
		 // eli ***VakioSymboli
		 //     ***VakioTeksti
		 //     ***AikaTeksti
		 //     ***Nimip‰iv‰t

			DoTimeDependent(false,
					  currentStepInd,
					  theFile,
					  theOutput,
					  saveObject);
		 // ********** alun AikaSidotutObjektit loppu **************

	     itsStations.Reset();
		 fIsFirstStation = true;

		 // ********** ASEMAluuppi *****************
		 itsCurrentStationIndex = 0;
	     while(itsStations.Next())
		   {
			 statAll++;
			 itsCurrentStationIndex++;
			 stationPoint = NFmiStationPoint(*static_cast<const NFmiStationPoint *>(itsStations.Location())).Point();
			 NFmiStationPoint statPoint = *static_cast<const NFmiStationPoint *>(itsStations.Location());
			 NFmiPoint lonLat = statPoint.GetLocation();
			 if(fabs(lonLat.X()) < 0.0001 &&  // HUOM puuttuva testi, mikseiv‰t ole tasan nolla !!
				fabs(lonLat.Y()) < 0.0001)
			   {
				 if(itsDataIter->IsGrid())
				   {
					 if (!SetLonLat(statPoint))
					   continue; // ->seuraava asema jos ei taulukossa
				   }
				 else
				   {
					 if(itsDataIter->Location(NFmiStation(*statPoint.Station()).GetName()))
					   {
						 statPoint.SetLongitude(itsDataIter->Location()->GetLongitude());
						 statPoint.SetLatitude(itsDataIter->Location()->GetLatitude());
					   }
				   }

			   }
			 lonLat = statPoint.GetLocation();

			 itsCurrentStation = NFmiStation(*statPoint.Station());
			 if (FindQDStationName(statPoint) || fDataNotNeeded)
			   {

				 if(itsCurrentStep == 1)
				   {
					// ********* AsemaSidotutObjektit ************* 
					// *** eli AsemanNimi (vainko?)

					 stationObjectIter.Reset();
					 object = static_cast<NFmiPressScaling *>(stationObjectIter.Next());

					 NFmiPoint unScaledPoint = itsScale.UnScale(stationPoint);
					 if(object && itsStations.CurrentIndex() == 0)
					   {
						 // jokaisella objektilla pit‰‰ olla oma, j‰seneksi
						 nameFromData = NFmiPoint(object->Place().X() - unScaledPoint.X(),
												  object->Place().Y() - unScaledPoint.Y());
					   }
					 while (object)
					   {
						 NFmiPoint savePlace = object->Place();
						 object->Place(unScaledPoint+nameFromData);
						 object->Set(itsScale, theFile);
						 object->SetRotatingPoint(object->Place());
						 if (!(object->WritePS(theOutput)))
						   {
							 if(itsLogFile)
							   *itsLogFile << "*** ERROR: (statDep)object->WritePS() in NFmiPressParam" << endl;
							 return false;
						   }
						 if(theOutput == kPostScript)
						   object->WriteGRestore();
						 object->Place(savePlace); // jotta toimisi seuraavalle writePs-k‰skylle

						 object = static_cast<NFmiPressScaling *>(stationObjectIter.Next());
					   }
				   }
				 // ************ AsemaSidotutObjektit loppu ************

				 if(theOutput == kPlainText && saveObject)
					  saveObject->WritePSUpdatingSubText(theOutput);

				 statPoint.Point(statPoint.Point() + stationPointMovement);

				 // ***************************************
				 // ****** itse symbolit/numerot jne ******
				 // ***************************************

				 fInterruptSymbolGroup = false; //l‰heisyystesti-optiota varten
				 if(!(itsSymbols.WritePS(statPoint,theOutput)))
				   {
					 if(itsLogFile)
					   *itsLogFile << "*** ERROR: Aseman piirto ei onnistunut: " << endl;
					 return false;
				   }
			   } //if(FindQStationName()
			 else
			   {
				 NFmiString statName = statPoint.Station()->GetName();
				 if(!(statName == NFmiString("Tyhj‰") || statName == NFmiString("None")))
				   {
				     if(itsLogFile)
					   *itsLogFile << "  *** ERROR: Station missing from data: "
								   << static_cast<char *>(statName)
								   << endl;
				   }

			   }
			 fIsFirstStation = false;
		   } //while(itsStations.Next())

  // ************** Lopun AikaSidotutObjektit, jotka m‰‰ritelty loppuun ******************
		 // eli ***VakioSymboli
		 //     ***VakioTeksti
		 //     ***AikaTeksti
		 //     ***Nimip‰iv‰t
			DoTimeDependent(true,
					  currentStepInd,
					  theFile,
					  theOutput,
					  saveObject);

  // ********** Lopun AikaSidotutObjektit loppu **************
  } //if(itsData.Time())
		  else
			{   //HUOM QD:ss‰ pit‰‰ olla segmentin aika vaikka data-alkioissa k‰ytett‰issin eri tuntia
			  NFmiString firstText;
			  if(done)
			     firstText = "  aika tehty yll‰: ";
			  else if(supplementLater)
			     firstText = "  Segment time missing from this data: ";
			  else
			     firstText = "  *** ERROR: Segment time missing from data: ";

			  *itsLogFile << static_cast<char *>(firstText)
							<< static_cast<char *>(time.ToStr("DD.MM.YYYY HH"))
							<< " utc"
							<< endl;

			  // aikariippuvat siirrett‰v‰ eteenp‰in vaikkei ole dataa 20.3
			  objectIter.Reset();
			  object = static_cast<NFmiPressScaling *>(objectIter.Next());
			  while (object)
				{
				  object->Move(itsSteps[currentStepInd]);
				  object = static_cast<NFmiPressScaling *>(objectIter.Next());
				}
			}

		} //if(itsDataIter)

	  stationPointMovement += itsSteps[currentStepInd];
	  if(fIsLevelLoop)
		{

		}
	  else
	  {
		long hourStep;
		if(fIsPureRegTimeLoop)
		  hourStep = itsHourStep;
//		  time.ChangeByHours(itsHourStep);
		else
		  hourStep = itsTimeSteps[currentStepInd];
		  //time.ChangeByHours(itsTimeSteps[currentStepInd]);

		 time.ChangeByHours(hourStep);
	     StepTimeDependent(hourStep);
	  }

	  itsCurrentStep++;

	} //while(time/level Step)

  if(itsLogFile)
	*itsLogFile << "  " << statAll << " asemaa*aikaa(tai pintaa) k‰sitelty" << endl;

  long num = itsSymbols.NumOfMissing();
  if(num > 0 && itsLogFile)
	*itsLogFile << "  WARNING: " << num << " puuttuvaa dataa" << endl;
  itsSymbols.InitMissing(); // jotta ei manageri-k‰ytˆss‰ kertyisi

  for(int i=1; i< itsNumberOfSteps; i++)
	{
	  objectIter.Reset();
	  object = static_cast<NFmiPressScaling *>(objectIter.Next());
	  while (object)  // toistuvat writePs:t vaativat
		 {
		   object->Move(NFmiPoint(-itsUnscaledSteps[i].X(), -itsUnscaledSteps[i].Y()));
		   object = static_cast<NFmiPressScaling *>(objectIter.Next());
		 }
	}
  return true;

}
// ----------------------------------------------------------------------
/*!
 * Aika-askeltaminen. Vain nimip‰iv‰t, joilla ei dataa.
 */
// ----------------------------------------------------------------------
void NFmiPressParam::StepTimeDependent(long theHourStep)
{ 
	NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
	NFmiPressScaling * object;

	objectIter.Reset();
	object = static_cast<NFmiPressScaling *>(objectIter.Next());
	while (object)
	{
		object->ChangeByHours(theHourStep);
		object = static_cast<NFmiPressScaling *>(objectIter.Next());
	}
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------
bool NFmiPressParam::DoTimeDependent(bool isLastLoop,
							 FmiCounter theCurrentStepInd,
							 ofstream & theFile,
							 FmiPressOutputMode theOutput,
							 NFmiPressScaling *& theSaveObject)
{ 
  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling * object;

    objectIter.Reset();
	object = static_cast<NFmiPressScaling *>(objectIter.Next());
	while (object)
	{
		if(object->IsToWriteLast() == isLastLoop)
		{
			if(object->ActiveTimeIndex(GetCurrentStep()))
			{
				object->Set(itsScale, theFile);
				if(itsCurrentStep == 1 || !object->GetPrintOnce())
				{
					object->SetRotatingPoint(object->Place());
					if(theOutput != kPlainText)
					{
						if (!(object->WritePSUpdatingSubText(theOutput)))
						{
							if(itsLogFile)
							*itsLogFile << "*** ERROR: (timeDep)object->WritePS() in NFmiPressParam" << endl;
							return false;
						}
					//	if(theOutput == kPostScript)
							object->WriteGRestore();
					}
				else
					//ei kyll‰ k‰ytet‰ en‰‰ jos isLastLoop
					theSaveObject = object;
				}
			}

			object->Move(itsUnscaledSteps[theCurrentStepInd]);
		}
		object = static_cast<NFmiPressScaling *>(objectIter.Next());
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

NFmiString NFmiPressParam::MakeLogComment(void) const
{
  NFmiString res;
  if(fTwoPartLogComment)
	{
	  res = itsName;
	  res += NFmiString("; ");
	  res += itsLogComment;
	}
  else if(itsLogComment.IsValue())
	{
	  res = itsLogComment;
	}
  else
	res = itsName;

  return res;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStation Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::FindQDStationName(const NFmiStationPoint& theStation)
{
  if(!itsDataIter)
	return false;

  NFmiString toFindName = theStation.GetName();
  if(!itsDataIter->IsGrid()) // perinteinen asemannimell‰ haku
	{
	  NFmiStation station;
	  itsDataIter->ResetLocation();
	  while(itsDataIter->NextLocation())
		{
		  NFmiString dataName = itsDataIter->Location()->GetName();
		  if(toFindName == dataName)
			{
			  return true;
			}
		}
	}
  else
	{
	  // gridej‰ varten joutuu ottamaan pois t‰‰lt‰ ja sijoittamaan
	  // piirto-olioon, jossa lat/longilla haetaan interpoloiden tai l‰hint‰ pistett‰,
	  // t‰‰lt‰ onnistuu vain l‰hin piste

	  //true PITƒƒ OLLA
	  return true;
	  // return (itsData->Location(theStation.GetLocation()));
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStation Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetLonLat(NFmiStationPoint & theStation)
{
	if(!itsDataIter)
	{
		return false;
	}
	// ensin tarkistetaan jos on pistedata niin otetaan sielta aseman Lon/lat
  if(!itsDataIter->IsGrid())
	{

	  if(itsDataIter->Location(NFmiStation(*theStation.Station()).GetName()))
		{
		  theStation.SetLongitude(itsDataIter->Location()->GetLongitude());
		  theStation.SetLatitude(itsDataIter->Location()->GetLatitude());
		  return true;
		}
	  else
		{
		  *itsLogFile << "*** ERROR: datasta ei saatu lon/lat:ia" << endl;
		  return false;
		}
	}

  //muuten asema/LonLat-tiedostosta

  NFmiPoint lonLat;
  NFmiString statName(theStation.GetName());
  if(itsPressProduct->FindLonLatFromList(statName, lonLat))
  {
	  theStation.SetLongitude(lonLat.X());
	  theStation.SetLatitude(lonLat.Y());
	  return true;
  }
  return false;
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param scale Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressParam::SetStationScale(const NFmiRectScale& scale)
{
  itsCurrentStationScale = scale;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param scale Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressParam::SetScale(const NFmiRectScale& scale)
{
  itsScale = scale;
}

// ======================================================================
