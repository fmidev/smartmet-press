// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiSymbolParamRect
 */
// ======================================================================
/*!
 * \class NFmiSymbolParamRect
 *
 * T‰m‰ luokka hallitsee dataohjattujen symbolien tuottamisen
 * m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
 * Ensimm‰isi‰ luokkia, jonka j‰lkeen on syntynyt rinnakkaisia
 * rakenteita (ps-tiedoston kirjoitus, Illu-symbolit->"lyhyt-
 * symboleiksi").
 *
 * T‰st‰ on edelleen peritty k‰‰ntyv‰ symboli (NFmiRotatingParamRect).
 *
 */
// ======================================================================

#ifndef NFMISYMBOLPARAMRECT_H
#define NFMISYMBOLPARAMRECT_H

#include "NFmiParamMapping.h"
#include "NFmiParamRect.h"

//! Undocumented
enum NFmiSymbolParamRectObjects
{
  dSymbolDirectory = 100,
  dMapping,
  dScaleMapping,
  dSymbolDefSize,
  dRelSize,
  dConstSymbolName,
  dIncludeNightSymbols
};

//! Undocumented
class NFmiSymbolParamRect : public NFmiParamRect
{
 public:
  ~NFmiSymbolParamRect() override;

  NFmiSymbolParamRect();
  NFmiSymbolParamRect(const NFmiSymbolParamRect& theSymbolRect);

  int ConvertDefText(NFmiString& object) override;
  bool ReadDescription(NFmiString& retString) override;
  bool ReadRemaining() override;
  NFmiParamRect* Clone() const override;

  bool WritePS(const NFmiRect& AbsoluteRectOfSymbolGroup,
               NFmiFastQueryInfo* theQI,
               std::ofstream& theDestinationFile,
               FmiPressOutputMode theOutput) override;

  void SetConstSymbol(bool boo);

 protected:
  virtual void DoPostReading();
  virtual bool CopyShortSymbol2Dest(NFmiString* symbolFile,
                                    std::ofstream& theDestinationFile,
                                    float theRotating = kFloatMissing);
  virtual bool ConvertOrig2Short(NFmiString* symbolFile);
  virtual bool ReadValues(NFmiFastQueryInfo* theQI, bool SetRelHour = true);
  virtual void ScaleByValue();
  virtual void WriteMetaCode(NFmiString* symbolFile, NFmiPoint realPos, std::ofstream& os);
  bool RawSymbolToBeConverted(NFmiString* symbolFil);
  void UseDayNightSymbols();

 protected:
  NFmiString itsConstSymbol;  // #VakioSymbolia varten
  bool fIsConstSymbol;        // #VakioSymbolia varten
  NFmiPoint itsSizeFactor;    // RotParr tarttee
  NFmiParamMapping* itsMapping;
  NFmiString* itsSubDir;
  NFmiString* itsOrigDir;
  NFmiString itsSymbolSetName;
  NFmiRectScale itsDefToProductScale;
  bool fUseDayNightSymbols;
};  // class NFmiSymbolParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiSymbolParamRect::NFmiSymbolParamRect()
    : NFmiParamRect(),
      fIsConstSymbol(false),
      itsMapping(nullptr),
      itsSubDir(nullptr),
      itsOrigDir(nullptr),
      itsDefToProductScale(NFmiRect(0., 0., 140., 140.),  // sovittu koko
                           NFmiRect(0., 0., 1., 1.)),
      fUseDayNightSymbols(false)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param boo Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiSymbolParamRect::UseDayNightSymbols()
{
  fUseDayNightSymbols = true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param boo Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiSymbolParamRect::SetConstSymbol(bool boo)
{
  fIsConstSymbol = boo;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiSymbolParamRect::DoPostReading() {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiSymbolParamRect::ScaleByValue() {}
#endif  // NFMISYMBOLPARAMRECT_H

// ======================================================================
