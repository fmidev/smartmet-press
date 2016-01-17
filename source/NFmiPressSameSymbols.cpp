// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressSameSymbols
 */
// ======================================================================

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressSameSymbols.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param scale Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressSameSymbols::SetScale(const NFmiRectScale &scale) { itsScale = scale; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressSameSymbols::ReadDescription(NFmiString &retString)
{
  NFmiString dataFileName;
  NFmiString str;
  NFmiValueString valueString;
  NFmiPoint *place;
  char object[255];
  long long1;
  unsigned short numOfTablePlaces, currentPlaceNum = 0;
  NFmiString string1;
  NFmiPoint point1;
  double tableX = 0, tableY1 = 0, tableY2 = 0, x, y;
  double xmin, xmax, ymin, ymax;

  *itsDescriptionFile >> object;
  str = object;
  int iobject = ConvertDefText(str);

  numOfTablePlaces = 0;
  itsNumberOfSteps = 1;
  itsStepSize = NFmiPoint(20., 0.);

  while (iobject != dEnd || itsCommentLevel)
  {
    if (itsLoopNum > itsMaxLoopNum)
    {
      if (itsLogFile)
        *itsLogFile << "*** ERROR: max file length exceeded in #FixedSymbolLocations" << endl;
      retString = str;
      return isFalse;
    }
    itsLoopNum++;
    if (iobject != dEndComment && itsCommentLevel) iobject = dComment;
    switch (iobject)
    {
      case dOther:
      {
        if (itsLogFile)
          *itsLogFile << "*** ERROR: Unknown word in #FixedSymbolLocations: "
                      << static_cast<char *>(object) << endl;
        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dComment:
      {
        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dEndComment:
      {
        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dPlaceSubViews:
      {
        if (!ReadEqualChar()) break;
        if (ReadLong(long1)) itsNumberOfSteps = static_cast<unsigned short>(long1);

        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dPlaceStepSize:
      {
        if (!ReadEqualChar()) break;
        if (Read2Double(x, y))
        {
          point1.Set(x, y);

          itsStepSize = NFmiPoint(point1.X() * itsScale.GetScaling().X(),
                                  point1.Y() * itsScale.GetScaling().Y());
        }
        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dPlaceTable:
      {
        if (!ReadEqualChar()) break;
        if (Read2Double(xmin, ymax))
        {
          if (ReadDouble(ymin))
          {
            if (ReadLong(long1))
            {
              tableX = xmin;
              tableY1 = ymax;
              tableY2 = ymin;
              numOfTablePlaces = static_cast<unsigned short>(long1);
            }
          }
        }
        currentPlaceNum = 0;

        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dPlaceDefArea:
      {
        if (!ReadEqualChar()) break;
        if (Read4Double(xmin, ymin, xmax, ymax))
        {
          itsCurrentScale.SetStartScales(NFmiRect(NFmiPoint(xmin, ymin), NFmiPoint(xmax, ymax)));
        }
        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dPlacePlotArea:
      {
        if (!ReadEqualChar()) break;
        if (Read4Double(xmin, ymin, xmax, ymax))
        {
          itsCurrentScale.SetEndScales(NFmiRect(NFmiPoint(xmin, ymin), NFmiPoint(xmax, ymax)));
        }
        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dPlace:
      {
        currentPlaceNum++;
        if (!ReadEqualChar()) break;
        if (Read2Double(x, y))
        {
          if (numOfTablePlaces > 1)
          {
            x = tableX;
            y = (tableY2 - tableY1) / (numOfTablePlaces - 1) * (currentPlaceNum - 1) + tableY1;
          }
          point1 = itsCurrentScale.Scale(NFmiPoint(x, y));
          place = new NFmiPoint(point1);
          itsPlaces.Add(place);
        }

        *itsDescriptionFile >> object;
        str = object;
        iobject = ConvertDefText(str);
        break;
      }
      case dPlaceSymbol:
      {
        itsPsSymbol.SetLogFile(itsLogFile);
        itsPsSymbol.SetHome(GetHome());
        itsPsSymbol.SetDescriptionFile(itsDescriptionFile);
        itsPsSymbol.SetWriteLast(itsPressProduct->GetElementsAfterSegments());

        itsPsSymbol.ReadDescription(str);

        iobject = ConvertDefText(str);
        break;
      }
    }
  }
  retString = str;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiPressSameSymbols::ConvertDefText(NFmiString &object)
{
  NFmiString lowChar(object);
  lowChar.LowerCase();

  if (lowChar == NFmiString("place") || lowChar == NFmiString("paikka"))
    return dPlace;

  else if (lowChar == NFmiString("#fixedsymbol") || lowChar == NFmiString("#constantsymbol") ||
           lowChar == NFmiString("#vakiosymboli") || lowChar == NFmiString("#kuva"))
    return dPlaceSymbol;

  else if (lowChar == NFmiString("placedefarea") || lowChar == NFmiString("paikkojenmittaalue"))
    return dPlaceDefArea;

  else if (lowChar == NFmiString("placeplotarea") || lowChar == NFmiString("paikkojensijoitusalue"))
    return dPlacePlotArea;

  else if (lowChar == NFmiString("subviews") || lowChar == NFmiString("siirtolukum‰‰r‰"))
    return dPlaceSubViews;

  else if (lowChar == NFmiString("stepsize") || lowChar == NFmiString("siirrot"))
    return dPlaceStepSize;

  else if (lowChar == NFmiString("placetable") || lowChar == NFmiString("paikkataulukko"))
    return dPlaceTable;

  else
    return NFmiDescription::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressSameSymbols::WritePS(ofstream &theOutFile)
{
  itsPsSymbol.Set(itsScale, theOutFile);
  NFmiVoidPtrIterator placeIter(itsPlaces);
  void *item;
  NFmiPoint place;
  NFmiPoint placeMovement;

  FmiCounter itsCurrentStep = 1;
  placeMovement.Set(0., 0.);
  FmiCounter placeAll = 0;
  while (itsCurrentStep <= itsNumberOfSteps)
  {
    {
      placeIter.Reset();
      item = placeIter.Next();
      while (item)
      {
        placeAll++;
        place = *static_cast<NFmiPoint *>(item);
        {
          if (!(itsPsSymbol.MakeAndWritePS(place)))
          {
          }
        }
        // pit‰iskˆ lis‰t‰  item->WriteGRestore();
        item = placeIter.Next();
      }
    }

    placeMovement += itsStepSize;
    itsCurrentStep++;
  }
  if (itsLogFile)
    *itsLogFile << "VakioPaikat: " << static_cast<char *>(itsPsSymbol.GetSymbol()) << ": "
                << placeAll << " paikkaa k‰sitelty" << endl;

  for (int i = 0; i < itsNumberOfSteps; i++)
  {
    itsCurrentScale.MoveXEndScale(-itsStepSize.X());
  }
  return isTrue;
}

// ======================================================================
