//© Ilmatieteenlaitos  Lasse
//  Original 21.10. 1997
// Muutettu 201197/LW symboliDirin välitys pois
// Muutettu 111297/LW oma QueryData myös
// Muutettu 121297/LW station yhdellä rivillä myös
//                    taulukon siirtymä automaattiseksi
// Muutettu 121297/LW peritty myös NFmiDescription:sta
// Muutettu 070198/LW Määrittelyt suomennettu
// Muutettu 120198/LW +itsProducer
//Muutettu 150198/LW ReadDouble() ym käyttö
//Muutettu 190198/LW 
//Muutettu 220198/LW asemat myös ilman lat/lon:ia
//Muutettu 020298/LW
//Muutettu 270298/LW siirto korjattu 
//Muutettu 280298/LW x-siirto yhdellä parametrillä 
//Muutettu 020398/LW +FindQDStationName()
//Muutettu 040398/LW taulukkoon myös x-siirto
//Muutettu 090498/LW aikabugi korjattu
//Muutettu 290698/LW + case dRelDay, dHour
//Muutettu 050798/LW + aikaTaulukko
//Muutettu 120898/LW aikataulukkoon mukaan tuntiaskel
//Muutettu 090998/LW ReadDescriptionin käyttö: ReadNext(),ReadEqualChar,itsObject,itsIntObject
//Muutettu 121098/LW Aseman nimi myös skaalauksessa oikein
//Muutettu 031198/LW aikaTaulukon skaalaus korjattu; lisätty itsUnscaledStepSize
//Muutettu 051198/LW +AsemaNimeäVaihtaen
//Muutettu 290199/LW Merkkijonot myös lainausmerkkeihin, saadaan välilyönti
//Muutettu 290199/LW Datan puuttuminen -> hallittu jatko seuraavaav segmenttiin
//Muutettu 050399/LW Välitön keskeytys kun data ja datariippuvat ajat väärässä järjestyksessä 
//Muutettu 150699/LW data sallitaan gridinä 
//Muutettu 160699/LW +fLonLatMode: sallii kahden Asema-luvun tulkitsemisen lon/lat:ksi 
//Muutettu 130899/LW gridin paikkapointterin asetus siirretty piirto-olioon
//                   +itsCurrentLonLat 
//Muutettu 230899/LW itsData nyt qInfo eikä qData 
//Muutettu 120400/LW +painetaulukko 
//Muutettu 030500/LW +datalle First() ennen käyttöä
//Muutettu 030500/LW estetty useamman #DataAlkion kutsuminen 
//Muutettu 150600/LW +dNewLevelPlace,dNewTimePlace 
//Muutettu 140800/LW aikataulukko suhteellisena siirtymänä
//Muutettu 140800/LW typeid käyttöön EI ONNISTUNUT VOID* listaan 
//Muutettu 161000/LW +dLogText, dLogTextAdd eli lokiin enemmän selitettä 
//Muutettu 060201/LW datatiedostoille LowerCase()
//Muutettu 030401/LW +fDataNotNeeded, mahdollistaa (aur.)aikojen teon ilman dataa 
//Muutettu 010801/LW tiedostomuodossa ensin asema ja sitten aika outputtiin 
//Muutettu 010801/LW jos säännölliset aikastepit voidaan jatkaa miten pitkälle tahansa 
//-----------------------------------------------------------------------------------

//#ifdef WIN32
//#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
//#endif
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include <fstream> //STD 27.8.01

#ifndef __NPRESSPA_H__
#include "NFmiPressParam.h"
#endif//__NPRESSPA_H__

#ifndef __NPRESSPR_H__
#include "NFmiPressProduct.h"
#endif//__NPRESSPR_H__

#ifndef __NPSYMBOL_H__
#include "NFmiPsSymbol.h"
#endif

#ifndef __NPTIMTEX_H__
#include "NFmiPressDataTimeText.h"
#endif

#ifndef __NPSTATEX_H__
#include "NFmiPressStationText.h"
#endif

#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

//#include <typeinfo.h>  //1.9.00
#include "NFmiLocationFinder.h"  //110899
#include "NFmiTotalWind.h"
#include "NFmiYKJArea.h"
#include "NFmiEquidistArea.h"
#include "NFmiPressArea.h"

#include "NFmiMultiParamMapping.h"
#include "NFmiEnumConverter.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//18.9.2001----------------------------------------------------------------------------
void NFmiPressParam::PutInStorage(const float value, int storageQueue)
// ei saa inliniksi koska PressProduktin ja PressParamin headerit ristissä ??
{  //the PressProduct has the storage
	itsPressProduct->PutInStorage(value, storageQueue);
};  
//18.9.2001----------------------------------------------------------------------------
float NFmiPressParam::UseFromStorage(int storageQueue)
{
	return itsPressProduct->UseFromStorage(storageQueue);
}            
//23.1.2001----------------------------------------------------------------------------
void NFmiPressParam::SetNewGeoArea(const NFmiPressArea& theArea) 
{   //HUOM GeoArea on NFmiArea joka PressArean jäsen
	NFmiArea *newArea = 0;
	if (theArea.GetArea())
	{
		newArea = theArea.GetArea()->Clone(); 
/*
		NFmiString str= theArea.GetArea()->ClassName();
		if(str == NFmiString("NFmiYKJArea"))
		{
//			NFmiYKJArea testArea(*(NFmiYKJArea*)theArea.GetArea());
			newArea = new NFmiYKJArea(*(NFmiYKJArea*)theArea.GetArea());
		}
		else if(str == NFmiString("NFmiPolSterArea"))
		{
			newArea = new NFmiPolSterArea(*(NFmiPolSterArea*)theArea.GetArea());
		}
		else if(str == NFmiString("NFmiStereographicArea"))
		{
			newArea = new NFmiStereographicArea(*(NFmiStereographicArea*)theArea.GetArea());
		}
		else if(str == NFmiString("NFmiEquiDistArea"))
		{
			newArea = new NFmiEquidistArea(*(NFmiEquidistArea*)theArea.GetArea());
		}
		else
			*itsLogFile << "*** ERROR: Unknown Area type tried to import from Product to Segment: " << (char*)str << endl;
*/
  }
	itsArea.SetArea(newArea);
}
//17.9.2001----------------------------------------------------------------------------
bool NFmiPressParam::ChangeMaskNumber(unsigned long theMask)
{
	if(!itsStations.IsLocations())
		return false;

	itsEnvironment.ChangeMaskNumber(theMask); 

	NFmiEnumConverter converter(kRoadRegions);
	string name = converter.ToString(itsEnvironment.GetMaskNumber());
	itsName = NFmiString(name);

	itsStations.First();
	
//	NFmiLocation* firstLocation= itsStations.Location(); //10.9.02
	NFmiLocation* firstLocation= const_cast<NFmiLocation*> (itsStations.Location());
	firstLocation->SetName(itsName);

//?	itsStations.Location();

//	CreateStationFromAreaMask();
	return true;
}
//17.3.2000----------------------------------------------------------------------------
bool NFmiPressParam::SetFirstStation(const NFmiLocation& theLocation)   
{
//	NFmiString semi(NFmiString(";"));
//	NFmiString equal(NFmiString("="));
	if(!itsStations.IsLocations())
		return false;
//	NFmiString newName;

	itsStations.First();
	NFmiLocation* firstLocation= const_cast<NFmiLocation*>(itsStations.Location()); //10.9.02
	firstLocation->SetLongitude(theLocation.GetLongitude());
	firstLocation->SetLatitude(theLocation.GetLatitude());

	firstLocation->SetName(theLocation.GetName());
	firstLocation->SetIdent(theLocation.GetIdent());

	//16.10.00 segmentin nimeen pitää ottaa asetettu jälkiosa (kaksoispisteen jälkeen tuleva)
/*	NFmiString oldName = itsName;
	unsigned long posOfSemi = oldName.Search(semi);
	unsigned long posOfEqual = oldName.Search(equal);
	if(posOfSemi > 0 && posOfEqual > 0 )
	{
		itsName = NFmiString("=");
		itsName += theLocation.GetName();
		itsName += oldName.GetChars(posOfSemi, oldName.GetLen()-posOfSemi+1);
	}
	else if(posOfEqual > 0)
	{
		itsName = NFmiString("=");
		itsName += theLocation.GetName();
	}
//	else pysyy entisellään
*/
	itsName = theLocation.GetName();   
	
	return true;
}
//17.3.2000----------------------------------------------------------------------------
bool NFmiPressParam::SetStationRename(const NFmiRenaming& theRenaming)   
{
	if(!itsStations.IsLocations())
		return false;

	//ainoastaan asemanNimiä
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsStationDepObjects);
	objectIter.Reset();
	NFmiPressStationText* object = (NFmiPressStationText*)objectIter.Next();
	while(object)
	{
		object->SetNewName(theRenaming);
		object = (NFmiPressStationText*)objectIter.Next();
	}
	itsName = theRenaming.newName; //12.6.00 samalla muuttuu itse olion nimi  
	
	return true;
}
//28.1.2000----------------------------------------------------------------------------
NFmiStationPoint NFmiPressParam::GetFirstStationPoint(void)   
{
	if(itsStations.First())
	{
		return *(NFmiStationPoint*)itsStations.Location();
	}
	else
		return NFmiStationPoint();
}
//----------------------------------------------------------------------------
NFmiPoint NFmiPressParam::GetFirstPoint(void)   
{
	if(itsStations.First())  //28.1.2000
	{
		NFmiStationPoint* statPoint = (NFmiStationPoint*)itsStations.Location();
		return statPoint->Point();
	}
	else
		return NFmiPoint();

/*	itsStations.First();
	NFmiStationPoint* statPoint = (NFmiStationPoint*)itsStations.Location();
	return statPoint->Point();
	*/
}
//----------------------------------------------------------------------------
bool NFmiPressParam::SetData(const NFmiString& dataName)   
{//22.3.00 oma metodi, ->myös managerin käytössä
	if(itsDataIter)   //24.8.99 tänne eikä writeps:ään koska tarvitaan                
		delete itsDataIter;   // pressScaling-objektien initialisoinnissa
	NFmiQueryData* data = itsPressProduct->DataByName(dataName);
	if(data)
	{
		//17.5.02: hidastaa noin 10% jos tehdään SuperSmartInfoja (muuten pitäisi
		// antaa määrittelyssä käytetäänkö SuperSmartia tai muuttaa data SuperSmartiksi
		// kun havaitaan että sitä tarvitaan)
		// kuitenkin noin 60% nopeutus kun SuperSmartit korvattu FastInfoilla
		// parametreissa ja muualla
		itsDataIter = new NFmiSuperSmartInfo(data);
//		itsDataIter = new NFmiFastQueryInfo(data);
		itsDataIter->First();
		itsDataName = dataName; //29.8.00
	}
	else
		itsDataIter = 0;


//vain PressDataTimeText näistä datariippuvainen
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsTimeDepObjects);
	NFmiPressScaling* scaleObject;
	objectIter.Reset();
	scaleObject = (NFmiPressScaling*)objectIter.Next();
