// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiStationPoint
 */
// ======================================================================
/*!
 * \class NFmiStationPoint
 *
 * Luokka jossa yhdistetty NFmiStation ja NFmiPoint (j�sen).
 * Point:ia k�ytet��n sijoittamaan asema esim Illun
 * koordinaattiavaruuteen. Peritty my�s NFmiDescriptionista
 * (pit�isi olla PressDescription) mutta t�m� perint� on sovelluksen
 * nykymuodossa viraton koska luokka ei itse lue lehtituotteiden
 * m��rittelyteksti�. Asemat voidaan antaa yhdell� m��rittelyrivill�
 * luokissa jossa ne on j�seni�.
 *
 */
// ======================================================================

#ifndef NFMISTATIONPOINT_H
#define NFMISTATIONPOINT_H

#include "NFmiDescription.h"
#include "NFmiMetTime.h"
#include "NFmiStation.h"

// perint� NFmiPoint:sta ei l�htenyt toimimaan, debuggeri ei osannut n�ytt��
// pointia kun sen perint� savebase:sta tehtiin virtuaaliseksi ja t�ss�
// kirjoitettu copy-konstruktori kaatoi ohjelman, sensijaan ilman
// copy-konstruktoria ohjelma ajautui, ehk� oikein.
//
// 061197 perinn�n sijaan NFmiPoint otettu j�seneksi
// tehty ja copykonstruktori kaataa edelleen!
// debuggeri alkoi n�ytt�� oikein vasta kun NFmiPointin perint�
// NFmisaveBase:sta muutettiin takaisin ei-virtuaaliseksi

// Undocumented
enum NFmiStationPointObjects
{
  dLonLat = 1,
  dLatitude,
  dLongitude,
  dPrintName,
  dCoordinates,
  dIdent
};

//! Undocumented
class NFmiStationPoint : public NFmiStation, public NFmiDescription
{
 public:
  virtual ~NFmiStationPoint(void);
  NFmiStationPoint(void);
  NFmiStationPoint(const NFmiStation &theStation, const NFmiPoint &thePoint);
  NFmiStationPoint(const NFmiStationPoint &theStation);

  NFmiStationPoint &operator=(const NFmiStationPoint &theStation);
  virtual bool ReadDescription(NFmiString &retString);
  int ConvertDefText(NFmiString &object);

  virtual NFmiLocation *Clone(void) const;
  NFmiString LocalWmoTime(int utc) const;
  bool LocalWmoTime(NFmiTime &utc) const;
  bool LocalTime(NFmiTime &utc, int &errorCode);

  double X(void) const;
  double Y(void) const;

  //  Ei hyv�, eih�n se s�ily kun ei tehd� new:ll�
  NFmiStation *Station(void) const;
  NFmiPoint Point(void) const;
  void Station(NFmiStation theStation);
  void Location(NFmiLocation theLocation);
  void Point(NFmiPoint thePoint);
  bool IsBackup(void) const;
  void SetBackup(bool theStatus);

  virtual std::ostream &Write(std::ostream &file) const;
  virtual std::istream &Read(std::istream &file);
  void SetAlternatingSizeFactor(float theSizeFactor);
  float GetAlternatingSizeFactor(void) const;

 protected:
  float GetWmoDifference(void) const;

 private:
  NFmiPoint itsPoint;
  bool fIsBackup;
  float itsAlternatingSizeFactor;

};  // class NFmiStationPoint

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiStationPoint::~NFmiStationPoint(void) {}
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiStationPoint::NFmiStationPoint(void)
    : NFmiStation(2111, "Helsinki"),
      itsPoint(0., 0.),
      fIsBackup(false),
      itsAlternatingSizeFactor(-1.)
{
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theStation Undocumented
 * \param thePoint Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiStationPoint::NFmiStationPoint(const NFmiStation &theStation, const NFmiPoint &thePoint)
    : NFmiStation(theStation), itsPoint(thePoint), fIsBackup(false), itsAlternatingSizeFactor(-1.)
{
}
// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theStation The object being copied
 */
// ----------------------------------------------------------------------

inline NFmiStationPoint::NFmiStationPoint(const NFmiStationPoint &theStation)
    : NFmiStation(*theStation.Station()),
      NFmiDescription(),
      itsPoint(theStation.Point()),
      fIsBackup(theStation.fIsBackup),
      itsAlternatingSizeFactor(theStation.GetAlternatingSizeFactor())
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiStation *NFmiStationPoint::Station(void) const
{
  return static_cast<NFmiStation *>(
      NFmiStation(GetIdent(), GetName(), GetLongitude(), GetLatitude()).Clone());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPoint NFmiStationPoint::Point(void) const
{
  return itsPoint;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiStationPoint::SetAlternatingSizeFactor(float theSizeFactor)
{
  itsAlternatingSizeFactor = theSizeFactor;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline float NFmiStationPoint::GetAlternatingSizeFactor(void) const
{
  return itsAlternatingSizeFactor;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiStationPoint::SetBackup(bool theStatus)
{
  fIsBackup = theStatus;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline bool NFmiStationPoint::IsBackup(void) const
{
  return fIsBackup;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiStationPoint::X(void) const
{
  return itsPoint.X();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline double NFmiStationPoint::Y(void) const
{
  return itsPoint.Y();
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theStation Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiStationPoint::Station(NFmiStation theStation)
{
  SetIdent(theStation.GetIdent());
  SetName(theStation.GetName());
  SetLatitude(theStation.GetLatitude());
  SetLongitude(theStation.GetLongitude());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theLocation Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiStationPoint::Location(NFmiLocation theLocation)
{
  SetLatitude(theLocation.GetLatitude());
  SetLongitude(theLocation.GetLongitude());
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param thePoint Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiStationPoint::Point(NFmiPoint thePoint)
{
  itsPoint = thePoint;
}
#endif  // NFMISTATIONPOINT_H

// ======================================================================
