//© Ilmatieteenlaitos/Lasse.
//  Original 25.5.1998
// 
//Palstoittava tekstiolio ps-generointiin
//MUUTETTU 010698/LW kirjoitus vapaaseen (konveksiin alueeseen)

//*****************************
//
// POISTETTU 181298
// yhdistetty NFmiPressText:iin
//
//*****************************
//---------------------------------------------------------------------------

#include "ncoltext.h"
 #include  "nhyphstr.h" 

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 
NFmiColumnText::NFmiColumnText(const NFmiColumnText& thePressText)
: NFmiPressText() 

,itsLineStep(thePressText.itsLineStep)
,itsTopMargin(thePressText.itsTopMargin)
,itsLeftMargin(thePressText.itsLeftMargin)
,itsRightMargin(thePressText.itsRightMargin)
{
/*	if(thePressText.itsText)
		itsText = new NFmiString(*thePressText.itsText);
	else
		itsText = 0;
		*/
}
//---------------------------------------------------------------------------
NFmiColumnText::~NFmiColumnText() 
{
	if (itsText)
		delete itsText;
};
/*
//---------------------------------------------------------------------------
NFmiParamRect* NFmiColumnText::Clone() const
{
	return (NFmiParamRect *) new NFmiColumnText(*this);
};
*/

//----------------------------------------------------------------------------
FmiBoolean NFmiColumnText::ReadDescription(NFmiString& retString) //16.1
{			 
//	NFmiString string; SIIVOTTU 8.12.98
	NFmiString fileName;

	NFmiValueString valueString;
//	char object[255];// ch;
//	int iobject;
//	long long1;                 
	double r1,r2,r3,r4;
	double xmin,xmax,ymin,ymax;	  
	xmin = ymin = 0;				 
	xmax = ymax = 1;

	itsFont = NFmiString("Times-Roman"); //30.9 

	cin >> itsObject;
	itsString = itsObject;                     
	itsIntObject = ConvertDefText(itsString);
	
	while(itsIntObject != dEnd || itsCommentLevel)
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; //9.12
 	if(itsLoopNum > itsMaxLoopNum)  //2.2
	{
		if(itsLogFile)
			*itsLogFile << "ERROR: tuotetiedoston maksimipituus ylitetty #Tekstiss‰" << endl;
	    retString = itsString;
		return isFalse;
	}
    itsLoopNum++;
	switch(itsIntObject)
	{
			case dOther:	  
			{    
				if(itsLogFile)
					*itsLogFile << "ERROR: Tuntematon sana #Tekstiss‰: " << (char*)itsObject << endl;  
				ReadNext();
				break;
			}
			case dComment:	  
			{
//				isComment = isTrue;
				ReadNext();
				break;
			}
			case dEndComment:	  
			{
//				isComment = isFalse;
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
			}
		    ReadNext();
			break;
		}
		case dTextColor:
		{
			if (!ReadEqualChar())
				break;
			if(Read4Double(r1,r2,r3,r4))
			{
			   itsColor.cyan = r1;
			   itsColor.magenta = r2;
			   itsColor.yellow = r3;
			   itsColor.black = r4;
			}
		    ReadNext();
			break;
		}
		case dTextAlignment:
		{
			if (!ReadEqualChar())
				break;

			cin >> itsObject;
			itsString = itsObject;
			if (itsString == NFmiString ("Center") || itsString == NFmiString ("KeskiPiste"))
               itsAlignment = kCenter;
			else if (itsString == NFmiString ("Right") || itsString == NFmiString ("OikeaLaita"))
               itsAlignment = kRight;
			else if (itsString == NFmiString ("Left") || itsString == NFmiString ("VasenLaita"))
               itsAlignment = kLeft;

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
		case dTextFont:
		{
			if (!ReadEqualChar())
				break;
			cin >> itsObject;
			itsFont = itsObject;

		    ReadNext();
			break;
		}
		case dTextString:
		{
			if (!ReadEqualChar())
				break;
			cin >> itsObject;
			itsText = new NFmiString(itsObject);

		    ReadNext();
			break;
		}
		case dTextStyle:
		{
			if (!ReadEqualChar())
				break;
			cin >> itsObject;
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
				   cin >> itsObject;
			       valueString = itsObject;
				   if(valueString.IsNumeric())  
				   {
					  r2 = (double)valueString;
					  itsRectSize.Set(r1,r2);

					  cin >> itsObject;
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
					  cin >> itsObject;
					  itsString = itsObject;
				}
			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dTopMargin:
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{
			   itsTopMargin = r1;
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
		case dFile:
		{
			if (!ReadEqualChar())
				break;
			cin >> itsObject;
			fileName = NFmiString(itsObject);

		    ReadNext();
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
			cin >> itsObject;
			itsAddInFront = itsObject;

			ReadNext();
			break;
		}
		case dAddAfter:            //081298   
		{
			if (!ReadEqualChar())
				break;
			cin >> itsObject;
			itsAddAfter = itsObject;

			ReadNext();
			break;
		}
		default:            //151298   
		{
			ReadRemaining();
			break;
		}

	}
	} //while
	

	delete itsText;
	itsText = new NFmiString();
	  
	NFmiHyphenationString aString, helpString;
	
	if(fileName.IsValue())  
	{
      NFmiString dataFile = GetHome();  
	  dataFile += NFmiString("\\Tekstit\\");
	  dataFile += fileName;
	  dataFile += NFmiString(".txt");


	  *itsLogFile << "PalstaTeksti: "<< (char*)dataFile << endl;	 

	  fstream in(dataFile, ios::in|ios::nocreate);
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
		  helpString = aString.CreateHyphens("~");
		  *itsText = helpString.ReplaceChar(NFmiString("-"), NFmiString("\\255")); //29.6 Illussa "-" ei mene l‰pi ??
		  *itsLogFile << "  teksti luettu ja tavutettu" << endl;
	  }
	  else
	  {
		  *itsLogFile << "  ERROR: teksti ei lˆydy" << endl;
		 retString = itsString;
		 return kFalse;
	  }
	}
	else
	{
		*itsLogFile << "  WARNING: tekstitiedostoa ei ole: " << (char*)fileName << endl;	 
		retString = itsString;
		return kFalse;    //25.6
	}
	NFmiRect *rect;
	rect = new NFmiRect(xmin,ymax,xmax,ymin);
//	Set(*dataident,*rect);

	retString = itsString;    //081298
	return kTrue;
}

