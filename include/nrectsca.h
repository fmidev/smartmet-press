/*-------------------------------------------------------------------------------------*/

//© Ilmatieteenlaitos/Lasse

//Originaali 21.10. 1997
//Muutettu 021297/LW +GetStartStartPoint() 
//Muutettu 101297/LW SetXStartScale() ja muut
//Muutettu 090398/LW +GetXStartScale() ja muut
//Muutettu 121098/LW +UnScale().....
//Muutettu 111298/LW +Scale(NFmiRectScale)

//*-------------------------------------------------------------------------------------*/

#ifndef __NRECTSCA_H__
#define __NRECTSCA_H__

#ifndef __NSCALE_H__
#include "nscale.h"
#endif //__NSCALE_H__

#ifndef __NRECT_H__
#include "NFmiRect.h"
#endif //__NRECT_H__

//_________________________________________________________ 


class _FMI_DLL NFmiRectScale 
{
  public:

	NFmiRectScale (void);
	NFmiRectScale (NFmiRect theStartRect, NFmiRect theEndRect);
//	NFmiRectScale (const NFmiScale& anOtherScale);

	virtual ~NFmiRectScale(void);
//	virtual ostream_withassign& Write(ostream_withassign &file);
//	virtual istream_withassign& Read(istream_withassign &file);

//	NFmiScale& operator=  (const NFmiScale& anOtherScale);
//	NFmiScale& operator+= (const NFmiScale& anOtherScale);
// 	NFmiScale& operator-= (const NFmiScale& anOtherScale);

	void SetStartScales(const NFmiRect& startRect);
	void SetXStartScale(const NFmiScale& scale); //101297
	void SetYStartScale(const NFmiScale& scale);//101297
	void SetXEndScale(const NFmiScale& scale);//101297
	void SetYEndScale(const NFmiScale& scale);//101297
	void MoveXEndScale (double theDelta);      //101297
	void SetEndScales(const NFmiRect& endRect);
	void SetScales(const NFmiRect& startRect, const NFmiRect& endRect);
	double GetXScaling(void)const;
	double GetYScaling(void)const;
    NFmiRect GetStartScales (void) const;
    NFmiRect GetEndScales (void) const;
    NFmiScale GetXStartScale (void) const;
    NFmiScale GetYStartScale (void) const;
    NFmiScale GetXEndScale (void) const;
    NFmiScale GetYEndScale (void) const;
	NFmiPoint GetEndStartPoint(void) const;
	NFmiPoint GetStartStartPoint(void) const;
	NFmiPoint GetStartCenter(void) const;
	NFmiPoint GetEndCenter(void) const;
	NFmiPoint GetScaling(void) const;
	NFmiPoint Scale(const NFmiPoint& fromPoint) const;
	NFmiRectScale Scale(const NFmiRectScale& rectScale) const; //111298 kaksi rectskaalausta peräkkäin
   NFmiRect ScaleRect (const NFmiRect& fromRect) const;
   double ScaleX (double fromValue) const;
   double ScaleY (double fromValue) const;
   NFmiPoint UnScale(const NFmiPoint& fromPoint) const;
   NFmiRect UnScaleRect (const NFmiRect& fromRect) const;
   double UnScaleX (double fromValue) const;
   double UnScaleY (double fromValue) const;

protected:

private:
	NFmiScale itsXStartScale; 
	NFmiScale itsYStartScale; 
	NFmiScale itsXEndScale; 
	NFmiScale itsYEndScale; 
};


#endif // NRECTSCA_H





