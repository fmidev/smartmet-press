//© Ilmatieteenlaitos/Lasse.
//  Original 19.3.1998 

//Muutettu xxxxxx/LW

//---------------------------------------------------------------------------

#include "nppartex.h"
#include "NFmiFastQueryInfo.h"

//---------------------------------------------------------------------------
FmiBoolean	NFmiPressParamText::WritePS(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

    SetText(itsData->Param().GetParamName());

	return WriteString(NFmiString("PARAMETRITEKSTI"), theOutput);
};
