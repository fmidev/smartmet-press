/*-------------------------------------------------------------------------------------*/
//© Ilmatieteenlaitos/Lasse.               

//Originaali 21. 10.1997
//****************************************************************
// T‰m‰ on lehtis‰‰ohjelman p‰‰luokka. Segmentit (NFmiPressParam)
// ja monet muut oliot ovat t‰m‰n j‰seni‰. M‰‰rittelytiedoston 
// lukeminen ja ps-tiedoston kirjoitus aloitetaan t‰‰lt‰ 
// kontrollin siirtyess‰ j‰senille useammankin tason kautta.
// Yleiset asiat kuten pohjat, tiedostonimet, data hoidetaan 
// t‰‰ll‰ (Windowsin ymp‰ristˆmuuttujat luetaan).   
//****************************************************************
//
//Muutettu 031297/LW data luetaan nyt t‰‰ll‰ ,eik‰ p‰‰ohjelmassa
//Muutettu 121297/LW peritty myˆs NFmiDescription:sta
//Muutettu 301297/LW Tuotetiedosto preprosessoidaan, jossa includet haetaan
//Muutettu 301297/LW +itsPath
//Muutettu 070198/LW +piirrett‰v‰ aika
//Muutettu 120198/LW +itsProducer
//Muutettu 160198/LW ReadDescription():iin palautusStringi 
//Muutettu 160298/LW +itsSameSymbols
//Muutettu 190398/LW +itsLanguage
//Muutettu 250398/LW +itsObjects,+itsFirstPlotHours,+itsFirstDeltaDays
//Muutettu 030498/LW +itsEpsFileNames, myˆs pohja t‰nne
//Muutettu 150498/LW +itsBoundingBorder
//Muutettu 110698/LW +Paletti
//Muutettu 180698/LW palettipiirteet j‰‰neet virattomiksi koska voidaan hoitaa Perlill‰
//Muutettu 230698/LW useampia boxeja (QD) listaan
//Muutettu 180898/LW +itsOutFileName, jotta m‰‰rittelytiedoston nime‰
//                   voitaisiin k‰ytt‰‰ tulostiedoston nimen‰
//Muutettu 190898/LW useampaa QueryDataa yritet‰‰n (1 per segmentti)
//Muutettu 090998/LW +itsPageSize
//Muutettu 090998/LW +dDataTime
//Muutettu 090299/LW +dTimeStamp: aikaleima tulosTiedostoon 
//Muutettu 240299/LW +dProductPlaceMove 
//Muutettu 030399/LW +FirstDataName()
//Muutettu 080399/LW +DefinePar(),ReplacePar()
//Muutettu 110899/LW +itsNameToLonLat grididata vaatii jos halutaan ett‰ vanhat m‰‰rittelyt
//                   joissa ei ole LonLatia, toimisivat. Olion t‰yttˆ hoidetaan PressParameissa tarvittaessa.
//Muutettu 110899/LW +dRelativeHours
//Muutettu 170100/LW +fChangeScandinavian
//Muutettu 250100/LW +nimip‰iv‰, itsNameDay
//Muutettu 150300/LW +eri outputformaatteja PS:n lis‰ksi
//Muutettu 160300/LW output-tiedoston nime‰minen ReadDescr.-puolelta writePs-puolelle 
//                   +itsProductNameFormat
//Muutettu xx0300/LW +SetFirstStation(),+Close(),+SetProductName()
//Muutettu 070600/LW +SetFirstRename()
//Muutettu 070600/LW +itsOutputMode
//Muutettu 280800/LW itsLanguage -> PressDescriptioniin
//Muutettu 010900/LW +ActivateNumberToName() ja De....
//Muutettu 290900/LW +SetImagePreNames
//Muutettu 290900/LW +itsStringNameTimeFormat //merkkijonona jota tulkataan
//Muutettu 241000/LW +itsNumOfWritePS
//Muutettu 241000/LW ofstream outFile  j‰seneksi, metafilen pit‰‰ tulla samaan   
//Muutettu 301100/LW +SetFirstObjectActivity()
//Muutettu 081200/LW +itsMagicSavePath
//Muutettu 231200/LW +itsArea
//Muutettu 231200/LW +WriteScalingObjects()
//Muutettu 250101/LW +fMakeElementsAfterSegments

//----------------------------------------------------------------------------

#ifndef  __NPRESSPR_H__
#define  __NPRESSPR_H__

#ifndef __NPRESSPA_H__
#include "npresspa.h"
#endif//__NPRESSPA_H__

#ifndef __NPRTIDES_H__  
#include "nprtides.h"
#endif//__NPRTIDES_H__
 
#ifndef __NQDLIST_H__
#include "nnamedqd.h"
#endif//__NQDLIST_H__
 
