//© Ilmatieteenlaitos/Lasse.
//  Original 3.1.2001 

//Muutettu xxxxxx/LW puuttuva lasku/nousu hoidettu
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressParam.h"

//---------------------------------------------------------------------------

#include "NFmiExtremeTimeParamRect.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

NFmiExtremeTimeParamRect::NFmiExtremeTimeParamRect(const NFmiExtremeTimeParamRect& theExtremeTimeParamRect)
:NFmiTimeParamRect(theExtremeTimeParamRect)
//:NFmiTimeParamRect(*(NFmiTimeParamRect*)&theExtremeTimeParamRect)
{
}
//---------------------------------------------------------------------------
NFmiExtremeTimeParamRect::~NFmiExtremeTimeParamRect() 
{
};
//---------------------------------------------------------------------------
NFmiParamRect* NFmiExtremeTimeParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiExtremeTimeParamRect(*this);
};
//----------------------------------------------------------------------------
//---------------------------------------------------------------------------
NFmiTime NFmiExtremeTimeParamRect::TimeToWrite(NFmiFastQueryInfo* theQI) 
{
	return itsPressParam->GetOptionTime();
}

