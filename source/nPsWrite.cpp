//© Ilmatieteenlaitos/Lasse.
// Original 12.2.1998
// kirjoitusfunktiot siirretty erillisistä funktioista tänne
// 
// 1.6.98/LW puskurinkoko suurennettu 130->520 kun Metview vaatii, ps-määrittelyn vastaisesti!
//Muutettu 180698/LW ps-puolelle korjattu kun symboli ei ala origosta
//Muutettu 261198/LW kierto-optio eps-kuviin
//Muutettu 150399/LW kierto myös ennen skaalausta
//Muutettu 011200/LW +copy-konstr.
//---------------------------------------------------------------------------

#include "nPsWrite.h"
#include "NFmiString.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//tehty vasta 1.12.2000---------------------------------------------------------------------------
NFmiPsWriting::NFmiPsWriting(const NFmiPsWriting& thePsWriting)
  : fWriteAsLast(thePsWriting.fWriteAsLast)
  , fRotBeforeScale(thePsWriting.fRotBeforeScale)   
  , itsRotatingPoint(thePsWriting.itsRotatingPoint)  
  , itsRotatingAngle(thePsWriting.itsRotatingAngle)  
  , fOverPrint(thePsWriting.fOverPrint)
  , itsInFile(thePsWriting.itsInFile ?
		new ifstream(*(ifstream*)thePsWriting.itsInFile) : 0)  //25.3.02
  , itsOutFile(thePsWriting.itsOutFile ?
		new ofstream(*(ofstream*)thePsWriting.itsOutFile) : 0)

