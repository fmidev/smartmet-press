//� Ilmatieteenlaitos/Lasse.
//  Original 2.4.1998 

//Muutettu 121099/LW +ReadRemaining() jossa P�iv� ja Tunti
//Muutettu 131000/LW InterpretToStr()-k�ytt�
//Muutettu 180402/LW utc-tulostus kuntoon
//---------------------------------------------------------------------------

#include "nptigtex.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiPressGivenTimeText::~NFmiPressGivenTimeText() 
{
};
//121099----------------------------------------------------------------------------
FmiBoolean NFmiPressGivenTimeText::ReadRemaining(void)  
{
	unsigned long long1;
	switch(itsIntObject)
	{	
/*		case dAddASecondTime:   //121099 ei n�in sittenk��n  
		{
			fHasASecondTime = kTrue;
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
			   itsFirstPlotTime.SetHour((short)long1);

			ReadNext();
			break;
		}
		default:
		{
			NFmiPressTimeText:: ReadRemaining();
			break;
		}
	}
	return kTrue;
}
/*
//---------------------------------------------------------------------------
int NFmiPressGivenTimeText::ConvertDefText(NFmiString & object) //121099
{
	if(object==NFmiString("AddASecondTime") || object==NFmiString("Lis��ToinenAika"))
		return dAddASecondTime;
	else
		return NFmiPressTimeText::ConvertDefText(object);
	
}
*/
//---------------------------------------------------------------------------
FmiBoolean	NFmiPressGivenTimeText::WritePS(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

//    SetText(((NFmiPressTime)(itsFirstPlotTime.LocalTime(25.f))).ToStr(itsFormat,itsLanguage));
	SetText();
	return WriteString(NFmiString("AnnettuAikaTeksti"), theOutput);
};
//131099---------------------------------------------------------------------------
FmiBoolean	NFmiPressGivenTimeText::SetText()									
{
	if(itsStringNameTimeFormat.IsValue())
	{
		if(fWriteAsUtc) //18.4.02
			SetText(((NFmiPressTime)(itsFirstPlotTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
		else
			SetText(((NFmiPressTime)(itsFirstPlotTime.LocalTime(25.f))).InterpretToStr(itsStringNameTimeFormat,itsLanguage)); 
	}
	else
	{
		if(fWriteAsUtc) //18.4.02
			SetText(((NFmiPressTime)(itsFirstPlotTime)).ToStr(itsFormat,itsLanguage));
		else
			SetText(((NFmiPressTime)(itsFirstPlotTime.LocalTime(25.f))).ToStr(itsFormat,itsLanguage));
	}
	return kTrue;
};
