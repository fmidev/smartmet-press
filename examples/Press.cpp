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

using namespace std;

int main(int argc, const char ** argv)
{

  NFmiCmdLine cmdLine(argc,argv,"");
  NFmiString dataExt[4] = {"pre", "PRE", "prt", "PRT"};
  
  if(argc<=1)
    {
      cerr << "Error: Filename argument missing" << endl;
      return 1;
    }
  
  NFmiFileString descriptionFile(argv[1]);
  cout << "File: " << argv[1] << endl;
  
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
