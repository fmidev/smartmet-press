/*---------------------------------------------------------------------------*/ 
//� Ilmatieteenlaitos/Lasse.               nfmipressarea.h 
// 
//Originaali 16.12.2000 
// 
// Area-luokka, joka m��ritelt�viss�. Voidaan k�ytt�� asemien 
// sijoituksessa tuotteeseen. Kun area on annettu riitt�� pelk�n
// (x,y) paikan antaminen, jolloin ohjelmaa osaa ratkaista lon/latin.
// Tai p�invastoin voidaan antaa lon/lat, jolloin ohjelma sijoittaa
// oikealle paikalle tuotteessa. My�s paikan nimi kelpaa jos se on
// asematLonLat-tiedostossa. 
//
// 5.2001: Areaa voidaan my�s k�ytt�� aluekeskiarvojen, summien ym
// laskemisessa. T�ll�in ei tarvitse antaa xy-mittoja (jos ylemm�ll� tasolla 
// tiedet��n ett� t�h�n k�ytet��n)
//
// muutos 18.5.01/LW +fIsXyRequest
  
//
/*---------------------------------------------------------------------------*/
 
#ifndef __NFMIPRESSAREA_H__ 
#define __NFMIPRESSAREA_H__ 
 
//#include  "fglob.h"
#include "NFmiPressDescription.h"
//#include "NFmiArea.h"
//class NFmiArea;
#include "NFmiYKJArea.h"
#include "NFmiPolSterArea.h"
#include "NFmiStereographicArea.h"

typedef enum
{
	 dLonLatCorners = 400
	,dXYCorners
	,dWorldXYCorners
	,dProjection
	,dOrientation
	,dTrueLat      //17.8.01
}NFmiAreaObjects;
 
class _FMI_DLL NFmiPressArea : public NFmiPressDescription 
{ 
 public: 
	 NFmiPressArea (void) :
	   itsArea(0)
	  ,fIsXyRequest(true)
	   {                         
	   };
    NFmiPressArea (const NFmiPressArea& anArea);
    ~NFmiPressArea(); 
//    virtual  NFmiArea *Clone(void) const;
    virtual int             ConvertDefText(NFmiString & object);
	virtual bool		ReadDescription(NFmiString& retString);
	NFmiArea* GetArea(void) const{return itsArea;};
	void SetArea(NFmiArea* theArea) {itsArea = theArea;};
	void SetXyRequest(bool value){fIsXyRequest=value;}; //18.5.01
 private:
	NFmiArea* itsArea;
	bool fIsXyRequest;  //18.5.01
}; 
#endif //__NFMIPRESSAREA_H__
