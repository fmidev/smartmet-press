//� Ilmatieteenlaitos/Lasse.
//  Original 19.2.2000 

//Muutettu 020600/LW puuttuva lasku/nousu hoidettu

//---------------------------------------------------------------------------

#include "NFmiSunTimeParamRect.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

NFmiSunTimeParamRect::NFmiSunTimeParamRect(const NFmiSunTimeParamRect& theSunTimeParamRect)
//:NFmiTimeParamRect(*(NFmiTimeParamRect*)&theSunTimeParamRect)
:NFmiTimeParamRect(theSunTimeParamRect)
,fIsSunRise(theSunTimeParamRect.fIsSunRise)

{
}
//---------------------------------------------------------------------------
NFmiSunTimeParamRect::~NFmiSunTimeParamRect() 
{
};
//---------------------------------------------------------------------------
NFmiParamRect* NFmiSunTimeParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiSunTimeParamRect(*this);
};
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
FmiBoolean NFmiSunTimeParamRect::ReadRemaining(void) 
{			 
//	long long1;
//	double double1,double2; 

	switch(itsIntObject)
	{
		case dSunRise:	  
		{
			fIsSunRise = kTrue;
			ReadNext();

			break;
		}
		case dSunSet:	  
		{
			fIsSunRise = kFalse;
			ReadNext();

			break;
		}
		default: 
		{
			return NFmiTimeParamRect::ReadRemaining();  
			break;
		}
	}
	return kTrue;
}
//---------------------------------------------------------------------------
int NFmiSunTimeParamRect::ConvertDefText(NFmiString & object) 
{
	if(object==NFmiString("SunRise") || object==NFmiString("AuringonNousu")
		                 || object==NFmiString("Auringonnousu")
						 || object==NFmiString("Nousu"))
		return dSunRise;
	else if(object==NFmiString("SunSet") || object==NFmiString("AuringonLasku")
		                 || object==NFmiString("Auringonlasku")
						 || object==NFmiString("Lasku")) 
		return dSunSet;
	else
		return NFmiTimeParamRect::ConvertDefText(object);
	
}
//---------------------------------------------------------------------------
NFmiTime NFmiSunTimeParamRect::TimeToWrite(NFmiFastQueryInfo* theQI) 
	//aina local time piirtoalkioissa (vrt segmentti ja tuote miss� ei)
{
	NFmiMetTime time = itsCurrentSegmentTime; 
		
		double latitude = itsStationPoint.GetLatitude();//Testiin
		double longitude = itsStationPoint.GetLongitude();//Testiin
		if(fabs(longitude) <= .001 && fabs(latitude) <= .001) //ei pit�isi voida tapahtua
			*itsLogFile << "*** ERROR: aurinkoajan longitudi puuttuu " << (char*)itsStationPoint.GetName()
			            << ":lta paikalliseen aikaan"<< endl;
		
//	FmiBoolean currentDay; 
	if(fIsSunRise)
	{    //2.6.00 +fIsValidTime; false jos ei laske/nouse
		return itsStationPoint.TimeOfSunrise(time, (FmiBoolean&)fIsValidTime);
	}
	else
	{
		return itsStationPoint.TimeOfSunset(time, (FmiBoolean&)fIsValidTime);
	}
}