#ifndef __NFILESTR_H__
#include "NFmiFileString.h"
#endif//__NFILESTR_H__

#ifndef __NRECTSCA_H__
#include "nrectsca.h"
#endif//__NRECTSCA_H__

#ifndef __NVPLIST_H__
#include "NFmiVoidPtrList.h"
#endif//__NVPLIST_H__

#ifndef __NPRESAME_H__
#include "npresame.h"
#endif//__NPRESAME_H__

#ifndef __NPRIMAGE_H__
#include "nprimage.h"
#endif//__NPRIMAGE_H__


#ifndef __NCMYKBAG_H__
#include "ncmykbag.h"
#endif//__NCMYKBAG_H__

#include "npstatex.h"
#include "nhyphstr.h"
#include "NFmiPressArea.h"

class NFmiLocationFinder;
class NFmiNameDay;
//class NFmiSuperSmartInfo;
class NFmiFastQueryInfo;
#include "NFmiPreProcessor.h"

#include<queue>
#include<deque>

typedef enum
{
	 dProduct			=20 
	,dOutDir
	,dMapFile
	,dDataFile
	,dMapIsReduced
	,dPlottingView
	,dMapDefSize
	,dClippingRectangle
	,dBoundingBorder
	,dSubViews
	,dMargin           //=30
	,dGap
//	,dLanguage
	,dPressParam
    ,dSymbolPlaces
	,dPalette           
	,dSubImage          
	,dPageSize          
	,dDate       
	,dProductWithTimeStamp               //080299
	,dProductWithDataTimeStamp // = 1020  //080299
	,dTimeStampFormat       = 1020            //040399
	,dProductSizeFactor                 //110299
	,dProductPlaceMove                  //240299
	,dRelativeHours                     //251099
	,dChangeScandinavian				//170100
	,dNameDay							//250100
	,dProductNameFormat                 //160300
	,dProductNameTimeFormat              //220300
	,dSecondProductNameTimeFormat        //271000
	,dOutputMode                         //200600
	,dNumberAddingToName   = 2020        //050900
	,dMagicSavePath                      //8.12.00
	,dMapArea                            //17.12.00
	,dMakeElementsAfterSegments          //250101
	,dMaskFile                           //120901
	,dWeekdayDir                         //21.5.02
}NFmiPressProductObjects;

struct FmiPressSeasons   //20.5.02
{
	 bool wintertime;
	 bool summer;
	 bool pollen;
	 bool snow;
	 bool pollenOrSnow;  //4.9.02
	 int weekday;
	 int dayAdvance;
};
 
class _FMI_DLL NFmiPressProduct : public NFmiPressTimeDescription 
{

 public:

	NFmiPressProduct(void);
 //  NFmiPressProduct(NFmiQueryData* theData); //3.12
	
	virtual ~NFmiPressProduct (void);

	FmiBoolean WritePS(FmiPressOutputMode=kPostScript); //15.3.00 mod.

//	NFmiPressProduct	&operator= (const NFmiPressProduct &thePressProduct);
    void ReadPalette(void);   
	virtual FmiBoolean ReadDescriptionFile(NFmiString inputFileName); 
	virtual FmiBoolean ReadDescription(NFmiString& retString);  
   int     ConvertDefText(NFmiString & object);
	FmiBoolean SetSegmentData(const NFmiString& theDataName); //22.3.00
	FmiBoolean PreProcessDefinition(const std::string& inFileName, const std::string& outFileName); //25.10.01
   FmiBoolean PreProcessProduct( std::ifstream& origInput,  std::ofstream& output);    
//   FmiBoolean CleanFromComments(std::ifstream& inFile);
//	NFmiString FirstDataName(void);                   
	FmiBoolean SetAllLanguages(FmiLanguage theLanguage); //24.3.00   
	NFmiStationPoint FirstParamLocation(); //16.3.00 
	NFmiQueryData* DataByName(NFmiString givenName); 
	NFmiQueryData* FirstData(void); 
	NFmiString FirstDataName(void); //030399
	NFmiString FirstSegmentName(void);//12.6.00
	FmiBoolean SetMaskNumber(unsigned long theNumber); //7.9.01
	FmiBoolean SetFirstStation(const NFmiLocation& theLocation); //17.3.2000   
	FmiBoolean SetStationRename(const NFmiRenaming& theRename); //7.6.00 
	FmiBoolean SetImagePreNames(const NFmiLocation& theLocation); //29.9.00   
	FmiBoolean SetProductName(const NFmiString& theName);//20.3.00 
	FmiBoolean SetProductNameFormat(const NFmiString& theName);//15.1.01 
	FmiBoolean SetLogFile(FmiBoolean OnOff);
    FmiBoolean ReadData();  
	FmiBoolean DefinePar(const NFmiString& theString);  
	FmiBoolean ReplacePar(NFmiString& theString);  
	FmiBoolean ReadNameToLonLatList(); //11.8.99
	FmiBoolean Close(); //17.3.00 
	NFmiString GetInFileName(void)const {return itsInFileName;}; //21.3.00
	FmiBoolean SetAllTimes(const NFmiMetTime& theTime); //22.3.00   
//	NFmiString StationName(); //8.6.00
	void ActivateNumberToName(long theStartNumber){itsCurrentNumberToName = theStartNumber;}; //1.9.00
	void DeActivateNumberToName(void){itsCurrentNumberToName = kLongMissing;}; //1.9.00
	FmiBoolean SetFirstObjectActivity(bool theActivity);     //30.11.00   
	void PutInStorage(const float value, int queueNum=1);              //18.9.01 
	float UseFromStorage(int queueNum=1, bool errorReport = true); //errorReport 4.10.01

