/*-------------------------------------------------------------------------------------*/
//� Ilmatieteenlaitos/Lasse.               

//Originaali 12.2.1998
//********************************************************
// 
// Luokalla on tarkoitus tuottaa samaa vakio-"lyhytsymbolia" 
// useampiin paikkoihin ilman datariippuvuutta. Paikat 
// talletetaan voidPtrListaan. 
// Toimivuutta viimeisess� versiossa ei ole testattu.
// K�ytt�tarve aika pient�, joten kannattaako vied� k�ytt�-
// oppaaseen?  
//
//********************************************************
//
//Muutettu 250998/LW perint�   

/*-------------------------------------------------------------------------------------*/


#ifndef  __NPRESAME_H__
#define  __NPRESAME_H__

#ifndef  __NVPLIST__
#include "NFmiVoidPtrList.h"
#endif// __NVPLIST__

#ifndef __NPSYMBOL_H__
#include "npsymbol.h"
#endif//__NPSYMBOL_H__

#ifndef __NRECTSCA_H__
#include "nrectsca.h"
#endif//__NRECTSCA_H__

#ifndef __NPRDESCR_H__
#include "nprdescr.h"
#endif//__NPRDESCR_H__


typedef enum
{
	 dPlace         =200
	,dPlaceSymbol
	,dPlaceDefArea              
	,dPlacePlotArea               
    ,dPlaceSubViews
    ,dPlaceStepSize                
    ,dPlaceTable

}NFmiPressSameSymbolsObjects;

class _FMI_DLL NFmiPressSameSymbols : public NFmiPressDescription //25.9 
{
 public:

	NFmiPressSameSymbols(void)   //loki puuttuu
		       :itsNumberOfSteps(1)
               {};
	NFmiPressSameSymbols(const NFmiRectScale& scale)    
		          : itsNumberOfSteps(1)
		           , itsScale(scale)
                {};
	
	virtual ~NFmiPressSameSymbols (void) {/*itsStations.Destroy()*/};

	FmiBoolean WritePS(std::ofstream& theOutFile);

 //   void    SetStationScale(const NFmiRectScale& scale); 
    void    SetScale(const NFmiRectScale& scale);        
	virtual FmiBoolean ReadDescription(NFmiString& retString); 
    int     ConvertDefText(NFmiString & object);
 	void	SetOutFile(std::ofstream& outFile){itsPsSymbol.SetFile(outFile);};//19.3.02
	NFmiPsSymbol* GetPsSymbol(void) {return &itsPsSymbol;};     //20.11.01
 protected:

	 void NextPoint(void);
//	 void Place(void);
 private:
	 FmiCounter itsNumberOfSteps;
	 NFmiPoint itsStepSize;   
	 NFmiRectScale itsCurrentScale; 
	 NFmiRectScale itsScale;
	 NFmiVoidPtrList itsPlaces;
	 NFmiPsSymbol itsPsSymbol;
};


#endif// __NPRESSPR_H__
