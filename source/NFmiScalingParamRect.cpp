//© Ilmatieteenlaitos/Lasse.
//  Original 7.11.00
//
// Muutettu xxxxxx/LW   
//---------------------------------------------------------------------------

#include "NFmiScalingParamRect.h"
#include "npresspa.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01


//---------------------------------------------------------------------------
NFmiScalingParamRect::NFmiScalingParamRect(const NFmiScalingParamRect& theSymbolRect)
: NFmi2SymbolParamRect(*(NFmi2SymbolParamRect*)&theSymbolRect)
, itsXValueScaling(theSymbolRect.itsXValueScaling)  //17.10.00
, itsYValueScaling(theSymbolRect.itsYValueScaling)  //17.10.00                               

{
};
 
//---------------------------------------------------------------------------
NFmiScalingParamRect::~NFmiScalingParamRect() 
{
};

//---------------------------------------------------------------------------

NFmiParamRect* NFmiScalingParamRect::Clone() const
{
	return (NFmiParamRect *) new NFmiScalingParamRect(*this);
};
/*
//---------------------------------------------------------------------------
FmiBoolean NFmiscalingParamRect::Set(NFmiDataIdent theParam, NFmiRect theRect)
{
	itsscalingDataIdent = theParam; //080199 lisä tälle luokalle 
	return NFmi2SymbolParamRect :: Set(theParam, theRect);
	*/
//----------------------------------------------------------------------------
FmiBoolean NFmiScalingParamRect::ReadRemaining(void)  //161000
{

	switch(itsIntObject)
	{
		case dHeightScale:       
		{
			ReadSymbolScale(itsYValueScaling);
			break;
		}
		case dWidthScale:       
		{
			ReadSymbolScale(itsXValueScaling);
			break;
		}
		default: 
		{
			return NFmi2SymbolParamRect::ReadRemaining();  
			break;
		}
	}
	return kTrue;
}
//181000----------------------------------------------------------------------------
FmiBoolean NFmiScalingParamRect::ReadSymbolScale(FmiValueScaling& theScale) //20.10.00
{
	if (SetThree(theScale.noneValue,theScale.symbolValue,theScale.maxValue))
	{
		if(theScale.noneValue == theScale.symbolValue)
		{
			*itsLogFile << "*** ERROR: SymboliSkaalan kaksi ekaa arvoa ei voi olla samoja"  << endl;
			theScale.symbolValue = kFloatMissing;
			return kFalse;
		}
		return kTrue;
	}
	return kFalse;
}
//----------------------------------------------------------------------------
int NFmiScalingParamRect::ConvertDefText(NFmiString & object)  //041198
{
	NFmiString lowChar = object;
	lowChar.LowerCase(); 
						
	if(lowChar==NFmiString("heightscale")   || lowChar==NFmiString("korkeusskaala"))  
		return dHeightScale;   //16.10.00
	else if(lowChar==NFmiString("widthscale")   || lowChar==NFmiString("leveysskaala"))  
		return dWidthScale;   //17.10.00                              	//emossa nimellä ToinenParametri
	else if(lowChar==NFmiString("scalingparameter") || lowChar==NFmiString("skaalaavaparametri"))
		return dSecondPar;
	else
		return NFmi2SymbolParamRect::ConvertDefText(object);
}

//----------------------------------------------------------------------------
void NFmiScalingParamRect::DoPostReading(void) 
{    //normaalillakaan symboolilla ei ole
//	if(fNewScaling)  
//	   itsRelRect += NFmiPoint(-itsSizeFactor.X()/2, -itsSizeFactor.Y()/2);

	if(!itsSecondDataIdent.IsDataParam())
		itsSecondDataIdent.SetParam(*GetDataIdent().GetParam());
	itsSecondDataIdent.SetProducer(*GetDataIdent().GetProducer());
}
//----------------------------------------------------------------------------
void NFmiScalingParamRect::ScaleByValue()
{
	if(itsYValueScaling.symbolValue != kFloatMissing) //17.10.00 
	{
		NFmiScale endYScale = itsDefToProductScale.GetYEndScale();
//		endYScale.Set(endYScale.StartValue()+20., endYScale.EndValue()+20.); //8.11.00
		double valueScale = (FmiMin(itsSecondParamValue,itsYValueScaling.maxValue)-itsYValueScaling.noneValue)
			/(itsYValueScaling.symbolValue-itsYValueScaling.noneValue);
		endYScale.SetEndValue((endYScale.EndValue()-endYScale.StartValue())*valueScale + endYScale.StartValue());
		itsDefToProductScale.SetYEndScale(endYScale);
	}
	if(itsXValueScaling.symbolValue != kFloatMissing)  //17.10.00
	{
		NFmiScale endXScale = itsDefToProductScale.GetXEndScale();
//		endXScale.Set(endXScale.StartValue()+20., endXScale.EndValue()+20.); //8.11.00
		double valueScale = (FmiMin(itsSecondParamValue,itsXValueScaling.maxValue)-itsXValueScaling.noneValue)
			/(itsXValueScaling.symbolValue-itsXValueScaling.noneValue);
		endXScale.SetEndValue((endXScale.EndValue()-endXScale.StartValue())*valueScale + endXScale.StartValue());
		itsDefToProductScale.SetXEndScale(endXScale);
	}
}
/*
//----------------------------------------------------------------------------
FmiBoolean NFmiScalingParamRect::CopyShortSymbol2Dest(NFmiString* symbolFile, ofstream theDestinationFile)
{//11.2
	NFmiString fileName = *itsSubDir;
	fileName += itsSymbolSetName;    
	fileName += NFmiString("_");
	fileName += *symbolFile;
	fileName += NFmiString(".ps");
	ifstream inFile = ifstream(fileName, ios::in|ios::nocreate); 
    if(inFile.good() && !inFile.eof())  
    {
	   FmiBoolean tempBool;
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

