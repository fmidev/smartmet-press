//© Ilmatieteenlaitos/Salla.
//  Original 20.11.1997
//
// Muutettu xx1198/LW +ReadRemaining(),+DoPostReading() 
// Muutettu 080199/LW itsRelRect siirretty  
// Muutettu 250899/LW tuottaja poistettu  
// Muutettu 300899/LW ReadValues():ssa
// Muutettu 110400/LW +itsVerticalLong
// Muutettu 071100/LW perintä uudesta NFmi2SymbolParamRect:stä
// Muutettu 081100/LW +AddSymbolMetaOptions()  
//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiRotatingParamRect.h"
#include "NFmiPressParam.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

//---------------------------------------------------------------------------
NFmiRotatingParamRect::NFmiRotatingParamRect(const NFmiRotatingParamRect& theSymbolRect)
//: NFmi2SymbolParamRect(*(NFmi2SymbolParamRect*)&theSymbolRect)
: NFmi2SymbolParamRect(theSymbolRect)
{
//	itsRotatingDataIdent = theSymbolRect.itsRotatingDataIdent;    
	itsNotRotatingMinValue = theSymbolRect.itsNotRotatingMinValue; 
	itsNotRotatingMaxValue = theSymbolRect.itsNotRotatingMaxValue; 
	itsVerticalLong = theSymbolRect.itsVerticalLong;               //110400
};
 
//---------------------------------------------------------------------------
NFmiRotatingParamRect::~NFmiRotatingParamRect() 
{
//	 NFmi2SymbolParamRect::~NFmi2SymbolParamRect();
};

//---------------------------------------------------------------------------

NFmiParamRect* NFmiRotatingParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiRotatingParamRect(*this);
};
/*
//---------------------------------------------------------------------------
bool NFmiRotatingParamRect::Set(NFmiDataIdent theParam, NFmiRect theRect)
{
	itsRotatingDataIdent = theParam; //080199 lisä tälle luokalle 
	return NFmi2SymbolParamRect :: Set(theParam, theRect);
	*/
//----------------------------------------------------------------------------
bool NFmiRotatingParamRect::ReadRemaining(void)  //171198
{
//	long long1;
	double double1,double2;

/* EI TÄNNE MITÄÄN TÄLLAISTA KOSKA TÄNNE TULLAAN USEAMMAN KERRAN, vaan DoPostReading:iin*/
//	if(fNewScaling)  //041298 oli pielessä 20 yksikköä ????
////	   itsRelRect += NFmiPoint(-.5, -.5);
//	   itsRelRect += NFmiPoint(-itsSizeFactor.X()/2, -itsSizeFactor.Y()/2);//080199 ???

	switch(itsIntObject)
	{
		case dVerticalLong:	 //11.4.00 
		{
			SetOne(itsVerticalLong);
			break;
		}
/*		case dRotatingPar:	  
		{
			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))
               itsRotatingDataIdent.SetParam(long1);
			
			ReadNext();

			break;
		}
		*/
		case dNotRotInterval:	  
		{
			if (!ReadEqualChar())
				break;

			if(Read2Double(double1,double2))
			{
			   itsNotRotatingMinValue=double1;
			   itsNotRotatingMaxValue=double2;
			}

			ReadNext();

			break;
		}
		default: 
		{
//			return NFmiPressDescription::ReadRemaining();  
			return NFmi2SymbolParamRect::ReadRemaining();  //220199
			break;
		}
	}
	return true;
}
//----------------------------------------------------------------------------
int NFmiRotatingParamRect::ConvertDefText(NFmiString & object)  //041198
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //11.4.00 kaikille pitäisi sallia vapaa isot/pienet kirj.
							//emossa nimellä ToinenParametri
	if(lowChar==NFmiString("rotatingparameter") || lowChar==NFmiString("kääntäväparametri"))
		return dSecondPar;
	else if(lowChar==NFmiString("notrotinterval")||lowChar==NFmiString("eikääntyvätarvot"))
		return dNotRotInterval;
	else if(lowChar==NFmiString("verticallongitude")||lowChar==NFmiString("pystylongitudi")
		     ||  lowChar==NFmiString("pystypituuspiiri"))
		return dVerticalLong;
	else
		return NFmi2SymbolParamRect::ConvertDefText(object);
}

//----------------------------------------------------------------------------
void NFmiRotatingParamRect::DoPostReading(void) 
{
	if(fNewScaling)  
	   itsRelRect += NFmiPoint(-itsSizeFactor.X()/2, -itsSizeFactor.Y()/2);

	if(!itsSecondDataIdent.IsDataParam())
		itsSecondDataIdent.SetParam(*GetDataIdent().GetParam());
//	if(!itsRotatingDataIdent.IsDataProducer())
		itsSecondDataIdent.SetProducer(*GetDataIdent().GetProducer());
}


