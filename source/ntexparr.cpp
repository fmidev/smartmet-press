//© Ilmatieteenlaitos/Lasse.
//  Original 6.11.1997 kopioitu NFmiSymbolParamRect:stä
//  
//	Changed	18.11.97	Salla	Clone()
//Muutettu 201197/LW
//     - " -                  itsSubDir ei enää tuoda ylempää vaan luetaan suoraan ymp.muuttujasta
//Muutettu 241197/LW +fColorValueDependent
//Muutettu 271197/LW +itsFormat, +kRight alignment
//Muutettu 050198/LW tarkka sijoutus LusideConsole fontille (kCenter ja kRight)
//Muutettu 070198/LW " ja kohdistus rectin sisällä oikeaan laitaan
//Muutettu 150198/LW ReadDouble() ym käyttö
//Muutettu 050398/LW Abstrakti luokka(vai olisiko käyttöä tällaisena),
//                   numeropiirtoon (fontti) liittyvät ominaisuudet siirretty uuteen
//                   NFmiNumberParamRect-luokkaan
//Muutettu 121098/LW +setoverprint
//Muutettu 181199/LW Muunnos (ei moniMuunnos) siirretty Letteristä tänne jotta Numeronkin käytössä
//Muutettu 290200/LW Copykonstr. siivottu
//Muutettu 300501/LW +suhteellinenmerkkileveys
//---------------------------------------------------------------------------

#include "ntexparr.h"
#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

NFmiTextParamRect::NFmiTextParamRect(const NFmiTextParamRect& theTextParamRect)
  : NFmiParamRect(*(NFmiParamRect*)&theTextParamRect)
  , itsAddInFront(theTextParamRect.itsAddInFront) 
  , itsAddAfter(theTextParamRect.itsAddAfter)  
  , fUseSelectLatinFont(theTextParamRect.fUseSelectLatinFont)  		
  , itsFont(theTextParamRect.itsFont) 
  , itsAlignment(theTextParamRect.itsAlignment)
  , itsStyle(theTextParamRect.itsStyle)
  //, itsColor(theTextParamRect.itsColor)
  //, fColorValueDependent(theTextParamRect.fColorValueDependent) 
  , fParenthesis(theTextParamRect.fParenthesis)
  , itsMapping(theTextParamRect.itsMapping ? new NFmiParamMapping(*theTextParamRect.itsMapping) : 0)
  , itsCurrentNumOfColMaps(theTextParamRect.itsCurrentNumOfColMaps)
  , itsRelCharWidth(theTextParamRect.itsRelCharWidth)
{
	for(unsigned int ind =0; ind < maxNumOfColMaps; ind++)
	{
		itsColorMapping[ind] = theTextParamRect.itsColorMapping[ind];

	}
}
//---------------------------------------------------------------------------
NFmiTextParamRect::~NFmiTextParamRect() 
{
	if(itsMapping)    //181199
		delete (NFmiParamMapping*)itsMapping;
};
/*
//---------------------------------------------------------------------------
NFmiParamRect* NFmiTextParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiTextParamRect(*this);
};
*/
//----------------------------------------------------------------------------
FmiBoolean NFmiTextParamRect::ReadRemaining(void)  //300699
{
	double r1,r2;
//	double r3,r4;
	NFmiValueString helpValueString;
	NFmiString helpString;
	double intervalEps = 0.00001; 

	switch(itsIntObject)
	{
		case dTextMapping:   //181199 tänne Letteristä
		{
			NFmiMappingInterval interval;
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))  
			{
            	*itsDescriptionFile >> itsObject;
				helpValueString = NFmiValueString(itsObject);
	            if(helpValueString.IsNumeric())
				{
					r2 = (double)helpValueString;      
                 	*itsDescriptionFile >> itsObject;
					helpString = itsObject;
				}
				else
				{
					r2 = r1;
				}


               interval.itsBottomValue = r1 - intervalEps;
               interval.itsTopValue = r2 + intervalEps;

			   NFmiString* mappingName = new NFmiString(itsObject);
			   interval.itsSymbol = *mappingName;
			   itsMapping->AddMappingInterval(interval);
			}

			ReadNext();
			break;
		}
		case dRelCharWidth:               
		{
			if (!ReadEqualChar())
				break;

			if(ReadOne(r1))  
               itsRelCharWidth = r1;

			ReadNext();
			break;
		}
		case dAddTextInFront:               
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsAddInFront = itsObject;

			ReadNext();
			break;
		}
		case dAddTextAfter:              
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsAddAfter = itsObject;

			ReadNext();
			break;
		}
		case dRGBMapping:              
		{
			itsCurrentNumOfColMaps ++;
			if(itsCurrentNumOfColMaps > maxNumOfColMaps)
			{
				itsCurrentNumOfColMaps = maxNumOfColMaps;
				*itsLogFile << "*** ERROR: värimuunosten maxlkm ylitetty"  << endl;
			}
			if (SetFive(itsColorMapping[itsCurrentNumOfColMaps-1].minValue
						,itsColorMapping[itsCurrentNumOfColMaps-1].maxValue
				        ,itsColorMapping[itsCurrentNumOfColMaps-1].color.color.rgb.red
						,itsColorMapping[itsCurrentNumOfColMaps-1].color.color.rgb.green
						,itsColorMapping[itsCurrentNumOfColMaps-1].color.color.rgb.blue))
			{
				itsColorMapping[itsCurrentNumOfColMaps-1].color.isRgb = true;
			}
			else
			{
				itsCurrentNumOfColMaps--;
				*itsLogFile << "*** ERROR: värimuunoksen lukeminen epäonnistui"  << endl;
			}

			break;
		}
		case dCMYKMapping:              
		{
			itsCurrentNumOfColMaps ++;
			if(itsCurrentNumOfColMaps > maxNumOfColMaps)
			{
				itsCurrentNumOfColMaps = maxNumOfColMaps;
				*itsLogFile << "*** ERROR: värimuunosten maxlkm ylitetty"  << endl;
			}
			if (SetSix(itsColorMapping[itsCurrentNumOfColMaps-1].minValue
						,itsColorMapping[itsCurrentNumOfColMaps-1].maxValue
				        ,itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.cyan
						,itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.magenta
						,itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.yellow
						,itsColorMapping[itsCurrentNumOfColMaps-1].color.color.cmyk.black))
			{
				itsColorMapping[itsCurrentNumOfColMaps-1].color.isRgb = false;
			}
			else
			{
				itsCurrentNumOfColMaps--;
				*itsLogFile << "*** ERROR: värimuunoksen lukeminen epäonnistui"  << endl;
			}

			break;
		}
		default: 
		{
			return NFmiParamRect::ReadRemaining();  
			break;
		}
	}
	return kTrue;
}

