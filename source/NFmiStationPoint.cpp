	 //© Ilmatieteenlaitos  Lasse
//  Original 10.10. 1997
//Muutettu 261197/LW LatLon -> LonLat (koska n‰inp‰in ne annetaan)
//Muutettu 251099/LW kes‰/talviaika automatisoitu
//Muutettu 060300/LW kes‰/talviaika edelleen yritetty saada kuntoon
//Muutettu 260301/LW kes‰/talviaika toivottavasti lopullisesti kunnossa

//----------------------------------------------------------------------------
#include <math.h>
#include <fstream> //STD 27.8.01
#include <time.h>     //25.10.99
#include <sys/timeb.h>

#ifndef __NSTATPNT_H__
#include "NFmiStationPoint.h"
#endif//__NSTATPNT_H__

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

/* 
//----------------------------------------------------------------------------
bool NFmiStationPoint::IsEqual(const NFmiLocation & theLocation) const
{	//2.12/LW t‰h‰n nyt laitetaan t‰llainen hyvin lˆys‰ vertaus lehtituotteita varten
	return((GetIdent()   == ((NFmiStation *)(NFmiLocation*)&theLocation)->GetIdent()) ||
			(GetName()     == ((NFmiStation *)(NFmiLocation*)&theLocation)->GetName())  || 
			(GetLocation() == theLocation.GetLocation()));
}

*/
/*
//----------------------------------------------------------------------------
NFmiStationPoint::NFmiStationPoint(const NFmiStationPoint &theStationPoint)
{
	NFmiStation :: NFmiStation(theStationPoint.Station());
	NFmiPoint :: NFmiPoint(theStationPoint.Point());
}
*/
//----------------------------------------------------------------------------
NFmiStationPoint& NFmiStationPoint::operator=(const NFmiStationPoint &theStationPoint)
{
  Station(*theStationPoint.Station());
  Point(theStationPoint.Point());
  return *this;
}

/*
//----------------------------------------------------------------------------
void NFmiStationPoint::SetLocation(const NFmiLocation &theLocation)
{
//  NFmiIndividual::operator=(*((NFmiIndividual*)&theLocation));
  NFmiIndividual::operator=(*((NFmiIndividual*) (NFmiStation *)&theLocation));
//  NFmiLocation::operator=(*((NFmiLocation*) (NFmiStation *)&theLocation));
  NFmiLocation::SetLocation(theLocation);
}
*/

