// ======================================================================
/*!
 * \file
 * \brief Interfaces for file copying functions
 *
 * T‰ss‰ on funktioita joita voi k‰ytt‰‰ lehtikarttojen koostamiseen
 * avaukset, sulkemiset ym on hoidettava kutsuvissa ohjelmissa
 */
// ======================================================================

#ifndef NFMICOPYFILE_H
#define NFMICOPYFILE_H

#include "NFmiCmykColorBag.h"
#include "NFmiRectScale.h"
#include <iostream>
#include <fstream>

bool NFmiCopyFile(ifstream & inFile, ofstream & outFile);

bool NFmiCopyFileWithoutShowpage(ifstream & inFile, ofstream & outFile);

bool NFmiCopyFileColoring(ifstream & inFile,
						  ofstream & outFile,
						  NFmiCmykColorBag * colorBag);

bool NFmiCopyFileCroppingAndColoring(ifstream & inFile,
									 ofstream & outFile,
									 NFmiRect theRect,
									 NFmiCmykColorBag * colorBag);

bool NFmiWriteBoundingBox(ofstream & outFile, NFmiRect theRect);

bool NFmiCopyFileCropping(ifstream & inFile,
						  ofstream & outFile,
						  NFmiRect theRect);

bool NFmiWritePSConcat(NFmiRectScale theScale, ofstream & outFile);
bool NFmiWriteEPSConcat(NFmiRectScale theScale, ofstream & outFile);

bool NFmiWriteEPSConcatClipping(NFmiRectScale theScale,
								ofstream & outFile,
								NFmiRect theClippingRect);

bool NFmiWritePSConcatRotating(NFmiRectScale theScale,
							   float theDirection,
							   ofstream & outFile);

bool NFmiWritePSEnd(ofstream & outFile);
bool NFmiWriteEPSEnd(ofstream & outFile);

#endif // NFMICOPYFILE_H

// ======================================================================