//  , itsInFile(thePsWriting.itsInFile)
//  , itsOutFile(thePsWriting.itsOutFile)
  , itsWriteScale(thePsWriting.itsWriteScale)
{
};
//---------------------------------------------------------------------------
NFmiPsWriting::~NFmiPsWriting()
{
	if (itsInFile)    //25.3.02
		delete itsInFile;
};
//141298---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WriteOutString(const NFmiString& outString)
{
	// ostream saveCout = cout;
	// cout = itsOutFile;
	
	// itsOutFile << (char*) outString << endl;
	*itsOutFile << (char*) outString << endl;

	// cout = saveCout;

	return isTrue;
}
//141298---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WriteGRestore(void)
{
	// ostream saveCout = cout;
	// cout = itsOutFile;
	
	// itsOutFile << "grestore " << endl;
	*itsOutFile << "grestore " << endl;

	// cout = saveCout;

	return isTrue;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::CopyFile(void)
{
  const short lineSize = 520; //130; 1.6 Metview:n ps-driveri tuottaa 513 pitkiä riviä
  char inBuf[lineSize];       //     vaikka standardi puhuu 256:sta
  // Mika: operator= change
  // while (itsInFile.getline(inBuf, lineSize, '\n'))
  while (itsInFile->getline(inBuf, lineSize, '\n'))
  {
    // short num = itsInFile.gcount();
    short num = itsInFile->gcount();
    // itsOutFile.write(inBuf, num-1);	 
    // itsOutFile.put('\x0A');		  //jotta difference käytettävissä
    itsOutFile->write(inBuf, num-1);	 
    itsOutFile->put('\x0A');		  //jotta difference käytettävissä
  }
  return isTrue;
}
//--------------------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::CopyFileWithoutShowpage(void)
{ //lisätty showpagen poisto koska itse tuotteessa on se varsinainen showpage
  // kaksi showpagea saa jotkut ohjelmat tekemään animaation
  const short lineSize = 2800; //14.8  //1.6 Metview:n ps-driveri tuottaa 513 pitkiä rivejä	
//  unsigned char inBuf[lineSize];
  char inBuf[lineSize];
  unsigned short n, m, num;
  NFmiString notShowpage("gsave annotatepage grestore");
  NFmiString string;
  // while (itsInFile.getline(inBuf, lineSize, '\n'))
  while (itsInFile->getline(inBuf, lineSize, '\n'))
  {
	   // num = itsInFile.gcount();
	   num = itsInFile->gcount();
	   string.Set((unsigned char*)inBuf, num); //27.8.01
	   n = (short)string.Search( NFmiString("showp"));
	   if (n > 0 )
	   {
		   m = (short)string.Search( NFmiString("grestore showpage"));
		   if(m > 0)
				// itsOutFile.write(notShowpage.GetCharPtr(), notShowpage.GetLen());
				//HUOM 19.3.02: + (const char*)
				itsOutFile->write((const char*)(notShowpage.GetCharPtr()), notShowpage.GetLen());
		   else
			   	// itsOutFile.write(inBuf, num-1);
			   	itsOutFile->write(inBuf, num-1);

	   }
	   else
	   {
		   // itsOutFile.write(inBuf, num-1);
		   itsOutFile->write(inBuf, num-1);
		}
	   // itsOutFile.put('\x0A');		  
	   itsOutFile->put('\x0A');		  
  }
  return isTrue;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WritePSConcat(void)
{
	   double xScale = itsWriteScale.GetXScaling();
	   double yScale = itsWriteScale.GetYScaling();
	   double xTrans = itsWriteScale.GetEndStartPoint().X() / xScale
		                     - itsWriteScale.GetStartStartPoint().X(); //18.6;
	   double yTrans = itsWriteScale.GetEndStartPoint().Y() / yScale
		                     - itsWriteScale.GetStartStartPoint().Y(); //18.6;
	   // ostream saveCout = cout;
	   // cout = itsOutFile;
	   *itsOutFile << "%********* WritePSConcat alkaa ******" << endl;

	   // itsOutFile << "gsave" << endl;
	   // itsOutFile << xScale << " " << yScale << " scale" << endl;
	   // itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
	   // itsOutFile << "%********* WritePSConcat tehty ******" << endl;
	   // itsOutFile << "%********* WritePSConcat alkaa ******" << endl;
	   *itsOutFile << "gsave" << endl;
	   *itsOutFile << xScale << " " << yScale << " scale" << endl;
	   *itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
	   *itsOutFile << "%********* WritePSConcat tehty ******" << endl;

	   // cout = saveCout;
	   // itsOutFile << "AAAAAAAAAAAAAA" << endl;  //kaatuu
	   return isTrue;
}
//261198---------------------------------------------------------------------------
void NFmiPsWriting::Rotate(void)
{   //tässä pitää edetä ikäänkuin käänteisessä järjestyksessä
    NFmiPoint scaledRotP = itsWriteScale.Scale(itsRotatingPoint);
	if(fRotBeforeScale)  //150399
		scaledRotP = itsRotatingPoint;
	*itsOutFile << scaledRotP.X() << " " << scaledRotP.Y() << " translate" << endl;
	*itsOutFile << itsRotatingAngle << " rotate" << endl;
	*itsOutFile << -scaledRotP.X() << " " << -scaledRotP.Y() << " translate" << endl;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WriteEPSConcat(void)
{
	    double xScale = itsWriteScale.GetXScaling();
	    double yScale = itsWriteScale.GetYScaling();
		double xTrans = itsWriteScale.GetEndStartPoint().X() / xScale - itsWriteScale.GetStartStartPoint().X(); //2.12
		double yTrans = itsWriteScale.GetEndStartPoint().Y() / yScale - itsWriteScale.GetStartStartPoint().Y();
		// ostream saveCout = cout;
		// cout = itsOutFile;

		// itsOutFile << "%********* WriteEPSConcat alkaa ******" << endl;
		// itsOutFile << "gsave" << endl;
		// itsOutFile << "BeginEPSF" << endl;
		*itsOutFile << "%********* WriteEPSConcat alkaa ******" << endl;
		*itsOutFile << "gsave" << endl;
		*itsOutFile << "BeginEPSF" << endl;

		if(fRotBeforeScale)   //150399
		{        //näennäisesti väärässä järjestyksessä
			// itsOutFile << xScale << " " << yScale << " scale" << endl;
			// itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
			*itsOutFile << xScale << " " << yScale << " scale" << endl;
			*itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
			if(itsRotatingAngle != 0.)  
				Rotate();
		}
		else
		{     //näennäisesti väärässä järjestyksessä
			if(itsRotatingAngle != 0.)  //261198
				Rotate();
			// itsOutFile << xScale << " " << yScale << " scale" << endl;
			// itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
			*itsOutFile << xScale << " " << yScale << " scale" << endl;
			*itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
		}
		// itsOutFile << "%%BeginDocument: XXX.EPS" << endl;
		// itsOutFile << "%********* WriteEPSConcat tehty ******" << endl;
		*itsOutFile << "%%BeginDocument: XXX.EPS" << endl;
		*itsOutFile << "%********* WriteEPSConcat tehty ******" << endl;
		// cout = saveCout;
		return isTrue;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WriteEPSConcatClipping(NFmiRect theClippingRect)
{//2.12
	   double xScale = itsWriteScale.GetXScaling();
	   double yScale = itsWriteScale.GetYScaling();
		double xTrans = itsWriteScale.GetEndStartPoint().X() / xScale - itsWriteScale.GetStartStartPoint().X(); //2.12
		double yTrans = itsWriteScale.GetEndStartPoint().Y() / yScale - itsWriteScale.GetStartStartPoint().Y();
		// ostream saveCout = cout;
		// cout = itsOutFile;

		// itsOutFile << "%********* WriteEPSConcatClipping alkaa ******" << endl;
		// itsOutFile << "gsave" << endl;  //5.7
		*itsOutFile << "%********* WriteEPSConcatClipping alkaa ******" << endl;
		*itsOutFile << "gsave" << endl;  //5.7

		//miksei rectclip käy suoraan ??
//5.7		itsOutFile << "newpath" << endl;

		// itsOutFile << theClippingRect.Left() << " " << theClippingRect.Top() << " " << "moveto" << endl; 
		// itsOutFile << theClippingRect.Right() << " " << theClippingRect.Top() << " " << "lineto" << endl; 
		// itsOutFile << theClippingRect.Right() << " " << theClippingRect.Bottom() << " " << "lineto" << endl; 
		// itsOutFile << theClippingRect.Left() << " " << theClippingRect.Bottom() << " " << "lineto" << endl; 
		*itsOutFile << theClippingRect.Left() << " " << theClippingRect.Top() << " " << "moveto" << endl; 
		*itsOutFile << theClippingRect.Right() << " " << theClippingRect.Top() << " " << "lineto" << endl; 
		*itsOutFile << theClippingRect.Right() << " " << theClippingRect.Bottom() << " " << "lineto" << endl; 
		*itsOutFile << theClippingRect.Left() << " " << theClippingRect.Bottom() << " " << "lineto" << endl; 


//5.7		itsOutFile << "closepath" << endl;

		// itsOutFile << "clip" << endl;
		// itsOutFile << "newpath" << endl;
		*itsOutFile << "clip" << endl;
		*itsOutFile << "newpath" << endl;

		// itsOutFile << "BeginEPSF" << endl;
		*itsOutFile << "BeginEPSF" << endl;

		if(itsRotatingAngle != 0.)  //261198
			Rotate();

		// itsOutFile << xScale << " " << yScale << " scale" << endl;
		// itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
		// itsOutFile << "%%BeginDocument: XXX.EPS" << endl;
		// itsOutFile << "%********* WriteEPSConcatClipping tehty ******" << endl;
		*itsOutFile << xScale << " " << yScale << " scale" << endl;
		*itsOutFile << xTrans << " " << yTrans  << " translate" << endl;
		*itsOutFile << "%%BeginDocument: XXX.EPS" << endl;
		*itsOutFile << "%********* WriteEPSConcatClipping tehty ******" << endl;

		// cout = saveCout;
		return isTrue;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WritePSConcatRotating(float theDirection)
{
	   
	   double xScale = itsWriteScale.GetXScaling();
	   double yScale = itsWriteScale.GetYScaling();
		double xTrans = itsWriteScale.GetEndCenter().X();
		double yTrans = itsWriteScale.GetEndCenter().Y();
//		const double radConv = .0174533; // = 2 * pii / 360
//		double sinus =  sin(theDirection * radConv);
		// ostream saveCout = cout;
		*itsOutFile << "%********* WritePSConcatRotating alkaa ******" << endl;
		// cout = itsOutFile;

		// itsOutFile << "gsave" << endl;
		// itsOutFile << "[" << xScale << " " << 0 << " " << 0 << " " 
		// 	  << yScale << " " << xTrans << " " << yTrans << "] concat" << endl;
		// itsOutFile << (270.f - theDirection) << " rotate" << endl;//1012		//määrittelysymboli näyttää länteen

		*itsOutFile << "gsave" << endl;
		*itsOutFile << "[" << xScale << " " << 0 << " " << 0 << " " 
			    << yScale << " " << xTrans << " " << yTrans << "] concat" << endl;
		*itsOutFile << (270.f - theDirection) << " rotate" << endl;//1012		//määrittelysymboli näyttää länteen

		                                                      //ja PS kiertää eri suuntaan kuin meteorologiassa
/*		*itsOutFile << xScale << " " << yScale << " scale" << endl;
		*itsOutFile << xTrans << " " << yTrans  << " translate" << endl;  */


		// itsOutFile << "%********* WritePSConcatRotating tehty ******" << endl;
		*itsOutFile << "%********* WritePSConcatRotating tehty ******" << endl;

		// cout = saveCout;
		return isTrue;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WritePSEnd(void)
{	   
		// ostream saveCout = cout;
		// cout = itsOutFile;

		// itsOutFile << "grestore" << endl;
		*itsOutFile << "grestore" << endl;

		// cout = saveCout;
		return isTrue;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiPsWriting::WriteEPSEnd(void)
{	  
		// ostream saveCout = cout;
		// cout = itsOutFile;

		// itsOutFile << "%%EndDocument" << endl;
		// itsOutFile << "EndEPSF" << endl;
		// itsOutFile << "grestore" << endl;  //5.7
		*itsOutFile << "%%EndDocument" << endl;
		*itsOutFile << "EndEPSF" << endl;
		*itsOutFile << "grestore" << endl;  //5.7

		// cout = saveCout;
		return isTrue;
}
//---------------------------------------------------------------------------
NFmiString NFmiPsWriting::AlignmentToMeta (const FmiDirection& thePressAlignment)const  //24.10.00
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
	}
	return retString;
}
//---------------------------------------------------------------------------
void NFmiPsWriting::WriteColor (const FmiGenericColor& theColor, FmiPressOutputMode mode, ofstream & os)const  //3.11.00
{
	if(mode == kPostScript)
	{
		if(theColor.isRgb)  
		{	
			os << theColor.color.rgb.red << " "
			   <<theColor.color.rgb.green << " "
			   <<theColor.color.rgb.blue << " setrgbcolor" << endl;
		}
		else
		{
			os << theColor.color.cmyk.cyan << " "
			   << theColor.color.cmyk.magenta << " "
			   << theColor.color.cmyk.yellow << " "
			   << theColor.color.cmyk.black << " setcmykcolor" << endl;
		}
	}
	else
	{
		// METAA
	}
}
