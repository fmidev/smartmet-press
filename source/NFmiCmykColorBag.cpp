// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiCmykColorBag
 */
// ======================================================================

#include "NFmiCmykColorBag.h"

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal array of colors
 *
 * \todo The if is unnecessary according to the standard
 */
// ----------------------------------------------------------------------

NFmiCmykColorBag::~NFmiCmykColorBag()
{
  if (itsColors)
    delete[] static_cast<NFmiNamedCmykColor *>(itsColors);
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theBag The object being copied
 * \todo Make loop index i unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

NFmiCmykColorBag::NFmiCmykColorBag(const NFmiCmykColorBag &theBag) : NFmiSize(theBag.GetSize())
{
  itsColors = new NFmiNamedCmykColor[itsSize];
  for (int i = 0; i < static_cast<int>(itsSize); i++)
  {
    itsColors[i].color = theBag.itsColors[i].color;
    itsColors[i].name = theBag.itsColors[i].name;
  }
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 * \todo Make loop index j unsigned to avoid static_cast
 * \todo Do not use j outside the loop, use itsSize+1 instead
 * \todo The if(itsColors) is unnecessary before the delete
 * \todo Reimplement using a proper vector or a list
 */
// ----------------------------------------------------------------------

void NFmiCmykColorBag::AddColor(const NFmiNamedCmykColor &theColor)
{
  // Huom: ei tarkista alkioiden järjestystä tai onko samoja
  // alkioita jo ennestään. (lisää vain tyhmästi listan loppuun).

  NFmiNamedCmykColor *tempColors;
  tempColors = new NFmiNamedCmykColor[GetSize() + 1];

  int j;
  for (j = 0; j < static_cast<int>(itsSize); j++)
  {
    tempColors[j].color = itsColors[j].color;
    tempColors[j].name = itsColors[j].name;
  }

  tempColors[j].color = theColor.color;
  tempColors[j].name = theColor.name;

  if (itsColors)
    delete[] static_cast<NFmiNamedCmykColor *>(itsColors);

  itsColors = new NFmiNamedCmykColor[GetSize() + 1];
  itsSize = GetSize() + 1;
  for (j = 0; j < static_cast<int>(GetSize()); j++)
  {
    itsColors[j].color = tempColors[j].color;
    itsColors[j].name = tempColors[j].name;
  }
  delete[] static_cast<NFmiNamedCmykColor *>(tempColors);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theName Undocumented
 * \return Undocumented
 * \todo Should probably return a const reference instead of a pointer,
 *       since the method itself is const. (a const pointer is the
 *       second alternative)
 * \todo Make loop index j unsigned to avoid static_cast
 */
// ----------------------------------------------------------------------

NFmiCmykColor *NFmiCmykColorBag::GetColor(const NFmiString &theName) const
{
  int j;
  for (j = 0; j < static_cast<int>(itsSize); j++)
  {
    if (theName == itsColors[j].name)
      break;
  }
  if (j == static_cast<int>(itsSize))
    return 0;
  else
    return &itsColors[j].color;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theName Undocumented
 * \return Undocumented
 *
 * \todo There is no need to use j outisde the loop, use itsSize instead
 * \todo Make j unsigned to avoid static_cast
 * \todo Should use std::string instead
 */
// ----------------------------------------------------------------------

NFmiString NFmiCmykColorBag::GetColorString(const NFmiString &theName) const
{
  int j;
  for (j = 0; j < static_cast<int>(itsSize); j++)
  {
    if (theName == itsColors[j].name)
      break;
  }

  NFmiString format = NFmiString("%.2f");  // HUOM hardkoodattu
  NFmiString str;
  if (j == static_cast<int>(itsSize))
    str = NFmiString("0. 0. 0. 0.");
  else
  {
    str = NFmiValueString(static_cast<float>(itsColors[j].color.c), format);
    str += NFmiString(" ");
    str += NFmiValueString(static_cast<float>(itsColors[j].color.m), format);
    str += NFmiString(" ");
    str += NFmiValueString(static_cast<float>(itsColors[j].color.y), format);
    str += NFmiString(" ");
    str += NFmiValueString(static_cast<float>(itsColors[j].color.k), format);
    str += NFmiString(" ");  // Huom viimeiseksikin välilyönti
  }
  return str;
}

// ======================================================================
