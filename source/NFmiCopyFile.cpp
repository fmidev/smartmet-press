//© Ilmatieteenlaitos  Lasse
//  Original 30.10. 1997
// T‰ss‰ on funktioita joita voi k‰ytt‰‰ lehtikarttojen koostamiseen
// avaukset, sulkemiset ym on hoidettava kutsuvissa ohjelmissa
// 15.5.98 +NFmiCopyFileCropping()
// 1.6.98/LW puskurinkoko suurennettu 130->520 kun Metview vaatii, ps-m‰‰rittelyn vastaisesti!

/* HUOM samat rutiinit NFmiPressWrite-luokalla
   pit‰isi yhdist‰‰, mutta ainakin PressProduct tarttee viel‰ n‰it‰ */

#include <iostream> //STD 27.8.01
#include <fstream> //STD 27.8.01
#include <math.h>
#include "NFmiGlobals.h"
#include "NFmiRectScale.h"
#include "NFmiValueString.h"
#include "NFmiCmykColorBag.h"

using namespace std; //27.8.01

//--------------------------------------------------------------------------------------
bool NFmiCopyFile(ifstream& inFile, ofstream& outFile)
{
	const short lineSize = 2800;//130  1.6 Metview:n ps-driveri tuottaa 513 pitki‰ rivi‰
  char inBuf[lineSize];           //  vaikka standardi puhuu 256:sta
//  inFile.rewind();	 ei ole
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
	   short numToWrite = inFile.gcount()-1;
	   if(inBuf[numToWrite-1] == '\x0D')   //22.3.02 miten aikaisemmin j‰i itsest‰‰n pois???
		 numToWrite--;

	   outFile.write(inBuf, numToWrite);	 
	   outFile.put('\x0A');		  //jotta difference k‰ytett‰viss‰
  }
  return isTrue;
}
//--------------------------------------------------------------------------------------
bool NFmiCopyFileWithoutShowpage(ifstream& inFile, ofstream& outFile)
{ //lis‰tty showpagen poisto koska itse tuotteessa on se varsinainen showpage
  // kaksi showpagea saa jotkut ohjelmat tekem‰‰n animaation

  // 5.11.99 TARVITAANKO ENƒƒ KUN STARTEISSA AKTIVOITU SHOWPAGE{}

  const short lineSize = 2800; //14.8  //1.6 Metview:n ps-driveri tuottaa 513 pitki‰ rivej‰	
//  unsigned char inBuf[lineSize];
   char inBuf[lineSize];  //27.8.01
  unsigned short n, m, num;
  NFmiString notShowpage("gsave annotatepage grestore");
  NFmiString string;
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
	   num = inFile.gcount();
	   string.Set((unsigned char*)inBuf, num);
	   n = (short)string.Search( NFmiString("showp"));
	   if (n > 0 )
	   {
		   m = (short)string.Search( NFmiString("grestore showpage"));
		   if(m > 0)
		                // Mika: GetCharPtr() takaisin; Lasse otti Get:n pois
				outFile.write(notShowpage./*Get*/CharPtr(), notShowpage.GetLen());
		   else
			   	outFile.write(inBuf, num-1);

	   }
	   else
	   {
		   outFile.write(inBuf, num-1);
		}
	   outFile.put('\x0A');		  
  }
  return isTrue;
}
//--------------------------------------------------------------------------------------
//10.6  //18.6 t‰m‰ funktio p‰‰tetty tehd‰ Perlill‰ joten toistaiseksi hyˆdytˆn
        // eik‰ lopullisesti testattu
bool NFmiCopyFileColoring(ifstream& inFile, ofstream& outFile, NFmiCmykColorBag*)
{
  const short lineSize = 2800;//130  1.6 Metview:n ps-driveri tuottaa 513 pitki‰ rivi‰
  char inBuf[lineSize];           //  vaikka standardi puhuu 256:sta
  short nColor;
  NFmiString ownColor("LampoVari");
  NFmiString proc2("%%");
  NFmiString zeroX("0 x");
  NFmiString pc("Pc");
  NFmiString string;
  short nProc2=0;
  short nZeroX=0;
  short nPc=0;
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
	short num = inFile.gcount();
//    string.Set(inBuf, num-1);
	nColor = (short)string.Search(ownColor);
	if (nColor > 0)
	{
	   if (string.Search(proc2))
	   {
		   nProc2++;
	   }
	   if (string.Search(zeroX))
	   {
		   nZeroX++;
	   }
	   if (string.Search(pc))
	   {
		   nPc++;
	   }
	   outFile.write(inBuf, num-1);	 
	}
	else
	{
	   outFile.write(inBuf, num-1);
	}
	outFile.put('\x0A');		  //jotta difference k‰ytett‰viss‰
  }
  return isTrue;
}

//--------------------------------------------------------------------------------------
//10.6  //18.6 t‰m‰ funktio p‰‰tetty tehd‰ Perlill‰ joten toistaiseksi hyˆdytˆn
        // eik‰ lopullisesti testattu
