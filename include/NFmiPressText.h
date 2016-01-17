// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressText
 */
// ======================================================================
/*!
 * \class NFmiPressText
 *
 * Hoitaa kaikenlaisten tekstien (aikamerkinnät, asemannimet
 * vakiotekstit, palstat) ps-tuottamisen samoinkuin määrittelyn
 * lukemisen. Kaikki tekstiin
 * liittyvä (fontti,väri,kohdistus, tyyli) samoin kuin itse
 * teksti on täällä. Myös elementin kieli, joka vaikuttaa
 * aikamerkintöihin, hoidetaan täältä.
 *
 */
// ======================================================================

#ifndef NFMIPRESSTEXT_H
#define NFMIPRESSTEXT_H

#include "NFmiPressScaling.h"
#include "NFmiPressTypes.h"
#include "NFmiHyphenationString.h"
#include "NFmiSettings.h"

//! Undocumented
enum NFmiPressTextObjects
{
  dTextAlignment = 140,
  dTextFont,
  dTextStyle,
  dTextString,
  dAddLocalTime,
  dCharSpace,
  dAddAfter,
  dAddInFront,
  dLowerCase,
  dUpperCase,
  dFirstUpperCase = 1140,
  dInParagraph,
  dParagraphMove,
  dTopMargin,
  dLeftMargin,
  dRightMargin,
  dParagraphBorders,
  dLineStep,
  dIndent,
  dLF,
  dLFFactor = 2140,
  dSubTextObject,
  dSubTimeTextObject,
  dSubComputerTimeTextObject,
  dTextLanguage,
  dTextDir,
  dTextPath,
  dMaxTextLength,
  dNotRightJustification,
  dTextBorders,
  dHeaderFont = 3140,
  dHeaderSize,
  dMainHeaderFont,
  dMainHeaderSize
};

//! Undocumented
class _FMI_DLL NFmiPressText : public NFmiPressScaling
{
 public:
  virtual ~NFmiPressText(void);
  NFmiPressText();
  NFmiPressText(const NFmiPressText& theTextParamRect);

  virtual bool ReadDescription(NFmiString& retString);
  virtual bool ReadRemaining(void);

  int ConvertDefText(NFmiString& object);
  NFmiString Construct(NFmiString* theString) const;

  using NFmiPressScaling::WritePS;
  virtual bool WritePS(FmiPressOutputMode theOutput);
  void SetValueDepending(float theValue);

  virtual bool SetText(void);
  virtual void SetText(NFmiString theText);
  virtual void AddText(const NFmiString& theText);
  virtual void SetPreText(const NFmiString& theText);
  virtual void SetPostText(const NFmiString& theText);
  bool WriteString(const NFmiString& commentString, FmiPressOutputMode theOutput);

  virtual bool WritePSUpdatingSubText(FmiPressOutputMode theOutput);

  NFmiString* GetText(void) const;
  void SetLanguage(FmiLanguage theLanguage);
  void SetEncoding(std::string theEncoding);
  FmiLanguage GetLanguage(void);
  virtual bool IsInParagraph(void);
  virtual void ScalePlotting(void);
  double GetLineStep(void) const;
  void SetLineStep(double lineStep);
  void SetLastLineStep(double lineStep);
  double GetLineStepFactor(void) const;
  void SetLineStepFactor(double stepFactor);
  virtual void SetTime(const NFmiMetTime& theTime);
  double GetWidthFactor(void) const;
  void Move(const NFmiPoint& addPlace);
  virtual bool HasSubText(void) const;
  NFmiPressText* GetSubText(void) const;

 protected:
  double itsLastLineStep;  // tarvitaan jotta ei uusi kappale alkaisi kuin olisi vanha rivinväli
  double itsLineStep;
  double itsLineStepFactor;
  double itsTopMargin;
  double itsRightMargin;
  double itsLeftMargin;
  double itsIndent;
  bool fRightJustification;
  bool fLoopErrorReported;
  bool fInParagraph;
  bool fInFreeArea;
  NFmiPoint itsParagraphMove;
  NFmiString itsAddInFront;
  NFmiString itsAddAfter;
  bool fUpperCase;
  bool fLowerCase;
  bool fFirstUpperCase;
  bool fAddLocalTime;
  NFmiString* itsText;
  NFmiPressText* itsSubText;
  double itsCharSpace;
  unsigned long itsMaxLen;
  double itsWidthFactor;
  std::vector<NFmiPoint> itsFreePath;
  // std::vector<NFmiHyphenationString> itsNextTexts;
  std::vector<NFmiPressText> itsNextTexts;
  bool fNarrowColumn;
};  // class NFmiPressText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressText::NFmiPressText()
    : NFmiPressScaling(),
      itsLastLineStep(0.),
      itsLineStep(0.),
      itsLineStepFactor(0.),
      itsTopMargin(-1000.),
      itsRightMargin(0),
      itsLeftMargin(0),
      itsIndent(0.),
      fRightJustification(true),
      fLoopErrorReported(false),
      fInParagraph(false),
      fInFreeArea(false),
      fUpperCase(0),
      fLowerCase(0),
      fFirstUpperCase(false),
      fAddLocalTime(false),
      itsText(0),
      itsCharSpace(0.),
      itsMaxLen(kUnsignedLongMissing),
      itsWidthFactor(1.),
      fNarrowColumn(false)
{
  itsSubText = 0;
  itsLanguage = kFinnish;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressText::SetText(void) { return true; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetText(NFmiString theText)
{
  if (itsText) delete itsText;
  itsText = new NFmiString(theText);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::AddText(const NFmiString& theText)
{
  if (itsText) itsText->Add(theText);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetPreText(const NFmiString& theText) { itsAddInFront = theText; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetPostText(const NFmiString& theText) { itsAddAfter = theText; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressText* NFmiPressText::GetSubText(void) const { return itsSubText; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressText::HasSubText(void) const { return itsSubText ? true : false; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressText::WritePSUpdatingSubText(FmiPressOutputMode theOutput)
{
  return WritePS(theOutput);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiString* NFmiPressText::GetText(void) const { return itsText; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiPressText::GetWidthFactor(void) const { return itsWidthFactor; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLanguage(FmiLanguage theLanguage) { itsLanguage = theLanguage; }
inline FmiLanguage NFmiPressText::GetLanguage(void) { return itsLanguage; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressText::IsInParagraph(void) { return fInParagraph; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLineStep(double lineStep) { itsLineStep = lineStep; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiPressText::GetLineStep(void) const { return itsLineStep; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param lineStep Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLastLineStep(double lineStep) { itsLastLineStep = lineStep; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLineStepFactor(double lineStep) { itsLineStepFactor = lineStep; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiPressText::GetLineStepFactor(void) const { return itsLineStepFactor; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetTime(const NFmiMetTime& theTime) {}
#endif  // NFMIPRESSTEXT_H

// ======================================================================
