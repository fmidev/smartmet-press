//© Ilmatieteenlaitos/Lasse.
//  Original 2.4.1998 

//Muutettu 271098/LW 
//Muutettu 160800/LW 
//Muutettu 131000/LW InterpretToStr-käyttö

//---------------------------------------------------------------------------

#include "nptidtex.h"
//#include "NFmiSuperSmartInfo.h"
#include "NFmifastQueryInfo.h"

//---------------------------------------------------------------------------
NFmiPressDataTimeText::~NFmiPressDataTimeText() 
{
};
//250201----------------------------------------------------------------------------
FmiBoolean NFmiPressDataTimeText::ReadRemaining(void)  
{
	NFmiString helpString;
	NFmiValueString valueString;
	switch(itsIntObject)
	{	
		case dUseOriginTime:   
		{
			fUseOriginTime = kTrue; 

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

//---------------------------------------------------------------------------
int NFmiPressDataTimeText::ConvertDefText(NFmiString & object) //LW 20.11
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //20.3.00 kaikille pitäisi sallia vapaa isot/pienet kirj.
	if(lowChar==NFmiString("analysetime") || lowChar==NFmiString("analyysiaika"))
		return dUseOriginTime;
	else
		return NFmiPressTimeText::ConvertDefText(object);
	
}
//---------------------------------------------------------------------------
// kutsutaan segmentistä: GivenTimeText pitää päivittää oikealla ajalla ainakin aikataulukossa
FmiBoolean	NFmiPressDataTimeText::WritePSUpdatingSubText(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

	NFmiMetTime utcTime;
	NFmiMetTime localTime;

	if(fUseOriginTime)
	{
			utcTime = ((NFmiMetTime)(itsData->OriginTime())); 
			localTime = ((NFmiMetTime)(itsData->OriginTime())).LocalTime(); 
	}
	else
	{
			utcTime = ((NFmiMetTime)(((NFmiQueryInfo*)itsData)->Time())); 
			localTime = ((NFmiMetTime)(((NFmiQueryInfo*)itsData)->Time())).LocalTime(); 
	}

	if(itsStringNameTimeFormat.IsValue())
	{
		if(fWriteAsUtc) //25.2.01		
			SetText(((NFmiPressTime)(utcTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage));
		else
			SetText(((NFmiPressTime)(localTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage));

	}
	else
	{
		if(fWriteAsUtc) //18.4.02		
			SetText(((NFmiPressTime)(utcTime)).ToStr(itsFormat,itsLanguage));
		else
			SetText(((NFmiPressTime)(localTime)).ToStr(itsFormat,itsLanguage));
	}

	if(itsSubText)
//		if(itsSubText->IsWriteAsUtc())
			itsSubText->SetTime(NFmiMetTime(utcTime));
//		else
//			itsSubText->SetTime(NFmiMetTime(localTime));

	return WriteString(NFmiString("AIKATEKSTI"), theOutput);
};
//---------------------------------------------------------------------------
FmiBoolean	NFmiPressDataTimeText::WritePS(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

	NFmiTime time = ((NFmiMetTime)(((NFmiQueryData*)itsData)->Time())).LocalTime(); //oletus Suomen aika
    SetText(((NFmiPressTime)(time)).ToStr(itsFormat,itsLanguage)); 

	return WriteString(NFmiString("AIKATEKSTI"), theOutput);
};
