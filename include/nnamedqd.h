// © Ilmatieteenlaitos/Lasse.
// Original 26.8.1998
// 
//********************************************************
// 
// Tässä on vain QueryData yhdistetty stringiin jolla saadaan 
// useammat QD:t yksilöityä tarvittaessa. Tarvitaan kun 
// lehtituotteisiin halutaan useampi QD samaan tuotteeseen    
//
//********************************************************
//
//Muutettu xxxxxx/    

//---------------------------------------------------------------------------nhyphstr.h

#ifndef __NNAMEDQD_H__
#define __NNAMEDQD_H__


#include "NFmiQueryData.h"

class _FMI_DLL NFmiNamedQueryData 
{
	public:
		NFmiNamedQueryData  (void):
		  itsData(0){};
				NFmiNamedQueryData  (NFmiQueryData *data, NFmiString name):
						itsData(data)
						,itsName(name){};

	  NFmiNamedQueryData  (const NFmiNamedQueryData &theNQD)
						{itsData = theNQD.itsData;
						 itsName = theNQD.itsName;};
		

     ~NFmiNamedQueryData (void);

	 void           SetData(NFmiQueryData *theData) {itsData = theData;}; //4.9
	 NFmiQueryData* GetData(void)  {return itsData;};
	 NFmiString GetName (void){return itsName;};

	private:
		NFmiQueryData *itsData ;
		NFmiString itsName;

};

#endif //__NQDLIST_H__