//----------------------------------------------------------------------------
bool NFmiRotatingParamRect::CopyShortSymbol2Dest(NFmiString* symbolFile, ofstream& theDestinationFile)
{//11.2
	NFmiString fileName = *itsSubDir;
	fileName += itsSymbolSetName;    
	fileName += NFmiString("_");
	fileName += *symbolFile;
	fileName += NFmiString(".ps");
	// Mika: Oli 2 kertaa ios::in
	ifstream inFile(fileName, ios::in|ios::binary); 
    if(inFile.good() && !inFile.eof())  
    {
	   bool tempBool;
	   float direction = itsSecondParamValue;
	   float adjustedDirection = AdjustToMap(direction);
	   if(!Rotate()) adjustedDirection = 270.;
	   tempBool = NFmiWritePSConcatRotating(itsDefToProductScale, adjustedDirection, theDestinationFile);
	   tempBool = NFmiCopyFile(inFile,theDestinationFile);
	   tempBool = NFmiWritePSEnd(theDestinationFile);
	   return isTrue;
	}
	else
		return isFalse;
}
//----------------------------------------------------------------------------
void NFmiRotatingParamRect::WriteMetaCode(NFmiString* symbolFile, NFmiPoint realPos, ofstream & os)
{
	NFmiString hStr(*symbolFile);
	hStr.FirstCharToUpper();
//	os << endl << "SymbolPath Symbolit/" << (char*)itsSymbolSetName << endl;
	os << endl << (char*)hStr  
		<< " -x " << realPos.X() 
		<< " -y " << realPos.Y() 
//			<< " -scalex " << int(itsDefToProductScale.GetXScaling()*100.) << "%"     
//			<< " -scaley " << int(itsDefToProductScale.GetYScaling()*100.) << "%";
		<< " -angle " << (int)itsSecondParamValue;
	os << endl;
}	
/*
//----------------------------------------------------------------------------
void NFmiRotatingParamRect::AddSymbolMetaOptions(void) 
{
	cout << " -rotate " << (int)itsSecondParamValue; 
}
*/
//----------------------------------------------------------------------------
float NFmiRotatingParamRect::AdjustToMap(float theDirection) const
{//110400
	if(itsVerticalLong != kFloatMissing)
	{
		double lon = itsPressParam->GetCurrentStation().GetLongitude();
		if(lon != kFloatMissing)
			return theDirection - (float)lon + (float)itsVerticalLong;
		else
		{
//			NFmiString name = itsPressParam->GetCurrentStation().GetName();
		   *itsLogFile << "*** ERROR: longitudi puuttuu tuulensuuntaa varten asemalta "
			           << (char*)(itsPressParam->GetCurrentStation().GetName()) << endl;
		   return theDirection;
		}
	}

	return theDirection;
}
/*	return itsVerticalLong < kFloatMissing ?
		   (float)theDirection - (float)itsPressParam->GetCurrentStation().GetLongitude()
		                       + (float)itsVerticalLong
		 : (float)theDirection;
		 */
//----------------------------------------------------------------------------
bool NFmiRotatingParamRect::Rotate(void) const
{//241198
	return itsCurrentParamValue < itsNotRotatingMinValue
		|| itsCurrentParamValue > itsNotRotatingMaxValue;
}
//080199---------------------------------------------------------------------------
/* 1.5.00 yhdistetty PointOnParam:iin
bool NFmiRotatingParamRect:: PointOnSecondParam(NFmiQueryInfo* theQI)
{
	if(!PointOnLevel(theQI)) //12.4.00 varsinaisen paramin leveli
		return false;

    fParamErrorReported = false;  
	if (!theQI->Param(*GetSecondDataIdent().GetParam()))
	{
		if(itsLogFile && !fParamErrorReported)
		{
		   long paramIdent = GetSecondDataIdent().GetParam()->GetIdent();  //1.1.99 +Second
		   *itsLogFile << "*** ERROR: Parametri ei löydy(KääntäväParametri): " << paramIdent 
			         << endl;
		   fParamErrorReported = isTrue;
		}
		return false;
	}
	return isTrue;
}
*/
/*
//----------------------------------------------------------------------------
bool NFmiRotatingParamRect::ReadValues( NFmiQueryInfo* theQI)
{
//   if(!PointOnSecondParam(theQI)) 
   if(!PointOnParam(theQI, GetSecondDataIdent().GetParam()) || !PointOnLevel(theQI)) //5.5.00
	   return false;
   
//   Vain tunnit voi muuttaa tällä tasolla, segmentin aikaluuppi menee muuten sekaisin
	if(!SetRelativeHour(theQI,NFmiString("KääntyväSymboli"))) //17.6.2000
//	if(!SetDataHour(theQI, NFmiString("KääntyväSymboli")))
		return isFalse;

	FloatValue(theQI, itsSecondParamValue); //1.11.99 oli ReadCurrentData joka skaalasa tuulensuunnan!!!

    return NFmi2SymbolParamRect::ReadValues(theQI);
}
*/
