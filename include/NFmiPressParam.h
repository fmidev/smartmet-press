/*-------------------------------------------------------------------------------------*/
//© Ilmatieteenlaitos/Lasse.               

//Originaali 21. 10.1997
// 
//****************************************************************
// Tämä luokka vastaa määrittelyn #Segmentti:ä. Luokka hoitaa
// asema- ja aikasilmukat. Jäseninä on sekä datariippuvia 
// (NFmiSymbolGroupin kautta) että muita olioita.   
//****************************************************************
//
//Muutettu 201197/LW SymboliDirin välitys pois
//Muutos 281197/LW useampia aikoja nyt myös pressParamtasolla (itsNumberOfTimeSteps)	
//Muutos 101297/LW useampia aika-askeleita tähän myös
//Muutettu 121297/LW peritty myös NFmiDescription:sta
//Muutettu 291297/LW +dStationDefSize
//Muutettu 311297/LW +itsCurrentStationScale, +SetStationScale()
//Muutettu 070198/LW käytettävä aika mukaan
//Muutettu 070198/LW uusi konstruktori
//Muutettu 080198/LW ajan tuonti PressProduktista
//Muutettu 120198/LW perintä NFmiPressDescriptionista
//Muutettu 160197/LW ReadDescription():iin palautusStringi   
//Muutettu 020398/LW +FindQDStationName()
//Muutettu 090398/LW +itsPressObjects  (myöh ->itsTimeDepObjects
//Muutettu 160398/LW +itsStationDepObjects, +itsStationStepSize
//Muutettu 180398/LW -itsStationStepSize
//Muutettu 260398/LW +SetTime()
//Muutettu 270898/LW itsDataName
//Muutettu 031198/LW +itsUnscaledStepSize
//Muutettu 030399/LW +SetDataName()
//Muutettu 160699/LW +fLonLatMode: sallii kahden Asema-luvun tulkitsemisen lon/lat:ksi 
//Muutettu 130899/LW +itsCurrentLonLat
//Muutettu 130899/LW muutettu SetLonLat() ja FindQDStation()
//Muutettu 230899/LW +itsDataIter, käytetään writePS:ssä  
//Muutettu 230899/LW +itsCurrentLonLat->itsCurrentStation
//Muutettu 211099/LW +fGridMode  
//Muutettu 291199/LW SetData() testi ettei olematon
//Muutettu 280100/LW +GetFirstStation()  
//Muutettu 310100/LW +fStationsAreLocalTime, +dStationsLocalTime
//Muutettu 100200/LW +fIsFirstStation
//Muutettu 080300/LW +minuutit
//Muutettu 150300/LW +eri outputformaatteja PS:n lisäksi
//Muutettu 120400/LW +painetaulukko 
//Muutettu 260500/LW +kMaxNumOfTableElemenst 
//Muutettu 300500/LW +aika/painepinta luupit säännöllisestä taulukosta 
//                    taulukkoon, joka mahdollistaa epäsäännölliset siirtymät 
//Muutettu 150600/LW +dNewLevelPlace,dNewTimePlace 
//Muutettu 140800/LW +dTimeTableRelative
//Muutettu 161000/LW +dLogText, dLogTextAdd, +MakeLogComment()
//Muutettu 231200/LW +itsArea
//Muutettu 231200/LW +itsOptionTime (aluksi välityspaikka dataelementtien 
//                    ääriarvojen talletukseen)
//Muutettu 030401/LW +dDataNotNeeded 
//Muutettu 180501/LW +dAreaOperation eli aluekeskiarvot ym
//Muutettu 010801/LW +fIsPureRegTimeLoop
//Muutettu 060901/LW +maski
//*-------------------------------------------------------------------------------------*/


#ifndef  __NPRESSPA_H__
#define  __NPRESSPA_H__

#ifndef __NFILESTR_H__
#include "NFmiFileString.h"
#endif//__NFILESTR_H__

#ifndef __NQUEDATA_H__
#include "NFmiQueryData.h"
#endif//__NQUEDATA_H__

#ifndef __NRECTSCA_H__
#include "NFmiRectScale.h"
#endif//__NRECTSCA_H__

#ifndef __NSYMGRP_H__
#include "NFmiSymbolGroup.h"
#endif//__NSYMGRP_H__

#ifndef __NPRTIDES_H__
#include "NFmiPressTimeDescription.h"
#endif//__NPRDESCR_H__

#include "NFmiPsWriting.h"
#include "NFmiPressStationText.h"
#include "NFmiPressArea.h"
//#include "NFmiSuperSmartInfo.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiInfoAreaMask.h"

