//© Ilmatieteenlaitos/Lasse.
//Original 12.1.1998

//****************************************************************
// Peritty yleisemm‰st‰ NFmiPressDescription-luokasta. T‰nne 
// lis‰tty ajank‰sittely m‰‰rittelytiedostoa luettaessa.
//****************************************************************

//Muutettu 120698/LW +dRelHour
// Muutettu 200698/LW +SetPre.. ja SetPostReadingTimes()
//                    +itsFirstPlotHours,+itsFirstDeltaDays....
// Muutettu 120898/LW +fIsHourLoop, jos lyhyt tuntiluuppi tuntia ei muuteta 
//                                  alimmilla tasoilla
// Muutettu 250199/LW karkausvuosi huomioitu
// Muutettu 080300/LW minuutit mukaan, +ReadRemaining()
// Muutettu 270300/LW +ReadTimeFormat()
// Muutettu 040500/LW +fGivenHoursAreRelative
// Muutettu 031000/LW +itsStringNameTimeFormat
// Muutettu 271000/LW +itsSecondStringNameTimeFormat
// Muutettu 080201/LW +NextUseTime()
// Muutettu 140201/LW +fWriteAsUtc
//---------------------------------------------------------------------------

#ifndef __NPRTIDES_H__
#define __NPRTIDES_H__

#include "nprdescr.h"
#include "NFmiQueryData.h"

#ifndef __NMETTIM_H__
#include "NFmiMetTime.h"
#endif

//class NFmiSuperSmartInfo;
class NFmiFastQueryInfo;

typedef enum				 
{
	 dRelDay    = 10
	,dHour
	,dMinute     
	,dRelHour            
	,dHourStep
	,dDataTime                     
	,dRelativeDataTime              
	,dGivenRelHours     
	,dWriteTimeAsUtc   //140201
	,dTimeTableActive      //2.10.01 t‰nne
	,dBypassValue   = 1010       //2.10.01  
}NFmiPressTimeDescriptionObjects;

struct FmiLoopActivity   //2.10.01 t‰nne
{
	 unsigned long startIndex;
	 unsigned long step;
	 unsigned long stopIndex;
	 bool bypassWithValue; 
	 float bypassValue;   
};

class _FMI_DLL NFmiPressTimeDescription : public NFmiPressDescription 
{
	public:
		NFmiPressTimeDescription(void)			   
			   :fGivenHoursAreRelative(kFalse) 
			   ,fIsHourLoop(kFalse)   
			   ,itsFirstPlotHours(12)
			   ,itsFirstPlotMinutes(0)
			   ,itsFirstDeltaDays(+1)
//			   ,itsFirstDeltaYears(+0) 
			   ,fWriteAsUtc(false)     //14.2.01
				{
				itsLoopActivity.startIndex = 0; //26.9.01
	            itsLoopActivity.bypassWithValue = false; //2.10.01
				};
		NFmiPressTimeDescription(short theFirstPlotMinutes
								,short theFirstPlotHours
			                    ,short theFirstDeltaDays
//								,short theFirstDeltaYears  
                                 )  
								{itsFirstPlotMinutes = theFirstPlotMinutes;
								  itsFirstPlotHours = theFirstPlotHours;
			                      itsFirstDeltaDays = theFirstDeltaDays;
//			                      itsFirstDeltaYears = theFirstDeltaYears; 
								  fIsHourLoop = kFalse;    
								  fWriteAsUtc = false;     //14.2.01
								  itsLoopActivity.startIndex = 0; //10.01
								  itsLoopActivity.bypassWithValue = false; //10.01
								};
		
		virtual ~NFmiPressTimeDescription(void){;};

		FmiBoolean ReadRemaining(void); 
        virtual int ConvertDefText(NFmiString & object);
        void SetTime (const NFmiMetTime& theTime){itsFirstPlotTime = theTime;};
        void SetHourLoop (FmiBoolean isHourLoop){fIsHourLoop = isHourLoop;}; 
        FmiBoolean IsHourLoop (void) const {return fIsHourLoop;}; 
		void Set(short theFirstPlotMinutes
				,short theFirstPlotHours 
		        ,short theFirstDeltaDays  
				,short theFirstOriginalDeltaDays
				,NFmiMetTime theFirstPlotTime)   
		{itsFirstPlotMinutes = theFirstPlotMinutes;
		 itsFirstPlotHours = theFirstPlotHours; 
	     itsFirstDeltaDays =theFirstDeltaDays;  
	     itsFirstOriginalDeltaDays= theFirstOriginalDeltaDays;
	     itsFirstPlotTime = theFirstPlotTime;};
		 short GetFirstPlotHours(void) const {return itsFirstPlotHours;};
		 short GetFirstPlotMinutes(void) const {return itsFirstPlotMinutes;};
		 short GetFirstDeltaDays(void) const {return itsFirstDeltaDays;};
		 short GetFirstOriginalDeltaDays(void) const {return itsFirstOriginalDeltaDays;};
		 NFmiMetTime GetFirstPlotTime(void) const {return itsFirstPlotTime;}; //1.7
		 FmiBoolean SetDataHour(NFmiFastQueryInfo* data, const NFmiString& calledFrom); //23.8.99 oli QD
		 unsigned long ReadTimeFormat(bool isSecond= false);  
//		 void SetFirstStation(void) {fFirstStation = kTrue;}; 
//		 void UnSetFirstStation(void) {fFirstStation = kFalse;}; 
		 NFmiMetTime NextUseTime(long relHours, long hourRes, long hourDelta); //8.2.01 
		 NFmiMetTime TimeToWrite(NFmiMetTime givenTime)const; //14.2.01
		 bool ActiveTimeIndex(int currentInd)const; //2.10.01 t‰nne
		 bool IsWriteAsUtc(void)const {return fWriteAsUtc;}; //18.4.02

	protected:
		long JustifyByLeaps(long dayDiff); 
		void SetPreReadingTimes(void); 
		void SetPostReadingTimes(void);



	protected:
	 FmiLoopActivity itsLoopActivity; //10.01
	 FmiBoolean fGivenHoursAreRelative;  
	 FmiBoolean fIsHourLoop;  
 	 short itsFirstPlotHours; 
 	 short itsFirstPlotMinutes;  
	 short itsFirstDeltaDays;  
	 short itsFirstOriginalDeltaDays;
	 NFmiMetTime itsFirstPlotTime;  
	 NFmiString itsStringNameTimeFormat; 
	 NFmiString itsSecondStringNameTimeFormat;     
	 bool fWriteAsUtc;               //14.2.01
	private: 

};


#endif // __NPRTIDES_H__
