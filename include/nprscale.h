//© Ilmatieteenlaitos/Lasse.
//Original 6.3.1998
//****************************************************************
// PS-tuotteiden skaalaukset hallitaan tästä luokasta
// Koska tämä on peritty luokista NFmiDescription ja NFmiPsWriting
// on tämä perusluokka kaikille ps-olioille ja hallitsee 
// - ps-koodin kirjoittamisen
// - määrittelytiedoston lukemisen
// - skaalaukset
// Käytössä on kuitenkin rinnakkainen vanhempi luokkahierarkia, jossa
// ovat esim datariippuvien sääsymbolien ja numeroiden teko  
//****************************************************************
//
// Erotettu omaksi olioksi NFmiPsSymbolista
// 
//Muutettu 180398/LW +Place(void)
//Muutettu 060498/LW +fOnePrint
//Muutettu 260898/LW +SetSize(point) tarvitaan DataAlkiot:n sisäisille olioille 
//Muutettu 090998/LW +fPrintOnce
//Muutettu 250998/LW perintä NFmiDescription -> NFmiPressDescription
//Muutettu 111298/LW +SetScale()
//Muutettu 111298/LW +fInParagraph
//Muutettu 181298/LW +GetLineStep()
//Muutettu 121099/LW perintä NFmiPressTimeDescriptionista eika NFmiPressDescriptionista
//Muutettu 150300/LW metakieli ja xml output eps:n lisäksi
//Muutettu 160800/LW + tyhjä WritePSUpdatingSubText()
//Muutettu 240101/LW +dMakeAsLast
//Muutettu 250101/LW +CreatePath()
//---------------------------------------------------------------------------

#ifndef __NPRSCALE_H__
#define __NPRSCALE_H__

#ifndef __NPRDESCR_H__
#include "nprtides.h"
#endif//__NPRDESCR_H__

#ifndef __NPSWRITE_H__
#include "nPsWrite.h"
#endif//__NPSWRITE_H__

#ifndef __NRECTSCA_H__
#include "nrectsca.h"
#endif//__NRECTSCA_H__

#include "NFmiFileString.h"

class NFmiQueryData;

typedef enum
{
	 dSymbolSize
	,dRelSymbolSize
	,dPsPlaceMove   //6.10
	,dSymbolPlace
	,dPrintOnce     //6.4
	,dRotate        //261198
	,dRotateFirst   //150399
	,dMakeAsLast    //240101
	,dFileTimestamp   //190802
}NFmiPressScalingObjects;
                                      //121099
class _FMI_DLL NFmiPressScaling : public NFmiPressTimeDescription ,public NFmiPsWriting
{
	public:
		NFmiPressScaling(void): NFmiPressTimeDescription()       //121099
							,itsRelArea(NFmiPoint(0.,0.)
							           ,NFmiPoint(1.,1.))
			                ,itsRectSize(40.,40.)
							,itsPlace(NFmiPoint(0.,0.))
							,fPrintOnce(kFalse)
							,fScaleNotPlace(kFalse)
							,itsTimestampDayGap(kShortMissing)
//							,fInParagraph(kFalse) 
								 {}; 
		
		NFmiPressScaling(const NFmiPressScaling& thePsSymbol);
		
		virtual ~NFmiPressScaling();
        virtual int             ConvertDefText(NFmiString& object);
		virtual FmiBoolean		ReadDescription(NFmiString& retString)=0;
        virtual FmiBoolean      ReadRemaining(void);        
		virtual FmiBoolean      WritePS(const NFmiPoint& thePoint, FmiPressOutputMode theOutput);//15.3.00
		virtual FmiBoolean      WritePS(FmiPressOutputMode theOutput)=0;  //15.3.00
	    virtual FmiBoolean		WritePSUpdatingSubText(FmiPressOutputMode theOutput){return kTrue;}; //16.8.00
				FmiBoolean      SetSize (const NFmiPoint &theSize) 
								{itsRectSize = theSize; return isTrue;}; 
                FmiBoolean      Set( NFmiRectScale &theRectScale
							        ,std::ofstream& theDestinationFile);
                FmiBoolean      SetScale(const NFmiRectScale &theRectScale);
		        void            Place(const NFmiPoint& thePlace){itsPlace = thePlace;};
		        void            Move(const NFmiPoint& addPlace)
				                          {itsPlace += addPlace;};
		        NFmiPoint       Place(void){return itsPlace;};     
				void            SetPrintOnceOn(void){fPrintOnce=kTrue;};
				void            ScaleNotPlace(void){fScaleNotPlace=kTrue;};
				FmiBoolean      GetPrintOnce(void){return fPrintOnce;};
	   virtual	FmiBoolean      IsInParagraph(void){return kFalse;};
//				void            SetInParagraph(void){fInParagraph = kTru
	   virtual double GetLineStep(void) const {return 0.;}; 
	   virtual  void SetLastLineStep(double step) {;}; 
	   virtual void SetLanguage(FmiLanguage theLanguage) {;}; //24.3.00
//	   virtual bool IsDataObject(void) {return false;};  //4.9.00 
				

    protected:           
        virtual void 	ScalePlotting(void);
		NFmiFileString  CreatePath(NFmiString defaultDir, NFmiString givenPath
			                   , NFmiString givenDir, NFmiString givenFile
							   , NFmiString extension = NFmiString()); //26.1.01
		int GetTimestampDayGap(void)const {return itsTimestampDayGap;}; 
		bool AddTimeStamp(NFmiString& theGivenFile, const NFmiString& theTimeFormat=NFmiString("DDMM")) const; //16.8.02 
	private:

	protected:
//		FmiBoolean          fInParagraph; //141298 palstaan edellisen jälkeen 
        NFmiRectScale       itsRectScale; //koko kartta, tulee vasta writePs;ssä
		NFmiRect            itsRelArea;   
	    NFmiPoint           itsRectSize;  
	    NFmiPoint           itsPlace;  
		FmiBoolean          fPrintOnce;   //6.4 pakotettu kerran-vain-tulostus
		FmiBoolean          fScaleNotPlace;   //9.9 DataAlkoi-jäsenet tarvii jotta ei kahteen kertaan siirrettäisi
		int					itsTimestampDayGap; //19.8.02
		};


#endif //__NPRSCALE_H__

