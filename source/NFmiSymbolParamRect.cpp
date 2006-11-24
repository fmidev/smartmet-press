// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiSymbolParamRect.cpp
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiSymbolParamRect.h"
#include "NFmiRectScale.h"
#include "NFmiPsWriting.h"
#include "NFmiPressParam.h"
#include "NFmiPressProduct.h"
#include "NFmiSettings.h"
#include "NFmiFileSystem.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor deletes the internal data
 * \todo Remove the unnecessary ifs
 */
// ----------------------------------------------------------------------

NFmiSymbolParamRect::~NFmiSymbolParamRect(void)
{
  if(itsSubDir)
	delete static_cast<NFmiString *>(itsSubDir);
  if(itsMapping)
	delete static_cast<NFmiParamMapping *>(itsMapping);
  if(itsOrigDir)
	delete static_cast<NFmiString *>(itsOrigDir);

}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theSymbolRect The object to copy
 */
// ----------------------------------------------------------------------

NFmiSymbolParamRect::NFmiSymbolParamRect(const NFmiSymbolParamRect & theSymbolRect)
  : NFmiParamRect(theSymbolRect)
  , itsConstSymbol(theSymbolRect.itsConstSymbol)
  , fIsConstSymbol(theSymbolRect.fIsConstSymbol)
  , itsMapping(new NFmiParamMapping(*theSymbolRect.itsMapping))
  , itsSubDir(new NFmiString(*theSymbolRect.itsSubDir))
  , itsOrigDir(new NFmiString(*theSymbolRect.itsOrigDir))
  , itsSymbolSetName(theSymbolRect.itsSymbolSetName)
  , itsDefToProductScale(theSymbolRect.itsDefToProductScale)

{
}

