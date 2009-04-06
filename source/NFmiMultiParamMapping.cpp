// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiMultiParamMapping
 */
// ======================================================================

#include "NFmiMultiParamMapping.h"
#include "NFmiHyphenationString.h"
#include "NFmiValueString.h"
#include "NFmiStaticTime.h"
#include <fstream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal storage of mapping intervals
 */
// ----------------------------------------------------------------------

NFmiMultiParamMapping::~NFmiMultiParamMapping(void)
{
  delete [] static_cast<FmiMultiMapping *>(itsMappingIntervals);
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theParamMapping The object being copied
 * \todo Make loop index i unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

NFmiMultiParamMapping::NFmiMultiParamMapping(const NFmiMultiParamMapping & theParamMapping)
  : NFmiSize(theParamMapping.GetSize())
{
  itsMappingIntervals = new FmiMultiMapping[itsSize];
  for(int i=0; i< static_cast<int>(itsSize); i++)
	{
	  itsMappingIntervals[i] = theParamMapping.itsMappingIntervals[i];
	}
  itsNumOfParams = theParamMapping.itsNumOfParams;
  fIncomplete = theParamMapping.fIncomplete;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param inFile Undocumented
 * \return Undocumented
 * \todo The inFile parameter should be a reference, not a pointer
 */
// ----------------------------------------------------------------------

FmiMultiMapping NFmiMultiParamMapping::ReadOneMapping(ifstream * inFile)
{
  const short lineSize = 130;	//max rivipituus
  char inBuf[lineSize];
  NFmiString comma(",");

  FmiMultiMapping mapping;

  inFile->getline(inBuf, lineSize, '\n'); 

  NFmiHyphenationString row(inBuf);
  NFmiHyphenationString hyphOnePair;
  NFmiString onePair;
  NFmiValueString str;
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
	  hyphOnePair.NextSubString(str);
	  
	  if(str.IsNumeric())
		{
		  mapping.mappingInterval[ind].lowBorder = static_cast<float>(str);
		  
		  hyphOnePair.NextSubString(str);
		  if(str.IsNumeric())
			{
			  mapping.mappingInterval[ind].highBorder = static_cast<float>(str);
			  if(hyphOnePair.NextSubString(str)) //mahdollinen symboli
				break;     //jotta mahdollinen kommentii pilkkuineen ei sotkisi
			}
		}
	  ind++;
	}
  if(str == NFmiString("-"))   //loppuu: - symboli
	hyphOnePair.NextSubString(str);
  mapping.symbol = str;
  return mapping;
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param theInterval Undocumented
 * \todo Make loop index j unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

void NFmiMultiParamMapping::AddMappingInterval(const FmiMultiMapping & theInterval)
{
  FmiMultiMapping * tempIntervals;
  tempIntervals = new FmiMultiMapping[GetSize() + 1];
  
  int j;
  for(j=0; j<static_cast<int>(itsSize); j++)
	{
	  tempIntervals[j]= itsMappingIntervals[j];
	}
  
  tempIntervals[j] = theInterval;
  
  for(int i=0; i<itsNumOfParams; i++)
  {		
	if(theInterval.mappingInterval[i].lowBorder >= FmiStartOfIncompleteValues &&
	   theInterval.mappingInterval[i].lowBorder < kFloatMissing
	   || theInterval.mappingInterval[i].highBorder >= FmiStartOfIncompleteValues &&
	   theInterval.mappingInterval[i].highBorder < kFloatMissing)
		fIncomplete = true;
  }
  
  if(itsMappingIntervals)
	delete [] itsMappingIntervals;
  
  itsMappingIntervals = new FmiMultiMapping[GetSize()+1];
  itsSize = GetSize()+1;
  for(j=0; j< static_cast<int>(GetSize()); j++)
	{
	  itsMappingIntervals[j] = tempIntervals[j];
	}
  delete [] tempIntervals;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param values Undocumented
 * \param missingFound Undocumented
 * \return Undocumented
 * \todo Make loop indices i and j unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

NFmiString *	NFmiMultiParamMapping::Map(float values[FmiMaxNumOfMappingParams], bool & missingFound)
{
  missingFound = false;
  for(int j=0; j<static_cast<int>(itsSize); j++)
	{
	  int i;
	  for(i=0; i<static_cast<int>(itsNumOfParams); i++)
	  {  
		  {
		   if(itsMappingIntervals[j].mappingInterval[i].lowBorder >= FmiStartOfIncompleteValues 
				&&itsMappingIntervals[j].mappingInterval[i].lowBorder != kFloatMissing 
			|| itsMappingIntervals[j].mappingInterval[i].highBorder >= FmiStartOfIncompleteValues
				&&itsMappingIntervals[j].mappingInterval[i].highBorder != kFloatMissing) 
		     break; 
		   if(!(itsMappingIntervals[j].mappingInterval[i].lowBorder == kFloatMissing ||
			   itsMappingIntervals[j].mappingInterval[i].highBorder == kFloatMissing &&
			   values[i] == itsMappingIntervals[j].mappingInterval[i].lowBorder ||
			   (values[i] >= itsMappingIntervals[j].mappingInterval[i].lowBorder &&
				values[i]  <= itsMappingIntervals[j].mappingInterval[i].highBorder &&
				values[i]  != kFloatMissing &&
				itsMappingIntervals[j].mappingInterval[i].highBorder != kFloatMissing)))
			break;
		  }
		}
	  if(i >= static_cast<int>(itsNumOfParams)) 
		return &itsMappingIntervals[j].symbol;

	  // moniparametrien puuttuva-testi t‰‰ll‰ eli
	  // puuttuva vain jos jokin signifikantti puuttuu
	  if(i<static_cast<int>(itsNumOfParams) && values[i] == kFloatMissing  ||
		 i==static_cast<int>(itsNumOfParams) && values[i-1] == kFloatMissing)
		{											
		  missingFound = true;
		  //return itsMissingString;
		  return 0;
		}
	}
  return 0;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \result Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiMultiParamMapping::Complete(float theOldValue, float theNewValue)
{
  for(int j=0; j<static_cast<int>(itsSize); j++)
  {
		int i;
		for(i=0; i<static_cast<int>(itsNumOfParams); i++)
		{  
			if(itsMappingIntervals[j].mappingInterval[i].lowBorder == theOldValue)
				itsMappingIntervals[j].mappingInterval[i].lowBorder = theNewValue;
			if(itsMappingIntervals[j].mappingInterval[i].highBorder == theOldValue)
				itsMappingIntervals[j].mappingInterval[i].highBorder = theNewValue;
	    }
	}

	return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \result Undocumented
 */
// ----------------------------------------------------------------------

short NFmiMultiParamMapping::CheckIfIncomplete(void)
{
  for(int j=0; j<static_cast<int>(itsSize); j++)
  {
		int i;
		for(i=0; i<static_cast<int>(itsNumOfParams); i++)
		{  
		   if(itsMappingIntervals[j].mappingInterval[i].lowBorder >= FmiStartOfIncompleteValues 
				&&itsMappingIntervals[j].mappingInterval[i].lowBorder != kFloatMissing)
				return static_cast<short>(itsMappingIntervals[j].mappingInterval[i].lowBorder);
		   if(itsMappingIntervals[j].mappingInterval[i].highBorder >= FmiStartOfIncompleteValues
				&&itsMappingIntervals[j].mappingInterval[i].highBorder != kFloatMissing) 
				return static_cast<short>(itsMappingIntervals[j].mappingInterval[i].highBorder);
	    }
	}
	return 0;
}
// ======================================================================