//---------------------------------------------------------------------------
int NFmiTextParamRect::ConvertDefText(NFmiString & object) //LW 20.11
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //2.11.00

//	if(lowChar==NFmiString("color") ||     lowChar==NFmiString("väri")) //3.11.00 pois
//		return dColor;
	if(lowChar==NFmiString("alignment")||lowChar==NFmiString("kohdistus"))
		return dAlignment;
	else if(lowChar==NFmiString("font") || lowChar==NFmiString("kirjasin")
		|| lowChar==NFmiString("fontti"))
		return dFont;
	else if(lowChar==NFmiString("size") || lowChar==NFmiString("koko"))
		return dParamSize;
	else if(lowChar==NFmiString("style") || lowChar==NFmiString("tyyli"))
		return dStyle;
	else if(lowChar==NFmiString("parenthesis") || lowChar==NFmiString("sulkuihin"))
		return dParenthesis;              //20.8
//	else if(lowChar==NFmiString("colorisvaluedependent")||lowChar==NFmiString("VäriOnArvoRiippuva")
//		||  lowChar==NFmiString("Värionarvoriippuva"))
//		return dColorValueDependent;
	else if(lowChar==NFmiString("addafter") || lowChar==NFmiString("lisääperään"))
		return dAddTextAfter;             //300699
	else if(lowChar==NFmiString("addinfront") || lowChar==NFmiString("lisääeteen"))
		return dAddTextInFront;             //300699
	else if(lowChar==NFmiString("mapping") || lowChar==NFmiString("muunnos")) //18.11.99
		return dTextMapping;
	else if(lowChar==NFmiString("rgbmapping") || lowChar==NFmiString("rgbmuunnos")) //2.11.00
		return dRGBMapping;
	else if(lowChar==NFmiString("cmykmapping") || lowChar==NFmiString("cmykmuunnos")
		                       || lowChar==NFmiString("värimuunnos")) //2.11.00
		return dCMYKMapping;
	else if(lowChar==NFmiString("charspacejustification") || lowChar==NFmiString("merkkivälinsäätö")) //30.5.01
		return dRelCharWidth;
	else
		return NFmiParamRect::ConvertDefText(object);
	
}
// 12.10---------------------------------------------------------------------------
FmiBoolean NFmiTextParamRect::WriteCode(const NFmiString& theText 
										,const NFmiRect & AbsoluteRectOfSymbolGroup
										,ofstream& theDestinationFile									
										,const NFmiString& theObjectName
										,FmiPressOutputMode theOutput) //15.3.00
		
