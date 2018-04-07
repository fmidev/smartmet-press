// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiSymbolGroup
 */
// ======================================================================
/*!
 * \class NFmiSymbolGroup
 *
 * Luokka toimii datariippuvien olioiden (symbolit, numerot)
 * listana yhden segmentin (NFmiPresParam) puitteissa. Skaalauksen
 * hoitaminen tätä kautta on vanhentunut.
 *
 */
// ======================================================================

#ifndef NFMISYMBOLGROUP_H
#define NFMISYMBOLGROUP_H

// press
#include "NFmiParamRect.h"
#include "NFmiPressTimeDescription.h"
#include "NFmiRectScale.h"
#include "NFmiStationPoint.h"
// newbase
#include "NFmiQueryData.h"
#include "NFmiSize.h"

class NFmiPressParam;
class NFmiPsSymbol;

//! Undocumented
class _FMI_DLL NFmiSymbolGroup : public NFmiSize, public NFmiPressTimeDescription
{
 public:
  ~NFmiSymbolGroup(void);
  NFmiSymbolGroup(void);
  NFmiSymbolGroup(NFmiPressParam* pressParam, NFmiParamRect** theSymbolList, int num);

  long NumOfMissing(void) const;
  void InitMissing(void);
  bool ReadDescription(NFmiString& retString);

  bool WritePS(NFmiStationPoint* theStationPoint, FmiPressOutputMode theOutput);

  bool SetPressParam(NFmiPressParam* param);
  bool SetProducer(unsigned int producer);

  bool Set(NFmiRectScale& theRectScale,
           NFmiFastQueryInfo* theQueryDataIter,
           std::ofstream& theDestinationFile);

  bool Add(const NFmiParamRect& theParamRect);
  int ConvertDefText(NFmiString& object);
  bool SetAllTimes(const NFmiMetTime& theTime);
  bool SetAllLanguages(FmiLanguage theLanguage);

 private:
  void SetScalingMode(void);

  NFmiPressParam* itsPressParam;
  bool fNewScaling;
  unsigned long itsProducer;
  NFmiPsSymbol* itsPsSymbol;
  NFmiVoidPtrList itsPressScalingObjects;
  NFmiParamRect** itsParamRects;
  NFmiPoint itsRectSize;

  NFmiRectScale* itsRectScale;
  NFmiQueryData* itsQueryData;
  NFmiFastQueryInfo* itsQueryDataIter;
  std::ofstream* itsOutFile;

};  // class NFmiSymbolGroup

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiSymbolGroup::NFmiSymbolGroup(void)
    : itsPressParam(0),
      itsProducer(230),
      itsPsSymbol(0),
      itsParamRects(0),
      itsRectSize(40, 40),
      itsRectScale(0),
      itsQueryData(0),
      itsQueryDataIter(0)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param param Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiSymbolGroup::SetPressParam(NFmiPressParam* param)
{
  itsPressParam = param;
  return isTrue;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param producer Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiSymbolGroup::SetProducer(unsigned int producer)
{
  itsProducer = producer;
  return isTrue;
}

#endif  // NFMISYMBOLGROUP_H

// ======================================================================
