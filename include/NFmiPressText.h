// © Ilmatieteenlaitos/Lasse.
// Original 6.3.1998
//
//********************************************************
// Hoitaa kaikenlaisten tekstien (aikamerkinnät, asemannimet
// vakiotekstit, palstat) ps-tuottamisen. Kaikki tekstiin
// liittyvä (fontti,väri,kohdistus, tyyli) samoin kuin itse 
// teksti on täällä. Myös sovelluksen kieli, joka vaikuttaa
// aikamerkintöihin, hoidetaan täältä.
//********************************************************
// 
//Muutettu 190398/LW +itsLanguage    
//Muutettu 300998/LW +itsString -> itsText koska itsString myös Descriptionissa    
//Muutettu 091098/LW +IsPureBlack()
//Muutettu 161098/LW itsAlignment=kCenter oletus kun hävisi PsWritestä
//Muutettu 231098/LW +fAddLocalTime, +dAddLocalTime
//Muutettu 231098/LW +itsCharSpace
//Muutettu 081298/LW +vakioteksti etten/taakse; pienillä/isoilla kirjaimilla
//Muutettu 171298/LW +fInParagraph, itsParagraphMove
//Muutettu 181298/LW ****YHDISTETTY NFmiColumnText TÄHÄN****
//Muutettu 181298/LW itsLastLineStep
//Muutettu 210199/LW palstaan lisätty ekan rivin sisennys
//Muutettu 220199/LW rivinsiirtoväli säädettävissä
//Muutettu 250899/LW +fLoopErrorReported
//Muutettu 131099/LW +itsSubText,dSubTextObject jne, GetText()
//Muutettu 150300/LW erilaisia output-formaatteja mahdollistettu
//Muutettu 160800/LW + tyhjä SetTime()
//Muutettu 160800/LW itsLanguage pois, on PressDescriptionissa
//Muutettu 010900/LW +WritePSUpdatingSubText()
//Muutettu 031100/LW oma väri pois, environmentissä geneerinen nyt
//Muutettu 250101/LW +dTextDir
//Muutettu 160208/LW +AddTimeStamp, dTimestampDayGap
//---------------------------------------------------------------------------

#ifndef __NPRETEXT_H__
#define __NPRETEXT_H__

#include "NFmiPressTypes.h"  

typedef enum
{
//	 dTextColor	= 140 pois 3.11.00
	 dTextAlignment = 140
	,dTextFont
	,dTextStyle
	,dTextString
	,dAddLocalTime  
	,dCharSpace     
	,dAddAfter      
	,dAddInFront      
	,dLowerCase      
	,dUpperCase      
	,dInParagraph  =1140
	,dParagraphMove
	,dTopMargin	         
	,dLeftMargin
	,dRightMargin
	,dParagraphBorders
	,dLineStep
	,dIndent
	,dLF
	,dLFFactor   
	,dSubTextObject  =2140           //131099
	,dSubTimeTextObject
	,dSubComputerTimeTextObject
	,dTextLanguage
	,dTextDir                        //250101
	,dTextPath                        //250101
}NFmiPressTextObjects;

#include "NFmiPressScaling.h"
//#include "NFmiHyphenationString.h"
//class NFmiHyphenationString;
class _FMI_DLL NFmiPressText : public NFmiPressScaling
{
	public:
		NFmiPressText(void)
		  :NFmiPressScaling()
		  ,itsLastLineStep (0.)
		  ,itsLineStep (0.)
		  ,itsLineStepFactor (0.)
		  ,itsTopMargin(-1000.)
		  ,itsIndent (0.)
		  ,fLoopErrorReported(false)
		  ,fInParagraph(0)
		  ,fUpperCase(0)
		  ,fLowerCase(0)
		  ,fAddLocalTime(false)  
		  ,itsText(0)
		  ,itsFont(NFmiString("Courier"))
		  ,itsAlignment(kCenter) 
		  // Mika: Ei ole maaritelty muuttuja
		  // ,itsLanguage(kFinnish)
		  ,itsCharSpace(0.)      
		  {/*itsColor.cyan =0.f;				 
		     itsColor.magenta =0.f; 
		     itsColor.yellow =0.f;
		     itsColor.black =1.f*/;
		     itsSubText = 0;
		     itsLanguage =kFinnish; //29.8.00
		  }; 
		NFmiPressText(const NFmiPressText& theTextParamRect); 