// ----------------------------------------------------------------------
/*!
 * Make a new copy of this
 *
 * \return The copy
 * \todo Should return an auto_ptr
 * \todo Remove the unnecessary static_cast
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmiSymbolParamRect::Clone(void) const
{
  return static_cast<NFmiParamRect *>(new NFmiSymbolParamRect(*this));
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ReadDescription(NFmiString & retString)
{
  double x,y;
  NFmiValueString valueString;
  if (itsOrigDir) delete itsOrigDir;
  NFmiString origDir;
  NFmiString inDir;
 
  inDir = GetHome();
  inDir += kFmiDirectorySeparator;
  inDir += "LyhytSymbolit";
#ifdef UNIX
  if(NFmiSettings::IsSet("press::tmpsymbolpath"))
	inDir = NFmiSettings::Require<string>("press::tmpsymbolpath");
#endif
  inDir += kFmiDirectorySeparator;
  origDir = GetHome();
  origDir += kFmiDirectorySeparator;
  origDir += "Symbolit";
  origDir += kFmiDirectorySeparator;
 
  NFmiString subDir = NFmiString("Massa");  //oletus vaikka t‰m‰
  itsOrigDir = new NFmiString(origDir);	//oikeastaan koko polku
 
  NFmiString  helpString;
  NFmiString* mappingName;
  NFmiValueString helpValueString;
  itsMapping = new NFmiParamMapping;
  itsModifier = kNoneModifier;
  SetPreReadingTimes();

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);

  long long1,long2;
  double r1,r2,r3,r4;
  double intervalEps = 0.00001;
  NFmiPoint sizePoint1, sizePoint2;
  sizePoint1.Set(0.,0.);
  sizePoint2.Set(c40,c40); //ksymbolgrupin oletus on myˆs t‰m‰ -> skaala 1 : 1
 
  itsRelRect.Set(NFmiPoint(0.,0.),NFmiPoint(1.,1.));
  itsSizeFactor = NFmiPoint(1. ,1.);

  // onko asetettu, ent‰s rotparr jossa oli sijoitusvirhe toimiiko vanha sijoitus
  if(fNewScaling)
	itsRelRect += NFmiPoint(-.5, -.5);

  itsMultiMapping = 0;

  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel)
		itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Symbolissa" << endl;
		  retString = itsString;
		  return false;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:	  //tuntematon sana
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #Symbolissa: "
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
		 
		case dSymbolDirectory:
		  {
			if (!ReadEqualChar())
			  break;
		
			subDir = ReadString();
		
			ReadNext();
			break;
		  }
		case dConstSymbolName:
		  {
			if (!ReadEqualChar())
			  break;
			itsConstSymbol = ReadString();
		
			ReadNext();
			break;
		  }
		case dMapping:
		  {
			NFmiMappingInterval interval;
			if (!ReadEqualChar())
			  break;

			if(ReadDouble(r1))
			  {
            	*itsDescriptionFile >> itsObject;
				helpValueString = NFmiValueString(itsObject);
	            if(helpValueString.IsNumeric())
				  {
					r2 = static_cast<double>(helpValueString);
                 	*itsDescriptionFile >> itsObject;
				  }
				else
				  {
					r2 = r1;
				  }
			
				interval.itsBottomValue = r1 - intervalEps; //varmuuden vuoksi jotta vertailut oiken p‰in
				interval.itsTopValue = r2 + intervalEps;
			
				mappingName = new NFmiString(itsObject);
				interval.itsSymbol = *mappingName;
				interval.fIsScaled = false;
				itsMapping->AddMappingInterval(interval);
			  }
		
			ReadNext();
			break;
		  }
		case dScaleMapping:
		  {
			NFmiMappingInterval interval;
			if (!ReadEqualChar())
			  break;
		
			if(ReadTwo(long1, long2))
			  {
				interval.itsBottomValue = long1;
				interval.itsTopValue = long2;
			
				mappingName = new NFmiString(itsObject);
				interval.itsSymbol = *mappingName;
				interval.fIsScaled = true;
				itsMapping->AddMappingInterval(interval);
			  }
		
			ReadNext();
			break;
		  }
		case dRelSize:
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadOne(x))
			  {
				double xh,yh;
				xh = yh = 1.;
				if(x>0.)
				  xh = x;
			
				*itsDescriptionFile >> itsObject;
				valueString = itsObject;
				if(valueString.IsNumeric())
				  {
					y = static_cast<double>(valueString);
					if(y>0)
					  yh = y;
					*itsDescriptionFile >> itsObject;
					itsString = itsObject;
				  }
				else
				  {
					yh = xh;
					itsString = valueString;
				  }
				itsRelRect.Inflate((xh-1.)/2., (yh-1)/2.);
				itsRelRect += NFmiPoint(-(2-xh)/2., -(2-yh)/2.);
				itsRelRect += NFmiPoint(.5, .5);
			
				itsSizeFactor = NFmiPoint(xh,yh);
			  }
			else
			  {
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
			  }

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dPlaceMove:
		  {
			if (!ReadEqualChar())
			  break;
		
			if(ReadTwo(r1,r2))
			  {
				itsRelRect += NFmiPoint(r1/c40, r2/c40);
			  }
		
			ReadNext();
			break;
		  }
		case dRelPlace:
		  {
			if (!ReadEqualChar())
			  break;

			if(Read4Double(r1,r2,r3,r4))
			  {
				itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
			  }
		
			ReadNext();
		
			break;
		  }
		case dProducer:
		  {
			SetOne(long1); // luetaan vain pois
			break;
		  }
		case dRelDay:
		  {
			SetOne(itsFirstDeltaDays);
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: P‰ivi‰ ei voi asettaa #Symbolissa"  << endl;
			break;
		}
		case dHour:
		  {
			SetOne(itsFirstPlotHours);
			break;
		  }
		default:
		  {
			ReadRemaining();
			break;
		  }
		}
	} //while

  if (itsSubDir)
	delete itsSubDir;
  itsSubDir = new NFmiString(inDir);	//oikeastaan koko polku
  itsSymbolSetName = subDir;

  // SYMBOLIKOKO ON NYT OMASSA TIEDOSTOSSAAN JOKAISTA SETTIƒ VARTEN
  // MUTTA NYT JO VANHAA TEKNIIKKAA (kes‰ 88)
  // ei anneta en‰‰ m‰‰rittelyiss‰

  NFmiString sizeFile = *itsOrigDir;
  sizeFile += itsSymbolSetName;
  sizeFile += NFmiString("/");
  sizeFile += NFmiString("symbolikoko.txt");
  ifstream input(sizeFile, ios::in);
/*
  if(input.good() && !input.eof())
    {
	  if(Read4Double(r1,r2,r3,r4))
		{
		  if(r1 == r3) // yksinkertaistettu yhteen lukuun, PITƒISI TEHDƒ PAREMMIN
			{
			  sizePoint1.Set(0., 0.);
			  sizePoint2.Set(r1,r1);
			}
		  else
			{
			  sizePoint1.Set(r1,r2);
			  sizePoint2.Set(r3,r4);
			}
		 
		}
	  else
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: Ongelmia symbolikoko-tiedostossa"  << endl;
		}
	  fNewScaling = false;
	}
  else
	{
	*/
	  fNewScaling = true;
	//}
 
  itsDefToProductScale.SetStartScales(NFmiRect(sizePoint1,sizePoint2));
 
  input.close();
 
  SetPostReadingTimes();

  SetIdent(NFmiDataIdent(NFmiParam(itsIdentPar),240)); // joku NFmiProducer(itsProducer)

  DoPostReading();

  if(fNewScaling)    // t‰m‰ DoPostReading:n j‰lkeen
	itsRelRect += NFmiPoint(1.,1.);

  SetRect(NFmiRect(itsRelRect)); // vain Rect t‰h‰n

  retString = itsString;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 * \todo Check the implementation is complete, it seems strange
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ReadRemaining(void)
{
  return NFmiParamRect::ReadRemaining();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

int NFmiSymbolParamRect::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();

  if(lowChar==NFmiString("symboldirectory") ||
	 lowChar==NFmiString("symbolitaulu") ||
	 lowChar==NFmiString("kuvakansio"))
	return dSymbolDirectory;

  else if(lowChar==NFmiString("sizefactor") ||
		  lowChar==NFmiString("kokokerroin"))
	return dRelSize;

  else if(lowChar==NFmiString("mapping") ||
		  lowChar==NFmiString("arvoistasymboli") ||
		  lowChar==NFmiString("muunnos"))
	return dMapping;

  else if(lowChar==NFmiString("scaledsymbol") ||
		  lowChar==NFmiString("skaalamuunnos"))
	return dScaleMapping;
  
  else if(lowChar==NFmiString("name") ||
          lowChar==NFmiString("nimi"))
	return dConstSymbolName;

  else
	return NFmiParamRect::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 * \param theDestinationFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::CopyShortSymbol2Dest(NFmiString * symbolFile,
											   ofstream & theDestinationFile,
											   float theRotating)
{
  NFmiString fileName = *itsSubDir;
  fileName += itsSymbolSetName;
  fileName += NFmiString("_");
  fileName += *symbolFile;
  fileName += NFmiString(".ps");
  ifstream inFile(fileName, ios::in|ios::binary);
  if(inFile.good() && !inFile.eof())
  {
	  NFmiWritePSConcat(itsDefToProductScale, theDestinationFile, theRotating);
	  NFmiCopyFile(inFile,theDestinationFile);
	  NFmiWritePSEnd(theDestinationFile);
	  return isTrue;
  }
  else
	return false;
}

// ----------------------------------------------------------------------
/*!
 * raw, uncompressed symbol code newer than compressed one; 
 * a new compressed one should be built
 *
 * \param symbolFile Undocumented
 * \param theDestinationFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::RawSymbolToBeConverted(NFmiString * symbolFile)
{
  NFmiString fileName = *itsSubDir;
  fileName += itsSymbolSetName;
  fileName += NFmiString("_");
  fileName += *symbolFile;
  fileName += NFmiString(".ps");
  long comprAge = NFmiFileSystem::FileAge(fileName.CharPtr());

  NFmiString inputName = *itsOrigDir;
  inputName += itsSymbolSetName;
  inputName += kFmiDirectorySeparator;
  inputName += *symbolFile;
  inputName += NFmiString(".ai");
  long unComprAge = NFmiFileSystem::FileAge(inputName.CharPtr());

  return unComprAge >= 0 
	  && (comprAge < 0 || unComprAge <= comprAge);
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ConvertOrig2Short(NFmiString * symbolFile)
{

  // VAIN WINDOWSILLA TEHDYILLE SYMBOLEILLE

  NFmiString inputName = *itsOrigDir;
  inputName += itsSymbolSetName;
  inputName += NFmiString("/");
  inputName += *symbolFile;
  inputName += NFmiString(".ai");
  ifstream input(inputName, ios::in);

  if(input.good() && !input.eof())
	{
	  NFmiString outputName = *itsSubDir;
	  outputName +=  itsSymbolSetName;
	  outputName += NFmiString("_");
	  outputName += *symbolFile;
	  outputName += NFmiString(".ps");
	  ofstream output(outputName);
	 
	  const short lineSize = 250;	// HUOM max rivipituus
	  char inBuf[lineSize];
	  bool copyOn = false;
	  unsigned long copiedLines = 0;
	  NFmiString mess = NFmiString ("%*** ");
	  mess += *symbolFile;
	  mess += NFmiString (" ALKAA ***");
	  unsigned long len = mess.GetLen();
	  output.write(mess.CharPtr(),len);
	  output.write(static_cast<char *>(NFmiString("\n")),1);
	  short num;
	  char endLine = '\n';
	  while (input.getline(inBuf, lineSize, endLine)) //newline PC:lle
		{
		  NFmiString apu = NFmiString(inBuf);
		 
		  if(apu.Search(NFmiString("%AI5_BeginLayer")) > 0 && copyOn)
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: Symbolin "
							<< static_cast<char *>(*symbolFile)
							<< "konvertointi ep‰onnistui, BeginLayer:ta, sis‰kk‰in"
							<< endl;
			  copyOn = isTrue;
			}
		  else if(apu.Search(NFmiString("%AI5_BeginLayer")) > 0 &&
				  apu.Search(NFmiString("%AI5_EndLayer")) < 1)
			{
			  copyOn = isTrue;
			}
		 
		  if(copyOn)
			{
			  num = input.gcount();
			  output.write(inBuf, num-1);
			  output.put('\x0A');		  //jotta difference k‰ytett‰viss‰
			  copiedLines++;
			}
		  if(apu.Search(NFmiString("%AI5_EndLayer")) > 0 && copyOn)
			{
			  copyOn = false;
			}
		}
	  mess = NFmiString ("%*** ");
	  mess += *symbolFile;
	  mess += NFmiString (" LOPPU ***");
	  len = mess.GetLen();
	  output.write(mess.CharPtr(),len);
	  output.write(static_cast<char *>(NFmiString("\n")),1);
	 
	  if(itsLogFile)
		*itsLogFile << "rivej‰ kopioitu: " << copiedLines << " kun" << endl;
	 
	  return isTrue;
	}
   else
	 return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theAbsoluteRectOfSymbolGroup Undocumented
 * \param theQI Undocumented
 * \param theDestinationFile Undocumented
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::WritePS(const NFmiRect & theAbsoluteRectOfSymbolGroup,
								  NFmiFastQueryInfo * theQI,
								  ofstream & theDestinationFile,
								  FmiPressOutputMode theOutput)
{
  bool lastElementStatus = GetPressProduct()->LastSymbolStatus();
  GetPressProduct()->SetLastSymbolStatus(true);
  if(itsPressParam->IsBackupStation())
  {
	  if(lastElementStatus)
	  {
			*itsLogFile << "  vara-asemaa ei tarvita symbolille" << endl;
		    return true;
	  }
	  else
			*itsLogFile << "  vara-asemaa k‰ytet‰‰n symbolissa" << endl;
  }

  if(!ActiveTimeIndex(itsPressParam->GetCurrentStep()))
    {
	  return true;
    }
 
  itsCurrentSegmentTime = (static_cast<NFmiQueryInfo *>(theQI))->Time();
  itsCurrentTime = itsCurrentSegmentTime;

  itsDefToProductScale.SetEndScales(theAbsoluteRectOfSymbolGroup.ToAbs(NFmiRect(TopLeft(),BottomRight())));
  if(itsPressParam->GetCurrentStationStep()%2 == 0)
  {
	    itsDefToProductScale.MoveEndScales(itsAlternating);
  }

  if(!fIsConstSymbol)
	{
	  if(!ReadValues(theQI))
		return false;
	}
 
  if(!fMarkingValue)
	  return true;

  bool isScaled;
  NFmiString * symbolFile;
 
  if(fIsConstSymbol)
	{
	  symbolFile = &itsConstSymbol;
	}
  else
	{
	 
	  if (itsMultiMapping)
		{
		  bool missingFound;
		  CompleteMultiMapping();

	      symbolFile = itsMultiMapping->Map(itsCurrentParamArray, missingFound);
		  if(symbolFile->IsEqual(NFmiString("ristiriita")))
		  {
			  *itsLogFile << "ERROR: ristiriita symbolia m‰‰r‰tt‰ess‰" << endl;
		  }
		  if(missingFound)
			{
			  itsNumOfMissing++;
			  if(!symbolFile)
			  {
				if(!itsMissingString)
				{
				  GetPressProduct()->SetLastSymbolStatus(false);
				  return false;
				}
				else
					symbolFile = itsMissingString;
			  }	
			}
		  isScaled = false;         //skaalaus viel‰ poissa
	  }
	  else
	  {
		  symbolFile = itsMapping->Map(itsCurrentParamValue, isScaled);
		  if(!symbolFile && itsCurrentParamValue==kFloatMissing 
			  && itsMissingString)
		  {
			GetPressProduct()->SetLastSymbolStatus(false);

			symbolFile = itsMissingString;
		    isScaled = false;         
		  }
	  }
	}
 
  if(isScaled)
	{
	  double scaleFactor = ((static_cast<long>(itsCurrentParamValue)) % 1000)/100.; //tonni on nyt fixattu
	  NFmiRect endScales = itsDefToProductScale.GetEndScales();
	  endScales.Inflate(scaleFactor);
	  itsDefToProductScale.SetEndScales(endScales);
	}
 
  ScaleByValue();

  if(!symbolFile)
	  itsPressParam->GetPressProduct()->SetLastSymbolStatus(false);
  else
	  itsPressParam->GetPressProduct()->SetLastSymbolStatus(true);

  if(!symbolFile)
	{
	  if(itsMultiMapping)
		*itsLogFile << "WARNING: Arvoilla "
					<< itsCurrentParamArray[0]
					<< ", "
					<< itsCurrentParamArray[1]
					<< ", "
					<< itsCurrentParamArray[2]
					<< "... ei ole symbolivastaavuutta"
					<< endl;
	  else
		*itsLogFile << "WARNING: Arvolla "
					<< itsCurrentParamValue
					<< " ei ole symbolivastaavuutta"
					<< endl;
	}
  else if(*symbolFile != NFmiString("None")  && theOutput == kPostScript)
	{
	  if (itsPressParam->IsDistanceCheck() && GetOrder() <= 1)
		{
		  float value = itsCurrentParamValue;
		  if(itsMultiMapping)
			  value = itsCurrentParamArray[0];

			if (!itsPressParam->CheckAndSetDistance(static_cast<long>(FmiRound(value)), theAbsoluteRectOfSymbolGroup.Place()))
				return false;
		}

      bool rawToBeConverted = RawSymbolToBeConverted(symbolFile);

	  if(rawToBeConverted)
	  {
		  if(ConvertOrig2Short(symbolFile))
			{
			  if(itsLogFile)
				*itsLogFile << "Symboli "
							<< static_cast<char *>(*symbolFile)
							<< " konvertoitu"
							<< endl;			 
			}
		  else
			  *itsLogFile << "*** ERROR: Symbol missing from set: "
						  << static_cast<char *>(itsSymbolSetName)
						  << ":" << static_cast<char *>(*symbolFile)
						  << endl;
	  }
	  if (CopyShortSymbol2Dest(symbolFile,theDestinationFile, itsRotatingAngle))
		{
		}
	  else
		{
		  if(itsLogFile)
			{
			  *itsLogFile << "*** ERROR: Short symbol not found: "
						  << static_cast<char *>(itsSymbolSetName)
						  << ":" << static_cast<char *>(*symbolFile)
						  << endl;
			 if(rawToBeConverted)
			  *itsLogFile << "       missing permission ?"
						  << endl;
			}
		}
	}
  else if(*symbolFile != NFmiString("None") && theOutput == kMetaLanguage)
	{
	  NFmiPoint realPos(theAbsoluteRectOfSymbolGroup.BottomLeft());

	  //HUOM ??? otetaan takaisin t‰m‰ alunperin selitt‰m‰tˆn siirto
	  realPos += NFmiPoint(20, -20);

	  WriteMetaCode(symbolFile, realPos, theDestinationFile);

	}

  else if(*symbolFile != NFmiString("None") && theOutput == kPlainText)
	{
	  theDestinationFile <<  static_cast<char *>(*symbolFile) << endl;
	}
  else if(*symbolFile != NFmiString("None") && theOutput == kXml)
	{
		theDestinationFile << "<Picture>" << endl;
	}
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param symbolFile Undocumented
 * \param realPos Undocumented
 * \param os Undocumented
 */
// ----------------------------------------------------------------------

void NFmiSymbolParamRect::WriteMetaCode(NFmiString * symbolFile,
										NFmiPoint realPos,
										ofstream & os)
{
  os << endl
	 << "SymbolPath /var/www/share/symbolit/"
	 << static_cast<char *>(itsSymbolSetName)
	 << endl;
  os << "Symbol "
	 << static_cast<char *>(*symbolFile)
	 << ".gif "
	 << realPos.X()
	 << " "
	 << realPos.Y()
	 << " -scalex "
	 << int(itsDefToProductScale.GetXScaling()*100.)
	 << "%"
	 << " -scaley "
	 << int(itsDefToProductScale.GetYScaling()*100.)
	 << "%";
  os << endl;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theQI Undocumented
 * \param setRelHour Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiSymbolParamRect::ReadValues(NFmiFastQueryInfo * theQI, bool setRelHour)
{
  //  Vain tunnit voi muuttaa t‰ll‰ tasolla, segmentin aikaluuppi menee muuten sekaisin

  if(setRelHour) // estet‰‰n 2SymbolParamRectilt‰ kahta aikamuutosta
	{
	  if(!SetRelativeHour(theQI,NFmiString("#Symboli")))
		return isFalse;
	}

  if(itsMultiMapping)
	{
	  return ReadCurrentValueArray (theQI);
	}
  else
	{
	  if(!PointOnParam(theQI, GetDataIdent().GetParam()) || !PointOnLevel(theQI))
		return false;
	 
	  return ReadCurrentValue(theQI, itsCurrentParamValue);
	}
}

// ======================================================================
