// ======================================================================
/*!
 * \file
 * \brief Definition of various essentian enumerations
 */
// ======================================================================

#ifndef NFMIPRESSTYPES_H
#define NFMIPRESSTYPES_H

// RGB kopiotu t‰nne fcoltyp.h:sta koska ei haluta
// sekoittaa nViewt‰ pressiin mitenk‰‰n

typedef double FmiColorValue;

//! Undocumented
struct FmiRGBColor
{
  FmiColorValue red;
  FmiColorValue green;
  FmiColorValue blue;
  FmiColorValue alpha;
};


//! Undocumented
struct FmiCMYK
{
  double cyan;
  double magenta;
  double yellow;
  double black;
};


//! Undocumented
enum FmiPageSize
  {
	kLetter = 1,
	kA4,
	kA4Maisema,
	kA3,
	kA3Maisema
};

//! Undocumented
union FmiUnionColor
{
  FmiRGBColor rgb;
  FmiCMYK cmyk;  
};


//! Undocumented
struct FmiGenericColor
{
  bool isRgb;   
  FmiUnionColor color;
};
 
#endif // NFMIPRESSTYPES_H

// ======================================================================
