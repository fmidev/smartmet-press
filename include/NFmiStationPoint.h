/*-------------------------------------------------------------------------------------*/
//© Ilmatieteenlaitos/Lasse.               

//--------------------------------------------------------------------
// Luokka jossa yhdistetty NFmiStation ja NFmiPoint (jäsen). 
// Point:ia käytetään sijoittamaan asema esim Illun koordinaatti-
// avaruuteen.
// Peritty myös NFmiDescriptionista (pitäisi olla PressDescription)
// mutta tämä perintä on sovelluksen nykymuodossa viraton
// koska luokka ei itse lue lehtituotteiden määrittelytekstiä. Asemat
// voidaan antaa yhdellä määrittelyrivillä luokissa jossa ne on jäseniä.
//--------------------------------------------------------------------- 
// 
//Originaali 10. 10.1997
//Muutettu 121297/LW peritty myös NFmiDescription:sta
//Muutettu 160198/LW 
//Muutettu 231098/LW +LocalTime()
//Muutettu 150699/LW lat/lon järjestys korjattu
//Muutettu 280100/LW LocalTime() myös MetTimenä.
//Muutettu xx0300/LW aurinkoajat vaatineet muutoksia
/*-------------------------------------------------------------------------------------*/


#ifndef  __NSTATPNT_H__
#define  __NSTATPNT_H__

#ifndef __NSTATION_H__
#include "NFmiStation.h"
#endif//__NSTATION_H__

#ifndef __NDESCRIP_H__
#include "NFmiDescription.h"
#endif//__NDESCRIP_H__

#include "NFmiMetTime.h"
/*
#ifndef __NPOINT_H__
#include "NFmiPoint.h"
#endif//__NPOINT_H__
*/
// perintä NFmiPoint:sta ei lähtenyt toimimaan, debuggeri ei osannut näyttää
// pointia kun sen perintä savebase:sta tehtiin virtuaaliseksi ja tässä 
// kirjoitettu copy-konstruktori kaatoi ohjelman, sensijaan ilman copy-konstruktoria 
// ohjelma ajautui, ehkä oikein.
// 061197 perinnän sijaan NFmiPoint otettu jäseneksi
// tehty ja copykonstruktori kaataa edelleen!
// debuggeri alkoi näyttää oikein vasta kun NFmiPointin perintä NFmisaveBase:sta muutettiin 
// takaisin ei-virtuaaliseksi

typedef enum
{
	 dLonLat			=1 
	,dLatitude
	,dLongitude
	,dPrintName
	,dCoordinates
	,dIdent

}NFmiStationPointObjects;
										
class _FMI_DLL NFmiStationPoint : public NFmiStation , public  NFmiDescription
{

 public:

	NFmiStationPoint (void)  :  NFmiStation(2111, "Helsinki")
					   ,itsPoint(0.,0.)
	                    {};
	NFmiStationPoint (const NFmiStation& theStation, const NFmiPoint& thePoint)
		              :NFmiStation(theStation)
						  ,itsPoint(thePoint)
	                    {};
 
	NFmiStationPoint (const NFmiStationPoint &theStation) 
				: NFmiStation(*theStation.Station())
				, NFmiDescription()
				, itsPoint(theStation.Point())				
	           {};
	
	virtual ~NFmiStationPoint (void) {};

	NFmiStationPoint	&operator= (const NFmiStationPoint &theStation);
	virtual bool ReadDescription(NFmiString& retString); //16.1 
//	virtual bool IsEqual(const NFmiLocation & theLocation) const;
    int    ConvertDefText(NFmiString & object);

	virtual NFmiLocation* Clone(void) const; 
	NFmiString LocalWmoTime(int utc) const;     //23.10
	bool LocalWmoTime(NFmiTime& utc) const; //28.1.00
	bool LocalTime(NFmiTime& utc, int& errorCode) ; //2.3.00

	double X(void) const {return itsPoint.X();};
	double Y(void) const {return itsPoint.Y();};

//  Ei hyvä, eihän se säily kun ei tehdä new:llä
	NFmiStation* Station(void) const{return (NFmiStation*)NFmiStation(GetIdent()
		                                               ,GetName()
																	  ,GetLongitude() //15.6.99 lon/lat oli väärin päin
																	  ,GetLatitude()).Clone();};
/* Ei onnistu, virtuaali per.??? (ylhäällä copy-konstr.:sta oli * myös pois)
	NFmiStation Station(void) const{return NFmiStation(GetIdent()
		                                               ,GetName()
																	  ,GetLongitude()
																     ,GetLatitude());};
*/	
	NFmiPoint Point(void) const {return itsPoint;};

	void Station(NFmiStation theStation){SetIdent(theStation.GetIdent());
	                                     SetName(theStation.GetName());
	                                     SetLatitude(theStation.GetLatitude());
													 SetLongitude(theStation.GetLongitude());};
 
	void Location(NFmiLocation theLocation){SetLatitude(theLocation.GetLatitude());
													    SetLongitude(theLocation.GetLongitude());};

//	void Point(NFmiPoint thePoint){NFmiPoint::Set(thePoint.X(),thePoint.Y());};
	void Point(NFmiPoint thePoint){itsPoint = thePoint;};

//	virtual ostream_withassign &Write(ostream_withassign &file); 
//	virtual istream_withassign &Read(istream_withassign &file); 
	virtual std::ostream &Write(std::ostream &file); 
	virtual std::istream &Read(std::istream &file); 

 protected:
	float GetWmoDifference(void)const;

 private:
	 NFmiPoint itsPoint;
	 
};


#endif// __NSTATPNT_H__
