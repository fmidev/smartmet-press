// ======================================================================
/*!
 * \file
 * \brief Interface of class NFmiPressDataObject
 */
// ======================================================================
/*!
 * \class NFmiPressDataObject
 *
 * Undocumented
 *
 */
// ======================================================================

#ifndef NFMIPRESSDATAOBJECT_H
#define NFMIPRESSDATAOBJECT_H

#include "NFmiDataIdent.h"
#include "NFmiQueryData.h"

class NFmiFastQueryInfo;

//! Undocumented
class NFmiPressDataObject
{
 public:
  virtual ~NFmiPressDataObject(void);
  NFmiPressDataObject(void);

  NFmiPressDataObject(NFmiFastQueryInfo* theData);
  NFmiPressDataObject(const NFmiPressDataObject& theRect);

  void SetData(NFmiFastQueryInfo* theData);
  NFmiFastQueryInfo* GetData(void) const;

 protected:
  NFmiFastQueryInfo* itsData;

};  // class NFmiPressDataObject

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiPressDataObject::~NFmiPressDataObject(void) {}
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressDataObject::NFmiPressDataObject(void) : itsData(0) {}
// ----------------------------------------------------------------------
/*!
 * Constructor based on query info
 *
 * \param theData Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiPressDataObject::NFmiPressDataObject(NFmiFastQueryInfo* theData) : itsData(theData) {}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \param theData Undocumented
 */
// ----------------------------------------------------------------------

inline void NFmiPressDataObject::SetData(NFmiFastQueryInfo* theData) { itsData = theData; }
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiFastQueryInfo* NFmiPressDataObject::GetData(void) const { return itsData; }
#endif  // NFMIPRESSDATAOBJECT_H

// ======================================================================
