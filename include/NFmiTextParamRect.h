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
  dStyle,
  dColorValueDependent,
  dParenthesis,
  dParamSize,
  dAddTextAfter,
  dAddTextInFront,
  dTextMapping,
  dRGBMapping,
  dCMYKMapping = 1080,
  dRelCharWidth
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

//#include "NFmiPressEnvironment.h"

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

  void SetTextAttributes(NFmiString theFont,
						 FmiDirection theAlignment,
						 NFmiString theStyle,
						 FmiCMYK theColor);
       
  NFmiString Construct(NFmiString * theString) const;
  NFmiString GetFont(void) const;
  FmiDirection GetAlignment(void) const;
  NFmiString GetStyle(void) const;
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
					   const NFmiString & theWidthString,
					   const NFmiString & theShowString,
					   std::ofstream & os) const;

protected:

  NFmiString itsAddInFront;
  NFmiString itsAddAfter;
  bool fUseSelectLatinFont;
  NFmiString itsFont;
  FmiDirection itsAlignment;
  NFmiString itsStyle;
  bool fParenthesis;
  NFmiParamMapping * itsMapping;
  FmiPressColorMapping itsColorMapping[maxNumOfColMaps];
  unsigned long itsCurrentNumOfColMaps;
  double itsRelCharWidth;

}; // class NFmiTextParamRect


// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiTextParamRect::NFmiTextParamRect(void)
  : NFmiParamRect()
  , fUseSelectLatinFont(false)
  , itsFont(NFmiString("Courier"))
  , itsAlignment(kCenter)
  , fParenthesis(isFalse)
  , itsMapping(0)
  , itsCurrentNumOfColMaps(0)
  , itsRelCharWidth(0.)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFont Undocumented
 * \param theAlignment Undocumented
 * \param theStyle Undocumented
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiTextParamRect::SetTextAttributes(NFmiString theFont,
										  FmiDirection theAlignment,
										  NFmiString theStyle,
										  FmiCMYK theColor)
{
  itsFont = theFont;
  itsAlignment = theAlignment;
  itsStyle = theStyle;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiTextParamRect::GetFont(void) const
{
  return itsFont;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
FmiDirection NFmiTextParamRect::GetAlignment(void) const
{
  return itsAlignment;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiTextParamRect::GetStyle(void) const
{
  return itsStyle;
}

#endif // NFMITEXTPARAMRECT_H

// ======================================================================
