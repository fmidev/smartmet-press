// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiMultiParamMapping
 */
// ======================================================================

#include "NFmiMultiParamMapping.h"
#include "NFmiHyphenationString.h"
#include "NFmiStaticTime.h"
#include "NFmiValueString.h"
#include <fstream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal storage of mapping intervals
 */
// ----------------------------------------------------------------------

NFmiMultiParamMapping::~NFmiMultiParamMapping(void)
{
  delete[] static_cast<FmiMultiMapping *>(itsMappingIntervals);
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theParamMapping The object being copied
 * \todo Make loop index i unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

NFmiMultiParamMapping::NFmiMultiParamMapping(const NFmiMultiParamMapping &theParamMapping)
    : NFmiSize(theParamMapping.GetSize())
{
  itsMappingIntervals = new FmiMultiMapping[itsSize];
  for (int i = 0; i < static_cast<int>(itsSize); i++)
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

FmiMultiMapping NFmiMultiParamMapping::ReadOneMapping(ifstream *inFile)
{
  const short lineSize = 130;  // max rivipituus
  char inBuf[lineSize];
  NFmiString comma(",");

  FmiMultiMapping mapping;

  inFile->getline(inBuf, lineSize, '\n');

  NFmiHyphenationString row(inBuf);
  NFmiHyphenationString hyphOnePair;
  NFmiString onePair;
  NFmiValueString str;
  NFmiHyphenationString hyphSubString;

  for (int i = 0; i < FmiMaxNumOfMappingParams; i++)
  {
    mapping.mappingInterval[i].lowBorder = kFloatMissing;
    mapping.mappingInterval[i].highBorder = kFloatMissing;
  }

  short ind = 0;
  row.ReplaceChars(NFmiString("\t"), NFmiString(" "));
  while (row.NextSubString(comma, onePair))
  {
    hyphOnePair = onePair.CharPtr();
    hyphOnePair.NextSubString(str);

    if (str.IsNumeric())
    {
      mapping.mappingInterval[ind].lowBorder = static_cast<float>(str);

      hyphOnePair.NextSubString(str);
      if (str.IsNumeric())
      {
        mapping.mappingInterval[ind].highBorder = static_cast<float>(str);
        if (hyphOnePair.NextSubString(str))  // mahdollinen symboli
          break;                             // jotta mahdollinen kommentii pilkkuineen ei sotkisi
      }
    }
    ind++;
  }
  if (str == NFmiString("-"))  // loppuu: - symboli
    hyphOnePair.NextSubString(str);
  mapping.symbol = str;
  return mapping;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theInterval Undocumented
 * \todo Make loop index j unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

void NFmiMultiParamMapping::AddMappingInterval(const FmiMultiMapping &theInterval)
{
  FmiMultiMapping *tempIntervals;
  tempIntervals = new FmiMultiMapping[GetSize() + 1];

  int j;
  for (j = 0; j < static_cast<int>(itsSize); j++)
  {
    tempIntervals[j] = itsMappingIntervals[j];
  }

  tempIntervals[j] = theInterval;

  for (int i = 0; i < itsNumOfParams; i++)
  {
    if ((theInterval.mappingInterval[i].lowBorder >= FmiStartOfIncompleteValues &&
         theInterval.mappingInterval[i].lowBorder < kFloatMissing) ||
        (theInterval.mappingInterval[i].highBorder >= FmiStartOfIncompleteValues &&
         theInterval.mappingInterval[i].highBorder < kFloatMissing))
      fIncomplete = true;
  }

  if (itsMappingIntervals)
    delete[] itsMappingIntervals;

  itsMappingIntervals = new FmiMultiMapping[GetSize() + 1];
  itsSize = GetSize() + 1;
  for (j = 0; j < static_cast<int>(GetSize()); j++)
  {
    itsMappingIntervals[j] = tempIntervals[j];
  }
  delete[] tempIntervals;
}

// ----------------------------------------------------------------------
/*!
 * Match data value to text in mapping configureation
 * If value in data is within limits return the symbol name in the mapping
 *
 * Each mapping has defined limits for a number of parameters
 * Each parameters value in data is checked against given limits.
 * If the value is within limis, another parameter is checked.
 * If the value is not within limits, the method moves on check the next mapping.
 *
 * Should the limits be defind as missing values, they are not taken into account
 * and all data values are considered to being within limits.
 *
 * \param values Data parameter values
 * \param missingFound Undocumented
 * \return Undocumented
 * \todo Make loop indices i and j unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------
NFmiString *NFmiMultiParamMapping::Map(const vector<float> &values, bool &missingFound)
{
  missingFound = false;

  // Iterate over mapping definitions (Rows in the multimapping configuration)
  for (int j = 0; j < static_cast<int>(itsSize); j++)
  {
    // Count all matching mappings
    int numberOfMatchedValues = 0;
    int numberOfParameters = static_cast<int>(itsNumOfParams);

    // std::cout << std::endl << itsMappingIntervals[j].symbol;
    // Compare  over all defined parameter limits
    for (int i = 0; i < numberOfParameters; i++)
    {
      // To avoid complex conditional expressions, introduce temporary variables
      float lowBorder = itsMappingIntervals[j].mappingInterval[i].lowBorder;
      float highBorder = itsMappingIntervals[j].mappingInterval[i].highBorder;
      float valueToCheck = values[i];
      bool valueIsMissing = valueToCheck == kFloatMissing;
      bool lowBorderIsNotMissing = lowBorder != kFloatMissing;
      if (highBorder == kFloatMissing && lowBorderIsNotMissing)
      {
        // If only low limit is defined, it is used as a fixed limit
        highBorder = lowBorder;
      }
      bool highBorderIsNotMissing = highBorder != kFloatMissing;
      bool lowBorderIsIncomplete =
          (lowBorder >= FmiStartOfIncompleteValues && lowBorderIsNotMissing);
      bool highBorderIsIncomplete =
          (highBorder >= FmiStartOfIncompleteValues && highBorderIsNotMissing);

      // std::cout << lowBorder << " - " << valueToCheck << " - " << highBorder << std::endl;

      if (valueIsMissing)
      {
        // Set missing true, so it may be handled if no matching symbol mapping is found
        missingFound = true;
      }

      if (lowBorderIsIncomplete || highBorderIsIncomplete)
      {
        // The mappings high and low settings are something above 31690 but not 32700
        // Not sure why this is checked. Maybe something historical?
        break;
      }

      if (lowBorderIsNotMissing && valueIsMissing)
      {
        // Cannot compare defined limit with missing value
        break;
      }

      if (lowBorderIsNotMissing && (valueToCheck < lowBorder))
      {
        // Limits are defined and value is not within them
        break;
      }

      if (highBorderIsNotMissing && (valueToCheck > highBorder))
      {
        // Limits are defined and value is not within them
        // highBorder may have the same value as lowBorder
        break;
      }
      // Currently evaluated parameter's value is within the given limits
      // std::cout <<  "match" << std::endl;
      numberOfMatchedValues++;
    }

    if (numberOfMatchedValues == numberOfParameters)
    {
      // All of the mappings have been checked and they pass.
      // Set missing to false, because the data may have a missing value in purpose,
      // for an example precipitation type can be missing when there is no rain.
      missingFound = false;
      // std::cout <<  "symbol in use " << std::endl;
      // Return the current symbol filename or text from multimapping setting
      return &itsMappingIntervals[j].symbol;
    }
  }
  // Return without a matching symbol
  // std::cout <<  "NO SYMBOL FOUND " << std::endl;
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
  for (int j = 0; j < static_cast<int>(itsSize); j++)
  {
    int i;
    for (i = 0; i < static_cast<int>(itsNumOfParams); i++)
    {
      if (itsMappingIntervals[j].mappingInterval[i].lowBorder == theOldValue)
        itsMappingIntervals[j].mappingInterval[i].lowBorder = theNewValue;
      if (itsMappingIntervals[j].mappingInterval[i].highBorder == theOldValue)
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
  for (int j = 0; j < static_cast<int>(itsSize); j++)
  {
    int i;
    for (i = 0; i < static_cast<int>(itsNumOfParams); i++)
    {
      if (itsMappingIntervals[j].mappingInterval[i].lowBorder >= FmiStartOfIncompleteValues &&
          itsMappingIntervals[j].mappingInterval[i].lowBorder != kFloatMissing)
        return static_cast<short>(itsMappingIntervals[j].mappingInterval[i].lowBorder);
      if (itsMappingIntervals[j].mappingInterval[i].highBorder >= FmiStartOfIncompleteValues &&
          itsMappingIntervals[j].mappingInterval[i].highBorder != kFloatMissing)
        return static_cast<short>(itsMappingIntervals[j].mappingInterval[i].highBorder);
    }
  }
  return 0;
}
// ======================================================================
