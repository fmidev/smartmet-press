/*-------------------------------------------------------------------------------------*/

//© Ilmatieteenlaitos/Lasse

//Originaali 2. 12.1994

//Muutettu   xx.12.1994/LW
//Muutettu   10.11.1995/LW  Expand()
//Muutettu    7.12.1995/LW  Operator +=
//Muutettu   21.12.1995/LW  Ok
//Muutettu   22.12.1995/LW  itsEpsilon
//Muutettu   16.1.1996/LW  Operator -=
// 041196/LW +IsMissing()
// 061196/LW Difference publiciksi
// 241097/LW +CenterValue()
// 020501/LW +ExcludePositive() (Negative)
//*-------------------------------------------------------------------------------------*/

#ifndef __NSCALE_H__
#define __NSCALE_H__

#include "NFmiSaveBase.h"
#include "NFmiGlobals.h"

//_________________________________________________________ NFmiScale


class _FMI_DLL NFmiScale : public NFmiSaveBase
{
  public:

	NFmiScale (void);
	NFmiScale (float theStartValue, float theEndValue);
	NFmiScale (const NFmiScale& anOtherScale);

	virtual ~NFmiScale(void);
//	virtual std::ostream& Write(std::ostream &file);
//	virtual std::istream& Read(std::istream &file);
	NFmiScale& operator=  (const NFmiScale& anOtherScale);
	NFmiScale& operator+= (const NFmiScale& anOtherScale);
 	NFmiScale& operator-= (const NFmiScale& anOtherScale);

		   float StartValue     (void) const;
		   float EndValue       (void) const;
			double CenterValue    (void) const {return (itsStartValue+itsEndValue)/2.;};			//241097/LW
		    void SetStartValue  (float theValue);		  
		    void SetEndValue    (float theValue);
		    void Set            (float theStartValue, float theEndValue);
		   float RelLocation    (float theValue) const;
		   float Location       (float theRelValue) const;	   
	  bool Inside         (float theValue) const;
    virtual void StartFromZeroOptionally(float theFactor=1.);
    virtual void ExpandIfNotZero(float theFactor=.05);
    virtual void ExpandIfEqual  (float theAdditionToBothEnds=1.);
    virtual void Expand         (float theAdditionToBothEnds=1.);
			void LimitCheck (bool theCheck) {itsLimitCheck=theCheck;};
    virtual void ExcludePositive (void);    //2.5.01
    virtual void ExcludeNegative (void);    //2.5.01
      bool Ok         (void)const {return itsDataOk;};
			float Difference (void) const;//061196/LW 
/*
	void     MoveFromNearest(const float newValue);
	void     MoveNearest(const float newValue);
	void     MoveStart(const float newValue);
	void     MoveEnd(const float newValue);
*/
	protected:

//			float Difference (void) const;
			 void Check      (void);
	   bool IsMissing	 (void) const;

		float itsStartValue;
		float itsEndValue;
		float itsEpsilon;
 bool itsDataOk;
 bool itsLimitCheck;

private:

};

//______________________________________________________________________________
//______________________________________________________________________________
inline
float NFmiScale :: StartValue (void) const
{
	return itsStartValue;
}
//______________________________________________________________________________
inline
float NFmiScale :: EndValue (void) const
{
	return itsEndValue;
}
//______________________________________________________________________________
inline
void NFmiScale :: SetStartValue (float theValue)
{
	itsStartValue = theValue;
	Check();
}
//______________________________________________________________________________
inline
void NFmiScale :: SetEndValue (float theValue)
{
	itsEndValue = theValue;
	Check();
}
//______________________________________________________________________________
inline
void NFmiScale :: Set(float theStartValue, float theEndValue)
{
	itsStartValue = theStartValue;
	itsEndValue = theEndValue;
	Check();
}
//______________________________________________________________________________
inline
float NFmiScale :: Difference (void) const
{
  return itsEndValue - itsStartValue;
}


#endif // NSCALE_H





