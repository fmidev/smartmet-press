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

#include "NFmiHyphenationString.h"
#include "NFmiPressScaling.h"
#include "NFmiPressTypes.h"
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
class NFmiPressText : public NFmiPressScaling
{
 public:
  ~NFmiPressText() override;
  NFmiPressText();
  NFmiPressText(const NFmiPressText& theTextParamRect);
  NFmiPressText& operator=(const NFmiPressText&) = default;

  bool ReadDescription(NFmiString& retString) override;
  bool ReadRemaining() override;

  int ConvertDefText(NFmiString& object) override;
  NFmiString Construct(NFmiString* theString) const;

  using NFmiPressScaling::WritePS;
  bool WritePS(FmiPressOutputMode theOutput) override;
  void SetValueDepending(float theValue);

  virtual bool SetText();
  virtual void SetText(NFmiString theText);
  virtual void AddText(const NFmiString& theText);
  virtual void SetPreText(const NFmiString& theText);
  virtual void SetPostText(const NFmiString& theText);
  bool WriteString(const NFmiString& commentString, FmiPressOutputMode theOutput);

  bool WritePSUpdatingSubText(FmiPressOutputMode theOutput) override;

  NFmiString* GetText() const;
  void SetLanguage(FmiLanguage theLanguage) override;
  void SetEncoding(std::string theEncoding);
  FmiLanguage GetLanguage();
  bool IsInParagraph() override;
  void ScalePlotting() override;
  double GetLineStep() const override;
  void SetLineStep(double lineStep);
  void SetLastLineStep(double lineStep) override;
  double GetLineStepFactor() const;
  void SetLineStepFactor(double stepFactor);
  virtual void SetTime(const NFmiMetTime& theTime);
  double GetWidthFactor() const;
  void Move(const NFmiPoint& addPlace) override;
  bool HasSubText() const override;
  NFmiPressText* GetSubText() const;

 protected:
  double itsLastLineStep = 0.;  // tarvitaan jotta ei uusi kappale alkaisi kuin olisi vanha rivinväli
  double itsLineStep = 0.;
  double itsLineStepFactor = 0.;
  double itsTopMargin = -1000.;
  double itsRightMargin = 0;
  double itsLeftMargin = 0;
  double itsIndent = 0.;
  bool fRightJustification = true;
  bool fLoopErrorReported = false;
  bool fInParagraph = false;
  bool fInFreeArea = false;
  NFmiPoint itsParagraphMove;
  NFmiString itsAddInFront;
  NFmiString itsAddAfter;
  bool fUpperCase = false;
  bool fLowerCase = false;
  bool fFirstUpperCase = false;
  bool fAddLocalTime = false;
  NFmiString* itsText = nullptr;
  NFmiPressText* itsSubText;
  double itsCharSpace = 0.;
  unsigned long itsMaxLen;
  double itsWidthFactor = 1.;
  std::vector<NFmiPoint> itsFreePath;
  // std::vector<NFmiHyphenationString> itsNextTexts;
  std::vector<NFmiPressText> itsNextTexts;
  bool fNarrowColumn = false;
};  // class NFmiPressText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressText::NFmiPressText() : itsSubText(nullptr), itsMaxLen(kUnsignedLongMissing)

{

  itsLanguage = kFinnish;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressText::SetText()
{
  return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetText(NFmiString theText)
{
    delete itsText;
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
  if (itsText)
    itsText->Add(theText);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetPreText(const NFmiString& theText)
{
  itsAddInFront = theText;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theText Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetPostText(const NFmiString& theText)
{
  itsAddAfter = theText;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressText* NFmiPressText::GetSubText() const
{
  return itsSubText;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressText::HasSubText() const
{
  return itsSubText != nullptr;
}
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

inline NFmiString* NFmiPressText::GetText() const
{
  return itsText;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiPressText::GetWidthFactor() const
{
  return itsWidthFactor;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLanguage(FmiLanguage theLanguage)
{
  itsLanguage = theLanguage;
}
inline FmiLanguage NFmiPressText::GetLanguage()
{
  return itsLanguage;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressText::IsInParagraph()
{
  return fInParagraph;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLineStep(double lineStep)
{
  itsLineStep = lineStep;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiPressText::GetLineStep() const
{
  return itsLineStep;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param lineStep Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLastLineStep(double lineStep)
{
  itsLastLineStep = lineStep;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetLineStepFactor(double lineStep)
{
  itsLineStepFactor = lineStep;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiPressText::GetLineStepFactor() const
{
  return itsLineStepFactor;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressText::SetTime(const NFmiMetTime& theTime)
{
  itsFirstPlotTime = theTime;
}

#endif  // NFMIPRESSTEXT_H

// ======================================================================
