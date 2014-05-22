/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This file is in charge of defining the unit variables
 *      and passing these to the ROOT tree
 *
 *      All units MUST be internal GEANT4 units or this will not work :-(
 *
 * \date <b>Date:</b> 07-17-2013
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b>
 *
********************************************/

// geant4 includes
#include <G4SystemOfUnits.hh>

//HMolPol includes
#include "HMolPolEventUnits.hh"

// ROOT stuff
ClassImp(HMolPolEventUnits)

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolEventUnits
 *
 * Purpose: to construct the HMolPolEventUnits
 *      - Initializes all the units to their proper value
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
HMolPolEventUnits::HMolPolEventUnits()
: mm(CLHEP::mm),
  cm(CLHEP::cm),
  m(CLHEP::m),
  mm2(CLHEP::mm2),
  cm2(CLHEP::cm2),
  m2(CLHEP::m2),
  mm3(CLHEP::mm3),
  cm3(CLHEP::cm3),
  m3(CLHEP::m3),
  keV(CLHEP::keV),
  MeV(CLHEP::MeV),
  GeV(CLHEP::GeV),
  rad(CLHEP::rad),
  deg(CLHEP::deg),
  sr(CLHEP::sr),
  mbarn(CLHEP::millibarn),
  nsec(CLHEP::ns),
  msec(CLHEP::ms),
  sec(CLHEP::s),
  Tesla(CLHEP::tesla)
{

}
