// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressParam
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiPressParam.h"
#include "NFmiMultiParamMapping.h"
#include "NFmiPressArea.h"
#include "NFmiPressDataTimeText.h"
#include "NFmiPressNameDay.h"
#include "NFmiPressProduct.h"
#include "NFmiPressStationText.h"
#include "NFmiPsSymbol.h"
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
extern std::list<std::string> errors;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetSegmentCurrentTimeStatus(bool theStatus)
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

void NFmiPressParam::SetNewGeoArea(const NFmiPressArea &theArea)
{
  // HUOM GeoArea on NFmiArea joka PressArean j‰sen
  NFmiArea *newArea = 0;
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

bool NFmiPressParam::ChangeMaskNumber(unsigned long theMask, FmiEnumSpace theEnumSpace)
{
  if (!itsStations.IsLocations()) return false;

  itsEnvironment.ChangeMask(theMask, theEnumSpace);

  NFmiEnumConverter converter(itsEnvironment.GetEnumSpace());
  string name = converter.ToString(itsEnvironment.GetMaskNumber());
  itsName = NFmiString(name);

  itsStations.First();

  NFmiLocation *firstLocation = const_cast<NFmiLocation *>(itsStations.Location());
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
bool NFmiPressParam::SetFirstStation(const NFmiLocation &theLocation)
{
  if (!itsStations.IsLocations()) return false;

  itsStations.First();
  NFmiLocation *firstLocation = const_cast<NFmiLocation *>(itsStations.Location());
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

bool NFmiPressParam::SetStationRename(const NFmiRenaming &theRenaming)
{
  if (!itsStations.IsLocations()) return false;

  // ainoastaan asemanNimi‰
  //  NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsStationDepObjects);
  NFmiVoidPtrIterator objectIter(itsStationDepObjects);
  objectIter.Reset();
  NFmiPressStationText *object = static_cast<NFmiPressStationText *>(objectIter.Next());
  while (object)
  {
    object->SetNewName(theRenaming);
    object = static_cast<NFmiPressStationText *>(objectIter.Next());
  }
  itsName = theRenaming.newName;  // samalla muuttuu itse olion nimi

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
  if (itsStations.First())
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
  if (itsStations.First())
  {
    const NFmiStationPoint *statPoint =
        static_cast<const NFmiStationPoint *>(itsStations.Location());
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

bool NFmiPressParam::IsMaxMin(bool &theIsMax, NFmiPoint &theCorrectedPoint)
{
  std::list<FmiMaxMinPoint>::iterator pos;
  for (pos = itsMaxMinLocations.begin(); pos != itsMaxMinLocations.end(); ++pos)
  {
    if ((*pos).index == itsCurrentStationIndex)
    {
      theIsMax = (*pos).isMax;
      theCorrectedPoint = (*pos).point;
      return true;
    }
  }
  return false;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param dataName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetMaxMinPoints(void)
{
  if (fMaxMinSearched) return true;

  itsMaxMinLocations.clear();
  unsigned long index = 0;
  bool isMax, isMin;
  float allowedXDist, allowedYDist;
  NFmiSuperSmartInfo info(*itsDataIter);
  NFmiLocationBag tempStations(itsStations);
  tempStations.Reset();
  float maxMinSeparator = 1015.;  // huom parametririippuva, pit‰isi olla esim parametriin
  //  sidottu tai laskettava (min+max)/2;
  // 1015 yli keskipaineen jotta matalat herkemmin
  float smallerDistAllowed = 995.;  // samoin parametririippuva
  list<FmiMaxMinPoint>::iterator pos;
  vector<list<FmiMaxMinPoint>::iterator> eraseVector;
  vector<list<FmiMaxMinPoint>::iterator>::iterator posErase;
  NFmiPoint point, point1, point2, point3, pointDiffX, pointDiffY, pointJust;

  while (tempStations.Next())
  {
    eraseVector.clear();
    index++;

    isMax = false;
    isMin = false;
    info.Location(*tempStations.Location());
    if (index == 1)
    {
      point1 = (*static_cast<const NFmiStationPoint *>(tempStations.Location())).Point();
    }
    if (index == 2)
    {
      point2 = (*static_cast<const NFmiStationPoint *>(tempStations.Location())).Point();
      pointDiffX = point2 - point1;
    }
    if (index == info.GridYNumber() + 1)
    {
      point3 = (*static_cast<const NFmiStationPoint *>(tempStations.Location())).Point();
      pointDiffY = point3 - point1;
    }

    float value = info.FloatValue();
    if (value == kFloatMissing) continue;
    float value1 = info.PeekValue(0, 1, 0);
    float value2 = info.PeekValue(0, 1, -1);
    float value3 = info.PeekValue(0, 0, -1);
    float value4 = info.PeekValue(0, -1, -1);
    float value5 = info.PeekValue(0, -1, 0);
    float value6 = info.PeekValue(0, -1, 1);
    float value7 = info.PeekValue(0, 0, 1);
    float value8 = info.PeekValue(0, 1, 1);
    // suoraan kahden pisteen p‰‰ss‰
    float value9 = info.PeekValue(0, 2, 0);
    float value10 = info.PeekValue(0, 0, -2);
    float value11 = info.PeekValue(0, -2, 0);
    float value12 = info.PeekValue(0, 0, 2);

    // aivan reunoilla ei oteta
    if (value1 < kFloatMissing && value2 < kFloatMissing && value3 < kFloatMissing &&
        value4 < kFloatMissing && value5 < kFloatMissing && value6 < kFloatMissing &&
        value7 < kFloatMissing && value8 < kFloatMissing)
    {
      if (value > value1 && value > value2 && value >= value3 && value >= value4 &&
          value >= value5 && value >= value6 && value > value7 &&
          value > value8
          // reuna voi olla kahden pisteen p‰‰ss‰
          && (value > value9 || value9 == kFloatMissing) &&
          (value >= value10 || value10 == kFloatMissing) &&
          (value >= value11 || value11 == kFloatMissing) &&
          (value > value12 || value12 == kFloatMissing))
        isMax = true;
      if (value < value1 && value < value2 && value <= value3 && value <= value4 &&
          value <= value5 && value <= value6 && value < value7 && value < value8 &&
          value < value9 && value <= value10 && value <= value11 && value < value12)
        isMin = true;
    }
    bool near = false;

    if (isMax || isMin)
    {
      float significance;
      if (isMax)
        significance = value - maxMinSeparator;
      else
        significance = maxMinSeparator - value;

      /*
      float significance = fabs(value - ((value1 + value2 + value3 + value4 +
                value5 + value6 + value7 + value8) / 8.f));
      if(isMax)
              significance = significance*0.2f;
*/

      NFmiStationPoint *statPoint =
          static_cast<NFmiStationPoint *>(const_cast<NFmiLocation *>(tempStations.Location()));
      point = statPoint->Point();

      // s‰‰det‰‰n tarkkaan paikkaansa olettaen ett‰ kentt‰ paraabelin muotoinen molempiin suuntiin
      // ax**2 + bx + c = y
      // ekstreemin paikka miss‰ derivaatta nolla: 2ax + b = 0
      // a,b,c(=value) ratkaistavissa kolmen pisteen avulla

      // X-suunta
      float a1 = (value5 + value1) / 2.f - value;
      float b1 = (value1 - value5) / 2.f;
      float just1 = -b1 / (2.f * a1);
      pointJust = NFmiPoint(pointDiffX.X() * just1, pointDiffX.Y() * just1);

      // Y-suunta
      float a2 = (value3 + value7) / 2.f - value;
      float b2 = (value7 - value3) / 2.f;
      float just2 = -b2 / (2.f * a2);
      pointJust += NFmiPoint(pointDiffY.X() * just2, pointDiffY.Y() * just2);

      point += pointJust;

      // statPoint->Point(point); //sivuvaikutuksia??
      // statPoint->Point(NFmiPoint(20.,20.)); //sivuvaikutuksia??

      for (pos = itsMaxMinLocations.begin(); pos != itsMaxMinLocations.end(); ++pos)
      {
        allowedXDist = static_cast<float>(itsCheckDistance.X());
        allowedYDist = static_cast<float>(itsCheckDistance.Y());

        // kaksi v‰h‰n syvemp‰‰ matalaa hyv‰ksyt‰‰n l‰hemp‰n‰kin
        if (!isMax && !(*pos).isMax && value < smallerDistAllowed &&
            (*pos).value < smallerDistAllowed)
        {
          allowedXDist = allowedXDist * .65f;
          allowedYDist = allowedYDist * .65f;
        }
        if (abs((*pos).point.X() - point.X()) < allowedXDist &&
            abs((*pos).point.Y() - point.Y()) < allowedYDist)
        {
          // float mean = (value+(*pos).value)/2.f;
          if ((isMax && (*pos).isMax && value > (*pos).value)      // uusi ‰‰rev‰mpi maximi
              || (isMin && !(*pos).isMax && value < (*pos).value)  // uusi ‰‰rev‰mpi minimi
              || significance > (*pos).significance)
          //||  isMax && !(*pos).isMax && mean > meanOfGrid  //max korvaa minimin suurilla arvoilla
          //||  isMin && (*pos).isMax && mean  < meanOfGrid)) //min korvaa maximin pienill‰ arvoilla
          {
            eraseVector.push_back(pos);
            // pos = itsMaxMinLocations.erase(pos); //pos talteen ettei j‰‰ roikuumaan ilmaan
            // pos--;
          }
          else
          {
            near = true;
            break;
          }
        }
      }

      if (!near)
      {  // vain kun uusi rankattu merkitt‰v‰mm‰ksi kaikki l‰hipisteet poistetaan
        // jos on hila niit‰ voi kai olla vain max kaksi

        for (posErase = eraseVector.begin(); posErase != eraseVector.end(); ++posErase)
        {
          itsMaxMinLocations.erase(*posErase);
        }

        FmiMaxMinPoint maxMinPoint;
        maxMinPoint.value = value;
        maxMinPoint.point = point;
        maxMinPoint.isMax = isMax;
        maxMinPoint.index = index;
        maxMinPoint.significance = significance;
        itsMaxMinLocations.push_back(maxMinPoint);
      }
    }
  }
  fMaxMinSearched = true;
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

bool NFmiPressParam::CheckAndSetDistance(long theValue, const NFmiPoint &point)
{
  std::vector<FmiValuePoint>::iterator pos;

  for (pos = itsCheckLocations.begin(); pos != itsCheckLocations.end(); ++pos)
  {
    if ((*pos).value == theValue && abs((*pos).point.X() - point.X()) < itsCheckDistance.X() &&
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

bool NFmiPressParam::SetData(const NFmiString &dataName)
{
  // t‰nne eik‰ writeps:‰‰n koska tarvitaan pressScaling-objektien initialisoinnissa
  if (itsDataIter) delete itsDataIter;

  bool yearData;
  fYearData = false;
  NFmiQueryData *data = itsPressProduct->DataByName(dataName, yearData);

  if (data)
  {
    // hidastaa noin 10% jos tehd‰‰n SuperSmartInfoja (muuten pit‰isi
    // antaa m‰‰rittelyss‰ k‰ytet‰‰nkˆ SuperSmartia tai muuttaa data SuperSmartiksi
    // kun havaitaan ett‰ sit‰ tarvitaan)
    // kuitenkin noin 60% nopeutus kun SuperSmartit korvattu FastInfoilla
    // parametreissa ja muualla
    itsDataIter = new NFmiSuperSmartInfo(data);
    fYearData = yearData;

    itsDataIter->First();
    itsDataName = dataName;
  }
  else
    itsDataIter = 0;

  // vain PressDataTimeText n‰ist‰ datariippuvainen
  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling *scaleObject;
  objectIter.Reset();
  scaleObject = static_cast<NFmiPressScaling *>(objectIter.Next());

  while (scaleObject)
  {
    switch (scaleObject->ClassId())
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

  // ainoastaan asemanNimi‰
  NFmiPressStationText *object;
  NFmiVoidPtrIterator objectIter2(itsStationDepObjects);
  objectIter2.Reset();
  object = static_cast<NFmiPressStationText *>(objectIter2.Next());
  while (object)
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
 * \param dataName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::SetPrimaryData(const NFmiString &dataName)
{
  if (itsPrimaryDataIter) delete itsPrimaryDataIter;

  bool fYearData = false;
  NFmiQueryData *data = itsPressProduct->DataByName(dataName, fYearData);

  if (data)
  {
    itsPrimaryDataIter = new NFmiSuperSmartInfo(data);
    itsPrimaryDataIter->First();
    itsPrimaryDataName = dataName;
  }
  else
  {
    itsPrimaryDataIter = 0;
    *itsLogFile << "  *** ERROR: Primary data lost: " << static_cast<char *>(itsPrimaryDataName)
                << endl;
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

bool NFmiPressParam::SetAllTimes(const NFmiMetTime &theTime)
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
  SetLanguage(theLanguage);  // ensin oma

  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling *object;
  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while (object)
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

unsigned long NFmiPressParam::GetCurrentStationStep(void) const { return itsCurrentStationIndex; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressParam::ReadDescription(NFmiString &retString)
{
  NFmiPoint tableDirectPoints[kMaxNumOfTableElements];

  bool isRelativeTable = false;
  fTwoPartLogComment = false;
  NFmiString logName;

  bool symbolGroupCalled = false;
  bool fNextStationBackup = false;
  bool helpBool;
  bool firstStation = true;
  fIsTimeLoop = false;
  fIsLevelLoop = false;
  NFmiPoint stationStepSize;
  NFmiString dataFileName;
  NFmiValueString valueString;
  long long1, long2, long3;
  unsigned short numOfTableStations, currentStationNum = 0;  // taulukoissa
  unsigned short currentStationNumOnMap = 0;
  NFmiString string1, string2;
  NFmiPoint point0, point1, point2, scaledTimeLast, unscaledTimeLast, firstUnscaledPoint;
  NFmiPoint unscaledTableMove, scaledTableMove;
  double tableX1 = 0, tableX2, tableY1 = 0, tableY2;
  double x = 0, y = 0;
  double xmin, xmax, ymin, ymax;
  bool timeOrLevelTableSet = false;
  long statNum;
  double lat = kFloatMissing;
  double lon = kFloatMissing;

  NFmiVoidPtrList *newNames = new NFmiVoidPtrList;

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
  NFmiPoint stepSize = NFmiPoint(20., 0.);
  NFmiPoint unscaledStepSize = NFmiPoint(20., 0.);
  NFmiStationPoint newStation;
  bool dataTimeGiven = false;
  bool takeToMainArea = false;
  FmiLevelType levelType = kFmiPressureLevel;
  float alternatingSize = -1.;
  while (itsIntObject != dEnd || itsCommentLevel)
  {
    if (itsLoopNum > itsMaxLoopNum)
    {
      if (itsLogFile)
        *itsLogFile << "*** ERROR: Maximum length of product exceeded in #pressparam" << endl;
      retString = itsString;
      return false;
    }
    itsLoopNum++;
    if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
    switch (itsIntObject)
    {
      case dOther:  // ylim‰‰r‰ist‰ roinaa,
      {
        if (itsLogFile)
          *itsLogFile << "*** ERROR: Unknown keyword in #pressparam: "
                      << static_cast<char *>(itsObject) << endl;
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
        if (!ReadEqualChar()) break;

        itsLogComment = ReadString();

        ReadNext();
        break;
      }
      case dParamSubViews:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1)) numberOfTimeSteps = static_cast<unsigned short>(long1);

        ReadNext();
        break;
      }
      case dParamStepSize:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(x))
        {
          *itsDescriptionFile >> itsObject;
          valueString = itsObject;
          if (valueString.IsNumeric())
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
          point1.Set(x, y);
          stepSize = NFmiPoint(point1.X() * itsScale.GetScaling().X(),
                               point1.Y() * itsScale.GetScaling().Y());
          unscaledStepSize.Set(x, y);
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
        if (!ReadEqualChar()) break;

        if (Read2Double(xmax, ymax))
        {
          fIsLevelLoop = true;
          fIsTimeLoop = false;
          point1.Set(xmax, ymax);
          unscaledTimeLast.Set(xmax, ymax);
          scaledTimeLast = itsScale.Scale(point1);
          scaledTimeLast = itsCurrentStationScale.Scale(scaledTimeLast);
          timeOrLevelTableSet = true;

          if (ReadLong(long1))
          {
            numberOfLevelSteps = FmiMin(kMaxNumOfTableElements, static_cast<unsigned short>(long1));
          }
        }
        // itsStepSize joudutaan asettamaan luupin j‰lkeen alla
        ReadNext();
        break;
      }
      case dSegmentHybridLevel:
        levelType = kFmiHybridLevel;
      case dSegmentLevel:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1))
        {
          if (++currentLevelInd > kMaxNumOfTableElements)
          {
            *itsLogFile << "*** ERROR: Maximum allowed number of pressure levels exceeded: "
                        << numOfTableLevels << endl;
            --currentLevelInd;
          }
          else
          {
            NFmiLevel level = NFmiLevel(levelType, static_cast<float>(long1));
            itsLevels[currentLevelInd] = level;
            fIsLevelLoop = true;  // HUOM vaikka yksi vain
            fIsTimeLoop = false;
            timeOrLevelTableSet = true;
          }
        }
        levelType = kFmiPressureLevel;  // default for next
        ReadNext();
        break;
      }
      case dHybridLevelPlace:
        levelType = kFmiHybridLevel;
      case dLevelPlace:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1))
        {
          if (++currentLevelInd > kMaxNumOfTableElements)
          {
            *itsLogFile << "*** ERROR: Maximum allowed number of pressure levels exceeded: "
                        << numOfTableLevels << endl;
            ReadTwo(x, y);  // luetaan vaan pois
            --currentLevelInd;
          }
          else
          {
            NFmiLevel level = NFmiLevel(levelType, static_cast<float>(long1));
            itsLevels[currentLevelInd] = level;
            if (ReadTwo(x, y))
            {
              fIsLevelLoop = true;
              fIsTimeLoop = false;
              timeOrLevelTableSet = true;
              tableDirectPoints[currentLevelInd].Set(x, y);
            }
          }
        }
        levelType = kFmiPressureLevel;  // default for next
        ReadNext();
        break;
      }
      case dTimeTable:
      {
        if (!ReadEqualChar()) break;

        if (Read2Double(xmax, ymax))
        {
          fIsLevelLoop = false;
          fIsTimeLoop = true;
          point1.Set(xmax, ymax);
          unscaledTimeLast.Set(xmax, ymax);
          scaledTimeLast = itsScale.Scale(point1);
          scaledTimeLast = itsCurrentStationScale.Scale(scaledTimeLast);

          if (ReadLong(long1))
          {
            numberOfTimeSteps = static_cast<unsigned short>(long1);
            if (numberOfTimeSteps >= kMaxNumOfTableElements)
            {
              *itsLogFile
                  << "*** WARNING: Maximum number of time steps allocated for tables exceeded: "
                  << numberOfTimeSteps << "; slowing down" << endl;
            }
            if (ReadLong(long1))
            {
              itsHourStep = static_cast<unsigned short>(long1);
              timeOrLevelTableSet = true;
              if (itsHourStep < 24) SetHourLoop(true);
              currentTotalTimeDiff = (numberOfTimeSteps - 1) * itsHourStep;
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
        if (!ReadEqualChar()) break;

        if (Read2Double(xmax, ymax))
        {
          isRelativeTable = true;
          fIsLevelLoop = false;
          fIsTimeLoop = true;

          point1.Set(xmax, ymax);
          unscaledTableMove.Set(xmax, ymax);
          scaledTableMove = point1;
          scaledTableMove = itsScale.Scale(point1);  // globaali skaalaus

          // asemamittakaava ei saa vaikuttaa mutta koko tuotteen vaikuttaa
          // eli siirtym‰t on mitattava ilman globaalia skaalausta
          // scaledTableMove = itsCurrentStationScale.Scale(scaledTableMove);

          if (ReadLong(long1))
          {
            numberOfTimeSteps = static_cast<unsigned short>(long1);
            if (numberOfTimeSteps >= kMaxNumOfTableElements)
            {
              *itsLogFile
                  << "*** WARNING: Maximum number of time steps allocated for tables exceeded: "
                  << numberOfTimeSteps << "; slowing down" << endl;
            }
            if (ReadLong(long1))
            {
              itsHourStep = static_cast<unsigned short>(long1);
              timeOrLevelTableSet = true;
              if (itsHourStep < 24) SetHourLoop(true);
              currentTotalTimeDiff = (numberOfTimeSteps - 1) * itsHourStep;
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
        if (!ReadEqualChar()) break;

        if (ReadOne(long1))
        {
          if (++currentTimeInd > kMaxNumOfTableElements)
          {
            *itsLogFile << "*** ERROR: Maximum number of time steps exceeded in #pressparam: "
                        << currentTimeInd << endl;
            ReadTwo(x, y);  // luetaan vaan pois
            --currentTimeInd;
          }
          else
          {
            // edellisen siitym‰ t‰h‰n!!
            itsTimeSteps[currentTimeInd - 1] = long1 - currentTotalTimeDiff;
            currentTotalTimeDiff += itsTimeSteps[currentTimeInd - 1];
            if (ReadTwo(x, y))
            {
              fIsLevelLoop = false;
              fIsTimeLoop = true;
              timeOrLevelTableSet = true;
              fIsPureRegTimeLoop = false;
              tableDirectPoints[currentTimeInd].Set(x, y);  // miksi t‰m‰ ind ??? lasketaan alla
            }
          }
        }

        ReadNext();
        break;
      }
      case dTimePlaceRelative:  // EI VALMIS
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1))
        {
          if (++currentTimeInd > kMaxNumOfTableElements)
          {
            *itsLogFile << "*** ERROR: Maximum number of time steps exceeeded in #pressparam: "
                        << currentTimeInd << endl;
            ReadTwo(x, y);  // luetaan vaan pois
            --currentTimeInd;
          }
          else
          {
            // edellisen siirtym‰ t‰h‰n!!
            itsTimeSteps[currentTimeInd - 1] = long1 - currentTotalTimeDiff;
            currentTotalTimeDiff += itsTimeSteps[currentTimeInd - 1];
            if (ReadTwo(x, y))
            {
              // NFmiPoint point3, point5;

              fIsLevelLoop = false;
              fIsTimeLoop = true;
              timeOrLevelTableSet = true;
              fIsPureRegTimeLoop = false;

              // point1 = itsCurrentStationScale.Scale(firstUnscaledPoint);
              // point2 = itsScale.Scale(point1);

              point1 = firstUnscaledPoint;
              point1 = itsCurrentStationScale.Scale(point1);
              point1 = itsScale.Scale(point1);
              point2 = NFmiPoint(x, y);
              point1 += point2;
              tableDirectPoints[currentTimeInd].Set(point1.X(), point1.Y());
              // point11 = itsCurrentStationScale.Scale(point11);
              // point11 = itsScale.Scale(point11);
              // tableDirectPoints[currentTimeInd].Set(point11.X(),point11.Y());
              /*
                                                              point3 =
                 itsCurrentStationScale.UnScale(NFmiPoint(x,y));
                                                              point5 = firstUnscaledPoint +
                 NFmiPoint(x,y);
                                                              tableDirectPoints[currentTimeInd].Set(point5.X(),point5.Y());
              */
            }
          }
        }

        ReadNext();
        break;
      }
      case dStationTable:
      {
        if (!ReadEqualChar()) break;

        if (Read4Double(xmin, ymin, xmax, ymax))
        {
          *itsDescriptionFile >> itsObject;
          valueString = itsObject;
          if (valueString.IsNumeric())
          {
            tableX1 = xmin;
            tableX2 = xmax;
            tableY1 = ymin;
            tableY2 = ymax;
            numOfTableStations = static_cast<int>(valueString);
            *itsDescriptionFile >> itsObject;
            itsString = itsObject;
          }
          else  // yhteensopivuus vanhan formaatin kanssa
          {
            tableX1 = xmin;
            tableX2 = xmin;
            tableY1 = ymin;
            tableY2 = xmax;  //! ok
            numOfTableStations = static_cast<unsigned short>(ymax);
            itsString = itsObject;
          }
          stationStepSize.Set((tableX2 - tableX1) / (numOfTableStations - 1),
                              (tableY2 - tableY1) / (numOfTableStations - 1));
        }
        currentStationNum = 0;

        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dSegmentDataFile:
      {
        if (!ReadEqualChar()) break;

        itsDataName = ReadString();
        itsDataName.LowerCase();

        if (dataTimeGiven)
        {
          *itsLogFile << "*** ERROR: Data '" << static_cast<char *>(itsDataName)
                      << "' in #pressparam must be defined prior to" << endl;
          *itsLogFile << "       'UseDataStartTime' and 'RelativeHoursFromData'" << endl;
          *itsLogFile << "       -> #pressparam possibly uses an incorrect time" << endl;
        }
        SetData(itsDataName);

        ReadNext();
        break;
      }
      case dSegmentPrimaryDataFile:
      {
        if (!ReadEqualChar()) break;

        itsPrimaryDataName = ReadString();
        itsPrimaryDataName.LowerCase();

        SetPrimaryData(itsPrimaryDataName);

        ReadNext();
        break;
      }
      case dDataNotNeeded:
      {
        fStationNotNeeded = true;

        ReadNext();
        break;
      }

      case dStationRelocation:
      {
        if (SetTwo(x, y))
        {
          float upRight = 100.;
          itsCurrentStationScale.SetStartScales(
              NFmiRect(NFmiPoint(0., 0.), NFmiPoint(upRight, upRight)));
          itsCurrentStationScale.SetEndScales(
              NFmiRect(NFmiPoint(x, y), NFmiPoint(upRight + x, upRight + y)));
        }
        else
          *itsLogFile << "*** ERROR: StationRelocation parameters incorrect" << endl;

        break;
      }
      case dStationDefArea:
      {
        if (!ReadEqualChar()) break;

        if (Read4Double(xmin, ymin, xmax, ymax))
        {
          if (xmin == xmax || ymin == ymax)
            *itsLogFile << "*** ERROR: StationDefArea min == max" << endl;
          else
            itsCurrentStationScale.SetStartScales(
                NFmiRect(NFmiPoint(xmin, ymin), NFmiPoint(xmax, ymax)));
        }

        ReadNext();
        break;
      }
      case dStationPlotArea:
      {
        if (!ReadEqualChar()) break;

        if (Read4Double(xmin, ymin, xmax, ymax))
        {
          // MoveSegmentPlaceConditionally(xmin,ymin);
          // MoveSegmentPlaceConditionally(xmax,ymax);
          if (xmin == xmax || ymin == ymax)
            *itsLogFile << "*** ERROR: StationPlotArea min == max" << endl;
          else
            itsCurrentStationScale.SetEndScales(
                NFmiRect(NFmiPoint(xmin, ymin), NFmiPoint(xmax, ymax)));
        }
        ReadNext();
        break;
      }
      case dProducer:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1)) itsProducer = long1;

        ReadNext();
        break;
      }
      case dStationNumberObject:
      case dStationTextObject:
      {
        if (!itsDataIter)  // ilman data ei voi saada asemannimi‰
        {
          *itsLogFile << "*** ERROR: StationName used without a defined data source" << endl;
        }
        NFmiPressStationText *text = new NFmiPressStationText;
        if (itsIntObject == dStationNumberObject) text->SetStationNumberMode();
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

        if (text->ReadDescription(itsString))
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
        NFmiPsSymbol *image = new NFmiPsSymbol;
        image->SetHome(GetHome());
        image->SetLogFile(itsLogFile);
        image->SetDescriptionFile(itsDescriptionFile);
        if (image->ReadDescription(itsString))
          itsTimeDepObjects.Add(image);
        else
          delete image;

        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dTextObject:
      {
        NFmiPressText *text = new NFmiPressText;
        text->SetEnvironment(itsEnvironment);
        text->SetHome(GetHome());
        text->SetLogFile(itsLogFile);
        text->SetDescriptionFile(itsDescriptionFile);
        text->SetLanguage(itsLanguage);
        if (text->ReadDescription(itsString))
          itsTimeDepObjects.Add(text);
        else
          delete text;

        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dTimeTextObject:
      {
        if (!itsDataIter)  // ilman data ei voi saada aikateksti‰
        {
          *itsLogFile << "*** ERROR: TimeText used without a data source in the segment" << endl;
        }
        NFmiPressDataTimeText *text = new NFmiPressDataTimeText;
        text->SetEnvironment(itsEnvironment);
        text->SetHome(GetHome());
        text->SetLogFile(itsLogFile);
        text->SetDescriptionFile(itsDescriptionFile);
        text->SetData(itsDataIter);
        text->SetLanguage(itsLanguage);
        text->Place(firstUnscaledPoint);
        if (text->ReadDescription(itsString))
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
        //			*itsLogFile << "*** ERROR: Nimip‰iv‰‰ yritetty ilman dataa
        // segmentiss‰"
        //<< endl;
        //
        //			  }

        NFmiPressNameDay *text = new NFmiPressNameDay;
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
        if (text->ReadDescription(itsString))
        {
          text->ScalePlotting();
          if (!itsPressProduct->GetNameDay(text->GetLanguage()))
          {
            NFmiNameDay *nameDay = new NFmiNameDay;
            itsPressProduct->SetNameDay(nameDay, text->GetLanguage());  // omistaja
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
        if (symbolGroupCalled)
        {
          *itsLogFile << "*** ERROR: Cannot use more than one #symbolgroup in a #pressparam "
                         "segment, bypassing"
                      << endl;
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
          SetPostReadingTimes();                 // aikamuutos pit‰isi antaa ennen piirtoryhmi‰
          itsSymbols.SetTime(itsFirstPlotTime);  // Ei k‰ytet‰ kuin tuntia, jota voi ylitt‰‰
                                                 // j‰seniss‰(mutta p‰iv‰‰ ei)
          itsSymbols.SetHourLoop(IsHourLoop());

          itsSymbols.ReadDescription(itsString);
        }

        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dRelDay:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1))
          itsFirstDeltaDays =
              static_cast<short>(JustifyByLeaps(long1 + itsEnvironment.GetDayAdvance()));

        ReadNext();
        break;
      }
      case dHour:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1)) itsFirstPlotHours = static_cast<unsigned short>(long1);

        ReadNext();
        break;
      }
      case dHourStep:
      {
        if (!ReadEqualChar()) break;

        if (ReadOne(long1))
        {
          itsHourStep = static_cast<unsigned short>(long1);
          if (itsHourStep < 25) SetHourLoop(true);
        }

        ReadNext();
        break;
      }
      case dDataTime:
      {
        itsPressProduct->ReadData();
        if (itsDataIter)
        {
          itsDataIter->Reset();
          itsDataIter->NextTime();
          itsFirstPlotTime = (static_cast<NFmiQueryInfo *>(itsDataIter))->Time();
          itsFirstPlotHours = itsFirstPlotTime.GetHour();
        }
        else
        {
          *itsLogFile << "*** ERROR: Trying to read time from undefined data source in a "
                         "#pressparam segment"
                      << endl;
        }

        dataTimeGiven = true;

        ReadNext();
        break;
      }
      case dRelativeDataTime:
      {
        SetOne(long1);

        itsPressProduct->ReadData();

        if (itsDataIter)
        {
          itsDataIter->Reset();
          itsDataIter->NextTime();
          itsFirstPlotTime = (static_cast<NFmiQueryInfo *>(itsDataIter))->Time();
          itsFirstPlotTime.ChangeByHours(long1);
          itsFirstPlotHours = itsFirstPlotTime.GetHour();
        }
        else
        {
          *itsLogFile << "*** ERROR: Trying to read time from undefined data source in a "
                         "#pressparam segment"
                      << endl;
        }
        dataTimeGiven = true;

        break;
      }
      case dRelHour:  // ei PressTimeDscr:iin koska ei dataelementeill‰
      {
        if (!ReadEqualChar()) break;
        ReadTwo(long1, long2);
        itsFirstPlotTime = NFmiMetTime(60);  // huom tunnin res.
        itsFirstPlotTime.ChangeByHours(long1);
        itsFirstPlotTime.NextMetTime(static_cast<short>(long2 * 60));

        *itsDescriptionFile >> itsObject;
        valueString = itsObject;
        if (valueString.IsNumeric())
        {
          // lasketulla itsFirstPlotTime:ll‰ ei tee mit‰‰n
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
      case dDistanceCheck:
      {
        if (SetTwo(x, y))
        {
          point1 = itsCurrentStationScale.Scale(NFmiPoint(x, y));
          point2 = itsScale.Scale(point1);
          SetDistanceCheck(point2);
        }
        break;
      }
      case dBackupStation:
      {
        fNextStationBackup = true;
        ReadNext();
        break;
      }
      case dStationNameReplace:  // stationiin verrattuna lis‰ykset: *** PrintName
      {
        lon = kFloatMissing;
        lat = kFloatMissing;
        bool hasOwnPoint = false;  // talukkoEscapea varten

        if (!ReadEqualChar()) break;

        helpBool = true;

        valueString = ReadString();

        if (valueString.IsNumeric())
        {
          statNum = static_cast<int>(valueString);
          *itsDescriptionFile >> itsObject;
          string1 = itsObject;
          *itsDescriptionFile >> itsObject;
          string2 = itsObject;
        }
        else
        {
          statNum = itsLoopNum;   // toimii satunnaislukuna,n‰in saadaan bagiin useita samannimisi‰
          string1 = valueString;  // Object;
        }
        string2 = ReadString();

        *itsDescriptionFile >> itsObject;
        valueString = itsObject;      // parillinen m‰‰r‰ lukuja sallittu: 0 2 tai 4
        if (valueString.IsNumeric())  // >= 1 luku
        {
          lon = static_cast<double>(valueString);
          if (ReadDouble(lat))  // >= 2 lukua
          {
            *itsDescriptionFile >> itsObject;
            valueString = itsObject;
            if (valueString.IsNumeric())  // >= 3 lukua
            {
              x = static_cast<short>(valueString);
              if (!ReadDouble(y)) helpBool = false;  // 3 lukua virhe
              *itsDescriptionFile >> itsObject;      // 4 lukua
              itsString = itsObject;
              hasOwnPoint = true;
            }
            else  // 2 lukua
            {
              if (!fLonLatMode)
              {
                x = lon;
                y = lat;
                lon = kFloatMissing;
                lat = kFloatMissing;
                hasOwnPoint = true;
              }
              else
                hasOwnPoint = false;

              itsString = valueString;
            }
            MoveSegmentPlaceConditionally(x, y);
          }
          else  // 1 luku
          {
            itsString = valueString;
            if (numOfTableStations < 1)
            {
              *itsLogFile << "*** ERROR: Station " << static_cast<char *>(string1)
                          << " which is not in the table should have a defined location" << endl;
              helpBool = false;
            }
          }
        }
        else  // 0 lukua
        {
          itsString = valueString;
          if (numOfTableStations < 1) helpBool = false;
        }

        if (helpBool)
        {
          if (numOfTableStations > 1 && !hasOwnPoint)
          {
            currentStationNum++;

            x = stationStepSize.X() * (currentStationNum - 1) + tableX1;
            y = stationStepSize.Y() * (currentStationNum - 1) + tableY1;
          }
          point1 = itsCurrentStationScale.Scale(NFmiPoint(x, y));
          point2 = itsScale.Scale(point1);
          NFmiStationPoint station =
              NFmiStationPoint(NFmiStation(statNum, string1, lon, lat), point2);
          station.SetAlternatingSizeFactor(alternatingSize);
          if (fNextStationBackup) station.SetBackup(true);
          itsStations.AddLocation(station, false);
          if (firstStation)
          {
            // EI TOIMI
            itsName = station.GetName();  // EI TOIMI vai toimiiko
            firstUnscaledPoint = NFmiPoint(x, y);
            firstStation = false;
          }

          // *** PrintName

          NFmiRenaming *renaming = new NFmiRenaming;
          renaming->originalName = string1;
          renaming->newName = string2;
          newNames->Add(renaming);
        }
        fNextStationBackup = false;
        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dStation:
      {
        lon = kFloatMissing;
        lat = kFloatMissing;
        bool hasOwnPoint = false;  // talukkoEscapea varten

        if (!ReadEqualChar()) break;

        helpBool = true;

        valueString = ReadString();
        if (valueString.IsNumeric())
        {
          statNum = static_cast<int>(valueString);
          *itsDescriptionFile >> itsObject;
          string1 = itsObject;
        }
        else
        {
          statNum = itsLoopNum;  // toimii satunnaislukuna,n‰in saadaan bagiin useita samannimisi‰
          string1 = valueString;
        }
        valueString = ReadString();
        // parillinen m‰‰r‰ lukuja sallittu: 0 2 tai 4
        if (valueString.IsNumeric())  // >= 1 luku
        {
          lon = static_cast<double>(valueString);
          if (ReadOne(lat))  // >= 2 lukua
          {
            *itsDescriptionFile >> itsObject;
            valueString = itsObject;
            if (valueString.IsNumeric())  // >= 3 lukua
            {
              x = static_cast<double>(valueString);
              if (!ReadOne(y)) helpBool = false;  // 3 lukua virhe
              *itsDescriptionFile >> itsObject;   // 4 lukua
              itsString = itsObject;
              hasOwnPoint = true;
            }
            else  // 2 lukua
            {
              if (!fLonLatMode)
              {
                x = lon;
                y = lat;
                lon = kFloatMissing;
                lat = kFloatMissing;
                hasOwnPoint = true;
              }
              else
                hasOwnPoint = false;

              itsString = valueString;
            }
            MoveSegmentPlaceConditionally(x, y);
          }
          else  // 1 luku
          {
            itsString = valueString;
            helpBool = false;
          }
        }
        else  // 0 lukua
        {
          itsString = valueString;
          if (numOfTableStations < 1)
          {
            *itsLogFile << "*** ERROR: Station " << static_cast<char *>(string1)
                        << " which is not in a table should have a defined location" << endl;
            helpBool = false;
          }
        }

        if (helpBool)
        {
          if (numOfTableStations > 1 && !hasOwnPoint &&
              !fNextStationBackup)  // vara-asema edellisen paikalle
          {
            currentStationNum++;
            x = stationStepSize.X() * (currentStationNum - 1) + tableX1;
            y = stationStepSize.Y() * (currentStationNum - 1) + tableY1;
          }
          point1 = itsCurrentStationScale.Scale(NFmiPoint(x, y));
          point2 = itsScale.Scale(point1);

          NFmiStationPoint station(NFmiStation(statNum, string1, lon, lat), point2);
          // t‰m‰ temppu jotta koordinaatit oikein PsSymbolille
          // voidaan tallettaa point1:een kun kaikki periytyy NFmiPsSymbolista
          // lat/loniahan ei tarvita mihink‰‰n
          station.SetAlternatingSizeFactor(alternatingSize);
          if (fNextStationBackup) station.SetBackup(true);
          itsStations.AddLocation(station, false);
          if (firstStation)
          {
            itsName = station.GetName();  // EI TOIMI vai toimiiko
            firstUnscaledPoint = NFmiPoint(x, y);
            firstStation = false;
          }
        }
        fNextStationBackup = false;
        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dStationPlaceOnMap:
      {
        if (!ReadEqualChar()) break;

        NFmiString name;
        if (ReadOneStringAndTwoNumbers(name, x, y))
        {
          MoveSegmentPlaceConditionally(x, y);
          if (itsArea.GetArea())
          {
            if (fCoordinatesFromMainMap)
            {
              NFmiArea *mainArea = itsPressProduct->itsMainArea;
              NFmiArea *ownArea = itsArea.GetArea();
              if (!mainArea || !ownArea)
              {
                *itsLogFile << "*** ERROR: "
                            << "Main or own map missing" << endl;
              }
              else
              {
                double xZoom = ownArea->Width() / mainArea->Width();
                x = (x - mainArea->Left()) * xZoom + ownArea->Left();
                double yZoom = ownArea->Height() / mainArea->Height();
                y = (y - mainArea->Bottom()) * yZoom + ownArea->Bottom();
              }
            }
            currentStationNumOnMap++;
            int version = itsEnvironment.GetVersion();
            point0 = NFmiPoint(x, y);

            double bottom = (itsArea.GetArea())->Bottom();
            double top = (itsArea.GetArea())->Top();
            double y0 = point0.Y();
            point1.Set(point0.X(), bottom - (y0 - top));
            NFmiPoint lonlat;
            if (version >= 20)
            {
              point1 = itsCurrentStationScale.Scale(NFmiPoint(x, y));
              bottom = (itsArea.GetArea())->Bottom();
              top = (itsArea.GetArea())->Top();
              y0 = point1.Y();
              NFmiPoint point3;
              point3.Set(point1.X(), bottom - (y0 - top));
              lonlat = (itsArea.GetArea()->ToLatLon(point3));
            }
            else
              lonlat = (itsArea.GetArea()->ToLatLon(point1));

            lon = lonlat.X();
            lat = lonlat.Y();
            if (version < 20) point1 = itsCurrentStationScale.Scale(NFmiPoint(x, y));
            point2 = itsScale.Scale(point1);

            if (!name.IsValue())
            {
              name = NFmiString("As");
              name += NFmiValueString(
                  static_cast<int>(currentStationNumOnMap));  // uniikki nimi jokaiselle
            }
            NFmiStationPoint station(NFmiStation(currentStationNumOnMap, name, lon, lat), point2);
            station.SetAlternatingSizeFactor(alternatingSize);
            if (fNextStationBackup) station.SetBackup(true);
            itsStations.AddLocation(station, false);
            if (firstStation)
            {
              itsName = station.GetName();
              firstUnscaledPoint = NFmiPoint(x, y);
              firstStation = false;
            }
            firstStation = false;
          }
          else
            *itsLogFile << "*** ERROR: Missing map projection" << endl;
        }
        fNextStationBackup = false;
        ReadNext();
        break;
      }
      case dStationPlaceMatrixOnMap:
      {
        float xStart = 0, yStart = 0, xStop = 0, yStop = 0, xIncr = 0, yIncr = 0;
        if (SetSix(xStart, yStart, xStop, yStop, xIncr, yIncr))
        {
          NFmiString name;
          if (itsArea.GetArea())
          {
            bool errReported = false;
            for (x = xStart; x <= xStop; x += xIncr)
            {
              for (y = yStart; y <= yStop; y += yIncr)
              {
                if (fCoordinatesFromMainMap)
                {
                  NFmiArea *mainArea = itsPressProduct->itsMainArea;
                  NFmiArea *ownArea = itsArea.GetArea();
                  if (!mainArea || !ownArea)
                  {
                    if (!errReported) *itsLogFile << "*** ERROR: Main or own map missing" << endl;
                    errReported = true;
                  }
                  else
                  {
                    double xZoom = ownArea->Width() / mainArea->Width();
                    x = (x - mainArea->Left()) * xZoom + ownArea->Left();
                    double yZoom = ownArea->Height() / mainArea->Height();
                    y = (y - mainArea->Bottom()) * yZoom + ownArea->Bottom();
                  }
                }
                currentStationNumOnMap++;
                point0 = NFmiPoint(x, y);

                double bottom = (itsArea.GetArea())->Bottom();
                double top = (itsArea.GetArea())->Top();
                double y0 = point0.Y();
                point1.Set(point0.X(), bottom - (y0 - top));

                NFmiPoint lonlat(itsArea.GetArea()->ToLatLon(point1));
                lon = lonlat.X();
                lat = lonlat.Y();
                point1 = itsCurrentStationScale.Scale(NFmiPoint(x, y));
                point2 = itsScale.Scale(point1);
                if (!name.IsValue())
                {
                  name = NFmiString("As");
                  name += NFmiValueString(
                      static_cast<int>(currentStationNumOnMap));  // uniikki nimi jokaiselle
                }
                NFmiStationPoint station(NFmiStation(currentStationNumOnMap, name, lon, lat),
                                         point2);
                station.SetAlternatingSizeFactor(alternatingSize);
                if (fNextStationBackup) station.SetBackup(true);
                itsStations.AddLocation(station, false);
                if (firstStation)
                {
                  itsName = station.GetName();
                  firstUnscaledPoint = NFmiPoint(x, y);
                  firstStation = false;
                }
                firstStation = false;
              }
            }
          }
          else
            *itsLogFile << "*** ERROR: Map projection missing" << endl;
        }
        else
          *itsLogFile << "*** ERROR: Location matrix not fully defined" << endl;

        fNextStationBackup = false;
        // ReadNext();
        break;
      }
      case dStationNameOnMap:
      {
        if (!ReadEqualChar()) break;

        NFmiString name = ReadString();
        {
          if (itsArea.GetArea())
          {
            currentStationNumOnMap++;
            NFmiStationPoint statPoint(NFmiStation(currentStationNumOnMap, name),
                                       NFmiPoint(10, 10));
            statPoint.SetAlternatingSizeFactor(alternatingSize);
            if (SetLonLat(statPoint))
            {
              point0 = (itsArea.GetArea())
                           ->ToXY(NFmiPoint(statPoint.GetLongitude(), statPoint.GetLatitude()));
              double bottom = (itsArea.GetArea())->Bottom();
              double top = (itsArea.GetArea())->Top();
              double y0 = point0.Y();
              point1 = NFmiPoint(point0.X(), bottom - y0 + top);
              point2 = itsScale.Scale(point1);
              NFmiStationPoint station(
                  NFmiStation(currentStationNumOnMap, name, kFloatMissing, kFloatMissing), point2);
              station.SetAlternatingSizeFactor(alternatingSize);
              if (fNextStationBackup) station.SetBackup(true);
              itsStations.AddLocation(station, false);
              if (firstStation)
              {
                itsName = station.GetName();
                firstUnscaledPoint = NFmiPoint(point1);
                firstStation = false;
              }
              firstStation = false;
            }
            else
              *itsLogFile << "*** ERROR: "
                          << "lat/lon missing: " << static_cast<char *>(name) << endl;
          }
          else
            *itsLogFile << "*** ERROR: map projection missing" << endl;
        }

        fNextStationBackup = false;
        ReadNext();
        break;
      }
      case dStationLonLatOnMap:
      {
        if (!ReadEqualChar()) break;

        NFmiString name;
        if (ReadOneStringAndTwoNumbers(name, lon, lat))
        {
          if (itsArea.GetArea())
          {
            currentStationNumOnMap++;
            NFmiPoint lonlat(lon, lat);
            point0 = itsArea.GetArea()->ToXY(lonlat);
            lon = lonlat.X();
            lat = lonlat.Y();
            double bottom = (itsArea.GetArea())->Bottom();
            double top = (itsArea.GetArea())->Top();
            double y0 = point0.Y();
            point1 = NFmiPoint(point0.X(), bottom - (y0 - top));
            point2 = itsScale.Scale(point1);
            if (!name.IsValue())
            {
              name = NFmiString("As");
              name += NFmiValueString(
                  static_cast<int>(currentStationNumOnMap));  // uniikki nimi jokaiselle
            }
            NFmiStationPoint station(NFmiStation(currentStationNumOnMap, name, lon, lat), point2);
            station.SetAlternatingSizeFactor(alternatingSize);
            if (fNextStationBackup) station.SetBackup(true);
            itsStations.AddLocation(station, false);
            if (firstStation)
            {
              itsName = station.GetName();
              firstUnscaledPoint = NFmiPoint(point1);
              firstStation = false;
            }
            firstStation = false;
          }
          else
            *itsLogFile << "*** ERROR: Map projection missing" << endl;
        }

        fNextStationBackup = false;
        ReadNext();
        break;
      }

      case dSegmentMapArea:
      {
        itsArea.SetLogFile(itsLogFile);
        itsArea.SetDescriptionFile(itsDescriptionFile);
        itsArea.SetXyRequest(
            !fIsAreaOperation);  // mittoja ei tarvita jos k‰ytet‰‰n alueoperaatioihin
        itsArea.SetProduct(itsPressProduct);
        itsArea.SetEnvironment(itsEnvironment);
        itsArea.ReadDescription(itsString);

        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dMainArea:
      {
        takeToMainArea = true;  // toteutetaan lopussa jotta j‰rjestyksell‰ ei v‰li‰

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
        if (itsArea.GetArea())
        {
          if (!itsDataIter)
          {
            *itsLogFile
                << "*** ERROR: Stations defined by data locations, but data itself is missing "
                << endl;
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
            point1 = NFmiPoint(point0.X(), bottom - (y0 - top));
            point2 = itsScale.Scale(point1);
            NFmiString name("As");
            name += NFmiValueString(
                static_cast<int>(currentStationNumOnMap));  // uniikki nimi jokaiselle
            NFmiStationPoint station(
                NFmiStation(currentStationNumOnMap, name, lonlat.X(), lonlat.Y()), point2);
            station.SetAlternatingSizeFactor(alternatingSize);
            itsStations.AddLocation(station, false);
            if (firstStation)
            {
              itsName = station.GetName();
              firstUnscaledPoint = NFmiPoint(x, y);
              firstStation = false;
            }
            firstStation = false;
          }
        }
        else
          *itsLogFile << "*** ERROR: Map projection missing from grid" << endl;

        ReadNext();
        break;
      }

      case dStationsFromDataCropping:
      {
        float x1, y1, x2, y2;
        if (SetFour(x1, y1, x2, y2))
        {
          NFmiRect croppingRect(NFmiPoint(x1, y1), NFmiPoint(x2, y2));
          if (itsArea.GetArea())
          {
            if (!itsDataIter)
            {
              *itsLogFile << "*** ERROR: Data station locations used, but data itself is missing "
                          << endl;
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
              point1 = NFmiPoint(point0.X(), bottom - (y0 - top));
              point2 = itsScale.Scale(point1);
              if (!croppingRect.IsInside(point1)) continue;
              NFmiString name("As");
              name += NFmiValueString(
                  static_cast<int>(currentStationNumOnMap));  // uniikki nimi jokaiselle
              NFmiStationPoint station(
                  NFmiStation(currentStationNumOnMap, name, lonlat.X(), lonlat.Y()), point2);
              station.SetAlternatingSizeFactor(alternatingSize);
              itsStations.AddLocation(station, false);
              if (firstStation)
              {
                itsName = station.GetName();
                firstUnscaledPoint = NFmiPoint(x, y);
                firstStation = false;
              }
              firstStation = false;
            }
          }
          else
            *itsLogFile << "*** ERROR: Map projection missing from grid" << endl;
        }
        else
          *itsLogFile << "*** ERROR: Area missing from StationsFromDataCropping command"
                      << "" << endl;

        // ReadNext();
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
        fSupplementary = true;                     // t‰m‰ segmentti
        itsPressProduct->SetSupplementMode(true);  // muille segmenteille tiedoksi

        ReadNext();
        break;
      }
      case dDataCoordinatesMoved:
      {
        fDataCoordinatesMoved = true;

        ReadNext();
        break;
      }
      case dStationNamesAfterParams:
      {
        fStationNamesAfterParams = true;

        ReadNext();
        break;
      }
      case dDayChangeText:
      {
        itsDayChangeText = new NFmiPressText;
        itsDayChangeText->SetEnvironment(itsEnvironment);
        itsDayChangeText->SetHome(GetHome());
        itsDayChangeText->SetLogFile(itsLogFile);
        itsDayChangeText->SetDescriptionFile(itsDescriptionFile);
        itsDayChangeText->SetLanguage(itsLanguage);
        itsDayChangeText->Place(firstUnscaledPoint);
        // tarvitaankohan
        // point1 = itsCurrentStationScale.Scale(text->Place());
        // text->Place(point1);

        if (itsDayChangeText->ReadDescription(itsString))
        {
          // PITƒISI KAI OLLA VAIN OPTIO, SAMOIN MUILLA JA AIKALUUPILLA
          // point1 = itsCurrentStationScale.Scale(text->Place());
          // text->Place(point1);
        }
        else
          delete itsDayChangeText;

        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dOptimizeGlobalObs:
      {
        fOptimizeGlobalObs = true;
        fStationsAreLocalTime = true;
        itsEnvironment.UseBackupPreviousDay(true);

        ReadNext();
        break;
      }
      case dAlternatingSizeFactor:
      {
        SetOne(alternatingSize);
        break;
      }
      default:
      {
        ReadRemaining();
        break;
      }
    }
  }  // while

  CreateStationFromAreaMask();
  if (itsEnvironment.GetMaskNumber() > 0)  // t‰nne, oli vikana
    fIsAreaOperation = true;
  short numberOfRegularSteps =
      static_cast<unsigned short>(FmiMax(numberOfTimeSteps, numberOfLevelSteps));
  itsNumberOfSteps = numberOfRegularSteps;
  if (fIsLevelLoop)
    itsNumberOfSteps = currentLevelInd;
  else if (fIsTimeLoop)
    itsNumberOfSteps = currentTimeInd;

  if (itsStations.GetSize() < 1)
  // jos segmentti‰ k‰ytet‰‰n aika ja/tai nimip‰iv‰luuppina tarvitaan joku asema
  {
    NFmiStationPoint station(NFmiStation(1, "Helsinki", 25., 60.), NFmiPoint(100., 100.));
    itsStations.AddLocation(station, false);
    *itsLogFile << "   *** WARNING: #Segment created a dummy station since there are none in the "
                   "definition itself"
                << endl;
  }

  if (timeOrLevelTableSet)  // ei voi asettaa yll‰ p‰‰luupissa koska ekan aseman paikkaa ei
                            // v‰ltt‰m‰tt‰ tied‰
  {
    itsStations.Reset();
    itsStations.Next();
    NFmiStationPoint statPoint = *static_cast<const NFmiStationPoint *>(itsStations.Location());
    NFmiPoint point = statPoint.Point();

    if (isRelativeTable)
    {
      short factor = numberOfRegularSteps;  // nollakin sallitaan
      stepSize.Set(scaledTableMove.X(), scaledTableMove.Y());
      unscaledStepSize.Set(unscaledTableMove.X(), unscaledTableMove.Y());
      scaledTimeLast.Set(point.X() + stepSize.X() * factor, point.Y() + stepSize.Y() * factor);
      unscaledTimeLast.Set(firstUnscaledPoint.X() + unscaledStepSize.X() * factor,
                           firstUnscaledPoint.Y() + unscaledStepSize.Y() * factor);
    }
    else
    {
      short factor =
          (FmiMax(numberOfRegularSteps - 1, 1));  // 12.4.00 oli ..Time..; nollalla jaon esto
      stepSize.Set((scaledTimeLast.X() - point.X()) / factor,
                   (scaledTimeLast.Y() - point.Y()) / factor);
      unscaledStepSize.Set((unscaledTimeLast.X() - firstUnscaledPoint.X()) / factor,
                           (unscaledTimeLast.Y() - firstUnscaledPoint.Y()) / factor);
    }

    NFmiPoint previousNextScaled = scaledTimeLast;
    NFmiPoint previousNextUnscaled = unscaledTimeLast;
    if (numberOfRegularSteps <= 1)
    {
      previousNextUnscaled = firstUnscaledPoint;
      previousNextScaled = itsScale.Scale(firstUnscaledPoint);
      previousNextScaled = itsCurrentStationScale.Scale(previousNextScaled);
    }
    for (int ind = 1; ind <= itsNumberOfSteps && ind < kMaxNumOfTableElements;
         ind++)  // writePS:ss‰ indeksointi 1:st‰
    {
      // itsSteps on siirtym‰ seuraavaan
      if (ind == itsNumberOfSteps)
      {
        itsSteps[ind] = NFmiPoint(0., 0.);
        itsUnscaledSteps[ind] = NFmiPoint(0., 0.);
      }
      else
      {
        if (ind >= numberOfRegularSteps)
        {  // tableDirectPoints on skaalattu
          NFmiPoint nextScaledP = itsScale.Scale(tableDirectPoints[ind + 1]);
          itsSteps[ind] = nextScaledP - previousNextScaled;
          // HUOM ei-skaalattu on t‰m‰ unscaled:kin nyt vastoin nime‰‰n!!
          itsUnscaledSteps[ind] = itsSteps[ind];
          previousNextScaled = nextScaledP;
        }
        else  // s‰‰nnˆllinen taulukko (alussa, muttei vain lopussa)
        {
          itsSteps[ind] = stepSize;
          itsUnscaledSteps[ind] = unscaledStepSize;
          itsTimeSteps[ind] = itsHourStep;
        }
      }
    }
  }

  if (takeToMainArea)
  {
    if (itsArea.GetArea() == 0)
      *itsLogFile << "*** ERROR: Cannot use a missing map as the main map" << endl;
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

int NFmiPressParam::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar(object);
  lowChar.LowerCase();

  if (lowChar == NFmiString("#symbolgroup") || lowChar == NFmiString("#params") ||
      lowChar == NFmiString("#parameters") || lowChar == NFmiString("#dataalkiot") ||
      lowChar == NFmiString("#parametrit") || lowChar == NFmiString("#symboliryhm‰"))
    return dSymbolGroup;

  else if (lowChar == NFmiString("#fixedsymbol") || lowChar == NFmiString("#kuva") ||
           lowChar == NFmiString("#vakiosymboli") || lowChar == NFmiString("#vakiokuva"))
    return dImageObject;

  else if (lowChar == NFmiString("#text") || lowChar == NFmiString("#teksti") ||
           lowChar == NFmiString("#vakioteksti"))
    return dTextObject;

  else if (lowChar == NFmiString("#time") || lowChar == NFmiString("#timetext") ||
           lowChar == NFmiString("#aika") || lowChar == NFmiString("#aikateksti"))
    return dTimeTextObject;

  else if (lowChar == NFmiString("#stationname") || lowChar == NFmiString("#asemannimi") ||
           lowChar == NFmiString("#asemanimi"))
    return dStationTextObject;

  else if (lowChar == NFmiString("#stationnumber") || lowChar == NFmiString("#asemannumero"))
    return dStationNumberObject;

  else if (lowChar == NFmiString("datanotneeded") || lowChar == NFmiString("eidataa"))
    return dDataNotNeeded;

  else if (lowChar == NFmiString("logtext") || lowChar == NFmiString("lokiteksti"))
    return dLogText;

  else if (lowChar == NFmiString("logtextadd") || lowChar == NFmiString("lokitekstilis‰ys"))
    return dLogTextAdd;

  // seuraavat kaksi t‰‰ll‰ eik‰ PressTimeDescriptionissa, koska t‰m‰n kaikilla lapsilla ei
  // sallittua

  else if (lowChar == NFmiString("usedatastarttime") ||
           lowChar == NFmiString("k‰yt‰datanalkuaikaa"))
    return dDataTime;

  else if (lowChar == NFmiString("hoursfromdata") ||
           lowChar == NFmiString("relativehoursfromdata") ||
           lowChar == NFmiString("tuntejadatasta") ||
           lowChar == NFmiString("suhteellinentuntidatasta"))
    return dRelativeDataTime;

  else if (lowChar == NFmiString("datafile") || lowChar == NFmiString("data"))
    return dSegmentDataFile;

  else if (lowChar == NFmiString("primarydata") || lowChar == NFmiString("primarydatafile") ||
           lowChar == NFmiString("ensisijainendata"))
    return dSegmentPrimaryDataFile;

  else if (lowChar == NFmiString("grid") || lowChar == NFmiString("hila"))
  {
    return dGridMode;  // ei koodattu viel‰, Tarvitaanko???
  }
  else if (lowChar == NFmiString("usestationslocaltime") ||
           lowChar == NFmiString("k‰yt‰asemienpaikallistaaikaa"))
  {
    return dStationsLocalTime;
  }

  // datan syˆttˆ gridein‰ vaatii muutoksia: pit‰‰ voida erottaa milloin kaksi lukua
  // tarkoittavat tuotepaikkaa ja milloin lonLatia (varsinkin taulukoissa ep‰selv‰‰),
  // tehty uusi j‰sen fLonLatMode, jota muutetaan uusilla avainsanoilla xxxxLonLatxxxx.

  else if (lowChar == NFmiString("station") || lowChar == NFmiString("asema"))
  {
    fLonLatMode = false;
    return dStation;
  }
  else if (lowChar == NFmiString("backupstation") || lowChar == NFmiString("varaasema"))
  {
    return dBackupStation;
  }
  else if (lowChar == NFmiString("stationnewnaming") || lowChar == NFmiString("asemanime‰vaihtaen"))
  {
    fLonLatMode = false;
    return dStationNameReplace;
  }
  else if (lowChar == NFmiString("stationlonlat") || lowChar == NFmiString("asemalonlat"))
  {
    fLonLatMode = true;
    return dStation;
  }
  else if (lowChar == NFmiString("stationlonlatnewnaming") ||
           lowChar == NFmiString("asemalonlatnime‰vaihtaen"))
  {
    fLonLatMode = true;
    return dStationNameReplace;
  }
  else if (lowChar == NFmiString("initialscalingpoints") ||
           lowChar == NFmiString("stationdefarea") ||
           lowChar == NFmiString("alkuskaalauspisteet") ||
           lowChar == NFmiString("asemienmittaalue"))
    return dStationDefArea;

  else if (lowChar == NFmiString("stationmove") || lowChar == NFmiString("stationrelocation") ||
           lowChar == NFmiString("asemiensiirto"))
    return dStationRelocation;

  else if (lowChar == NFmiString("currentscalingpoints") ||
           lowChar == NFmiString("stationplotarea") ||
           lowChar == NFmiString("nykyskaalauspisteet") ||
           lowChar == NFmiString("asemiensijoitusalue"))
    return dStationPlotArea;

  else if (lowChar == NFmiString("paramsubviews") || lowChar == NFmiString("siirtolukum‰‰r‰") ||
           lowChar == NFmiString("aikoja"))
    return dParamSubViews;

  else if (lowChar == NFmiString("paramstepsize") || lowChar == NFmiString("siirrot"))
    return dParamStepSize;

  else if (lowChar == NFmiString("stationtable") || lowChar == NFmiString("asemataulukko"))
    return dStationTable;

  else if (lowChar == NFmiString("timetable") || lowChar == NFmiString("aikataulukko"))
    return dTimeTable;

  else if (lowChar == NFmiString("timeseries") || lowChar == NFmiString("timetablerelative") ||
           lowChar == NFmiString("aikasarja") ||
           lowChar == NFmiString("aikataulukkosuhteellisena") ||
           lowChar == NFmiString("aikataulukkosuht"))
    return dTimeTableRelative;

  else if (lowChar == NFmiString("leveltable") || lowChar == NFmiString("painepintataulukko"))
    return dLevelTable;

  else if (lowChar == NFmiString("level") || lowChar == NFmiString("painepinta"))
    return dSegmentLevel;

  else if (lowChar == NFmiString("hybridlevel") || lowChar == NFmiString("hybridipinta"))
    return dSegmentHybridLevel;

  else if (lowChar == NFmiString("levelplace") || lowChar == NFmiString("painepintapaikka"))
    return dLevelPlace;

  else if (lowChar == NFmiString("hybridlevelplace") || lowChar == NFmiString("hybridipintapaikka"))
    return dHybridLevelPlace;

  else if (lowChar == NFmiString("timeplace") || lowChar == NFmiString("aikapaikka"))
    return dTimePlace;

  else if (lowChar == NFmiString("timeplacemoving") || lowChar == NFmiString("timeplacerelative") ||
           lowChar == NFmiString("aikapaikkasuhteellisena") ||
           lowChar == NFmiString("aikapaikkasuht"))
    return dTimePlaceRelative;

  else if (lowChar == NFmiString("stationlonlatonmap") ||
           lowChar == NFmiString("asemalonlatillakartalle"))
    return dStationLonLatOnMap;

  else if (lowChar == NFmiString("stationnameonmap") ||
           lowChar == NFmiString("asemanimell‰kartalle"))
    return dStationNameOnMap;

  else if (lowChar == NFmiString("stationplaceonmap") || lowChar == NFmiString("placeonmap") ||
           lowChar == NFmiString("asemapaikallakartalle") || lowChar == NFmiString("paikka"))
    return dStationPlaceOnMap;

  else if (lowChar == NFmiString("stationplacematrixonmap") ||
           lowChar == NFmiString("paikkamatriisi"))
    return dStationPlaceMatrixOnMap;

  else if (lowChar == NFmiString("#map") || lowChar == NFmiString("#kartta"))
    return dSegmentMapArea;

  else if (lowChar == NFmiString("mainmap") || lowChar == NFmiString("t‰ss‰p‰‰kartta"))
    return dMainArea;

  else if (lowChar == NFmiString("coordinatesfrommainmap") ||
           lowChar == NFmiString("paikkakoordinaatitp‰‰kartasta"))
    return dCoordinatesFromMainMap;

  else if (lowChar == NFmiString("usegridasstationpoints") ||
           lowChar == NFmiString("stationsfromdata") ||
           lowChar == NFmiString("aseminadatanpisteet"))
    return dStationsFromData;

  else if (lowChar == NFmiString("usegridasstationpointscropping") ||
           lowChar == NFmiString("aseminadatanpisteetrajaten"))
    return dStationsFromDataCropping;

  else if (lowChar == NFmiString("areaoperation") || lowChar == NFmiString("alueoperaatio"))
    return dAreaOperation;

  else if (lowChar == NFmiString("distancecheck") || lowChar == NFmiString("l‰heisyysesto"))
    return dDistanceCheck;

  else if (lowChar == NFmiString("onlysupplement") || lowChar == NFmiString("vaint‰ydennys"))
    return dSupplement;

  else if (lowChar == NFmiString("#nameday") || lowChar == NFmiString("#nimip‰iv‰"))
    return dSegmentNameDay;

  else if (lowChar == NFmiString("datacoordinatesmoved") ||
           lowChar == NFmiString("datankoordinaatitsiirretty"))
    return dDataCoordinatesMoved;

  else if (lowChar == NFmiString("stationnamesafterparams") ||
           lowChar == NFmiString("asemannimetparametrienj‰lkeen") ||
           lowChar == NFmiString("asemanimetparametrienj‰lkeen"))
    return dStationNamesAfterParams;

  else if (lowChar == NFmiString("#daychangetext") || lowChar == NFmiString("#p‰iv‰nvaihtoteksti"))
    return dDayChangeText;

  else if (lowChar == NFmiString("optimizeglobalobs") ||
           lowChar == NFmiString("optimoimaailmahavainnot"))
    return dOptimizeGlobalObs;

  else if (lowChar == NFmiString("relativesymbolsize") ||
           lowChar == NFmiString("suhteellinensymbolikoko") ||
           lowChar == NFmiString("suhtsymbolikoko"))
    return dAlternatingSizeFactor;

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
  if (itsAreaMask) delete itsAreaMask;

  if (itsMaskIter && itsEnvironment.GetMaskNumber() > 0)
  {
    itsMaskIter->First();
    unsigned long maskNr = itsEnvironment.GetMaskNumber();
    NFmiCalculationCondition maskOperation(kFmiMaskEqual, maskNr);

    // maski on siis info-tyyppinen (ei esim. bitmap tai lennossa
    // laskettava kuten auringon-kulma-maski)
    NFmiAreaMask::Type maskType = NFmiAreaMask::kInfo;

    NFmiInfoData::Type dataType = NFmiInfoData::kStationary;
    NFmiAreaMask::BinaryOperator postBinaryOperator = NFmiAreaMask::kAnd;

    //#ifndef UNIX
    //	  itsAreaMask = 0;
    //	  *itsLogFile << "  AREAMASK ON WINDOWS = 0, search Boost " << endl;
    //#else
    itsAreaMask =
        new NFmiInfoAreaMask(maskOperation, maskType, dataType, itsMaskIter, postBinaryOperator);
    *itsLogFile << "  areamask created: " << maskNr << endl;
    //#endif
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
  // tarvitaan nimi vaikka tekstioutputiin
  if (itsMaskIter)
  {
    NFmiEnumConverter converter(itsEnvironment.GetEnumSpace());
    string name = converter.ToString(itsEnvironment.GetMaskNumber());
    itsName = NFmiString(name);
    NFmiStationPoint station(NFmiStation(100, itsName, 25, 60), NFmiPoint(100., 100.));
    // if(itsStations.GetSize() < 1)  // pit‰isi tuhota entinen ettei kumuloidu; p‰‰asia ett‰ nimi
    // ok
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
                             ofstream &theFile,
                             FmiPressOutputMode theOutput)
{
  UnsetAllErrorReported();

  if (fSupplementary) *itsLogFile << "  SUPPLEMENT ONLY" << endl;

  if (!itsDataIter)
  {
    *itsLogFile << "  *** ERROR: data lost: " << static_cast<char *>(itsDataName) << endl;
    return true;  // false lopettaa kaikki segmentit
  }
  itsDataIter->First();  // ainakin level pit‰‰ asettaa nollaksi eik‰ -1:ksi

  if (itsPrimaryDataIter)
  {
    itsPrimaryDataIter->First();
  }

  CreateAreaMask();

  NFmiPoint stationPoint;
  NFmiPoint stationPointMovement;

  if (itsLogFile)
  {
    if (itsDataName.IsValue())
      *itsLogFile << "  data: " << static_cast<char *>(itsDataName) << endl;
    else
      *itsLogFile << "  data: first one defined in the product" << endl;
  }

  if (itsPrimaryDataName.IsValue())
    *itsLogFile << "  primary data: " << static_cast<char *>(itsPrimaryDataName) << endl;

  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling *object;

  // jotta voidaan tehd‰ aika vasta asemannimen j‰lkeen (tiedostomuoto)
  NFmiPressScaling *saveObject;
  saveObject = 0;

  NFmiVoidPtrIterator stationObjectIter(itsStationDepObjects);

  itsSymbols.Set(theScale, itsDataIter, theFile);

  NFmiMetTime time = itsFirstPlotTime;

  itsCurrentStep = 1;
  stationPointMovement.Set(0., 0.);
  itsCurrentStationIndex = 0;
  FmiCounter statAll = 0;
  bool done, supplementLater;
  fPrimaryDataOk = true;
  bool primaryDataTimeOk = true;

  /********* AIKA/PAINEPINTA-luuppi ********/
  while (itsCurrentStep <= itsNumberOfSteps)
  {
    FmiCounter currentStepInd =
        FmiMin(static_cast<int>(itsCurrentStep), kMaxNumOfTableElements - 1);
    fMaxMinSearched = false;
    itsPrimaryDataNum = 0;

    if (!fSupplementary) itsPressProduct->SetSegmentTimeStatus(itsCurrentStep, false);

    if (!itsDataIter)
    {
      if (itsLogFile) *itsLogFile << "  *** ERROR: Data missing" << endl;
    }
    else
    {
      if (fIsLevelLoop)
      {
        bool ok = itsDataIter->Level(itsLevels[currentStepInd]);
        if (itsPrimaryDataIter) ok = itsPrimaryDataIter->Level(itsLevels[currentStepInd]);
        if (!ok)
          *itsLogFile << "*** ERROR: missing level= "
                      << static_cast<unsigned long>(itsLevels[currentStepInd].LevelValue()) << endl;
      }

      done = fSupplementary && itsPressProduct->GetSegmentTimeStatus(itsCurrentStep);
      supplementLater = itsPressProduct->GetSupplementMode() && !fSupplementary;

      // HUOM segmentin aika pit‰‰ olla datassa vaikka piirtoalkiossa muutettaisiin tuntia
      // ei prosessoida jos t‰ydennyssegmentti ja on jo tehty
      NFmiMetTime actTime(time);
      if (fYearData) actTime.SetYear(itsDataIter->Time().GetYear());
      bool timeFound = itsDataIter->Time(actTime);
      if (itsPrimaryDataIter)
      {
        if (itsPrimaryDataIter->Time(actTime))
          primaryDataTimeOk = true;
        else
          primaryDataTimeOk = false;
      }

      // if((itsDataIter->Time(actTime) || fStationNotNeeded) && !done)
      if ((timeFound || fStationNotNeeded) && !done)
      {
        if (itsCurrentStep == 1 || fIsTimeLoop)
        {
          bool writeLog = true;
          if (itsNumberOfSteps > 6)  // lyhennet‰‰n lokiskriptia
          {
            if (itsCurrentStep == 1)
            {
              *itsLogFile << "  first time for the segment: "
                          << static_cast<char *>(time.ToStr("DD.MM.YYYY HH"));
            }
            else if (itsCurrentStep == itsNumberOfSteps)
            {
              *itsLogFile << "  last time for the segment: "
                          << static_cast<char *>(time.ToStr("DD.MM.YYYY HH"));
            }
            else
              writeLog = false;
          }
          else
          {
            *itsLogFile << "  Segment time: " << static_cast<char *>(time.ToStr("DD.MM.YYYY HH"));
          }
          if (IsStationLocalTimeMode() && writeLog)
            *itsLogFile << " local time" << endl;
          else if (writeLog)
            *itsLogFile << " utc" << endl;
        }
        if (!fSupplementary) itsPressProduct->SetSegmentTimeStatus(itsCurrentStep, true);

        if (fIsLevelLoop)
        {
          if (itsLevels[currentStepInd].LevelType() == kFmiPressureLevel)
            *itsLogFile << "  Segment pressure level: " << itsLevels[currentStepInd].LevelValue()
                        << endl;
          else
            *itsLogFile << "  Segment hybrid level: " << itsLevels[currentStepInd].LevelValue()
                        << endl;
        }

        // ************** AikaSidotutObjektit ensin (paitsi ne jotka pyydetty lopussa) ***********
        // eli ***VakioSymboli
        //     ***VakioTeksti
        //     ***AikaTeksti
        //     ***Nimip‰iv‰t

        if (timeFound) DoTimeDependent(false, currentStepInd, theFile, theOutput, saveObject);
        // ********** alun AikaSidotutObjektit loppu **************

        itsStations.Reset();
        fIsFirstStation = true;

        // ********** ASEMAluuppi *****************
        itsCurrentStationIndex = 0;
        while (itsStations.Next())
        {
          fBackupDayReported = false;
          fDayChanged = false;
          statAll++;
          itsCurrentStationIndex++;

          stationPoint =
              NFmiStationPoint(*static_cast<const NFmiStationPoint *>(itsStations.Location()))
                  .Point();
          itsCurrentUnscaledPoint = stationPoint;
          // NFmiStationPoint statPoint = *static_cast<const NFmiStationPoint
          // *>(itsStations.Location());
          if (itsCurrentStationPoint) delete itsCurrentStationPoint;
          NFmiStationPoint *itsCurrentStationPoint =
              new NFmiStationPoint(*static_cast<const NFmiStationPoint *>(itsStations.Location()));

          fCurrentStationBackup = itsCurrentStationPoint->IsBackup() ? true : false;
          NFmiPoint lonLat = itsCurrentStationPoint->GetLocation();

          if (lonLat.X() == kFloatMissing)
          {
            if (itsDataIter->IsGrid())
            {
              if (!SetLonLat(*itsCurrentStationPoint))
                continue;  // ->seuraava asema jos ei taulukossa
            }
            else
            {
              if (itsDataIter->Location(NFmiStation(*itsCurrentStationPoint->Station()).GetName()))
              {
                itsCurrentStationPoint->SetLongitude(itsDataIter->Location()->GetLongitude());
                itsCurrentStationPoint->SetLatitude(itsDataIter->Location()->GetLatitude());
              }
              // t‰h‰n testi onko identti, jollei  yritet‰‰n luoda asemalonlatuusi:sta
              //
              else if (itsDataIter->Location(
                           NFmiStation(*itsCurrentStationPoint->Station()).GetIdent()))
              {
                itsCurrentStationPoint->SetLongitude(itsDataIter->Location()->GetLongitude());
                itsCurrentStationPoint->SetLatitude(itsDataIter->Location()->GetLatitude());
              }
            }
          }

          if (itsPrimaryDataIter && primaryDataTimeOk)
          {
            if (!itsPrimaryDataIter->Location(
                    NFmiStation(*itsCurrentStationPoint->Station()).GetName()))
              fPrimaryDataOk = false;
            else
              fPrimaryDataOk = true;
          }
          else
            fPrimaryDataOk = false;

          itsCurrentStation = NFmiStation(*itsCurrentStationPoint->Station());
          if (FindQDStationName(*itsCurrentStationPoint) || fStationNotNeeded)
          {
            if (itsCurrentStep == 1 && !fCurrentStationBackup && !IsStationNamesAfterParams())
            {
              // ********* AsemaSidotutObjektit ennen parametrej‰ *************
              // *** eli AsemanNimi (vainko?)

              stationObjectIter.Reset();
              object = static_cast<NFmiPressScaling *>(stationObjectIter.Next());

              NFmiPoint unScaledPoint = itsScale.UnScale(stationPoint);
              while (object)
              {
                NFmiPoint savePlace = object->Place();
                object->Place(unScaledPoint + object->GetMovePlace());
                object->Set(itsScale, theFile);
                object->SetRotatingPoint(object->Place());
                if (!(object->WritePS(theOutput)))
                {
                  if (itsLogFile)
                    *itsLogFile << "*** ERROR: (statDep)object->WritePS() in NFmiPressParam"
                                << endl;
                  return false;
                }
                if (theOutput == kPostScript) object->WriteGRestore();
                object->Place(savePlace);  // jotta toimisi seuraavalle writePs-k‰skylle

                object = static_cast<NFmiPressScaling *>(stationObjectIter.Next());
              }
            }
            // ************ AsemaSidotutObjektit loppu ************

            if (theOutput == kPlainText && saveObject)
              saveObject->WritePSUpdatingSubText(theOutput);

            itsCurrentStationPoint->Point(itsCurrentStationPoint->Point() + stationPointMovement);

            // ***************************************
            // ****** itse symbolit/numerot jne ******
            // ***************************************

            fInterruptSymbolGroup = false;  // l‰heisyystesti-optiota varten
            if (!(itsSymbols.WritePS(itsCurrentStationPoint, theOutput)))
            {
              if (itsLogFile) *itsLogFile << "*** ERROR: Failed to draw station: " << endl;
              return false;
            }

            // ********* P‰iv‰nVaihtoTeksti

            if (itsDayChangeText && fDayChanged)
            {
              NFmiPoint unScaledPoint = itsScale.UnScale(stationPoint);
              // NFmiPoint savePlace = itsDayChangeText->Place(); //tarvitaanko t‰h‰n, otin vaan
              // asematekstist‰
              itsDayChangeText->Place(unScaledPoint + itsDayChangeText->GetMovePlace());
              itsDayChangeText->Set(itsScale, theFile);
              itsDayChangeText->SetRotatingPoint(itsDayChangeText->Place());

              if (!(itsDayChangeText->WritePS(theOutput)))
              {
                *itsLogFile << "*** ERROR: DayChangeText->WritePS() in NFmiPressParam" << endl;
                return false;
              }

              if (theOutput == kPostScript) itsDayChangeText->WriteGRestore();
              // itsDayChangeText->Place(savePlace); // jotta toimisi seuraavalle writePs-k‰skylle
            }

            if (itsCurrentStep == 1 && !fCurrentStationBackup && IsStationNamesAfterParams())
            {
              // ********* AsemaSidotutObjektit parametrien j‰lkeen *************
              // ********* eli ilmeisesti vain AsemanNimi

              // AsemanNimi
              stationObjectIter.Reset();
              object = static_cast<NFmiPressScaling *>(stationObjectIter.Next());

              NFmiPoint unScaledPoint = itsScale.UnScale(stationPoint);
              while (object)
              {
                NFmiPoint savePlace = object->Place();
                object->Place(unScaledPoint + object->GetMovePlace());
                object->Set(itsScale, theFile);
                object->SetRotatingPoint(object->Place());
                if (!(object->WritePS(theOutput)))
                {
                  if (itsLogFile)
                    *itsLogFile << "*** ERROR: (statDep)object->WritePS() in NFmiPressParam"
                                << endl;
                  return false;
                }
                if (theOutput == kPostScript) object->WriteGRestore();
                object->Place(savePlace);  // jotta toimisi seuraavalle writePs-k‰skylle

                object = static_cast<NFmiPressScaling *>(stationObjectIter.Next());
              }
            }
          }  // if(FindQStationName()
          else
          {
            NFmiString statName = itsCurrentStationPoint->Station()->GetName();
            if (!(statName == NFmiString("Tyhj‰") || statName == NFmiString("None")))
            {
              string msg = static_cast<char *>(statName);
              errors.push_back(msg);
              if (itsLogFile) *itsLogFile << "  ***ERROR: " << msg << endl;
            }
          }
          fIsFirstStation = false;

        }  // while(itsStations.Next())

        // ************** Lopun AikaSidotutObjektit, jotka m‰‰ritelty loppuun ******************
        // eli ***VakioSymboli
        //     ***VakioTeksti
        //     ***AikaTeksti
        //     ***Nimip‰iv‰t
        DoTimeDependent(true, currentStepInd, theFile, theOutput, saveObject);

        // ********** Lopun AikaSidotutObjektit loppu **************
      }  // if(itsData.Time())
      else
      {  // HUOM QD:ss‰ pit‰‰ olla segmentin aika vaikka data-alkioissa k‰ytett‰issin eri tuntia
        NFmiString firstText;
        string msg;
        if (done)
          firstText = "  time already done: ";
        else if (supplementLater)
          firstText = "  Segment time missing from this data: ";
        else
        {
          firstText = "  *** ERROR: Segment time missing from data: ";
          msg = string("Segment time missing from data: ") +
                static_cast<char *>(time.ToStr("DD.MM.YYYY HH")) + string(" utc");
          // errors.push_back(msg);
        }
        *itsLogFile << static_cast<char *>(firstText)
                    << static_cast<char *>(time.ToStr("DD.MM.YYYY HH")) << " utc" << endl;

        // aikariippuvat siirrett‰v‰ eteenp‰in vaikkei ole dataa 20.3
        objectIter.Reset();
        object = static_cast<NFmiPressScaling *>(objectIter.Next());
        while (object)
        {
          object->Move(itsSteps[currentStepInd]);
          object = static_cast<NFmiPressScaling *>(objectIter.Next());
        }
      }

    }  // if(itsDataIter)

    stationPointMovement += itsSteps[currentStepInd];
    if (fIsLevelLoop)
    {
    }
    else
    {
      long hourStep;
      if (fIsPureRegTimeLoop) hourStep = itsHourStep;
      //		  time.ChangeByHours(itsHourStep);
      else
        hourStep = itsTimeSteps[currentStepInd];
      // time.ChangeByHours(itsTimeSteps[currentStepInd]);

      time.ChangeByHours(hourStep);
      StepTimeDependent(hourStep);
    }
    if (itsPrimaryDataNum > 0)
    {
      *itsLogFile << "  Numbers of Primary data used in this time step = " << itsPrimaryDataNum
                  << endl;
    }

    itsCurrentStep++;

  }  // while(time/level Step)

  if (itsLogFile)
  {
    if (fReportProseccedNumber)
      *itsLogFile << "  " << statAll << " stations*times (or levels) processed" << endl;
    else
      *itsLogFile << "  stations processed" << endl;
  }

  long num = itsSymbols.NumOfMissing();
  if (num > 0 && itsLogFile)
  {
    string msg;
    NFmiValueString valString(num);
    // NFmiMetTime now;

    if (GetFirstDeltaDays() <= 0)  // miksi oli <= 1
    //	GetFirstDeltaDays() == 0 && now.GetHour() >= GetFirstPlotHours())
    {
      msg = static_cast<char *>(valString) + string(" missing observation data");
      *itsLogFile << "  WARNING: " << msg << endl;
    }
    else  // vakavampaa
    {
      msg = static_cast<char *>(valString) + string(" missing forecast data");
      *itsLogFile << "  ***WARNING: " << msg << endl;
      // errors.push_back(msg);
    }
  }
  itsSymbols.InitMissing();  // jotta ei manageri-k‰ytˆss‰ kertyisi

  for (int i = 1; i < itsNumberOfSteps; i++)
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
  NFmiPressScaling *object;

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
                                     ofstream &theFile,
                                     FmiPressOutputMode theOutput,
                                     NFmiPressScaling *&theSaveObject)
{
  NFmiVoidPtrIterator objectIter(itsTimeDepObjects);
  NFmiPressScaling *object;

  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while (object)
  {
    if (object->IsToWriteLast() == isLastLoop)
    {
      if (object->ActiveTimeIndex(GetCurrentStep()))
      {
        object->Set(itsScale, theFile);
        if (itsCurrentStep == 1 || !object->GetPrintOnce())
        {
          object->SetRotatingPoint(object->Place());
          if (theOutput != kPlainText)
          {
            if (!(object->WritePSUpdatingSubText(theOutput)))
            {
              if (itsLogFile)
                *itsLogFile << "*** ERROR: (timeDep)object->WritePS() in NFmiPressParam" << endl;
              return false;
            }
            //	if(theOutput == kPostScript)
            object->WriteGRestore();
          }
          else
            // ei kyll‰ k‰ytet‰ en‰‰ jos isLastLoop
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
  if (fTwoPartLogComment)
  {
    res = itsName;
    res += NFmiString("; ");
    res += itsLogComment;
  }
  else if (itsLogComment.IsValue())
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

bool NFmiPressParam::FindQDStationName(const NFmiStationPoint &theStation)
{
  if (!itsDataIter) return false;

  NFmiString dataName;
  NFmiString plotName = theStation.GetName();

  if (!itsDataIter->IsGrid())  // perinteinen asemannimell‰ haku
  {
    NFmiStation station;
    itsDataIter->ResetLocation();
    unsigned long dataWmo;
    unsigned long plotWmo = 0;
    while (itsDataIter->NextLocation())
    {
      dataName = itsDataIter->Location()->GetName();
      if (dataName.IsValue())
      {
        if (plotName == dataName) return true;
      }
      else
      {
        dataWmo = itsDataIter->Location()->GetIdent();
        if (plotWmo <= 0)
        {
          plotWmo = theStation.GetIdent();
          if (plotWmo < 1000) plotWmo = itsPressProduct->FindWmoFromList(plotName);
          if (plotWmo == 0) return false;
        }

        if (dataWmo > 0 && plotWmo > 0 && dataWmo == plotWmo) return true;
      }
    }
  }
  else
  {
    // gridej‰ varten joutuu ottamaan pois t‰‰lt‰ ja sijoittamaan
    // piirto-olioon, jossa lat/longilla haetaan interpoloiden tai l‰hint‰ pistett‰,
    // t‰‰lt‰ onnistuu vain l‰hin piste

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

bool NFmiPressParam::SetLonLat(NFmiStationPoint &theStation)
{
  if (!itsDataIter)
  {
    return false;
  }
  // ensin tarkistetaan jos on pistedata niin otetaan sielta aseman Lon/lat
  // paitsi jos asemat ei oikeilla paikoillaan
  if (!itsDataIter->IsGrid() && !fDataCoordinatesMoved)
  {
    if (itsDataIter->Location(NFmiStation(*theStation.Station()).GetName()))
    {
      theStation.SetLongitude(itsDataIter->Location()->GetLongitude());
      theStation.SetLatitude(itsDataIter->Location()->GetLatitude());
      return true;
    }
    else
    {
      *itsLogFile << "*** ERROR: Could not extract lat/lon from data" << endl;
      return false;
    }
  }

  // muuten asema/LonLat-tiedostosta

  NFmiPoint lonLat;
  NFmiString statName(theStation.GetName());
  if (itsPressProduct->FindLonLatFromList(statName, lonLat))
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

void NFmiPressParam::SetStationScale(const NFmiRectScale &scale) { itsCurrentStationScale = scale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param scale Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressParam::SetScale(const NFmiRectScale &scale) { itsScale = scale; }
// ======================================================================
