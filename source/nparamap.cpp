// © Ilmatieteenlaitos/Salla.			File: nparamap.cpp
// Original 20.10.1997
// 
// Muutettu 19.11.1997	Salla Copy constr.
// Muutettu 6.4.1998	fIsScaled lisatty
//---------------------------------------------------------------------------

#include "nparamap.h"

NFmiParamMapping::NFmiParamMapping(const NFmiParamMapping& theParamMapping)
: NFmiSize(theParamMapping.GetSize())
{
	itsMappingIntervals = new NFmiMappingInterval[itsSize];
	for(int i=0; i< (int)itsSize; i++)
	{
		itsMappingIntervals[i].itsBottomValue = theParamMapping.itsMappingIntervals[i].itsBottomValue;
		itsMappingIntervals[i].itsTopValue = theParamMapping.itsMappingIntervals[i].itsTopValue;
		itsMappingIntervals[i].itsSymbol = theParamMapping.itsMappingIntervals[i].itsSymbol;
		itsMappingIntervals[i].fIsScaled = theParamMapping.itsMappingIntervals[i].fIsScaled;
	}

}
NFmiParamMapping::~NFmiParamMapping() 
{
	delete [] (NFmiMappingInterval*)itsMappingIntervals;
};


void NFmiParamMapping::AddMappingInterval(const NFmiMappingInterval & theInterval)
{
	//Note: ei tarkista alkioiden järjestystä tai onko samoja alkioita jo ennestään.
	//(lisää vain tyhmästi listan loppuun).
		NFmiMappingInterval *tempIntervals;
		tempIntervals = new NFmiMappingInterval[GetSize() + 1];

		int j;
		for(j=0; j<(int)itsSize; j++)
		{
			tempIntervals[j].itsBottomValue = itsMappingIntervals[j].itsBottomValue;
			tempIntervals[j].itsTopValue = itsMappingIntervals[j].itsTopValue;
			tempIntervals[j].itsSymbol = itsMappingIntervals[j].itsSymbol;
			tempIntervals[j].fIsScaled = itsMappingIntervals[j].fIsScaled;
		}
		
		tempIntervals[j].itsBottomValue = theInterval.itsBottomValue;
		tempIntervals[j].itsTopValue = theInterval.itsTopValue;
		tempIntervals[j].itsSymbol = theInterval.itsSymbol;
		tempIntervals[j].fIsScaled = theInterval.fIsScaled;

		if(itsMappingIntervals)
			delete [] itsMappingIntervals;

		itsMappingIntervals = new NFmiMappingInterval[GetSize()+1];
		itsSize = GetSize()+1;
		for(j=0; j< (int)GetSize(); j++)
		{
			itsMappingIntervals[j].itsBottomValue = tempIntervals[j].itsBottomValue;
			itsMappingIntervals[j].itsTopValue = tempIntervals[j].itsTopValue;
			itsMappingIntervals[j].itsSymbol = tempIntervals[j].itsSymbol;
			itsMappingIntervals[j].fIsScaled = tempIntervals[j].fIsScaled;
		}
		delete [] tempIntervals;

};

void NFmiParamMapping::AddMappingInterval(double theBottomValue, double theTopValue, const NFmiString & theSymbol)
{   //6.4 skaalatieto pitäisi lisätä
	NFmiMappingInterval temp;
	temp.itsBottomValue = theBottomValue;
	temp.itsTopValue = theTopValue;
	temp.itsSymbol = theSymbol;
	AddMappingInterval(temp);
};

NFmiString*	NFmiParamMapping::Map(const double theValue)
{  //6.4 tarvitaanko enää 
	int j;
	for(j=0; j<(int)itsSize; j++)
	{
		if(theValue >= itsMappingIntervals[j].itsBottomValue
			&& theValue <= itsMappingIntervals[j].itsTopValue)
				break;
	}
	if(j == (int)itsSize)
		return 0;
	else
		return &itsMappingIntervals[j].itsSymbol;
};

FmiBoolean NFmiParamMapping::IsInFirst(const double theValue)
{  //241198  Ei-kiertävää tuulisymbolia varten, keinotekoinen  
     return	theValue >= itsMappingIntervals[0].itsBottomValue
			&& theValue <= itsMappingIntervals[0].itsTopValue;			
};

NFmiString*	NFmiParamMapping::Map(const double theValue, FmiBoolean &outIsScaled)
{
	int j;
	for(j=0; j<(int)itsSize; j++)
	{
		if(theValue >= itsMappingIntervals[j].itsBottomValue
			&& theValue <= itsMappingIntervals[j].itsTopValue)
				break;
	}
	if(j == (int)itsSize)
		return 0;
	else
        outIsScaled = itsMappingIntervals[j].fIsScaled;
		return &itsMappingIntervals[j].itsSymbol;
};
