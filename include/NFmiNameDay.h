// © Ilmatieteenlaitos/Lasse.
// Original 25.1.2000
// 
//********************************************************
//
// Nimip‰iv‰ olio
// Tekee annetusta tiedostosta nimip‰iv‰taulukon. Tiedoston tulee olla muotoa:
//      ....
//		9.tammi	Veijo
//		9.tammi	Veikko
//		9.tammi	Veli
//		10.tammi	Nyyrikki
//		11.tammi	Kari
//		11.tammi	Karri
//      ....
// Oleellista on kaksi stringi‰ per rivi. Ekan tulee olla erilainen
// (sis‰llˆll‰ ei v‰li‰) jokaiselle uudelle p‰iv‰lle, ja tokassa tulee
// itse nimi. Samalle p‰iv‰lle voi antaa 1-n kappaletta
// nimi‰. Nimip‰iv‰ kysyt‰‰n oliolta MetTime:lla tuloksen ollessa
// muotoa "Kari, Karri". Tuloksen voi rajoittaa nimien lukum‰‰r‰n tai merkkim‰‰r‰n 
// mukaan. Karkausp‰iv‰lle ei voi antaa arvoa. 
//
// Soveltuu myˆs muihin samankaltaisiin teht‰viin.
//
// K‰ytet‰‰n aluksi lehtis‰‰ohjelmasta.
// 
//********************************************************
//
//Muutettu xxxxxx/LW 

//---------------------------------------------------------------------------

#ifndef __NFMINAMEDAY_H__
#define __NFMINAMEDAY_H__

#include "NFmiString.h"
#include "NFmiMetTime.h"

class _FMI_DLL NFmiNameDay 
{
	public:
		NFmiNameDay(void) {fRead=kFalse; fValue = kFalse;};

		virtual ~NFmiNameDay();

		FmiBoolean IsRead(void) {return fRead;};
		FmiBoolean IsValue(void) {return fValue;};
		FmiBoolean ReadFile(const NFmiString& theFileName);  
		NFmiString GetName(NFmiMetTime time, FmiCounter maxNumber=kShortMissing, FmiCounter maxLength=kShortMissing);
				
	private:

		NFmiString itsNames[366]; //0 ei k‰ytet‰
		FmiBoolean fRead;
		FmiBoolean fValue;
};

#endif //__NFMIPRESSNAMEDAY_H__
