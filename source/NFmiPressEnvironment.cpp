// ======================================================================
/*!
 * \file
 * \brief Implementation of class NFmiPressEnvironment
 */
// ======================================================================

#include "NFmiPressEnvironment.h"
#include "NFmiPressArea.h"

// ----------------------------------------------------------------------
/*!
 * The destructor does nothing special
 */
// ----------------------------------------------------------------------

NFmiPressEnvironment::~NFmiPressEnvironment(void) {}
// ----------------------------------------------------------------------
/*!
 * Copy constructor
 *
 * \param theOtherEnvironment The object being copied
 */
// ----------------------------------------------------------------------

NFmiPressEnvironment::NFmiPressEnvironment(const NFmiPressEnvironment& theOtherEnvironment)
    : itsMaskNumber(theOtherEnvironment.itsMaskNumber),
      itsEnumSpace(theOtherEnvironment.itsEnumSpace),
      fLongNumberMinus(theOtherEnvironment.fLongNumberMinus),
      itsGenericColor(theOtherEnvironment.itsGenericColor),
      itsAdditionalDayAdvance(theOtherEnvironment.itsAdditionalDayAdvance),
      itsTextAttributes(theOtherEnvironment.itsTextAttributes),
      itsVersion(theOtherEnvironment.itsVersion),
      itsSegmentMove(theOtherEnvironment.itsSegmentMove),
      itsSymbolSet(theOtherEnvironment.itsSymbolSet),
      itsSymbolSizeFactor(theOtherEnvironment.itsSymbolSizeFactor),
      fCV(theOtherEnvironment.fCV),
      fAvoidOrphanSyllables(theOtherEnvironment.fAvoidOrphanSyllables),
      fUseBackupPreviousDay(theOtherEnvironment.fUseBackupPreviousDay),
      itsEncoding(theOtherEnvironment.itsEncoding)
{
}

// ======================================================================
