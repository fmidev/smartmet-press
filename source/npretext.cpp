//© Ilmatieteenlaitos/Lasse.
//  Original 6.3.1998
// 
//Muutettu 070598/LW Skandit kuntoon Mikan makrolla
//Siirretty 100898/LW includesta sourceen 
//Muutettu 260898/LW Vasemman mukaan kohdistus mukaan
//Muutettu 310898/LW Alaviivojen tilalle v‰lilyˆnti
//Muutettu 070998/LW tasaus oikealle korjattu
//Muutettu 091098/LW +IsPureBlack()
//Muutettu 141098/LW kohdistus oikein kaikilla fonteilla (PS:n stringwidth)
//Muutettu 231098/LW +AddLocalTime
//Muutettu 131198/LW +merkkiv‰lin s‰‰tˆ (itsCharSpace)
//Muutettu 181298/LW YHDISTETTY NFmiColumnText t‰h‰n
//Muutettu 070199/LW palstan alalaita -500 -> 0 jotta ei Quark sekoaisi
//Muutettu 210199/LW palstaan lis‰tty ekan rivin sisennys
//Muutettu 190599/LW tarkistus ett‰ ylip‰‰ns‰ teksti annettu, kts seuraava 
//Muutettu 280599/LW edellinen pois koska ei sovi esim stationTextiin 
//Muutettu 2x0899/LW korjattu k‰‰nnettyjen tekstien y-koord. siten ett‰ t‰sm‰‰ myˆs
//                   koko tuotteen uudelleenskaalauksessa (mitta- +sijoitusalue) 
//Muutettu 131099/LW +itsSubText ja m‰‰rittelyyn kolme lajia niit‰
//Muutettu 080801/LW ConvertDefTextiin isot/pienet vapaus
//Muutettu 021001/LW itsSubText->SetDescriptionFile() muutamalle alaoliolle
//---------------------------------------------------------------------------

#include "npretext.h"
#include "nhyphstr.h"
#include "nptigtex.h"
#include "nptictex.h"
#include "nptidtex.h"
#include "NFmiFileString.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 
NFmiPressText::NFmiPressText(const NFmiPressText& thePressText)
: NFmiPressScaling() 
,itsLastLineStep(thePressText.itsLastLineStep)
,itsLineStep(thePressText.itsLineStep)
,itsLineStepFactor(thePressText.itsLineStepFactor)
,itsTopMargin(thePressText.itsTopMargin)
,itsRightMargin(thePressText.itsRightMargin)
,itsLeftMargin(thePressText.itsLeftMargin)
,itsIndent(thePressText.itsIndent)
,fLoopErrorReported(thePressText.fLoopErrorReported)
,fInParagraph(thePressText.fInParagraph)
,itsParagraphMove(thePressText.itsParagraphMove)
,fUpperCase (thePressText.fUpperCase)
,fLowerCase (thePressText.fLowerCase)
,fAddLocalTime(thePressText.fAddLocalTime)
,itsFont(thePressText.itsFont)
,itsAlignment(thePressText.itsAlignment)
,itsStyle(thePressText.itsStyle)
//,itsColor(thePressText.itsColor)
,itsCharSpace(thePressText.itsCharSpace)
{
	itsText = thePressText.itsText ? new NFmiString(*thePressText.itsText) : 0;
    itsSubText = thePressText.itsSubText ? new NFmiPressText(*thePressText.itsSubText) : 0;
}
//---------------------------------------------------------------------------
NFmiPressText::~NFmiPressText() 
{
	if (itsText)
		delete itsText;
	if (itsSubText)
		delete itsSubText;
};
/*
//---------------------------------------------------------------------------
NFmiParamRect* NFmiPressText::Clone() const
{
	return (NFmiParamRect *) new NFmiPressText(*this);
};
*/

