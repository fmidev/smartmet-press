//� Ilmatieteenlaitos/Lasse.
//  Original 6.11.2000
//
// Muutettu xxxxxx/LW  
//---------------------------------------------------------------------------

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k��nt�j�n varoitusta
#endif

#include "NFmi2SymbolParamRect.h"
//#include "NFmiPressParam.h"


//---------------------------------------------------------------------------
NFmi2SymbolParamRect::NFmi2SymbolParamRect(const NFmi2SymbolParamRect& theSymbolRect)
: NFmiSymbolParamRect(theSymbolRect)
{
	itsSecondDataIdent = theSymbolRect.itsSecondDataIdent;    
};
 
//---------------------------------------------------------------------------
NFmi2SymbolParamRect::~NFmi2SymbolParamRect() 
{
};

//---------------------------------------------------------------------------

NFmiParamRect* NFmi2SymbolParamRect::Clone() const
{
	return static_cast<NFmiParamRect *>(new NFmi2SymbolParamRect(*this));
};
//----------------------------------------------------------------------------
bool NFmi2SymbolParamRect::ReadRemaining(void)  
{
	long long1;
//	double double1,double2;

	switch(itsIntObject)
	{
		case dSecondPar:	  
		{
			if (!ReadEqualChar())
				break;

			if(ReadLong(long1))
//               itsSecondDataIdent.SetParam(long1);
               itsSecondDataIdent.SetParam(NFmiParam(long1)); //10.9.02
			
			ReadNext();

			break;
		}
		default: 
		{
			return NFmiSymbolParamRect::ReadRemaining();  
			break;
		}
	}
	return true;
}
//----------------------------------------------------------------------------
int NFmi2SymbolParamRect::ConvertDefText(NFmiString & object)  
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); //11.4.00 kaikille pit�isi sallia vapaa isot/pienet kirj.

	if(lowChar==NFmiString("secondparameter") || lowChar==NFmiString("toinenparametri"))
		return dSecondPar;
	else
		return NFmiSymbolParamRect::ConvertDefText(object);
}
/*
//----------------------------------------------------------------------------
void NFmi2SymbolParamRect::DoPostReading(void) 
{
	if(fNewScaling)  
	   itsRelRect += NFmiPoint(-itsSizeFactor.X()/2, -itsSizeFactor.Y()/2);

	if(!itsSecondDataIdent.IsDataParam())
		itsSecondDataIdent.SetParam(*GetDataIdent().GetParam());
//	if(!itsRotatingDataIdent.IsDataProducer())
		itsSecondDataIdent.SetProducer(*GetDataIdent().GetProducer());
}
*/
/*
//----------------------------------------------------------------------------
bool NFmi2SymbolParamRect::CopyShortSymbol2Dest(NFmiString* symbolFile, ofstream theDestinationFile)
{//11.2
	NFmiString fileName = *itsSubDir;
	fileName += itsSymbolSetName;    
	fileName += NFmiString("_");
	fileName += *symbolFile;
	fileName += NFmiString(".ps");
	ifstream inFile = ifstream(fileName, ios::in|ios::nocreate); 
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
*/
//----------------------------------------------------------------------------
bool NFmi2SymbolParamRect::ReadValues( NFmiFastQueryInfo* theQI, bool hearDummy)
{
   if(!PointOnParam(theQI, GetSecondDataIdent().GetParam()) || !PointOnLevel(theQI)) //5.5.00
	   return false;
   
//   Vain tunnit voi muuttaa t�ll� tasolla, segmentin aikaluuppi menee muuten sekaisin
	if(!SetRelativeHour(theQI,NFmiString("K��ntyv�Symboli"))) //17.6.2000
		return isFalse;

	FloatValue(theQI, itsSecondParamValue); 

    return NFmiSymbolParamRect::ReadValues(theQI, false); //3.5.02: ei SetRelativeHour toistamiseen
}
