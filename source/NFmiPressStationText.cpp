//© Ilmatieteenlaitos/Lasse.
//  Original 16.3.1998 

//Muutettu 231098/LW +LocalTime
//Muutettu 051198/LW printNamen käyttö eikä oikean asemannimen
//Muutettu 240899/LW station eritavalla QI:sta kuin ajemmin QD:stä

//---------------------------------------------------------------------------

#include "NFmiPressStationText.h"
#include "NFmiStationPoint.h"
#include "NFmiPressParam.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NFmiPressStationText::~NFmiPressStationText() 
{
};
//7.6.00---------------------------------------------------------------------------
bool NFmiPressStationText::SetNewName(const NFmiRenaming& theRename)//7.6.00
// originalName avaimena:
// - vaihdetaan newName jos löytyy
// - tai tehdään uusi original/new -pari 
{
	if(itsNewNames)
	{
		NFmiVoidPtrIterator iter = NFmiVoidPtrIterator(itsNewNames); 
		iter.Reset();
		NFmiRenaming* renaming = (NFmiRenaming*)iter.Next();
		while (renaming)
		{
			if(renaming->originalName == theRename.originalName)
			{
				renaming->newName = theRename.newName;
				return true;
			}
			renaming = (NFmiRenaming*)iter.Next();
		}
	    NFmiRenaming* newRenaming = new NFmiRenaming;
	    newRenaming->originalName = theRename.originalName;
	    newRenaming->newName = theRename.newName;
	    itsNewNames->Add(newRenaming);
	}
	return false;
}
//---------------------------------------------------------------------------
bool	NFmiPressStationText::WritePS(FmiPressOutputMode theOutput)									
{
	if(!itsData)    //4.9
		return isFalse;

    ScalePlotting();

	NFmiString string;
//	string = itsData->Station().GetName(); 
	string = StationName(); //051198 

	if(fAddLocalTime)   //23.10
	{                   // vähän mutkan kautta
		if(itsData->IsGrid())
		{
			if(!fLoopErrorReported)
			{
				*itsLogFile << "  *** ERROR: gridistä ei saa paikallisia aikoja" << endl;
				fLoopErrorReported = true;
			}
		}
		else
		{
			NFmiStationPoint stationP;
			const NFmiLocation loc(*itsData->Location()); //23.8.99 
			stationP.SetIdent(loc.GetIdent());
			NFmiString tString = stationP.LocalWmoTime(12);
	//		if(string.GetLen() < 10) //121198 HS haluaa kaikkiin
				string += (" ");
			string += ("(");
			string += tString;
			if(tString == NFmiString("-"))
			   *itsLogFile << "  *** ERROR: asemalta " << (char*)stationP.GetName() 
							<< " puuttuu paik.aika" << endl;
			string += (")");
		}
	}
 
	SetText(string);

	return WriteString(NFmiString("ASEMATEKSTI"),theOutput);
};
//051198---------------------------------------------------------------------------
NFmiString NFmiPressStationText::StationName()									
{
	NFmiString name;
	if(itsData->IsGrid())
		name = itsPressParam->GetCurrentStation().GetName(); //25.8.99
	else
		name = itsData->Location()->GetName();    //24.8.99

	if(itsNewNames)
	{
		NFmiVoidPtrIterator iter = NFmiVoidPtrIterator(itsNewNames); 
		iter.Reset();
		NFmiRenaming* renaming = (NFmiRenaming*)iter.Next();
		while (renaming)
		{
			if(renaming->originalName == name)
				return renaming->newName;

			renaming = (NFmiRenaming*)iter.Next();
		}
	}
	return name;
}
