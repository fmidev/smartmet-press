//© Ilmatieteenlaitos/Lasse.
//  Original 9.4.1998 

//Muutettu xxxxxx/LW 
//Muutettu 180699/LW +itsDeltaDays, ReadRemaining(),ConvertDefText() 
//Muutettu 180699/LW +InterpretToStr()-käyttö
//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPressComputerTimeText.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiPressComputerTimeText::~NFmiPressComputerTimeText() 
{
};
//----------------------------------------------------------------------------
bool	NFmiPressComputerTimeText::SetText()									
{
    NFmiMetTime computerTime = NFmiMetTime(1);
	NFmiMetTime useTime = TimeToWrite(computerTime); //14.2.01
//	NFmiMetTime localTime(computerTime.LocalTime(25.f),1); 
	useTime.ChangeByDays(itsDeltaDays);  
	if(itsStringNameTimeFormat.IsValue())   //13.10.00
		SetText((static_cast<NFmiPressTime>(useTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
	else
		SetText((static_cast<NFmiPressTime>(useTime)).ToStr(itsFormat,itsLanguage));

	return true;
}
//---------------------------------------------------------------------------
bool NFmiPressComputerTimeText::ReadRemaining(void)  
{ //18.6.99
	long long1;

	switch(itsIntObject)
	{
		case dTimeTextDays:	  
		{
			if (!ReadEqualChar())
				break;

 			if(ReadOne(long1))    
              itsDeltaDays =  static_cast<short>(long1);

			ReadNext();
			break;
		}
		default: 
		{
			return NFmiPressTimeText::ReadRemaining();  //131099 pois kommentista
			break;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
int NFmiPressComputerTimeText::ConvertDefText(NFmiString & object) //LW 20.11
{
	if(object==NFmiString("Day") || object==NFmiString("Päivä"))
		return dTimeTextDays;
	else
		return NFmiPressTimeText::ConvertDefText(object);
	
}
//---------------------------------------------------------------------------
bool	NFmiPressComputerTimeText::WritePS(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

/*    NFmiMetTime computerTime = NFmiMetTime(1);
	NFmiMetTime localTime(computerTime.LocalTime(25.f),1); //Vilin avustuksella
	localTime.ChangeByDays(itsDeltaDays);  //18.6.99

	SetText(((NFmiPressTime)localTime).ToStr(itsFormat,itsLanguage));
*/
	SetText();

//    SetText(((NFmiPressTime)(itsTime.LocalTime(25.f))).ToStr(itsFormat,itsLanguage));

	return WriteString(NFmiString("KoneenAika"),theOutput);
};
