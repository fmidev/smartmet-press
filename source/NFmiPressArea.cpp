// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressArea
 */
// ======================================================================

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

  //HUOM lonLat on alavasen ja yläoikea mutta xy:t ylävasen ja alaoikea
  double tlX,tlY,brX,brY;

  double tlwX,tlwY,brwX,brwY;  //world
  double trueLat;
  trLat=brY=brwY = kFloatMissing;
  NFmiString helpString;
  unsigned long proj = kNFmiYKJArea;
  double orientation = 0;
  
  ReadNext();

  // itsCommentLevel alla pois koska sekosi jos #KarttaLopun
  // jälkeen välittömästi kommentti nyt sen sijaan tulkitsee Kartan
  // sisäiset #-sanat loppumerkiksi vaikka kommenteissa
  // pitäisi karsia kaikki kommentit erikseen

  // pitäisi olla kunnossa preprosessoinnin myötä joten itsCommentLevel
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
			else if (helpString == NFmiString ("polster"))
			  proj = kNFmiPolSterArea;
			else if (helpString.GetChars(1,6) == NFmiString ("stereo"))
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
		case dXYCorners:
		  {
			SetFour(tlX,tlY,brX,brY);
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
  
   // jos alueoperaatio mittoja ei tarvita mihinkään
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
		  else if (proj == kNFmiPolSterArea)
			itsArea = new NFmiPolSterArea(NFmiPoint(blLon,blLat),
										  NFmiPoint(trLon,trLat),
										  orientation,
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
	  *itsLogFile << "*** ERROR: " << "#Kartassa pitää LatLonin lisäksi antaa mitat jollei alueoperaatio"  << endl;
	  
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
		  lowChar==NFmiString("pystyssä") ||
		  lowChar==NFmiString("pituuspiiri"))
	return dOrientation;

  else if(lowChar==NFmiString("truelat") ||
		  lowChar==NFmiString("referenssilatitudi") ||
		  lowChar==NFmiString("reflat"))
	return dTrueLat;

  else if(lowChar==NFmiString("lonlatcorners") ||
		  lowChar==NFmiString("lonlatkulmat"))
	return dLonLatCorners;

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