//----------------------------------------------------------------------------
FmiBoolean NFmiPressText::ReadDescription(NFmiString& retString) //16.1
{			 
	NFmiValueString valueString;
	NFmiString  textFile, textPath, textDir; //25.1.01
	double r1,r2,r3;
	double xmin,xmax,ymin,ymax;	  
	xmin = ymin = 0;				 
	xmax = ymax = 1;

	itsFont = NFmiString("Times-Roman"); 

	*itsDescriptionFile >> itsObject;
	itsString = itsObject;                     
	itsIntObject = ConvertDefText(itsString);
	
	while(itsIntObject != dEnd || itsCommentLevel)
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; //9.12
 	if(itsLoopNum > itsMaxLoopNum)  //2.2
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Tekstiss‰" << endl;
	    retString = itsString;
		return isFalse;
	}
    itsLoopNum++;
	switch(itsIntObject)
	{
			case dOther:	  
			{    
				if(itsLogFile)
					*itsLogFile << "*** ERROR: Tuntematon sana #Tekstiss‰: " << (char*)itsObject << endl;  
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
		case dAddLocalTime:   //20.10
		{
			fAddLocalTime = kTrue;

			ReadNext();
			break;
		}
		case dSymbolPlace:
		{
			if (!ReadEqualChar())
				break;
			if(Read2Double(r1,r2))
			{
			   Place(NFmiPoint(r1,r2));
			   itsRotatingPoint = NFmiPoint(r1,r2); //151298
			}
			ReadNext();
			break;
		}
		case dPsPlaceMove:
		{
			if (!ReadEqualChar())
				break;
			if(Read2Double(r1,r2))
			{
			   Move(NFmiPoint(r1,r2));
			}
			ReadNext();
			break;
		}
		case dCharSpace:    //131198
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsCharSpace = r1;
			}
			ReadNext();
			break;
		}
/*		case dLF:  //171298
		{
		    itsColor.cyan = 0.;   //valkoinen piste kun ei muutakaan keksi
		    itsColor.magenta = 0.;
		    itsColor.yellow = 0.;
		    itsColor.black = 0.;
 
            itsAlignment = kLeft;
			SetInParagraph();
			itsText = new NFmiString("."); //pelkk‰ v‰lilyˆnti tai rivinsiirto ei k‰y

			ReadNext();
			break;
		} */
