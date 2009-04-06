// ======================================================================
/*
 * \file
 * \brief Implementation of class NFmiValueCorrection
 */
// ======================================================================

#include "NFmiValueCorrection.h"
#include <fstream>

using namespace std;

// ----------------------------------------------------------------------
/*!
 */
// ----------------------------------------------------------------------

void NFmiValueCorrection::SetFiles(const string & theFile, ofstream* theLogFile)
{
  itsFile = theFile;
  itsLogFile = theLogFile;
}
// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiValueCorrection::~NFmiValueCorrection(void)
{
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiValueCorrection::ReadFile(void)
{  
  fRead = true;
  //fValue = false;
  string str1, str2, str3;
  
  //string includePath(itsFile);
  //ifstream in(includePath.c_str(),ios::in|ios::binary);
  ifstream in(itsFile.c_str(),ios::in|ios::binary);

  if(!in)
	{
		fMissing = true;
		*itsLogFile << "*** WARNING: ei Lampokorjaus-tiedostoa" << endl;
		return false;
	}

  string::size_type start1, end1, start2, end2, start3, end3;
  const string delims(" \t");
  string line;
  while(getline(in,line))
	{
	  // Ignore the line if it is a comment line or too short

	  line += " "; //rivin lopetusmerkki
	  if(line.substr(0,3)=="END" || line.substr(0,5)=="LOPPU")
		break;

	  if(line[0]=='#' || line.substr(0,2)=="//" || line.size() < 6)
		continue;

	  start1 = line.find_first_not_of(delims);
	  end1 = line.find_first_of(delims, start1);
	  start2 = line.find_first_not_of(delims, end1);
	  end2 = line.find_first_of(delims, start2);
	  start3 = line.find_first_not_of(delims, end2);
	  end3 = line.find_first_of(delims, start3); 
	  //end3 = line.find_last_not_of(delims, start3); //HUOM

	  if(end3 == string::npos)
		{
		  *itsLogFile << "***ERROR: invalid line in Lampokorjaus: "
					  << line
					  << endl;
		  continue;
		}
	  else
		{
		  FmiTempCorrections tempCorrection;
		  str1 = line.substr(start1, end1-start1);
		  str2 = line.substr(start2, end2-start2);
		  str3 = line.substr(start3, end3-start3);
		  tempCorrection.stationName = str1;
          tempCorrection.dayCorr   = static_cast<float>(atof(str2.c_str()));
          tempCorrection.nightCorr = static_cast<float>(atof(str3.c_str()));
		  itsTempCorrections.push_back(tempCorrection);
		}
	}
	*itsLogFile << "   Lampokorjaus-tiedosto luettu" << endl;
	in.close();
	in.clear();
	return true;
}

// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

bool NFmiValueCorrection::CorrectValue(float& thevalue, const NFmiString& theStation,
								bool dayTime)
{
	if(!IsRead())
		ReadFile();

	if(IsMissing())
		return false;

	std::vector<FmiTempCorrections>::iterator pos;

	for(pos=itsTempCorrections.begin(); pos != itsTempCorrections.end(); ++pos)
	{
		if((*pos).stationName == theStation)
		{
			if(dayTime)
				thevalue += (*pos).dayCorr;
			else
				thevalue += (*pos).nightCorr;

			return true;
		}
	}
	return false;
}

// ======================================================================
