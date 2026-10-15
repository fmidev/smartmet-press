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
#include <fstream>
#include <iostream>

bool NFmiCopyFile(std::ifstream& inFile, std::ofstream& outFile);

bool NFmiCopyFileWithoutShowpage(std::ifstream& inFile, std::ofstream& outFile);
bool NFmiCopyFileCroppingWithoutShowpage(std::ifstream* inFile,
                                         std::ofstream* outFile,
                                         const NFmiRect& theRect);

bool NFmiCopyFileColoring(std::ifstream& inFile,
                          std::ofstream& outFile,
                          NFmiCmykColorBag* colorBag);

bool NFmiCopyFileCroppingAndColoring(std::ifstream& inFile,
                                     std::ofstream& outFile,
                                     const NFmiRect& theRect,
                                     NFmiCmykColorBag* colorBag);

bool NFmiWriteBoundingBox(std::ofstream& outFile, const NFmiRect& theRect);

bool NFmiCopyFileCropping(std::ifstream& inFile, std::ofstream& outFile, const NFmiRect& theRect);

bool NFmiGenerateAndCopyUniversalSize(std::ifstream& inFile,
                                      std::ofstream& outFile,
                                      const NFmiRect& theRect,
                                      const NFmiString& theName,
                                      const NFmiString& theDate);

bool NFmiWritePSConcat(const NFmiRectScale& theScale,
                       std::ofstream& outFile,
                       float theRotating = kFloatMissing);
bool NFmiWriteEPSConcat(const NFmiRectScale& theScale, std::ofstream& outFile);

bool NFmiWriteEPSConcatClipping(const NFmiRectScale& theScale,
                                std::ofstream& outFile,
                                const NFmiRect& theClippingRect);

bool NFmiWritePSConcatRotating(const NFmiRectScale& theScale, float theDirection, std::ofstream& outFile);

bool NFmiWritePSEnd(std::ofstream& outFile);
bool NFmiWriteEPSEnd(std::ofstream& outFile);

#endif  // NFMICOPYFILE_H

// ======================================================================
