// � Ilmatieteenlaitos/Lasse.
// Original 26.8.1998
// 
//********************************************************
// 
// T�m� luokka hallitsee listan nimettyj� QD-olioita. T�st� listasta 
// voi nimen perusteella poimia pointterin haluttuun QD:hen.
// Nimen� k�ytet��n QD:n header-teksti�, jonka alkuper�inen
// sis�lt� n�in tuohoutuu.   
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


