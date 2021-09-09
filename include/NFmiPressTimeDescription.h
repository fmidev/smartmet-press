// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressTimeDescription
 */
// ======================================================================
/*!
 * \class NFmiPressTimeDescription
 *
 * Peritty yleisemm‰st‰ NFmiPressDescription-luokasta. T‰nne
 * lis‰tty ajank‰sittely m‰‰rittelytiedostoa luettaessa.
 *
 */
// ======================================================================

#ifndef NFMIPRESSTIMEDESCRIPTION_H
#define NFMIPRESSTIMEDESCRIPTION_H

#ifndef UNIX
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiPressDescription.h"
// newbase
#include "NFmiMetTime.h"
#include "NFmiPreProcessor.h"
#include "NFmiQueryData.h"

class NFmiFastQueryInfo;

//! Undocumented
enum NFmiPressTimeDescriptionObjects
{
  dRelDay = 10,
  dDayAddition,
  dHour,
  dMinute,
  dRelHour,
  dHourStep,
  dDataTime,
  dRelativeDataTime,
  dGivenRelHours,
  dWriteTimeAsUtc,
  dTimeTableActive = 1010,
  dBypassValue
};

//! Undocumented
struct FmiLoopActivity
{
  unsigned long startIndex;
  unsigned long step;
  unsigned long stopIndex;
  bool bypassWithValue;
  float bypassValue;
};

//! Undocumented
struct FmiPressSeasons
{
  bool wintertime;
  bool summer;
  bool pollen;
  bool snow;
  bool pollenOrSnow;
  int weekday;
  int hour;
  int day;
  int month;
  int dayAdvance;
  bool editdata;
  bool editdataOwn;
  bool allowRandom;
  // bool afternoon;
};

//! Undocumented
class NFmiPressTimeDescription : public NFmiPressDescription
{
 public:
  virtual ~NFmiPressTimeDescription(void);

  NFmiPressTimeDescription(void);
  NFmiPressTimeDescription(short theFirstPlotMinutes,
                           short theFirstPlotHours,
                           short theFirstDeltaDays);

  bool ReadRemaining(void);
  virtual int ConvertDefText(NFmiString& object);
  void SetTime(const NFmiMetTime& theTime);
  void SetHourLoop(bool isHourLoop);
  bool IsHourLoop(void) const;

  void Set(short theFirstPlotMinutes,
           short theFirstPlotHours,
           short theFirstDeltaDays,
           short theFirstOriginalDeltaDays,
           NFmiMetTime theFirstPlotTime);

  short GetFirstPlotHours(void) const;
  short GetFirstPlotMinutes(void) const;
  short GetFirstDeltaDays(void) const;
  short GetFirstOriginalDeltaDays(void) const;
  NFmiMetTime GetFirstPlotTime(void) const;
  bool SetDataHour(NFmiFastQueryInfo* data, const NFmiString& calledFrom);
  unsigned long ReadTimeFormat(bool isSecond = false);
  NFmiMetTime NextUseTime(long relHours, long hourRes, long hourDelta);
  NFmiMetTime TimeToWrite(NFmiMetTime givenTime) const;
  bool ActiveTimeIndex(int currentInd) const;
  bool IsWriteAsUtc(void) const;
  FmiPressSeasons* GetSeasonsStatus(void) const;
  int Weekday2Num(const NFmiString& theWeekday) const;

 protected:
  long JustifyByLeaps(long dayDiff);
  void SetPreReadingTimes(void);
  void SetPostReadingTimes(void);
  void SetSeasonsStatus(FmiPressSeasons* theSeasonsStatus);

  bool PreProcessDefinition(const std::string& inFileName, const std::string& outFileName);
  bool WeekdayDirectiveActive(const std::string& theDefinition,
                              const FmiPressSeasons* theSeasons) const;
  bool PreProcessConditionally(NFmiPreProcessor& thePrepr,
                               bool theCondValue,
                               const std::string& theConditionalBeginDirective,
                               const std::string& theConditionalNotBeginDirective,
                               const std::string& theConditionalEndDirective,
                               const std::string& theConditionalElseDirective);