/*		case dTextColor:  //pit‰isi olla pressdescriptionissa
		{
			if (!ReadEqualChar())
				break;
			if(Read4Double(r1,r2,r3,r4))
			{
			   FmiGenericColor color;
			   color.color.cmyk.cyan = r1;
			   color.color.cmyk.magenta = r2;
			   color.color.cmyk.yellow = r3;
			   color.color.cmyk.black = r4;
			   color.isRgb = false;
			   SetColor(color);
			}
			ReadNext();
			break;
		}
		*/
		case dTextAlignment:
		{
			if (!ReadEqualChar())
				break;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;

			NFmiString lowChar = itsString; //10.5.02
			lowChar.LowerCase(); 

			if (lowChar == NFmiString ("center") || lowChar == NFmiString ("keskipiste")
				|| lowChar == NFmiString ("keski"))
               itsAlignment = kCenter;
			else if (lowChar == NFmiString ("right") || lowChar == NFmiString ("oikealaita")
				|| lowChar == NFmiString ("oikea"))
               itsAlignment = kRight;
			else if (lowChar == NFmiString ("left") || lowChar == NFmiString ("vasenlaita")
				|| lowChar == NFmiString ("vasen"))
               itsAlignment = kLeft;
			else             //7.9
				*itsLogFile << "*** ERROR: Tuntematon kohdistus tekstiss‰: " << (char*)itsObject << endl;  

			ReadNext();
			break;
		}
		case dTextFont:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsFont = itsObject;

			ReadNext();
			break;
		}
		case dTextString:
		{
			if (!ReadEqualChar())
				break;
			itsText = new NFmiString(ReadString());
//			isText = kTrue;

			ReadNext();
			break;
		}
		case dTextStyle:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsStyle = itsObject;

			ReadNext();
			break;
		}
		case dSymbolSize:
		{
			if (!ReadEqualChar())
				break;

			if(ReadDouble(r1))       
				{				
				   *itsDescriptionFile >> itsObject;
			       valueString = itsObject;
				   if(valueString.IsNumeric())  
				   {
					  r2 = (double)valueString;
					  itsRectSize.Set(r1,r2);

					  *itsDescriptionFile >> itsObject;
					  itsString = itsObject;
				    }
				    else
				    {
					   itsRectSize.Y(r1);
					   itsString = valueString;
				    }
  				}
				else
				{
					  *itsDescriptionFile >> itsObject;
					  itsString = itsObject;
				}

//			*itsDescriptionFile >> itsObject;
//			itsString = itsObject;
			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dUpperCase:            //081298
		{
			fUpperCase = kTrue;

			ReadNext();
			break;
		}
		case dLowerCase:            //081298
		{
			fLowerCase = kTrue;

			ReadNext();
			break;
		}
		case dAddInFront:            //081298   
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsAddInFront = itsObject;

			ReadNext();
			break;
		}
		case dAddAfter:            //081298   
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsAddAfter = itsObject;

			ReadNext();
			break;
		}
		case dTopMargin:
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsTopMargin = r1;
			   fInParagraph = kTrue;
			}
		    ReadNext();
			break;
		}
		case dLeftMargin:
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsLeftMargin = r1;
			   fInParagraph = kTrue;
			}
		    ReadNext();
			break;
		}
		case dRightMargin:
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsRightMargin = r1;
			   fInParagraph = kTrue;
			}
		    ReadNext();
			break;
		}
		case dLineStep:
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsLineStep = r1;
			}
		    ReadNext();
			break;
		}
		case dIndent:      //210199
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsIndent = r1;
			}
		    ReadNext();
			break;
		}
		case dLF:      //220199
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsLineStep = r1;
			}
		    ReadNext();
			break;
		}
		case dLFFactor:      //220199
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsLineStepFactor = r1;
			}
		    ReadNext();
			break;
		}
		case dParagraphBorders:  //181298
		{
			if (!ReadEqualChar())
				break;

			if(Read2Double(r1,r2) && ReadDouble(r3))
			{
			   if(r3 > r1 + 10. && r2 > -100.)
			   {
				   itsLeftMargin = r1;
				   itsTopMargin =  r2;
				   itsRightMargin = r3;
			       fInParagraph = kTrue;
			   }
			   else
			   {
					*itsLogFile << "*** ERROR: Virheelliset palstanrajat: " << 
						          r1 << " " << r2 << " " << r3 << endl;  
			   }
			}
		    ReadNext();
			break;
		}
		case dFile:
		{
			if (!ReadEqualChar())
				break;
			textFile = ReadString();
//			isText = kTrue;

		    ReadNext();
			break;
		}
		case dTextDir: //25.1.01 
		{
			if (!ReadEqualChar())
				break;

            textDir = ReadString();

			ReadNext();
			break;
		}

		case dTextPath:  //25.1.01
		{
			if (!ReadEqualChar())
				break;

            textPath = ReadString();

			ReadNext();
			break;
		}
		default:               
		{
			ReadRemaining();
			break;
		}

	}
	} 

	if(textFile.IsValue())  
	{
	  delete itsText;
	  itsText = new NFmiString;
	  
	  NFmiHyphenationString aString;

	  if(GetTimestampDayGap() != kShortMissing)
		  AddTimeStamp(textFile);

	  NFmiFileString dataFile = CreatePath(NFmiString("Tekstit"), textPath, textDir
		                                  ,textFile, NFmiString("txt"));

	  *itsLogFile << "VakioTeksti: "<< (char*)dataFile ;	 

	  std::fstream in(dataFile, ios::in|ios::in);
	  if(in.good())
	  {
		  while( !in.eof())
		  {
			  char text[120];
			  in >> text;
			  if(!in.eof())    //29.6 est‰‰ viimeisen sanan kahdentumisen jos loppuu pelkk‰‰n rivinsiirtoon
			       aString += NFmiHyphenationString(text) += NFmiHyphenationString(" ");
		  }
		  in.close();
		  *itsText = aString;//ReplaceChar(NFmiString("-"), NFmiString("\\255")); //29.6 Illussa "-" ei mene l‰pi ??
          *itsLogFile << " luettu" << endl;
	  }
	  else
	  {
		  *itsLogFile << endl << "  *** ERROR: teksti ei lˆydy" << endl;
		 retString = itsString;
		 return kFalse;
	  }
	}

	retString = itsString;
	return kTrue;
}
//261198----------------------------------------------------------------------------
FmiBoolean NFmiPressText::ReadRemaining(void)  
{
	double r1;// r2, r3;
	NFmiString helpString;
	NFmiValueString valueString;
	switch(itsIntObject)
	{	
		case dTextLanguage:   //5.1.2000
		{
			if (!ReadEqualChar())
				break;

			itsLanguage = ReadLanguage(); //24.3.00

			ReadNext();
			break;
		}
		case dParagraphMove:    //171298
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
				fInParagraph = kTrue;
				*itsDescriptionFile >> itsObject;
			    valueString = itsObject;
				if(valueString.IsNumeric())
				{
					itsParagraphMove = NFmiPoint(r1,(double)valueString);
					ReadNext();
				}
				else
				{
					itsParagraphMove = NFmiPoint(0,r1);
				    itsString = valueString;
					itsIntObject = ConvertDefText(itsString);
				}
			}
			else
				ReadNext();
			break;
		}
		case dInParagraph:    //181298
		{
			fInParagraph = kTrue;
			ReadNext();

			break;
		}
	//** kaikki subText caset lis‰tty 13.10.99
		case dSubTextObject:  
		{
			if (itsSubText)    //onko mahdollista
				delete itsSubText;
			itsSubText = new NFmiPressText;
            itsSubText->SetHome(GetHome()); 
		    itsSubText->SetLogFile(itsLogFile);
			itsSubText->SetLanguage(itsLanguage);
			itsSubText->SetDescriptionFile(itsDescriptionFile); //2.10.01 miten on voinut toimia ilman
			if(!itsSubText->ReadDescription(itsString))
			{
				delete itsSubText;
				itsSubText = 0;
			}
			itsIntObject = ConvertDefText(itsString);
			break;
		}
