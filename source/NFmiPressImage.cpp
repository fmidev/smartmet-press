// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressImage
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressImage.h"
#include "NFmiPressProduct.h"
#include "NFmiSettings.h"
#include "NFmiCopyFile.h"
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>

extern std::list<std::string> errors;

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

NFmiPressImage::NFmiPressImage(void)
  : NFmiPressScaling()
  ,itsPressProduct(0)
  ,itsRelHoursFromFirst(0)
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
  , itsFileWithoutTimeStamp(thePressImage.itsFileWithoutTimeStamp)
  , itsTempImagePath(thePressImage.itsTempImagePath)
  , itsTempImageDir(thePressImage.itsTempImageDir)
  , itsRelHoursFromFirst(thePressImage.itsRelHoursFromFirst)
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
  bool timeStamp = false;
  SetPreReadingTimes();
  NFmiString imageFileWithoutTimeStamp;

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
			{
			  string msg = "Maximum product length exceeded in #subimage";
			  errors.push_back(msg);
			  *itsLogFile << "*** ERROR: " << msg << endl;
			}
		  retString = itsString;
		  return false;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  {
				string msg = string("Unknown keyword in #subimage: ") + static_cast<char *>(itsObject);
				errors.push_back(msg);
				*itsLogFile << "*** ERROR: " << msg << endl;
			  }
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
				{
				  string msg = "Subimage has no product pointer. PROGRAMMING ERROR";
				  errors.push_back(msg);
				  *itsLogFile << "*** ERROR: " << msg << endl;
				}
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
				{
				  string msg = "Subimage has no product pointer. PROGRAMMING ERROR";
				  errors.push_back(msg);
				  *itsLogFile << "*** ERROR: " << msg << endl;
				}
			}
			else
			  delete image;

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dNewImageRelWithTimeStamp:
			timeStamp = true;
		case dNewImageRel:
		  {
			if (!ReadEqualChar())
			  break;

			if(!timeStamp)
			{
				tempString = ReadString();			
				tempString += NFmiString(".eps");
			}
			else
			{
				tempString = itsFileWithoutTimeStamp;			
				//tempString += NFmiString(".eps");
			}

			if(Read2Double (r1, r2))
			{
				NFmiRectScale tempScale(itsImageScale);
				tempScale.MoveEndScales(NFmiPoint(r1,r2));
				NFmiPressImage* image = new NFmiPressImage;//owner pressProduct
				*image = *this;
				if(timeStamp)
				{
				   timeStamp = false;
				   int relHour;
				   if(ReadOne(relHour))
				   {    //HUOM imagea k‰sitell‰‰n eik‰ this
					    NFmiMetTime firstTime = image->GetFirstPlotTime();
						int hourSum = image->GetRelHoursFromFirst() + relHour;
					    firstTime.ChangeByHours(hourSum);
						NFmiString timeFormat("YYYYMMDDHH00");
						AddValidTimeTimeStamp(tempString, timeFormat, firstTime);
						image->SetRelHoursFromFirst(hourSum);
			       }
				   else
				   {
						delete image;
						itsIntObject = ConvertDefText(itsString);
						break;
				   }

				}
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
					{
					  string msg = "Subimage has no product pointer. PROGRAMMING ERROR";
					  errors.push_back(msg);
					  *itsLogFile << "*** ERROR: " << msg << endl;
					}
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
		case dSummerWinterChoise:
		  {
			if (!ReadEqualChar())
			  break;

			NFmiString summerFile(ReadString());
			NFmiString winterFile(ReadString());
			NFmiString countryPart("Middle");

			if(itsPressProduct->IsSummerWeather(countryPart))
				itsTempImageFile = summerFile;
			else
				itsTempImageFile = winterFile;

			itsTempImageFile += NFmiString(".eps");
			
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
		case dImageFileWithTimeStamp:
			timeStamp = true;
		case dImageFile: 			
		  {
			if (!ReadEqualChar())
			  break;
			
			itsTempImageFile = ReadString();
            if (itsTempImageFile.Search(NFmiString(".")) <=0)			
				itsTempImageFile += NFmiString(".eps");
			itsFileWithoutTimeStamp = itsTempImageFile;
			if(timeStamp)
			{
				SetPostReadingTimes();
				NFmiString timeFormat("YYYYMMDDHH00");
				AddValidTimeTimeStamp(itsTempImageFile, timeFormat, itsFirstPlotTime);
				timeStamp = false;
			}
			
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
				  {
					string msg = "#subimage definition size min == max";
					errors.push_back(msg);
					*itsLogFile << "*** ERROR: " << msg << endl;
				  }
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
				  {
					string msg = "#subimage plotting view min == max";
					errors.push_back(msg);
					*itsLogFile << "*** ERROR: " << msg << endl;
				  }
				else
				  itsImageScale.SetEndScales(NFmiRect(NFmiPoint(xmin,ymin),
													  NFmiPoint(xmax,ymax)));
			  }
			ReadNext();
			break;
		  }
		case dImagePlottingAndClipping:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				if(xmin == xmax || ymin == ymax)
				  {
					string msg = "#subimage plotting view min == max";
					errors.push_back(msg);
					*itsLogFile << "*** ERROR: " << msg << endl;
				  }
				else
				{
				  itsImageScale.SetEndScales(NFmiRect(NFmiPoint(xmin,ymin),
													  NFmiPoint(xmax,ymax)));
				  itsClippingRect.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
				  itsClippingPoints.clear();
				}
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
				  {
					string msg = "#subimage clipping rectangle min == max";
					errors.push_back(msg);
					*itsLogFile << "*** ERROR: " << msg << endl;
				  }
				else
				{
				  itsClippingRect.Set(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax));
				  itsClippingPoints.clear();
				}
			  }
			ReadNext();
			break;
		  }
		case dImageClippingPath:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadFour(x1, y1, x2, y2) && ReadTwo(x3, y3)) //minimi 3 pistett‰
			  {
			    itsClippingRect.Set(NFmiPoint(0.,0.), NFmiPoint(0.,0.));

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
					  {
						string msg = "x-coordinate missing y-coordinate pair in clipping path";
						errors.push_back(msg);
						*itsLogFile << "*** ERROR: " << msg << endl;
					  }
				}
				itsString = valueString;
				itsIntObject = ConvertDefText(valueString);
			  }

			else
			{
			  string msg = "Too few coordinates in clipping path";
			  errors.push_back(msg);
			  *itsLogFile << "*** ERROR: " << msg << endl;
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
		{
		  string msg = "#subimage file not defined";
		  errors.push_back(msg);
		  *itsLogFile << "*** ERROR: " << msg << endl;
		}
	  retString = itsString;
	  return false;
	}
  else
	{
      NFmiString defExtension("eps");
#ifndef UNIX
	  char * env;
	  env = getenv("lehtiPuoliValmiitDir");
	  if(env == 0)
		itsPath = CreatePath(NFmiString("PuoliValmiit"),
					 		 itsTempImagePath,
							 itsTempImageDir,
							 itsTempImageFile,
							 defExtension);
	  else
		itsPath = CreatePath(NFmiString(env),
							 itsTempImagePath,
							 itsTempImageDir,
							 itsTempImageFile,
							 defExtension);
#else
	  string path = NFmiSettings::Require<string>("press::outpath");
	  NFmiString dir = static_cast<NFmiString>(path);
	  dir += kFmiDirectorySeparator;
	  dir += "misc"; // The old PuoliValmiit directory

	  itsPath = CreatePath(dir,
			       itsTempImagePath,
			       itsTempImageDir,
			       itsTempImageFile,
			       defExtension);
	  
#endif
	}

