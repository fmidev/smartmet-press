// © Ilmatieteenlaitos/Lasse.
// Original 3.7.1998
// 
//********************************************************
// 
// Valmiiden EPS-kuvien littäminen ps-tiedostoon. Peritty
// NFmiScaling:stä mutta jouduttu lisäämään skaala-käsittelyä
// ja alueen leikkausta (clip).
//
//********************************************************
//
//Muutettu 080998/LW +dImageDir
//Muutettu 180299/LW +dImageSizeFactor, +dImagePlaceMove
//Muutettu 021000/LW +GetPath() 
//********************************************************

#ifndef __NPRIMAGE_H__
#define __NPRIMAGE_H__

typedef enum
{
	 dImageFile	= 260
	,dImagePath
	,dImageDir
	,dImageDefSize
	,dImagePlottingView
    ,dImageClippingRectangle
	,dImageSizeFactor        //180299
	,dImagePlaceMove         //180299
}NFmiPressImageObjects;

//---------------------------------------------------------------------------

#include "nprscale.h"
#include "NFmiFileString.h"

class _FMI_DLL NFmiPressImage : public NFmiPressScaling
{
	public:
		NFmiPressImage(void) :  NFmiPressScaling() {};
		NFmiPressImage(const NFmiPressImage& theOtherImage); 

		virtual ~NFmiPressImage();

		virtual FmiBoolean		ReadDescription(NFmiString& retString); 
//		virtual NFmiParamRect*	Clone(void) const; 
        int ConvertDefText(NFmiString & object);

		virtual FmiBoolean	WritePS(FmiPressOutputMode theOutput); //15.3.00 
        void ScalePlotting(void);
		void SetPath (const NFmiString& thePath) {itsPath=thePath;};
		NFmiFileString GetPath (void) const {return itsPath;}; //2.10.00
	    virtual unsigned long ClassId(void){return kNFmiPressImage;}; //2.10.00 
		
		
    protected:

	protected:               
			 		
		NFmiFileString itsPath;  //1.9 ->files.
		NFmiRectScale itsImageScale;
		NFmiRect itsClippingRect;
};

#endif //__NPRIMAGE_H__