/*
		case dSubTimeTextObject:  
		{	// testi; vaikeata, pit‰isi yritt‰‰ WritePs-vaiheessa
		    //  ajan vieminen ala-aikaoliolle

			if (itsSubText)
				delete itsSubText;
			NFmiPressDataTimeText* itsSubText = new NFmiPressDataTimeText;  
			itsSubText->SetHome(GetHome()); 
			itsSubText->SetLogFile(itsLogFile);
			itsSubText->SetData(((NFmiPressDataTimeText*)this)->GetData());        
			itsSubText->SetLanguage(itsLanguage); 

			if(!itsSubText->ReadDescription(itsString))
			{
				delete itsSubText;
				itsSubText = 0;
			}

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		*/
		case dSubTimeTextObject:  
		{		
			if (itsSubText)
				delete itsSubText;
			itsSubText = new NFmiPressGivenTimeText;
            itsSubText->SetHome(GetHome()); 
		    itsSubText->SetLogFile(itsLogFile);
			itsSubText->SetTime(itsFirstPlotTime);
			itsSubText->SetLanguage(itsLanguage); 
			itsSubText->SetDescriptionFile(itsDescriptionFile); //2.10.01 miten on voinut toimia ilman
			if(!itsSubText->ReadDescription(itsString))
			{
				delete itsSubText;
				itsSubText = 0;
			}

			itsIntObject = ConvertDefText(itsString);
			break;
		}

		case dSubComputerTimeTextObject:  
		{		
			if (itsSubText)
				delete itsSubText;
			itsSubText = new NFmiPressComputerTimeText;
            itsSubText->SetHome(GetHome()); 
		    itsSubText->SetLogFile(itsLogFile);
			itsSubText->SetLanguage(itsLanguage); 
			itsSubText->SetDescriptionFile(itsDescriptionFile); //2.10.01 miten on voinut toimia ilman
			if(!itsSubText->ReadDescription(itsString))
			{
				delete itsSubText;
				itsSubText = 0;
			}

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		default:
		{
			NFmiPressScaling:: ReadRemaining();
			break;
		}
	}
	return kTrue;
}

//---------------------------------------------------------------------------
int NFmiPressText::ConvertDefText(NFmiString & object) 
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //8.8.01

	if(lowChar==NFmiString("language")     || lowChar==NFmiString("kieli"))
		return dTextLanguage;                        
	else if(lowChar==NFmiString("#subconsttext")|| lowChar==NFmiString("#jatkovakioteksti")
		|| lowChar==NFmiString("#jatkovakioteksti"))
		return dSubTextObject;
	else if(lowChar==NFmiString("#subtimetext") || lowChar==NFmiString("#jatkoaikateksti")
		 || lowChar==NFmiString("#jatkoaikateksti"))
		return dSubTimeTextObject;
	else if(lowChar==NFmiString("#subcomputertime")|| lowChar==NFmiString("#jatkokoneenaika"))
		return dSubComputerTimeTextObject;
	else if(lowChar==NFmiString("paragraphmove")||lowChar==NFmiString("kappaleensiirto"))
		return dParagraphMove;             
	else if(lowChar==NFmiString("indent") ||    lowChar==NFmiString("sisennys"))
		return dIndent;             
	else if(lowChar==NFmiString("linefeed") ||    lowChar==NFmiString("rivinv‰li")
		||    lowChar==NFmiString("riviv‰li"))
		return dLF;             
	else if(lowChar==NFmiString("linefeedfactor")||lowChar==NFmiString("rivinv‰likerroin")
		||    lowChar==NFmiString("riviv‰likerroin"))
		return dLFFactor;             
	else if(lowChar==NFmiString("inparagraph") || lowChar==NFmiString("palstaan"))
		return dInParagraph;            
	else if(lowChar==NFmiString("addafter") || lowChar==NFmiString("lis‰‰per‰‰n"))
		return dAddAfter;           
	else if(lowChar==NFmiString("addinfront") || lowChar==NFmiString("lis‰‰eteen"))
		return dAddInFront;             
	else if(lowChar==NFmiString("lowercase") || lowChar==NFmiString("pienill‰kirjaimilla")
		       || lowChar==NFmiString("pienetkirjaimet"))
		return dLowerCase;           
	else if(lowChar==NFmiString("uppercase") || lowChar==NFmiString("isoillakirjaimilla")
		          || lowChar==NFmiString("isotkirjaimet"))
		return dUpperCase;          
	else if(lowChar==NFmiString("addlocaltime") || lowChar==NFmiString("lis‰‰paikallinenaika"))
		return dAddLocalTime;            
	else if(lowChar==NFmiString("alignment") || lowChar==   NFmiString("kohdistus")
		|| lowChar==NFmiString("tasaus"))   
		return dTextAlignment;
	else if(lowChar==NFmiString("text") || lowChar==        NFmiString("teksti"))
		return dTextString;
	else if(lowChar==NFmiString("font") || lowChar==        NFmiString("kirjasin")
		|| lowChar==NFmiString("fontti"))                        
		return dTextFont;
	else if(lowChar==NFmiString("style") || lowChar==       NFmiString("tyyli"))
		return dTextStyle;
	else if(lowChar==NFmiString("charspacejustification") || lowChar==NFmiString("merkkiv‰lins‰‰tˆ"))
		return dCharSpace;
	if(lowChar==NFmiString("file") || lowChar==          NFmiString("tiedosto"))
		return dFile;
	else if(lowChar==NFmiString("directory") || lowChar==NFmiString("hakemisto"))
			return dTextDir;      //25.1.01
	else if(lowChar==NFmiString("path")      || lowChar==NFmiString("polku")) 
			return dTextPath;   //25.1.01
	else if(lowChar==NFmiString("topmargin") || lowChar==NFmiString("yl‰raja"))
		return dTopMargin;
	else if(lowChar==NFmiString("leftmargin") || lowChar==NFmiString("vasenraja"))
		return dLeftMargin;
	else if(lowChar==NFmiString("rightmargin") || lowChar==NFmiString("oikearaja"))
		return dRightMargin;
	else if(lowChar==NFmiString("paragraphborders") || lowChar==NFmiString ("palstanrajat"))
		return dParagraphBorders;
	else if(lowChar==NFmiString("timestamp") || lowChar==NFmiString ("aikaleima"))
		return dFileTimestamp;
	else
		return NFmiPressScaling::ConvertDefText(object);
	
}

