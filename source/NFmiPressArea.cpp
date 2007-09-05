// ======================================================================
/*!
 * \file
 *�\brief Implementation of class NFmiPressArea
 */
// ======================================================================

#include "NFmiPressProduct.h"
#include "NFmiPressArea.h"
#include "NFmiEquidistArea.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal itsArea variable
 *
 * \todo The if is unnecessary
 */
// ----------------------------------------------------------------------

NFmiPressArea::~NFmiPressArea(void)
{
  if(itsArea)
	delete itsArea;
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theOtherArea The object to copy
 */
// ----------------------------------------------------------------------

NFmiPressArea::NFmiPressArea(const NFmiPressArea & theOtherArea)
  : NFmiPressDescription()
{
  itsArea = theOtherArea.GetArea()->Clone();
  fIsXyRequest = theOtherArea.fIsXyRequest;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressArea::ReadDescription(NFmiString & retString)
{
	//toistaiseksi vain ykj-projektio
  
  double blLon,blLat,trLon,trLat;

  //HUOM lonLat on alavasen ja yl�oikea mutta xy:t yl�vasen ja alaoikea
  double tlX,tlY,brX,brY;

  double tlwX,tlwY,brwX,brwY;  //world
  double trueLat;
  trLat=brY=brwY = kFloatMissing;
  NFmiString helpString, helpString2;
  unsigned long proj = kNFmiYKJArea;
  double orientation = 0;
  
  ReadNext();

  // itsCommentLevel alla pois koska sekosi jos #KarttaLopun
  // j�lkeen v�litt�m�sti kommentti nyt sen sijaan tulkitsee Kartan
  // sis�iset #-sanat loppumerkiksi vaikka kommenteissa
  // pit�isi karsia kaikki kommentit erikseen

  // pit�isi olla kunnossa preprosessoinnin my�t� joten itsCommentLevel
  // takaisin

  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel)
		itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Kartassa" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #Kartassa: "
						  << static_cast<char *>(itsObject)
						  << endl;
			ReadNext();
			break;
		  }
		case dComment:
		  {
			ReadNext();
			break;
		  }
		case dEndComment:
		  {
			ReadNext();
			break;
		  }
		case dProjection:
		  {
			if (!ReadEqualChar())
			  break;
			*itsDescriptionFile >> itsObject;
			helpString = itsObject;
			helpString.LowerCase();
			if (helpString == NFmiString ("ykj"))
			  proj = kNFmiYKJArea;
			else if (helpString.GetChars(1,6) == NFmiString ("stereo") ||
				     helpString.GetChars(1,7) == NFmiString ("polster"))
			  proj = kNFmiStereographicArea;
			else if (helpString.GetChars(1,6) == NFmiString ("equidi"))
			  proj = kNFmiEquiDistArea;
			else
			  *itsLogFile << "*** ERROR: Tuntematon projektio: "
						  << static_cast<char *>(helpString)
						  << endl;
			ReadNext();
 			break;
		  }
		case dOrientation:
		  {
			SetOne(orientation);
			break;
		  }
		case dTrueLat:
		  {
			SetOne(trueLat);
			break;
		  }
		case dLonLatCorners:
		  {
			SetFour(blLon,blLat,trLon,trLat);
			break;
		  }
		case dLonLatCornerNames:
		  {
			if (!ReadEqualChar())
			  break;
			helpString = ReadString();
			helpString2 = ReadString();

			NFmiPoint lonLat1, lonLat2;
			if(itsPressProduct->FindLonLatFromList(helpString, lonLat1)
			&& itsPressProduct->FindLonLatFromList(helpString2, lonLat2))
			{
				blLon = lonLat1.X();
				blLat = lonLat1.Y();
				trLon = lonLat2.X();
				trLat = lonLat2.Y();
			}
			else
			  *itsLogFile << "*** ERROR: Kartta ei pysty k�ytt�m��n asemaparia: "
						  << static_cast<char *>(helpString)
						  << " "
						  << static_cast<char *>(helpString2)
						  << endl;
			ReadNext();
			break;
		  }
		case dXYCorners:
		  {
			SetFour(tlX,tlY,brX,brY);
			MoveSegmentPlaceConditionally(tlX,tlY);
			MoveSegmentPlaceConditionally(brX,brY);
			break;
		  }
		case dWorldXYCorners:
		  {
			SetFour(tlwX,tlwY,brwX,brwY);
			break;
		  }
		default:
		  {
			ReadRemaining();
			break;
		  }
		  
		}
	} //while
  
  retString = itsString;
  
   // jos alueoperaatio mittoja ei tarvita mihink��n
  if(brwY == kFloatMissing && !fIsXyRequest)
	{
	  brY = 0.;
	  brX = 0.;
	  tlY = 10.;
	  tlX = 10.;
	}
  
  if(brY != kFloatMissing)
	{
	  if(trLat != kFloatMissing)
		{
		  if(itsArea)
			{
			  delete itsArea;
			  itsArea = 0;
			}
		  
		  if(proj == kNFmiYKJArea)
			itsArea = new NFmiYKJArea(NFmiPoint(blLon,blLat),
									  NFmiPoint(trLon,trLat),
									  NFmiPoint(tlX,tlY),
									  NFmiPoint(brX,brY));
		  else if (proj == kNFmiStereographicArea)
				itsArea = new NFmiStereographicArea(NFmiPoint(blLon,blLat),
													NFmiPoint(trLon,trLat),
													orientation,
													NFmiPoint(tlX,tlY),
													NFmiPoint(brX,brY));
		  else if (proj == kNFmiEquiDistArea)
			itsArea = new NFmiEquidistArea(NFmiPoint(blLon,blLat),
										   NFmiPoint(trLon,trLat),
										   orientation,
										   NFmiPoint(tlX,tlY),
										   NFmiPoint(brX,brY),
										   trueLat);
		  return true;
		}
	  else if(brwY != kFloatMissing)
		{
		  if(itsArea)
			{
			  delete itsArea;
			  itsArea = 0;
			}
		  
		  itsArea = new NFmiYKJArea(NFmiPoint(tlwX,tlwY),
									NFmiPoint(brwX,brwY),
									true,
									NFmiPoint(tlX,tlY),
									NFmiPoint(brX,brY));
		  return true;
		}
	}
  else
	{
	  *itsLogFile << "*** ERROR: " << "#Kartassa pit�� LatLonin lis�ksi antaa mitat jollei alueoperaatio"  << endl;
	  
	}
  return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiPressArea::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();
  
  if(lowChar==NFmiString("projection") ||
	 lowChar==NFmiString("projektio"))
	return dProjection;

  else if(lowChar==NFmiString("orientation") ||
		  lowChar==NFmiString("pystyss�") ||
		  lowChar==NFmiString("pituuspiiri"))
	return dOrientation;

  else if(lowChar==NFmiString("truelat") ||
		  lowChar==NFmiString("referenssilatitudi") ||
		  lowChar==NFmiString("reflat"))
	return dTrueLat;

  else if(lowChar==NFmiString("lonlatcorners") ||
		  lowChar==NFmiString("lonlatkulmat"))
	return dLonLatCorners;

  else if(lowChar==NFmiString("lonlatcornersnames") ||
		  lowChar==NFmiString("lonlatnimet"))
	return dLonLatCornerNames;

  else if(lowChar==NFmiString("pointcorners") ||
		  lowChar==NFmiString("pistekulmat") ||
		  lowChar==NFmiString("xykulmat"))
	return dXYCorners;

  else if(lowChar==NFmiString("worldcorners") ||
		  lowChar==NFmiString("maailmakulmat"))
	return dWorldXYCorners;

  else
	return NFmiPressDescription::ConvertDefText(object);
}

// ======================================================================
