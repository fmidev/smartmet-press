// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressProduct
 */
// ======================================================================

#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiCopyFile.h"
#include "NFmiHyphenationString.h"
#include "NFmiPressArea.h"
#include "NFmiPressComputerTimeText.h"
#include "NFmiPressGivenTimeText.h"
#include "NFmiPressNameDay.h"
#include "NFmiPressProduct.h"
#include "NFmiPressStationText.h"
#include "NFmiPsSymbol.h"
// newbase
#include "NFmiLocationFinder.h"
#include "NFmiSettings.h"
#include "NFmiSuperSmartInfo.h"
#include "NFmiFileSystem.h"   
// system
#include <cstdlib>
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Void constructor
 *
 * \todo Should use initialization list instead of code in body
 */
// ----------------------------------------------------------------------

NFmiPressProduct::NFmiPressProduct(void)
{
  itsNameTimeFormat = kUndefined;
  itsSecondNameTimeFormat = kUndefined;
  itsNameDay = 0;
  itsLogFile = new ofstream;
  itsCurrentDataIter = 0;
  itsNameToLonLat = new NFmiLocationFinder();
  itsLanguage = kFinnish;
  itsPalette = 0;
  itsPageSize = kLetter;  //*****HUOM m‰‰rittelyihin A4- jne arkki, 
                //t‰m‰n muuttaminen tn. vaikuttaisi joihinkin vanhoihin
  fChangeScandinavian = false;
  itsCurrentNumberToName = kLongMissing;
  itsNumOfWritePS = 0;
  fMakeElementsAfterSegments = false;
  itsMaskIter = 0;
}

// ----------------------------------------------------------------------
/*!
 * The destructor deletes all internally stored pointers
 *
 * \todo The ifs are unnecessary, delete 0 is safe
 */
// ----------------------------------------------------------------------

