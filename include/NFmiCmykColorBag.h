// © Ilmatieteenlaitos/Lasse.
// Original 10.6.1998
// 
// Muutettu xxxxxx	
//---------------------------------------------------------------------------

#ifndef __NCMYKBAG_H__
#define __NCMYKBAG_H__

#include "NFmiValueString.h"
#include "NFmiSize.h"


typedef struct
{
	double		c;
	double		m;
	double		y;
	double		k;
}NFmiCmykColor;

typedef struct
{
	NFmiCmykColor color;
	NFmiString	  name;
}NFmiNamedCmykColor;


// NFmiParamMapping: organized series of mapping intervals; knows how to add
// new mapping interval; when given a value, finds the right interval and returns
// associated symbol.

class _FMI_DLL NFmiCmykColorBag : public NFmiSize
{
	public:
		NFmiCmykColorBag() : NFmiSize(0), itsColors(0) {};
		NFmiCmykColorBag(const NFmiCmykColorBag& theCmykColorBag);
		virtual ~NFmiCmykColorBag();

		void		AddColor(const NFmiNamedCmykColor & theColor);
/*		void		AddColor(double theCValue, double theMValue
			               ,double theYValue, double theKValue
									  , const NFmiString & theName);
*/
		NFmiCmykColor* GetColor (const NFmiString& theName) const;
		NFmiString GetColorString (const NFmiString& theName) const;

	private:
		NFmiNamedCmykColor*	itsColors;
};

#endif // __NPARAMAP_H__

