// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPsSymbol
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

// press
#include "NFmiPsSymbol.h"
#include "NFmiFileString.h"
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
  NFmiString fileName = itsShortDir;
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
	  bool tempBool = WritePSConcat();
	  tempBool = CopyFile();
	  tempBool = WritePSEnd();
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

  NFmiString inputName = itsOrigDir;
  inputName += NFmiString("/");
  inputName += itsSymbol;
  inputName += NFmiString(".ai");
  ifstream input(inputName, ios::in);

  if(input.good() && !input.eof())
	{
      NFmiString outputName = itsShortDir;
 	  outputName += NFmiFileString(itsOrigDir).Directory();
      outputName += NFmiString("_");
      outputName += itsSymbol;
      outputName += NFmiString(".ps");
      ofstream output(outputName, ios::out);

      const short lineSize = 250;	// HUOM max rivipituus
      char inBuf[lineSize];
      bool copyOn = isFalse;
      unsigned long copiedLines = 0;
      NFmiString mess = NFmiString ("%****** ");
      mess += itsSymbol;
      mess += NFmiString (" ALKAA ********************");
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
				*itsLogFile << "*** ERROR: Symbolin "
							<< static_cast<char *>(itsSymbol)
							<< "konvertointi epäonnistui, BeginLayer:ta, sisäkkäin"
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
      mess += NFmiString (" LOPPU ************************");
      output.write(mess.CharPtr(),40);
      output.write(static_cast<char *>(NFmiString("\n")),1);

      if(itsLogFile)
		*itsLogFile << "  rivejä kopioitu: " << copiedLines << endl;

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
  NFmiString inDir;

  inDir = GetHome();
  inDir += kFmiDirectorySeparator;
  inDir += "LyhytSymbolit";
  inDir += kFmiDirectorySeparator;
  itsOrigDir = GetHome();
  itsOrigDir += kFmiDirectorySeparator;
  itsOrigDir += "Symbolit";
  itsOrigDir += kFmiDirectorySeparator;

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
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #VakioSymbolissa" << endl;
		  retString = itsString;
		  return isFalse;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:
		  {
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #VakioSymbolissa: "
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
		case dSymbolSize:
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
		case dRelSymbolSize:
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
			*itsLogFile << "*** ERROR: Ongelmia symbolikoko-tiedostossa"  << endl;
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
	if(object==NFmiString("Directory") ||
	   object==NFmiString("KuvaKansio") ||
	   object==NFmiString("Kuvakansio") ||
	   object==NFmiString("Hakemisto"))
	  return dSymbolDir;

	else if(object==NFmiString("Name") ||
			object==NFmiString("Nimi") ||
			object==NFmiString("Tiedosto"))
	  return dConstantSymbol;

	else if(object==NFmiString("SizeFactor") ||
			object==NFmiString("KokoKerroin") ||
			object==NFmiString("Kokokerroin"))
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
			*itsLogFile << "Symboli "
						<< static_cast<char *>(itsSymbol)
						<< " konvertoitu"
						<< endl;

		  if (CopyShortSymbol2Dest()) //itsShortDir
			{
			  return isTrue;
			}
		  else
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: Symbolia ei konvertoinnin jälkeen löydy: "
							<< static_cast<char *>(itsSymbol)
							<< endl;
			  return isFalse;
			}
		}
	  else
		if(itsLogFile)
		  *itsLogFile << "*** ERROR: Symbolia ei ole: "
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

bool NFmiPsSymbol::WritePS(FmiPressOutputMode theOutput)
{
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
			*itsLogFile << "Symboli "
						<< static_cast<char *>(itsSymbol)
						<< " konvertoitu"
						<< endl;

		  if (CopyShortSymbol2Dest()) //itsShortDir
			{
			  return isTrue;
			}
		  else
			{
			  if(itsLogFile)
				*itsLogFile << "*** ERROR: Symbolia ei konvertoinnin jälkeen löydy: "
							<< static_cast<char *>(itsSymbol)
							<< endl;
			  return isFalse;
			}
		}
	  else
		if(itsLogFile)
		  *itsLogFile << "*** ERROR: Symbolia ei ole: "
					  << static_cast<char *>(itsSymbol)
					  << endl;
	  return isFalse;
	}
}

// ======================================================================
