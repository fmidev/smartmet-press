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
class _FMI_DLL NFmiExtremeTimeParamRect : public NFmiTimeParamRect
{
public:

  virtual ~NFmiExtremeTimeParamRect();
  NFmiExtremeTimeParamRect(void);
  NFmiExtremeTimeParamRect(const NFmiExtremeTimeParamRect & theTimeParamRect); 

  virtual NFmiParamRect * Clone(void) const; 

protected:

  NFmiTime TimeToWrite(NFmiFastQueryInfo * theQI);

private:

}; // class NFmiExtremeTimeParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiExtremeTimeParamRect::NFmiExtremeTimeParamRect(void)
  : NFmiTimeParamRect()						
{
	fUseSelectLatinFont = true;
}


#endif // NFMIEXTREMETIMEPARAMRECT_H

// ======================================================================
