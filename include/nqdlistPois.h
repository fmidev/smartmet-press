// © Ilmatieteenlaitos/Lasse.
// Original 26.8.1998
// 
//********************************************************
// 
// Tämä luokka hallitsee listan nimettyjä QD-olioita. Tästä listasta 
// voi nimen perusteella poimia pointterin haluttuun QD:hen.
// Nimenä käytetään QD:n header-tekstiä, jonka alkuperäinen
// sisältö näin tuohoutuu.   
//
//********************************************************
//
//Muutettu xxxxxx/    

//---------------------------------------------------------------------------nhyphstr.h

#ifndef __NQDLIST_H__
#define __NQDLIST_H__


#include "NFmiQueryData.h"

class _FMI_DLL NFmiQueryDataList 
{
	public:
	  NFmiQueryDataList  (void);
	  NFmiQueryDataList  (const NFmiQueryDataList &theQDList);

     ~NFmiQueryDataList (void);

	 void Reset();
	 NFmiQueryData* Next();
	 NFmiQueryData* Get(const NFmiString &theQDName);
	 void Add(NFmiQueryData* theQD, NFmiString theQDName);

	private:
		NFmiVoidPtrList itsQDs;
//		NFmiVoidPtrIterator itsIter;

};

#endif //__NQDLIST_H__


