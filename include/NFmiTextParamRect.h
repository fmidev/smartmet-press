// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiTextParamRect
 */
// ======================================================================
/*!
 * \class NFmiTextParamRect
 *
 * T‰m‰ abstrakti luokka hallitsee dataohjattujen tekstien tuottamisen
 * m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
 *
 * Lapset NFmiNumberParamRect ja NFmiLetterParamRect.
 *
 */
// ======================================================================

#ifndef NFMITEXTPARAMRECT_H
#define NFMITEXTPARAMRECT_H

#include "NFmiParamMapping.h"
#include "NFmiParamRect.h"
#include "NFmiPressTypes.h"


//! Undocumented
enum NFmiTextParamRectObjects
{
  dAlignment = 80,
  dFont,
  dColorValueDependent,
  dParenthesis,
  dParamSize,
  dAddTextAfter,
  dAddTextInFront,
  dTextMapping,
  dRGBMapping, 
  dCMYKMapping,
  dRelCharWidth,
  dAddStationName = 1080,
  dPRLowerCase,
  dPRUpperCase,
  dPRFirstUpperCase,
  dPRMaxTextLength,
  dFillWithUnderscore
};


//! Undocumented
struct FmiPressColorMapping
{
  FmiGenericColor color;
  double minValue;
  double maxValue;
};

//! Undocumented
const unsigned long maxNumOfColMaps = 20;

class _FMI_DLL NFmiTextParamRect : public NFmiParamRect
{

public:
  
  virtual ~NFmiTextParamRect(void);

  NFmiTextParamRect(void);
  NFmiTextParamRect(const NFmiTextParamRect & theTextParamRect); 

  virtual bool ReadDescription(NFmiString & retString) = 0;
  int ConvertDefText(NFmiString & object);
  virtual bool ReadRemaining(void);

  virtual bool WritePS(const NFmiRect & AbsoluteRectOfSymbolGroup,
					   NFmiFastQueryInfo * theQI  ,
					   std::ofstream & theDestinationFile,
					   FmiPressOutputMode theOutput) = 0;

  void SetValueDepending(float theValue);

  NFmiString Construct(NFmiString * theString) const;

  virtual bool WriteCode(const NFmiString & theText,
						 const NFmiRect & AbsoluteRectOfSymbolGroup,
						 std::ofstream & theDestinationFile ,
						 const NFmiString & theObjectName,
						 FmiPressOutputMode theOutput);

protected:

  virtual FmiGenericColor MapColor(void) const;


private:

  bool WriteShowString(double x,
					   double y,
					   const NFmiString & theShowString,
					   std::ofstream & os) const;
  bool WriteShowStringLongMinus(double x,
					   double y,
					   const NFmiString & theMinusString,
					   const NFmiString & theWidthMinus,
					   const NFmiString & theShowString,
					   std::ofstream & os) const;
  bool WriteShowStringText(double x,
					   double y,
					   const NFmiString & theMinusString,
					   const NFmiString & theWidthMinus,
					   const NFmiString & theShowString,
					   std::ofstream & os) const;

protected:

  NFmiString itsAddInFront;
  NFmiString itsAddAfter;
  bool fUseSelectLatinFont;
  bool fParenthesis;
  NFmiParamMapping * itsMapping;
  FmiPressColorMapping itsColorMapping[maxNumOfColMaps];
  unsigned long itsCurrentNumOfColMaps;
  double itsRelCharWidth;
  double itsWidthFactor;
  bool fAddStationName;
  bool fUpperCase;  
  bool fLowerCase;   
  bool fFirstUpperCase;  
  unsigned long itsMaxLen;
  bool fFillWithUnderscore;
}; 


// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiTextParamRect::NFmiTextParamRect(void)
  : NFmiParamRect()
  , fUseSelectLatinFont(false)
  , fParenthesis(false)
  , itsMapping(0)
  , itsCurrentNumOfColMaps(0)
  , itsRelCharWidth(0.)
  , itsWidthFactor(1.)
  , fAddStationName(false)
  , fUpperCase(false)  
  , fLowerCase(false)   
  , fFirstUpperCase(false)
  , itsMaxLen(0)
  , fFillWithUnderscore(false)
{
}

#endif // NFMITEXTPARAMRECT_H

// ======================================================================
