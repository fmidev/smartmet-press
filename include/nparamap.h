// © Ilmatieteenlaitos/Salla.
// Original 20.10.1997
// 
// Muutettu 19.11.97	Copy constr.
// Muutettu 6.4.98	+fIsScaled ja uusi ja Map
// Muutettu 241198/LW	+IsInFirst()
//---------------------------------------------------------------------------

#ifndef __NPARAMAP_H__
#define __NPARAMAP_H__

#include "NFmiString.h"
#include "NFmiSize.h"


// NFmiMappingInterval: information of single mapping interval and associated symbol
typedef struct
{
	double		itsBottomValue;
	double		itsTopValue;
	NFmiString	itsSymbol;
	FmiBoolean fIsScaled;    //6.4

}NFmiMappingInterval;


// NFmiParamMapping: organized series of mapping intervals; knows how to add
// new mapping interval; when given a value, finds the right interval and returns
// associated symbol.

class _FMI_DLL NFmiParamMapping : public NFmiSize
{
	public:
		NFmiParamMapping() : NFmiSize(0), itsMappingIntervals(0) {};
		NFmiParamMapping(const NFmiParamMapping& theParamMapping);
		virtual ~NFmiParamMapping();

		void		AddMappingInterval(const NFmiMappingInterval & theInterval);
		void		AddMappingInterval(double theBottomValue, double theTopValue
									  , const NFmiString & theSymbol);
		FmiBoolean IsInFirst(const double theValue); //241198

		NFmiString*	Map(const double theValue);	
		NFmiString*	Map(const double theValue, FmiBoolean& outIsScaled);	

	private:
		NFmiMappingInterval*	itsMappingIntervals;
};

#endif // __NPARAMAP_H__

