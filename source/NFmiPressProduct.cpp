//© Ilmatieteenlaitos  Lasse
//  Original 21.10. 1997
// Muutettu 261197/LW Scale() korjattu, +MapIsEps
// Muutettu 301297/LW Tuotetiedosto preprosessoidaan, jossa includet haetaan
// Muutettu 050198/LW konkatenoudut stringien += poistettu
// Muutettu 070198/LW Määrittelyt suomennettu
// Muutettu 120198/LW peritty NFmiPressDescriptistä eikä NFmiDescriptinista
//Muutettu 150198/LW ReadDouble() ym käyttö
//Muutettu 190198/LW data omasta ympäristömuuttujasta, tuote saman niminen kuin määrittelytied.
//Muutettu 210198/LW #Liitä voidaan aloittaa myöh. kuin rivin alusta
//Muutettu 290198/LW valmis tuote ympäristömuuttujan ilmoittamaan paikkaan
//Muutettu 160298/LW +itsSameSymbols
//Muutettu 100398/LW marginaali ->0, auttaako skaalavirheeseen
//Muutettu 100398/LW data-pointteri menee jo ennen readD:ia alatasoille
//Muutettu 030498/LW .pss-tiedostot temppiin
//Muutettu 090498/LW +KoneenAika
//Muutettu 110598/LW StartEpsiin LatinFontit
//Muutettu 150598/LW Boudary-boxin muutosmahdollisuus (giffit)
//Muutettu 200598/LW Query-data ei enää pakollinen (tehdään vain vakio-osia)
//Muutettu 100698/LW +Paletti
//Muutettu 180698/LW palettipiirteet jääneet virattomiksi koska nyt hoidetaan Perlillä
//Muutettu 230698/LW useampi QD
//Muutettu 180898/LW tulostiedosto .ps -> .eps
//Muutettu 210898/LW tuloshakemisto muutettavissa, esim PuoliValmiit
//Muutettu 040998/LW datanlukuun poikkeusmenettelyn mukainen lukutarkistus
//                   tiedostohierarkia automatisoitu kun ei annettu ympäristömuuttujia
//Muutettu 090998/LW ReadDescriptionin käyttö: ReadNext(),ReadEqualChar,itsObject,itsIntObject
//Muutettu 230998/LW kLehtiTuoteDir FmiPressTypeen
//Muutettu 221098/LW + piirrettävä päivämäärä
//Muutettu 181298/LW palstateksti sulautettu pressTekstiin
//Muutettu 290199/LW Merkkijonot myös lainausmerkkeihin, saadaan välilyönti
//Muutettu 240299/LW Pelkkä siirto ilman skaalausta mahdollista
//Muutettu 050399/LW Välitön keskeytys kun data ja datariippuvat ajat väärässä järjestyksessä 
//                   +aikaleima myös datasta
//Muutettu 110899/LW +itsNameToLonLat 
//Muutettu 240899/LW +datat luetaan viimeistään kun tavataan ensimmäinen jäsenelementti 
//Muutettu 260899/LW jolle fqd-tiedostoa löydy yritetään samannimistä sqd:tä
//Muutettu 251099/LW aika=suhteelliset tunnit ajohetkestä  
//Muutettu 261099/LW showpage pois pohjista ja osakuvista
//Muutettu 170100/LW +fChangeScandinavian 
//Muutettu 080300/LW +minuutit 
//Muutettu 160300/LW output-tiedoston nimeäminen ReadDescr.-puolelta writePs-puolelle 
//Muutettu 170300/LW +SetFirstStation
//Muutettu 050500/LW aikaleima kuntoon
//Muutettu 000/LW +SetFirstStationRename()  
//Muutettu 250800/LW +espanja ja saksa 
//Muutettu 301100/LW +SetFirstObjectActivity()
//Muutettu 150101/LW +SetProductNameFormat()
//Muutettu 060201/LW datatiedostoille LowerCase()
//Muutettu 310801/LW Versio 6 (std) vaatimat i/o ja muut
//                   mm streamien välitys parametrissa oliona kaataa, pitää olla viittaus
//                   määritteytiedosto nyt filenä, joka välitetään jasenolioille eikä pelkkä cin
//Muutettu 251001/LW Uusi preprosessointi direktiiveineen
//Muutettu 201101/LW VakioPaikat myös ennen segmenttejä (merisääfax)
//Muutettu 231101/LW BoundingBorder myös eps-koodin loppuun jos gifrajaus
//                   psConvertin gif-konversio ottaa sieltä viimeisen (eikä graafien suppeampaa) 
//----------------------------------------------------------------------------

#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#ifndef __NPRESSPR_H__
#include "NFmiPressProduct.h"
#endif//__NPRESSPR_H__

#ifndef __NPSYMBOL_H__
#include "NFmiPsSymbol.h"
#endif

#ifndef __NPTIMTEX_H__
#include "NFmiPressGivenTimeText.h"
#endif

#ifndef __NPTICTEX_H__
#include "NFmiPressComputerTimeText.h"
#endif

#ifndef __NPSTATEX_H__
#include "NFmiPressStationText.h"
#endif

#include "NFmiLocationFinder.h" 
#include "NFmiHyphenationString.h"
#include "NFmiPressNameDay.h" 
#include "NFmiPressArea.h"    
#include "NFmiSuperSmartInfo.h" 
//#include "NFmiFastQueryInfo.h" 
//#include "NFmiPreProcessor.h"

//#include <fstream> //STD 27.8.01  Mika poisti
//#include <iostream>  //STL 27.8.01
#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

extern bool NFmiCopyFile(ifstream&, ofstream&);
extern bool NFmiCopyFileWithoutShowpage(ifstream&, ofstream&);
extern bool NFmiCopyFileColoring(ifstream&, ofstream&, NFmiCmykColorBag*);
extern bool NFmiCopyFileCropping(ifstream&, ofstream&,NFmiRect);  //15.5
extern bool NFmiCopyFileCroppingAndColoring(ifstream&, ofstream&,NFmiRect,NFmiCmykColorBag*);//16.6
extern bool NFmiWritePSConcat(NFmiRectScale, ofstream&);
extern bool NFmiWritePSEnd(ofstream&);
extern bool NFmiWriteEPSConcat(NFmiRectScale, ofstream&);
extern bool NFmiWriteEPSConcatClipping(NFmiRectScale, ofstream&, NFmiRect);
extern bool NFmiWriteEPSEnd(ofstream&);
extern bool NFmiWriteBoundingBox(ofstream& outFile, NFmiRect theRect); //23.11.01

//----------------------------------------------------------------------------
NFmiPressProduct::NFmiPressProduct(void) 
{
/*	itsFloatQueue.push(32.);
	itsFloatQueue.push(35.);
	itsFloatQueue.push(39.);
	int testlen = itsFloatQueue.size();
	float test1 = itsFloatQueue.front();
	float test2 = itsFloatQueue.back();
	int testlen2 = itsFloatQueue.size();
	itsFloatQueue.pop();
	int testlen3 = itsFloatQueue.size();
	itsFloatQueue.pop();
	itsFloatQueue.pop();
	itsFloatQueue.pop();
	itsFloatQueue.pop();
	itsFloatQueue.pop();
	int testlen4 = itsFloatQueue.size();
	itsFloatQueue.push(39.);
	int testlen5 = itsFloatQueue.size();
*/
	itsNameTimeFormat = kUndefined; //22.3.00
	itsSecondNameTimeFormat = kUndefined; //27.10.00
	itsNameDay = 0;   //26.1.2000
	itsLogFile = new ofstream; //3.9.01
	itsCurrentDataIter = 0;
	itsNameToLonLat = new NFmiLocationFinder(); 
//	itsCurrentData = 0;
	itsLanguage = kFinnish;   
	itsPalette = 0;           
	itsPageSize = kLetter;    
	fChangeScandinavian = false;
	itsCurrentNumberToName = kLongMissing;      //1.9.00
	itsNumOfWritePS = 0;                       //24.10.00
	fMakeElementsAfterSegments = false;
	itsMaskIter = 0;                           //12.9.01
//	itsArea = 0;                               //23.12.00
}
/*
//----------------------------------------------------------------------------
NFmiPressProduct::NFmiPressProduct(NFmiQueryData* theData)  
{
	itsCurrentData = 0;
//	itsData = theData;	  //3.12
//	itsStartTime = itsData->OriginTime(); //on an.aika 
}
*/
//----------------------------------------------------------------------------
NFmiPressProduct :: ~NFmiPressProduct (void) 
{
	if(itsNameDay)
		delete itsNameDay;
	if(itsCurrentDataIter)
		delete itsCurrentDataIter; 
	if(itsMaskIter)
		delete itsMaskIter; //12.9.01
	delete itsNameToLonLat;  
	itsParams.Clear(true);
	itsSameSymbols.Clear(true); 
	itsEpsFileNames.Clear(true); 
	itsDatas.Clear(true);       
	if(itsLogFile)               
		delete itsLogFile;
	if(itsPalette)
		delete itsPalette;
	if(itsDescriptionFile)   //31.8.01            
		delete itsDescriptionFile;
}
//----------------------------------------------------------------------------
void NFmiPressProduct::PutInStorage(const float value, int queueNum) 
{
	if(queueNum == 1)
	{
		itsFloatQueue.push(value);
//		int	test = itsFloatQueue.size();
	}
	else
	{
		itsFloatQueue2.push(value);
//		int	test = itsFloatQueue2.size();
	}
}
//----------------------------------------------------------------------------
float NFmiPressProduct::UseFromStorage(int queueNum, bool errorReport) 
{
	float value;
//	int test;
	if(queueNum == 1)
	{
		if(itsFloatQueue.size() < 1)
		{
			if(errorReport)
				*itsLogFile << "  ***ERROR: Use of empty Product storage 1"  << endl;
			value = kFloatMissing;
		}
		else
		{
			value = itsFloatQueue.front();
			itsFloatQueue.pop();
	//		test = itsFloatQueue.size();
		}
	}
	else
	{
		if(itsFloatQueue2.size() < 1)
		{
			if(errorReport)
				*itsLogFile << "  ***ERROR: Use of empty Product storage 2"  << endl;
			value = kFloatMissing;
		}
		else
		{
			value = itsFloatQueue2.front();
			itsFloatQueue2.pop();
	//		test = itsFloatQueue.size();
		}
	}
	return value;
}
//20.3.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetLogFile(bool OnOff)
{ //tuleekohan ongelmia kun ohjataan logfileen vaikka se on suljettu
	if(!OnOff)
	{
		if(itsLogFile)
		{
 //			*itsLogFile << "** Loki suljetaan käskystä" << endl; 
			itsLogFile->close();
			itsLogFile->clear();
			return true;
		}
		return false;
	}
	else
	{
		if(itsLogFile)
		{
	/* ei onnaa, pitäisi välittää muillekin olioille
			delete itsLogFile;
			itsLogFile = new ofstream;

			itsLogFile->open(itsLogFileName, ios::out|ios::ate);  
 			*itsLogFile << "** Loki avataan käskystä" << endl; 
			return true;
	*/
		}
		return false;
	}
}; 
//20.3.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetProductName(const NFmiString& theName)
{
	itsOutFileName = theName;
	fTimeStamp = false;
	fDataTimeStamp = false;
	itsProductNameFormat = NFmiString(); //nollaus

	return true;
}; 
//15.1.01----------------------------------------------------------------------------
bool NFmiPressProduct::SetProductNameFormat(const NFmiString& theName)
{
	itsOutFileName = NFmiString(); //nollaus
//	fTimeStamp = false;
//	fDataTimeStamp = false;
	itsProductNameFormat = theName;

	return true;
}; 
//7.9.01----------------------------------------------------------------------------
bool NFmiPressProduct::SetMaskNumber(unsigned long theNumber)
{
	bool retCode = false;
	if (GetEnvironment().ChangeMaskNumber(theNumber)) //muutetaan vain asetettua
		retCode = true; 
	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	while(param)
	{
//		if (param->GetEnvironment().ChangeMaskNumber(theNumber)) //muutetaan vain asetettua
//			retCode = true; //ainakin yhdessä vaihdettu
		if (param->ChangeMaskNumber(theNumber)) //muutetaan vain asetettua
			retCode = true; //ainakin yhdessä vaihdettu
		param = (NFmiPressParam*)paramIter.Next();
	}
	return retCode;
}; 
//22.3.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetSegmentData(const NFmiString& theDataName)
{
	bool retCode = false;
	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	while(param)
	{
		if (param->SetData(theDataName))
			retCode = true; //ainakin yhdessä vaihdettu
		param = (NFmiPressParam*)paramIter.Next();
	}
	return retCode;
}; 
//30.11.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetFirstObjectActivity(bool theActivity)   
{
	//voisi kehittää niin että muutetaan eka sellainen joka 
	//on eri kuin theActivity, niin alusta voisi (de)aktivoida n kpl
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsObjects);
	NFmiPressScaling* object;  //PsWriting kaataa, entä NFmiPressScaling
	objectIter.Reset();
	object = (NFmiPressScaling*)objectIter.Next();
	object->SetActivity(theActivity);
	return true;
}

