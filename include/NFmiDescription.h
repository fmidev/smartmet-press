// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiDescription
 */
// ======================================================================
/*!
 * \class NFmiDescription
 *
 * Luokka hallitsee m‰‰rittelytiedoston 
 * lukemisen. Tiedosto on tyyppi‰ lehtituotem‰‰rittely. Jos jokin
 * muu sovellus k‰ytt‰‰ samanlaista logikkaa voi t‰t‰ k‰ytt‰‰.
 * T‰‰ll‰ on funktiot esim kokonaisluvun tai kahden
 * liukuluvun lukemiseen (virheilmoituksineen jos havaitaan ei-
 * sallittuja merkkej‰). Kaikille luokille yhteiset (kommentit)
 * m‰‰rittelyt on sijoitettu t‰nne. Myˆs lokitiedosto 
 * (johon l‰hinn‰ m‰‰rittelytiedosto generoi ilmoituksia) hallitaan
 * t‰st‰. T‰st‰ on edelleen peritty NFmiPressDescription ja 
 * NFmiPressTimeDescription-luokat. 
 * Luokat, jotka lukevat lehtituotteiden m‰‰rittelytiedostoa 
 * tulee periyty‰ n‰ist‰.
 *
 */
// ======================================================================

#ifndef NFMIDESCRIPTION_H
#define NFMIDESCRIPTION_H

#include "NFmiValueString.h"
#include <cstdlib>

using namespace std;

const unsigned int kNFmiNone				= 10;
const unsigned int kNFmiPressStationText	= 11;
const unsigned int kNFmiPressParamName		= 12;
const unsigned int kNFmiPressDataTimeText	= 13;
const unsigned int kNFmiPressImage			= 14;

//! Undocumented
enum NFmiDescriptionObjects
{
  dComment      = 8880,	//ei pienempi‰ t‰nne	  
  dEndComment,
  dOther        = 8888,
  dEnd          = 9999,
  dFile
};

//! Undocumented
class _FMI_DLL NFmiDescription 
{

public:

  virtual ~NFmiDescription(void);
  NFmiDescription(void);
  NFmiDescription(std::ofstream * theLogFile, unsigned short theMaxLoopNum);
  NFmiDescription(const NFmiDescription & theD);

  void SetLogFile(std::ofstream * logFile)   ;
  void SetDescriptionFile(std::ifstream* descFile);

  virtual bool ReadDescription(NFmiString & retString) = 0; 
  bool ReadMaxTwoFloatToDelimiter(float & retValue1, float & retValue2); 
  bool ReadDouble(double & retValue);
  bool Read4Double(double & retValue1, double & retValue2, double & retValue3, double & retValue4);
  bool Read2Double(double & retValue1, double & retValue2);
  NFmiString ReadString(void); 
  bool ReadLong(long & retValue, bool errorReport=true);
  bool ReadUnsignedLong(unsigned long & retValue); 
  bool ReadEqualChar(void); 
  void ReadNext(void); 
  bool Set1Double(double & theMember); 
  bool Set1Long(long & theMember); 
  bool Set1UnsignedLong(unsigned long & theMember);
  void SetTrue(bool& theBool);

  unsigned short GetMaxLoopNum(void) const;
  std::ofstream * GetLogFile (void) const;
		
  virtual int ConvertDefText(NFmiString & object);

  virtual unsigned long ClassId(void);

