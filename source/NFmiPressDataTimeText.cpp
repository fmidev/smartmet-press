//© Ilmatieteenlaitos/Lasse.
//  Original 2.4.1998 

//Muutettu 271098/LW 
//Muutettu 160800/LW 
//Muutettu 131000/LW InterpretToStr-k‰yttˆ

//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressDataTimeText.h"
//#include "NFmiSuperSmartInfo.h"
#include "NFmiFastQueryInfo.h"

//---------------------------------------------------------------------------
NFmiPressDataTimeText::~NFmiPressDataTimeText() 
{
};
//250201----------------------------------------------------------------------------
bool NFmiPressDataTimeText::ReadRemaining(void)  
{
	NFmiString helpString;
	NFmiValueString valueString;
	switch(itsIntObject)
	{	
		case dUseOriginTime:   
		{
			fUseOriginTime = true; 

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

//---------------------------------------------------------------------------
int NFmiPressDataTimeText::ConvertDefText(NFmiString & object) //LW 20.11
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //20.3.00 kaikille pit‰isi sallia vapaa isot/pienet kirj.
	if(lowChar==NFmiString("analysetime") || lowChar==NFmiString("analyysiaika"))
		return dUseOriginTime;
	else
		return NFmiPressTimeText::ConvertDefText(object);
	
}
//---------------------------------------------------------------------------
// kutsutaan segmentist‰: GivenTimeText pit‰‰ p‰ivitt‰‰ oikealla ajalla ainakin aikataulukossa
bool	NFmiPressDataTimeText::WritePSUpdatingSubText(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

	NFmiMetTime utcTime;
	NFmiMetTime localTime;

	if(fUseOriginTime)
	{
			utcTime = (static_cast<NFmiMetTime>(itsData->OriginTime())); 
			localTime = (static_cast<NFmiMetTime>(itsData->OriginTime())).LocalTime(); 
	}
	else
	{
			utcTime = (static_cast<NFmiMetTime>((static_cast<NFmiQueryInfo *>(itsData))->Time())); 
			localTime = (static_cast<NFmiMetTime>((static_cast<NFmiQueryInfo *>(itsData))->Time())).LocalTime(); 
	}

	if(itsStringNameTimeFormat.IsValue())
	{
		if(fWriteAsUtc) //25.2.01		
			SetText((static_cast<NFmiPressTime>(utcTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage));
		else
			SetText((static_cast<NFmiPressTime>(localTime)).InterpretToStr(itsStringNameTimeFormat,itsLanguage));

	}
	else
	{
		if(fWriteAsUtc) //18.4.02		
			SetText((static_cast<NFmiPressTime>(utcTime)).ToStr(itsFormat,itsLanguage));
		else
			SetText((static_cast<NFmiPressTime>(localTime)).ToStr(itsFormat,itsLanguage));
	}

	if(itsSubText)
//		if(itsSubText->IsWriteAsUtc())
			itsSubText->SetTime(NFmiMetTime(utcTime));
//		else
//			itsSubText->SetTime(NFmiMetTime(localTime));

	return WriteString(NFmiString("AIKATEKSTI"), theOutput);
};
//---------------------------------------------------------------------------
bool	NFmiPressDataTimeText::WritePS(FmiPressOutputMode theOutput)									
{
    ScalePlotting();

	// Mika suspects a bug in the reinterpret cast,
	// since the cast is from NFmiFastQueryInfo * to NFmiQueryData *
	NFmiTime tim = (static_cast<NFmiMetTime>((reinterpret_cast<NFmiQueryData *>(itsData))->Time())).LocalTime(); //oletus Suomen aika
    SetText((static_cast<NFmiPressTime>(tim)).ToStr(itsFormat,itsLanguage)); 

	return WriteString(NFmiString("AIKATEKSTI"), theOutput);
};
