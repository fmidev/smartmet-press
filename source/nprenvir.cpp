//© Ilmatieteenlaitos/Lasse.
//  Original 17.11.1998
// 
// Muutettu  070901
//---------------------------------------------------------------------------

#include "nprenvir.h"
#include "NFmiPressArea.h"

//---------------------------------------------------------------------------
NFmiPressEnvironment::NFmiPressEnvironment(const NFmiPressEnvironment& theOtherEnvironment)
: itsMaskNumber(theOtherEnvironment.itsMaskNumber)
 ,fLongNumberMinus(theOtherEnvironment.fLongNumberMinus)
 ,itsGenericColor(theOtherEnvironment.itsGenericColor)
 ,itsAdditionalDayAdvance(theOtherEnvironment.itsAdditionalDayAdvance) //23.5.02
{
};

//---------------------------------------------------------------------------
NFmiPressEnvironment::~NFmiPressEnvironment() 
{
};
