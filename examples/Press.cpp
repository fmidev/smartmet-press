// ======================================================================
/*!
 * \file
 * \brief The main program for creating press products
 */
// ======================================================================
/*!
 * \page press Press
 *
 * Undocumented program.
 *
 */
// ======================================================================

#ifdef WIN32
 #pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

// press
#include "NFmiPressManager.h"
// newbase
#include "NFmiCmdLine.h"   
#include "NFmiPreProcessor.h"
#include "NFmiSettings.h"

#include <cstdlib>

using namespace std;

int main(int argc, const char ** argv)
{
  NFmiString dataExt[4] = {"pre", "PRE", "prt", "PRT"};

  NFmiCmdLine cmdLine(argc,argv,"h!o!l!");

  if(cmdLine.Status().IsError())
	{
	  cerr << "Error on the command line:" << endl
		   << " --> " << cmdLine.Status().ErrorLog() << endl;
	  return 1;
	}
  
  if(cmdLine.NumberofParameters() != 1)
	{
      cerr << "Error: Filename argument missing" << endl;
      return 1;
    }

  string homedir("lehtiTuoteDir=");
  if(cmdLine.isOption('h'))
	{
	  string dir = cmdLine.OptionValue('h');
#ifdef UNIX
	  NFmiSettings::Set("press::path",dir);
#else
	  homedir += dir;
	  putenv(homedir.c_str());
#endif	 
	}

  string logdir("lehtiLokiDir=");
  if(cmdLine.isOption('l'))
	{
	  string dir = cmdLine.OptionValue('l');
#ifdef UNIX
	  NFmiSettings::Set("press::logpath",dir);
#else
	  logdir += dir;
	  putenv(logdir.c_str());
#endif	 
	}

  string outdir("lehtiOutDir=");
  if(cmdLine.isOption('o'))
	{
	  string dir = cmdLine.OptionValue('o');
#ifdef UNIX
	  NFmiSettings::Set("press::outpath",dir);
#else
	  outdir += dir;
	  putenv(outdir.c_str());
#endif	 
	}

  
  NFmiFileString descriptionFile(cmdLine.Parameter(1));
  cout << "File: " << cmdLine.Parameter(1) << endl;
  
  bool ok;
  bool managerReadFailed;

  if(descriptionFile.Extension() == dataExt[0] ||
	 descriptionFile.Extension() == dataExt[1] ||
	 descriptionFile.Extension() == dataExt[2] ||
	 descriptionFile.Extension() == dataExt[3] 
	 )
   {
     NFmiPressProduct pressProduct;
      if(pressProduct.ReadDescriptionFile(NFmiString(descriptionFile))) 
  {
	  NFmiPressManager manager;
	  if(manager.ReadDescriptionAndWrite(pressProduct, managerReadFailed))
		  ok = true;
	  else
	  {
		 if(managerReadFailed)
			ok = false;
		 else
			ok = pressProduct.WritePS(); //manager not used
		  }
	  }
	  else
	  {
		  ok = false;
	  }
	  pressProduct.Close();
	  
	  return (ok ? EXIT_SUCCESS : EXIT_FAILURE);
	}
  cout << "ERROR: tuntematon tiedostotyyppi" << endl;
  
  return EXIT_FAILURE;
}

// ======================================================================
