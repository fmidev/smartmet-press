// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiExtremePlaceParamRect.h
 */
// ======================================================================
/*!
 * \class NFmiExtremePlaceParamRect
 *
 * Datan ‰‰riarvojen sattumispaikan luokka.
 * Vaatii aina parikseen dataolion, joka suoritetaan ensin ja jossa
 * ‰‰riarvo (max/min) m‰‰ritet‰‰n ja jossa samalla talletetaan paikka
 * t‰t‰ oliota varten. Paikka(Location) on viety NFmiPressParam:lle, joka on yhteinen
 * Container molemmille.
 *
 */
// ======================================================================

#ifndef NFMIEXTREMEPLACEPARAMRECT_H
#define NFMIEXTREMEPLACEPARAMRECT_H

#include "NFmiTextParamRect.h"
class NFmiText;

//! Undocumented
class NFmiExtremePlaceParamRect : public NFmiTextParamRect
{
 public:
  virtual ~NFmiExtremePlaceParamRect();
  NFmiExtremePlaceParamRect(void);
  NFmiExtremePlaceParamRect(const NFmiExtremePlaceParamRect& theTextParamRect);

  virtual NFmiParamRect* Clone(void) const;
  bool ReadDescription(NFmiString& retString);
  bool WritePS(const NFmiRect& theAbsoluteRectOfSymbolGroup,
               NFmiFastQueryInfo* theQI,
               ofstream& theDestinationFile,
               FmiPressOutputMode theOutput);

 protected:
  NFmiText TextToWrite(void) const;

 private:
};

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiExtremePlaceParamRect::NFmiExtremePlaceParamRect(void) : NFmiTextParamRect()
{
  fUseSelectLatinFont = true;
}

#endif  // NFMIEXTREMEPLACEPARAMRECT_H

// ======================================================================