//	NFmiPressDataTimeText* timeObject;
	while(scaleObject)
	{
		switch(scaleObject->ClassId())
		{
			case kNFmiPressDataTimeText:	  
			{
				((NFmiPressDataTimeText*)scaleObject)->SetData(itsDataIter);
			}
			case kNFmiNone:
			default:
			{
			}
		}
		scaleObject = (NFmiPressScaling*)objectIter.Next();
	}

	//ainoastaan asemanNimiä
	NFmiPressStationText* object;
	objectIter = NFmiVoidPtrIterator(itsStationDepObjects);
	objectIter.Reset();
	object = (NFmiPressStationText*)objectIter.Next();
	while(object)
	{
		object->SetData(itsDataIter);
		object = (NFmiPressStationText*)objectIter.Next();
	}
	return true;
}

//17.3.00----------------------------------------------------------------------------
bool NFmiPressParam::SetAllTimes(const NFmiMetTime& theTime)   
{
	SetTime(theTime);
	bool retCode;

/* ei näyttänyt olevan aikariippuvuutta???
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsObjects); 
	NFmiPressScaling *object;
	objectIter.Reset();
	object = (NFmiPressScaling*)objectIter.Next();
	while(object)
	{
		object->SetTime(theTime);
		object = (NFmiPressScaling*)objectIter.Next();
	}
*/
	retCode = itsSymbols.SetAllTimes(theTime);

	return retCode;
}
//17.3.00----------------------------------------------------------------------------
bool NFmiPressParam::SetAllLanguages(FmiLanguage theLanguage)   
{
	SetLanguage(theLanguage); //ensin oma
//	bool retCode;

	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsTimeDepObjects); 
	NFmiPressScaling *object;
	objectIter.Reset();
	object = (NFmiPressScaling*)objectIter.Next();
	while(object)
	{
		object->SetLanguage(theLanguage);
		object = (NFmiPressScaling*)objectIter.Next();
	}

	itsSymbols.SetAllLanguages(theLanguage);

	return true;
}
//----------------------------------------------------------------------------
bool NFmiPressParam::ReadDescription(NFmiString& retString) 
{
	NFmiPoint tableDirectPoints[kMaxNumOfTableElements];  //16.6.00

	bool isRelativeTable = false;             //14.8.00
	fTwoPartLogComment = false;     //16.10.00  sekä nimi että lisäkommenti
	NFmiString logName;						  //16.10.00		

	bool symbolGroupCalled = false;    //8.5.00 
	bool helpBool;
	bool firstStation = true;
	fIsTimeLoop = false;
	fIsLevelLoop = false;
    NFmiPoint stationStepSize;
	NFmiString dataFileName;
	NFmiValueString valueString;
	long long1, long2, long3;    
	unsigned short numOfTableStations, currentStationNum=0; //taulukoissa
	unsigned short currentStationNumOnMap = 0; //23.12.00
	NFmiString string1, string2;
	NFmiPoint point0, point1, point2,scaledTimeLast,unscaledTimeLast, firstUnscaledPoint;
	NFmiPoint unscaledTableMove, scaledTableMove;  //14.8.00
	double tableX1=0,tableX2, tableY1=0, tableY2, x, y;
	double xmin,xmax,ymin,ymax;
	bool timeOrLevelTableSet = false;
	long statNum;
	double lat, lon;

	NFmiVoidPtrList* newNames = new NFmiVoidPtrList; 

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

	while(itsIntObject != dEnd || itsCommentLevel) 
	{
	
	  if(itsLoopNum > itsMaxLoopNum)  
	  {
		if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Segmentissä" << endl;
		retString = itsString;
		return false;
	  }
	  itsLoopNum++;
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  switch(itsIntObject)
	  {
	   case dOther:	  //ylimääräistä roinaa,
	   {
			if(itsLogFile)
				*itsLogFile << "*** ERROR: Tuntematon sana #Segmentissä: " << (char*)itsObject << endl;  

			ReadNext(); //9.9 tämä korvaa seuraavat
//			*itsDescriptionFile >> itsObject;
//			itsString = itsObject;
//			itsIntObject = ConvertDefText(itsString);
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
			}  //huom jatkuu seuraavaan

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
               numberOfTimeSteps = (unsigned short)long1;

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
					  y = (double)valueString;
					  *itsDescriptionFile >> itsObject;
					  itsString = itsObject;
				    }
				    else
				    {
					   y = 0.;
					   itsString = valueString;
				    }
                point1.Set(x,y);
                stepSize = NFmiPoint(point1.X()*itsScale.GetScaling().X()
				                   ,point1.Y()*itsScale.GetScaling().Y());
				unscaledStepSize.Set(x,y);  //3.11
				}
				else
				{
					  *itsDescriptionFile >> itsObject;
					  itsString = itsObject;
				}

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dLevelTable: //12.4.00	 käytetään aikataulukon muuttujia  		 
		{   
			if (!ReadEqualChar())
				break;

			if(Read2Double(xmax,ymax))    
			{
				fIsLevelLoop = true;
				fIsTimeLoop = false;
				point1.Set(xmax,ymax);
				unscaledTimeLast.Set(xmax,ymax);      
				scaledTimeLast = itsScale.Scale(point1); //3.11
				scaledTimeLast = itsCurrentStationScale.Scale(scaledTimeLast); 
				timeOrLevelTableSet = true; 

				if(ReadLong(long1))
				{
					numberOfLevelSteps = FmiMin(kMaxNumOfTableElements,(unsigned short)long1);
				}
			}
				// itsStepSize joudutaan asettamaan luupin jälkeen alla
			ReadNext();
			break;
		}
		case dSegmentLevel: //12.4.00	  		 
		{   
			if (!ReadEqualChar())
				break;

			if(ReadOne(long1))    
			{
				if(++currentLevelInd > kMaxNumOfTableElements)
				{
					*itsLogFile << "*** ERROR: sallittu painepintamäärä ylitetty: " << numOfTableLevels << endl;
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
		case dLevelPlace: //15.6.00	  		 
		{   
			if (!ReadEqualChar())
				break;

			if(ReadOne(long1))    
			{
				if(++currentLevelInd > kMaxNumOfTableElements)
				{
					*itsLogFile << "*** ERROR: sallittu painepintamäärä ylitetty: " << numOfTableLevels << endl;
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
//						unscaledTimeLast.Set(x,y);      
//						scaledTimeLast = itsScale.Scale(point1); //3.11
//						scaledTimeLast = itsCurrentStationScale.Scale(scaledTimeLast); 
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
                scaledTimeLast = itsScale.Scale(point1); //3.11
				scaledTimeLast = itsCurrentStationScale.Scale(scaledTimeLast); 

				if(ReadLong(long1))
				{
					numberOfTimeSteps = (unsigned short)long1;
					if(numberOfTimeSteps >= kMaxNumOfTableElements) //1.8.01
					{
						*itsLogFile << "*** WARNING: taulukkoon varattu aika-askelmäärä ylitetty: " << numberOfTimeSteps << endl;
					}
					if(ReadLong(long1))       //12.8
					{
						itsHourStep = (unsigned short)long1;
						timeOrLevelTableSet = true; 
						if(itsHourStep < 24) //050399 oli 25???
							SetHourLoop(true);
						currentTotalTimeDiff = (numberOfTimeSteps-1) * itsHourStep;
						currentTimeInd = (short)numberOfTimeSteps;
					}
				}
			}
				// itsStepSize joudutaan asettamaan luupin jälkeen alla
			ReadNext();
			break;
		}
		case dTimeTableRelative:
		// 14.8.00 taulualkioiden siirtymä joka kerran (ei siis vikan suht. paikka)
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
//8.5.02 asemamittakaava ei saa vaikuttaa mutta koko tuotteen vaikuttaa
//       eli siirtymät on mitattava ilman globaalia skaalausta
//              scaledTableMove = itsCurrentStationScale.Scale(scaledTableMove); 

				if(ReadLong(long1))
				{
					numberOfTimeSteps = (unsigned short)long1;
					if(numberOfTimeSteps >= kMaxNumOfTableElements) //1.8.01
					{
						*itsLogFile << "*** WARNING: taulukkoon varattu aika-askelmäärä ylitetty: " << numberOfTimeSteps << endl;
					}
					if(ReadLong(long1))       
					{
						itsHourStep = (unsigned short)long1;
						timeOrLevelTableSet = true; 
						if(itsHourStep < 24) //050399 oli 25???
							SetHourLoop(true);
						currentTotalTimeDiff = (numberOfTimeSteps-1) * itsHourStep;
						currentTimeInd = (short)numberOfTimeSteps;
					}
				}
			}
				// itsStepSize joudutaan asettamaan luupin jälkeen alla
			ReadNext();
			break;
		}
		case dTimePlace: //17.6.00	  		 
		{   
			if (!ReadEqualChar())
				break;

			if(ReadOne(long1))    
			{
				if(++currentTimeInd > kMaxNumOfTableElements)
				{
					*itsLogFile << "*** ERROR: sallittu aika-askelmäärä ylitetty segmentissä: " << currentTimeInd << endl;
					ReadTwo(x, y); //luetaan vaan pois
					--currentTimeInd;
				}
				else
				{    //edellisen siitymä tähän!!    //24.11.00 alla korjattu
					itsTimeSteps[currentTimeInd-1] = long1 - currentTotalTimeDiff;
					currentTotalTimeDiff += itsTimeSteps[currentTimeInd-1];  //long1;
					if(ReadTwo(x, y))
					{
						fIsLevelLoop = false;
						fIsTimeLoop = true;
						timeOrLevelTableSet = true;
						fIsPureRegTimeLoop = false; //3.10.01 //oli true???
						tableDirectPoints[currentTimeInd].Set(x,y); //miksi tämä ind ???   lasketaan alla
					}
				}
			}
			
			ReadNext();
			break;
		}
		case dTimePlaceRelative: //8.5.02 EI VALMIS	  		 
		{   
			if (!ReadEqualChar())
				break;

			if(ReadOne(long1))    
			{
				if(++currentTimeInd > kMaxNumOfTableElements)
				{
					*itsLogFile << "*** ERROR: sallittu aika-askelmäärä ylitetty segmentissä: " << currentTimeInd << endl;
					ReadTwo(x, y); //luetaan vaan pois
					--currentTimeInd;
				}
				else
				{    //edellisen siirtymä tähän!!    
					itsTimeSteps[currentTimeInd-1] = long1 - currentTotalTimeDiff;
					currentTotalTimeDiff += itsTimeSteps[currentTimeInd-1];  
					if(ReadTwo(x, y))
					{
						NFmiPoint point3, point5;
						//isRelativeTable = true;  //relatiivisuus tarkoittaa taulukkoa
						fIsLevelLoop = false;
						fIsTimeLoop = true;
						timeOrLevelTableSet = true;
						fIsPureRegTimeLoop = false; 

			  point1 = itsCurrentStationScale.Scale(firstUnscaledPoint);  
  			  point2 = itsScale.Scale(point1);

						point3 = itsCurrentStationScale.UnScale(NFmiPoint(x,y));
/*						point4 = itsScale.Scale(NFmiPoint(x,y));

						point5 = point2 + point4;
*/
						point5 = firstUnscaledPoint + NFmiPoint(x,y);
		//		point1.Set(x,y);
		//		unscaledTableMove.Set(x,y);
		//		scaledTableMove = point1;
						tableDirectPoints[currentTimeInd].Set(point5.X(),point5.Y()); 
						
//						tableDirectPoints[currentTimeInd].Set(x,y); //miksi tämä ind ???   lasketaan alla
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
					numOfTableStations = (int)valueString;
			        *itsDescriptionFile >> itsObject;
			        itsString = itsObject;
				}
				else     //yhteensopivuus vanhan formaatin kanssa
				{
					tableX1 = xmin;
					tableX2 = xmin;
					tableY1 = ymin;
					tableY2 = xmax;  //! ok
					numOfTableStations = (unsigned short)ymax;
					itsString = itsObject;    //4.3
				}
				stationStepSize.Set((tableX2 - tableX1) / (numOfTableStations-1)  //16.3
					                 ,(tableY2 - tableY1) / (numOfTableStations-1));

			}
            currentStationNum = 0;

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dSegmentDataFile:   //28.8
		{
			if (!ReadEqualChar())
				break;

			itsDataName = ReadString();
			itsDataName.LowerCase();

			if(dataTimeGiven)
			{
			    *itsLogFile << "*** ERROR: Data '" << (char*)itsDataName << "' segmentissä pitää antaa ennen" << endl;
			    *itsLogFile << "       'KäytäDatanAlkuaikaa' ja 'SuhteellinenTuntiDatasta'" << endl;
			    *itsLogFile << "       -> segmentissä mahdollisesti väärä aika" << endl;
			}
			SetData(itsDataName);

/*       22.3.00 tämä kirjoitettu omaan metodiin SetData
			if(itsDataIter)   //24.8.99 tänne eikä writeps:ään koska tarvitaan                
				delete itsDataIter;   // pressScaling-objektien initialisoinnissa
			NFmiQueryData* data = itsPressProduct->DataByName(itsDataName);
			if(data)
			{
				itsDataIter = new NFmiQueryInfo(data);
				itsDataIter->First();
			}
			else
				itsDataIter = 0;
*/
			ReadNext();
			break;
		}
		case dDataNotNeeded:     //3.4.01
		{
			fDataNotNeeded = true;

			ReadNext();
			break;
		}

		case dStationDefArea:   
		{
			if (!ReadEqualChar())
				break;

			if(Read4Double(xmin,ymin,xmax,ymax))
			{
				if(xmin == xmax || ymin == ymax)   //2.10.00
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
				if(xmin == xmax || ymin == ymax)   //2.10.00
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

//			if(ReadLong(long1))
 			if(ReadOne(long1))   //5.2.99 template   
              itsProducer = long1;

			ReadNext();
			break;
		}
		case dStationTextObject:  
		{
			if(!itsDataIter) //24.8.99 ilman data ei voi saada asemannimiä
			{
				*itsLogFile << "*** ERROR: AsemanNimeä yritetty ilman dataa" << endl; 
			}
			NFmiPressStationText* text = new NFmiPressStationText;
            text->SetEnvironment(itsEnvironment); 
			text->SetHome(GetHome()); 
			text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
			text->SetData(itsDataIter);
			text->SetPressParam(this);   //25.8.99
			text->SetLanguage(itsLanguage); 
			text->SetNewNames(newNames);  
			text->Place(firstUnscaledPoint); 
				// tarvitaankohan
				point1 = itsCurrentStationScale.Scale(text->Place());  
  				text->Place(point1);
				
			if(text->ReadDescription(itsString))
			{         // 9.9 PITÄISI KAI OLLA VAIN OPTIO, SAMOIN MUILLA JA AIKALUUPILLA
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
			NFmiPsSymbol* image = new NFmiPsSymbol;
			image->SetHome(GetHome()); 
		    image->SetLogFile(itsLogFile);
			image->SetDescriptionFile(itsDescriptionFile); //31.8.01
			if(image->ReadDescription(itsString))
				 itsTimeDepObjects.Add(image);
			else
				delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dTextObject:  
		{
			NFmiPressText* text = new NFmiPressText;
            text->SetEnvironment(itsEnvironment); 
			text->SetHome(GetHome()); 
		    text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
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
			if(!itsDataIter) //24.8.99 ilman data ei voi saada aikatekstiä
			{
				*itsLogFile << "*** ERROR: AikaTekstiä yritetty ilman dataa segmentissä" << endl; 

			}
			NFmiPressDataTimeText* text = new NFmiPressDataTimeText;  
            text->SetEnvironment(itsEnvironment); 
			text->SetHome(GetHome()); 
			text->SetLogFile(itsLogFile);
			text->SetDescriptionFile(itsDescriptionFile); //31.8.01
			text->SetData(itsDataIter);        
			text->SetLanguage(itsLanguage); 
			text->Place(firstUnscaledPoint);
			//point1 = itsCurrentStationScale.Scale(firstUnscaledPoint); //eka ok kun asemaskaalausta 
  			//point2 = itsScale.Scale(point1);
			//text->Place(point2);
			if(text->ReadDescription(itsString))
			{
				text->ScalePlotting();
// **				text->SetScale(itsCurrentStationScale);
// **				text->ScalePlotting();
//				text->SetScale(itsScale); //27.11.00 auttaako
//				text->ScalePlotting();
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
				*itsLogFile << "*** ERROR: Useamman #DataAlkoit-elementin käyttö kielletty #Segmentissä, ohitetaan"  << endl;
				NFmiSymbolGroup dummyGroup;
				dummyGroup.ReadDescription(itsString);
			}
			else
			{
				symbolGroupCalled = true;
				itsSymbols.SetPressParam(this);   //13.8.99
				itsSymbols.SetEnvironment(itsEnvironment); //171198
				itsSymbols.SetProducer(itsProducer);
				itsSymbols.SetHome(GetHome()); 
				itsSymbols.SetAllLanguages(itsLanguage);    //23.3.00

				itsSymbols.SetLogFile(itsLogFile);      
				itsSymbols.SetDescriptionFile(itsDescriptionFile); //31.8.01
				SetPostReadingTimes(); //1.7 aikamuutos pitäisi antaa ennen piirtoryhmiä
				itsSymbols.SetTime(itsFirstPlotTime);   //29.6 Ei käytetä kuin tuntia, jota voi ylittää jäsenissä(mutta päivää ei)
				itsSymbols.SetHourLoop(IsHourLoop());   

				// bool ok = itsSymbols.ReadDescription(itsString);
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
               itsFirstDeltaDays = (short)(JustifyByLeaps(long1+ itsEnvironment.GetDayAdvance())); //23.5.02 +GetDayAdvance); 

			ReadNext();
			break;
		}
/*		case dRelYear:     //220199
		{
			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))    
               itsFirstDeltaDays = (short)long1*365; 

			ReadNext();
			break;
		}
		*/
		case dHour:       
		{
			if (!ReadEqualChar())
				break;

//			if(ReadLong(long1))    
 			if(ReadOne(long1))    
              itsFirstPlotHours = (unsigned short)long1;

			ReadNext();
			break;
		}
		case dHourStep:       
		{
			if (!ReadEqualChar())
				break;

//			if(ReadLong(long1))    
			if(ReadOne(long1))    
			{
               itsHourStep = (unsigned short)long1;
			   if(itsHourStep < 25)                  
							SetHourLoop(true);
			}

			ReadNext();
			break;
		}
		case dDataTime:     //040399
		{
			itsPressProduct->ReadData();
			if(itsDataIter)
			{
				itsDataIter->Reset();
				itsDataIter->NextTime();
				itsFirstPlotTime = ((NFmiQueryInfo*)itsDataIter)->Time(); //4.9.01
				itsFirstPlotHours = itsFirstPlotTime.GetHour();
			}
			else
			{
				*itsLogFile << "*** ERROR: Aikaa yritetty lukea olemattomasta datasta Segmentissä"  << endl; 
			}
			
			dataTimeGiven = true;

			ReadNext();
			break;
		}
		case dRelativeDataTime:     //040399
		{
			SetOne (long1);

			itsPressProduct->ReadData();

			if(itsDataIter)
			{
				itsDataIter->Reset();
				itsDataIter->NextTime();
				itsFirstPlotTime = ((NFmiQueryInfo*)itsDataIter)->Time();
				itsFirstPlotTime.ChangeByHours(long1);
				itsFirstPlotHours = itsFirstPlotTime.GetHour();
			}
			else
			{
				*itsLogFile << "*** ERROR: Aikaa yritetty lukea olemattomasta datasta Segmentissä"  << endl; 
			}
			dataTimeGiven = true;

//			ReadNext();
			break;
		}
		case dRelHour:     //19.1.01 ei PressTimeDscr:iin koska ei dataelementeillä
		{
			if (!ReadEqualChar())
				break;
			ReadTwo(long1, long2);
			itsFirstPlotTime = NFmiMetTime(60); //huom tunnin res.
			itsFirstPlotTime.ChangeByHours(long1);
			itsFirstPlotTime.NextMetTime(long2*60);

			*itsDescriptionFile >> itsObject;
			valueString = itsObject;
		    if(valueString.IsNumeric())  //8.2.01
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
		case dStationsLocalTime:     //31.1.2000
		{
			fStationsAreLocalTime = true;

			ReadNext();
			break;
		}
		case dStationNameReplace: //051198 stationiin verrattuna lisäykset: *** PrintName
		{
			 lon = 0.; //8.2.2000
			 lat = 0.;
			bool hasOwnPoint = false;  //4.3 talukkoEscapea varten
            
			if (!ReadEqualChar())
				break;

			helpBool = true;
			     
			valueString = ReadString();

		    if(valueString.IsNumeric())
			{
				statNum = (int)valueString;
				*itsDescriptionFile >> itsObject;
			    string1 = itsObject;
				*itsDescriptionFile >> itsObject;
			    string2 = itsObject;
			}
			else
			{
				statNum = itsLoopNum; //18.3 toimii satunnaislukuna,näin saadaan bagiin useita samannimisiä
				string1 = valueString; //Object;
			}
			string2 = ReadString();

			*itsDescriptionFile >> itsObject;    //2.3
			valueString = itsObject;         // parillinen määrä lukuja sallittu: 0 2 tai 4
		    if(valueString.IsNumeric())   // >= 1 luku
			{
			   lon = (double)valueString;
			   if(ReadDouble(lat))         // >= 2 lukua
			   {
			      *itsDescriptionFile >> itsObject;
			      valueString = itsObject;
			      if(valueString.IsNumeric())  // >= 3 lukua
			      {
				     x = (short)valueString;
			         if(!ReadDouble(y))
						  helpBool = false;  // 3 lukua virhe
				     *itsDescriptionFile >> itsObject;            // 4 lukua
				     itsString = itsObject;
					 hasOwnPoint = true;
			      }
			      else               //2 lukua
			      {
					 if(!fLonLatMode)//16.6.99
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
					if(numOfTableStations < 1) //16.6.99
					{
						*itsLogFile << "*** ERROR: "<< (char*)string1 << ": asemalla, joka ei taulukossa, tulisi olla paikka"  << endl; 
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
				  // Mika: Mitahan tuo + tuossa edessa tarkoittaa?
				  //  + x = stationStepSize.X() * (currentStationNum-1) + tableX1; 
				  x = stationStepSize.X() * (currentStationNum-1) + tableX1; 
				  y = stationStepSize.Y() * (currentStationNum-1) + tableY1;
			  }
			  point1 = itsCurrentStationScale.Scale(NFmiPoint(x,y));  
  			  point2 = itsScale.Scale(point1);
              NFmiStationPoint station                     
				      = NFmiStationPoint(NFmiStation(statNum, string1, lon, lat), point2); //14.2.00 ??? oli alla oleva
//				      = NFmiStationPoint(NFmiStation(statNum, string1, point1.X(), point1.Y()), point2);
			  itsStations.AddLocation(station);
			  if(firstStation)    
			  { //EI TOIMI
				  itsName = station.GetName();  //EI TOIMI vai toimiiko
                  firstUnscaledPoint = NFmiPoint(x,y); //9.10
				  firstStation = false;
			  }

			  // *** PrintName

			  NFmiRenaming* renaming = new NFmiRenaming;
			  renaming->originalName = string1;
			  renaming->newName = string2;
			  newNames->Add(renaming);

/*			  NFmiQueryInfo* qInfo; eihän data vielä ole luettu
			  qInfo = itsData->Info();
			  NFmiDataIdent dataIdent;
			  NFmiLocation location;
			  NFmiStation* infoStation;
			  location.SetName(string1);

              qInfo->ResetLocation();
              if(qInfo->Location(location))
			  {
			      infoStation = (NFmiStation*)qInfo->Location();
                  infoStation->SetName(string2);
			  }
			  else
				*itsLogFile << "*** ERROR: Aseman nimen vaihto epäoonistui: " << (char*)string1 << endl;  
			  // *** PrintName

*/
			}
			itsName = string2;    //12.6.00

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dStation:     
		{
			 lon = 0.; //8.2.2000
			 lat = 0.;
			bool hasOwnPoint = false;  //4.3 talukkoEscapea varten
            
			if (!ReadEqualChar())
				break;

			helpBool = true;
			     
			valueString = ReadString();
		    if(valueString.IsNumeric())
			{
				statNum = (int)valueString;
				*itsDescriptionFile >> itsObject;
			    string1 = itsObject;

			}
			else
			{
				statNum = itsLoopNum; //18.3 toimii satunnaislukuna,näin saadaan bagiin useita samannimisiä
				string1 = valueString;
			}
			valueString = ReadString();
                                          // parillinen määrä lukuja sallittu: 0 2 tai 4
			if(valueString.IsNumeric())   // >= 1 luku
			{
			   lon = (double)valueString;
			   if(ReadOne(lat))         // >= 2 lukua
			   {
			      *itsDescriptionFile >> itsObject;
			      valueString = itsObject;
			      if(valueString.IsNumeric())  // >= 3 lukua
			      {
//				     x = (short)valueString;
				     x = (double)valueString; //15.6.99
			         if(!ReadOne(y))
						  helpBool = false;  // 3 lukua virhe
				     *itsDescriptionFile >> itsObject;            // 4 lukua
				     itsString = itsObject;
					 hasOwnPoint = true;
			      }
			      else               //2 lukua
			      {
					 if(!fLonLatMode) //16.6.99
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
				if(numOfTableStations < 1)  //16.6.99
				{
					*itsLogFile << "*** ERROR: "<< (char*)string1 << ": asemalla, joka ei taulukossa, tulisi olla paikka"  << endl; 
					helpBool = false;
				}
			}

            if(helpBool) 
			{
			  if (numOfTableStations > 1 && !hasOwnPoint)
			  {
				  //       x = tableX;  
				  currentStationNum++;   
/*				  x = (tableX2 - tableX1) / (numOfTableStations-1) * (currentStationNum-1)
					                    + tableX1;
				  y = (tableY2 - tableY1) / (numOfTableStations-1) * (currentStationNum-1)
					                    + tableY1;
*/
				  // Mika: Mitahan tuo + tuossa edessa tarkoittaa?
				  // + x = stationStepSize.X() * (currentStationNum-1) + tableX1; 
				  x = stationStepSize.X() * (currentStationNum-1) + tableX1; 
				  y = stationStepSize.Y() * (currentStationNum-1) + tableY1;
			  }
			  point1 = itsCurrentStationScale.Scale(NFmiPoint(x,y));  
  			  point2 = itsScale.Scale(point1);
              NFmiStationPoint station 
				//25.2 tämä temppu jotta koordinaatit oikein PsSymbolille
				// voidaan tallettaa point1:een kun kaikki periytyy NFmiPsSymbolista
				// lat/loniahan ei tarvita mihinkään
// 4.3 seur tilalle	 = NFmiStationPoint(NFmiStation(long1, string1, point1.X(), point1.Y()), point2);
//				      = NFmiStationPoint(NFmiStation(statNum, string1, point1.X(), point1.Y()), point2);
				      (NFmiStation(statNum, string1, lon, lat), point2); //15.6.99
			  itsStations.AddLocation(station);
			  if(firstStation)    //10.9
			  { 
				  itsName = station.GetName();  //EI TOIMI vai toimiiko
                  firstUnscaledPoint = NFmiPoint(x,y); //9.10
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

			if(ReadTwo(x,y))
			{
				if(itsArea.GetArea())
				{
					currentStationNumOnMap++; 
					point0 = NFmiPoint(x,y);
					  double bottom = (itsArea.GetArea())->Bottom();
					  double top = (itsArea.GetArea())->Top();
					  double y0 = point0.Y();
				//	  point1 = NFmiPoint(point0.X(), bottom -(y0-top)); 
					  point1.Set(point0.X(), bottom -(y0-top));

//					  NFmiPoint lonlat = (/*(NFmiYKJArea*)*/itsArea.GetArea())->ToLatLon(point1);


					  NFmiPoint lonlat(itsArea.GetArea()->ToLatLon(point1));
					  lon = lonlat.X();
					  lat = lonlat.Y();
					  point1 = itsCurrentStationScale.Scale(NFmiPoint(x,y)); //lis. 10.1.02 
  					  point2 = itsScale.Scale(point1);                  //10.1.02 0->1
					  NFmiString name("As");     
					  name += NFmiValueString(currentStationNumOnMap); //uniikki nimi jokaiselle
					  NFmiStationPoint station 
							  (NFmiStation(currentStationNumOnMap, name, lon, lat), point2); 
					  itsStations.AddLocation(station);
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
						  itsStations.AddLocation(station);
						  if(firstStation)    
						  { 
							  itsName = station.GetName();  
							  firstUnscaledPoint = NFmiPoint(x,y); 
							  firstStation = false;
						  }
						  firstStation = false;
					}
						else
						*itsLogFile << "*** ERROR: "<< "lat/lon puuttuu: "  << (char*)name << endl; 
				}
				else
					*itsLogFile << "*** ERROR: "<< "karttaprojektio puuttuu"  << endl; 

			}

			ReadNext();
			break;
		}
		case dStationLonLatOnMap:  //koodi 17.8.01
		{
			if (!ReadEqualChar())
				break;

			if(ReadTwo(lon,lat))
			{
				if(itsArea.GetArea())
				{
					currentStationNumOnMap++;
					 NFmiPoint lonlat(lon,lat);
					point0 = itsArea.GetArea()->ToXY(lonlat);
					  lon = lonlat.X();
					  lat = lonlat.Y();
//					point0 = NFmiPoint(x,y);
					  double bottom = (itsArea.GetArea())->Bottom();
					  double top = (itsArea.GetArea())->Top();
					  double y0 = point0.Y();
					  point1 = NFmiPoint(point0.X(), bottom -(y0-top)); 

//					  NFmiPoint lonlat = (/*(NFmiYKJArea*)*/itsArea.GetArea())->ToLatLon(point1);
//					  lon = lonlat.X();
//					  lat = lonlat.Y();
  					  point2 = itsScale.Scale(point0);
					  NFmiString name("As");     
					  name += NFmiValueString(currentStationNumOnMap); //uniikki nimi jokaiselle
					  NFmiStationPoint station 
							  (NFmiStation(currentStationNumOnMap, name, lon, lat), point2); 
					  itsStations.AddLocation(station);
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

		case dSegmentMapArea:    //10.1.01
		{
			itsArea.SetLogFile(itsLogFile);      
			itsArea.SetDescriptionFile(itsDescriptionFile);  //3.9.01      
			itsArea.SetXyRequest(!fIsAreaOperation); //18.5.01 mittoja ei tarvita jos käytetään alueoperaatioihin     
			itsArea.ReadDescription(itsString);
			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dStationsFromData:    //10.1.01
		{
			if(itsArea.GetArea())
			{
				itsDataIter->ResetLocation();
				double bottom = (itsArea.GetArea())->Bottom();
				double top = (itsArea.GetArea())->Top();
				NFmiPoint lonlat;
                int num = 0;
				while (itsDataIter->NextLocation())
				{
				  num++;
//				  if(!(num< 12 || num%11 == 1 || num%11 == 0 || num > 110)) //HUOM
//				  {
					  currentStationNumOnMap++; 
					  lonlat = itsDataIter->LatLon();
					  point0 = itsArea.GetArea()->ToXY(lonlat); 
					  double y0 = point0.Y();
					  point1 = NFmiPoint(point0.X(), bottom -(y0-top)); 
  					  point2 = itsScale.Scale(point1);
					  NFmiString name("As");     
					  name += NFmiValueString(currentStationNumOnMap); //uniikki nimi jokaiselle
					  NFmiStationPoint station 
							  (NFmiStation(currentStationNumOnMap, name, lonlat.X(), lonlat.Y()), point2); 
					  itsStations.AddLocation(station);
					  if(firstStation)    
					  { 
						  itsName = station.GetName();  
						  firstUnscaledPoint = NFmiPoint(x,y); 
						  firstStation = false;
					  }
					  firstStation = false;
//				  }
				}
			}
			else
				*itsLogFile << "*** ERROR: "<< "karttaprojektio puuttuu gridiltä"  << endl; 

			ReadNext();
			break;
		}
		case dAreaOperation:     //18.5.01
		{
			fIsAreaOperation = true;

			ReadNext();
			break;
		}
		default:  //171198
		{
			ReadRemaining();  
			break;
		}
		}		
	} //while

	CreateStationFromAreaMask(); //13.9.01
	if(itsEnvironment.GetMaskNumber() > 0) //4.10.01 tänne, oli vikana
		fIsAreaOperation = true;
/*
	NFmiString hString = itsName;  //16.10.00 0 jotta asema erottuisi annetusta jatkonimestä
	itsName = NFmiString("=");
	itsName += hString;

	if(logName.IsValue())   //16.10.00
	{
		if(addLogNameToStationName)
		{
			itsName += NFmiString("; ");
			itsName += logName;
		}
		else
			itsName = logName;
	}
	*/
	short numberOfRegularSteps = (unsigned short)FmiMax(numberOfTimeSteps,numberOfLevelSteps);
	itsNumberOfSteps = numberOfRegularSteps;
	if(fIsLevelLoop)
		itsNumberOfSteps = currentLevelInd;
	else if(fIsTimeLoop)
		itsNumberOfSteps = currentTimeInd;

	if(timeOrLevelTableSet) //5.7 ei voi asettaa yllä pääluupissa koska ekan aseman paikkaa ei välttämättä tiedä
	{
		if(itsStations.GetSize() < 1 )  //1.10.01
		{
			*itsLogFile << "*** ERROR: "<< "station(s) required for segment with table"  << endl; 
			return false; 
		}
		
		itsStations.Reset();
		itsStations.Next();
		NFmiStationPoint statPoint = *(NFmiStationPoint*)(itsStations.Location());
		NFmiPoint point = statPoint.Point();

		if(isRelativeTable) //14.8.00
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
			short factor = (FmiMax(numberOfRegularSteps-1,1)); //12.4.00 oli ..Time..; nollalla jaon esto
			stepSize.Set((scaledTimeLast.X()-point.X())/factor
			             ,(scaledTimeLast.Y()-point.Y())/factor);
			unscaledStepSize.Set((unscaledTimeLast.X()-firstUnscaledPoint.X())/factor //3.11
			             ,(unscaledTimeLast.Y()-firstUnscaledPoint.Y())/factor);
		}

		NFmiPoint previousNextScaled = scaledTimeLast;
		NFmiPoint previousNextUnscaled = unscaledTimeLast;
		if(numberOfRegularSteps <= 1)
		{
			previousNextUnscaled = firstUnscaledPoint;
			previousNextScaled = itsScale.Scale(firstUnscaledPoint);
			previousNextScaled = itsCurrentStationScale.Scale(previousNextScaled);
		}                                      //&& lis. 1.8.01 
		for (int ind=1; ind<=itsNumberOfSteps && ind < kMaxNumOfTableElements; ind++) //writePS:ssä indeksointi 1:stä 
		{   //itsSteps on siirtymä seuraavaan
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
		
					itsUnscaledSteps[ind] = tableDirectPoints[ind+1] - previousNextUnscaled; //15.5.02
					//itsUnscaledSteps[ind] = nextScaledP - previousNextUnscaled;

					previousNextScaled = nextScaledP;
					previousNextUnscaled = tableDirectPoints[ind+1];
				}
				else //säännöllinen taulukko (alussa, muttei vain lopussa)
				{
					itsSteps[ind] = stepSize;
					itsUnscaledSteps[ind] = unscaledStepSize;
					itsTimeSteps[ind] =itsHourStep;
				}
			}
		}
	}

	retString = itsString;

	return true;	
}
//---------------------------------------------------------------------------------------
int NFmiPressParam:: ConvertDefText(NFmiString & object)
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //15.6.00 kaikille pitäisi sallia vapaa isot/pienet kirj.

	if(lowChar==NFmiString("#symbolgroup")     
		|| lowChar==NFmiString("#symboliryhmä") || lowChar==NFmiString("#dataalkiot")
		|| lowChar==NFmiString("#parametrit") 
		|| lowChar==NFmiString("#symboliryhmä"))
		return dSymbolGroup;
	else if(lowChar==NFmiString("#image")     
		|| lowChar==NFmiString("#kuva")         || lowChar==NFmiString("#vakiosymboli")
		|| lowChar==NFmiString("#vakiokuva"))
		return dImageObject;
	else if(lowChar==NFmiString("#text")     
		|| lowChar==NFmiString("#teksti")       || lowChar==NFmiString("#vakioteksti"))
		return dTextObject;
	else if(lowChar==NFmiString("#time")     
		|| lowChar==NFmiString("#aika")         || lowChar==NFmiString("#aikateksti"))
		return dTimeTextObject;
	
	else if(lowChar==NFmiString("#stationname") || lowChar==NFmiString("#asemannimi"))
		return dStationTextObject;
	else if(lowChar==NFmiString("datanotneeded") || lowChar==NFmiString("eidataa"))
		return dDataNotNeeded;
    //16.10.00 selventämään lokin tulkintaa
	else if(lowChar==NFmiString("logtext")       || lowChar==NFmiString("lokiteksti"))
		return dLogText;
	else if(lowChar==NFmiString("logtextadd")    || lowChar==NFmiString("lokitekstilisäys"))
		return dLogTextAdd;
	
	// 040399
   //seuraavat kaksi täällä eikä PressTimeDescriptionissa, koska tämän kaikilla lapsilla ei sallittua
	else if(lowChar==NFmiString("usedatastarttime")    || lowChar==NFmiString("käytädatanalkuaikaa")) 
		return dDataTime;    
	else if(lowChar==NFmiString("relativehoursfromdata")|| lowChar==NFmiString("suhteellinentuntidatasta")) 
		return dRelativeDataTime;    

	else if(lowChar==NFmiString("datafile")             || lowChar==NFmiString("data"))
		return dSegmentDataFile;
	
	else if(lowChar==NFmiString("grid")                 || lowChar==NFmiString("hila"))
	{
		return dGridMode;    //10.99 ei koodattu vielä, Tarvitaanko???
	}
	else if(lowChar==NFmiString("usestationslocaltime") || lowChar==NFmiString("käytäasemienpaikallistaaikaa"))
	{
		return dStationsLocalTime;    //31.1.2000
	}
	//16.6.99 datan syöttö grideinä vaatii muutoksia: pitää voida erottaa milloin kaksi lukua
	//        tarkoittavat tuotepaikkaa ja milloin lonLatia (varsinkin taulukoissa epäselvää),
	//        tehty uusi jäsen fLonLatMode, jota muutetaan uusilla avainsanoilla xxxxLonLatxxxx.
	else if(lowChar==NFmiString("station")               || lowChar==NFmiString("asema"))
	{
		fLonLatMode = false;
		return dStation;
	}
	else if(lowChar==NFmiString("stationnewnaming")      || lowChar==NFmiString("asemanimeävaihtaen"))
	{
		fLonLatMode = false;
		return dStationNameReplace;
	}                         
	else if(lowChar==NFmiString("stationlonlat")          || lowChar==NFmiString("asemalonlat"))
	{    //16.6.99
		fLonLatMode = true;
		return dStation;
	}
	else if(lowChar==NFmiString("stationlonlatnewnaming") || lowChar==NFmiString("asemalonlatnimeävaihtaen"))
	{    //16.6.99
		fLonLatMode = true;
		return dStationNameReplace;
	}
	else if(lowChar==NFmiString("stationdefarea")      || lowChar==NFmiString("asemienmittaalue")) 
		return dStationDefArea;
	else if(lowChar==NFmiString("stationplotarea")     || lowChar==NFmiString("asemiensijoitusalue")) 
		return dStationPlotArea;
	else if(lowChar==NFmiString("paramsubviews")  
		|| lowChar==NFmiString("siirtolukumäärä")      || lowChar==NFmiString("aikoja"))    
		return dParamSubViews;
	else if(lowChar==NFmiString("paramstepsize")       || lowChar==NFmiString("siirrot"))
		return dParamStepSize;
	else if(lowChar==NFmiString("stationtable")        || lowChar==NFmiString("asemataulukko"))
		return dStationTable;
	else if(lowChar==NFmiString("timetable")           || lowChar==NFmiString("aikataulukko"))  
		return dTimeTable;   
	else if(lowChar==NFmiString("timetablerelative")   || lowChar==NFmiString("aikataulukkosuhteellisena")
		                     || lowChar==NFmiString("aikataulukkosuht"))  
		return dTimeTableRelative;   
	else if(lowChar==NFmiString("leveltable")          || lowChar==NFmiString("painepintataulukko"))  
		return dLevelTable;   //11.4.00
	else if(lowChar==NFmiString("level")               || lowChar==NFmiString("painepinta"))  
		return dSegmentLevel;   //11.4.00
	else if(lowChar==NFmiString("levelplace")          || lowChar==NFmiString("painepintapaikka"))  
		return dLevelPlace;   //15.6.00
	else if(lowChar==NFmiString("timeplace")           || lowChar==NFmiString("aikapaikka"))  
		return dTimePlace;   //15.6.00
	else if(lowChar==NFmiString("timeplacerelative")   || lowChar==NFmiString("aikapaikkasuhteellisena")
		         || lowChar==NFmiString("aikapaikkasuht"))  
		return dTimePlaceRelative;   //8.5.02
	else if(lowChar==NFmiString("stationlonlatonmap")  || lowChar==NFmiString("asemalonlatillakartalle"))  
		return dStationLonLatOnMap;									//20.12.00
	else if(lowChar==NFmiString("stationnameonmap")    || lowChar==NFmiString("asemanimelläkartalle"))  
		return dStationNameOnMap;									//20.12.00
	else if(lowChar==NFmiString("stationplaceonmap")   || lowChar==NFmiString("asemapaikallakartalle")
													   || lowChar==NFmiString("paikka"))       //20.11.01 
		return dStationPlaceOnMap;									//20.12.00
	else if(lowChar==NFmiString("#map")|| lowChar==NFmiString("#kartta")) //10.1.01
		return dSegmentMapArea;
	else if(lowChar==NFmiString("stationsfromdata")    || lowChar==NFmiString("aseminadatanpisteet")) //10.1.01
		return dStationsFromData;
	else if(lowChar==NFmiString("areaoperaion")        || lowChar==NFmiString("alueoperaatio")) //18.5.01
		return dAreaOperation;
	else
		return NFmiPressTimeDescription::ConvertDefText(object);
}
//----------------------------------------------------------------------------
//13.9.01
bool NFmiPressParam::CreateAreaMask(void)
{
	if(itsAreaMask)
		delete itsAreaMask;

	if(itsMaskIter && itsEnvironment.GetMaskNumber() > 0)
	{
		itsMaskIter->First();
		unsigned long maskNr = itsEnvironment.GetMaskNumber();

		// Käyttämättömiä muuttujia:
#if 0
		   NFmiCalculationCondition maskOperation(kFmiMaskEqual, maskNr);  
		   NFmiAreaMask::Type maskType = NFmiAreaMask::kInfo; // maski on siis info-tyyppinen (ei esim. bitmap tai lennossa laskettava kuten auringon-kulma-maski)
		   NFmiAreaMask::DataType dataType = NFmiAreaMask::kStationary;
		   bool ownsInfo = false;
#endif

/* MUKAAN ********************
		NFmiAreaMask::BinaryOperation postBinaryOperation = NFmiAreaMask::kAnd;
		itsAreaMask = new NFmiInfoAreaMask(maskOperation, maskType, dataType, itsMaskIter, ownsInfo, postBinaryOperation);
************/
		*itsLogFile << "  areamask created: " <<  maskNr << endl;		
	}
	return true;
}
//----------------------------------------------------------------------------
//13.9.01
bool NFmiPressParam::CreateStationFromAreaMask(void)
{ //tarvitaan nimi vaikka tekstioutputiin
	if(itsMaskIter /* && itsEnvironment.GetMaskNumber() > 0*/)
	{
//		NFmiValueString name = NFmiValueString((long)itsEnvironment.GetMaskNumber());
		NFmiEnumConverter converter(kRoadRegions);
		string name = converter.ToString(itsEnvironment.GetMaskNumber());
		itsName = NFmiString(name);
		  NFmiStationPoint station 
				  (NFmiStation(100, itsName, 25, 60), NFmiPoint(100.,100.));
//		  if(itsStations.GetSize() < 1)  //pitäisi tuhota entinen ettei kumuloidu; pääasia että nimi ok
			itsStations.AddLocation(station);
	}
	return true;
}

//----------------------------------------------------------------------------
bool NFmiPressParam::WritePS(
			      NFmiRectScale theScale
		         ,ofstream& theFile
				 ,FmiPressOutputMode theOutput)
{
	UnsetAllErrorReported(); //3.3.00

	if (!itsDataIter)  
	{
		*itsLogFile << "  *** ERROR: data lost: " << (char*)itsDataName << endl;
		return true; //false lopettaa kaikki segmentit
	}
	itsDataIter->First(); //3.5.00 ainakin level pitää asettaa nollaksi eikä -1:ksi

// maskit 12.9.01
	CreateAreaMask();
//	CreateStationFromAreaMask(); //13.9.01 pitää ajaa jo ReadDescreptionissa

    NFmiPoint stationPoint;
	NFmiPoint stationPointMovement;
	NFmiPoint nameFromData;

	if(itsLogFile)
		if(itsDataName.IsValue())
			*itsLogFile << "  data: " << (char*)itsDataName << endl;
		else
			*itsLogFile << "  data: tuotteessa ekana oleva" << endl;


	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsTimeDepObjects);
	NFmiPressScaling *object;
	NFmiPressScaling *saveObject;//31.7.01 jotta voidaan tehdä aika vasta asemannimen 
    saveObject = 0;    //18.9.01                         // jälkeen (tiedostomuoto)

	NFmiPressScaling *testObject;
	NFmiVoidPtrIterator stationObjectIter = NFmiVoidPtrIterator(itsStationDepObjects);

	itsSymbols.Set(theScale, itsDataIter, theFile);  
     
	NFmiMetTime time = itsFirstPlotTime;
// 	FmiCounter itsCurrentStep = 1;
 	itsCurrentStep = 1;    //26.9.01 jäseneksi
	stationPointMovement.Set(0., 0.); 
	FmiCounter statAll = 0;
	while(itsCurrentStep <= itsNumberOfSteps) /********* AIKA/PAINEpintaluuppi ********/ //12.4.00
	{
	  FmiCounter itsCurrentStepInd = FmiMin(itsCurrentStep,kMaxNumOfTableElements-1); //1.8.01 
	  if(!itsDataIter)  
	  {
		if(itsLogFile)
			*itsLogFile << "  *** ERROR: Dataa ei ole" << endl;
	  }
	  else
	  {
	   if(fIsLevelLoop)  //30.5.00
		   itsDataIter->Level(NFmiLevel(kFmiPressureLevel,itsLevels[itsCurrentStepInd]));

	   if(itsDataIter->Time(time) || fDataNotNeeded) // HUOM segmentin aika pitää olla datassa vaikka piirtoalkiossa muutettaisiin tuntia 
//	   if(itsDataIter->Time(time)) // HUOM segmentin aika pitää olla datassa vaikka piirtoalkiossa muutettaisiin tuntia 
//	   if(true) // 8.2.2000:Segmentin ajan ei tarvi enää osua datan aikoihin, mahdollistaa max/min ym 
 	   {
//		 if(!itsDataIter->Time(time))   //8.2.2000
//			   *itsLogFile << "  WARNING: Segmentin aikaa ei ole: " << time << endl; 

		 // AikaSidotutObjektit ensin  9.3
// eli ***VakioSymboli
//     ***VakioTeksti
//     ***AikaTeksti
		 if(itsCurrentStep == 1 || fIsTimeLoop)
		 {
			bool writeLog = true;
			if(itsNumberOfSteps > 6)  //1.8.01 lyhennetään lokiskriptia
			{
				if(itsCurrentStep == 1)
				{
					*itsLogFile << "  ekan ajan segmentti-aika: " << time;  
				}
				else if (itsCurrentStep == itsNumberOfSteps)
				{
					*itsLogFile << "  vikan ajan segmentti-aika: " << time;  
				}
				else
					writeLog = false;
			}
			else
			{
			    *itsLogFile << "  Segmentin aika: " << time;  
			}
			if(IsStationLocalTimeMode() && writeLog)             
			    *itsLogFile << " pa"<< endl;  
			else if(writeLog)
			    *itsLogFile << " utc"<< endl;  
		}
		 if(fIsLevelLoop) 
			*itsLogFile << "  Segmentin painepinta: " << itsLevels[itsCurrentStepInd] << endl;  

		 objectIter.Reset();
		 object = (NFmiPressScaling*)objectIter.Next();
		 while (object)
		 {
		   if(object->ActiveTimeIndex(GetCurrentStep())) //2.10.01
		   {
				object->Set(itsScale, theFile);
				if(itsCurrentStep == 1 || !object->GetPrintOnce())  
				{
					object->SetRotatingPoint(object->Place());
					if(theOutput != kPlainText)
					{
						if (!(object->WritePSUpdatingSubText(theOutput))) //160800 +Updating..  
						{
						   if(itsLogFile)
							  *itsLogFile << "*** ERROR: (timeDep)object->WritePS() in NFmiPressParam" << endl;
						   return false;
						}
					}
					else
						saveObject = object; //31.7.01
					
				}
		   }
			if(theOutput == kPostScript) 
				object->WriteGRestore();
			
 			object->Move(itsUnscaledSteps[itsCurrentStepInd]); 
			object = (NFmiPressScaling*)objectIter.Next();   
		 }
//		 AikaSidotutObjektit loppu
//		 if(!fGridMode)  //21.10.99
//		 {
	     itsStations.Reset();
		 fIsFirstStation = true;
	     while(itsStations.Next()) /********** ASEMAluuppi ********/
         {	
			  statAll++;
              stationPoint = NFmiStationPoint(*(NFmiStationPoint*)(itsStations.Location())).Point();
		      NFmiStationPoint statPoint = *(NFmiStationPoint*)(itsStations.Location());
			  NFmiPoint lonLat = statPoint.GetLocation();
				if(/*itsDataIter->IsGrid() && */ fabs(lonLat.X()) < 0.0001   // HUOM puuttuva testi, mikseivät ole tasan nolla !!
					&& fabs(lonLat.Y()) < 0.0001)   //31.1.2000 abs -> fabs
				{
					if(itsDataIter->IsGrid())
					{
						if (!SetLonLat(statPoint)) 
							continue; //291099 ->seuraava asema jos ei taulukossa
					}
					else   //310100
					{
						if(itsDataIter->Location(NFmiStation(*statPoint.Station()).GetName()))
						{
							statPoint.SetLongitude(itsDataIter->Location()->GetLongitude());
							statPoint.SetLatitude(itsDataIter->Location()->GetLatitude());
						}
					}

				}
			  lonLat = statPoint.GetLocation();//31.1.2000  pot. muutettu

			  itsCurrentStation = NFmiStation(*statPoint.Station()); //25.8.99 ja ed. pois
			  if (FindQDStationName(statPoint) || fDataNotNeeded)  //3.4.01
		      {

					if(itsCurrentStep == 1)
					 {
					// AsemaSidotutObjektit 16.3
					// ***eli AsemanNimi (vain?)

						 stationObjectIter.Reset();
						 object = (NFmiPressScaling*)stationObjectIter.Next();
						 if(fIsFirstStation) 
							 testObject = object;

						 NFmiPoint unScaledPoint = itsScale.UnScale(stationPoint);
						 if(object && itsStations.CurrentIndex() == 0)  //20.3  
						 {  // jokaisella objektilla pitää olla oma, jäseneksi
							nameFromData = NFmiPoint(object->Place().X() - unScaledPoint.X()
													,object->Place().Y() - unScaledPoint.Y());
						 }
						 while (object)
						 {
							NFmiPoint savePlace = object->Place(); //20.3.00
							object->Place(unScaledPoint+nameFromData); 
							object->Set(itsScale, theFile);
							object->SetRotatingPoint(object->Place());   
							if (!(object->WritePS(theOutput))) //13.3.00  
							{
							   if(itsLogFile)
								  *itsLogFile << "*** ERROR: (statDep)object->WritePS() in NFmiPressParam" << endl;
							   return false;
							}
							if(theOutput == kPostScript) //4.1.01
								object->WriteGRestore(); 
							object->Place(savePlace); //20.3.00 jotta toimisi seuraavalle writePs-käskylle

							object = (NFmiPressScaling*)stationObjectIter.Next();   
						 }

						 //Testi
						 stationObjectIter.Reset();
						 object = (NFmiPressScaling*)stationObjectIter.Next();
						 unScaledPoint = itsScale.UnScale(stationPoint);
						 //testi loppu

				//AsemaSidotutObjektit loppu
					} //if itsCurrentStep==1
if(theOutput == kPlainText && saveObject)     //18.9.01          //1.8.01
saveObject->WritePSUpdatingSubText(theOutput); //***********************************
					 statPoint.Point(statPoint.Point() + stationPointMovement);
					 
					 // ****** tässä itse symbolit/numerot jne
					 if(!(itsSymbols.WritePS(statPoint,theOutput)))  //13.3.00
					 {
						 if(itsLogFile)
							 *itsLogFile << "*** ERROR: Aseman piirto ei onnistunut: " << endl; 
						 return false;
				 }
			  } //if(FindQStationName()
			  else
			  {
				  NFmiString statName = statPoint.Station()->GetName();
				  if(!(statName == NFmiString("Tyhjä") || statName == NFmiString("None"))) 
				  {
				     if(itsLogFile)
				        *itsLogFile << "  *** ERROR: Station missing from data: " << (char*)statName << endl;
				  }
		  
			  }
			  fIsFirstStation = false;
		 } //while(itsStations.Next())
/*		 }
	     else //isGridMode
		 {
		   *itsLogFile << "GRID GRID GRID " << endl; 
		 }
		 */
		} //if(itsData.Time())   
	    else
		{   //HUOM QD:ssä pitää olla segmentin aika vaikka data-alkioissa käytettäissin eri tuntia
			if(itsLogFile)
			   *itsLogFile << "  *** ERROR: Segment time missing from data: " << time << " utc" << endl; 
 
			// aikariippuvat siirrettävä eteenpäin vaikkei ole dataa 20.3
			objectIter.Reset();
		    object = (NFmiPressScaling*)objectIter.Next();
		    while (object)
		    {
 		       object->Move(itsSteps[itsCurrentStepInd]); 
			   object = (NFmiPressScaling*)objectIter.Next();   
		    }

		}
	
	  } //if(itsDataIter)

	  stationPointMovement += itsSteps[itsCurrentStepInd];
	  if(fIsLevelLoop)  //25.5.00
	  {
//	     itsDataIter->SetLevel(itsLevels
	  }
	  else 
		 if(fIsPureRegTimeLoop)  //1.8.01
			time.ChangeByHours(itsHourStep);
		 else
			time.ChangeByHours(itsTimeSteps[itsCurrentStepInd]);  //17.6.00

	  itsCurrentStep++;

	} //while(time/level Step)

	if(itsLogFile)
       *itsLogFile << "  " << statAll << " asemaa*aikaa(tai pintaa) käsitelty" << endl;
  
	long num = itsSymbols.NumOfMissing();
	if(num > 0 && itsLogFile)
		*itsLogFile << "  WARNING: " << num << " puuttuvaa dataa" << endl;
	itsSymbols.InitMissing(); //27.6.00 jotta ei manageri-käytössä kertyisi

//	NFmiPoint revStep(-itsUnscaledStepSize.X(),-itsUnscaledStepSize.Y()); 
	for(int i=1; i< itsNumberOfSteps; i++) //26.6.00 oli 0-> ; vikan siirto on nolla 
	{
		 objectIter.Reset();
		 object = (NFmiPressScaling*)objectIter.Next();
		 while (object)                    //17.3.00 toistuvat writePs:t vaativat
		 {
// 			object->Move(itsSteps[i]); 
 		    object->Move(NFmiPoint(-itsUnscaledSteps[i].X(), -itsUnscaledSteps[i].Y())); //26.6.00
			object = (NFmiPressScaling*)objectIter.Next();   
		 }
//	   theScale.MoveXEndScale(itsSteps[i].X()); //mikä vaikutus
	}
	return true;

}
//16.10.00----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------
bool NFmiPressParam::FindQDStationName(const NFmiStationPoint& theStation)
{                                            
	if(!itsDataIter)    
		return false;

	NFmiString toFindName = theStation.GetName();
	if(!itsDataIter->IsGrid()) //15.6.99 perinteinen asemannimellä haku
	{
		NFmiStation station;
//		itsDataIter->First();
		itsDataIter->ResetLocation();    
		while(itsDataIter->NextLocation())
		{
//		   NFmiStation station2 = itsData->Station();
//		   NFmiPoint latlontest = itsDataIter->LatLon();
		   NFmiString dataName = itsDataIter->Location()->GetName(); 
//		   station = itsDataIter->RefQueryData()->Station(); //23.8.99
		   if(toFindName == dataName)
//		   if(station.IsEqualName(theStation))
		   {
			   return true;
		   }
		}
	}
	else  //15.6.99 gridin käyttö myös mahdollista
	{
//		NFmiQueryInfo info(itsData); //pitäisi olla jäsen ja kaikkialla QInfo QDatan tilalle
//		NFmiPoint lonLat(theStation.GetLocation()); //11.8.99
/*
		if(abs(lonLat.X()) < 0.0001   
		    && abs(lonLat.Y()) < 0.0001)
		{
			if (!SetLonLat(theStation))
			return false;
		}
		*/
		//13.8.99 gridejä varten joutuu ottamaan pois täältä ja sijoittamaan 
		// piirto-olioon, jossa lat/longilla haetaan interpoloiden tai lähintä pistettä,
		// täältä onnistuu vain lähin piste

		//true PITÄÄ OLLA
		return true;
//		return (itsData->Location(theStation.GetLocation()));
	}
	return false;
}
//----------------------------------------------------------------------------
bool NFmiPressParam::SetLonLat(NFmiStationPoint& theStation)
{
	//20.8.01 ensin tarkistetaan jos on pistedata niin otetaan sielta aseman Lon/lat
    if(!itsDataIter->IsGrid())
	{
	//					if(itsDataIter->Location(NFmiStation(*statPoint.Station()).GetName()))
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

	if(itsPressProduct->itsNameToLonLat->Empty())
	{
		if(!itsPressProduct->ReadNameToLonLatList())
		{
			*itsLogFile << "*** ERROR: nimi/lonlat-tiedoston luku epäonnistui" << endl;
			return false;
		}
	}

	NFmiString name(theStation.GetName());
	//rivi aktiiviseksi kun AsematLonLatista alaviivat poistetaan
//	name.ReplaceChars(NFmiString("_"), NFmiString(" "));     //25.3.02 asematiedostossa ei saa olla enää alaviivoja
	NFmiPoint lonLat(itsPressProduct->itsNameToLonLat->Find(name));
//	if(lonLat.X() != 0. //pointilla saisi olla Is-metodia
//		|| lonLat.Y() != 0.)
	if(!(lonLat.X() == kFloatMissing))
	{
		theStation.SetLongitude(lonLat.X());
		theStation.SetLatitude(lonLat.Y());
	}
	else
	{
		if(name != NFmiString("Tyhjä"))   //2.4.02
			*itsLogFile << "  WARNING: " << (char*)name << " ei ole nimi/lonLat-tiedostossa" << endl;
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
void NFmiPressParam::SetStationScale(const NFmiRectScale& scale)
{
	itsCurrentStationScale = scale;
}
//----------------------------------------------------------------------------
void NFmiPressParam::SetScale(const NFmiRectScale& scale)
{
	itsScale = scale;
}


