//© Ilmatieteenlaitos/Salla.
//Original 21.10.1997
//Muutettu 220299/LW ReadRemaining() kutsuu ‰idin vastaavaa eik‰ iso‰idin
// 
//****************************************************************
// T‰m‰ luokka hallitsee dataohjattujen symbolien tuottamisen
// m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
// Ensimm‰isi‰ luokkia, jonka jˆlkeen on syntynyt rinnakkaisia
// rakenteita (ps-tiedoston kirjoitus, Illu-symbolit->"lyhyt-
// symboleiksi").
// T‰st‰ on edelleen peritty k‰‰ntyv‰ symboli (NFmiRotatingParamRect).
//****************************************************************
//  
//Changed	10-11.97	/LW
//Changed	06.11.97	/LW		ReadDescription() emolle koska yhteinen TextParamRectin kanssa 
//Changed	18.11.97	/Salla	Clone()	
//Changed	19.11.97	/Salla	Copy const.
//Muutettu  20.11.97 /LW 		ConvertDefText() j‰seneksi
//     - " -                  itsSubDir ei en‰‰ tuoda ylemp‰‰ vaan luetaan suoraan ymp.muuttujasta
//Muutettu 211197/LW  NFmiSymbolParamRectObjects t‰nne typistettyn‰
//Muutettu 160197/LW ReadDescription():iin palautusStringi   
// Muutettu 100298/LW +itsOrigDir, +itsSymbolSetName, +CopyShortSymbol2Dest()
// Muutettu 100298/LW +itsCurrentParamValue
// Muutettu 100298/LW +ReadRemaining()
// Muutettu 231198/LW +DoPostReading()
// Muutettu 080199/LW +ReadValues(), itsSizeFactor
// Muutettu 230899/LW +kirjoituspuolelle (WritePS jne) QD korvatti QI:ll‰
//Muutettu 150300/LW metakieli ja xml output eps:n lis‰ksi
//Muutettu 240500/LW +fIsConstSymbol, t‰ll‰ muutetaan t‰m‰ olio #VakioSymboliksi
// Muutettu 171000/LW +itsYValueScaling ja X
// Muutettu 071100/LW skaalaus siirretty NFmiScalingParamRectiin
// Muutettu 030502/LW ReadValues() toka par

//---------------------------------------------------------------------------

#ifndef __NSYMPARR_H__
#define __NSYMPARR_H__

#include "NFmiParamRect.h"
#include "NFmiParamMapping.h"

/*
typedef struct       //17.10.00   //7.11.00 -> ScalingParamRectiin
{
	double		noneValue;  //t‰ll‰ j‰‰ pois, normaalisti nolla
	double		symbolValue;//talletettua symbolia vastaava arvo
	double	    maxValue;   //max-arvo, jonka mukaan skaalataan
}FmiValueScaling;
*/
typedef enum
{
	 dSymbolDirectory = 100
	,dMapping
	,dScaleMapping
	,dSymbolDefSize
	,dRelSize          
	,dConstSymbolName        //24.5.00 #VakioSymboleille
//	,dHeightScale            //16.10.00
//	,dWidthScale            //17.10.00
}NFmiSymbolParamRectObjects;

class _FMI_DLL NFmiSymbolParamRect : public NFmiParamRect
{
	public:
		NFmiSymbolParamRect(void)
		: NFmiParamRect()
		,fIsConstSymbol(false)  //24.5.00
		, itsMapping(0)
		, itsSubDir(0)	
		, itsOrigDir(0)	//10.2
		// , itsCurrentParamValue(kFloatMissing)
		, itsDefToProductScale(NFmiRect(0.,0.,140.,140.)  //sovittu koko
				       ,NFmiRect(0.,0.,1.,1.))
		  {}
//								 {itsXValueScaling.symbolValue=kFloatMissing;
//								  itsYValueScaling.symbolValue=kFloatMissing;}; 
/*
		NFmiSymbolParamRect ( NFmiDataIdent theParam
							, NFmiRect theRelativeSize)
							: NFmiParamRect(theParam, theRelativeSize)
 							, itsDefToProductScale(NFmiRect(0.,0.,140.,140.)  //sovittu koko
							, NFmiRect(0.,0.,1.,1.))
							, itsCurrentParamValue(kFloatMissing)
		                    {}; 
*/		
		NFmiSymbolParamRect(const NFmiSymbolParamRect& theSymbolRect);
		
		virtual ~NFmiSymbolParamRect();
        virtual int             ConvertDefText(NFmiString & object);
		virtual bool		ReadDescription(NFmiString& retString); 
		virtual bool      ReadRemaining(void) /*{return NFmiParamRect::ReadRemaining();}*/; 
		virtual NFmiParamRect*	Clone(void) const; 

		virtual bool	WritePS( const NFmiRect & AbsoluteRectOfSymbolGroup
									,NFmiFastQueryInfo* theQI
									,std::ofstream& theDestinationFile
									,FmiPressOutputMode theOutput); //15.3.00
		void SetConstSymbol(bool boo){fIsConstSymbol=boo;}; //24.5.00

    protected:
//		virtual void AddSymbolMetaOptions(void){;};		           //8.11.00

		virtual void DoPostReading(void){;}; //231198   
		virtual bool CopyShortSymbol2Dest(NFmiString* symbolFile, std::ofstream& theDestinationFile); 
        virtual bool ConvertOrig2Short(NFmiString* symbolFile);
        virtual bool ReadValues( NFmiFastQueryInfo* theQI, bool SetRelHour=true); //3.5.02 toka par  
		virtual void       ScaleByValue(){;};                  //7.11.00
//		bool ReadSymbolScale(FmiValueScaling& theScale); //201000
		virtual void WriteMetaCode(NFmiString* symbolFile, NFmiPoint realPos, std::ofstream & os); //14.11.00

	protected:
		NFmiString          itsConstSymbol;   //24.5.00 #VakioSymbolia varten
		bool		    fIsConstSymbol;   //24.5.00 #VakioSymbolia varten
		NFmiPoint           itsSizeFactor; //080199 RotParr tarttee
		NFmiParamMapping*	itsMapping;
		NFmiString*			itsSubDir;	
		NFmiString*			itsOrigDir;
		NFmiString          itsSymbolSetName;
		NFmiRectScale		itsDefToProductScale;
//		FmiValueScaling     itsXValueScaling;  //17.10.00
//		FmiValueScaling     itsYValueScaling;  //17.10.00
/*		double				itsHeightScale;  //16.10.00
		double				itsHeightScaleBase;  //16.10.00
		*/
};


#endif //__NSYMPARR_H__

/*
class NFmiDirectedSymbolParamRect : NFmiSymbolParamRect
{

	private:
		xxxx	itsDirection;
};

*/
