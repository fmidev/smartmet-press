//© Ilmatieteenlaitos/Salla.
//Original 21.10.1997
// 
//********************************************************
// 
// Tämä on perusluokka datariippuvien ps-olioiden tuottamiselle.
// Sääsymboli- numeroluokat peritään tästä. Tämän luokan valmistumisen jälkeen 
// on osittain rinnakkainen luokkahierarkia lähtenyt kehittymään
// (jossa NFmiPressScaling keskeinen). Ps-koodin kirjoittaminen 
// tapahtuu tässä erillisillä extern-funktioilla kun taas NFmiPressScaling
// on peritty asiaa hoitavasta NFmiPsWriting-luokasta. 
// Määrittelytiedoston lukeminen sentään periytyy yhteisestä emosta.
// NFmiRect:stä periytymisellä on hoidettu skaala-asioita. 
//
//********************************************************
//
//Changed	18.11.97	/Salla	Clone()	
//			19.11.97	/Salla	theDataIdent() -> const
//Muutettu 201197/LW symbolDir pois
//Muutettu 201197/LW +ConvertDefText()
//Muutettu 211197/LW +NFmiParamRectObjects
//Muutettu 121297/LW peritty myös NFmiDescriptionista
//Muutettu 291297/LW Producer ja param nyt myös yhdellä rivillä
//Muutettu 120198/LW perintä NFmiPressDescriptionista, +itsProducer
//Muutettu 040298/LW +paramErrorReported
//Muutettu 040298/LW +NumOfMissing()
//Muutettu 050398/LW copyKonstruktori ym
//Muutettu 200398/LW +PointParam()
//Muutettu 290698/LW perintä NFmiPressTimeDescription:sta eikä NFmiPressDescription:sta
//Muutettu 110998/LW +itsSymbolGroup
//Muutettu 051098/LW +itsRelRect
//Muutettu 091098/LW +dAbsPlace
//Muutettu 010299/LW +SetIdent(),SetRect()
//Muutettu 090299/LW +itsValueFactor, itsCurrentValue tänne Symbolista
//Muutettu 130899/LW +itsPressParam
//Muutettu 160899/LW PointParam()->PointOnParam()
//Muutettu 230899/LW +kirjoituspuolelle (WritePS jne) QD korvatti QI:llä
//Muutettu 300899/LW ReadCurrentValue(...+value)
//Muutettu 080999/LW +itsSymbolIntegrationPeriod
//Muutettu 230999/LW multiParamMapping ja kaikki mikä siihen kuuluu:PointOnMultiParam() jne
//Muutettu 011199/LW +FloatValue()
//Muutettu 030200/LW maksimi/minimi/keskiarvo
//Muutettu 080200/LW itsCurrentSegmentTime
//Muutettu 150200/LW ReadCurrentValue(,,localTimeSet)
//Muutettu 150200/LW +SetRelativeHour()
//Muutettu 160200/LW +itsCurrentTime, tätä editomailla ei tartte koskea QI:n aikaan kesken ajanmäärityksen
//Muutettu 150300/LW metakieli ja xml output eps:n lisäksi
//Muutettu 280300/LW +SunElevation(), +itsCurrentPar
//Muutettu 120400/LW +itsLevel
//Muutettu 020500/LW PointOnParam(,+toka par)
//Muutettu 030500/LW min,max,keskiarvolle myös suhteelliset rajatunnit
//Muutettu 050500/LW +levelErrorReported
//Muutettu 270600/LW +InitMissing()
//Muutettu 161000/LW ConvertDefText() virtuaaliseksi, ja taas pois
//Muutettu 281100/LW +dYearPeriod,+itsFirstExtremYear ja Last
//Muutettu 010201/LW +FmiPressValueOption, +itsIdentPar
//Muutettu 180501/LW +itsAreaModifier aluesummat,keskiarvot ym; pitäisi rakentaa riippumattomasti ajan kanssa
//Muutettu 310501/LW +itsRealPar (kun par=keskituuli itsRealPar voi olla minimi=362)
//Muutettu 180901/LW +fUseFromStorage,+fPutInStorage,+fIsProbability
//Muutettu 210901/LW +fAllowMissing
//Muutettu 030502/LW +fTimeErrorReported
//Muutettu 030602/LW +UpdateModifierTimes
//---------------------------------------------------------------------------

