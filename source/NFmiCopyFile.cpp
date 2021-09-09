// ======================================================================
/*!
 * \file
 * \brief Implementation of file copying functions
 *
 * \todo Replace isTrue with true
 */
// ======================================================================

#include "NFmiCopyFile.h"

#include "NFmiGlobals.h"
#include "NFmiValueString.h"

#include <cmath>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param inFile Undocumented
 * \param outFile Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiCopyFile(ifstream &inFile, ofstream &outFile)
{
  // 1.6 Metview:n ps-driveri tuottaa 513 pitkiä riviä
  // vaikka standardi puhuu 256:sta
  const short lineSize = 2800;
  char inBuf[lineSize];

  while (inFile.getline(inBuf, lineSize, '\n'))
  {
    short numToWrite = inFile.gcount() - 1;

    // miten aikaisemmin jäi itsestään pois???
    if (inBuf[numToWrite - 1] == '\x0D')
      numToWrite--;

    outFile.write(inBuf, numToWrite);

    // jotta difference käytettävissä
    outFile.put('\x0A');
  }

  return true;
}

// ----------------------------------------------------------------------
/*!
 * 10.06/Lasse: yhdistetty PsWritingin NFmiCopyFileCropping ja
 *              NFmiCopyFileWithoutShowpage; tämä kutsutaan nyt
 *              PsWritingista
 *
 * \param inFile Undocumented
 * \param outFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiCopyFileCroppingWithoutShowpage(ifstream *inFile, ofstream *outFile, NFmiRect theRect)
{
  // lisätty showpagen poisto koska itse tuotteessa on se varsinainen showpage
  // kaksi showpagea saa jotkut ohjelmat tekemään animaation

  // 5.11.99 TARVITAANKO ENÄÄ KUN STARTEISSA AKTIVOITU SHOWPAGE{}
  // 6.2.07 Poistettu tuo edellä mainittu koska ongelmia Metview:n kanssa

  // 1.6 Metview:n ps-driveri tuottaa 513 pitkiä rivejä
  const short lineSize = 2800;

  char inBuf[lineSize];
  unsigned short n, num;
  // NFmiString notShowpage("gsave annotatepage grestore");
  NFmiString bbZero("%%BoundingBox: 0 0 0 0");
  NFmiString str;
  while (inFile->getline(inBuf, lineSize, '\n'))
  {
    num = inFile->gcount();
    str.Set(reinterpret_cast<unsigned char *>(inBuf), num);
    n = static_cast<short>(str.Search(NFmiString("showp")));
    if (n > 0)
    {
      outFile->write(inBuf, num - 1);
      outFile->put('\x0A');
      continue;
    }
    n = static_cast<short>(str.Search(NFmiString("%BoundingBox:")));
    if (n > 0)
    {
      NFmiWriteBoundingBox(*outFile, theRect);
      continue;
    }
    outFile->write(inBuf, num - 1);
    outFile->put('\x0A');
  }
  return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param inFile Undocumented
 * \param outFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiCopyFileWithoutShowpage(ifstream &inFile, ofstream &outFile)
{
  // lisätty showpagen poisto koska itse tuotteessa on se varsinainen showpage
  // kaksi showpagea saa jotkut ohjelmat tekemään animaation

  // 5.11.99 TARVITAANKO ENÄÄ KUN STARTEISSA AKTIVOITU SHOWPAGE{}
  // 6.2.07 Poistettu tuo edellä mainittu koska ongelmia Metview:n kanssa

  // 1.6 Metview:n ps-driveri tuottaa 513 pitkiä rivejä
  const short lineSize = 2800;
  char inBuf[lineSize];
  unsigned short num;
  // NFmiString notShowpage("gsave annotatepage grestore");
  NFmiString str;
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
    num = inFile.gcount();
    str.Set(reinterpret_cast<unsigned char *>(inBuf), num);
    /*	  n = static_cast<short>(str.Search( NFmiString("showp")));
              if (n > 0 )
                    {

                      m = static_cast<short>(str.Search( NFmiString("grestore showpage")));
                      if(m > 0)
                            outFile.write(notShowpage.CharPtr(), notShowpage.GetLen());
                      else

                            outFile.write(inBuf, num-1);
                    }
              else
                    {
            */
    outFile.write(inBuf, num - 1);
    //	}
    outFile.put('\x0A');
  }
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Tämä funktio päätetty tehdä Perlillä joten toistaiseksi hyödytön
 * eikä lopullisesti testattu
 *
 * \param inFile Undocumented
 * \param outFile Undocumented
 * \param colorBag Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiCopyFileColoring(ifstream &inFile, ofstream &outFile, NFmiCmykColorBag *colorBag)
{
  // 1.6 Metview:n ps-driveri tuottaa 513 pitkiä riviä
  // vaikka standardi puhuu 256:sta

  const short lineSize = 2800;
  char inBuf[lineSize];

  short nColor;
  NFmiString ownColor("LampoVari");
  NFmiString proc2("%%");
  NFmiString zeroX("0 x");
  NFmiString pc("Pc");
  NFmiString str;
  short nProc2 = 0;
  short nZeroX = 0;
  short nPc = 0;
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
    short num = inFile.gcount();
    nColor = static_cast<short>(str.Search(ownColor));
    if (nColor > 0)
    {
      if (str.Search(proc2))
      {
        nProc2++;
      }
      if (str.Search(zeroX))
      {
        nZeroX++;
      }
      if (str.Search(pc))
      {
        nPc++;
      }
      outFile.write(inBuf, num - 1);
    }
    else
    {
      outFile.write(inBuf, num - 1);
    }
    outFile.put('\x0A');  // jotta difference käytettävissä
  }
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Tämä funktio päätetty tehdä Perlillä joten toistaiseksi hyödytön
 * eikä lopullisesti testattu
 *
 * \param inFile Undocumented
 * \param outFile Undocumented
 * \param theRect Undocumented
 * \param colorBag Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiCopyFileCroppingAndColoring(ifstream &inFile,
                                     ofstream &outFile,
                                     NFmiRect theRect,
                                     NFmiCmykColorBag *colorBag)
{
  // 1.6 Metview:n ps-driveri tuottaa 513 pitkiä rivejä
  const short lineSize = 2800;
  char inBuf[lineSize];

  unsigned short nSlash, num, nColor;
  bool BBFound = false;
  NFmiString newBounding("%%BoundingBox: ");
  NFmiString bounding("%%BoundingBox:");
  NFmiString ownColor("LampoVari");
  NFmiString proc2("%%");
  NFmiString proc2par("%%+ (");
  NFmiString customColors("CustomColors");
  NFmiString zeroX("0 x");
  NFmiString pc("Pc");
  NFmiString f("f ");
  NFmiString str, str2, hString, colorName;
  short nProc2 = 0;
  short nZeroX = 0;
  short nPc = 0;

  inFile.getline(inBuf, lineSize, '\n');
  num = inFile.gcount();
  str.Set(reinterpret_cast<unsigned char *>(inBuf), num - 1);
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
    num = inFile.gcount();
    str2.Set(reinterpret_cast<unsigned char *>(inBuf), num - 1);

    // vain kerran startTiedostoon, epseihin pitää erikseen
    if (!BBFound)
    {
      nSlash = static_cast<short>(str2.Search(bounding));
      if (nSlash > 0)
      {
        BBFound = true;
        newBounding += NFmiValueString(long(theRect.Left()));
        newBounding += NFmiString(" ");
        newBounding += NFmiValueString(long(theRect.Top()));
        newBounding += NFmiString(" ");
        newBounding += NFmiValueString(long(theRect.Right()));
        newBounding += NFmiString(" ");
        newBounding += NFmiValueString(long(theRect.Bottom()));
        str2 = NFmiString(newBounding);
      }
    }
    else  // riittää kun aloitetaan vasta BoundBoxin jälkeen
    {
      nColor = static_cast<short>(str2.Search(ownColor));
      if (nColor > 0)
      {
        hString = NFmiString(str2);
        colorName = str2.GetChars(nColor, 13);
        if (str2.Search(proc2))
        {
          if (!str2.Search(proc2par) && !str2.Search(customColors))
          {
            nProc2++;
            if (str2.Search(customColors))
              hString = NFmiString("%%CMYKCustomColor: ");
            else
              hString = NFmiString("%%+ ");
            hString += colorBag->GetColorString(colorName);
            hString += NFmiString("(");
            hString += colorName;
            hString += NFmiString(")");
          }
        }
        if (str2.Search(zeroX))
        {
          nZeroX++;
          if (str2.Search(f))
            hString = NFmiString("f ");  // ON PELKÄLLÄ CR:LLÄ EROTETTU !!
          else
            hString = NFmiString("");
          hString += colorBag->GetColorString(colorName);
          hString += NFmiString("(");
          hString += colorName;
          hString += NFmiString(") 0 x");
        }
        if (str2.Search(pc))
        {
          // \12 oli alkupaletissa ja iltiksessä vähän erilainen
          nPc++;
          str = colorBag->GetColorString(colorName);  // HUOM ed. rivi
          str += NFmiString("k");
        }
        str2 = NFmiString(hString);
      }
    }

    outFile.write(str.CharPtr(), str.GetLen());
    str = NFmiString(str2);
    outFile.put('\x0A');
  }

  outFile.write(str.CharPtr(), newBounding.GetLen());  // vielä viimeinen
  outFile.put('\x0A');
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param outFile Undocumented
 * \param theRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiWriteBoundingBox(ofstream &outFile, NFmiRect theRect)
{
  outFile.put('\x0A');
  NFmiString newBounding("%%BoundingBox: ");
  newBounding += NFmiValueString(long(theRect.Left()));
  newBounding += NFmiString(" ");
  newBounding += NFmiValueString(long(theRect.Top()));
  newBounding += NFmiString(" ");
  newBounding += NFmiValueString(long(theRect.Right()));
  newBounding += NFmiString(" ");
  newBounding += NFmiValueString(long(theRect.Bottom()));
  outFile.write(newBounding.CharPtr(), newBounding.GetLen());
  outFile.put('\x0A');
  outFile.put('\x0A');
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param inFile Undocumented
 * \param outFile Undocumented
 * \param theRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiCopyFileCropping(ifstream &inFile, ofstream &outFile, NFmiRect theRect)
{
  // 1.6 Metview:n ps-driveri tuottaa 513 pitkiä rivejä
  const short lineSize = 2800;
  char inBuf[lineSize];

  unsigned short nSlash, num;
  bool BBFound = false;
  NFmiString newBounding("%%BoundingBox: ");
  NFmiString str;
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
    num = inFile.gcount();
    // vain kerran startTiedostoon, epseihin pitää erikseen
    if (!BBFound)
    {
      str.Set(reinterpret_cast<unsigned char *>(inBuf), num);
      nSlash = static_cast<short>(str.Search(NFmiString("%%BoundingBox:")));
      if (nSlash > 0)
      {
        BBFound = true;
        newBounding += NFmiValueString(long(theRect.Left()));
        newBounding += NFmiString(" ");
        newBounding += NFmiValueString(long(theRect.Top()));
        newBounding += NFmiString(" ");
        newBounding += NFmiValueString(long(theRect.Right()));
        newBounding += NFmiString(" ");
        newBounding += NFmiValueString(long(theRect.Bottom()));
        outFile.write(newBounding.CharPtr(), newBounding.GetLen());
      }
      else
        outFile.write(inBuf, num - 1);
    }
    else
    {
      outFile.write(inBuf, num - 1);
    }
    outFile.put('\x0A');
  }
  return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param inFile Undocumented
 * \param outFile Undocumented
 * \param theRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiGenerateAndCopyUniversalSize(ifstream &inFile,
                                      ofstream &outFile,
                                      const NFmiRect &theRect,
                                      const NFmiString &theName,
                                      const NFmiString &theDate)
{
  // 1.6 Metview:n ps-driveri tuottaa 513 pitkiä rivejä
  const short lineSize = 2800;
  char inBuf[lineSize];

  unsigned short nSlash, num;
  bool PsFound = false;
  // bool BbFound = false;
  NFmiString line("%!PS-Adobe-3.0 EPSF-3.0\n");
  outFile.write(line.CharPtr(), line.GetLen());
  line = NFmiString("%%Title: ");
  line += theName;
  line += NFmiString(".eps");
  line += "\n";
  outFile.write(line.CharPtr(), line.GetLen());
  line = NFmiString("%%Creator: Windows XP\n");
  outFile.write(line.CharPtr(), line.GetLen());
  line = NFmiString("%%CreationDate: ");
  line += theDate;
  line += "\n";
  outFile.write(line.CharPtr(), line.GetLen());
  line = NFmiString("%%Pages: (atend)\n");
  outFile.write(line.CharPtr(), line.GetLen());

  NFmiWriteBoundingBox(outFile, theRect);

  NFmiString str;
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
    num = inFile.gcount();
    // vain kerran startTiedostoon, epseihin pitää erikseen
    if (!PsFound)
    {
      str.Set(reinterpret_cast<unsigned char *>(inBuf), num);
      nSlash = static_cast<short>(str.Search(NFmiString("PageSize [")));
      if (nSlash > 0)
      {
        PsFound = true;
        line = NFmiString("	<</DeferredMediaSelection true /PageSize [");
        line += NFmiValueString(long(theRect.Left()));
        line += NFmiString(" ");
        line += NFmiValueString(long(theRect.Top()));
        line += NFmiString(" ");
        line += NFmiValueString(long(theRect.Right()));
        line += NFmiString(" ");
        line += NFmiValueString(long(theRect.Bottom()));
        line += NFmiString("] /ImagingBBox null>> setpagedevice");
        outFile.write(line.CharPtr(), line.GetLen());
        outFile.put('\x0A');
        continue;
      }
    }
    /*
      if(!BbFound)
            {
              str.Set(reinterpret_cast<unsigned char *>(inBuf), num);
              nSlash = static_cast<short>(str.Search( NFmiString("%BoundingBox:")));
              if (nSlash > 0)
                    {
                      BbFound = true;
                      line = NFmiString("%%BoundingBox: 0 0 0 0");
                      outFile.write(line.CharPtr(), line.GetLen());
                  outFile.put('\x0A');
                      continue;
                    }
            }
            */
    outFile.write(inBuf, num - 1);
    outFile.put('\x0A');
  }
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \param outFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiWritePSConcat(NFmiRectScale theScale, ofstream &outFile, float theRotating)
{
  double xScale = theScale.GetXScaling();
  double yScale = theScale.GetYScaling();
  double xTrans = theScale.GetEndStartPoint().X() / xScale;
  double yTrans = theScale.GetEndStartPoint().Y() / yScale;
  outFile << "gsave" << endl;
  outFile << xScale << " " << yScale << " scale" << endl;
  outFile << xTrans << " " << yTrans << " translate" << endl;
  if (theRotating != kFloatMissing)
    outFile << theRotating << " rotate" << endl;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \param outFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiWriteEPSConcat(NFmiRectScale theScale, ofstream &outFile)
{
  double xScale = theScale.GetXScaling();
  double yScale = theScale.GetYScaling();
  double xTrans = (theScale.GetEndStartPoint().X() / xScale - theScale.GetStartStartPoint().X());
  double yTrans = (theScale.GetEndStartPoint().Y() / yScale - theScale.GetStartStartPoint().Y());

  outFile << "BeginEPSF" << endl;
  outFile << xScale << " " << yScale << " scale" << endl;
  outFile << xTrans << " " << yTrans << " translate" << endl;
  outFile << "%%BeginDocument: XXX.EPS" << endl;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \param outFile Undocumented
 * \param theClippingRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiWriteEPSConcatClipping(NFmiRectScale theScale, ofstream &outFile, NFmiRect theClippingRect)
{
  double xScale = theScale.GetXScaling();
  double yScale = theScale.GetYScaling();
  double xTrans = (theScale.GetEndStartPoint().X() / xScale - theScale.GetStartStartPoint().X());
  double yTrans = (theScale.GetEndStartPoint().Y() / yScale - theScale.GetStartStartPoint().Y());

  outFile << "newpath" << endl;
  outFile << theClippingRect.Left() << " " << theClippingRect.Top() << " "
          << "moveto" << endl;
  outFile << theClippingRect.Right() << " " << theClippingRect.Top() << " "
          << "lineto" << endl;
  outFile << theClippingRect.Right() << " " << theClippingRect.Bottom() << " "
          << "lineto" << endl;
  outFile << theClippingRect.Left() << " " << theClippingRect.Bottom() << " "
          << "lineto" << endl;
  outFile << "closepath" << endl;
  outFile << "clip" << endl;
  outFile << "newpath" << endl;
  outFile << "BeginEPSF" << endl;
  outFile << xScale << " " << yScale << " scale" << endl;
  outFile << xTrans << " " << yTrans << " translate" << endl;
  outFile << "%%BeginDocument: XXX.EPS" << endl;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theScale Undocumented
 * \param theDirection Undocumented
 * \param outFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiWritePSConcatRotating(NFmiRectScale theScale, float theDirection, ofstream &outFile)
{
  double xScale = theScale.GetXScaling();
  double yScale = theScale.GetYScaling();
  double xTrans = theScale.GetEndCenter().X();
  double yTrans = theScale.GetEndCenter().Y();
  outFile << "gsave" << endl;
  outFile << "[" << xScale << " " << 0 << " " << 0 << " " << yScale << " " << xTrans << " "
          << yTrans << "] concat" << endl;

  // määrittelysymboli näyttää länteen
  // ja PS kiertää eri suuntaan kuin meteorologiassa

  outFile << (270.f - theDirection) << " rotate" << endl;

  return true;
}

// ----------------------------------------------------------------------
/*!
 * \param outFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiWritePSEnd(ofstream &outFile)
{
  outFile << "grestore" << endl;
  return true;
}

// ----------------------------------------------------------------------
/*!
 * \param outFile Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiWriteEPSEnd(ofstream &outFile)
{
  outFile << "%%EndDocument" << endl;
  outFile << "EndEPSF" << endl;
  return true;
}

// ======================================================================
