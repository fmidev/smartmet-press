//© Ilmatieteenlaitos  Lasse
//  Original 21.3.2000
// Muutettu 070600/LW +asemanvaihto nimenmuutoksella
// Muutettu 270600/LW .prem tiedosto siirretty Muut-haaraan kutsuvan pre-tiedoston haarasta  
//                    piti tehd‰ kun Vespassa prem:it meniv‰t prosessointiin pre-tiedostoina
// Muutettu 060900/LW .prem tiedosto siirretty omaan Managerit-haaraan  
// Muutettu 301100/LW +dMan(De)activateFirst
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressManager.h"
#include "NFmiTiesaaAlueet.h"

#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiEnumConverter.h"


#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//----------------------------------------------------------------------------
bool NFmiPressManager::ReadDescriptionAndWrite(NFmiPressProduct& thePressProduct
											,FmiPressOutputMode theOutMode) 
{
	long long1,long2,statNum;
	unsigned long uLong;
	double lon, lat;
	NFmiString helpString;
	itsLogFile = thePressProduct.GetLogFile();
	FmiPressOutputMode outMode = theOutMode; 

	NFmiMetTime firstPlotTime(thePressProduct.GetFirstPlotTime()); 

//6.9.00 prem nyt omassa managerit-haarassa, aikaisemmin muut-haarassa
	NFmiFileString inputFileName = thePressProduct.GetHome();
    inputFileName += kFmiDirectorySeparator;
    inputFileName += NFmiString("Managerit");
    inputFileName += kFmiDirectorySeparator;
    inputFileName += NFmiString("nimi.prem");
	inputFileName.Header(static_cast<char *>(NFmiFileString(thePressProduct.GetInFileName()).Header()));

	*itsLogFile << "manageri olisi: " << static_cast<char *>(inputFileName) << endl;  //29.9.00

//    ifstream input(inputFileName, ios::in);
    itsDescriptionFile = new ifstream(inputFileName, ios::in);
	if(!itsDescriptionFile->good() || itsDescriptionFile->eof())
	{
		if(itsLogFile)
				*itsLogFile << "ei manageria" << endl;  
		return false;
	}
	else
		if(itsLogFile)
				*itsLogFile << "Manageri-tiedosto avattu: "<< static_cast<char *>(inputFileName) << endl;  

//    istream saveCin = cin;
//    cin = input;

	*itsDescriptionFile >> itsObject;
	itsString = itsObject;
	itsIntObject = ConvertDefText(itsString);

	bool changed = true; //sallitaan aloitus suoraan TeeTuotteesta
	
	while(itsIntObject != dEnd || itsCommentLevel) 
	{
	
	  if(itsLoopNum > itsMaxLoopNum)  
	  {
		if(itsLogFile)
			*itsLogFile << "*** ERROR: max length exceeded in the Manager" << endl;
//		retString = itsString;
		return false;
	  }
	  itsLoopNum++;
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  switch(itsIntObject)
	  {
	   case dOther:	  //ylim‰‰r‰ist‰ roinaa,
	   {
			if(itsLogFile)
				*itsLogFile << "*** ERROR: Tuntematon sana #Managerissa: " << static_cast<char *>(itsObject) << endl;  

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
			if(changed)
			{
				if(!thePressProduct.WritePS(outMode))
				{
					if(itsLogFile)
					    *itsLogFile << "*** ERROR: problems with the manager" << endl;  
					return false;
				}
			}
			else
				if(itsLogFile)
				    *itsLogFile << "*** ERROR: Identical product from manager, not processed" << endl;  

			changed = false;
			ReadNext();
			break;
		}
		case dManStation:	  
		case dManStationAndImages:  //29.9.00 myˆs osakuvien nimet muutetaan	  
		{
			bool isAlsoImages = itsIntObject == dManStationAndImages; //2.10.00 koska itsIntObject muuttuu 
			NFmiString string1;
			lon = 0.;
			lat = 0.;
			statNum = 0;
            
			if (!ReadEqualChar())
				break;
			     
			NFmiValueString valueString = ReadString();
		    if(valueString.IsNumeric())
			{
				statNum = static_cast<int>(valueString);
				string1 = ReadString();
	//			cin >> itsObject;
	//		    string1 = itsObject;
			}
			else
			{
				statNum = itsLoopNum; 
				string1 = valueString;
			}
			valueString = ReadString();
                                          
			if(valueString.IsNumeric())   
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

			if(isAlsoImages)     //2.10.00, ero n‰iden kahden v‰lill‰
				thePressProduct.SetImagePreNames(location);

			changed = true;

			break;
		}
		case dManStationNewName:        //7.6.00
		{
			NFmiString string1,string2;
			lon = 0.;
			lat = 0.;
			statNum = 0;
            
			if (!ReadEqualChar())
				break;
			     
			NFmiValueString valueString = ReadString();
		    if(valueString.IsNumeric())
			{
				statNum = static_cast<int>(valueString);
				string1 = ReadString();
	//			cin >> itsObject;
	//		    string1 = itsObject;
			}
			else
			{
				statNum = itsLoopNum; 
				string1 = valueString;
			}

			string2 = ReadString(); //t‰m‰ extraa edelliseen verrattuna

			valueString = ReadString();
                                          
			if(valueString.IsNumeric())   
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
			thePressProduct.SetStationRename(reName); //SetFirstStation:n j‰lkeen

			changed = true;

//			ReadNext();
			break;
		}
		case dManMaskNumber:	  //7.9.01
		{
			if (!ReadEqualChar())
				break;

			NFmiString maskName = ReadString();
			// Mika: Menee paremmin l‰pi n‰in
			string stdString(maskName);
			NFmiEnumConverter converter(kRoadRegions);
			uLong = converter.ToEnum(stdString);

			if(uLong != kTieAlueNone)
			{
				thePressProduct.SetMaskNumber(uLong);
				changed = true;
			}
			else
				*itsLogFile << "*** ERROR: Not valid Mask: " << static_cast<char *>(maskName) << endl;  

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
		case dManNumberAddingToName:	  
		{
			if(SetOne(long1))
				thePressProduct.ActivateNumberToName(long1);

			changed = true;

	//		ReadNext();
			break;
		}
		case dManDeleteNumberAddingToName:	  
		{
			thePressProduct.DeActivateNumberToName();

			changed = true;

	//		ReadNext();
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
		case dManProductFormat: //15.1.01	  
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
			{                 //8.9.00 j‰rj. muutettu
			*itsLogFile << "** Loki suljetaan managerista" << endl;  
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
			str.LowerCase();  //21.3.01 kaikkialla k‰sitell‰‰n pienin‰

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
			firstPlotTime.NextMetTime(long2*60);

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
			if (helpString == NFmiString ("postscript") || helpString == NFmiString ("ps")|| helpString == NFmiString ("eps"))
               outMode = kPostScript;
			else if (helpString == NFmiString ("meta") || helpString == NFmiString ("metakieli")
				        || helpString.GetChars(1,5) == NFmiString ("magic"))
               outMode = kMetaLanguage;
			else if (helpString == NFmiString ("teksti"))
               outMode = kPlainText;
			else if (helpString == NFmiString ("xml"))
               outMode = kXml;
			else
			{
				if (itsLogFile)
				   *itsLogFile << "*** ERROR: Tuntematon tulostusmuoto managerissa: " << static_cast<char *>(helpString) << endl; 
			}
//			changed = true; //ei kuitenkaan kesken kaiken voi muuttaa

			ReadNext();
			break;
		}
		case dEnd:	  
		{
			return true;
			break;
		}
	  }
	}
	return true;
}
//---------------------------------------------------------------------------------------
int NFmiPressManager:: ConvertDefText(NFmiString & object)
{														
	NFmiString lowChar = object;
	lowChar.LowerCase(); 

	if(lowChar==NFmiString("teetuote"))
		return dManWritePs;
	else if(lowChar==NFmiString("maski"))       //7.9.01
		return dManMaskNumber;           
	else if(lowChar==NFmiString("aktivoiekaelementti"))   //30.11.00
		return dManActivateFirst;           
	else if(lowChar==NFmiString("deaktivoiekaelementti"))  //30.11.00
		return dManDeactivateFirst;           
	else if(lowChar==NFmiString("tuotenimi"))
		return dManProduct;           
	else if(lowChar==NFmiString("tuotenimiformaatti"))      //15.1.01
		return dManProductFormat;           
	else if(lowChar==NFmiString("asema"))
		return dManStation;           
	else if(lowChar==NFmiString("asemajaosakuvat"))    //29.9.00
		return dManStationAndImages;           
	else if(lowChar==NFmiString("asemanime‰vaihtaen")) //7.6.00
		return dManStationNewName;           
	else if(lowChar==NFmiString("tuoteniminumerointi"))  //1.9.00
		return dManNumberAddingToName;           
	else if(lowChar==NFmiString("tuoteniminumerointipois"))  //1.9.00
		return dManDeleteNumberAddingToName;           
	else if(lowChar==NFmiString("suljeloki"))
		return dManCloseLog;           
	else if(lowChar==NFmiString("avaaloki"))   //ei kai toimi
		return dManOpenLog;           
/*	else if(lowChar==NFmiString("muutaaika"))
		return dManChangeTime; */
	else if(lowChar==NFmiString("data"))
		return dManData;
	else if(lowChar==NFmiString("tulostusmuoto"))
		return dManOutputMode;
	else if(lowChar==NFmiString("suhteellinentunti"))
		return dManRelativeHours;
	else if(lowChar==NFmiString("tuntimuutos"))
		return dManAddHours;
	else if(lowChar==NFmiString("kieli"))
		return dManLanguage;
	else
		return NFmiPressDescription::ConvertDefText(object);
}
