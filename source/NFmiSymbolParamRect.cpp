//© Ilmatieteenlaitos/Salla.
//  Original 23.10.1997
// 
//	Changed	18.11.1997	/Salla	Clone()
//								SymbolDefSize+ tarv.lisäykset
//  Changed 19.11.1997	/Salla	Copy Const.
// Muutettu 20.11.97/LW +ConvertDefString() (ent. irrallinen Convert)
// Muutettu 21.11.97/LW  NFmiSymbolParamRectObjects:ja siirretty emolle
// Muutettu 070198/LW Määrittelyt suomennettu
// Muutettu 150198/LW ReadDouble() ym käyttö
// Muutettu 220198/LW symbolimappaus myös yhdellä arvolla
// Muutettu 090298/LW unsigned short -> short
// Muutettu 100298/LW +itsOrigDir, +itsSymbolSetName
// Muutettu 060498/LW muunnoksessa huomioitu skaala-optio
// Muutettu 060498/LW kokotiedostoon riittää yksi luku
// Muutettu 130999/LW Muunnosarvot luetaan nyt double-tyyppisinä eikä integereinä
// Muutettu 290200/LW Copy-konstr. siivottu
// Muutettu 130300/LW makronTeko-optio (ps:n sijasta)
// Muutettu 240500/LW +fIsConstSymbol.
// Muutettu 171000/LW +itsYValueScaling ja X
//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiSymbolParamRect.h"
#include "NFmiRectScale.h"
#include "NFmiPsWriting.h"
#include "NFmiPressParam.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiSymbolParamRect::NFmiSymbolParamRect(const NFmiSymbolParamRect& theSymbolRect)
: NFmiParamRect(theSymbolRect)
//: NFmiParamRect(*(NFmiParamRect*)&theSymbolRect)
, itsConstSymbol(theSymbolRect.itsConstSymbol)
, fIsConstSymbol(theSymbolRect.fIsConstSymbol)
, itsMapping(new NFmiParamMapping(*theSymbolRect.itsMapping))
, itsSubDir(new NFmiString(*theSymbolRect.itsSubDir))
, itsOrigDir(new NFmiString(*theSymbolRect.itsOrigDir))       
, itsSymbolSetName(theSymbolRect.itsSymbolSetName)
, itsDefToProductScale(theSymbolRect.itsDefToProductScale)
//, itsXValueScaling(theSymbolRect.itsXValueScaling)  //17.10.00
//, itsYValueScaling(theSymbolRect.itsYValueScaling)  //17.10.00                               
{
};

//---------------------------------------------------------------------------
NFmiSymbolParamRect::~NFmiSymbolParamRect() 
{
	if(itsSubDir) 								 
		delete (NFmiString*)itsSubDir;
	if(itsMapping)
		delete (NFmiParamMapping*)itsMapping;
	if(itsOrigDir)
		delete (NFmiString*)itsOrigDir;;
		
};
//---------------------------------------------------------------------------
NFmiParamRect* NFmiSymbolParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiSymbolParamRect(*this);
};

