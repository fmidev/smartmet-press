/*---------------------------------------------------------------------------*/ 
//� Ilmatieteenlaitos/Lasse.               
// 
//Originaali 6.3.1998 
// 
// Joitakin lis�yksi� tekstiksi-koodaukseen
//
//Muutettu xx0800/LW Ruotsin kielest� pois kuukauden j�lkeinen piste
//Muutettu xx1000/LW +InterpretToStr()
//Muutettu 291100/LW bugi korj.
//Muutettu 050101/LW RelativeDay() =eilen,t�n��n.....
//Muutettu 080801/LW strx bugit korjattu
//Muutettu 140502/LW + www -formaatit
/*---------------------------------------------------------------------------*/

 #include  "npretime.h"
 #include  "nhyphstr.h" //13.10.00
 #include  "NFmiValueString.h" //27.08.01
 


//__________________________________________________________________ 
NFmiString NFmiPressTime :: ToStr(const unsigned long theTimeMask  
								, const FmiLanguage theLanguage) const   //19.3
{                   
  NFmiValueString theString;
  
  if(kI & theTimeMask) // 15.3.00
  {
	   theString += NFmiValueString(GetWeekday() ,"%01d");
	   return theString;
  }

  if(kLongYear & theTimeMask && kDot & theTimeMask) // 190299
  {
	  if(kDay & theTimeMask)
		  theString += NFmiValueString(GetDay() ,"%02d");
	  else if (kShortDay & theTimeMask)
		  theString += NFmiValueString(GetDay() ,"%d");

      theString += NFmiValueString(GetDay() ,"."); 

	  if(kMonth & theTimeMask)
		  theString += NFmiValueString(GetMonth() ,"%02d");
	  else if (kShortMonth & theTimeMask)
		  theString += NFmiValueString(GetMonth() ,"%d");
 
	  if(theLanguage != kSwedish) //24.8.00
		theString += NFmiValueString(GetDay() ,"."); 


      theString += NFmiValueString(GetYear() ,"%04d");

	  return theString;
  }


  if(kLongMonthName & theTimeMask)
    theString += MonthName(theLanguage);   //19.3
  else if(kShortMonthName & theTimeMask)
    theString += MonthName(theLanguage).GetChars(1,3);   //1.4

  if(kShortWeekDay & theTimeMask)
    theString += Weekday(theLanguage).GetChars(1,2);   //19.3
  else if(kLongWeekDay & theTimeMask)
    theString += Weekday(theLanguage);
 
  if(kSpace & theTimeMask)
    theString += NFmiString(" ");

//  theString += NFmiStaticTime::ToStr(theTimeMask);
// HUOM p�iv�m��r�muuttujien j�rjestys vaihdettu �itiin verrattuna
  
  if(kDay & theTimeMask)
  {
     theString += NFmiValueString(GetDay() ,"%02d");
     if(kDot & theTimeMask)                         //7.4 t�nne
        theString += NFmiValueString(GetDay() ,"."); 
  }
  if(kShortDay & theTimeMask)
  {
     theString += NFmiValueString(GetDay() ,"%d");
     if(kDot & theTimeMask)                        //7.4 t�nne
        theString += NFmiValueString(GetDay() ,"."); 
  }
 
  if(kMonth & theTimeMask)
  {
     theString += NFmiValueString(GetMonth() ,"%02d");
     if(kDot & theTimeMask 	&& theLanguage != kSwedish) //29.8.00
       theString += NFmiValueString(GetDay() ,".");
  }
  if(kShortMonth & theTimeMask)
  {
     theString += NFmiValueString(GetMonth() ,"%d");
     if(kDot & theTimeMask && theLanguage != kSwedish) //29.8.00)                          
       theString += NFmiValueString(GetDay() ,".");
   
  }
  if(kShortYear & theTimeMask)
    theString += NFmiValueString(GetYear() ,"%02d").GetChars(3,2);
  else
    if(kLongYear & theTimeMask)
      theString += NFmiValueString(GetYear() ,"%04d");

 
  if(kHour & theTimeMask)
  {
     theString += NFmiValueString(GetHour() ,"%02d");
     if(kDot & theTimeMask)
       theString += NFmiValueString(GetDay() ,".");     //7.4
  }
  if(kMinute & theTimeMask)
    theString += NFmiValueString(GetMin() ,"%02d");

  if(kSecond & theTimeMask)
    theString += NFmiValueString(GetSec() ,"%02d");

  if(kShortHour & theTimeMask)                     //1.3.2000
  {
	theString += NFmiValueString(GetHour() ,"%d");      // kH

	if (kDot & theTimeMask)
       theString += NFmiValueString(GetDay() ,".");     

	if(kMinute & theTimeMask)
       theString += NFmiValueString(GetMin() ,"%02d");   //kHdMM
 }

  return theString;
} 

