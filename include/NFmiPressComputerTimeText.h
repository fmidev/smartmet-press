// © Ilmatieteenlaitos/Lasse.
// Original 9.4.1998

//********************************************************
// 
// Luokka tuottaa koneen ajan ps-oliona output-
// tiedostoon. Toistaiseksi k‰ytet‰‰n Suomen aikaa. 
// Emo on abstrakti NFmiPressTimeText ja sisaruksia ovat
// NFmiPressDataTimeText ja NFmiPressComputerTimeText.  . 
//
//********************************************************
//
// T‰m‰ tulostaa koneen aktueelin ajan (Suomen aikaa)
// 
//Muutettu xxxxxx/LW 
//Muutettu 180699/LW +itsDeltaDays, ReadReamaining(),ConvertDefText()  
//Muutettu 131099/LW SetText()

//---------------------------------------------------------------------------

#ifndef __NPTICTEX_H__
#define __NPTICTEX_H__


#include "NFmiPressTimeText.h"

class _FMI_DLL NFmiPressComputerTimeText : public NFmiPressTimeText
{
	public:
		NFmiPressComputerTimeText(void) : NFmiPressTimeText()
			                             ,itsDeltaDays(+0)  //18.6.99
		                          {};

		virtual ~NFmiPressComputerTimeText();
		virtual bool ReadRemaining(void); //18.6.99  
		virtual bool SetText(void);           //131099									
		virtual void SetText(NFmiString theText)   //131099
							{NFmiPressTimeText::SetText(theText);};
		int ConvertDefText(NFmiString & object); //18.6.99

		virtual bool	WritePS(FmiPressOutputMode theOutput); //15.3.00
       
	protected:
		short itsDeltaDays; //18.6.99
};

#endif //__NPTICTEX_H__