//----------------------------------------------------------------------------
bool NFmiSymbolParamRect::ReadDescription(NFmiString& retString) 
{										 
					                
   double x,y;
 	NFmiValueString valueString;
 	if (itsOrigDir) delete itsOrigDir;
	NFmiString origDir;
    NFmiString inDir;

		   inDir = GetHome();
		   inDir += kFmiDirectorySeparator;
		   inDir += "LyhytSymbolit";
		   inDir += kFmiDirectorySeparator;
		   origDir = GetHome();
		   origDir += kFmiDirectorySeparator;
		   origDir += "Symbolit";
		   origDir += kFmiDirectorySeparator;

	NFmiString subDir = NFmiString("Massa");  //oletus vaikka tämä
	itsOrigDir = new NFmiString(origDir);	//oikeastaan koko polku

	NFmiString  helpString;
	NFmiString* mappingName;
	NFmiValueString helpValueString;
	itsMapping = new NFmiParamMapping;
	itsModifier = kNoneModifier; //15.2.00
	SetPreReadingTimes();  

	*itsDescriptionFile >> itsObject;
	itsString = itsObject;
	itsIntObject = ConvertDefText(itsString);

	long long1,long2;
	double r1,r2,r3,r4;
	double intervalEps = 0.00001; //13.9.99
	NFmiPoint sizePoint1, sizePoint2;
	sizePoint1.Set(0.,0.);
    sizePoint2.Set(c40,c40); //ksymbolgrupin oletus on myös tämä -> skaala 1 : 1

	itsRelRect.Set(NFmiPoint(0.,0.),NFmiPoint(1.,1.));
	itsSizeFactor = NFmiPoint(1. ,1.); //080199 RotParr tarttee
	if(fNewScaling)  //041298 onko asetettu, entäs rotparr jossa oli sijoitusvirhe
	                 // toimiiko vanha sijoitus
	   itsRelRect += NFmiPoint(-.5, -.5);

	itsMultiMapping = 0; //27.9.99

	while(itsIntObject != 9999 || itsCommentLevel) 
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; 
	if(itsLoopNum > itsMaxLoopNum)  //2.2
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
					*itsLogFile << "*** ERROR: Tuntematon sana #Symbolissa: " << (char*)itsObject << endl;  
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
		case dConstSymbolName:  //24.5.00
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

//			if(ReadLong(long1))  
			if(ReadDouble(r1))  //13.9.99
			{
            	*itsDescriptionFile >> itsObject;
				helpValueString = NFmiValueString(itsObject);
	            if(helpValueString.IsNumeric())
				{
					r2 = (double)helpValueString;      
                 	*itsDescriptionFile >> itsObject;
				}
				else
				{
					r2 = r1;
				}

               interval.itsBottomValue = r1 - intervalEps; //varmuuden vuoksi jotta vertailut oiken päin
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

//			if(ReadLong(long1) && ReadLong(long2))  
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

//			if(ReadDouble(x))      
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
				  y = (double)valueString;
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
				itsRelRect += NFmiPoint(.5, .5); //12.10

				itsSizeFactor = NFmiPoint(xh,yh); //080199
			}
			else
			{
				  *itsDescriptionFile >> itsObject;
				  itsString = itsObject;
			}
//			fNewScaling = true;  //30.9

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
//			if(ReadFour(r1,r2,r3,r4))   ei ole
			{
			   itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
			}

			ReadNext();
		
			break;
		}
		case dProducer: 
		{
//			SetOne(itsProducer); 
			SetOne(long1); //290200 luetaan vain pois
			break;
		}
/*		case dParam: 
		{
			SetOne(itsIdentPar); 
			break;
		}
		*/
		case dRelDay:     
		{
			SetOne(itsFirstDeltaDays);
			if(itsLogFile)
					*itsLogFile << "*** ERROR: Päiviä ei voi asettaa #Symbolissa"  << endl;  
			break;
		}
/*		case dRelYear:     //220199
		{
			if (!ReadEqualChar())
				break;

//			if(ReadLong(long1))    //15.1
//               itsFirstDeltaYears = (unsigned short)long1;

			ReadNext();
			if(itsLogFile)
					*itsLogFile << "*** ERROR: Vuosia ei voi asettaa #Symbolissa"  << endl;  
			break;
		}
		*/
		case dHour:       
		{
			SetOne(itsFirstPlotHours);
			break;
		}
		default:  //041198
		{
			ReadRemaining();  
			break;
		}
	}
	} //while
	
	if (itsSubDir) delete itsSubDir;
	itsSubDir = new NFmiString(inDir);	//oikeastaan koko polku
	itsSymbolSetName = subDir;    //10.2

//** 7.1 SYMBOLIKOKO ON NYT OMASSA TIEDOSTOSSAAN JOKAISTA SETTIÄ VARTEN
// MUTTA NYT JO VANHAA TEKNIIKKAA (kesä 88)
// ei anneta enää määrittelyissä
	NFmiString sizeFile = *itsOrigDir;            //11.2
 	sizeFile += itsSymbolSetName;
    sizeFile += NFmiString("/");
    sizeFile += NFmiString("symbolikoko.txt");    //9.1
    ifstream input(sizeFile, ios::in); //27.8.01

