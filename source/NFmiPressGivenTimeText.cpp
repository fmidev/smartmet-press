//© Ilmatieteenlaitos/Lasse.
//  Original 2.4.1998 

//Muutettu 121099/LW +ReadRemaining() jossa P‰iv‰ ja Tunti
//Muutettu 131000/LW InterpretToStr()-k‰yttˆ
//Muutettu 180402/LW utc-tulostus kuntoon
//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressGivenTimeText.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiPressGivenTimeText::~NFmiPressGivenTimeText() 
{
};
//121099----------------------------------------------------------------------------
bool NFmiPressGivenTimeText::ReadRemaining(void)  
{
	unsigned long long1;
	switch(itsIntObject)
	{	
/*		case dAddASecondTime:   //121099 ei n‰in sittenk‰‰n  
		{
			fHasASecondTime = true;
			itsSecondTime = itsFirstPlotTime;
			itsSecondTime.ChangeByDays(1); //oletus vaikka ylihuominen (kun varsinaisen oletus on huomenna)

			ReadNext();
			break;
		} */
		case dRelDay:     
		{
			if (!ReadEqualChar())
				break;

			if(ReadOne(long1))
			{			
			   itsFirstPlotTime = NFmiMetTime();
			   itsFirstPlotTime.ChangeByDays(long1+ itsEnvironment.GetDayAdvance()); //23.5.02 +GetDayAdvance); );
			}

			ReadNext();
			break;
		}
		case dHour:       
		{
			if (!ReadEqualChar())
				break;

 			if(ReadOne(long1))    
			   itsFirstPlotTime.SetHour(static_cast<short>(long1));

			ReadNext();
			break;
		}
		default:
		{
			NFmiPressTimeText:: ReadRemaining();
			break;
		}
	}
	return true;
}
/*
//---------------------------------------------------------------------------
int NFmiPressGivenTimeText::ConvertDefText(NFmiString & object) //121099
{
	if(object==NFmiString("AddASecondTime") || object==NFmiString("Lis‰‰ToinenAika"))
		return dAddASecondTime;
	else
		return NFmiPressTimeText::ConvertDefText(object);
	
}
*/
//---------------------------------------------------------------------------
bool	NFmiPressGivenTimeText::WritePS(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

//    SetText(((NFmiPressTime)(itsFirstPlotTime.LocalTime(25.f))).ToStr(itsFormat,itsLanguage));
	SetText();
	return WriteString(NFmiString("AnnettuAikaTeksti"), theOutput);
};
//131099---------------------------------------------------------------------------
bool	NFmiPressGivenTimeText::SetText()									
{
	if(itsStringNameTimeFormat.IsValue())
	{
		if(fWriteAsUtc) //18.4.02
			SetText((static_cast<NFmiPressTime>(itsFirstPlotTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
		else
			SetText((static_cast<NFmiPressTime>(itsFirstPlotTime.LocalTime(25.f))).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
	}
	else
	{
		if(fWriteAsUtc) //18.4.02
			SetText((static_cast<NFmiPressTime>(itsFirstPlotTime)).ToStr(itsFormat,itsLanguage));
		else
			SetText((static_cast<NFmiPressTime>(itsFirstPlotTime.LocalTime(25.f))).ToStr(itsFormat,itsLanguage));
	}
	return true;
};
