// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiStationPoint
 */
// ======================================================================

#include "NFmiStationPoint.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/timeb.h>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Assignment operator
 *
 * \param theStationPoint The object being copied
 * \return The object assigned to
 */
// ----------------------------------------------------------------------

NFmiStationPoint & NFmiStationPoint::operator=(const NFmiStationPoint & theStationPoint)
{
  Station(*theStationPoint.Station());
  Point(theStationPoint.Point());
  fIsBackup = theStationPoint.IsBackup();
  itsAlternatingSizeFactor = theStationPoint.GetAlternatingSizeFactor();

  return *this;
}

// ----------------------------------------------------------------------
/*!
 * Return a copy of this
 *
 * \return The clone
 * \todo Should return an auto_ptr
 * \todo The static_cast is probably unnecessary
 */
// ----------------------------------------------------------------------

NFmiLocation * NFmiStationPoint::Clone(void) const
{
  return static_cast<NFmiLocation *>(new NFmiStationPoint(*this));
}

// ----------------------------------------------------------------------
/*!
 * Write the object to the given output stream
 *
 * \param file The output stream to write to
 * \return The output stream written to
 */
// ----------------------------------------------------------------------

ostream & NFmiStationPoint::Write(ostream & file) const
{
  NFmiStation::Write(file);
  file << itsPoint;
  return file;
}

// ----------------------------------------------------------------------
/*!
 * Read new object contents from the given input stream
 *
 * \param file The input stream to read from
 * \return The input stream read from
 */
// ----------------------------------------------------------------------

