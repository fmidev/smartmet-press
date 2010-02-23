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

#ifndef UNIX
#pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiDescription.h"
#include "NFmiPressEnvironment.h"
#include "NFmiPressTypes.h"
#include <map>

// T‰m‰ nyt on t‰llainen kun tarvitaan rinnan vanhaa ja uutta skaalausta
const double c40 = 40;

//! Undocumented
enum NFmiPressDescriptionObjects
{
  dProducer = 50,
  dTextObject,
  dColumnTextObject,
  dTimeTextObject,
  dOrigTimeObject,
  dComputerTimeTextObject,
  dImageObject,
  dStationTextObject,
  dLongNumberMinus,
  dShortNumberMinus,
  dLanguage = 1050,
  dRGBColor,
  dColor,
  dMaskNumber,
  dPressMaskNumber,
  dDescFont,
  dDescTextSize,
  dDescTextAlignment,
  dWidthFactor,
  dMissingPrecedingImage,
  dVersion = 2050,
  dSegmentMove,
  dDescSymbolSet,
  dSymbolSizeFactor,
  dStationNumberObject,
  dAvoidOrphanSyllables,
  dUseBackupPreviousDay
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

  NFmiString getCnfPath(void) const;
  NFmiString getTmpPath(void) const;
  NFmiString getSymbolCachePath(void) const;
  NFmiString getIncPath(void) const;
  NFmiString getDataPath(void) const;
  NFmiString getOutPath(void) const;
  NFmiString getLogPath(void) const;
  NFmiString getProductName(void) const;

  void setCnfPath(const NFmiString & path);
  void setTmpPath(const NFmiString & path);
  void setSymbolCachePath(const NFmiString & path);
  void setIncPath(const NFmiString & path);
  void setDataPath(const NFmiString & path);
  void setOutPath(const NFmiString & path);
  void setLogPath(const NFmiString & path);
  void setProductName(const NFmiString & productName);

  void SetEnvironment(const NFmiPressEnvironment & theEnvironment);
  NFmiPressEnvironment GetEnvironment(void) const;

  FmiLanguage ReadLanguage(void);
  bool IsRGB(void) const;
  bool IsCMYK(void) const;

	FmiGenericColor GetColor(void) const;
	void SetColor(const FmiGenericColor & theColor);
	bool IsPureBlack(void);
	NFmiString GetFont(void)const;
	void SetFont(const NFmiString& font);
	double GetTextSize(void)const;
	void SetTextSize(double size);
	FmiDirection GetTextAlignment(void)const;
	void SetTextAlignment(FmiDirection alignment);
	bool GetOnlyForMissingPrecedingElementFlag(void) const;
	void SetOnlyForMissingPrecedingElementFlag(bool theFlag);  
    bool IsSegmentMove(void)const;
	bool BlockLatinFont(const NFmiString& font) const; 


protected:

  FmiDirection String2FmiDirection(const NFmiString& theString) const;
  virtual bool ReadRemaining(void);

  NFmiPressEnvironment itsEnvironment;  
  NFmiString itsHomePath; 

  NFmiString itsProductName;
  NFmiString itsCnfPath;
  NFmiString itsTmpPath;
  NFmiString itsSymbolCachePath;
  NFmiString itsIncPath;
  NFmiString itsDataPath;
  NFmiString itsOutPath;
  NFmiString itsLogPath;

