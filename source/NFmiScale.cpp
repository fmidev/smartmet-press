/*-------------------------------------------------------------------------------------*/

//© Ilmatieteenlaitos/Lasse

//Originaali 5. 12.1994

//Muutettu   08.12.1994/LW  TEMPLATE
//Muutettu   xx.02.1995/LW  << >>
//Muutettu   19.10.1995/LW  Location(float)
//Muutettu   10.11.1995/LW  Expand(float)
//Muutettu   13.11.1995/LW  stdlib
//Muutettu    7.12.1995/LW  Operator +=
//Muutettu   20.12.1995/LW  Lisätty Check()-kutsuja
//Muutettu   21.12.1995/LW  Expandeja ym ei tehdä jollei Ok
//Muutettu   22.12.1995/LW  itsEpsilon
//Muutettu   17.1.1996/LW  Operator -= korjattu
// 041196/LW +IsMissing() ja tämän käyttö itsDataOk:n sijaan muutamassa paikassa
// 120397/LW hyväksytään akseli jossa alkuarvo=loppuarvo->saadaan meteogramiin yksi sarake
///*-------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <math.h>

#ifndef __NSCALE_H__
#include "NFmiScale.h"
#endif//__NSCALE_H__

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01


//______________________________________________________________________________
//inline
NFmiScale :: NFmiScale (void)
			  : itsStartValue (kFloatMissing)
			  , itsEndValue (kFloatMissing)
			  , itsEpsilon(0.f)
			  , itsDataOk(false)
			  , itsLimitCheck(false)
{
}
//______________________________________________________________________________
//inline
NFmiScale :: NFmiScale (float theStartValue, float theEndValue)
			  : itsStartValue (theStartValue),itsEndValue (theEndValue)
			  , itsEpsilon(0.f)
			  , itsLimitCheck(false)
{

  Check();
}

