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

    const Double_t mm;        /// \ingroup root Units of mm
    const Double_t cm;        /// \ingroup root Units of cm
    const Double_t m;         /// \ingroup root Units of m
    const Double_t mm2;       /// \ingroup root Units of mm^2
    const Double_t cm2;       /// \ingroup root Units of cm^2
    const Double_t m2;        /// \ingroup root Units of m^2
    const Double_t mm3;       /// \ingroup root Units of mm^3
    const Double_t cm3;       /// \ingroup root Units of cm^3
    const Double_t m3;        /// \ingroup root Units of m^3
    const Double_t keV;       /// \ingroup root Units of keV
    const Double_t MeV;       /// \ingroup root Units of MeV
    const Double_t GeV;       /// \ingroup root Units of GeV
    const Double_t rad;       /// \ingroup root Units of rad
    const Double_t deg;       /// \ingroup root Units of deg

    Int_t fEventNumber; ///< event number

    // define a new Class known to ROOT? Huh?
    ClassDef(HMolPolEvent,1)

  private:

};

#endif /* HMOLPOLEVENT_HH_ */
