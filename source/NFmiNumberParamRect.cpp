//© Ilmatieteenlaitos/Lasse.
//  Original 5.3.1998
//  Erotettu yleisestä tekstioliosta numeroita varten
// 
//Muutettu 050398/LW 
//Muutettu 070598/LW Mikan makro käyttöön jotta saataisiin sakndit ok 
//Muutettu 211098/LW lisätty "-" -merkki puuttuviin 
//Muutettu 161198/LW pitkä miinus -optio 
//Muutettu 251198/LW Paikansiirrosta -40, miksi ?????????? 
//Muutettu 010799/LW tekstiä eteen ja taakse 
//Muutettu 290200/LW copykonstr. siivottu
//Muutettu 040201/LW (+)formaatit: plussa vain yksinumeroisiin
//Muutettu 060201/LW .(1)f formaatit: desimaali vain alle yhden arvoihin
//Muutettu 100402/LW +fDetachSign

//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiNumberParamRect.h"
#include "NFmiHyphenationString.h"
#include "NFmiPressParam.h"


#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//int Convert(NFmiString & object);

NFmiNumberParamRect::NFmiNumberParamRect(const NFmiNumberParamRect& theNumberParamRect)
: NFmiTextParamRect(theNumberParamRect)
//: NFmiTextParamRect(*(NFmiTextParamRect*)&theNumberParamRect)
, itsFormat(theNumberParamRect.itsFormat)
, fZeroMinus(theNumberParamRect.fZeroMinus)
, fDetachSign(theNumberParamRect.fDetachSign)
{
}

//---------------------------------------------------------------------------
NFmiNumberParamRect::~NFmiNumberParamRect() 
{
};
//---------------------------------------------------------------------------
NFmiParamRect* NFmiNumberParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiNumberParamRect(*this);
};

//----------------------------------------------------------------------------
bool NFmiNumberParamRect::ReadDescription(NFmiString& retString) 
{	
	long long1;                
	double r1,r2,r3,r4;
	itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));

	itsFont = NFmiString("Helvetica");    
	itsAlignment = kCenter; //5.10
	itsFormat = NFmiString("%.f");            
//	SetColor(0.f,0.f,0.f,1.f); //3.11.00 pois, onko hoidettu                

	SetPreReadingTimes();  //29.6
	itsMapping = new NFmiParamMapping; //18.11.99 
	itsModifier = kNoneModifier; //15.2.00

	ReadNext();

	//011298 MIKSI IHMEESSÄ
	itsRelRect -= NFmiPoint(1., 1.);

	while(itsIntObject != 9999 || itsCommentLevel) 
	{
	if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment; 
 	if(itsLoopNum > itsMaxLoopNum)  
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Numerossa" << endl;
	    retString = itsString;
		return false;
	}
    itsLoopNum++;
	switch(itsIntObject)
	{
			case dOther:	  
			{    
				if(itsLogFile)
					*itsLogFile << "*** ERROR: Tuntematon sana #Numerossa: " << (char*)itsObject << endl;  
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
				*itsLogFile << "*** ERROR: Tuntematon kohdistus #Numerossa: " << (char*)itsObject << endl;  


			ReadNext();
			break;
		}
		case dFont:
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsFont = itsObject;

			ReadNext();
			break;
		}
		case dFormat:		  
		{
			if (!ReadEqualChar())
				break;
			*itsDescriptionFile >> itsObject;
			itsFormat = itsObject;

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
//			   r1 -= 40.; //251198 miksi ihmeessä yhtäkkiä vaaditaan ??????????
//			   r2 -= 40.;
			   itsRelRect += NFmiPoint(r1/c40, r2/c40);
//			   itsRelRect += NFmiPoint(r1/c40, r2/c40);
			}
//			fNewScaling = true;  //ei tarvita

			ReadNext();
			break;
		}
		/* PITÄISI OLLA ABS PAIKKA TIEDOSSA
		case dAbsPlace:  //9.10
		{
			if (!ReadEqualChar())
				break;

			if(Read2Double(r1,r2))
			{    //6.10        // EI NÄIN
			   itsRelRect += NFmiPoint(r1/c40, r2/c40);
			}

			ReadNext();
			break;
		}
		*/
		case dRelPlace:
		{
			if (!ReadEqualChar())
				break;
			if(Read4Double(r1,r2,r3,r4))
			{
			   itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
	/*		   xmin = r1;
			   ymin = r2;
			   xmax = r3;
			   ymax = r4; */
			}
			fNewScaling = false;  
			ReadNext();
			break;
		}
		case dParamSize:  //14.9
		{
			if (!ReadEqualChar())
				break;
			if(ReadDouble(r1))
			{ 
			   itsRelRect.Inflate(-(c40-r1)/(c40*2));
	//		   itsRelRect.Set(NFmiPoint(-.5,(c40 - r1)/(2.*c40) -.5),NFmiPoint(.5,-((c40 - r1)/(2.*c40) -.5)));
	/*		   xmin = -.5;
			   ymin = (c40 - r1)/(2.*c40) -.5 ;
			   xmax = .5;
			   ymax = - ymin; */
	//		   fNewScaling = true;  
	//		   itsDefToProductScale.SetStartScales(NFmiRect(sizePoint1,sizePoint2));
			}

			ReadNext();
			break;
		}
		
		case dProducer: 
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
 //              itsProducer = long1;
               long1=1;  //dummy koska tuottaja hylätty
			
			ReadNext();
			break;
		}
