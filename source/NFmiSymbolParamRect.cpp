// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiSymbolParamRect.cpp
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k��nt�j�n varoitusta
#endif

#include "NFmiSymbolParamRect.h"
#include "NFmiFileSystem.h"
#include "NFmiPressParam.h"
#include "NFmiPressProduct.h"
#include "NFmiPsWriting.h"
#include "NFmiRectScale.h"
#include "NFmiSettings.h"
#include <iostream>
#include <list>
#include <string>

extern std::list<std::string> errors;

// Boost
#include <boost/filesystem/operations.hpp>
using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal data
 * \todo Remove the unnecessary ifs
 */
// ----------------------------------------------------------------------

NFmiSymbolParamRect::~NFmiSymbolParamRect(void)
{
  if (itsSubDir)
    delete static_cast<NFmiString *>(itsSubDir);
  if (itsMapping)
    delete static_cast<NFmiParamMapping *>(itsMapping);
  if (itsOrigDir)
    delete static_cast<NFmiString *>(itsOrigDir);
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theSymbolRect The object to copy
 */
// ----------------------------------------------------------------------

NFmiSymbolParamRect::NFmiSymbolParamRect(const NFmiSymbolParamRect &theSymbolRect)
    : NFmiParamRect(theSymbolRect),
      itsConstSymbol(theSymbolRect.itsConstSymbol),
      fIsConstSymbol(theSymbolRect.fIsConstSymbol),
      itsMapping(new NFmiParamMapping(*theSymbolRect.itsMapping)),
      itsSubDir(new NFmiString(*theSymbolRect.itsSubDir)),
      itsOrigDir(new NFmiString(*theSymbolRect.itsOrigDir)),
      itsSymbolSetName(theSymbolRect.itsSymbolSetName),
      itsDefToProductScale(theSymbolRect.itsDefToProductScale),
      fUseDayNightSymbols(theSymbolRect.fUseDayNightSymbols)

{
}

// ----------------------------------------------------------------------
/*!
 * Make a new copy of this
 *
 * \return The copy
 * \todo Should return an auto_ptr
 * \todo Remove the unnecessary static_cast
 */
// ----------------------------------------------------------------------

NFmiParamRect *NFmiSymbolParamRect::Clone(void) const
{
  return static_cast<NFmiParamRect *>(new NFmiSymbolParamRect(*this));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ReadDescription(NFmiString &retString)
{
  double x, y;
  NFmiValueString valueString;
  if (itsOrigDir)
    delete itsOrigDir;
  NFmiString origDir;
  NFmiString inDir;

  inDir = GetHome();
  inDir += kFmiDirectorySeparator;
  inDir += "LyhytSymbolit";
#ifdef UNIX
  // Create the symbol cache directory if it doesn't exist already
  inDir = NFmiSettings::Require<string>("press::symbolcachepath");
  try
  {
    std::filesystem::create_directory(static_cast<string>(inDir));
  }
  catch (exception &e)
  {
    cout << "Creating the symbol cache directory failed." << endl;
  }
#endif

  inDir += kFmiDirectorySeparator;

#ifndef UNIX
  origDir = GetHome();
  origDir += kFmiDirectorySeparator;
  origDir += "Symbolit";
  origDir += kFmiDirectorySeparator;
#else
  origDir = NFmiSettings::Require<string>("press::path");
  origDir += kFmiDirectorySeparator;
  origDir += "symbols";
  origDir += kFmiDirectorySeparator;
#endif

  // NFmiString subDir = NFmiString("Kymi");  //oletus vaikka t�m�
  NFmiString subDir = itsEnvironment.GetSymbolSet();

  itsOrigDir = new NFmiString(origDir);  // oikeastaan koko polku

  NFmiString helpString;
  NFmiString *mappingName;
  NFmiValueString helpValueString;
  itsMapping = new NFmiParamMapping;
  itsModifier = kNoneModifier;
  SetPreReadingTimes();

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);

  long long1, long2;
  double r1, r2, r3, r4;
  double intervalEps = 0.00001;
  NFmiPoint sizePoint1, sizePoint2;
  sizePoint1.Set(0., 0.);
  sizePoint2.Set(c40, c40);  // ksymbolgrupin oletus on my�s t�m� -> skaala 1 : 1

  itsRelRect.Set(NFmiPoint(0., 0.), NFmiPoint(1., 1.));
  itsSizeFactor = NFmiPoint(1., 1.);

  // onko asetettu, ent�s rotparr jossa oli sijoitusvirhe toimiiko vanha sijoitus
  if (fNewScaling)
    itsRelRect += NFmiPoint(-.5, -.5);

  itsMultiMapping = 0;
  bool symbDirGiven = false;
  // bool symbSizeGiven = false;
  double xh, yh;
  xh = yh = 1.;

  while (itsIntObject != 9999 || itsCommentLevel)
  {
    if (itsIntObject != dEndComment && itsCommentLevel)
      itsIntObject = dComment;
    if (itsLoopNum > itsMaxLoopNum)
    {
      if (itsLogFile)
        *itsLogFile << "*** ERROR: max output file length exceeded in #Symbol" << endl;
      retString = itsString;
      return false;
    }
    itsLoopNum++;
    switch (itsIntObject)
    {
      case dOther:  // tuntematon sana
      {
        if (itsLogFile)
          *itsLogFile << "*** ERROR: Unknown string in #Symbol: " << static_cast<char *>(itsObject)
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

      case dSymbolDirectory:
      {
        if (!ReadEqualChar())
          break;

        subDir = ReadString();
        NFmiString apu = subDir;
        apu.LowerCase();
        if (apu == "kuunvaiheet"  // pit�isi hoitaa komennolla
            || apu == "moon_phases")
          SetMoonPhase();

        symbDirGiven = true;

        ReadNext();
        break;
      }
      case dConstSymbolName:
      {
        if (!ReadEqualChar())
          break;
        itsConstSymbol = ReadString();

        ReadNext();
        break;
      }
      case dMapping:
      {
        NFmiMappingInterval interval;
        if (!ReadEqualChar())
          break;

        if (ReadDouble(r1))
        {
          *itsDescriptionFile >> itsObject;
          helpValueString = NFmiValueString(itsObject);
          if (helpValueString.IsNumeric())
          {
            r2 = static_cast<double>(helpValueString);
            *itsDescriptionFile >> itsObject;
          }
          else
          {
            r2 = r1;
          }

          interval.itsBottomValue = r1 - intervalEps;  // varmuuden vuoksi jotta vertailut oiken
                                                       // p�in
          interval.itsTopValue = r2 + intervalEps;

          mappingName = new NFmiString(itsObject);
          interval.itsSymbol = *mappingName;
          interval.fIsScaled = false;
          itsMapping->AddMappingInterval(interval);
        }

        ReadNext();
        break;
      }
      case dScaleMapping:
      {
        NFmiMappingInterval interval;
        if (!ReadEqualChar())
          break;

        if (ReadTwo(long1, long2))
        {
          interval.itsBottomValue = long1;
          interval.itsTopValue = long2;

          mappingName = new NFmiString(itsObject);
          interval.itsSymbol = *mappingName;
          interval.fIsScaled = true;
          itsMapping->AddMappingInterval(interval);
        }

        ReadNext();
        break;
      }
      case dRelSize:
      {
        if (!ReadEqualChar())
          break;

        if (ReadOne(x))
        {
          // symbSizeGiven = true;
          // xh = yh = 1.;
          if (x > 0.)
            xh = x;

          *itsDescriptionFile >> itsObject;
          valueString = itsObject;
          if (valueString.IsNumeric())
          {
            y = static_cast<double>(valueString);
            if (y > 0)
              yh = y;
            *itsDescriptionFile >> itsObject;
            itsString = itsObject;
          }
          else
          {
            yh = xh;
            itsString = valueString;
          }
          // itsRelRect.Inflate((xh-1.)/2., (yh-1)/2.);
          // itsRelRect += NFmiPoint(-(2-xh)/2., -(2-yh)/2.);
          // itsRelRect += NFmiPoint(.5, .5);

          // itsSizeFactor = NFmiPoint(xh,yh);
        }
        else
        {
          *itsDescriptionFile >> itsObject;
          itsString = itsObject;
        }

        itsIntObject = ConvertDefText(itsString);
        break;
      }
      case dPlaceMove:
      {
        if (!ReadEqualChar())
          break;

        if (ReadTwo(r1, r2))
        {
          itsRelRect += NFmiPoint(r1 / c40, r2 / c40);
        }

        ReadNext();
        break;
      }
      case dRelPlace:
      {
        if (!ReadEqualChar())
          break;

        if (Read4Double(r1, r2, r3, r4))
        {
          itsRelRect.Set(NFmiPoint(r1, r2), NFmiPoint(r3, r4));
        }

        ReadNext();

        break;
      }
      case dProducer:
      {
        SetOne(long1);  // luetaan vain pois
        break;
      }
      case dRelDay:
      {
        SetOne(itsFirstDeltaDays);
        if (itsLogFile)
          *itsLogFile << "*** ERROR: The day cannot be set in #Symbol" << endl;
        break;
      }
      case dHour:
      {
        SetOne(itsFirstPlotHours);
        break;
      }
      case dIncludeNightSymbols:
      {
        UseDayNightSymbols();
        ReadNext();
        break;
      }
      default:
      {
        ReadRemaining();
        break;
      }
    }
  }  // end while *************************************

  if (itsPressParam->IsOptimizeGlobalObs())
  {
    if (itsIdentPar == 4 && !itsMultiMapping)
    {
      if (fModifierUsed)
      {
        *itsLogFile
            << "*** WARNING: sum after other similar command (#Symbol, OptimizeWorldObservations)"
            << endl;
      }
      SetRelModifierTimes(10, 18);
      fModifierUsed = true;
      itsModifier = kMaximum;
      fAllowMissing = true;
    }
    else
      fUseBackupTime = true;
  }

  // jos symbolisetti annettu t�ss�, t�ss� annettu kokokerroin sellaisenaan,
  //   muuten kerrotaan p��tason kertoimella
  //    if(symbSizeGiven)
  //	{
  if (!symbDirGiven)
  {
    double sizeF = static_cast<double>(itsEnvironment.GetSymbolSizeFactor());
    xh = sizeF * xh;
    yh = sizeF * yh;
  }
  itsRelRect.Inflate((xh - 1.) / 2., (yh - 1) / 2.);
  itsRelRect += NFmiPoint(-(2 - xh) / 2., -(2 - yh) / 2.);
  itsRelRect += NFmiPoint(.5, .5);
  itsSizeFactor = NFmiPoint(xh, yh);
  //	}

  if (itsSubDir)
    delete itsSubDir;
  itsSubDir = new NFmiString(inDir);  // oikeastaan koko polku
  itsSymbolSetName = subDir;

  // SYMBOLIKOKO ON NYT OMASSA TIEDOSTOSSAAN JOKAISTA SETTI� VARTEN
  // MUTTA NYT JO VANHAA TEKNIIKKAA (kes� 98)
  // ei anneta en�� m��rittelyiss�

  NFmiString sizeFile = *itsOrigDir;
  sizeFile += itsSymbolSetName;
  sizeFile += NFmiString("/");

  if (!NFmiFileSystem::DirectoryExists(sizeFile.CharPtr()))
  {
    string msg = string("Symbolihakemisto ") + sizeFile.CharPtr() + " puuttuu!";
    errors.push_back(msg);
    if (itsLogFile)
      *itsLogFile << "*** Error: " << msg << endl;
  }

  sizeFile += NFmiString("symbolikoko.txt");
  ifstream input(sizeFile, ios::in);
  /*
    if(input.good() && !input.eof())
      {
            if(Read4Double(r1,r2,r3,r4))
                  {
                    if(r1 == r3) // yksinkertaistettu yhteen lukuun, PIT�ISI TEHD� PAREMMIN
                          {
                            sizePoint1.Set(0., 0.);
                            sizePoint2.Set(r1,r1);
                          }
                    else
                          {
                            sizePoint1.Set(r1,r2);
                            sizePoint2.Set(r3,r4);
                          }

                  }
            else
                  {
                    if(itsLogFile)
                          *itsLogFile << "*** ERROR: Ongelmia symbolikoko-tiedostossa"  << endl;
                  }
            fNewScaling = false;
          }
    else
          {
          */
  fNewScaling = true;
  //}

  itsDefToProductScale.SetStartScales(NFmiRect(sizePoint1, sizePoint2));

  input.close();

  SetPostReadingTimes();

  SetIdent(
      NFmiDataIdent(NFmiParam(itsIdentPar), NFmiProducer(240)));  // joku NFmiProducer(itsProducer)

  DoPostReading();

  if (fNewScaling)  // t�m� DoPostReading:n j�lkeen
    itsRelRect += NFmiPoint(1., 1.);

  SetRect(NFmiRect(itsRelRect));  // vain Rect t�h�n

  retString = itsString;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 * \todo Check the implementation is complete, it seems strange
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ReadRemaining(void)
{
  return NFmiParamRect::ReadRemaining();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiSymbolParamRect::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if (lowChar == NFmiString("symboldirectory") || lowChar == NFmiString("symbolset") ||
      lowChar == NFmiString("symbolitaulu") || lowChar == NFmiString("kuvakansio"))
    return dSymbolDirectory;

  else if (lowChar == NFmiString("scale") || lowChar == NFmiString("sizefactor") ||
           lowChar == NFmiString("kokokerroin"))
    return dRelSize;

  else if (lowChar == NFmiString("mapping") || lowChar == NFmiString("arvoistasymboli") ||
           lowChar == NFmiString("muunnos"))
    return dMapping;

  else if (lowChar == NFmiString("scaledsymbol") || lowChar == NFmiString("skaalamuunnos"))
    return dScaleMapping;

  else if (lowChar == NFmiString("name") || lowChar == NFmiString("nimi"))
    return dConstSymbolName;

  else if (lowChar == NFmiString("includenightsymbols") ||
           lowChar == NFmiString("y�symbolitmukaan"))
    return dIncludeNightSymbols;

  else
    return NFmiParamRect::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 * \param theDestinationFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::CopyShortSymbol2Dest(NFmiString *symbolFile,
                                               ofstream &theDestinationFile,
                                               float theRotating)
{
  double xScale = itsDefToProductScale.GetXScaling();
  double yScale = itsDefToProductScale.GetYScaling();
  double xTrans = itsDefToProductScale.GetEndStartPoint().X() / xScale;
  double yTrans = itsDefToProductScale.GetEndStartPoint().Y() / yScale;
  NFmiPoint currentPoint(xTrans, yTrans);
  if (currentPoint == itsPreviousPoint)
  {
    *itsLogFile << "  Covering symbol dropped: " << static_cast<char *>(*symbolFile) << " at "
                << static_cast<char *>(itsPressParam->GetCurrentStation().GetName()) << endl;
    return true;  // false antaisi virheilmoituksen
  }

  NFmiString fileName = *itsSubDir;
  fileName += itsSymbolSetName;
  fileName += NFmiString("_");
  fileName += *symbolFile;
  fileName += NFmiString(".ps");
  ifstream inFile(fileName, ios::in | ios::binary);
  if (inFile.good() && !inFile.eof())
  {
    NFmiWritePSConcat(itsDefToProductScale, theDestinationFile, theRotating);
    NFmiCopyFile(inFile, theDestinationFile);
    NFmiWritePSEnd(theDestinationFile);
    itsPreviousPoint = currentPoint;
    return isTrue;
  }
  else
    return false;
}

// ----------------------------------------------------------------------
/*!
 * raw, uncompressed symbol code newer than compressed one;
 * a new compressed one should be built
 *
 * \param symbolFile Undocumented
 * \param theDestinationFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::RawSymbolToBeConverted(NFmiString *symbolFile)
{
  NFmiString fileName = *itsSubDir;
  fileName += itsSymbolSetName;
  fileName += NFmiString("_");
  fileName += *symbolFile;
  fileName += NFmiString(".ps");
  long comprAge = NFmiFileSystem::FileAge(fileName.CharPtr());

  NFmiString inputName = *itsOrigDir;
  inputName += itsSymbolSetName;
  inputName += kFmiDirectorySeparator;
  inputName += *symbolFile;
  inputName += NFmiString(".ai");
  long unComprAge = NFmiFileSystem::FileAge(inputName.CharPtr());

  return unComprAge >= 0 && (comprAge < 0 || unComprAge <= comprAge);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ConvertOrig2Short(NFmiString *symbolFile)
{
  // VAIN WINDOWSILLA TEHDYILLE SYMBOLEILLE

  NFmiString inputName = *itsOrigDir;
  inputName += itsSymbolSetName;
  inputName += NFmiString("/");
  inputName += *symbolFile;
  inputName += NFmiString(".ai");
  ifstream input(inputName, ios::in);

  if (input.good() && !input.eof())
  {
    NFmiString outputName = *itsSubDir;
    outputName += itsSymbolSetName;
    outputName += NFmiString("_");
    outputName += *symbolFile;
    outputName += NFmiString(".ps");
    ofstream output(outputName);

    const short lineSize = 250;  // HUOM max rivipituus
    char inBuf[lineSize];
    bool copyOn = false;
    unsigned long copiedLines = 0;
    NFmiString mess = NFmiString("%*** ");
    mess += *symbolFile;
    mess += NFmiString(" BEGINS ***");
    unsigned long len = mess.GetLen();
    output.write(mess.CharPtr(), len);
    output.write(static_cast<char *>(NFmiString("\n")), 1);
    short num;
    char endLine = '\n';
    while (input.getline(inBuf, lineSize, endLine))  // newline PC:lle
    {
      NFmiString apu = NFmiString(inBuf);

      if (apu.Search(NFmiString("%AI5_BeginLayer")) > 0 && copyOn)
      {
        if (itsLogFile)
          *itsLogFile << "*** ERROR: Conversion of Symbol " << static_cast<char *>(*symbolFile)
                      << " failed, BeginLayers inside each others" << endl;
        copyOn = isTrue;
      }
      else if (apu.Search(NFmiString("%AI5_BeginLayer")) > 0 &&
               apu.Search(NFmiString("%AI5_EndLayer")) < 1)
      {
        copyOn = isTrue;
      }

      if (copyOn)
      {
        num = input.gcount();
        output.write(inBuf, num - 1);
        output.put('\x0A');  // jotta difference k�ytett�viss�
        copiedLines++;
      }
      if (apu.Search(NFmiString("%AI5_EndLayer")) > 0 && copyOn)
      {
        copyOn = false;
      }
    }
    mess = NFmiString("%*** ");
    mess += *symbolFile;
    mess += NFmiString(" ENDS ***");
    len = mess.GetLen();
    output.write(mess.CharPtr(), len);
    output.write(static_cast<char *>(NFmiString("\n")), 1);

    if (itsLogFile)
      *itsLogFile << "lines copied: " << copiedLines << " as" << endl;

    return isTrue;
  }
  else
    return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theAbsoluteRectOfSymbolGroup Undocumented
 * \param theQI Undocumented
 * \param theDestinationFile Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::WritePS(const NFmiRect &theAbsoluteRectOfSymbolGroup,
                                  NFmiFastQueryInfo *theQI,
                                  ofstream &theDestinationFile,
                                  FmiPressOutputMode theOutput)
{
  fBackupReported = false;
  fBackupDayForThisPar = false;
  bool lastElementStatus = GetPressProduct()->LastSymbolStatus();
  GetPressProduct()->SetLastSymbolStatus(true);
  if (itsPressParam->IsBackupStation())
  {
    if (lastElementStatus)
    {
      *itsLogFile << "  backup station not needed for Symbol" << endl;
      return true;
    }
    else
      *itsLogFile << "  backup station used for Symbol" << endl;
  }

  if (!ActiveTimeIndex(itsPressParam->GetCurrentStep()))
  {
    return true;
  }

  bool isMax;
  NFmiPoint correctedPoint;

  if (IsMaxMinPlotting())
  {
    itsPressParam->SetMaxMinPoints();  // vain ekalla kerralla
    if (!itsPressParam->IsMaxMin(isMax, correctedPoint))
      return true;
  }

  itsCurrentSegmentTime = (static_cast<NFmiQueryInfo *>(theQI))->Time();
  itsCurrentTime = itsCurrentSegmentTime;

  itsDefToProductScale.SetEndScales(
      theAbsoluteRectOfSymbolGroup.ToAbs(NFmiRect(TopLeft(), BottomRight())));

  float rescale = GetAlternatingSizeFactor();
  if (rescale > 0.)
    itsDefToProductScale.RescaleEndScales(rescale);

  if (itsPressParam->GetCurrentStationStep() % 2 == 0)
  {
    itsDefToProductScale.MoveEndScales(itsAlternating);
  }

  if (!fIsConstSymbol)
  {
    if (!ReadValues(theQI))
      return false;
  }

  if (IsEquiDistanceAndCorrMode())
    itsDefToProductScale.MoveEndScales(itsCorrPoint);

  if (!fMarkingValue)
    return true;

  bool isScaled = false;
  NFmiString *symbolFile;

  if (fIsConstSymbol)
  {
    symbolFile = &itsConstSymbol;
  }
  else
  {
    if (IsMaxMinPlotting())
    {
      if (isMax)
        symbolFile = &itsMaxText;
      else
        symbolFile = &itsMinText;
    }

    else if (itsMultiMapping)
    {
      bool missingFound;
      CompleteMultiMapping();

      symbolFile = itsMultiMapping->Map(itsCurrentParamArray, missingFound);

      if (fUseDayNightSymbols && IsDayNightString(*symbolFile))
      {
        //	*itsLogFile << "SYMBOL "
        //				<< static_cast<char *>(*symbolFile)
        //				<< endl;
        float elev = SunElevation(theQI);
        //*itsLogFile << "ELEV " << elev << endl;
        if (elev < -.83)
          // if(itsLanguage == kLatvian)
          //	*symbolFile += NFmiString("_night");
          // else
          //*symbolFile += NFmiString("y�");
          *symbolFile += NFmiString("_night");
      }

      if (symbolFile && symbolFile->IsEqual(NFmiString("ristiriita")))
      {
        *itsLogFile << "ERROR: conflict in selecting Symbol" << endl;
      }
      if (missingFound)
      {
        itsNumOfMissing++;
        if (!symbolFile)
        {
          if (!itsMissingString)
          {
            GetPressProduct()->SetLastSymbolStatus(false);
            return false;
          }
          else
          {
            symbolFile = itsMissingString;
          }
        }
      }
      isScaled = false;  // skaalaus viel� poissa
    }
    else
    {
      symbolFile = itsMapping->Map(itsCurrentParamValue, isScaled);
      if (!symbolFile && itsCurrentParamValue == kFloatMissing && itsMissingString)
      {
        GetPressProduct()->SetLastSymbolStatus(false);

        symbolFile = itsMissingString;
        isScaled = false;
      }
    }
  }

  if (isScaled)
  {
    double scaleFactor =
        ((static_cast<long>(itsCurrentParamValue)) % 1000) / 100.;  // tonni on nyt fixattu
    NFmiRect endScales = itsDefToProductScale.GetEndScales();
    endScales.Inflate(scaleFactor);
    itsDefToProductScale.SetEndScales(endScales);
  }

  ScaleByValue();

  if (!symbolFile)
    itsPressParam->GetPressProduct()->SetLastSymbolStatus(false);
  else
    itsPressParam->GetPressProduct()->SetLastSymbolStatus(true);

  if (!symbolFile)
  {
    if (itsMultiMapping)
    {
      *itsLogFile << "WARNING: No symbol mapping for " << itsCurrentParamArray[0] << ", "
                  << itsCurrentParamArray[1] << ", " << itsCurrentParamArray[2] << endl;
    }
    else
    {
      *itsLogFile << "WARNING: No symbol mapping for value " << itsCurrentParamValue << endl;
    }
  }
  else if (*symbolFile != NFmiString("None") && theOutput == kPostScript)
  {
    if (itsPressParam->IsDistanceCheck() && GetOrder() <= 1)
    {
      float value = itsCurrentParamValue;
      if (itsMultiMapping)
        value = itsCurrentParamArray[0];

      if (!itsPressParam->CheckAndSetDistance(static_cast<long>(round(value)),
                                              theAbsoluteRectOfSymbolGroup.Place()))
        return false;
    }

    bool rawToBeConverted = RawSymbolToBeConverted(symbolFile);

    if (rawToBeConverted)
    {
      if (ConvertOrig2Short(symbolFile))
      {
        if (itsLogFile)
          *itsLogFile << "Symbol " << static_cast<char *>(*symbolFile) << " converted" << endl;
      }
      else
      {
        string msg = string("Symbol missing from set: ") + static_cast<char *>(itsSymbolSetName) +
                     ":" + static_cast<char *>(*symbolFile);

        *itsLogFile << "*** ERROR: " << msg << endl;
        errors.push_back(msg);
      }
    }
    if (CopyShortSymbol2Dest(symbolFile, theDestinationFile, itsRotatingAngle))
    {
    }
    else
    {
      if (itsLogFile)
      {
        string msg = string("Short symbol not found: ") + static_cast<char *>(itsSymbolSetName) +
                     ":" + static_cast<char *>(*symbolFile);

        *itsLogFile << "*** ERROR: " << msg << endl;
        errors.push_back(msg);

        if (rawToBeConverted)
          *itsLogFile << "       missing permission ?" << endl;
      }
    }
  }
  else if (*symbolFile != NFmiString("None") && theOutput == kMetaLanguage)
  {
    NFmiPoint realPos(theAbsoluteRectOfSymbolGroup.BottomLeft());

    // HUOM ??? otetaan takaisin t�m� alunperin selitt�m�t�n siirto
    realPos += NFmiPoint(20, -20);

    WriteMetaCode(symbolFile, realPos, theDestinationFile);
  }

  else if (*symbolFile != NFmiString("None") && theOutput == kPlainText)
  {
    theDestinationFile << static_cast<char *>(*symbolFile) << endl;
  }
  else if (*symbolFile != NFmiString("None") && theOutput == kXml)
  {
    theDestinationFile << "<Picture>" << endl;
  }

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 * \param realPos Undocumented
 * \param os Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSymbolParamRect::WriteMetaCode(NFmiString *symbolFile, NFmiPoint realPos, ofstream &os)
{
  os << endl
     << "SymbolPath /var/www/share/symbolit/" << static_cast<char *>(itsSymbolSetName) << endl;
  os << "Symbol " << static_cast<char *>(*symbolFile) << ".gif " << realPos.X() << " "
     << realPos.Y() << " -scalex " << int(itsDefToProductScale.GetXScaling() * 100.) << "%"
     << " -scaley " << int(itsDefToProductScale.GetYScaling() * 100.) << "%";
  os << endl;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \param setRelHour Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ReadValues(NFmiFastQueryInfo *theQI, bool setRelHour)
{
  //  Vain tunnit voi muuttaa t�ll� tasolla, segmentin aikaluuppi menee muuten sekaisin

  if (setRelHour)  // estet��n 2SymbolParamRectilt� kahta aikamuutosta
  {
    if (!SetRelativeHour(theQI, NFmiString("#Symboli")))
      return isFalse;
  }

  if (itsMultiMapping)
  {
    return ReadCurrentValueArray(theQI);
  }
  else
  {
    if (!PointOnParam(theQI, GetDataIdent().GetParam()) || !PointOnLevel(theQI))
      return false;

    return ReadCurrentValue(theQI, itsCurrentParamValue);
  }
}

// ======================================================================
