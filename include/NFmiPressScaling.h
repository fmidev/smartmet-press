// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressScaling
 */
// ======================================================================
/*!
 * \class NFmiPressScaling
 *
 * PS-tuotteiden skaalaukset hallitaan tästä luokasta.
 * Koska tämä on peritty luokista NFmiDescription ja NFmiPsWriting
 * on tämä perusluokka kaikille ps-olioille ja hallitsee 
 *
 *  - ps-koodin kirjoittamisen
 *  - määrittelytiedoston lukemisen
 *  - skaalaukset
 *
 * Käytössä on kuitenkin rinnakkainen vanhempi luokkahierarkia, jossa
 * ovat esim datariippuvien sääsymbolien ja numeroiden teko.
 *
 */
// ======================================================================

#ifndef NFMIPRESSSCALING_H
#define NFMIPRESSSCALING_H

// press
#include "NFmiPressTimeDescription.h"
#include "NFmiPsWriting.h"
#include "NFmiRectScale.h"
#include "NFmiPressTime.h"
// newbase
#include "NFmiFileString.h"

class NFmiQueryData;

//! Undocumented
enum NFmiPressScalingObjects
{
  dSymbolSize,
  dRelSymbolSize,
  dPsPlaceMove,
  dSymbolPlace,
  dPrintOnce,
  dRotate,
  dRotateFirst,
  dMakeAsLast,
  dFileTimestamp,
  dNoLineFeed
};

//! Undocumented
class _FMI_DLL NFmiPressScaling : public NFmiPressTimeDescription,
								  public NFmiPsWriting
{
public:

  virtual ~NFmiPressScaling(void);
  NFmiPressScaling(void);
  NFmiPressScaling(const NFmiPressScaling & thePsSymbol);
  
  virtual int ConvertDefText(NFmiString & object);
  virtual bool ReadDescription(NFmiString & retString) = 0;
  virtual bool ReadRemaining(void);        
  virtual bool WritePS(const NFmiPoint & thePoint, FmiPressOutputMode theOutput);
  virtual bool WritePS(FmiPressOutputMode theOutput) = 0;
  virtual bool WritePSUpdatingSubText(FmiPressOutputMode theOutput);
  bool SetSize(const NFmiPoint & theSize);
  bool Set( NFmiRectScale & theRectScale, std::ofstream & theDestinationFile);
  bool SetScale(const NFmiRectScale & theRectScale);
  void Place(const NFmiPoint & thePlace);
  virtual void Move(const NFmiPoint& addPlace);
  NFmiPoint Place(void);
  void SetPrintOnceOn(void);
  void ScaleNotPlace(void);
  bool GetPrintOnce(void);
  virtual bool IsInParagraph(void);
  virtual double GetLineStep(void) const;
  virtual void SetLastLineStep(double step);
  virtual void SetLanguage(FmiLanguage newLanguage);
  virtual void ChangeByHours(long hours);
  virtual bool HasSubText(void)const;

protected:           

  virtual void ScalePlotting(void);
  NFmiFileString CreatePath(NFmiString defaultDir,
							NFmiString givenPath,
							NFmiString givenDir,
							NFmiString givenFile,
							NFmiString extension = NFmiString());

  int GetTimestampDayGap(void) const {return itsTimestampDayGap;}; 
  bool AddTimeStamp(NFmiString & theGivenFile,
					const NFmiString & theTimeFormat=NFmiString("DDMM")) const;
  bool AddValidTimeTimeStamp(NFmiString & theFile,
									 const NFmiString & theFormat,
									 const NFmiPressTime & theValidTime) const;

private:
  
protected:

  NFmiRectScale itsRectScale; //koko kartta, tulee vasta writePs;ssä
  NFmiRect itsRelArea;   
  NFmiPoint itsRectSize;  
  NFmiPoint itsPlace;  
  bool fPrintOnce;		// pakotettu kerran-vain-tulostus
  bool fScaleNotPlace;	// DataAlkio-jäsenet tarvii jotta ei kahteen kertaan siirrettäisi
  int itsTimestampDayGap;

}; // class NFmiPressScaling


// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiPressScaling::NFmiPressScaling(void)
  : NFmiPressTimeDescription()
  , itsRelArea(NFmiPoint(0.,0.), NFmiPoint(1.,1.))
  , itsRectSize(40.,40.)
  , itsPlace(NFmiPoint(0.,0.))
  , fPrintOnce(false)
  , fScaleNotPlace(false)
  , itsTimestampDayGap(kShortMissing)
{
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
bool NFmiPressScaling::HasSubText(void)const
{
  return false;
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
bool NFmiPressScaling::WritePSUpdatingSubText(FmiPressOutputMode theOutput)
{
  return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param maxNumber Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressScaling::ChangeByHours(long hours)
{
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theSize Undocumented
 * \return Undocumented
 */
// ---------------------------------------------------------------------

inline
bool NFmiPressScaling::SetSize(const NFmiPoint & theSize) 
{
  itsRectSize = theSize;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePlace Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressScaling::Place(const NFmiPoint & thePlace)
{
  itsPlace = thePlace;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param addPlace Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressScaling::Move(const NFmiPoint & addPlace)
{
  itsPlace += addPlace;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
NFmiPoint NFmiPressScaling::Place(void)
{
  return itsPlace;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressScaling::SetPrintOnceOn(void)
{
  fPrintOnce=true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressScaling::ScaleNotPlace(void)
{
  fScaleNotPlace=true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressScaling::GetPrintOnce(void)
{
return fPrintOnce;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
bool NFmiPressScaling::IsInParagraph(void)
{
return false;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
double NFmiPressScaling::GetLineStep(void) const
{
  return 0.;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param lineStep Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressScaling::SetLastLineStep(double lineStep)
{
}
 
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLanguage Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiPressScaling::SetLanguage(FmiLanguage theLanguage)
{
}

#endif // NFMIPRESSSCALING_H

// ======================================================================