  FmiLanguage itsLanguage;
  bool fOnlyForMissingPrecedingElement;
  void MoveSegmentPlace(double& theX, double& theY)const;
  void MoveSegmentPlaceConditionally(double& theX, double& theY)const;

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
	fOnlyForMissingPrecedingElement= false;
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
	, itsEnvironment(theD.itsEnvironment)
	, itsHomePath(theD.itsHomePath)
	, itsProductName(theD.itsProductName)
	, itsCnfPath(theD.itsCnfPath)
	, itsTmpPath(theD.itsTmpPath)
	, itsSymbolCachePath(theD.itsSymbolCachePath)
	, itsIncPath(theD.itsIncPath)
	, itsDataPath(theD.itsDataPath)
	, itsOutPath(theD.itsOutPath)
	, itsLogPath(theD.itsLogPath)
	, itsLanguage(theD.itsLanguage)
	, fOnlyForMissingPrecedingElement(false)
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
	fOnlyForMissingPrecedingElement= false;
}	 

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressDescription::BlockLatinFont(const NFmiString& font) const
{
  return true;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::SetOnlyForMissingPrecedingElementFlag(bool theFlag)
{
  fOnlyForMissingPrecedingElement = theFlag;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressDescription::GetOnlyForMissingPrecedingElementFlag(void) const
{
  return fOnlyForMissingPrecedingElement;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
FmiDirection NFmiPressDescription::GetTextAlignment(void)const
{
  return itsEnvironment.GetTextAlignment();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::SetTextAlignment(FmiDirection alignment)
{
  itsEnvironment.SetTextAlignment(alignment);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
double NFmiPressDescription::GetTextSize(void)const
{
  return itsEnvironment.GetTextSize();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::SetTextSize(double size)
{
  itsEnvironment.SetTextSize(size);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::SetFont(const NFmiString& font)
{
  itsEnvironment.SetFont(font);
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::GetFont(void) const
{
  return itsEnvironment.GetFont();
}// ----------------------------------------------------------------------
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
 * Gets the name of the product under process.
 *
 *†\return The name of the product.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getProductName(void) const
{
  return itsProductName;
}

// ----------------------------------------------------------------------
/*!
 * Sets the name of the product under process.
 *
 *†\param productName The name to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setProductName(const NFmiString & productName)
{
  itsProductName = productName;
}

// ----------------------------------------------------------------------
/*!
 * Gets the path to the configuration files.
 *
 *†\return The configuration path.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getCnfPath(void) const
{
  return itsCnfPath;
}

// ----------------------------------------------------------------------
/*!
 * Sets the path to the configuration files.
 *
 *†\param path The path to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setCnfPath(const NFmiString & path)
{
  itsCnfPath=path;
}

// ----------------------------------------------------------------------
/*!
 * Gets the path to the temporary files
 *
 *†\return The temp path.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getTmpPath(void) const
{
  return itsTmpPath;
}

// ----------------------------------------------------------------------
/*!
 * Sets the path to the temporary files.
 *
 *†\param path The path to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setTmpPath(const NFmiString & path)
{
  itsTmpPath=path;
}

// ----------------------------------------------------------------------
/*!
 * Gets the path to the symbol cache files
 *
 *†\return The symbol cache path.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getSymbolCachePath(void) const
{
  return itsSymbolCachePath;
}

// ----------------------------------------------------------------------
/*!
 * Sets the path to the symbol cache files.
 *
 *†\param path The path to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setSymbolCachePath(const NFmiString & path)
{
  itsSymbolCachePath=path;
}

// ----------------------------------------------------------------------
/*!
 * Gets the path to the .inc files
 *
 *†\return The .inc file path.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getIncPath(void) const
{
  return itsIncPath;
}

// ----------------------------------------------------------------------
/*!
 * Sets the path to the .inc files.
 *
 *†\param path The path to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setIncPath(const NFmiString & path)
{
  itsIncPath=path;
}

// ----------------------------------------------------------------------
/*!
 * Gets the path to the data files
 *
 *†\return The data file path.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getDataPath(void) const
{
  return itsDataPath;
}

// ----------------------------------------------------------------------
/*!
 * Sets the path to the data files.
 *
 *†\param path The path to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setDataPath(const NFmiString & path)
{
  itsDataPath=path;
}

// ----------------------------------------------------------------------
/*!
 * Gets the path to the output files
 *
 *†\return The output file path.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getOutPath(void) const
{
  return itsOutPath;
}

// ----------------------------------------------------------------------
/*!
 * Sets the path to the output files.
 *
 *†\param path The path to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setOutPath(const NFmiString & path)
{
  itsOutPath=path;
}

// ----------------------------------------------------------------------
/*!
 * Gets the path to the log files
 *
 *†\return The log file path.
 */
// ----------------------------------------------------------------------

inline
NFmiString NFmiPressDescription::getLogPath(void) const
{
  return itsLogPath;
}

// ----------------------------------------------------------------------
/*!
 * Sets the path to the log files.
 *
 *†\param path The path to be set.
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::setLogPath(const NFmiString & path)
{
  itsLogPath=path;
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

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressDescription::IsSegmentMove(void)const
{
  return itsEnvironment.IsSegmentMove();
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressDescription::MoveSegmentPlace(double& theX, double& theY)const
{
  theX += itsEnvironment.GetSegmentMove().X();
  theY += itsEnvironment.GetSegmentMove().Y();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theColor Undocumented
 */
// ----------------------------------------------------------------------
inline
void NFmiPressDescription::MoveSegmentPlaceConditionally(double& theX, double& theY)const
{
  if(IsSegmentMove())
  {
	  MoveSegmentPlace(theX, theY);
  }
}

#endif // NFMIPRESSDESCRIPTION_H

// ======================================================================