/*
		case dParam:   parrect:iin
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))
               itsIdentPar = long1;
			
			ReadNext();
			break;
		}
*/
		case dColorValueDependent: //poistettu
		{	
//			fColorValueDependent = true;

			ReadNext();
			break;
		}

		case dParenthesis:     //24.8
		{	
			fParenthesis = true;

			ReadNext();
			break;
		}

		case dZeroMinus:     
		{	
			fZeroMinus = true;

			ReadNext();
			break;
		}

		case dDetachSign:     //10.4.02
		{	
			fDetachSign = true;

			ReadNext();
			break;
		}
		case dRelDay:     
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))   
               itsFirstDeltaDays = (unsigned short)(long1+ itsEnvironment.GetDayAdvance()); //23.5.02 +GetDayAdvance); ;

			ReadNext();
			if(itsLogFile)
					*itsLogFile << "*** ERROR: Päiviä ei voi asettaa #Numerossa"  << endl;  
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
					*itsLogFile << "ERROR: Vuosia ei voi asettaa #Numerossa"  << endl;  
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
		default:  //171198
		{
			ReadRemaining();  
			break;
		}
	}
	} //while
	
	SetPostReadingTimes();

//	fNewScaling = false; //11.9

//171198
	if(fNewScaling)
	   itsRelRect += NFmiPoint(1.,1.);
	Set(NFmiDataIdent(NFmiParam(itsIdentPar),240) //1.2.01 +its
	   ,NFmiRect(itsRelRect));

	retString = itsString;
	return true;
}
//171198----------------------------------------------------------------------------
bool NFmiNumberParamRect::ReadRemaining(void)  
{
/*
	switch(itsIntObject)
	{	
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
		
		default:
	*/
			return NFmiTextParamRect::ReadRemaining(); 
//	}
	return true;
}
//---------------------------------------------------------------------------
int NFmiNumberParamRect::ConvertDefText(NFmiString & object) 
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //10.4.02

	if(lowChar==NFmiString("format") || lowChar==NFmiString("formaatti"))
		return dFormat;
	else if(lowChar==NFmiString("zerowithminus") || lowChar==NFmiString("nollaviivalla"))
		return dZeroMinus;
	else if(lowChar==NFmiString("detachsign") || lowChar==NFmiString("irrotaetumerkki"))
		return dDetachSign;
	else
		return NFmiTextParamRect::ConvertDefText(object);
	
}
//---------------------------------------------------------------------------
bool NFmiNumberParamRect::WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
										,NFmiFastQueryInfo* theQI
										,ofstream& theDestinationFile
										,FmiPressOutputMode theOutput) //15.3.00									
//										,const NFmiString& theSymbolDirTurha) //LW 20.11
		