//17.3.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetFirstStation(const NFmiLocation& theLocation)   
{
	bool retCode = false;
	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	while(param)
	{
		if (param->SetFirstStation(theLocation))
			retCode = true; //ainakin yhdessä vaihdettu
		param = (NFmiPressParam*)paramIter.Next();
	}
	return retCode;
}
//29.9.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetImagePreNames(const NFmiLocation& theLocation)   
{
	bool retCode = false;   
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsObjects);
	NFmiPressImage *image;
	objectIter.Reset();
	NFmiHyphenationString oldFile;
	NFmiFileString path, newFile;
	NFmiString dummy;
	NFmiString underScore("_"); // ennen alaviiva oleva osa korvataan uudella asemanimellä
	image = (NFmiPressImage*)objectIter.Next();
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
				path.Header((char*)newFile);
				image->SetPath(path);
				retCode = true;
			}
		}
		image = (NFmiPressImage*)objectIter.Next();
	}
	return retCode;
}
//7.6.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetStationRename(const NFmiRenaming& theRename)   
{
	bool retCode = false;
	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	while(param)
	{
		if (param->SetStationRename(theRename))
			retCode = true; //ainakin yhdessä vaihdettu
		param = (NFmiPressParam*)paramIter.Next();
	}
	return retCode;
}
//17.3.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetAllTimes(const NFmiMetTime& theTime)   
{
	SetTime(theTime); //oma ensi

	bool retCode = false;

	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams); //segmentit
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	while(param)
	{
		if (param->SetAllTimes(theTime))
			retCode = true; //ainakin yhdessä vaihdettu
		param = (NFmiPressParam*)paramIter.Next();
	}
                              //muut objektit, kaikissa ei tosin aikaa
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsObjects); 
	NFmiPressScaling *object;
	objectIter.Reset();
	object = (NFmiPressScaling*)objectIter.Next();
	while(object)
	{
		object->SetTime(theTime);
//		if (object->SetTime(theTime)) //ei looginen
//			retCode = true; //ainakin yhdessä vaihdettu
		object = (NFmiPressScaling*)objectIter.Next();
	}

	return retCode;
}
//17.3.00----------------------------------------------------------------------------
bool NFmiPressProduct::SetAllLanguages(FmiLanguage theLanguage)   
{
	itsLanguage = theLanguage; //oma ensi

	bool retCode=true;//HUOM

	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams); //segmentit
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	while(param)
	{
//		param->SetLanguage(theLanguage));
		if (param->SetAllLanguages(theLanguage))
			retCode = true;          //ainakin yhdessä vaihdettu
		param = (NFmiPressParam*)paramIter.Next();
	}
                              //muut objektit, kaikissa ei tosin aikaa
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsObjects); 
	NFmiPressScaling *object;
	objectIter.Reset();
	object = (NFmiPressScaling*)objectIter.Next();
	while(object)
	{
		object->SetLanguage(theLanguage);
		object = (NFmiPressScaling*)objectIter.Next();
	}

	return retCode;
}
//11.8.99----------------------------------------------------------------------------
bool NFmiPressProduct::ReadNameToLonLatList()
{
	NFmiString fileName, fileName1;
    fileName = GetHome();

//Mikan vaihdos
	fileName += kFmiDirectorySeparator;
	fileName += NFmiString("Muut");
	fileName += kFmiDirectorySeparator;

	fileName1 = fileName;
	fileName1 += NFmiString("AsemaNimetLonLatUusi.txt");
	if (itsNameToLonLat->AddFile(fileName1, true, true))   //3.6.02 uusi/vanha muoto mahdollistamaan versionpäivityksen Vespaan
	{
		*itsLogFile << "  AsemaNimetLonLatUusi.txt luettu" << endl;
		return true;
	}
/* 20.6.02 ei uusi ohjelma osaa vanhaa tiedostoa
	fileName += NFmiString("AsemaNimetLonLat.txt");
	if (itsNameToLonLat->AddFile(fileName, true, true))
	{
		*itsLogFile << "  AsemaNimetLonLat.txt luettu" << endl;
		return true;
	}
	*/
	else
	{
		*itsLogFile << "*** ERROR: AsemaNimetLonLatUusi.txt:n luku epäonnistui" << endl;
		return false;
	}
}
//----------------------------------------------------------------------------
bool NFmiPressProduct::WeekdayDirectiveActive(const string & theDefinition, const FmiPressSeasons& theSeasons) const
//21.5.02: tarkistaa onko viikonpäiväehdot tähän vuodenaikaan voimassa, esim.
//         lumikartta ei piirretä torstaisin jos snow-kausi ei ole päällä
//         oletus tietysti että viikonpäivädirrit on aktiivisia
{
	string::size_type  start1, end1, start2, end2, pos;
	const string delims(" \t\n");
	pos = theDefinition.find("#Viikonpäivä");
	if(pos == string::npos)
	{
		pos = theDefinition.find("#viikonpäivä");
		if(pos == string::npos)
		{
			pos = theDefinition.find("#Weekday");
			if(pos == string::npos)
			{
				pos = theDefinition.find("#weekday");
			}
		}
	}
	if(pos != string::npos)
	{
		pos = theDefinition.find_first_of(delims, pos);
		start1 = theDefinition.find_first_not_of(delims, pos);
		end1 = theDefinition.find_first_of(delims, start1);
		start2 = theDefinition.find_first_not_of(delims, end1);
		end2 = theDefinition.find_first_of(delims, start2);
		string str1 = theDefinition.substr(start1, end1-start1);
		string str2 = theDefinition.substr(start2, end2-start2);
		bool in;
		NFmiString str = str1;
		str.LowerCase();
		if(str == "onlyin" || str == "vainkun")
			in = true;
		else if(str == "onlyoutside" || str == "vainkunei")
			in = false;
		else
		{
			*itsLogFile << "***ERROR: on #Weekday line"<< endl;
			return true;
		}
		str = str2;
		str.LowerCase();
		if(str == "summer" || str == "kesä")
		{
			*itsLogFile << "viikonpäiväohjauksen riippuvuus: kesä"<< endl;
			return in == theSeasons.summer;
		}
		else if(str == "snowperiod" || str == "lumikausi")
		{
			*itsLogFile << "viikonpäiväohjauksen riippuvuus: lumikausi"<< endl;
			return in == theSeasons.snow; 
		}
		else if(str == "pollenperiod" || str == "siitepölykausi")
		{
			*itsLogFile << "viikonpäiväohjauksen riippuvuus: siitepölykausi"<< endl;
			return in == theSeasons.pollen; 
		}
		else if(str == "wintertime" || str == "talviaika")
		{
			*itsLogFile << "viikonpäiväohjauksen riippuvuus: talviaika"<< endl;
			return in == theSeasons.wintertime; 
		}
		else if(str == "summertime" || str == "kesäaika")
		{
			*itsLogFile << "viikonpäiväohjauksen riippuvuus: kesäaika"<< endl;
			return in != theSeasons.wintertime; //HUOM
		}
		else
		{
			*itsLogFile << "***ERROR: on #weekday line"<< endl;
			return true;
		}
	}
	return true; //oletus: ei löydy tiedostoa -> ei kielletä viikonloppudirektiivien käyttöä
}
//----------------------------------------------------------------------------
bool NFmiPressProduct::GetSeasonsStatus(FmiPressSeasons& theSeasonsStatus) 
//20.5.2002 ratkaisee voimassa olevat kausivaihtelut tiedostosta lukien tai käyttämällä oletusarvoja
{
//asetetaan oletukset
	NFmiString statusString;
	NFmiTime today;
	NFmiTime summerStart(today);
	summerStart.SetDate(today.GetYear(), 5, 31); 
	NFmiTime summerStop(today);
	summerStop.SetDate(today.GetYear(), 8, 31); 
	theSeasonsStatus.wintertime = today.GetZoneDifferenceHour() == -2; 
	theSeasonsStatus.summer = today >= summerStart && today < summerStop; //2.9.02 vain <
	theSeasonsStatus.pollen = !theSeasonsStatus.wintertime;
	theSeasonsStatus.snow = !theSeasonsStatus.pollen;
	theSeasonsStatus.pollenOrSnow = theSeasonsStatus.pollen || theSeasonsStatus.snow; //4.9.02
	theSeasonsStatus.weekday = today.GetWeekday();
	theSeasonsStatus.dayAdvance = +0;

	string includePath(itsHomePath);
	includePath += kFmiDirectorySeparator;
	includePath += "Muut";
	includePath += kFmiDirectorySeparator;
	includePath += "KausiTilanne.txt";
	ifstream in(includePath.c_str(),ios::in|ios::binary);

	if(!in)
	{
		*itsLogFile << "ei kausitilanne-tiedostoa"<< endl;
		return true;
	}

	string::size_type start1, end1, start2, end2;
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

		if(end2 == string::npos)
		{
			*itsLogFile << "***ERROR: invalid line in seasonstatus: "<< line << endl;
			continue;
		}
		else
		{
			string str1 = line.substr(start1, end1-start1);
			NFmiString fmiStr1 = str1;
			fmiStr1.LowerCase();
			string shortStr1 = str1.substr(start1, 4); //loppuosan ei tartte olla oikein kirjoitettu
			NFmiString fmiShortStr1 = shortStr1;
			fmiShortStr1.LowerCase();
			string str2 = line.substr(start2, end2-start2);
			string shortStr2 = str2.substr(0, 2);
			NFmiString fmiShortStr2 = shortStr2;
			fmiShortStr2.LowerCase();

			bool status;
			NFmiString statusString("???");
			bool boolGiven = true;
			bool undef = false;
			if(fmiShortStr2 == "ye" || fmiShortStr2 == "on")
			{
				status = true;
				statusString = "PÄÄLLE";
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
					theSeasonsStatus.wintertime = status;				
					*itsLogFile << "Kausitilanne pakotettu: talviaika "<< (char*)statusString << endl;
				}
			}
			else if(fmiShortStr1 == "summ" || fmiShortStr1 == "kesä")
			{
				if(boolGiven && !undef)
				{
					theSeasonsStatus.summer = status;				
					*itsLogFile << "Kausitilanne pakotettu: kesä "<< (char*)statusString << endl;
				}
			}
			else if(fmiShortStr1 == "poll" || fmiShortStr1 == "siit")
			{
				if(boolGiven && !undef)
				{
					theSeasonsStatus.pollen = status;				
					*itsLogFile << "Kausitilanne pakotettu: siitepölykausi "<< (char*)statusString << endl;
				}
			}
			else if(fmiShortStr1 == "snow" || fmiShortStr1 == "lumi")
			{
				if(boolGiven && !undef)
				{
					theSeasonsStatus.snow = status;				
					*itsLogFile << "Kausitilanne pakotettu: lumikausi "<< (char*)statusString << endl;
				}
			}
			else if(fmiShortStr1 == "week" || fmiShortStr1 == "viik")
			{
				if(!undef)
				{
					if(fmiShortStr2 == "mo" || fmiShortStr2 == "ma")
						theSeasonsStatus.weekday = 1;
					else if(fmiShortStr2 == "tu" || fmiShortStr2 == "ti")
						theSeasonsStatus.weekday = 2;
					else if(fmiShortStr2 == "we" || fmiShortStr2 == "ke")
						theSeasonsStatus.weekday = 3;
					else if(fmiShortStr2 == "th" || fmiShortStr2 == "to")
						theSeasonsStatus.weekday = 4;
					else if(fmiShortStr2 == "fr" || fmiShortStr2 == "pe")
						theSeasonsStatus.weekday = 5;
					else if(fmiShortStr2 == "sa" || fmiShortStr2 == "la")
						theSeasonsStatus.weekday = 6;
					else if(fmiShortStr2 == "su" /*turha|| fmiShortStr2 == "su"*/)
						theSeasonsStatus.weekday = 7;
					else 
					{
						*itsLogFile << "***ERROR: invalid weekday in seasonstatus"<< str2 << endl;
						continue;
					}
					*itsLogFile << "Viikonpäivä pakotettu: "<< fmiShortStr2 << endl;
				}
			}
			else if(fmiShortStr1 == "adva" || fmiShortStr1 == "enna")
			{
				if(!undef)
				{
					theSeasonsStatus.dayAdvance = atoi(str2.c_str()); //tarvitaanko missään tässä
					itsEnvironment.SetDayAdvance(theSeasonsStatus.dayAdvance);
					*itsLogFile << "Ennakko pakotettu: "<< str2 << endl;
				}
			}
			else 
			{
				*itsLogFile << "***ERROR: invalid keyword in seasonstatus"<< str1 << endl;
				continue;
			}
		}
	}
	theSeasonsStatus.pollenOrSnow = theSeasonsStatus.pollen || theSeasonsStatus.snow; //4.9.02
	return true;
}
//24.5.02----------------------------------------------------------------------------
bool NFmiPressProduct::PreProcessConditionally(NFmiPreProcessor& thePrepr, bool theCondValue, const string & theConditionalBeginDirective, const string & theConditionalNotBeginDirective
										, const string & theConditionalEndDirective, const string & theConditionalElseDirective )
{
	bool res1, res2;
	res1 = thePrepr.SetConditionalStripping(theCondValue, theConditionalBeginDirective, theConditionalNotBeginDirective
	,theConditionalEndDirective, theConditionalElseDirective);
	res2 = thePrepr.Strip();
	if(!res1 || !res2)
	{
		*itsLogFile << "*** ERROR: Preprocessing failed: " << theConditionalBeginDirective << endl;
		string message = thePrepr.GetMessage();
		if(!message.empty())
			*itsLogFile << "*** "  << message << endl;
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------
bool NFmiPressProduct::PreProcessDefinition(const string& inFileName, const string& outFileName)
//25.10.2001 Tämä korvaa PreProcessProduct:n tehden kaikki siivoushommat: //-kommentit,
// /* */-kommentit, if/else/endif-direktiivit ja Includet
// nyt voi koodin käsittelystä poistaa kommenttien käsittelyn, entäs prem???
 
{
	FmiPressSeasons seasonsStatus;  //20.5.02
	GetSeasonsStatus(seasonsStatus);

	NFmiPreProcessor prePr;
	bool res, res2;
	NFmiTime tim;
 
	string includePath(itsHomePath);
	includePath += kFmiDirectorySeparator;
	includePath += "Include";
	res = prePr.ReadFile(inFileName);

	while (!prePr.NoFilesIncluded())
	{
		if(!PreProcessConditionally(prePr, seasonsStatus.pollenOrSnow, "#ifPollenOrSnowPeriod", "#ifNotPollenOrSnowPeriod", "#pollenOrSnowPeriodEndif", "#pollenOrSnowPeriodElse"))  //4.9.02
			return false;
		if(!PreProcessConditionally(prePr, seasonsStatus.wintertime, "#ifWinterTime", "#ifNotWinterTime", "#endif", "#else"))
			return false;
		if(!PreProcessConditionally(prePr, seasonsStatus.snow, "#ifSnowPeriod", "#ifNotSnowPeriod", "#snowPeriodEndif", "#snowPeriodElse"))
			return false;
		if(!PreProcessConditionally(prePr, seasonsStatus.summer, "#ifSummer", "#ifNotSummer", "#summerEndif", "#summerElse"))  //13.6.02
			return false;
		if(!PreProcessConditionally(prePr, seasonsStatus.pollen, "#ifPollenPeriod", "#ifNotPollenPeriod", "#pollenPeriodEndif", "#pollenPeriodElse"))  //4.9.02
			return false;
		if(!prePr.IncludeFiles("#Include", includePath, "inc"))
		{
			*itsLogFile << "*** ERROR: Preprocessing failed to include file" << endl;
			string message = prePr.GetMessage();
			if(!message.empty())
				*itsLogFile << "*** "  << message << endl;
			return false;
		}
	}

/*
	//bool isWinterTime= tim.GetZoneDifferenceHour() == -2; 
	res = prePr.SetConditionalStripping(seasonsStatus.wintertime, "#ifWinterTime", "#ifNotWinterTime", "#endif", "#else");



// Mikan vaihdos
//	string includePath(itsHomePath);
//	includePath += kFmiDirectorySeparator;
//	includePath += "Include";
// **	string includePath = itsHomePath; 
// **	includePath += "\\Include";   
	res = prePr.SetIncluding("#Include", includePath, "inc");
	res = prePr.ReadAndStripFile(inFileName);
	string message = prePr.GetMessage();
	res2 = prePr.SetIncluding("#Liitä", includePath, "inc"); //voidaan poistaa kun määrittelyistä poistettu
	res2 = prePr.Strip();
//	string message = prePr.GetMessage();
	if(!message.empty())
	    *itsLogFile << "*** "  << message << endl;
	if(prePr.NumOfLiita() > 0)
	    *itsLogFile << "*** WARNING: LIITÄ käytetty"  << endl;
	if(!res || !res2)
	{
	    *itsLogFile << "*** ERROR: Preprocessing failed" << endl;
		return false;
	}
*/
	//jokaisen viikonpäivän direktiivit tarkastetaan
	bool isTheDay; //= true;
	bool weekdayActive = WeekdayDirectiveActive(prePr.GetString(),seasonsStatus); 
	for (int day = 1; day <= 7; day++)
	{
		isTheDay = tim.GetWeekday() == seasonsStatus.weekday;
		string weekday(tim.Weekday(kEnglish));
		string ifDir = "#if" + weekday;
		string ifNotDir = "#ifNot" + weekday;
		string elseDir = "#" + weekday + "Else";
		string endifDir = "#" + weekday + "Endif";
		res = prePr.SetConditionalStripping(isTheDay && weekdayActive, ifDir, ifNotDir, endifDir, elseDir);
		res2 = prePr.Strip();
		string message = prePr.GetMessage();
		if(!message.empty())
			*itsLogFile << "*** ERROR: "  << message << endl;
		if(prePr.NumOfLiita() > 0)
			*itsLogFile << "*** WARNING: LIITÄ käytetty"  << endl;
		if(!res || !res2)
		{
			*itsLogFile << "*** ERROR: Preprocessing failed" << endl;
			return false;
		}
		tim.ChangeByDays(1);
		//isToday = false;
	}

	string outString = prePr.GetString();
	ofstream file(outFileName.c_str());
	file << outString ;
	file.close();
	file.clear();
	return true;
}
//----------------------------------------------------------------------------
bool NFmiPressProduct::PreProcessProduct( ifstream& inFile, ofstream& outFile)
{
  const short lineSize = 130;	//max rivipituus
 // unsigned char inBuf[lineSize];
  char inBuf[lineSize]; //27.8.01
//  short comLevel = 0;
  NFmiString str, file;
  NFmiString incString = NFmiString("#Include");
  NFmiString liitaString = NFmiString("#Liitä");

  while (inFile.getline(inBuf, lineSize, '\n'))
  {
	  short num = inFile.gcount();
	  str.Set((unsigned char*)inBuf, num);
//     unsigned short nStar = str.Search( NFmiString("*"));
//	  if(nStar > 0)
//	  {
//		  NFmiString testS = str.GetCharsPtr(nStar+1,1);
//		  if(str.GetCharsPtr(nStar+1,1) == NFmiString("/"))
//			  *itsLogFile << "*/ " << endl;
//	  }
  
	  unsigned short nRisu = (unsigned short)str.Search( NFmiString("#"));//21.1 jotta löytyisi
/*	                                                       // tabinkin takaa
	  if(nRisu>0)               // TÄSTÄ OLISI TARKOITUS KEHITTÄÄ MÄÄRITTELYTIEDOSTON PARAMETRISOINTI
	  {
		  if(str.Search(NFmiString("##PAR")))
		  {
			  if(!ReplacePar(str))
			  {
				  return false;
			  }
		  }

		if(str.Search(NFmiString("#PAR")))    //080399 ##PARit yllä poistettu
		{
			if(str.Search(NFmiString("##"))) //määrittely
			{
				if(!DefinePar(str))
				{
//				  *itsLogFile << "*** ERROR: PARin asetus ei onnistunut" << endl;
//				  *itsLogFile << "       keskeytetään" << endl;
				  return false;					
				}
			}
		}
		*/
		if((nRisu>0 && num-nRisu > 8) && (str.GetChars(nRisu,8) == incString //Huom string-bugi
		             || str.GetChars(nRisu,6) == liitaString)) //7.1
		  {
			  if(str.GetChars(nRisu,8) == incString)   //8.1
				 file = str.GetChars(nRisu+9,num-9 -nRisu); //vois toki tehdä alykkäämmin
			  else
				 file = str.GetChars(nRisu+7,num-7-nRisu);

			  file.TrimR(' ');      //12.1

			  NFmiFileString includeFileName = GetHome(); //6.8.01 +File
//Mikan vaihdos
			  includeFileName += kFmiDirectorySeparator;
			  includeFileName += NFmiString("Include");
			  includeFileName += kFmiDirectorySeparator;  //25.9 
//			  includeFileName += NFmiString("\\Include\\");  //25.9 
			  includeFileName += file;
			  if(!includeFileName.HasExtension())  //6.8.01
				includeFileName += NFmiString(".inc"); 

			  ifstream includeFile(includeFileName, ios::in);
			  if(includeFile.good() && !includeFile.eof())  //21.1
			  {
				 NFmiCopyFile(includeFile, outFile);
			  }
			  else
			  {
				  if(itsLogFile)
					*itsLogFile << "*** ERROR: LiiteTiedostoa ei ole: " << (char*)file << endl;
			  }
			  includeFile.close();
			  includeFile.clear();
//		  }
	  }
	  else
	  {
		outFile.write(inBuf, num-1);	 
		outFile.put('\x0A');
	  }
  }
  return true;
}
//-----------------------------------------------------------------------------
bool NFmiPressProduct::DefinePar(const NFmiString& theString)  
{
	NFmiHyphenationString hString(theString);
	NFmiString retString;

// Mika: Onko sivuvaikutuksia?
	hString.Search(NFmiString("##PAR"));
//	short firstSkip = (short)hString.Search(NFmiString("##PAR"));

	return true;
}
//-080399----------------------------------------------------------------------------
bool NFmiPressProduct::ReplacePar(NFmiString& theString)  
{
	return true;
}
//----------------------------------------------------------------------------
bool NFmiPressProduct::ReadDescriptionFile(NFmiString inputFile) 
{  
 
  itsOutputMode = kPostScript; //200600

  char* hChar;

  NFmiString fmiString, origHome;
  
   
   hChar = getenv("lehtiTuoteDir"); 

   if(hChar == 0)
   {
//Mikan vaihdos
#ifndef UNIX
	   fmiString = NFmiString("C:\\Program Files\\LehtiAuto");
#else
	   char * home = getenv("HOME");
	   fmiString = NFmiString(home) + NFmiString("/LehtiAuto");
#endif
//	   fmiString = NFmiString("C:\\Program Files\\LehtiAuto");

	   origHome = NFmiString("(Oletus)");
//	   cout << "Kotihakemisto(Oletus): " << (char*)string << endl;
   }
   else
   {
	   fmiString = hChar;
	   origHome = NFmiString("(Ympäristömuuttuja)");

//	   cout << "Kotihakemisto(Ympäristömuuttuja): " << (char*)string << endl;
   }
//   cout << "Kotihakemisto: " << (char*)string << endl;
   SetHome(fmiString);                

//**Tarkistus että hakemisto olemassa, vaikka nyt endA4.eps:n olemassaolo
   NFmiString endFileName = GetHome();

//Mikan vaihdos
   endFileName += kFmiDirectorySeparator;
   endFileName += NFmiString("Muut");
   endFileName += kFmiDirectorySeparator;
//   endFileName += NFmiString("\\Muut\\");		   

   endFileName += NFmiString("endA4.eps");   
   ifstream endFile(endFileName, ios::in|ios::in); 
   if(!endFile)   
	{
// Mikan korjaus ja Lassen muutos siihen
//		cout << "**Kotihakemisto " << endFileName.GetCharPtr() << " kadoksissa**" <<  endl;
		cout << "**Kotihakemisto " << GetHome().GetCharPtr() << " kadoksissa**" <<  endl;
		cout << "  joko annettava ympäristömuuttujana" <<  endl;
#ifndef UNIX
		cout << "  tai oltava C:\\Program Files\\LehtiAuto" <<  endl;
#else
		cout << "  tai oltava $HOME/LehtiAuto" <<  endl;
#endif
		return false;
	}
//**

   hChar = getenv("lehtiLokiDir");   
   if(hChar == 0)
   {
	   fmiString = GetHome();
//Mikan vaihdos
       fmiString += kFmiDirectorySeparator;
       fmiString += NFmiString("Loki");
 //      fmiString += NFmiString("\\loki");
   }
   else
	   fmiString = hChar;
   
   NFmiString logFileName = fmiString;//hChar;
//Mikan vaihdos
   logFileName += kFmiDirectorySeparator;
   logFileName += NFmiString("lehti.log");
//   logFileName += NFmiString("\\lehti.log");
   itsLogFileName = logFileName;   //21.3.00 jotta voidaan sulkea/avata jatkossa
//   itsLogFile->open(logFileName, ios::out|ios::ate);  
   itsLogFile->open(logFileName, ios::out|ios::app); //3.9.01, edellinen aloitti alusta
   NFmiTime tim;
   *itsLogFile << endl << "** Loki avattu " << tim << " **" << endl;

   NFmiString retString;
   NFmiString origInputFileName = inputFile;
   NFmiFileString inputFileName = inputFile.GetChars(1,inputFile.GetLen()-3);
//   itsInFileName = inputFileName; //21.3.00 jotta manageri voi siitä muodostaa prem-tiedoston 
   itsInFileName = inputFileName.Header(); //27.8.02 skriptistä kun kutsutaan tulee muuten koko polku
//  *itsLogFile << endl << "*1*" << (char*)itsInFileName << endl;

   //*** 3.4 .pss-tiedostot Temppiin
   NFmiFileString tempInput;
		   tempInput = GetHome();
//Mikan vaihdos
		   tempInput += kFmiDirectorySeparator;
		   tempInput += NFmiString("Temp");
		   tempInput += kFmiDirectorySeparator;
//		   tempInput += NFmiString("\\Temp\\");

   NFmiString inputOnlyFile = inputFileName.FileName();
//   *itsLogFile << endl << "*2*" << (char*)inputOnlyFile << endl;

   tempInput += inputFileName.FileName();  
   tempInput += NFmiString("pss");
   
   itsOutFileName = inputOnlyFile;  

   inputFileName += NFmiString("pss");
 
   NFmiString writeString = inputFileName.Header();
   *itsLogFile << "** " << (char*)writeString << " **"<< endl; 
   *itsLogFile << "program version = 23.9.2002" << endl;       
   *itsLogFile << "Home dir " <<(char*)origHome << ": " << (char*)GetHome()  << endl;  

/* pois 25.10.01
   ifstream origInput(origInputFileName, ios::in);
   ofstream output(tempInput);  
   PreProcessProduct(origInput, output);
   origInput.close();
   output.close(); */ 

// tilalle 25.10.01
   string inputStdName(origInputFileName);
   string outputStdName(tempInput);
   if(!PreProcessDefinition(inputStdName, outputStdName))
   {
	  *itsLogFile << "***ERROR: interrupted, see above" << endl;
	  return false;
   }

   itsDescriptionFile = new ifstream(tempInput, ios::in);

   fDataRead = false;  

   if (!ReadDescription(retString))   
	   return false;

   if(!FirstData())  
      itsCurrentDataIter = 0; 	

//   cin = saveCin;
//   input.close();
   itsDescriptionFile->close();
   itsDescriptionFile->clear();

   if (!fDataRead)
	   ReadData();

	if(itsLogFile)
	  *itsLogFile << "KOOSTETAAN TUOTE" << endl;	 

  return true;
}
//050199----------------------------------------------------------------------------
bool NFmiPressProduct::ReadData()
{
   if (fDataRead)   
	   return true;

   if(itsLogFile)
		  *itsLogFile << "LUETAAN ANNETUT DATATIEDOSTOT" << endl;

	
   char* hChar;
   NFmiHyphenationString str; 
   hChar = getenv("lehtiDataDir");
   if(hChar == 0)
   {

		str = GetHome();
//Mikan vaihdos
		str += kFmiDirectorySeparator;
       str += NFmiString("Data");
//       str += NFmiString("\\Data");
   }
   else
	   str = hChar;

	*itsLogFile << "  datahakemistot: " << (char*)str << endl;	 

   NFmiString dataPath, dataPath2;  //5.50.99 kaksi datahakemistoa, luupituksella voisi ottaa useampiakin
   str.NextSubString(NFmiString(";"),dataPath);
   bool secondDir = false;
   bool twoOptinalTypes = true; //6.8.01
   if(str.NextSubString(NFmiString(";"),dataPath2))
		secondDir = true;


    NFmiQueryData** data;
	NFmiQueryData* dataPtr;
	NFmiNamedQueryData* nData;
	NFmiVoidPtrIterator iter = NFmiVoidPtrIterator(itsDatas);
	NFmiString name;
	NFmiFileString dataFile, dataFile2;
	NFmiFileString dataFileSqd, dataFileSqd2;
	iter.Reset();
	nData = (NFmiNamedQueryData*)iter.Next();
	short loop = 1;
	while(nData)
	{
		dataPtr = nData->GetData();
		data = &dataPtr;
		dataFile = dataPath;
		dataFile2 = dataPath2;
//Mikan vaihdos
		dataFile += kFmiDirectorySeparator;
		dataFile2 += kFmiDirectorySeparator;
//		dataFile += NFmiString("\\");
//		dataFile2 += NFmiString("\\");
		dataFile += nData->GetName();
		dataFile2 += nData->GetName();
	    dataFileSqd = dataFile;
	    dataFileSqd2 = dataFile2;
		if(!dataFile.HasExtension())
		{
			dataFile += NFmiString(".fqd");
			dataFile2 += NFmiString(".fqd");
			dataFileSqd += NFmiString(".sqd");
			dataFileSqd2 += NFmiString(".sqd");
		}
		else
			twoOptinalTypes = false;

// 4.9 käyetään nyt Vilin poikkeusmenettelyä 
		
//26.8.99 Yritetään sekä fqd-tiedostoa että sqd:tä ja 5.10.99 kahdesta hakemistosta

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
							*itsLogFile << "  *** ERROR: datan luku epäonnistui: " << (char*)nData->GetName() << endl;	 
						}
						else
						{
							*itsLogFile << "  sqd-data luettu: " <<  (char*)dataFileSqd2 << endl;
						}
					}
					else
					{
						*itsLogFile << "  fqd-data luettu: " <<  (char*)dataFile2 << endl;
					}
				}
				else
				{
					nData->SetData(0);
					*itsLogFile << "  *** ERROR: datan luku epäonnistui: " << (char*)nData->GetName() << endl;	 
				}
			}
			else
			{
				*itsLogFile << "  sqd-data luettu: " <<  (char*)dataFileSqd << endl;
			}
		}
		else
		{
			*itsLogFile << "  fqd-data luettu: " <<  (char*)dataFile << endl;
		}
 
		loop++;		
	    nData = (NFmiNamedQueryData*)iter.Next();
	}
	fDataRead = true;

	if(itsMaskIter)   //12.9.01
		delete itsMaskIter;
	if(DataByName(itsMaskFileName) /*&& itsEnvironment.GetMaskNumber() > 0*/)
		itsMaskIter = new NFmiSuperSmartInfo(DataByName(itsMaskFileName)); 
	//		*itsLogFile << "  ERROR new NFmiSuperSmartInfo" << endl;
	else
		itsMaskIter = 0;

	if(itsLogFile)
	{
		if(loop > 1)
		  *itsLogFile << "KAIKKI DATAT LUETTU" << endl;
		else
		  *itsLogFile << "EI ANNETTUJA DATATIEDOSTOJA" << endl;
	}

  return true;
}

