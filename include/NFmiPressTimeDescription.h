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
  ~NFmiPressTimeDescription() override;

  NFmiPressTimeDescription();
  NFmiPressTimeDescription(short theFirstPlotMinutes,
                           short theFirstPlotHours,
                           short theFirstDeltaDays);
  NFmiPressTimeDescription(const NFmiPressTimeDescription&) = default;
  NFmiPressTimeDescription& operator=(const NFmiPressTimeDescription&) = default;

  bool ReadRemaining() override;
  int ConvertDefText(NFmiString& object) override;
  void SetTime(const NFmiMetTime& theTime);
  void SetHourLoop(bool isHourLoop);
  bool IsHourLoop() const;

  void Set(short theFirstPlotMinutes,
           short theFirstPlotHours,
           short theFirstDeltaDays,
           short theFirstOriginalDeltaDays,
           const NFmiMetTime& theFirstPlotTime);

  short GetFirstPlotHours() const;
  short GetFirstPlotMinutes() const;
  short GetFirstDeltaDays() const;
  short GetFirstOriginalDeltaDays() const;
  NFmiMetTime GetFirstPlotTime() const;
  bool SetDataHour(NFmiFastQueryInfo* data, const NFmiString& calledFrom);
  unsigned long ReadTimeFormat(bool isSecond = false);
  NFmiMetTime NextUseTime(long relHours, long hourRes, long hourDelta);
  NFmiMetTime TimeToWrite(const NFmiMetTime& givenTime) const;
  bool ActiveTimeIndex(int currentInd) const;
  bool IsWriteAsUtc() const;
  FmiPressSeasons* GetSeasonsStatus() const;
  int Weekday2Num(const NFmiString& theWeekday) const;

 protected:
  long JustifyByLeaps(long dayDiff);
  void SetPreReadingTimes();
  void SetPostReadingTimes();
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

  FmiLoopActivity itsLoopActivity{};
  bool fGivenHoursAreRelative;
  bool fIsHourLoop;
  short itsFirstPlotHours;
  short itsFirstPlotMinutes;
  short itsFirstDeltaDays;
  short itsFirstOriginalDeltaDays{};
  NFmiMetTime itsFirstPlotTime;
  NFmiString itsStringNameTimeFormat;
  NFmiString itsSecondStringNameTimeFormat;
  bool fWriteAsUtc;
  FmiPressSeasons* itsSeasonsStatus;  // owner NFmiPressProduct
  std::map<std::string, std::string> itsReplaceMap;

 private:
  void ReadTimeTableActive();
};  // class NFmiPressTimeDescription

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiPressTimeDescription::~NFmiPressTimeDescription() = default;
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressTimeDescription::NFmiPressTimeDescription()
    : fGivenHoursAreRelative(false),
      fIsHourLoop(false),
      itsFirstPlotHours(12),
      itsFirstPlotMinutes(0),
      itsFirstDeltaDays(+1),
      fWriteAsUtc(false),
      itsSeasonsStatus(nullptr)
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
                                                          short theFirstDeltaDays) : itsFirstPlotMinutes(theFirstPlotMinutes)
{

  itsFirstPlotHours = theFirstPlotHours;
  itsFirstDeltaDays = theFirstDeltaDays;
  fIsHourLoop = false;
  fWriteAsUtc = false;
  itsLoopActivity.startIndex = 0;
  itsLoopActivity.bypassWithValue = false;
  itsSeasonsStatus = nullptr;
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

inline bool NFmiPressTimeDescription::IsHourLoop() const
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
                                          const NFmiMetTime& theFirstPlotTime)
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

inline short NFmiPressTimeDescription::GetFirstPlotHours() const
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

inline short NFmiPressTimeDescription::GetFirstPlotMinutes() const
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

inline short NFmiPressTimeDescription::GetFirstDeltaDays() const
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

inline short NFmiPressTimeDescription::GetFirstOriginalDeltaDays() const
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

inline NFmiMetTime NFmiPressTimeDescription::GetFirstPlotTime() const
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

inline bool NFmiPressTimeDescription::IsWriteAsUtc() const
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

inline FmiPressSeasons* NFmiPressTimeDescription::GetSeasonsStatus() const
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
