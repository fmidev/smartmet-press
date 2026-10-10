// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiExtremeTimeParamRect.h
 */
// ======================================================================
/*!
 * \class NFmiExtremeTimeParamRect
 *
 * Datan ‰‰riarvojen sattumisajan luokka.
 * Vaatii aina parikseen dataolion, joka suoritetaan ensin ja jossa
 * ‰‰riarvo (max/min) m‰‰ritet‰‰n ja jossa samalla talletetaan aika
 * t‰t‰ oliota varten. Aika on viety NFmiPressParam:lle (miksei NFmiSymbolGroup?),
 * joka on yhteinen Container molemmille.
 *
 */
// ======================================================================

#ifndef NFMIEXTREMETIMEPARAMRECT_H
#define NFMIEXTREMETIMEPARAMRECT_H

#include "NFmiTimeParamRect.h"

//! Undocumented
class NFmiExtremeTimeParamRect : public NFmiTimeParamRect
{
 public:
  ~NFmiExtremeTimeParamRect() override;
  NFmiExtremeTimeParamRect(void);
  NFmiExtremeTimeParamRect(const NFmiExtremeTimeParamRect& theTimeParamRect);

  NFmiParamRect* Clone(void) const override;

 protected:
  NFmiTime TimeToWrite(NFmiFastQueryInfo* theQI) override;

 private:
};  // class NFmiExtremeTimeParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiExtremeTimeParamRect::NFmiExtremeTimeParamRect(void) : NFmiTimeParamRect()
{
  fUseSelectLatinFont = true;
}

#endif  // NFMIEXTREMETIMEPARAMRECT_H

// ======================================================================