//	SetPostReadingTimes();
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

  else if(lowChar==NFmiString("filewithtimestamp") ||
	 lowChar==NFmiString("tiedostoaikaleimalla") ||
	 lowChar==NFmiString("nimiaikaleimalla") )
	return dImageFileWithTimeStamp;

  else if(lowChar==NFmiString("summerwinterchoice") ||
	      lowChar==NFmiString("kes‰talvivalinta"))
	 return dSummerWinterChoise;

  else if(lowChar==NFmiString("summerwinterchoicefocusing") ||
	 lowChar==NFmiString("summerwinterfile") ||
	 lowChar==NFmiString("kes‰talvivalintatarkentaen") ||
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

  else if(lowChar==NFmiString("newrelativelywithtimestamp") ||
		  lowChar==NFmiString("uusisuhteellisenaaikaleimalla") ||
	      lowChar==NFmiString("uusisuhtaikaleimalla")
		  )
	return dNewImageRelWithTimeStamp;
  
  else if(lowChar==NFmiString("newfilemoving") ||
		  lowChar==NFmiString("newrelatively") ||
		  lowChar==NFmiString("uusitiedostosiirtyen") ||
		  lowChar==NFmiString("uusisuhteellisena") ||
	      lowChar==NFmiString("uusisuht")
		  )
	return dNewImageRel;

  else if(lowChar==NFmiString("directory") ||
		  lowChar==NFmiString("hakemisto"))
	return dImageDir;

  else if(lowChar==NFmiString("rotationfirst") ||
	      lowChar==NFmiString("rotatefirst") ||
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
	string msg = "#stationplaceonmap not allowed in #subimage, should you use #placemove instead?";
	errors.push_back(msg);
	*itsLogFile << "*** ERROR: " << msg << endl;
	return dImagePlaceMove;
  }

  else if(lowChar==NFmiString("scale") ||
	      lowChar==NFmiString("sizefactor") ||
		  lowChar==NFmiString("kokokerroin"))
	return dImageSizeFactor;

  else if(lowChar==NFmiString("currentscalingpoints") ||
		  lowChar==NFmiString("plottingview") ||
		  lowChar==NFmiString("nykyskaalauspisteet") ||
		  lowChar==NFmiString("sijoitusalue"))
	return dImagePlottingView;
  
  else if(lowChar==NFmiString("currentscalingandclipping") ||
		  lowChar==NFmiString("plottingandclipping") ||
		  lowChar==NFmiString("nykyskaalausjarajaus") ||
		  lowChar==NFmiString("sijoitusjarajaus"))
	return dImagePlottingAndClipping;

  else if(lowChar==NFmiString("initialscalingpoints") ||
		  lowChar==NFmiString("mapdefsize") ||
		  lowChar==NFmiString("alkuskaalauspisteet") ||
		  lowChar==NFmiString("mittaalue"))
	return dImageDefSize;

  else if(lowChar==NFmiString("clippingrectangle") ||
		  lowChar==NFmiString("clip") ||
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
if(theOutput == kPlainText)
	return true;

#ifdef UNIX
  bool precedingElementMissing = false;
#else
  extern bool precedingElementMissing;
#endif

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

  
#ifdef UNIX
  NFmiString imageFile;

  if (static_cast<string>(itsTempImageFile).find("/") == string::npos )
	{
	  string path = NFmiSettings::Require<string>("press::cnfpath");
	  string productName = NFmiSettings::Require<string>("press::product");
	  NFmiString filename = itsPath;
	  
	  string temppath = path;
	  itsPath = static_cast<NFmiString>(path);
	  itsPath += kFmiDirectorySeparator;
	  itsPath += productName;
	  itsPath += kFmiDirectorySeparator;
	  itsPath += itsTempImageFile;
	  imageFile = itsPath;
	}
  else
	{
	  imageFile = itsTempImageFile;
	}
#endif
  
#ifndef UNIX
  itsInFile->open(itsPath, ios::in|ios::binary);
#else
  itsInFile->open(imageFile, ios::in|ios::binary);
#endif
  if(!itsInFile->good() || itsInFile->eof())
	{
			  //string msg = string(string("#OsaKuvassa (selite?) tuntematon maankolkka: ")
				//				  +static_cast<char *>(theCountryPart));
#ifndef UNIX
	  string msg = string(string("Missing EPS image: ") + static_cast<char *>(itsPath));
#else	  
	  string msg = string(string("Missing EPS image: ") + static_cast<char *>(imageFile));
#endif
	  errors.push_back(msg);
	  *itsLogFile << "*** ERROR: " << msg << endl;
	  precedingElementMissing = true;
	}
  else
	{
	  precedingElementMissing = false;

	  // int size = itsClippingPoints.size();

	  if (!itsClippingRect.IsEmpty())
		WriteEPSConcatClipping(itsClippingRect);
	  else if (itsClippingPoints.size() > 0)
		WriteEPSConcatClipping(itsClippingPoints);
	  else
		WriteEPSConcat();

      if(GetPressProduct()->GetPageSize() == kUniversal)
	  {
		  NFmiCopyFileCroppingWithoutShowpage(itsInFile, itsOutFile
			            ,NFmiRect(NFmiPoint(0.,0.), NFmiPoint(0.,0.)));
	  }
	  else
		CopyFileWithoutShowpage();
	  
	  WriteEPSEnd();
#ifndef UNIX	  
	  *itsLogFile << "EPS image included into eps output: "
				  << static_cast<char *>(itsPath)
				  << endl;
#else
	  *itsLogFile << "EPS image included into eps output: "
				  << static_cast<char *>(imageFile)
				  << endl;
#endif
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