//----------------------------------------------------------------------------
NFmiLocation* NFmiStationPoint::Clone() const
{
	return (NFmiLocation *) new NFmiStationPoint(*this);
}
//----------------------------------------------------------------------------
ostream& NFmiStationPoint::Write(ostream &file) 
{
	NFmiStation::Write(file);
	file << itsPoint;
//	itsPoint.Write(file);

  return file;
}
//----------------------------------------------------------------------------
istream& NFmiStationPoint::Read(istream &file) 
{
	NFmiStation::Read(file);
	file >> itsPoint;
//	itsPoint.Read(file);

	return file;
}
//----------------------------------------------------------------------------
bool NFmiStationPoint::ReadDescription(NFmiString& retString) //16.1
{
	NFmiString str;
	NFmiString name;
	char object[255], ch;
	int iobject;
	double value;

	*itsDescriptionFile >> object;
	str = object;
	iobject = ConvertDefText(str);
	
	while((iobject != 9999 || itsCommentLevel) && itsLoopNum < itsMaxLoopNum) //2.2
	{
	if (iobject != dEndComment && itsCommentLevel) iobject = dComment;
    itsLoopNum++;

	switch(iobject)
	{
		case dOther:	  //ylim‰‰r‰ist‰ roinaa, END lopettaa
		{
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		case dComment:	  
		{
//			isComment = isTrue;
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		case dEndComment:	  
		{
//			isComment = isFalse;

			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		case dIdent:
		{
			*itsDescriptionFile >> ch; // =
			*itsDescriptionFile >> value;

			SetIdent((unsigned long)value);

			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		case dLonLat:		  
		{
			*itsDescriptionFile >> ch; // =
			*itsDescriptionFile >> itsLongitude >> itsLatitude;

			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		case dLatitude:
		{
			*itsDescriptionFile >> ch; // =
			*itsDescriptionFile >> itsLatitude;

			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		case dLongitude:
		{
			*itsDescriptionFile >> ch; // =
			*itsDescriptionFile >> itsLongitude;
			
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		
		case dPrintName:
		{
			*itsDescriptionFile >> ch;
			*itsDescriptionFile >> object;
			str = object;   //27.1
			SetName(str);
//			itsObject = new NFmiString(object);
//			name = *itsObject;
//			SetName(name);
  
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}

		case dCoordinates:
		{
			*itsDescriptionFile >> ch;
			*itsDescriptionFile >> itsPoint;

			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		}
		
  	}
	} //while
    
    retString = str;
 	return isTrue;	
}
//---------------------------------------------------------------------------------------
int NFmiStationPoint:: ConvertDefText(NFmiString & object)
{
	if(object==NFmiString("LonLat"))
		return dLonLat;
	else if(object==NFmiString("Latitude"))
		return dLatitude;
	else if(object==NFmiString("Longitude"))
		return dLongitude;
	else if(object==NFmiString("PrintName"))
		return dPrintName;
	else if(object==NFmiString("Place"))
		return dCoordinates;
	else if(object==NFmiString("Ident"))
		return dIdent;
	else
		return NFmiDescription::ConvertDefText(object);
}
//---------------------------------------------------------------------------------------
bool NFmiStationPoint:: LocalTime(NFmiTime& utc, int& errorCode) //const
// joko WMO-numeron avulla tai aproksimaatio longitudista
//errorCode: 0=ihanne (Wmo-nrosta)
//           1=ei wmo-numeroa eik‰ longitudia
//           2=suoraan longitudista joten likiarvo
//           3=wmo annettu mutta puuttuu oliolta eik‰ longitudiakaan ole
//           4=     - " -                       , k‰ytetty longitudia 
{
    errorCode = 0;	
	if(GetIdent() > 1000) //mwo-nro tunnettu
	{
		if(LocalWmoTime(utc))
		{
			return true;  //ihanne
		}
		else
		{
			errorCode = 2;  //blokki numero pit‰isi lis‰t‰ listaan
		}
	}

	 NFmiMetTime metTime(utc,1);
	 if(fabs(GetLongitude()) < .001 && fabs(GetLatitude()) < .001) //v‰h‰n ontuva mutta toimii
	 {
		 //ennallaan
		 errorCode += 1; //ei wmonumeroa eik‰ longitudia
		 return false;  
	 }
	 else
	 {
		 utc = metTime.LocalTime((float)(GetLongitude()));
		 errorCode += 2; //aproksimaatio pituuspiirist‰
		 return true; 
	 }
}
//---------------------------------------------------------------------------------------
bool NFmiStationPoint:: LocalWmoTime(NFmiTime& utc) const
{     
//	NFmiMetTime time(utc);	
	float diffHour = GetWmoDifference();
    if(diffHour < -80.)
	{      //ei itsLogFile jotta yleinen
//		*itsLogFile << "  WARNING: WMO-nro puuttuu paikallisaika-metodista" << endl;
//		time.ChangeByHours(2);  //Suomenaika jos ei lˆydy
		return false;
	}
	else
	{
		double min,hour;
		min = modf(diffHour,&hour)*60.;
		utc.ChangeByHours((long)hour);
		utc.ChangeByMinutes((long)min);
		return true;
	}
}

// Kaisalta 23.10.98 ---------------------------------------------------------------------------------------
NFmiString NFmiStationPoint:: LocalWmoTime(int utc) const
{     //utc vain tasatunnit, tuloksessa voi olla minuutit

	float diffHour = GetWmoDifference();  
    diffHour += utc;
	NFmiValueString retString;
    if(diffHour < -80.) 
		retString = NFmiString("-");
	else if(fmod(diffHour,1.0f) == 0.)
		retString.SetValue((int)(diffHour),NFmiString("%02d"));
	else
		retString.SetValue((float)(diffHour),NFmiString("%05.2f"));

return retString;
}
//--------------------------------------------------------------------------------
float NFmiStationPoint::GetWmoDifference(void) const
{
	//28.3.99 valikoiva kes‰aika, suljetaan etel‰inen pallonpuolisko ulkopuolelle

	bool useSummerTime = true;

	int wmoCode = GetIdent();
	float zoneDifferenceHour = -100; //Lasse
															  //**** EUROOPPA
	if(wmoCode >= 3000 && wmoCode < 4000 ||	//Englanti,Irlanti
	   wmoCode >= 8500 && wmoCode < 8600 ||	//Portugal
	   wmoCode >= 60000 && wmoCode < 60041)// Kanarian saaret, Lasse siirsi t‰nne
	{
	  zoneDifferenceHour = 0;
	}
	else if(wmoCode >= 4000 && wmoCode < 4100) //Islanti
	{
	  zoneDifferenceHour = 0;
	  useSummerTime = false;
	}

	else if((wmoCode >= 1000 && wmoCode < 1500) ||		//Norja
	        (wmoCode >= 2000 && wmoCode < 2700) ||		//Ruotsi
            (wmoCode >= 4100 && wmoCode < 4400) ||		//Grˆnlanti
            (wmoCode >= 6000 && wmoCode < 8500) ||	 //Tanska,Hollanti,Belgia,Luxemburg,Sveitsi,Ranska,Espanja,
            (wmoCode >= 9000 && wmoCode < 13700) ||	 //Saksa,It‰valta,Tsekki,Slovakia,Puola,Unkari,Jugot,Albania
            (wmoCode >= 16000 && wmoCode < 16600))  	 //Italia,Malta
	{
	  zoneDifferenceHour = 1;
	}
	else if((wmoCode >= 2800 && wmoCode < 3000) ||		//Suomi
			(wmoCode >= 15000 && wmoCode <16000) ||	    //Romania,Bulgaria
			(wmoCode >= 16600 && wmoCode < 17620))	//Kreikka,Turkki,Kypros
	{
	  zoneDifferenceHour = 2;
	}
	else if( wmoCode == 26038 || wmoCode == 26422)//Tallinna,Riika,Vilna?
	{
	  zoneDifferenceHour = 2;
//	  useSummerTime = false; //17.3.2000, ensi kes‰n‰ baltia ei en‰‰ kes‰ajassa; 11.4.2002 taas kes‰ajassa
	}	  
	else if( wmoCode == 26629)//Tallinna,Riika,Vilna?
	{
	  zoneDifferenceHour = 2;
	  useSummerTime = false;    //17.3.2000, ensi kes‰n‰ baltia ei en‰‰ kes‰ajassa ENTƒ 2002??
	}	  
	else if(( wmoCode == 26063 )||( wmoCode == 27612 ))	// Pietari, Moskova
	{
	  zoneDifferenceHour = 3;
	}
	else if(( wmoCode == 33345 ))			// Kiova
	{
	  zoneDifferenceHour = 2;
	}
													//*****AASIA
	else if((wmoCode == 54511)||(wmoCode == 45004)||(wmoCode == 98429)// Peking , HongKong,Manila
            ||wmoCode == 48698)           // Singapore ?
	{
	  zoneDifferenceHour = 8;
	  useSummerTime = false;
	}	  
	else if( wmoCode >= 40000 && wmoCode < 41000) //ainakin Israel
	{
	  zoneDifferenceHour = 2;
	}															
	else if( wmoCode == 41240 )	 // l‰hell‰ Dubaita
	{
	  useSummerTime = false;
	  zoneDifferenceHour = 4;
	}	  
	else if( wmoCode == 47662 )	 // Tokio
	{
	  zoneDifferenceHour = 9;
	  useSummerTime = false;
	}	  
	else if(( wmoCode == 48455 )||( wmoCode == 48456 )||( wmoCode == 96747 ))// Bangkok,Jakarta
	{
	  zoneDifferenceHour = 7;
	  useSummerTime = false;
	}
	else if( wmoCode == 41150 )		// Bahrain
	{
	  zoneDifferenceHour = 3;
	  useSummerTime = false;
	}	  
	else if( wmoCode == 42182 || wmoCode == 43192) // Delhi  HUOMIO 5h 30 min
	{
	  zoneDifferenceHour = 5.5f; //550; HUOM/Lasse //oli 530
	  useSummerTime = false;
	}	                                                    //***** AFRIKKA
	else if( wmoCode >= 60000 && wmoCode < 60400) // marokko
	{
	  zoneDifferenceHour = 0;														//AFRIKKA
	  useSummerTime = false;
	}	  
	else if( wmoCode >= 60400 && wmoCode < 60800) // Tunisia, Algeria
	{
	  zoneDifferenceHour = 1;														//AFRIKKA
	  useSummerTime = false;
	}	  
	else if(( wmoCode == 62366 ))	// Kairo,
	{
	  zoneDifferenceHour = 2;
	}	  
	else if(( wmoCode == 68816 ))	// ,Kapkaupunki
	{
	  zoneDifferenceHour = 2;
	  useSummerTime = false;
	}	  
	else if( wmoCode == 64210 )			// Kinshasa
	{
	  zoneDifferenceHour = 1;
	  useSummerTime = false;
	}
	else if( wmoCode == 63740 )			// Nairobi
	{
	  zoneDifferenceHour = 3;
	  useSummerTime = false;
	}	  
													//*****AMERIKKA
	else if(( wmoCode == 87576 )||( wmoCode == 83743 ))	// Buenos Aires,Rio de Janeiro
	{
	  zoneDifferenceHour = -3;
	  useSummerTime = false;
	}

	else if((wmoCode == 72503)||( wmoCode == 72202)||( wmoCode == 72203)||(wmoCode == 71624)	// New York,Miami,Toronto
          ||(wmoCode == 78073) || wmoCode == 71627)                  // Nassau, Montreal
	{
	  zoneDifferenceHour = -5;
	  useSummerTime = true;
	}
	else if( wmoCode == 72530 )			// Chicago 

	{
	  zoneDifferenceHour = -6;
	  useSummerTime = true;
	}	  
	
	else if( wmoCode == 72295 || wmoCode == 71892)// Los Angeles, Vancouver
	{
	  zoneDifferenceHour = -8;
	  useSummerTime = true;
	}	  
	
													//*****AUSTRALIA
	else if( wmoCode == 94767 )			// Sydney
	{
	  zoneDifferenceHour = 11;  //18.9.00 Olympialaisiin laitettu kes‰aikaan
	  useSummerTime = false;
	}	  
	else if( wmoCode == 94610 )			// Perth
	{
	  zoneDifferenceHour = 8;
	  useSummerTime = false;
	}
	else
	{
	  zoneDifferenceHour = -100;
//	  cout <<"TUNTEMATON ASEMA " << wmoCode << endl;
	}

//	if (useSummerTime /*&& kes‰aika*/) zoneDifferenceHour += 1;

// ***joko t‰m‰ (sama kuin FmiTimess‰ nyt)
/*
  struct _timeb tstruct;
  _ftime( &tstruct );
  short theWinterTimeDiff = (short) short(_timezone/3600);

  struct tm *newtime;
  time_t aclock;

  time(&aclock);
  newtime = localtime(&aclock);

  theWinterTimeDiff = short(theWinterTimeDiff - newtime->tm_isdst);
  if (useSummerTime) zoneDifferenceHour += theWinterTimeDiff + 2; 
 */

	/* 3.11.00 nyt pit‰isi sitten toimia */
  NFmiTime helpTime;                          //26.3.01 - lis‰tty alla kun ei viel‰k‰‰n toiminut
  if (useSummerTime) zoneDifferenceHour += -(helpTime.GetZoneDifferenceHour() + 2); 

  /* talvella sitten taas 0 !!! */
// *** tai t‰m‰ (windowsissa rasti pois) ?????
//	short tesrtheWinterTimeDiff = _daylight; /* t‰m‰ on kai vain se t‰pp‰ */
// ***joka tapauksessa toimii vain t‰m‰n p‰iv‰n mukaan, ei jos aktuelli aika on jotain muuta

//	if (useSummerTime && _daylight) zoneDifferenceHour += 1; //25.10.99 service pack 5 kaikkialla?
//	if (useSummerTime) zoneDifferenceHour += theWinterTimeDiff; //6.3.2000 pois 31.10.00
	
	return zoneDifferenceHour;
}

