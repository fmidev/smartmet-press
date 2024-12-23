// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiRotatingParamRect
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k��nt�j�n varoitusta
#endif

#include "NFmiRotatingParamRect.h"
#include "NFmiPressParam.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiRotatingParamRect::~NFmiRotatingParamRect(void) {}
// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theSymbolRect The object to copy
 */
// ----------------------------------------------------------------------

NFmiRotatingParamRect::NFmiRotatingParamRect(const NFmiRotatingParamRect &theSymbolRect)
    : NFmi2SymbolParamRect(theSymbolRect)
{
  itsNotRotatingMinValue = theSymbolRect.itsNotRotatingMinValue;
  itsNotRotatingMaxValue = theSymbolRect.itsNotRotatingMaxValue;
  itsVerticalLong = theSymbolRect.itsVerticalLong;
}

// ----------------------------------------------------------------------
/*!
 * Make a copy of this
 *
 * \return The clone
 * \todo Should return an auto_ptr
 */
// ----------------------------------------------------------------------

NFmiParamRect *NFmiRotatingParamRect::Clone(void) const
{
  return new NFmiRotatingParamRect(*this);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiRotatingParamRect::ReadRemaining(void)
{
  //	whar_t wideh L'\0';
  double double1, double2;

  switch (itsIntObject)
  {
    case dVerticalLong:
    {
      SetOne(itsVerticalLong);
      break;
    }
    case dNotRotInterval:
    {
      if (!ReadEqualChar())
        break;

      if (Read2Double(double1, double2))
      {
        itsNotRotatingMinValue = double1;
        itsNotRotatingMaxValue = double2;
      }

      ReadNext();

      break;
    }
    default:
    {
      return NFmi2SymbolParamRect::ReadRemaining();
    }
  }
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiRotatingParamRect::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();  // kaikille pit�isi sallia vapaa isot/pienet kirj.
  // emossa nimell� ToinenParametri

  if (lowChar == NFmiString("rotatingparam") || lowChar == NFmiString("rotatingparameter") ||
      lowChar == NFmiString("k��nt�v�parametri"))
    return dSecondPar;

  else if (lowChar == NFmiString("notrotatingrange") || lowChar == NFmiString("notrotrange") ||
           lowChar == NFmiString("notrotinterval") ||
           lowChar == NFmiString("notrotatinginterval") ||
           lowChar == NFmiString("eik��ntyv�tarvot"))
    return dNotRotInterval;

  else if (lowChar == NFmiString("verticallongitude") || lowChar == NFmiString("pystylongitudi") ||
           lowChar == NFmiString("pystypituuspiiri"))
    return dVerticalLong;

  else
    return NFmi2SymbolParamRect::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiRotatingParamRect::DoPostReading(void)
{
  if (fNewScaling)
    itsRelRect += NFmiPoint(-itsSizeFactor.X() / 2, -itsSizeFactor.Y() / 2);

  if (!itsSecondDataIdent.IsDataParam())
    itsSecondDataIdent.SetParam(*GetDataIdent().GetParam());

  itsSecondDataIdent.SetProducer(*GetDataIdent().GetProducer());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 *�\param theDestinationFile Undocumented
 * \return Undocumented
 * \todo symbolFile should be const
 */
// ----------------------------------------------------------------------

bool NFmiRotatingParamRect::CopyShortSymbol2Dest(NFmiString *symbolFile,
                                                 ofstream &theDestinationFile,
                                                 float theRotating)  // turha t�ss� mutta pit��
// olla samoin kuin emossa
{
  NFmiString fileName = *itsSubDir;
  fileName += itsSymbolSetName;
  fileName += NFmiString("_");
  fileName += *symbolFile;
  fileName += NFmiString(".ps");
  ifstream inFile(fileName, ios::in | ios::binary);
  if (inFile.good() && !inFile.eof())
  {
    float direction = itsSecondParamValue;
    float adjustedDirection = AdjustToMap(direction);
    if (!Rotate())
      adjustedDirection = 270.;
    NFmiWritePSConcatRotating(itsDefToProductScale, adjustedDirection, theDestinationFile);
    NFmiCopyFile(inFile, theDestinationFile);
    NFmiWritePSEnd(theDestinationFile);
    return isTrue;
  }
  else
    return isFalse;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 * \param realPos Undocumented
 * \param os Undocumented
 * \todo symbolFile should be const
 * \todo realPos should be const reference
 */
// ----------------------------------------------------------------------

void NFmiRotatingParamRect::WriteMetaCode(NFmiString *symbolFile, NFmiPoint realPos, ofstream &os)
{
  NFmiString hStr(*symbolFile);
  hStr.FirstCharToUpper();
  os << endl
     << static_cast<char *>(hStr) << " -x " << realPos.X() << " -y " << realPos.Y() << " -angle "
     << static_cast<int>(itsSecondParamValue);
  os << endl;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDirection Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

float NFmiRotatingParamRect::AdjustToMap(float theDirection) const
{
  if (itsVerticalLong != kFloatMissing)
  {
    double lon = itsPressParam->GetCurrentStation().GetLongitude();
    if (lon != kFloatMissing)
      return theDirection - static_cast<float>(lon) + static_cast<float>(itsVerticalLong);
    else
    {
      //*itsLogFile << "*** ERROR: longitudi puuttuu tuulensuuntaa varten asemalta "
      *itsLogFile << "*** ERROR: longitude missing from station, needed for wind direction "
                  << static_cast<char *>(itsPressParam->GetCurrentStation().GetName()) << endl;
      return theDirection;
    }
  }

  return theDirection;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiRotatingParamRect::Rotate(void) const
{
  return (itsCurrentParamValue < itsNotRotatingMinValue ||
          itsCurrentParamValue > itsNotRotatingMaxValue);
}

// ======================================================================
