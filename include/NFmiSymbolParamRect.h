// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiSymbolParamRect
 */
// ======================================================================
/*!
 * \class NFmiSymbolParamRect
 *
 * T‰m‰ luokka hallitsee dataohjattujen symbolien tuottamisen
 * m‰‰rittelytiedoston lukemisesta ps-tiedoston kirjoittamiseen.
 * Ensimm‰isi‰ luokkia, jonka j‰lkeen on syntynyt rinnakkaisia
 * rakenteita (ps-tiedoston kirjoitus, Illu-symbolit->"lyhyt-
 * symboleiksi").
 *
 * T‰st‰ on edelleen peritty k‰‰ntyv‰ symboli (NFmiRotatingParamRect).
 *
 */
// ======================================================================

#ifndef NFMISYMBOLPARAMRECT_H
#define NFMISYMBOLPARAMRECT_H

#include "NFmiParamMapping.h"
#include "NFmiParamRect.h"

//! Undocumented
enum NFmiSymbolParamRectObjects
  {
	dSymbolDirectory = 100,
	dMapping,
	dScaleMapping,
	dSymbolDefSize,
	dRelSize,
	dConstSymbolName
  };


//! Undocumented
class _FMI_DLL NFmiSymbolParamRect : public NFmiParamRect
{

public:

  virtual ~NFmiSymbolParamRect(void);

  NFmiSymbolParamRect(void);
  NFmiSymbolParamRect(const NFmiSymbolParamRect & theSymbolRect);
		
  virtual int  ConvertDefText(NFmiString & object);
  virtual bool ReadDescription(NFmiString & retString); 
  virtual bool ReadRemaining(void);
  virtual NFmiParamRect * Clone(void) const; 

  virtual bool WritePS(const NFmiRect & AbsoluteRectOfSymbolGroup,
					   NFmiFastQueryInfo * theQI,
					   std::ofstream & theDestinationFile,
					   FmiPressOutputMode theOutput);

  void SetConstSymbol(bool boo);

protected:

  virtual void DoPostReading(void);
  virtual bool CopyShortSymbol2Dest(NFmiString * symbolFile,
									std::ofstream & theDestinationFile); 
  virtual bool ConvertOrig2Short(NFmiString * symbolFile);
  virtual bool ReadValues(NFmiFastQueryInfo * theQI, bool SetRelHour=true);
  virtual void ScaleByValue(void);
  virtual void WriteMetaCode(NFmiString * symbolFile,
							 NFmiPoint realPos,
							 std::ofstream & os);

protected:

  NFmiString         itsConstSymbol;   // #VakioSymbolia varten
  bool		         fIsConstSymbol;   // #VakioSymbolia varten
  NFmiPoint          itsSizeFactor;		// RotParr tarttee
  NFmiParamMapping * itsMapping;
  NFmiString *       itsSubDir;	
  NFmiString *       itsOrigDir;
  NFmiString         itsSymbolSetName;
  NFmiRectScale		 itsDefToProductScale;

}; // class NFmiSymbolParamRect

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiSymbolParamRect::NFmiSymbolParamRect(void)
  : NFmiParamRect()
  , fIsConstSymbol(false)
  , itsMapping(0)
  , itsSubDir(0)	
  , itsOrigDir(0)
  , itsDefToProductScale(NFmiRect(0.,0.,140.,140.),  //sovittu koko
						 NFmiRect(0.,0.,1.,1.))
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param boo Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiSymbolParamRect::SetConstSymbol(bool boo)
{
  fIsConstSymbol=boo;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiSymbolParamRect::DoPostReading(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiSymbolParamRect::ScaleByValue(void)
{
}

#endif // NFMISYMBOLPARAMRECT_H

// ======================================================================
