//© Ilmatieteenlaitos/Lasse.
//  Original 6.3.1998 

//Muutettu xxxxxx/LW
//Muutettu 121098/LW WritePs korjattu
//Muutettu 010799/LW tekstiä eteen ja taakse 
//Muutettu 020200/LW +itsLanguage ja muita korjauksia
//Muutettu 290200/LW copykonstr. siivottu
//Muutettu 020600/LW fIsValidTime
//Muutettu 131000/LW InterpretToStr()-käyttö

//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiTimeParamRect.h"
#include "NFmiPressParam.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//int Convert(NFmiString & object);

NFmiTimeParamRect::NFmiTimeParamRect(const NFmiTimeParamRect& theTimeParamRect)
:NFmiTextParamRect(theTimeParamRect)
//:NFmiTextParamRect(*(NFmiTextParamRect*)&theTimeParamRect)
,itsFormat(theTimeParamRect.itsFormat)
,itsOrigFormat(theTimeParamRect.itsOrigFormat)  //24.3.00
,itsOrigFont(theTimeParamRect.itsOrigFont)       //24.3.00
,itsStationPoint(theTimeParamRect.itsStationPoint) //2.3.2000
,fIsValidTime(theTimeParamRect.fIsValidTime)     //2.6.00
,itsLanguage(theTimeParamRect.itsLanguage) //2.2.2000
{
}
//---------------------------------------------------------------------------
NFmiTimeParamRect::~NFmiTimeParamRect() 
{
//	if(itsMapping)
//		delete (NFmiParamMapping*)itsMapping;
};
//---------------------------------------------------------------------------
NFmiParamRect* NFmiTimeParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiTimeParamRect(*this);
};
//----------------------------------------------------------------------------
void NFmiTimeParamRect:: SetLanguage(FmiLanguage newLanguage)		
{
	if(newLanguage == kChinese && (itsOrigFormat == kShortWeekDay || itsOrigFormat == kI))
	{
		itsFormat = kI;
		itsFont = NFmiString("Cviikko");
	}
	else if(itsLanguage == kChinese) //mutta uusi ei
	{
		itsFormat = itsOrigFormat;
		itsFont = itsOrigFont;
	}
	itsLanguage = newLanguage;

}; 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool NFmiTimeParamRect::ReadDescription(NFmiString& retString) 
{			 
	NFmiString helpString;
    NFmiValueString helpValueString;
	long long1;                 
	double r1,r2,r3,r4;
	itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));

	itsFont = NFmiString("Helvetica");  //30.9
//	itsFormat = kHour;                 //1.3.2000 konstruktoriin
	itsAlignment = kCenter; //5.10

	SetPreReadingTimes();  //29.6

	ReadNext();
	
	//1.10.01 kopsattu vain numerosta,jotta oletussuht.paikka = (0,0) toimii 
	itsRelRect -= NFmiPoint(1., 1.);

	while(itsIntObject != 9999 || itsCommentLevel) 
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; 
 	if(itsLoopNum > itsMaxLoopNum)  
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Numerossa" << endl;
	    retString = itsString;
		return isFalse;
	}
    itsLoopNum++;
	switch(itsIntObject)
	{
			case dOther:	  
			{    
				if(itsLogFile)
					*itsLogFile << "*** ERROR: Tuntematon sana #AikaTekstissä: " << (char*)itsObject << endl;  
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
		case dParamRectLanguage:   //2.2.2000
		{
			if (!ReadEqualChar())
				break;
			itsLanguage = ReadLanguage();
/*
			*itsDescriptionFile >> itsObject;          
			helpString = itsObject;
			if (helpString == NFmiString ("Suomi") || helpString == NFmiString ("Finnish"))
               itsLanguage = kFinnish;
			else if (helpString == NFmiString ("Ruotsi") || helpString == NFmiString ("Swedish"))
               itsLanguage = kSwedish;
			else if (helpString == NFmiString ("Englanti") || helpString == NFmiString ("English"))
               itsLanguage = kEnglish;
			else
				*itsLogFile << "*** ERROR: Tuntematon kieli: " << (char*)helpString << endl; 
*/
			ReadNext();
			break;
		}
		case dAlignment:
		{
			if (!ReadEqualChar())
				break;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
			if (itsString == NFmiString ("Center") || itsString == NFmiString ("KeskiPiste")
				|| itsString == NFmiString ("Keski"))
               itsAlignment = kCenter;
			else if (itsString == NFmiString ("Right") || itsString == NFmiString ("OikeaLaita")
				|| itsString == NFmiString ("Oikea"))
               itsAlignment = kRight;
			else if (itsString == NFmiString ("Left") || itsString == NFmiString ("VasenLaita")
				|| itsString == NFmiString ("Vasen"))
               itsAlignment = kLeft;  //2.2.2000

			ReadNext();
			break;
		}
		case dFont:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsFont = itsObject;

			ReadNext();
			break;
		}
		
		case dTimeFormat:		   
		{
			if (!ReadEqualChar())
				break;                   //kI

			itsFormat = ReadTimeFormat(); //24.3.00

			ReadNext();
			break;
		}
		
		case dStyle:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsStyle = itsObject;

			ReadNext();
			break;
		}
		case dPlaceMove:  //2.10
		{
			if (!ReadEqualChar())
				break;

			if(Read2Double(r1,r2))
			{    
//			   r1 -= 40.; //251198 miksi ?????  pois 2.11.01 ????????
//			   r2 -= 40.;
			   itsRelRect += NFmiPoint(r1/c40, r2/c40);
			}

			ReadNext();
			break;
		}
		case dRelPlace:
		{
			if (!ReadEqualChar())
				break;
			if(Read4Double(r1,r2,r3,r4))
			{
			   itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
			}
			ReadNext();
			break;
		}
		case dParamSize:  //1.10
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{           
			   itsRelRect.Inflate(-(c40-r1)/(c40*2));
			}

			ReadNext();
			break;
		}
		case dProducer: 
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
				long1 = 0; //dummy
 //              itsProducer = long1;
			
			ReadNext();
			break;
		}
