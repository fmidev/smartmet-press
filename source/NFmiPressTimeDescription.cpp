// ======================================================================
/*
 *†\file
 * \brief Implementation of class NFmiPressTimeDescription
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiPressTimeDescription.h"
#include "NFmiPressTime.h"
// newbase
#include "NFmiFastQueryInfo.h"
#include "NFmiValueString.h"
// system
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param isSecond Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

unsigned long NFmiPressTimeDescription::ReadTimeFormat(bool isSecond)
{
  NFmiString helpString;
  unsigned long format = kWwsHH; //oletus

  helpString = ReadString(); //tuo myˆs v‰lilyˆnnit hipsuissa
 
  if (helpString == NFmiString ("HHdMM"))   
	format = kHHdMM;
  else if (helpString == NFmiString ("HdMM"))   
	format = kHdMM;
  else if (helpString == NFmiString ("wwsHH") || helpString == NFmiString ("WwsHH"))
	format = kWwsHH;
  else if (helpString == NFmiString ("DdMd"))
	format = kDdMd; 
  else if (helpString == NFmiString ("WwsDDdMMd"))
	format = kWwsDDdMMd; 
  else if (helpString == NFmiString ("WwsDdMd"))
	format = kWwsDdMd; 
  else if (helpString == NFmiString ("NnnnsYYYY"))
	format = kNnnnsYYYY; 
  else if (helpString == NFmiString ("DDdMMdYYYY")) 
	format = kDDdMMdYYYY; 
  else if (helpString == NFmiString ("DdMdYYYY"))   
	format = kDdMdYYYY; 
  else
	{
	  if(isSecond)
		itsSecondStringNameTimeFormat = helpString;
	  else
		itsStringNameTimeFormat = helpString;
	}
  return format;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::ReadRemaining(void)  
{ 
  long long1, long2, long3;
  NFmiValueString valueString;
  switch(itsIntObject)
	{	
	case dGivenRelHours:
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
	case dWriteTimeAsUtc:
	  {
		fWriteAsUtc = true;
		
		ReadNext();
		break;
	  }
	case dTimeTableActive:
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
			  *itsLogFile << "*** ERROR: Not valid looping (start,step,stop): "
						  << long1
						  << long2
						  << long3
						  << endl;
		  }
		
		
		itsIntObject = ConvertDefText(itsString);
		
		break;
	  }
	case dBypassValue:
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

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiPressTimeDescription::ConvertDefText(NFmiString & object) 
{
  NFmiString lowChar = object;
  lowChar.LowerCase(); 
  
  if(lowChar==NFmiString("day") ||
	 lowChar==NFmiString("p‰iv‰"))
	return dRelDay;
  else if(lowChar==NFmiString("minute") ||
		  lowChar==NFmiString("minuutti"))
	return dMinute;
  else if(lowChar==NFmiString("hour") ||
		  lowChar==NFmiString("tunti"))
	return dHour;
  else if(lowChar==NFmiString("relhour") ||
		  lowChar==NFmiString("suhteellinentunti"))
	return dRelHour;                     
  else if(lowChar==NFmiString("relhours") ||
		  lowChar==NFmiString("suhteellisettunnit")) // data-alkioissa
	return dGivenRelHours;                     
  else if(lowChar==NFmiString("hourstep") ||
		  lowChar==NFmiString("tuntiaskel") ||
		  lowChar==NFmiString("tuntiaskel"))
	return dHourStep;
  else if(lowChar==NFmiString("utcwriting") ||
		  lowChar==NFmiString("utctulostus")) // data-alkioissa
	return dWriteTimeAsUtc;                     
  else if(lowChar==NFmiString("timetableactive") ||
		  lowChar==NFmiString("aikataulukonaktiiviset"))
	return dTimeTableActive;
  else if(lowChar==NFmiString("bypassvalue") ||
		  lowChar==NFmiString("ohitusarvo"))
	return dBypassValue;
  else 
	return NFmiPressDescription::ConvertDefText(object);
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param currentInd Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::ActiveTimeIndex(int currentInd) const
{
  //halutessa olioita voidaan piirt‰‰ esim joka n:teen aika-askeleeseen
  // Mika: Kannattaisikohan laittaa sulkuja?
  if(itsLoopActivity.startIndex < 1 || (currentInd-itsLoopActivity.startIndex)
	 % itsLoopActivity.step == 0
	 && currentInd >= static_cast<int>(itsLoopActivity.startIndex)
	 && currentInd <= static_cast<int>(itsLoopActivity.stopIndex)
	 ) return true;
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param relHours Undocumented
 * \param hourRes Undocumented
 * \param hourDelta Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiMetTime NFmiPressTimeDescription::NextUseTime(long relHours, long hourRes, long hourDelta) 
{
  //MetTimelt‰ ei lˆydy palvelua, jolla saisi NextTimen kun resoluutio ei ala nollasta
  //     eli esim ajat ovat 2,8 14,20,2.... tai aina 12 (hourDelta)
  //esim nyt = 8.2. klo 8.30 utc ->
  //	         NextUseTime(4,24,12) -> 9.2. klo 12 utc
  //	         NextUseTime(3,24,12) -> 8.2. klo 12
  //	         NextUseTime(-3,6,1)  -> 8.2. klo 7   utc
  NFmiMetTime curTime(60); 
  curTime.ChangeByHours(relHours);
  NFmiMetTime compTime(60); 
  compTime.ChangeByDays(-2); //riitt‰‰kˆ
  compTime.SetHour(hourDelta);
  if (compTime >= curTime)
	*itsLogFile << "*** ERROR: suhteellinen aikaero liian suuri: "
				<< relHours
				<< endl;
  else
	{
	  while(compTime <= curTime)
		compTime.ChangeByHours(hourRes);
	}
  
  return compTime;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param dayDiff Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

long NFmiPressTimeDescription::JustifyByLeaps(long dayDiff) 
{
  //ei kyll‰ ihan tarkka, ja toimii vain yhdelle vuodelle toistaiseksi
  long ret = dayDiff;
  short add;
  NFmiTime now; 
  NFmiTime compDate = now;
  short sign = +1;        //onko c:ss‰
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

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressTimeDescription::SetPreReadingTimes(void) 
{
  NFmiTime curDate;
  curDate.SetHour(0);
  curDate.SetMin(0);
  NFmiTime firstPlotDate = NFmiTime(itsFirstPlotTime);
  firstPlotDate.SetHour(0);
  firstPlotDate.SetMin(0);
  itsFirstDeltaDays = static_cast<short>(firstPlotDate.DifferenceInDays(curDate));
  
  itsFirstOriginalDeltaDays = itsFirstDeltaDays;  
  itsFirstPlotHours = itsFirstPlotTime.GetHour(); 
  itsFirstPlotMinutes = itsFirstPlotTime.GetMin();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressTimeDescription::SetPostReadingTimes(void)
{
  itsFirstPlotTime.ChangeByDays(itsFirstDeltaDays-itsFirstOriginalDeltaDays);
  itsFirstPlotTime.SetTime(itsFirstPlotHours);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param data Undocumented
 * \param calledFrom Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressTimeDescription::SetDataHour(NFmiFastQueryInfo * data,
										   const NFmiString & calledFrom)
{
  // Kun luupataan p‰iv‰n sis‰ll‰ ei voi k‰ytt‰‰ omaa tuntia
  // vaan k‰ytet‰‰n pressparamin aikaluuppi-arvoja
  // pidemmiss‰ (>vrk) luupeissa pit‰isi oma arvo toimia t‰ss‰kin
  if(!IsHourLoop())
	{
	  NFmiMetTime saveTime = (static_cast<NFmiQueryInfo *>(data))->Time();
	  NFmiMetTime tim = saveTime;
	  if(!(tim.GetHour() == itsFirstPlotHours) )
		*itsLogFile << "    tunti muutettu pirtoalkiossa: "
					<< itsFirstPlotHours
					<< " utc"
					<< endl;

	  tim.SetTime(itsFirstPlotHours); 
	  if(!data->Time(tim))
		{
		  if(itsLogFile)      
			*itsLogFile << "*** ERROR: aikaa ei ole: "
						<< tim
						<< static_cast<char *>(calledFrom)
						<< endl;
		  data->Time(saveTime);  //pit‰‰ asettaa takaisin 
		  return false;
		}
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param givenTime Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiMetTime NFmiPressTimeDescription::TimeToWrite(NFmiMetTime givenTime) const
{
  if(fWriteAsUtc)
	return givenTime;
  else 
	return 	NFmiMetTime(givenTime.LocalTime(25.f),1);	
}

// ======================================================================
