// © Ilmatieteenlaitos/Lasse.
// Original 3.1.2001
//
// Datan ‰‰riarvojen sattumisajan luokka.
// Vaatii aina parikseen dataolion, joka suoritetaan ensin ja jossa ‰‰riarvo (max/min)
// m‰‰ritet‰‰n ja jossa samalla talletetaan aika t‰t‰ oliota varten.
// Aika on viety NFmiPressParam:lle, joka on yhteinen Container molemmille.
//  
// 
// Muutettu xxxxxx/LW 

//---------------------------------------------------------------------------

#ifndef __NFMIEXTREMETIMEPARAMRECT_H__
#define __NFMIEXTREMETIMEPARAMRECT_H__

#include "ntimparr.h"

class _FMI_DLL NFmiExtremeTimeParamRect : public NFmiTimeParamRect
{
	public:
		NFmiExtremeTimeParamRect(void) :  NFmiTimeParamRect()						
						{};

		NFmiExtremeTimeParamRect(const NFmiExtremeTimeParamRect& theTimeParamRect); 

		virtual ~NFmiExtremeTimeParamRect();

//		virtual FmiBoolean		ReadRemaining(void); 
		virtual NFmiParamRect*	Clone(void) const; 
//        int ConvertDefText(NFmiString & object);

	protected:
		NFmiTime	TimeToWrite(NFmiFastQueryInfo* theQI);

	private:
};

#endif //__NFMIEXTREMETIMEPARAMRECT_H__
