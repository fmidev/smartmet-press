// © Ilmatieteenlaitos/Lasse.			File: NFmiCmykColorBag.cpp
// Original 11.6.1998
// 
// Muutettu xxxxxx
//---------------------------------------------------------------------------

#include "NFmiCmykColorBag.h"

//---------------------------------------------------------------------------
NFmiCmykColorBag::NFmiCmykColorBag(const NFmiCmykColorBag& theBag)
: NFmiSize(theBag.GetSize())
{
	itsColors = new NFmiNamedCmykColor[itsSize];
	for(int i=0; i< (int)itsSize; i++)
	{
		itsColors[i].color = theBag.itsColors[i].color;
		itsColors[i].name = theBag.itsColors[i].name;		
	}

}
//---------------------------------------------------------------------------
NFmiCmykColorBag::~NFmiCmykColorBag() 
{
	if(itsColors)
	    delete [] (NFmiNamedCmykColor*)itsColors;
};

//---------------------------------------------------------------------------

void NFmiCmykColorBag::AddColor(const NFmiNamedCmykColor & theColor)
{
	//Huom: ei tarkista alkioiden järjestystä tai onko samoja alkioita jo ennestään.
	//(lisää vain tyhmästi listan loppuun).
		NFmiNamedCmykColor *tempColors;
		tempColors = new NFmiNamedCmykColor[GetSize() + 1];

		int j;
		for(j=0; j<(int)itsSize; j++)
		{
			tempColors[j].color = itsColors[j].color;
			tempColors[j].name = itsColors[j].name;
		}
		
		tempColors[j].color = theColor.color;
		tempColors[j].name = theColor.name;

		if(itsColors)
			delete [] (NFmiNamedCmykColor*)itsColors;

		itsColors = new NFmiNamedCmykColor[GetSize()+1];
		itsSize = GetSize()+1;
		for(j=0; j< (int)GetSize(); j++)
		{
			itsColors[j].color = tempColors[j].color;
			itsColors[j].name = tempColors[j].name;
		}
		delete [] (NFmiNamedCmykColor*)tempColors;

};

//---------------------------------------------------------------------------
NFmiCmykColor* NFmiCmykColorBag::GetColor(const NFmiString& theName) const
{   
	int j;
	for(j=0; j<(int)itsSize; j++)
	{
		if(theName == itsColors[j].name)
				break;
	}
	if(j == (int)itsSize)
		return 0;
	else
		return &itsColors[j].color;
};
//---------------------------------------------------------------------------
NFmiString NFmiCmykColorBag::GetColorString(const NFmiString& theName) const
{   
	int j;
	for(j=0; j<(int)itsSize; j++)
	{
		if(theName == itsColors[j].name)
				break;
	}
	NFmiString format = NFmiString("%.2f");  //HUOM hardkoodattu
	NFmiString string;
	if(j == (int)itsSize)
		string = NFmiString("0. 0. 0. 0.");
	else
	{
		string = NFmiValueString((float)itsColors[j].color.c, format);
		string += NFmiString(" ");
		string += NFmiValueString((float)itsColors[j].color.m, format);
		string += NFmiString(" ");
		string += NFmiValueString((float)itsColors[j].color.y, format);
		string += NFmiString(" ");
		string += NFmiValueString((float)itsColors[j].color.k, format);
		string += NFmiString(" "); //Huom viimeiseksikin välilyönti
	}
	return string;
};