#ifndef __NPARRECT_H__
#define __NPARRECT_H__

#include "NFmiRect.h"
#include "NFmiDataIdent.h"
#include "NFmiSuperSmartInfo.h"
//#include "NFmiFastQueryInfo.h"
#include "NFmiSaveBase.h"
#include "NFmiRectScale.h"
#include "NFmiPressTimeDescription.h"  
#include "NFmiPsWriting.h"  //15.3.00
#include "NFmiMultiParamMapping.h"  //23.9.99

class _FMI_DLL NFmiPressParam;

extern bool NFmiCopyFile(std::ifstream&, std::ofstream&);
extern bool NFmiWritePSConcat(NFmiRectScale, std::ofstream&);
extern bool NFmiWritePSConcatRotating(NFmiRectScale, float, std::ofstream&); //LW 20.11
extern bool NFmiWritePSEnd(std::ofstream&);

typedef enum				 
{
	 dRelPlace		= 60
	,dPlaceMove   
	,dParam
	,dValueFactor   
	,dIntegrationPeriod  
	,dMaximum                
	,dMinimum               
	,dMean                  
	,dWeightedMean         
	,dSum                    
	,dLevel       =1060      
	,dYearPeriod             
	,dWindCill               
	,dDegreeDays             
	,dFahrenheit             //010201
	,dAreaMaximum                //180501
	,dAreaMinimum                //180501
	,dAreaMean                   //180501
	,dAreaSum                    //180501
	,dAreaUnion                    //240901
	,dPutInStorage    =5060        //180901
	,dUseFromStorage              //180901
	,dProbability                //180901
	,dAllowMissing               //210901
//	,dTimeTableActive            //260901
	,dUseFromStorageCond         //280901
	,dStorageQueue               //280901  1 or 2
	,dAcceptanceInterval               //2.11.01  
}NFmiParamRectObjects;
/*
struct FmiLoopActivity   //26.9.01
{
	 unsigned long startIndex;
	 unsigned long step;
	 unsigned long stopIndex;
	 bool bypassWithValue; //2.10.01
	 float bypassValue;   //2.10.01
};
*/
struct FmiIntegrationPeriod
{
	 unsigned long period;
	 float startWeight;
	 float centreWeight;
	 float endWeight;
};

typedef enum	//3.2.2000			 
{
	kNoneModifier
	,kMinimum
	,kMaximum
	,kMean
	,kWeightedMean
	,kSum      //9.5.00
	,kUnion    //24.9.01 sateen olomuoto
}FmiModifier;

typedef enum	//1.2.01			 
{
	 kNoneValueOption
	,kFahrenheit
	,kDegreeDays
}FmiPressValueOption;

