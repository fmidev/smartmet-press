//© Ilmatieteenlaitos/Lasse.
//Original 21.3.2000

//****************************************************************
// Manageroi lehtis‰‰tuotantoa ylemm‰lt‰ tasolta eli 
// k‰skee NFmiPressProductia (xxx.pre-tiedostoja).
// Mahdollistaa yksinkertaisten muutosten teon (esim aseman vaihto)
// ja taas uuden eps-(tai muun) tiedoston kirjoittamisen
//****************************************************************

// Muutettu 010900/LW +dMan(Delete)NumberAddingToName
// Muutettu 290900/LW +dManStationAndImages
// Muutettu 301100/LW +dMan(De)activateFirst
// Muutettu 150101/LW +dManProductFormat
//---------------------------------------------------------------------------

#ifndef __NFMIPRESSMANAGER_H__
#define __NFMIPRESSMANAGER_H__

#include "npresspr.h"
#include "nprdescr.h"
//#include "npswrite.h"

//#include "NFmiMetTime.h"


typedef enum				 
{
	 dManWritePs    = 3000
	,dManProduct     
	,dManProductFormat       //15.1.01   
	,dManStation
	,dManStationAndImages   //29.9.00
	,dManStationNewName //7.6.00
	,dManCloseLog
	,dManOpenLog                     
	,dManChangeTime
	,dManRelativeHours
	,dManAddHours
	,dManOutputMode
	,dManData
	,dManLanguage
	,dManNumberAddingToName          //1.9.00
	,dManDeleteNumberAddingToName    //1.9.00
	,dManActivateFirst               //30.11.00
	,dManDeactivateFirst             //30.11.00
	,dManMaskNumber                  //7.9.01
}NFmiPressManagerObjects;            

class _FMI_DLL NFmiPressManager : public NFmiPressDescription 
{
	public:
		NFmiPressManager(void)			   
				{};
		
		virtual ~NFmiPressManager(void){};
		virtual FmiBoolean		ReadDescription(NFmiString& retString){return kTrue;}; 
		FmiBoolean ReadDescriptionAndWrite(NFmiPressProduct& pressProduct,FmiPressOutputMode theOutMode=kPostScript);   
        virtual int ConvertDefText(NFmiString & object);

	protected:

	private: 

};


#endif // __NFMIPRESSMANAGER_H__