istream & NFmiStationPoint::Read(istream & file) 
{
  NFmiStation::Read(file);
  file >> itsPoint;
  return file;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiStationPoint::ReadDescription(NFmiString & retString)
{
  NFmiString str;
  NFmiString name;
  char object[255], ch;
  int iobject;
  double value;
  
  *itsDescriptionFile >> object;
  str = object;
  iobject = ConvertDefText(str);
  
  while((iobject != 9999 || itsCommentLevel) && itsLoopNum < itsMaxLoopNum)
	{
	  if (iobject != dEndComment && itsCommentLevel) iobject = dComment;
	  itsLoopNum++;
	  
	  switch(iobject)
		{
		case dOther:	  //ylimääräistä roinaa, END lopettaa
		  {
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		  }
		case dComment:	  
		  {
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		  }
		case dEndComment:	  
		  {
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		  }
		case dIdent:
		  {
			*itsDescriptionFile >> ch;
			*itsDescriptionFile >> value;
			
			SetIdent(static_cast<unsigned long>(value));
			
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		  }
		case dLonLat:		  
		  {
			double lon, lat;
			*itsDescriptionFile >> ch;
			*itsDescriptionFile >> lon >> lat;
			SetLongitude(lon);
			SetLatitude(lat);
			
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		  }
		case dLatitude:
		  {
			double lat;
			*itsDescriptionFile >> ch;
			*itsDescriptionFile >> lat;
			SetLatitude(lat);
			
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		  }
		case dLongitude:
		  {
			double lon;
			*itsDescriptionFile >> ch;
			*itsDescriptionFile >> lon;
			SetLongitude(lon);
			
			*itsDescriptionFile >> object;
			str = object;
			iobject = ConvertDefText(str);
			break;
		  }
		  
		case dPrintName:
		  {
			*itsDescriptionFile >> ch;
			*itsDescriptionFile >> object;
			str = object;
			SetName(str);
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

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

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

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param utc Undocumented
 * \param errorCode Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiStationPoint:: LocalTime(NFmiTime & utc, int & errorCode)
  // joko WMO-numeron avulla tai aproksimaatio longitudista
  //errorCode: 0=ihanne (Wmo-nrosta)
  //           1=ei wmo-numeroa eikä longitudia
  //           2=suoraan longitudista joten likiarvo
  //           3=wmo annettu mutta puuttuu oliolta eikä longitudiakaan ole
  //           4=     - " -                       , käytetty longitudia 
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
		  errorCode = 2;  //blokki numero pitäisi lisätä listaan
		}
	}
  
  NFmiMetTime metTime(utc,1);
  if(fabs(GetLongitude()) < .001 && fabs(GetLatitude()) < .001) //vähän ontuva mutta toimii
	{
	  //ennallaan
	  errorCode += 1; //ei wmonumeroa eikä longitudia
	  return false;  
	}
  else
	{
	  bool isFinland = GetLongitude() > 19.1 && GetLongitude() < 31.6
		          && GetLatitude() > 59.5 && GetLatitude() < 70.15;
	  if(isFinland)
		utc = metTime.CorrectLocalTime();
	  else  
	  {   //ei hanskaa ulkomaiden talvi/kesäaikoja kun poikkeavat tästä päivästä
		utc = metTime.LocalTime(static_cast<float>(GetLongitude()));
		errorCode += 2; //aproksimaatio pituuspiiristä
	  }
	  return true; 
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param utc Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiStationPoint:: LocalWmoTime(NFmiTime & utc) const
{     
  float diffHour = GetWmoDifference();
  if(diffHour < -80.)
	{
	  return false;
	}
  else
	{
	  double min,hour;
	  min = modf(diffHour,&hour)*60.;
	  utc.ChangeByHours(static_cast<long>(hour));
	  utc.ChangeByMinutes(static_cast<long>(min));
	  return true;
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param utc Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiStationPoint::LocalWmoTime(int utc) const
{
  //utc vain tasatunnit, tuloksessa voi olla minuutit

  float diffHour = GetWmoDifference();  
  diffHour += utc;
  NFmiValueString retString;
  if(diffHour < -80.) 
	retString = NFmiString("-");
  else if(fmod(diffHour,1.0f) == 0.)
	retString.SetValue(static_cast<int>(diffHour),NFmiString("%02d"));
  else
  {
    int hour = static_cast<int>(diffHour);
	float corrDiffHour = static_cast<float>(hour) + (diffHour-hour) * 0.6f;
	retString.SetValue(static_cast<float>(corrDiffHour),NFmiString("%05.2f"));
  }
  
  return retString;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 * \todo Remove the awful if-sequence, use a configuration file instead
 */
// ----------------------------------------------------------------------

float NFmiStationPoint::GetWmoDifference(void) const
{
  // valikoiva kesäaika, suljetaan eteläinen pallonpuolisko ulkopuolelle
  
  bool useSummerTime = true;
  
  int wmoCode = GetIdent();

  float zoneDifferenceHour = -100; //Lasse
  //**** EUROOPPA
  if((wmoCode >= 3000 && wmoCode < 4000) ||	//Englanti,Irlanti
	 (wmoCode >= 8500 && wmoCode < 8600) ||	//Portugal
	 (wmoCode >= 60000 && wmoCode < 60041))	// Kanarian saaret, Lasse siirsi tänne
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
		  (wmoCode >= 4100 && wmoCode < 4400) ||		//Grönlanti
		  (wmoCode >= 6000 && wmoCode < 8500) ||	 //Tanska,Hollanti,Belgia,Luxemburg,Sveitsi,Ranska,Espanja,
		  (wmoCode >= 9000 && wmoCode < 13700) ||	 //Saksa,Itävalta,Tsekki,Slovakia,Puola,Unkari,Jugot,Albania
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
  else if( wmoCode >= 26229 && wmoCode < 26551) //Latvia, pienin/suurin WMO:n taulukosta 
	{
	  zoneDifferenceHour = 2;
	}	  
  else if( wmoCode == 26629)//Tallinna,Riika,Vilna?
	{
	  zoneDifferenceHour = 2;
	  useSummerTime = false;    //17.3.2000, ensi kesänä baltia ei enää kesäajassa ENTÄ 2002??
	}	  
else if( wmoCode == 26629)//Tallinna,Riika,Vilna?
	{
	  zoneDifferenceHour = 2;
	  useSummerTime = false;    //17.3.2000, ensi kesänä baltia ei enää kesäajassa ENTÄ 2002??
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
  else if( wmoCode == 41240 )	 // lähellä Dubaita
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
	  zoneDifferenceHour = 11;  //18.9.00 Olympialaisiin laitettu kesäaikaan
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

  NFmiTime helpTime;
  if (useSummerTime) zoneDifferenceHour += -(helpTime.GetZoneDifferenceHour() + 2); 

  return zoneDifferenceHour;
}

// ======================================================================