class NFmiParamRect : public NFmiRect, public NFmiPressTimeDescription  
{
	public:
		NFmiParamRect(void)
		  : NFmiRect()  
		  {
		    fParamErrorReported=false;
		    fLevelErrorReported=false;  //5.5.00
		    fTimeErrorReported=false;  //3.5.02
		    itsNumOfMissing = 0;
		    itsValueFactor = 1.;
		    itsIntegrationPeriod.period = kUnsignedLongMissing; // 6.2.2002 Mika
		    itsIntegrationPeriod.startWeight = kFloatMissing;
		    itsMultiMapping = 0;
		    itsCurrentParamArray = 0;
		    itsMultiParams = 0;
		    itsModifier = kNoneModifier;
		    itsAreaModifier = kNoneModifier;
		    itsValueOption = kNoneValueOption; //1.2.01
		    itsFirstExtremYear = kLongMissing;  //28.11.00
		    itsIdentPar = kFmiTemperature;//2.2.01
		    itsRealPar = 0;     //31.5.01
		    fUseFromStorage = false; //18.9.01
		    fUseFromStorageConditionally = false; //28.9.01
		    fPutInStorage = false; //18.9.01
		    itsStorageQueue = 1;  //28.9.01
		    fIsProbability = false;  //18.9.01
		    fAllowMissing = false;   //21.9.01
		    // itsLoopActivity.startIndex = 0; //26.9.01
		    // itsLoopActivity.bypassWithValue = false; //2.10.01
		    itsFirstExtremRelHour = kLongMissing;  //27.9.01
		    itsValueIntervalMin = kFloatMissing;   //2.11.01 max ei tartte
		};  

  
		NFmiParamRect(NFmiDataIdent theParam,
			      NFmiRect theRect,
			      // unsigned long theProducer,
			      std::ofstream* theLogFile,
			      unsigned short theMaxLoopNum)
			:NFmiRect(theRect)
			,itsValueIntervalMin(kFloatMissing)  //2.11.01 
			,fAllowMissing(false)   //21.9.01
			,fUseFromStorage(false) //18.9.01
			,fUseFromStorageConditionally(false) //28.9.01
			,fPutInStorage(false) //18.9.01
			,fIsProbability(false) //18.9.01
			,itsRealPar(0)
			,itsCurrentParamValue (kFloatMissing)
			,itsValueFactor (1.)
			,fParamErrorReported(false)
			,fLevelErrorReported(false)
			,fTimeErrorReported(false)  //3.5.02
			,itsNumOfMissing(0)
			,itsMultiMapping(0)
			,itsCurrentParamArray(0)
			,itsMultiParams(0)
			,itsFirstExtremYear(kLongMissing)            //28.11.00
			,itsModifier(kNoneModifier)
			,itsAreaModifier(kNoneModifier)
			,itsValueOption(kNoneValueOption)     //1.2.01
			,itsIdentPar(kFmiTemperature)   //2.2.01
			,itsDataIdent(theParam)
			// ,itsProducer(theProducer)
		{
				itsLogFile = theLogFile;
				itsMaxLoopNum = theMaxLoopNum;
				itsIntegrationPeriod.period = kUnsignedLongMissing; // 6.2.2002 Mika
				itsIntegrationPeriod.startWeight = kFloatMissing;
//				itsLoopActivity.startIndex = 0; //26.9.01
		};

		NFmiParamRect(const NFmiParamRect& theRect);  
		
		virtual ~NFmiParamRect(void);

//		virtual bool		ReadDescription(void) = 0; 
		virtual NFmiParamRect*	Clone(void) const = 0; 

		virtual bool	WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
									,NFmiFastQueryInfo* theQueryInfo //4.9.01
									,std::ofstream& theDestinationFile 
									,FmiPressOutputMode theOutput)=0;   //13.3.00
//									,const NFmiString& theSymbolDir)=0; //LW 20.11 pois
		int NumOfMappingParams(void);   //240999
		NFmiDataIdent	GetDataIdent(void) const;        
//		unsigned long	GetProducer(void) const {return itsProducer;};          //5.4
/*		NFmiDataIdent	GetParamErrorReported(void)const { return itsDataIdent; }; //5.4
		NFmiDataIdent	GetNumOfMissing(void)const { return itsDataIdent; };      //5.4
		NFmiDataIdent	SetParamErrorReported(NFmiDataIdent)const {itsDataIdent=theDataIdent; }; //5.4
		NFmiDataIdent	SetNumOfMissing(void)const ;      
*/
		bool SetPressParam (NFmiPressParam* pressParam) 
								{itsPressParam = pressParam;
								return true;};
		virtual bool	Set(NFmiDataIdent theParam, NFmiRect theRect);
		bool	SetIdent(NFmiDataIdent theParam); 
		bool	SetRect(NFmiRect theRect);        
		long   NumOfMissing(void) const {return itsNumOfMissing;};
		void   InitMissing(void) {itsNumOfMissing=0;};   //27.6.00
        /*virtual*/ int ConvertDefText(NFmiString & object); //16.10.00 
		bool      IsNewScaling(void) {return fNewScaling;}; 
		void            SetNewScaling(bool newScaling) {fNewScaling=newScaling;}; 
		virtual void SetLanguage(FmiLanguage){;} //24.3.00 tulee timeParamRect:ssä
		bool UpdateModifierTimes(void); //3.6.02

		// kelpaavat ihan yleiseen käyttöön:
