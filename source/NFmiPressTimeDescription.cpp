// © Ilmatieteenlaitos/Lasse.
// Original 12.1.1998
// Muutettu 200698/LW +SetPre.. ja SetPostReadingTimes()
// Muutettu 120898/LW estetty alle vrk:n luupeissa oman tuntiarvon käyttöä
// Muutettu 300998/LW kuunvaihde ei toiminut
// Muutettu 250199/LW karkausvuosi huomioitu
// Muutettu 270300/LW +ReadTimeFormat()
// Muutettu 250800/LW +kShortDay ja kShortMonth
// Muutettu 131000/LW ReadString() aikaformaatteihin
// Muutettu 140201/LW TimeToWrite() 
// Muutettu 040601/LW Vanhasta aikaformaatista pois ne jotka saadaan uudella 
//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressTimeDescription.h"
#include "NFmiPressTime.h"
//#include "NFmiSuperSmartInfo.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiValueString.h"
#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01


//---------------------------------------------------------------------------
unsigned long NFmiPressTimeDescription::ReadTimeFormat(bool isSecond)//27.10.00 par lis. 
{
	NFmiString helpString;
	unsigned long format = kWwsHH; //oletus
/*
	cin >> itsObject;
	helpString = itsObject;
*/
	helpString = ReadString(); //tuo myös vlilyönnit hipsuissa
 
	//HUOM**************
	//4.6.01 kaikki jotka tulkittavissa uudella tavalla pitäisi ottaa vanhasta pois niin tuotteen nimi toimii paremmin
/*	if (helpString == NFmiString ("HH"))
       format = kHour;
	else if (helpString == NFmiString ("H"))   
       format = kH;
	else if (helpString == NFmiString ("D"))   
       format = kShortDay;
	else if (helpString == NFmiString ("M"))   
       format = kShortMonth;
	   */
	if (helpString == NFmiString ("HHdMM"))   
       format = kHHdMM;
	else if (helpString == NFmiString ("HdMM"))   
       format = kHdMM;
//	else if (helpString == NFmiString ("WwHH"))
//       format = kWwHH;
	else if (helpString == NFmiString ("wwsHH") || helpString == NFmiString ("WwsHH"))
       format = kWwsHH;
//	else if (helpString == NFmiString ("Ww"))
//       format = kShortWeekDay;
//	else if (helpString == NFmiString ("Wwww"))
//       format = kLongWeekDay; 
	else if (helpString == NFmiString ("DdMd"))
       format = kDdMd; 
	else if (helpString == NFmiString ("WwsDDdMMd"))
       format = kWwsDDdMMd; 
	else if (helpString == NFmiString ("WwsDdMd"))
       format = kWwsDdMd; 
	else if (helpString == NFmiString ("NnnnsYYYY"))
       format = kNnnnsYYYY; 
//	else if (helpString == NFmiString ("Nnnn"))
//       format = kLongMonthName; 
//	else if (helpString == NFmiString ("Nnn"))
//       format = kShortMonthName; 
	else if (helpString == NFmiString ("DDdMMdYYYY")) 
       format = kDDdMMdYYYY; 
	else if (helpString == NFmiString ("DdMdYYYY"))   
       format = kDdMdYYYY; 
//	else if (helpString == NFmiString ("I"))   //15.3.00
//       format = kI; 
//	else if (helpString == NFmiString ("YYYYMMDD")) //4.4.00, 13.10.00 pois
//       format = kYYYYMMDD; 
	else
	{
		if(isSecond)
			itsSecondStringNameTimeFormat = helpString;   //27.10.00 
		else
			itsStringNameTimeFormat = helpString;   //13.10.00 
	}
	return format;
}
//080300----------------------------------------------------------------------------
bool NFmiPressTimeDescription::ReadRemaining(void)  
{ 
	long long1, long2, long3;
	NFmiValueString valueString;
	switch(itsIntObject)
	{	
		case dGivenRelHours:  //4.5.00
		{
			fGivenHoursAreRelative = true;

			ReadNext();
			break;
		}
		case dMinute:  
		{
			SetOne(itsFirstPlotMinutes);

			break;
		}
		case dWriteTimeAsUtc:  //14.2.01
		{
			fWriteAsUtc = true;

			ReadNext();
			break;
		}
		case dTimeTableActive:  //2.10.01 tänne	  
		{
			if (!ReadEqualChar())
				break;

			long3 = 1000;
			if(ReadTwo(long1, long2))
			{
			   *itsDescriptionFile >> itsObject;
			   valueString = itsObject;
			   if(valueString.IsNumeric())  
			   {
				  long3 = static_cast<long>(valueString);

				  *itsDescriptionFile >> itsObject;
				  itsString = itsObject;
				}
				else
				{
				   itsString = valueString;
				}

				if(long1 > 0 && long2 > 0  && long3 > 0  && long1 < long3)
				{
					itsLoopActivity.startIndex = long1;
					itsLoopActivity.step = long2;
					itsLoopActivity.stopIndex = long3;
				}
				else
					*itsLogFile << "*** ERROR: Not valid looping (start,step,stop): "<< long1<< long2<< long3<< endl;
			}


			itsIntObject = ConvertDefText(itsString);

			break;
		}
		case dBypassValue:  //2.10.01 tänne	  
		{
			SetOne(itsLoopActivity.bypassValue);
			itsLoopActivity.bypassWithValue = true;

			break;
		}
		default:
		{
			return NFmiPressDescription::ReadRemaining();  
			break;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
int NFmiPressTimeDescription::ConvertDefText(NFmiString & object) 
{
	NFmiString lowChar = object;  //14.2.01
	lowChar.LowerCase(); 

	if(lowChar==NFmiString("day")        || lowChar==NFmiString("päivä"))
		return dRelDay;
	else if(lowChar==NFmiString("minute") || lowChar==NFmiString("minuutti"))
		return dMinute;
	else if(lowChar==NFmiString("hour")     || lowChar==NFmiString("tunti"))
		return dHour;
	else if(lowChar==NFmiString("relhour")  || lowChar==NFmiString("suhteellinentunti"))
		return dRelHour;                     
	else if(lowChar==NFmiString("relhours")  || lowChar==NFmiString("suhteellisettunnit"))//4.5.00 data-alkioissa
		return dGivenRelHours;                     
	else if(lowChar==NFmiString("hourstep") || lowChar==NFmiString("tuntiaskel")
		|| lowChar==NFmiString("tuntiaskel"))
		return dHourStep;
	else if(lowChar==NFmiString("utcwriting")  || lowChar==NFmiString("utctulostus"))//4.5.00 data-alkioissa
		return dWriteTimeAsUtc;                     
	else if(lowChar==NFmiString("timetableactive")||     lowChar==NFmiString("aikataulukonaktiiviset"))//2.10.01 tänne
		return dTimeTableActive;
	else if(lowChar==NFmiString("bypassvalue")||     lowChar==NFmiString("ohitusarvo"))  //2.10.01
		return dBypassValue;
	else 
		return NFmiPressDescription :: ConvertDefText(object);
}

//---------------------------------------------------------------------------
bool NFmiPressTimeDescription::ActiveTimeIndex(int currentInd)const
{   //halutessa olioita voidaan piirtää esim joka n:teen aika-askeleeseen
  // Mika: Kannattaisikohan laittaa sulkuja?
  if(itsLoopActivity.startIndex < 1 || (currentInd-itsLoopActivity.startIndex)
	 % itsLoopActivity.step == 0
	 && currentInd >= static_cast<int>(itsLoopActivity.startIndex)
	 && currentInd <= static_cast<int>(itsLoopActivity.stopIndex)
	 ) return true;
  return false;
}
//8.2.01---------------------------------------------------------------------------
NFmiMetTime NFmiPressTimeDescription::NextUseTime(long relHours, long hourRes, long hourDelta) 
{
  //MetTimeltä ei löydy palvelua, jolla saisi NextTimen kun resoluutio ei ala nollasta
  //     eli esim ajat ovat 2,8 14,20,2.... tai aina 12 (hourDelta)
  //esim nyt = 8.2. klo 8.30 utc ->
  //	         NextUseTime(4,24,12) -> 9.2. klo 12 utc
  //	         NextUseTime(3,24,12) -> 8.2. klo 12
  //	         NextUseTime(-3,6,1)  -> 8.2. klo 7   utc
	 	NFmiMetTime curTime(60); 
		curTime.ChangeByHours(relHours);
		NFmiMetTime compTime(60); 
		compTime.ChangeByDays(-2); //riittääkö
		compTime.SetHour(hourDelta);
		if (compTime >= curTime)
		    *itsLogFile << "*** ERROR: suhteellinen aikaero liian suuri: " << relHours << endl;
		else
		{
			while(compTime <= curTime)
				compTime.ChangeByHours(hourRes);
		}

		return compTime;
}
//260199---------------------------------------------------------------------------
long NFmiPressTimeDescription::JustifyByLeaps(long dayDiff) 
{   //ei kyllä ihan tarkka, ja toimii vain yhdelle vuodelle toistaiseksi
	long ret = dayDiff;
	short add;
	// short delta = 5;
	NFmiTime now; 
	NFmiTime compDate = now;
	short sign = +1;        //onko c:ssä
	if (dayDiff < 0) sign = -1;
	compDate.SetYear(now.GetYear()+sign);
	if(abs(abs(dayDiff)-365) < 5)
		if(abs(now.DifferenceInDays(compDate)) == 365)
			add = 0;
		else
			add = 1;
	else
		add = 0;

	return ret + add*sign;
}
//---------------------------------------------------------------------------
void NFmiPressTimeDescription::SetPreReadingTimes(void) 
{   //30.9 ei toiminut kuunvaihteessa
	NFmiTime curDate;
	curDate.SetHour(0);
	curDate.SetMin(0);
	NFmiTime firstPlotDate = NFmiTime(itsFirstPlotTime);
    firstPlotDate.SetHour(0);
    firstPlotDate.SetMin(0);
	itsFirstDeltaDays = static_cast<short>(firstPlotDate.DifferenceInDays(curDate));

	itsFirstOriginalDeltaDays = itsFirstDeltaDays;  
	itsFirstPlotHours = itsFirstPlotTime.GetHour() ; 
	itsFirstPlotMinutes = itsFirstPlotTime.GetMin() ; //8.3.2000
}
//---------------------------------------------------------------------------
void NFmiPressTimeDescription::SetPostReadingTimes(void)
{
	itsFirstPlotTime.ChangeByDays(itsFirstDeltaDays-itsFirstOriginalDeltaDays);//6.4 original...
//ei ole	itsFirstPlotTime.ChangeByYears(itsFirstDeltaYears-itsFirstOriginalDeltaYears);//220199
/*	short year = itsFirstPlotTime.GetYear();
	year += itsFirstDeltaYears-itsFirstOriginalDeltaYears;
    itsFirstPlotTime.SetYear(year);
*/
	itsFirstPlotTime.SetTime(itsFirstPlotHours);
}
//---------------------------------------------------------------------------
bool NFmiPressTimeDescription::SetDataHour(NFmiFastQueryInfo* data, const NFmiString& calledFrom) //23.8.99 oli QD
{
 if(!IsHourLoop())  //12.8 Kun luupataan päivän sisällä ei voi käyttää omaa tuntia
	                //     vaan käytetään pressparamin aikaluuppi-arvoja
					//     pidemmissä (>vrk) luupeissa pitäisi oma arvo toimia tässäkin
 {
   NFmiMetTime saveTime = (static_cast<NFmiQueryInfo *>(data))->Time(); //4.9.01 HUOM
   NFmiMetTime tim = saveTime;
   if(!(tim.GetHour() == itsFirstPlotHours) )//&& fFirstStation)
		  *itsLogFile << "    tunti muutettu pirtoalkiossa: " << itsFirstPlotHours << " utc" << endl;

   tim.SetTime(itsFirstPlotHours); 
   if(!data->Time(tim))  //29.6 nyt aika ohjataan tällä alimmalla tasolla
   {	                     //pääseeköhän tänne jos ei aikaa
	   if(itsLogFile)      
		  *itsLogFile << "*** ERROR: aikaa ei ole: " << tim
   	              << static_cast<char *>(calledFrom) << endl;
       data->Time(saveTime);  //pitää asettaa takaisin 
	   return false;
   }
 }
 return true;
}
//---------------------------------------------------------------------------
NFmiMetTime NFmiPressTimeDescription::TimeToWrite(NFmiMetTime givenTime)const
{//14.2.01
//	NFmiMetTime time;
	if(fWriteAsUtc)
		  return givenTime;
	else 
		return 	NFmiMetTime(givenTime.LocalTime(25.f),1);	
}
