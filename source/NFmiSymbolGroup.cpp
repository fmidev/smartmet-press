

//© Ilmatieteenlaitos/Salla.
// Original 21.10.1997
// 
// Changed 18.11.1997	Add(NFmiParamRect&)
//						NFmiSymbolGroupObjects + ReadDescription() uusiksi
//Muutettu 201197/LW +Case Size
//Muutettu -"-       SymbolDir pois, NFmiRotatingParamRect mukaan
//Muutettu 150198/LW ReadDouble() ym k‰yttˆ
//Muutettu 040298/LW +NumOfMissing()
//Muutettu 240298/LW +itsPsSymbol
//Muutettu 270298/LW +dLetter
//Muutettu 060398/LW NumParamRect korvaa textParamRectin
//Muutettu 260898/LW Vakiosymbolille v‰litet‰‰n ryhm‰n koko
//Muutettu 080998/LW constSymbol taulukoksi ja +constText
//Muutettu 110998/LW skaalaus uusiksi(vanhan tavan lis‰ksi):
//                   ei k‰ytet‰ kokotiedostoa, koko annetaan suhteellisena
//                   symbolit tehd‰‰n origokeskeisesti
//Muutettu 051098/LW +sizeGiven, ohjaa vanhaa/uutta skaalausta
//Muutettu 130899/LW pressparam-v‰litet‰‰n t‰nne ja edelleen piirto-olioille
//Muutettu 160200/LW datan aika asetetaan ennen jokaista piirtoalkiota
//Muutettu 290200/LW producer-kutsut poistettu
//Muutettu 290200/LW +auringon lasku/nousu
//Muutettu 231000/LW paramRectit joka kerta uudelleen konstruktorilla
//Muutettu 071100/LW +NFmiScalingParamRect
//Muutettu 040101/LW +‰‰riarvoaika
//---------------------------------------------------------------------------
#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif
 
#include "NFmiSymbolGroup.h"
#include <iostream> //STD 27.8.01
#include "NFmiSymbolParamRect.h"
#include "NFmiNumberParamRect.h"  //6.3
#include "NFmiRotatingParamRect.h"
#include "NFmiLetterParamRect.h"
#include "NFmiTimeParamRect.h"
#include "NFmiPressText.h"
#include "NFmiSunTimeParamRect.h"
#include "NFmiScalingParamRect.h"  //7.11.00
#include "NFmiSuperSmartInfo.h"

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01

#ifndef __NPSYMBOL_H__
#include "NFmiPsSymbol.h"
#endif//__NPSYMBOL_H__



typedef enum
{
	 dGroupSize			 =120
	,dSymbol			 
	,dConstSymbol			 
	,dConstText     //8.9
	,dText         //=Numero
	,dRotating
	,dLetter
	,dTime
    ,dExtremeTime  //4.1.01
	,dSunTime     //29.2.2000
	,dScaling         = 1120//7.11.00

}NFmiSymbolGroupObjects;

//---------------------------------------------------------------------------
NFmiSymbolGroup::NFmiSymbolGroup(NFmiPressParam* pressParam  //13.8.99
								 ,NFmiParamRect** theItemList, int numOfItems) 
  : NFmiSize(numOfItems)
  , itsPressParam(pressParam)
  , itsPsSymbol(0)  
  , itsRectSize(NFmiPoint(40,40)) 
  , itsRectScale(0)
  , itsQueryData(0)
  , itsQueryDataIter(0)
{
	itsParamRects = new NFmiParamRect*[numOfItems];
	for(int i=0; i < (int)GetSize(); i++)
		itsParamRects[i] = theItemList[i];

};