{
	NFmiString longMinus ("\\226");

	NFmiRect numberRect = AbsoluteRectOfSymbolGroup.ToAbs(NFmiRect(TopLeft(),BottomRight()));


		NFmiPoint topLeft = numberRect.TopLeft(); //TopLeft on alaVasen! johtuuko RectScale:sta
		NFmiPoint bottomRight = numberRect.BottomRight();       //7.1
		// double height = numberRect.Height();
		// double width = numberRect.Width();
		double centerX = (bottomRight.X()-topLeft.X())/2. + topLeft.X();  //7.1
// 22.10 tarvitaan jotta vanha OsaAlue-menetelmä toimisi
		if(!fNewScaling && itsAlignment == kRight )
		{
			centerX = bottomRight.X();
		}
		else if (!fNewScaling && itsAlignment == kLeft)
		{
			centerX = topLeft.X();
		}
		// int number = theText.GetLen();

		//laskettu fontille LucideConsole 5.1.98
		// 13.10 universaali fontti-riippumaton paikan määritys
		// vain keskikohta voi heittää vähän, saishan senkin paikalleen

//		double x = centerX + numberRect.Height() * (-.31f - .302f*(theText.GetLen()-1));
		double x = centerX;
	/*
	    if(itsAlignment == kRight)
		{
			   x = bottomRight.X() + numberRect.Height() * (-.55f - .604f*(theText.GetLen()-1));
			   x += minusAdjust;
		}
		else if(itsAlignment == kLeft)  
		{
			   x =  numberRect.BottomLeft().X();  
		}
	*/	
		double y = topLeft.Y() + .147f * numberRect.Height(); //051298 oli .125; nostaa vähän mutta = illu


		// ostream saveCout = cout; //27.8.01
		// cout = theDestinationFile;

		if(theOutput == kPlainText) //4.1.01
		{
			theDestinationFile << "\t" << (char*)theText;			
		}
		else if(theOutput == kXml)
		{
			theDestinationFile << "<Text>" << endl;
			theDestinationFile << "  <Set>Tekstit" << endl;
			theDestinationFile << "  <Place>" << x << " " << y << endl;
			
		}
		else if(theOutput == kMetaLanguage)
		{
			theDestinationFile << endl << "Font " << (char*)itsFont << endl; 
			theDestinationFile << "TextAlign " << (char*)itsPsWriting.AlignmentToMeta(itsAlignment) << endl;
			theDestinationFile << "FontSize " << numberRect.Height() << endl;
			
/*			NFmiString font = NFmiString("-*-");
			font += itsFont;
			font += NFmiString("-r-normal-*-");
			font += NFmiValueString((int)numberRect.Height());
			font += NFmiString("-*-*-*-*-*-*-*-");
			theDestinationFile << (char*)font << endl;
*/
			itsPsWriting.WriteColor(MapColor(), theOutput, theDestinationFile);
			theDestinationFile << "Text \"" << (char*)theText << "\" " 
				<< x << " " << y << endl;			
		}
		else
		{
			theDestinationFile << "%*** " << (char*)theObjectName << " ALKAA ***" << endl;
			theDestinationFile << "gsave" << endl;

			if(IsPureBlack())   //12.10
				theDestinationFile << "true setoverprint" << endl;
			else
				theDestinationFile << "false setoverprint" << endl;

			itsPsWriting.WriteColor(MapColor(), theOutput, theDestinationFile);
		/*
			if(IsRGB())  //18.10.00
			{	
				theDestinationFile << GetColor().color.rgb.red << " "
					  <<rgb.green << " "
					  <<rgb.blue << " setrgbcolor" << endl;
			}
			else
			{
				theDestinationFile << itsColor.cyan << " "
					  << itsColor.magenta << " "
					  << itsColor.yellow << " "
					  << itsColor.black << " setcmykcolor" << endl;
			}
			*/
	/* Uusi makro selectlatinfont käyttöön jotta skandit tulisivat ok
	   Mika Heiskasen tutkima asia
	   12.11.98: Mikan makrolla eräitä puutteita:
	   - ei tule miinus
	   - ei sovi eräille fonteille, ainakin ZapfDingbats
	   mutta tarvitaan:
	   + skandeihin (muuten kuin sanan viimeisinä)
	*/

			if (fUseSelectLatinFont)
			{
				theDestinationFile << "/" << (char*)itsFont << " /" << (char*)itsFont << "_" << endl;
				theDestinationFile << numberRect.Height() << " selectlatinfont" << endl;
			}
			else
			{
				theDestinationFile << "/" << (char*)itsFont << " " << numberRect.Height() << " selectfont" << endl;
			}

			FmiBoolean firstIs226 = theText.GetLen() > 3 
				  && NFmiString(theText.GetCharsPtr(1,4)).IsEqual(longMinus);

			NFmiString widthString, restString;
			if(!firstIs226) //ei ongelmia
			{
				WriteShowString(x, y, theText, theText, theDestinationFile);
			}
			else
			{
				widthString = NFmiString("n"); //HUOM sama leveys kuin pitkällä miinuksella kun kerran se itse ei käy
				if(theText.GetLen()>4)
					widthString += theText.GetCharsPtr(5,theText.GetLen()-4);
				WriteShowString(x, y, widthString, longMinus, theDestinationFile);
				if(theText.GetLen()>4)
				{
					restString = theText.GetCharsPtr(5,theText.GetLen()-4); 
					WriteShowString(x, y, restString, restString, theDestinationFile);
				}

			}

			theDestinationFile << "false setoverprint" << endl; //12.10

			theDestinationFile << "grestore" << endl;
	//		theDestinationFile << "%****** NUMERO LOPPU ***" << endl;
			// cout = saveCout;
//		kDescCounter++;  //3.2
		}
	return kTrue;
};
// 031100----------------------------------------------------------------------------
FmiGenericColor NFmiTextParamRect::MapColor(void) const 
{
	return GetColor();
}