bool NFmiCopyFileCroppingAndColoring(ifstream& inFile, ofstream& outFile
								,NFmiRect theRect, NFmiCmykColorBag* colorBag)
{
  const short lineSize = 2800; //1.6 Metview:n ps-driveri tuottaa 513 pitki‰ rivej‰	
//  unsigned char inBuf[lineSize];
  char inBuf[lineSize]; //27.8.01
//  unsigned char inBuf2[lineSize];
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
  NFmiString string, string2, hString, colorName;
  short nProc2=0;
  short nZeroX=0;
  short nPc=0;
 
  inFile.getline(inBuf, lineSize, '\n');
  num = inFile.gcount();
  string.Set((unsigned char*)inBuf, num-1);
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
	   num = inFile.gcount();
	   string2.Set((unsigned char*)inBuf, num-1);
	   if(!BBFound)   //vain kerran startTiedostoon, epseihin pit‰‰ erikseen
	   {
		   nSlash = (short)string2.Search(bounding); //NFmiString("%%BoundingBox:"));  //15.6
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
			   string2 = NFmiString(newBounding); 
		   }
	   }
	   else  //riitt‰‰ kun aloitetaan vasta BoundBoxin j‰lkeen 
	   {
		    nColor = (short)string2.Search(ownColor);
		    if (nColor > 0)
		    {
			   hString = NFmiString(string2);
			   colorName = string2.GetChars(nColor, 13);
			   if (string2.Search(proc2))
			   { 
				   if(!string2.Search(proc2par) && !string2.Search(customColors))
				   {
					   nProc2++;
					   if(string2.Search(customColors))
							hString = NFmiString("%%CMYKCustomColor: ");
					   else
							hString = NFmiString("%%+ "); 
					   hString += colorBag->GetColorString(colorName);
					   hString += NFmiString("(");
					   hString += colorName;
					   hString += NFmiString(")");
				   }
				   
			   }
			   if (string2.Search(zeroX))
			   {
				   nZeroX++;
				   if(string2.Search(f))
				      hString = NFmiString("f "); // ON PELKƒƒLLƒ CR:LLƒ EROTETTU !!
				   else
                      hString = NFmiString("");
				   hString += colorBag->GetColorString(colorName);
				   hString += NFmiString("(");
				   hString += colorName;
				   hString += NFmiString(") 0 x");
				   
			   }
			   if (string2.Search(pc))
			   {
				   nPc++;         // \12 oli alkupaletissa
				                  // ja iltiksess‰ v‰h‰n erilainen
				   string = colorBag->GetColorString(colorName); //HUOM ed. rivi
				   string += NFmiString("k");
			   }
			   string2 = NFmiString(hString);
		   }
	   }
	   // Mika: GetCharPtr takaisin; Lasse otti Get:n pois mutta castaus on kyll‰ Stringill‰
	  outFile.write(string./*Get*/CharPtr(), string.GetLen());
	  string = NFmiString(string2);
	  outFile.put('\x0A');		  
  }
  // Mika: GetCharPtr8) p‰‰lle; Lasse otti Get:n pois
  outFile.write(string./*Get*/CharPtr(), newBounding.GetLen()); //viel‰ viimeinen
  outFile.put('\x0A');		  
  return isTrue;
}
//23.11.01--------------------------------------------------------------------------------------
bool NFmiWriteBoundingBox(ofstream& outFile
								,NFmiRect theRect)
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
	// Mika: GetCharPtr() p‰‰lle; Lasse otti Get:n pois
	outFile.write(newBounding./*Get*/CharPtr(), newBounding.GetLen());	   
	outFile.put('\x0A');		  
	outFile.put('\x0A');		  
    return true;
}
//--------------------------------------------------------------------------------------
bool NFmiCopyFileCropping(ifstream& inFile, ofstream& outFile
								,NFmiRect theRect)
{ 
  const short lineSize = 2800; //14.8  //1.6 Metview:n ps-driveri tuottaa 513 pitki‰ rivej‰	
//  unsigned char inBuf[lineSize];
  char inBuf[lineSize]; //27.8.01
  unsigned short nSlash, num;
  bool BBFound = false;
  NFmiString newBounding("%%BoundingBox: ");
  NFmiString string;
  while (inFile.getline(inBuf, lineSize, '\n'))
  {
	   num = inFile.gcount();
	   if(!BBFound)   //vain kerran startTiedostoon, epseihin pit‰‰ erikseen
	   {
		   string.Set((unsigned char*)inBuf, num);
		   nSlash = (short)string.Search( NFmiString("%%BoundingBox:"));
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
			   // Mika: GetCharPtr8) takaisin; Lasse otti Get:n pois
			   outFile.write(newBounding./*Get*/CharPtr(), newBounding.GetLen());
		   }
		   else
			   outFile.write(inBuf, num-1);
	   }
	   else
	   {
			outFile.write(inBuf, num-1);
	   }
	   outFile.put('\x0A');		  
  }
  return isTrue;
}
//--------------------------------------------------------------------------------------
bool NFmiWritePSConcat(NFmiRectScale theScale, ofstream& outFile)
{
	   double xScale = theScale.GetXScaling();
	   double yScale = theScale.GetYScaling();
	   double xTrans = theScale.GetEndStartPoint().X() / xScale;
	   double yTrans = theScale.GetEndStartPoint().Y() / yScale;
	   // Mika: Ei k‰ytet‰ saveCouttia
	   // ostream saveCout = cout;
	   // cout = outFile;
	   outFile << "gsave" << endl;
	   outFile << xScale << " " << yScale << " scale" << endl;
	   outFile << xTrans << " " << yTrans  << " translate" << endl;
	   // cout = saveCout;
	   // cout << "AAAAAAAAAAAAAA" << endl;  //kaatuu
		return isTrue;
}
//--------------------------------------------------------------------------------------
bool NFmiWriteEPSConcat(NFmiRectScale theScale, ofstream& outFile)
{
	   double xScale = theScale.GetXScaling();
	   double yScale = theScale.GetYScaling();
		double xTrans = theScale.GetEndStartPoint().X() / xScale - theScale.GetStartStartPoint().X(); //2.12
		double yTrans = theScale.GetEndStartPoint().Y() / yScale - theScale.GetStartStartPoint().Y();
		// Mika: Ei k‰ytet‰ saveCouttia
		// ostream saveCout = cout;
		// cout = outFile;
		// outFile << "gsave" << endl;
		outFile << "BeginEPSF" << endl;
		outFile << xScale << " " << yScale << " scale" << endl;
		outFile << xTrans << " " << yTrans  << " translate" << endl;
		outFile << "%%BeginDocument: XXX.EPS" << endl;
		// cout = saveCout;
		return isTrue;
}
//--------------------------------------------------------------------------------------
bool NFmiWriteEPSConcatClipping(NFmiRectScale theScale, ofstream& outFile, NFmiRect theClippingRect)
{//2.12
	   double xScale = theScale.GetXScaling();
	   double yScale = theScale.GetYScaling();
		double xTrans = theScale.GetEndStartPoint().X() / xScale - theScale.GetStartStartPoint().X(); //2.12
		double yTrans = theScale.GetEndStartPoint().Y() / yScale - theScale.GetStartStartPoint().Y();
		// Mika: Onpas n‰it‰..
		// ostream saveCout = cout;
		// cout = outFile;
		//miksei rectclip k‰y suoraan ??
		outFile << "newpath" << endl;
		outFile << theClippingRect.Left() << " " << theClippingRect.Top() << " " << "moveto" << endl; 
		outFile << theClippingRect.Right() << " " << theClippingRect.Top() << " " << "lineto" << endl; 
		outFile << theClippingRect.Right() << " " << theClippingRect.Bottom() << " " << "lineto" << endl; 
		outFile << theClippingRect.Left() << " " << theClippingRect.Bottom() << " " << "lineto" << endl; 
		outFile << "closepath" << endl;
		outFile << "clip" << endl;
		outFile << "newpath" << endl;

		outFile << "BeginEPSF" << endl;
		outFile << xScale << " " << yScale << " scale" << endl;
		outFile << xTrans << " " << yTrans  << " translate" << endl;
		outFile << "%%BeginDocument: XXX.EPS" << endl;
		// cout = saveCout;
		return isTrue;
}
//--------------------------------------------------------------------------------------
bool NFmiWritePSConcatRotating(NFmiRectScale theScale, float theDirection, ofstream& outFile)
{
	   
	    double xScale = theScale.GetXScaling();
	    double yScale = theScale.GetYScaling();
		double xTrans = theScale.GetEndCenter().X();
		double yTrans = theScale.GetEndCenter().Y();
//		const double radConv = .0174533; // = 2 * pii / 360
//		double sinus =  sin(theDirection * radConv);
		// Mika: No jo on..
		// ostream saveCout = cout;
		// cout = outFile;
		outFile << "gsave" << endl;
		outFile << "[" << xScale << " " << 0 << " " << 0 << " " 
			  << yScale << " " << xTrans << " " << yTrans << "] concat" << endl;
		outFile << (270.f - theDirection) << " rotate" << endl;//1012		//m‰‰rittelysymboli n‰ytt‰‰ l‰nteen
		                                                      //ja PS kiert‰‰ eri suuntaan kuin meteorologiassa
/*		outFile << xScale << " " << yScale << " scale" << endl;
		outFile << xTrans << " " << yTrans  << " translate" << endl;  */
		// cout = saveCout;
		return isTrue;
}
//--------------------------------------------------------------------------------------
bool NFmiWritePSEnd(ofstream& outFile)
{	  
		// Mika: Perkele
		// ostream saveCout = cout;
		// cout = outFile;
		outFile << "grestore" << endl;
		// cout = saveCout;
		return isTrue;
}
//--------------------------------------------------------------------------------------
bool NFmiWriteEPSEnd(ofstream& outFile)
{	  
		// Mika: Lasse hirteen!
		// ostream saveCout = cout;
		// cout = outFile;
		outFile << "%%EndDocument" << endl;
		outFile << "EndEPSF" << endl;
		// cout << "grestore" << endl;
		// cout = saveCout;
		return isTrue;
}
