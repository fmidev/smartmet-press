//© Ilmatieteenlaitos/Lasse.
//Original 10.3.1998
// 
// Muutettu xxxxxx/LW 
// Muutettu 230899/LW ítsData muutettu qDatasta qInfoon 
// Muutettu 150800/LW +GetData() 

//---------------------------------------------------------------------------

#ifndef __NPREDATA_H__
#define __NPREDATA_H__

#include "NFmiDataIdent.h"
#include "NFmiQueryData.h"

//class NFmiSuperSmartInfo;
class NFmiFastQueryInfo;

class _FMI_DLL NFmiPressDataObject 
{
	public:
		NFmiPressDataObject(void):itsData(0){};

		NFmiPressDataObject(NFmiFastQueryInfo* theData)   
				:itsData(theData)
		{
		};

		NFmiPressDataObject(const NFmiPressDataObject& theRect);  //5.4
		
		virtual ~NFmiPressDataObject(void){};


		void		        SetData(NFmiFastQueryInfo* theData){itsData = theData;};
		NFmiFastQueryInfo*		GetData(void) const{return itsData;}; //15.8.00

	protected:
	
	protected: 
		NFmiFastQueryInfo* itsData;
};

#endif // __NPARRECT_H__
