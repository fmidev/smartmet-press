//© Ilmatieteenlaitos/Lasse.
//  Original 25.1.2000 

//Muutettu xxxxxx/LW 

//---------------------------------------------------------------------------

#include "NFmiPressNameDay.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NFmiPressNameDay::~NFmiPressNameDay() 
{
};
//---------------------------------------------------------------------------
FmiBoolean NFmiPressNameDay::ReadRemaining(void)  
{
	unsigned long long1;
	FmiCounter count;
	switch(itsIntObject)
	{	
		case dMaxLength:   
		{
			if (!ReadEqualChar())
				break;

			if (ReadOne(count))
			{
				itsMaxLength = count;
			}

			ReadNext();
			break;
		}
		case dMaxNumber:    
		{
			if (!ReadEqualChar())
				break;

			if (ReadOne(count))
			{
				itsMaxNumber = count;
			}

			ReadNext();
			break;
		}
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
	//**
		default:
		{
			NFmiPressText:: ReadRemaining();
			break;
		}
	}
	return kTrue;
}
//---------------------------------------------------------------------------
int NFmiPressNameDay::ConvertDefText(NFmiString & object) 
{
	if(object==NFmiString("MaxNumber") || object==NFmiString("MaksimiMäärä"))
		return dMaxNumber;
	else if(object==NFmiString("MaxLength") || object==NFmiString("MaksimiPituus"))
		return dMaxLength;
	else
		return NFmiPressText::ConvertDefText(object);
	
}

//---------------------------------------------------------------------------
FmiBoolean	NFmiPressNameDay::WritePS(FmiPressOutputMode theOutput)									
{
	if(!itsNameDay)
	{
		*itsLogFile << "*** ERROR: NimiPäiväOlio puuttuu, ohjelmointivirhe" << endl;
		return kFalse;
	}
    ScalePlotting();

	NFmiString string;

	if(!itsNameDay->IsRead())
	{
		NFmiString fileName = GetHome();
		fileName += kFmiDirectorySeparator;
		fileName += NFmiString("Muut");
		fileName += kFmiDirectorySeparator;
		if(itsLanguage == kFinnish)
		  {
			fileName += NFmiString("nimipäivät.txt"); 
		  }
		else
		  {
			fileName += NFmiString("nimipäivätRuotsi.txt"); 
		  }

		if(!itsNameDay->ReadFile(fileName))
		{
			*itsLogFile << "*** ERROR: Nimipäivien lukeminen epäonnistui" << endl;
			return kFalse; 
		}
	}

	if(itsNameDay->IsValue())
	{
		string = itsNameDay->GetName(itsFirstPlotTime, itsMaxNumber, itsMaxLength);
		SetText(string);
		return WriteString(NFmiString("NIMIPÄIVÄ"), theOutput);
	}
	else
		return kFalse;
};
