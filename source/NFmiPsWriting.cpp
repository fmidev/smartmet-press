// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPsWriting
 */
// ======================================================================

#include "NFmiPsWriting.h"
#include "NFmiString.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPsWriting::~NFmiPsWriting(void)
{
	if(itsInFile)
		delete itsInFile;
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param thePsWriting The object to copy
 */
// ----------------------------------------------------------------------

NFmiPsWriting::NFmiPsWriting(const NFmiPsWriting & thePsWriting)
  : fWriteAsLast(thePsWriting.fWriteAsLast)
  , fRotBeforeScale(thePsWriting.fRotBeforeScale)   
  , itsRotatingPoint(thePsWriting.itsRotatingPoint)  
  , itsRotatingAngle(thePsWriting.itsRotatingAngle)  
  , fOverPrint(thePsWriting.fOverPrint)
  , itsInFile(thePsWriting.itsInFile)
  , itsOutFile(thePsWriting.itsOutFile)
  , itsWriteScale(thePsWriting.itsWriteScale)
  , fLineFeed(thePsWriting.fLineFeed)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param outString Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WriteOutString(const NFmiString & outString)
{
  *itsOutFile << static_cast<char *>(outString) << endl;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WriteGRestore(void)
{
  *itsOutFile << "grestore " << endl;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::CopyPsFile(void)
{
  const short lineSize = 520; //130; 1.6 Metview:n ps-driveri tuottaa 513 pitkiä riviä
  char inBuf[lineSize];       //     vaikka standardi puhuu 256:sta

  while (itsInFile->getline(inBuf, lineSize, '\n'))
	{
	  short num = itsInFile->gcount();
	  itsOutFile->write(inBuf, num-1);	 
	  itsOutFile->put('\x0A');		  //jotta difference käytettävissä
	}
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::CopyFileWithoutShowpage(void)
{
  //lisätty showpagen poisto koska itse tuotteessa on se varsinainen showpage
  // kaksi showpagea saa jotkut ohjelmat tekemään animaation

  //1.6 Metview:n ps-driveri tuottaa 513 pitkiä rivejä
  const short lineSize = 2800;
  char inBuf[lineSize];
  unsigned short n, m, num;
  NFmiString notShowpage("gsave annotatepage grestore");
  NFmiString str;
  while (itsInFile->getline(inBuf, lineSize, '\n'))
  {
	num = itsInFile->gcount();
	str.Set(reinterpret_cast<unsigned char *>(inBuf), num);
	n = static_cast<short>(str.Search( NFmiString("showp")));
	if (n > 0 )
	  {
		m = static_cast<short>(str.Search( NFmiString("grestore showpage")));
		if(m > 0)
		  itsOutFile->write(reinterpret_cast<const char *>(notShowpage.GetCharPtr()), notShowpage.GetLen());
		else
		  itsOutFile->write(inBuf, num-1);
	  }
	else
	  {
		itsOutFile->write(inBuf, num-1);
	  }
	itsOutFile->put('\x0A');		  
  }
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WritePSConcat(void)
{
  double xScale = itsWriteScale.GetXScaling();
  double yScale = itsWriteScale.GetYScaling();
  double xTrans = (itsWriteScale.GetEndStartPoint().X() / xScale
				   - itsWriteScale.GetStartStartPoint().X());
  double yTrans = (itsWriteScale.GetEndStartPoint().Y() / yScale
				   - itsWriteScale.GetStartStartPoint().Y());

  *itsOutFile << "%********* WritePSConcat alkaa ******" << endl;

  *itsOutFile << "gsave" << endl;
  *itsOutFile << xScale << " " << yScale << " scale" << endl;
  *itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
  *itsOutFile << "%********* WritePSConcat tehty ******" << endl;

  return isTrue;
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPsWriting::Rotate(void)
{
  //tässä pitää edetä ikäänkuin käänteisessä järjestyksessä
  NFmiPoint scaledRotP = itsWriteScale.Scale(itsRotatingPoint);
  if(fRotBeforeScale)
	scaledRotP = itsRotatingPoint;
  *itsOutFile << scaledRotP.X() << " " << scaledRotP.Y() << " translate" << endl;
  *itsOutFile << itsRotatingAngle << " rotate" << endl;
  *itsOutFile << -scaledRotP.X() << " " << -scaledRotP.Y() << " translate" << endl;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WriteEPSConcat(void)
{
  double xScale = itsWriteScale.GetXScaling();
  double yScale = itsWriteScale.GetYScaling();
  double xTrans = itsWriteScale.GetEndStartPoint().X() / xScale - itsWriteScale.GetStartStartPoint().X();
  double yTrans = itsWriteScale.GetEndStartPoint().Y() / yScale - itsWriteScale.GetStartStartPoint().Y();

  *itsOutFile << "gsave" << endl;
  *itsOutFile << "BeginEPSF" << endl;

  if(fRotBeforeScale)
	{
	  //näennäisesti väärässä järjestyksessä
	  *itsOutFile << xScale << " " << yScale << " scale" << endl;
	  *itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
	  if(itsRotatingAngle != 0.)  
		Rotate();
	}
  else
	{
	  //näennäisesti väärässä järjestyksessä
	  if(itsRotatingAngle != 0.)
		Rotate();
	  *itsOutFile << xScale << " " << yScale << " scale" << endl;
	  *itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
	}
  *itsOutFile << "%%BeginDocument: XXX.EPS" << endl;
  *itsOutFile << "%********* WriteEPSConcat tehty ******" << endl;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theClippingRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WriteEPSConcatClipping(NFmiRect theClippingRect)
{
  double xScale = itsWriteScale.GetXScaling();
  double yScale = itsWriteScale.GetYScaling();
  double xTrans = itsWriteScale.GetEndStartPoint().X() / xScale - itsWriteScale.GetStartStartPoint().X();
  double yTrans = itsWriteScale.GetEndStartPoint().Y() / yScale - itsWriteScale.GetStartStartPoint().Y();

  *itsOutFile << "%********* WriteEPSConcatClipping alkaa ******" << endl;
  *itsOutFile << "gsave" << endl;  //5.7
  
  //miksei rectclip käy suoraan ??
  *itsOutFile << theClippingRect.Left()
			  << " " << theClippingRect.Top()
			  << " " <<	"moveto"
			  << endl; 
  *itsOutFile << theClippingRect.Right()
			  << " " << theClippingRect.Top()
			  << " " << "lineto"
			  << endl; 
  *itsOutFile << theClippingRect.Right()
			  << " " << theClippingRect.Bottom()
			  << " " << "lineto"
			  << endl; 
  *itsOutFile << theClippingRect.Left()
			  << " " << theClippingRect.Bottom()
			  << " " << "lineto"
			  << endl; 
  
  *itsOutFile << "clip" << endl;
  *itsOutFile << "newpath" << endl;

  *itsOutFile << "BeginEPSF" << endl;

  if(itsRotatingAngle != 0.)
	Rotate();

  *itsOutFile << xScale << " " << yScale << " scale" << endl;
  *itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
  *itsOutFile << "%%BeginDocument: XXX.EPS" << endl;
  *itsOutFile << "%********* WriteEPSConcatClipping tehty ******" << endl;

  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theClippingRect Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WriteEPSConcatClipping(std::vector<NFmiPoint> thePoints)
{
  double xScale = itsWriteScale.GetXScaling();
  double yScale = itsWriteScale.GetYScaling();
  double xTrans = itsWriteScale.GetEndStartPoint().X() / xScale - itsWriteScale.GetStartStartPoint().X();
  double yTrans = itsWriteScale.GetEndStartPoint().Y() / yScale - itsWriteScale.GetStartStartPoint().Y();

  *itsOutFile << "%********* WriteEPSConcatClippingPath alkaa ******" << endl;
  *itsOutFile << "gsave" << endl;  //5.7

	std::vector<NFmiPoint>::iterator pos;

	NFmiString command("moveto");
	for(pos= thePoints.begin(); pos != thePoints.end(); ++pos)
	{
		*itsOutFile << (*pos).X()
			  << " " << (*pos).Y()
			  << " " <<	static_cast<char *>(command)
			  << endl;
		command = "lineto";
	}

  *itsOutFile << "clip" << endl;
  *itsOutFile << "newpath" << endl;

  *itsOutFile << "BeginEPSF" << endl;

  if(itsRotatingAngle != 0.)
	Rotate();

  *itsOutFile << xScale << " " << yScale << " scale" << endl;
  *itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
  *itsOutFile << "%%BeginDocument: XXX.EPS" << endl;
  *itsOutFile << "%********* WriteEPSConcatClippingPath tehty ******" << endl;

  return isTrue;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDirection Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WritePSConcatRotating(float theDirection)
{
  double xScale = itsWriteScale.GetXScaling();
  double yScale = itsWriteScale.GetYScaling();
  double xTrans = itsWriteScale.GetEndCenter().X();
  double yTrans = itsWriteScale.GetEndCenter().Y();

  *itsOutFile << "%********* WritePSConcatRotating alkaa ******" << endl;

  *itsOutFile << "gsave" << endl;
  *itsOutFile << "[" << xScale
			  << " " << 0
			  << " " << 0
			  << " " << yScale
			  << " " << xTrans
			  << " " << yTrans
			  << "] concat"
			  << endl;

  // määrittelysymboli näyttää länteen ja PS kiertää eri suuntaan kuin meteorologiassa
  *itsOutFile << (270.f - theDirection) << " rotate" << endl;

  *itsOutFile << "%********* WritePSConcatRotating tehty ******" << endl;

  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WritePSEnd(void)
{	   
  *itsOutFile << "grestore" << endl;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiPsWriting::WriteEPSEnd(void)
{	  
  *itsOutFile << "%%EndDocument" << endl;
  *itsOutFile << "EndEPSF" << endl;
  *itsOutFile << "grestore" << endl;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePressAlignment Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

NFmiString NFmiPsWriting::AlignmentToMeta(const FmiDirection & thePressAlignment) const
{
  //HUOM eps:ssä keskiviiva ja magics:ssä alareuna
  
  NFmiString retString;
  switch (thePressAlignment)
	{
	case kCenter:
	  {
		retString = NFmiString("South");
		break;
	  }
	case kRight:
	  {
		retString = NFmiString("SouthEast");
		break;
	  }
	case kLeft:
	  {
		retString = NFmiString("SouthWest");
		break;
	  }
	default:
	  break;
	}
  return retString;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 * \param mode Undocumented
 * \param os Undocumented
 */
// ----------------------------------------------------------------------

void NFmiPsWriting::WriteColor (const FmiGenericColor & theColor,
								FmiPressOutputMode mode,
								ofstream & os) const
{
  if(mode == kPostScript)
	{
	  if(theColor.isRgb)  
		{	
		  os << theColor.color.rgb.red << " "
			 << theColor.color.rgb.green << " "
			 << theColor.color.rgb.blue << " setrgbcolor"
			 << endl;
		}
	  else
		{
		  os << theColor.color.cmyk.cyan << " "
			 << theColor.color.cmyk.magenta << " "
			 << theColor.color.cmyk.yellow << " "
			 << theColor.color.cmyk.black << " setcmykcolor"
			 << endl;
		}
	}
  else
	{
	  // METAA
	}
}

// ======================================================================