// 7.9	OletusSkaala 1->1 jos kokotiedosto ei annettu
//	itsDefToProductScale.SetEndScales(itsDefToProductScale.GetStartScales());
// skaala tuli potenssiin kaksi (13*13, 6*6)
 

	if(input.good() && !input.eof())  
    {

	// istream saveCin = cin;
	// cin = input;

	   if(Read4Double(r1,r2,r3,r4))
	   {
		   if(r1 == r3)     //4.9 yksinkertaistettu yhteen lukuun
			                // PITÄISI TEHDÄ PAREMMIN
		   {
			   sizePoint1.Set(0., 0.);  
			   sizePoint2.Set(r1,r1);
		   }
		   else

//		         if(input.good() && !input.eof())  EI PELAA
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
       // cin = saveCin;
	   fNewScaling = false;
	}
	else 
	{
	    fNewScaling = true;
	}

    itsDefToProductScale.SetStartScales(NFmiRect(sizePoint1,sizePoint2));

    input.close();
//**

 	SetPostReadingTimes();

	// oli Set(ident ja rect) ; jaettu kahteen osaan 1.2.99
	SetIdent(NFmiDataIdent(NFmiParam(itsIdentPar),240 /*joku vaan NFmiProducer(itsProducer)*/)); 
	                //1.2.99 ennen DoPostReading:iä, vain Ident tähän

	DoPostReading();  //tähän aikaisemmaksi

	if(fNewScaling)    // tämä DoPostReading:n jälkeen
	   itsRelRect += NFmiPoint(1.,1.);

	SetRect(NFmiRect(itsRelRect)); // vain Rect tähän


	retString = itsString;
	return true; 
}

//----------------------------------------------------------------------------
bool NFmiSymbolParamRect::ReadRemaining(void)  //161000
{

	switch(itsIntObject)
	{
		default: 
		{
			return NFmiParamRect::ReadRemaining();  
			break;
		}
	}
	return true;
}
/*
//181000----------------------------------------------------------------------------
bool NFmiSymbolParamRect::ReadSymbolScale(FmiValueScaling& theScale) //20.10.00
{
	if (SetThree(theScale.noneValue,theScale.symbolValue,theScale.maxValue))
	{
		if(theScale.noneValue == theScale.symbolValue)
		{
			*itsLogFile << "*** ERROR: SymboliSkaalan kaksi ekaa arvoa ei voi olla samoja"  << endl;
			theScale.symbolValue = kFloatMissing;
			return false;
		}
		return true;
	}
	return false;
}
*/
//----------------------------------------------------------------------------
int NFmiSymbolParamRect::ConvertDefText(NFmiString & object)  
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //16.10.00

	if(lowChar==NFmiString("symboldirectory") 
		|| lowChar==NFmiString("symbolitaulu")|| lowChar==NFmiString("kuvakansio"))
		return dSymbolDirectory;
	else if(lowChar==NFmiString("sizefactor") || lowChar==NFmiString("kokokerroin"))
		return dRelSize;
	else if(lowChar==NFmiString("mapping") 
		|| lowChar==NFmiString("arvoistasymboli")|| lowChar==NFmiString("muunnos")) 
		return dMapping;
	else if(lowChar==NFmiString("scaledsymbol")|| lowChar==NFmiString("skaalamuunnos"))
		return dScaleMapping;
	else if(lowChar==NFmiString("name")||          lowChar==NFmiString("nimi"))
		return dConstSymbolName;
