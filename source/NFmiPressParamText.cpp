//© Ilmatieteenlaitos/Lasse.
//  Original 19.3.1998 

//Muutettu xxxxxx/LW

//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressParamText.h"
#include "NFmiFastQueryInfo.h"

//---------------------------------------------------------------------------
bool	NFmiPressParamText::WritePS(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

    SetText(itsData->Param().GetParamName());

	return WriteString(NFmiString("PARAMETRITEKSTI"), theOutput);
};
