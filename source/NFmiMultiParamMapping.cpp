// © Ilmatieteenlaitos/Lasse.			
// Original 17.9.99
// 
// Muutettu 290501/Lasse  rivi poistettu mappauksesta
//---------------------------------------------------------------------------

#include "NFmiMultiParamMapping.h"
#include "nhyphstr.h"
#include "NFmiValueString.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiMultiParamMapping::NFmiMultiParamMapping(const NFmiMultiParamMapping& theParamMapping)
: NFmiSize(theParamMapping.GetSize())
{
	itsMappingIntervals = new FmiMultiMapping[itsSize];
	for(int i=0; i< (int)itsSize; i++)
	{
		itsMappingIntervals[i] = theParamMapping.itsMappingIntervals[i];
	}
	itsNumOfParams = theParamMapping.itsNumOfParams;
}

//---------------------------------------------------------------------------
NFmiMultiParamMapping::~NFmiMultiParamMapping() 
{
	delete [] (FmiMultiMapping*)itsMappingIntervals;
};
 //9.9---------------------------------------------------------------------------
FmiMultiMapping NFmiMultiParamMapping::ReadOneMapping (ifstream* inFile)
{
	const short lineSize = 130;	//max rivipituus
//    unsigned char inBuf[lineSize];
    char inBuf[lineSize]; //27.8.01 korvaa ed.
	NFmiString comma(",");

	FmiMultiMapping mapping;

//	cin.getline(inBuf, lineSize, '\n'); //31.8.01 PITÄÄ FIKSATA
	inFile->getline(inBuf, lineSize, '\n'); 

	NFmiHyphenationString row(inBuf);
	NFmiHyphenationString hyphOnePair;
	NFmiString onePair;
	NFmiValueString string;
	NFmiHyphenationString hyphSubString;

	for(int i=0; i<FmiMaxNumOfMappingParams; i++)
	{
		mapping.mappingInterval[i].lowBorder = kFloatMissing;
		mapping.mappingInterval[i].highBorder = kFloatMissing;
	}

	short ind = 0;
	while (row.NextSubString(comma, onePair))
	{
		hyphOnePair = onePair.CharPtr();
		hyphOnePair.NextSubString(string);

		if(string.IsNumeric())
		{
			mapping.mappingInterval[ind].lowBorder = (float)string;

			hyphOnePair.NextSubString(string);
			if(string.IsNumeric())
			{
				mapping.mappingInterval[ind].highBorder = (float)string;
				if(hyphOnePair.NextSubString(string)) //mahdollinen symboli
					break;     //jotta mahdollinen kommentii pilkkuineen ei sotkisi
			}
//			else    string mahd. symboli
		}
		else
		{

//			subString.NextSubString(space, string); //string mahd. symboli 
		}
		ind++;
	}
	if(string == NFmiString("-"))   //loppuu: - symboli
		hyphOnePair.NextSubString(string);
	mapping.symbol = string;
	return mapping;
}
//---------------------------------------------------------------------------
void NFmiMultiParamMapping::AddMappingInterval(const FmiMultiMapping& theInterval)
{
		FmiMultiMapping *tempIntervals;
		tempIntervals = new FmiMultiMapping[GetSize() + 1];

		int j;
		for(j=0; j<(int)itsSize; j++)
		{
			tempIntervals[j]= itsMappingIntervals[j];
		}
		
		tempIntervals[j] = theInterval;

		if(itsMappingIntervals)
			delete [] itsMappingIntervals;

		itsMappingIntervals = new FmiMultiMapping[GetSize()+1];
		itsSize = GetSize()+1;
		for(j=0; j< (int)GetSize(); j++)
		{
			itsMappingIntervals[j] = tempIntervals[j];
		}
		delete [] tempIntervals;
};

//---------------------------------------------------------------------------
NFmiString*	NFmiMultiParamMapping::Map(float* values, FmiBoolean& missingFound)
{
	missingFound = kFalse;
	for(int j=0; j<(int)itsSize; j++)
	{
		int i;
		for(i=0; i<(int)itsNumOfParams; i++)
		{
			if(!(itsMappingIntervals[j].mappingInterval[i].lowBorder == kFloatMissing
				|| itsMappingIntervals[j].mappingInterval[i].highBorder == kFloatMissing
					&& values[i] == itsMappingIntervals[j].mappingInterval[i].lowBorder
				|| (values[i] >= itsMappingIntervals[j].mappingInterval[i].lowBorder
				    && values[i]  <= itsMappingIntervals[j].mappingInterval[i].highBorder
					&& values[i]  != kFloatMissing  //29.5.01 pois, vaikuttaako johonkin vanhaan;vielä mukaan
					&& itsMappingIntervals[j].mappingInterval[i].highBorder != kFloatMissing)))
				break;
		}
		if(i >= (int)itsNumOfParams) 
					return &itsMappingIntervals[j].symbol;
		if(i<(int)itsNumOfParams && values[i] == kFloatMissing    //moniparametrien puuttuva-testi täällä eli
		||i==(int)itsNumOfParams && values[i-1] == kFloatMissing) //puuttuva vain jos jokin signifikantti puuttuu
		{											
			missingFound = kTrue;
			return 0;
		}
	}
	return 0;
};