//		float DegreeDays(float value, int month);
//		float Celsius2Fahrenheit(float value);

	protected:
		bool SetRelModifierTimes(long startRelHour, long endRelHour);
		long RelativeHour(void)const;             
		bool SetRelativeHour(NFmiFastQueryInfo* data, const NFmiString& calledFrom); 
		NFmiMetTime CalculatePeriodTime(long theHour); 
		bool SetStationLocalTime(NFmiFastQueryInfo* theQI); //15.5.02 
//		bool ResetFromLocalTime(NFmiFastQueryInfo* theQI); 
		virtual FmiInterpolationMethod InterpolationMethod(NFmiFastQueryInfo* theQueryInfo);   
		bool ReadCurrentValueArray(NFmiFastQueryInfo* theQueryInfo); 
		bool ReadCurrentValue(NFmiFastQueryInfo* theQueryInfo, float& value,bool localTimeSet=false); 
		bool FloatValue(NFmiFastQueryInfo* theQueryInfo, float& value); 
        virtual bool PointOnParam (NFmiFastQueryInfo* theQI, NFmiParam* theParam); //+toka par., 2.5.00
		virtual bool PointOnMultiParam(NFmiFastQueryInfo* theQI, short theNum);//23.9.99 
		virtual bool PointOnLevel(NFmiFastQueryInfo* theQI);  
		virtual bool ReadRemaining(void);  
		float SunElevation(NFmiFastQueryInfo* theQueryInfo); 
//		NFmiString AlignmentToMeta (const FmiDirection& pressAlignment)const;  //24.10.00
	private:
		int WaWa2PresentWeather(int value, NFmiFastQueryInfo* theData); //4.9.01
		int PreWeaWithPrecForm(int value, NFmiFastQueryInfo* theData);          //4.9.01

	protected:
		float itsValueIntervalMin; //2.11.01
		float itsValueIntervalMax; //2.11.01
//		FmiLoopActivity itsLoopActivity; //26.9.01
		bool fAllowMissing; //21.9.01
		bool fUseFromStorage; //18.9.01
		bool fUseFromStorageConditionally; //28.9.01
		bool fPutInStorage;   //18.9.01
		int itsStorageQueue; //28.9.01
		bool fIsProbability;   //18.9.01
		NFmiLevel itsLevel; 
		NFmiMetTime itsCurrentSegmentTime; 
		NFmiMetTime itsCurrentTime;        
		unsigned long itsCurrentMultiParNum; 
		unsigned long itsCombinedIdent;
		unsigned long itsCurrentPar;   // 280300 mahdollistaa auringonKorkeuden (ja muiden) erilaisen käsittelyn 
		unsigned long itsRealPar;      //31.5.01
		float    itsCurrentParamValue; 
		double   itsValueFactor;       
		NFmiRect itsRelRect; //Suhteellisen alueen muutokset kerätään tähän
		bool fNewScaling;    
        bool    fParamErrorReported;  
        bool    fLevelErrorReported;  
        bool    fTimeErrorReported; //3.5.01 
		long itsNumOfMissing;     
		NFmiPressParam* itsPressParam; 
		FmiIntegrationPeriod itsIntegrationPeriod; 
		NFmiMultiParamMapping* itsMultiMapping; // yhteinen symbolille ja letterille vaikka tav mappi ilmeisesti erikseen
		float* itsCurrentParamArray;   
		FmiParameterName* itsMultiParams;//[FmiMaxNumOfMappingParams+1]; //23.9.99, vika = lopetusarvo
	    long itsFirstExtremRelHour; //27.9.01 Uudet metodit tarttee
	    long itsLastExtremRelHour;  
	    long itsFirstExtremHour;  
	    long itsLastExtremHour;  
	    long itsFirstExtremYear;  //28.11.2000
	    long itsLastExtremYear;  //28.11.2000
		FmiModifier itsModifier;     
		FmiModifier itsAreaModifier; //18.5.01     
		FmiPressValueOption itsValueOption; //1.2.01     
		NFmiPsWriting itsPsWriting;  //25.10.00 jotta Mikan metaKonversio sieltä
		unsigned long   itsIdentPar; //1.2.01 jotta ReadDescriptionissa voidaan asettaa kun luettu ReadRemainingissä
	private:
		NFmiDataIdent	itsDataIdent;
};

inline NFmiDataIdent NFmiParamRect::GetDataIdent() const { return itsDataIdent; }


#endif // __NPARRECT_H__
