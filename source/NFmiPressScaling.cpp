//© Ilmatieteenlaitos/Lasse.
//  Original 6.3.1998
// 
// Muutettu 250998/LW perint‰ muuttunut 
// Muutettu 261198/LW +ReadRemaining() 
// Muutettu 111298/LW +SetScale()
// Muutettu 240101/LW +dMakeAsLast 
// Muutettu 190802/LW +dTimestampDayGap
//---------------------------------------------------------------------------

#include "NFmiPressScaling.h"
#include "NFmiPressTime.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiPressScaling::NFmiPressScaling(const NFmiPressScaling& thePsSymbol)
: NFmiPressTimeDescription() //121099
, NFmiPsWriting(*(NFmiPsWriting*)&thePsSymbol)            //011200

, itsRectScale(thePsSymbol.itsRectScale)
, itsRelArea(thePsSymbol.itsRelArea)
, itsRectSize(thePsSymbol.itsRectSize)
, itsPlace(thePsSymbol.itsPlace)
, fPrintOnce(thePsSymbol.fPrintOnce)
, fScaleNotPlace(thePsSymbol.fScaleNotPlace)
, itsTimestampDayGap(thePsSymbol.itsTimestampDayGap)  
{
};

//---------------------------------------------------------------------------
NFmiPressScaling::~NFmiPressScaling() 
{
};
//---------------------------------------------------------------------------
bool NFmiPressScaling::Set( NFmiRectScale &theScale
							 , ofstream& theDestinationFile) //19.3.02	
{
	itsRectScale = theScale;	
	itsOutFile = &theDestinationFile;
	//itsOutFile = theDestinationFile;
	return true;
};