//	else if(lowChar==NFmiString("heightscale")   || lowChar==NFmiString("korkeusskaala"))  
//		return dHeightScale;   //16.10.00
//	else if(lowChar==NFmiString("widthscale")   || lowChar==NFmiString("leveysskaala"))  
//		return dWidthScale;   //17.10.00
	else
		return NFmiParamRect::ConvertDefText(object);
}
//----------------------------------------------------------------------------
bool NFmiSymbolParamRect::CopyShortSymbol2Dest(NFmiString* symbolFile, ofstream& theDestinationFile)
{//10.2
		NFmiString fileName = *itsSubDir;
		fileName += itsSymbolSetName;    
		fileName += NFmiString("_");
	    fileName += *symbolFile;
		fileName += NFmiString(".ps");
		// Mika: Tupla ios::in ??
		ifstream inFile(fileName, ios::in|ios::binary); //27.8.01
        if(inFile.good() && !inFile.eof())  
        {
		   bool tempBool = NFmiWritePSConcat(itsDefToProductScale, theDestinationFile);
		   tempBool = NFmiCopyFile(inFile,theDestinationFile);
		   tempBool = NFmiWritePSEnd(theDestinationFile);
		   return isTrue;
		}
		else
			return false;
}
//----------------------------------------------------------------------------
bool NFmiSymbolParamRect::ConvertOrig2Short(NFmiString* symbolFile)
{ //10.2
// VAIN WINDOWSILLA TEHDYILLE SYMBOLEILLE

 
   NFmiString inputName = *itsOrigDir;
   inputName += itsSymbolSetName;
   inputName += NFmiString("/");
   inputName += *symbolFile;
   inputName += NFmiString(".ai");
   ifstream input(inputName, ios::in); //27.8.01

   if(input.good() && !input.eof())  
   {
   NFmiString outputName = *itsSubDir;
   outputName +=  itsSymbolSetName;
   outputName += NFmiString("_");
   outputName += *symbolFile;
   outputName += NFmiString(".ps");
   ofstream output(outputName);

   const short lineSize = 250;	//27.11 HUOM max rivipituus
   char inBuf[lineSize];
   bool copyOn = false;  
//   bool isWindows = isFalse;  //11.2 oltava Windows
   unsigned long copiedLines = 0;
   NFmiString mess = NFmiString ("%*** ");
   mess += *symbolFile;
   mess += NFmiString (" ALKAA ***");
   unsigned long len = mess.GetLen();  
//   output.write(mess.CharPtr(),15);
   output.write(mess.CharPtr(),len);   
   output.write((char*)NFmiString("\n"),1); 
   short num;
   char endLine = '\n';  
   while (input.getline(inBuf, lineSize, endLine)) //newline PC:lle
   {
	NFmiString apu = NFmiString(inBuf);

	if(apu.Search(NFmiString("%AI5_BeginLayer")) > 0 && copyOn)
//	if(NFmiString(inBuf) == NFmiString("%AI5_BeginLayer") && copyOn)
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: Symbolin " << (char*)*symbolFile
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
   output.write((char*)NFmiString("\n"),1);  
   
   if(itsLogFile)
      *itsLogFile << "rivejä kopioitu: " << copiedLines << " kun" << endl;

   return isTrue;
   }
   else
	   return false;
}
//----------------------------------------------------------------------------
bool NFmiSymbolParamRect::WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
										,NFmiFastQueryInfo* theQI 
										,ofstream& theDestinationFile
										,FmiPressOutputMode theOutput)
{
    if(!ActiveTimeIndex(itsPressParam->GetCurrentStep())) //26.9.01
    {
		   return true;
    }

   itsCurrentSegmentTime = ((NFmiQueryInfo*)theQI)->Time(); //8.2.2000 
   itsCurrentTime = itsCurrentSegmentTime;//16.2.2000

   itsDefToProductScale.SetEndScales(AbsoluteRectOfSymbolGroup.ToAbs(NFmiRect(TopLeft(),BottomRight())));

   if(!fIsConstSymbol)  //24.5.00
   {
	   if(!ReadValues(theQI)) 
		   return false;
   }


    bool isScaled;
	NFmiString* symbolFile;

   if(fIsConstSymbol)  //24.5.00
   {
	   symbolFile = &itsConstSymbol;
   }
   else
   {

		if (itsMultiMapping)  //230999
		{
			bool missingFound;
			symbolFile = itsMultiMapping->Map(itsCurrentParamArray, missingFound);
			if(missingFound)
			{
				itsNumOfMissing++;
	//			 (theQI); //1.2.2000	
				return false;
			}
			isScaled = false;         //skaalaus vielä poissa
		}
		else 
			symbolFile = itsMapping->Map(itsCurrentParamValue, isScaled); 
   }
    
	if(isScaled)  
	{
		double scaleFactor = (((long) itsCurrentParamValue) % 1000)/100.; //tonni on nyt fixattu
		NFmiRect endScales = itsDefToProductScale.GetEndScales();
        endScales.Inflate (scaleFactor);
		itsDefToProductScale.SetEndScales(endScales);
	}

	ScaleByValue(); //7.11.00 ja koodi scalingParamRectiin

	if(!symbolFile)      //9.2
	{
		if(itsMultiMapping)
		  *itsLogFile << "WARNING: Arvoilla " << itsCurrentParamArray[0] << ", "
		  << itsCurrentParamArray[1] << ", " << itsCurrentParamArray[2] 
		  << "... ei ole symbolivastaavuutta" << endl;
		else
		  *itsLogFile << "WARNING: Arvolla " << itsCurrentParamValue << " ei ole symbolivastaavuutta" << endl;
	}
	else if(*symbolFile != NFmiString("None") && theOutput == kPostScript)  //15.3.00 
	{
        if (CopyShortSymbol2Dest(symbolFile,theDestinationFile))
		{
		}
 		
		else
		{
 		   if(ConvertOrig2Short(symbolFile))
		   {
  		    if(itsLogFile)                               
			      *itsLogFile << "Symboli " << (char*)*symbolFile << " konvertoitu" << endl;
 
			if (CopyShortSymbol2Dest(symbolFile,theDestinationFile))
		      {
		      }
		      else
		      {   
				  if(itsLogFile)
				  {
//					  *itsLogFile << "*** ERROR: Symbolia ei konvertoinnin jälkeen löydy: " << (char*)*symbolFile << endl;
//					  *itsLogFile << "       oliko oikeuksia LyhytSymbolit-kansioon ?" << endl;
					  *itsLogFile << "*** ERROR: Symbol not found after conversion: " << (char*)*symbolFile << endl;
					  *itsLogFile << "       missing permission ?" << endl;
				  }
			  }
		   }
		   else
  		     if(itsLogFile)                                                       //31.10.01 +symbolset
			      *itsLogFile << "*** ERROR: Symbol missing from set: " << (char*)itsSymbolSetName << ":" << (char*)*symbolFile << endl;

		}
//		inFile.close();
	}
	else if(*symbolFile != NFmiString("None") && theOutput == kMetaLanguage) //13.3.00 metaKieltä
	{
		NFmiPoint realPos(AbsoluteRectOfSymbolGroup.BottomLeft());
		realPos += NFmiPoint(20, -20);  //HUOM ??? otetaan takaisin tämä alunperin selittämätön siirto
		// ostream saveCout = cout;
		// cout = theDestinationFile;

		WriteMetaCode(symbolFile, realPos, theDestinationFile); 

/*		theDestinationFile << endl << "SymbolPath Symbolit/" << (char*)itsSymbolSetName << endl;
		theDestinationFile << "Symbol " << (char*)*symbolFile << ".gif " 
			<< realPos.X() << " "<< realPos.Y() 
			<< " -scalex " << int(itsDefToProductScale.GetXScaling()*100.) << "%"     
			<< " -scaley " << int(itsDefToProductScale.GetYScaling()*100.) << "%";
//		AddSymbolMetaOptions();		
		theDestinationFile << endl;
*/	
	}

	else if(*symbolFile != NFmiString("None") && theOutput == kPlainText) //4.1.01 
	{
 
		// ostream saveCout = cout;
		// cout = theDestinationFile;
		theDestinationFile <<  (char*)*symbolFile << endl;
		// cout = saveCout;

	}
	else if(*symbolFile != NFmiString("None") && theOutput == kXml) //13.3.00 
	{
 
		// ostream saveCout = cout;
		// cout = theDestinationFile;
// xml:
		theDestinationFile << "<Picture>" << endl;
//		theDestinationFile << "  <Set>" << (char*)itsSymbolSetName << endl;
//		theDestinationFile << "  <Symbol>" << (char*)*symbolFile << endl;
//		theDestinationFile << "  <Place>" << realPos.X() << " "<< realPos.Y() << endl;
		// cout = saveCout;

	}
	return true;
};
//----------------------------------------------------------------------------
void NFmiSymbolParamRect::WriteMetaCode(NFmiString* symbolFile, NFmiPoint realPos, ofstream & os)
{
		os << endl 
			<< "SymbolPath /var/www/share/symbolit/" << (char*)itsSymbolSetName << endl;
		os << "Symbol " << (char*)*symbolFile << ".gif " 
			<< realPos.X() << " "<< realPos.Y() 
			<< " -scalex " << int(itsDefToProductScale.GetXScaling()*100.) << "%"     
			<< " -scaley " << int(itsDefToProductScale.GetYScaling()*100.) << "%";
		os << endl;
}	
//----------------------------------------------------------------------------
bool NFmiSymbolParamRect::ReadValues( NFmiFastQueryInfo* theQI, bool setRelHour)
{
//   Vain tunnit voi muuttaa tällä tasolla, segmentin aikaluuppi menee muuten sekaisin
	if(setRelHour) //3.5.02 estetään 2SymbolParamRectiltä kahta aikamuutosta
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
	    if(!PointOnParam(theQI, GetDataIdent().GetParam()) || !PointOnLevel(theQI))  //1.5.00
		   return false;
   
		return ReadCurrentValue(theQI, itsCurrentParamValue); //30.8.99
	}
}