 protected:
  FmiLoopActivity itsLoopActivity;
  bool fGivenHoursAreRelative;
  bool fIsHourLoop;
  short itsFirstPlotHours;
  short itsFirstPlotMinutes;
  short itsFirstDeltaDays;
  short itsFirstOriginalDeltaDays;
  NFmiMetTime itsFirstPlotTime;
  NFmiString itsStringNameTimeFormat;
  NFmiString itsSecondStringNameTimeFormat;
  bool fWriteAsUtc;
  FmiPressSeasons* itsSeasonsStatus;  // owner NFmiPressProduct
  std::map<std::string, std::string> itsReplaceMap;

 private:
};  // class NFmiPressTimeDescription

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiPressTimeDescription::~NFmiPressTimeDescription(void) {}
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressTimeDescription::NFmiPressTimeDescription(void)
    : fGivenHoursAreRelative(false),
      fIsHourLoop(false),
      itsFirstPlotHours(12),
      itsFirstPlotMinutes(0),
      itsFirstDeltaDays(+1),
      fWriteAsUtc(false),
      itsSeasonsStatus(0)
{
  itsLoopActivity.startIndex = 0;
  itsLoopActivity.bypassWithValue = false;
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theFirstPlotMinutes Undocumented
 * \param theFirstPlotHours Undocumented
 * \param theFirstDeltaDays Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressTimeDescription::NFmiPressTimeDescription(short theFirstPlotMinutes,
                                                          short theFirstPlotHours,
                                                          short theFirstDeltaDays)
{
  itsFirstPlotMinutes = theFirstPlotMinutes;
  itsFirstPlotHours = theFirstPlotHours;
  itsFirstDeltaDays = theFirstDeltaDays;
  fIsHourLoop = false;
  fWriteAsUtc = false;
  itsLoopActivity.startIndex = 0;
  itsLoopActivity.bypassWithValue = false;
  itsSeasonsStatus = 0;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressTimeDescription::SetTime(const NFmiMetTime& theTime)
{
  itsFirstPlotTime = theTime;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param isHourLoop Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressTimeDescription::SetHourLoop(bool isHourLoop)
{
  fIsHourLoop = isHourLoop;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressTimeDescription::IsHourLoop(void) const
{
  return fIsHourLoop;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theFirstPlotMinutes Undocumented
 * \param theFirstPlotHours Undocumented
 * \param theFirstDeltaDays Undocumented
 * \param theFirstOriginalDeltaDays Undocumented
 * \param theFirstPlotTime Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressTimeDescription::Set(short theFirstPlotMinutes,
                                          short theFirstPlotHours,
                                          short theFirstDeltaDays,
                                          short theFirstOriginalDeltaDays,
                                          NFmiMetTime theFirstPlotTime)
{
  itsFirstPlotMinutes = theFirstPlotMinutes;
  itsFirstPlotHours = theFirstPlotHours;
  itsFirstDeltaDays = theFirstDeltaDays;
  itsFirstOriginalDeltaDays = theFirstOriginalDeltaDays;
  itsFirstPlotTime = theFirstPlotTime;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline short NFmiPressTimeDescription::GetFirstPlotHours(void) const
{
  return itsFirstPlotHours;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline short NFmiPressTimeDescription::GetFirstPlotMinutes(void) const
{
  return itsFirstPlotMinutes;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline short NFmiPressTimeDescription::GetFirstDeltaDays(void) const
{
  return itsFirstDeltaDays;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline short NFmiPressTimeDescription::GetFirstOriginalDeltaDays(void) const
{
  return itsFirstOriginalDeltaDays;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiMetTime NFmiPressTimeDescription::GetFirstPlotTime(void) const
{
  return itsFirstPlotTime;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiPressTimeDescription::IsWriteAsUtc(void) const
{
  return fWriteAsUtc;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline FmiPressSeasons* NFmiPressTimeDescription::GetSeasonsStatus(void) const
{
  return itsSeasonsStatus;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressTimeDescription::SetSeasonsStatus(FmiPressSeasons* theSeasonsStatus)
{
  itsSeasonsStatus = theSeasonsStatus;
}

#endif  // NFMIPRESSTIMEDESCRIPTION_H

// ======================================================================
