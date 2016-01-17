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

bool NFmiCopyFile(std::ifstream& inFile, std::ofstream& outFile);

bool NFmiCopyFileWithoutShowpage(std::ifstream& inFile, std::ofstream& outFile);
bool NFmiCopyFileCroppingWithoutShowpage(std::ifstream* inFile,
                                         std::ofstream* outFile,
                                         NFmiRect theRect);

bool NFmiCopyFileColoring(std::ifstream& inFile,
                          std::ofstream& outFile,
                          NFmiCmykColorBag* colorBag);

bool NFmiCopyFileCroppingAndColoring(std::ifstream& inFile,
                                     std::ofstream& outFile,
                                     NFmiRect theRect,
                                     NFmiCmykColorBag* colorBag);

bool NFmiWriteBoundingBox(std::ofstream& outFile, NFmiRect theRect);

bool NFmiCopyFileCropping(std::ifstream& inFile, std::ofstream& outFile, NFmiRect theRect);

bool NFmiGenerateAndCopyUniversalSize(std::ifstream& inFile,
                                      std::ofstream& outFile,
                                      const NFmiRect& theRect,
                                      const NFmiString& theName,
                                      const NFmiString& theDate);

bool NFmiWritePSConcat(NFmiRectScale theScale,
                       std::ofstream& outFile,
                       float theRotating = kFloatMissing);
bool NFmiWriteEPSConcat(NFmiRectScale theScale, std::ofstream& outFile);

bool NFmiWriteEPSConcatClipping(NFmiRectScale theScale,
                                std::ofstream& outFile,
                                NFmiRect theClippingRect);

bool NFmiWritePSConcatRotating(NFmiRectScale theScale, float theDirection, std::ofstream& outFile);

bool NFmiWritePSEnd(std::ofstream& outFile);
bool NFmiWriteEPSEnd(std::ofstream& outFile);

#endif  // NFMICOPYFILE_H

// ======================================================================