class NFmiPressProduct;  
class NFmiArea;

typedef enum
{
	 dSymbolGroup			=40 
	,dStation
	,dRisuStation
	,dStationDefArea               
	,dStationPlotArea               
    ,dParamSubViews
    ,dParamStepSize                
    ,dStationTable
    ,dTimeTable                    
    ,dTimeTableRelative                      
	,dSegmentDataFile    = 1040        
	,dStationNameReplace     //051198, 50 on varattu 
	,dMultiParams    
	,dMultiMapping   
	,dGridMode       
	,dStationsLocalTime 
	,dLevelTable        
	,dSegmentLevel        
	,dLevelPlace       
	,dTimePlace        
	,dTimePlaceRelative       //8.5.02     
	,dLogText           =2040
	,dLogTextAdd       
	,dStationLonLatOnMap     
	,dStationNameOnMap       
	,dStationPlaceOnMap      
	,dSegmentMapArea         //10.1.01
	,dStationsFromData       //10.1.01
	,dDataNotNeeded          //3.4.01
	,dAreaOperation           //18.5.01
}NFmiPressParamObjects;

// Mika: Muutin unsignediksi - vältytään varoituksilta vertailuissa
const unsigned short kMaxNumOfTableElements = 25;  

class _FMI_DLL NFmiPressParam : public NFmiPressTimeDescription 
{
 public:

	NFmiPressParam(void)  
		:NFmiPressTimeDescription()
			   ,fDataNotNeeded(false)
			   ,fStationsAreLocalTime(0)
			   ,fGridMode(false)
			   ,itsPressProduct(0)
			   ,itsHourStep(24)
			   ,itsDataIter(0)
			   ,itsAreaMask(0)
		       ,itsNumberOfSteps(1)
			   ,fIsPureRegTimeLoop(true)
			   ,fIsAreaOperation(false)
             {itsLanguage=kFinnish;};
	NFmiPressParam(const NFmiRectScale& scale    
                  ,const short firstDeltaDays      
				  ,const short firstPlotHours       
				  ,const short firstPlotMinutes       
				  ,const short hourStep        
				  ,const unsigned long producer
				  ,NFmiPressProduct* pressProduct)  
				  :NFmiPressTimeDescription(firstDeltaDays,firstPlotHours,firstPlotMinutes) 
				  ,fDataNotNeeded(false)
 				  ,fStationsAreLocalTime(0)
				  ,fGridMode(false)
			      ,itsPressProduct(pressProduct) 
				  ,itsProducer(producer)             
				  ,itsHourStep(hourStep)          
				  ,itsDataIter(0)
				  ,itsAreaMask(0)
		          ,itsNumberOfSteps(1)
				  ,fIsPureRegTimeLoop(true)
				  ,itsScale(scale)
			      ,fIsAreaOperation(false)
             {itsLanguage=kFinnish;};
	
	virtual ~NFmiPressParam (void) {itsStations.Destroy();
	                                itsTimeDepObjects.Clear(isTrue);    
	                                itsStationDepObjects.Clear(isTrue); 
 	                                if(itsDataIter)
										delete itsDataIter;  
  	                                if(itsAreaMask)
										delete itsAreaMask;  
                                  };
	bool WritePS(
			      NFmiRectScale scale
		         ,std::ofstream& file
				 ,FmiPressOutputMode theOutput); 

//	NFmiPressProduct	&operator= (const NFmiPressProduct &thePressProduct);
//	void    SetDataName(const NFmiString& name){itsDataName = name;};//4.9 takaisin
	bool SetData(const NFmiString& dataName);    
//	bool SetMaskFile(const NFmiString& dataName);    
	void    SetStationScale(const NFmiRectScale& scale); 
    void    SetScale(const NFmiRectScale& scale);        
	virtual bool ReadDescription(NFmiString& retString); 
    int     ConvertDefText(NFmiString & object);
	void    SetData(NFmiQueryData* data){data ? itsDataIter = new NFmiSuperSmartInfo(data) : 0;}; //4.19.01 
	void    SetMaskIter(NFmiFastQueryInfo* info){itsMaskIter = info;}; //12.9.01    
	NFmiFastQueryInfo*  GetMaskIter(void){return itsMaskIter;}; //12.9.01    
	NFmiInfoAreaMask* GetAreaMask(void){return itsAreaMask;}; //12.9.01
	void    SetDataName(NFmiString name) {itsDataName = name;};  
    void        SetLanguage(FmiLanguage theLanguage) {itsLanguage=theLanguage;}; 
	FmiLanguage GetLanguage(void) {return itsLanguage;};  
	NFmiString  GetName(void) {return itsName;};
	NFmiPoint   GetFirstPoint(void);
	NFmiStation   GetCurrentStation(void) const{return itsCurrentStation;};  	
	NFmiStationPoint   GetFirstStationPoint(void) ;//28.1.00 
	bool SetFirstStation(const NFmiLocation& theLocation);    
	bool SetStationRename(const NFmiRenaming& theRenaming);    
	bool SetAllTimes(const NFmiMetTime& theTime);    
	bool SetAllLanguages(FmiLanguage theLanguage);    

