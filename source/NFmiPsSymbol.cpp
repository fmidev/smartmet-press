//© Ilmatieteenlaitos/Salla.
//  Original 12.2.1998
// 
// Muutettu 060398/LW tästä erotettu n. puolet uuteen luokkaan NFmiPressScaling 
//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiPsSymbol.h"
#include "NFmiFileString.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiPsSymbol::NFmiPsSymbol(const NFmiPsSymbol& thePsSymbol)
: NFmiPressScaling() 

, itsShortDir(NFmiString(thePsSymbol.itsShortDir))
, itsOrigDir(NFmiString(thePsSymbol.itsOrigDir))       
//, itsRectScale(thePsSymbol.itsRectScale)
//, itsRectSize(thePsSymbol.itsRectSize)
//, itsRelArea(thePsSymbol.itsRelArea)
{
};

//---------------------------------------------------------------------------
NFmiPsSymbol::~NFmiPsSymbol() 
{
};
//---------------------------------------------------------------------------
NFmiPsSymbol* NFmiPsSymbol::Clone() const
{
	return (NFmiPsSymbol *) new NFmiPsSymbol(*this);
};
/*
//---------------------------------------------------------------------------
bool NFmiPsSymbol::Set( NFmiRectScale &theScale
							 , ofstream theDestinationFile)	
{
//	itsDefToProductScale = theScale;	
	itsRectScale = theScale;	
	itsOutFile = theDestinationFile;
	return true;
};
*/
/* 25.9 pois
//----------------------------------------------------------------------------
NFmiString NFmiPsSymbol::SymbolSetName(void) const
{//23.2  Tällainen metodi pitäisi olla FileStringillä 
	short lastSlash = itsOrigDir.SearchLast(NFmiString("/"));
	return itsOrigDir.GetChars(lastSlash+1, itsOrigDir.GetLen()-lastSlash);
}
*/
//----------------------------------------------------------------------------
bool NFmiPsSymbol::CopyShortSymbol2Dest(void)//const NFmiString& symbolFile)
{//10.2
		NFmiString fileName = itsShortDir;
//		fileName += SymbolSetName();
		fileName += NFmiFileString(itsOrigDir).Directory(); //25.9
		fileName += NFmiString("_");     
	    fileName += itsSymbol;
		fileName += NFmiString(".ps");

		// itsInFile.open(fileName, ios::in|ios::in);
        // if(itsInFile.good() && !itsInFile.eof())  
		itsInFile->open(fileName, ios::in|ios::in);
        if(itsInFile->good() && !itsInFile->eof())  
        {
		   bool tempBool = WritePSConcat();
		   tempBool = CopyFile();
		   tempBool = WritePSEnd();
           // itsInFile.close();
           // itsInFile.clear();
           itsInFile->close();
           itsInFile->clear();
		   return isTrue;
		}
		else
            // itsInFile.close();
            // itsInFile.clear();
            itsInFile->close();
            itsInFile->clear();
			return isFalse;
}
//----------------------------------------------------------------------------
bool NFmiPsSymbol::ConvertOrig2Short(void)//const NFmiString& symbolFile) 
{ //10.2
// VAIN WINDOWSILLA TEHDYILLE SYMBOLEILLE

   NFmiString inputName = itsOrigDir;
   inputName += NFmiString("/");
   inputName += itsSymbol;
   inputName += NFmiString(".ai");
   ifstream input(inputName, ios::in);

   if(input.good() && !input.eof())  
   {
      NFmiString outputName = itsShortDir;
 //     outputName += SymbolSetName();
 	  outputName += NFmiFileString(itsOrigDir).Directory(); //25.9
      outputName += NFmiString("_");
      outputName += itsSymbol;
      outputName += NFmiString(".ps");
      ofstream output(outputName, ios::out);

      const short lineSize = 250;	//27.11 HUOM max rivipituus
      char inBuf[lineSize];
      bool copyOn = isFalse;  //6.2
      unsigned long copiedLines = 0;
      NFmiString mess = NFmiString ("%****** ");
      mess += itsSymbol;
      mess += NFmiString (" ALKAA ********************"); 
      output.write(mess.CharPtr(),40);
      output.write((char*)NFmiString("\n"),1); //6.2
      short num;
      char endLine = '\n';  
      while (input.getline(inBuf, lineSize, endLine)) //newline PC:lle
      {
	      NFmiString apu = NFmiString(inBuf);

	      if(apu.Search(NFmiString("%AI5_BeginLayer")) > 0 && copyOn)
//	      if(NFmiString(inBuf) == NFmiString("%AI5_BeginLayer") && copyOn)
	      {
		      if(itsLogFile)
			  *itsLogFile << "*** ERROR: Symbolin " << (char*)itsSymbol
                << "konvertointi epäonnistui, BeginLayer:ta, sisäkkäin" << endl;
		      copyOn = isTrue;
	      }
	      else if(apu.Search(NFmiString("%AI5_BeginLayer")) > 0
		      && apu.Search(NFmiString("%AI5_EndLayer")) < 1)  //6.2
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
          if(apu.Search(NFmiString("%AI5_EndLayer")) > 0 && copyOn)  //6.2
	      {
		     copyOn = isFalse;
	      }
      }
      mess = NFmiString ("%****** ");
      mess += itsSymbol;
      mess += NFmiString (" LOPPU ************************"); 
      output.write(mess.CharPtr(),40);
      output.write((char*)NFmiString("\n"),1);  //6.2
   
      if(itsLogFile)
         *itsLogFile << "  rivejä kopioitu: " << copiedLines << endl;  //29.4

	  output.close();
	  output.clear();
       //25.2 tämä tekee mahdolliseksi lukemisen saman ohjelman puitteissa ????? 
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

//----------------------------------------------------------------------------
bool NFmiPsSymbol::ReadDescription(NFmiString& retString)
{										 
   NFmiValueString valueString;
	NFmiPoint sizePoint1Size, sizePoint2Size;
	NFmiPoint sizePoint1NotSize, sizePoint2NotSize;
	sizePoint1Size.Set(0,0);   //Vanha
    sizePoint2Size.Set(c40,c40); //ksymbolgrupin oletus on myös tämä -> skaala 1 : 1
	double c20 = c40/2.;
	sizePoint1NotSize.Set(-c20,-c20);   //Uusi, 30.9
    sizePoint2NotSize.Set(c20,c20); //ksymbolgrupin oletus on myös tämä -> skaala 1 : 1
/*					                
   char *polku = getenv("lehtiTuoteDir");
   if(!polku)
   {
	  if(itsLogFile)
		  *itsLogFile << "*** ERROR: lehtiTuoteDir ei määritelty" << endl;	 
	  return isFalse;
   }
 	NFmiString inDir = NFmiString(polku) ;
	inDir += NFmiString("/LyhytSymbolit/");  
 	NFmiString subDir = NFmiString("Massa");  //oletus vaikka tämä
	itsOrigDir = NFmiString(polku) ;
*/
    NFmiString inDir;
/*	   if(kLehtiTuoteDir == 0)     //23.9
	   {
		   inDir = NFmiString(..) + kFmiDirectorySeparator + NFmiString("LyhytSymbolit") + kFmiDirectorySeparator;   
		   itsOrigDir = NFmiString(..) + kFmiDirectorySeparator + NFmiString("Symbolit") + kFmiDirectorySeparator;   
	   }
	   else
	   {*/
		   inDir = GetHome();
		   inDir += kFmiDirectorySeparator;
		   inDir += "LyhytSymbolit";
		   inDir += kFmiDirectorySeparator;
		   itsOrigDir = GetHome();
		   itsOrigDir += kFmiDirectorySeparator;
		   itsOrigDir += "Symbolit";
		   itsOrigDir += kFmiDirectorySeparator;
//	   }	   

 	NFmiString subDir = NFmiString("Massa");  //oletus vaikka tämä

	NFmiString  helpString;
	NFmiValueString helpValueString;

//	char object[255];// ch;
	*itsDescriptionFile >> itsObject;
	itsString = itsObject;
	itsIntObject = ConvertDefText(itsString);
//	long long1,long2;
	double r1,r2,r3,r4;
//	short s1,s2;
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
					*itsLogFile << "*** ERROR: Tuntematon sana #VakioSymbolissa: " << (char*)itsObject << endl;  
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

		case dPsRelSize:  //29.9
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
				  y = (double)valueString;
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
		case dSymbolPlace:   //11.3
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
//			*itsDescriptionFile >> itsObject;
//			subDir = itsObject;
			subDir = ReadString();

			ReadNext();
			break;
		}
		case dConstantSymbol:
		{
			if (!ReadEqualChar())
				break;
//			*itsDescriptionFile >> itsObject;
//			itsSymbol = itsObject;
			itsSymbol = ReadString();

			ReadNext();
			break;
		}
		case dSymbolSize:	  		 //23.2
		{
			if (!ReadEqualChar())
				break;
 
				if(ReadDouble(x))      //26.2 
				{				
				   *itsDescriptionFile >> itsObject;
			       valueString = itsObject;
				   if(valueString.IsNumeric())  
				   {
					  y = (double)valueString;
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

//				*itsDescriptionFile >> itsObject;
//				itsString = itsObject;
				itsIntObject = ConvertDefText(itsString);
				break;
		}
		case dRelSymbolSize:	  		 //24.2
		{
			if (!ReadEqualChar())
				break;
			if(Read4Double(xmin,ymin,xmax,ymax))
			{
               itsRelArea.Set(NFmiPoint(xmin,ymin),NFmiPoint(xmax,ymax));
//				   itsRectArea.ToAbs(itsRelArea);    //26.8
			}

			ReadNext();
			break;
		}
		default:  //240101
		{
			ReadRemaining();  
			break;
		}
	}
	} //while
	itsShortDir = NFmiString(inDir);	//oikeastaan koko polku
//	itsSymbolSetName = subDir;    //10.2
	itsOrigDir += subDir;
	itsOrigDir += kFmiDirectorySeparator;  //29.9

//** 7.1 SYMBOLIKOKO ON NYT OMASSA TIEDOSTOSSAAN JOKAISTA SETTIÄ VARTEN
// ei anneta enää määrittelyissä
	NFmiString sizeFile = itsOrigDir;            
    sizeFile += kFmiDirectorySeparator;     //
    sizeFile += NFmiString("symbolikoko.txt");   //kokoa ei tarvita ellei eps-file
    ifstream input(sizeFile, ios::in);
// 28.9	OletusSkaala 1->1 jos kokotiedosto ei annettu
//	itsDefToProductScale.SetEndScales(itsDefToProductScale.GetStartScales());
// skaala tuli potenssiin kaksi (13*13, 6*6)
   if(input.good() && !input.eof())  
    {

	// Read3Double ei kayta cin kanavaa - tarpeeton save
	// istream saveCin = cin;
	// cin = input;

 //      point.Set(0.,0.);
 //      point2.Set(100.,100.); 
	   if(Read4Double(r1,r2,r3,r4))  
/*	   {
		   point.Set(r1,r2);
		   point2.Set(r3,r4);

           itsWriteScale.SetStartScales(NFmiRect(point,point2));
	   }*/
	   {
		   if(r1 == r3)     //4.9 yksinkertaistettu yhteen lukuun
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
       // cin = saveCin;
	   itsWriteScale.SetStartScales(NFmiRect(sizePoint1Size,sizePoint2Size));
	}
	else 
	{//11.2
//	   if(itsLogFile)  //28.9
//		  *itsLogFile << "*** ERROR: SymboliSetin koko puuttuu: " << (char*)subDir << endl;  
		itsWriteScale.SetStartScales(NFmiRect(sizePoint1NotSize,sizePoint2NotSize));
        
	}

    input.close();
	
//**

	retString = itsString;
	return true;
}

//----------------------------------------------------------------------------
int NFmiPsSymbol::ConvertDefText(NFmiString & object)  
{
	if(object==NFmiString("Directory") || object==NFmiString("KuvaKansio")
		|| object==NFmiString("Kuvakansio")
		|| object==NFmiString("Hakemisto"))      //11.3
		return dSymbolDir;                        
	else if(object==NFmiString("Name") || object==NFmiString("Nimi")
		|| object==NFmiString("Tiedosto"))       //11.3
		return dConstantSymbol;                        
	else if(object==NFmiString("SizeFactor") || object==NFmiString("KokoKerroin")
		|| object==NFmiString("Kokokerroin"))
		return dPsRelSize;                          //29.9
/*11.3	else if(object==NFmiString("Size") || object==NFmiString("Koko"))  
		return dSymbolSize;                       
	else if(object==NFmiString("RelPlace") || object==NFmiString("OsaAlue")
		|| object==NFmiString("Osaalue")) 
		return dRelSymbolSize;
*/
	else
		return NFmiPressScaling::ConvertDefText(object); //11.3
}
//----------------------------------------------------------------------------
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
			  *itsLogFile << "Symboli " << (char*)itsSymbol << " konvertoitu" << endl;

		if (CopyShortSymbol2Dest()) //itsShortDir
		  {
			return isTrue;
		  }
		  else
		  {   if(itsLogFile)                               
			  *itsLogFile << "*** ERROR: Symbolia ei konvertoinnin jälkeen löydy: " << (char*)itsSymbol << endl;
			  return isFalse;
		  }
	   }
	   else
  		 if(itsLogFile)                               
			  *itsLogFile << "*** ERROR: Symbolia ei ole: " << (char*)itsSymbol << endl;
         return isFalse;
	}

}
//----------------------------------------------------------------------------
bool NFmiPsSymbol::WritePS(FmiPressOutputMode theOutput) //15.3.00
{
//	Place(place);
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
			      *itsLogFile << "Symboli " << (char*)itsSymbol << " konvertoitu" << endl;
 
			if (CopyShortSymbol2Dest()) //itsShortDir
		      {
				return isTrue;
		      }
		      else
		      {   if(itsLogFile)                               
			      *itsLogFile << "*** ERROR: Symbolia ei konvertoinnin jälkeen löydy: " << (char*)itsSymbol << endl;
				  return isFalse;
			  }
		   }
		   else
  		     if(itsLogFile)                               
			      *itsLogFile << "*** ERROR: Symbolia ei ole: " << (char*)itsSymbol << endl;
             return isFalse;
		}

}
