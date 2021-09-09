// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressManager
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiPressManager.h"
// newbase
#include "NFmiEnumConverter.h"
#include "NFmiSettings.h"
#include "NFmiTiesaaAlueet.h"
// system
#include <cstdlib>
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePressProduct Undocumented
 * \param theOutMode Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressManager::PreProcessManager(const NFmiFileString &inputFile)
{
  NFmiFileString inputFileName = inputFile.GetChars(1, inputFile.GetLen() - 4);
  //   itsInFileName = inputFileName.Header(); //27.8.02 skriptist‰ kun kutsutaan tulee muuten koko
  //   polku

  NFmiFileString tempInput;
#ifndef UNIX
  tempInput = GetHome();
  tempInput += kFmiDirectorySeparator;
  tempInput += NFmiString("Temp");
  char *env = getenv("lehtiTempDir");
  if (env != 0)
    tempInput = static_cast<NFmiString>(env);
#else
  tempInput = getTmpPath();
  tempInput += kFmiDirectorySeparator;
  tempInput += getProductName();

#endif
  tempInput += kFmiDirectorySeparator;

  NFmiString inputOnlyFile = inputFileName.FileName();

  tempInput += inputFileName.FileName();
  tempInput += NFmiString("pssm");

  NFmiString outFileName = inputOnlyFile;

  inputFileName += NFmiString("pssm");

  string inputStdName(inputFile);
  string outputStdName(tempInput);

  if (!PreProcessDefinition(inputStdName, outputStdName))
  {
    *itsLogFile << "***ERROR: PROGRAM INTERRUPTED, see above" << endl;
    return false;
  }

  delete itsDescriptionFile;
  itsDescriptionFile = new ifstream(tempInput, ios::in);

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePressProduct Undocumented
 * \param theOutMode Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressManager::ReadDescriptionAndWrite(NFmiPressProduct &thePressProduct,
                                               bool &theManagerReadFailed,
                                               FmiPressOutputMode theOutMode)
{
  theManagerReadFailed = false;
  long long1 = 0, long2 = 0, statNum = 0;
  unsigned long uLong = 0;
  double lon = 0, lat = 0;
  NFmiString helpString;

  itsLogFile = thePressProduct.GetLogFile();

  itsHomePath = thePressProduct.GetHome();
  itsTmpPath = thePressProduct.getTmpPath();

  FmiPressOutputMode outMode = theOutMode;
  FmiEnumSpace helpEnumSpace = kPressRegions;

  NFmiMetTime firstPlotTime(thePressProduct.GetFirstPlotTime());

#ifndef UNIX
  NFmiFileString inputFileName = thePressProduct.GetHome();
  inputFileName += kFmiDirectorySeparator;
  inputFileName += NFmiString("Managerit");
#else
  NFmiString mpath = NFmiSettings::Require<string>("press::managerspath");
  NFmiFileString inputFileName = mpath;
#endif
  inputFileName += kFmiDirectorySeparator;
  inputFileName += NFmiString("nimi.prem");
  inputFileName.Header(
      static_cast<char *>(NFmiFileString(thePressProduct.GetInFileName()).Header()));

  *itsLogFile << "the manager would be: " << static_cast<char *>(inputFileName) << endl;

  if (itsDescriptionFile)
    delete itsDescriptionFile;

  itsDescriptionFile = new ifstream(inputFileName, ios::in);
  if (!itsDescriptionFile)
  {
    if (itsLogFile)
      *itsLogFile << "Failed to open file: '" << inputFileName << "'" << endl;
    return false;
  }

  if (!itsDescriptionFile->good() || itsDescriptionFile->eof())
  {
    if (itsLogFile)
      *itsLogFile << "no manager found" << endl;
    return false;
  }
  else if (itsLogFile)
    *itsLogFile << "Manager file opened: " << static_cast<char *>(inputFileName) << endl;

  SetSeasonsStatus(thePressProduct.GetSeasonsStatus());
  SetHome(thePressProduct.GetHome());
  if (!PreProcessManager(inputFileName))
  {
    theManagerReadFailed = true;
    return false;
  }

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;

  itsIntObject = ConvertDefText(itsString);

  bool changed = true;  // sallitaan aloitus suoraan TeeTuotteesta

  while (itsIntObject != dEnd || itsCommentLevel)
  {
    if (itsLoopNum > itsMaxLoopNum)
    {
      if (itsLogFile)
        *itsLogFile << "*** ERROR: Max length exceeded in the Manager" << endl;
      return false;
    }
    itsLoopNum++;
    if (itsIntObject != dEndComment && itsCommentLevel)
      itsIntObject = dComment;
    switch (itsIntObject)
    {
      case dOther:  // ylim‰‰r‰ist‰ roinaa
      {
        if (itsLogFile)
          *itsLogFile << "*** ERROR: Unknown keyword in manager: " << static_cast<char *>(itsObject)
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
      case dManWritePs:
      {
        if (changed)
        {
          if (!thePressProduct.WritePS(outMode))
          {
            if (itsLogFile)
              *itsLogFile << "*** ERROR: Problems writing output with the manager" << endl;
            return false;
          }
        }
        else if (itsLogFile)
          *itsLogFile << "*** ERROR: Identical product from manager, not processed" << endl;

        changed = false;
        ReadNext();
        break;
      }
      case dManStation:
      case dManStationAndImages:
      {
        bool isAlsoImages = itsIntObject == dManStationAndImages;
        NFmiString string1;
        lon = kFloatMissing;
        lat = kFloatMissing;
        statNum = 0;

        if (!ReadEqualChar())
          break;

        NFmiValueString valueString = ReadString();
        if (valueString.IsNumeric())
        {
          statNum = static_cast<int>(valueString);
          string1 = ReadString();
        }
        else
        {
          statNum = itsLoopNum;
          string1 = valueString;
        }
        valueString = ReadString();

        if (valueString.IsNumeric())
        {
          lon = static_cast<double>(valueString);
          ReadOne(lat);
          ReadNext();
        }
        else
        {
          itsString = valueString;
          itsIntObject = ConvertDefText(itsString);
        }

        NFmiLocation location(lon, lat);
        location.SetName(string1);
        location.SetIdent(statNum);

        thePressProduct.SetFirstStation(location);

        if (isAlsoImages)
          thePressProduct.SetImagePreNames(location);

        changed = true;

        break;
      }
      case dManStationNewName:
      {
        NFmiString string1, string2;
        lon = kFloatMissing;
        lat = kFloatMissing;
        statNum = 0;

        if (!ReadEqualChar())
          break;

        NFmiValueString valueString = ReadString();
        if (valueString.IsNumeric())
        {
          statNum = static_cast<int>(valueString);
          string1 = ReadString();
        }
        else
        {
          statNum = itsLoopNum;
          string1 = valueString;
        }

        string2 = ReadString();  // t‰m‰ extraa edelliseen verrattuna

        valueString = ReadString();

        if (valueString.IsNumeric())
        {
          lon = static_cast<double>(valueString);
          ReadOne(lat);
          ReadNext();
        }
        else
        {
          itsString = valueString;
          itsIntObject = ConvertDefText(itsString);
        }

        NFmiLocation location(lon, lat);
        location.SetName(string1);
        location.SetIdent(statNum);
        thePressProduct.SetFirstStation(location);

        NFmiRenaming reName;
        reName.originalName = string1;
        reName.newName = string2;
        thePressProduct.SetStationRename(reName);  // SetFirstStation:n j‰lkeen

        changed = true;

        break;
      }
      case dManMaskNumber:  // vain tiemaskit;
        // MANAGERISSA TOSIN EI ONNISTUNE ENUMSPACEN VAIHTO
        helpEnumSpace = kRoadRegions;
      case dManPressMaskNumber:  // muut kuin tiemaskit
      {
        if (!ReadEqualChar())
          break;

        NFmiString maskName = ReadString();
        string stdString(maskName);
        NFmiEnumConverter converter(helpEnumSpace);
        uLong = converter.ToEnum(stdString);

        if (uLong != kTieAlueNone)
        {
          thePressProduct.SetMaskNumber(uLong, helpEnumSpace);
          changed = true;
        }
        else
        {
          *itsLogFile << "*** ERROR: Invalid PressMask : " << static_cast<char *>(maskName)
                      << " in space " << static_cast<int>(helpEnumSpace) << endl;
        }

        ReadNext();
        break;
      }
      case dManActivateFirstSegment:
      {
        thePressProduct.SetFirstSegmentActivity(true);
        changed = true;

        ReadNext();
        break;
      }
      case dManDeactivateFirstSegment:
      {
        thePressProduct.SetFirstSegmentActivity(false);
        changed = true;

        ReadNext();
        break;
      }
      case dManActivateFirst:
      {
        thePressProduct.SetFirstObjectActivity(true);
        changed = true;

        ReadNext();
        break;
      }
      case dManDeactivateFirst:
      {
        thePressProduct.SetFirstObjectActivity(false);
        changed = true;

        ReadNext();
        break;
      }
      case dManActivateFirstPassive:
      {
        thePressProduct.ChangeFirstPossibleObject(true);
        changed = true;

        ReadNext();
        break;
      }
      case dManDeactivateFirstActive:
      {
        thePressProduct.ChangeFirstPossibleObject(false);
        changed = true;

        ReadNext();
        break;
      }
      case dManNumberAddingToName:
      {
        if (SetOne(long1))
          thePressProduct.ActivateNumberToName(long1);

        changed = true;

        break;
      }
      case dManDeleteNumberAddingToName:
      {
        thePressProduct.DeActivateNumberToName();

        changed = true;

        break;
      }
      case dManProduct:
      {
        if (!ReadEqualChar())
          break;

        ReadNext();
        thePressProduct.SetProductName(itsString);

        changed = true;

        ReadNext();
        break;
      }
      case dManProductFormat:
      {
        if (!ReadEqualChar())
          break;

        ReadNext();
        thePressProduct.SetProductNameFormat(itsString);

        changed = true;

        ReadNext();
        break;
      }
      case dManCloseLog:
      {
        *itsLogFile << "** Closing log from the manager" << endl;
        thePressProduct.SetLogFile(false);

        ReadNext();
        break;
      }
      case dManOpenLog:
      {
        // ei toimi
        thePressProduct.SetLogFile(true);

        ReadNext();
        break;
      }
      case dManData:
      {
        if (!ReadEqualChar())
          break;

        NFmiString str = ReadString();
        str.LowerCase();  // kaikkialla k‰sitell‰‰n pienin‰

        thePressProduct.SetSegmentData(str);
        changed = true;

        ReadNext();
        break;
      }
      case dManRelativeHours:
      {
        SetTwo(long1, long2);
        firstPlotTime = NFmiMetTime();
        firstPlotTime.ChangeByHours(long1);
        firstPlotTime.NextMetTime(static_cast<short>(long2 * 60));

        thePressProduct.SetAllTimes(firstPlotTime);
        changed = true;

        break;
      }
      case dManAddHours:
      {
        SetOne(long1);
        firstPlotTime = thePressProduct.GetFirstPlotTime();
        firstPlotTime.ChangeByHours(long1);

        thePressProduct.SetAllTimes(firstPlotTime);
        changed = true;

        break;
      }
      case dManLanguage:
      {
        if (!ReadEqualChar())
          break;

        thePressProduct.SetAllLanguages(ReadLanguage());

        changed = true;

        ReadNext();

        break;
      }
      case dManOutputMode:
      {
        if (!ReadEqualChar())
          break;

        *itsDescriptionFile >> itsObject;
        helpString = itsObject;
        helpString.LowerCase();
        if (helpString == NFmiString("postscript") || helpString == NFmiString("ps") ||
            helpString == NFmiString("eps"))
          outMode = kPostScript;
        else if (helpString == NFmiString("meta") || helpString == NFmiString("metakieli") ||
                 helpString.GetChars(1, 5) == NFmiString("magic"))
          outMode = kMetaLanguage;
        else if (helpString == NFmiString("teksti") || helpString == NFmiString("text"))
          outMode = kPlainText;
        else if (helpString == NFmiString("xml"))
          outMode = kXml;
        else
        {
          if (itsLogFile)
            *itsLogFile << "*** ERROR: Unknown output format in manager: "
                        << static_cast<char *>(helpString) << endl;
        }
        changed = true;

        ReadNext();
        break;
      }
      case dEnd:
      {
        return true;
      }
    }
  }
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

int NFmiPressManager::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if (lowChar == NFmiString("makeproduct") || lowChar == NFmiString("teetuote"))
    return dManWritePs;

  else if (lowChar == NFmiString("mask") || lowChar == NFmiString("maski"))
    return dManMaskNumber;

  else if (lowChar == NFmiString("areamask") || lowChar == NFmiString("aluemaski"))
    return dManPressMaskNumber;

  else if (lowChar == NFmiString("activatefirstelement") ||
           lowChar == NFmiString("aktivoiekaelementti"))
    return dManActivateFirst;

  else if (lowChar == NFmiString("deactivatefirstelement") ||
           lowChar == NFmiString("deaktivoiekaelementti"))
    return dManDeactivateFirst;

  else if (lowChar == NFmiString("activatefirstpassiveelement") ||
           lowChar == NFmiString("aktivoiekapassiivinenelementti"))
    return dManActivateFirstPassive;

  else if (lowChar == NFmiString("deactivatefirstactiveelement") ||
           lowChar == NFmiString("deaktivoiekaaktiivinenelementti"))
    return dManDeactivateFirstActive;

  else if (lowChar == NFmiString("activatefirstsegment") ||
           lowChar == NFmiString("aktivoiekasegmentti"))
    return dManActivateFirstSegment;

  else if (lowChar == NFmiString("deactivatefirstsegment") ||
           lowChar == NFmiString("deaktivoiekasegmentti"))
    return dManDeactivateFirstSegment;

  else if (lowChar == NFmiString("productname") || lowChar == NFmiString("product") ||
           lowChar == NFmiString("tuotenimi"))
    return dManProduct;

  else if (lowChar == NFmiString("productnameformat") ||
           lowChar == NFmiString("tuotenimiformaatti"))
    return dManProductFormat;

  else if (lowChar == NFmiString("station") || lowChar == NFmiString("asema"))
    return dManStation;

  else if (lowChar == NFmiString("stationandfilenames") ||
           lowChar == NFmiString("stationandimages") || lowChar == NFmiString("asemajaosakuvat") ||
           lowChar == NFmiString("asemajatiedostonimet"))
    return dManStationAndImages;

  else if (lowChar == NFmiString("stationnewnaming") || lowChar == NFmiString("asemanime‰vaihtaen"))
    return dManStationNewName;

  else if (lowChar == NFmiString("productnamenumbering") ||
           lowChar == NFmiString("tuoteniminumerointi"))
    return dManNumberAddingToName;

  else if (lowChar == NFmiString("stopnumbering") ||
           lowChar == NFmiString("tuoteniminumerointipois"))
    return dManDeleteNumberAddingToName;

  else if (lowChar == NFmiString("closelogfile") || lowChar == NFmiString("suljeloki"))
    return dManCloseLog;

  else if (lowChar == NFmiString("openlogfile") ||
           lowChar == NFmiString("avaaloki"))  // ei kai toimi
    return dManOpenLog;

  else if (lowChar == NFmiString("data"))
    return dManData;

  else if (lowChar == NFmiString("outputmode") || lowChar == NFmiString("tulostusmuoto"))
    return dManOutputMode;

  else if (lowChar == NFmiString("relativehours") || lowChar == NFmiString("suhteellinentunti"))
    return dManRelativeHours;

  else if (lowChar == NFmiString("addhours") || lowChar == NFmiString("addhour") ||
           lowChar == NFmiString("tuntimuutos"))
    return dManAddHours;

  else if (lowChar == NFmiString("language") || lowChar == NFmiString("kieli"))
    return dManLanguage;
  else
    return NFmiPressDescription::ConvertDefText(object);
}

// ======================================================================