//---------------------------------------------------------------------------
bool NFmiSymbolGroup::Set(NFmiRectScale &theRectScale
							 , NFmiFastQueryInfo* theQueryDataIter 
							 , ofstream& theDestinationFile)
{
	if(itsRectScale) delete (NFmiRectScale*)itsRectScale;
	itsRectScale = new NFmiRectScale(theRectScale);	
	itsQueryDataIter = theQueryDataIter; 
	itsOutFile = &theDestinationFile;
	return true;
};
//---------------------------------------------------------------------------
NFmiSymbolGroup::~NFmiSymbolGroup()
{	if(itsParamRects) delete [] (NFmiParamRect**)itsParamRects;
	if(itsRectScale) delete (NFmiRectScale*)itsRectScale;
	if(itsPsSymbol) delete itsPsSymbol;
	itsPressScalingObjects.Clear(isTrue);      
};
//22.3.00----------------------------------------------------------------------------
bool NFmiSymbolGroup::SetAllTimes(const NFmiMetTime& theTime)   
{
	SetTime(theTime); //varmaan turha

	for(int i=0; i < (int)GetSize(); i++)
	{
		itsParamRects[i]->SetTime(theTime); 
		itsParamRects[i]->UpdateModifierTimes(); //3.6.02	
	}

	return true;
}
//24.3.00----------------------------------------------------------------------------
bool NFmiSymbolGroup::SetAllLanguages(FmiLanguage theLanguage)   
{
	itsLanguage = theLanguage;

	for(int i=0; i < (int)GetSize(); i++)
	{
		itsParamRects[i]->SetLanguage(theLanguage); 
	}

	return true;
}
//---------------------------------------------------------------------------
bool NFmiSymbolGroup::Add(const NFmiParamRect & theParamRect)
{

		NFmiParamRect** tempRects;
		tempRects = new NFmiParamRect*[itsSize + 1];
		int j;
		for(j=0; j<(int)itsSize; j++)
			tempRects[j] = itsParamRects[j];
		tempRects[j] = theParamRect.Clone(); 

		if(itsParamRects)
			delete [] (NFmiParamRect**)itsParamRects;

		itsParamRects = new NFmiParamRect*[itsSize+1];
		itsSize = itsSize+1;
		for(j=0; j< (int)itsSize; j++)
			itsParamRects[j] = tempRects[j];

		delete [] (NFmiParamRect**)tempRects;
		return true;//doesn't compare if already in the list ->true is returned as a default. 
	
};
//---------------------------------------------------------------------------
long NFmiSymbolGroup::NumOfMissing(void) const //4.2
{
	long num = 0;
	for(int i=0; i < (int)GetSize(); i++)
	{
		num += itsParamRects[i]->NumOfMissing();
	}

	return num;
}

//---------------------------------------------------------------------------
void NFmiSymbolGroup::InitMissing(void)  //27.6.00
{
	for(int i=0; i < (int)GetSize(); i++)
	{
		itsParamRects[i]->InitMissing();
	}
}

