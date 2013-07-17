/*
 * HMolPolUserMainEvent.hh
 *
 *  Created on: Jul 11, 2013
 *      Author: vmgray
 */

//CHANGE ME!!!
//=============================================================================
//
//=============================================================================
//
//   ---------------------------
//  | Doxygen Class Information |
//  ---------------------------
/**
   \class HMolPolEvent

   \brief Defines Top ROOT Tree structure of the ROOT file for each event.

          Primary

 */
//=============================================================================

#ifndef HMOLPOLEVENT_HH_
#define HMOLPOLEVENT_HH_

// root include
#include <TObject.h>

// HMolPol includes
#include "HMolPolPrimaryEvent.hh"

class HMolPolEvent : public TObject
{
  public:

    // Constructor
    HMolPolEvent();
    // Destructor
    virtual ~HMolPolEvent();

    HMolPolPrimaryEvent* fPrimary;
    ///< object containing primary particle info

    const Double_t mm;
    const Double_t MeV;
    const Double_t rad;
    const Double_t deg;

    Int_t fEventNumber; ///< event number

    // define a new Class known to ROOT? Huh?
    ClassDef(HMolPolEvent,1)

  private:

};

#endif /* HMOLPOLEVENT_HH_ */
