//© Ilmatieteenlaitos/Lasse.
//  Original 19.2.2000 

//Muutettu 020600/LW puuttuva lasku/nousu hoidettu

//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

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
bool NFmiSunTimeParamRect::ReadRemaining(void) 
{			 
//	long long1;
//	double double1,double2; 

	switch(itsIntObject)
	{
		case dSunRise:	  
		{
			fIsSunRise = true;
			ReadNext();

			break;
		}
		case dSunSet:	  
		{
			fIsSunRise = false;
			ReadNext();

			break;
		}
		default: 
		{
			return NFmiTimeParamRect::ReadRemaining();  
			break;
		}
	}
	return true;
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
	//aina local time piirtoalkioissa (vrt segmentti ja tuote miss‰ ei)
{
	NFmiMetTime tim = itsCurrentSegmentTime; 
		
		double latitude = itsStationPoint.GetLatitude();//Testiin
		double longitude = itsStationPoint.GetLongitude();//Testiin
		if(fabs(longitude) <= .001 && fabs(latitude) <= .001) //ei pit‰isi voida tapahtua
			*itsLogFile << "*** ERROR: aurinkoajan longitudi puuttuu " << (char*)itsStationPoint.GetName()
			            << ":lta paikalliseen aikaan"<< endl;
		
//	bool currentDay; 
	if(fIsSunRise)
	{    //2.6.00 +fIsValidTime; false jos ei laske/nouse
		return itsStationPoint.TimeOfSunrise(tim, (bool&)fIsValidTime);
	}
	else
	{
		return itsStationPoint.TimeOfSunset(tim, (bool&)fIsValidTime);
	}
}

