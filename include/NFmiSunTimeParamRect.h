// © Ilmatieteenlaitos/Lasse.
// Original 29.2.2000
//
// Auringon nousu- ja laskuaika
// 
// Muutettu xxxxxx/LW 

//---------------------------------------------------------------------------

#ifndef __NFMISUNTIMEPARAMRECT_H__
#define __NFMISUNTIMEPARAMRECT_H__

typedef enum
{
	 dSunRise = 535
	,dSunSet //29.2.2000

}NFmiSunTimeParamRectObjects;


#include "NFmiTimeParamRect.h"
//#include "NFmiPressTime.h"

class _FMI_DLL NFmiSunTimeParamRect : public NFmiTimeParamRect
{
	public:
		NFmiSunTimeParamRect(void) :  NFmiTimeParamRect()
						,fIsSunRise(true)
						{itsFormat=kHHdMM;};

		NFmiSunTimeParamRect(const NFmiSunTimeParamRect& theTimeParamRect); 

		virtual ~NFmiSunTimeParamRect();

		virtual bool		ReadRemaining(void); 
		virtual NFmiParamRect*	Clone(void) const; 
        int ConvertDefText(NFmiString & object);

	protected:
		NFmiTime	TimeToWrite(NFmiFastQueryInfo* theQI);

	private:
		bool fIsSunRise;
};

#endif //__NLETPARR_H__