 protected:
	 void StepMap(void);
	 void Scale(void);


 private:
	 FmiBoolean ReadQueryData(NFmiQueryData* theQD 
		                     ,char *fileName);
	 FmiBoolean ConstructOutFileName(); //16.3.00
	 bool WriteScalingObjects(bool doPreSegments, FmiPressOutputMode theOutput); //24.1.01 
	 bool WriteSameSymbols(bool doPreSegments, FmiPressOutputMode theOutput); //20.11.01 
	//Mika ehdottaa const pois
	 bool WriteMetaInit(void); //; 18.3.02const; 
	 bool GetSeasonsStatus(FmiPressSeasons& theSeasonsStatus); //20.5.02
	 bool WeekdayDirectiveActive(const std::string & theDefinition, const FmiPressSeasons& theSeasons) const; //21.5.02
	 bool PreProcessConditionally(NFmiPreProcessor& thePrepr, bool theCondValue, const std::string & theConditionalBeginDirective //24.5.02
		                                , const std::string & theConditionalNotBeginDirective
										, const std::string & theConditionalEndDirective, const std::string & theConditionalElseDirective);
public:
 	 NFmiLocationFinder* itsNameToLonLat; //11.8.99
	 std::queue <float /*, std::deque<float>*/> itsFloatQueue; //17.9.01
	 std::queue <float /*, std::deque<float>*/> itsFloatQueue2; //28.9.01

 private:
	 bool fMakeElementsAfterSegments; //25.1.01
	 NFmiNameDay* itsNameDay;       //26.1.00
	 FmiBoolean fChangeScandinavian; //17.1.00
	 FmiBoolean fDataRead;    //050199
	 FmiPageSize itsPageSize; 
	 NFmiString itsPaletteName;
	 NFmiCmykColorBag* itsPalette;
	 unsigned long itsProducer; 
	 unsigned short itsHourStep;   
	 NFmiString itsPath;           
	 NFmiVoidPtrList itsEpsFileNames; 
	 NFmiString itsDataFileName;	
	 NFmiString itsMaskFileName;  //12.9.01	
 	 NFmiString itsMagicSavePath;   
 	 NFmiString itsOutDir;    
 	 NFmiHyphenationString itsProductNameFormat; 
	 FmiBoolean fTimeStamp;  
	 FmiBoolean fDataTimeStamp;  
	 NFmiFileString itsOutFile;    
	 NFmiString itsOutFileName;    
	 std::ofstream outFile;           
	 NFmiString itsInFileName;      
	 FmiBoolean fMapIsEps;
	 double itsMapGap;
	 double itsMargin;
//	 FmiLanguage itsLanguage; 
	 FmiCounter itsNumberOfMaps;
	 FmiCounter itsCurrentMap;
	 NFmiRectScale itsScale;
	 NFmiRect itsPlottingRect;
	 NFmiRect itsClippingRect;
	 NFmiRect itsBoundingBorder;      
//	 NFmiSuperSmartInfo* itsCurrentDataIter;   
	 NFmiFastQueryInfo* itsCurrentDataIter;   
//	 NFmiFastQueryInfo* itsMaskIter; //12.9.01 
	 NFmiSuperSmartInfo* itsMaskIter; //12.9.01 
	 NFmiVoidPtrList itsDatas;        
	 NFmiVoidPtrList itsParams;
     NFmiVoidPtrList itsSameSymbols;         
	 NFmiVoidPtrList itsObjects;
	 NFmiString itsLogFileName; //21.3.00 Tarvitaanko
	 unsigned long itsNameTimeFormat; 
	 unsigned long itsSecondNameTimeFormat; //27.10.00 long?????
//	 NFmiString itsStringNameTimeFormat; 
	 FmiPressOutputMode itsOutputMode;  
	 long itsCurrentNumberToName;      
	 unsigned long itsNumOfWritePS; 
	 NFmiPressArea itsArea; 
};


#endif// __NPRESSPR_H__