/*		case dParam: 
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
               itsIdentPar = long1;
			
			ReadNext();
			break;
		}
		*/
		case dColorValueDependent: 
		{	
//			fColorValueDependent = isTrue;

			ReadNext();
			break;
		}

		case dRelDay:     //7.1
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))    //15.1
               itsFirstDeltaDays = (unsigned short)(long1+ itsEnvironment.GetDayAdvance()); //23.5.02 +GetDayAdvance); ;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
			itsIntObject = ConvertDefText(itsString);
			if(itsLogFile)
					*itsLogFile << "*** ERROR: Päiviä ei voi asettaa #AikaParamissa"  << endl;  
			break;
		}
/*		case dRelYear:     //7.1
		{
			if (!ReadEqualChar())
				break;
//			if(ReadLong(long1))    //15.1
//               itsFirstDeltaYears = (unsigned short)long1;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
			itsIntObject = ConvertDefText(itsString);
			if(itsLogFile)
					*itsLogFile << "*** ERROR: Vuosia ei voi asettaa #AikaParamissa"  << endl;  
			break;
		}
		*/
		case dHour:       //7.1
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))    //15.1
               itsFirstPlotHours = (unsigned short)long1;

			ReadNext();
			break;
		}
		default:  //010799
		{
			ReadRemaining();  
			break;
		}
	}
	} //while
	
	//flush viimeinen takaisin sreamiin! Miten?

	itsOrigFormat = itsFormat;  //24.3.00
	itsOrigFont = itsFont;
	if(itsLanguage == kChinese && (itsFormat == kShortWeekDay || itsFormat == kI)) //24.3.00
	{
		itsFormat = kI;
		itsFont = NFmiString("Cviikko");
	}

	SetPostReadingTimes();
//	fNewScaling = false; //11.9