//----------------------------------------------------------------------------
// 4.9 Vililtä kopsattu poikkeusmenettely
bool NFmiPressProduct::ReadQueryData(NFmiQueryData *theQD,char *fileName)
{
  std::fstream  dataFile;
//  istream  readCout;

  dataFile.open(fileName,ios::in | ios::binary); //24.8.99 + binary
//Mika pois
//istream readCout = dataFile;

  if(dataFile.good() && !dataFile.eof()) //lasse lisännyt eof (jos puuttuu)            
  {
          try                       //Asetaan poikkeutuskäsittely
			                         // tänne voisi yhdistää testin yllä (ifit pois)
          {
                 dataFile >> *theQD;
 //                readCout >> *theQD;
          }
          catch(char *msg)          //virheen sattuessa jatketaan tästä suoritusta 
                {
					  if(itsLogFile)
			              *itsLogFile << "  " << msg << endl;	 
			        
  //                  cout << msg << endl;
 //                       cout << "Main::ReadQueryData(..) readCout >> *theQD Error. " << endl;
                        dataFile.close();
						dataFile.clear();
                        return false;
                }

    dataFile.close();
	dataFile.clear();
  }
  else    //Lasse
  {
	  return false;
  }

  return true;
}
//----------------------------------------------------------------------------
NFmiQueryData* NFmiPressProduct::FirstData(void) 
{
	NFmiNamedQueryData* nData;
	NFmiVoidPtrIterator iter = NFmiVoidPtrIterator(itsDatas);
	iter.Reset();
	nData = (NFmiNamedQueryData*)iter.Next();
	if(nData)
//		if(nData->GetData())
			return nData->GetData();
//		else
//			return 0;
	else
		return 0;
}
//----------------------------------------------------------------------------
NFmiString NFmiPressProduct::FirstDataName(void) //3.3.99
{
	NFmiNamedQueryData* nData;
	NFmiVoidPtrIterator iter = NFmiVoidPtrIterator(itsDatas);
	iter.Reset();
	nData = (NFmiNamedQueryData*)iter.Next();
	if(nData)
		return nData->GetName();
	else
		return NFmiString();
}
//----------------------------------------------------------------------------
NFmiQueryData* NFmiPressProduct::DataByName(NFmiString givenName) 
{
	NFmiNamedQueryData* nData;
	NFmiVoidPtrIterator iter = NFmiVoidPtrIterator(itsDatas);
	NFmiString name;
	iter.Reset(); 
	nData = (NFmiNamedQueryData*)iter.Next();
	while(nData)
	{
		name = nData->GetName();
		if (name == givenName)
			return nData->GetData();
	    nData = (NFmiNamedQueryData*)iter.Next();
	}
	return 0;
}
//12.12 katkaistu----------------------------------------------------------------------------
bool NFmiPressProduct::ReadDescription(NFmiString& retString) //16.1
{

    NFmiString helpString;
	NFmiValueString valueString;
	itsScale.SetStartScales(NFmiRect(NFmiPoint(0,0), NFmiPoint(614,794)));
	itsPlottingRect.Set(NFmiPoint(0, 0), NFmiPoint(614,794));  //12.1
	itsMargin = 0;
	itsMapGap = 0;
	itsNumberOfMaps = 1;
	fMapIsEps = true;
	fTimeStamp = false;  //16.3.00
	fDataTimeStamp = false; //16.3.00
	itsLanguage = kFinnish;
	double xmin,xmax,ymin,ymax;
	long long1, long2, long3;

//	long firstDeltaDays = +1;
//	long origFirstDeltaDays = +1; 
	long firstDeltaDays = 1 + itsEnvironment.GetDayAdvance();  //23.5.02
	long origFirstDeltaDays = firstDeltaDays;
    short firstPlotHours = 12;
    short firstPlotMinutes = 0;
	itsFirstPlotTime.ChangeByDays(firstDeltaDays); 
    itsFirstPlotTime.SetTime(firstPlotHours);      
	short hourStep = 24;

	unsigned long producer = 230; 

//	cin >> itsObject;                     
	*itsDescriptionFile >> itsObject; //30.8.01
	itsString = itsObject;
	itsIntObject = ConvertDefText(itsString);
	if(!(itsString == NFmiString("#!PressProduct")))     
	{
		*itsLogFile << "Määrittelytiedostoa ei ole tai ei ole lehtituote" << endl; //25.6
		cin >> hourStep; //pysähdys
		return false;
	}
//	cin >> itsObject;                     
	*itsDescriptionFile >> itsObject; //30.8.01
	itsString = itsObject;
	itsIntObject = ConvertDefText(itsString);
	
	while(itsIntObject != 9999 || itsCommentLevel) 
	{
	if(itsLoopNum > itsMaxLoopNum)  
	{
		*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty päätasolla" << endl;
	    retString = itsString;
		return false;
	}
	itsLoopNum++;  //19.12 jotta virhe ei aiheuttaisi ikuista luuppia

	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	switch(itsIntObject)
	{
		case dOther:	  //ylimääräistä roinaa, END lopettaa
		{
			*itsLogFile << "*** ERROR: Tuntematon sana päätasolla: " << (char*)itsObject << endl;  

			ReadNext();
//			*itsDescriptionFile >> itsObject;
//			itsString = itsObject;
//			itsIntObject = ConvertDefText(itsString);
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
		case dOutputMode:	//20.6.2000 	  
		{
			if (!ReadEqualChar())
				break;

			*itsDescriptionFile >> itsObject;          
			helpString = itsObject;
			helpString.LowerCase();
			if (helpString == NFmiString ("postscript") || helpString == NFmiString ("ps")|| helpString == NFmiString ("eps"))
               itsOutputMode = kPostScript;
			else if (helpString == NFmiString ("meta") || helpString == NFmiString ("metakieli")
							|| helpString.GetChars(1,5) == NFmiString ("magic"))
               itsOutputMode = kMetaLanguage;
			else if (helpString == NFmiString ("teksti"))
               itsOutputMode = kPlainText;
			else if (helpString == NFmiString ("xml"))
               itsOutputMode = kXml;
			else
			{
				if (itsLogFile)
				   *itsLogFile << "*** ERROR: Tuntematon tulostusmuoto: " << (char*)helpString << endl; 
			}

			ReadNext();
			break;
		}
		case dMagicSavePath:    //8.12.00
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

			itsOutFileName = ReadString(); //290199
//			*itsLogFile << "Tulostiedosto: " << (char*)itsOutFileName << endl; //

			ReadNext();
			break;
		}
		case dProductWithTimeStamp:  //080299
		{
			if (!ReadEqualChar())
				break;

			itsOutFileName = ReadString(); //290199

			fTimeStamp = true;

			ReadNext();
			break;
		}
		case dProductWithDataTimeStamp:  //050399
		{
			if (!ReadEqualChar())
				break;

			itsOutFileName = ReadString(); 

			fDataTimeStamp = true;

			ReadNext();
			break;
		}
		case dProductNameFormat:  //160300
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

			NFmiString* epsName;

			epsName = new NFmiString(ReadString()); //290199

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

			itsOutDir = ReadString(); //16.3.00 jäsen

			ReadNext();
			break;
		}
		case dMaskFile:  //12.9.01
		{
			if (!ReadEqualChar())
				break;

			itsMaskFileName = ReadString();
			itsMaskFileName.LowerCase(); 
			
			NFmiQueryData* newData = new NFmiQueryData();
			NFmiNamedQueryData* newNData = new NFmiNamedQueryData(newData,itsMaskFileName);
			itsDatas.Add(newNData);
/*
			if(itsMaskIter)   //12.9.01
			   delete itsMaskIter;
//		    if(DataByName(itsMaskFileName) && itsEnvironment.GetMaskNumber() > 0)
//				itsMaskIter = new NFmiSuperSmartInfo(DataByName(maskFileName)); 
				itsMaskIter = new NFmiFastQueryInfo(newData); 
//			else
//				itsMaskIter = 0;
*/
			ReadNext();
			break;
		}
		case dDataFile:
		{
			if (!ReadEqualChar())
				break;

			itsDataFileName = ReadString();
			itsDataFileName.LowerCase(); //6.2.01

			if(fDataRead)
			{
				*itsLogFile << "*** ERROR: Data " << (char*)itsDataFileName << "pitää antaa ennen 'KäytäDatanAlkuAikaa' ja 'SuhteellinenTuntiDatasta'" << endl; 
				*itsLogFile << "            " << "sekä kaikkia jäsenelementtejä" << endl; 
				*itsLogFile << "       KESKEYTETÄÄN" << endl; 
				return false;
			}
			
			NFmiQueryData* newData = new NFmiQueryData();
			NFmiNamedQueryData* newNData = new NFmiNamedQueryData(newData,itsDataFileName);
			itsDatas.Add(newNData);

			ReadNext();
			break;
		}
		case dChangeScandinavian:    //17.1.99
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

		case dPageSize:    //9.9  //200199 Pysty/Vaaka nimitys
		{
			if (!ReadEqualChar())
				break;

			*itsDescriptionFile >> itsObject;
			helpString = itsObject;
			if (helpString == NFmiString ("A4") || helpString == NFmiString ("a4")
				|| helpString == NFmiString ("A4Pysty"))
               itsPageSize = kA4;
			else if (helpString == NFmiString ("A4Maisema") || helpString == NFmiString ("a4maisema")
			     || helpString == NFmiString ("A4Vaaka"))
               itsPageSize = kA4Maisema;
			else if (helpString == NFmiString ("A3Maisema") || helpString == NFmiString ("a3maisema")
				      || helpString == NFmiString ("A3Vaaka"))
               itsPageSize = kA3Maisema;
			else if (helpString == NFmiString ("A3") || helpString == NFmiString ("a3")
				      || helpString == NFmiString ("A3Pysty"))
               itsPageSize = kA3; //HUOM toistaiseksi
			else if (helpString == NFmiString ("Letter") || helpString == NFmiString ("letter"))
               itsPageSize = kLetter;
			else
				*itsLogFile << "*** ERROR: Tuntematon arkkikoko: " << (char*)helpString << endl; 

			ReadNext();
			break;
		}
		case dSubViews:
		{
			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))
               itsNumberOfMaps = (unsigned short)long1;

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
		case dProductNameTimeFormat:	//22.3.2000 	  
		{
			if (!ReadEqualChar())
				break;

			itsNameTimeFormat = ReadTimeFormat(); //4.4.00
			ReadNext();
			break;
		}
		case dSecondProductNameTimeFormat:	//22.3.2000 	  
		{
			if (!ReadEqualChar())
				break;

			itsSecondNameTimeFormat = ReadTimeFormat(true); //27.10.00
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
					{    //menis ehkä suoremmin mutta tämä on yhteensopivaa vanhan koodin kanssa
						NFmiTime tim = NFmiTime((short)long3, (short)long2, (short)long1);
						NFmiTime thisDay;
						firstDeltaDays = tim.DifferenceInDays(thisDay);
        				itsFirstPlotTime.ChangeByDays(firstDeltaDays-origFirstDeltaDays); 
					}
				}
			}

			ReadNext();
			break;
		}
		case dDataTime:     //050199
		{
			ReadData();

			NFmiQueryData* firstData = FirstData();
			if(firstData)
			{
				firstData->Reset();
				firstData->NextTime();
				itsFirstPlotTime = firstData->Time();
			}
			else
			{
				*itsLogFile << "*** ERROR: Pitää antaa datat ennen 'KäytäDatanAlkuAikaa'"  << endl; 
				*itsLogFile << "       KESKEYTETÄÄN"  << endl; 
				return false; 
			}

			ReadNext();
			break;
		}
		case dRelHour:    //dRelativeHours:    //19.1.01 on pressTimeDescr:ssa
		{
			if (!ReadEqualChar())
				break;
			ReadTwo(long1, long2);
			itsFirstPlotTime = NFmiMetTime(60); //huom tunnin res.
			itsFirstPlotTime.ChangeByHours(long1);
			itsFirstPlotTime.NextMetTime(long2*60);

			*itsDescriptionFile >> itsObject;
			valueString = itsObject;
		    if(valueString.IsNumeric()) //7.2.01
			{
				//lasketulla itsFirstPlotTime:llä ei tee mitään
				long3 = (int)valueString;
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
		case dRelativeDataTime:     //030399
		{
			SetOne (long1);

			ReadData();

			NFmiQueryData* firstData = FirstData();
			if(firstData)
			{
				firstData->Reset();
				firstData->NextTime();
				itsFirstPlotTime = firstData->Time();
				itsFirstPlotTime.ChangeByHours(long1);
			}
			else
			{
				*itsLogFile << "*** ERROR: Pitää antaa datat ennen 'SuhteellinenTuntiDatasta'"  << endl; 
				*itsLogFile << "       KESKEYTETÄÄN"  << endl; 
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
               firstDeltaDays = (short)(long1 + itsEnvironment.GetDayAdvance()); //23.5.02 
			   itsFirstPlotTime.ChangeByDays(JustifyByLeaps(firstDeltaDays)-origFirstDeltaDays); 
			}

			ReadNext();
			break;
		}
		
/*		case dRelYear:     //220199 EI TOIMI!!!!!
		{
			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))
			{
				firstDeltaYears = (short)long1; 
        		itsFirstPlotTime.ChangeByDays((firstDeltaDays+firstDeltaYears*365)-origFirstDeltaDays); 
			}
			ReadNext();
			break;
		}
		*/
		case dHour:       
		{
			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))
               firstPlotHours = (unsigned short)long1;
            itsFirstPlotTime.SetTime(firstPlotHours); 

			ReadNext();
			break;
		}