  // template-funtioista joutuu laittamaan myˆs koodin t‰nne h-fileen,
  // muuten linkkeri ei tykk‰‰
  //
  // HUOM! Voi olla, ett‰ m‰‰ritelmien edess‰ luokan ulkopuolella
  // pit‰isi olla inline, selvinnee myˆhemmin linkatessa p‰‰ohjelma.

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type>
bool ReadOne(Type & retValue)
{
  *itsDescriptionFile >> itsObject;
  itsValueHelpString = itsObject;
  if(itsValueHelpString.IsNumeric())
	{
	  // valueStringill‰ tulisi olla cast longiin ja uns.longiin
	  retValue = static_cast<Type>(static_cast<double>(itsValueHelpString));
	  return isTrue;
	}
  retValue = 0;
  *itsLogFile << "*** ERROR: pit‰‰ olla luku: " << static_cast<char *>(itsValueHelpString) << endl;
  return isFalse;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue1 Undocumented
 *†\param retValue2 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type1, class Type2>
bool ReadTwo(Type1 & retValue1, Type2 & retValue2)
{
  if((ReadOne(retValue1)) && ReadOne(retValue2))
	{
	  return true;
	}
  return false;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue1 Undocumented
 *†\param retValue2 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type1, class Type2>
bool ReadOneStringAndTwoNumbers(NFmiString& theString, Type1 & retValue1, Type2 & retValue2)

{
  *itsDescriptionFile >> itsObject;
  itsValueHelpString = itsObject;
  if(itsValueHelpString.IsNumeric())
	{
	  retValue1 = static_cast<int>(itsValueHelpString);
	  theString = NFmiString();
	  return ReadOne(retValue2);
	}
  else
	{
	  theString = itsValueHelpString;
	  return ReadTwo(retValue1, retValue2);
	}
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type>
bool SetOne(Type & theMember)
{
  bool ok = false;
  Type num;
  if(ReadEqualChar())
	{
	  if(ReadOne(num))
		{
		  theMember = num;
		  ok = true;
		}
	  ReadNext(); //ep‰onnistuneessa ReadEqualChar:ssa on jo
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember1 Undocumented
 * \param theMember2 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type1, class Type2>
bool SetTwo(Type1 & theMember1,
							 Type2 & theMember2)
{
  bool ok = false;
  if (ReadEqualChar())
	{
	  Type1 num1;
	  Type2 num2;
	  // ei suoraan parametriin koska saatetaan lukea roskaa
	  // ja parametriss‰ on ennest‰‰n j‰rkev‰ oletusarvo
	  if(ReadTwo(num1, num2))
		{
		  theMember1 = num1;
		  theMember2 = num2;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember1 Undocumented
 * \param theMember2 Undocumented
 * \param theMember3 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type1, class Type2, class Type3>
bool SetThree(Type1 & theMember1,
							   Type2 & theMember2,
							   Type3 & theMember3)
{
  bool ok = false;
  if (ReadEqualChar())
	{
	  Type1 num1;
	  Type2 num2;
	  Type3 num3;
	  if(ReadTwo(num1, num2) && ReadOne(num3)) 					             
		{
		  theMember1 = num1;
		  theMember2 = num2;
		  theMember3 = num3;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember1 Undocumented
 * \param theMember2 Undocumented
 * \param theMember3 Undocumented
 * \param theMember4 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type1, class Type2, class Type3, class Type4>
bool SetFour(Type1 & theMember1,
							  Type2 & theMember2,
							  Type3 & theMember3,
							  Type4 & theMember4)
{
  bool ok = false;
  if (ReadEqualChar())
	{
	  Type1 num1;
	  Type2 num2;
	  Type3 num3;
	  Type4 num4;
	  if(ReadTwo(num1, num2) && ReadTwo(num3, num4)) 					             
		{
		  theMember1 = num1;
		  theMember2 = num2;
		  theMember3 = num3;
		  theMember4 = num4;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember1 Undocumented
 * \param theMember2 Undocumented
 * \param theMember3 Undocumented
 * \param theMember4 Undocumented
 * \param theMember5 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type1, class Type2, class Type3, class Type4, class Type5>
bool SetFive(Type1 & theMember1,
							  Type2 & theMember2,
							  Type3 & theMember3,
							  Type4 & theMember4,
							  Type5 & theMember5)
{
  bool ok = false;
  if (ReadEqualChar())
	{
	  Type1 num1;
	  Type2 num2;
	  Type3 num3;
	  Type4 num4;
	  Type5 num5;
	  if(ReadTwo(num1, num2) && ReadTwo(num3, num4) && ReadOne(num5)) 					             
		{
		  theMember1 = num1;
		  theMember2 = num2;
		  theMember3 = num3;
		  theMember4 = num4;
		  theMember5 = num5;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theMember1 Undocumented
 * \param theMember2 Undocumented
 * \param theMember3 Undocumented
 * \param theMember4 Undocumented
 * \param theMember5 Undocumented
 * \param theMember6 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------

template <class Type1, class Type2, class Type3, class Type4, class Type5, class Type6>
bool SetSix(Type1 & theMember1, Type2 & theMember2,
							 Type3 & theMember3, Type4 & theMember4,
							 Type5 & theMember5, Type6 & theMember6)
{
  bool ok = false;
  if (ReadEqualChar())
	{
	  Type1 num1;
	  Type2 num2;
	  Type3 num3;
	  Type4 num4;
	  Type5 num5;
	  Type6 num6;
	  if(ReadTwo(num1, num2) && ReadTwo(num3, num4) && ReadTwo(num5, num6)) 					             
		{
		  theMember1 = num1;
		  theMember2 = num2;
		  theMember3 = num3;
		  theMember4 = num4;
		  theMember5 = num5;
		  theMember6 = num6;
		  ok = true;
		}
	  ReadNext();
	}
  return ok;
}
/*
  template<class Type>
  bool ReadOne(Type & retValue);

  template <class Type1, class Type2>
  bool ReadTwo (Type1 & retValue1, Type2 & retValue2);

  template <class Type>
  bool SetOne(Type& theMember);

  template <class Type1, class Type2>
  bool SetTwo(Type1& theMember1, Type2& theMember2);

  template <class Type1, class Type2, class Type3>
  bool SetThree(Type1 & theMember1, Type2 & theMember2, Type3 & theMember3);

  template <class Type1, class Type2, class Type3, class Type4>
  bool SetFour(Type1 & theMember1, Type2 & theMember2,
			   Type3 & theMember3, Type4 & theMember4);

  template <class Type1, class Type2, class Type3, class Type4, class Type5> //10.2.00
  bool SetFive(Type1 & theMember1, Type2 & theMember2,
			   Type3 & theMember3, Type4 & theMember4, Type5 & theMember5);

  template <class Type1, class Type2, class Type3, class Type4, class Type5, class Type6> //3.11.00
  bool SetSix (Type1 & theMember1, Type2 & theMember2,
			   Type3 & theMember3, Type4 & theMember4,
			   Type5 & theMember5, Type6 & theMember6);
*/
protected:

  std::ofstream * itsLogFile;       
  std::ifstream * itsDescriptionFile;
  unsigned short itsLoopNum; 
  unsigned short itsMaxLoopNum; 
  char itsObject[255];
  int itsIntObject;    
  NFmiValueString itsValueHelpString;
  NFmiString itsString;      
  unsigned short itsCommentLevel;	// Vain olion sis‰ll‰ tietysti


}; // class NFmiDescription

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline
NFmiDescription::~NFmiDescription(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline
NFmiDescription::NFmiDescription(void)
{
  itsCommentLevel=0;
  itsLoopNum = 0;
  itsMaxLoopNum = 4000;
  itsLogFile = 0;
  itsDescriptionFile = 0;
}

// ----------------------------------------------------------------------
/*!
 * Constructor
 *
 * \param theLogFile Undocumented
 * \param theMaxLoopNum Undocumenetd
 */
// ----------------------------------------------------------------------

inline
NFmiDescription::NFmiDescription(std::ofstream * theLogFile, 
								 unsigned short theMaxLoopNum)
{
  itsMaxLoopNum = theMaxLoopNum ;
  itsLogFile = theLogFile;
  itsDescriptionFile = 0;
  itsLoopNum = 0;
  itsCommentLevel=0;
}

// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 *†\param theD The description being copied
 */
// ----------------------------------------------------------------------

inline
NFmiDescription::NFmiDescription(const NFmiDescription & theD)
{
  itsLogFile = theD.itsLogFile;
  itsDescriptionFile = theD.itsDescriptionFile;
  itsCommentLevel=theD.itsCommentLevel;
  itsLoopNum = theD.itsLoopNum;
  itsMaxLoopNum = theD.itsMaxLoopNum;
}

  // ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param retValue1 Undocumented
 *†\param retValue2 Undocumented
 * \return Undocumented
 */
// ----------------------------------------------------------------------
inline
void NFmiDescription::SetTrue(bool& theBool) 
{
	theBool = true;	
	ReadNext(); 
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param logFile Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiDescription::SetLogFile(std::ofstream * logFile)   
{
  itsLogFile = logFile;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param descFile Undocumented
 */
// ----------------------------------------------------------------------

inline
void NFmiDescription::SetDescriptionFile(std::ifstream * descFile)   
{
  itsDescriptionFile = descFile;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
unsigned short NFmiDescription::GetMaxLoopNum(void) const
{
  return itsMaxLoopNum;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
std::ofstream * NFmiDescription::GetLogFile(void) const
{
  return itsLogFile;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline
unsigned long NFmiDescription::ClassId(void)
{
  return kNFmiNone;
}


#endif // NFMIDESCRIPTION_H

// ======================================================================

