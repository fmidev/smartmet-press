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

class NFmiTextParamRect : public NFmiParamRect
{
 public:
  ~NFmiTextParamRect() override;

  NFmiTextParamRect();
  NFmiTextParamRect(const NFmiTextParamRect& theTextParamRect);
  NFmiTextParamRect& operator=(const NFmiTextParamRect&) = default;

  bool ReadDescription(NFmiString& retString) override = 0;
  int ConvertDefText(NFmiString& object) override;
  bool ReadRemaining() override;

  bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
               NFmiFastQueryInfo* theQI,
               std::ofstream& theDestinationFile,
               FmiPressOutputMode theOutput) override = 0;

  void SetValueDepending(float theValue);

  NFmiString Construct(NFmiString* theString) const;

  virtual bool WriteCode(const NFmiString& theText,
                         const NFmiRect& AbsoluteRectOfSymbolGroup,
                         std::ofstream& theDestinationFile,
                         const NFmiString& theObjectName,
                         FmiPressOutputMode theOutput);

 protected:
  virtual FmiGenericColor MapColor() const;

 private:
  bool WriteShowString(double x,
                       double y,
                       const NFmiString& theShowString,
                       std::ofstream& os) const;
  bool WriteShowStringLongMinus(double x,
                                double y,
                                const NFmiString& theMinusString,
                                const NFmiString& theWidthMinus,
                                const NFmiString& theShowString,
                                std::ofstream& os) const;
  bool WriteShowStringText(double x,
                           double y,
                           const NFmiString& theMinusString,
                           const NFmiString& theWidthMinus,
                           const NFmiString& theShowString,
                           std::ofstream& os) const;

 protected:
  NFmiString itsAddInFront;
  NFmiString itsAddAfter;
  bool fUseSelectLatinFont = false;
  bool fParenthesis = false;
  NFmiParamMapping* itsMapping = nullptr;
  FmiPressColorMapping itsColorMapping[maxNumOfColMaps]{};
  unsigned long itsCurrentNumOfColMaps = 0;
  double itsRelCharWidth = 0.;
  double itsWidthFactor = 1.;
  bool fAddStationName = false;
  bool fUpperCase = false;
  bool fLowerCase = false;
  bool fFirstUpperCase = false;
  unsigned long itsMaxLen = 0;
  bool fFillWithUnderscore = false;
};

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiTextParamRect::NFmiTextParamRect() {}

#endif  // NFMITEXTPARAMRECT_H

// ======================================================================