/*  pois 19.1.01
		case dRelHour: //12.6  EI ONNAA; ON OLEMASSA KäytäDatanAlkuAikaa
		{         

			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))
               firstPlotHours = (unsigned short)long1;
			itsFirstPlotTime = NFmiMetTime();
            itsFirstPlotTime.ChangeByHours(firstPlotHours); 

			ReadNext();
			break;
		}
	*/
		case dHourStep:       
		{
			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))
               hourStep = (unsigned short)long1;

			ReadNext();
			break;
		}
		case dMapDefSize:
		{
			if (!ReadEqualChar())
				break;

			if(Read4Double(xmin,ymin,xmax,ymax))   
			{
				if(xmin == xmax || ymin == ymax)   //2.10.00
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

			if(Read4Double(xmin,ymin,xmax,ymax))   //15.1
			{
				if(xmin == xmax || ymin == ymax)   //2.10.00
					*itsLogFile << "*** ERROR: sijoitusalueen min == max"  << endl;
				else
					itsPlottingRect.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
            }
			ReadNext();
			break;
		}
		case dProductSizeFactor:  //110299
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
	/*	case dProductPlaceMove:  //240299   EI TOIMI
		{
			if (!ReadEqualChar())
				break;
			if(Read2Double(r1, r2))   
			{
		           itsScale.SetStartScales(NFmiRect(NFmiPoint(0.,0.), NFmiPoint(100.,100.)));
				itsPlottingRect.Set(NFmiPoint(r1, r2), NFmiPoint(100.+r1,100.+r2));			
			}
			ReadNext();
			break;
		} */
		case dBoundingBorder:  //15.4
		{
			if (!ReadEqualChar())
				break;

			if(Read4Double(xmin,ymin,xmax,ymax))   
			{
				if(xmin == xmax || ymin == ymax)   //2.10.00
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
				if(xmin == xmax || ymin == ymax)   //2.10.00
					*itsLogFile << "*** ERROR: rajauksen min == max"  << endl;
				else
					itsClippingRect.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
            }
			ReadNext();
			break;
		}
		case dProducer:  //12.1
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
			ReadData(); //24.8.99 ennen jäsenoilioita
            itsCurrentMap = 1;
			Scale();
			NFmiPressParam* newParam = new NFmiPressParam (itsScale
				                                          ,(short)firstDeltaDays
														  ,(short)firstPlotHours
														  ,(short)firstPlotMinutes
														  ,(short)hourStep    
														  ,producer
														  ,this);
			newParam->SetDataName(FirstDataName());  //030399
            newParam->SetEnvironment(itsEnvironment); 
			newParam->SetHome(GetHome());
			newParam->SetData(FirstData());          
			newParam->SetMaskIter(itsMaskIter); //12.9.01         
			newParam->SetLogFile(itsLogFile);
			newParam->SetDescriptionFile(itsDescriptionFile); //31.8.01
			newParam->SetLanguage(itsLanguage);   
			newParam->SetTime(itsFirstPlotTime);
			newParam->SetNewGeoArea(itsArea);              //23.12.00
			if(newParam->ReadDescription(itsString)) 
			{
				itsParams.Add(newParam);
				fMakeElementsAfterSegments = true; //25.1.01; jos muita elementtejä määrittelyn lopussa
			}                                      //         ne myös piirretään viimeisenä
			else
				delete newParam;

			itsIntObject = ConvertDefText(itsString);

			break;
		}
		case dSymbolPlaces:
		{
 			ReadData(); //24.8.99 ennen jäsenoilioita

			itsCurrentMap = 1;
			Scale();
			NFmiPressSameSymbols* newSameSymbols = new NFmiPressSameSymbols(itsScale);
            newSameSymbols->SetHome(GetHome()); 
			newSameSymbols->SetLogFile(itsLogFile);
			newSameSymbols->SetDescriptionFile(itsDescriptionFile); //31.8.01
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
			ReadData(); //24.8.99 ennen jäsenoilioita
			NFmiPsSymbol* image = new NFmiPsSymbol;
			image->SetWriteLast(fMakeElementsAfterSegments); //25.1.01
            image->SetHome(GetHome()); 
		    image->SetLogFile(itsLogFile);
			image->SetDescriptionFile(itsDescriptionFile); //31.8.01
			if(image->ReadDescription(itsString))
				 itsObjects.Add(image);
			else
				delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dSubImage:  
		{
			ReadData(); //24.8.99 ennen jäsenoilioita
			NFmiPressImage* image = new NFmiPressImage;
//			image->SetWriteLast(fMakeElementsAfterSegments); //25.1.01
			image->SetScale(itsScale);   
		    image->SetHome(GetHome()); 
            image->SetLogFile(itsLogFile);
			image->SetDescriptionFile(itsDescriptionFile); //31.8.01
			if(image->ReadDescription(itsString))
				 itsObjects.Add(image);
			else
				delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dNameDay:    //25.1.2000  
		{
			ReadData();
			if(!itsNameDay)
				itsNameDay = new NFmiNameDay;

			NFmiPressNameDay* text = new NFmiPressNameDay;
			text->SetWriteLast(fMakeElementsAfterSegments); //25.1.01
            text->SetEnvironment(itsEnvironment); 
			text->SetNameDay(itsNameDay);
            text->SetHome(GetHome()); 
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
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
			ReadData(); //24.8.99 ennen jäsenoilioita
			NFmiPressText* text = new NFmiPressText;
			text->SetWriteLast(fMakeElementsAfterSegments); //25.1.01
            text->SetEnvironment(itsEnvironment); 
            text->SetHome(GetHome()); 
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
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
			ReadData(); //24.8.99 ennen jäsenoilioita
			NFmiPressText* text = new NFmiPressText; 
			text->SetWriteLast(fMakeElementsAfterSegments); //25.1.01
            text->SetEnvironment(itsEnvironment); 
			text->SetScale(itsScale); 
            text->SetHome(GetHome()); 
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
			text->SetLanguage(itsLanguage); 
			if(text->ReadDescription(itsString))
				itsObjects.Add(text);
			else
				delete text;

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dTimeTextObject:  
		// 2.4 tehty uudelleen NFmiPressGivenTimeTextinä
			// jotta riippumaton QD:sta ja sen ajoista			
		{
		
			ReadData(); //24.8.99 ennen jäsenoilioita
			NFmiPressGivenTimeText* text = new NFmiPressGivenTimeText;
			text->SetWriteLast(fMakeElementsAfterSegments); //25.1.01
            text->SetEnvironment(itsEnvironment); 
            text->SetHome(GetHome()); //25.9
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
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
		
			ReadData(); //24.8.99 ennen jäsenoilioita
			NFmiPressComputerTimeText* text = new NFmiPressComputerTimeText;
			text->SetWriteLast(fMakeElementsAfterSegments); //25.1.01
            text->SetEnvironment(itsEnvironment); 
            text->SetHome(GetHome()); 
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
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
			itsArea.SetDescriptionFile(itsDescriptionFile); //31.8.01
//			itsArea.SetXyRequest(true); //18.5.01 tuotetasolla ei tiedetä onko alueoperaatioihin     
			itsArea.ReadDescription(itsString);
			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dMakeElementsAfterSegments:   //25.1.01
		{
			fMakeElementsAfterSegments = true;
			ReadNext();
			break;
		}
		case dWeekdayDir:   //21.5.02 direktiivitieto, ei virkaa täällä mutta veisi aikaa poistaa commentstripperillä
		{
			helpString = ReadString();
			helpString = ReadString();
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

/* siirretty writePs-puolelle
	if(outDir.IsValue())  
	{
		//kirjoitus hakemist esim ->Puolivalmiit
	}
	itsOutFile += NFmiString("/");
	//miksi kaksi pistettä kun raahataan ikonin päälle
//	*itsLogFile << "**** itsOutFileName: " << ( char*)itsOutFileName << endl;  
	itsOutFile += itsOutFileName;

	if(timeStamp)
	{
		NFmiMetTime computerTime = NFmiMetTime(1);
	    NFmiMetTime localTime(computerTime.LocalTime(25.f),1); 
	    NFmiString timeStampT = localTime.ToStr(kYYYYMMDDHH);//170299
		itsOutFile += timeStampT;
	}
 	else if(dataTimeStamp) //050499
	{
	    NFmiString timeStampT = NFmiMetTime(itsFirstPlotTime.LocalTime(25.),1).ToStr(kYYYYMMDDHH);
		timeStampT += NFmiString("00");
		itsOutFile += timeStampT;
	}

//	itsOutFile += NFmiString(".eps");   //14.3.00 siirretty writePs-puolelle, jotta voidaan erottaa eps/macro
	itsOutFile.NormalizeDelimiter();
	if(fChangeScandinavian)        //17.1.00 skandit pois jos Unixiin
		itsOutFile.ChangeScandinavian();

	if(outDir.IsValue())  
	{
		itsOutFile.ReplaceDirectory(outDir);
	}
     *itsLogFile << "Output: " << (char*)itsOutFile << endl; 
*/
	if(itsLogFile)
	{
	   *itsLogFile << "Lopetusrivi (pitää olla #LOPPU): " << ( char*)itsString << endl;  
       if(itsString.GetLen() >= 2 &&                                 
		     NFmiString(itsString.GetCharsPtr(1,2)) != NFmiString("#E") 
	      && NFmiString(itsString.GetCharsPtr(1,2)) != NFmiString("#L"))
	      *itsLogFile << "*** ERROR: MÄÄRITTELYTIEDOSTON LUKEMINEN KESKEYTETTY ?" << endl;
	   *itsLogFile << "TUOTETIEDOSTO LUETTU" << endl;
	}
	return true;	
}
//---------------------------------------------------------------------------------------
int NFmiPressProduct:: ConvertDefText(NFmiString & object)
{
														//"Oikeat" tässä alla
	NFmiString lowChar = object;
	lowChar.LowerCase(); //20.3.00 kaikille pitäisi sallia vapaa isot/pienet kirj.

	if(lowChar==NFmiString("product")           || lowChar==NFmiString("tuote")
	/*	|| object==NFmiString("TuoteNimi")|| object==NFmiString("Tuotenimi") */)
		return dProduct;
	else if(lowChar==NFmiString("tuoteniminumerointi"))  //5.9.00
		return dNumberAddingToName;           
	else if(lowChar==NFmiString("outputmode")     || lowChar==NFmiString("tulosmuoto")
		                   || lowChar==NFmiString("tulostusmuoto"))
		return dOutputMode;                  //20.6.2000
	else if(lowChar==NFmiString("#nameday")     || lowChar==NFmiString("#nimipäivä"))
		return dNameDay;                     //25.1.00
	else if(lowChar==NFmiString("deletescandinavianfromproductname")|| lowChar==NFmiString("poistaskandittuotenimestä")
		                                                            || lowChar==NFmiString("poistaääkkösettuotenimestä"))
		return dChangeScandinavian;           //17.1.00
	else if(lowChar==NFmiString("timestamp")    || lowChar==NFmiString("tuoteaikaleimalla")) 
		return dProductWithTimeStamp;
	else if(lowChar==NFmiString("datatimestamp")    || lowChar==NFmiString("tuotedatanaikaleimalla")) 
		return dProductWithDataTimeStamp;
	else if(lowChar==NFmiString("productnameformat") || lowChar==NFmiString("tuotenimiformaatti")) 
		return dProductNameFormat;             //160300
	else if(lowChar==NFmiString("productnametimeformat")||lowChar==NFmiString("tuotenimiaikaformaatti")) 
		return dProductNameTimeFormat;           //220300
	else if(lowChar==NFmiString("secondproductnametimeformat")||lowChar==NFmiString("toinentuotenimiaikaformaatti")) 
		return dSecondProductNameTimeFormat;           //271000
	else if(lowChar==NFmiString("timestampformat")    || lowChar==NFmiString("aikaleimaformaatti")) 
		return dTimeStampFormat;      //040399
	else if(lowChar==NFmiString("date")             || lowChar==NFmiString("päivämäärä")) 
		return dDate;
	else if(lowChar==NFmiString("usedatastarttime") || lowChar==NFmiString("käytädatanalkuaikaa")) 
		return dDataTime;    //050199
	else if(lowChar==NFmiString("relativehoursfromdata")|| lowChar==NFmiString("suhteellinentuntidatasta")) 
		return dRelativeDataTime;    //030399
//	else if(lowChar==NFmiString("relativehours")|| lowChar==NFmiString("suhteellinentunti")) 
//		return dRelativeHours;    //251099
	else if(lowChar==NFmiString("outdir")       || lowChar==NFmiString("tulostus")) 
		return dOutDir;
	else if(lowChar==NFmiString("pagesize")     || lowChar==NFmiString("arkki")) 
		return dPageSize;
	else if(lowChar==NFmiString("palette")      || lowChar==NFmiString("paletti")) //10.6
		return dPalette;
	else if(lowChar==NFmiString("mapfile")      || lowChar==NFmiString("pohja"))
		return dMapFile;
	else if(lowChar==NFmiString("datafile")     || lowChar==NFmiString("data"))
		return dDataFile;
	else if(lowChar==NFmiString("maskfile")     || lowChar==NFmiString("maskidata")) //12.9.01
		return dMaskFile;
	//25.3 #-objektit lisätty
	else if(lowChar==NFmiString("#constimage")  || lowChar==NFmiString("#vakiosymboli"))
		return dImageObject;                                  
	else if(lowChar==NFmiString("#subimage")    || lowChar==NFmiString("#osakuva"))
		return dSubImage;
	else if(lowChar==NFmiString("#consttext")   || lowChar==NFmiString("#vakioteksti"))
		return dTextObject;
	else if(lowChar==NFmiString("#columntext")  || lowChar==NFmiString("#palstateksti"))//vanha
		return dColumnTextObject;
	else if(lowChar==NFmiString("#timetext")    || lowChar==NFmiString("#aikateksti")
		 || lowChar==NFmiString("#aika"))
		return dTimeTextObject;
	else if(lowChar==NFmiString("#computertime")|| lowChar==NFmiString("#koneenaika")
		                                        || lowChar==NFmiString("#tekoaika"))  //27.9.01
		return dComputerTimeTextObject;
	else if(lowChar==NFmiString("plottingview") || lowChar==NFmiString("sijoitusalue"))
		return dPlottingView;
	else if(lowChar==NFmiString("mapdefsize")   || lowChar==NFmiString("mittaalue"))
		return dMapDefSize;
	else if(lowChar==NFmiString("sizefactor")   || lowChar==NFmiString("kokokerroin"))
		return dProductSizeFactor;
//	else if(lowChar==NFmiString("PlaceMove") || lowChar==NFmiString("PaikanSiirto")
//		|| lowChar==NFmiString("Paikansiirto"))     EI TOIMI
//		return dProductPlaceMove;
	else if(lowChar==NFmiString("clippingrectangle")|| lowChar==NFmiString("rajaus") 
		|| lowChar==NFmiString("leikkausalue"))
		return dClippingRectangle;
	else if(lowChar==NFmiString("boundingborder")|| lowChar==NFmiString("gifrajaus")
		|| lowChar==NFmiString("rajaus")) //15.4
		return dBoundingBorder;
	else if(lowChar==NFmiString("mapisreduced") 
		|| lowChar==NFmiString("pohjaonriisuttu")|| lowChar==NFmiString("pohjaonps")) //25.3
		return dMapIsReduced;
	else if(lowChar==NFmiString("subviews")     || lowChar==NFmiString("lukumäärä"))
		return dSubViews;
	else if(lowChar==NFmiString("margin")       || lowChar==NFmiString("reunus"))
		return dMargin;
	else if(lowChar==NFmiString("gap")          || lowChar==NFmiString("väli"))
		return dGap;
//	else if(lowChar==NFmiString("language")     || lowChar==NFmiString("kieli"))
//		return dLanguage;
	else if(lowChar==NFmiString("#pressparam")  || lowChar==NFmiString("#segmentti")
		        || lowChar==NFmiString("#parametriasemat")) //11.4.00 takaisin, oli vanhoissa reittisäissä
		return dPressParam;
	else if(lowChar==NFmiString("#constplaces") || lowChar==NFmiString("#vakiopaikat"))       
		return dSymbolPlaces;
	else if(lowChar==NFmiString("magicsavepath")|| lowChar==NFmiString("metapolku")
		                                  || lowChar==NFmiString("magicpolku")) //8.12.00
		return dMagicSavePath;
	else if(lowChar==NFmiString("#map")|| lowChar==NFmiString("#kartta")) //17.12.00
		return dMapArea;
	else if(lowChar==NFmiString("makeelementsaftersegments")|| lowChar==NFmiString("teeelementitsegmenttienjälkeen")) //17.12.00
		return dMakeElementsAfterSegments;
	else if(lowChar==NFmiString("#weekday") ||     lowChar==NFmiString("#viikonpäivä")) //21.5.02      
		return dWeekdayDir;
	else
		return NFmiPressTimeDescription::ConvertDefText(object);
}

//----------------------------------------------------------------------------
void NFmiPressProduct::ReadPalette(void) 
{
    itsPalette = new NFmiCmykColorBag;
	NFmiNamedCmykColor color;
	char object[255];
	NFmiString name;

//Mikan vaihdos
	NFmiString palFileName = NFmiString("..");
	palFileName += kFmiDirectorySeparator;
	palFileName += "LyhytPaletit";
	palFileName += kFmiDirectorySeparator;

//  NFmiString palFileName = NFmiString("..\\LyhytPaletit\\");
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
//----------------------------------------------------------------------------
bool NFmiPressProduct::WritePS(FmiPressOutputMode theGivenOutput) 
{
   itsNumOfWritePS++;  
   FmiPressOutputMode output = theGivenOutput;
   if(itsOutputMode != kPostScript)
	   output = itsOutputMode;

   if(itsCurrentDataIter)
	   delete itsCurrentDataIter;
   if(FirstData())
		itsCurrentDataIter = new NFmiSuperSmartInfo(FirstData()); //3.9.01
//		itsCurrentDataIter = new NFmiFastQueryInfo(FirstData()); 
	else
        itsCurrentDataIter = 0;

//	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsObjects); 
//	NFmiPressScaling *object;

	NFmiString endFileName;// = NFmiString("..\\Muut\\");
	endFileName = GetHome();
//Mikan vaihdos
	endFileName += kFmiDirectorySeparator;
	endFileName += NFmiString("Muut");
	endFileName += kFmiDirectorySeparator;

//	endFileName += NFmiString("\\Muut\\");
	endFileName += NFmiString("endA4.eps");   
	ifstream endFile(endFileName, ios::in|ios::in); 

	NFmiString startFileName;
    
	startFileName = GetHome();
//Mikan vaihdos
    startFileName += kFmiDirectorySeparator;
    startFileName += NFmiString("Muut");
    startFileName += kFmiDirectorySeparator;
//    startFileName += NFmiString("\\Muut\\");

//	startFileName += NFmiString("startEpsTavu3.ps");   
//	startFileName += NFmiString("startA4.eps");  //*********9.9 
//	startFileName += NFmiString("startA3Maisema.eps");  //*********9.9 
	if(itsPageSize == kA3Maisema)                                   //*********9.9
		startFileName += NFmiString("startEpsA3Maisema.ps");  //*********9.9
	else if(itsPageSize == kA3)
		startFileName += NFmiString("startEpsA3Potretti.ps");
	else if(itsPageSize == kLetter)
		startFileName += NFmiString("startEpsLetter.ps");
	else if(itsPageSize == kA4Maisema)
		startFileName += NFmiString("startEpsA4Maisema.ps");  //210199
	else //A4Pysty
		startFileName += NFmiString("startEpsA4Potretti.ps");

// *** 16.3.00 tänne read-puolelta
//	itsOutFile += NFmiString("/");
	//miksi kaksi pistettä kun raahataan ikonin päälle
	if(!ConstructOutFileName()) //itsOutFileName; //160300
		return false;
/*

	itsOutFile.NormalizeDelimiter();
	if(fChangeScandinavian)        //17.1.00 skandit pois jos Unixiin
		itsOutFile.ChangeScandinavian();

	if(itsOutDir.IsValue())  
	{
		itsOutFile.ReplaceDirectory(itsOutDir);
	}
	*/
// *** 16.3.00 loppu
	
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
		else if(output==kPlainText) //4.1.01
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
			*itsLogFile << "Output: " << (char*)itsOutFile << endl; 

// 		outFile.open(itsOutFile, ios::out | ios::binary); //24.10.00 nyt jäsen
	}

	if(!startFile)   
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: StartPs-tiedostossa vikaa: " << (char*) startFileName << endl;
		return false;
	}

	if(!endFile)    
	{   
		if(itsLogFile)
		   *itsLogFile << "*** ERROR: LoppuPs-tiedostossa vikaa: " << (char*) endFileName << endl;
		return false;
	}

	if(!outFile)
	{
		if(itsLogFile)	
		   *itsLogFile << "*** ERROR: Output-tiedostossa vikaa: " << (char*) itsOutFile << endl;
		return false;
	}

	if(output==kPostScript)
	{
		if (itsBoundingBorder.IsEmpty())    
			NFmiCopyFile(startFile, outFile);
		else
			NFmiCopyFileCropping(startFile, outFile, itsBoundingBorder);
	}
	else if(output==kXml)   //15.3.00
	{
//		ostream saveCout = cout;
//		cout = outFile;

		//tästä se lähtee
		outFile << "$<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standlone=\"yes\"?>" << endl;
//		cout << "$<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standlone=\"yes\"?>" << endl;

		outFile << "<Name>" << (char*)itsOutFile.Header() << endl;
//		cout << "<Name>" << (char*)itsOutFile.Header() << endl;
		
//		cout = saveCout;
	}
	else if(output==kPlainText)   //4.1.01
	{
		/* Sonera ei halua mitään
//		ostream_withassign saveCout = cout;
//		cout = outFile;

		cout << (char*)itsOutFile.Header() << endl;
		outFile << (char*)itsOutFile.Header() << endl;
		
//		cout = saveCout;
		*/
	}
	else if(output==kMetaLanguage)   //15.3.00
	{
		WriteMetaInit(); 
	}

	startFile.close();
	startFile.clear();
	
	NFmiString mapFilePath;
	
    mapFilePath = GetHome();
//Mikan vaihdos
    mapFilePath += kFmiDirectorySeparator;
    mapFilePath += NFmiString("Pohjat");
    mapFilePath += kFmiDirectorySeparator;
//    mapFilePath += NFmiString("\\Pohjat\\");

	ifstream mapFile;

	itsCurrentMap=1;
//	if(itsCurrentData)
// 		itsCurrentData->First();
	if(itsCurrentDataIter)
 		itsCurrentDataIter->First();
	Scale();

	while(itsCurrentMap <= itsNumberOfMaps)//itsNumberOfMaps=koko tuote monistettu n kpl
	{
	// 3.4 luuppi, voi olla useampia EPSeja 
		NFmiVoidPtrIterator epsIter = NFmiVoidPtrIterator(itsEpsFileNames);
		epsIter.Reset();
		NFmiString* epsFileName;
		epsFileName = (NFmiString*)epsIter.Next();
		short numOfEps = 0;

		/************* pohjat ****************/
		while (epsFileName)
		{
			numOfEps++;
			if(output==kMetaLanguage)   
			{
				if(itsNumOfWritePS < 2)
				{
					NFmiString fileString("/var/www/share/pohjat/htv");//8.12.00 htv pitää parametrisoida

					outFile << endl << "SymbolPath " << (char*)fileString << endl;

					NFmiString epsFile = *epsFileName;
					epsFile.Add(NFmiString(".jpg"));
					outFile << "Symbol "  << (char*)epsFile; 
				}
			}
			else if(output==kXml)   
			{
				outFile << "<Background>"  << endl;
				outFile << "  <Set>Pohjat"  << endl;
				outFile << "  <File>" << (char*)*epsFileName  << endl;				
			}
			else
			{

				NFmiString fullEpsFileName = mapFilePath;
				fullEpsFileName += *epsFileName;
				fullEpsFileName += NFmiString(".eps");
				mapFile.open(fullEpsFileName, ios::in|ios::in);
				if(!mapFile.good() || mapFile.eof())  //5.2
				{
					if(itsLogFile)
					     *itsLogFile << "WARNING: Pohjaa ei ole: " << (char*)fullEpsFileName << endl;
		//			return false;    
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
						   NFmiCopyFileWithoutShowpage(mapFile, outFile);   //291099 writeps:n

					  else  //Epsiin voi laittaa nolla-BoundaryBording koska alussa on oikea ja maximi otetaan
						if(itsPalette)   //15.6  //22.6 ei käytössä
						   NFmiCopyFileCroppingAndColoring(mapFile, outFile, NFmiRect(NFmiPoint(0.,0.),    //15.6
																		NFmiPoint(0.,0.))
														, itsPalette);
						else
						   NFmiCopyFileCropping(mapFile, outFile, NFmiRect(NFmiPoint(0.,0.),
																		NFmiPoint(0.,0.)));
					  
					  NFmiWriteEPSEnd(outFile);
				   }
	  			   else
				   {
					  NFmiWritePSConcat(itsScale, outFile);
					  NFmiCopyFileWithoutShowpage(mapFile, outFile); //261099
					  NFmiWritePSEnd(outFile);
				   }
				   if(itsLogFile)
				      *itsLogFile << "Pohja kopioitu: " << (char*)fullEpsFileName << endl; //190599
				}
			} //14.3.00
			epsFileName = (NFmiString*)epsIter.Next();
			mapFile.close();
			mapFile.clear();
		}

	//25.3 vakio-objektit
    //2.4  itsData->Time(itsFirstPlotTime);

		if(output==kMetaLanguage && itsNumOfWritePS < 2)   //31.10.00
		{
//			ostream saveCout = cout;
//			cout = outFile;

			if(output==kMetaLanguage && numOfEps < 1)   //8.12.00
			{
				outFile << endl << "New " << itsBoundingBorder.Size().X() << " "
									   << itsBoundingBorder.Size().Y()  
									   << " Transparent" << endl;
			}

			outFile << endl << "Store pohja" << endl << endl;
//			cout = saveCout;
		}

		/*********** norm. objektit (myös osakuvat) jotka ennen segmenttejä ***********/
		if(!WriteScalingObjects(true, output))
			return false;

		/*********** vakiopaikat, jotka ennen segmenttejä ***********/
		if(!WriteSameSymbols(true, output))
			return false;

		if(itsCurrentDataIter) //23.8.99, 20.5 kaatuu ilman dataa
		{
	    //****************SEGMENTIT***************
			NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
			NFmiPressParam *param;
			paramIter.Reset();
			param = (NFmiPressParam*)paramIter.Next();
			short segNum = 1;
			while (param)
			{
				if(itsLogFile)
				   *itsLogFile << segNum << ". segmentti (" 
//				   << (char*)param->GetName()
				   << (char*)param->MakeLogComment()             //16.10.00
				               << ")" << endl;				
				if (!(param->WritePS(itsScale, outFile, output))) //13.3.00 
				{
					outFile.close();
					outFile.clear();
					mapFile.close();
					mapFile.clear();
					if(itsLogFile)
						*itsLogFile << "*** ERROR: param->WritePS() in NFmiPressProduct" << endl;
					return false;
				}
 				param = (NFmiPressParam*)paramIter.Next();
				segNum++;
			}
		}
		else  //2.11.00 tarvittaisko segmenttiä ilman dataa
		{     // jokatapauksessa kunnon varoitus
			*itsLogFile << "WARNING: Dataa ei löydy segmenteille" << endl;
		}
		/*********** vakiopaikat jotka segmenttien jälkeen ***********/
		
		if(!WriteSameSymbols(false, output))
			return false;

		/*********** norm. objektit,(myös osakuvat) jotka jälkeen segmenttejä ***********/
		if(!WriteScalingObjects(false, output)) //24.1.01 toiseen kertaan tänne segmenttien jälkeen 
			return false;

		StepMap();
		mapFile.close();
		mapFile.clear();
	}
	if(output == kPostScript)  
	{
		if (!itsBoundingBorder.IsEmpty())    
			NFmiWriteBoundingBox(outFile, itsBoundingBorder);  //23.11.01
		NFmiCopyFile(endFile, outFile);
		outFile.close();
		outFile.clear();
	}
	else if(output == kPlainText) //4.1.01
	{
		ConstructOutFileName();
//		cout << endl;      //6.8.01 Klasun toivomus, kuitenkin pois
//		cout << "LOPPU" << endl;  
	}
	else if(output == kMetaLanguage)
	{
		ConstructOutFileName();
//		cout << endl << "Save " << (char*)itsOutFile.Header() << ".gif" << endl;
		outFile << endl << "Save " << (char*)itsOutFile.Header() << ".gif" << endl;//22.3.02
	}

//	outFile.close(); //24.10.00 pois metafileltä
	NFmiTime tim;           

	if(UseFromStorage(1,false) != kFloatMissing)  //4.10.01	
 	   *itsLogFile << "WARNING: Unused values in storage queue 1"  << endl;
	if(UseFromStorage(2,false) != kFloatMissing)	
 	   *itsLogFile << "WARNING: Unused values in storage queue 2"  << endl;
	
    if(itsLogFile)
 	   *itsLogFile << "** " << (char*)itsOutFileName << " tehty "  << tim << endl; //25.3.02
// 	   *itsLogFile << "** Tuote tehty "  << tim << endl; 

	return true;
}
//----------------------------------------------------------------------------------
bool NFmiPressProduct::WriteMetaInit(void)// const 
{
//		ostream saveCout = cout;
//		cout = outFile;
	   //outFile on jäsen vaikkei its.. !!!

		if(itsNumOfWritePS < 2)
		{

			//tästä se lähtee
			outFile << "# Lehtisääohjelmalla tehtyä ImageMagick-ohjauskieltä" << endl;
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

//			NFmiFileString fileName = GetHome();
//			fileName += NFmiString("\\Symbolit\\");
//			outFile << "symbolpath " << (char*)fileName << endl;
//			outFile << "symbolpath " << (char*)NFmiString("Symbolit/HTV") << endl;

			outFile << "FontType X" << endl;
			outFile << "FlipY on" << endl;

//			fileName.ReplaceDirectory(NFmiString("Valmiit"));
//			outFile << "savepath " << (char*)NFmiString("Valmiit") << endl << endl;
//			outFile << "SavePath " << (char*)itsOutFileName << endl << endl;
			if (itsMagicSavePath.IsValue())
				outFile << "SavePath /var/www/share/saakartat/" 
				<< (char*)itsMagicSavePath << endl << endl;
			else
				*itsLogFile << "magicPolku puuttuu"  << endl; 

//			outFile << "align center half" << endl;
			outFile << "Compose Over" << endl;
			outFile << "Align Center" << endl;
			outFile << "TextAlign South" << endl;  //8.12.00 HUOM
//			outFile << "transparent #F781BE" << endl;
//			outFile << "transparent #C0C0C0" << endl; //photoshopin oletus

//			outFile <<endl << "store default.magic " << endl << endl;
		}
		else
		{
			outFile << endl <<"# *** UUSI KUVA *** " << endl << endl;
//			outFile <<"restore default.magic " << endl << endl;
			outFile <<"Restore pohja" << endl << endl;
		}
              // ei tarvi jos pohjaa on, siitä tulee koko
/*		outFile << "flip" << endl;		
		outFile << "new " << itsBoundingBorder.Size().X() << " "
			           << itsBoundingBorder.Size().Y() << endl << endl; 
*/
		
//		cout = saveCout;
		return true;
}
//20.11.01----------------------------------------------------------------------------
bool NFmiPressProduct::WriteSameSymbols(bool theDoPreSegments, FmiPressOutputMode theOutput) //otherwise postSegments 
{
	//kutsutaan kahteen kertaan: ennen ja jälkeen segmenttien

		NFmiVoidPtrIterator constIter = NFmiVoidPtrIterator(itsSameSymbols);
		NFmiPressSameSymbols *sameSymbols;
		constIter.Reset();
		sameSymbols = (NFmiPressSameSymbols*)constIter.Next();
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
//					 mapFile.close();
					 return false;
				}
			}
			sameSymbols = (NFmiPressSameSymbols*)constIter.Next();   
		}
	 return true;
}
//24.1.01----------------------------------------------------------------------------
bool NFmiPressProduct::WriteScalingObjects(bool theDoPreSegments, FmiPressOutputMode theOutput) //otherwise postSegments 
{
	//kutsutaan kahteen kertaan: ennen ja jälkeen segmenttien

	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsObjects); 
	NFmiPressScaling *object;

	 NFmiPressScaling* lastObject = 0;
	 double lastLineStep =0.;
	 double lineStep;
	 objectIter.Reset();
	 object = (NFmiPressScaling*)objectIter.Next();
	 while (object)  
	 {
		object->Set(itsScale, outFile);
        object->SetLastLineStep(lastLineStep);

		if(object->IsActive() //1.12.00 managerista hallittavissa
			&& object->IsToWriteLast() != theDoPreSegments) //24.1.01
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
		object = (NFmiPressScaling*)objectIter.Next();
        lastLineStep = lineStep;

		if((!object || !(object->IsInParagraph())) && itsOutputMode == kPostScript)//4.1.01 +kPostScript
			lastObject->WriteGRestore();
	 }
	 return true;
}
//----------------------------------------------------------------------------
bool NFmiPressProduct::Close() 
{
	if(itsLogFile)
	{
		NFmiFileString inFile(itsInFileName);
		NFmiString str(inFile.Header());
 		*itsLogFile << "** Loki suljettu: "  << (char*)str << endl; 
		itsLogFile->close();
		itsLogFile->clear();
	}
	return true;
}
//----------------------------------------------------------------------------
bool NFmiPressProduct::ConstructOutFileName() 
{
	char* hChar;
	NFmiString str;  

	hChar = getenv("lehtiOutDir");
	if(hChar == 0)
	{
	   str = GetHome();            
//Mikan vaihdos
	   str += kFmiDirectorySeparator;
	   str += "Valmiit";
//	   str += NFmiString("\\Valmiit");
	}
	else
	   str = hChar;

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
			if(timeFormat.IsValue())   //4.10.00
				timeStampT = pressLocalTime.InterpretToStr(timeFormat);
			else if(itsNameTimeFormat == kUndefined)
			{
				timeStampT = localTime.ToStr(kYYYYMMDDHH);//yhteensopivuus meneeseen
				timeStampT += NFmiString("00"); //13.10.00 tänne
			}
			else
				timeStampT = pressLocalTime.ToStr(itsNameTimeFormat);//22.3.00

			itsOutFile += timeStampT; //5.5.00

			timeFormat = itsSecondStringNameTimeFormat; //27.10.00
		}
 		else if(fDataTimeStamp) 
		{
			NFmiString timeStampT;
			NFmiPressTime pressTime(itsFirstPlotTime.LocalTime(25.));
			if(timeFormat.IsValue())   //4.10.00
				timeStampT = timeStampT = pressTime.InterpretToStr(timeFormat);
			else if(itsNameTimeFormat == kUndefined)     
				timeStampT = NFmiMetTime(itsFirstPlotTime.LocalTime(25.),1).ToStr(kYYYYMMDDHH);//yhteensopivuus meneeseen
			else
				timeStampT = pressTime.ToStr(itsNameTimeFormat);//22.3.00

			timeStampT += NFmiString("00"); //Minuutit !!??
			itsOutFile += timeStampT; //5.5.00
			timeFormat = itsSecondStringNameTimeFormat; //27.10.00
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
//				addString = StationName();
				addString = FirstSegmentName();
//				addString = FirstParamLocation().GetName();

				if(subString.FirstCharIsUpper())
					addString.FirstCharToUpper();
				else
					addString.LowerCase();
			}
			else if(lowerString == NFmiString("ase")) //else 30.3.00
			{
//				addString = StationName();
				addString = FirstSegmentName();
				addString = addString.GetChars(1,3);
//				addString = FirstParamLocation().GetName().GetChars(1,3);

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
				if(timeFormat.IsValue())    //3.10.00 ei testattu
					addString = localPressTime.InterpretToStr(timeFormat);
				else
					addString = localPressTime.ToStr(itsNameTimeFormat); //22.3.00
				
				timeFormat = itsSecondStringNameTimeFormat; //27.10.00

			}
			else if(lowerString == NFmiString("aikaleimautc"))  //5.6.02
			{
				NFmiMetTime computerTime = NFmiMetTime(1);
				//NFmiMetTime localTime(computerTime.LocalTime(25.f),1);
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
				if(timeFormat.IsValue())    //3.10.00 ei testattu
					addString = localPressTime.InterpretToStr(timeFormat);
				else
					addString = localPressTime.ToStr(itsNameTimeFormat);
				
				timeFormat = itsSecondStringNameTimeFormat; //27.10.00

			}
			else if(lowerString == NFmiString("datanaikaleimautc")) //5.6.02
			{
				NFmiPressTime localPressTime(itsFirstPlotTime/*.LocalTime(25.)*/);
				if(timeFormat.IsValue())   
					addString = localPressTime.InterpretToStr(timeFormat);
				else
					addString = localPressTime.ToStr(itsNameTimeFormat);
				
				timeFormat = itsSecondStringNameTimeFormat; 

			}
			else if(lowerString == NFmiString("analyysiaika") ||
				    lowerString == NFmiString("analyysiaikautc"))  //5.6.02
			{
				NFmiPressTime tim = ((NFmiMetTime)(FirstData()->OriginTime())); 

				if(timeFormat.IsValue())    
					addString = tim.InterpretToStr(timeFormat);
				else
					addString = tim.ToStr(itsNameTimeFormat);
				
				timeFormat = itsSecondStringNameTimeFormat; //27.10.00

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
					addString = NFmiString("esp");  //28.8.00				
				else if(itsLanguage == kGermany)
					addString = NFmiString("sak");  //28.8.00				
				else if(itsLanguage == kFrench)
					addString = NFmiString("ran");  //14.5.02				
				else if(itsLanguage == kChinese)
					addString = NFmiString("kii");				

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
					addString = NFmiString("espanja");	//28.8.00			
				else if(itsLanguage == kGermany)
					addString = NFmiString("saksa");	//28.8.00			
				else if(itsLanguage == kFrench)
					addString = NFmiString("ranska");	//14.5.02			
				else if(itsLanguage == kChinese)
					addString = NFmiString("kiina");				

				if(subString.FirstCharIsUpper())
					addString.FirstCharToUpper();
				else
					addString.LowerCase();

			}
			else
				addString = subString;

			name += addString;
		}
		itsOutFile += name; //11.4.00 ulkoa sisään
	}

	if(itsCurrentNumberToName != kLongMissing)  //1.9.00
	{
		itsOutFile += NFmiValueString(itsCurrentNumberToName);
		itsCurrentNumberToName++;
	}

	itsOutFile.NormalizeDelimiter();
	if(fChangeScandinavian)        //17.1.00 skandit pois jos Unixiin
		itsOutFile.ChangeScandinavian();

	if(itsOutDir.IsValue())  
	{
		itsOutFile.ReplaceDirectory(itsOutDir);
	}

	return true;
}
//----------------------------------------------------------------------------
NFmiString NFmiPressProduct::FirstSegmentName()
{
//	NFmiString name;
//	name = FirstParamLocation().GetName();

	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	if(param)
	{
		return param->GetName();
/*		NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(param->RefStationDepObjects());
		objectIter.Reset();
		NFmiPressStationText* object = (NFmiPressStationText*)objectIter.Next();
		if(object)
		{
			return object->StationName();
			object = (NFmiPressStationText*)objectIter.Next();
		}
	*/
	}
	return NFmiString("Ei ole");
}