//---------------------------------------------------------------------------
bool  NFmiSymbolGroup::ReadDescription(NFmiString& retString) //16.1	
{//29.9 itsObject jne
//	bool isComment = isFalse;
//	NFmiString string;
	NFmiValueString valueString;
//	char object[255], ch;
//	int iobject=0;
	double x, y;

/*** 23.10.99 kaikki siirretty case:n sis‰pulelle koska t‰‰ll‰ saman segmentin
     olioille j‰‰ edellisen samanlaisen olion vanhat muuttujat
	 nyt konstruoidaan uudelleen ***/	
//	NFmiSymbolParamRect tempSPar;    
//	NFmiSymbolParamRect tempCSPar;    
//	NFmiNumberParamRect tempNPar;   
//	NFmiTimeParamRect tempTPar;   
//	NFmiSunTimeParamRect tempSTPar;   
//	NFmiRotatingParamRect tempRPar;
//	NFmiLetterParamRect tempLPar;
	NFmiPressScaling* psObject;       
	bool sizeGiven = false;  

	ReadNext();
	while(itsIntObject != 9999 || itsCommentLevel) //2.2)
	{
	   if(itsLoopNum > itsMaxLoopNum)  //2.2
	   {
		   if(itsLogFile)
			   *itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #DataAlkiot-elementiss‰" << endl;
	       retString = itsString;
		   return isFalse;
	   }
		itsLoopNum ++;
		if (itsIntObject != dEndComment && itsCommentLevel) itsIntObject = dComment;

		switch(itsIntObject)
		{
			case dOther:	  //ylim‰‰r‰ist‰ roinaa, End.. lopettaa
			{
				if(itsLogFile)
					*itsLogFile << "*** ERROR: Tuntematon sana #DataAlkiot-elementiss‰: " << (char*)itsObject << endl;  
			    ReadNext();
			/*	*itsDescriptionFile >> object;
				itsString = object;
				iobject = ConvertDefText(itsString);
				*/
				break;
			}
			case dComment:	  
			{
//				isComment = isTrue;
				ReadNext();
				break;
			}
			case dEndComment:	  
			{
//				isComment = isFalse;
				ReadNext();
				break;
			}
			case dGroupSize:	  		 //LW 20.11
			{
				if (!ReadEqualChar())
				   break;

			    if(ReadDouble(x))      //25.2 
				{				
				   *itsDescriptionFile >> itsObject;
			       valueString = itsObject;
				   if(valueString.IsNumeric())  
				   {
					  y = (double)valueString;
					  *itsDescriptionFile >> itsObject;
					  itsString = itsObject;
				    }
				    else
				    {
					   y = x;
					   itsString = valueString;
				    }

					itsRectSize.Set(x,y);
				}
				else
				{
					  *itsDescriptionFile >> itsObject;
					  itsString = itsObject;
				}

				  
//				*itsDescriptionFile >> object;
//				itsString = object;
				itsIntObject = ConvertDefText(itsString);
				sizeGiven = true;   //5.10
				break;
			}
			case dSymbol:
			{
				NFmiSymbolParamRect tempSPar; //23.10.00   
                tempSPar.SetPressParam(itsPressParam); 
                tempSPar.SetHome(GetHome()); 
                tempSPar.SetLogFile(itsLogFile);   
                tempSPar.SetDescriptionFile(itsDescriptionFile);   
			    tempSPar.SetTime(itsFirstPlotTime); 
				tempSPar.SetHourLoop(IsHourLoop());   
				tempSPar.SetNewScaling(!sizeGiven);
				if(tempSPar.ReadDescription(itsString))
					Add(tempSPar);

				itsIntObject = ConvertDefText(itsString);
				break;
			}
			case dConstSymbol: //24.5.00 #VakioSymboli tekee nyt SymbolParamRect
				               // jossa ohitetaan data ja mappaus
							   //22.6.00 tempCSPar t‰ss‰ eik‰ tempSPar
			{
 				NFmiSymbolParamRect tempCSPar; //23.10.00   
                tempCSPar.SetConstSymbol(true);  //HUOM
                tempCSPar.SetPressParam(itsPressParam); 
                tempCSPar.SetHome(GetHome()); 
                tempCSPar.SetLogFile(itsLogFile);   
                tempCSPar.SetDescriptionFile(itsDescriptionFile);   
			    tempCSPar.SetTime(itsFirstPlotTime); 
				tempCSPar.SetHourLoop(IsHourLoop());   
				tempCSPar.SetNewScaling(!sizeGiven);
				if(tempCSPar.ReadDescription(itsString))
					Add(tempCSPar);

				itsIntObject = ConvertDefText(itsString);
				break;
			}
			case dConstText:
			{
				psObject = new NFmiPressText;
                psObject->SetHome(GetHome()); 
				psObject->SetLogFile(itsLogFile);
                psObject->SetDescriptionFile(itsDescriptionFile);   
				psObject->SetSize(itsRectSize);
				if(psObject->ReadDescription(itsString))
					 itsPressScalingObjects.Add(psObject);

				itsIntObject = ConvertDefText(itsString);
				break;
			}
			case dText: //oik. number
			{
				NFmiNumberParamRect tempNPar; //23.10.00   
                tempNPar.SetPressParam(itsPressParam); //13.8.99
                tempNPar.SetEnvironment(itsEnvironment); 
                tempNPar.SetHome(GetHome()); 
                tempNPar.SetLogFile(itsLogFile);   
                tempNPar.SetDescriptionFile(itsDescriptionFile);   
			    tempNPar.SetTime(itsFirstPlotTime); 
				tempNPar.SetHourLoop(IsHourLoop());   
				tempNPar.SetNewScaling(!sizeGiven);
				if(tempNPar.ReadDescription(itsString))
					Add(tempNPar);
				itsIntObject = ConvertDefText(itsString);
				break;
			}		
			case dTime:
			{
				NFmiTimeParamRect tempTPar; //23.10.00   
                tempTPar.SetPressParam(itsPressParam); //13.8.99
                tempTPar.SetEnvironment(itsEnvironment); 
                tempTPar.SetHome(GetHome()); 
				tempTPar.SetLanguage(itsLanguage);  //23.3.00
                tempTPar.SetLogFile(itsLogFile);  
			    tempTPar.SetTime(itsFirstPlotTime); 
                tempTPar.SetDescriptionFile(itsDescriptionFile);   
				tempTPar.SetHourLoop(IsHourLoop());   
				tempTPar.SetNewScaling(!sizeGiven);
				if(tempTPar.ReadDescription(itsString))
					Add(tempTPar);
				itsIntObject = ConvertDefText(itsString);
				break;
			}		
			case dExtremeTime:  //4.1.01
			{
				NFmiExtremeTimeParamRect tempETPar;    
                tempETPar.SetPressParam(itsPressParam); 
                tempETPar.SetEnvironment(itsEnvironment); 
                tempETPar.SetHome(GetHome()); 
				tempETPar.SetLanguage(itsLanguage);  
                tempETPar.SetLogFile(itsLogFile);  
                tempETPar.SetDescriptionFile(itsDescriptionFile);   
			    tempETPar.SetTime(itsFirstPlotTime); 
				tempETPar.SetHourLoop(IsHourLoop());   
				tempETPar.SetNewScaling(!sizeGiven);
				if(tempETPar.ReadDescription(itsString))
					Add(tempETPar);
				itsIntObject = ConvertDefText(itsString);
				break;
			}		
			case dSunTime:   //29.2.2000
			{
				NFmiSunTimeParamRect tempSTPar; //23.10.00   
                tempSTPar.SetPressParam(itsPressParam); 
                tempSTPar.SetEnvironment(itsEnvironment); 
                tempSTPar.SetHome(GetHome()); 
 				tempSTPar.SetLanguage(itsLanguage);  //23.3.00
                tempSTPar.SetLogFile(itsLogFile);  
                tempSTPar.SetDescriptionFile(itsDescriptionFile);   
			    tempSTPar.SetTime(itsFirstPlotTime); 
				tempSTPar.SetHourLoop(IsHourLoop());   
				tempSTPar.SetNewScaling(!sizeGiven);
				if(tempSTPar.ReadDescription(itsString))
					Add(tempSTPar);
				itsIntObject = ConvertDefText(itsString);
				break;
			}		
			case dLetter:                           
			{
				NFmiLetterParamRect tempLPar; //23.10.00   
                tempLPar.SetPressParam(itsPressParam); //13.8.99
                tempLPar.SetEnvironment(itsEnvironment); 
                tempLPar.SetHome(GetHome()); 
                tempLPar.SetLogFile(itsLogFile);   
                tempLPar.SetDescriptionFile(itsDescriptionFile);   
			    tempLPar.SetTime(itsFirstPlotTime); 
				tempLPar.SetHourLoop(IsHourLoop());   
				tempLPar.SetNewScaling(!sizeGiven);
				if(tempLPar.ReadDescription(itsString))
					Add(tempLPar);

				itsIntObject = ConvertDefText(itsString);
				break;
			}		
			case dRotating:								
			{
				NFmiRotatingParamRect tempRPar; //23.10.00   
                tempRPar.SetPressParam(itsPressParam); //13.8.99
                tempRPar.SetEnvironment(itsEnvironment); 
                tempRPar.SetHome(GetHome()); 
                tempRPar.SetLogFile(itsLogFile);   
                tempRPar.SetDescriptionFile(itsDescriptionFile);   
			    tempRPar.SetTime(itsFirstPlotTime); 
				tempRPar.SetHourLoop(IsHourLoop());   
				tempRPar.SetNewScaling(!sizeGiven);
			    if(tempRPar.ReadDescription(itsString))
					Add(tempRPar);

				itsIntObject = ConvertDefText(itsString);
				break;
			}		
			case dScaling:	//7.11.00							
			{
				NFmiScalingParamRect tempScPar;    
                tempScPar.SetPressParam(itsPressParam); 
                tempScPar.SetEnvironment(itsEnvironment); 
                tempScPar.SetHome(GetHome()); 
                tempScPar.SetLogFile(itsLogFile);   
                tempScPar.SetDescriptionFile(itsDescriptionFile);   
			    tempScPar.SetTime(itsFirstPlotTime); 
				tempScPar.SetHourLoop(IsHourLoop());   
				tempScPar.SetNewScaling(!sizeGiven);
			    if(tempScPar.ReadDescription(itsString))
					Add(tempScPar);

				itsIntObject = ConvertDefText(itsString);
				break;
			}		
			default:  
			{
				ReadRemaining();  
				break;
			}
		}
	
	}//end of while
	SetScalingMode();
    retString = itsString;
	return true;
};
//---------------------------------------------------------------------------
void NFmiSymbolGroup::SetScalingMode(void)
{
     //pit‰isi kai tarkistaa myˆs muut kuin datariippuvat
	fNewScaling = false;
	for(int i=0; i < (int)GetSize(); i++)
	{	
		if(itsParamRects[i]->IsNewScaling())
			fNewScaling = true;
	}
}
//---------------------------------------------------------------------------
int NFmiSymbolGroup::ConvertDefText(NFmiString & object)
{
	NFmiString lowChar = object;
	lowChar.LowerCase();

	if(lowChar==NFmiString("size") ||             lowChar==NFmiString("koko"))
		return dGroupSize;
	else if(lowChar==NFmiString("#symbol") ||     lowChar==NFmiString("#symboli")
		|| lowChar==NFmiString("#kuva"))                                     
		return dSymbol;
	else if(lowChar==NFmiString("#constantsymbol")||lowChar==NFmiString("#vakiosymboli")
		 || lowChar==NFmiString("#vakiokuva"))
		return dConstSymbol;
	else if(lowChar==NFmiString("#constanttext") ||lowChar==NFmiString("#vakioteksti"))
		return dConstText;
	else if(lowChar==NFmiString("#text")   
	     || lowChar==NFmiString("#number") || lowChar==NFmiString("#numero"))  
		return dText;
	else if(lowChar==NFmiString("#letter") || lowChar==NFmiString("#teksti")  
		|| lowChar==NFmiString("#kirjain"))    
		return dLetter;
	else if(lowChar==NFmiString("#time") ||   lowChar==NFmiString("#aikateksti")
		                       ||   lowChar==NFmiString("#aika") )     
		return dTime;
	else if(lowChar==NFmiString("#extremetime") || lowChar==NFmiString("#‰‰riarvoaika"))     
		return dExtremeTime;   //4.1.01
	else if(lowChar==NFmiString("#suntime") ||   lowChar==NFmiString("#aurinkoaika")
							   ||   lowChar==NFmiString("#aurinko"))     
		return dSunTime;    //29.2.2000
	else if(lowChar==NFmiString("#rotating") || lowChar==NFmiString("#k‰‰ntyv‰symboli") 
	    ||lowChar==NFmiString("#k‰‰ntyv‰kuva"))
		return dRotating;
	else if(lowChar==NFmiString("#scaledsymbol") || lowChar==NFmiString("#skaalautuvasymboli"))//7.11.00
		return dScaling;
	else
		return NFmiPressDescription::ConvertDefText(object);
};
//---------------------------------------------------------------------------
bool	NFmiSymbolGroup::WritePS(const  NFmiStationPoint &theStationPoint
									 ,FmiPressOutputMode theOutput) //15.3.00
{
	
	NFmiPoint sizeScaling = itsRectScale->GetScaling();
	double scaledXSize = itsRectSize.X() * sizeScaling.X();
	double scaledYSize = itsRectSize.Y() * sizeScaling.Y();

	NFmiRect absRectOfGroup;
/*	if(fNewScaling)    //11.9
		absRectOfGroup = NFmiRect(theStationPoint.X()  //left
								,theStationPoint.Y() + scaledYSize //top
								,theStationPoint.X() + scaledXSize //right
								,theStationPoint.Y() ); //bottom
	else */
		absRectOfGroup = NFmiRect(theStationPoint.X() -  scaledXSize/2 //left
								,theStationPoint.Y() + scaledYSize/2 //top
								,theStationPoint.X() + scaledXSize/2 //right
								,theStationPoint.Y() - scaledYSize/2); //bottom
 
	NFmiVoidPtrIterator objectIter = NFmiVoidPtrIterator(itsPressScalingObjects);
	NFmiPressScaling *object;

//	itsSymbols.Set(theScale, itsData, theFile);  //28.8
     
	 objectIter.Reset();
	 object = (NFmiPressScaling*)objectIter.Next();
	 while (object)
	 {
		NFmiRectScale rectScale = NFmiRectScale(*itsRectScale);
		NFmiRect rect = NFmiRect(NFmiPoint(0.,0.),itsRectSize);
        rectScale.SetStartScales(rect);
		rectScale.SetEndScales(absRectOfGroup);
		object->Set(*itsRectScale, *itsOutFile);

        object->ScaleNotPlace(); //9.9 koska jo kerran skaalat
		if (!(object->WritePS(theStationPoint.Point(),theOutput))) //15.3.00 
		{
		   if(itsLogFile)
			  *itsLogFile << "*** ERROR: (timeDep)object->WritePS() in NFmiSymbolGroup" << endl;
		   return false;
		}
		object = (NFmiPressScaling*)objectIter.Next();   
	 }

    NFmiMetTime currentSegmentTime = ((NFmiQueryInfo*)itsQueryDataIter)->Time(); //16.2.2000 

	for(int i=0; i < (int)GetSize(); i++)
	{
		itsQueryDataIter->Time(currentSegmentTime);//16.2.2000, piirtoalkiot saattavat muuttaa (ainakin tuntia)
		itsParamRects[i]->WritePS(absRectOfGroup
								 ,itsQueryDataIter  //23.8.99 oli itsQueryData
								 ,*itsOutFile
								 ,theOutput); //15.3.00
	}
	itsQueryDataIter->Time(currentSegmentTime);   //16.2.2000
	return true;
};				
//---------------------------------------------------------------------------
