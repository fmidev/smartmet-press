//� Ilmatieteenlaitos/Lasse.
//  Original 25.1.2000 

//Muutettu xxxxxx/LW 

//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k��nt�j�n varoitusta
#endif

#include "NFmiPressNameDay.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NFmiPressNameDay::~NFmiPressNameDay() 
{
};
//---------------------------------------------------------------------------
bool NFmiPressNameDay::ReadRemaining(void)  
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
	return true;
}
//---------------------------------------------------------------------------
int NFmiPressNameDay::ConvertDefText(NFmiString & object) 
{
	if(object==NFmiString("MaxNumber") || object==NFmiString("MaksimiM��r�"))
		return dMaxNumber;
	else if(object==NFmiString("MaxLength") || object==NFmiString("MaksimiPituus"))
		return dMaxLength;
	else
		return NFmiPressText::ConvertDefText(object);
	
}

//---------------------------------------------------------------------------
bool	NFmiPressNameDay::WritePS(FmiPressOutputMode theOutput)									
{
	if(!itsNameDay)
	{
		*itsLogFile << "*** ERROR: NimiP�iv�Olio puuttuu, ohjelmointivirhe" << endl;
		return false;
	}
    ScalePlotting();

	NFmiString str;

	if(!itsNameDay->IsRead())
	{
		NFmiString fileName = GetHome();
		fileName += kFmiDirectorySeparator;
		fileName += NFmiString("Muut");
		fileName += kFmiDirectorySeparator;
		if(itsLanguage == kFinnish)
		  {
			fileName += NFmiString("nimip�iv�t.txt"); 
		  }
		else
		  {
			fileName += NFmiString("nimip�iv�tRuotsi.txt"); 
		  }

		if(!itsNameDay->ReadFile(fileName))
		{
			*itsLogFile << "*** ERROR: Nimip�ivien lukeminen ep�onnistui" << endl;
			return false; 
		}
	}

	if(itsNameDay->IsValue())
	{
		str = itsNameDay->GetName(itsFirstPlotTime, itsMaxNumber, itsMaxLength);
		SetText(str);
		return WriteString(NFmiString("NIMIP�IV�"), theOutput);
	}
	else
		return false;
};
