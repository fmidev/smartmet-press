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
  dInParagraph = 1140,
  dParagraphMove,
  dTopMargin,
  dLeftMargin,
  dRightMargin,
  dParagraphBorders,
  dLineStep,
  dIndent,
  dLF,
  dLFFactor,
  dSubTextObject = 2140,
  dSubTimeTextObject,
  dSubComputerTimeTextObject,
  dTextLanguage,
  dTextDir,
  dTextPath
};


//! Undocumented
class _FMI_DLL NFmiPressText : public NFmiPressScaling
{
public:

  virtual ~NFmiPressText(void);
  NFmiPressText(void);
  NFmiPressText(const NFmiPressText & theTextParamRect); 

  virtual bool ReadDescription(NFmiString & retString);
  virtual bool ReadRemaining(void); 

  int ConvertDefText(NFmiString & object);
  NFmiString Construct(NFmiString * theString) const;

  using NFmiPressScaling::WritePS;
  virtual bool WritePS(FmiPressOutputMode theOutput);
  void SetValueDepending(float theValue);

  virtual bool SetText(void);
  virtual void SetText(NFmiString theText);

  void SetTextAttributes(NFmiString theFont,
						 FmiDirection theAlignment,
						 NFmiString theStyle);

  bool WriteString(const NFmiString & commentString,
				   FmiPressOutputMode theOutput);

  virtual bool WritePSUpdatingSubText(FmiPressOutputMode theOutput);

  NFmiString GetFont(void) const;
  FmiDirection GetAlignment(void) const;
  NFmiString GetStyle(void) const;
  NFmiString * GetText(void) const;
  void SetLanguage(FmiLanguage theLanguage);
  FmiLanguage GetLanguage(void);
  virtual bool IsInParagraph(void);
  virtual void ScalePlotting(void);
  double GetLineStep(void) const;
  void SetLastLineStep(double lineStep);
  virtual void SetTime(const NFmiMetTime& theTime);

protected:               

  double itsLastLineStep;	// tarvitaan jotta ei uusi kappale alkaisi kuin olisi vanha rivinväli 
  double itsLineStep;
  double itsLineStepFactor;
  double itsTopMargin;
  double itsRightMargin;
  double itsLeftMargin;
  double itsIndent;
  bool fLoopErrorReported;
  bool fInParagraph;     
  NFmiPoint  itsParagraphMove;  
  NFmiString itsAddInFront;   
  NFmiString itsAddAfter;     
  bool fUpperCase;  
  bool fLowerCase;   
  bool fAddLocalTime;  
  NFmiString * itsText;       
  NFmiPressText * itsSubText;
  NFmiString itsFont;
  FmiDirection itsAlignment;
  NFmiString itsStyle;
  double itsCharSpace;      

}; // class NFmiPressText

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressText::NFmiPressText(void)
  : NFmiPressScaling()
  , itsLastLineStep(0.)
  , itsLineStep(0.)
  , itsLineStepFactor(0.)
  , itsTopMargin(-1000.)
  , itsIndent(0.)
  , fLoopErrorReported(false)
  , fInParagraph(0)
  , fUpperCase(0)
  , fLowerCase(0)
  , fAddLocalTime(false)  
  , itsText(0)
  , itsFont(NFmiString("Courier"))
  , itsAlignment(kCenter) 
  , itsCharSpace(0.)      
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

inline
bool NFmiPressText::SetText(void)
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

inline
void NFmiPressText::SetText(NFmiString theText)
{
  if (itsText)
	delete itsText;
  itsText = new NFmiString(theText);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFont Undocumented
 * \param theAlignment Undocumented
 * \param theStyle Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressText::SetTextAttributes(NFmiString theFont,
									  FmiDirection theAlignment,
									  NFmiString theStyle)
{
  itsFont = theFont;
  itsAlignment = theAlignment;
  itsStyle = theStyle;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theOutput Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressText::WritePSUpdatingSubText(FmiPressOutputMode theOutput)
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

inline
NFmiString NFmiPressText::GetFont(void) const
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
FmiDirection NFmiPressText::GetAlignment(void) const
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
NFmiString NFmiPressText::GetStyle(void) const
{
  return itsStyle;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiString * NFmiPressText::GetText(void) const
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

inline
void NFmiPressText::SetLanguage(FmiLanguage theLanguage)
{
  itsLanguage=theLanguage;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
FmiLanguage NFmiPressText::GetLanguage(void)
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

inline
bool NFmiPressText::IsInParagraph(void)
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

inline
double NFmiPressText::GetLineStep(void) const
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

inline
void NFmiPressText::SetLastLineStep(double lineStep)
{
  itsLastLineStep=lineStep;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressText::SetTime(const NFmiMetTime & theTime)
{
}

#endif // NFMIPRESSTEXT_H

// ======================================================================
