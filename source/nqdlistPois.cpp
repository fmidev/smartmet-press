/*---------------------------------------------------------------------------*/ 
//© Ilmatieteenlaitos/Lasse.               
// 
//Originaali 27.8.98 
// 
// 
//
//Muutettu xxxxxx/LW
//
/*---------------------------------------------------------------------------nhyphstr.cpp*/

 #include  "nqdlist.h"
#include "NFmiStringList.h"
//#include <string.h> 
 
//---------------------------------------------------------------

NFmiQueryDataList :: NFmiQueryDataList  (void)
{
 //  itsIter = NFmiVoidPtrIterator(itsQDs);
}


//---------------------------------------------------------------
NFmiQueryDataList :: NFmiQueryDataList  (const NFmiQueryDataList &theQDList)
 
{
	itsQDs = theQDList.itsQDs;

}
//---------------------------------------------------------------
NFmiQueryDataList :: ~NFmiQueryDataList  (void)
{
	itsQDs.Clear(isTrue);
}

//---------------------------------------------------------------
void NFmiQueryDataList::Reset (void)
{   
	NFmiVoidPtrIterator iter(itsQDs);
	iter.Reset();
}
//---------------------------------------------------------------
NFmiQueryData* NFmiQueryDataList::Next (void)
{   
	NFmiVoidPtrIterator iter(itsQDs);
	return (NFmiQueryData*)iter.Next();
}
/*
//---------------------------------------------------------------
NFmiString NFmiQueryDataList::CurrentName (void)
{   
	NFmiVoidPtrIterator iter(itsQDs);
	iter.Reset();
}
*/
//---------------------------------------------------------------
NFmiQueryData* NFmiQueryDataList::Get(const NFmiString& theName)
{
	NFmiQueryData* data;
	NFmiVoidPtrIterator objectIter(itsQDs);
	NFmiString name; 
	objectIter.Reset();
	data = (NFmiQueryData*)objectIter.Next();
	while (data)
	{
		name = data->Info()->Leader();
		if (name == theName)  
		{
		   return data;
		}
		data = (NFmiQueryData*)objectIter.Next();   
	}
	return 0;
}

//---------------------------------------------------------------
void NFmiQueryDataList::Add (NFmiQueryData* theQD, NFmiString theName)
{ 
  // Mika: Ei näytä tekevän mitään hyödyllistä, vai onko jotain sivuvaikutuksia?

  /*  
   * NFmiStringList stringList;
   * NFmiString header;
   * stringList.Add(&theQD->Info()->Owner());
   * stringList.Add(&theQD->Info()->TimeStamp());
   * stringList.Add(&theName);
   * stringList.Add(&theQD->Info()->Text());
   * 
   * NFmiString testString = stringList.Reset();
   * testString = stringList.Next();
   * testString = *stringList.Current();
   */
}
