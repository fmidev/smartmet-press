// © Ilmatieteenlaitos/Lasse.
// Original 6.11.1997
// 
// 
//****************************************************************
// T‰m‰ abstrakti luokka hallitsee dataohjattujen tekstien tuottamisen
// m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
// Lapset NFmiNumberParamRect ja NFmiLetterParamRect.
// 
//****************************************************************
//
// Muutos 18.11.97 Clone()	Salla
// Muutos 19.11.97 Copy constr.	Salla
// Muutettu  20.11.97 /LW 	ConvertDefText() j‰seneksi
//     - " -               itsSubDir ei en‰‰ tuoda ylemp‰‰ vaan luetaan suoraan ymp.muuttujasta
// Muutettu 211197/LW +FmiCMYK
// Muutettu 211197/LW  NFmiTextParamRectObjects t‰nne cpp:st‰ typistettyn‰
// Muutettu 241197/LW  v‰rille oletusarvo=musta, +fColorValueDependent
// Muutettu 271197/LW +itsFormat
// Muutettu 111297/LW oletusCMYK=0001 (oli 1111)
// Muutettu 160198/LW ReadDescription():iin palautusStringi   
// Muutettu 200898/LW +Sulkuoptio   
// Muutettu 121098/LW +WriteCode(), IsPureBlack()   
// Muutettu 161098/LW itsAlignment=kCenter oletus, kun Writest‰ h‰visi   
// Muutettu 121198/LW +fUseSelectLatinFont
// Muutettu 161198/LW WriteShowString
// Muutettu 300699/LW +itsAddInFront,itsAddAfter,ReadRemaining(),Construct()
// Muutettu 230899/LW +kirjoituspuolelle (WritePS jne) QD korvatti QI:ll‰
// Muutettu 181199/LW Muunnos (ei moniMuunnos) siirretty Letterist‰ t‰nne jotta Numeronkin k‰ytˆss‰
// Muutettu 150300/LW metakieli ja xml output eps:n lis‰ksi
// Muutettu 031100/LW +MapColor() ja oma v‰ri nyt envirnmentiss‰ (geneerinen)
// Muutettu 300501/LW +itsRelCharWidth
//---------------------------------------------------------------------------

#ifndef __NTEXPARR_H__
#define __NTEXPARR_H__

#include "NFmiPressTypes.h"  //8.9
#include "NFmiParamMapping.h" //181199
/*
typedef struct
{
	double cyan;
	double magenta;
	double yellow;
	double black;

}FmiCMYK; //LW 21.11 PS-color
*/
typedef enum
{
//	 dColor	= 80 //3.11.00 pois
	 dAlignment = 80
	,dFont
	,dStyle
	,dColorValueDependent
	,dParenthesis        
	,dParamSize
	,dAddTextAfter      //300699
	,dAddTextInFront      //300699
	,dTextMapping       //181199
	,dRGBMapping       //2.11.00
	,dCMYKMapping    =1080     //2.11.00
	,dRelCharWidth             //30.5.01
}NFmiTextParamRectObjects;

typedef struct          //2.11.00
{
	FmiGenericColor color;
	double			minValue;
	double			maxValue;
}FmiPressColorMapping;

const unsigned long maxNumOfColMaps=20; //2.11.00

#include "NFmiParamRect.h"
//#include "NFmiPressEnvironment.h"

class _FMI_DLL NFmiTextParamRect : public NFmiParamRect
{
	public:
		NFmiTextParamRect(void)
		  :NFmiParamRect()
		  ,fUseSelectLatinFont(false) //121198
		  ,itsFont(NFmiString("Courier"))
		  // 5.3 ,itsFormat(NFmiString("%1.f"))
		  ,itsAlignment(kCenter) //16.10
		  ,fParenthesis(isFalse)
		  ,itsMapping(0)     //181199
		  ,itsCurrentNumOfColMaps(0)
		  ,itsRelCharWidth(0.)  //300501
		  {};

/*		                         {itsColor.cyan =0.f;				 //3.11.00 pois
										  itsColor.magenta =0.f; 
										  itsColor.yellow =0.f;
										  itsColor.black =1.f;
										 }
										 ; */
										 
		NFmiTextParamRect(const NFmiTextParamRect& theTextParamRect); 

/*	//LW
		NFmiSymbolParamRect(  NFmiDataIdent theParam
							, NFmiRect theRelativeSize
							, NFmiString theDirectory = 0 	 
							): NFmiParamRect(theParam, theRelativeSize)
 							, itsDefToProductScale(NFmiRect(0.,0.,1.,1.)
									                ,NFmiRect(0.,0.,140.,140.)) //sovittu koko
*/		
		virtual ~NFmiTextParamRect();

		virtual bool		ReadDescription(NFmiString& retString)=0; //5.3
//		virtual NFmiParamRect*	Clone(void) const; 
      int ConvertDefText(NFmiString & object);
		virtual bool ReadRemaining(void);  //300699

		virtual bool	WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
									,NFmiFastQueryInfo* theQI  
									,std::ofstream& theDestinationFile
									,FmiPressOutputMode theOutput) = 0;  //13.3.00
		void SetValueDepending(float theValue);
//		void SetColor(float theC, float theM, float theY, float theK); //3.11.00 pois
		void SetTextAttributes(NFmiString theFont      //5.3
			                  ,FmiDirection theAlignment
							  ,NFmiString theStyle
							  ,FmiCMYK theColor
//							  ,bool theColorValueDepending = isFalse
								)
		                {itsFont = theFont;
		                 itsAlignment = theAlignment;
                         itsStyle = theStyle;
//                         itsColor = theColor;
						};
       
		NFmiString Construct(NFmiString *theString) const; //300699 
		NFmiString GetFont(void)const {return itsFont;};
		FmiDirection GetAlignment(void)const {return itsAlignment;};
		NFmiString GetStyle(void) const{return itsStyle;};
//		FmiCMYK GetColor(void)const {return itsColor;};
//		bool GetColorValueDepending(void)const {return fColorValueDependent;};
		virtual bool WriteCode(const NFmiString& theText   //12.10
			                 ,const NFmiRect &AbsoluteRectOfSymbolGroup
			                 ,std::ofstream& theDestinationFile 
							 ,const NFmiString& theObjectName
							 ,FmiPressOutputMode theOutput);    //15.3.00 
//		bool IsPureBlack(void);
    protected:
		virtual FmiGenericColor MapColor(void) const; //3.11.00 

	private:
		bool WriteShowString( double x         //161198
					    ,double y
					    ,const NFmiString& theWidthString
					    ,const NFmiString& theShowString
						,std::ofstream & os) const;

	protected:                //10.2

		NFmiString itsAddInFront;   //300699
		NFmiString itsAddAfter;     //300699
		bool fUseSelectLatinFont;  //121198		
		NFmiString itsFont;
		FmiDirection itsAlignment;
		NFmiString itsStyle;
//		FmiCMYK itsColor;	//3.11.00 pois k‰ytet‰‰n environmentin geneerist‰		           
//		bool fColorValueDependent; //2.11.00 pois, colorMapping hoitaa
		bool fParenthesis;         //20.8
		NFmiParamMapping* itsMapping;   //181199
		FmiPressColorMapping itsColorMapping[maxNumOfColMaps]; //2.11.00
		unsigned long itsCurrentNumOfColMaps; //2.11.00
		double itsRelCharWidth; //30.5.01     
};

#endif //__NTEXPARR_H__
