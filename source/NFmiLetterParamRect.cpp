//© Ilmatieteenlaitos/Lasse.
//  Original 10.2.1998 kopioitu NFmiTextParamRect:stä

//Muutettu 030498/LW muunnoksen paluuarvo testataan
//Muutettu 070598/LW Mikan makro käyttöön jotta saataisiin sakndit ok 
//Muutettu 220299/LW puuttuvallekin koodausmahdollisuus, - haluttaessa pitkäksi 
//Muutettu 010799/LW tekstiä eteen ja taakse 
// Muutettu 130999/LW Muunnosarvot luetaan nyt double-tyyppisinä eikä integereinä
// Muutettu 230999/LW moniParamMuunnos mahdolliseksi
// Muutettu 181199/LW Muunnos (ei moniMuunnos) siirretty emolla jotta Numeronkin käytössä
// Muutettu 290200/LW copykonstr. siivottu
//
//---------------------------------------------------------------------------

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiLetterParamRect.h"
#include "NFmiPressParam.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

NFmiLetterParamRect::NFmiLetterParamRect(const NFmiLetterParamRect& theLetterParamRect)
: NFmiTextParamRect(theLetterParamRect) //20.3.02 miksi noin konstikkaasti
//: NFmiTextParamRect(*(NFmiLetterParamRect*)&theLetterParamRect)
{
}
//---------------------------------------------------------------------------
NFmiLetterParamRect::~NFmiLetterParamRect() 
{
//	if(itsMapping)
//		delete (NFmiParamMapping*)itsMapping;
};
//---------------------------------------------------------------------------
NFmiParamRect* NFmiLetterParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiLetterParamRect(*this);
};
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
bool NFmiLetterParamRect::ReadDescription(NFmiString& retString) 
{			 
	NFmiString helpString;
    NFmiValueString helpValueString;
	long long1;
	double r1,r2,r3,r4;
//	double intervalEps = 0.00001; //13.9.99
	itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));
	itsMapping = new NFmiParamMapping;

	SetPreReadingTimes();  //29.6

	itsFont = NFmiString("Helvetica");  //30.9 
	itsAlignment = kCenter; //5.10

	itsMultiMapping = 0; //27.9.99
	itsModifier = kNoneModifier; //15.2.00

	//1.10.01 kopsattu vain numerosta,jotta oletussuht.paikka = (0,0) toimii 
	itsRelRect -= NFmiPoint(1., 1.);
	ReadNext();
	
	while(itsIntObject != 9999 || itsCommentLevel) 
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; 
 	if(itsLoopNum > itsMaxLoopNum)  
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Tekstisssä" << endl;
	    retString = itsString;
		return isFalse;
	}
    itsLoopNum++;
	switch(itsIntObject)
	{
			case dOther:	  
			{    
				if(itsLogFile)
					*itsLogFile << "*** ERROR: Tuntematon sana #Tekstisssä: " << (char*)itsObject << endl;  
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
/*
		case dLetterMapping:
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
*/
		case dAlignment:
		{
			if (!ReadEqualChar())
				break;

			*itsDescriptionFile >> itsObject;
			itsString = itsObject;
			if (itsString == NFmiString ("Center") || itsString == NFmiString ("KeskiPiste")
				|| itsString == NFmiString ("Keski"))
               itsAlignment = kCenter;
			else if (itsString == NFmiString ("Right") || itsString == NFmiString ("OikeaLaita")
				|| itsString == NFmiString ("Oikea"))
               itsAlignment = kRight;
			else if (itsString == NFmiString ("Left") || itsString == NFmiString ("VasenLaita")
				|| itsString == NFmiString ("Vasen"))
               itsAlignment = kLeft;      //7.9
			else
				*itsLogFile << "*** ERROR: Tuntematon kohdistus #Tekstissä: " << (char*)itsObject << endl;  

			ReadNext();
			break;
		}
		case dFont:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsFont = itsObject;
			if (itsFont == NFmiString("ZapfDingbats"))  //121198
				fUseSelectLatinFont = false;

			ReadNext();
			break;
		}
		case dStyle:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsStyle = itsObject;

			ReadNext();
			break;
		}
		case dPlaceMove:  //2.10
		{
			if (!ReadEqualChar())
				break;

			if(Read2Double(r1,r2))
			{ 
			//pois 30.10.01 miksi taas muuttuu tulos ???????????
//			   r1 -= 40.; //251198 miksi ?????????????
//			   r2 -= 40.;
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
			{
			   itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
			}
			ReadNext();
			break;
		}
		case dParamSize:  //1.10
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{           
			   itsRelRect.Inflate(-(c40-r1)/(c40*2));
			}

			ReadNext();
			break;
		}
		case dProducer: 
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
				long1 = 0; //dummy
//               itsProducer = long1;
			
			ReadNext();
			break;
		}
