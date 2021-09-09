// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPsWriting
 */
// ======================================================================
/*!
 * \class NFmiPsWriting
 *
 * Luokka hoitaa PS-koodin kirjoittamisen tulostiedostoon
 * ja siinä yhteydessä koodaa skaalauksen myös (scale+transform)
 * Tästä peritään luokat jotka tuottavat ps-koodia
 */
// ======================================================================

#ifndef NFMIPSWRITING_H
#define NFMIPSWRITING_H

// press
#include "NFmiPressTypes.h"
#include "NFmiRectScale.h"
// newbase
#include "NFmiString.h"
#include <vector>

// Undocumented
enum FmiPressOutputMode
{
  kPostScript = 1,
  kMetaLanguage,
  kXml,
  kPlainText
};

//! Undocumented
class NFmiPsWriting
{
 public:
  ~NFmiPsWriting(void);
  NFmiPsWriting(void);
  NFmiPsWriting(const NFmiPsWriting& thePsWriting);

  bool WriteOutString(const NFmiString& outString);
  bool CopyPsFile(void);
  bool CopyFileWithoutShowpage(void);
  bool WriteGRestore(void);
  void Rotate(void);
  bool WritePSConcat(void);
  bool WritePSConcatRotating(float);
  bool WritePSEnd(void);
  bool WriteEPSConcatClipping(NFmiRect theClippingRect);
  bool WriteEPSConcatClipping(std::vector<NFmiPoint> points);
  bool WriteEPSConcat(void);
  bool WriteEPSEnd(void);
  void SetRotatingPoint(const NFmiPoint& thePoint);
  void SetRotBeforeScale(const bool rBeforeS);
  NFmiString AlignmentToMeta(const FmiDirection& pressAlignment) const;
  void SetFile(std::ofstream& theDestinationFile);

  void WriteColor(const FmiGenericColor& theColor,
                  FmiPressOutputMode mode,
                  std::ofstream& os) const;

  bool IsActive(void) const;
  void SetActivity(bool theActivity);
  bool IsToWriteLast(void) const;
  void SetWriteLast(bool theWriteLat);
  void SetLineFeed(bool theValue);
  bool IsLineFeed(void);

 protected:
  bool fActivity;
  bool fWriteAsLast;
  bool fRotBeforeScale;
  NFmiPoint itsRotatingPoint;
  double itsRotatingAngle;
  bool fOverPrint;
  std::ifstream* itsInFile;
  std::ofstream* itsOutFile;  // does not own, does not destroy
  NFmiRectScale itsWriteScale;
  bool fLineFeed;

};  // class NFmiPsWriting

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPsWriting::NFmiPsWriting(void)
    : fActivity(true),
      fWriteAsLast(false),
      fRotBeforeScale(false),
      itsRotatingAngle(0.),
      fOverPrint(false),
      itsInFile(0),
      itsOutFile(0),
      itsWriteScale(NFmiRectScale(NFmiRect(NFmiPoint(0., 0.), NFmiPoint(1., 1.)),
                                  NFmiRect(NFmiPoint(0., 0.), NFmiPoint(1., 1.)))),
      fLineFeed(true)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePoint Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPsWriting::IsLineFeed(void)
{
  return fLineFeed;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePoint Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPsWriting::SetLineFeed(bool theValue)
{
  fLineFeed = theValue;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePoint Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPsWriting::SetRotatingPoint(const NFmiPoint& thePoint)
{
  itsRotatingPoint = thePoint;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param rBeforeS Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPsWriting::SetRotBeforeScale(const bool rBeforeS)
{
  fRotBeforeScale = rBeforeS;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theDestinationFile Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPsWriting::SetFile(std::ofstream& theDestinationFile)
{
  itsOutFile = &theDestinationFile;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPsWriting::IsActive(void) const
{
  return fActivity;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theActivity Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPsWriting::SetActivity(bool theActivity)
{
  fActivity = theActivity;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPsWriting::IsToWriteLast(void) const
{
  return fWriteAsLast;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theWriteLat Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPsWriting::SetWriteLast(bool theWriteLat)
{
  fWriteAsLast = theWriteLat;
}
#endif  // NFMIPSWRITING_H

// ======================================================================
