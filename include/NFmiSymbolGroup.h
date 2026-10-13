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
class NFmiSymbolGroup : public NFmiSize, public NFmiPressTimeDescription
{
 public:
  ~NFmiSymbolGroup() override;
  NFmiSymbolGroup();
  NFmiSymbolGroup(NFmiPressParam* pressParam, NFmiParamRect** theSymbolList, int num);

  long NumOfMissing() const;
  void InitMissing();
  bool ReadDescription(NFmiString& retString) override;

  bool WritePS(NFmiStationPoint* theStationPoint, FmiPressOutputMode theOutput);

  bool SetPressParam(NFmiPressParam* param);
  bool SetProducer(unsigned int producer);

  bool Set(const NFmiRectScale& theRectScale,
           NFmiFastQueryInfo* theQueryDataIter,
           std::ofstream& theDestinationFile);

  bool Add(const NFmiParamRect& theParamRect);
  int ConvertDefText(NFmiString& object) override;
  bool SetAllTimes(const NFmiMetTime& theTime);
  bool SetAllLanguages(FmiLanguage theLanguage);

 private:
  void SetScalingMode();

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

inline NFmiSymbolGroup::NFmiSymbolGroup()
    : itsPressParam(nullptr),
      itsProducer(230),
      itsPsSymbol(nullptr),
      itsParamRects(nullptr),
      itsRectSize(40, 40),
      itsRectScale(nullptr),
      itsQueryData(nullptr),
      itsQueryDataIter(nullptr)
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