//171198
	if(fNewScaling)
	   itsRelRect += NFmiPoint(1.,1.);
	Set(NFmiDataIdent(NFmiParam(itsIdentPar),240/*=dummy NFmiProducer(itsProducer)*/)
	   ,NFmiRect(itsRelRect));

	retString = itsString;
	return true;
}
//---------------------------------------------------------------------------
int NFmiTimeParamRect::ConvertDefText(NFmiString & object) 
{
	if(object==NFmiString("Format") || object==NFmiString("Formaatti"))
		return dTimeFormat;
	else if(object==NFmiString("Language") || object==NFmiString("Kieli")) //2.2.2000
		return dParamRectLanguage;
	else
		return NFmiTextParamRect::ConvertDefText(object);
	
}
//---------------------------------------------------------------------------
bool	NFmiTimeParamRect::WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
										,NFmiFastQueryInfo* theQI 
										,ofstream& theDestinationFile
										,FmiPressOutputMode theOutput)									
{  
    if(!ActiveTimeIndex(itsPressParam->GetCurrentStep())) //2.10.01
    {
		   return true;
    }

    itsCurrentSegmentTime = ((NFmiQueryInfo*)theQI)->Time(); //8.2.2000 
    itsCurrentTime = itsCurrentSegmentTime;//16.2.2000

	//2.3.2000 StationPoint hallitsee lokaaliajat (wmo-nro ja longitudi)
	// joita myös tarvitaan auringon lasku/nousuissa
	itsStationPoint = NFmiStationPoint(itsPressParam->GetCurrentStation(), NFmiPoint(0.,0.));
 	
	if(!PointParam(theQI)) 
	   return isFalse;
	if(!SetRelativeHour(theQI,NFmiString("#AikaTeksti"))) //16.2.2000
		return isFalse;
	// double longitude = itsStationPoint.GetLongitude();

	NFmiPressTime pTime = TimeToWrite(theQI);


	//aina local time piirtoalkioissa (vrt segmentti ja tuote missä ei)
	int errCode;
	itsStationPoint.LocalTime(pTime, errCode);
	if(errCode > 0  && !itsPressParam->GetErrorReported(errCode))
	{
		*itsLogFile     << "  WARNING: possible problems to calculate local time, first: " <<
			    (char*) ((NFmiString&)itsStationPoint.GetName()) << endl; //Markon maili 3.3.00

		if(errCode == 1)
			*itsLogFile << "  ->ei Wmo-numeroa eikä longitudia" << endl;
		else if(errCode == 2)
			*itsLogFile << "  ->Wmo-nro puuttuu Asema-määrittelyltä(longitudi käytetty)" << endl;
		else if(errCode == 3)
			*itsLogFile << "  ->Wmo-nro puuttuu ohjelmasta eikä longitudia ole"  << endl;
		else if(errCode == 4)
			*itsLogFile << "  ->Wmo-nro puuttuu ohjelmasta (longitudi käytetty)" << endl;
		itsPressParam->SetErrorReported(errCode);
	}
	NFmiString timeString; // = pTime.ToStr(itsFormat,itsLanguage);
	if(fIsValidTime)
	{
		if(itsStringNameTimeFormat.IsValue())   //13.10.00
			timeString = pTime.InterpretToStr(itsStringNameTimeFormat,itsLanguage);
		else
			timeString = pTime.ToStr(itsFormat,itsLanguage);
	}
	else
		timeString = NFmiString(" -   "); //2.6.00 puuttuva auringon nousu/lasku 

	NFmiString string;

		if(fParenthesis)             
		{
            string = NFmiString("(");
			string += NFmiString (timeString);	 
            string += NFmiString(")");
		}
		else
		{
			string = NFmiString (timeString);	 
		}

	//300699 +Construct (lisäykset eteen ja perään)
		return WriteCode(Construct(&string), AbsoluteRectOfSymbolGroup, 
			             theDestinationFile, NFmiString("AIKATEKSTI"), theOutput); 
}
/*
//---------------------------------------------------------------------------
//1.3.00 
bool NFmiTimeParamRect::ToLocalTime(NFmiPressTime& theTime) const

// jos järkevä blokki-numero löytyy asemalta käytetään sitä,
// muuten aproksimaatio longitudin avulla
{
//	NFmiTime localTime; //MetTime hävittää minuutit
//	NFmiStationPoint location(itsPressParam->GetCurrentStation(),NFmiPoint(0.,0.));

//	return itsStationPoint.LocalTime(theTime); ei const

//	return itsStationPoint.LocalTime();
	if(itsStationPoint.GetIdent() > 1000) 
	{
//		NFmiStationPoint statPoint(location,NFmiPoint(0.,0.));
		return itsStationPoint.LocalWmoTime(theTime);
	}
	else
	{
		 NFmiMetTime metTime(theTime,1);
		 NFmiTime localTime = metTime.LocalTime((float)(itsStationPoint.GetLongitude()));
		 theTime = localTime;
		 return true; //entäs jos longitudi puuttuu
	}
	
}
*/
//---------------------------------------------------------------------------
NFmiTime	NFmiTimeParamRect::TimeToWrite(NFmiFastQueryInfo* theQI)
	//aina local time piirtoalkioissa (vrt segmentti ja tuote missä ei)
{ //pitäisi mahdollistaa UTC-ajassa piirto/15.2.01
	NFmiMetTime time(itsCurrentTime,1); //localTimea varten
	if(itsPressParam->IsStationLocalTimeMode())  
	{
		//HUOM tämä koodi ylläpidettävä samanlaisena myös NFmiParamRectissä
		double latitude = itsStationPoint.GetLatitude();//Testiin
		double longitude = itsStationPoint.GetLongitude();//Testiin
		if(fabs(longitude) <= .001 && fabs(latitude) <= .001)
			*itsLogFile << "*** ERROR: longitudi puuttuu " << (char*)itsStationPoint.GetName()
			            << ":lta paikalliseen aikaan"<< endl; 
		NFmiTime localTime = time.LocalTime(-(float)longitude);
	//Hullua näin
		NFmiMetTime saveTime = ((NFmiQueryInfo*)theQI)->Time(); 
		theQI->TimeToNearestStep (localTime, kCenter, theQI->TimeResolution()/2);
		time =  ((NFmiQueryInfo*)theQI)->Time();
		theQI->Time(saveTime);
	}
	return time;	
}

//-050399---------------------------------------------------------------------------
bool NFmiTimeParamRect:: PointParam(NFmiFastQueryInfo* theQI)
{ //aika ei tarvitse parametriä
    fParamErrorReported = false;  
	return true;
}