{

   float value;
   itsCurrentSegmentTime = ((NFmiQueryInfo*)theQI)->Time(); //4.9.01 
   itsCurrentTime = itsCurrentSegmentTime;//16.2.2000

   if(fUseFromStorage)   //18.9.01
   {
	   value = itsPressParam->UseFromStorage(itsStorageQueue);
   }
   else
   {
	   if(!PointOnParam(theQI, GetDataIdent().GetParam())) 
		   return false;

	   if(!SetRelativeHour(theQI,NFmiString("#Numero"))) //16.2.2000
			return false;

		if(!ReadCurrentValue(theQI, itsCurrentParamValue))  //+if 9.2.00
			return false;

		value = itsCurrentParamValue;
	}

   if(!ActiveTimeIndex(itsPressParam->GetCurrentStep())) //26.9.01
   {
	   if(itsLoopActivity.bypassWithValue)  //2.10.01
		   value = itsLoopActivity.bypassValue;
	   else
		   return true;
   }

   if(fPutInStorage)   //18.9.01
   {
	   itsPressParam->PutInStorage(value, itsStorageQueue);
//	   float testvalue = itsPressParam->UseFromStorage();
//	   itsPressParam->PutInStorage(value);
	   return true;
   }
	
    NFmiString format = itsFormat;

	NFmiHyphenationString hypString(itsFormat);
	bool isParen = false;
//4.2.01 esim formaatti=%(+)1.f -> kaksinumeroiset ilman plussaa; pelkkä ( tai ) riittää
	if(itsFormat.Search(NFmiString("(+")) > 0 || itsFormat.Search(NFmiString("+)")) > 0)
	{
		if (value >= 9.5)
			hypString = hypString.DropChar(NFmiString("+"));
		isParen = true;
	}
//6.2.01 esim formaatti=%.(1)f -> alle yhden desimaalin kanssa, muuten ilman
	if(itsFormat.Search(NFmiString(".(")) > 0 || itsFormat.Search(NFmiString(")f")) > 0)
	{
		if (fabs(value) >= .95)
			hypString = hypString.ReplaceChar(NFmiString("1"), (NFmiString("0")));
		isParen = true;
	}

	if(isParen)
	{
		hypString = hypString.DropChar(NFmiString("("));
		hypString = hypString.DropChar(NFmiString(")"));
		format = hypString;
	}

// 090299 tasan nolla pirretään ilman desimaaliosia
// tehty maaseudun tulevaisuutta varten, häiritseeköhän jossain

	if(value == 0.) format = NFmiString("%d");

//*** 21.10.98 lisätty "-" -merkki puuttuviin 
	NFmiValueString string;
	NFmiString* mapString =0;

	if(itsMapping)  //on aina luotu vaikkei mapattu, mapString==0 ilmaisee puuttuvaa mappausta
	{
		mapString = itsMapping->Map(value);
	}
	
	if(mapString) //18.11.99 mappingmahdollisuus myös numerolle
	{
		if(*mapString == NFmiString("None")) //13.11.00
			return false;

		NFmiString helpS(*mapString);  //2.8.01 mappauksessa olevat miinukset pitkinä haluttaessa
		if(helpS == NFmiString("-") && itsEnvironment.GetLongNumberMinus())
			helpS = NFmiString("\\226");
//		return WriteCode(Construct(mapString), AbsoluteRectOfSymbolGroup, 
		return WriteCode(Construct(&helpS), AbsoluteRectOfSymbolGroup, 
			         theDestinationFile, NFmiString("NUMERO"), theOutput);
	}
	else
	{

		if(fParenthesis)            
		{
			string = NFmiString("(");
		}
		if(value == 0 && fZeroMinus)   //NOLLA PUUTTUVANA //110299
		{	
			if(!itsEnvironment.GetLongNumberMinus())
			{
				string += NFmiString ("-");
			}
			else
			{
				string += NFmiString("\\226"); 

			}
		}        
		else if(value != kFloatMissing)  // NORMAALI POS. TAI NEG. ARVO  
			if(value >= 0. || !itsEnvironment.GetLongNumberMinus()) //171198
			{
				string += NFmiValueString (value,format);
			}
			else
			{
				string += NFmiString("\\226");
				NFmiHyphenationString helpFormat = format;
				NFmiString hFormat = helpFormat.DropChar("+");
				string += NFmiValueString ((float)fabs(value),hFormat);
			}
		else      // PUUTTUVA
		{
     		if(!itsEnvironment.GetLongNumberMinus())     //171198
				string += NFmiString("-");
			else
				string += NFmiString("\\226");  

	//		itsNumOfMissing++;   //090299 siirretty ParamRectille
		}
	//	string.ReplaceChars(NFmiString("-"),NFmiString("\\226"));//131198 vain yksi merkki
 		if(fParenthesis)
		{
		   string += NFmiString(")");
		}
	} //ei-mappi luuppi

	string = DetachSign(string, value);    //10.4.02 

	fUseSelectLatinFont = value >= 0.//; //15.5.2000 astemerkki edes positiivisille 
	      && value != kFloatMissing     //13.11.00
		|| value == kFloatMissing       //2.8.01 pitkät miinukset näkyviin
		  && itsEnvironment.GetLongNumberMinus(); 

	//300699 +Construct (lisäykset eteen ja perään)
	return WriteCode(Construct(&string), AbsoluteRectOfSymbolGroup, 
			         theDestinationFile, NFmiString("NUMERO"), theOutput);
}
// 031100----------------------------------------------------------------------------
NFmiString NFmiNumberParamRect::DetachSign(const NFmiString& theString, float theValue) const
//tehty Hesaria varten, joka haluuaa välilyönnin etumerkin jälkeen jos yksinumeroinen luku
//ei universaali vielä, esim jos sulkuja mukana tms 
{
	NFmiString newString;
	if(fDetachSign && theValue < 9.5 && theValue > -9.5)
	{
		NFmiString firstChar(theString.GetChars(1,1));
		NFmiString numString = theString.GetChars(2, theString.GetLen()-1);
		if(firstChar == NFmiString("+"))
		{
			newString = NFmiString("+ ");
			newString += numString;
		}
		else if(firstChar == NFmiString("-"))
		{
			newString = NFmiString("- ");
			newString += numString;
		}
		else if(theString.GetChars(1, 4) == NFmiString("\\226"))
		{
			newString = NFmiString("\\226  ");
			newString += theString.GetChars(5, theString.GetLen()-4);
		}
		else
			newString = theString;
	}
	else
		newString = theString;

	return newString;
}
// 031100----------------------------------------------------------------------------
FmiGenericColor NFmiNumberParamRect::MapColor(void) const 
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
/*
//----------------------------------------------------------------------------
void NFmiNumberParamRect::SetValueDepending(float value)
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
