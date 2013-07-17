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
   \class HMolPolMainEvent

   \brief Defines Top ROOT Tree structure of the ROOT file for each event.

          Primary

 */
//=============================================================================

#ifndef HMOLPOLMAINEVENT_HH_
#define HMOLPOLMAINEVENT_HH_

// root include
#include <TObject.h>

// HMolPol includes
//#include "HMolPolUserPrimaryEvent.hh"

class HMolPolMainEvent : public TObject
{
  public:

    //HMolPolPrimaryEvent Primary;  ///< object containing primary particle info

    // Constructor
    HMolPolMainEvent(){};
    // Destructor
    virtual ~HMolPolMainEvent(){};

    // HMolPolPrimaryEvent            Primary;
    ///< object containing primary particle info

    Int_t run_number;   ///< run number
    Int_t event_number; ///< event number


    // define a new Class known to ROOT? Huh?
    ClassDef(HMolPolMainEvent,1)

  private:

};

#endif /* HMOLPOLMAINEVENT_HH_ */
