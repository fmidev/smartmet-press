// © Ilmatieteenlaitos/Lasse.
// Original 17.9.1999
//---------------------------------------------------------------------------

#ifndef __NFMIMULTIMAPPING_H__
#define __NFMIMULTIMAPPING_H__

#include "NFmiString.h"
#include "NFmiSize.h"
#include "NFmiParameterName.h"

typedef struct
{
	float		lowBorder;
	float		highBorder;
}FmiMappingInterval;

const int FmiMaxNumOfMappingParams=8; //saisko dynaamiseksi

typedef struct
{
    FmiMappingInterval mappingInterval[FmiMaxNumOfMappingParams];
	NFmiString	symbol;

}FmiMultiMapping;


class _FMI_DLL NFmiMultiParamMapping : public NFmiSize
{
	public:
		NFmiMultiParamMapping() : NFmiSize(0), itsMappingIntervals(0),itsNumOfParams(0) {};
		NFmiMultiParamMapping(const NFmiMultiParamMapping& theParamMapping);
		virtual ~NFmiMultiParamMapping();
		FmiMultiMapping ReadOneMapping (std::ifstream* inFile); // par. 1.9.01

		void AddMappingInterval(const FmiMultiMapping & theInterval);

		NFmiString*	Map(float values[FmiMaxNumOfMappingParams], bool& missingFound);
		void NumOfParams (short num) {itsNumOfParams=num;};
		short NumOfParams(void){return itsNumOfParams;};
	protected:

	private:
		FmiMultiMapping*	itsMappingIntervals;
//		FmiParameterName    itsParams[FmiMaxNumOfMappingParams];
		short               itsNumOfParams;
};

#endif // __NPARAMAP_H__