/*
//----------------------------------------------------------------------------
NFmiString NFmiPressProduct::StationName()
{
//	NFmiString name;
//	name = FirstParamLocation().GetName();

	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	if(param)
	{
		NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(param->RefStationDepObjects());
		objectIter.Reset();
		NFmiPressStationText* object = (NFmiPressStationText*)objectIter.Next();
		if(object)
		{
			return object->StationName();
			object = (NFmiPressStationText*)objectIter.Next();
		}
	}
	return NFmiString("Ei ole");
}
*/
//----------------------------------------------------------------------------
NFmiStationPoint NFmiPressProduct::FirstParamLocation() 
//16.3.00 ekan segmentin eka asema
{
	NFmiVoidPtrIterator paramIter = NFmiVoidPtrIterator(itsParams);
	NFmiPressParam *param;
	paramIter.Reset();
	param = (NFmiPressParam*)paramIter.Next();
	if(param)
		return param->GetFirstStationPoint();

	return NFmiStationPoint();
}
//----------------------------------------------------------------------------
void NFmiPressProduct::Scale() 
{
	double oneWidth = (itsPlottingRect.Width() - 2.*itsMargin - (itsNumberOfMaps-1)*itsMapGap)/itsNumberOfMaps; //261197
	double oneHeight = itsPlottingRect.Height() - 2.*itsMargin;
	double xOffset = itsPlottingRect.Left() + itsMargin + (itsCurrentMap-1)*(itsMapGap+oneWidth);
	double yOffset = itsPlottingRect.Top() + itsMargin; //pitäisi olla Bottom()										//261197
	itsScale.SetEndScales(NFmiRect(NFmiPoint(xOffset,yOffset)
		                           ,NFmiPoint(xOffset+oneWidth,yOffset+oneHeight)));
}
//----------------------------------------------------------------------------
void NFmiPressProduct::StepMap() 
{
	itsCurrentMap++;
	Scale();
	if(itsCurrentDataIter)     //23.8.99 oli itsCurrentData
		itsCurrentDataIter->NextTime();
}


