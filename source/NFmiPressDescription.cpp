// © Ilmatieteenlaitos/Lasse.
// Original 12.1.1998
// Muutettu 240998/LW +Path()
// Muutettu 171198/LW +NumerotPitk‰ll‰Viivalla, NumerotLyhyell‰Viivalla
// Muutettu 250800/LW +espanja ja saksa
// Muutettu 140502/LW +ranska
// 
//---------------------------------------------------------------------------
#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiPressDescription.h"
#include "NFmiEnumConverter.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//171198----------------------------------------------------------------------------
bool NFmiPressDescription::ReadRemaining(void)  
{  //t‰ss‰ tulisi olla kaikki mit‰ ConvertDefText:ss‰kin
	FmiRGBColor rgb;
	double r1,r2,r3,r4;
	unsigned long helpLong;
	switch(itsIntObject)
	{
		
		case dMaskNumber:     //7.9.01 t‰nne vaikka vain Produktissa ja segmentiss‰ k‰ytˆss‰
		{
			if (!ReadEqualChar())
				break;

			NFmiString maskName = ReadString();
			string stdString(maskName);
			NFmiEnumConverter converter(kRoadRegions);
			helpLong = converter.ToEnum(stdString);

			if(helpLong == 0)
				*itsLogFile << "*** ERROR: Not valid Mask: " << (char*)maskName << endl;  

			else
				itsEnvironment.SetMaskNumber(helpLong);

			ReadNext();

/*
			if (SetOne (helpLong))
			{
				itsEnvironment.SetMaskNumber(helpLong);

				NFmiEnumConverter converter(kRoadRegions);
				string region = converter.ToString(helpLong);
				string region2 = converter.ToString(204);
				int num1 = converter.ToName(region);
				int num2 = converter.ToName(region2);
				string testsS("paha");
				int num11 = converter.ToName(testsS);
				
				NFmiEnumConverter converter2(kParamNames);
				string par1 = converter2.ToString(4);
				string par2 = converter2.ToString(10);
				int num3 = converter2.ToName(par1);
				int num4 = converter2.ToName(par2);
				string testsS2("pahis");
				NFmiString fmiStr(testsS2);
				int num14 = converter2.ToName(testsS2);
			}
	*/

			break;
		}
		
		case dLanguage:   //28.8.2000 t‰nne PressProductista
		{
			if (!ReadEqualChar())
				break;

			itsLanguage = ReadLanguage(); 

			ReadNext();
			break;
		}
		case dLongNumberMinus:  
		{
			itsEnvironment.SetLongNumberMinus();

			ReadNext();
			break;
		}
		case dShortNumberMinus:  
		{
			itsEnvironment.SetShortNumberMinus();

			ReadNext();
			break;
		}
		case dRGBColor:  
		{
			if (SetThree (rgb.red, rgb.green,rgb.blue))
				itsEnvironment.SetRGB(rgb);
			break;
		case dColor: //3.11.00 t‰nne ja muutettu
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

		}
	}
	return true;
}
//---------------------------------------------------------------------------
FmiLanguage NFmiPressDescription::ReadLanguage(void) 
{
	NFmiString helpString;     // pit‰‰ viel‰ yll‰pit‰‰ myˆs PressProduktissa(25.8.00)
	FmiLanguage language = kFinnish;

	*itsDescriptionFile >> itsObject;          
	helpString = itsObject;
	helpString.LowerCase();
	if (helpString == NFmiString ("suomi") || helpString == NFmiString ("finnish"))
       language = kFinnish;
	else if (helpString == NFmiString ("ruotsi") || helpString == NFmiString ("swedish"))
       language = kSwedish;
	else if (helpString == NFmiString ("englanti") || helpString == NFmiString ("english"))
       language = kEnglish;
	else if (helpString == NFmiString ("saksa") || helpString == NFmiString ("germany"))
       language = kGermany;
	else if (helpString == NFmiString ("espanja") || helpString == NFmiString ("spanish"))
       language = kSpanish;
	else if (helpString == NFmiString ("ranska") || helpString == NFmiString ("french"))  //14.5.02
       language = kFrench;
	else if (helpString == NFmiString ("kiina") || helpString == NFmiString ("chinese"))
       language = kChinese;
	else
		*itsLogFile << "*** ERROR: Tuntematon kieli: " << (char*)helpString << endl; 
    
	return language;
}
//---------------------------------------------------------------------------
int NFmiPressDescription::ConvertDefText(NFmiString & object) 
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //18.10.00 kaikille pit‰isi sallia vapaa isot/pienet kirj.
	
	short len = lowChar.GetLen();
    if (!itsCommentLevel
		&& lowChar.GetChars(1,1) == NFmiString("#")
		&& ((len > 8 && lowChar.GetChars(len-4,5) == NFmiString("loppu")) //#LOPPU j‰tt‰is luuppiin
		||  (len > 6 && lowChar.GetChars(len-2,3) == NFmiString("end"))))
		
	{	
		ReadNext(); 
		return dEnd;
	}
	else if(lowChar==NFmiString("mask") || lowChar==NFmiString("maski"))
		return dMaskNumber;
	else if(lowChar==NFmiString("producer") || lowChar==NFmiString("tuottaja"))
		return dProducer;
	else if(lowChar==NFmiString("longminus") || lowChar==NFmiString("numerotpitk‰ll‰viivalla")
		|| lowChar==NFmiString("pitk‰ll‰viivalla"))
		return dLongNumberMinus;
	else if(lowChar==NFmiString("shortminus") || lowChar==NFmiString("numerotlyhyell‰viivalla")
		|| lowChar==NFmiString("lyhyell‰viivalla"))
		return dShortNumberMinus;
	else if(lowChar==NFmiString("language")     || lowChar==NFmiString("kieli")) //28.8.00 t‰nne
		return dLanguage;
	else if(lowChar==NFmiString("rgbcolor")     || lowChar==NFmiString("rgbv‰ri")) //18.10.00 
		return dRGBColor;
	else if(lowChar==NFmiString("color") ||     lowChar==NFmiString("v‰ri")
									      ||lowChar==NFmiString("cmykv‰ri")) //3.11.00 t‰nne
		return dColor;
	else 
		return NFmiDescription :: ConvertDefText(object);
}
// 12.10.98----------------------------------------------------------------------------
bool NFmiPressDescription::IsPureBlack(void)
{
	return GetColor().color.cmyk.cyan <= 0. && GetColor().color.cmyk.magenta <= 0.
	    && GetColor().color.cmyk.yellow <= 0. && GetColor().color.cmyk.black >= 1.;		
}