//13.10.00__________________________________________________________________ 
NFmiString NFmiPressTime :: InterpretToStr(const NFmiString theTimeCode  
								, const FmiLanguage theLanguage) const
{
 /* sallittuja osasia:
		YYYY ->	        1999
		YY		        99
		MM		        08
		M		        8
		DD		        05
		D		        5
		HH		        15
		mm		        30  (huom pienet, isot =kuukausi)
		Wwww wwww WWWW	Perjantai perjantai PERJANTAI
		Ww ww WW		Pe pe PE
		Nnnn nnnn NNNN	Maaliskuu maaliskuu MAALISKUU
		Nnn nnn NNN		Maa maa MAA
		Tttt tttt TTTT  Eilen t�n��n HUOMENNA jne (m�ys Tttt+1, tttt-2 jne)
		I		        2 (tiistai) (kiinan viikonp�iv�t)
		ja melkein mit� tahansa merkkej� sellaisinaan
			esim v�lil : . + - ja ne jotka eiv�t edell� varattuja
			mutta ne varatutkin saadaan sijoittamalla ne kenoviivojen v�liin
		esim: 
		H.mm ->	9.32
		"D.M.YY HH:mm" -> 13.2.99 14:32
		"DD\HH\"       ->  13HH
		(en�� eiv�t d ja s kelpaa vanhassa merkityksess��n)
		YYMMDD -> 001013
		osaset vapaassa j�rjestyksess�
		jatkoaikatekstien tarve v�henee
		tiedostonimiin ei sovi laittaa mit� tahansa merkkej� (aikaleima)
		*/
  bool noConversion = false;
  int ind = 1;
  int len = theTimeCode.GetLen();
  NFmiString str6, str4, str3, str2, str1, strh;
  NFmiValueString theString;
  int plusInd, subPlusInd;

  while(ind <= len)
  {
	  str1 = theTimeCode.GetChars(ind,1);
//	  if(str1 == NFmiString(kFmiDirectorySeparator))  //my�s formaatille varatut merkit saadaan kun laitetaan ne kenojen v�liin
// Mikan korjaus ei k�y koska ei kyse polusta
	  if(str1 == NFmiString("\\"))  //my�s formaatille varatut merkit saadaan kun laitetaan ne kenojen v�liin
	  {
		noConversion = !noConversion;
		ind++;
		continue;
	  }
	  else if (noConversion)
	  {
		theString += str1;
		ind++;
		continue;
	  }

	  str4 = theTimeCode.GetChars(ind,4);
	  plusInd = 4;
	  if(str4 == NFmiString("YYYY")|| str4 == NFmiString("yyyy"))
			theString += NFmiValueString(GetYear() ,"%04d");
	  else if(str4 == NFmiString("Wwww"))
			theString += Weekday(theLanguage);
	  else if(str4 == NFmiString("wwww"))
	  {
		    strh = Weekday(theLanguage);
			strh.LowerCase();
			theString += strh;
	  }
	  else if(str4 == NFmiString("WWWW"))
	  {
		    strh = Weekday(theLanguage);
			strh.UpperCase();
			theString += strh;
	  }
	  else if(str4 == NFmiString("Nnnn"))
			theString += MonthName(theLanguage);   
	  else if(str4 == NFmiString("nnnn"))
	  {
		    strh = MonthName(theLanguage);
			strh.LowerCase();
			theString += strh;
	  }
	  else if(str4 == NFmiString("NNNN"))
	  {
		    strh = MonthName(theLanguage);
			strh.UpperCase();
			theString += strh;
	  }
	  else if(str4 == NFmiString("Tttt") || str4 == NFmiString("tttt") || str4 == NFmiString("TTTT"))
	  {
		  	str6 = theTimeCode.GetChars(ind,6);
			theString += RelativeDay(theLanguage, str6, subPlusInd);
			plusInd += subPlusInd;
	  }
	  else
	  {
		str3 = theTimeCode.GetChars(ind,3);
		plusInd += -1;
		if(str3 == NFmiString("Nnn"))
			theString += MonthName(theLanguage).GetChars(1,3);   
		else if(str3 == NFmiString("nnn"))
		{
				strh = MonthName(theLanguage).GetChars(1,3);
				strh.LowerCase();
				theString += strh;
		}
		else if(str3 == NFmiString("NNN"))
		{
				strh = MonthName(theLanguage).GetChars(1,3);
				strh.UpperCase();
				theString += strh;
		}
		else if(str3 == NFmiString("Www"))   //n�m� kolme 14.5.02
			theString += Weekday(theLanguage).GetChars(1,3);   
		else if(str3 == NFmiString("www"))
		{
				strh = Weekday(theLanguage).GetChars(1,3);
				strh.LowerCase();
				theString += strh;
		}
		else if(str3 == NFmiString("WWW"))
		{
				strh = Weekday(theLanguage).GetChars(1,3);
				strh.UpperCase();
				theString += strh;
		}
		else
		{
			str2 = theTimeCode.GetChars(ind,2);
			plusInd += -1;
			if(str2 == NFmiString("YY") || str2 == NFmiString("yy"))
				theString += NFmiValueString(GetYear() ,"%02d").GetChars(3,2);
			else if(str2 == NFmiString("MM"))
				theString += NFmiValueString(GetMonth() ,"%02d");
			else if(str2 == NFmiString("DD")||str2 == NFmiString("dd"))
				theString += NFmiValueString(GetDay() ,"%02d");
			else if(str2 == NFmiString("HH") || str2 == NFmiString("hh"))
				theString += NFmiValueString(GetHour() ,"%02d");
			else if(str2 == NFmiString("mm"))
				theString += NFmiValueString(GetMin() ,"%02d");
			else if(str2 == NFmiString("Ww"))
				theString += Weekday(theLanguage).GetChars(1,2);
			else if(str2 == NFmiString("ww"))
			{
					strh = Weekday(theLanguage).GetChars(1,2);
					strh.LowerCase();
					theString += strh;
			}
			else if(str2 == NFmiString("WW"))
			{
					strh = Weekday(theLanguage).GetChars(1,2);
					strh.UpperCase();
					theString += strh;
			}
			else
			{
//				str1 = theTimeCode.GetChars(ind,1);
				plusInd += -1;
				if(str1 == NFmiString("M"))
					theString += NFmiValueString(GetMonth() ,"%d");
				else if(str1 == NFmiString("D"))
					theString += NFmiValueString(GetDay() ,"%d"); //18.10.00
				else if(str1 == NFmiString("H"))
					theString += NFmiValueString(GetHour() ,"%d"); //29.11.00 bugi korjattu
				else if(str1 == NFmiString("I"))
					theString += NFmiValueString(GetWeekday() ,"%01d");
				else
					theString += str1;
				//pit�isi ottaa ruotsista p�iv�n j�lkeinen piste pois
			}
		}
	  }
	  ind += plusInd;
  }
  return theString;
} 

