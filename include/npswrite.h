//© Ilmatieteen laitos 12.2.1997/Lasse
//
//****************************************************************
// Luokka hoitaa PS-koodin kirjoittamisen tulostiedostoon
// ja siinä yhteydessä koodaa skaalauksen myös (scale+transform) 
// Tästä peritään luokat jotka tuottavat ps-koodia
//****************************************************************
//  
//Muutettu 091098/LW +fOverPrint
//Muutettu 261198/LW +itsRotatingAngle, itsRotatingPoint, Rotate()
//Muutettu 261198/LW +WriteGRestore()
//Muutettu 050199/LW +SetRotatingPoint()
//Muutettu 150399/LW +fRotBeforeScale
//Muutettu 291099/LW +CopyFileWithoutShowpage
//Muutettu 130300/LW +WriteOutString
//Muutettu 150300/LW +FmiPressOutputMode
//Muutettu 200600/LW +kPlainText
//Muutettu 031100/LW +WriteColor
//Muutettu 301100/LW +fActivity (voidaan managerista aktivoida/deaktivoida)
//Muutettu 011200/LW +copy-konstr.
//Muutettu 011200/LW +fWriteAsLast
//---------------------------------------------------------------------------
#ifndef __NPSWRITE_H__
#define __NPSWRITE_H__

#include "nrectsca.h"
#include "NFmiString.h"
#include "npretype.h"

typedef enum         //15.3.99                                     
{kPostScript       = 1                                                  
,kMetaLanguage     
,kXml
,kPlainText               //20.6.00
} FmiPressOutputMode;


class _FMI_DLL NFmiPsWriting 
{
	public:
		NFmiPsWriting(void)
		  :fActivity(true)
		  ,fWriteAsLast(false)
		  ,fRotBeforeScale(kFalse)
		  ,itsRotatingAngle(0.)
		  ,fOverPrint(kFalse)
		  ,itsInFile(0)    //25.3.02
		  ,itsOutFile(0)
		  ,itsWriteScale(NFmiRectScale(NFmiRect(NFmiPoint(0.,0.),NFmiPoint(1.,1.))
					       ,NFmiRect(NFmiPoint(0.,0.),NFmiPoint(1.,1.))))
		{};
		NFmiPsWriting(const NFmiPsWriting& thePsWriting); //1.12.00
		
		~NFmiPsWriting(void);

		FmiBoolean WriteOutString(const NFmiString& outString); //13.3.00
        FmiBoolean CopyFile(void);
		FmiBoolean CopyFileWithoutShowpage(void); //291099
		FmiBoolean WriteGRestore(void); //141298
		void       Rotate(void); //261198
        FmiBoolean WritePSConcat(void);
        FmiBoolean WritePSConcatRotating(float); 
        FmiBoolean WritePSEnd(void);
        FmiBoolean WriteEPSConcatClipping(NFmiRect theClippingRect); //toimiiko
        FmiBoolean WriteEPSConcat(void);       //tarvitaanko
        FmiBoolean WriteEPSEnd(void);
		void SetRotatingPoint(const NFmiPoint& thePoint) {itsRotatingPoint = thePoint;}; //050199
		void SetRotBeforeScale(const FmiBoolean rBeforeS) {fRotBeforeScale = rBeforeS;};  //150399
		NFmiString AlignmentToMeta (const FmiDirection& pressAlignment)const;  //24.10.00

		// Mika: operator= change 
		// void	SetFile(const std::ofstream& theDestinationFile){itsOutFile=theDestinationFile;}
		// void	SetFile(std::ofstream& theDestinationFile){itsOutFile=&theDestinationFile;}
		void	SetFile(std::ofstream& theDestinationFile){itsOutFile=&theDestinationFile;}
		void    WriteColor (const FmiGenericColor& theColor, FmiPressOutputMode mode, std::ofstream & os)const;  //3.11.00
		//void    WriteColor (const FmiGenericColor& theColor, FmiPressOutputMode mode)const; 
		bool    IsActive (void)const {return fActivity;}; //30.11.00
		void    SetActivity (bool theActivity) {fActivity=theActivity;}; //30.11.00
		bool    IsToWriteLast (void)const {return fWriteAsLast;}; //24.1.01
		void    SetWriteLast (bool theWriteLat) {fWriteAsLast=theWriteLat;}; //30.11.00

	protected:
		bool      fActivity;         //301100
		bool      fWriteAsLast;      //240101
		FmiBoolean      fRotBeforeScale;   //150399
 	    NFmiPoint       itsRotatingPoint;  //261198
		double          itsRotatingAngle;  //261198
		FmiBoolean      fOverPrint;        //9.10
		// Mika: operator= change 
		// std::ifstream itsInFile;
		// std::ofstream itsOutFile;	  
		std::ifstream * itsInFile; 
		std::ofstream * itsOutFile;	  
        NFmiRectScale   itsWriteScale;
};

#endif //__NPSWRITE_H__
