/*-------------------------------------------------------------------------------------*/
//© Ilmatieteenlaitos/Lasse.               

//Originaali 12.2.1998
//********************************************************
// 
// Luokalla on tarkoitus tuottaa samaa vakio-"lyhytsymbolia" 
// useampiin paikkoihin ilman datariippuvuutta. Paikat 
// talletetaan voidPtrListaan. 
// Toimivuutta viimeisessä versiossa ei ole testattu.
// Käyttötarve aika pientä, joten kannattaako viedä käyttö-
// oppaaseen?  
//
//********************************************************
//
//Muutettu 250998/LW perintä   

/*-------------------------------------------------------------------------------------*/


#ifndef  __NPRESAME_H__
#define  __NPRESAME_H__

#ifndef  __NVPLIST__
#include "NFmiVoidPtrList.h"
#endif// __NVPLIST__

#ifndef __NPSYMBOL_H__
#include "NFmiPsSymbol.h"
#endif//__NPSYMBOL_H__

#ifndef __NRECTSCA_H__
#include "NFmiRectScale.h"
#endif//__NRECTSCA_H__

#ifndef __NPRDESCR_H__
#include "NFmiPressDescription.h"
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

	bool WritePS(std::ofstream& theOutFile);

 //   void    SetStationScale(const NFmiRectScale& scale); 
    void    SetScale(const NFmiRectScale& scale);        
	virtual bool ReadDescription(NFmiString& retString); 
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
