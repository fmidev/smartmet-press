//© Ilmatieteenlaitos/Lasse.
//  Original 10.3.1998 

//Muutettu 300398/LW oletus = Suomen paik. aika
//Muutettu 161098/LW +PsPlaceMove; ReadNext() ym
//Muutettu 161098/LW uusia aikaformaatteja
//Muutettu 150300/LW erilaisia output-formaatteja mahdollistettu
//Muutettu 291100/LW +lyhennetyt kohdistusformaatit

//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressTimeText.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//int Convert(NFmiString & object);
/*
NFmiPressTimeText::NFmiPressTimeText(const NFmiTimeParamRect& theTimeParamRect)
: NFmiTextParamRect()
,itsFormat(theTimeParamRect.itsFormat)
{
  Set(theTimeParamRect.GetDataIdent(),NFmiRect(theTimeParamRect.TopLeft(),theTimeParamRect.BottomRight())); 
  SetProducer(theTimeParamRect.GetProducer());
  SetLogFile (theTimeParamRect.GetLogFile());
  SetTextAttributes(theTimeParamRect.GetFont() 
	                  ,theTimeParamRect.GetAlignment()      
					  ,theTimeParamRect.GetStyle()
					  ,theTimeParamRect.GetColor()
					  ,theTimeParamRect.GetColorValueDepending());
 }
 */
//---------------------------------------------------------------------------
NFmiPressTimeText::~NFmiPressTimeText() 
{
};
/*
//---------------------------------------------------------------------------
NFmiPressTimeText* NFmiPressTimeText::Clone() const
{
	return (NFmiParamRect *) new NFmiPressTimeText(*this);
};
*/
//----------------------------------------------------------------------------
void NFmiPressTimeText:: SetLanguage(FmiLanguage newLanguage)		
{
	if(newLanguage == kChinese && (itsOrigFormat == kShortWeekDay || itsOrigFormat == kI))
	{
		itsFormat = kI;
		itsFont = NFmiString("Cviikko");
	}
	else if(itsLanguage == kChinese) //mutta uusi ei
	{
		itsFormat = itsOrigFormat;
		itsFont = itsOrigFont;
	}
	itsLanguage = newLanguage;
}; 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool NFmiPressTimeText::ReadDescription(NFmiString& retString) //16.1
{	//16.10 siivottu: ReadNext() ym		 
//	NFmiString string;
	NFmiValueString valueString;
	NFmiString helpString;
//	char object[255], ch;
//	int iobject;
//	long long1;                 
	double r1,r2;
	double xmin,xmax,ymin,ymax;	  
	xmin = ymin = 0;				 
	xmax = ymax = 1;

	itsFont = NFmiString("Times-Roman"); //30.9 
  
	*itsDescriptionFile >> itsObject;
	itsString = itsObject;                     
	itsIntObject = ConvertDefText(itsString);
	
	while(itsIntObject != dEnd || itsCommentLevel)
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; //9.12
 	if(itsLoopNum > itsMaxLoopNum)  //2.2
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #AikaTekstiss‰" << endl;
	    retString = itsString;
		return isFalse;
	}
    itsLoopNum++;
	switch(itsIntObject)
	{
			case dOther:	  
			{    
				if(itsLogFile)
					*itsLogFile << "*** ERROR: Tuntematon sana #AikaTekstiss‰: " << (char*)itsObject << endl;  
				ReadNext();
				break;
			}
			case dComment:	  
			case dEndComment:	  
			{
				ReadNext();
				break;
			}
		case dPrintOnce:
		{
			SetPrintOnceOn();

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
			   itsRotatingPoint = NFmiPoint(r1,r2); //riitt‰‰ kun antaa joko Paikan tai RotPaikan
			}
			ReadNext();
			break;
		}
		case dPsPlaceMove:   //16.10
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
/*		case dTextColor:
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
		*/
		case dTextAlignment:
		{
			if (!ReadEqualChar())
				break;

			*itsDescriptionFile >> itsObject;   //29.11.00 lyhennetyt mukaan
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
			else
				*itsLogFile << "*** ERROR: Tuntematon kohdistus ajalla: " << (char*)itsObject << endl;  


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
		case dTextStyle:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsStyle = itsObject;

			ReadNext();
			break;
		}
		case dTimeTextFormat:	//1.3.2000 lis‰yksi‰	  
		{
			if (!ReadEqualChar())
				break;

			itsFormat = ReadTimeFormat(); //27.3.00

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

			itsIntObject = ConvertDefText(itsString);
			break;
		}
		case dUpperCase:            //081298
		{
			fUpperCase = true;

			ReadNext();
			break;
		}
		case dLowerCase:            //081298
		{
			fLowerCase = true;

			ReadNext();
			break;
		}
		case dAddInFront:            //081298   
		{
			if (!ReadEqualChar())
				break;
//			*itsDescriptionFile >> itsObject;
//			itsAddInFront = itsObject;
			itsAddInFront = ReadString();

			ReadNext();
			break;
		}
		case dAddAfter:            //081298   
		{
			if (!ReadEqualChar())
				break;
//			*itsDescriptionFile >> itsObject;
//			itsAddAfter = itsObject;
			itsAddAfter = ReadString();

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

	itsOrigFormat = itsFormat;  //24.3.00
	itsOrigFont = itsFont;
	if(itsLanguage == kChinese && (itsFormat == kShortWeekDay || itsFormat == kI)) //+24.3.00
	{
		itsFormat = kI;
		itsFont = NFmiString("Cviikko");
	}
	NFmiRect *rect;
	rect = new NFmiRect(xmin,ymax,xmax,ymin);

	retString = itsString;
	return true;
}

//---------------------------------------------------------------------------
int NFmiPressTimeText::ConvertDefText(NFmiString & object) //LW 20.11
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //20.3.00 kaikille pit‰isi sallia vapaa isot/pienet kirj.
	if(lowChar==NFmiString("format") || lowChar==NFmiString("formaatti"))
		return dTimeTextFormat;
	else
		return NFmiPressText::ConvertDefText(object);
	
}