	bool IsStationLocalTimeMode (void) const{return fStationsAreLocalTime;}; 
	bool IsFirstStation (void) const{return fIsFirstStation;}; 
	void SetErrorReported(unsigned short ind){fErrorReported[ind-1] = true;}; 
	bool GetErrorReported(unsigned short ind){return fErrorReported[ind-1];}; 
	NFmiString MakeLogComment(void) const; 
 	void SetNewGeoArea(const NFmiPressArea& theArea); //23.1.01 tämä muoto
    NFmiTime GetOptionTime(void) {return itsOptionTime;}; //3.1.01
    void SetOptionTime(NFmiTime theTime) {itsOptionTime = theTime;}; //4.1.01
	bool IsAreaOperation(void){return fIsAreaOperation;};        //18.5.01
	NFmiArea*  GetGeoArea(void) {return itsArea.GetArea();};     //18.5.01
	bool ChangeMaskNumber(unsigned long theMask);                //17.9.01  
	void PutInStorage(const float value, int storageQueue);  //18.9.01 the PressProduct has the storage
	float UseFromStorage(int storageQueue);            //18.9.01
	FmiCounter GetCurrentStep(void){return itsCurrentStep;}; //26.9.01
	
protected:
	void UnsetAllErrorReported(void){fErrorReported[0] = false;
									fErrorReported[1] = false;
									fErrorReported[2] = false;
									fErrorReported[3] = false;}; //3.3.00

	 void NextStation(void);
	 bool SetLonLat(NFmiStationPoint& theStation); // 16.8.99 NFmiStation-> NFmiStationPoint

 private:
     bool FindQDStationName (const NFmiStationPoint& theStation); 
	 bool CreateAreaMask(void); //13.9.01
	 bool CreateStationFromAreaMask(void); //13.9.01

	 bool fDataNotNeeded; //3.4.01
	 bool fErrorReported[4];  
	 bool fIsFirstStation; 
	 bool fStationsAreLocalTime;  
	 bool fGridMode;         
	 NFmiStation itsCurrentStation; 
	 NFmiString itsName;       
	 bool fLonLatMode;   
	 NFmiPressProduct* itsPressProduct; 
	 unsigned long itsProducer; 
	 short itsHourStep;   
	 NFmiString itsDataName;   
//	 NFmiSuperSmartInfo* itsDataIter; //4.9.01    
	 NFmiFastQueryInfo* itsDataIter; //4.9.01    
	 NFmiFastQueryInfo* itsMaskIter; //12.9.01 
	 NFmiInfoAreaMask* itsAreaMask; //12.9.01 
	 FmiCounter itsCurrentStep;   //26.9.01 jäseneksi
	 FmiCounter itsNumberOfSteps;   //12.4.00 edellisten geneerinen
	 unsigned long itsLevels[kMaxNumOfTableElements];  
	 NFmiPoint itsSteps[kMaxNumOfTableElements];  
	 NFmiPoint itsUnscaledSteps[kMaxNumOfTableElements]; 
	 long itsTimeSteps[kMaxNumOfTableElements];        
	 bool fIsTimeLoop;
	 bool fIsLevelLoop;
	 bool fIsPureRegTimeLoop;                  //1.8.01
	 NFmiRectScale itsCurrentStationScale; 
	 NFmiRectScale itsScale;
	 NFmiLocationBag itsStations;
	 NFmiVoidPtrList itsTimeDepObjects;  
	 NFmiVoidPtrList itsStationDepObjects;   
	 NFmiSymbolGroup itsSymbols;
	 bool fTwoPartLogComment;   
	 NFmiString itsLogComment;  
	 NFmiPressArea  itsArea;     //9.1.01 ptr pois
	 bool  fIsAreaOperation;     //18.5.01
	 NFmiTime       itsOptionTime; //3.2.01
};


#endif// __NPRESSPR_H__
