//© Ilmatieteenlaitos/Lasse.
//Original 12.2.1998
//
//********************************************************
// "Lyhytymbolien" tuottaminen suoraan ps-koodiin ilman data-
// ohjausta. Tarkoittaa Symbolit-hakemiston kuvia, joista
// tehd‰‰n tiiviit esitykset LyhytSymbolit-hakemistoon. 
// Datariippuvat symbolit ovat toisessa luokkahierarkiassa.
//********************************************************
//
// Erotettu omaksi olioksi NFmiSymbolParamRectist‰
// 
// Muutettu 060398/LW t‰st‰ erotettu n. puolet uuteen luokkaan NFmiPressScaling 
// Muutettu 250998/LW SymbolSetName() pois
// Muutettu 290998/LW +dPsRelSize
// Muutettu 190299/LW +GetSymbol()
  
//---------------------------------------------------------------------------

#ifndef __NPSYMBOL_H__
#define __NPSYMBOL_H__

#include <fstream> //STD 27.8.01
/*
#ifndef __NPRDESCR_H__
#include "NFmiPressDescription.h"
#endif//__NPRDESCR_H__

#ifndef __NPSWRITE_H__
#include "NFmiPsWriting.h"
#endif//__NPSWRITE_H__

#ifndef __NRECTSCA_H__
#include "NFmiRectScale.h"
#endif//__NRECTSCA_H__
*/
#ifndef __NPRSCALE_H__
#include "NFmiPressScaling.h"
#endif//__NPRSCALE_H__

typedef enum
{
	 dSymbolDir = 150
    ,dConstantSymbol
	,dPsRelSize       //29.9
//	,dSymbolSize
//	,dRelSymbolSize
}NFmiPsSymbolObjects;

class _FMI_DLL NFmiPsSymbol : public NFmiPressScaling 
{
	public:
		NFmiPsSymbol(void): NFmiPressScaling()
	//		                ,itsRectSize(40.,40.)
	//						,itsRelArea(NFmiPoint(0.,0.)
	//						           ,NFmiPoint(1.,1.))
								 {}; 
		
		NFmiPsSymbol(const NFmiPsSymbol& thePsSymbol);
		
		virtual ~NFmiPsSymbol();
        virtual int             ConvertDefText(NFmiString& object);
		virtual bool		ReadDescription(NFmiString& retString); 
		virtual NFmiPsSymbol*	Clone(void) const; 
        virtual bool      MakeAndWritePS(NFmiPoint place); //t‰n voi sitten joskus hyl‰t‰
		virtual bool      WritePS(FmiPressOutputMode theOutput); //15.3.00
		        NFmiString      GetSymbol(void) const {return itsSymbol;};//190299
//6.3       bool              Set( NFmiRectScale &theRectScale
//							   , ofstream theDestinationFile);	

    protected:           
		virtual bool CopyShortSymbol2Dest(void); 
        virtual bool ConvertOrig2Short(void);
 //       NFmiString              SymbolSetName(void)const; //25.9 pois //23.2

	protected:
//6.3        NFmiRectScale       itsRectScale; //koko kartta, tulee vasta writePs;ss‰
		NFmiString			itsShortDir;	
		NFmiString			itsOrigDir;
		NFmiString          itsSymbol;
//6.3		NFmiRect            itsRelArea;   //24.2
//6.3	    NFmiPoint           itsRectSize;  //23.2
};


#endif //__NPSYMBOL_H__

/*
class NFmiDirectedSymbolParamRect : NFmiSymbolParamRect
{

	private:
		xxxx	itsDirection;
};

*/
