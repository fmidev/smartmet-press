//© Ilmatieteenlaitos  Lasse
//  Original 13.2. 1998
//Muutettu cccccc/LW 

//----------------------------------------------------------------------------

//#include <fstream.h>

#ifndef __NPRESAME_H__
#include "NFmiPressSameSymbols.h"
#endif//__NPRESAME_H__

#include <iostream>  //STL 27.8.01
using namespace std; //27.8.01


//----------------------------------------------------------------------------
/*
void NFmiPressSameSymbols::SetStationScale(const NFmiRectScale& scale)
{
	itsCurrentStationScale = scale;
}
*/
//----------------------------------------------------------------------------
void NFmiPressSameSymbols::SetScale(const NFmiRectScale& scale)
{
	itsScale = scale;
}
//----------------------------------------------------------------------------
bool NFmiPressSameSymbols::ReadDescription(NFmiString& retString) //16.1
{
	// bool hasOwnData = isFalse; 
	bool helpBool;
    NFmiString dataFileName;
	NFmiString string;
	NFmiValueString valueString;
	NFmiPoint* place;
	char object[255];// ch;
	long long1;    
	unsigned short numOfTablePlaces, currentPlaceNum=0;
	NFmiString string1;
	NFmiPoint point1;
	double tableX=0, tableY1=0, tableY2=0, x, y;
	double xmin,xmax,ymin,ymax;

	*itsDescriptionFile >> object;
	string = object;
	int iobject = ConvertDefText(string);

	numOfTablePlaces = 0;
	itsNumberOfSteps = 1; 
	itsStepSize = NFmiPoint(20.,0.); 
//	NFmiStationPoint newStation;

	while(iobject != dEnd || itsCommentLevel) 
	{
	
	if(itsLoopNum > itsMaxLoopNum)  
	{
		if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #SymboliPaikat-oliossa" << endl;
	    retString = string;
		return isFalse;
	}
    itsLoopNum++;
	if (iobject != dEndComment && itsCommentLevel) iobject = dComment;
	switch(iobject)
	{
		case dOther:	  
		{
			if(itsLogFile)
				*itsLogFile << "*** ERROR: Tuntematon sana #ParametriAsematissa: " << (char*)object << endl;  
			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dComment:	  
		{
			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dEndComment:	  
		{
			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dPlaceSubViews:	  		
		{
			if (!ReadEqualChar())
				break;
			if(ReadLong(long1))  //15.1
               itsNumberOfSteps = (unsigned short)long1;

			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dPlaceStepSize:	  		 
		{
			if (!ReadEqualChar())
				break;
  			if(Read2Double(x,y))    
			{
               point1.Set(x,y);
	
               itsStepSize = NFmiPoint(point1.X()*itsScale.GetScaling().X()
				                   ,point1.Y()*itsScale.GetScaling().Y());       
            }
			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dPlaceTable:	  		 
		{   
			if (!ReadEqualChar())
				break;
			if(Read2Double(xmin,ymax))    
			{
 				if(ReadDouble(ymin))
				{
					if(ReadLong(long1))
					{
						tableX = xmin;
						tableY1 = ymax;
						tableY2 = ymin;
						numOfTablePlaces = (unsigned short)long1;
					}
				}
			}
            currentPlaceNum = 0;

			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dPlaceDefArea:   
		{
			if (!ReadEqualChar())
				break;
			if(Read4Double(xmin,ymin,xmax,ymax))
			{
				itsCurrentScale.SetStartScales(NFmiRect(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax)));
			}
			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dPlacePlotArea:   //29.12
		{
			if (!ReadEqualChar())
				break;
			if(Read4Double(xmin,ymin,xmax,ymax))   //15.1
			{
			   itsCurrentScale.SetEndScales(NFmiRect(NFmiPoint(xmin,ymin), NFmiPoint(xmax,ymax)));
            }
			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dPlace:     
		{
            currentPlaceNum++;
			if (!ReadEqualChar())
				break;
			helpBool = isTrue;
			if(Read2Double(x,y))
			{
			   if (numOfTablePlaces > 1)
			   {
				   x = tableX;
				   y = (tableY2 - tableY1) / (numOfTablePlaces-1) * (currentPlaceNum-1)
					                    + tableY1;
			    }
			    point1 = itsCurrentScale.Scale(NFmiPoint(x,y));  
  				place = new NFmiPoint(point1);
			    itsPlaces.Add(place);
			}

			*itsDescriptionFile >> object;
			string = object;
			iobject = ConvertDefText(string);
			break;
		}
		case dPlaceSymbol:     
		{
            itsPsSymbol.SetLogFile(itsLogFile);   
			itsPsSymbol.SetHome(GetHome());  //29.9
			itsPsSymbol.SetDescriptionFile(itsDescriptionFile); //20.11.01

			// bool ok = itsPsSymbol.ReadDescription(string);
			itsPsSymbol.ReadDescription(string);

			iobject = ConvertDefText(string);
			break;
		}		
	} 
	
	}
	retString = string;
	return isTrue;	
}
//---------------------------------------------------------------------------------------
int NFmiPressSameSymbols:: ConvertDefText(NFmiString & object)
{
	if(object==NFmiString("Place")     || object==NFmiString("Paikka"))
		return dPlace;
	else if(object==NFmiString("#ConstantSymbol") || object==NFmiString("#VakioSymboli")
		|| object==NFmiString("#Vakiosymboli") || object==NFmiString("#Kuva"))  //11.3
		return dPlaceSymbol;
	else if(object==NFmiString("PlaceDefArea") || object==NFmiString("PaikkojenMittaAlue")
		|| object==NFmiString("Paikkojenmittaalue")) 
		return dPlaceDefArea;
	else if(object==NFmiString("PlacePlotArea")|| object==NFmiString("PaikkojenSijoitusAlue")
		|| object==NFmiString("Paikkojensijoitusalue")) 
		return dPlacePlotArea;
	else if(object==NFmiString("SubViews")  || object==NFmiString("SiirtoLukum‰‰r‰")
		|| object==NFmiString("Siirtolukum‰‰r‰"))  
		return dPlaceSubViews;
	else if(object==NFmiString("StepSize")  || object==NFmiString("Siirrot"))
		return dPlaceStepSize;
	else if(object==NFmiString("PlaceTable")   || object==NFmiString("PaikkaTaulukko")
		|| object==NFmiString("Paikkataulukko"))
		return dPlaceTable;
	else
		return NFmiDescription::ConvertDefText(object);
}

//----------------------------------------------------------------------------
bool NFmiPressSameSymbols::WritePS(ofstream& theOutFile)
{
    itsPsSymbol.Set( itsScale, theOutFile);
	NFmiVoidPtrIterator placeIter = NFmiVoidPtrIterator(itsPlaces);
	void* item;
    NFmiPoint place;
	NFmiPoint placeMovement;

 	FmiCounter itsCurrentStep = 1;
	placeMovement.Set(0., 0.); 
	FmiCounter placeAll = 0;
	while(itsCurrentStep <= itsNumberOfSteps)  
	{   
 	   {
	      placeIter.Reset();
          item = placeIter.Next(); 
	      while(item)
           {	
			  placeAll++;
              place = *(NFmiPoint*)item;
		      {
			     if(!(itsPsSymbol.MakeAndWritePS(place)))
			     {
			     }
		      }
//pit‰iskˆ lis‰t‰  item->WriteGRestore(); //040199
            item = placeIter.Next(); 
	       }
	    }

	    placeMovement += itsStepSize;
	    itsCurrentStep++;
	}
	if(itsLogFile)
         *itsLogFile << "VakioPaikat: " << (char*)itsPsSymbol.GetSymbol() 
		 << ": " << placeAll << " paikkaa k‰sitelty" << endl;

	for(int i=0; i< itsNumberOfSteps; i++)  //11.12
	{
	   itsCurrentScale.MoveXEndScale(-itsStepSize.X());
	}
	return isTrue;
}


