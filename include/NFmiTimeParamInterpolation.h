// NFmiTimeParamInterpolation.h: interface for the NFmiTimeParamInterpolation class.
//
// Lasse 9.99
// 
//////////////////////////////////////////////////////////////////////
#ifndef NFMITIMEPARAMINTERPOLATION_H
#define NFMITIMEPARAMINTERPOLATION_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//class NFmiSuperSmartInfo;
class NFmiFastQueryInfo;
class NFmiWeightTimeScale;

class NFmiTimeParamInterpolation 
{
public:
	NFmiTimeParamInterpolation(void);
	NFmiTimeParamInterpolation(NFmiFastQueryInfo* theInfo
		                      ,NFmiWeightTimeScale *theWeights):
							itsInfo(theInfo)
						   ,itsWeightScale(theWeights)					
						{};

	virtual ~NFmiTimeParamInterpolation(void);

/*	virtual bool SubValue(double theValue, FmiParameterName theParam)=0;
	virtual double SubValue(FmiParameterName theParam)=0;
*/
protected:
	NFmiFastQueryInfo *itsInfo; //4.9.01
	NFmiWeightTimeScale *itsWeightScale;
};

#endif