/*		case dParam: 
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
               itsIdentPar = long1;
			
			ReadNext();
			break;
		}
		*/
		case dColorValueDependent: 
		{	
//			fColorValueDependent = isTrue;

			ReadNext();
			break;
		}

		case dRelDay:     //7.1
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))    //15.1
               itsFirstDeltaDays = (unsigned short)(long1+ itsEnvironment.GetDayAdvance()); //23.5.02 +GetDayAdvance); ;

			ReadNext();
			if(itsLogFile)
					*itsLogFile << "*** ERROR: Päiviä ei voi asettaa #Tekstissä"  << endl;  
			break;
		}
/*		case dRelYear:     //7.1
		{
			if (!ReadEqualChar())
				break;
//			if(ReadLong(long1))    //15.1
//               itsFirstDeltaYears = (unsigned short)long1;

			ReadNext();
			if(itsLogFile)
					*itsLogFile << "ERROR: Vuosia ei voi asettaa #Tekstissä"  << endl;  
			break;
		}
		*/
		case dHour:       //7.1
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))    //15.1
               itsFirstPlotHours = (unsigned short)long1;

			ReadNext();
			break;
		}
		default:  //090299
		{
			ReadRemaining();  
			break;
		}
	}
	} //while
	
	//flush viimeinen takaisin sreamiin! Miten?
	SetPostReadingTimes();

//171198
	if(fNewScaling)
	   itsRelRect += NFmiPoint(1.,1.);
	Set(NFmiDataIdent(NFmiParam(itsIdentPar),240/*=dummy NFmiProducer(itsProducer)*/)
	   ,NFmiRect(itsRelRect));

	retString = itsString;
	return true;
}
//---------------------------------------------------------------------------
int NFmiLetterParamRect::ConvertDefText(NFmiString & object) //LW 20.11
{
/*	if(object==NFmiString("Mapping") 
		|| object==NFmiString("ArvoistaKirjain")|| object==NFmiString("Muunnos")) //1.4
		return dLetterMapping;
	else */
		return NFmiTextParamRect::ConvertDefText(object);
	
}
//---------------------------------------------------------------------------
bool	NFmiLetterParamRect::WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
										,NFmiFastQueryInfo* theQI
										,ofstream& theDestinationFile									
										,FmiPressOutputMode theOutput) //15.3.00									

{
   itsCurrentSegmentTime = ((NFmiQueryInfo*)theQI)->Time(); //4.9.01 
   itsCurrentTime = itsCurrentSegmentTime;//16.2.2000

	NFmiString hString;
	float value = 0;

	if(!SetRelativeHour(theQI,NFmiString("#Teksti"))) 
		return isFalse;

	if(itsMultiMapping) 
	{
        if(!ReadCurrentValueArray (theQI))  //25.5.01
		   return false;
	}
	else
	{
	    if(!PointOnParam(theQI, GetDataIdent().GetParam())) 
		{
		   return false;
		}
		if(!ReadCurrentValue(theQI, itsCurrentParamValue))
		{
			return false;        
		}
		value = itsCurrentParamValue;
	}

   if(!ActiveTimeIndex(itsPressParam->GetCurrentStep())) //26.9.01
   {
	   if(itsLoopActivity.bypassWithValue)  //2.10.01
		   value = itsLoopActivity.bypassValue;
	   else
		   return true;
   }

//	if(value != kFloatMissing) //220299 pitäähän puuttuvaakin voida koodata
//	{											 
//		SetValueDepending(value);
		MapColor(); //3.11.00 korvaa ed.
		NFmiString* mapString;

		if (itsMultiMapping)  //270999
		{
			bool missingFound;
			mapString = itsMultiMapping->Map(itsCurrentParamArray, missingFound);
			if(missingFound)
			{
				itsNumOfMissing++;
				return false;
			}
//			isScaled = false;         //onko tässä ollut niinkuin symbolissa
		}
		else 
			 mapString = itsMapping->Map(value); 

//		NFmiString objectMapString(*mapString); //edellisestä vain olio

		if(mapString && !(*mapString == NFmiString("None"))) 
		{
			NFmiString str;  
//			if(fParenthesis)     EI TOIMI        
//				str = NFmiString("(");

			hString = NFmiString (*mapString);   //220299 pitäisi olla yleisempi
			if(itsEnvironment.GetLongNumberMinus() && hString == NFmiString("-"))
				hString = NFmiString("\\226");
			str += hString;

	//300699 +Construct (lisäykset eteen ja perään)
			return WriteCode(Construct(&str), AbsoluteRectOfSymbolGroup, 
			             theDestinationFile, NFmiString("TEKSTI datasta"),theOutput); 
		}
		else if(!mapString)  //2.9
		{
			 *itsLogFile << "WARNING: arvolle " << value 
				        << " ei muunnosta (#Teksti datasta)" << endl;
			return isFalse;
		}
		else       //2.9
		{
			 return isFalse;
		}
};
// 061100----------------------------------------------------------------------------
FmiGenericColor NFmiLetterParamRect::MapColor(void) const 
{
	for(unsigned long ind=0; ind < itsCurrentNumOfColMaps; ind++)
	{
		if(itsCurrentParamValue >= itsColorMapping[ind].minValue
		&& itsCurrentParamValue <= itsColorMapping[ind].maxValue)
		{
			return itsColorMapping[ind].color;
		}
	}
	return GetColor();
}
