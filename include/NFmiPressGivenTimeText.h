// © Ilmatieteenlaitos/Lasse.
// Original 2.4.1998
// 
//********************************************************
// 
// Luokalle annetaan aika jonka voi kirjoittaa ps-oliona output-
// tiedostoon. Emo on abstrakti NFmiPressTimeText ja sisaruksia
// NFmiPressDataTimeText ja NFmiPressComputerTimeText.  . 
//
//********************************************************
//
//Muutettu 121099/LW +ReadRemaining() jossa Päivä ja Tunti

//---------------------------------------------------------------------------

#ifndef __NPTIGTEX_H__
#define __NPTIGTEX_H__

#include "NFmiPressTimeText.h"

class _FMI_DLL NFmiPressGivenTimeText : public NFmiPressTimeText
{
	public:
		NFmiPressGivenTimeText(void) :  NFmiPressTimeText()
											{;};

		virtual ~NFmiPressGivenTimeText();

		virtual bool ReadRemaining(void);  
		virtual bool	WritePS(FmiPressOutputMode theOutput); 
		virtual bool SetText();	   								
		virtual void SetText(NFmiString theText)   
							{NFmiPressTimeText::SetText(theText);};
		void SetTime (const NFmiMetTime& theTime) {itsFirstPlotTime=theTime;};//11.4.02 +const koska Mika emossakin
       
	protected:
};

#endif //__NPTIGTEX_H__