		virtual ~NFmiPressText();

		virtual bool		ReadDescription(NFmiString& retString); 
		virtual bool		ReadRemaining(void); 
//		virtual NFmiParamRect*	Clone(void) const; 
        int ConvertDefText(NFmiString & object);
		NFmiString Construct(NFmiString *theString) const;

		virtual bool      WritePS(FmiPressOutputMode theOutput);  //15.3.00
		void SetValueDepending(float theValue);
//		void SetColor(float theC, float theM, float theY, float theK);
		virtual bool SetText(void){return true;};//131099 tarvitaan NFmiGiventextiä varten
		virtual void SetText(NFmiString theText)
		                           {if (itsText)
										delete itsText;
		                            itsText = new NFmiString(theText);};
		void SetTextAttributes(NFmiString theFont      
			                  ,FmiDirection theAlignment
							  ,NFmiString theStyle
							  /*,FmiCMYK theColor*/)							
		                {itsFont = theFont;
		                 itsAlignment = theAlignment;
                         itsStyle = theStyle;
                         /*itsColor = theColor;*/};
        bool WriteString(const NFmiString& commentString,FmiPressOutputMode theOutput);																					

	    virtual bool WritePSUpdatingSubText(FmiPressOutputMode theOutput){return WritePS(theOutput);}; //31.8.00

//		bool IsPureBlack(void); 
		NFmiString GetFont(void)const {return itsFont;};
		FmiDirection GetAlignment(void)const {return itsAlignment;};
		NFmiString GetStyle(void) const{return itsStyle;};
//		FmiCMYK GetColor(void)const {return itsColor;}; //3.11.00
		NFmiString* GetText(void)const {return itsText;};
		void SetLanguage(FmiLanguage theLanguage) {itsLanguage=theLanguage;}; 
		FmiLanguage GetLanguage(void) {return itsLanguage;}; 
	    virtual	bool      IsInParagraph(void){return fInParagraph;};
		virtual void ScalePlotting(void);
		double GetLineStep(void) const {return itsLineStep;};
		void SetLastLineStep(double lineStep)  { itsLastLineStep=lineStep;};
		// Mika: Lisasin constin
		virtual void SetTime (const NFmiMetTime& theTime) {}; //16.8.00 tarvitaan GivenTimeTextissä

	protected:               

		double itsLastLineStep;//181298 tarvitaan jotta ei uusi kappale alkaisi kuin olisi vanha rivinväli 
		double itsLineStep;
		double itsLineStepFactor;  //220199
		double itsTopMargin;
		double itsRightMargin;
		double itsLeftMargin;
		double itsIndent;          //210199
		bool fLoopErrorReported; //250899
		bool fInParagraph;     
		NFmiPoint  itsParagraphMove;  
		NFmiString itsAddInFront;   
		NFmiString itsAddAfter;     
		bool fUpperCase;  
		bool fLowerCase;   
		bool fAddLocalTime;  
//		FmiLanguage itsLanguage; //29.8.00 Pois, onnistuuko
		NFmiString* itsText;       
		NFmiPressText* itsSubText;  //131099       
		NFmiString itsFont;
		FmiDirection itsAlignment;
		NFmiString itsStyle;
//		FmiCMYK itsColor;    //3.11.00
		double itsCharSpace;      
//		bool fColorValueDependent; ei ole arvoa	  

};

#endif //__NPRETEXT_H__
