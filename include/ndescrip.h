//© Ilmatieteenlaitos/Lasse.
//Original 12.12.1997
//****************************************************************
// Luokka hallitsee määrittelytiedoston 
// lukemisen. Tiedosto on tyyppiä lehtituotemäärittely. Jos jokin
// muu sovellus käyttää samanlaista logikkaa voi tätä käyttää.
// Täällä on funktiot esim kokonaisluvun tai kahden
// liukuluvun lukemiseen (virheilmoituksineen jos havaitaan ei-
// sallittuja merkkejä). Kaikille luokille yhteiset (kommentit)
// määrittelyt on sijoitettu tänne. Myös lokitiedosto 
// (johon lähinnä määrittelytiedosto generoi ilmoituksia) hallitaan
// tästä. Tästä on edelleen peritty NFmiPressDescription ja 
// NFmiPressTimeDescription-luokat. 
// Luokat, jotka lukevat lehtituotteiden määrittelytiedostoa 
// tulee periytyä näistä.
//****************************************************************
//
//Muutettu 221297/LW +itsCommentLevel
//Muutettu 150198/LW +itsValueHelpString, +ReadFloat(), +ReadInt() 
//Muutettu 160197/LW ReadDescription():iin palautusStringi
//Muutettu 020298/LW +itsLoopNum,+itsMaxLoopNum jotta ei ikuiseen luuppiin virhemäärittelyissä   
//Muutettu 020298/LW +kDescCounter    
//Muutettu 020298/LW +itsLogFile
//Muutettu 020298/LW ReadShort(unsigned short) -> ReadLong(long)    
//Muutettu 050398/LW +copyk.
//Muutettu 050398/LW +ReadEqualChar(), +ReadNext()    
//Muutettu 290998/LW itsHelpString -> itsString tarvitaan return-koodeissa    
//Muutettu 171198/LW +Set1(Unsigned)Long(),ReadUnsignedLong()    
//Muutettu 290199/LW +ReadString() -> välilyönti merkkijonoihin    
//Muutettu 050299/LW Read(type) ja Set(Type) -funktioita korvattu template-versioilla
//                   vastaavat tyypitetyt voidi hävittää (jollei Javaan tarvita)    
//Muutettu 050299/LW +ReadMaxTwoDoubleToDelimiter()
//Muutettu 100200/LW +SetFive()
//Muutettu 040900/LW +ClassId()
//---------------------------------------------------------------------------

#ifndef __NDESCRIP_H__
#define __NDESCRIP_H__

#include <stdlib.h> //STL 29.8.01   
//#include <iostream>  //STL 27.8.01  
//#include <fstream>  //STL 27.8.01  

//static long kDescCounter;  

//#include "NFmiSaveBase.h"
#include "NFmiValueString.h"

// Mika: Muutin unsignedeiksi
const unsigned int kNFmiNone			             = 10; //nämä 4.9.00
const unsigned int kNFmiPressStationText	         = 11;
const unsigned int kNFmiPressParamName			 = 12;
const unsigned int kNFmiPressDataTimeText		 = 13;
const unsigned int kNFmiPressImage				 = 14;  //2.10.00

typedef enum				 
{
	dComment      =8880	//ei pienempiä tänne	  
	,dEndComment   
	,dOther        =8888
	,dEnd          =9999
	,dFile                 //25.5

}NFmiDescriptionObjects;

class _FMI_DLL NFmiDescription 
{
	public:
		NFmiDescription(void) {itsCommentLevel=0;
		                       itsLoopNum = 0;
		                       itsMaxLoopNum = 4000;
		                       itsLogFile = 0;
								itsDescriptionFile = 0;};
        //5.3
		NFmiDescription(std::ofstream* theLogFile, 
		                unsigned short theMaxLoopNum)
		                { itsMaxLoopNum = theMaxLoopNum ;
		                  itsLogFile = theLogFile;
						  itsDescriptionFile = 0;
		                  itsLoopNum = 0;
						  itsCommentLevel=0;
						  };

		NFmiDescription(const NFmiDescription& theD)   //5.3
		                      {itsLogFile = theD.itsLogFile;
							  itsDescriptionFile = theD.itsDescriptionFile;
			                   itsCommentLevel=theD.itsCommentLevel;
		                       itsLoopNum = theD.itsLoopNum;
		                       itsMaxLoopNum = theD.itsMaxLoopNum;
		};

		//template-funtioista joutuu laittamaan myös koodin tänne h-fileen, muuten linkkeri ei tykkää

		template <class Type>
	    FmiBoolean ReadOne (Type &retValue)
		{
			*itsDescriptionFile >> itsObject;
			itsValueHelpString = itsObject;
			if(itsValueHelpString.IsNumeric())
			{
				retValue = (Type)(double)itsValueHelpString; //110299 valueStringillä tulisi olla cast longiin ja uns.longiin
				return isTrue;
			}
			retValue = 0;
			*itsLogFile << "ERROR: pitää olla luku: " << (char*)itsValueHelpString << endl;
			return isFalse;
		};

		template <class Type1, class Type2>
		FmiBoolean ReadTwo (Type1 &retValue1,Type2 &retValue2)
		{
			if((ReadOne(retValue1)) && ReadOne(retValue2))
			{
				return kTrue;
			}
			return kFalse;
		};

		template <class Type>
		FmiBoolean SetOne (Type& theMember)
		{
			FmiBoolean ok = kFalse;
			Type num;
			if (ReadEqualChar())
			{
				if(ReadOne(num))
				{
				   theMember = num;
				   ok = kTrue;
				}
			    ReadNext(); //epäonnistuneessa ReadEqualChar:ssa on jo
			}
			return ok;
		};

