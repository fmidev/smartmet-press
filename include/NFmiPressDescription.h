//© Ilmatieteenlaitos/Lasse.
//Original 12.1.1998
//Muutos 050398/LW konstruktoreita.
//Muutos 250998/LW +itsHomePath
//Muutos 171198/LW +itsEnvironment, ReadRemaining()
//Muutos 240300/LW ReadLanguage()
//Muutos 181000/LW +rgb
//Muutos 220101/LW +dUpOneLevel; pakotettu yhden tason nousu, esim. #KarttaLoppu
//****************************************************************
// Peritty yleisest‰ NFmiDescription-luokasta. T‰‰ll‰ ovat sellaiset
// ominaisuudet, jotka ovat yhteisi‰ lehtituottieden luokkien 
// m‰‰rittelyiss‰.   
//****************************************************************
//---------------------------------------------------------------------------

#ifndef __NPRDESCR_H__
#define __NPRDESCR_H__

#include "NFmiDescription.h"
#include "NFmiPressTypes.h"   //23.9
#include "NFmiPressEnvironment.h"   //171198

const double c40 = 40; //14.9 T‰m‰ nyt on t‰llainen kun tarvitaan rinnan vanhaa ja 
                       // uutta skaalausta //28.9 t‰nne

typedef enum				 
{
	 dProducer = 50     	  
	,dTextObject                    //9.3  //16.3 t‰nne
	,dColumnTextObject                    //25.5
	,dTimeTextObject                 //10.3
	,dComputerTimeTextObject         //9.4
    ,dImageObject                    //11.3
    ,dStationTextObject              //16.3
	,dLongNumberMinus         //171198 eka Environmentiin
	,dShortNumberMinus        //171198
	,dLanguage                //280800 t‰nne
	,dRGBColor = 1050          //181000
	,dColor                    //031100 t‰nne
	,dMaskNumber               //7.9.01
}NFmiPressDescriptionObjects;

class _FMI_DLL NFmiPressDescription : public NFmiDescription 
{
	public:
		NFmiPressDescription(void){};
//		{itsCommentLevel=0;};
		NFmiPressDescription(const NFmiPressDescription& theD)    //5.3
			:NFmiDescription(theD.GetLogFile(),theD.GetMaxLoopNum()){};

		NFmiPressDescription(std::ofstream* theLogFile,                 //5.3
		                unsigned short theMaxLoopNum)
						:NFmiDescription(theLogFile, theMaxLoopNum){}; 
		
		virtual ~NFmiPressDescription(void){;};

//		virtual bool		ReadDescription(void) = 0;
//		virtual NFmiParamRect*	Clone(void) const = 0; 

/*		virtual bool	WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
									,NFmiQueryData* theQueryData
									,ofstream theDestinationFile )=0;
*/
        virtual int ConvertDefText(NFmiString & object); 
		NFmiString GetHome(void)const {return itsHomePath;} ;
		void SetHome(const NFmiString& path) {itsHomePath=path;};
		void                 SetEnvironment(const NFmiPressEnvironment& theEnvironment) {itsEnvironment=theEnvironment;};
		NFmiPressEnvironment GetEnvironment(void) const {return itsEnvironment;};
		FmiLanguage ReadLanguage(void); //24.3.00
		bool IsRGB(void) const {return itsEnvironment.IsRGB();}; //18.10.00
//		bool GetRGB(FmiRGBColor& rgb)const {return itsEnvironment.GetRGB(rgb);}; //18.10.00
		bool IsCMYK(void) const {return itsEnvironment.IsCMYK();};					//2.11.00
//		bool GetCMYK(FmiCMYK& cmyk)const {return itsEnvironment.GetCMYK(cmyk);}; //2.11.00
		FmiGenericColor GetColor(void)const {return itsEnvironment.GetColor();}; //2.11.00
		void SetColor  (const FmiGenericColor& theColor){itsEnvironment.SetColor(theColor);}; //3.11.00 
		bool IsPureBlack(void); //3.11.00

	protected:
		virtual bool ReadRemaining(void);    //171198  

	protected:
	    NFmiPressEnvironment itsEnvironment;  
		NFmiString itsHomePath; 
		FmiLanguage itsLanguage; //28.8.00 t‰nne pressProductista
	private: 

};


#endif // __NPRDESCR_H__
