// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressImage
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressImage.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressImage::~NFmiPressImage(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param thePressImage The object to copy
 */
// ----------------------------------------------------------------------

NFmiPressImage::NFmiPressImage(const NFmiPressImage & thePressImage)
  : NFmiPressScaling()
  , itsPath(thePressImage.itsPath)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param retString Undocumented
 *†\return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressImage::ReadDescription(NFmiString & retString)
{
  NFmiString  imageFile, imagePath, imageDir;
  NFmiValueString valueString;
  double r1,r2,r3;
  double xmin,xmax,ymin,ymax;
  xmin = ymin = 0;
  xmax = ymax = 1;
  
  NFmiPoint point1 = NFmiPoint(0.,0.);
  NFmiPoint point2 = NFmiPoint(600.,750.);
  itsImageScale.SetStartScales(NFmiRect(point1,point2));
  itsImageScale.SetEndScales(NFmiRect(point1,point2));
  
  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);
  
  while(itsIntObject != dEnd || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel)
		itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #OsaKuvassa" << endl;
		  retString = itsString;
		  return false;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #OsaKuvassa: "
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
		case dSymbolPlace:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read2Double(r1,r2))
			  {
				Place(NFmiPoint(r1,r2));
			  }
			ReadNext();
			break;
		  }
		case dImageFile:
		  {
			if (!ReadEqualChar())
			  break;
			
			imageFile = ReadString();
			
			imageFile += NFmiString(".eps");
			
			ReadNext();
			break;
		  }
		case dImageDir:
		  {
			if (!ReadEqualChar())
			  break;
			
            imageDir = ReadString();
			
			ReadNext();
			break;
		  }
		case dImagePath:
		  {
			if (!ReadEqualChar())
			  break;
			
            imagePath = ReadString();
			
			ReadNext();
			break;
		  }
		case dImageSizeFactor:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read2Double(r1, r2))
			  {
				itsImageScale.SetStartScales(NFmiRect(NFmiPoint(0.,0.),
													  NFmiPoint(100.,100.)));
				itsImageScale.SetEndScales(NFmiRect(NFmiPoint(0.,0.),
													NFmiPoint(100.*r1,100.*r2)));
			  }
			ReadNext();
			break;
		  }
		case dImagePlaceMove:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read2Double(r1, r2))
			  {
				itsImageScale.SetStartScales(NFmiRect(NFmiPoint(0.,0.),
													  NFmiPoint(100.,100.)));
				itsImageScale.SetEndScales(NFmiRect(NFmiPoint(r1,r2),
													NFmiPoint(100.+r1,100+r2)));
			  }
			ReadNext();
			break;
		  }
		case dImageDefSize:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: OsaKuvan mittaAlueen min == max"  << endl;
				else
				  itsImageScale.SetStartScales(NFmiRect(NFmiPoint(xmin,ymin),
														NFmiPoint(xmax,ymax)));
			  }
			ReadNext();
			break;
		  }
		case dImagePlottingView:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: OsaKuvan SijoitusAlueen min == max"  << endl;
				else
				  itsImageScale.SetEndScales(NFmiRect(NFmiPoint(xmin,ymin),
													  NFmiPoint(xmax,ymax)));
			  }
			ReadNext();
			break;
		  }
		case dImageClippingRectangle:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  *itsLogFile << "*** ERROR: OsaKuvan rajauksen min == max"  << endl;
				else
				  itsClippingRect.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
			  }
			ReadNext();
			break;
		  }
		case dRotate:
		  {
			SetThree(itsRotatingAngle, r2, r3);
			
			itsRotatingPoint = NFmiPoint(r2,r3);
			SetRotBeforeScale(false);
			
			break;
		  }
		case dRotateFirst:
		  {
			SetThree(itsRotatingAngle, r2, r3);
			
			itsRotatingPoint = NFmiPoint(r2,r3);
			SetRotBeforeScale(true);
			
			break;
		  }
		default:
		  {
			ReadRemaining();
			break;
		}
		}
	} //while
  
  if(!imagePath.IsValue() && !imageFile.IsValue())
	{
	  if(itsLogFile)
		*itsLogFile << "*** ERROR: OsaKuva-tiedosto ei m‰‰ritelty" << endl;
	  retString = itsString;
	  return false;
	}
  else
	itsPath = CreatePath(NFmiString("PuoliValmiit"),imagePath,imageDir,imageFile);
  retString = itsString;
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

int NFmiPressImage::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if(lowChar==NFmiString("file") ||
	 lowChar==NFmiString("tiedosto") ||
	 lowChar==NFmiString("nimi"))
	return dImageFile;

  else if(lowChar==NFmiString("directory") ||
		  lowChar==NFmiString("hakemisto"))
	return dImageDir;

  else if(lowChar==NFmiString("rotatefirst") ||
		  lowChar==NFmiString("kiertoensin"))
	return dRotateFirst;

  else if(lowChar==NFmiString("path") ||
		  lowChar==NFmiString("polku"))
	return dImagePath;

  else if(lowChar==NFmiString("placemove") ||
		  lowChar==NFmiString("paikansiirto") ||
	      lowChar==NFmiString("suhtpaikka") ||
		  lowChar==NFmiString("suhteellinenpaikka"))
	return dImagePlaceMove;

  else if(lowChar==NFmiString("sizefactor") ||
		  lowChar==NFmiString("kokokerroin"))
	return dImageSizeFactor;

  else if(lowChar==NFmiString("plottingview") ||
		  lowChar==NFmiString("sijoitusalue"))
	return dImagePlottingView;

  else if(lowChar==NFmiString("mapdefsize") ||
		  lowChar==NFmiString("mittaalue"))
	return dImageDefSize;

  else if(lowChar==NFmiString("clippingrectangle") ||
		  lowChar==NFmiString("rajaus"))
	return dImageClippingRectangle;

  else
	return NFmiPressScaling::ConvertDefText(object);

}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPressImage::WritePS(FmiPressOutputMode theOutput)
{
  ScalePlotting();
  
  if (itsInFile)
	delete itsInFile;
  itsInFile = new ifstream;
  itsInFile->open(itsPath, ios::in|ios::binary);
  if(!itsInFile->good() || itsInFile->eof())
	{
	  *itsLogFile << "WARNING: Missing EPS image: "
				  << static_cast<char *>(itsPath)
				  << endl;
	}
  else
	{
	  if (itsClippingRect.IsEmpty())
		WriteEPSConcat();
	  else
		WriteEPSConcatClipping(itsClippingRect);
	  
	  CopyFileWithoutShowpage();
	  
	  WriteEPSEnd();
	  
	  *itsLogFile << "EPS image included into eps output: "
				  << static_cast<char *>(itsPath)
				  << endl;
	}
  itsInFile->close();
  itsInFile->clear(); // seuraava open ei ollut good (kun managerista vaihtoi)
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPressImage::ScalePlotting(void)
{
  itsWriteScale = itsRectScale.Scale(itsImageScale);
}

// ----------------------------------------------------------------------