//---------------------------------------------------------------------------
FmiBoolean	NFmiPressText::WritePS(FmiPressOutputMode theOutput)																					
{
    ScalePlotting();
	return WriteString(NFmiString("VAKIOTEKSTI"), theOutput);
}
//-10.3---------------------------------------------------------------------------
FmiBoolean	NFmiPressText::WriteString(const NFmiString& commentString, FmiPressOutputMode theOutput)																					
	{
//141298 Mahdollistettu ett‰ eri elementit voivat tulla allekkain palstassa

		NFmiString text = Construct(itsText); //081298
		NFmiHyphenationString hypString, helpString;
		FmiBoolean firstParagraph = fInParagraph && itsTopMargin > -100.;
		FmiBoolean nParagraph = fInParagraph && !firstParagraph;

//040199 ???		if(fInParagraph)
		    text.ReplaceChars(NFmiString("_"), NFmiString(" ")); // 31.8 alaviivat pois

		NFmiRect rect = itsWriteScale.GetEndScales();
		NFmiPoint topLeft = rect.TopLeft(); 
		NFmiPoint bottomRight = rect.BottomRight();      
		double centerX = itsWriteScale.GetEndCenter().X();  
		// int number = text.GetLen();
 //TESTI
//		*itsLogFile << "*** LEFT MARGIN: " << itsLeftMargin << endl; 
//		*itsLogFile << "*** RIGHT MARGIN: " << itsRightMargin << endl; 

		if(itsRightMargin-itsLeftMargin < rect.Height()*1.2 && fInParagraph && theOutput == kPostScript) //12.11.01 +ps
		{
			*itsLogFile << "*** ERROR: too narrow text column: " << itsRightMargin-itsLeftMargin << endl; 
			return kFalse;
		}

		double lineStep = itsLineStep;
		double lineStepFactor = itsLineStepFactor;
		if(lineStepFactor <= 0.)          //220199
				lineStepFactor = 1.2;
		if(lineStep <= 0.)
		    lineStep = rect.Height() * lineStepFactor;   //220199
		
		itsLineStep = lineStep; //pressProduct kysyy

		double lineStepAdd = lineStep - itsLastLineStep; //181298
		if(firstParagraph) lineStepAdd = 0; 

		//*******************************************************************
		//laskettu fontille LucideConsole 5.1.98
		// 14.10 alkaen k‰ytet‰‰n PS:n stringwidth -> oikea sijoitus kaikilla fonteilla
		// y-koordinaatinkin saisi paikalleen samoin keinoin
		//*******************************************************************

//		double x = centerX;  
/* 13.10	    if(itsAlignment == kRight)   
			   x = centerX + rect.Height() * (-.55f - .604f*(itsText->GetLen()-1)); //7.9
	    else if(itsAlignment == kLeft)   //26.8
			   x = centerX;
		else   // keskipiste
			   x = centerX + rect.Height() * (-.31f - .302f*(itsText->GetLen()-1));
*/
		double heightHalf = .147f * rect.Height(); //kokeellinen vakio
		double heightHalfX =.345f * rect.Height(); //kokeellinen vakio
		double heightHalfY =.5 * rect.Height(); //kokeellinen vakio, t‰m‰ sijoittaa samoin kuin
		                                        // entinen itsRotatingPoint ainakin yhdell‰ koolla
//		double heightCenter = topLeft.Y() + .147f * rect.Height(); //HUOM 051298 oli .125; nostaa merkit v‰h‰n=Illun

			double x, y;
	//		double yTest;
			if(itsRotatingAngle == 90.)  //151298
			{
				x = centerX + heightHalfX;
	//			yTest = itsRotatingPoint.Y();
				y = topLeft.Y() + heightHalfY; //20.8.99,miksi oli itsRotatingPoint.Y();
										   // ei edes skaalattu joten koko kuvan skaalaus 
											   // ei k‰‰nnetyille toiminut
			}
			else
			{
				x = centerX;
				y = topLeft.Y() + heightHalf;
			}

		// ostream saveCout = cout;
		// cout = itsOutFile;

		if(theOutput == kMetaLanguage)
		{
			// itsOutFile << endl << "# TEKSTI" << endl; 
			// itsOutFile << "font " << (char*)itsFont  << endl;
			// itsOutFile << "fontsize " << rect.Height()  << endl;
			// itsOutFile << "textalign " << (char*)AlignmentToMeta(itsAlignment)  << endl; 
			*itsOutFile << endl << "# TEKSTI" << endl; 
			*itsOutFile << "font " << (char*)itsFont  << endl;
			*itsOutFile << "fontsize " << rect.Height()  << endl;
			*itsOutFile << "textalign " << (char*)AlignmentToMeta(itsAlignment)  << endl; 

			WriteColor(GetColor(), theOutput, *itsOutFile);

			// itsOutFile << "text (" << (char*)text << ") " << x << " " << y << endl;
			*itsOutFile << "text (" << (char*)text << ") " << x << " " << y << endl;
		}
		else if(theOutput == kPlainText)
		{
			// itsOutFile << endl << (char*)text;
			*itsOutFile << endl << (char*)text;
		}
		else if(theOutput == kXml)
		{
			// itsOutFile << "<text>" << (char*)text << ") " << x << " " << y << endl;
			*itsOutFile << "<text>" << (char*)text << ") " << x << " " << y << endl;
		}
		else
		{
			// itsOutFile << "%*** " << (char*)commentString << " ALKAA ***" << endl;
			*itsOutFile << "%*** " << (char*)commentString << " ALKAA ***" << endl;
		

	// skandit vaatii erikoisk‰sittelyn:
	// ‰,Â,ˆ,÷ onnistuvat vain sanan lopussa, ƒ,≈ ei silloinkaan
	// ON 5.98 RATKAISTU MIKAN KANSSA

			if(!nParagraph)
				// itsOutFile << "gsave" << endl;
				*itsOutFile << "gsave" << endl;

			if(IsPureBlack())   //9.10
				// itsOutFile << "true setoverprint" << endl;
				*itsOutFile << "true setoverprint" << endl;
			else
				// itsOutFile << "false setoverprint" << endl;
				*itsOutFile << "false setoverprint" << endl;

			WriteColor(GetColor(), theOutput, *itsOutFile);
/*			if(GetRGB(rgb))  //18.10.00
			{	
				itsOutFile << rgb.red << " "
					  << rgb.green << " "
					  << rgb.blue << " setrgbcolor" << endl;
			}
			else
			{
				itsOutFile << itsColor.cyan << " "
					  << itsColor.magenta << " "
					  << itsColor.yellow << " "
					  << itsColor.black << " setcmykcolor" << endl;
			}
*/
			if(fInParagraph)
			{
				text += NFmiString(" ");
				hypString = NFmiHyphenationString(text);
				helpString = hypString.CreateHyphens("~");
				text = helpString.ReplaceChar(NFmiString("-"), NFmiString("\\255")); //29.6 Illussa "-" ei mene l‰pi ??
			}
	// Jostain syyst‰ Mikan makrot ei l‰p‰ise miinusta, 
	// joten ei k‰ytet‰ jos on tavuviivaa (silloin taas ei skandit tule!, mit‰ tehd‰)

			FmiBoolean isHyphen = text.Search(NFmiString("-"));
			if(fInParagraph || !isHyphen)
			{
				// itsOutFile << "/" << (char*)itsFont << " /" << (char*)itsFont << "_" << endl;
				// itsOutFile << rect.Height() << " selectlatinfont" << endl;
				*itsOutFile << "/" << (char*)itsFont << " /" << (char*)itsFont << "_" << endl;
				*itsOutFile << rect.Height() << " selectlatinfont" << endl;
			}
			else
				// itsOutFile << "/" << (char*)itsFont << " " << rect.Height() << " selectfont" << endl;
				*itsOutFile << "/" << (char*)itsFont << " " << rect.Height() << " selectfont" << endl;

	/*		if(isHyphen)              //161198
				itsOutFile << "/" << (char*)itsFont << " " << rect.Height() << " selectfont" << endl;
			else
			{
				itsOutFile << "/" << (char*)itsFont << " /" << (char*)itsFont << "_" << endl;
				itsOutFile << rect.Height() << " selectlatinfont" << endl;
			}
	*/
			if(firstParagraph)
			{
				// itsOutFile << "{" << endl;                                   //25.6
				*itsOutFile << "{" << endl;                                   //25.6

				// alaraja tarpeeksi alas, jos ei koko teksti mahdu alueeseen j‰‰ PS ilmeisesti
				// luuppiin 

				// itsOutFile << itsLeftMargin << " " << 0 << " moveto" << endl;  //070199 oli -500->Quark sekaisin
				// itsOutFile << itsLeftMargin << " " << itsTopMargin << " lineto" << endl;
				// itsOutFile << itsRightMargin << " " << itsTopMargin << " lineto" << endl;
				// itsOutFile << itsRightMargin << " " << 0 << " lineto" << endl; //070199 oli -500->Quark sekaisin
				// itsOutFile << "closepath" << endl;
				// itsOutFile << "}" << endl;                                   //25.6
				// // itsOutFile << "false upath" << endl;
				// itsOutFile << "/TextPath exch def" << endl;
				// // itsOutFile << "/Indent 0 def" << endl;
				// itsOutFile << "/Indent " << itsIndent << " def" << endl;              //210199
				// itsOutFile << "/Leading " << lineStep << " def" << endl;     //210199
				// itsOutFile << "SetFirstText" << endl;

				*itsOutFile << itsLeftMargin << " " << 0 << " moveto" << endl;  //070199 oli -500->Quark sekaisin
				*itsOutFile << itsLeftMargin << " " << itsTopMargin << " lineto" << endl;
				*itsOutFile << itsRightMargin << " " << itsTopMargin << " lineto" << endl;
				*itsOutFile << itsRightMargin << " " << 0 << " lineto" << endl; //070199 oli -500->Quark sekaisin
				*itsOutFile << "closepath" << endl;
				*itsOutFile << "}" << endl;                                   //25.6
				// *itsOutFile << "false upath" << endl;
				*itsOutFile << "/TextPath exch def" << endl;
				// *itsOutFile << "/Indent 0 def" << endl;
				*itsOutFile << "/Indent " << itsIndent << " def" << endl;              //210199
				*itsOutFile << "/Leading " << lineStep << " def" << endl;     //210199
				*itsOutFile << "SetFirstText" << endl;
			}
			if(!fInParagraph)
			{
				// itsOutFile << x << " " << y << " moveto" << endl;
				*itsOutFile << x << " " << y << " moveto" << endl;
		//*****13.10
				if(itsAlignment == kRight )
				{
					if(itsRotatingAngle == 90.)  //151298
					{
						// itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						// itsOutFile << "neg exch neg" << endl; 
						// itsOutFile  << " rmoveto" << endl;
						*itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						*itsOutFile << "neg exch neg" << endl; 
						*itsOutFile  << " rmoveto" << endl;
					}
					else
					{
						// itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						// itsOutFile << "neg exch neg exch" << endl; 
						// itsOutFile  << " rmoveto" << endl;
						*itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						*itsOutFile << "neg exch neg exch" << endl; 
						*itsOutFile  << " rmoveto" << endl;
					}
				}
				else if (itsAlignment == kCenter)
				{
					if(itsRotatingAngle == 90.)  //151298
					{
						// itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						// itsOutFile << "-2. div exch -2. div " << endl; //puolikas matka
						// itsOutFile  << " rmoveto" << endl;
						*itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						*itsOutFile << "-2. div exch -2. div " << endl; //puolikas matka
						*itsOutFile  << " rmoveto" << endl;
					}
					else
					{
						// itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						// itsOutFile << "-2. div exch -2. div exch" << endl; //puolikas matka
						// itsOutFile  << " rmoveto" << endl;
						*itsOutFile << "(" << (char*)text << ") " << "stringwidth" << endl;
						*itsOutFile << "-2. div exch -2. div exch" << endl; //puolikas matka
						*itsOutFile  << " rmoveto" << endl;
					}
				}
			}

	//*****
			if(itsRotatingAngle == 90. && !fInParagraph)  
			{
				// itsOutFile << itsRotatingPoint.X() << " " << itsRotatingPoint.Y() << " translate" << endl;
				// itsOutFile << itsRotatingAngle << " rotate" << endl;
				// itsOutFile << -itsRotatingPoint.X() << " " << -itsRotatingPoint.Y() << " translate" << endl;
				*itsOutFile << itsRotatingPoint.X() << " " << itsRotatingPoint.Y() << " translate" << endl;
				*itsOutFile << itsRotatingAngle << " rotate" << endl;
				*itsOutFile << -itsRotatingPoint.X() << " " << -itsRotatingPoint.Y() << " translate" << endl;
			}

			if(fInParagraph)
			{
				double yMove = itsParagraphMove.Y() - lineStepAdd;
				// itsOutFile << "/Leading " << lineStep << " def" << endl;     //210199
				// itsOutFile << itsParagraphMove.X() << " " <<             //171298 EI ONNAA X
				// 		yMove << " rmoveto" << endl;
				// itsOutFile << "(" << (char*)text << ") Paragraph" << endl;
				*itsOutFile << "/Leading " << lineStep << " def" << endl;     //210199
				*itsOutFile << itsParagraphMove.X() << " " <<             //171298 EI ONNAA X
				               yMove << " rmoveto" << endl;
				*itsOutFile << "(" << (char*)text << ") Paragraph" << endl;
			}
			else
			{
				if(itsCharSpace == 0.) //131198 merkkiv‰lin s‰‰tˆ
					// itsOutFile << "(" << (char*)text << ") show" << endl;
					*itsOutFile << "(" << (char*)text << ") show" << endl;
				else    //HUOM ashow
					// itsOutFile << itsCharSpace << " 0. (" << (char*)text << ") ashow" << endl;
					*itsOutFile << itsCharSpace << " 0. (" << (char*)text << ") ashow" << endl;
			}

			// itsOutFile << "false setoverprint" << endl; //9.10
			*itsOutFile << "false setoverprint" << endl; //9.10
		} //loppu eps-moodi
		// cout = saveCout;
	return kTrue;
};