//---------------------------------------------------------------------------
int NFmiColumnText::ConvertDefText(NFmiString & object) //LW 20.11
{
	if(object==NFmiString("File") || object==NFmiString("Tiedosto"))
		return dFile;
	else if(object==NFmiString("TopMargin") || object==NFmiString("Yl‰Raja")
		|| object==NFmiString("Yl‰raja"))
		return dTopMargin;
	else if(object==NFmiString("LeftMargin") || object==NFmiString("VasenRaja")
		|| object==NFmiString("Vasenraja"))
		return dLeftMargin;
	else if(object==NFmiString("RightMargin") || object==NFmiString("OikeaRaja")
		|| object==NFmiString("Oikearaja"))
		return dRightMargin;
	else if(object==NFmiString("LineStep") || object==NFmiString("RivinSiirto")
		|| object==NFmiString("Rivinsiirto"))
		return dLineStep;
	else
		return NFmiPressText::ConvertDefText(object);
	
}

//---------------------------------------------------------------------------
FmiBoolean	NFmiColumnText::WritePS()																					
{
    ScalePlotting();
	return WriteString(NFmiString("PALSTATEKSTI"));
}
//-10.3---------------------------------------------------------------------------
FmiBoolean	NFmiColumnText::WriteString(const NFmiString& commentString)																					
	{	
		NFmiRect rect = itsWriteScale.GetEndScales();
		double lineStep = itsLineStep;
		if(lineStep < 0.) lineStep = rect.Height() * 1.2; 

		ostream_withassign saveCout = cout;
		cout = itsOutFile;  
		cout << "%*** " << (char*)commentString << " ALKAA ***" << endl;

		if(!fInParagraph)
			cout << "gsave" << endl;
		cout << itsColor.cyan << " "
			  << itsColor.magenta << " "
			  << itsColor.yellow << " "
			  << itsColor.black << " setcmykcolor" << endl;
        cout << "/" << (char*)itsFont << " /" << (char*)itsFont << "_" << endl;
        cout << rect.Height() << " selectlatinfont" << endl;

// 1.6 tehd‰‰n nyt aluksi vain suorakaiteelle, koska vapaamuotoinen polku (esim
//     ympyr‰ ja kolmio) eiv‰t menneet l‰pi Illussa (ei Mac eik‰ Pc) mutta kyll‰
//     printterille ja esim ghostscriptill‰. Mika kyseli netist‰ muttei saanut 
//     kunnollista vastausta. Toiskohan Illun seuraava versio tai joku p‰ivitys 
//     parannusta
//25.6  cout << "newpath" << endl;

		if(!fInParagraph)
		{
			cout << "{" << endl;                                   //25.6
			// alaraja tarpeeksi alas, jos ei koko teksti mahdu alueeseen j‰‰ PS ilmeisesti
			// luuppiin 
			cout << itsLeftMargin << " " << -500         << " moveto" << endl;
			cout << itsLeftMargin << " " << itsTopMargin << " lineto" << endl;
			cout << itsRightMargin << " " << itsTopMargin << " lineto" << endl;
			cout << itsRightMargin << " " << -500         << " lineto" << endl;
			cout << "closepath" << endl;
			cout << "}" << endl;                                   //25.6
	//		cout << "false upath" << endl;
			cout << "/TextPath exch def" << endl;
			cout << "/Indent 0 def" << endl;
			cout << "/Leading " << lineStep << " def" << endl;     //10.9
/*     k‰ytt‰‰ Mikan aikaisempaa makroa:
 
		cout << "/TextY " << itsTopMargin << " def" << endl;
		cout << "/LeftMargin " << itsLeftMargin << " def" << endl;
		cout << "/RightMargin " << itsRightMargin << " def" << endl;
		cout << "/TextX " << itsRightMargin << " def" << endl;
		if(lineStep > 0.)
			cout << "/Leading " << lineStep << " def" << endl;     //25.6
	
*/
			cout << "SetFirstText" << endl;
		}
		else
		{
			cout << "/Leading " << lineStep << " def" << endl; //171298 muuttuu jos koko muuttuu
			cout << itsParagraphMove.X() << " " << 
				    itsParagraphMove.Y() << " rmoveto" << endl;  //171298
		}

/*
//T‰ss‰ olisi rivien p‰tkiminen PS:n maksimipituuden alle. Vaan ei ihan toimi
// koska rivinsiirto aiheuttaa v‰lilyˆnnin lopulliseen kuvaan ja muutakin h‰iriˆt‰
// mm viimeinen merkki j‰‰ pois
// Sit‰paitsi t‰m‰nh‰n pit‰isi olla emossa eli PressTextiss‰

		short maxLength = 250;  //29.6
		long printed = 1;
		NFmiString helpString;
		cout << "(";
		while (itsText->GetLen() > printed)
		{
			short length = FmiMin(maxLength, itsText->GetLen() - printed); 
			helpString = itsText->GetChars(printed, length);
			cout <<  (char*)helpString  << endl;
			printed += maxLength;
		}
		cout  << ") " << "Paragraph" << endl;
*/		
		cout << "(" << (char*)*itsText << ") " << "Paragraph" << endl;


//		cout << "grestore" << endl;
		cout = saveCout;
	return kTrue;
};

//----------------------------------------------------------------------------
void NFmiColumnText::ScalePlotting(void)
{ 
	itsRightMargin = itsRectScale.ScaleX(itsRightMargin);
	itsLeftMargin = itsRectScale.ScaleX(itsLeftMargin);
	itsTopMargin = itsRectScale.ScaleY(itsTopMargin);
	NFmiPressText::ScalePlotting(); //tarvitaankohan kaikki 
}