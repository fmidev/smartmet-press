// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiParamMapping
 */
// ======================================================================

#include "NFmiParamMapping.h"

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal storage of mapping intervals
 */
// ----------------------------------------------------------------------

NFmiParamMapping::~NFmiParamMapping(void)
{
  delete [] static_cast<NFmiMappingInterval *>(itsMappingIntervals);
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theParamMapping The object being copied
 * \todo Make loop index i unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

NFmiParamMapping::NFmiParamMapping(const NFmiParamMapping & theParamMapping)
  : NFmiSize(theParamMapping.GetSize())
{
  itsMappingIntervals = new NFmiMappingInterval[itsSize];
  for(int i=0; i< static_cast<int>(itsSize); i++)
	{
	  itsMappingIntervals[i].itsBottomValue = theParamMapping.itsMappingIntervals[i].itsBottomValue;
	  itsMappingIntervals[i].itsTopValue = theParamMapping.itsMappingIntervals[i].itsTopValue;
	  itsMappingIntervals[i].itsSymbol = theParamMapping.itsMappingIntervals[i].itsSymbol;
	  itsMappingIntervals[i].fIsScaled = theParamMapping.itsMappingIntervals[i].fIsScaled;
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theInterval Undocumented
 * \todo Make loop index j unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

void NFmiParamMapping::AddMappingInterval(const NFmiMappingInterval & theInterval)
{
  //Note: ei tarkista alkioiden j‰rjestyst‰ tai onko samoja alkioita jo ennest‰‰n.
  //(lis‰‰ vain tyhm‰sti listan loppuun).

  NFmiMappingInterval * tempIntervals;
  tempIntervals = new NFmiMappingInterval[GetSize() + 1];
  
  int j;
  for(j=0; j<static_cast<int>(itsSize); j++)
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
  for(j=0; j< static_cast<int>(GetSize()); j++)
	{
	  itsMappingIntervals[j].itsBottomValue = tempIntervals[j].itsBottomValue;
	  itsMappingIntervals[j].itsTopValue = tempIntervals[j].itsTopValue;
	  itsMappingIntervals[j].itsSymbol = tempIntervals[j].itsSymbol;
	  itsMappingIntervals[j].fIsScaled = tempIntervals[j].fIsScaled;
	}
  delete [] tempIntervals;
  
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theBottomValue Undocumented
 * \param theTopValue Undocumented
 * \param theSymbol Undocumented
 */
// ----------------------------------------------------------------------

void NFmiParamMapping::AddMappingInterval(double theBottomValue,
										  double theTopValue,
										  const NFmiString & theSymbol)
{
  // skaalatieto pit‰isi lis‰t‰
  NFmiMappingInterval temp;
  temp.itsBottomValue = theBottomValue;
  temp.itsTopValue = theTopValue;
  temp.itsSymbol = theSymbol;
  AddMappingInterval(temp);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theValue Undocumented
 * \return Undocumented
 * \todo Make loop index j unsigned to avoid static_cast
 * \todo Should probably return a const pointer or a reference instead
 */
// ----------------------------------------------------------------------

NFmiString * NFmiParamMapping::Map(const double theValue)
{
  // tarvitaanko en‰‰ 
  int j;
  for(j=0; j<static_cast<int>(itsSize); j++)
	{
	  if(theValue >= itsMappingIntervals[j].itsBottomValue &&
		 theValue <= itsMappingIntervals[j].itsTopValue)
		break;
	}
  if(j == static_cast<int>(itsSize))
	return 0;
  else
	return &itsMappingIntervals[j].itsSymbol;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theValue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiParamMapping::IsInFirst(const double theValue)
{
  // Ei-kiert‰v‰‰ tuulisymbolia varten, keinotekoinen  
  return (theValue >= itsMappingIntervals[0].itsBottomValue &&
		  theValue <= itsMappingIntervals[0].itsTopValue);			
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theValue Undocumented
 * \param outIsScaled Undocumented
 * \return Undocumented
 * \todo make loop index j unsigned to avoid static_cast
 * \todo Should probably return a const pointer or a reference instead
 */
// ----------------------------------------------------------------------

NFmiString * NFmiParamMapping::Map(const double theValue, bool & outIsScaled)
{
  int j;
  for(j=0; j<static_cast<int>(itsSize); j++)
	{
	  if(theValue >= itsMappingIntervals[j].itsBottomValue &&
		 theValue <= itsMappingIntervals[j].itsTopValue)
		break;
	}
  if(j == static_cast<int>(itsSize))
	return 0;
  else
	outIsScaled = itsMappingIntervals[j].fIsScaled;
  return &itsMappingIntervals[j].itsSymbol;
}

// ----------------------------------------------------------------------
