//© Ilmatieteenlaitos/Lasse.
//  Original 16.12.2000 
// 
// Muutettu 210501 +NFmiStereographicArea 
// Muutettu 170801 +equiDistArea 
//---------------------------------------------------------------------------

#include "NFmiPressArea.h"
#include "NFmiEquidistArea.h"

#include <iostream>  //STL 27.8.01
using namespace std;

//---------------------------------------------------------------------------
NFmiPressArea::NFmiPressArea(const NFmiPressArea& theOtherArea)
{
	itsArea = theOtherArea.GetArea()->Clone();
	fIsXyRequest = theOtherArea.fIsXyRequest; //25.3.02
};

//---------------------------------------------------------------------------
NFmiPressArea::~NFmiPressArea() 
{
	if(itsArea)
		delete itsArea;
};
/*
//---------------------------------------------------------------------------
NFmiArea *NFmiPressArea::Clone(void) const
{
  return (NFmiPressArea *) new NFmiPressArea(*this);
}
*/
//----------------------------------------------------------------------------
FmiBoolean NFmiPressArea::ReadDescription(NFmiString& retString) 
{
	//toistaiseksi vain ykj-projektio
	
	double blLon,blLat,trLon,trLat;
	double tlX,tlY,brX,brY;    //HUOM lonLat on alavasen ja yläoikea mutta xy:t ylävasen ja alaoikea
	double tlwX,tlwY,brwX,brwY;  //world
	double trueLat;
    trLat=brY=brwY = kFloatMissing;
	NFmiString helpString;
	unsigned long proj = kNFmiYKJArea;
	double orientation = 0;

	ReadNext();
	//20.8.01: itsCommentLevel alla pois koska sekosi jos #KarttaLopun jälkeen välittömästi kommentti
	//  nyt sen sijaan tulkitsee Kartan sisäset #-sanat loppumerkiksi vaikka kommenteissa
	//  pitäisi karsia kaikki kommentit erikseen
	//2002: pitäisi olla kunnossa preprosessoinnin myötä joten itsCommentLevel  takaisin
	while(itsIntObject != 9999 || itsCommentLevel) 
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; 
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
					*itsLogFile << "*** ERROR: Tuntematon sana #Kartassa: " << (char*)itsObject << endl;  
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
			else if (helpString.GetChars(1,6) == NFmiString ("equidi")) //17.8.01
			   proj = kNFmiEquiDistArea;
			else
				*itsLogFile << "*** ERROR: Tuntematon projektio: " << (char*)helpString << endl;
	
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
		default:  //22.1.01
		{
			ReadRemaining();  
			break;
		}
		
	}
	} //while

	retString = itsString;

	if(brwY == kFloatMissing && !fIsXyRequest) //jos alueoperaatio mittoja ei tarvita mihinkään
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
				itsArea = new NFmiYKJArea(NFmiPoint(blLon,blLat),NFmiPoint(trLon,trLat) 
	                             ,NFmiPoint(tlX,tlY),NFmiPoint(brX,brY));
			else if (proj == kNFmiPolSterArea)
				itsArea = new NFmiPolSterArea(NFmiPoint(blLon,blLat),NFmiPoint(trLon,trLat) 
								 ,orientation
	                             ,NFmiPoint(tlX,tlY),NFmiPoint(brX,brY));
			else if (proj == kNFmiStereographicArea)    //21.5.01
				itsArea = new NFmiStereographicArea(NFmiPoint(blLon,blLat),NFmiPoint(trLon,trLat) 
								 ,orientation
	                             ,NFmiPoint(tlX,tlY),NFmiPoint(brX,brY));
			else if (proj == kNFmiEquiDistArea)    //17.8.01
				itsArea = new NFmiEquidistArea(NFmiPoint(blLon,blLat),NFmiPoint(trLon,trLat) 
								 ,orientation
	                             ,NFmiPoint(tlX,tlY),NFmiPoint(brX,brY)
								 ,trueLat);
	//		itsArea->Init(); //25.3.02
			return kTrue;
		}
		else if(brwY != kFloatMissing)
		{
			if(itsArea)
			{
				delete itsArea;
				itsArea = 0;
			}

			itsArea = new NFmiYKJArea(NFmiPoint(tlwX,tlwY),NFmiPoint(brwX,brwY)
								 ,kTrue
	                             ,NFmiPoint(tlX,tlY),NFmiPoint(brX,brY));
	//		itsArea->Init(); //25.3.02
			return kTrue;
		}
	}
	else 
	{
		*itsLogFile << "*** ERROR: " << "#Kartassa pitää LatLonin lisäksi antaa mitat jollei alueoperaatio"  << endl; 

	}
	return kFalse;
}
//----------------------------------------------------------------------------
int NFmiPressArea::ConvertDefText(NFmiString & object)  
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); 

	if(lowChar==NFmiString("projection")|| lowChar==NFmiString("projektio"))
		return dProjection;
	else if(lowChar==NFmiString("orientation")|| lowChar==NFmiString("pystyssä")
		                                      || lowChar==NFmiString("pituuspiiri"))
		return dOrientation;
	else if(lowChar==NFmiString("truelat")|| lowChar==NFmiString("referenssilatitudi")
		                                  || lowChar==NFmiString("reflat"))
		return dTrueLat;
	else if(lowChar==NFmiString("lonlatcorners")|| lowChar==NFmiString("lonlatkulmat"))
		return dLonLatCorners;
	else if(lowChar==NFmiString("pointcorners") || lowChar==NFmiString("pistekulmat")
		                                         || lowChar==NFmiString("xykulmat"))
		return dXYCorners;
	else if(lowChar==NFmiString("worldcorners") || lowChar==NFmiString("maailmakulmat"))
		return dWorldXYCorners;
	else
		return NFmiPressDescription::ConvertDefText(object);
}