NFmiPressProduct::~NFmiPressProduct(void)
{
  if(itsNameDay)
	delete itsNameDay;
  if(itsCurrentDataIter)
	delete itsCurrentDataIter;
  if(itsMaskIter)
	delete itsMaskIter;
  delete itsNameToLonLat;
  itsParams.Clear(true);
  itsSameSymbols.Clear(true);
  itsEpsFileNames.Clear(true);
  itsDatas.Clear(true);
  if(itsLogFile)
	delete itsLogFile;
  if(itsPalette)
	delete itsPalette;
  if(itsDescriptionFile) //owner of NFmiTimeDescription member
	delete itsDescriptionFile;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param value Undocumented
 * \param queueNum Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressProduct::PutInStorage(const float value, int queueNum)
{
  if(queueNum == 1)
	{
	  itsFloatQueue.push(value);
	}
  else
	{
	  itsFloatQueue2.push(value);
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param queueNum Undocumented
 * \param errorReport Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiPressProduct::UseFromStorage(int queueNum, bool errorReport)
{
  float value;
  if(queueNum == 1)
	{
	  if(itsFloatQueue.size() < 1)
		{
		  if(errorReport)
			*itsLogFile << "  ***ERROR: Use of empty Product storage 1"
						<< endl;
		  value = kFloatMissing;
		}
	  else
		{
		  value = itsFloatQueue.front();
		  itsFloatQueue.pop();
		}
	}
  else
	{
	  if(itsFloatQueue2.size() < 1)
		{
		  if(errorReport)
			*itsLogFile << "  ***ERROR: Use of empty Product storage 2"
						<< endl;
		  value = kFloatMissing;
		}
	  else
		{
		  value = itsFloatQueue2.front();
		  itsFloatQueue2.pop();
		}
	}
  return value;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param OnOff Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetLogFile(bool OnOff)
{
  //tuleekohan ongelmia kun ohjataan logfileen vaikka se on suljettu
  if(!OnOff)
	{
	  if(itsLogFile)
		{
		  itsLogFile->close();
		  itsLogFile->clear();
		  return true;
		}
	  return false;
	}
  else
	{
	  return false;
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetProductName(const NFmiString & theName)
{
  itsOutFileName = theName;
  fTimeStamp = false;
  fDataTimeStamp = false;
  itsProductNameFormat = NFmiString(); //nollaus

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetProductNameFormat(const NFmiString & theName)
{
  itsOutFileName = NFmiString(); //nollaus
  itsProductNameFormat = theName;

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theNumber Undocumented
 * \param theEnumSpace Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetMaskNumber(unsigned long theNumber, FmiEnumSpace theEnumSpace)
{
  bool retCode = false;
//  if (GetEnvironment().ChangeMaskNumber(theNumber)) //muutetaan vain asetettua
  if (GetEnvironment().ChangeMask(theNumber, theEnumSpace)) //muutetaan vain asetettua
	retCode = true;
  NFmiVoidPtrIterator paramIter(itsParams);
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  while(param)
	{
	  if (param->ChangeMaskNumber(theNumber, theEnumSpace))	// muutetaan vain asetettua
		retCode = true;							// ainakin yhdess‰ vaihdettu
	  param = static_cast<NFmiPressParam *>(paramIter.Next());
	}
  return retCode;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDataName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetSegmentData(const NFmiString & theDataName)
{
  bool retCode = false;
  NFmiVoidPtrIterator paramIter(itsParams);
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  while(param)
	{
	  if (param->SetData(theDataName))
		retCode = true; //ainakin yhdess‰ vaihdettu
	  param = static_cast<NFmiPressParam *>(paramIter.Next());
	}
  return retCode;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theActivity Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetFirstSegmentActivity(bool theActivity)
{
  //voisi kehitt‰‰ niin ett‰ muutetaan eka sellainen joka
  //on eri kuin theActivity, niin alusta voisi (de)aktivoida n kpl

  NFmiVoidPtrIterator objectIter(itsParams);
  NFmiPressParam* object;  //PsWriting kaataa, ent‰ NFmiPressScaling
  objectIter.Reset();
  object = static_cast<NFmiPressParam *>(objectIter.Next());
  if(object)
	object->SetActivity(theActivity);
  else
  {
	  *itsLogFile << "*** ERROR: elementtej‰ puuttuu (de)aktivoitavaksi managerista" << endl;
	  return false;
  }

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theActivity Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetFirstObjectActivity(bool theActivity)
{
  NFmiVoidPtrIterator objectIter(itsObjects);
  NFmiPressScaling* object;  
  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  if(object)
	object->SetActivity(theActivity);
  else
  {
	  *itsLogFile << "*** ERROR: elementtej‰ puuttuu (de)aktivoitavaksi managerista" << endl;
	  return false;
  }

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theActivity Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ChangeFirstPossibleObject(bool toNewActivity)
{
  //alusta voi vuoronper‰‰n (de)aktivoida n kpl

  NFmiVoidPtrIterator objectIter(itsObjects);
  NFmiPressScaling* object;  
  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while(object)
	if(object->IsActive() == toNewActivity)
	{
		object = static_cast<NFmiPressScaling *>(objectIter.Next());
	}
	else
	{
		object->SetActivity(toNewActivity);
		return true;
	}

  *itsLogFile << "*** ERROR: ei ole elementtej‰ (de)aktivoitavaksi managerista" << endl;
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLocation Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetFirstStation(const NFmiLocation& theLocation)
{
  bool retCode = false;
  NFmiVoidPtrIterator paramIter(itsParams);
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  while(param)
	{
	  if (param->SetFirstStation(theLocation))
		retCode = true; //ainakin yhdess‰ vaihdettu
	  param = static_cast<NFmiPressParam *>(paramIter.Next());
	}
  return retCode;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLocation Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetImagePreNames(const NFmiLocation& theLocation)
{
  bool retCode = false;
  NFmiVoidPtrIterator objectIter(itsObjects);
  NFmiPressImage * image;
  objectIter.Reset();
  NFmiHyphenationString oldFile;
  NFmiFileString path, newFile;
  NFmiString dummy;

  // ennen alaviiva oleva osa korvataan uudella asemanimell‰
  NFmiString underScore("_");
  image = static_cast<NFmiPressImage *>(objectIter.Next());
  while(image)
	{
	  if(image->ClassId() == kNFmiPressImage)
		{
		  path = image->GetPath();
		  oldFile = path.Header();
		  if(oldFile.NextSubString(underScore, dummy))
			{
			  newFile = theLocation.GetName();
			  newFile += oldFile.GetChars(oldFile.CurrentCharPosition()-1, oldFile.GetLen());
			  path.Header(static_cast<char *>(newFile));
			  image->SetPath(path);
			  retCode = true;
			}
		}
	  image = static_cast<NFmiPressImage *>(objectIter.Next());
	}
  return retCode;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theRename Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetStationRename(const NFmiRenaming& theRename)
{
  bool retCode = false;
  NFmiVoidPtrIterator paramIter(itsParams);
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  while(param)
	{
	  if (param->SetStationRename(theRename))
		retCode = true; //ainakin yhdess‰ vaihdettu
	  param = static_cast<NFmiPressParam *>(paramIter.Next());
	}
  return retCode;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::SetAllTimes(const NFmiMetTime& theTime)
{
  SetTime(theTime); //oma ensi

  bool retCode = false;

  NFmiVoidPtrIterator paramIter(itsParams); //segmentit
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  while(param)
	{
	  if (param->SetAllTimes(theTime))
		retCode = true; //ainakin yhdess‰ vaihdettu
	  param = static_cast<NFmiPressParam *>(paramIter.Next());
	}
  //muut objektit, kaikissa ei tosin aikaa
  NFmiVoidPtrIterator objectIter(itsObjects);
  NFmiPressScaling * object;
  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while(object)
	{
	  object->SetTime(theTime);
	  object = static_cast<NFmiPressScaling *>(objectIter.Next());
	}

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

bool NFmiPressProduct::SetAllLanguages(FmiLanguage theLanguage)
{
  itsLanguage = theLanguage; //oma ensi

  bool retCode=true;//HUOM

  NFmiVoidPtrIterator paramIter(itsParams); //segmentit
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  while(param)
	{
	  if (param->SetAllLanguages(theLanguage))
		retCode = true;          //ainakin yhdess‰ vaihdettu
	  param = static_cast<NFmiPressParam *>(paramIter.Next());
	}
  //muut objektit, kaikissa ei tosin aikaa
  NFmiVoidPtrIterator objectIter(itsObjects);
  NFmiPressScaling * object;
  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while(object)
	{
	  object->SetLanguage(theLanguage);
	  object = static_cast<NFmiPressScaling *>(objectIter.Next());
	}

  return retCode;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------
bool NFmiPressProduct::IsSummerWeather(const NFmiString& theCountryPart)
{
    NFmiMetTime today;
	if(FirstData())
	{	
		NFmiFastQueryInfo info = NFmiFastQueryInfo(FirstData());
		today.SetHour(12);
		today.ChangeByDays(2); //tuotteet keskim‰‰rin kahden p‰iv‰n p‰‰ss‰
		if (info.Time(today))
		{
			NFmiPoint lonLat(99.,99.); //not valid on earth 
			NFmiString lowChar(theCountryPart);
			lowChar.LowerCase();
			if(lowChar == NFmiString("lounas")|| lowChar == NFmiString("southwest"))
				lonLat.Set(22.5, 60.5);
			else if(lowChar == NFmiString("etel‰") || lowChar == NFmiString("south"))
				lonLat.Set(25., 60.3);
			else if(lowChar == NFmiString("kaakko") || lowChar == NFmiString("southeast"))
				lonLat.Set(27.5, 61.2);
			else if(lowChar == NFmiString("l‰nsi") || lowChar == NFmiString("west"))
				lonLat.Set(22., 63.);
			else if(lowChar == NFmiString("keski") || lowChar == NFmiString("centre"))
				lonLat.Set(26., 62.5);
			else if(lowChar == NFmiString("it‰") || lowChar == NFmiString("east"))
				lonLat.Set(30., 62.5);
			else if(lowChar == NFmiString("oulu"))
				lonLat.Set(26., 65.);
			else if(lowChar == NFmiString("lappi") || lowChar == NFmiString("north"))
				lonLat.Set(26., 66.5);
			else
			{
				*itsLogFile << "*** ERROR: #OsaKuvassa tuntematon maanosa: " 
					<< static_cast<char *>(theCountryPart) 
					<< endl;
			}
			if(lonLat.Y() < 98. && info.Location(lonLat)) //eka ehto pistetietokantoja varten
			{                                             //jotka muuten aina menev‰t l‰himp‰‰n
				if(info.Param(kFmiTemperature))
				{
					return info.FloatValue() > 2.;
				}
			}
		}
	}
	//jos sopivaa dataa ei ole, hyv‰ aproksimaatio on kes‰/talviaika
    return today.GetZoneDifferenceHour() == -3;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ReadNameToLonLatList(void)
{
  NFmiString fileName, fileName1;
  fileName = GetHome();

  fileName += kFmiDirectorySeparator;
  fileName += NFmiString("Muut");
  fileName += kFmiDirectorySeparator;

  fileName1 = fileName;
  fileName1 += NFmiString("AsemaNimetLonLatUusi.txt");

  // uusi/vanha muoto mahdollistamaan versionp‰ivityksen Vespaan
  if (itsNameToLonLat->AddFile(fileName1, true, true))
	{
	  *itsLogFile << "  AsemaNimetLonLatUusi.txt luettu" << endl;
	  return true;
	}
  else
	{
	  *itsLogFile << "*** ERROR: AsemaNimetLonLatUusi.txt:n luku ep‰onnistui"
				  << endl;
	  return false;
	}
}

// ----------------------------------------------------------------------
/*!
 * ratkaisee voimassa olevat kausivaihtelut tiedostosta lukien tai
 * k‰ytt‰m‰ll‰ oletusarvoja
 *
 * \param theSeasonsStatus Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ReadSeasonsStatus(void)
{
  if(itsSeasonsStatus)
  {
	  delete itsSeasonsStatus;
	  *itsLogFile << "***Warning: Kausivaihtelun lukeminen toistuvasti" <<  endl;
  }
  itsSeasonsStatus = new FmiPressSeasons; 

  NFmiTime today;
  NFmiTime summerStart(today);
  summerStart.SetDate(today.GetYear(), 5, 31);
  NFmiTime summerStop(today);
  summerStop.SetDate(today.GetYear(), 8, 31);
  itsSeasonsStatus->wintertime = today.GetZoneDifferenceHour() == -2;
  itsSeasonsStatus->summer = today >= summerStart && today < summerStop;
  itsSeasonsStatus->pollen = !itsSeasonsStatus->wintertime;
  itsSeasonsStatus->snow = !itsSeasonsStatus->pollen;
  itsSeasonsStatus->pollenOrSnow = itsSeasonsStatus->pollen || itsSeasonsStatus->snow;
  itsSeasonsStatus->weekday = today.GetWeekday();
  itsSeasonsStatus->dayAdvance = +0;
  itsSeasonsStatus->editdata = true;

  string includePath(itsHomePath);
  includePath += kFmiDirectorySeparator;
  includePath += "Muut";
  includePath += kFmiDirectorySeparator;
  includePath += "KausiTilanne.txt";
  ifstream in(includePath.c_str(),ios::in|ios::binary);

  if(!in)
	{
	  *itsLogFile << "ei kausitilanne/muuttuja-tiedostoa"<< endl;
	  return true;
	}

  string::size_type start1, end1, start2, end2, start3, end3;
  const string delims(" \t");
  string line;
  while(getline(in,line))
	{
	  // Ignore the line if it is a comment line or "thrash" line

	  if(line.substr(0,3)=="END" || line.substr(0,5)=="LOPPU")
		break;

	  if(line[0]=='#' || line.substr(0,2)=="//" || line.size() < 6)
		continue;

	  start1 = line.find_first_not_of(delims);
	  end1 = line.find_first_of(delims, start1);
	  start2 = line.find_first_not_of(delims, end1);
	  end2 = line.find_first_of(delims, start2);
	  start3 = line.find_first_not_of(delims, end2);
	  end3 = line.find_first_of(delims, start3);

	  if(end3 == string::npos)
		{
		  *itsLogFile << "***ERROR: invalid line in seasonstatus: "
					  << line
					  << endl;
		  continue;
		}
	  else
		{
		  string str1 = line.substr(start1, end1-start1);
		  NFmiString fmiStr1 = str1;
		  fmiStr1.LowerCase();
		  // loppuosan ei tartte olla oikein kirjoitettu
		  string shortStr1 = str1.substr(start1, 4);
		  NFmiString fmiShortStr1 = shortStr1;
		  fmiShortStr1.LowerCase();
		  string str2 = line.substr(start2, end2-start2);
		  string shortStr2 = str2.substr(0, 2);
		  NFmiString fmiShortStr2 = shortStr2;
		  fmiShortStr2.LowerCase();
		  string str3 = line.substr(start3, end3-start3);

		  bool status = false;
		  NFmiString statusString("???");
		  bool boolGiven = true;
		  bool undef = false;
		  if(fmiShortStr2 == "ye" || fmiShortStr2 == "on")
			{
			  status = true;
			  statusString = "PƒƒLLE";
			}
		  else if(fmiShortStr2 == "no" || fmiShortStr2 == "ei")
			{
			  status = false;
			  statusString = "POIS";
			}
		  else if(fmiShortStr2 == "de" || fmiShortStr2 == "ol")
			undef = true;
		  else
			boolGiven = false;

		  if(fmiStr1 == "wintertime" || fmiStr1 == "talviaika")
			{
			  if(boolGiven && !undef)
				{
				  itsSeasonsStatus->wintertime = status;
				  *itsLogFile << "  Kausitilanne pakotettu: talviaika "<< static_cast<char *>(statusString) << endl;
				}
			}
		  else if(fmiShortStr1 == "summ" || fmiShortStr1 == "kes‰")
			{
			  if(boolGiven && !undef)
				{
				  itsSeasonsStatus->summer = status;
				  *itsLogFile << "  Kausitilanne pakotettu: kes‰ "<< static_cast<char *>(statusString) << endl;
				}
			}
		  else if(fmiShortStr1 == "poll" || fmiShortStr1 == "siit")
			{
			  if(boolGiven && !undef)
				{
				  itsSeasonsStatus->pollen = status;
				  *itsLogFile << "  Kausitilanne pakotettu: siitepˆlykausi "<< static_cast<char *>(statusString) << endl;
				}
			}
		  else if(fmiShortStr1 == "snow" || fmiShortStr1 == "lumi")
			{
			  if(boolGiven && !undef)
				{
				  itsSeasonsStatus->snow = status;
				  *itsLogFile << "  Kausitilanne pakotettu: lumikausi "<< static_cast<char *>(statusString) << endl;
				}
			}
		  else if(fmiShortStr1 == "edit")
			{
			  if(boolGiven && !undef)
				{
				  itsSeasonsStatus->editdata = status;
				  *itsLogFile << "  Editoridata pakotettu: edotori "<< static_cast<char *>(statusString) << endl;
				}
			}
		  else if(fmiShortStr1 == "week" || fmiShortStr1 == "viik")
			{
			  if(!undef)
				{
				  if(fmiShortStr2 == "mo" || fmiShortStr2 == "ma")
					itsSeasonsStatus->weekday = 1;
				  else if(fmiShortStr2 == "tu" || fmiShortStr2 == "ti")
					itsSeasonsStatus->weekday = 2;
				  else if(fmiShortStr2 == "we" || fmiShortStr2 == "ke")
					itsSeasonsStatus->weekday = 3;
				  else if(fmiShortStr2 == "th" || fmiShortStr2 == "to")
					itsSeasonsStatus->weekday = 4;
				  else if(fmiShortStr2 == "fr" || fmiShortStr2 == "pe")
					itsSeasonsStatus->weekday = 5;
				  else if(fmiShortStr2 == "sa" || fmiShortStr2 == "la")
					itsSeasonsStatus->weekday = 6;
				  else if(fmiShortStr2 == "su")
					itsSeasonsStatus->weekday = 7;
				  else
					{
					  *itsLogFile << "***ERROR: invalid weekday in seasonstatus"<< str2 << endl;
					  continue;
					}
				  *itsLogFile << "  Viikonp‰iv‰ pakotettu: "<< fmiShortStr2 << endl;
				}
			}
		  else if(fmiShortStr1 == "adva" || fmiShortStr1 == "enna")
			{
			  if(!undef)
				{
				  itsSeasonsStatus->dayAdvance = atoi(str2.c_str()); //tarvitaanko miss‰‰n t‰ss‰
				  itsEnvironment.SetDayAdvance(itsSeasonsStatus->dayAdvance);
				  *itsLogFile << "  Ennakko pakotettu: "<< str2 << endl;
				}
			}
		  else if(fmiShortStr1 == "muut" || fmiShortStr1 == "vari")
			{
			  itsReplaceMap.insert(pair<string, string>(str2, str3));
			}
		  else
			{
			  *itsLogFile << "***ERROR: invalid keyword in seasonstatus"<< str1 << endl;
			  continue;
			}
		}
	}
  itsSeasonsStatus->pollenOrSnow = itsSeasonsStatus->pollen || itsSeasonsStatus->snow;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param origInput Undocumented
 * \param output Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::PreProcessProduct( ifstream& origInput, ofstream& output)
{
  const short lineSize = 130;	//max rivipituus
  char inBuf[lineSize];
  NFmiString str, file;
  NFmiString incString = NFmiString("#Include");
  NFmiString liitaString = NFmiString("#Liit‰");

  while (origInput.getline(inBuf, lineSize, '\n'))
	{
	  short num = origInput.gcount();
	  str.Set(reinterpret_cast<unsigned char *>(inBuf), num);

	  unsigned short nRisu = static_cast<unsigned short>(str.Search( NFmiString("#")));// jotta lˆytyisi tabinkin takaa

	  if((nRisu>0 && num-nRisu > 8) &&
		 (str.GetChars(nRisu,8) == incString //Huom string-bugi
		  || str.GetChars(nRisu,6) == liitaString))
		{
		  if(str.GetChars(nRisu,8) == incString)
			file = str.GetChars(nRisu+9,num-9 -nRisu); //vois toki tehd‰ alykk‰‰mmin
		  else
			file = str.GetChars(nRisu+7,num-7-nRisu);

		  file.TrimR(' ');

		  NFmiFileString includeFileName = GetHome();

		  includeFileName += kFmiDirectorySeparator;
		  includeFileName += NFmiString("Include");
		  includeFileName += kFmiDirectorySeparator;
		  includeFileName += file;
		  if(!includeFileName.HasExtension())
			includeFileName += NFmiString(".inc");

		  ifstream includeFile(includeFileName, ios::in);
		  if(includeFile.good() && !includeFile.eof())
			{
			  NFmiCopyFile(includeFile, output);
			}
		  else
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: LiiteTiedostoa ei ole: " << static_cast<char *>(file) << endl;
			}
		  includeFile.close();
		  includeFile.clear();
		}
	  else
		{
		  output.write(inBuf, num-1);
		  output.put('\x0A');
		}
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::DefinePar(const NFmiString & theString)
{
  NFmiHyphenationString hString(theString);
  NFmiString retString;

  // Mika: Onko sivuvaikutuksia?
  hString.Search(NFmiString("##PAR"));

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ReplacePar(NFmiString & theString)
{
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param inputFile Undocumented
 * \return Undocumented
 * \todo Take parameter by const reference instead
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ReadDescriptionFile(NFmiString inputFile)
{

  itsOutputMode = kPostScript;

  NFmiString fmiString, origHome;

#ifndef UNIX
  char * hChar;
  hChar = getenv("lehtiTuoteDir");
  if(hChar == 0)
	{
	  fmiString = NFmiString("C:\\Program Files\\LehtiAuto");
	  origHome = NFmiString("(Oletus)");
	}
  else
	{
	  fmiString = hChar;
	  origHome = NFmiString("(Ymp‰ristˆmuuttuja)");
	}
#else
  fmiString = NFmiSettings::Require<string>("press::path");
  origHome = NFmiString("(fmi.conf)");
#endif

  SetHome(fmiString);

  // Tarkistus ett‰ hakemisto olemassa, vaikka nyt endA4.eps:n olemassaolo
  NFmiString endFileName = GetHome();

  endFileName += kFmiDirectorySeparator;
  endFileName += NFmiString("Muut");
  endFileName += kFmiDirectorySeparator;

  endFileName += NFmiString("endA4.eps");
  ifstream endFile(endFileName, ios::in|ios::in);
  if(!endFile)
	{
	  // Mikan korjaus ja Lassen muutos siihen
	  cout << "**Kotihakemisto "
		   << GetHome().GetCharPtr()
		   << " kadoksissa**" <<  endl;
	  cout << "  joko annettava ymp‰ristˆmuuttujana" <<  endl;
#ifndef UNIX
	  cout << "  tai oltava C:\\Program Files\\LehtiAuto" <<  endl;
#else
	  cout << "  tai oltava $HOME/LehtiAuto" <<  endl;
#endif
	  return false;
	}

#ifndef UNIX
  hChar = getenv("lehtiLokiDir");
  if(hChar == 0)
	{
	  fmiString = GetHome();
	  fmiString += kFmiDirectorySeparator;
	  fmiString += NFmiString("Loki");
	}
  else
	   fmiString = hChar;
#else
  fmiString = NFmiSettings::Require<string>("press::logpath");
#endif

   NFmiString logFileName = fmiString;
   logFileName += kFmiDirectorySeparator;
#ifdef UNIX
   if(NFmiSettings::IsSet("press::logfile"))
	 logFileName += NFmiSettings::Require<string>("press::logfile");
   else
	 logFileName += NFmiString("lehti.log");
#else
   logFileName += NFmiString("lehti.log");
#endif


   itsLogFileName = logFileName;   //jotta voidaan sulkea/avata jatkossa
   itsLogFile->open(logFileName, ios::out|ios::app); // edellinen aloitti alusta
   NFmiTime time;
   *itsLogFile << endl << "** Loki avattu " << time << " **" << endl;
//*******
   NFmiString retString;
   NFmiString origInputFileName = inputFile;
   NFmiFileString inputFileName = inputFile.GetChars(1,inputFile.GetLen()-3);
   itsInFileName = inputFileName.Header(); //27.8.02 skriptist‰ kun kutsutaan tulee muuten koko polku

   NFmiFileString tempInput;
   tempInput = GetHome();
   tempInput += kFmiDirectorySeparator;
   tempInput += NFmiString("Temp");
#ifndef UNIX
   char * env = getenv("lehtiTempDir");
   if(env != 0)
	 tempInput =  static_cast<NFmiString>(env);
#else
   if(NFmiSettings::IsSet("press::tmppath"))
	 {
	   string tmp = NFmiSettings::Require<string>("press::tmppath");
	   tempInput = NFmiString(tmp);
	 }
#endif
   tempInput += kFmiDirectorySeparator;

   NFmiString inputOnlyFile = inputFileName.FileName();

   tempInput += inputFileName.FileName();  
   tempInput += NFmiString("pss");
   
   itsOutFileName = inputOnlyFile;  

   inputFileName += NFmiString("pss");
 
   NFmiString writeString = inputFileName.Header();
   *itsLogFile << "** " << static_cast<char *>(writeString) << " **"<< endl;
   *itsLogFile << "program version = 18.1.2004" << endl;       
   *itsLogFile << "Home dir " << static_cast<char *>(origHome) << ": " << static_cast<char *>(GetHome())  << endl;

   string inputStdName(origInputFileName);
   string outputStdName(tempInput);

    ReadSeasonsStatus();
	if (!GetSeasonsStatus())
	{
			*itsLogFile << "*** ERROR: kausivaihtelutiedot puuttuvat; ohjelmointivirhe"  << endl;
			return false;
	}

   if(!PreProcessDefinition(inputStdName, outputStdName))
   {
	  *itsLogFile << "***ERROR: PROGRAM INTERRUPTED, see above" << endl;
	  return false;
	}

  itsDescriptionFile = new ifstream(tempInput, ios::in);

  fDataRead = false;

  if (!ReadDescription(retString))
	return false;

   if(!FirstData())
	 itsCurrentDataIter = 0;

   itsDescriptionFile->close();
   itsDescriptionFile->clear();

   if(!ReadData())
		return false;

   if(itsLogFile)
	 *itsLogFile << "KOOSTETAAN TUOTE" << endl;

   return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return false if reading of mandatory file unsuccessful
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ReadData(void)
{
  if (fDataRead)
	return true;

  if(itsLogFile)
	*itsLogFile << "LUETAAN ANNETUT DATATIEDOSTOT" << endl;

  bool mandatoryNotFound = false;

  NFmiHyphenationString str;
#ifndef UNIX
  char * hChar;
  hChar = getenv("lehtiDataDir");
  if(hChar == 0)
	{
	  str = GetHome();
	  str += kFmiDirectorySeparator;
	  str += NFmiString("Data");
	}
  else
	str = hChar;
#else
  const string tmp = NFmiSettings::Require<string>("press::datapath");
  str = tmp.c_str();
#endif

  *itsLogFile << "  datahakemistot: " << static_cast<char *>(str) << endl;

  // kaksi datahakemistoa, luupituksella voisi ottaa useampiakin
  NFmiString dataPath, dataPath2;
  str.NextSubString(NFmiString(";"),dataPath);
  bool secondDir = false;
  bool twoOptinalTypes = true;
  if(str.NextSubString(NFmiString(";"),dataPath2))
	secondDir = true;


  NFmiQueryData ** data;
  NFmiQueryData * dataPtr;
  NFmiNamedQueryData * nData;
  NFmiVoidPtrIterator iter(itsDatas);
  NFmiString name;
  NFmiFileString dataFile, dataFile2;
  NFmiFileString dataFileSqd, dataFileSqd2;
  iter.Reset();
  nData = static_cast<NFmiNamedQueryData *>(iter.Next());
  short loop = 1;
  while(nData)
	{
	  dataPtr = nData->GetData();
	  data = & dataPtr;
	  dataFile = dataPath;
	  dataFile2 = dataPath2;
	  if(dataFile.IsValue()) dataFile += kFmiDirectorySeparator;
	  if(dataFile2.IsValue()) dataFile2 += kFmiDirectorySeparator;
	  dataFile += nData->GetName();
	  dataFile2 += nData->GetName();
	  dataFileSqd = dataFile;
	  dataFileSqd2 = dataFile2;
	  if(!dataFile.HasExtension())
		{
		  if(!NFmiFileSystem::DirectoryExists(dataFile.CharPtr()))
			dataFile += NFmiString(".fqd");
		  if(!NFmiFileSystem::DirectoryExists(dataFile2.CharPtr()))
			dataFile2 += NFmiString(".fqd");
		  if(!NFmiFileSystem::DirectoryExists(dataFileSqd.CharPtr()))
			dataFileSqd += NFmiString(".sqd");
		  if(!NFmiFileSystem::DirectoryExists(dataFileSqd2.CharPtr()))
			dataFileSqd2 += NFmiString(".sqd");
		}
	  else
		twoOptinalTypes = false;

	  //oletus suoraan odinista; edelleen voi jonkin aikaa k‰ytt‰‰ vanhaa kepaa jos
	  // kausivaihtelut muutetaan
	  // pit‰‰ laittaa myˆs LightBoy
	  if(dataFile.Header() == NFmiString("kepa_suomi_168_1_uusin") && itsSeasonsStatus->editdata)
	  {
			std::string odinDir = "O:\\data\\in\\";
			std::string path;
			path = odinDir;
			path += "PAL_Scand*";
			std::string theFoundFileName;
			NFmiFileSystem::FindFile(path, true, &theFoundFileName);
			//cout << theFoundFileName << endl;
			odinDir += theFoundFileName;
			dataFile = NFmiString(odinDir);
	  }

	  //  k‰yet‰‰n nyt Vilin poikkeusmenettely‰
	  // Yritet‰‰n sek‰ fqd-tiedostoa ett‰ sqd:t‰ ja kahdesta hakemistosta
	 
	  if(!ReadQueryData(*data,dataFile))
		{
		  if(!twoOptinalTypes || !ReadQueryData(*data,dataFileSqd))
			{
			  if(secondDir)
				{
				  if(!ReadQueryData(*data,dataFile2))
					{
					  if(!twoOptinalTypes || !ReadQueryData(*data,dataFileSqd2))
						{
						  nData->SetData(0);
						  *itsLogFile << "  *** ERROR: datan luku ep‰onnistui: " << static_cast<char *>(nData->GetName()) << endl
							         <<  "      jos pakollinen keskeytet‰‰n" << endl;
						  if(nData->IsMandatory())
						     mandatoryNotFound = true;
						}
					  else
						{
						  *itsLogFile << "  sqd-data luettu: " <<  static_cast<char *>(dataFileSqd2) << endl;
						}
					}
				  else
					{
					  *itsLogFile << "  fqd-data luettu: " <<  static_cast<char *>(dataFile2) << endl;
					}
				}
			  else
				{
				  nData->SetData(0);
				  *itsLogFile << "  *** ERROR: datan luku ep‰onnistui: " << static_cast<char *>(nData->GetName()) << endl
							 <<  "      jos pakollinen keskeytet‰‰n" << endl;

				  if(nData->IsMandatory())
					mandatoryNotFound = true;
				}
			}
		  else
			{
			  *itsLogFile << "  sqd-data luettu: " <<  static_cast<char *>(dataFileSqd) << endl;
			}
		}
	  else
		{
		  *itsLogFile << "  fqd-data luettu: " <<  static_cast<char *>(dataFile) << endl;
		}

	  loop++;
	  nData = static_cast<NFmiNamedQueryData *>(iter.Next());
	}
  fDataRead = true;

  if(itsMaskIter)
	delete itsMaskIter;
  if(DataByName(itsMaskFileName))
	itsMaskIter = new NFmiSuperSmartInfo(DataByName(itsMaskFileName));
  else
	itsMaskIter = 0;

  if(itsLogFile)
	{
	  if(loop > 1)
		*itsLogFile << "KAIKKI DATAT LUETTU" << endl;
	  else
		*itsLogFile << "EI ANNETTUJA DATATIEDOSTOJA" << endl;
	}

  return !mandatoryNotFound;
;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param theQD Undocumented
 * \param fileName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ReadQueryData(NFmiQueryData * theQD,char * fileName)
{
  // If directory, find newest file in the directory
  string filename(fileName);
  if(NFmiFileSystem::DirectoryExists(filename))
	{
	  const string file = NFmiFileSystem::NewestFile(filename);
	  *itsLogFile << "  Hakemiston '"
				  << filename
				  << "' uusin tiedosto on '"
				  << file
				  << "'"
				  << endl;
	  filename += kFmiDirectorySeparator;
	  filename += file;
	}

  // This prevents crashes
  if(filename.empty())
	return false;

  std::ifstream dataFile(filename.c_str(), ios::in | ios::binary);

  if(dataFile.good() && !dataFile.eof())
	{
	  // Asetaan poikkeutusk‰sittely
	  // t‰nne voisi yhdist‰‰ testin yll‰ (ifit pois)
	  try
		{
		  dataFile >> *theQD;
		}
	  catch(char * msg)          //virheen sattuessa jatketaan t‰st‰ suoritusta
		{
		  if(itsLogFile)
			*itsLogFile << "  " << msg << endl;

		  dataFile.close();
		  dataFile.clear();
		  return false;
		}

	  dataFile.close();
	  dataFile.clear();
	}
  else
	{
	  return false;
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

NFmiQueryData * NFmiPressProduct::FirstData(void)
{
  NFmiNamedQueryData * nData;
  NFmiVoidPtrIterator iter(itsDatas);
  iter.Reset();
  nData = static_cast<NFmiNamedQueryData *>(iter.Next());
  if(nData)
	return nData->GetData();
  else
	return 0;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiPressProduct::FirstDataName(void)
{
  NFmiNamedQueryData * nData;
  NFmiVoidPtrIterator iter(itsDatas);
  iter.Reset();
  nData = static_cast<NFmiNamedQueryData *>(iter.Next());
  if(nData)
	return nData->GetName();
  else
	return NFmiString();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param givenName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiQueryData * NFmiPressProduct::DataByName(NFmiString givenName)
{
  NFmiNamedQueryData * nData;
  NFmiVoidPtrIterator iter(itsDatas);
  NFmiString name;
  iter.Reset();
  nData = static_cast<NFmiNamedQueryData *>(iter.Next());
  while(nData)
	{
	  name = nData->GetName();
	  if (name == givenName)
		return nData->GetData();
	  nData = static_cast<NFmiNamedQueryData *>(iter.Next());
	}
  return 0;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::ReadDescription(NFmiString & retString)
{
  NFmiString helpString;
  NFmiValueString valueString;
  itsScale.SetStartScales(NFmiRect(NFmiPoint(0,0), NFmiPoint(614,794)));
  itsPlottingRect.Set(NFmiPoint(0, 0), NFmiPoint(614,794));
  itsMargin = 0;
  itsMapGap = 0;
  itsNumberOfMaps = 1;
  fMapIsEps = true;
  fTimeStamp = false;
  fDataTimeStamp = false;
  bool allDataFilesCritical = false;
  itsLanguage = kFinnish;
  double xmin,xmax,ymin,ymax;
  long long1, long2, long3;

  long firstDeltaDays = 1 + itsEnvironment.GetDayAdvance();
  long origFirstDeltaDays = firstDeltaDays;
  short firstPlotHours = 12;
  short firstPlotMinutes = 0;
  itsFirstPlotTime.ChangeByDays(firstDeltaDays);
  itsFirstPlotTime.SetTime(firstPlotHours);
  short hourStep = 24;

  unsigned long producer = 230;

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);
  if(!(itsString == NFmiString("#!PressProduct")))
	{
	  *itsLogFile << "M‰‰rittelytiedostoa ei ole tai ei ole lehtituote" << endl;
	  cin >> hourStep; //pys‰hdys
	  return false;
	}

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);

  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  *itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty p‰‰tasolla" << endl;
		  retString = itsString;
		  return false;
		}
	  itsLoopNum++;  // jotta virhe ei aiheuttaisi ikuista luuppia

	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  switch(itsIntObject)
		{
		case dOther:	  //ylim‰‰r‰ist‰ roinaa, END lopettaa
		  {
			*itsLogFile << "*** ERROR: Tuntematon sana p‰‰tasolla: " << static_cast<char *>(itsObject) << endl;

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
		case dNumberAddingToName:
		  {
			if(SetOne(long1))
			  ActivateNumberToName(long1);

			break;
		  }
		case dOutputMode:
		  {
			if (!ReadEqualChar())
			  break;

			*itsDescriptionFile >> itsObject;
			helpString = itsObject;
			helpString.LowerCase();
			if (helpString == NFmiString ("postscript") ||
				helpString == NFmiString ("ps") ||
				helpString == NFmiString ("eps"))
			  itsOutputMode = kPostScript;
			else if (helpString == NFmiString ("meta") ||
					 helpString == NFmiString ("metakieli") ||
					 helpString.GetChars(1,5) == NFmiString ("magic"))
			  itsOutputMode = kMetaLanguage;
			else if (helpString == NFmiString ("teksti"))
			  itsOutputMode = kPlainText;
			else if (helpString == NFmiString ("xml"))
			  itsOutputMode = kXml;
			else
			  {
				if (itsLogFile)
				  *itsLogFile << "*** ERROR: Tuntematon tulostusmuoto: "
							  << static_cast<char *>(helpString)
							  << endl;
			  }

			ReadNext();
			break;
		  }
		case dMagicSavePath:
		  {
			if (!ReadEqualChar())
			  break;

			itsMagicSavePath = ReadString();

			ReadNext();
			break;
		  }
		case dProduct:
		  {
			if (!ReadEqualChar())
			  break;

			itsOutFileName = ReadString();

			ReadNext();
			break;
		  }
		case dProductWithTimeStamp:
		  {
			if (!ReadEqualChar())
			  break;

			itsOutFileName = ReadString();

			fTimeStamp = true;

			ReadNext();
			break;
		  }
		case dProductWithDataTimeStamp:
		  {
			if (!ReadEqualChar())
			  break;

			itsOutFileName = ReadString();

			fDataTimeStamp = true;

			ReadNext();
			break;
		  }
		case dProductNameFormat:
		  {
			if (!ReadEqualChar())
			  break;

			itsProductNameFormat = ReadString();

			ReadNext();
			break;
		  }
		case dMapFile:
		  {
			if (!ReadEqualChar())
			  break;

			NFmiString * epsName;

			epsName = new NFmiString(ReadString());

			itsEpsFileNames.Add(epsName);

			ReadNext();
			break;
		  }
		case dPalette:
		  {
			if (!ReadEqualChar())
			  break;

			itsPaletteName = ReadString();

			ReadNext();
			break;
		  }
		case dOutDir:
		  {
			if (!ReadEqualChar())
			  break;

			itsOutDir = ReadString();

			ReadNext();
			break;
		  }
		case dMaskFile:
		  {
			if (!ReadEqualChar())
			  break;

			itsMaskFileName = ReadString();
			itsMaskFileName.LowerCase();

			NFmiQueryData * newData = new NFmiQueryData();
			NFmiNamedQueryData * newNData = new NFmiNamedQueryData(newData,itsMaskFileName);
			itsDatas.Add(newNData);
			ReadNext();
			break;
		  }
		case dDataFile:
		  {
			if (!ReadEqualChar())
			  break;

			itsDataFileName = ReadString();
			itsDataFileName.LowerCase();

			if(fDataRead)
			  {
				*itsLogFile << "*** ERROR: Data "
							<< static_cast<char *>(itsDataFileName)
							<< "pit‰‰ antaa ennen 'K‰yt‰DatanAlkuAikaa' ja 'SuhteellinenTuntiDatasta'"
							<< endl;
				*itsLogFile << "            " << "sek‰ kaikkia j‰senelementtej‰" << endl;
				*itsLogFile << "       KESKEYTETƒƒN" << endl;
				return false;
			  }

			NFmiQueryData * newData = new NFmiQueryData();
			NFmiNamedQueryData * newNData = new NFmiNamedQueryData(newData,itsDataFileName, allDataFilesCritical);
			itsDatas.Add(newNData);

			ReadNext();
			break;
		  }
		case dMandatoryDataFile:
		  {
			if (!ReadEqualChar())
			  break;

			itsDataFileName = ReadString();
			itsDataFileName.LowerCase();

			if(fDataRead)
			  {
				*itsLogFile << "*** ERROR: Data "
							<< static_cast<char *>(itsDataFileName)
							<< "pit‰‰ antaa ennen 'K‰yt‰DatanAlkuAikaa' ja 'SuhteellinenTuntiDatasta'"
							<< endl;
				*itsLogFile << "            " << "sek‰ kaikkia j‰senelementtej‰" << endl;
				*itsLogFile << "       KESKEYTETƒƒN" << endl;
				return false;
			  }

			NFmiQueryData * newData = new NFmiQueryData();
			NFmiNamedQueryData * newNData = new NFmiNamedQueryData(newData,itsDataFileName, true);
			itsDatas.Add(newNData);

			ReadNext();
			break;
		  }
		case dChangeScandinavian:
		  {
			fChangeScandinavian = true;

			ReadNext();
			break;
		  }
		case dMapIsReduced:
		  {
			fMapIsEps = false;

			ReadNext();
			break;
		  }

		case dPageSize:    // Pysty/Vaaka nimitys
		  {
			if (!ReadEqualChar())
			  break;

			*itsDescriptionFile >> itsObject;
			helpString = itsObject;
			if (helpString == NFmiString ("A4") ||
				helpString == NFmiString ("a4") ||
				helpString == NFmiString ("A4Pysty"))
			  itsPageSize = kA4;
			else if (helpString == NFmiString ("A4Maisema") ||
					 helpString == NFmiString ("a4maisema") ||
					 helpString == NFmiString ("A4Vaaka"))
			  itsPageSize = kA4Maisema;
			else if (helpString == NFmiString ("A3Maisema") ||
					 helpString == NFmiString ("a3maisema") ||
					 helpString == NFmiString ("A3Vaaka"))
			  itsPageSize = kA3Maisema;
			else if (helpString == NFmiString ("A3") ||
					 helpString == NFmiString ("a3") ||
					 helpString == NFmiString ("A3Pysty"))
			  itsPageSize = kA3; //HUOM toistaiseksi
			else if (helpString == NFmiString ("Letter") ||
					 helpString == NFmiString ("letter"))
			  itsPageSize = kLetter;
			else
			  *itsLogFile << "*** ERROR: Tuntematon arkkikoko: "
						  << static_cast<char *>(helpString)
						  << endl;

			ReadNext();
			break;
		  }
		case dSubViews:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadLong(long1))
			  itsNumberOfMaps = static_cast<unsigned short>(long1);

			ReadNext();
			break;
		  }
		case dMargin:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadDouble(xmin))
			  itsMargin = xmin;

			ReadNext();
			break;
		  }
		case dGap:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadDouble(xmin))
			  itsMapGap = xmin;

			ReadNext();
			break;
		  }
		case dProductNameTimeFormat:
		  {
			if (!ReadEqualChar())
			  break;

			itsNameTimeFormat = ReadTimeFormat();
			ReadNext();
			break;
		  }
		case dSecondProductNameTimeFormat:
		  {
			if (!ReadEqualChar())
			  break;

			itsSecondNameTimeFormat = ReadTimeFormat(true);
			ReadNext();
			break;
		  }
		case dDate:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadLong(long1))
			  {
				if(ReadLong(long2))
				  {
					if(ReadLong(long3))
					  {    //menis ehk‰ suoremmin mutta t‰m‰ on yhteensopivaa vanhan koodin kanssa
						NFmiTime tim = NFmiTime(static_cast<short>(long3),
												static_cast<short>(long2),
												static_cast<short>(long1));
						NFmiTime thisDay;
						firstDeltaDays = tim.DifferenceInDays(thisDay);
        				itsFirstPlotTime.ChangeByDays(firstDeltaDays-origFirstDeltaDays);
					  }
				  }
			  }

			ReadNext();
			break;
		  }
		case dDataTime:
		  {
			if(!ReadData())
				return false;

			NFmiQueryData * firstData = FirstData();
			if(firstData)
			  {
				firstData->Reset();
				firstData->NextTime();
				itsFirstPlotTime = firstData->Time();
			  }
			else
			  {
				*itsLogFile << "*** ERROR: Pit‰‰ antaa datat ennen 'K‰yt‰DatanAlkuAikaa'"
							<< endl;
				*itsLogFile << "       KESKEYTETƒƒN"  << endl;
				return false;
			  }

			ReadNext();
			break;
		  }
		case dRelHour:
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
		case dRelativeDataTime:
		  {
			SetOne (long1);

			if(!ReadData())
				return false;

			NFmiQueryData * firstData = FirstData();
			if(firstData)
			  {
				firstData->Reset();
				firstData->NextTime();
				itsFirstPlotTime = firstData->Time();
				itsFirstPlotTime.ChangeByHours(long1);
			  }
			else
			  {
				*itsLogFile << "*** ERROR: Pit‰‰ antaa datat ennen 'SuhteellinenTuntiDatasta'"
							<< endl;
				*itsLogFile << "       KESKEYTETƒƒN"  << endl;
				return false;
			  }

			break;
		  }
		case dRelDay:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadLong(long1))
			  {
				firstDeltaDays = static_cast<short>(long1 + itsEnvironment.GetDayAdvance());
				itsFirstPlotTime.ChangeByDays(JustifyByLeaps(firstDeltaDays)-origFirstDeltaDays);
			  }

			ReadNext();
			break;
		  }

		case dHour:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadLong(long1))
			  firstPlotHours = static_cast<unsigned short>(long1);
            itsFirstPlotTime.SetTime(firstPlotHours);

			ReadNext();
			break;
		  }
		case dHourStep:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadLong(long1))
			  hourStep = static_cast<unsigned short>(long1);

			ReadNext();
			break;
		  }
		case dMapDefSize:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: mitta-alueen min == max"  << endl;
				else
				  itsScale.SetStartScales(NFmiRect(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax)));
			  }
			ReadNext();
			break;
		  }
		case dPlottingView:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: sijoitusalueen min == max"  << endl;
				else
				  itsPlottingRect.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
			  }
			ReadNext();
			break;
		  }
		case dProductSizeFactor:
		  {
			if (!ReadEqualChar())
			  break;

			if (ReadOne(xmin))
			  {
		        itsScale.SetStartScales(NFmiRect(NFmiPoint(0.,0.), NFmiPoint(100.,100.)));
				itsPlottingRect.Set(NFmiPoint(0.,0.), NFmiPoint(100.*xmin,100.*xmin));
			  }

			ReadNext();
			break;
		  }
		case dBoundingBorder:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: GifRajauksen min == max"  << endl;
				else
				  itsBoundingBorder.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
			  }

			ReadNext();
			break;
		  }
		case dClippingRectangle:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: rajauksen min == max"  << endl;
				else
				  itsClippingRect.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
			  }
			ReadNext();
			break;
		  }
		case dProducer:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadLong(long1))
			  producer = long1;

			ReadNext();
			break;
		  }
		case dPressParam:
		  {
			if(!ReadData())
				return false;

            itsCurrentMap = 1;
			Scale();
			NFmiPressParam * newParam = new NFmiPressParam(itsScale,
														   static_cast<short>(firstDeltaDays),
														   static_cast<short>(firstPlotHours),
														   static_cast<short>(firstPlotMinutes),
														   static_cast<short>(hourStep)    ,
														   producer,
														   this);
			newParam->SetDataName(FirstDataName());
            newParam->SetEnvironment(itsEnvironment);
			newParam->SetHome(GetHome());
			newParam->SetData(FirstData());
			newParam->SetMaskIter(itsMaskIter);
			newParam->SetLogFile(itsLogFile);
			newParam->SetDescriptionFile(itsDescriptionFile);
			newParam->SetLanguage(itsLanguage);
			newParam->SetTime(itsFirstPlotTime);
			newParam->SetNewGeoArea(itsArea);
			if(newParam->ReadDescription(itsString))
			  {
				itsParams.Add(newParam);
				fMakeElementsAfterSegments = true; // jos muita elementtej‰ m‰‰rittelyn lopussa
			  }                                      //         ne myˆs piirret‰‰n viimeisen‰
			else
			  delete newParam;

			itsIntObject = ConvertDefText(itsString);

			break;
		  }
		case dSymbolPlaces:
		  {
			if(!ReadData())
				return false;

			itsCurrentMap = 1;
			Scale();
			NFmiPressSameSymbols * newSameSymbols = new NFmiPressSameSymbols(itsScale);
			newSameSymbols->SetPressProduct(this);
            newSameSymbols->SetHome(GetHome());
			newSameSymbols->SetLogFile(itsLogFile);
			newSameSymbols->SetDescriptionFile(itsDescriptionFile);
			if(newSameSymbols->ReadDescription(itsString))
			  {
				itsSameSymbols.Add(newSameSymbols);
			  }
			else
			  delete	newSameSymbols;

			itsIntObject = ConvertDefText(itsString);

			break;
		  }
		case dImageObject:
		  {
			if(!ReadData())
				return false;

			NFmiPsSymbol * image = new NFmiPsSymbol;
			image->SetWriteLast(fMakeElementsAfterSegments);
            image->SetHome(GetHome());
		    image->SetLogFile(itsLogFile);
			image->SetDescriptionFile(itsDescriptionFile);
			if(image->ReadDescription(itsString))
			  itsObjects.Add(image);
			else
			  delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dSubImage:
		  {
			if(!ReadData())
				return false;

			NFmiPressImage * image = new NFmiPressImage;
			image->SetScale(itsScale);
		    image->SetHome(GetHome());
            image->SetLogFile(itsLogFile);
			image->SetDescriptionFile(itsDescriptionFile);
			image->SetPressProduct(this);
			if(image->ReadDescription(itsString))
			  itsObjects.Add(image);
			else
			  delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dNameDay:
		  {
			if(!ReadData())
				return false;

			if(!itsNameDay)
			  itsNameDay = new NFmiNameDay;

			NFmiPressNameDay * text = new NFmiPressNameDay;
			text->SetWriteLast(fMakeElementsAfterSegments);
            text->SetEnvironment(itsEnvironment);
			text->SetNameDay(itsNameDay);
            text->SetHome(GetHome());
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetLanguage(itsLanguage);
			text->SetTime(itsFirstPlotTime);
			if(text->ReadDescription(itsString))
				itsObjects.Add(text);
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dTextObject:
		  {
			if(!ReadData())
				return false;

			NFmiPressText * text = new NFmiPressText;
			text->SetWriteLast(fMakeElementsAfterSegments);
            text->SetEnvironment(itsEnvironment);
            text->SetHome(GetHome());
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetLanguage(itsLanguage);
			if(text->ReadDescription(itsString))
			  itsObjects.Add(text);
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dColumnTextObject: //vanhentunut
		  {
			if(!ReadData())
				return false;

			NFmiPressText * text = new NFmiPressText;
			text->SetWriteLast(fMakeElementsAfterSegments);
            text->SetEnvironment(itsEnvironment);
			text->SetScale(itsScale);
            text->SetHome(GetHome());
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetLanguage(itsLanguage);
			if(text->ReadDescription(itsString))
			  itsObjects.Add(text);
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dTimeTextObject:
		  //  tehty uudelleen NFmiPressGivenTimeTextin‰
		  // jotta riippumaton QD:sta ja sen ajoista
		  {

			if(!ReadData())
				return false;

			NFmiPressGivenTimeText * text = new NFmiPressGivenTimeText;
			text->SetWriteLast(fMakeElementsAfterSegments);
            text->SetEnvironment(itsEnvironment);
            text->SetHome(GetHome());
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetTime(itsFirstPlotTime);
			text->SetLanguage(itsLanguage);
			if(text->ReadDescription(itsString))
			  itsObjects.Add(text);
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dComputerTimeTextObject:
		  {

			if(!ReadData())
				return false;

			NFmiPressComputerTimeText * text = new NFmiPressComputerTimeText;
			text->SetWriteLast(fMakeElementsAfterSegments);
            text->SetEnvironment(itsEnvironment);
            text->SetHome(GetHome());
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile);
			text->SetLanguage(itsLanguage);
			if(text->ReadDescription(itsString))
			  itsObjects.Add(text);
			else
			  delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dMapArea:
		  {
			itsArea.SetLogFile(itsLogFile);
			itsArea.SetDescriptionFile(itsDescriptionFile);
			itsArea.ReadDescription(itsString);
			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dMakeElementsAfterSegments:
		  {
			fMakeElementsAfterSegments = true;
			ReadNext();
			break;
		  }
		case dWeekdayDir:   // direktiivitieto, ei virkaa t‰‰ll‰ mutta veisi aikaa poistaa commentstripperill‰
		  {
			helpString = ReadString();
			helpString = ReadString();
			ReadNext();
			break;
		  }
		case dAllDataFilesCritical:
		  {
			allDataFilesCritical = true;
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

  if(itsLogFile)
	{
	  *itsLogFile << "Lopetusrivi (pit‰‰ olla #LOPPU): "
				  << static_cast<char *>(itsString)
				  << endl;
	  if(itsString.GetLen() >= 2 &&
		 NFmiString(itsString.GetCharsPtr(1,2)) != NFmiString("#E") &&
		 NFmiString(itsString.GetCharsPtr(1,2)) != NFmiString("#L"))
		*itsLogFile << "*** ERROR: MƒƒRITTELYTIEDOSTON LUKEMINEN KESKEYTETTY ?" << endl;

	  *itsLogFile << "TUOTETIEDOSTO LUETTU" << endl;
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

int NFmiPressProduct:: ConvertDefText(NFmiString & object)
{
  //"Oikeat" t‰ss‰ alla
  NFmiString lowChar = object;
  lowChar.LowerCase(); // kaikille pit‰isi sallia vapaa isot/pienet kirj.

  if(lowChar==NFmiString("product") || lowChar==NFmiString("tuote"))

	return dProduct;
  else if(lowChar==NFmiString("tuoteniminumerointi"))
	return dNumberAddingToName;
  else if(lowChar==NFmiString("outputmode") ||
		  lowChar==NFmiString("tulosmuoto") ||
		  lowChar==NFmiString("tulostusmuoto"))
	return dOutputMode;
  else if(lowChar==NFmiString("#nameday") ||
		  lowChar==NFmiString("#nimip‰iv‰"))
	return dNameDay;
  else if(lowChar==NFmiString("deletescandinavianfromproductname") ||
		  lowChar==NFmiString("poistaskandittuotenimest‰") ||
		  lowChar==NFmiString("poista‰‰kkˆsettuotenimest‰"))
	return dChangeScandinavian;
  else if(lowChar==NFmiString("timestamp") ||
		  lowChar==NFmiString("tuoteaikaleimalla"))
	return dProductWithTimeStamp;
  else if(lowChar==NFmiString("datatimestamp") ||
		  lowChar==NFmiString("tuotedatanaikaleimalla"))
	return dProductWithDataTimeStamp;
  else if(lowChar==NFmiString("productnameformat") ||
		  lowChar==NFmiString("tuotenimiformaatti"))
	return dProductNameFormat;
  else if(lowChar==NFmiString("productnametimeformat") ||
		  lowChar==NFmiString("tuotenimiaikaformaatti"))
	return dProductNameTimeFormat;
  else if(lowChar==NFmiString("secondproductnametimeformat") ||
		  lowChar==NFmiString("toinentuotenimiaikaformaatti"))
	return dSecondProductNameTimeFormat;
  else if(lowChar==NFmiString("timestampformat") ||
		  lowChar==NFmiString("aikaleimaformaatti"))
	return dTimeStampFormat;
  else if(lowChar==NFmiString("date") ||
		  lowChar==NFmiString("p‰iv‰m‰‰r‰"))
	return dDate;
  else if(lowChar==NFmiString("usedatastarttime") ||
		  lowChar==NFmiString("k‰yt‰datanalkuaikaa"))
	return dDataTime;
  else if(lowChar==NFmiString("relativehoursfromdata") ||
		  lowChar==NFmiString("suhteellinentuntidatasta"))
	return dRelativeDataTime;
  else if(lowChar==NFmiString("outdir") ||
		  lowChar==NFmiString("tulostus"))
	return dOutDir;
  else if(lowChar==NFmiString("pagesize") ||
		  lowChar==NFmiString("arkki"))
	return dPageSize;
  else if(lowChar==NFmiString("palette") ||
		  lowChar==NFmiString("paletti"))
	return dPalette;
  else if(lowChar==NFmiString("mapfile") ||
		  lowChar==NFmiString("pohja"))
	return dMapFile;
  else if(lowChar==NFmiString("datafile") ||
		  lowChar==NFmiString("data"))
	return dDataFile;
  else if(lowChar==NFmiString("datafilewithtimestamp") ||
		  lowChar==NFmiString("dataaikaleimalla"))
	return dDataFileTimeStamp;
 else if(lowChar==NFmiString("mandatorydatafile") ||
		  lowChar==NFmiString("pakollinendata"))
	return dMandatoryDataFile;
  else if(lowChar==NFmiString("maskfile") ||
		  lowChar==NFmiString("maskidata"))
	return dMaskFile;

  else if(lowChar==NFmiString("#constimage") ||
		  lowChar==NFmiString("#vakiosymboli"))
	return dImageObject;
  else if(lowChar==NFmiString("#subimage") ||
		  lowChar==NFmiString("#subimages") ||
		  lowChar==NFmiString("#osakuva")  ||
		  lowChar==NFmiString("#osakuvia"))
	return dSubImage;
  else if(lowChar==NFmiString("#consttext") ||
		  lowChar==NFmiString("#vakioteksti")||
		  lowChar==NFmiString("#teksti"))
	return dTextObject;
  else if(lowChar==NFmiString("#columntext") ||
		  lowChar==NFmiString("#palstateksti"))
	return dColumnTextObject;
  else if(lowChar==NFmiString("#timetext") ||
		  lowChar==NFmiString("#aikateksti") ||
		  lowChar==NFmiString("#aika"))
	return dTimeTextObject;
  else if(lowChar==NFmiString("#computertime") ||
		  lowChar==NFmiString("#koneenaika") ||
		  lowChar==NFmiString("#tekoaika"))
	return dComputerTimeTextObject;
  else if(lowChar==NFmiString("plottingview") ||
		  lowChar==NFmiString("sijoitusalue"))
	return dPlottingView;
  else if(lowChar==NFmiString("mapdefsize") ||
		  lowChar==NFmiString("mittaalue"))
	return dMapDefSize;
  else if(lowChar==NFmiString("sizefactor") ||
		  lowChar==NFmiString("kokokerroin"))
	return dProductSizeFactor;
  else if(lowChar==NFmiString("clippingrectangle") ||
		  lowChar==NFmiString("rajaus") ||
		  lowChar==NFmiString("leikkausalue"))
	return dClippingRectangle;
  else if(lowChar==NFmiString("boundingborder") ||
		  lowChar==NFmiString("gifrajaus") ||
		  lowChar==NFmiString("rajaus"))
	return dBoundingBorder;
  else if(lowChar==NFmiString("mapisreduced") ||
		  lowChar==NFmiString("pohjaonriisuttu") ||
		  lowChar==NFmiString("pohjaonps"))
	return dMapIsReduced;
  else if(lowChar==NFmiString("subviews") ||
		  lowChar==NFmiString("lukum‰‰r‰"))
	return dSubViews;
  else if(lowChar==NFmiString("margin") ||
		  lowChar==NFmiString("reunus"))
	return dMargin;
  else if(lowChar==NFmiString("gap") ||
		  lowChar==NFmiString("v‰li"))
	return dGap;
  else if(lowChar==NFmiString("#pressparam") ||
		  lowChar==NFmiString("#segmentti") ||
		  lowChar==NFmiString("#parametriasemat")) // takaisin, oli vanhoissa reittis‰iss‰
	return dPressParam;
  else if(lowChar==NFmiString("#constplaces") ||
		  lowChar==NFmiString("#vakiopaikat"))
	return dSymbolPlaces;
  else if(lowChar==NFmiString("magicsavepath") ||
		  lowChar==NFmiString("metapolku") ||
		  lowChar==NFmiString("magicpolku"))
	return dMagicSavePath;
  else if(lowChar==NFmiString("#map") ||
		  lowChar==NFmiString("#kartta"))
	return dMapArea;
  else if(lowChar==NFmiString("makeelementsaftersegments") ||
		  lowChar==NFmiString("teeelementitsegmenttienj‰lkeen"))
	return dMakeElementsAfterSegments;
  else if(lowChar==NFmiString("#weekday") ||
		  lowChar==NFmiString("#viikonp‰iv‰"))
	return dWeekdayDir;
  else if(lowChar==NFmiString("alldatafilescritical") ||
		  lowChar==NFmiString("pakollisetdatatiedostot"))
	return dAllDataFilesCritical;
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

void NFmiPressProduct::ReadPalette(void)
{
  itsPalette = new NFmiCmykColorBag;
  NFmiNamedCmykColor color;
  char object[255];
  NFmiString name;

  NFmiString palFileName = NFmiString("..");
  palFileName += kFmiDirectorySeparator;
  palFileName += "LyhytPaletit";
  palFileName += kFmiDirectorySeparator;

  palFileName += itsPaletteName;
  palFileName += NFmiString(".pal");
  ifstream palFile(palFileName, ios::in|ios::in);

  while(!palFile.eof())
	{
	  color.color.c = -10.;  //varmistaa tiedoston loppumisen
	  palFile >> color.color.c;
	  palFile >> color.color.m;
	  palFile >> color.color.y;
	  palFile >> color.color.k;
	  palFile >> object;
	  color.name = object;
	  if (color.color.c > -1.)
		itsPalette->AddColor(color);
	}
  palFile.close();
  palFile.clear();

  return ;//palette;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theGivenOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::WritePS(FmiPressOutputMode theGivenOutput)
{
  //kutsutaan aina vaikka harvoin tarvitaan
  srand(static_cast<unsigned int>(time(NULL)));
  
  itsNumOfWritePS++;
  FmiPressOutputMode output = theGivenOutput;
  if(itsOutputMode != kPostScript)
	output = itsOutputMode;

  if(itsCurrentDataIter)
	delete itsCurrentDataIter;
  if(FirstData())
	itsCurrentDataIter = new NFmiSuperSmartInfo(FirstData());
  else
	itsCurrentDataIter = 0;

  NFmiString endFileName;	// = NFmiString("..\\Muut\\");
  endFileName = GetHome();
  endFileName += kFmiDirectorySeparator;
  endFileName += NFmiString("Muut");
  endFileName += kFmiDirectorySeparator;

  endFileName += NFmiString("endA4.eps");
  ifstream endFile(endFileName, ios::in|ios::in);

  NFmiString startFileName;

  startFileName = GetHome();

  startFileName += kFmiDirectorySeparator;
  startFileName += NFmiString("Muut");
  startFileName += kFmiDirectorySeparator;

  if(itsPageSize == kA3Maisema)
	startFileName += NFmiString("startEpsA3Maisema.ps");
  else if(itsPageSize == kA3)
	startFileName += NFmiString("startEpsA3Potretti.ps");
  else if(itsPageSize == kLetter)
	startFileName += NFmiString("startEpsLetter.ps");
  else if(itsPageSize == kA4Maisema)
	startFileName += NFmiString("startEpsA4Maisema.ps");
  else //A4Pysty
	startFileName += NFmiString("startEpsA4Potretti.ps");

  //miksi kaksi pistett‰ kun raahataan ikonin p‰‰lle
  if(!ConstructOutFileName())
	return false;

  ifstream startFile(startFileName, ios::in|ios::in);

  if(output==kPostScript || itsNumOfWritePS < 2)
	{
	  if(output==kPostScript)
		{
		  itsOutFile += NFmiString(".eps");
		  outFile.open(itsOutFile, ios::out | ios::binary);
		}
	  else if(output==kMetaLanguage)
		{
		  itsOutFile += NFmiString(".magic");
		  outFile.open(itsOutFile, ios::out | ios::binary);
		}
	  else if(output==kPlainText)
		{
		  itsOutFile += NFmiString(".txt");
		  outFile.open(itsOutFile, ios::out);
		}
	  else
		{
		  itsOutFile += NFmiString(".xml");
		  outFile.open(itsOutFile, ios::out | ios::binary);
		}

	  if(itsLogFile)
		*itsLogFile << "Output: " << static_cast<char *>(itsOutFile) << endl;

	}

  if(!startFile)
	{
	  if(itsLogFile)
		*itsLogFile << "*** ERROR: StartPs-tiedostossa vikaa: "
					<< static_cast<char *>(startFileName)
					<< endl;
	  return false;
	}

  if(!endFile)
	{
	  if(itsLogFile)
		*itsLogFile << "*** ERROR: LoppuPs-tiedostossa vikaa: "
					<< static_cast<char *>(endFileName)
					<< endl;
	  return false;
	}

  if(!outFile)
	{
	  if(itsLogFile)
		*itsLogFile << "*** ERROR: Output-tiedostossa vikaa: "
					<< static_cast<char *>(itsOutFile)
					<< endl;
	  return false;
	}

  if(output==kPostScript)
	{
	  if (itsBoundingBorder.IsEmpty())
		NFmiCopyFile(startFile, outFile);
	  else
		NFmiCopyFileCropping(startFile, outFile, itsBoundingBorder);
	}
  else if(output==kXml)
	{

	  //t‰st‰ se l‰htee
	  outFile << "$<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standlone=\"yes\"?>" << endl;

	  outFile << "<Name>" << static_cast<char *>(itsOutFile.Header()) << endl;
	}
  else if(output==kPlainText)
	{
	}
  else if(output==kMetaLanguage)
	{
	  WriteMetaInit();
	}

  startFile.close();
  startFile.clear();

  NFmiString mapFilePath;

  mapFilePath = GetHome();
  mapFilePath += kFmiDirectorySeparator;
  mapFilePath += NFmiString("Pohjat");
  mapFilePath += kFmiDirectorySeparator;

  ifstream mapFile;

  itsCurrentMap=1;
  if(itsCurrentDataIter)
	itsCurrentDataIter->First();
  Scale();

  while(itsCurrentMap <= itsNumberOfMaps)//itsNumberOfMaps=koko tuote monistettu n kpl
	{
	  // luuppi, voi olla useampia EPSeja
	  NFmiVoidPtrIterator epsIter(itsEpsFileNames);
	  epsIter.Reset();
	  NFmiString * epsFileName;
	  epsFileName = static_cast<NFmiString *>(epsIter.Next());
	  short numOfEps = 0;

	  // *************************************
	  //             POHJAT
	  // *************************************
	  while (epsFileName)
		{
		  numOfEps++;
		  if(output==kMetaLanguage)
			{
			  if(itsNumOfWritePS < 2)
				{
				  // htv pit‰‰ parametrisoida
				  NFmiString fileString("/var/www/share/pohjat/htv");

				  outFile << endl << "SymbolPath " << static_cast<char *>(fileString) << endl;

				  NFmiString epsFile = *epsFileName;
				  epsFile.Add(NFmiString(".jpg"));
				  outFile << "Symbol "  << static_cast<char *>(epsFile);
				}
			}
		  else if(output==kXml)
			{
			  outFile << "<Background>"  << endl;
			  outFile << "  <Set>Pohjat"  << endl;
			  outFile << "  <File>" << static_cast<char *>(*epsFileName)  << endl;
			}
		  else
			{

			  NFmiString fullEpsFileName = mapFilePath;
			  fullEpsFileName += *epsFileName;
			  fullEpsFileName += NFmiString(".eps");
			  mapFile.open(fullEpsFileName, ios::in|ios::in);
			  if(!mapFile.good() || mapFile.eof())
				{
				  if(itsLogFile)
					*itsLogFile << "WARNING: Pohjaa ei ole: "
								<< static_cast<char *>(fullEpsFileName)
								<< endl;
				}
			  else
				{
				  if(fMapIsEps)
					{
					  if (itsClippingRect.IsEmpty())
						NFmiWriteEPSConcat(itsScale, outFile);
					  else
						NFmiWriteEPSConcatClipping(itsScale, outFile, itsClippingRect);

					  if (itsBoundingBorder.IsEmpty())
						if(itsPalette)
						  NFmiCopyFileColoring(mapFile, outFile, itsPalette);
						else
						  NFmiCopyFileWithoutShowpage(mapFile, outFile);   // writeps:n

					  else  //Epsiin voi laittaa nolla-BoundaryBording koska alussa on oikea ja maximi otetaan
						if(itsPalette)   // ei k‰ytˆss‰
						  NFmiCopyFileCroppingAndColoring(mapFile,
														  outFile,
														  NFmiRect(NFmiPoint(0.,0.),
																   NFmiPoint(0.,0.)),
														  itsPalette);
						else
						  NFmiCopyFileCropping(mapFile,
											   outFile,
											   NFmiRect(NFmiPoint(0.,0.),NFmiPoint(0.,0.)));

					  NFmiWriteEPSEnd(outFile);
					}
				  else
					{
					  NFmiWritePSConcat(itsScale, outFile);
					  NFmiCopyFileWithoutShowpage(mapFile, outFile);
					  NFmiWritePSEnd(outFile);
					}
				  if(itsLogFile)
					*itsLogFile << "Pohja kopioitu: "
								<< static_cast<char *>(fullEpsFileName)
								<< endl;
				}
			}
		  epsFileName = static_cast<NFmiString *>(epsIter.Next());
		  mapFile.close();
		  mapFile.clear();
		}

	  if(output==kMetaLanguage && itsNumOfWritePS < 2)
		{

		  if(output==kMetaLanguage && numOfEps < 1)
			{
			  outFile << endl
					  << "New "
					  << itsBoundingBorder.Size().X()
					  << " "
					  << itsBoundingBorder.Size().Y()
					  << " Transparent" << endl;
			}

		  outFile << endl << "Store pohja" << endl << endl;
		}

	  // **************************************
	  // norm. objektit (myˆs osakuvat) jotka ennen segmenttej‰
	  // **************************************
	  if(!WriteScalingObjects(true, output))
		return false;

	  // **************************************
	  // vakiopaikat, jotka ennen segmenttej‰
	  // **************************************
	  if(!WriteSameSymbols(true, output))
		return false;

//	  if(itsCurrentDataIter) // kaatuu ilman dataa; 2.03 ei en‰‰ kaadukaan
		{
		  // ****************************************
		  //               SEGMENTIT
		  // ****************************************
		  NFmiVoidPtrIterator paramIter(itsParams);
		  NFmiPressParam * param;
		  paramIter.Reset();
		  param = static_cast<NFmiPressParam *>(paramIter.Next());
		  short segNum = 1;
		  while (param)
			{		
				if(param->IsActive())
				{
				  if(itsLogFile)
					*itsLogFile << segNum << ". segmentti ("
								<< static_cast<char *>(param->MakeLogComment())
								<< ")" << endl;
				  if (!(param->WritePS(itsScale, outFile, output)))
					{
					  outFile.close();
					  outFile.clear();
					  mapFile.close();
					  mapFile.clear();
					  if(itsLogFile)
						*itsLogFile << "*** ERROR: param->WritePS() in NFmiPressProduct" << endl;
					  return false;
					}
				}
			    param = static_cast<NFmiPressParam *>(paramIter.Next());
			    segNum++;
			}
		}
//	  else  // tarvittaisko segmentti‰ ilman dataa
//		{     // jokatapauksessa kunnon varoitus
//		  *itsLogFile << "WARNING: Dataa ei lˆydy segmenteille" << endl;
//		}

	  // **********************************************
	  // vakiopaikat jotka segmenttien j‰lkeen
	  // **********************************************

	  if(!WriteSameSymbols(false, output))
		return false;

	  // *******************************************************
	  // norm.objektit(myˆs osakuvat), jotka j‰lkeen segmenttej‰
	  // *******************************************************
	  if(!WriteScalingObjects(false, output)) //toiseen kertaan t‰nne segmenttien j‰lkeen
		return false;

	  StepMap();
	  mapFile.close();
	  mapFile.clear();
	}
  if(output == kPostScript)
	{
	  if (!itsBoundingBorder.IsEmpty())
		NFmiWriteBoundingBox(outFile, itsBoundingBorder);
	  NFmiCopyFile(endFile, outFile);
	  outFile.close();
	  outFile.clear();
	}
  else if(output == kPlainText)
	{
	  ConstructOutFileName();
	}
  else if(output == kMetaLanguage)
	{
	  ConstructOutFileName();
	  outFile << endl
			  << "Save "
			  << static_cast<char *>(itsOutFile.Header())
			  << ".gif"
			  << endl;
	}

  NFmiTime tim;

  if(UseFromStorage(1,false) != kFloatMissing)
	*itsLogFile << "WARNING: Unused values in storage queue 1"  << endl;
  if(UseFromStorage(2,false) != kFloatMissing)
	*itsLogFile << "WARNING: Unused values in storage queue 2"  << endl;

  if(itsLogFile)
	*itsLogFile << "** " << static_cast<char *>(itsOutFileName) << " tehty "  << tim << endl;

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressProduct::WriteMetaInit(void)
{

  if(itsNumOfWritePS < 2)
	{

	  //t‰st‰ se l‰htee
	  outFile << "# Lehtis‰‰ohjelmalla tehty‰ ImageMagick-ohjauskielt‰" << endl;
	  NFmiTime tim;
	  outFile << "# " << tim << endl << endl;

	  outFile << "Define TuuliNuoli" << endl;
	  outFile << "Draw primitive=>polygon points=>\"7,7 0,20 -7,7\" stroke=>black fill=>$fill linewidth=>1.5 translate=>\"$x,$y\" rotate=>$rotate"  << endl;
	  outFile << "Draw primitive=>circle points=>\"0,0 8,0\" stroke=>black fill=>$fill linewidth=>1.5 translate=>\"$x,$y\""  << endl;
	  outFile << "End" << endl << endl;

	  outFile << "Define Tuuli"<< endl;
	  outFile << "TuuliNuoli -x $x -y $y -rotate $angle -fill white" << endl;
	  outFile << "End" << endl<< endl;

	  outFile << "Define Huomautus"<< endl;
	  outFile << "TuuliNuoli -x $x -y $y -rotate $angle -fill yellow" << endl;
	  outFile << "End" << endl<< endl;

	  outFile << "Define KovaTuuli"<< endl;
	  outFile << "TuuliNuoli -x $x -y $y -rotate $angle -fill \"#FFB500\"" << endl;
	  outFile << "End" << endl<< endl;

	  outFile << "Define Myrsky"<< endl;
	  outFile << "TuuliNuoli -x $x -y $y -rotate $angle -fill red" << endl;
	  outFile << "End" << endl<< endl;


	  outFile << "FontType X" << endl;
	  outFile << "FlipY on" << endl;

	  if (itsMagicSavePath.IsValue())
		outFile << "SavePath /var/www/share/saakartat/"
				<< static_cast<char *>(itsMagicSavePath) << endl << endl;
	  else
		*itsLogFile << "magicPolku puuttuu"  << endl;

	  outFile << "Compose Over" << endl;
	  outFile << "Align Center" << endl;
	  outFile << "TextAlign South" << endl;
	}
  else
	{
	  outFile << endl <<"# *** UUSI KUVA *** " << endl << endl;
	  outFile <<"Restore pohja" << endl << endl;
	}

  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDoPreSegments Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

 //otherwise postSegments
bool NFmiPressProduct::WriteSameSymbols(bool theDoPreSegments, FmiPressOutputMode theOutput)
{
  //kutsutaan kahteen kertaan: ennen ja j‰lkeen segmenttien

  NFmiVoidPtrIterator constIter(itsSameSymbols);
  NFmiPressSameSymbols * sameSymbols;
  constIter.Reset();
  sameSymbols = static_cast<NFmiPressSameSymbols *>(constIter.Next());
  while (sameSymbols)
	{
	  if(sameSymbols->GetPsSymbol()->IsToWriteLast() != theDoPreSegments)
		{

		  sameSymbols->SetScale(itsScale);
		  if (!(sameSymbols->WritePS(outFile)))
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: sameSymbols->WritePS() in NFmiPressProduct" << endl;
			  outFile.close();
			  outFile.clear();
			  return false;
			}
		}
	  sameSymbols = static_cast<NFmiPressSameSymbols *>(constIter.Next());
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDoPreSegments Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

//otherwise postSegments
bool NFmiPressProduct::WriteScalingObjects(bool theDoPreSegments, FmiPressOutputMode theOutput)
{
  //kutsutaan kahteen kertaan: ennen ja j‰lkeen segmenttien

  NFmiVoidPtrIterator objectIter(itsObjects);
  NFmiPressScaling * object;

  NFmiPressScaling * lastObject = 0;
  double lastLineStep =0.;
  double lineStep;
  objectIter.Reset();
  object = static_cast<NFmiPressScaling *>(objectIter.Next());
  while (object)
	{
	  object->Set(itsScale, outFile);
	  object->SetLastLineStep(lastLineStep);

	  // managerista hallittavissa
	  if(object->IsActive() && object->IsToWriteLast() != theDoPreSegments)
		{
		  if (!(object->WritePS(theOutput)))
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: (timeDep)object->WritePS() in NFmiPressProduct" << endl;
			  return false;
			}
		}
	  lineStep = object->GetLineStep();
	  lastObject = object;
	  object = static_cast<NFmiPressScaling *>(objectIter.Next());
	  lastLineStep = lineStep;

	  if((!object || !(object->IsInParagraph())) && itsOutputMode == kPostScript)
		lastObject->WriteGRestore();
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

bool NFmiPressProduct::Close(void)
{
  if(itsLogFile)
	{
	  NFmiFileString inFile(itsInFileName);
	  NFmiString str(inFile.Header());
	  *itsLogFile << "** Loki suljettu: "  << static_cast<char *>(str) << endl;
	  itsLogFile->close();
	  itsLogFile->clear();
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

bool NFmiPressProduct::ConstructOutFileName(void)
{
  NFmiString str;

#ifndef UNIX
  char * hChar;
  hChar = getenv("lehtiOutDir");
  if(hChar == 0)
	{
	  str = GetHome();

	  str += kFmiDirectorySeparator;
	  str += "Valmiit";
	}
  else
	str = hChar;
#else
  str = NFmiSettings::Require<string>("press::outpath");
#endif

  itsOutFile = str;
  itsOutFile += NFmiString("/");

  NFmiString name;

  NFmiString timeFormat = itsStringNameTimeFormat;
  if(!itsProductNameFormat.IsValue())
	{
	  itsOutFile += itsOutFileName;

	  if(fTimeStamp)
		{
		  NFmiMetTime computerTime = NFmiMetTime(1);
		  NFmiMetTime localTime(computerTime.LocalTime(25.f),1);
		  NFmiPressTime pressLocalTime(localTime); //jotta oikea ToStr
		  NFmiString timeStampT;
		  if(timeFormat.IsValue())
			timeStampT = pressLocalTime.InterpretToStr(timeFormat);
		  else if(itsNameTimeFormat == kUndefined)
			{
			  timeStampT = localTime.ToStr(kYYYYMMDDHH);//yhteensopivuus meneeseen
			  timeStampT += NFmiString("00");
			}
		  else
			timeStampT = pressLocalTime.ToStr(itsNameTimeFormat);

		  itsOutFile += timeStampT;

		  timeFormat = itsSecondStringNameTimeFormat;
		}
	  else if(fDataTimeStamp)
		{
		  NFmiString timeStampT;
		  NFmiPressTime pressTime(itsFirstPlotTime.LocalTime(25.));
		  if(timeFormat.IsValue())
			timeStampT = timeStampT = pressTime.InterpretToStr(timeFormat);
		  else if(itsNameTimeFormat == kUndefined)
			timeStampT = NFmiMetTime(itsFirstPlotTime.LocalTime(25.),1).ToStr(kYYYYMMDDHH);//yhteensopivuus meneeseen
		  else
			timeStampT = pressTime.ToStr(itsNameTimeFormat);

		  timeStampT += NFmiString("00"); //Minuutit !!??
		  itsOutFile += timeStampT;
		  timeFormat = itsSecondStringNameTimeFormat;
		}
	}
  else
	{
	  NFmiHyphenationString subString, smallString;
	  itsProductNameFormat.ResetPosition();
	  while (itsProductNameFormat.NextSubString(NFmiString("/"),subString))
		{
		  NFmiHyphenationString lowerString(subString);
		  lowerString.LowerCase(); //kelpaa isot/pienet kirjaimet
		  NFmiString addString;
		  if(lowerString == NFmiString("asema"))
			{
			  addString = FirstSegmentName();

			  if(subString.FirstCharIsUpper())
				addString.FirstCharToUpper();
			  else
				addString.LowerCase();
			}
		  else if(lowerString == NFmiString("ase"))
			{
			  addString = FirstSegmentName();
			  addString = addString.GetChars(1,3);

			  if(subString.FirstCharIsUpper())
				addString.FirstCharToUpper();
			  else
				addString.LowerCase();
			}
		  else if(lowerString == NFmiString("aikaleima"))
			{
				NFmiMetTime computerTime = NFmiMetTime(1);
				NFmiMetTime localTime(computerTime.LocalTime(25.f),1);
				NFmiPressTime localPressTime(localTime);
				if(timeFormat.IsValue())    // ei testattu
				  addString = localPressTime.InterpretToStr(timeFormat);
				else
				  addString = localPressTime.ToStr(itsNameTimeFormat);

				timeFormat = itsSecondStringNameTimeFormat;

			}
		  else if(lowerString == NFmiString("aikaleimautc"))
			{
			  NFmiMetTime computerTime = NFmiMetTime(1);
			  NFmiPressTime localPressTime(computerTime);
			  if(timeFormat.IsValue())
				addString = localPressTime.InterpretToStr(timeFormat);
			  else
				addString = localPressTime.ToStr(itsNameTimeFormat);

			  timeFormat = itsSecondStringNameTimeFormat;

			}
		  else if(lowerString == NFmiString("datanaikaleima"))
			{
			  NFmiPressTime localPressTime(itsFirstPlotTime.LocalTime(25.));
			  if(timeFormat.IsValue())
				addString = localPressTime.InterpretToStr(timeFormat);
			  else
				addString = localPressTime.ToStr(itsNameTimeFormat);

			  timeFormat = itsSecondStringNameTimeFormat;

			}
		  else if(lowerString == NFmiString("datanaikaleimautc"))
			{
			  NFmiPressTime localPressTime(itsFirstPlotTime);
			  if(timeFormat.IsValue())
				addString = localPressTime.InterpretToStr(timeFormat);
			  else
				addString = localPressTime.ToStr(itsNameTimeFormat);

			  timeFormat = itsSecondStringNameTimeFormat;

			}
		  else if(lowerString == NFmiString("analyysiaika") ||
				  lowerString == NFmiString("analyysiaikautc"))
			{
			  NFmiPressTime tim = (static_cast<NFmiMetTime>(FirstData()->OriginTime()));

			  if(timeFormat.IsValue())
				addString = tim.InterpretToStr(timeFormat);
			  else
				addString = tim.ToStr(itsNameTimeFormat);

			  timeFormat = itsSecondStringNameTimeFormat;

			}
		  else if(lowerString == NFmiString("kie"))
			{

			  if(itsLanguage == kFinnish)
				addString = NFmiString("suo");
			  else if(itsLanguage == kSwedish)
				addString = NFmiString("ruo");
			  else if(itsLanguage == kEnglish)
				addString = NFmiString("eng");
			  else if(itsLanguage == kSpanish)
				addString = NFmiString("esp");
			  else if(itsLanguage == kGermany)
				addString = NFmiString("sak");
			  else if(itsLanguage == kFrench)
				addString = NFmiString("ran");
			  else if(itsLanguage == kChinese)
				addString = NFmiString("kii");
			  else if(itsLanguage == kSaame)
				addString = NFmiString("saa");

			  if(subString.FirstCharIsUpper())
				addString.FirstCharToUpper();
			  else
				addString.LowerCase();

			}
		  else if(lowerString == NFmiString("kieli"))
			{
			  if(itsLanguage == kFinnish)
				addString = NFmiString("suomi");
			  else if(itsLanguage == kSwedish)
				addString = NFmiString("ruotsi");
			  else if(itsLanguage == kEnglish)
				addString = NFmiString("englanti");
			  else if(itsLanguage == kSpanish)
				addString = NFmiString("espanja");
			  else if(itsLanguage == kGermany)
				addString = NFmiString("saksa");
			  else if(itsLanguage == kFrench)
				addString = NFmiString("ranska");
			  else if(itsLanguage == kChinese)
				addString = NFmiString("kiina");
			  else if(itsLanguage == kSaame)
				addString = NFmiString("saame");

			  if(subString.FirstCharIsUpper())
				addString.FirstCharToUpper();
			  else
				addString.LowerCase();

			}
		  else
			addString = subString;

		  name += addString;
		}
	  itsOutFile += name; // ulkoa sis‰‰n
	}

  if(itsCurrentNumberToName != kLongMissing)
	{
	  itsOutFile += NFmiValueString(itsCurrentNumberToName);
	  itsCurrentNumberToName++;
	}

  itsOutFile.NormalizeDelimiter();
  if(fChangeScandinavian)        // skandit pois jos Unixiin
	itsOutFile.ChangeScandinavian();

  if(itsOutDir.IsValue())
	{
	  itsOutFile.ReplaceDirectory(itsOutDir);
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

NFmiString NFmiPressProduct::FirstSegmentName(void)
{
  NFmiVoidPtrIterator paramIter(itsParams);
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  if(param)
	{
	  return param->GetName();
	}
  return NFmiString("Ei ole");
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

// ekan segmentin eka asema
NFmiStationPoint NFmiPressProduct::FirstParamLocation(void)
{
  NFmiVoidPtrIterator paramIter(itsParams);
  NFmiPressParam * param;
  paramIter.Reset();
  param = static_cast<NFmiPressParam *>(paramIter.Next());
  if(param)
	return param->GetFirstStationPoint();

  return NFmiStationPoint();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressProduct::Scale(void)
{
  double oneWidth = (itsPlottingRect.Width() - 2.*itsMargin - (itsNumberOfMaps-1)*itsMapGap)/itsNumberOfMaps;
  double oneHeight = itsPlottingRect.Height() - 2.*itsMargin;
  double xOffset = itsPlottingRect.Left() + itsMargin + (itsCurrentMap-1)*(itsMapGap+oneWidth);
  double yOffset = itsPlottingRect.Top() + itsMargin; //pit‰isi olla Bottom()
  itsScale.SetEndScales(NFmiRect(NFmiPoint(xOffset,yOffset),
								 NFmiPoint(xOffset+oneWidth,yOffset+oneHeight)));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressProduct::StepMap(void)
{
  itsCurrentMap++;
  Scale();
  if(itsCurrentDataIter)
	itsCurrentDataIter->NextTime();
}

// ======================================================================