//______________________________________________________________________________
//inline
NFmiScale :: NFmiScale (const NFmiScale& anOtherScale)
  : NFmiSaveBase()
  , itsStartValue (anOtherScale.itsStartValue)
  , itsEndValue (anOtherScale.itsEndValue)
  , itsEpsilon (anOtherScale.itsEpsilon)
  , itsDataOk(anOtherScale.itsDataOk)
  , itsLimitCheck(anOtherScale.itsLimitCheck)
{
}
//______________________________________________________________________________
//inline
NFmiScale :: ~NFmiScale(void)
{
}
//______________________________________________________________________________
//inline
NFmiScale& NFmiScale :: operator= (const NFmiScale& anOtherScale)
{
	itsStartValue = anOtherScale.itsStartValue;
	itsEndValue = anOtherScale.itsEndValue;
	itsDataOk = anOtherScale.itsDataOk;
	itsLimitCheck = anOtherScale.itsLimitCheck;
	itsEpsilon = anOtherScale.itsEpsilon;

	return *this;
}
//______________________________________________________________________________
//inline
NFmiScale& NFmiScale :: operator+= (const NFmiScale& anOtherScale)
{	   // oletetaan että start < end
	if(itsStartValue == kFloatMissing)
	{
	   itsStartValue = anOtherScale.itsStartValue;
	}
	else if (anOtherScale.itsStartValue != kFloatMissing)
	{
	   itsStartValue = FmiMin(anOtherScale.itsStartValue, itsStartValue);
	}
	if (itsEndValue == kFloatMissing)
	{
	   	itsEndValue = anOtherScale.itsEndValue;
	}
	else if (anOtherScale.itsEndValue != kFloatMissing)
	{
	    itsEndValue = FmiMax(anOtherScale.itsEndValue, itsEndValue);
	}
	Check();
	return *this;
}
//______________________________________________________________________________
//inline
NFmiScale& NFmiScale :: operator-= (const NFmiScale& anOtherScale)
{	   // oletetaan että start < end
    if(Ok() && anOtherScale.Ok())
	{
	   if(Inside(anOtherScale.itsEndValue))
	   {
	      itsStartValue = anOtherScale.itsEndValue;
	   }
	   else if (Inside(anOtherScale.itsStartValue))
	   {
	      itsEndValue = anOtherScale.itsStartValue;
	   }
	   else if (anOtherScale.Inside(itsStartValue) && anOtherScale.Inside(itsEndValue))
	   {
	      itsStartValue = kFloatMissing;	   // vanha sisältyy täysin vähennettävään
	      itsEndValue = kFloatMissing;     
	   }
	}
	Check();
	return *this;
}
/*
//______________________________________________________________________________
ostream& NFmiScale :: Write(ostream &file)
{
	file << itsStartValue << " ";
	file << itsEndValue   << " ";
	return file;
}
//______________________________________________________________________________
istream& NFmiScale :: Read(istream &file)
{
	file >> itsStartValue;
	file >> itsEndValue;
	Check();
	return file;
}
*/
//______________________________________________________________________________
void NFmiScale :: ExcludePositive (void)
{    //2.5.01
    if(!IsMissing())  
	{
	   {
		   if(itsStartValue > 0.)
			   itsStartValue = 0.f;
		   if(itsEndValue > 0.)
			   itsStartValue = 0.f;
	   }
	   Check();
	} 
}
//______________________________________________________________________________
void NFmiScale :: ExcludeNegative (void)
{    //2.5.01
    if(!IsMissing())  
	{
	   {
		   if(itsStartValue < 0.)
			   itsStartValue = 0.f;
		   if(itsEndValue < 0.)
			   itsStartValue = 0.f;
	   }
	   Check();
	} 
}
//______________________________________________________________________________
//inline
void NFmiScale :: StartFromZeroOptionally (float theFactor)
{    // ei toimi laskevalle skaalalle
    if(!IsMissing()) // 041196/LW oli itsDataOk 
	{
	   if ((itsEndValue-itsStartValue)*theFactor > FmiMin(fabs(itsEndValue), fabs(itsStartValue))
		   && !Inside(0.f))
	   {
		   if(itsStartValue > 0.)
			   itsStartValue = 0.f;
	       else
			   itsEndValue = 0.f;
	   }
//	   Check();
	} 
}
//______________________________________________________________________________
//inline  //+041196/LW
bool NFmiScale :: IsMissing(void) const
{
  return itsStartValue == kFloatMissing ||
		 itsEndValue   == kFloatMissing; 
}
//______________________________________________________________________________
//inline
void NFmiScale :: Check (void)
{
  itsDataOk = itsStartValue != kFloatMissing &&
				  itsEndValue   != kFloatMissing; 
//			   && itsStartValue != itsEndValue;	  //120397 ******
  if (itsDataOk)            //041196/LW
      itsEpsilon = (itsEndValue - itsStartValue) * .001f;
  else 
	  itsEpsilon = .000001f; //041196/LW
}
//______________________________________________________________________________
//inline
void NFmiScale :: ExpandIfNotZero (float theFactor)
{
    if(itsDataOk)
	{
       float expansion = (itsEndValue - itsStartValue) * theFactor;
       if (itsStartValue != 0.)
           itsStartValue += -expansion; 
       if (itsEndValue != 0.)
          itsEndValue += expansion;
//       Check();
    } 
}
//______________________________________________________________________________
//inline
void NFmiScale :: ExpandIfEqual (float theInterval)
{
  if (itsStartValue == itsEndValue)	//041196/LW pois itsDataOk &&
  	  Expand (theInterval);
}
//______________________________________________________________________________
//inline
void NFmiScale :: Expand (float theInterval)
{
    if (!IsMissing()) //041196/LW oli itsDataOk
	{
	   itsStartValue -= theInterval;
	   itsEndValue += theInterval;
	   itsDataOk = true;                                  //041196/LW
       itsEpsilon = (itsEndValue - itsStartValue) * .001f; //041196/LW
	} 
}
//______________________________________________________________________________
//inline
bool NFmiScale :: Inside (float theValue) const
{	// epsilon näyttää tuovan yhden merkitsevän numeron lisää
	return itsDataOk && theValue+itsEpsilon >= itsStartValue && theValue-itsEpsilon <= itsEndValue
			  ? true : false;
}

//______________________________________________________________________________
//inline
float NFmiScale :: RelLocation (float theValue) const
{
  float value = kFloatMissing;
  if (itsDataOk && theValue != kFloatMissing)
	  if(Difference() > 0.f)   // 120397
		  value = (theValue-itsStartValue)/Difference();
      else
		  value = 0.f;

  return  value == kFloatMissing || !Inside(theValue) && itsLimitCheck
			? kFloatMissing : value;
}
//______________________________________________________________________________
//inline
float NFmiScale :: Location (float theRelValue) const
{
  return  itsDataOk ? Difference()*theRelValue + itsStartValue : kFloatMissing;
}








