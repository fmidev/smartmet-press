// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressDescription
 */
// ======================================================================
/*!
 * \class NFmiPressDescription
 *
 * Peritty yleisest‰ NFmiDescription-luokasta. T‰‰ll‰ ovat sellaiset
 * ominaisuudet, jotka ovat yhteisi‰ lehtituotteiden luokkien 
 * m‰‰rittelyiss‰.
 *
 */
// ======================================================================

#ifndef NFMIPRESSDESCRIPTION_H
#define NFMIPRESSDESCRIPTION_H

#include "NFmiDescription.h"
#include "NFmiPressEnvironment.h"
#include "NFmiPressTypes.h"

// T‰m‰ nyt on t‰llainen kun tarvitaan rinnan vanhaa ja uutta skaalausta
const double c40 = 40;

//! Undocumented
enum NFmiPressDescriptionObjects
{
  dProducer = 50,
  dTextObject,
  dColumnTextObject,
  dTimeTextObject,
  dComputerTimeTextObject,
  dImageObject,
  dStationTextObject,
  dLongNumberMinus,
  dShortNumberMinus,
  dLanguage,
  dRGBColor = 1050,
  dColor,
  dMaskNumber
};

//! Undocumented
class _FMI_DLL NFmiPressDescription : public NFmiDescription 
{

public:

  virtual ~NFmiPressDescription(void);
  NFmiPressDescription(void);
  NFmiPressDescription(const NFmiPressDescription & theD);
  NFmiPressDescription(std::ofstream * theLogFile,
					   unsigned short theMaxLoopNum);

  virtual int ConvertDefText(NFmiString & object); 

  NFmiString GetHome(void) const;
  void SetHome(const NFmiString & path);

  void SetEnvironment(const NFmiPressEnvironment & theEnvironment);
  NFmiPressEnvironment GetEnvironment(void) const;

  FmiLanguage ReadLanguage(void);
  bool IsRGB(void) const;
  bool IsCMYK(void) const;

  FmiGenericColor GetColor(void) const;
  void SetColor(const FmiGenericColor & theColor);
  bool IsPureBlack(void);

protected:

  virtual bool ReadRemaining(void);

  NFmiPressEnvironment itsEnvironment;  
  NFmiString itsHomePath; 
  FmiLanguage itsLanguage;

private: 

}; // class NFmiPressDescription

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline
NFmiPressDescription::~NFmiPressDescription(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressDescription::NFmiPressDescription(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theD The object being copied
 */
// ----------------------------------------------------------------------

inline
NFmiPressDescription::NFmiPressDescription(const NFmiPressDescription & theD)
  : NFmiDescription(theD.GetLogFile(),theD.GetMaxLoopNum())
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theLogFile Undocumented
 * \param theMaxLoopNum Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiPressDescription::NFmiPressDescription(std::ofstream * theLogFile,
										   unsigned short theMaxLoopNum)
  : NFmiDescription(theLogFile, theMaxLoopNum)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::GetHome(void) const
{
  return itsHomePath;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param path Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::SetHome(const NFmiString & path)
{
  itsHomePath=path;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 *†\param theEnvironment Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::SetEnvironment(const NFmiPressEnvironment & theEnvironment)
{
  itsEnvironment=theEnvironment;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiPressEnvironment NFmiPressDescription::GetEnvironment(void) const
{
  return itsEnvironment;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressDescription::IsRGB(void) const
{
  return itsEnvironment.IsRGB();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressDescription::IsCMYK(void) const
{
  return itsEnvironment.IsCMYK();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
FmiGenericColor NFmiPressDescription::GetColor(void) const
{
  return itsEnvironment.GetColor();
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::SetColor(const FmiGenericColor & theColor)
{
  itsEnvironment.SetColor(theColor);
}

#endif // NFMIPRESSDESCRIPTION_H

// ======================================================================
