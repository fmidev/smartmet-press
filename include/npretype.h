// © Ilmatieteenlaitos/Lasse.
// Original 8.9.1998
//
//********************************************************

//********************************************************
// 
//Muutettu 271098/LW  +FmiPageSize
//Muutettu 181000/LW  +FmiRGBColor   
//Muutettu 021100/LW  +FmiGenericColor
//---------------------------------------------------------------------------

#ifndef __NPRETYPE_H__
#define __NPRETYPE_H__
/*
#ifndef __NBASESTR_H__
#include "NFmiString.h"
#endif//__NBASESTR_H__

static NFmiString kLehtiTuoteDir;  //25.9
*/
/*  RGB kopiotu t‰nne fcoltyp.h:sta koska ei haluta sekoittaa nViewt‰
pressiin mitenk‰‰n */
	typedef double FmiColorValue;
	typedef struct {FmiColorValue  red, green, blue, alpha;} FmiRGBColor;


typedef struct
{
	double cyan;
	double magenta;
	double yellow;
	double black;

}FmiCMYK;

typedef enum		                                             
	{kLetter       =1                                                  
	,kA4     
	,kA4Maisema     
	,kA3     
	,kA3Maisema     
} FmiPageSize;

union FmiUnionColor     //3.11.2000 //vain toinen v‰rij‰rjestelm‰ kulloinkin valittu
{
	FmiRGBColor rgb;
	FmiCMYK cmyk;  
};
typedef struct      //3.11.2000
{
	bool isRgb;   
	FmiUnionColor color;
}FmiGenericColor;
 
#endif //__NPRETYPE_H__
