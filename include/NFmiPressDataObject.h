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
  virtual ~NFmiPressDataObject();
  NFmiPressDataObject();

  NFmiPressDataObject(NFmiFastQueryInfo* theData);
  NFmiPressDataObject(const NFmiPressDataObject& theRect);

  void SetData(NFmiFastQueryInfo* theData);
  NFmiFastQueryInfo* GetData() const;

 protected:
  NFmiFastQueryInfo* itsData;

};  // class NFmiPressDataObject

// ----------------------------------------------------------------------
/*!
 * Destructor does nothing special
 */
// ----------------------------------------------------------------------

inline NFmiPressDataObject::~NFmiPressDataObject() = default;
// ----------------------------------------------------------------------
/*!
 * Void constructor
 */
// ----------------------------------------------------------------------

inline NFmiPressDataObject::NFmiPressDataObject() : itsData(nullptr) {}
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

inline void NFmiPressDataObject::SetData(NFmiFastQueryInfo* theData)
{
  itsData = theData;
}
// ----------------------------------------------------------------------
/*!
 * Undocumented
 *
 * \return Undocumented
 */
// ----------------------------------------------------------------------

inline NFmiFastQueryInfo* NFmiPressDataObject::GetData() const
{
  return itsData;
}
#endif  // NFMIPRESSDATAOBJECT_H

// ======================================================================
