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
#include "NFmiPressProduct.h"
#include "NFmiSettings.h"
#include <cstdlib>
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

NFmiPressImage::NFmiPressImage(void)
  : NFmiPressScaling()
  ,itsPressProduct(0)
  ,itsShear(0.)
{
  NFmiPoint point1 = NFmiPoint(0.,0.);
  NFmiPoint point2 = NFmiPoint(600.,750.);
  itsImageScale.SetStartScales(NFmiRect(point1,point2));
  itsImageScale.SetEndScales(NFmiRect(point1,point2));
}
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
  , itsImageScale(thePressImage.itsImageScale)
  , itsClippingRect(thePressImage.itsClippingRect)
  , itsClippingPoints(thePressImage.itsClippingPoints)
  , itsPressProduct(thePressImage.itsPressProduct)
  , itsTempImageFile(thePressImage.itsTempImageFile)
  , itsTempImagePath(thePressImage.itsTempImagePath)
  , itsTempImageDir(thePressImage.itsTempImageDir)
  , itsShear(thePressImage.itsShear)
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
  NFmiString tempString;
  NFmiValueString valueString;
  double r1,r2,r3;
  double x1, x2, x3, y1, y2, y3;
  double xmin,xmax,ymin,ymax;
  xmin = ymin = 0;
  xmax = ymax = 1;
 /* 
  NFmiPoint point1 = NFmiPoint(0.,0.);
  NFmiPoint point2 = NFmiPoint(600.,750.);
  itsImageScale.SetStartScales(NFmiRect(point1,point2));
  itsImageScale.SetEndScales(NFmiRect(point1,point2));
  */
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
		case dNewImage:
		  {
			NFmiPressImage* image = new NFmiPressImage;//owner pressProduct
			*image = *this;
			if(image->ReadDescription(itsString))
			{
			  if(itsPressProduct)
			     itsPressProduct->itsObjects.Add(image);
			  else
				  *itsLogFile << "*** ERROR: OsaKuvalla ei tuotepointteria, OHJELMOINTIVIRHE" << endl;
			}
			else
			  delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dNewImageName:
		  {
			if (!ReadEqualChar())
			  break;
			
			tempString = ReadString();			
			tempString += NFmiString(".eps");
			
			NFmiPressImage* image = new NFmiPressImage;//owner pressProduct
			*image = *this;
			image->SetTempImageFile(tempString);
			if(image->ReadDescription(itsString))
			{
			  if(itsPressProduct)
			     itsPressProduct->itsObjects.Add(image);
			  else
				  *itsLogFile << "*** ERROR: OsaKuvalla ei tuotepointteria, OHJELMOINTIVIRHE" << endl;
			}
			else
			  delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
	
		case dNewImageRel:
		  {
			if (!ReadEqualChar())
			  break;

			tempString = ReadString();			
			tempString += NFmiString(".eps");

			if(Read2Double (r1, r2))
			{
				NFmiRectScale tempScale(itsImageScale);
				tempScale.MoveEndScales(NFmiPoint(r1,r2));
				NFmiPressImage* image = new NFmiPressImage;//owner pressProduct
				*image = *this;
				image->SetTempImageFile(tempString);
				image->SetImageScale(tempScale);

				if (!itsClippingRect.IsEmpty())
				{
					NFmiRect tempRect(itsClippingRect);
					tempRect += NFmiPoint(r1,r2);
					image->SetClippingRect(tempRect);
				}
				if (itsClippingPoints.size() > 0)
				{
					std::vector<NFmiPoint> tempVector(itsClippingPoints);

					std::vector<NFmiPoint>::iterator pos;
					for(pos= tempVector.begin(); pos != tempVector.end(); ++pos)
					{
						*pos += NFmiPoint(r1,r2);
					}
					image->SetClippingPoints(tempVector);
				}
				if(image->ReadDescription(itsString))
				{
				  if(itsPressProduct)
					 itsPressProduct->itsObjects.Add(image);
				  else
					  *itsLogFile << "*** ERROR: OsaKuvalla ei tuotepointteria, OHJELMOINTIVIRHE" << endl;
				}
				else
				  delete image;
			}

			itsIntObject = ConvertDefText(itsString);
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
		case dSummerWinterImageFile:
		  {
			if (!ReadEqualChar())
			  break;

			NFmiString summerFile(ReadString());
			NFmiString winterFile(ReadString());
			NFmiString countryPart(ReadString());

			if(itsPressProduct->IsSummerWeather(countryPart))
				itsTempImageFile = summerFile;
			else
				itsTempImageFile = winterFile;

			itsTempImageFile += NFmiString(".eps");
			
			ReadNext();
			break;
		  }
		case dImageFile:
		  {
			if (!ReadEqualChar())
			  break;
			
			itsTempImageFile = ReadString();
			
			itsTempImageFile += NFmiString(".eps");
			
			ReadNext();
			break;
		  }
		case dImageDir:
		  {
			if (!ReadEqualChar())
			  break;
			
            itsTempImageDir = ReadString();
			
			ReadNext();
			break;
		  }
		case dImagePath:
		  {
			if (!ReadEqualChar())
			  break;
			
            itsTempImagePath = ReadString();
			
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
		case dImageClippingPath:
		  {
			NFmiValueString valueString;
			if (!ReadEqualChar())
			  break;
			if(ReadFour(x1, y1, x2, y2) && ReadTwo(x3, y3)) //minimi 3 pistett‰
			  {
			    NFmiPoint point;
				point.X(x1);
				point.Y(y1);
                itsClippingPoints.push_back(point);			  
				point.X(x2);
				point.Y(y2);
                itsClippingPoints.push_back(point);			  
				point.X(x3);
				point.Y(y3);
                itsClippingPoints.push_back(point);

			    valueString = ReadString();
				while (valueString.IsNumeric())
				{
					x1 = static_cast<float>(valueString);
			        valueString = ReadString();
					if(valueString.IsNumeric())
					{
						point.X(x1);
						point.Y(static_cast<float>(valueString));
						itsClippingPoints.push_back(point);
			            valueString = ReadString();
					}
					else
						*itsLogFile << "*** ERROR: x:lle puuttuu y-pari kuvan leikkauksessa"  << endl;
				}
				itsString = valueString;
				itsIntObject = ConvertDefText(valueString);
			  }

			else
			{
				 *itsLogFile << "*** ERROR: liian v‰h‰n pisteit‰ kuvan leikkauksessa"  << endl;
			     ReadNext();
			}
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
		case dImageShear:
		  {
			SetOne(itsShear);
						
			break;
		  }
		default:
		  {
			ReadRemaining();
			break;
		}
		}
	} //while
  
  if(!itsTempImagePath.IsValue() && !itsTempImageFile.IsValue())
	{
	  if(itsLogFile)
		*itsLogFile << "*** ERROR: OsaKuva-tiedosto ei m‰‰ritelty" << endl;
	  retString = itsString;
	  return false;
	}
  else
	{
#ifndef UNIX
	  char * env;
	  env = getenv("lehtiPuoliValmiitDir");
	  if(env == 0)
		itsPath = CreatePath(NFmiString("PuoliValmiit"),
							 itsTempImagePath,
							 itsTempImageDir,
							 itsTempImageFile);
	  else
		itsPath = CreatePath(NFmiString(env),
							 itsTempImagePath,
							 itsTempImageDir,
							 itsTempImageFile);
#else
	  string dir = NFmiSettings::Optional<string>("press::inputpath","PuoliValmiit");
	  itsPath = CreatePath(NFmiString(dir),
						   itsTempImagePath,
						   itsTempImageDir,
						   itsTempImageFile);
#endif
	}
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
	 lowChar==NFmiString("nimi") )
	return dImageFile;

  else if(lowChar==NFmiString("summerwinterfile") ||
	 lowChar==NFmiString("kes‰talvitiedosto") ||
	 lowChar==NFmiString("kes‰talvinimi") )
	return dSummerWinterImageFile;

  else if(lowChar==NFmiString("newimage") ||
		  lowChar==NFmiString("uusikuva"))
	return dNewImage;

  else if(lowChar==NFmiString("newname") ||
		  lowChar==NFmiString("uusitiedosto") ||
		  lowChar==NFmiString("uusinimi"))
	return dNewImageName;

  else if(lowChar==NFmiString("newrelatively") ||
		  lowChar==NFmiString("uusisuhteellisena") ||
	      lowChar==NFmiString("uusisuht")
		  )
	return dNewImageRel;

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

  else if(lowChar==NFmiString("paikka"))
  {
	*itsLogFile << 
	"*** ERROR: Paikka ei sallittu #OsaKuvassa, pit‰‰kˆ olla suhtPaikka"
	  << endl;
	return dImagePlaceMove;
  }

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
   
  else if(lowChar==NFmiString("clippingpath") ||
		  lowChar==NFmiString("rajauspisteet"))
	return dImageClippingPath;

  else if(lowChar==NFmiString("shear") ||
		  lowChar==NFmiString("kallistus"))
	return dImageShear;

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
  extern bool precedingElementMissing;

	//testeiss‰ k‰tev‰ merkata None:ksi
  if(itsPath.Header() == NFmiString("None"))
  {
	  *itsLogFile << "WARNING: image marked None"
				  << endl;
	  return true;
  }

  ScalePlotting();
  
  if (itsInFile)
	delete itsInFile;
  itsInFile = new ifstream;
  itsInFile->open(itsPath, ios::in|ios::binary);
  if(!itsInFile->good() || itsInFile->eof())
	{
	  *itsLogFile << "*** WARNING: Missing EPS image: "
				  << static_cast<char *>(itsPath) << endl;
	   precedingElementMissing = true;
	}
  else
	{
	  precedingElementMissing = false;

	  int size = itsClippingPoints.size();

	  if (!itsClippingRect.IsEmpty())
		WriteEPSConcatClipping(itsClippingRect);
	  else if (itsClippingPoints.size() > 0)
		WriteEPSConcatClipping(itsClippingPoints);
	  else
		WriteEPSConcat();

	  
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
