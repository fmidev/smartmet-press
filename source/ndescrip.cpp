// © Ilmatieteenlaitos/Lasse.
// Original 12.12.1997
// 
//
// Muutettu 221297/LW kommentit yleisemmiksi	
// Muutettu 070198/LW #End suomennettu	
//Muutettu 150198/LW +itsValueHelpString, +ReadFloat(), +ReadInt()
//Muutettu 160198/LW olion loppu ei enää #Loppu.. vaan luokalle tuntematon #...
//Muutettu 020298/LW
//Muutettu 090298/LW unsigned short -> long
//Muutettu 250298/LW virheet: cout->itsLogFile
//Muutettu 290199/LW määrittelytekstissä mrkkijonot "-parin väliin myös ->
//                   mahdollistaa välilyönnin käytön 
//Muutettu 050299/LW +ReadMaxTwoDoubleToDelimiter()
//---------------------------------------------------------------------------
#include "ndescrip.h"
#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

static unsigned char kSecondC = '\"';

//23.9.99---------------------------------------------------------------------------
//20.10.99 ei tarvita enää multiMappingissä
FmiBoolean NFmiDescription::ReadMaxTwoFloatToDelimiter (float &retValue1, float &retValue2)
{  //false kun löytynyt muu stringi kuin pilkku eli tiedoston nimi, jolloin kutsuva ohjelma lopettaa luupin
	retValue1 = kFloatMissing;
	retValue2 = kFloatMissing;

	*itsDescriptionFile >> itsObject;
	itsValueHelpString = itsObject;
	if(itsValueHelpString.IsNumeric())
	{
		retValue1 = (float)itsValueHelpString;
	}
	else 
	{
		if(itsValueHelpString == NFmiString(","))
			return kTrue;
		else if(itsValueHelpString == NFmiString("-,"))
		{
			return kTrue;
		}
		else if(itsValueHelpString == NFmiString("-"))
		{
			//floatmissing, on jo
			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
			if(itsString == NFmiString(","))
				return kTrue;
			else
				return kFalse;
		}
		else //tiedostonimi
			return kFalse;
	}

	*itsDescriptionFile >> itsObject;
	itsValueHelpString = itsObject;
	if(itsValueHelpString.IsNumeric())
	{
		retValue2 = (float)itsValueHelpString;
		*itsDescriptionFile >> itsObject;   //luetaan aina seuraava stringi itsObjektiin
		itsString = itsObject;
		if(itsString == NFmiString(","))
			return kTrue;
		else
			return kFalse;
	}
	else if(itsValueHelpString == NFmiString("-,"))
	{
		return kTrue;
	}
	else if(itsObject == NFmiString("-"))
	{
		//floatmissing, on jo
		*itsDescriptionFile >> itsObject;
		itsString = itsObject;
		if(itsString == NFmiString(","))
			return kTrue;
		else
			return kFalse;
	}
	else if(itsValueHelpString == NFmiString(","))
		return kTrue;
	else
		return kFalse;

}
//261198---------------------------------------------------------------------------
FmiBoolean NFmiDescription::Set1Double (double& theMember)
{
	FmiBoolean ok = kFalse;
	double aDouble;
	if (ReadEqualChar())
	{
		if(ReadDouble(aDouble))
		{
		   theMember = aDouble;
		   ok = kTrue;
		}
		ReadNext();
	}
	return ok;
}
//171198---------------------------------------------------------------------------
FmiBoolean NFmiDescription::Set1Long (long& theMember)
{
	FmiBoolean ok = kFalse;
	long aLong;
	if (ReadEqualChar())
	{
		if(ReadLong(aLong))
		{
		   theMember = aLong;
		   ok = kTrue;
		}
		ReadNext();
	}
	return ok;
}
//171198---------------------------------------------------------------------------
FmiBoolean NFmiDescription::Set1UnsignedLong (unsigned long& theMember)
{
	FmiBoolean ok = kFalse;
	unsigned long aLong;
	if (ReadEqualChar())
	{
		if(ReadUnsignedLong(aLong))
		{
		   theMember = aLong;
		   ok = kTrue;
		}
		ReadNext();
	}
	return ok;
}
//9.9---------------------------------------------------------------------------
void NFmiDescription::ReadNext (void)
{

	*itsDescriptionFile >> itsObject;
	itsString = itsObject;
	itsIntObject = ConvertDefText(itsString);
}
//290199---------------------------------------------------------------------------
// mahdollistaa myös "-merkeillä ympäröidyn merkkijonon -> välilyönti voi sisältyä
NFmiString NFmiDescription::ReadString (void)
{
	*itsDescriptionFile >> itsObject;

	if(itsObject[0] == kSecondC) //290199 etsitään "-merkin pari, kaikki luettuun muuttujaan
	{
		char helpChar [255];
		NFmiString helpObject;

		helpObject = itsObject;
		helpObject = helpObject.GetChars(2, helpObject.GetLen()-1);
		short loop = 0;
		while (helpObject.Search(&kSecondC) <=0 && loop < 11) //korkeintaan 10 (erillistä)välilyönti
		{		                                         // tarkistus jotta ei jouduta luuppiin
				*itsDescriptionFile >> helpChar;
				loop++;
				helpObject += NFmiString(" "); // HUOM vain yksi völilyönti aina
				helpObject += helpChar;
		}

		if(loop >= 11)
				*itsLogFile << "*** ERROR: lainausmerkiltä puuttuu pari: " << (char*)helpObject  << endl; 

        return helpObject.GetChars(1, helpObject.GetLen()-1);

	}
	else
		return itsObject;
}
//9.9---------------------------------------------------------------------------
FmiBoolean NFmiDescription::ReadEqualChar (void)
{  //tarkistetaan että todella on annettu "=" -merkki
	*itsDescriptionFile >> itsObject;
	if(itsObject[0] == 61)
	{
		return kTrue;
	}
	itsValueHelpString = itsObject;
	*itsLogFile << "*** ERROR: yhtäkuin-merkki (=) puuttuu: " << (char*)itsValueHelpString << endl;
	ReadNext();
	return kFalse;
}
//171198---------------------------------------------------------------------------
FmiBoolean NFmiDescription::ReadUnsignedLong (unsigned long &retValue)
{
	*itsDescriptionFile >> itsObject;
	itsValueHelpString = itsObject;
	if(itsValueHelpString.IsNumeric())
	{
		retValue = (unsigned long)(int)itsValueHelpString;
		return kTrue;
	}
	retValue = 0;
	*itsLogFile << "*** ERROR: pitää olla luku: " << (char*)itsValueHelpString << endl;
	return kFalse;
}
/* ei onnaa täällä, yritetään Siirtää headeriin (markon ohje)
//---------------------------------------------------------------------------
template <class Type>
FmiBoolean NFmiDescription::ReadOne (Type retValue)
{
	*itsDescriptionFile >> itsObject;
	itsValueHelpString = itsObject;
	if(itsValueHelpString.IsNumeric())
	{
		retValue = (Type)(int)itsValueHelpString;
		return kTrue;
	}
	retValue = 0;
	*itsLogFile << "ERROR: pitää olla luku: " << (char*)itsValueHelpString << endl;
	return kFalse;
}
*/
//---------------------------------------------------------------------------
FmiBoolean NFmiDescription::ReadLong (long &retValue, FmiBoolean errorReport) //24.9.99
{
	*itsDescriptionFile >> itsObject;
	itsValueHelpString = itsObject;
	if(itsValueHelpString.IsNumeric())
	{
		retValue = (long)(int)itsValueHelpString;
		return kTrue;
	}
	retValue = 0;
	if(errorReport)
		*itsLogFile << "*** ERROR: pitää olla luku: " << (char*)itsValueHelpString << endl;

	return kFalse;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiDescription::ReadDouble (double &retValue)
{
	*itsDescriptionFile >> itsObject;
	itsValueHelpString = itsObject;
	if(itsValueHelpString.IsNumeric())
	{
		retValue = (double)itsValueHelpString;
		return kTrue;
	}
	retValue = 0.;

	*itsLogFile << "*** ERROR: pitää olla luku: " << (char*)itsValueHelpString << endl;
	return kFalse;
}
//---------------------------------------------------------------------------
FmiBoolean NFmiDescription::Read4Double (double &retValue1,double &retValue2
										,double &retValue3,double &retValue4)
{
	if((((ReadDouble(retValue1)) && ReadDouble(retValue2))  //sulut jotta luku
	&& ReadDouble(retValue3)) && ReadDouble(retValue4))    // tapahtuisi varmasti 
	{                                                      //oikeassa järjestyksesssä
		return kTrue;
	}
	return kFalse;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
FmiBoolean NFmiDescription::Read2Double (double &retValue1,double &retValue2)
{
	if((ReadDouble(retValue1)) && ReadDouble(retValue2))
	{
		return kTrue;
	}
	return kFalse;
}
//---------------------------------------------------------------------------
int NFmiDescription::ConvertDefText(NFmiString & object) 
{
	unsigned long len = object.GetLen(); 
	if(len >= 2 && 
		NFmiString(object.GetCharsPtr(1,2)) == NFmiString("/*"))   //22.12
	{
		if(NFmiString(object.GetCharsPtr(len-1,2)) != NFmiString("*/"))
			itsCommentLevel++;
		return dComment;
	}
	else if(len >= 2 && 
		NFmiString(object.GetCharsPtr(len-1,2)) == NFmiString("*/")) //22.12
	{
		itsCommentLevel--;
		return dEndComment;
	}
	else if(len >= 2 &&                            // 31.12 ekasta tänne
		NFmiString(object.GetCharsPtr(1,2)) == NFmiString("//"))   //22.12
		return dComment; 
/*	else if(len >= 4 &&          //bugi stringissä, seuraava rivi ei yksin riittänyt
		(NFmiString(object.GetCharsPtr(1,4)) == NFmiString("#End") 
		|| NFmiString(object.GetCharsPtr(1,4)) == NFmiString("#Lop")  //2.2 takaisin
		|| NFmiString(object.GetCharsPtr(1,4)) == NFmiString("#END") //EI EI
		|| NFmiString(object.GetCharsPtr(1,4)) == NFmiString("#LOP")))  //2.2
*/		
	else if(NFmiString(object.GetCharsPtr(1,1)) == NFmiString("#"))  //16.1 seuraavaksi ylemmälle tasolle
	{
		return dEnd;
	}
	else
	{
		return dOther;
	}
}

