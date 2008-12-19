// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiExtremePlaceParamRect
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiExtremePlaceParamRect.h"
#include "NFmiPressParam.h"
#include "NFmiPressProduct.h"
#include <iostream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiExtremePlaceParamRect::~NFmiExtremePlaceParamRect(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theLetterParamRect Undocumented
 */
// ----------------------------------------------------------------------

NFmiExtremePlaceParamRect::NFmiExtremePlaceParamRect(const NFmiExtremePlaceParamRect & theExtremePlaceParamRect)
  : NFmiTextParamRect(theExtremePlaceParamRect)
{  	
}

// ----------------------------------------------------------------------
/*!
 * Clone the object
 *
 * \result A new copy of this
 * \todo Should return an auto_ptr
 */
// ----------------------------------------------------------------------

NFmiParamRect * NFmiExtremePlaceParamRect::Clone(void) const
{
  return new NFmiExtremePlaceParamRect(*this);
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retString Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------


bool NFmiExtremePlaceParamRect::ReadDescription(NFmiString & retString)
{			 
  NFmiString helpString;
  NFmiValueString helpValueString;
  long long1;
  double r1,r2,r3,r4;

  itsRelRect.Set(NFmiPoint(0.,0.), NFmiPoint(1.,1.));
  itsMapping = new NFmiParamMapping;

  SetPreReadingTimes();

  itsMultiMapping = 0;
  itsModifier = kNoneModifier;
  bool relPlace = false;

  itsRelRect -= NFmiPoint(1., 1.);
  ReadNext();
	
  while(itsIntObject != 9999 || itsCommentLevel) 
	{
	  if (itsIntObject != dEndComment && itsCommentLevel)
		itsIntObject = dComment; 

	  if(itsLoopNum > itsMaxLoopNum)  
		{
		  if(itsLogFile)
			*itsLogFile << "*** ERROR: tuotetiedoston maksimipituus ylitetty #Tekstisss‰" << endl;
		  retString = itsString;
		  return false;
		}
	  itsLoopNum++;
	  switch(itsIntObject)
		{
		case dOther:	  
		  {    
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntematon sana #ƒ‰riarvoPaikassa: "
						  << static_cast<char *>(itsObject)
						  << endl;  
			ReadNext();
			break;
		  }
		case dComment:	  
		  {
			ReadNext();
			break;
		  }
		case dEndComment:	  
		  {
			ReadNext();
			break;
		  }
		case dPlaceMove:
		  {
			if (!ReadEqualChar())
			  break;
			
			if(Read2Double(r1,r2))
			{ 
			  itsRelRect += NFmiPoint(r1/c40, r2/c40);
			}
			
			ReadNext();
			break;
		  }
		case dRelPlace:
		  {
			if (!ReadEqualChar())
			  break;
			if(Read4Double(r1,r2,r3,r4))
			  {
				itsRelRect.Set(NFmiPoint(r1,r2),NFmiPoint(r3,r4));
			  }
			relPlace = true;
			ReadNext();
			break;
		  }

		case dColorValueDependent: 
		  {	
			ReadNext();
			break;
		  }
		  
		case dRelDay:
		  {
			if (!ReadEqualChar())
			  break;
			ReadLong(long1);
			//  itsFirstDeltaDays = static_cast<unsigned short>(long1+ itsEnvironment.GetDayAdvance());

			ReadNext();
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: P‰iv‰‰ ei voi asettaa #ƒ‰riarvoPaikassa"
						  << endl;  
			break;
		  }
		case dHour:
		  {
			if (!ReadEqualChar())
			  break;
			ReadLong(long1);
			if(itsLogFile)
			  *itsLogFile << "*** ERROR: Tuntia ei voi asettaa #ƒ‰riarvoPaikassa"
						  << endl;  
			
			ReadNext();
			break;
		  }

		default:
		  {
			ReadRemaining();  
			break;
		  }
		}
	} //while
	
  //flush viimeinen takaisin streamiin! Miten?
  SetPostReadingTimes();

  if(!relPlace)
	  itsRelRect.Inflate(-(c40 - GetTextSize())/(c40*2));
  
  if(fNewScaling)
	itsRelRect += NFmiPoint(1.,1.);
  Set(NFmiDataIdent(NFmiParam(itsIdentPar),240), NFmiRect(itsRelRect));

  retString = itsString;
  return true;
}


// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param object Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

/*
int NFmiExtremePlaceParamRect::ConvertDefText(NFmiString & object)
{
  NFmiString lowChar = object;
  lowChar.LowerCase();
  if    (lowChar == "#columntext"
	  || lowChar == "#palstateksti")
	  	return dColumnText;
  else
  
		return NFmiTextParamRect::ConvertDefText(object);	
}
*/

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theAbsoluteRectOfSymbolGroup Undocumented
 * \param theQI Undocumented
 * \param theDestinationFile Undocumented
 * \param theOutput Undocumented
 * \result Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiExtremePlaceParamRect::WritePS(const NFmiRect & theAbsoluteRectOfSymbolGroup,
								  NFmiFastQueryInfo * theQI,
								  ofstream & theDestinationFile,
								  FmiPressOutputMode theOutput)
{
	NFmiString str;     
    NFmiLocation* location = itsPressParam->GetOptionLocation();
	if(!location)
	{
		str = NFmiString("-");
		    //esim jos ei miss‰‰n satanut on max-paikka: -
		*itsLogFile << "   WARNING: ‰‰riarvon paikka puuttuu" << endl;
	}
	else
		str = location->GetName();

    if(itsMaxLen > 0)
		str = GetPressProduct()->CutOffString(str, itsMaxLen);

	if (fFillWithUnderscore)
	{
		unsigned long len = str.GetLen();
		if(len < itsMaxLen)
			str.FillR(itsMaxLen, '_');
	}

	return WriteCode(Construct(&str),
				theAbsoluteRectOfSymbolGroup, 
				theDestinationFile,
				NFmiString("ƒ‰riarvoasema"),
				theOutput); 	
}
