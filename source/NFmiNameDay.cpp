//© Ilmatieteenlaitos/Lasse.
//  Original 25.1.2000 

//Muutettu xxxxxx/LW 

//---------------------------------------------------------------------------

#include "NFmiNameDay.h"
#include "NFmiTime.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiNameDay::~NFmiNameDay() 
{
};
//---------------------------------------------------------------------------
bool	NFmiNameDay::ReadFile(const NFmiString& theFileName)									
{
	if(fRead)       //turhat uusinta-readit estetty
		return fValue;

	fRead = true;
	fValue = false;

	char dateBuff[20];
	char nameBuff[20];
	NFmiString name, date;
	NFmiString lastDate("tyhjä");
	ifstream in(theFileName, ios::in);
	short julDay = 0;
	if(in)
	{
		while(in)
		{
			in >> dateBuff;
			in >> nameBuff;
			if(in)
			{
				name = NFmiString(nameBuff);
				date = NFmiString(dateBuff);
				if(date == lastDate)
				{
					if(julDay > 0)
						itsNames[julDay] += NFmiString(", "); 
					itsNames[julDay] += name;
				}
				else
				{
					julDay++;
					if(julDay > 365)
					{
						in.close();
						return false;
					}
					itsNames[julDay] = name;
				}
				lastDate = date;
			}
		}
		in.close();
		if(julDay == 365)
		{
			fValue = true;
			return true;
		}
		else
			return false;
	}
	else
		return false;
};
//---------------------------------------------------------------------------
NFmiString NFmiNameDay::GetName(NFmiMetTime theTime, FmiCounter maxNumber, FmiCounter maxLength)
{
	int indYear = theTime.GetJulianDay();
	NFmiString names(itsNames[indYear]);

	//karkausvuonna indeksi yhden eteenpäin
	// HUOM oletetaan että nimipäivä on kuluvaa vuotta 
	if(theTime.DaysInMonth(2, theTime.GetYear()) == 29)
	{
		if(indYear == 60)
		{
//			if(itsLanguage == kFinnish) //EI OLE KIELTÄ 
				names = NFmiString("Karkauspäivä");
//			else
//				names = NFmiString("Skottdagen");
		}
		else if(indYear > 60)
			names = itsNames[indYear-1];
	}
	NFmiString separator(",");
	NFmiString helpString;
	if(maxNumber != kShortMissing && maxNumber > 0)
	{
		unsigned short num = names.CharCount(',')+1;
		if(num > maxNumber)
		{
			unsigned long fromPos = 1;
			for (int ind=1;ind<=maxNumber;ind++)
			{
				fromPos = names.Search(separator, fromPos)+1;
			}
			names = names.GetChars(1, fromPos-2);
		}
	}
	int len = names.GetLen();
	if(maxLength != kShortMissing && len > maxLength && maxLength > 0)
	{
		helpString = names.GetChars(1, maxLength+1); //jos heti seuraava pilkku se mukaan
		unsigned long lastStep = helpString.SearchLast(separator);
		if(lastStep > 0)
			names = helpString.GetChars(1, lastStep-1);
		else
			names = NFmiString();
	}
	return names;
}