// 8.12.98----------------------------------------------------------------------------
NFmiString NFmiPressText::Construct(NFmiString *theString) const 
{
	NFmiString string = NFmiString(*theString);
	if(fUpperCase) string.UpperCase();
	if(fLowerCase) string.LowerCase();
	NFmiString retString;
	if(itsAddInFront.IsValue())
		retString += itsAddInFront;
    retString += string;
	if(itsAddAfter.IsValue())
		retString += itsAddAfter;

	if(itsSubText)
	{
//		SetText(((NFmiPressTime)(itsFirstPlotTime.LocalTime(25.f))).ToStr(itsFormat,itsLanguage));
		if(itsSubText->SetText())  //tarvitaan NFmiGiventextiss‰
			retString += itsSubText->Construct(itsSubText->GetText());
	}
	return retString;
}
//----------------------------------------------------------------------------
void NFmiPressText::ScalePlotting(void)
{ 
	itsRightMargin = itsRectScale.ScaleX(itsRightMargin);
	itsLeftMargin = itsRectScale.ScaleX(itsLeftMargin);
	itsTopMargin = itsRectScale.ScaleY(itsTopMargin);
	NFmiPressScaling::ScalePlotting(); //tarvitaankohan kaikki 
}
/*
// 9.10.98----------------------------------------------------------------------------
FmiBoolean NFmiPressText::IsPureBlack(void)
{
	return itsColor.cyan <= 0. && itsColor.magenta <= 0.
	    && itsColor.yellow <= 0. && itsColor.black >= 1.;
		
}
*/
/*
//----------------------------------------------------------------------------
void NFmiPressText::SetValueDepending(float value)
{
	if(fColorValueDependent)
	{
		if(value > 0.f)
			SetColor(0.f, 1.f, 1.f, 0.f);
		else
			SetColor(1.f, 0.f, 0.f, 0.f);
	}
		
}
*/
/*
//----------------------------------------------------------------------------
void NFmiPressText::SetColor(float theC, float theM, float theY, float theK)
{
	itsColor.cyan = theC;		
	itsColor.magenta = theM;		
	itsColor.yellow = theY;		
	itsColor.black = theK;		
}
*/