// 300699----------------------------------------------------------------------------
//otettu PressTextiltä, tässä ei kaikki optiot
NFmiString NFmiTextParamRect::Construct(NFmiString *theString) const 
{
	NFmiString string = NFmiString(*theString);
//	if(fUpperCase) string.UpperCase();
//	if(fLowerCase) string.LowerCase();
	NFmiString retString;
	if(itsAddInFront.IsValue())
		retString += itsAddInFront;
    retString += string;
	if(itsAddAfter.IsValue())
		retString += itsAddAfter;

	retString.ReplaceChars(NFmiString("_"), NFmiString(" ")); 

	return retString;
}
// 161198---------------------------------------------------------------------------
FmiBoolean NFmiTextParamRect::WriteShowString( double x
					       ,double y
					       ,const NFmiString& theWidthString
					       ,const NFmiString& theShowString
					       ,ofstream & os) const		
{
		os << x << " " << y << " moveto" << endl;
	    if(itsAlignment == kRight )
		{
		    os << "(" << (char*)theWidthString << ") " << "stringwidth" << endl;
		    os << "neg exch neg exch" << endl; 
		    os  << " rmoveto" << endl;
		}
		else if (itsAlignment == kCenter)
		{
		    os << "(" << (char*)theWidthString << ") " << "stringwidth" << endl;
		    os << "-2. div exch -2. div exch" << endl; //puolikas matka
		    os  << " rmoveto" << endl;
		}
		if(itsRelCharWidth == 0.) //300501 merkkien levennys/kavennus
				os << "(" << (char*)theShowString << ") show" << endl;
		//os << "(" << (char*)theShowString << ") " << "show" << endl;
		else    
				os << itsRelCharWidth << " 0. (" << (char*)theShowString << ") ashow" << endl;

		return kTrue;
}
/*
// 12.10.98----------------------------------------------------------------------------
FmiBoolean NFmiTextParamRect::IsPureBlack(void)
{
	return GetColor().color.cmyk.cyan <= 0. && GetColor().color.cmyk.magenta <= 0.
	    && GetColor().color.cmyk.yellow <= 0. && GetColor().color.cmyk.black >= 1.;		
}
*/
/*
//----------------------------------------------------------------------------
void NFmiTextParamRect::SetValueDepending(float value)
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
void NFmiTextParamRect::SetColor(float theC, float theM, float theY, float theK)
{
	SetCMYK(
	itsColor.cyan = theC;		
	itsColor.magenta = theM;		
	itsColor.yellow = theY;		
	itsColor.black = theK;		
}
*/
