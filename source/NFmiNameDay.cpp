// ======================================================================
/*
 * \file
 * \brief Implementation of class NFmiNameDay
 */
// ======================================================================

#include "NFmiNameDay.h"
#include "NFmiTime.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiNameDay::~NFmiNameDay(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFileName Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiNameDay::ReadFile(const NFmiString & theFileName)
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
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 * \param maxNumber Undocumented
 * \param maxLength Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiNameDay::GetName(NFmiMetTime theTime,
								FmiCounter maxNumber,
								FmiCounter maxLength)
{
  int indYear = theTime.GetJulianDay();
  //karkausvuotena kaatui
  //NFmiString names(itsNames[indYear]);
  NFmiString names;
  
  //karkausvuonna indeksi yhden eteenpäin
  // HUOM oletetaan että nimipäivä on kuluvaa vuotta 

  if(theTime.DaysInMonth(2, theTime.GetYear()) == 29)
	{
	  if(indYear == 60)
		{
		  names = NFmiString("Karkauspäivä");
		}
	  else if(indYear > 60)
		names = itsNames[indYear-1];
	}
  else
	  names = itsNames[indYear];

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
	   // jos heti seuraava pilkku se mukaan
	  helpString = names.GetChars(1, maxLength+1);
	  unsigned long lastStep = helpString.SearchLast(separator);
	  if(lastStep > 0)
		names = helpString.GetChars(1, lastStep-1);
	  else
		names = NFmiString();
	}
  return names;
}

// ======================================================================