//__________________________________________________________________ 
static 
char 
    *reldays [] = 
	{"alivuoto","Toissap�iv�n�", "Eilen", "T�n��n", "Huomenna", "Ylihuomenna", "ylivuoto", 

	  "alivuoto","I f�rrg�r", "I g�r", "I dag", "I morgon", "I �vermorgon","ylivuoto", 

	  "alivuoto","The day before yesterday", "Yesterday", "Today", "Tomorrow", "The day after tomorrow","ylivuoto", 

	  "alivuoto","Yesterday", "Yesterday", "Today", "Tomorrow", "The day after tomorrow", "ylivuoto"}; 

//__________________________________________________________________ 
NFmiString NFmiPressTime :: RelativeDay(FmiLanguage theLanguage, NFmiString theStr6, int& thePlusInd) const 
{
	NFmiTime currentTime;
	NFmiString retString;
	thePlusInd = 0;

	//	short diff = DifferenceInDays(currentTime); // =minuutit/24h
	short diff = GetJulianDay() - currentTime.GetJulianDay(); //16.1.01
	if (diff < -100)     //vuodenvaihde ja karkausvuosi hoidettu, voisi vied� NFmiTime:een; ei tastattu
		diff += DaysInYear(currentTime.GetYear());
	if (diff > 100) 
		diff -= DaysInYear(GetYear());

	if(theStr6.GetChars(5,1) == NFmiString("+") || theStr6.GetChars(5,1) == NFmiString("-"))
	{
		int addDiff;
		NFmiValueString vString = NFmiValueString(theStr6);
		vString.ConvertToInt(addDiff, 5, 2);
		diff += -addDiff;
		thePlusInd = 2;
	}
	diff =  FmiMin(FmiMax(diff,-3),3); //pys�ytet��n ali/ylivuotoon
	retString = NFmiString(reldays[(theLanguage-1)*7+diff +3]);
	if (theStr6.GetChars(1,4) == NFmiString("tttt"))  //oletus Tttt
		retString.LowerCase();
	if (theStr6.GetChars(1,4) == NFmiString("TTTT"))
		retString.UpperCase();
	return retString;
} 