//---------------------------------------------------------------------------
bool NFmiPressScaling::SetScale(const NFmiRectScale &theScale)	
{
	itsRectScale = theScale;	
	return true;
};
//261198----------------------------------------------------------------------------
bool NFmiPressScaling::ReadRemaining(void)  
{
	double r1, r2, r3;
	NFmiValueString valueString;
	switch(itsIntObject)
	{	
		case dMakeAsLast: //24.1.01
		{
			SetWriteLast(true);
			ReadNext();
			break;
		}
		case dRotate:  //VakioKuvalla oma
		{
			if(!ReadEqualChar())
				return false;

			if(ReadDouble(r1)) 
			{
				if(r1 != 90.)
				{
					*itsLogFile << "WARNING: vain 90 asteen kierto sallittu, ei " << r1 << endl;
					r1 = 90.;
				}
				itsRotatingAngle = r1;

				*itsDescriptionFile >> itsObject;
			    valueString = itsObject;
				if(valueString.IsNumeric())
				{
					r2 = (double)valueString;

					if(ReadDouble(r3))
					{
					    Place(NFmiPoint(r2,r3));  //riitt‰‰ kun antaa joko Paikan tai RotPaikan
						itsRotatingPoint = NFmiPoint(r2,r3);
					}
					ReadNext();
				}
				else
				{
				    itsString = valueString;
					itsIntObject = ConvertDefText(itsString);
				}
			}
			else
				ReadNext();
			break;
		}
		case dFileTimestamp:  //19.8.02
		{
			itsTimestampDayGap = 0; //toista avainsanaa pit‰isi ohjelmoida jos vaikka eilist‰ teksti‰ (muuttuja kelpaa edlleen)   
			ReadNext();
			break;
		}
		default:
		{
			NFmiPressTimeDescription:: ReadRemaining(); //121099
			break;
		}
	}
	return true;
}
//----------------------------------------------------------------------------
int NFmiPressScaling::ConvertDefText(NFmiString & object)  
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //24.1.01

	if(lowChar==NFmiString("size") || lowChar==        NFmiString("koko"))
		return dSymbolSize;                       
	else if(lowChar==NFmiString("makeaftersegments") || lowChar==NFmiString("teesegmenttienj‰lkeen")) 
		return dMakeAsLast;                       
	else if(lowChar==NFmiString("relplace") || lowChar==NFmiString("osaalue")) 
		return dRelSymbolSize;
	else if(lowChar==NFmiString("printOnce") || lowChar==NFmiString("tulostakerran"))
		return dPrintOnce;
	else if(lowChar==NFmiString("place") || lowChar==    NFmiString("paikka"))
		return dSymbolPlace;
	else if(lowChar==NFmiString("placemove") || lowChar==NFmiString("paikansiirto")
	       ||lowChar==NFmiString("suhtpaikka")||lowChar==NFmiString("suhteellinenpaikka")) //1.10.01
		return dPsPlaceMove;
	else if(lowChar==NFmiString("rotate") || lowChar==   NFmiString("kierto"))
		return dRotate;                              //261198
	else if(lowChar==NFmiString("stationlist")   || lowChar==NFmiString("asemataulukko")
		|| lowChar==NFmiString("asemiensijoitusalue"))
		return dEnd;  //11.3 t‰m‰ ei ole oikea tapa, mutta karsii aika useita virheit‰
	                  // jotka aihetuvat m‰‰rittelyn v‰‰r‰st‰ j‰rjestyksest‰
	else if(lowChar==NFmiString("timestamp") || lowChar==NFmiString ("aikaleima")) //19.8.02
		return dFileTimestamp;
	else
		return NFmiPressTimeDescription::ConvertDefText(object); //121099
}
//16.8.02----------------------------------------------------------------------------
bool NFmiPressScaling:: AddTimeStamp(NFmiString& theFile, const NFmiString& theFormat) const 
{
	if(theFormat.IsValue())
	{		 
		NFmiPressTime pressTime;
		NFmiString string1(pressTime.InterpretToStr(theFormat));
		NFmiString string2(theFile);

		theFile = string1+string2;
	}
	
	return true;
}
//25.1.01----------------------------------------------------------------------------
NFmiFileString NFmiPressScaling:: CreatePath(NFmiString defDir, NFmiString givenPath
			           ,NFmiString givenDir, NFmiString givenFile, NFmiString theExtension) //25.1.01
{
   NFmiFileString fileStr;
   if (!givenPath.IsValue())
   {
	   fileStr = GetHome();
	   fileStr += kFmiDirectorySeparator;
	   fileStr += defDir;
	   fileStr += kFmiDirectorySeparator;
	   fileStr += givenFile;    
   }
   else
   {
	   fileStr = givenPath;
	   fileStr.NormalizeDelimiter();
	// 1.2.99 kenoviiva loppuun jos ei m‰‰rittelyss‰
	   if(!(fileStr.GetChars(givenPath.GetLen(),1) == NFmiString(kFmiDirectorySeparator)))
	   {
		   fileStr += kFmiDirectorySeparator;
	   }

	   fileStr += givenFile;    
   }
   if(givenDir.IsValue())  
   {
	    fileStr.ReplaceDirectory(givenDir);	
   }

   if(theExtension.IsValue() && !fileStr.HasExtension())
	    fileStr.Extension((char*)theExtension);
   
   return fileStr;
}
//----------------------------------------------------------------------------
bool NFmiPressScaling::	WritePS(const NFmiPoint& place, FmiPressOutputMode theOutput)
{ 
	itsPlace = place;
	return WritePS(theOutput);
}
//----------------------------------------------------------------------------
void NFmiPressScaling::	ScalePlotting(void)
{ 
	NFmiPoint scaledPlace;
	NFmiPoint sizeScaling = itsRectScale.GetScaling();
	if(!fScaleNotPlace)    //9.9
	     scaledPlace = itsRectScale.Scale(itsPlace);
	else
         scaledPlace = itsPlace;

	double scaledXSizeHalf = itsRectSize.X() * sizeScaling.X() / 2.;
	double scaledYSizeHalf = itsRectSize.Y() * sizeScaling.Y() / 2.;
	NFmiRect absRect(scaledPlace.X() - scaledXSizeHalf  //left
					,scaledPlace.Y() + scaledYSizeHalf  //top
					,scaledPlace.X() + scaledXSizeHalf  //right
					,scaledPlace.Y() - scaledYSizeHalf);//bottom
    NFmiRect absRelRect = absRect.ToAbs(itsRelArea);    //24.2
    itsWriteScale.SetEndScales(absRelRect);
}
