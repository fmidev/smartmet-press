// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPsSymbol
 */
// ======================================================================

#ifndef UNIX
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

// press
#include "NFmiPsSymbol.h"
//#include "NFmiPsWriting.h"
#include "NFmiFileString.h"
// newbase
#include "NFmiSettings.h"
// system
#include <iostream>

using namespace std;


// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPsSymbol::~NFmiPsSymbol(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param thePsSymbol The object to copy
 */
// ----------------------------------------------------------------------

NFmiPsSymbol::NFmiPsSymbol(const NFmiPsSymbol & thePsSymbol)
  : NFmiPressScaling()
  , itsShortDir(NFmiString(thePsSymbol.itsShortDir))
  , itsOrigDir(NFmiString(thePsSymbol.itsOrigDir))
{
}

// ----------------------------------------------------------------------
/*!
 * Make a copy of this
 *
 * \return The clone
 * \todo Should return an auto_ptr
 */
// ----------------------------------------------------------------------

NFmiPsSymbol * NFmiPsSymbol::Clone(void) const
{
  return new NFmiPsSymbol(*this);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsSymbol::CopyShortSymbol2Dest(void)
{
#ifdef UNIX
  string tmpDir = NFmiSettings::Require<string>("press::symbolcachepath");
  tmpDir += kFmiDirectorySeparator;
  NFmiString fileName = static_cast<NFmiString>(tmpDir);
#else
  NFmiString fileName = itsShortDir;
#endif

  fileName += NFmiFileString(itsOrigDir).Directory();
  fileName += NFmiString("_");
  fileName += itsSymbol;
  fileName += NFmiString(".ps");

  if (itsInFile)
	delete itsInFile;
  itsInFile = new ifstream;
  itsInFile->open(fileName, ios::in|ios::binary);
//  itsInFile->open(fileName, ios::in|ios::in);

  if(itsInFile->good() && !itsInFile->eof())
	{
	  WritePSConcat();
      CopyPsFile();	// for some reason the return value is ignored here
	  WritePSEnd();
	  itsInFile->close();
	  itsInFile->clear();
	  return isTrue;
	}
  else
	itsInFile->close();
  itsInFile->clear();
  return isFalse;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsSymbol::ConvertOrig2Short(void)
{

  // VAIN WINDOWSILLA TEHDYILLE SYMBOLEILLE

#ifdef UNIX
  string inputDir = NFmiSettings::Require<string>("press::path");
  inputDir += kFmiDirectorySeparator;
  inputDir += "symbols";
  inputDir += kFmiDirectorySeparator;
  NFmiString inputName = static_cast<NFmiString>(inputDir);
  inputName += NFmiFileString(itsOrigDir).Directory();
  inputName += kFmiDirectorySeparator;
  inputName += itsSymbol;
  inputName += NFmiString(".ai");
#else
  NFmiString inputName = itsOrigDir;
  //inputName += NFmiString("/");
  inputName += itsSymbol;
  inputName += NFmiString(".ai");
#endif

  ifstream input(inputName, ios::in);

  if(input.good() && !input.eof())
	{
#ifdef UNIX
	  string tmpDir = NFmiSettings::Require<string>("press::symbolcachepath");
	  tmpDir += kFmiDirectorySeparator;
	  NFmiString outputName = static_cast<NFmiString>(tmpDir);
#else
	  NFmiString outputName = itsShortDir;
#endif
 	  outputName += NFmiFileString(itsOrigDir).Directory();
      outputName += NFmiString("_");
      outputName += itsSymbol;
      outputName += NFmiString(".ps");
      ofstream output(outputName, ios::out);

	  if(!output && itsLogFile)
		*itsLogFile << "*** ERROR: Opening of '" << outputName << "' for output failed" << endl;


      const short lineSize = 250;	// HUOM max rivipituus
      char inBuf[lineSize];
      bool copyOn = isFalse;
      unsigned long copiedLines = 0;
      NFmiString mess = NFmiString ("%****** ");
      mess += itsSymbol;
      mess += NFmiString (" BEGINS ********************");
      output.write(mess.CharPtr(),40);
      output.write(static_cast<char *>(NFmiString("\n")),1);
      short num;
      char endLine = '\n';
      while (input.getline(inBuf, lineSize, endLine)) //newline PC:lle
		{
	      NFmiString apu = NFmiString(inBuf);

	      if(apu.Search(NFmiString("%AI5_BeginLayer")) > 0 && copyOn)
//	      if(NFmiString(inBuf) == NFmiString("%AI5_BeginLayer") && copyOn)
			{
		      if(itsLogFile)
				*itsLogFile << "*** ERROR: Conversion of "
							<< static_cast<char *>(itsSymbol)
							<< " to short symbol failed, nested BeginLayers"
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
			  output.put('\x0A');		  //jotta difference käytettävissä
			  copiedLines++;
			}
          if(apu.Search(NFmiString("%AI5_EndLayer")) > 0 && copyOn)
			{
			  copyOn = isFalse;
			}
		}
      mess = NFmiString ("%****** ");
      mess += itsSymbol;
      mess += NFmiString (" ENDS ************************");
      output.write(mess.CharPtr(),40);
      output.write(static_cast<char *>(NFmiString("\n")),1);

      if(itsLogFile)
		*itsLogFile << "  lines copied: " << copiedLines << endl;

	  output.close();
	  output.clear();

	  // tämä tekee mahdolliseksi lukemisen saman ohjelman puitteissa ?????
	  // itsInFile.open(outputName, ios::in|ios::in);
      // itsInFile.close();

	  itsInFile->open(outputName, ios::in|ios::in);
      itsInFile->close();
	  output.clear();
	  return isTrue;
	}
  else
	return isFalse;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsSymbol::ReadDescription(NFmiString & retString)
{
  NFmiValueString valueString;
  NFmiPoint sizePoint1Size, sizePoint2Size;
  NFmiPoint sizePoint1NotSize, sizePoint2NotSize;
  sizePoint1Size.Set(0,0);
  sizePoint2Size.Set(c40,c40); //ksymbolgrupin oletus on myös tämä -> skaala 1 : 1
  double c20 = c40/2.;
  sizePoint1NotSize.Set(-c20,-c20);
  sizePoint2NotSize.Set(c20,c20); //ksymbolgrupin oletus on myös tämä -> skaala 1 : 1
  itsWriteScale.SetStartScales(NFmiRect(sizePoint1NotSize,sizePoint2NotSize));

  NFmiString inDir;
  inDir = GetHome();
  inDir += kFmiDirectorySeparator;
  inDir += "LyhytSymbolit";
#ifdef UNIX
  inDir = getSymbolCachePath();
#endif
  inDir += kFmiDirectorySeparator;

#ifndef UNIX
  itsOrigDir = GetHome();
  itsOrigDir += kFmiDirectorySeparator;
  itsOrigDir += "Symbolit";
  itsOrigDir += kFmiDirectorySeparator;
#else
  itsOrigDir = getCnfPath();
  itsOrigDir += kFmiDirectorySeparator;
  itsOrigDir += "symbols";
  itsOrigDir += kFmiDirectorySeparator;
#endif

  NFmiString subDir = NFmiString("Massa");  //oletus vaikka tämä

  NFmiString  helpString;
  NFmiValueString helpValueString;

  *itsDescriptionFile >> itsObject;
  itsString = itsObject;
  itsIntObject = ConvertDefText(itsString);
  double r1,r2,r3,r4;
  double xmin,xmax,ymin,ymax;
  double x, y;
  xmin = ymin = 0;
  xmax = ymax = 1;

  while(itsIntObject != 9999 || itsCommentLevel)
	{
	  if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;
	  if(itsLoopNum > itsMaxLoopNum)
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: max file length exceeded in #FixedSymbol" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Unknown word in #FixedSymbol: "
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

		case dPsRelSize:
		  {
			if (!ReadEqualChar())
			  break;
			if(ReadDouble(x))
			  {
				double xh,yh;
				xh = yh = 1.;
				if(x>0.)
				  xh = c20/x;
				*itsDescriptionFile >> itsObject;
			   valueString = itsObject;
			   if(valueString.IsNumeric())
				 {
				   y = static_cast<double>(valueString);
				   if(y>0)
					 yh = c20/y;
				   *itsDescriptionFile >> itsObject;
				   itsString = itsObject;
				 }
			   else
				 {
				   yh = xh;
				   itsString = valueString;
				 }

			   sizePoint2NotSize.Set(xh,yh);
			   sizePoint1NotSize.Set(-xh,-yh);
			  }
			else
			  {
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
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
		  
		case dSymbolDir:
		  {
			if (!ReadEqualChar())
			  break;
			subDir = ReadString();
			ReadNext();
			break;
		  }
		case dConstantSymbol:
		  {
			if (!ReadEqualChar())
			  break;
			itsSymbol = ReadString();

			ReadNext();
			break;
		  }
		case dSymbolSize: //vanhentunut
		  {
			if (!ReadEqualChar())
			  break;

			if(ReadDouble(x))
			  {
				*itsDescriptionFile >> itsObject;
				valueString = itsObject;
				if(valueString.IsNumeric())
				  {
					y = static_cast<double>(valueString);
					*itsDescriptionFile >> itsObject;
					itsString = itsObject;
				  }
				else
				  {
					y = x;
					itsString = valueString;
				  }
				itsRectSize.Set(x,y);
			  }
			else
			  {
				*itsDescriptionFile >> itsObject;
				itsString = itsObject;
			  }

			itsIntObject = ConvertDefText(itsString);
			break;
		  }
		case dRelSymbolSize: //vanhentunut
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(xmin,ymin,xmax,ymax))
			  {
				itsRelArea.Set(NFmiPoint(xmin,ymin),NFmiPoint(xmax,ymax));
			  }

			ReadNext();
			break;
		  }
		default:
		  {
			ReadRemaining();
			break;
		  }
		}
	} // while
  itsShortDir = NFmiString(inDir);	//oikeastaan koko polku
  itsOrigDir += subDir;
  itsOrigDir += kFmiDirectorySeparator;

  // SYMBOLIKOKO ON NYT OMASSA TIEDOSTOSSAAN JOKAISTA SETTIÄ VARTEN
  // ei anneta enää määrittelyissä

  NFmiString sizeFile = itsOrigDir;
  sizeFile += kFmiDirectorySeparator;
  sizeFile += NFmiString("symbolikoko.txt");   //kokoa ei tarvita ellei eps-file
  ifstream input(sizeFile, ios::in);

  // OletusSkaala 1->1 jos kokotiedosto ei annettu
  // itsDefToProductScale.SetEndScales(itsDefToProductScale.GetStartScales());
  // skaala tuli potenssiin kaksi (13*13, 6*6)

  if(input.good() && !input.eof())
    {

	  if(Read4Double(r1,r2,r3,r4))
		{
		  if(r1 == r3)     // yksinkertaistettu yhteen lukuun
			               // PITÄISI TEHDÄ PAREMMIN
			{
			  sizePoint1Size.Set(0., 0.);
			  sizePoint2Size.Set(r1,r1);
			}
		  else
			{
			  sizePoint1Size.Set(r1,r2);
			  sizePoint2Size.Set(r3,r4);
			}

		}
	  else
		{
		  if(itsLogFile)
			//*itsLogFile << "*** ERROR: Ongelmia symbolikoko-tiedostossa"  << endl;
			*itsLogFile << "*** ERROR: OBSOLETE, check reason"  << endl;
		}
	  itsWriteScale.SetStartScales(NFmiRect(sizePoint1Size,sizePoint2Size));
	}
  else
	{
	  itsWriteScale.SetStartScales(NFmiRect(sizePoint1NotSize,sizePoint2NotSize));
	}

  input.close();

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

int NFmiPsSymbol::ConvertDefText(NFmiString & object)
{
    NFmiString lowChar = object;
    lowChar.LowerCase();
	
	if(lowChar==NFmiString("symbolset") ||
	   lowChar==NFmiString("directory") ||
	   lowChar==NFmiString("kuvakansio") ||
	   lowChar==NFmiString("hakemisto"))
	  return dSymbolDir;

	else if(lowChar==NFmiString("name") ||
			lowChar==NFmiString("nimi") ||
			lowChar==NFmiString("tiedosto"))
	  return dConstantSymbol;

	else if(lowChar==NFmiString("scale") ||
		    lowChar==NFmiString("sizefactor") ||
			lowChar==NFmiString("kokokerroin"))
	  return dPsRelSize;

	else
	  return NFmiPressScaling::ConvertDefText(object);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param place Undocumented
 * \return Undocumented
 * \todo Input by const reference instead
 */
// ----------------------------------------------------------------------

bool NFmiPsSymbol::MakeAndWritePS(NFmiPoint place)
{
  Place(place);
  ScalePlotting();
  if (CopyShortSymbol2Dest()) //itsSymbol
	{
	  return isTrue;
	}
  else
	{
	  if(ConvertOrig2Short()) //itsOrigDir
		{
		  if(itsLogFile)
			*itsLogFile << "Symbol "
						<< static_cast<char *>(itsSymbol)
						<< " converted"
						<< endl;

		  if (CopyShortSymbol2Dest()) //itsShortDir
			{
			  return isTrue;
			}
		  else
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: Symbol not found after conversion: "
							<< static_cast<char *>(itsSymbol)
							<< endl;
			  return isFalse;
			}
		}
	  else
		if(itsLogFile)
		  *itsLogFile << "*** ERROR: Missing Symbol: "
					  << static_cast<char *>(itsSymbol)
					  << endl;
	  return isFalse;
	}

}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------
bool NFmiPsSymbol::WritePSUpdatingSubText(FmiPressOutputMode theOutput)
{
	//onnistuuko Segmenttijäseniä varten näin 
	return WritePS(theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsSymbol::WritePS(FmiPressOutputMode theOutput)
{
  ScalePlotting();
  
  if(theOutput == kPlainText)
	  return true;
  
  if (CopyShortSymbol2Dest()) //itsSymbol
	{
	  return isTrue;
	}

  else
	{
	  if(ConvertOrig2Short()) //itsOrigDir
		{
		  if(itsLogFile)
			*itsLogFile << "Symbol "
						<< static_cast<char *>(itsSymbol)
						<< " converted"
						<< endl;

		  if (CopyShortSymbol2Dest()) //itsShortDir
			{
			  return isTrue;
			}
		  else
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: Symbol not found after conversion: "
							<< static_cast<char *>(itsSymbol)
							<< endl;
			  return isFalse;
			}
		}
	  else
		if(itsLogFile)
		  *itsLogFile << "*** ERROR: Missing Symbol: "
					  << static_cast<char *>(itsSymbol)
					  << endl;
	  return isFalse;
	}
}

// ======================================================================