		template <class Type1, class Type2>
		FmiBoolean SetTwo (Type1& theMember1, Type2& theMember2)
		{
			FmiBoolean ok = kFalse;
			if (ReadEqualChar())
			{
			    Type1 num1;
				Type2 num2;
				if(ReadTwo(num1, num2)) //ei suoraan parametriin koska saatetaan lukea roskaa
					             // ja parametrissä on ennestään järkevä oletusarvo
				{
				   theMember1 = num1;
				   theMember2 = num2;
				   ok = kTrue;
				}
				ReadNext();
			}
			return ok;
		};
		template <class Type1, class Type2, class Type3>
		FmiBoolean SetThree (Type1& theMember1, Type2& theMember2
		                   ,Type3& theMember3)
		{
			FmiBoolean ok = kFalse;
			if (ReadEqualChar())
			{
			    Type1 num1;
				Type2 num2;
			    Type3 num3;
				if(ReadTwo(num1, num2) && ReadOne(num3)) 					             
				{
				   theMember1 = num1;
				   theMember2 = num2;
				   theMember3 = num3;
				   ok = kTrue;
				}
				ReadNext();
			}
			return ok;
		};
		template <class Type1, class Type2, class Type3, class Type4>
		FmiBoolean SetFour (Type1& theMember1, Type2& theMember2
		                   ,Type3& theMember3, Type4& theMember4)
		{
			FmiBoolean ok = kFalse;
			if (ReadEqualChar())
			{
			    Type1 num1;
				Type2 num2;
			    Type3 num3;
				Type4 num4;
				if(ReadTwo(num1, num2) && ReadTwo(num3, num4)) 					             
				{
				   theMember1 = num1;
				   theMember2 = num2;
				   theMember3 = num3;
				   theMember4 = num4;
				   ok = kTrue;
				}
				ReadNext();
			}
			return ok;
		};
		template <class Type1, class Type2, class Type3, class Type4, class Type5> //10.2.00
		FmiBoolean SetFive (Type1& theMember1, Type2& theMember2
		                   ,Type3& theMember3, Type4& theMember4, Type5& theMember5)
		{
			FmiBoolean ok = kFalse;
			if (ReadEqualChar())
			{
			    Type1 num1;
				Type2 num2;
			    Type3 num3;
				Type4 num4;
				Type5 num5;
				if(ReadTwo(num1, num2) && ReadTwo(num3, num4) && ReadOne(num5)) 					             
				{
				   theMember1 = num1;
				   theMember2 = num2;
				   theMember3 = num3;
				   theMember4 = num4;
				   theMember5 = num5;
				   ok = kTrue;
				}
				ReadNext();
			}
			return ok;
		};
		template <class Type1, class Type2, class Type3, class Type4, class Type5, class Type6> //3.11.00
		FmiBoolean SetSix (Type1& theMember1, Type2& theMember2
		                   ,Type3& theMember3, Type4& theMember4, Type5& theMember5, Type6& theMember6)
		{
			FmiBoolean ok = kFalse;
			if (ReadEqualChar())
			{
			    Type1 num1;
				Type2 num2;
			    Type3 num3;
				Type4 num4;
				Type5 num5;
				Type6 num6;
				if(ReadTwo(num1, num2) && ReadTwo(num3, num4) && ReadTwo(num5, num6)) 					             
				{
				   theMember1 = num1;
				   theMember2 = num2;
				   theMember3 = num3;
				   theMember4 = num4;
				   theMember5 = num5;
				   theMember6 = num6;
				   ok = kTrue;
				}
				ReadNext();
			}
			return ok;
		};
	
		virtual ~NFmiDescription(void){};

		void SetLogFile (std::ofstream* logFile)   
									{itsLogFile = logFile;};
		void SetDescriptionFile (std::ifstream* descFile)   
									{itsDescriptionFile = descFile;}; //31.8.01

		virtual FmiBoolean		ReadDescription(NFmiString& retString)=0; 
		FmiBoolean ReadMaxTwoFloatToDelimiter (float &retValue1, float &retValue2); 
		FmiBoolean ReadDouble (double &retValue);
		FmiBoolean Read4Double (double &retValue1,double &retValue2
			                   ,double &retValue3,double &retValue4);
		FmiBoolean Read2Double (double &retValue1,double &retValue2);
		NFmiString ReadString (void); 
		FmiBoolean ReadLong (long &retValue, FmiBoolean errorReport=kTrue);
		FmiBoolean ReadUnsignedLong (unsigned long &retValue); 
		FmiBoolean ReadEqualChar (void); 
		void       ReadNext (void); 
		FmiBoolean Set1Double (double& theMember); 
		FmiBoolean Set1Long (long& theMember); 
		FmiBoolean Set1UnsignedLong (unsigned long& theMember);//171198

        unsigned short GetMaxLoopNum(void) const {return itsMaxLoopNum;};
        std::ofstream* GetLogFile (void) const {return itsLogFile;};
		
//		virtual NFmiParamRect*	Clone(void) const = 0; 
	    virtual unsigned long ClassId(void){return kNFmiNone;}; //4.9.00 

        int virtual ConvertDefText(NFmiString & object);
	protected:
        std::ofstream* itsLogFile;       
        std::ifstream* itsDescriptionFile;  //31.8.01       
		unsigned short itsLoopNum; 
		unsigned short itsMaxLoopNum; 
        char itsObject[255];
		int itsIntObject;    
		NFmiValueString itsValueHelpString;
		NFmiString itsString;      
		unsigned short itsCommentLevel; //Vain olion sisällä tietysti
	private: 

};


#endif // __NDESCRIP_H__
